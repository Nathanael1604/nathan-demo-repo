// Author: Nathanael Park and Hayden Becker
// f_brakes.c for ENCE361 Project 2
// Contains

#include "f_Brakes.h" // header file for f_Brakes.c

// Global variable declaration
uint16_t brakeInputHigh = 0;
uint16_t brakeStatus = 0, brakeSignalCounter = 0, brakeCounter = 0;

// The PortB interrupt handler is a pin change interrupt ISR that handles incoming signals from
// the brakes for monitoring of the brake pedal position.
// Also assists in toggling the audio and LED for the ABS system
void PortBIntHandler() {
	GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_1);
	if ((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) && (brakeInputHigh == 0))
	{
		brakeInputHigh = 1;
	}
	else if (brakeInputHigh == 1)
	{
		brakeInputHigh = 0;
		if (brakeStatus == 0)
		{
			toggleAudioLED(1);
		}
		brakeStatus = brakeCounter / 10;
		brakeCounter = 0;
		brakeSignalCounter = 0;
	}
}

// Updates the braking signal to the correct duty cycle by updating the PWM pulse width
void changeBrakeDuty()
{
	PWMGenDisable(PWM_BASE, PWM_GEN_0);
	PWMPulseWidthSet (PWM_BASE, PWM_OUT_1, SysCtlClockGet () *brakeStatus / 50000);
	PWMGenEnable(PWM_BASE, PWM_GEN_0);
}

// Checks for a braking signal and if not present for a suitable
// length of time, resets brakes and ABS to zero
void checkBrakeNoSignal()
{
	if (brakeSignalCounter > 5000)
	{
		brakeStatus = 0;
		toggleABS(0);
		slipping = 4; //#
		brakeSignalCounter = 0;
		toggleAudioLED(0);
		changeBrakeDuty();
	}
}

// Returns the value of brakeStatus
uint16_t getBrakeStatus()
{
	return brakeStatus;
}

// Returns the value of brakeInputHigh
uint16_t getBrakeInputHigh()
{
	return brakeInputHigh;
}
