// Author: Nathanael Park
// main.c for ENCE461 Wacky Racers Project

/* NB from NathanP: Ideally would abstract and modularise code a lot more (amongst many other things), 
but not necessarily a priority Project Requirements specified that code will not be marked for this project*/

#include "pacer.h"
//#include "uart.h"
#include "pio.h"
#include "delay.h"
#include "i2c_master.h"

#include <string.h>


#define SDA_PIO PIO_DEFINE(PORT_A, 3)
#define SCL_PIO PIO_DEFINE(PORT_A, 4)

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 2000};

/* Define LED flash rate and BT update rate in Hz.  */
enum 
{
	IR_POLL_RATE = 1000,
	ADC_READ_RATE = 10,
	SPEED_POLL_RATE = 1,
	VOLTAGE_POLL_RATE = 1,
	I2C_RATE = 100
};

static const i2c_bus_cfg_t i2c_bus_cfg =
{
    .scl = SCL_PIO,
    .sda = SDA_PIO
};


static const i2c_slave_cfg_t i2c_slave1_cfg =
{
    .id = 0x32,
};


#define PACER_RATE 10
#define LINE_BUFFER_SIZE 16

#define BR    115200                        /* Baud Rate */

#define BRD  (MCK/16/BR)                    /* Baud Rate Divisor */

#define US_RXD_PIN		AT91C_PA21_RXD1    /* JP9 must be close */
#define US_TXD_PIN		AT91C_PA22_TXD1    /* JP7 must be close */
#define US_RTS_PIN		AT91C_PA24_RTS1    /* JP8 must be close */
#define US_CTS_PIN		AT91C_PA25_CTS1    /* JP6 must be close */ 

#define EXT_OC          18432000   // Exetrnal ocilator MAINCK
#define MCK             48054857   // MCK (PLLRC div by 2)
#define MCKKHz          (MCK/1000) //

//AT91S_USART * pUSART = AT91C_BASE_US0;      /* Global Pointer to USART0 */
AT91S_USART * pUSART = AT91C_BASE_US1;

void uart0_init (void) {                   /* Initialize Serial Interface */

  /* mt: n.b: uart0 clock must be enabled to use it */
  
  // NB from NathanP: Should probably disable interrupts? and pullups?
  
  /* Enable RxD0 and TxD0 pins.  */
  //*AT91C_PIOA_PDR = AT91C_PA5_RXD0 |        /* Enable RxD0 Pin */
  //                 AT91C_PA6_TXD0;         /* Enalbe TxD0 Pin */
  *AT91C_PIOA_PDR = AT91C_PA21_RXD1 |        /* Enable RxD0 Pin */
                    AT91C_PA22_TXD1;         /* Enalbe TxD0 Pin */

  /* Reset and disable receiver and transmitter.  */
  pUSART->US_CR = AT91C_US_RSTRX |          /* Reset Receiver      */
                  AT91C_US_RSTTX |          /* Reset Transmitter   */
                  AT91C_US_RXDIS |          /* Receiver Disable    */
                  AT91C_US_TXDIS;           /* Transmitter Disable */

  /* Set normal mode, clock = MCK, 8-bit data, no parity, 1 stop bit.  */
  pUSART->US_MR = AT91C_US_USMODE_NORMAL |  /* Normal Mode */
                  AT91C_US_CLKS_CLOCK    |  /* Clock = MCK */
                  AT91C_US_CHRL_8_BITS   |  /* 8-bit Data  */
                  AT91C_US_PAR_NONE      |  /* No Parity   */
                  AT91C_US_NBSTOP_1_BIT;    /* 1 Stop Bit  */

  pUSART->US_BRGR = BRD;                    /* Baud Rate Divisor */

  /* Enable receiver and transmitter.  */
  pUSART->US_CR = AT91C_US_RXEN  |          /* Receiver Enable     */
                  AT91C_US_TXEN;            /* Transmitter Enable  */
  
  /* NB from NathanP: Should maybe pass Baud Rate Divisor as an input variable
  and return 1 (success) or similar*/
}

__inline void AT91F_PMC_EnablePeriphClock (
	AT91PS_PMC pPMC, // \arg pointer to PMC controller
	unsigned int periphIds)  // \arg IDs of peripherals
{
	pPMC->PMC_PCER = periphIds;
}

/* NB from NathanP: Ideally would abstract and modularise UART code a lot more, but not necessarily
a priority Project Requirements specified that code will not be marked for ENCE461*/

int uart0_putc(int ch) 
{
	while (!(pUSART->US_CSR & AT91C_US_TXRDY));   /* Wait for Empty Tx Buffer */
	return (pUSART->US_THR = ch);                 /* Transmit Character */
}	

int uart0_putchar (int ch) {                      /* Write Character to Serial Port */

  if (ch == '\n')  {                            /* Check for LF */
    uart0_putc( '\r' );                         /* Output CR */
  }
  return uart0_putc( ch );                     /* Transmit Character */
}

int uart0_puts ( char* s )
{
	while ( *s ) uart0_putchar( *s++ );
	return 0;
}

int uart0_kbhit( void ) /* returns true if character in receive buffer */
{
	if ( pUSART->US_CSR & AT91C_US_RXRDY) {
		return 1;
	}
	else {
		return 0;
	}
}

int uart0_getc ( void )  /* Read Character from Serial Port */
{    

  while (!(pUSART->US_CSR & AT91C_US_RXRDY));   /* Wait for Full Rx Buffer */
  return (pUSART->US_RHR);                      /* Read Character */
}


int main(void) 
{
	i2c_t i2c_slave1;
	
	
    i2c_slave1 = i2c_master_init (&i2c_bus_cfg, &i2c_slave1_cfg);
    
	uart0_init();
	char ch;
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, ( 1 << AT91C_ID_PIOA ) | ( 1 << AT91C_ID_US1 ) ) ;
	
	pacer_init(PACER_RATE);
	
	
	//uart0_puts("\nfdsjfahkjdsahfkjdshafklj\n");
	
	pio_config_set(LED4_PIO, PIO_OUTPUT_LOW);
	pio_config_set(LED5_PIO, PIO_OUTPUT_LOW);
	pio_config_set(LED6_PIO, PIO_OUTPUT_LOW);
	
		
	while (1)
	{
		pacer_wait ();
		uint8_t tx[2] = {0,0};
		uint8_t rx[4] = {0};
		
		if (uart0_kbhit())
		{
			ch = uart0_getc();
			// NB from NathanP: Should probably change to switch-case statements eventually, but not critical for project needs
			if (ch == 'F')
			{
				uart0_puts("\nForward\n");
				tx[0] = 1;
			}
			else if (ch == 'B')
			{
				uart0_puts("\nBackward\n");
				tx[0] = 2;
			}
			else if (ch == 'L')
			{
				uart0_puts("\nLeft\n");
				tx[0] = 3;
			}
			else if (ch == 'R')
			{
				uart0_puts("\nRight\n");
				tx[0] = 4;
			}
			else if (ch == 'S')
			{
				uart0_puts("\nSquare\n");
				tx[0] = 5;
			}
			else if (ch == 'D')
			{
				uart0_puts("\nTriangle\n");
				tx[0] = 6;
			}
			else if (ch == 'O')
			{
				uart0_puts("\nCircle\n");
				tx[0] = 7;
			}
			else if (ch == 'X')
			{
				uart0_puts("\nCross\n");
				tx[0] = 8;
			}
			else
			{
				uart0_puts("\nWhat did you press?\n");
				tx[0] = 9;
			}
			
		}
		

        i2c_addr_t addr = 1;
        
        i2c_master_addr_write (i2c_slave1, addr, 1, tx, sizeof(tx));
        
        //i2c_master_addr_read(i2c_slave1, addr, 1, rx, sizeof(rx));
        
        //~ if (rx[0] == 0)
        //~ {
			//~ pio_config_set(LED4_PIO, PIO_OUTPUT_HIGH);
		//~ }
		//~ if (rx[1] == 0)
		//~ {
			//~ pio_config_set(LED5_PIO, PIO_OUTPUT_HIGH);
		//~ }
		//~ if (rx[2] == 0)
		//~ {
			//~ pio_config_set(LED6_PIO, PIO_OUTPUT_HIGH);
		//~ }
		//~ if (rx[3] == 0)
		//~ {
			//~ pio_config_set(LED4_PIO, PIO_OUTPUT_HIGH);
			//~ pio_config_set(LED5_PIO, PIO_OUTPUT_HIGH);
		//~ }
		//~ if (rx[2] == 1)
		//~ {
			//~ pio_config_set(LED5_PIO, PIO_OUTPUT_HIGH);
			//~ pio_config_set(LED6_PIO, PIO_OUTPUT_HIGH);
		//~ }
		//~ else
		//~ {
			//~ pio_config_set(LED4_PIO, PIO_OUTPUT_HIGH);
			//~ pio_config_set(LED5_PIO, PIO_OUTPUT_HIGH);
			//~ pio_config_set(LED6_PIO, PIO_OUTPUT_HIGH);
		//~ }
		
	}
			
	
		
		
	
}
