// top.v — Top-Level do processador multiciclo 

module top #(
    parameter WIDTH = 8
) (
    input  wire             clk,
    input  wire             rst_n,
    input  wire [WIDTH-1:0] din_1,
    input  wire [WIDTH-1:0] din_2,
    input  wire [WIDTH-1:0] din_3,
    input  wire [6:0]       cmd_in,
    output wire             zero,
    output wire             error, 
    output wire [WIDTH-1:0] dout_high,
    output wire [WIDTH-1:0] dout_low,
    output wire             cpu_rdy
);

wire [WIDTH-1:0]     op_a;            // regA -> ALU.a e memory.address
wire [WIDTH-1:0]     op_b;            // regB -> ALU.b
wire [2*WIDTH-1:0]   alu_result;      // ALU.dout
wire                 alu_zero;        // flag zero combinacional da ALU
wire                 alu_error;       // flag error combinacional da ALU
wire [2*WIDTH-1:0]   mem_data_out;    // memory.data_out
wire [2*WIDTH-1:0]   aluout_sel;      // saída do mux selmux2 -> reg_out.d
wire [2*WIDTH-1:0]   reg_out_q;       // reg_out.q = {dout_high, dout_low}
wire [6:0]           reg_cmd_q;       // reg_cmd.q -> control.cmd_in
wire [1:0]           flags_q;         // reg_flags.q = {error, zero}

// Controle
wire [1:0]           in_select_a;
wire [1:0]           in_select_b;
wire                 invalid_data;    // nvalid_data
wire [2:0]           alu_op;          // opcode
wire                 aluin_reg_en;
wire                 datain_reg_en;
wire                 aluout_reg_en;
wire                 memory_write;
wire                 memory_read;
wire                 selmux2;

// -------------------------------------------------------------------------
// Mux A + regA e Mux B + regB (mux4_registered, roteiro §2.1 item 1)
// Fonte 11: realimentação dout_high (A) / dout_low (B)
// -------------------------------------------------------------------------
mux4_registered #(
    .WIDTH (WIDTH)
) u_mux_a (
    .din_1    (din_1),
    .din_2    (din_2),
    .din_3    (din_3),
    .din_4    (dout_high),          // realimentação parte alta
    .in_select(in_select_a),
    .clk      (clk),
    .rst_n    (rst_n),
    .en       (aluin_reg_en),
    .dout     (op_a)
);

mux4_registered #(
    .WIDTH (WIDTH)
) u_mux_b (
    .din_1    (din_1),
    .din_2    (din_2),
    .din_3    (din_3),
    .din_4    (dout_low),           // realimentação parte baixa
    .in_select(in_select_b),
    .clk      (clk),
    .rst_n    (rst_n),
    .en       (aluin_reg_en),
    .dout     (op_b)
);

alu #(
    .WIDTH (WIDTH)
) u_alu (
    .a           (op_a),
    .b           (op_b),
    .invalid_data(invalid_data),
    .alu_op      (alu_op),
    .zero        (alu_zero),
    .error       (alu_error),
    .dout        (alu_result)
);

localparam ADDR_W = 3;

memory #(
    .WIDTH (WIDTH)
) u_mem (
    .clk     (clk),
    .rst_n   (rst_n),
    .address (op_a[ADDR_W-1:0]),
    .data_in (reg_out_q),
    .write   (memory_write),
    .read    (memory_read),
    .data_out(mem_data_out)
);

assign aluout_sel = selmux2 ? mem_data_out : alu_result;

register_bank #(
    .WIDTH (2*WIDTH)
) u_reg_out (
    .clk (clk),
    .rst_n(rst_n),
    .en  (aluout_reg_en),
    .d   (aluout_sel),
    .q   (reg_out_q)
);

assign {dout_high, dout_low} = reg_out_q;

register_bank #(
    .WIDTH (7)
) u_reg_cmd (
    .clk (clk),
    .rst_n(rst_n),
    .en  (datain_reg_en),
    .d   (cmd_in),
    .q   (reg_cmd_q)
);

register_bank #(
    .WIDTH (2)
) u_reg_flags (
    .clk (clk),
    .rst_n(rst_n),
    .en  (aluout_reg_en),
    .d   ({alu_error, alu_zero}),
    .q   (flags_q)
);

assign error = flags_q[1];
assign zero  = flags_q[0];

control u_ctrl (
    .clk          (clk),
    .rst_n        (rst_n),
    .cmd_in       (reg_cmd_q),
    .p_error      (flags_q[1]),
    .in_select_a  (in_select_a),
    .in_select_b  (in_select_b),
    .aluin_reg_en (aluin_reg_en),
    .datain_reg_en(datain_reg_en),
    .aluout_reg_en(aluout_reg_en),
    .invalid_data (invalid_data),
    .alu_op       (alu_op),
    .memory_write (memory_write),
    .memory_read  (memory_read),
    .selmux2      (selmux2),
    .cpu_rdy      (cpu_rdy)
);

endmodule
