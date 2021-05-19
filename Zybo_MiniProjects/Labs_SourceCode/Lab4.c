/*----------------------------------------------------------
 520 L         Lab 4         Spring 2020
 -----------------------------------------------------------
 File Name: tbSPI.v
 Author: Ian O'Donnell
 -----------------------------------------------------------
 Version     Date            Description
 1.0         2-17-2020      Initial Release
 -----------------------------------------------------------
 Purpose: 
 Simulates a common synchronous serial communication protocol 
 called Serial Peripheral Interface for a slave core module.
 Full Duplex Mode is used
 ----------------------------------------------------------*/
 
`timescale 1ns/1ns
module tb_SPI(); 
  
  //PARAMETERS
  parameter ADDR_WIDTH = 7; //Register Width
  parameter DATA_WIDTH = 16; //data Width

  //Local Parameters 
  localparam CLOCK_CYCLE = 10;
  localparam SLOCK_CYCLE = 50;
  localparam MOSI_DELAY = 100; // One Duty Cycle of SCLOCK

  //INPUTS
  reg CLK;                    //Clock Input (10Mhz)
  reg SCLK;                   //Synchronous Clock Input (50Mhz)
  reg SSN;                    //Slave Select: active Low - Transmit Bit;
  reg MOSI;                   //Master Output/ Serial Input: (One Bit at a time) 
  reg [DATA_WIDTH-1:0] RDATA; //Read Data Input (Sent to MISO)

  //OUTPUTS
  wire [ADDR_WIDTH-1:0] ADDR;     //Address Output (From MISO)
  wire [DATA_WIDTH-1:0] WDATA;    //Data Output (From MISO)       
  wire WSTROBE;                   //Write Strobe (From MISO)
  wire MISO;                       //Master Input/Slave Output: one bit Serial Output (From RDATA)
  

  //declare i for for loop
  integer i;

  SPI UUT(SCLK, CLK, SSN, MOSI, RDATA, MISO, WSTROBE, WDATA, ADDR); 

    //Create Clock Generator
    initial begin
      CLK = 0;
      forever #CLOCK_CYCLE CLK = ~CLK;
    end

    //Create SClock Generator
     initial begin
      SCLK = 0;
      forever #SLOCK_CYCLE if (!SSN) SCLK = ~SCLK;
    end

    initial 
    begin 

    //Initialize Inputs (ground floating values)
    SSN = 1; MOSI = 0;
    RDATA = 16'b1100110011001100; //set read data to a 16 bit value
    #200 SSN = 0; //Transmit On

    for(i = 0; i<24; i = i + 1) begin    //test each operation
      #MOSI_DELAY MOSI = ~MOSI; 
    end
    
    SSN = 1;
    end 
endmodule

