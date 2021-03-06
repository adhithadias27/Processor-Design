/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * 
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */




#define D_USB_BASEADDR XPAR_D_USB_EPP_DSTM_0_BASEADDR

#define EPP_DATA_REG_OFFSET				  0x00
#define EPP_ADDRESS_REG_OFFSET      	  0x04
#define EPP_STATUS_REG_OFFSET       	  0x08
#define EPP_CONTROL_REG_OFFSET            0x0C

#define DSTM_DOUT_REG_OFFSET              0x10
#define DSTM_DIN_REG_OFFSET               0x14
#define DSTM_STATUS_REG_OFFSET            0x18
#define FIFO_TO_PROC_STATUS_REG_OFFSET    0x1C
#define FIFO_FROM_PROC_STATUS_REG_OFFSET  0x20
#define DSTM_CONTROL_REG_OFFSET           0x24

#define EPP_STATUS_READ_REQUESTED	31
#define EPP_STATUS_WRITE_PERFORMED	30
#define EPP_STATUS_PERFORMING			29

#define EPP_CONTROL_SEL_EXT_ADDR			8
#define EPP_CONTROL_BYPASS_ISR_READ		9
#define EPP_CONTROL_BYPASS_ISR_WRITE	10
#define EPP_CONTROL_RESET_EPP				11

#define DSTM_STATUS_READ_EXPECTED		0
#define DSTM_STATUS_WRITE_EXPECTED		1
#define DSTM_STATUS_DONE_READ				2
#define DSTM_STATUS_DONE_WRITE			3
#define DSTM_STATUS_DONE_TRANSFER		4

#define FIFO_TO_PROC_STATUS_FULL			31
#define FIFO_TO_PROC_STATUS_AFULL		30
#define FIFO_TO_PROC_STATUS_EMPTY		29
#define FIFO_TO_PROC_STATUS_AEMPTY		28
#define FIFO_TO_PROC_COUNT_MASK			0xFFF

#define FIFO_FROM_PROC_STATUS_FULL		31
#define FIFO_FROM_PROC_STATUS_AFULL		30
#define FIFO_FROM_PROC_STATUS_EMPTY		29
#define FIFO_FROM_PROC_STATUS_AEMPTY	28
#define FIFO_FROM_PROC_COUNT_MASK		0xFFF

#define DSTM_CONTROL_SET_PKTEND			0
#define DSTM_CONTROL_RESET_DSTM			1

