// Author: Vaz
// cam.c for ENCE461 Wacky Racers Project

// Modified from:
/* File:   tcm8230_dump2.c
   Author: M. P. Hayes, UCECE
   Date:   27 Apr 2013
   Descr: 
*/
#include "tcm8230.h"
#include <stdio.h>
#include "sys.h"
#include "pacer.h"
#include "led.h"
#include "usb_cdc.h"
#include "i2c_slave.h"

#define LOOP_POLL_RATE 2000
/* Define LEDs configuration.  */
static const led_cfg_t leds_cfg[] =
{
    {
        .pio = LED0_PIO,
        .active = 0
    },
    {
        .pio = LED1_PIO,
        .active = 0
    }
};

enum {LEDS_NUM = ARRAY_SIZE (leds_cfg)};


static const tcm8230_cfg_t cfg =
{
    .picsize = TCM8230_PICSIZE_SQCIF
};


static uint8_t image[SQCIF_WIDTH * SQCIF_HEIGHT * 2];


static void image_dump (uint8_t *image, unsigned int rows, unsigned int cols)
{
    unsigned int row;
    unsigned int col;
    
    for (row = 0; row < rows; row++)
    {
        printf ("%3d:", row);
        for (col = 0; col < cols * 2; col++)
            printf ("%3d,", image[row * cols * 2 + col]);
        printf ("\r\n");
    }
}

/* I2C slave config */
#define SDA_PIO PIO_DEFINE(PORT_A, 0)
#define SCL_PIO PIO_DEFINE(PORT_A, 1)

static const i2c_bus_cfg_t i2c_bus_cfg =
{
    .scl = SCL_PIO,
    .sda = SDA_PIO
};

static const i2c_slave_cfg_t i2c_slave1_cfg =
{
    .id = 0x32,
};

int
main (void)
{
    char txbuffer[64];
    char rxbuffer[64];
    usb_cdc_t usb_cdc;
    led_t leds[LEDS_NUM];
    int i;
    i2c_t i2c_slave1;

	pacer_init (LOOP_POLL_RATE);
	usb_cdc = usb_cdc_init ();
	i2c_slave1 = i2c_slave_init(&i2c_bus_cfg, &i2c_slave1_cfg);
    
    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
    {
		leds[i] = led_init(&leds_cfg[i]);
	}
    led_set (leds[0], 0);
    led_set (leds[1], 0);

    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

	/* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;
    tcm8230_init (&cfg);

    led_set (leds[0], 1);

    while (1)
    {
		i2c_addr_t addr;
        uint8_t cmdBuffer[3];
        i2c_ret_t i2cRet;
        int i2cI;
        int testData[4] = {'W', 'O', 'O', 'T'};
        
        int32_t ret;
        //fprintf(stderr, ".");
		
		i2cRet = i2c_slave_read(i2c_slave1, cmdBuffer, sizeof (cmdBuffer), 50);
        if (i2cRet == I2C_ERROR_TIMEOUT || i2cRet == I2C_ERROR_BUSY)
        {
            continue;
		}
		fprintf(stderr, "i2cRet: %d\n", i2cRet);
		if (i2cRet == 1)
		{
			fprintf(stderr, "should write\n");
            //i2c_slave_write (i2c_slave1, testData, sizeof(testData), 1000);
        }
        else if (i2cRet == 4)
        {
			fprintf(stderr, "packet recv'd\n");
			if (cmdBuffer[1] == 1)
			{
				led_set (leds[1], 1);
				while ( tcm8230_frame_ready_p ())
					continue;

				while (! tcm8230_frame_ready_p ())
					continue;

				ret = 1;//tcm8230_capture (image, sizeof(image), 200);
				led_set (leds[0], 1);

				if (ret < 0)
				{
					fprintf (stderr, "TCM8230 error: %d\r\n", (int)ret);
				}
				else
				{
					fprintf(stderr, "took pic\n");
					//image_dump(image, SQCIF_HEIGHT, SQCIF_WIDTH);
				}
			}
		
			else
			{
				led_set (leds[1], 0);
			}
		}
		usb_cdc_update ();
	}     
}
