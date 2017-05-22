/*motorLED.c*/

#include "motorLED.h"

void LEDconfig(void)
{
	pio_config_set (LED_1, PIO_OUTPUT_LOW);
    pio_config_set (LED_2, PIO_OUTPUT_LOW);
    pio_config_set (LED_3, PIO_OUTPUT_LOW);
}

void LEDset(bool on)
{
	if (on == true)
	{
		pio_output_high (LED_1); 
		pio_output_high (LED_2);
		pio_output_high (LED_3);
	}
	else
	{
		pio_output_low (LED_1); 
		pio_output_low (LED_2);
		pio_output_low (LED_3);
	}
}
