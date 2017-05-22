// Author: Nathanael Park and Hayden Becker
// Display Header for ENCE361 Project 1

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Includes
#include "my_adc.h"
#include "Buttons.h"
#include "peak_to_peak.h"
#include "frequency.h"
#include "waveform.h"

// Basic display function prototypes
void displayInit();
void displayMenu();
void displayStartup();
void displayClear();

// Specific display function prototypes
void displayP2P(uint16_t, uint16_t, uint16_t, uint16_t);
void displayFrequency(uint16_t, uint16_t, uint16_t, uint16_t);
void displayVrms(uint16_t,uint16_t, uint16_t, uint16_t);
void displayMSE(uint16_t, uint16_t, uint16_t);
void displayWave(uint16_t, uint16_t);

// Constant declarations
#define strBufSize  40

#endif /*DISPLAY_H_*/
