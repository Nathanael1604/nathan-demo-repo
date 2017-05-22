// Author: Nathanael Park and Hayden Becker
// f_systemABS.h for ENCE361 Project 2
// Controls function in relation to the implementation and monitoring of the ABS

#ifndef F_SYSTEMABS_H_
#define F_SYSTEMABS_H_

// Includes
#include "f_main.h"

// Define Constants
#define AUDIO_FREQUENCY 196 // G one octave below middle

// Function prototypes
void PWMInit(void);
void PWM1IntHandler(void);
void toggleABS(uint16_t);
void toggleAudioLED(uint16_t);
void setAudioVolume(uint16_t);
void LEDInit(void);
uint16_t getAbove10KPH(void);
void triggerABS(void);
uint16_t getABSStatus(void);

#endif /* F_SYSTEMABS_H_ */
