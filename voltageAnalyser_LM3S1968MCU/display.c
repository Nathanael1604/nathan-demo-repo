// Author: Nathanael Park and Hayden Becker
// Display Module for ENCE361 Project 1
// Controls the OLED display and all text to be displayed

#include "display.h"

// Global variable declaration
char strBufDisplay[strBufSize];

// Initializes the screen Module
void displayInit()
{
	RIT128x96x4Init(10000000);
}

// Clear the display to get rid of residual information
void displayClear()
{
	RIT128x96x4Clear();
}

// Displays the introduction text
void displayStartup()
{
	RIT128x96x4StringDraw( "Digital RMS Converter", 2, 4, 10);
	RIT128x96x4StringDraw( "User Interface", 23, 12, 10);
	RIT128x96x4StringDraw( "Please use the push", 6, 30, 15);
	RIT128x96x4StringDraw( "buttons to select", 6, 38, 15);
	RIT128x96x4StringDraw( "the desired mode.", 6, 46, 15);
}

// Displays the button menu
void displayMenu()
{
	RIT128x96x4StringDraw( "RMS Voltage -> up", 6, 64, 15);
	RIT128x96x4StringDraw( "P2P Voltage -> down", 6, 72, 15);
	RIT128x96x4StringDraw( "Frequency   -> left", 6, 80, 15);
	RIT128x96x4StringDraw( "MSE *bonus! -> right", 6, 88, 15);
}

// The following set of functions display specific user functions
// such as peak to peak voltage, frequency, RMS voltage and MSE
void displayP2P(uint16_t p2p1, uint16_t p2p2, uint16_t waveFlag1, uint16_t waveFlag2)
{
	uint16_t v1a = 0, v1b = 0, v2a = 0, v2b = 0;
	// Normalizing the voltage value in the range from 0-3 volts and sort to 2 d.p.
	v1a = (p2p1*3)/1023;
	v1b = ((p2p1*300)/1023) - (v1a*100);
	v2a = (p2p2*3)/1023;
	v2b = ((p2p2*300)/1023) - (v2a*100);

	// Displaying the normalized values
	RIT128x96x4StringDraw( "Pk-to-Pk Voltage [1]", 6, 4, 15);
	// Buffers values < 10 with an extra 0 to display correctly
	if (v1b < 10)
	{
		usprintf(strBufDisplay, "is: %d.0%d V",v1a,v1b);
	}
	else
	{
		usprintf(strBufDisplay, "is: %d.%d V",v1a,v1b);
	}
	RIT128x96x4StringDraw(strBufDisplay, 30, 12, 15);
	RIT128x96x4StringDraw( "Pk-to-Pk Voltage [2]", 6, 34, 15);
	// Buffers values < 10 with an extra 0 to display correctly
	if (v2b < 10)
	{
		usprintf(strBufDisplay, "is: %d.0%d V",v2a,v2b);
	}
	else
	{
		usprintf(strBufDisplay, "is: %d.%d V",v2a,v2b);
	}
	RIT128x96x4StringDraw(strBufDisplay, 30, 42, 15);
	displayWave(waveFlag1, waveFlag2);
}

void displayFrequency(uint16_t freq1, uint16_t freq2, uint16_t waveFlag1, uint16_t waveFlag2)
{
	RIT128x96x4StringDraw( "The Frequency [1]", 6, 4, 15);
	usprintf(strBufDisplay, "is: %d Hz", freq1);
	RIT128x96x4StringDraw(strBufDisplay, 30, 12, 15);
	RIT128x96x4StringDraw( "The Frequency [2]", 6, 34, 15);
	usprintf(strBufDisplay, "is: %d Hz", freq2);
	RIT128x96x4StringDraw(strBufDisplay, 30, 42, 15);
	displayWave(waveFlag1, waveFlag2);
}

void displayVrms(uint16_t vrms1, uint16_t vrms2, uint16_t waveFlag1, uint16_t waveFlag2)
{
	uint16_t v1a = 0, v1b = 0, v2a = 0, v2b = 0;
	// Normalizing the voltage value in the range from 0-3 volts and sort to 2 d.p.
	v1a = (vrms1*3)/1023;
	v1b = ((vrms1*300)/1023) - (v1a*100);
	v2a = (vrms2*3)/1023;
	v2b = ((vrms2*300)/1023) - (v2a*100);
	// Displaying the desired values
	RIT128x96x4StringDraw( "True RMS Voltage [1]", 6, 4, 15);
	// Buffers values < 10 with an extra 0 to display correctly
	if (v1b < 10) {
		usprintf(strBufDisplay, "is: %d.0%d V", v1a, v1b);
	}
	else {
		usprintf(strBufDisplay, "is: %d.%d V", v1a, v1b);
	}
	RIT128x96x4StringDraw(strBufDisplay, 30, 12, 15);
	RIT128x96x4StringDraw( "True RMS Voltage [2]", 6, 34, 15);
	// Buffers values < 10 with an extra 0 to display correctly
	if (v2b < 10) {
		usprintf(strBufDisplay, "is: %d.0%d V", v2a, v2b);
	}
	else {
		usprintf(strBufDisplay, "is: %d.%d V", v2a, v2b);
	}
	RIT128x96x4StringDraw(strBufDisplay, 30, 42, 15);
	displayWave(waveFlag1, waveFlag2);
}

void displayMSE(uint16_t MSEVal, uint16_t waveFlag1, uint16_t waveFlag2)
{
	RIT128x96x4StringDraw( "The Mean Square", 20, 20, 15);
	usprintf(strBufDisplay, "Error is: 0.%d", MSEVal/100);
	RIT128x96x4StringDraw(strBufDisplay, 20, 28, 15);
}

void displayWave(uint16_t waveFlag1, uint16_t waveFlag2)
{
	if (waveFlag1 == SINE_WAVE) {
		RIT128x96x4StringDraw("Sine wave", 6, 20, 15);
	}
	else if (waveFlag1 == TRI_WAVE) {
		RIT128x96x4StringDraw("Triangular wave", 6, 20, 15);
	}
	else if (waveFlag1 == SQ_WAVE) {
		RIT128x96x4StringDraw("Square wave", 6, 20, 15);
	}
	else if (waveFlag1 == DC_WAVE) {
		RIT128x96x4StringDraw("DC wave", 6, 20, 15);
	}
	else if (waveFlag1 == UNKNOWN_WAVE) {
		RIT128x96x4StringDraw("Unknown wave type", 6, 20, 15);
	}
	else {
		RIT128x96x4StringDraw("Error", 6, 20, 15);
	}

	if (waveFlag2 == SINE_WAVE) {
		RIT128x96x4StringDraw("Sine wave", 6, 50, 15);
	}
	else if (waveFlag2 == TRI_WAVE) {
		RIT128x96x4StringDraw("Triangular wave", 6, 50, 15);
	}
	else if (waveFlag2 == SQ_WAVE) {
		RIT128x96x4StringDraw("Square wave", 6, 50, 15);
	}
	else if (waveFlag2 == DC_WAVE) {
		RIT128x96x4StringDraw("DC wave", 6, 50, 15);
	}
	else if (waveFlag2 == UNKNOWN_WAVE) {
		RIT128x96x4StringDraw("Unknown wave type", 6, 50, 15);
	}
	else {
		RIT128x96x4StringDraw("Error", 6, 50, 15);
	}
}
