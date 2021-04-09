#include <msp430g2553.h>
#include "uartMsp.h"
void main(void) {
	WDTCTL = WDTHOLD + WDTPW; // Stop Watchdog timer
	P1DIR |= BIT0 + BIT6; // Configure LED pins as outputs
	P1OUT &= ~(BIT0 + BIT6); // Turn off LEDs
	clock_init(); // Set clock frequency to 1 MHz
	serial_init_inter(); // Initialize UART module
	IFG2 &= ~(UCA0RXIFG);
	IE2 |= UCA0RXIE; // Enable UART interrupt
	__bis_SR_register(GIE);
	while(1) { }
}
#pragma vector = USCIAB0RX_VECTOR //interrupt vector routine
__interrupt void RX_Function(void) {
	char c;
	while( !(IFG2 & UCA0RXIFG) ); // This should make the CPU wait since the flag should be set already
	c = UCA0RXBUF; // Store received character
	switch(c) {
	case 'r': // Reset the counter
	P1OUT |= (BIT0 + BIT6); // Turn off LEDs
	break;
	case 'i':
	P1OUT |= BIT6; // Turn On green LED
	P1OUT &= ~BIT0; // Turn Off red LED
	break;
	case 'd':
	P1OUT |= BIT0; // Turn On red LED
	P1OUT &= ~BIT6; // Turn Off green LED
	break;
	default:
		P1OUT &= ~(BIT0 + BIT6); // Turn off LEDs
		break;
	}
}
