/*
 * @file main.c
 *
 * @brief Main source code for the Timers program.
 *
 * This file contains the main entry point and function definitions for the Timers program.
 * It interfaces with the following:
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board Push Buttons (SW2 - SW5)
 *	- EduBase Board Seven-Segment Display
 *
 * The following timers are used to demonstrate periodic interrupts: Timer 0A and Timer 1A.
 *
 * To verify the pinout of the user LED, refer to the Tiva C Series TM4C123G LaunchPad User's Guide
 * Link: https://www.ti.com/lit/pdf/spmu296
 *
 * @author Aaron Nanas
 */
 
#include "TM4C123GH6PM.h"
#include "Timer_0A_Interrupt.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "Seven_Segment_Display.h"

//Declare the function prototype for the user-defined function for Timer CA
void Timer_0A_Periodic_Task(void);

//Initialize a global variable for Timer 0A to keep track of elapsed time in milliseconds
uint32_t Timer_0A_ms_elapsed = 0;

//Initialize a global variable to store the returned toggle rate(in ms)
uint32_t toggle_rate_ms = 0;

//Initialize an unsigned 8-bit global variable that will be
//incremented in the periodic task for Timer 0A
uint8_t count = 0;

int main(void)
{
	//Initialize the LEDs on the EduBase board (Port B)
	EduBase_LEDs_Init();
	
	//Initialize the buttons (Sw2-SW5) on the EduBase board (Port D)
	EduBase_Button_Init();
	
	//Initialize Timer 0A to generate periodic interrupts every 1ms
	Timer_0A_Interrupt_Init(&Timer_0A_Periodic_Task);
	
	//Initialize the SysTick timer used to provide blocking delay functions
	SysTick_Delay_Init();
	
	//Initialize the Seven-Segment Display on the EduBase board
	Seven_Segment_Display_Init();
	
	while(1)
	{
		uint8_t button_status = Get_EduBase_Button_Status();
		toggle_rate_ms = Change_Counter_Speed(button_status);
		Seven_Segment_Display(count);
	}
}

void Timer_0A_Periodic_Task(void)
{
	//Increments the Timer_0A_ms elapsed global variable by 1
	// for each time Timer 0A generates an interrupts
	Timer_0A_ms_elapsed++;
	
	//If 500 milliseconds have passed, clear Timer_0A_ms elapsed to zero
	// and toggle LED0 on the EduBase board
	if (Timer_0A_ms_elapsed >= toggle_rate_ms)
	{
		Timer_0A_ms_elapsed = 0;
		GPIOB -> DATA = GPIOB -> DATA ^ 0x01;
		
		if(count >= 15)
		{
			count = 0;
		}
		else
		{
			count = count + 1;
		}
	}
}