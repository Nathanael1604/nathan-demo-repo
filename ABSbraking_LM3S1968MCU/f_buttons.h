// Author: Nathanael Park and Hayden Becker
// Buttons Header for ENCE361 Project 2

#ifndef BUTTONS_H_
#define BUTTONS_H_

// Includes
#include "f_main.h"

// Function prototypes
void buttonsInit(void);
uint16_t getButtonEvent(uint16_t);

// Constant definitions
#define BUTTON_UP GPIO_PIN_3
#define BUTTON_DOWN GPIO_PIN_4
#define BUTTON_LEFT GPIO_PIN_5
#define BUTTON_RIGHT GPIO_PIN_6
#define BUTTON_SELECT GPIO_PIN_7
#define HOLD_DEBOUNCE_DELAY 1000 // 10ms
#define MAX_SYS_TICK 18000

#endif
