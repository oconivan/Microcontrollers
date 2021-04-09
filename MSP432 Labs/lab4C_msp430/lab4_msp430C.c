#include <msp430.h> 
#define BUTTON 0x08

/*
 * main.c
 */
int main(void)
{

  P1IE |= 0x08;         // P1.3 interrupt enabled
  P1IFG &= ~BUTTON;       // P1.3 IFG cleared
  __enable_interrupt();   // intrinsic function


  WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
  P1DIR |= 0x41;         // Set P1.0 and P1.6 to outputs
  P1REN = 0x08;         // Turn on pull up resistor to push button
  P1OUT = 0x49;// Drive high voltages to both LEDs

  while (1)// endless while loop - condition is always true (one)
  {
  volatile unsigned int i;       // volatile to prevent optimization


  for(i=0;i<20000;i++) {}  // empty for loop for delay - change number

  }

}
  // Port 1 interrupt service routine
  #pragma vector=PORT1_VECTOR
  __interrupt void Port_1(void)
  {
  // Add INTERRUPT SERVICE ROUTINE HERE.
  P1OUT ^= 0x41;                  // Toggle P1.0 and P1.6 using exclusive-OR
  P1IFG &= ~BUTTON; // P1.3 IFG cleared
  }

