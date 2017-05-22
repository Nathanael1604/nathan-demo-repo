// Author: Nathanael Park and Hayden Becker
// Scheduler Module for ENCE361 Project 2
// Controller to handle events and display

#include "f_scheduler.h" // header file for f_scheduler.c

// Global variable declaration
uint16_t mode = 0;

// Running state; get button input and display the menu
uint16_t schedulerRun()
{
	mode = getButtonEvent(mode);
	return mode;
}

// Scheduling the tasks to perform and menu to display depending on button events,
// also making sure the correct information is sent to the display functions
void schedulerTask(float* radius, float angle, float radians, uint16_t aboveKPH, uint16_t steeringStatus, float* expectedSpeeds, uint16_t brakeStatus, uint16_t ABSStatus, float* wheelSpeed)
{
	if (mode == 1)
    {
    	displayMilestone(steeringStatus, brakeStatus, wheelSpeed);
    }
    else if (mode == 2)
    {
    	displayExpectedSpeeds(angle, radians, expectedSpeeds);
    }
    else if (mode == 3)
    {
    	displayABSinfo(angle, aboveKPH, steeringStatus, brakeStatus, ABSStatus);
    }
    else if (mode == 4)
    {
    	displayTest(radius, steeringStatus);
    }
    else if (mode == 5)
    {
    	displayMenu();
    }
}
