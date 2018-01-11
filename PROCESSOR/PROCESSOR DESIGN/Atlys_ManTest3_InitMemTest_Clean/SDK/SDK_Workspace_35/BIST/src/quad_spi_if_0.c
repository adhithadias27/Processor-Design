
// 2010 Digilent RO

#include "quad_spi_if_0.h"
#include "xparameters.h"
#include "xio.h"
#include <stdio.h>


extern char verbose;

Xuint32 Status;
Xuint32 start_command;
Xuint32 end_command;
Xuint32 div_rate;
Xuint32 nr_of_bytes;
Xuint32 nr_of_dummy_clks;

u8 addr_0; // Most Significant
u8 addr_1;
u8 addr_2; // Least Significant

Xuint32 Write_Enable (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 MODE);

Xuint32 Read_Status_Register (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 MODE,
		Xuint32 NR_OF_BYTES, u8 *data)
{
	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock and number of Bytes to read (max. 256 Bytes)
	div_rate = (DIV_RATE << 17);
	nr_of_bytes = (1 << 4);

	start_command = 0xFFFE1FF4 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);
	end_command = 0x7FFE1FF4 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x22); // cmd_mode, read_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x11); // cmd_mode, read_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, read_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, RDSR);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);
	//xil_printf("\n0x%0.8x",Status);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)//((Status != 0xC0000003) && (Status != 0xC0000002) && (Status != 0xC0000011))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	xil_printf(" ");

	// Read Bytes from RxFIFO - data read from Flash memory
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*data = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	return 0;
}

Xuint32 Write_Status_Register (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE,
		Xuint32 MODE, u8 DATA)
{
	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xBFFE0018 & (div_rate | 0xC001FFFF);
	end_command = 0x3FFE0018 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x82); // cmd_mode, write_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x41); // cmd_mode, write_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, write_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, WRSR);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Data into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, DATA);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);
	//xil_printf("\n0x%x",Status);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	xil_printf(" ");

	return 0;
}

/*********************************************************************************************************
 * READ FLAG STATUS REGISTER
 *
 * This function reads the Flag Status Register, returning its value as an 8-bit unsigned number.
 * The Flag Status Register has the following structure:
 *
 *    bit 7  |  bit 6  |  bit 5  |  bit 4  |  bit 3  |  bit 2  |  bit 1  |  bit 0
 * __________|_________|_________|_________|_________|_________|_________|_________
 * |  P/E C  |   ES    |    E    |    P    |   Vpp   |    PS   |    Pr   |/////////|
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 * Where:
 * P/E C = Program/Erase Controller Status
 * ES = Erase Suspend
 * E = Erase Error
 * P = Program Error
 * Vpp = Vpp Error
 * PS = Program Suspend
 * Pr = Protection Error
 * Bit 0 = Reserved
 *
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 		memory.
 * @param	MODE is the mode on which the communication between Quad-SPI IP Core and Flash Memory is made:
 * 		can be Extended Mode, Dual Mode, Quad Mode.
 * @param	NR_OF_BYTES represents the number of bytes to be read in one cycle. If polling the Flag Status
 * 		Register, this may be 1.
 * @param	data is a pointer to the data read from Flash Memory.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available for all three modes.
 *
 ********************************************************************************************************/

Xuint32 Read_Flag_Status_Register (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 MODE,
		Xuint32 NR_OF_BYTES, u8 *data)
{
	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock and number of Bytes to read (max. 256 Bytes)
	div_rate = (DIV_RATE << 17);
	nr_of_bytes = (1 << 4);

	start_command = 0xFFFE1FF4 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);
	end_command = 0x7FFE1FF4 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x22); // cmd_mode, read_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x11); // cmd_mode, read_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, read_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, RFSR);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);
	//xil_printf("\n0x%0.8x",Status);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)//((Status != 0xC0000003) && (Status != 0xC0000002) && (Status != 0xC0000011))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	//xil_printf(" ");

	// Read Bytes from RxFIFO - data read from Flash memory
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*data = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	return 0;
}

/*********************************************************************************************************
 * WRITE ENABLE
 *
 * This function enables the WEL bit from the Write Latch. This must be done prior to every Program, Erase
 * or Write operation.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 		memory.
 * @param	MODE is the mode on which the communication between Quad-SPI IP Core and Flash Memory is made:
 * 		can be Extended Mode, Dual Mode, Quad Mode.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available for all three modes.
 ********************************************************************************************************/

Xuint32 Write_Enable (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 MODE)
{
	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting the mode and the div rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xFFFE0000 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0000 & (div_rate | 0xC001FFFF);


	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x02); // cmd_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x01); // cmd_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode = 0


	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, WREN);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);
	//xil_printf("\n0x%0.8x",Status);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	//xil_printf(" ");
	//xil_printf(" ");

	return 0;
}

/*********************************************************************************************************
 * WRITE VOLATILE ENHANCED CONFIGURATION REGISTER
 *
 * This function allows new values to be written to the Volatile Enhanced Configuration Register.
 * The Volatile Enhanced Configuration Register has the following structure:
 *
 *    bit 7  |  bit 6  |  bit 5  |  bit 4  |  bit 3  |  bit 2  |  bit 1  |  bit 0
 * __________|_________|_________|_________|_________|_________|_________|_________
 * |  QUAD   |   DUAL  |/////////|    R    |   Acc   |           OUTPUT            |
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 * Where:
 * QUAD = Quad Input Command
 * DUAL = Dual Input Command
 * Bit 5 = Reserved
 * R = Reset/Hold Disable
 * Acc = Accelerator pin enable
 * OUTPUT = Output Driver Strength
 *
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 		memory.
 * @param	MODE is the mode on which the communication between Quad-SPI IP Core and Flash Memory is made:
 * 		can be Extended Mode, Dual Mode, Quad Mode.
 * @param	DATA is the 8-bit unsigned value to write to the Volatile Enhanced Configuration Register.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available for all three modes.
 ********************************************************************************************************/

Xuint32 Write_Volatile_Enhanced_Configuration_Register (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE,
		Xuint32 MODE, u8 DATA)
{
	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xBFFE0018 & (div_rate | 0xC001FFFF);
	end_command = 0x3FFE0018 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x82); // cmd_mode, write_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x41); // cmd_mode, write_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, write_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, WRVECR);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Data into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, DATA);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);
	//xil_printf("\n0x%x",Status);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	//xil_printf(" ");

	return 0;
}

/*********************************************************************************************************
 * CLEAR FLAG STATUS REGISTER                                                                            *
 ********************************************************************************************************/

Xuint32 Clear_Flag_Status_Register (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE,
		Xuint32 MODE)
{

	// Enable multi-line command
	if (MODE == 2)
		Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F); // quad command input
	else if (MODE == 1)
		Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF); // dual command input
	else
		Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF); // single command input

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xFFFE0000 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0000 & (div_rate | 0xC001FFFF);


	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x02); // cmd_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x01); // cmd_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode = 0


	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, CLFSR);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);


	// Wait until session end
	while (!((Status | 0xFFFFFFF6) & 0x09))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	}

	return 0;
}

/*********************************************************************************************************
 * READ IDENTIFICATION
 *
 * This function reads Flash Memory's identification data.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MANUFACT_ID is a pointer to the 8-bit Manufacturer Identification.
 * @param	MEM_TYPE is a pointer to the 8-bit Memory Type.
 * @param	MEM_CAPACITY is a pointer to the 8-bit Memory Capacity.
 * @param	EDID is a pointer to the Extended Device ID.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available only for Extended Mode.
 ********************************************************************************************************/

Xuint32 Read_Identification (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, u8 *MANUFACT_ID,
		u8 *MEM_TYPE, u8 *MEM_CAPACITY, u8 *EDID)
{

	Xuint32 edid_first_byte;
	Xuint32 edid_second_byte;

	// single command input
	Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock
	div_rate = (DIV_RATE << 17);
	start_command = 0xFFFE0054 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0054 & (div_rate | 0xC001FFFF);

	// Setting modes (cmd_mode = 0, read_mode = 0)
	XIo_Out32(QuadSPI_Baseaddr + MODES_REG, 0x00);

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, RDID);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	// Wait until session end
	while ((Status != 0xC0000011) && (Status != 0xC0000002) && (Status != 0xC0000003))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	}

	xil_printf(" ");

	// Read 1 Byte from RxFIFO - manufacturer identification
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MANUFACT_ID = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - memory type
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MEM_TYPE = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - memory capacity
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MEM_CAPACITY = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - extended device ID
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	edid_first_byte = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - extended device ID
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	edid_second_byte = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	//edid_first_byte = (edid_first_byte << 8);
	*EDID = edid_first_byte;// & (0xFFFFFF00 | edid_second_byte);

	return 0;
}

/*********************************************************************************************************
 * MULTIPLE I/O READ IDENTIFICATION (works only in Dual and Quad Mode)
 *
 * This function reads Flash Memory's identification data.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MANUFACT_ID is a pointer to the 8-bit Manufacturer Identification.
 * @param	MEM_TYPE is a pointer to the 8-bit Memory Type.
 * @param	MEM_CAPACITY is a pointer to the 8-bit Memory Capacity.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available only for Dual and Quad Modes.
 ********************************************************************************************************/

Xuint32 Multiple_IO_Read_Identification (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE,
		Xuint32 MODE, u8 *MANUFACT_ID, u8 *MEM_TYPE, u8 *MEM_CAPACITY)
{

	if (MODE == 2)
		// quad command input
		Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
	else
		// quad command input
		Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xFFFE0034 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0034 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x22); // cmd_mode = 2, read_mode = 2
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x11); // cmd_mode = 1, read_mode = 1

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, MIORDID);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	//Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	// Wait until session end
	while ((Status != 0xC0000011) && (Status != 0xC0000002) && (Status != 0xC0000003))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
	}
	xil_printf(" ");


	// Read 1 Byte from RxFIFO - manufacturer identification
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MANUFACT_ID = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - memory type
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MEM_TYPE = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	// Read 1 Byte from RxFIFO - memory capacity
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	*MEM_CAPACITY = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);

	return 0;
}

/*********************************************************************************************************
 * FAST READ
 *
 * This function reads the data stored in Flash Memory.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MODE is the mode on which the data will be read: can be Extended Mode, Dual Mode, Quad Mode.
 * @param	ADDRESS is the start address from which the data will be read.
 * @param	NR_OF_BYTES is the number of bytes to read.
 * @param	NR_OF_DUMMY_CLKS is number of dummy clock cycles until data is ready to be read.
 * @param	DATA is the data read.
 *
 * @return	None.
 *
 * @note
 *
 * This function is available for all modes.
 *
 ********************************************************************************************************/

Xuint32 Fast_Read (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, Xuint32 MODE,
		Xuint32 ADDRESS, Xuint32 NR_OF_BYTES, Xuint32 NR_OF_DUMMY_CLKS, u8 DATA[256])
{
	int i;
	u8 data = 0;

	// Enable multi-line command
	if (CURRENT_MODE != MODE)
	{
		if (MODE == 2)
			// quad command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
		else if (MODE == 1)
			// dual command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);
		else
			// single command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);
	}

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting div_rate for serial clock, number of Bytes to read (max. 256 Bytes) and the
	// number of dummy clocks
	div_rate = (4 << 17);
	nr_of_bytes = (NR_OF_BYTES << 4);
	nr_of_dummy_clks = (NR_OF_DUMMY_CLKS << 13);

	// Setting the address from which the reading will be performed
	addr_2 = ADDRESS;
	addr_1 = (ADDRESS >> 8);
	addr_0 = (ADDRESS >> 16);

	start_command = 0xBFFFFFF7 & (div_rate | 0xC001FFFF) & (nr_of_dummy_clks | 0xFFFE1FFF) &
			(nr_of_bytes | 0xFFFFE00F);
	end_command = 0x3FFFFFF7 & (div_rate | 0xC001FFFF) & (nr_of_dummy_clks | 0xFFFE1FFF) &
			(nr_of_bytes | 0xFFFFE00F);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x2A); // cmd_mode, addr_mode, read_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x15); // cmd_mode, addr_mode, read_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, addr_mode, read_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, FAST_READ);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 0 (most significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_0);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 1 into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_1);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 2 (least significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_2);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	Status= 0x00000000;
	// Wait until session end
	while (!Status)
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}
	xil_printf("");
	xil_printf("");

	// Read Bytes from RxFIFO - data read from Flash Memory
	for (i = 0; i < NR_OF_BYTES; i++)
	{
		XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x01);
		//xil_printf("");
		XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
		data = XIo_In32(QuadSPI_Baseaddr + DATA_OUT_REG);
		DATA[i] = data;
	}

	return 0;
}

/*********************************************************************************************************
 * PAGE PROGRAM
 *
 * This function writes data to Flash Memory.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MODE is the mode on which the data will be writen: can be Extended Mode, Dual Mode, Quad Mode.
 * @param	ADDRESS is the start address from which the data will be written.
 * @param	NR_OF_BYTES is the number of bytes to write.
 * @param	DATA is the data to write.
 *
 * @return	If there was an error writing, returns the value of the Volatile Enhanced Configuration
 * 			Register, otherwise returns 0x03.
 *
 * @note
 *
 * This function is available for all modes.
 *
 ********************************************************************************************************/

Xuint32 Page_Program (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, Xuint32 MODE,
		Xuint32 ADDRESS, Xuint32 NR_OF_BYTES, u8 DATA[256])
{
	int i;
	u8 data;
	u8 Status_Reg = 0;

	// Enable multi-line command
	if (CURRENT_MODE != MODE)
	{
		if (MODE == 2)
			// quad command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
		else if (MODE == 1)
			// dual command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);
		else
			// single command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);
	}

	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting the mode and the div rate for serial clock and number of Bytes
	div_rate = (DIV_RATE << 17);
	nr_of_bytes = (NR_OF_BYTES << 4);

	// Setting the address from which the reading will be performed
	addr_2 = ADDRESS;
	addr_1 = (ADDRESS >> 8);
	addr_0 = (ADDRESS >> 16);

	start_command = 0xBFFE1FF9 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);
	end_command = 0x3FFE1FF9 & (div_rate | 0xC001FFFF) & (nr_of_bytes | 0xFFFFE00F);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x8A); // cmd_mode, addr_mode, write_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x45); // cmd_mode, addr_mode, write_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, addr_mode, write_mode = 0

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, PP);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 0 (most significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_0);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 1 into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_1);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 2 (least significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_2);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put data to write into TxFIFO
	for (i = 0; i < NR_OF_BYTES+1; i++)
	{
		data = DATA[i];
		XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, data);
		XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
		XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);
	}

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	Status = 0x00000000;
	// Wait until session end
	while (!Status)//((Status != 0xC0000011) && (Status != 0xC0000013) && (Status != 0xC0000012))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		Status &= 0x00000011;
	}

	xil_printf(" ");
	//xil_printf(" ");

	while (!Status_Reg)
	{
		Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
	}

	// Check if it was any error on programming
	if (Status_Reg == 0x80)
		return 0x03;
	else
		return Status_Reg;
}

/*********************************************************************************************************
 * SUBSECTOR ERASE
 *
 * This function erases a subsector(4kb) of data from Flash Memory.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MODE is the mode on which the data will be erased: can be Extended Mode, Dual Mode, Quad Mode.
 * @param	ADDRESS is the start address from which the data will be erased.
 *
 * @return	If there was an error erasing, returns the value of the Volatile Enhanced Configuration
 * 			Register, otherwise returns 0x03.
 *
 * @note
 *
 * This function is available for all modes.
 ********************************************************************************************************/

Xuint32 Subsector_Erase (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, Xuint32 MODE,
		Xuint32 ADDRESS)
{
	u8 Status_Reg = 0;

	// Enable multi-line command
	if (CURRENT_MODE != MODE)
	{
		if (MODE == 2)
			// quad command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
		else if (MODE == 1)
			// dual command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);
		else
			// single command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);
	}

	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting the div rate for serial clock
	div_rate = (DIV_RATE << 17);

	// Setting the address
	addr_2 = ADDRESS;
	addr_1 = (ADDRESS >> 8);
	addr_0 = (ADDRESS >> 16);

	start_command = 0xFFFE0001 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0001 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x0A); // cmd_mode, addr_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x05); // cmd_mode, addr_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, addr_mode = 0

	//Clear_Flag_Status_Register (QuadSPI_Baseaddr, 4, CURRENT_MODE, MODE);
	//Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
	//if (verbose) xil_printf("\n0x%x",Status_Reg);

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, SSE);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 0 (most significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_0);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 1 into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_1);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 2 (least significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_2);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	//Status = 0x00;
	// Wait until session end
	while ((Status != 0xC0000011) && (Status != 0xC0000012) && (Status != 0xC0000013))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		//Status &= 0x00000011;
	}
	xil_printf(" ");

	//Status_Reg = 0x00;
	while (!Status_Reg)
	{
		Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
		//xil_printf("\n0x%x",Status_Reg);
	}

	// Check if it was any error on programming
	if (Status_Reg == 0x80)
		return 0x03;
	else
		return Status_Reg;
}

/*********************************************************************************************************
 * SECTOR ERASE
 *
 * This function erases a sector of data from Flash Memory.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MODE is the mode on which the data will be erased: can be Extended Mode, Dual Mode, Quad Mode.
 * @param	ADDRESS is the start address from which the data will be erased.
 *
 * @return	If there was an error erasing, returns the value of the Volatile Enhanced Configuration
 * 			Register, otherwise returns 0x03.
 *
 * @note
 *
 * This function is available for all modes.
 ********************************************************************************************************/

Xuint32 Sector_Erase (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, Xuint32 MODE,
		Xuint32 ADDRESS)
{
	u8 Status_Reg = 0;

	// Enable multi-line command
	if (CURRENT_MODE != MODE)
	{
		if (MODE == 2)
			// quad command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
		else if (MODE == 1)
			// dual command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);
		else
			// single command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);
	}

	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting the div rate for serial clock
	div_rate = (DIV_RATE << 17);

	// Setting the address
	addr_2 = ADDRESS;
	addr_1 = (ADDRESS >> 8);
	addr_0 = (ADDRESS >> 16);

	start_command = 0xFFFE0001 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0001 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x0A); // cmd_mode, addr_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x05); // cmd_mode, addr_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode, addr_mode = 0

	//Clear_Flag_Status_Register (QuadSPI_Baseaddr, 4, CURRENT_MODE, MODE);
	//Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
	//xil_printf("\n0x%x",Status_Reg);

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, SE);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 0 (most significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_0);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 1 into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_1);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Put Address 2 (least significant) into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, addr_2);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	//Status = 0x00;
	// Wait until session end
	while ((Status != 0xC0000013) && (Status != 0xC0000012) && (Status != 0xC0000011))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		//Status &= 0x00000011;
	}
	xil_printf(" ");

	//Status_Reg = 0x00;
	while (!Status_Reg)
	{
		Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
	}

	// Check if it was any error on programming
	if (Status_Reg == 0x80)
		return 0x03;
	else
		return Status_Reg;
}

/*********************************************************************************************************
 * BULK ERASE
 *
 * This function erases the entire data from Flash Memory.
 *
 * @param	QuadSPI_Baseaddr is the base address for the Quad-SPI IP Core.
 * @param	DIV_RATE is the divide rate of the System Clock, for generating Serial Clock for the Flash
 * 			memory.
 * @param	CURRENT_MODE is the mode on which the Flash Memory was working on the previous operation: can
 * 			be Extended Mode, Dual Mode, Quad Mode.
 * @param	MODE is the mode on which the data will be erased: can be Extended Mode, Dual Mode, Quad Mode.
 * @param	ADDRESS is the start address from which the data will be erased.
 *
 * @return	If there was an error erasing, returns the value of the Volatile Enhanced Configuration
 * 			Register, otherwise returns 0x03.
 *
 * @note
 *
 * This function is available for all modes.
 *
 ********************************************************************************************************/

Xuint32 Bulk_Erase (Xuint32 QuadSPI_Baseaddr, Xuint32 DIV_RATE, Xuint32 CURRENT_MODE, Xuint32 MODE)
{
	u8 Status_Reg = 0;

	// Enable multi-line command
	if (CURRENT_MODE != MODE)
	{
		if (MODE == 2)
			// quad command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0x7F);
		else if (MODE == 1)
			// dual command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xBF);
		else
			// single command input
			Write_Volatile_Enhanced_Configuration_Register (QuadSPI_Baseaddr, DIV_RATE, CURRENT_MODE, 0xFF);
	}

	// Enable writing
	Write_Enable (QuadSPI_Baseaddr, DIV_RATE, MODE);

	// Reseting FSM's and FIFO's
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x01);
	XIo_Out32(QuadSPI_Baseaddr + RESET_REG, 0x00);

	// Setting the div rate for serial clock
	div_rate = (DIV_RATE << 17);

	start_command = 0xFFFE0000 & (div_rate | 0xC001FFFF);
	end_command = 0x7FFE0000 & (div_rate | 0xC001FFFF);

	// Setting modes
	if (MODE == 2)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x02); // cmd_mode = 2
	else if (MODE == 1)
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x01); // cmd_mode = 1
	else
		XIo_Out32 (QuadSPI_Baseaddr + MODES_REG, 0x00); // cmd_mode = 0

	//Clear_Flag_Status_Register (QuadSPI_Baseaddr, 4, CURRENT_MODE, MODE);
	//Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
	//xil_printf("\n0x%x",Status_Reg);

	// Put instruction command into TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + DATA_IN_REG, BE);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x02); //write enable on TxFIFO
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_FIFO_REG, 0x00);

	// Start a cycle session
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, start_command);
	XIo_Out32(QuadSPI_Baseaddr + CONTROL_CYCLE_REG, end_command);

	//Status = 0x00000000;
	// Wait until session end
	while ((Status != 0xC0000013) && (Status != 0xC0000011) && (Status != 0xC0000012))
	{
		Status = XIo_In32(QuadSPI_Baseaddr + STATUS_REG);
		//Status &= 0x00000011;
	}
	xil_printf(" ");


	while (!Status_Reg)
	{
		Read_Flag_Status_Register (QuadSPI_Baseaddr, DIV_RATE, MODE, 1, &Status_Reg);
		//xil_printf("\n0x%x", Status_Reg);
	}

	// Check if it was any error on programming
	if (Status_Reg == 0x80)
		return 0x03;
	else
		return Status_Reg;
}

/*********************************************************************************************************
 *********************************************************************************************************
 *********************************************************************************************************
 *********************************************************************************************************
 *********************************************************************************************************
 *********************************************************************************************************
 * CHECK INITIAL MODE                                                                                    *
 ********************************************************************************************************/

Xuint32 Check_Initial_Mode (Xuint32 QuadSPI_Baseaddr)
{
	u8 MANUFACT_ID;
	u8 MEM_TYPE;
	u8 MEM_CAPACITY;
	u8 EDID;

	// Check Read ID in MODE 0
	Read_Identification (QuadSPI_Baseaddr, 4, 0, &MANUFACT_ID, &MEM_TYPE, &MEM_CAPACITY, &EDID);

	if (verbose) xil_printf("\nManufact ID mode extended: 0x%x,", MANUFACT_ID);

	if (MANUFACT_ID == 0x20)
	{
		if (verbose) xil_printf("\nCurrent mode: extended");
		return 0x00;
	}
	else
	{
		// Check Read ID in MODE 1
		Multiple_IO_Read_Identification (QuadSPI_Baseaddr, 4, 1, 1, &MANUFACT_ID, &MEM_TYPE, &MEM_CAPACITY);

		if (verbose) xil_printf("\nManufact ID mode dual: 0x%x,", MANUFACT_ID);

		if (MANUFACT_ID == 0x20)
		{
			if (verbose) xil_printf("\nCurrent mode: dual");
			return 0x01;
		}
		else
		{
			// Check Read ID in MODE 2
			Multiple_IO_Read_Identification (QuadSPI_Baseaddr, 4, 2, 2, &MANUFACT_ID, &MEM_TYPE, &MEM_CAPACITY);

			if (verbose) xil_printf("\nManufact ID mode quad: 0x%x,", MANUFACT_ID);

			if (MANUFACT_ID == 0x20)
			{
				if (verbose) xil_printf("\nCurrent mode: quad");
				return 0x02;
			}
			else
			{
				if (verbose) xil_printf("\nError reading");
				return 0xFFFFFFFF;
			}
		}
	}
}

/*********************************************************************************************************
 * QUAD SPI FLASH MEMORY TEST (for Atlys bist)                                                           *
 ********************************************************************************************************/

Xuint32 Quad_SPI_Flash_Test (Xuint32 QuadSPI_Baseaddr)
{

	u8 DATA_TO_WRITE[6];
	u8 DATA_READ[6];
	Xuint32 Current_Mode;
	Xuint32 Error;
	Xuint32 Data_Return;

	// Assign the control Bytes (to be written into Flash)
	DATA_TO_WRITE[0] = 0x00;
	DATA_TO_WRITE[1] = 0xFF;
	DATA_TO_WRITE[2] = 0x01;
	DATA_TO_WRITE[3] = 0x02;
	DATA_TO_WRITE[4] = 0x04;
	DATA_TO_WRITE[5] = 0x08;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

	// Check if IDCODE is correct
	if (Current_Mode == 0xFFFFFFFF)
	{
		// Check again
/*		Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

		if (Current_Mode == 0xFFFFFFFF)
		{*/
			Data_Return = 0x70; // "memory not found"
//		}
	}
//	else
//	{
		// Erase the memory to ensure a proper writing
	Error = Subsector_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2, 0);

	if (Error != 0x03)
	{
		Clear_Flag_Status_Register (QuadSPI_Baseaddr, 4, Current_Mode, 2);
		Error = Subsector_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2, 0);

		if (Error != 0x03)
		{
			Data_Return = 0x80; // "erasing memory failed"
		}
	}
//		else
//		{
	// Write to memory in BY-4 mode: 0x00, 0xFF, 0x01, 0x02, 0x04, 0x08
	Error = Page_Program (QuadSPI_Baseaddr, 2, Current_Mode, 2, 0, 6, DATA_TO_WRITE);

	// Check if Writing was OK
	if (Error != 0x03)
	{
		Data_Return = 0x60; // "write to memory failed"
	}
//			else
//			{
				// Read data from the memory
	Fast_Read (QuadSPI_Baseaddr, 2, Current_Mode, 2, 0, 6, 10, DATA_READ);

	// Erase written data
	Error = Subsector_Erase (QuadSPI_Baseaddr, 2, 2, 2, 0);
	if (Error != 0x03)
	{
		Clear_Flag_Status_Register (QuadSPI_Baseaddr, 4, Current_Mode, 2);
		Error = Subsector_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2, 0);

		if (Error != 0x03)
		{
			Data_Return = 0x80; // "erasing memory failed"
		}
	}

	if (verbose)
	{
		xil_printf("\nData_Read[0]: 0x%x", DATA_READ[0]);
		xil_printf("\nData_Read[1]: 0x%x", DATA_READ[1]);
		xil_printf("\nData_Read[2]: 0x%x", DATA_READ[2]);
		xil_printf("\nData_Read[3]: 0x%x", DATA_READ[3]);
		xil_printf("\nData_Read[4]: 0x%x", DATA_READ[4]);
		xil_printf("\nData_Read[5]: 0x%x", DATA_READ[5]);
	}

	// Check if data read is the same as the written one
//	if (XIo_In32(QuadSPI_Baseaddr + OCCUPANCY_RXFIFO_REG) == 0x00)
//	{
	if ((DATA_READ[0] == DATA_TO_WRITE[0]) &&
			(DATA_READ[1] == DATA_TO_WRITE[1]) &&
			(DATA_READ[2] == DATA_TO_WRITE[2]) &&
			(DATA_READ[3] == DATA_TO_WRITE[3]) &&
			(DATA_READ[4] == DATA_TO_WRITE[4]) &&
			(DATA_READ[5] == DATA_TO_WRITE[5]))
	{
		Data_Return = 0x00; // "memory passed the test"
	}
	else
	{
		if (DATA_READ[2] == 0)
		{
			Data_Return = 0x10; // "Error on FIRST line"
		}
		else if (DATA_READ[3] == 0)
		{
			Data_Return = 0x20; // "Error on SECOND line"
		}
		else if (DATA_READ[4] == 0)
		{
			Data_Return = 0x30; // "Error on THIRD line"
		}
		else if (DATA_READ[5] == 0)
		{
			Data_Return = 0x40; // "Error on FOURTH line"
		}
		else
		{
			Data_Return = 0x50; // "read from memory failed"
		}
	}
//				}
//				else
//				{
//					Data_Return = 0x50; // "read from memory failed"
//				}
//			}
//		}
//	}
	return Data_Return;
}

/*********************************************************************************************************
 * BLANK CHECK ENTIRE FLASH MEMORY                                                                       *
 ********************************************************************************************************/

Xuint32 Blank_Check_Entire_Memory (Xuint32 QuadSPI_Baseaddr)
{
	Xuint32 i;
	Xuint32 j;
	Xuint32 Non_Blank_Bytes = 0;
	Xuint32 z = 0;
	Xuint32 Current_Mode;
	u8 DATA_READ[256];

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

	for (i = 0; i < 16776959; i += 256)
	{
		// Read every Byte from memory
		Fast_Read (QuadSPI_Baseaddr, 2, Current_Mode, 2, i, 256, 10, DATA_READ);
		if (verbose)
		{
			if (i == z)
			{
				if (verbose) xil_printf("\n DATA[0x%x]: 0x%x", i, DATA_READ[1]);
				z = z + 0x10000;
			}
		}

		// Compare every Byte read
		for (j = 0; j < 256; j++)
		{
			if (DATA_READ[j] != 0xFF)
				Non_Blank_Bytes++;
		}
	}

	if (Non_Blank_Bytes != 0)
		return Non_Blank_Bytes;
	else
		return 0x00;

}

/*********************************************************************************************************
 * ERASE ENTIRE FLASH MEMORY                                                                             *
 ********************************************************************************************************/

Xuint32 Erase_Entire_Memory (Xuint32 QuadSPI_Baseaddr)
{
	Xuint32 Current_Mode;
	Xuint32 Error;
	Xuint32 Return;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);
	if (Current_Mode == 0xFFFFFFFF)
		Return = 0xFFFFFFFF;

	Error = Bulk_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2);

	if (Error != 0x03)
	{
		Clear_Flag_Status_Register (QuadSPI_Baseaddr, 2, Current_Mode, 2);
		Error = Bulk_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2);

		if (Error != 0x03)
		{
			Return = Error;
		}
		else
			Return = 0x00;
	}
	else
		Return = 0x00;

	return Return;
}

/*********************************************************************************************************
 * BLANK CHECK SECTOR                                                                                    *
 ********************************************************************************************************/

Xuint32 Blank_Check_Sector (Xuint32 QuadSPI_Baseaddr, Xuint32 ADDRESS)
{
	Xuint32 i;
	Xuint32 j;
	Xuint32 z = 0;
	Xuint32 Non_Blank_Bytes = 0;
	Xuint32 Current_Mode;
	u8 DATA_READ[256];

	ADDRESS &= 0xFF0000;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

	for (i = ADDRESS; i < (ADDRESS + 65536); i += 256)
	{
		// Read every Byte from memory
		Fast_Read (QuadSPI_Baseaddr, 2, Current_Mode, 2, i, 256, 10, DATA_READ);

		if (verbose)
		{
			if (i == z)
			{
				if (verbose) xil_printf("\n DATA[0x%x]: 0x%x", i, DATA_READ[1]);
				z = z + 0x10000;
			}
		}

		// Compare every Byte read: if it is not blank erase that sector
		for (j = 0; j < 256; j++)
		{
			if (DATA_READ[j] != 0xFF)
				Non_Blank_Bytes++;
		}
	}


	if (Non_Blank_Bytes != 0)
		return Non_Blank_Bytes;
	else
		return 0x00;
}

/*********************************************************************************************************
 * ERASE SECTOR                                                                                          *
 ********************************************************************************************************/

Xuint32 Erase_Sector (Xuint32 QuadSPI_Baseaddr, Xuint32 ADDRESS)
{
	Xuint32 Current_Mode;
	Xuint32 Error;
	Xuint32 Return;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);
	if (Current_Mode == 0xFFFFFFFF)
		Return = 0xFFFFFFFF;

	Error = Sector_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2, ADDRESS);

	if (Error != 0x03)
	{
		Clear_Flag_Status_Register (QuadSPI_Baseaddr, 2, Current_Mode, 2);
		Error = Sector_Erase (QuadSPI_Baseaddr, 2, Current_Mode, 2, ADDRESS);

		if (Error != 0x03)
		{
			Return = Error;
		}
		else
			Return = 0x00;
	}
	else
		Return = 0x00;

	return Return;
}

/*********************************************************************************************************
 * MANUFACTURER IDENTIFICATION                                                                           *
 ********************************************************************************************************/

Xuint32 Manufact_ID (Xuint32 QuadSPI_Baseaddr)
{
	Xuint32 Current_Mode;
	Xuint32 Return;
	u8 MANUFACT_ID;
	u8 m1, m2, m3;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

	if (Current_Mode == 0xFFFFFFFF)
	{
		// Check again
		Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);

		if (Current_Mode == 0xFFFFFFFF)
		{
			Return = 0xFFFFFFFF;
		}
	}

	if (Current_Mode == 0x00)
	{
		Read_Identification (XPAR_QUAD_SPI_IF_0_BASEADDR, 4, Current_Mode, &MANUFACT_ID, &m1, &m2, &m3);
		Return = (MANUFACT_ID << 16) | (m1 << 8) | m2;
	}
	else
	{
		Multiple_IO_Read_Identification (XPAR_QUAD_SPI_IF_0_BASEADDR, 4, Current_Mode, 2, &MANUFACT_ID, &m1, &m2);
		Return = (MANUFACT_ID << 16) | (m1 << 8) | m2;
	}


	return Return;
}

Xuint32 Clear_Flags (Xuint32 QuadSPI_Baseaddr)
{
	Xuint32 Current_Mode;

	// Check initial mode (current mode)
	Current_Mode = Check_Initial_Mode (QuadSPI_Baseaddr);
	Clear_Flag_Status_Register (QuadSPI_Baseaddr, 2, Current_Mode, 2);

	return 0;
}

