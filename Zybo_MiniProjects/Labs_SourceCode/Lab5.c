/*----------------------------------------------------------
 520 L         Lab 5         Fall 2020
 -----------------------------------------------------------
 File Name: Interrupt.c
 Author: Ian O'Donnell
 -----------------------------------------------------------
 Version     Date            Description
 1.0         3-31-2020        Initial Release
 2.0         4-03-2020        Added Part 2-D requirements
 3.0         4-03-2020        Added Part 3 requirements

 -----------------------------------------------------------
 Purpose: 
 Create a project using buttons, a counter, and switches.
 The counter counts based on a set amount of time. 
 The buttons and switches will interrupt this process.  
 LED’s will display the current count.
 UART is used to display the function of each input and 
 The current count. 
 ----------------------------------------------------------*/
#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

#include <stdio.h>
#include "xbasic_types.h"

#include "xil_printf.h"

#include "xparameters.h"
#include "xstatus.h"
#include <time.h>
#include "sleep.h"

// Parameter definitions
#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define TMR_DEVICE_ID		XPAR_TMRCTR_0_DEVICE_ID
#define BTNS_DEVICE_ID		XPAR_AXI_GPIO_0_DEVICE_ID
#define LEDS_DEVICE_ID		XPAR_AXI_GPIO_1_DEVICE_ID
#define SWITCH_DEVICE_ID	XPAR_AXI_GPIO_2_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID_1 XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define INTC_GPIO_INTERRUPT_ID_2 XPAR_FABRIC_AXI_GPIO_2_IP2INTC_IRPT_INTR
#define INTC_TMR_INTERRUPT_ID XPAR_FABRIC_AXI_TIMER_0_INTERRUPT_INTR

#define BTN_INT 			XGPIO_IR_CH1_MASK
#define SW_INT 				XGPIO_IR_CH1_MASK
#define TMR_LOAD			0xF8000000
#define TMR_LOAD_2			0xFF000000 //0xF0000000 = double the time
//define timer counter variables
#define TMR_CNT_1  			0xF0000
#define TMR_CNT_2 			0x3

XGpio LEDInst, BTNInst, SWInst;
XScuGic INTCInst;
XTmrCtr TMRInst;
static int led_data;
static int btn_value;
static int tmr_count;
static int sw_value;

//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
static void BTN_Intr_Handler(void *baseaddr_p);
static void SW_Intr_Handler(void *baseaddr_p);
static void TMR_Intr_Handler(void *baseaddr_p);
static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static int IntcInitFunction(u16 DeviceId, XTmrCtr *TmrInstancePtr, XGpio *GpioInstancePtr);

//----------------------------------------------------
// INTERRUPT HANDLER FUNCTIONS
// - called by the timer, button interrupt, performs
// - LED flashing
//----------------------------------------------------
int sw1_latched = 0;
int sw2_latched = 0;
int sw3_latched = 0;
int sw4_latched;
int Switch_1;
int Switch_2;
int Switch_3;
int Switch_4;

int tmr_cnt;
int n = 1;

void SW_Intr_Handler(void *InstancePtr)
{
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&SWInst, SW_INT);
	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&SWInst) & SW_INT) !=
			SW_INT) {
			return;
		}
	sw_value = XGpio_DiscreteRead(&SWInst, 1);

	 Switch_1 = sw_value & 0x1;
	// Switch_2 = sw_value & 0x2;
	// Switch_3 = sw_value & 0x4;
	 Switch_4 = sw_value & 0x8;

	//Code Switch 1

	if (Switch_1 == 1 && sw1_latched == 0){
		sw1_latched = 1;
		XTmrCtr_Stop(&TMRInst,0);
		xil_printf("Timer Paused \n");
	}

	else if(Switch_1 == 0 && sw1_latched == 1){
		sw1_latched = 0;

		XTmrCtr_Start(&TMRInst,0);	//start timer
		xil_printf("Timer Resumed \n");
	}

	//Code Switch 2
		//Coder within the TMR_Intr_Handler because that's where the counting occurs

	//Code Switch 3
	Switch_3 = sw_value & 0x4;
	if (Switch_3 == 4 && sw3_latched == 0){
		sw3_latched = 1;
		 XTmrCtr_SetResetValue(&TMRInst, 0, TMR_LOAD_2);
		xil_printf("Timer Loaded Value: %d \n", TMR_LOAD_2);
	}
	else if(Switch_3 == 0 && sw3_latched == 1){
		sw3_latched = 0;
		 XTmrCtr_SetResetValue(&TMRInst, 0, TMR_LOAD);
		xil_printf("Timer Loaded Default Value: %d \n", TMR_LOAD);
	}

	//Code Switch 4
		//Moved to TMR_Intr_handler

	//xil_printf("Sw1: %d, Sw2: %d, Sw3: %d, Sw4: %d, Sw3_Latch = %d \n", Switch_1, Switch_2, Switch_3, Switch_4, sw3_latched);

    //XGpio_DiscreteWrite(&LEDInst, 1, led_data);

    (void)XGpio_InterruptClear(&SWInst, SW_INT);
    // Enable GPIO interrupts
    XGpio_InterruptEnable(&SWInst, SW_INT);
}



void BTN_Intr_Handler(void *InstancePtr)
{
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&BTNInst, BTN_INT);
	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) !=
			BTN_INT) {
			return;
		}
	btn_value = XGpio_DiscreteRead(&BTNInst, 1);

	// Reset if centre button pressed
	switch(btn_value){
		case 1 :
			XTmrCtr_Reset(&TMRInst,0);	//reset timer with right button
			xil_printf("Timer Reset\n");
			break;
		case 2 :
			XGpio_DiscreteWrite(&LEDInst, 1, 0);	//reset LEDs
			xil_printf("LED's Reset\n");
			break;
		case 4 :
			XTmrCtr_Start(&TMRInst,0);	//start timer
			xil_printf("Timer Started\n");
			break;
		case 8 :
			XTmrCtr_Stop(&TMRInst,0);	//stop timer
			xil_printf("Timer Stopped \n");
			break;
		default :

			break;
	}

	int value = XTmrCtr_GetValue(&TMRInst,0);
	//TODO: Add a feature to determine if the timer is STOPPED (get value = previous get value)

	//xil_printf("Get Value: %d\n", value);

    //XGpio_DiscreteWrite(&LEDInst, 1, led_data);

    (void)XGpio_InterruptClear(&BTNInst, BTN_INT);
    // Enable GPIO interrupts
    XGpio_InterruptEnable(&BTNInst, BTN_INT);
}

void TMR_Intr_Handler(void *data)
{
	if (XTmrCtr_IsExpired(&TMRInst,0)){
		sw_value = XGpio_DiscreteRead(&SWInst, 1);

		if (Switch_4 == 8 && sw4_latched == 0){
			sw4_latched = 1;
			tmr_cnt = TMR_CNT_1;
			xil_printf("tmr_count Max value set to %d \n", tmr_cnt);
		}
		else if(Switch_4 == 0 && sw4_latched == 1){
			sw4_latched = 0;
			tmr_cnt = TMR_CNT_2;
			xil_printf("tmr_count Max value set to %d \n", tmr_cnt);
		}



		// Once timer has expired 3 times, stop, increment counter
		// reset timer and start running again
		if(tmr_count == tmr_cnt){
			XTmrCtr_Stop(&TMRInst,0);
			tmr_count = 0;

			//if DIP Switch 2 is active, cause the LED up count to be a down count

			Switch_2 = sw_value & 0x2;
			if (Switch_2 == 2 && sw2_latched == 0){
				sw2_latched = 1;
				n = -1;
				xil_printf("Counting Down\n");
			} else if(Switch_2 == 0 && sw2_latched == 1) {
				sw2_latched = 1;
				n = 1;
				xil_printf("Counting Up \n");
			}

			led_data = led_data + n;

			//reset LED's with button value
			btn_value = XGpio_DiscreteRead(&BTNInst, 1);
			if (btn_value == 2){
				led_data = 0;
				xil_printf("led_data reset\n");
			}
			xil_printf("Count: %d\n", led_data);
			XGpio_DiscreteWrite(&LEDInst, 1, led_data);
			XTmrCtr_Reset(&TMRInst,0);
			XTmrCtr_Start(&TMRInst,0);


		}
		else tmr_count++;
		//xil_printf("tmr_count = %d\n", tmr_count);

	}
}



//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int main (void)
{
  int status;
  //----------------------------------------------------
  // INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
  //----------------------------------------------------
  // Initialise LEDs
  status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
  if(status != XST_SUCCESS) return XST_FAILURE;
  // Initialise Push Buttons
  status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
  if(status != XST_SUCCESS) return XST_FAILURE;
  // Initialize Switches
  status = XGpio_Initialize(&SWInst, SWITCH_DEVICE_ID);
  if(status != XST_SUCCESS) return XST_FAILURE;

  // Set LEDs direction to outputs
  XGpio_SetDataDirection(&LEDInst, 1, 0x00);
  // Set all buttons direction to inputs
  XGpio_SetDataDirection(&BTNInst, 1, 0xFF);
  // SET ALL SWITCHES DIRECTIONS TO INPUTS
  XGpio_SetDataDirection(&SWInst, 1, 0xFF);





  //----------------------------------------------------
  // SETUP THE TIMER
  //----------------------------------------------------
  status = XTmrCtr_Initialize(&TMRInst, TMR_DEVICE_ID);
  if(status != XST_SUCCESS) return XST_FAILURE;
  XTmrCtr_SetHandler(&TMRInst, TMR_Intr_Handler, &TMRInst);
  XTmrCtr_SetResetValue(&TMRInst, 0, TMR_LOAD);
  XTmrCtr_SetOptions(&TMRInst, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);


  // Initialize interrupt controller
  status = IntcInitFunction(INTC_DEVICE_ID, &TMRInst, &BTNInst);
  if(status != XST_SUCCESS) return XST_FAILURE;

  // Initialize interrupt controller
  status = IntcInitFunction(INTC_DEVICE_ID, &TMRInst, &SWInst);
  if(status != XST_SUCCESS) return XST_FAILURE;

  XTmrCtr_Start(&TMRInst, 0);

  while(1);

  return 0;
}

//----------------------------------------------------
// INITIAL SETUP FUNCTIONS
//----------------------------------------------------

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt - Buttons
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&BTNInst);

	// Enable interrupt - Switches
	XGpio_InterruptEnable(&SWInst, SW_INT);
	XGpio_InterruptGlobalEnable(&SWInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();


	return XST_SUCCESS;

}

int IntcInitFunction(u16 DeviceId, XTmrCtr *TmrInstancePtr, XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_INTERRUPT_ID_1,
					  	  	 (Xil_ExceptionHandler)BTN_Intr_Handler,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_INTERRUPT_ID_2,
					  	  	 (Xil_ExceptionHandler)SW_Intr_Handler,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect timer interrupt to handler
	status = XScuGic_Connect(&INTCInst,
							 INTC_TMR_INTERRUPT_ID,
							 (Xil_ExceptionHandler)TMR_Intr_Handler,
							 (void *)TmrInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID_1);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID_2);


	XScuGic_Enable(&INTCInst, INTC_TMR_INTERRUPT_ID);


	return XST_SUCCESS;
