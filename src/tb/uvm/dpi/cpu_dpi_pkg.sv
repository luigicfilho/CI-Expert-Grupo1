// cpu_dpi_pkg.sv — Pacote SystemVerilog com declaracoes DPI-C para o UVM

package cpu_dpi_pkg;

    // Reinicializa a CPU de referencia
    import "DPI-C" function void dpi_cpu_reset();

    // Executa uma instrucao completa (3 ciclos) e retorna as saidas
    import "DPI-C" function void dpi_cpu_predict_instruction(
        input  byte unsigned        cmd,
        input  byte unsigned        din1,
        input  byte unsigned        din2,
        input  byte unsigned        din3,
        output shortint unsigned    out_dout,
        output byte unsigned        out_zero,
        output byte unsigned        out_error
    );

    // Inspeciona os registradores internos da CPU 
    import "DPI-C" function void dpi_cpu_get_registers(
        output byte unsigned        reg_a,
        output byte unsigned        reg_b,
        output byte unsigned        reg_cmd,
        output shortint unsigned    reg_out
    );

    // Le uma posicao da memoria interna
    import "DPI-C" function shortint unsigned dpi_cpu_read_memory(
        input byte unsigned address
    );

endpackage : cpu_dpi_pkg
