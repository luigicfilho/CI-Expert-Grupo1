// tb_mux4.sv
// Testbench do modulo mux4 (multiplexador 4:1).

// teste exaustivo. Com WIDTH = 2 ha 4 entradas de 2 bits mais o
// seletor de 2 bits (10 bits no total), ou seja, 1024 combinacoes, todas testadas.
// Para cada combinacao, a saida do DUT eh comparada com um modelo de referencia
// (funcao "esperado"), escrito a partir da especificacao e sem reaproveitar o
// codigo do mux.

// testar com:
//   iverilog -g2012 -o sim_mux4.out tb_mux4.sv ../../../stubs/mux4.v
//   vvp sim_mux4.out
// Resultado: "RESULTADO: PASSOU" se todas as combinacoes conferem

`timescale 1ns/1ps

module tb_mux4;

    localparam WIDTH = 2;   //teste exaustivo

    // Sinais ligados ao DUT
    reg  [WIDTH-1:0] din_1, din_2, din_3, din_4;   // entradas de dados
    reg  [1:0]       in_select;                    // seletor da entrada
    wire [WIDTH-1:0] dout;                         // saida DUT

    integer erros  = 0;
    integer testes = 0;

    //dut
    mux4 #(.WIDTH(WIDTH)) dut (
        .din_1     (din_1),
        .din_2     (din_2),
        .din_3     (din_3),
        .din_4     (din_4),
        .in_select (in_select),
        .dout      (dout)
    );

    //devolve a entrada que o seletor deve escoljer
    function [WIDTH-1:0] esperado(
        input [1:0]       sel,
        input [WIDTH-1:0] a,
        input [WIDTH-1:0] b,
        input [WIDTH-1:0] c,
        input [WIDTH-1:0] d
    );
        begin
            if (sel == 2'b00)      esperado = a;
            else if (sel == 2'b01) esperado = b;
            else if (sel == 2'b10) esperado = c;
            else                   esperado = d;
        end
    endfunction

    //confere a saida atual do DUT com o valor esperado e conta os erros.
    //!== tambem detecta X e Z.
    task checa;
        reg [WIDTH-1:0] exp;
        begin
            #1;   // aguarda a saida do DUT estabilizar
            exp    = esperado(in_select, din_1, din_2, din_3, din_4);
            testes = testes + 1;
            if (dout !== exp) begin
                erros = erros + 1;
                $display("ERRO: sel=%b | esperado=%0d | obtido=%0d", in_select, exp, dout);
            end
        end
    endtask

    //estimulo: percorre todas as combinacoes de seletor e entradas
    integer s, a, b, c, d;

    initial begin
        din_1 = 0; din_2 = 0; din_3 = 0; din_4 = 0; in_select = 0;

        for (s = 0; s < 4; s = s + 1)
            for (a = 0; a < 2**WIDTH; a = a + 1)
                for (b = 0; b < 2**WIDTH; b = b + 1)
                    for (c = 0; c < 2**WIDTH; c = c + 1)
                        for (d = 0; d < 2**WIDTH; d = d + 1) begin
                            in_select = s;
                            din_1 = a;
                            din_2 = b;
                            din_3 = c;
                            din_4 = d;
                            checa;
                        end

        $display("FIM: %0d testes, %0d erros", testes, erros);
        if (erros == 0 && testes > 0) $display("RESULTADO: PASSOU");
        else                          $display("RESULTADO: FALHOU");
        $finish;
    end

    // Formas de onda
    initial begin
        $dumpfile("wave_mux4.vcd");
        $dumpvars(0, tb_mux4);
    end

endmodule