// Author: Nathanael Park and Hayden Becker
// Buttons Module for ENCE361 Project 1
// Initializes buttons and returns button input

#include "Buttons.h"

// Configures and initializes the buttons
void buttonsInit()
{
	// Enable GPIO port G pins for buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  	GPIODirModeSet(GPIO_PORTG_BASE, BUTTON_UP|BUTTON_DOWN|BUTTON_LEFT|BUTTON_RIGHT, GPIO_DIR_MODE_IN);
  	// Configure button pins
  	GPIOPadConfigSet(GPIO_PORTG_BASE, BUTTON_UP|BUTTON_DOWN|BUTTON_LEFT|BUTTON_RIGHT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

// Registers any push button events and returns a corresponding
// integer value that determines the operation mode
uint16_t getButtonEvent(uint16_t buttonEvent)
{
	if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_UP)==0)
	{
		buttonEvent = 1;
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_DOWN)==0)
	{
		buttonEvent = 2;
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_LEFT)==0)
	{
		buttonEvent = 3;
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_RIGHT)==0)
	{
		buttonEvent = 4;
	}
	return buttonEvent;
}
