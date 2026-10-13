# Discussão Técnica de Arquitetura: Comportamento da Flag `zero` e da ALU

**Data:** 13 de Outubro de 2026  
**Contexto:** Alinhamento entre RTL, UVM (Predictor/Coverage) e Golden Model (Simulador C)  
**Documento de Referência:** *Roteiro CI Expert — Projeto e Implementação de Processador Multiciclo*  

---

## 1. Introdução e Contexto

Durante o desenvolvimento e validação cruzada do **Golden Model em C** (simulador) e dos **programas de teste em Assembly** (programas em `tools/programs/`), surgiu uma ambiguidade na especificação do Roteiro que afeta diretamente:
- O módulo sintetizável `alu.v`;
- O módulo Top-Level `top.v`;
- O módulo UVM Predictor / Golden Model;
- O plano de cobertura funcional cruzada (`Cross Coverage: cmd_in x zero`).

---

## 2. O que diz a Especificação Oficial (Roteiro)

### 2.1 Unidade Lógica e Aritmética (ALU) — Roteiro §2.1, Item 3
> *"3. **Unidade Lógica e Aritmética (ALU)**: Executa operações aritméticas e lógicas. A ALU aceita dois operandos de largura WIDTH e produz um resultado de 2 x WIDTH bits. **Emite a flag zero (quando o resultado é 0)** e a flag error (em caso de divisão por zero ou quando dados de realimentação forem inválidos). Sob condição de erro, a saída da ALU é obrigatoriamente forçada para -1 (todos os bits em '1')."*

### 2.2 Conjunto de Instruções (ISA) — Roteiro §2.2
O opcode possui 3 bits (`cmd_in[2:0]`):
- `3'b000` (**ADD**): Soma ($A + B$) — ALU
- `3'b001` (**SUB**): Subtração ($A - B$) — ALU
- `3'b010` (**MUL**): Multiplicação ($A \times B$) — ALU
- `3'b011` (**DIV**): Divisão ($A / B$) — ALU
- `3'b100` (**NOP**): Nenhuma Operação
- `3'b101` (**LOAD**): Leitura da Memória Interna — Memória
- `3'b110` (**STORE**): Escrita na Memória Interna — Memória
- `3'b111` (**NOP**): Nenhuma Operação

### 2.3 Datapath e Registrador de Flags — Roteiro §2.3 e `src/stubs/top.v`
No RTL (`top.v`):
```verilog
// Mux selmux2 seleciona entre dado da memória (LOAD) ou resultado da ALU
assign aluout_sel = selmux2 ? mem_data_out : alu_result;

// Registrador de saída armazena aluout_sel (pode ser memória ou ALU)
register_bank #( .WIDTH(2*WIDTH) ) u_reg_out (
    .clk(clk), .rst_n(rst_n), .en(aluout_reg_en),
    .d(aluout_sel), .q(reg_out_q)
);

// Registrador de flags armazena DIRETO as saídas combinacionais da ALU:
register_bank #( .WIDTH(2) ) u_reg_flags (
    .clk(clk), .rst_n(rst_n), .en(aluout_reg_en),
    .d({alu_error, alu_zero}), .q(flags_q)
);
assign error = flags_q[1];
assign zero  = flags_q[0];
```

---

## 3. O Problema / Ambiguidade Identificada

No 3º ciclo da FSM (`STORE`), o sinal `aluout_reg_en` é ativado para registrar a saída e atualizar as flags:
1. Quando a instrução é `LOAD` (`opcode = 3'b101`):
   - O registrador de dados `reg_out` recebe o dado vindo da memória (`mem_data_out`, ex.: `0x0058`).
   - Porém, o registrador `u_reg_flags` recebe `{alu_error, alu_zero}` gerados pela **ALU**.
2. **Pergunta:** O que a ALU deve emitir em `alu_zero` quando o `opcode` NÃO é uma operação aritmética (`LOAD`, `STORE`, `NOP`)?

Atualmente, no código original de `alu.c`:
```c
switch (opcode) {
    case OP_ADD: resultado = (uint16_t)((uint32_t)a + (uint32_t)b); break;
    case OP_SUB: resultado = (uint16_t)((uint32_t)a - (uint32_t)b); break;
    case OP_MUL: resultado = (uint16_t)((uint32_t)a * (uint32_t)b); break;
    case OP_DIV: ... break;
    default:     resultado = 0; break;
}
*zero = (resultado == 0); // Avaliado fora do switch!
```
Como o `default` atribui `resultado = 0`, a expressão `(resultado == 0)` resulta em **`1`** (`true`).

Isso produz a seguinte situação em simulação:
- Executa-se `LOAD Mem[7]`, que contém o valor `0x0058` (88 decimal).
- A saída do processador `dout` é `0x0058` (valor diferente de zero).
- Mas a flag externa `zero` emitida pelo processador vai para **`1`**, porque a ALU combinacional interna foi para default (`0`).

---

## 4. As Alternativas para Decisão do Grupo

### Opção A: Manter como está (Comportamento Combinacional Estrito da ALU)
- **Definição:** A ALU simplesmente coloca `0` na saída para opcodes não suportados por ela (`LOAD`, `STORE`, `NOP`), e seu comparador combinacional `(dout == 0)` aciona `zero = 1`.
- **Impacto no RTL (`alu.v`):**
  ```verilog
  assign zero = (dout == {2*WIDTH{1'b0}});
  ```
- **Consequência para UVM e Testes:**
  - Em qualquer `LOAD`, `STORE` e `NOP`, a flag `zero` será sempre `1`.
  - Nos testes em Assembly (`.expect <dout>, <err>, <zero>`), todas as checagens após `load`, `store` ou `nop` devem esperar obrigatoriamente `zero = 1`, mesmo que o dado carregado da memória seja diferente de zero.
  - No Scoreboard UVM, deve-se predizer `zero = 1` para opcodes `3'b100`, `3'b101`, `3'b110`, `3'b111`.

### Opção B: Flag `zero` válida apenas para Instruções da ALU
- **Definição:** A flag `zero` da ALU só é assertada para as operações aritméticas (`ADD`, `SUB`, `MUL`, `DIV`) cujo resultado efetivamente foi zero. Para `NOP`, `LOAD` e `STORE`, a flag `zero` fica em `0` (ou mantém o valor anterior / é desabilitada).
- **Impacto no RTL (`alu.v`):**
  ```verilog
  assign zero = (alu_op <= 3'b011) && (dout == {2*WIDTH{1'b0}});
  ```
- **Consequência para UVM e Testes:**
  - `zero = 1` só ocorre quando uma conta deu zero (ex.: `sub 5, 5` ou `mul 0, x`).
  - `LOAD`, `STORE` e `NOP` nunca ativam a flag `zero`.

### Opção C: Flag `zero` reflete a saída final do processador (`reg_out`)
- **Definição:** Mudar a conexão no Top-Level (`top.v`) para que o registrador de flags compare o dado final selecionado por `selmux2` (ou seja, se o dado lido da memória for zero, `zero = 1`; se for não-nulo, `zero = 0`).
- **Impacto no RTL (`top.v`):**
  - O sinal conectado ao `u_reg_flags` passa a ser `(aluout_sel == 0)` em vez do `alu_zero` isolado da ALU.
- **Consequência:**
  - Mais intuitivo para quem programa em Assembly (a flag `zero` indica se o registrador `reg_out` contém zero ou não após qualquer instrução).
  - Requer ajuste na conexão do `top.v`.

---

## 5. Próximos Passos Sugeridos para a Reunião do Grupo
1. Discutir entre os integrantes de **RTL**, **DV (Verificação UVM)** e **DFT** qual das opções será adotada como padrão oficial do projeto.
2. Uma vez definida a opção (A, B ou C), alinhar:
   - A codificação em `src/stubs/alu.v` / `top.v`;
   - O Golden Model em C (`tools/simulator/core/`);
   - Os arquivos de teste Assembly em `tools/programs/`.
