/*----------------------------------------------------------
 520 L         Lab 3         Spring 2020
 -----------------------------------------------------------
 File Name: lab3.c
 Author: Ian O'Donnell
 -----------------------------------------------------------
 Version     Date            Description
 1.0         3-10-2020      Initial Release
 -----------------------------------------------------------
 Purpose:
 Use a Multiplier that is created in VHDL.
 Multiplier is written to with one 32 bit input that has
 two 16 bit inputs combined together.
 Output is one 32 bit output that can be read from.
-----------------------------------------------------------*/


#include <stdio.h>
#include "xbasic_types.h"
#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xstatus.h"
#include <time.h>
#include <complex.h>

//Base address of multiplier
Xuint32 *baseaddr_p = (Xuint32 *)XPAR_MY_VHDLMULTIPLIER_0_S00_AXI_BASEADDR;


int RandGen(int max_value)
{
	return 1 + rand() % max_value;

}

void TaskTwo()	//Calculate (A +jB)(C + jD)
{
	printf("Task Two\n");

	int Max_Value = 15;
	int A,B,C,D, AC, BD, AD, BC;
	double real, imag;


	//TODO Gather A , B , C and D Values
	A = RandGen(Max_Value);
	B = RandGen(Max_Value);
	C = RandGen(Max_Value);
	D = RandGen(Max_Value);
	xil_printf("A = %d | B = %d j| C = %d | D = %d j\n", A, B, C, D);

	//TODO Read/Write Values to Memory (uncomment code for troubleshooting)
	//AC
	*(baseaddr_p+0) = A <<16 | C;
	//xil_printf("AC Wrote: 0x%08x \n", *(baseaddr_p+0));
	//xil_printf("AC Read: 0x%08x \n\r", *(baseaddr_p+1));
	AC = *(baseaddr_p+1);
	//BD
	*(baseaddr_p+0) = B <<16 | D;
	//xil_printf("BD Wrote: 0x%08x \n", *(baseaddr_p+0));
	//xil_printf("BD Read: 0x%08x \n\r", *(baseaddr_p+1));
	BD = *(baseaddr_p+1);
	//AD
	*(baseaddr_p+0) = A <<16 | D;
	//xil_printf("AD Wrote: 0x%08x \n", *(baseaddr_p+0));
	//xil_printf("AD Read: 0x%08x \n\r", *(baseaddr_p+1));
	AD = *(baseaddr_p+1);
	//BC
	*(baseaddr_p+0) = B <<16 | C;
	//xil_printf("BC Wrote: 0x%08x \n", *(baseaddr_p+0));
	//xil_printf("BC Read: 0x%08x \n\r", *(baseaddr_p+1));
	BC = *(baseaddr_p+1);

	//TODO Write Output Files to  variables


	xil_printf("AC = %d BD = %d | AD = %d j| BC = %d j\n", AC, BD, AD, BC);
	xil_printf("%d - %d + %d j + %d j \n", AC, BD, AD, BC);
	//Calculate Real Portion
	real = AC - BD;
	//Calculate Complex number
	imag = AD + BC;
	//Calculate the Result
	double complex z = real + imag * I;

	//printf("Real: %f\n", real);
	//printf("imag: %f\n", imag);
	printf("result = %.1f% + .1fi\n", creal(z), cimag(z));



}

void TaskOne()
{

		printf("Task One\n");
		int Max_Value = 9999;
		int a = 0; int  b = 1;
		for (int i=0; i<101; i++)
		{
			printf("       Test: %d\n", i);
			printf("------------------------\n");

			int value = RandGen(Max_Value)<<16 | RandGen(Max_Value);
			*(baseaddr_p+a) = value;

			xil_printf("MWrote: 0x%08x \n", *(baseaddr_p+a));
			xil_printf("Read : 0x%08x \n", *(baseaddr_p+b));

			a = a + 4;	//every even register = read
			b = b + 4;	//every odd register = write

			printf("------------------------\n\r");
		}
}

int main()
{
	init_platform();
	xil_printf("Multiplier Test\n\r");
	TaskOne();

	for (int i=0; i<101; i++)
	{
		printf("       Test: %d\n", i);
		printf("------------------------\n");
		TaskTwo();
		printf("------------------------\n\r");
	}
	return 0;

}
