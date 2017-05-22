// Author: Nathanael Park and Hayden Becker
// my_timer Module for ENCE361 Project 1
// Handles the Stellaris board's on-board timer

#include "my_timer.h"

void timerInit(void)
{
	// Enable timer 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	// Configure the Timer 0 block to trigger an ADC conversion once every 10 milliseconds
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  // since we want a fast interrupt, use 32-bit timer

	// Automatically reload the timer register with the sample rate, based on the system clock
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/FREQ_SAMPLE);

	// We are using Timer_A to trigger ADC conversions
	TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

	// Enable the timer and start conversion process
	TimerEnable(TIMER0_BASE, TIMER_A);
}
