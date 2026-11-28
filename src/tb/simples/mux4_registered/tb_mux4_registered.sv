`timescale 1ns/1ps

module tb_mux4_registered;

    localparam WIDTH = 8;

    // Sinais ligados ao DUT
    reg              clk   = 1'b0;     // clock gerado abaixo
    reg              rst_n = 1'b1;     // reset assincrono, ativo em 0
    reg              en;               // habilita a captura
    reg  [WIDTH-1:0] din_1, din_2, din_3, din_4;   // entradas de dados
    reg  [1:0]       in_select;                    // seletor da entrada
    wire [WIDTH-1:0] dout;                         // saida registrada

    integer erros  = 0;    // verificacoes em q a saida diferiu do esperado
    integer testes = 0;    // verificacoes feitas

    mux4_registered #(.WIDTH(WIDTH)) dut (
        .din_1     (din_1),
        .din_2     (din_2),
        .din_3     (din_3),
        .din_4     (din_4),
        .in_select (in_select),
        .clk       (clk),
        .rst_n     (rst_n),
        .en        (en),
        .dout      (dout)
    );

    // clock 30ns
    always #15 clk = ~clk;

    task confere(input [WIDTH-1:0] exp);
        begin
            testes = testes + 1;
            if (dout !== exp) begin
                erros = erros + 1;
                $display("ERRO t=%0t | sel=%b en=%b rst_n=%b | esperado=%h | obtido=%h",
                         $time, in_select, en, rst_n, exp, dout);
            end
        end
    endtask

    reg [WIDTH-1:0] padroes [0:3];
    reg [WIDTH-1:0] anterior;      // valor que a saida deve estar guardando
    integer s, i;

    initial begin
        padroes[0] = 8'hFF;  padroes[1] = 8'h00;
        padroes[2] = 8'hAA;  padroes[3] = 8'h55;
        en = 1;  in_select = 0;
        din_1 = 0;  din_2 = 0;  din_3 = 0;  din_4 = 0;

        #2 rst_n = 0;
        #1 confere(0);
        @(negedge clk) rst_n = 1;
        anterior = 0;

        // 2, 3 e 4 p cada seletor e cada padrao
        for (s = 0; s < 4; s = s + 1)
            for (i = 0; i < 4; i = i + 1) begin
                @(negedge clk);
                en        = 1;
                in_select = s;
                din_1 = ~padroes[i];
                din_2 = ~padroes[i];
                din_3 = ~padroes[i];
                din_4 = ~padroes[i];
                case (s)
                    0: din_1 = padroes[i];
                    1: din_2 = padroes[i];
                    2: din_3 = padroes[i];
                    3: din_4 = padroes[i];
                endcase

                #1 confere(anterior);

                @(posedge clk);
                #1 confere(padroes[i]);
                anterior = padroes[i];

                @(negedge clk);
                en        = 0;
                din_1 = ~padroes[i];
                din_2 = ~padroes[i];
                din_3 = ~padroes[i];
                din_4 = ~padroes[i];
                @(posedge clk);
                #1 confere(padroes[i]);
            end

        @(negedge clk);
        #3 rst_n = 0;
        #1 confere(0);
        @(negedge clk) rst_n = 1;

        $display("FIM: %0d verificacoes, %0d erros", testes, erros);
        if (erros == 0 && testes > 0) $display("RESULTADO: PASSOU");
        else                          $display("RESULTADO: FALHOU");
        $finish;
    end

    //formas d onda
    initial begin
        $dumpfile("wave_mux4_registered.vcd");
        $dumpvars(0, tb_mux4_registered);
    end

endmodule