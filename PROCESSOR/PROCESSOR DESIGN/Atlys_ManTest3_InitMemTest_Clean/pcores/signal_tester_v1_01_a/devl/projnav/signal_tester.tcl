project new E:/Atlys/Atlys_BIST/Atlys_12_2_RevC_BIST/pcores/signal_tester_v1_01_a/devl/projnav/signal_tester.xise;
project set family spartan6;
project set device xc6slx45;
project set package csg324;
project set speed -2;
project set top_level_module_type HDL;
project set synthesis_tool "XST (VHDL/Verilog)";
lib_vhdl new signal_tester_v1_01_a;
xfile add E:/Atlys/Atlys_BIST/Atlys_12_2_RevC_BIST/pcores/signal_tester_v1_01_a/hdl/vhdl/signal_tester.vhd -lib_vhdl signal_tester_v1_01_a;
xfile add E:/Atlys/Atlys_BIST/Atlys_12_2_RevC_BIST/pcores/signal_tester_v1_01_a/hdl/vhdl/user_logic.vhd -lib_vhdl signal_tester_v1_01_a;
lib_vhdl new proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/proc_common_pkg.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/ipif_pkg.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/or_muxcy.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/or_gate128.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/family_support.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/pselect_f.vhd -lib_vhdl proc_common_v3_00_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/proc_common_v3_00_a/hdl/vhdl/counter_f.vhd -lib_vhdl proc_common_v3_00_a;
lib_vhdl new plbv46_slave_single_v1_01_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/plbv46_slave_single_v1_01_a/hdl/vhdl/plb_address_decoder.vhd -lib_vhdl plbv46_slave_single_v1_01_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/plbv46_slave_single_v1_01_a/hdl/vhdl/plb_slave_attachment.vhd -lib_vhdl plbv46_slave_single_v1_01_a;
xfile add D:/Xilinx/12.2/ISE_DS/EDK/hw/XilinxProcessorIPLib/pcores/plbv46_slave_single_v1_01_a/hdl/vhdl/plbv46_slave_single.vhd -lib_vhdl plbv46_slave_single_v1_01_a;
project close;
