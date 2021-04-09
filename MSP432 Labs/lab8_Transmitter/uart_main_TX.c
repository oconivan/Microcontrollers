#include <msp430g2553.h>
//#include “serial_msp.h”
#include "uartMsp.h"

void main(void) {
	WDTCTL = WDTHOLD + WDTPW; // Stop Watchdog timer
	P1DIR |= BIT0 + BIT6; // Configure LED pins as outputs
	P1OUT &= ~(BIT0 + BIT6); // Turn off LEDs
	P2DIR &= ~(BIT0 + BIT1 + BIT2); // Make button pins as inputs
	clock_init(); // Set clock frequency to 1 MHz
	serial_init(); // Initialize UART module
	while(1) {
		if( (P2IN & BIT2) == BIT2) {
			serial_charTX('r');
			P1OUT |= (BIT0 + BIT6); // Turn on BOTH LEDs
			__delay_cycles(1000000);
		}
		if( (P2IN & BIT1) == BIT1) {
			serial_charTX('i');
			P1OUT |= BIT6; // Turn On green LED
			P1OUT &= ~BIT0; // Turn Off red LED
			__delay_cycles(1000000);
		}
		if( (P2IN & BIT0) == BIT0) {
			serial_charTX('d');
			P1OUT |= BIT0; // Turn On red LED
			P1OUT &= ~BIT6; // Turn Off green LED
			__delay_cycles(1000000);
		} else {
			P1OUT &= ~(BIT0 + BIT6); // Turn on LEDs
		}
	}
}
