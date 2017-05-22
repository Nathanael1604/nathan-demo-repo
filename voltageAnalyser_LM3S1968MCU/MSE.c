// Author: Nathanael Park and Hayden Becker
// MSE Module for ENCE361 Project 1
// Calculates the MSE (mean square error) for two given waves

#include "MSE.h"

uint16_t getMSE (uint16_t *wave1, uint16_t *wave2) {
	uint32_t total = 0;
	uint16_t i = 1;
	for (i = 1; i <= DATA_BUFF_SIZE; i++ ) {
		//Multiply by 3000/1023 to normalize values for 0-3V
		total += ((wave1[i]-wave2[i])*3000/1023)*((wave1[i]-wave2[i])*3000/1023);
	}
	return total/DATA_BUFF_SIZE;
}
