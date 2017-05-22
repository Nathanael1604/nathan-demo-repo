// Author: Nathanael Park and Hayden Becker
// f_sysTick.c for ENCE361 Project 2
// Maintains a 64-bit real time clock with SysTick interrupts at 100 kHz.
// Also handles the Stellaris' on board-timer modules

// Based on the following program;
// SysTick.c
// Author:  P.J. Bones	UCECE
// Last modified:	28.4.2012

#include "f_sysTick.h" // header file for f_sysTick.c

// Global variable declaration
uint16_t sysTickCounter = 0;
float wheelSpeed[] = {0, 0, 0, 0};

// Initializes the Timer 2 block as a 32 bit periodic operating at 8 Hz
// and generates a periodic interrupt at this rate
void myTimerInit(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet() / 8);
	TimerIntRegister(TIMER2_BASE, TIMER_A, myTimerIntHandler);
	TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER2_BASE,TIMER_A);
	TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
}

// The interrupt service routine triggered by timer block 2. Is used to
// update the wheel speeds at a rate of 8 Hz
void myTimerIntHandler(void)
{
	TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	uint16_t i = 0;
	for (i = 0; i < 4; i++)
	{
		wheelSpeed[i] = wheelCounter[i] * pi * 0.36;
		wheelCounter[i] = 0;
	}
}

// Initialises the SysTick timer at a rate of 100 kHz
void sysTickInit(void)
{
   // Set up the period for the SysTick timer.  The SysTick timer period is
   // set as a function of the system clock.
   SysTickPeriodSet (SysCtlClockGet() / SYSTICK_RATE_HZ);

   // Register the interrupt handler
   SysTickIntRegister(sysTickIntHandler);

   // Enable interrupt and device
   SysTickIntEnable ();
   SysTickEnable ();
}

// The SysTick ISR updates the interrupt counts (and hence the duty cycles) of
// the steering wheel location and brake pedal position.
// The SysTick ISR is also assists in the toggling of the ABS light and sound
void sysTickIntHandler(void)
{
	sysTickCounter++;
	if (sysTickCounter > PULSE_DELAY)
	{
		sysTickCounter = 0;
		if (getABSStatus() == 1)
		{
			toggleAudioLED(2);
		}
	}
	if (getSteeringInputHigh())
	{
		steeringCounter++;
	}
	if (getBrakeInputHigh())
	{
		brakeCounter++;
	}
	brakeSignalCounter++;
}

// Returns the SysticK timer value
uint16_t getSysTickCounter(void)
{
	return sysTickCounter;
}

// Returns the monitored wheel speeds
float* getWheelSpeeds()
{
	return wheelSpeed;
}
