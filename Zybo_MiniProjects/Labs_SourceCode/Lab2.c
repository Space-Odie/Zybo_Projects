/*----------------------------------------------------------
 520 L         Lab 2         Spring 2020
 -----------------------------------------------------------
 File Name: lab2.c
 Author: Ian O'Donnell
 -----------------------------------------------------------
 Version     Date            Description
 1.0         3-5-2020      Initial Release
 2.0         3-7-2020     Added LED's and Print Statements
 -----------------------------------------------------------
 Purpose:
 read the status of the push buttons as well as dip switches
 and writes the corresponding values to the UART terminal
 output.

    a. Output sum of the value of dip switches and push buttons
    b. Output difference of the value of dip switches and push buttons
    c. Output product of the value of dip switches and push buttons
    d. Output the remainder of the division of the value of dip switches by push buttons
    e. Output the ceiling of square root of value of dip switches (You can use two functions:
    ceil and sqrt provided by math.h header file. Search online to see how you can add
    math.h to your SDK project.
    f. Write a function to calculate xy

-----------------------------------------------------------*/
/* Include Files */
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "stdio.h"
#include "sleep.h"
#include <math.h>

/* Definitions */
#define GPIO_PB  XPAR_GPIO_0_DEVICE_ID              // GPIO 0
#define GPIO_SW  XPAR_GPIO_1_DEVICE_ID              //GPIO 1
#define DELAY1 500000                               // Software delay length
#define DELAY2 1000000                              // Software delay length
#define PB_CHANNEL 1                                // GPIO port for PB
#define SW_CHANNEL 1                                // GPIO port for SWITCH
#define LED_CHANNEL 2                               // GPIO port for LED

XGpio sw_led_4bits, btns_4bits;                         // GPIO Device driver instance (switch/button inputs)

float Power(int x, int y)
{
    float pow = 1;
    for (int i=1;  i<=y; i++)
    {
        pow = pow * x;
    }
    return pow;
}

int main(void)
{

    u32 sw_state, pb_state;

    //Initialization Switches (0XF = input, 0x0 = output)
    XGpio_Initialize(&sw_led_4bits, GPIO_SW);    //Initialization using function XGpio_Initialize for dip switches
    XGpio_SetDataDirection(&sw_led_4bits, SW_CHANNEL, 0xF); //Setting data direction using function XGpio_SetDataDirection for dip switches
    //Initialization PB
    XGpio_Initialize(&btns_4bits, GPIO_PB);
    XGpio_SetDataDirection(&btns_4bits, PB_CHANNEL, 0xF);
    //initialize LED
    XGpio_SetDataDirection(&sw_led_4bits, LED_CHANNEL, 0x0);

    while(1)
    {
        //Constantly read the push button and dip switches values
        sw_state = XGpio_DiscreteRead(&sw_led_4bits, SW_CHANNEL);
        pb_state = XGpio_DiscreteRead(&btns_4bits, PB_CHANNEL);

        //and write them to the output terminal using xil_printf function.
        xil_printf("--------------------------------------------------\n");
        xil_printf("Switch Status is: %d \n", sw_state);
        xil_printf("Push Button Status is: %d \n", pb_state);

        //The LEDs represent the corresponding values on dip switches.
        XGpio_DiscreteWrite(&sw_led_4bits, LED_CHANNEL, sw_state);

        //Gather results
        int sum = sw_state + pb_state;
        int difference = sw_state - pb_state;
        int product = sw_state * pb_state;
        int remainder = sw_state % pb_state;
        double root = sqrt(sw_state);
        int round = ceil(root);

        float power = Power(sw_state, pb_state);    //I did this with int and float. int would not let me do 15^15 so I changed it to a float

        //Also assign a delay between each pair of the write function
        usleep(500000);

        //Print Results
        xil_printf("Switch + Button: %d\n",sum);
        xil_printf("Switch - Button: %d\n",difference);
        xil_printf("Switch * Button: %d\n",product);
        xil_printf("Switch % Button: %d\n",remainder);
        xil_printf("Square Root of Switch: %d\n",round);
        printf("Switches ^ Button:  %.0f\n",power);             //Use printf so float value can be printed
        xil_printf("--------------------------------------------------\n");

    }
}
