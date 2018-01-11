`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:53:22 04/12/2017 
// Design Name: 
// Module Name:    MAR 
// 
//	MEMORY ADDRESS REGISTER
//	THE REGISTER IS USED TO POINT TO THE DATA MEMORY
//	TO EITHER READ OR WRITE DATA
//
//
//////////////////////////////////////////////////////////////////////////////////
module MAR(
	input clk, load,
	input [23:0] C_bus,
	output reg [23:0] data_addr
	);

	always@(posedge clk)
		begin
			if(load) data_addr <= C_bus;
		end
	
endmodule