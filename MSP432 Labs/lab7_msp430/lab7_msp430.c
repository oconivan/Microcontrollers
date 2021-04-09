#include <msp430.h> 
#include "adcMsp.h"
#include "lcdLib.h"
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	lcdInit();// Initialize LCD

	adc_init();

	int x;
	int y;
	int z;
	while(1){
		lcdClear();
		x = adc_measure(5);
		y = adc_measure(6);
		z = adc_measure(7);

		lcdSetText("x", 0, 1);
		lcdSetInt(x, 0, 0);
		lcdSetText("y", 5, 1);
		lcdSetInt(y, 5, 0);
		lcdSetText("z", 10, 1);
		lcdSetInt(z, 10, 0);
		delay_ms(500); //delay

	}
}
