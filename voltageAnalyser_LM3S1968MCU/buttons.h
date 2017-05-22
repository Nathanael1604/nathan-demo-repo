// Author: Nathanael Park and Hayden Becker
// Buttons Header for ENCE361 Project 1

#ifndef BUTTONS_H_
#define BUTTONS_H_

// Includes
#include "my_adc.h"
#include "waveform.h"

// Function prototypes
void buttonsInit();
uint16_t getButtonEvent(uint16_t);

// Button values specifiers
#define BUTTON_UP GPIO_PIN_3  
#define BUTTON_DOWN GPIO_PIN_4  
#define BUTTON_LEFT GPIO_PIN_5  
#define BUTTON_RIGHT GPIO_PIN_6  

#endif
