// mux4_registered.v — Mux 4:1 com saída registrada 

module mux4_registered #(
    parameter WIDTH = 8
) (
    input  wire [WIDTH-1:0] din_1,
    input  wire [WIDTH-1:0] din_2,
    input  wire [WIDTH-1:0] din_3,
    input  wire [WIDTH-1:0] din_4,
    input  wire [1:0]       in_select,
    input  wire             clk,
    input  wire             rst_n,
    input  wire             en,
    output wire [WIDTH-1:0] dout
);

wire [WIDTH-1:0] mux_out;

mux4 #(
    .WIDTH (WIDTH)
) u_mux4 (
    .din_1(din_1),
    .din_2(din_2),
    .din_3(din_3),
    .din_4(din_4),
    .in_select(in_select),
    .dout(mux_out)
);

register_bank #(
    .WIDTH(WIDTH)
) u_reg (
    .clk (clk),
    .rst_n(rst_n),
    .en(en),
    .d(mux_out),
    .q(dout)
);

endmodule
