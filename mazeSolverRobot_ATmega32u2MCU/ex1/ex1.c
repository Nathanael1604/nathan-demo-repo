// Author: Nathanael Park
// f_main for ENEL300 Project 2 (Group 6)

/* Told by project specs that modularising code not necessary, so
that printing in Report Appendix is easier*/

// Includes
#include <avr/io.h>
#include "system.h"
#include "pio.h"
#include <util/delay.h>

// ADC pin defintions
#ifndef ADC_CHARGE_PIO
#define ADC_CHARGE_PIO PIO_DEFINE(PORT_D, 3)
#endif

// Constant definitions
#ifndef ADC_CAPACITANCE
#define ADC_CAPACITANCE 9.4e-9
#endif

#define WHITE_THRESHOLD 3
 
#ifndef ADC_RESISTANCE
#define ADC_RESISTANCE 27e3
#endif
 
#define ADC_TIME_CONSTANT_US (ADC_CAPACITANCE * ADC_RESISTANCE * 1e6)


/* Analog Comparator Input Multiplexer Register and associated bit number definitions. */
/* This is not defined in io32u2.h  */
#ifndef ACMUX
#define ACMUX _SFR_MEM8(0x7D)
#endif

// Analog comparator selection bits
CMUX0 0
CMUX1 1
CMUX2 2

// Analog input channel selections
#define AIN1 CMUX0
#define AIN3 CMUX1
#define AIN5 CMUX3

// Sensor analog channel definitions
#define SENSOR_1 3
#define SENSOR_2 1
#define SENSOR_3 5


/** Define the two PWM channels, channel 1 is on PB1 and channel 2 is on
    PB2 */

// PWM channels for signalling duty cycles to the left and right motors
typedef enum {PWM_RIGHT_MOTOR, PWM_LEFT_MOTOR} pwmChannel_t;


// Initialising the Atmega32u2's in-built comparator to achieve a makeshift ADC 
void adc_init(void)
{
    // Configuring the capacitor charging pin to PD3
    pio_config_set(ADC_CHARGE_PIO, PIO_OUTPUT_LOW);
 
    /* Disable digital input for AIN0.  */
    DIDR1 |= BIT(0);
 
    /* By default the analog comparator is enabled but let's enable it.  */
    ACSR &= BIT(ACD);
}

void adc_enable(uint8_t adc_channel)
{
   /* Disable digital input for selected channel.  */
    DIDR1 |= BIT(adc_channel);
}
 
 
void adc_disable(uint8_t adc_channel)
{
   /* Enable digital input for selected channel.  */
    DIDR1 &= ~BIT(adc_channel);
}
 
 // Acquire reading from makeshift ADC
 uint8_t adc_measure(uint8_t adc_channel)
{
    uint8_t count;
 
    count = 0;
 
    /* Select desired channel.  */
    ACMUX = adc_channel - 1;
    
    // Charge the capacitor
    pio_output_high (ADC_CHARGE_PIO);
    
    // Charge capacitor and count until the output from the comparator goes high i.e. the unknown voltage has been measured
    while (! (ACSR & BIT(ACO)))
        count++;
 
    // Discharge capacitor for next ADC measurement
    pio_output_low (ADC_CHARGE_PIO);    
    
    // Return count for 6-bit resolution ADC reading 
    return count;
}

// Functions to control motor pins
static inline void RIGHT_MOTOR_FORWARD(void)
{
    PORTB |= BIT(PB2); 
    PORTB &= ~BIT(PB4);
}

static inline void RIGHT_MOTOR_REVERSE(void)
{
    PORTB |= BIT(PB4); 
    PORTB &= ~BIT(PB2);
}

static inline void RIGHT_MOTOR_STOP(void)
{
    PORTB |= BIT(PB4); 
    PORTB |= BIT(PB2);
}


static inline void LEFT_MOTOR_FORWARD(void)
{ 
    PORTB &= ~BIT(PB3);
    PORTB |= BIT(PB6);
}


static inline void LEFT_MOTOR_REVERSE(void)
{ 
    PORTB |= BIT(PB3);
    PORTB &= ~BIT(PB6);
}


static inline void LEFT_MOTOR_STOP(void)
{ 
    PORTB |= BIT(PB6);
    PORTB |= BIT(PB3);
}

// Stop both motors
void motors_stop(void)
{
    RIGHT_MOTOR_STOP();
    LEFT_MOTOR_STOP();
}

/** Sets the duty cycle for a given PWM output. Duty cycles are an interger
 *  (between 0 - 100) to represent a percentage duty cycle */
// Set duty cycle for PWM output to motors, value is between 0-255 correlating to 0%- 100% 
void pwm_setDuty(pwmChannel_t channel, uint8_t duty)
{
    // Set the appropriate comparator value for the given channel
    switch(channel)
    {
        case PWM_RIGHT_MOTOR:
            //Set the Output Compare Register A for Timer0 to set right motor duty cycle
            OCR0A = duty;
            break;
        case PWM_LEFT_MOTOR:
            //Set the Output Compare Register B for Timer0 to set left motor duty cycle
            OCR0B = duty;
            break;
    }
}

void motors_init(void)
{
    /* Set control pins as outputs to the motors*/
    DDRB |= BIT(PB2) | BIT (PB3) | BIT(PB4) | BIT(PB6);

    /* Initalise the motors to be off */
    pwm_setDuty(PWM_RIGHT_MOTOR, 0); // Right Wheel
    pwm_setDuty(PWM_LEFT_MOTOR, 0);  // Left Wheel
    
    // Initially no motor motion
    RIGHT_MOTOR_STOP();
    LEFT_MOTOR_STOP();
}

void timerInit (void) {
    
    // PB7 and PD0 as outputs since shared with OCR1A and OCR1B
    DDRB |= BIT(PB7); // Right Motor PWM
    DDRD |= BIT(PD0); // Left Motor PWM
    //pio_config_set (HEADER_PC6_PIO, PIO_OUTPUT_HIGH);
    //pio_config_set (HEADER_PC5_PIO, PIO_OUTPUT_HIGH);
    
    // Set wave generation mode for 8 bit phase correct
    TCCR0A |= BIT(WGM00);
    
    // Set non-inverting mode (High at bottom, Low on Match)
    TCCR0A |= BIT(COM0A1) | BIT(COM0B1);
    // Set clock timer prescaler to 8 
    TCCR0B |= BIT(CS01);
}

/*
void adc_test(void) {
        counter = adc_measure(SENSOR_1);
        _delay_ms(1);
        counter3 = adc_measure(SENSOR_2);
        _delay_ms(1);
        counter3 = adc_measure(SENSOR_3);
        _delay_ms(1);
        if (counter >= 3) {
            RIGHT_MOTOR_FORWARD();
            _delay_ms(200);
        }
        else if (counter < 3) {
           RIGHT_MOTOR_REVERSE();
            _delay_ms(200);
        }
        RIGHT_MOTOR_STOP();
        _delay_ms(100);
        
        
        if (counter3 >= 3) {
            LEFT_MOTOR_FORWARD();
            _delay_ms(200);
        }
        else if (counter3 < 3) {
            LEFT_MOTOR_REVERSE();
            _delay_ms(200);
        }
        LEFT_MOTOR_STOP();
        _delay_ms(100);
}*/

// Turn robot hard left
void motor_hard_left(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 180);
    pwm_setDuty(PWM_RIGHT_MOTOR, 120);
    LEFT_MOTOR_REVERSE();
    RIGHT_MOTOR_FORWARD();
}

// Turn robot hard right
void motor_hard_right(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 180);
    pwm_setDuty(PWM_RIGHT_MOTOR, 120);
    LEFT_MOTOR_FORWARD();
    RIGHT_MOTOR_REVERSE();
}

// Move robot forward
void motor_forward(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 180);
    pwm_setDuty(PWM_RIGHT_MOTOR, 120);
    LEFT_MOTOR_FORWARD();
    RIGHT_MOTOR_FORWARD();
}

// Move robot backward
void motor_reverse(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 160);
    pwm_setDuty(PWM_RIGHT_MOTOR, 100);
    LEFT_MOTOR_REVERSE();
    RIGHT_MOTOR_REVERSE();
}

// Move robot right
void motor_right(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 210);
    pwm_setDuty(PWM_RIGHT_MOTOR, 105);
    LEFT_MOTOR_FORWARD();
    RIGHT_MOTOR_FORWARD();
}

// Move robot left
void motor_left(void) {
    pwm_setDuty(PWM_LEFT_MOTOR, 120);
    pwm_setDuty(PWM_RIGHT_MOTOR, 100);
    LEFT_MOTOR_FORWARD();
    RIGHT_MOTOR_FORWARD();
}

int main(void)
{
    uint8_t w_count = 0; // white counter to monitor how long the robot has not read black (i.e. has been off the line)
    bool power_flag = 0; // flag to allow a hard right turn
    
    // Initiliasing functions
    adc_init();
    timerInit();
    motors_init();
    //pwm_setDuty(PWM_LEFT_MOTOR, 100);
    //pwm_setDuty(PWM_RIGHT_MOTOR, 100);
    
    // Run while the grey finish line has not been encountered
    while (1) 
    {
        uint8_t counter = 0; // ADC reading from sensor 1
        uint8_t counter2 = 0; // ADC reading from sensor 2
        uint8_t counter3 = 0; // ADC reading from sensor 3
        
        // Measure and acquire ADC readings from each sensor channel
        counter = adc_measure(SENSOR_1);
        _delay_ms(1); // Delay for a short time between channel readings to allow for capacitor discharge
        counter2 = adc_measure(SENSOR_2);
        _delay_ms(1);
        counter3 = adc_measure(SENSOR_3);
        _delay_ms(1);
        
        // case 3: Right and middle sensors black. Move Forward
        if ((counter >= 3) && (counter2 >= 3) && (counter3 < 3)) {
            motor_forward();    
            _delay_ms(8);
            w_count = 0;
            power_flag = 0;
        }
        // case 0: No sensors black, all sensors white. Turn Right
        else if ((counter < 3) && (counter2 < 3) && (counter3 < 3)) {
            if (w_count < 4) {            
                motor_right();
                _delay_ms(6);
                w_count++;
            }
            // If all sensors have read white for 4 counts, then turn hard right to try to get back on the black line
            else if ((w_count >= 4) && (power_flag == 0)) {
                motor_hard_right();
                _delay_ms(10);
                w_count = 0; // set white counter back to zero after a hard right turn to allow the robot to try and get back to the line 
                             // using mild right turns for a while before trying another sharp right hard
                power_flag = 1; // set power flag high which allows a more extreme action next time if all sensors continue to read white
            }
            // If all sensors are still reading white after hard right turn, then the robot is either navigating a dead-end or
            // is way off the line. Perform a manuever involving a hard reverse and a very sharp right turn to back up and get back on line
            else if ((w_count >= 4) && (power_flag == 1)) {
                motor_reverse();
                _delay_ms(16);
                motor_hard_right();
                _delay_ms(14); // longer delay than before for an even sharper right turn
                w_count = 0; // reset white counter
                power_flag = 0; // reset power flag
            }
        }
        // case 4
        else if ((counter < 3) && (counter2 < 3) && (counter3 >= 3)) {
            motor_hard_left();
            _delay_ms(16);
            w_count = 0;
            power_flag = 0;
        }
        // case 7
        else if ((counter >= 3) && (counter2 >= 3) && (counter3 >= 3)) {
            motor_hard_left();
            _delay_ms(10);
            w_count = 0;
            power_flag = 0;
        }
        // case 1
        else if ((counter >= 3) && (counter2 < 3) && (counter3 < 3)) {
            motor_hard_left();
            _delay_ms(8);
            w_count = 0;
            power_flag = 0;
        }
        // case 5
        else if ((counter >= 3) && (counter2 < 3) && (counter3 >= 3)) {
            motor_hard_left();
            _delay_ms(13);
            w_count = 0;
            power_flag = 0;
        }
        // case 2
        else if ((counter < 3) && (counter2 >= 3) && (counter3 < 3)) {
            motor_left();
            _delay_ms(10);
            w_count = 0;
            power_flag = 0;
        }
        // case 6
        else if ((counter < 3) && (counter2 >= 3) && (counter3 >= 3)) {
            motor_forward();
            _delay_ms(8);
            w_count = 0;
            power_flag = 0;
        }
        // case default
        else { 
            motors_stop();
            _delay_ms(50);
            w_count = 0;
            power_flag = 0;
        }
        // Delay for a short time so that the robot does not move continuously but instead in short intervals, allowing for better control 
        // of robot movement, faster response and easier aquiring of feedback from sensors, and to draw less continuous current from the motors
        motors_stop();
        _delay_ms(15);
        
    }
}
