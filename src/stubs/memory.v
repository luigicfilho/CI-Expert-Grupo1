// memory.v — Memória interna 

module memory #(
    parameter WIDTH = 8,
    parameter DEPTH = 8
) (
    input  wire                    clk,
    input  wire                    rst_n,
    input  wire [$clog2(DEPTH)-1:0] address,
    input  wire [2*WIDTH-1:0]      data_in,
    input  wire                    write,
    input  wire                    read,
    output wire [2*WIDTH-1:0]      data_out
);

endmodule
