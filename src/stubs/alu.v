// alu.v — Unidade Lógica e Aritmética

module alu #(
    parameter WIDTH = 8
) (
    input  wire [WIDTH-1:0]     a,
    input  wire [WIDTH-1:0]     b,
    input  wire                 invalid_data,
    input  wire [2:0]           alu_op,
    output wire                 zero,
    output wire                 error,
    output wire [2*WIDTH-1:0]   dout
);

endmodule
