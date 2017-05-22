// Author: Nathanael Park and Hayden Becker
// f_systemABS.c for ENCE361 Project 2
// Controls function in relation to the implementation and monitoring of the ABS


#include "f_systemABS.h" // header file for f_scheduler.c

// Global variable declaration
uint16_t ABSStatus = 0;
uint16_t PWM1Paused = 0, PWM1Counter = 0;
uint16_t audioLEDMode = 0;
uint16_t slipping = 4; // 0 = FL, 1 = FR, 2 = RL, 3 = RR, 4 = None
uint16_t aboveKPH = 0;

// Initialise the LED
void LEDInit()
{
	// Enable LED3 - pin G2
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
	// Write 0x00 to it to turn it off at initialization
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_0);
}

// Initiliase Pulse Width Modulation on PortH Pin0 and PortD Pin1 for ABS audio and braking output
// signal respectively
void PWMInit()
{
	// Enable the PWM peripheral
    SysCtlPeripheralEnable (SYSCTL_PERIPH_PWM);

	// PWM2 for Audio - pin H0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);
	// Configuration for PWM2 - period controls frequency, pulse width controls volume
	PWMGenConfigure (PWM_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet (PWM_BASE, PWM_GEN_1, SysCtlClockGet () / AUDIO_FREQUENCY);
	PWMPulseWidthSet (PWM_BASE, PWM_OUT_2, 10);
	// Enable PWM generator 1
	PWMGenEnable (PWM_BASE, PWM_GEN_1);
	// Disable PWM2 to prevent audio on initialization
	PWMOutputState (PWM_BASE, PWM_OUT_2_BIT, false);

	// PWM1 for braking control output - pin D1
	SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOD);
	GPIOPinTypePWM (GPIO_PORTD_BASE, GPIO_PIN_1);
    // Configure PWM1 - 500 Hz, 600 usec pulses on PWM1 gives a 30% duty cycle
    PWMGenConfigure (PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    //PWMGenPeriodSet (PWM_BASE, PWM_GEN_0, SysCtlClockGet () / 500); // original program
    PWMGenPeriodSet (PWM_BASE, PWM_GEN_0, SysCtlClockGet () / 200);
    PWMPulseWidthSet (PWM_BASE, PWM_OUT_1, SysCtlClockGet () * 6 / 10000);
    // Set a PWM1 interrupt (PWM1IntHandler)
    PWMGenIntRegister(PWM_BASE, PWM_GEN_0, PWM1IntHandler);
    PWMGenIntTrigEnable(PWM_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);
    PWMIntEnable(PWM_BASE, PWM_INT_GEN_0);
    PWMGenIntClear(PWM_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);
    // Enable PWM generator 0 and PWM1 output
    PWMGenEnable (PWM_BASE, PWM_GEN_0);
    PWMOutputState (PWM_BASE, PWM_OUT_1_BIT, true);
}

// The PWM interrupt handler is responsible for output the braking activation signal of
// a PWM signal of alternating between: present for 25 pulses, absent for 50 ms during ABS
// activation. As specified
void PWM1IntHandler()
{
	PWMGenIntClear(PWM_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);
	if (ABSStatus == 1)
	{
		if (PWM1Paused == 0)
		{
			PWM1Counter++;
			if (PWM1Counter == 25)
			{
				PWM1Counter = 0;
				PWM1Paused = 1;
				PWMOutputState(PWM_BASE, PWM_OUT_1_BIT, false);
			}
		}
		else if (PWM1Paused == 1)
		{
			PWM1Counter++;
			if (PWM1Counter == 25)
			{
				PWM1Counter = 0;
				PWM1Paused = 0;
				PWMOutputState(PWM_BASE, PWM_OUT_1_BIT, true);
			}
		}
	}
}

// Toggles the ABS on/off
void toggleABS(uint16_t ABSMode)
{
	if (ABSStatus == 0 && ABSMode == 1)
	{
		ABSStatus = 1;
		displayClear();
	}
	else if (ABSStatus == 1 && ABSMode == 0)
	{
		ABSStatus = 0;
		PWM1Counter = 0;
		if (PWM1Paused == 1)
		{
			PWMOutputState(PWM_BASE, PWM_OUT_1_BIT, true);
		}
		displayClear();
	}
}

// For the toggling of the ABS systems LED light and audio
void toggleAudioLED(uint16_t mode)
{
	if (mode == 2)
	{
		if (audioLEDMode == 0) {audioLEDMode = 1;}
		else if (audioLEDMode == 1) {audioLEDMode = 0;}
	}
	else {audioLEDMode = mode;}
	if (audioLEDMode == 0)
	{
		PWMOutputState(PWM_BASE, PWM_OUT_2_BIT, false);
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_0);
	}
	else
	{
		//PWMOutputState(PWM_BASE, PWM_OUT_2_BIT, true);
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04);
	}
}

// Updates the ABS volume to correlate with braking duty cycle
void setAudioVolume(uint16_t volume)
{
	// Normalize volume from the range 5-95 to the range 28-100
	volume = volume * 8 / 10 + 24;
	// Disable PWM2, change pulse width (and thus volume), then re-enable
	PWMGenDisable(PWM_BASE, PWM_GEN_1);
	PWMPulseWidthSet (PWM_BASE, PWM_OUT_2, volume);
	PWMGenEnable(PWM_BASE, PWM_GEN_1);
}

// Checks and sets the triggering of the ABS
void triggerABS(void)
{
	uint16_t i = 0;
	uint16_t maxWheelSpeed = 0;
	uint16_t slip = 4;
	for (i = 0; i < 4; i++)
	{
		if (getWheelSpeeds()[i] > maxWheelSpeed)
		{
			maxWheelSpeed = getWheelSpeeds()[i];
		}
		if (maxWheelSpeed >= 10)
		{
			aboveKPH = 1;
		}
		else
		{
			aboveKPH = 0;
		}
	}

	for (i = 0; i < 4; i++)
	{
		if ((getWheelSpeeds()[i] < getExpectedSpeeds()[i] * 9 / 10) && (maxWheelSpeed > 9) && (getBrakeStatus() > 4))
		{
			slip = i; //#
			toggleABS(1);
			break;
		}
	}
	slipping = slip;
}

uint16_t getAbove10KPH(void)
{
	return aboveKPH;
}

uint16_t getABSStatus(void)
{
	return ABSStatus;
}
