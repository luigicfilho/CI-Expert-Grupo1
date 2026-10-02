# Projeto Final CI Expert: Processador Multiciclo
# Unidade de tempo: ns
# Unidade de capacitância: pF
set_units -time ns -capacitance pF

# Clock period 30 ns
create_clock -name clk -period 30.0 [get_ports clk]

# Clock setup uncertainty 10%
set_clock_uncertainty -setup 3.0 [get_clocks clk]

# Clock transition 10%
set_clock_transition 3.0 [get_clocks clk]

# Clock source latency 5%
set_clock_latency -source 1.5 [get_clocks clk]

# Clock network latency 3%
set_clock_latency 0.9 [get_clocks clk]

# Input delay 40%
set ports_in [remove_from_collection [all_inputs] [get_ports clk]]
set_input_delay -clock clk 12.0 $ports_in

# Output delay 50%
set_output_delay -clock clk 15.0 [all_outputs]

# Output load 0.04 pF
set_load 0.04 [all_outputs]

# Input min transition 1%
set_input_transition -min 0.3 $ports_in

# Input max transition 10%
set_input_transition -max 3.0 $ports_in

