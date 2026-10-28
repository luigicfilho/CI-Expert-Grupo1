// control.v — Unidade de Controle, FSM de 4 estados (RST, FETCH, EXEC, STORE)
// FSM de 2 blocos always: (1) sequencial para o estado, (2) combinacional para
// próximo estado e saídas.
//
// cmd_in  : palavra de comando externa (amostrada no FETCH; define in_select_a/b)
// reg_cmd : comando já registrado em reg_cmd (usado em EXEC/STORE; define alu_op)
// p_error : flag error do resultado anterior (reg_flags)

module control (
    input  wire       clk,
    input  wire       rst_n,
    input  wire [6:0] cmd_in,
    input  wire [6:0] reg_cmd,
    input  wire       p_error,
    output reg  [1:0] in_select_a,
    output reg  [1:0] in_select_b,
    output reg        aluin_reg_en,
    output reg        datain_reg_en,
    output reg        aluout_reg_en,
    output reg        invalid_data,
    output reg  [2:0] alu_op,
    output reg        memory_write,
    output reg        memory_read,
    output reg        selmux2,
    output reg        cpu_rdy
);

localparam [1:0] RST    = 2'b00;
localparam [1:0] FETCH  = 2'b01;
localparam [1:0] EXEC   = 2'b10;
localparam [1:0] STORE  = 2'b11;

localparam [2:0] OP_LOAD  = 3'b101;
localparam [2:0] OP_STORE = 3'b110;

reg [1:0] state, next_state;

wire [2:0] opcode = reg_cmd[2:0];

// Operando usa realimentação (dout_high / dout_low) se seletor == 2'b11
wire fb_fetch = (cmd_in[6:5]  == 2'b11) | (cmd_in[4:3]  == 2'b11);
wire fb_hold  = (reg_cmd[6:5] == 2'b11) | (reg_cmd[4:3] == 2'b11);

// ---------------------------------------------------------------------------
// Bloco 1: registrador de estado (reset assíncrono)
// ---------------------------------------------------------------------------
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        state <= RST;
    else
        state <= next_state;
end

// ---------------------------------------------------------------------------
// Bloco 2: próximo estado + saídas (combinacional)
// ---------------------------------------------------------------------------
always @(*) begin
    next_state    = RST;
    in_select_a   = 2'b00;
    in_select_b   = 2'b00;
    aluin_reg_en  = 1'b0;
    datain_reg_en = 1'b0;
    aluout_reg_en = 1'b0;
    invalid_data  = 1'b0;
    alu_op        = 3'b000;
    memory_write  = 1'b0;
    memory_read   = 1'b0;
    selmux2       = 1'b0;
    cpu_rdy       = 1'b0;

    case (state)
        RST: begin
            next_state = FETCH;
        end

        FETCH: begin
            next_state    = EXEC;
            cpu_rdy       = 1'b1;
            aluin_reg_en  = 1'b1;
            datain_reg_en = 1'b1;
            in_select_a   = cmd_in[6:5];
            in_select_b   = cmd_in[4:3];
            invalid_data  = fb_fetch & p_error;
        end

        EXEC: begin
            next_state   = STORE;
            alu_op       = opcode;
            invalid_data = fb_hold & p_error;
            if (opcode == OP_LOAD) begin
                memory_read = 1'b1;
                selmux2     = 1'b1;
            end
        end

        STORE: begin
            next_state    = FETCH;
            alu_op        = opcode;
            invalid_data  = fb_hold & p_error;
            aluout_reg_en = 1'b1;
            if (opcode == OP_LOAD) begin
                memory_read = 1'b1;   // mantém leitura até capturar em reg_out
                selmux2     = 1'b1;
            end
            if (opcode == OP_STORE)
                memory_write = 1'b1;
        end

        default: begin
            next_state = RST;
        end
    endcase
end

endmodule
