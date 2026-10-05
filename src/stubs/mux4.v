// mux4.v — Multiplexador combinacional 4:1 

module mux4 #(
    parameter WIDTH = 8
) (
    input  wire [WIDTH-1:0] din_1,
    input  wire [WIDTH-1:0] din_2,
    input  wire [WIDTH-1:0] din_3,
    input  wire [WIDTH-1:0] din_4,
    input  wire [1:0]       in_select,
    output wire [WIDTH-1:0] dout
);

endmodule
