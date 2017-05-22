// Authors: Andrew Errington & Nathanael Park
// motor.c for ENCE461 Wacky Racers Project
// A lot of tidying up could be done, but code is not marked for this project.
// Code functionality most important

#include "motorLED.h"
#include "ir_rc5_rx.h"
#include "pwm.h"

#include "pacer.h"
#include "pio.h"
#include "delay.h"
#include "adc.h"
#include "i2c_slave.h"
#include <string.h>
#include <stdio.h>
#include "target.h"


#define SDA_PIO PIO_DEFINE(PORT_A, 17)
#define SCL_PIO PIO_DEFINE(PORT_A, 18)

static const i2c_bus_cfg_t i2c_bus_cfg =
{
    .scl = SCL_PIO,
    .sda = SDA_PIO
};


static const i2c_slave_cfg_t i2c_slave1_cfg =
{
    .id = 0x32,
};

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 2000};

/* Define LED flash rate and BT update rate in Hz.  */
enum 
{
	IR_POLL_RATE = 100,
	ADC_READ_RATE = 10,
	SPEED_POLL_RATE = 1,
	VOLTAGE_POLL_RATE = 1,
	I2C_RATE = 200
	
};

#define MOTOR_PWM PIO_DEFINE (PORT_A, 0)
#define STEERING_PWM PIO_DEFINE (PORT_A, 1)
#define PWM_FREQ_HZ 50

#define SLIGHT_LEFT 90
#define LEFT 98
#define NEUTRAL 83
#define SLIGHT_RIGHT 76
#define RIGHT 65
//#define FORWARD 76
#define STOP 70
#define BACKWARD 65

static const pwm_cfg_t motor_pwm_cfg =
{
    .pio = MOTOR_PWM,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH
};

static const pwm_cfg_t steering_pwm_cfg =
{
    .pio = STEERING_PWM,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH
};


int main(void) 
{
	uint16_t IR_ticks;
	uint16_t speed_ticks;
	uint16_t voltage_ticks;
	uint16_t adc_read_ticks;
	uint16_t i2c_ticks;
	
	i2c_t i2c_slave1;
	uint8_t send[] = {0,1,2,3};
	//char send[] = {2};
	
	i2c_slave1 = i2c_slave_init (&i2c_bus_cfg, &i2c_slave1_cfg);
	
	int a = 0;
	int speedCount = 0;
	
	unsigned int FORWARD = 76;
	unsigned int turn = NEUTRAL;
	unsigned int dir = STOP;
	
	adc_sample_t voltage = 0;
	adc_sample_t speed = 0;
	
	int speedBuffer[10] = {0};
	
	pwm_t motor;
    pwm_t steering;

    motor = pwm_init (&motor_pwm_cfg);
    steering = pwm_init (&steering_pwm_cfg);
	
	// Configure LED's and set OFF
	LEDconfig();
	LEDset(0);
	
	/* Initialise IR driver.  */
    ir_rc5_rx_init ();
    
    //Enable 3V3 and 5V switched
    pio_config_set (THREE_ENABLE, PIO_OUTPUT_HIGH);
    pio_config_set (FIVE_ENABLE, PIO_OUTPUT_HIGH);
    
    //DEATH off
    pio_config_set (DEATH, PIO_OUTPUT_LOW);
	
	pacer_init (LOOP_POLL_RATE);
	
	IR_ticks = 0;
	speed_ticks = 0;
	voltage_ticks = 0;
	adc_read_ticks = 0;
	i2c_ticks = 0;
	
	bool tri_count = 0;
	bool IR_mode = 1;
	int16_t data, command;
	uint8_t buffer[2] = {0};
	//~ char buffer[2] = {0};
	pwm_duty_fraction_set(steering, NEUTRAL);
	pwm_duty_fraction_set(motor, STOP);
	pwm_start(steering);
	pwm_start(motor);
	
	while(1)
	{
		pacer_wait();
		
		i2c_ticks++;
		
		if (i2c_ticks >= LOOP_POLL_RATE / (I2C_RATE))
		{
			
			i2c_ticks = 0;
			
			i2c_addr_t addr;
			
			buffer[0] = 0;
			buffer[1] = 0;

			i2c_ret_t ret;
			
			ret = i2c_slave_read (i2c_slave1, buffer, sizeof (buffer), 500000);
			
			if (ret == I2C_ERROR_TIMEOUT 
				|| ret == I2C_ERROR_BUSY)
				continue;
				
			//delay_ms(50);
			addr = buffer[0];
			
			//~ if (addr >= sizeof (data))
				//~ addr = 0;
//~ 
			//~ if (ret == 1)
			//~ {
				//~ i2c_slave_write (i2c_slave1, send, sizeof(send), 1000);
			//~ }
			
			
			
			//~ if (buffer[1] == 1)
			//~ {
				//~ turn = NEUTRAL;
			//~ }
			//~ else if (buffer[1] == 2)
			//~ {
				//~ pio_config_set (DEATH, PIO_OUTPUT_LOW); // DOWN (TURN OFF DEATH)
			//~ }
			//~ else if (buffer[1] == 3)
			//~ {
				//~ turn = LEFT;
			//~ }
			//~ else if (buffer[1] == 4)
			//~ {
				//~ turn = RIGHT;
			//~ }
			//~ else if (buffer[1] == 5)	// SQUARE (STOP)
			//~ {
				//~ dir = STOP;
			//~ }
			//~ else if (buffer[1] == 6)	//TRIANGLE (STOP/REVERSE)
			//~ {
				//~ if (count == 0)
				//~ {
					//~ dir = STOP;
					//~ count = 1;
				//~ }
				//~ else
				//~ {
					//~ dir = BACKWARD;
					//~ count = 0;
				//~ }
			//~ }
			//~ else if (buffer[1] == 7)	// CIRCLE (DEATH ON)
			//~ {
				//~ pio_config_set (DEATH, PIO_OUTPUT_HIGH);
			//~ }
			//~ else if (buffer[1] == 8)	// X (FORWARD)
			//~ {
				//~ dir = FORWARD;
			//~ }
			//~ 
			//~ 
			//~ pwm_duty_fraction_set(steering, turn);
			//~ pwm_duty_fraction_set(motor, dir);

			
				
			//~ else if (ret == 5)
				//~ memcpy (send, buffer + 1, sizeof(send));
					
			switch(buffer[1])
			{
				
				case 1: //Steering neutral
				{
					pwm_duty_fraction_set(steering, NEUTRAL);
					break;
				}
				case 2:
				{
					pio_config_set (DEATH, PIO_OUTPUT_LOW); //TURN OFF DEATH
					break;
				}	
				case 3: //Steering left
				{
					pwm_duty_fraction_set(steering, LEFT);
					break;
				}
				case 4: //Steering right
				{
					pwm_duty_fraction_set(steering, RIGHT);
					break;
				}
				case 5: //Stop (SQUARE)
				{
					pwm_duty_fraction_set(motor, STOP);
					break;
				}
				case 6: //Reverse (TRIANGLE)
				{
					if (tri_count == 0)
					{
						pwm_duty_fraction_set(motor, STOP);
						tri_count = 1;
					}
					else
					{
						pwm_duty_fraction_set(motor, BACKWARD);
						tri_count = 0;
					}
					break;
				}
				case 7:
				{
					pio_config_set (DEATH, PIO_OUTPUT_HIGH); //TURN ON DEATH
					break;
				}
					
				case 8: //Forward (Cross)
				{
					pwm_duty_fraction_set(motor, FORWARD);
					break;
				}
				case 9: //Forward 
				{
					pwm_duty_fraction_set(steering, NEUTRAL);
					pwm_duty_fraction_set(motor, STOP);
					break;
				}
			}		
		}
		
		IR_ticks++;
		if (IR_ticks >= LOOP_POLL_RATE / (IR_POLL_RATE))
		{
			IR_ticks = 0;
			
			//~ pio_config_set(LED2_PIO, PIO_OUTPUT_LOW);
			//~ pio_config_set(LED3_PIO, PIO_OUTPUT_HIGH);
		
			/* Poll the IR driver.  */
			data = ir_rc5_rx_read ();
			
			
			if (data != 0)
			{	
				if (data >= 14336) 
				{
					command = data - 14336; // if toggle bit is high
				}
	
				else
				{
					command = data - 12288; // if toggle bit is low
				}
				
				switch(command) 
				{
					
					case 1:
					{
						pwm_duty_fraction_set(steering, SLIGHT_LEFT);
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}
					
					case 2:
					{
						pwm_duty_fraction_set(steering, NEUTRAL);
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}
					
					case 3:
					{
						pwm_duty_fraction_set(steering, SLIGHT_RIGHT);
						pwm_duty_fraction_set(motor, FORWARD);						
						break;
					}
					
					case 4:
					{
						pwm_duty_fraction_set(steering, LEFT);
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}
						
					case 5:
					{
						pwm_duty_fraction_set(motor, STOP);
						FORWARD = 76;
						break;
					}
					
					case 6:
					{
						pwm_duty_fraction_set(steering, RIGHT);
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}
					
					case 7:
					{
						pwm_duty_fraction_set(steering, LEFT);
						pwm_duty_fraction_set(motor, STOP);
						delay_ms(100);
						pwm_duty_fraction_set(motor, BACKWARD);
						break;	
					}
					
					case 8:
					{
						pwm_duty_fraction_set(steering, NEUTRAL);
						pwm_duty_fraction_set(motor, STOP);
						delay_ms(100);
						pwm_duty_fraction_set(motor, BACKWARD);
						break;	
					}
					
					case 9:
					{
						pwm_duty_fraction_set(steering, RIGHT);
						pwm_duty_fraction_set(motor, STOP);
						delay_ms(100);
						pwm_duty_fraction_set(motor, BACKWARD);
						break;	
					}
					
					case 16: // Volume increase
					{
						pio_config_set (DEATH, PIO_OUTPUT_HIGH); //TURN ON DEATH
						LEDset(1);
						break;
					}
					
					case 17: // Volume decrease
					{
						pio_config_set (DEATH, PIO_OUTPUT_LOW); //TURN OFF DEATH
						LEDset(0);
						break;
					}
					
					case 32:
					{
						FORWARD++;
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}
					
					case 33:
					{
						FORWARD--;
						pwm_duty_fraction_set(motor, FORWARD);
						break;
					}	
				}	
			}
		}
		
		voltage_ticks++;
		if (voltage_ticks >= LOOP_POLL_RATE / (VOLTAGE_POLL_RATE))
		{
			//~ pio_config_set(LED3_PIO, PIO_OUTPUT_LOW);
			voltage_ticks = 0;
			
			adc_read_wait(ADC_CHANNEL_4, &voltage);
			
			
			if (voltage < 500)
			{
				pio_config_set(LED3_PIO, PIO_OUTPUT_HIGH);
			}
			else
				pio_config_set(LED3_PIO, PIO_OUTPUT_LOW);
		}
		
		

		//~ speed_ticks++;
		//~ if (speed_ticks >= LOOP_POLL_RATE / (SPEED_POLL_RATE))
		//~ {
			//~ speed_ticks = 0;
			//~ pio_config_set(LED2_PIO, PIO_OUTPUT_HIGH);
			//~ delay_ms(20);
			//~ pio_config_set(LED2_PIO, PIO_OUTPUT_LOW);
			//~ a = 0;
			//~ int b = 0;
			//~ for (b == 0; b == 9; b++) 
			//~ {
				//~ if (&speedBuffer[b] == 1)
				//~ {
					//~ speedCount++;
				//~ }
				//~ 
			//~ } 
			//~ speedCount = 0;
		 //~ }
		
		//~ adc_read_ticks++;
		//~ if (adc_read_ticks >= LOOP_POLL_RATE / (ADC_READ_RATE))
		//~ {
			//~ adc_read_ticks = 0;
			//~ pio_config_set(LED1_PIO, PIO_OUTPUT_HIGH);
			//~ delay_ms(20);
			//~ pio_config_set(LED1_PIO, PIO_OUTPUT_LOW);
			//~ adc_read_wait(ADC_CHANNEL_6, &speed);
			//~ if (speed > 500)
			//~ {
				//~ speedBuffer[a] = 1;
				//~ a++;
			//~ }
		//~ }
	
	}
			
	
}
