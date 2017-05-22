// Author: Nathanael Park and Hayden Becker
// Steering/Wheels Module for ENCE361 Project 2
// Module to control functions regarding the steering wheel and wheels

#include "f_steeringWheels.h" // header file for f_steeringWheels.c
#include <math.h> //include math module for use of trigonometric functions

// ABS to Car simulator connections:
// P2.0: FL => PF4
// P2.1: FR => PF5
// P2.2: RL => PF6/CCP1
// P2.3: RR => PF7
// P2.4: Steering => PF2/PWM4
// P2.5: Brakes => PB1/CCP2
// P1.4: Brake Signal Output => PD1/PWM1

// Global variable declaration
uint32_t PFStatus = 0;
uint16_t steeringInputHigh = 0;
uint16_t steeringCounter = 0;
uint16_t steeringStatus = 0;
uint16_t wheelCounter[] = {0, 0, 0, 0};
float expectedWheelSpeed[] = {0, 0, 0, 0};
float radius[] = {0, 0, 0, 0}; //r_if = radius[0], r_of = radius[1], r_ir = radius[2], r_or = radius[3]
float angle = 0, angle_rad = 0;
float a = 0;

// Initialises PortF for wheel and steering monitoring and PortB for brake position monitoring
// Both ports were registered with interrupt handlers
void GPIOInputsInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPortIntRegister(GPIO_PORTB_BASE, PortBIntHandler);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPortIntRegister(GPIO_PORTF_BASE, PortFIntHandler);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_6);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_7);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_5);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_6);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_7);
}

// Handles the pin change interrupt for PortF, which handles the monitoring of the steering wheel
// position as well as the number of interrupts from the wheel sensors
void PortFIntHandler()
{
	PFStatus = GPIOPinIntStatus(GPIO_PORTF_BASE, true);
	if (PFStatus & GPIO_PIN_2)
	{
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_2);
		if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) == GPIO_PIN_2 && steeringInputHigh == 0)
		{
			steeringInputHigh = 1;
		}
		else if (steeringInputHigh == 1)
		{
			steeringInputHigh = 0;
			steeringStatus = steeringCounter / 5;
			steeringCounter = 0;
		}
	}
	if ((PFStatus & GPIO_PIN_4))// && ~(PFStatusPrev & GPIO_PIN_4))
	{
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
		wheelCounter[0]++;
	}
	if ((PFStatus & GPIO_PIN_5))// && ~(PFStatusPrev & GPIO_PIN_5))
	{
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_5);
		wheelCounter[1]++;
	}
	if ((PFStatus & GPIO_PIN_6))// && ~(PFStatusPrev & GPIO_PIN_6))
	{
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_6);
		wheelCounter[2]++;
	}
	if ((PFStatus & GPIO_PIN_7))// && ~(PFStatusPrev & GPIO_PIN_7))
	{
		GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_7);
		wheelCounter[3]++;
	}
}

// Calculates expected wheel speeds, radius' and angles to be used for comparison with the monitored
// inputs so that the ABS system is triggered if any wheels rotates less than 90% of the expected
// wheel speed
void calcExpectedSpeeds(void)
{
	if (steeringStatus >= 50)
	{
		a = ANGLE_MAX_RAD * ((steeringStatus - 50.0) / 45.0);
		radius[0] = 2.5/(sin(a));
		radius[1] = radius[0] +1.5;
		radius[2] = 2.5/(tan(a));
		radius[3] = radius[2] + 1.5;
	    if ((float)getWheelSpeeds()[0] < ((float)getWheelSpeeds()[1] / (5 / (3 * sin(a) + 5)) - EXPECTED_CALC_TOLERANCE))
	    {
	        // Outer front (left) is slipping
	        expectedWheelSpeed[0] = getWheelSpeeds()[1] / (5 / (3 * sin(a) + 5));
	        expectedWheelSpeed[1] = getWheelSpeeds()[1];
	        expectedWheelSpeed[2] = expectedWheelSpeed[0] * ((3 * sin(a) + 5 * cos(a))/ (3 * sin(a) + 5));
	        expectedWheelSpeed[3] = expectedWheelSpeed[0] * ((cos(a) * 5) / (3 * tan(a) + 5));
	    }
	    else
	    {
	    	expectedWheelSpeed[0] = getWheelSpeeds()[0];
	    	expectedWheelSpeed[1] = getWheelSpeeds()[0] * (5 / (3 * sin(a) + 5));
	    	expectedWheelSpeed[2] = getWheelSpeeds()[0] * ((3 * sin(a) + 5 * cos(a))/ (3 * sin(a) + 5));
	    	expectedWheelSpeed[3] = getWheelSpeeds()[0] * ((cos(a) * 5) / (3 * tan(a) + 5));
	    }
	}
	else
	{
		a = ANGLE_MAX_RAD * ((50.0 - steeringStatus) / 45.0);
		radius[0] = 2.5/(sin(a));
		radius[1] = radius[0] + 1.5;
		radius[2] = 2.5/(tan(a));
		radius[3] = radius[2] + 1.5;
	    if ((float)getWheelSpeeds()[1] < ((float)getWheelSpeeds()[0] / (5 / (3 * sin(a) + 5)) - EXPECTED_CALC_TOLERANCE))
	    {
	    	// Outer front (right) is slipping
	    	expectedWheelSpeed[0] = getWheelSpeeds()[0];
	    	expectedWheelSpeed[1] = getWheelSpeeds()[0] / (5 / (3 * sin(a) + 5));
	    	expectedWheelSpeed[2] = expectedWheelSpeed[1] * ((cos(a) * 5) / (3 * tan(a) + 5));
	    	expectedWheelSpeed[3] = expectedWheelSpeed[1] * ((3 * sin(a) + 5 * cos(a))/ (3 * sin(a) + 5));
	    }
	    else
	    {
	    	expectedWheelSpeed[0] = getWheelSpeeds()[1] * (5 / (3 * sin(a) + 5));
	    	expectedWheelSpeed[1] = getWheelSpeeds()[1];
	    	expectedWheelSpeed[2] = getWheelSpeeds()[1] * ((cos(a) * 5) / (3 * tan(a) + 5));
	    	expectedWheelSpeed[3] = getWheelSpeeds()[1] * ((3 * sin(a) + 5 * cos(a))/ (3 * sin(a) + 5));
	    }
	}
	if (steeringStatus == 50)
	{
		radius[0] = 0;
		radius[1] = 0;
		radius[2] = 0;
		radius[3] = 0;
	}
}

float* getRadius()
{
	return radius;
}

float* getExpectedSpeeds()
{
	return expectedWheelSpeed;
}

float getAngle()
{
	angle = ANGLE_MAX * ((steeringStatus - 50.0) / 45.0);
	return angle;
}

float getRadians()
{
	angle_rad = ANGLE_MAX_RAD * ((steeringStatus - 50.0) / 45.0);
	return angle_rad;
}

uint16_t getSteeringStatus()
{
	return steeringStatus;
}

uint16_t getSteeringInputHigh()
{
	return steeringInputHigh;
}
