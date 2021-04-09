
#include <msp430g2553.h>
#include "lcdLib.h"
#include <stdlib.h>
#include <stdio.h>
#include "adcMsp.h"
#include "uartMsp.h"
#include <math.h>
/*
 * main.c
 */
#define XMAX 369//480//560
#define XMIN 467//476//550
//#define XYZMAX 1565//1346
//#define XYZMIN 1597//1334
//#define XMAX
//#define XMIN
//#define YMAX
//#define YMIN
//#define ZMAX
//#define ZMIN
#define push 0
#define run 1
#define squats 2
#define menu 3
char str[3];

enum{
	UP,
	DOWN
};

void fNum (int i){
	if(i<10){
		sprintf(str,"0%d",i);
	}else{
		sprintf(str,"%d",i);
	}
}

void setTime(){
	int min = 5;
	fNum(min);
	lcdSetText(str,11,1);
	lcdSetText(":",13,1);
	fNum(0);
	lcdSetText(str,14,1);
	int i;
	for(i = min-1; i>=0; i--){
		int sec;
		for(sec = 59; sec>=0; sec--){
			delay_ms(10);
			fNum(i);
			lcdSetText(str,11,1);
			fNum(sec);
			lcdSetText(str,14,1);
		}
	}

}


int direction;//flag to keep track of directions
int state =menu;//menu state


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P2SEL &= ~ 0X40;
	P2SEL &= ~ 0XC0;

	P1DIR &=~ 0x0B; //set P1.0,1,3 as inputs
	P1REN = 0x0B;  //P1.0,1,3 Resistor enable
	P1OUT |= 0x0B;  // P1.0,1,3 Enable pull up resistor
	P1OUT &= ~BIT4;
	lcdInit();// Initialize LCD
	adc_init();// Initialize ADC

	clock_init();
	serial_init();
	//serial_charTX('9');

	long int x,y,z;//save accelerometer value-32 bits
	long int i;//for loop iterator-32 bits

	int pushCount=0;//push ups counter-16 bits

	direction = DOWN;//user needs to start from down
	x=0;//reset x to start measuring again
	y=0;//reset y to start measuring again
	z=0;//reset z to start measuring again

	lcdSetText("Insanity final ",0,0);
	lcdSetText("starts at",0,1);
	setTime();

	while(1){

		if ((P1IN&0X01)==0)			//read port1 p1.0 if = 0   state = push (0)			push button p1.0 = push ups mode
		{
			pushCount=0;
			x=0;
			y=0;
			z=0;
			lcdClear();
			state=push;
			while ((P1IN&0x01)==0){}  // Debounce
		}
		else if ((P1IN&0x02)==0)		//read port1 p1.1 if ==0   state = run (1)		push button p1.1 = run  mode
		{
			pushCount=0;
			x=0;
			y=0;
			z=0;
			lcdClear();
			state=run;
			while ((P1IN&0x02)==0){} // Debounce

		}
		else if ((P2IN&0x40)==0)		// read port XIN if ==0  state = squats (XIN = 2.6) 	push button XIN= squats mode
		{
			pushCount=0;
			x=0;
			y=0;
			z=0;
			lcdClear();
			state=squats;
			while ((P2IN&0xC0)==0){}
		}
		else if ((P1IN&0x08)==0)		// read port1 p1.3 if ==0  state = menu (3)		push button p1.3 = menu mode
		{
			lcdClear();
			state=menu;
			while ((P1IN&0x08)==0){}
		}

		switch (state){
		case push:
			lcdSetText("Push Ups=",0,0);
			lcdSetInt(pushCount,10,0);
			//lcdSetInt(adc_mesuare(5),0,1);
			if (pushCount==10){
				//setTime();
				P1DIR |= BIT4;
				P1REN &= ~BIT4;
				P1OUT |= BIT4;
				lcdSetText("Cal.Burned",0,1);
				lcdSetInt(pushCount*.500,11,1);

				P1DIR |= BIT2;
				P1OUT &= ~BIT2;

				char routine[]= "Push Ups = ";
				char*c = &routine[0];
				while(*c !='\0'){
					serial_charTX(*c++);
				}

				char pucounter[5];
				char*b= & pucounter[0];
				ltoa(pushCount, pucounter);
				while(*b !='\0'){
					serial_charTX(*b++);
				}

				char puroutine[]= "    Calories Burned = ";
				char*d = &puroutine[0];
				while(*d !='\0'){
					serial_charTX(*d++);
				}

				char pucalories[5];
				char*e= & pucalories[0];
				ltoa(pushCount*.500, pucalories);
				while(*e !='\0'){
					serial_charTX(*e++);
				}

				serial_charTX('\n');

				delay_ms(3000);
				pushCount=0;
				P1OUT &= ~BIT4;
				x=0;
				y=0;
				z=0;
				lcdClear();
			}

			break;

		case run:
			lcdSetText("Run=",0,0);
			lcdSetInt(pushCount,5,0);

			if (pushCount==10){
				P1DIR |= BIT4;
				P1REN &= ~BIT4;
				P1OUT |= BIT4;
				lcdSetText("Cal.Burned",0,1);
				lcdSetInt(pushCount*.500,11,1);

				P1DIR |= BIT2;
				P1OUT &= ~BIT2;

				char routine[]= "Run Steps = ";
				char*c = &routine[0];
				while(*c !='\0'){
					serial_charTX(*c++);

				}


				char runcounter[5];
				char*b= & runcounter[0];
				ltoa(pushCount, runcounter);
				while(*b !='\0'){
					serial_charTX(*b++);
				}

				char runroutine[]= "   Calories Burned = ";
				char*d = &runroutine[0];
				while(*d !='\0'){
					serial_charTX(*d++);
				}
				char runcalories[5];
				char*e= & runcalories[0];
				ltoa(pushCount*.500, runcalories);
				while(*e !='\0'){
					serial_charTX(*e++);
				}

				serial_charTX('\n');

				delay_ms(3000);
				pushCount=0;
				P1OUT &= ~BIT4;
				x=0;
				y=0;
				z=0;
				lcdClear();
			}

			break;
		case squats:
			lcdSetText("Squats= ",0,0);
			lcdSetInt(pushCount,7,0);

			if (pushCount==10){
				P1DIR |= BIT4;
				P1REN &= ~BIT4;
				P1OUT |= BIT4;
				lcdSetText("Cal.Burned",0,1);
				lcdSetInt(pushCount*.500,11,1);

				P1DIR |= BIT2;
				P1OUT &= ~BIT2;

				char routine[]= "Squats = ";
				char*c = &routine[0];
				while(*c !='\0'){
					serial_charTX(*c++);

				}


				char squatscounter[5];
				char*b= & squatscounter[0];
				ltoa(pushCount, squatscounter);
				while(*b !='\0'){
					serial_charTX(*b++);
				}

				char squatsroutine[]= "      Calories Burned = ";
				char*d = &squatsroutine[0];
				while(*d !='\0'){
					serial_charTX(*d++);
				}
				char squatscalories[5];
				char*e= & squatscalories[0];
				ltoa(pushCount*.500, squatscalories);
				while(*e !='\0'){
					serial_charTX(*e++);
				}

				serial_charTX('\n');

				delay_ms(3000);
				pushCount=0;
				P1OUT &= ~BIT4;
				x=0;
				y=0;
				z=0;
				lcdClear();
			}

			break;
		case menu:{

			lcdSetText("0=PushUps 1=run",0,0);
			lcdSetText("2=Squats 3=menu",0,1);

		}

		break;

		}//Switch End

		for(i=0; i<100; i++)
		{//read 10 samples from pin 2
			x += adc_measure(5);
			//y += adc_measure(6);
			//z += adc_measure(7);
		}

		x/=100;//take samples average
		//y/=100;//take samples average
		//z/=100;//take samples average

		//long int m = sqrt((x*x)+(y*y)+(z*z));

		if(x >=XMAX)//&& y >=YMAX && z >=ZMAX
		{
			//lcdSetText("up",0,1);
			if(direction == DOWN){
				direction = UP;
			}
		}
		else if(x <=XMIN)//&& y <=YMIN && z <=ZMIN
		{
			//lcdSetText("down",0,1);
			if(direction == UP){
				direction = DOWN;
				pushCount++;
			}
		}
		else{

		}
	}//End While
	return 0;
}//End Main
