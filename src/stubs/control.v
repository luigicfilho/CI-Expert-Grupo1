// control.v — Unidade de Controle, FSM de 4 estados 

module control (
    input  wire       clk,
    input  wire       rst_n,
    input  wire [6:0] cmd_in,
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


endmodule
