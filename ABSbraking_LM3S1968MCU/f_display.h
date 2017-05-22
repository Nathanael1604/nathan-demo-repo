// Author: Nathanael Park and Hayden Becker
// Display Header for ENCE361 Project 2

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Includes
#include "f_main.h"
#include "f_systemABS.h"

// Function prototypes
void displayInit(void);
void displayClear(void);
void displayIntro(void);
void displayMilestone(uint16_t, uint16_t, float*);
void displayExpectedSpeeds(float, float, float*);
void displayBrakes(void);
void displayTest(float*, uint16_t);
void displayMenu(void);
void displayHeader(void);
void displayABSinfo(float, uint16_t, uint16_t, uint16_t, uint16_t);

#endif /*DISPLAY_H_*/
