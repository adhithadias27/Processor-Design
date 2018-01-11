#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:D:\Digilent\Projects\Atlys\Atlys_BIST_Release_Images\Atlys_ManTest3_InitMemTest_Clean\system.xmp
#
# WARNING : This file will be re-generated every time a command
# to run a make target is invoked. So, any changes made to this  
# file manually, will be lost when make is invoked next. 
#################################################################

XILINX_EDK_DIR = /cygdrive/m/Xilinx/12.2/ISE_DS/EDK
NON_CYG_XILINX_EDK_DIR = M:/Xilinx/12.2/ISE_DS/EDK

SYSTEM = system

MHSFILE = system.mhs

MSSFILE = system.mss

FPGA_ARCH = spartan6

DEVICE = xc6slx45csg324-2

LANGUAGE = vhdl
GLOBAL_SEARCHPATHOPT = 
PROJECT_SEARCHPATHOPT =  -lp D:/Digilent/Projects/Atlys/edk-support-pack_100721/edk-support-pack/lib/

SEARCHPATHOPT = $(PROJECT_SEARCHPATHOPT) $(GLOBAL_SEARCHPATHOPT)

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT) -msg __xps/ise/xmsgprops.lst

LIBGEN_OPTIONS = -mhs $(MHSFILE) -p $(DEVICE) $(SEARCHPATHOPT) -msg __xps/ise/xmsgprops.lst \
                   $(MICROBLAZE_0_LIBG_OPT)

OBSERVE_PAR_OPTIONS = -error yes

TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT_DIR = TestApp_Memory_microblaze_0
TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT = $(TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT_DIR)/executable.elf
CYG_TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT_DIR = TestApp_Memory_microblaze_0
CYG_TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT = $(CYG_TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT_DIR)/executable.elf

MICROBLAZE_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop.elf
PPC405_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc405/ppc_bootloop.elf
PPC440_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc440/ppc440_bootloop.elf
BOOTLOOP_DIR = bootloops

MICROBLAZE_0_BOOTLOOP = $(BOOTLOOP_DIR)/microblaze_0.elf
MICROBLAZE_0_XMDSTUB = microblaze_0/code/xmdstub.elf

BRAMINIT_ELF_FILES =  $(CYG_TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT) 
BRAMINIT_ELF_FILE_ARGS =   -pe microblaze_0 $(TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT) 

ALL_USER_ELF_FILES = $(CYG_TESTAPP_MEMORY_MICROBLAZE_0_OUTPUT) 

SIM_CMD = vsim

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM)_setup.do

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM)_setup.do

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM)_setup.do

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

MIX_LANG_SIM_OPT = -mixed yes

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(BRAMINIT_ELF_FILE_ARGS) $(MIX_LANG_SIM_OPT) -msg __xps/ise/xmsgprops.lst -s mti -X D:/Digilent/Projects/Atlys/Atlys_BIST_Release_Images/Atlys_ManTest3_InitMemTest_Clean/


LIBRARIES =  \
       microblaze_0/lib/libxil.a 

LIBSCLEAN_TARGETS = microblaze_0_libsclean 

PROGRAMCLEAN_TARGETS = TestApp_Memory_microblaze_0_programclean 

CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/microblaze_0_wrapper.ngc \
implementation/mb_plb_wrapper.ngc \
implementation/ilmb_wrapper.ngc \
implementation/dlmb_wrapper.ngc \
implementation/dlmb_cntlr_wrapper.ngc \
implementation/ilmb_cntlr_wrapper.ngc \
implementation/lmb_bram_wrapper.ngc \
implementation/dip_switches_8bits_wrapper.ngc \
implementation/leds_8bits_wrapper.ngc \
implementation/push_buttons_5bits_wrapper.ngc \
implementation/mcb_ddr2_wrapper.ngc \
implementation/soft_temac_wrapper.ngc \
implementation/clock_generator_0_wrapper.ngc \
implementation/mdm_0_wrapper.ngc \
implementation/proc_sys_reset_0_wrapper.ngc \
implementation/xps_intc_0_wrapper.ngc \
implementation/vhdci_conn_wrapper.ngc \
implementation/phy_reset_component_0_wrapper.ngc \
implementation/ac97_if_0_wrapper.ngc \
implementation/d_usb_epp_dstm_0_wrapper.ngc \
implementation/quad_spi_if_0_wrapper.ngc \
implementation/xps_uart16550_0_wrapper.ngc \
implementation/i2c_bus_wrapper.ngc \
implementation/ps2_keyboard_mouse_wrapper.ngc \
implementation/dvi_in_native_0_wrapper.ngc \
implementation/dvi_out_native_0_wrapper.ngc \
implementation/pll_module_passthrough_0_wrapper.ngc \
implementation/dvi_reset_passthrough_0_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data/system.ucf

BMM_FILE = implementation/$(SYSTEM).bmm

BITGEN_UT_FILE = etc/bitgen.ut

XFLOW_OPT_FILE = etc/fast_runtime.opt
XFLOW_DEPENDENCY = __xps/xpsxflow.opt $(XFLOW_OPT_FILE)

XPLORER_DEPENDENCY = __xps/xplorer.opt
XPLORER_OPTIONS = -p $(DEVICE) -uc $(SYSTEM).ucf -bm $(SYSTEM).bmm -max_runs 7

FPGA_IMP_DEPENDENCY = $(BMM_FILE) $(POSTSYN_NETLIST) $(UCF_FILE) $(XFLOW_DEPENDENCY)

# cygwin path for windows
SDK_EXPORT_DIR = SDK/SDK_Export/hw
CYG_SDK_EXPORT_DIR = SDK/SDK_Export/hw
SYSTEM_HW_HANDOFF = $(SDK_EXPORT_DIR)/$(SYSTEM).xml
CYG_SYSTEM_HW_HANDOFF = $(CYG_SDK_EXPORT_DIR)/$(SYSTEM).xml
SYSTEM_HW_HANDOFF_BIT = $(SDK_EXPORT_DIR)/$(SYSTEM).bit
CYG_SYSTEM_HW_HANDOFF_BIT = $(CYG_SDK_EXPORT_DIR)/$(SYSTEM).bit
SYSTEM_HW_HANDOFF_BMM = $(SDK_EXPORT_DIR)/$(SYSTEM)_bd.bmm
CYG_SYSTEM_HW_HANDOFF_BMM = $(CYG_SDK_EXPORT_DIR)/$(SYSTEM)_bd.bmm
SYSTEM_HW_HANDOFF_DEP = $(CYG_SYSTEM_HW_HANDOFF) $(CYG_SYSTEM_HW_HANDOFF_BIT) $(CYG_SYSTEM_HW_HANDOFF_BMM)

#################################################################
# SOFTWARE APPLICATION TESTAPP_MEMORY_MICROBLAZE_0
#################################################################

TESTAPP_MEMORY_MICROBLAZE_0_SOURCES = TestApp_Memory_microblaze_0/src/TestApp_Memory.c 

TESTAPP_MEMORY_MICROBLAZE_0_HEADERS = 

TESTAPP_MEMORY_MICROBLAZE_0_CC = mb-gcc
TESTAPP_MEMORY_MICROBLAZE_0_CC_SIZE = mb-size
TESTAPP_MEMORY_MICROBLAZE_0_CC_OPT = -O2
TESTAPP_MEMORY_MICROBLAZE_0_CFLAGS = 
TESTAPP_MEMORY_MICROBLAZE_0_CC_SEARCH = # -B
TESTAPP_MEMORY_MICROBLAZE_0_LIBPATH = -L./microblaze_0/lib/ # -L
TESTAPP_MEMORY_MICROBLAZE_0_INCLUDES = -I./microblaze_0/include/ # -I
TESTAPP_MEMORY_MICROBLAZE_0_LFLAGS = # -l
TESTAPP_MEMORY_MICROBLAZE_0_LINKER_SCRIPT = TestApp_Memory_microblaze_0/src/TestApp_Memory_LinkScr.ld
TESTAPP_MEMORY_MICROBLAZE_0_LINKER_SCRIPT_FLAG = -T $(TESTAPP_MEMORY_MICROBLAZE_0_LINKER_SCRIPT) 
TESTAPP_MEMORY_MICROBLAZE_0_CC_DEBUG_FLAG =  -g 
TESTAPP_MEMORY_MICROBLAZE_0_CC_PROFILE_FLAG = # -pg
TESTAPP_MEMORY_MICROBLAZE_0_CC_GLOBPTR_FLAG= # -mxl-gp-opt
TESTAPP_MEMORY_MICROBLAZE_0_MODE = executable
TESTAPP_MEMORY_MICROBLAZE_0_LIBG_OPT = -$(TESTAPP_MEMORY_MICROBLAZE_0_MODE) microblaze_0
TESTAPP_MEMORY_MICROBLAZE_0_CC_INFERRED_FLAGS= -mno-xl-soft-mul -mxl-pattern-compare -mcpu=v7.30.b 
TESTAPP_MEMORY_MICROBLAZE_0_CC_START_ADDR_FLAG=  #  # -Wl,-defsym -Wl,_TEXT_START_ADDR=
TESTAPP_MEMORY_MICROBLAZE_0_CC_STACK_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_MEMORY_MICROBLAZE_0_CC_HEAP_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_MEMORY_MICROBLAZE_0_OTHER_CC_FLAGS= $(TESTAPP_MEMORY_MICROBLAZE_0_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_MEMORY_MICROBLAZE_0_CC_START_ADDR_FLAG) $(TESTAPP_MEMORY_MICROBLAZE_0_CC_STACK_SIZE_FLAG) $(TESTAPP_MEMORY_MICROBLAZE_0_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_MEMORY_MICROBLAZE_0_CC_INFERRED_FLAGS)  \
                  $(TESTAPP_MEMORY_MICROBLAZE_0_LINKER_SCRIPT_FLAG) $(TESTAPP_MEMORY_MICROBLAZE_0_CC_DEBUG_FLAG) $(TESTAPP_MEMORY_MICROBLAZE_0_CC_PROFILE_FLAG) 
