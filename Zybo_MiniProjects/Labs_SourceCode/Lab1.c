/*--------------------------------------------------------------
 526 L					Mini Project				Spring 2021
----------------------------------------------------------------
LAB Title: Final Exam Take Home Mini Project 1
File Name: Mini_Project.c
Author: Ian O'Donnell
----------------------------------------------------------------
Version					Date					Description
1.0         			3/25/21	     		 Initial Release
----------------------------------------------------------------
Purpose:
	generates a periodic waveform defined by five parameters:
	1. Final value, 2. Drop value, 3. Step Horizontal value,
	4. Step Vertical Value, 5. Period
---------------------------------------------------------------*/
#include <stdio.h>
#include "xbasic_types.h"
#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xstatus.h"
#include <time.h>
#include "sleep.h"

//write to BRAM: Xil_Out16(address, value);
//read from BRAM: int num = Xil_In16(address);
/*###############################################################################################################
#	Purpose: Creates a delay for the program - Could also use sleep command
###############################################################################################################*/
void DELAY(int delay)
{

	for (volatile int Delay=0; Delay<delay; Delay++);
}

/*###############################################################################################################
#	Purpose: 	Tells user if input values will generate an error 
#	Input: 		User Input Values
#	Output: 	Print error message 
###############################################################################################################*/

void Error_Handler(int Step_Horizontal_Value, int Step_Vertical_Value, int Period, int Drop_Value, int Final_Value)
{
//These Errors are not 'handled' as the design does not require it but the messages will display for the user.
	int Distance_Traveled = Drop_Value/Step_Vertical_Value * Step_Horizontal_Value;

	if (Distance_Traveled >= Period)
	{
		printf( "Period Value is too small, Final Value will not be reached. (Error: %d <= %d)\n", Period, Distance_Traveled );
	}


	if ( Step_Horizontal_Value > Period )
	{
		printf("Horizontal Value must be less than the Period\n");
	}

	if ( Step_Vertical_Value > Drop_Value )
	{
		printf( "Vertical Step Value must be less than the Drop Value\n");
	}

	if (Drop_Value > Final_Value)
	{
		printf( "Drop Value is greater than your Final Value, Can not use negative values");
	}
}

/*###############################################################################################################
#	Purpose: 	Generate Waveform based on user input values. First Value will be the Final Value. 
				The wave form will ramp up, from the dropped value, until the final value is reached. 
				The X axis is the period and y value is the vertical value. 
#	Input: 		User Input Values
#	Output: 	Print error message 
###############################################################################################################*/
int main()
{
	short int Final_Value, Drop_Value, Period, Step_Vertical_Value, Step_Horizontal_Value;
	//initialize
	xil_printf("Mini Project!\n");
	xil_printf("Gathering User Inputs. . .\n");
/*	//Hardcoded Values
	Final_Value = 10000;
	Drop_Value = 8500;
	Period = 851;
	Step_Vertical_Value = 20;
	Step_Horizontal_Value = 2;
*/
	//Allow User to Enter Values in:
    printf("Enter The Final Value: ");
    scanf("%hu", &Final_Value);

    printf("Enter Drop_Value: ");
    scanf("%hu", &Drop_Value);

    printf("Enter The Period: ");
    scanf("%hu", &Period);

    printf("Enter The Step_Vertical_Value: ");
    scanf("%hu", &Step_Vertical_Value);

    printf("Enter The Step_Horizontal_Value: ");
    scanf("%hu", &Step_Horizontal_Value);

	//Error Handler
	xil_printf("Checking for Errors . . .\n");
	Error_Handler(Step_Horizontal_Value, Step_Vertical_Value, Period, Drop_Value, Final_Value);


	//Write to BRAM (Calculate Values)
	xil_printf("Generating X Values. . .\n");
	int Current_Value = 0;

	int n = 2; //Step 2
	int H_Max = n *(Period / Step_Horizontal_Value);		//number of iterations needed for runs

	//Write the Horizontal Values (X) to the BRAM (H_Max will always be greater or equal to V_Max?)
	for (int j = 0; j <= H_Max; j = j + n)
	{
		if (j == 0)
		{
			Current_Value == 0;
			Xil_Out16(j, Current_Value);							//First value will always start at 0
		}

		else
		{
			Current_Value = Current_Value + Step_Horizontal_Value; 	//Additional Values will be incremented by the step value
			Xil_Out16(j, Current_Value);
		}

	}

	//Write the Vertical Values (Y) to the BRAM (in address starting after the last horizontal value.
	xil_printf("Generating Y Values. . .\n");
	int Y_Value = 0;
	int Y_Base = H_Max + n;
	for (int i = Y_Base; i<= Y_Base*2; i = i + n)
	{
		if (i == Y_Base)
		{
			Y_Value = Final_Value;										//First Value is Final Value
			Xil_Out16(i, Y_Value);
		}
		if (i == Y_Base + n)
		{
			Y_Value = Final_Value - Drop_Value;							//Second Value is Final Value - Drop Value
			Xil_Out16(i, Y_Value);
		}
		else
			if ((Y_Value + Step_Vertical_Value) < Final_Value)			//be looking one step ahead to verify it does not go over final value
			{
				Y_Value = Y_Value + Step_Vertical_Value;				//Additional Values will be incremented by the step value
				Xil_Out16(i, Y_Value);
			}
			else 														//Don't increment Current_Value once it has reached the Final Value, alsi set it to Final Vlaue
			{
				Xil_Out16(i, Final_Value);
			}
	}

	//Read Values (There is a chance that there are more X values than Y values, but never more Ys than Xs?
	xil_printf("Final Value = %d, Drop Value = %d Period = %d, \nStep Vertical Value = %d Step Horizontal Value = %d\n",
				Final_Value, Drop_Value, Period, Step_Vertical_Value, Step_Horizontal_Value);

	xil_printf(" X , Y \n");
	for (int k = 0; k <= H_Max + 1; k = k + n)
	{
		int H_Read = Xil_In16(k);
		int Y_Read = Xil_In16((Y_Base + k));
		xil_printf(" %d , %d \n",H_Read, Y_Read);

	}
}
