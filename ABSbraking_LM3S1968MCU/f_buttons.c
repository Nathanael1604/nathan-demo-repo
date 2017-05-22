// Author: Nathanael Park and Hayden Becker
// Buttons Module for ENCE361 Project 2
// Initializes buttons and returns button input

#include "f_buttons.h" // header file for f_buttons.c

// Global variable declaration
uint16_t initialSysTick = 0;

// Configures and initialises the buttons
void buttonsInit()
{
	// Enable GPIO port G pins for buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  	GPIODirModeSet(GPIO_PORTG_BASE, BUTTON_UP|BUTTON_DOWN|BUTTON_LEFT|BUTTON_RIGHT|BUTTON_SELECT, GPIO_DIR_MODE_IN);
  	// Configure button pins
  	GPIOPadConfigSet(GPIO_PORTG_BASE, BUTTON_UP|BUTTON_DOWN|BUTTON_LEFT|BUTTON_RIGHT|BUTTON_SELECT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

// Registers any push button events and returns a corresponding
// integer value that determines the operation mode
// Also debounces the buttons by employing the SysTick timer and setting
// a button input as high only if the button signal has remained high for
// 10 ms, otherwise remains the same
uint16_t getButtonEvent(uint16_t oldMode)
{
	uint16_t newMode = 6;
	initialSysTick = getSysTickCounter();
	if (initialSysTick > MAX_SYS_TICK)
	{
		initialSysTick -= MAX_SYS_TICK;
	}
	if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_UP)==0)
	{
		newMode = 1;
		while (getSysTickCounter() < initialSysTick + HOLD_DEBOUNCE_DELAY)
		{
			if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_UP) != 0)
			{
				return oldMode;
			}
		}
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_DOWN)==0)
	{
		newMode = 2;
		while (getSysTickCounter() < initialSysTick + HOLD_DEBOUNCE_DELAY)
		{
			if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_DOWN) != 0)
			{
				return oldMode;
			}
		}
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_LEFT)==0)
	{
		newMode = 3;
		while (getSysTickCounter() < initialSysTick + HOLD_DEBOUNCE_DELAY)
		{
			if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_LEFT) != 0)
			{
				return oldMode;
			}
		}
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_RIGHT)==0)
	{
		newMode = 4;
		while (getSysTickCounter() < initialSysTick + HOLD_DEBOUNCE_DELAY)
		{
			if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_RIGHT) != 0)
			{
				return oldMode;
			}
		}
	}
	else if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_SELECT)==0)
	{
		newMode = 5;
		while (getSysTickCounter() < initialSysTick + HOLD_DEBOUNCE_DELAY)
		{
			if (GPIOPinRead(GPIO_PORTG_BASE, BUTTON_SELECT) != 0)
			{
				return oldMode;
			}
		}
	}
	if (newMode == 6)
	{
		return oldMode;
	}
	else
	{
		if (oldMode != newMode)
		{
			displayClear();
		}
		return newMode;
	}
}
