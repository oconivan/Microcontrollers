	/*
	 * main.c
	 */

	#include <msp430g2553.h>

	#include "lcdLib.h"

	#include <stdlib.h>

	#include <stdio.h>

	void main(void)

	{

	WDTCTL = WDTPW + WDTHOLD;// Stop Watchdog

	lcdInit();// Initialize LCD
	int i;
	while(1){
		for(i=0;i<16;i++)
		{
			lcdSetText("IVAN",i,0);
			lcdSetText("OCON",i,1);
			//delay
			delay_ms(400);
			lcdClear();
		}
	}

	//lcdSetText("Go ", 0, 0);

	//lcdSetText("Miners! ", 0,1);

	//__bis_SR_register(LPM0_bits);   // Enter Low Power Mode 0 without interrupts

	}

