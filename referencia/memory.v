// memory.v — Memória interna: DEPTH palavras de 2*WIDTH bits.
// Escrita síncrona (write), leitura assíncrona habilitada por read
// (data_out = 0 quando read = 0). Reset assíncrono zera todo o conteúdo.

module memory #(
    parameter WIDTH = 8,
    parameter DEPTH = 8
) (
    input  wire                     clk,
    input  wire                     rst_n,
    input  wire [$clog2(DEPTH)-1:0] address,
    input  wire [2*WIDTH-1:0]       data_in,
    input  wire                     write,
    input  wire                     read,
    output wire [2*WIDTH-1:0]       data_out
);

reg [2*WIDTH-1:0] mem [0:DEPTH-1];
integer i;

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        for (i = 0; i < DEPTH; i = i + 1)
            mem[i] <= {(2*WIDTH){1'b0}};
    end else if (write) begin
        mem[address] <= data_in;
    end
end

assign data_out = read ? mem[address] : {(2*WIDTH){1'b0}};

endmodule
