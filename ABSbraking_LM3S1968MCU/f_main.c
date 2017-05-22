// Author: Nathanael Park and Hayden Becker
// f_main for ENCE361 Project 2
// Contains System Clock initialisation and main function

// ABS to Car simulator connections:
// P2.0: FL => PF4
// P2.1: FR => PF5
// P2.2: RL => PF6/CCP1
// P2.3: RR => PF7
// P2.4: Steering => PF2/PWM4
// P2.5: Brakes => PB1/CCP2
// P1.4: Brake Signal Output => PD1/PWM1

#include "f_main.h" // header file for f_main.c

void clockInit()
{
   // Sets the system clock rate to 20 MHz and external crystal frequency to 8MHz
   SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
}

void main()
{
	// Initialisation functions
	clockInit();
	GPIOInputsInit();
	PWMInit();
	LEDInit();
	myTimerInit();
	sysTickInit();
	buttonsInit();
	displayInit();
	// Allows the processor to respond to interrupts
	IntMasterEnable();

	// Displays the introduction screen
	displayIntro();
	while (1)
	{
		// Checks for a braking signal and if not present for a suitable
		// length of time, resets brakes and ABS to zero
		checkBrakeNoSignal();
    	// Checks button input and only progresses if there has been
    	// a button input, recording which input has occurred
		if (schedulerRun())
		{
			// Calculates expected wheel speeds, radius' and angles
			calcExpectedSpeeds();
			// Updates the braking signal to the correct duty cycle
			changeBrakeDuty();
			// Updates the ABS volume to correlate with braking duty cycle
			setAudioVolume(getBrakeStatus());
			// Triggers and toggles the ABS system if any wheels rotates less than
			// 90% of the expected wheel speed
			triggerABS();
			// Schedules the processes depending on button_events
			schedulerTask(getRadius(), getAngle(), getRadians(), getAbove10KPH(), getSteeringStatus(), getExpectedSpeeds(), getBrakeStatus(), getABSStatus(), getWheelSpeeds());
		}
    }
}
