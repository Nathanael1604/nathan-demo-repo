/*motorLED.h*/

#ifndef MOTORLED_H
#define MOTORLED_H
 
#include <pio.h>
#include "target.h"
//#include "delay.h"

#define LED_1 PIO_DEFINE(PORT_A, 11)
#define LED_2 PIO_DEFINE(PORT_A, 12)
#define LED_3 PIO_DEFINE(PORT_A, 26)

void LEDconfig(void);
void LEDset(bool on);
void LEDreset(void);

#endif
