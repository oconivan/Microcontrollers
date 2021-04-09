#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //set P1.3
    P1DIR &=~ 0x08;
    P1REN |= 0X08;
    P1OUT |= 0X08;

    //Set P1.2
    P1DIR |= 0x04;
    P1SEL |= 0x04;
    P1SEL2 &= ~0x04;

    //Timer
    TA0CTL |= 0x0210;
    TA0CCTL1 |= 0x00C0;
    TA0CCR0 = 1000;
    TA0CCR1 = 100;

    while(1){

    	if(!(P1IN&0x08)){

    		TA0CCR1+=100;

    		if(TA0CCR1==1100){
    			TA0CCR1 =0;
    		}


    		while(!(P1IN&0x08));
    	}

    }

	return 0;
}
