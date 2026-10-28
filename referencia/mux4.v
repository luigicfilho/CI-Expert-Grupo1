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

// 00: din_1 | 01: din_2 | 10: din_3 | 11: din_4 (realimentação)
assign dout = (in_select == 2'b00) ? din_1 :
              (in_select == 2'b01) ? din_2 :
              (in_select == 2'b10) ? din_3 :
                                     din_4;

endmodule
