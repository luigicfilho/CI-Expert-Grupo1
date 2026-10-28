// register_bank.v — Registrador genérico: clock na borda de subida,
// reset assíncrono ativo em baixo (rst_n) e habilitação de escrita (en).

module register_bank #(
    parameter WIDTH = 8
) (
    input  wire             clk,
    input  wire             rst_n,
    input  wire             en,
    input  wire [WIDTH-1:0] d,
    output reg  [WIDTH-1:0] q
);

always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q <= {WIDTH{1'b0}};
    else if (en)
        q <= d;
end

endmodule
