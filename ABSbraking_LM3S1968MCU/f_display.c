// Author: Nathanael Park and Hayden Becker
// Display Module for ENCE361 Project 2
// Initialises and controls the OLED display

#include "f_display.h"
#include "f_buttons.h"
#include "f_sysTick.h"
#include "f_scheduler.h"
#include "f_systemABS.h"

// Global variable declaration
char strBuffer[100];

// Initialises the OLED display
void displayInit(void)
{
  RIT128x96x4Init(1000000);
  RIT128x96x4Clear();
}

// Displays the Title Header
void displayHeader(void)
{
	RIT128x96x4StringDraw("ABS Controller", 20, 4, 15);
	RIT128x96x4StringDraw("------------------", 9, 12, 15);
}

// Clear the display to get rid of residual information
void displayClear(void)
{
	RIT128x96x4Clear();
}

// Displays the introduction screen
void displayIntro()
{
	RIT128x96x4StringDraw("==================", 9, 16, 15);
    RIT128x96x4StringDraw("Welcome to", 33, 24, 15);
    RIT128x96x4StringDraw("ABS Controller", 20, 32, 15);
    RIT128x96x4StringDraw("==================", 9, 40, 15);
    RIT128x96x4StringDraw("-> Please Press <-", 9, 54, 15);
    RIT128x96x4StringDraw("-> Select <-", 28, 63, 15);
    RIT128x96x4StringDraw("By H. Becker", 28, 80, 15);
    RIT128x96x4StringDraw("& N. Park", 36, 88, 15);
}

// Displays the menu modes
void displayMenu()
{
	displayHeader();
	RIT128x96x4StringDraw( "<ABS MENU MODES>", 15, 33, 15);
	RIT128x96x4StringDraw( "Milestone 2 -> up", 6, 51, 15);
	RIT128x96x4StringDraw( "Expect Info -> down", 6, 60, 15);
	RIT128x96x4StringDraw( "ABS Info    -> left", 6, 69, 15);
	RIT128x96x4StringDraw( "Debugging   -> right", 6, 78, 15);
}

// Displays the Milestone 2 information:
// - Wheel speeds from sensor inputs
// - Steering wheel location
// - Brake pedal position
void displayMilestone(uint16_t steeringStatus, uint16_t brakeStatus, float* wheelSpeed)
{
	displayHeader();
	RIT128x96x4StringDraw( "~ Milestone 2 Info ~", 5, 22, 15);
	sprintf(strBuffer, "Wheel FL: %1.0fkph ", wheelSpeed[0]);
	RIT128x96x4StringDraw(strBuffer, 5, 37, 15);
	sprintf(strBuffer, "Wheel FR: %1.0fkph ", wheelSpeed[1]);
	RIT128x96x4StringDraw(strBuffer, 5, 46, 15);
	sprintf(strBuffer, "Wheel RL: %1.0fkph ", wheelSpeed[2]);
	RIT128x96x4StringDraw(strBuffer, 5, 55, 15);
	sprintf(strBuffer, "Wheel RR: %1.0fkph ", wheelSpeed[3]);
	RIT128x96x4StringDraw(strBuffer, 5, 64, 15);
	sprintf(strBuffer, "Steering: %i%%  ", steeringStatus);
	RIT128x96x4StringDraw(strBuffer, 5, 73, 15);
	sprintf(strBuffer, "Brakes: %i%%  ", brakeStatus);
	RIT128x96x4StringDraw(strBuffer, 5, 82, 15);
}

// Displays relevant ABS information
void displayABSinfo(float angle, uint16_t aboveKPH, uint16_t steeringStatus, uint16_t brakeStatus, uint16_t ABSStatus)
{
	displayHeader();
	RIT128x96x4StringDraw( "~ ABS Information ~", 8, 22, 15);
	sprintf(strBuffer, "Above 10 KPH?: %s  ", aboveKPH ? "Yes" : "No");
	RIT128x96x4StringDraw(strBuffer, 5, 37, 15);
	sprintf(strBuffer, "Steering: %i%%    ", steeringStatus);
	RIT128x96x4StringDraw(strBuffer, 5, 46, 15);
	sprintf(strBuffer, "Wheel Angle: %0.1f  ", angle);
	RIT128x96x4StringDraw(strBuffer, 5, 55, 15);
	sprintf(strBuffer, "Brakes: %i%% ", brakeStatus);
	RIT128x96x4StringDraw(strBuffer, 5, 64, 15);
	sprintf(strBuffer, "ABS Status: %s   ", ABSStatus ? "On" : "Off");
	RIT128x96x4StringDraw(strBuffer, 5, 73, 15);
	switch(slipping)
		{
			case 0:
				sprintf(strBuffer, "Slipping: FL  ");
				break;
			case 1:
				sprintf(strBuffer, "Slipping: FR  ");
				break;
			case 2:
				sprintf(strBuffer, "Slipping: RL  ");
				break;
			case 3:
				sprintf(strBuffer, "Slipping: RR  ");
				break;
			case 4:
				sprintf(strBuffer, "Slipping: None");
		}
	RIT128x96x4StringDraw(strBuffer, 5, 84, 15);
}

// Diplays expected wheel speeds and turning angles
void displayExpectedSpeeds(float angle, float angle_rad, float* expectedWheelSpeed)
{
	displayHeader();
	RIT128x96x4StringDraw( "~ Expected Speeds ~", 8, 22, 15);
	sprintf(strBuffer, "Expected FL: %1.0fkph ", expectedWheelSpeed[0]);
	RIT128x96x4StringDraw(strBuffer, 5, 37, 15);
	sprintf(strBuffer, "Expected FR: %1.0fkph ", expectedWheelSpeed[1]);
	RIT128x96x4StringDraw(strBuffer, 5, 46, 15);
	sprintf(strBuffer, "Expected RL: %1.0fkph ", expectedWheelSpeed[2]);
	RIT128x96x4StringDraw(strBuffer, 5, 55, 15);
	sprintf(strBuffer, "Expected RR: %1.0fkph ", expectedWheelSpeed[3]);
	RIT128x96x4StringDraw(strBuffer, 5, 64, 15);
	sprintf(strBuffer, "Wheel Angle: %0.1f  ", angle);
	RIT128x96x4StringDraw(strBuffer, 5, 73, 15);
	sprintf(strBuffer, "Radians: %0.4f  ", angle_rad);
	RIT128x96x4StringDraw(strBuffer, 5, 82, 15);
}

// A test screen for displaying additional information or debugging purposes
// Displays the expected the turning radii of the wheels
void displayTest(float* radius, uint16_t steeringStatus)
{
	displayHeader();
	RIT128x96x4StringDraw( "~ Debug Screen ~", 15, 22, 15);
	RIT128x96x4StringDraw( "~ Test Variables: ~", 8, 31, 15);
	if (steeringStatus >= 50)
	{
		sprintf(strBuffer, "FL = r_of: %0.3f  ", radius[1]);
		RIT128x96x4StringDraw(strBuffer, 5, 46, 15);
		sprintf(strBuffer, "FR = r_if: %0.3f  ", radius[0]);
		RIT128x96x4StringDraw(strBuffer, 5, 55, 15);
		sprintf(strBuffer, "RL = r_or: %0.3f  ", radius[3]);
		RIT128x96x4StringDraw(strBuffer, 5, 64, 15);
		sprintf(strBuffer, "RR = r_ir: %0.3f  ", radius[2]);
		RIT128x96x4StringDraw(strBuffer, 5, 73, 15);
	}
	else
	{
		sprintf(strBuffer, "FL = r_if: %0.3f  ", radius[0]);
		RIT128x96x4StringDraw(strBuffer, 5, 46, 15);
		sprintf(strBuffer, "FR = r_of: %0.3f  ", radius[1]);
		RIT128x96x4StringDraw(strBuffer, 5, 55, 15);
		sprintf(strBuffer, "RL = r_ir: %0.3f  ", radius[2]);
		RIT128x96x4StringDraw(strBuffer, 5, 64, 15);
		sprintf(strBuffer, "RR = r_or: %0.3f  ", radius[3]);
		RIT128x96x4StringDraw(strBuffer, 5, 73, 15);
	}
}
