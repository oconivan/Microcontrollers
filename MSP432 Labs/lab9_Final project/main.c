#include <msp430g2553.h>
#include <stdio.h>
#include "lcdLib.h"
#include <stdlib.h>
#include "uartMsp.h"
#include "adcMsp.h"
//#define B1 BIT3
/*
 * main.c
 */
#define ymax 200
#define ymin 177
#define push 0
#define run 1
#define squats 2
#define menu 3
#define start 4

int pushcnt=0;
int direction;
int state =4;
long int y,i;
int menucnt=0;

volatile int seconds=0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
   //P1DIR |=0X08;   //P1.3 output
   //P1OUT &=~0x08;  // clear p1.3 off
   P1DIR &=~ 0x0F; //set P1.0,1,2,3 as inputs
   P1REN =0x0F;  //P1.0,1,2,3 Resistor enable
   P1OUT |=0x0F;  // P1.0,1,2,3 Enable pull up resistor

   //TIMER CONFIGURATION

   TA0CTL=0X02D2;
   TA0CCR0 =0xFFFF; //(sub timer config) = frequency = input clock (smckl)/ta0ccr0 value      smckl=1 Mhz  TA0CCRO will count 1000 cycles of the smackl and then return to cero if is in up mode
   TA0CTL &=~0x01;  //Clear flag
   TA0CCTL0 &=~0x01; //Clear flag
   TA0CCTL1 &=~0x01;  //Clear flag



    lcdInit();// Initialize LCD

    P2SEL&= ~0x40;		// Configure P2.6,7 as general I/O pins (configured as XIN by defalut) Note see datasheet p.53
       P2SEL2&=~0xC0;	// configure P2.6,7 as general I/O pins (configured as XIN by defalut) Note see datasheet p.53
       P2DIR &=~ 0x40;  //P2.6 (XIN) set as input
       P2REN |=0x40;	//P2.6 (XIN) pull resistor enable
       P2OUT|=0x40;		// P2.6 (XIN) set pull resistor to pull UP
       P2DIR |=0X80;
       P2OUT &=~ 0X80;

    adc_init(); // INITIALIZE ADC
    P1REN &=~0x04;  //P1.0,1,2,3 Resistor enable
    P1OUT &=~0x04;  // P1.0,1,2,3 Enable pull up resistor
    clock_init();
    serial_init(); // Initialize UART module

    //lcdSetText("welcome",3,0);
    	//  	  lcdSetText("Press 1 to Start",0,1);
    state=start;
    	  	 __enable_interrupt();
  while (1)
   	{


   		if ((P1IN&0X01)==0)			//read port1 p1.0 if = 0   state = push (0)			push button p1.0 = push ups mode
   		{
   			pushcnt=0;
   			y=0;
   			lcdClear();
   			state=push;
   			while ((P1IN&0x01)==0){}// Debounce


   		}
   		else if ((P1IN&0x02)==0)		//read port1 p1.1 if ==0   state = run (1)		push button p1.1 = run  mode
   		{
   			pushcnt=0;
			y=0;
   			lcdClear();
   			state=run;
   			while ((P1IN&0x02)==0){} // Debounce

   		}
   		else if ((P2IN&0x40)==0)		// read port2 p2.6(XIN) if ==0  state = squats (2) 	push button p2.6(XIN)= squats mode
   		{
   			pushcnt=0;
		    y=0;
   			lcdClear();
   			state=squats;
   			while ((P2IN&0x40)==0){}
   		}
   		else if ((P1IN&0x08)==0)		// read port1 p1.3 if ==0  state = menu (3)		push button p1.3 = menu mode
   		{
   			lcdClear();
   			state=menu;
   			while ((P1IN&0x08)==0){}
   		}
   		//else if (seconds==5)
   		//{
   			//lcdClear();
   			//state=start;
   			//seconds=0;
   		//}
   	//}

   switch (state){
   case push:

   	   lcdSetText("Push Ups =",0,0);
   	   lcdSetInt(pushcnt,11,0);

   	   if (pushcnt==10){
   			   lcdSetText("Cal.Burned",0,1);
   			   lcdSetInt(pushcnt*.825,11,1);

   			   serial_charTX('P');
   			serial_charTX('u');
   			serial_charTX('s');
   			serial_charTX('h');
   			serial_charTX('\r');
   			serial_charTX('U');
   			serial_charTX('p');
   			serial_charTX('s');


   			   delay_ms(3000);
   			   pushcnt=0;
   			   	y=0;
   			   lcdClear();
   		   }

   break;

   case run:
	   lcdSetText("Run Steps =",0,0);
	   lcdSetInt(pushcnt,12,0);
	   if (pushcnt==20){
	      		lcdSetText("Cal.Burned",0,1);
	      		lcdSetInt(pushcnt*.825,11,1);
	      		delay_ms(3000);
	      		pushcnt=0;
	      		y=0;
	      		lcdClear();
	      		   }

	   break;
   case squats:
	   lcdSetText("Squats = ",0,0);
	   lcdSetInt(pushcnt,9,0);
	   	   if (pushcnt==15){
	   	      		lcdSetText("Cal.Burned",0,1);
	   	      		lcdSetInt(pushcnt*.825,11,1);
	   	      		delay_ms(3000);
	   	      		pushcnt=0;
	   	      		y=0;
	   	      		lcdClear();
	   	      		   }

	   break;
   case menu:{

	   lcdSetText("Press 2=Push Ups",0,0);
	   lcdSetText("3=Run 4=Squats",0,1);

	   }



	   break;



   case start:{
	   lcdSetText("welcome",3,0);
	   lcdSetText("Press 1 to Start",0,1);
   }
   break;
   	}

   //long int y,i;
     enum {
  	   UP,
  	   DOWN
     };

     //int pushcnt=0;
     //direction=DOWN;
     //y=0;

   // while (1){



	   //lcdSetText("Push Ups Mode",0,0);

	   for (i=0; i<10; i++){
		   y += adc_measure(6);
	   }
	   y/=10;

	   if (y>ymax){
		   //lcdSetText("up",0,1);
		   if (direction==DOWN){
			   direction=UP;

		   }
	   }

	   else if (y<ymin){
		  // lcdSetText("down",0,1);
		   if (direction==UP){
			   direction=DOWN;
			   pushcnt++;

		   }
	   }
	   //if (pushcnt==10){
		 //  lcdSetText("Cal.burned",0,1);
		   //lcdSetInt(pushcnt*.825,11,1);
		  // P1OUT|=0x08;
		  // delay_ms(50);
		  // P1OUT &=~0x08;
		 //  delay_ms(50);
		 //  P1OUT|=0x08;
		 //  delay_ms(100);
		 //  P1OUT &=~0x08;
		 //  delay_ms(2000);
		  // lcdClear();

		  // pushcnt=0;

	   }
	 //  y=0;


}


//}


// Timer I
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0A1(void)
{

	P2OUT ^= 0x80;
	seconds++;
	TA0CTL &=~0x01;

}





















