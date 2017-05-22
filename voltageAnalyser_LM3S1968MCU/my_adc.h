// Author: Nathanael Park and Hayden Becker
// my_adc Header for ENCE361 Project 1

#define uint16_t unsigned short
#define uint32_t unsigned long

// Includes
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "ustdlib.h"
#include "inc/lm3s1968.h"

// Includes for timer
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "stdio.h"

// My modules
#include "buttons.h"
#include "display.h"
#include "frequency.h"
#include "MSE.h"
#include "my_timer.h"
#include "peak_to_peak.h"
#include "Scheduler.h"
#include "VRMS.h"
#include "waveform.h"

// Constant declarations
#define FREQ_SAMPLE 50000
#define DATA_BUFF_SIZE 10000
#define DEFAULT_WAVE 0
#define SINE_WAVE 1
#define TRI_WAVE 2
#define SQ_WAVE 3
#define DC_WAVE 4
#define UNKNOWN_WAVE 5
#define TOLERANCE 20

void adcInit(void);
void ADCISR (void);
