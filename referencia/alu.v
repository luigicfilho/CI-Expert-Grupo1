// alu.v — Unidade Lógica e Aritmética (combinacional)
//
// opcode: 000 ADD | 001 SUB | 010 MUL | 011 DIV | 100/111 NOP | 101 LOAD | 110 STORE
// LOAD/STORE/NOP não produzem resultado na ALU (dout = 0, zero = 1).
// Erro (invalid_data ou DIV por zero): error = 1, zero = 0, dout = todos os bits em '1' (-1).

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

localparam [2:0] OP_ADD = 3'b000;
localparam [2:0] OP_SUB = 3'b001;
localparam [2:0] OP_MUL = 3'b010;
localparam [2:0] OP_DIV = 3'b011;

wire [2*WIDTH-1:0] a_ext = {{WIDTH{1'b0}}, a};
wire [2*WIDTH-1:0] b_ext = {{WIDTH{1'b0}}, b};

wire div_by_zero = (alu_op == OP_DIV) && (b == {WIDTH{1'b0}});

assign error = invalid_data | div_by_zero;

reg [2*WIDTH-1:0] result;

always @(*) begin
    case (alu_op)
        OP_ADD:  result = a_ext + b_ext;
        OP_SUB:  result = a_ext - b_ext;
        OP_MUL:  result = a_ext * b_ext;
        OP_DIV:  result = (b == {WIDTH{1'b0}}) ? {(2*WIDTH){1'b0}} : (a_ext / b_ext);
        default: result = {(2*WIDTH){1'b0}};
    endcase
end

assign dout = error ? {(2*WIDTH){1'b1}} : result;
assign zero = ~error & (result == {(2*WIDTH){1'b0}});

endmodule
