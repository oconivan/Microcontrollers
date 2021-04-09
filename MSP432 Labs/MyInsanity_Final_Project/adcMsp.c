/*
 * ADC_msp.c
 *
 *  Created on: May 23, 2012
 *      Author: Erioto
 *      Modified by: E N Jaquez
 */
#include <msp430g2553.h>
#include "adcMsp.h"

void adc_init()
{
// Initialize ADC pg. 567-570
	ADC10CTL0 = 0x0010;// ****** VCC - VSS, 4 x ADC10CLKS, 200ksps, no output reference, ADC10 on, no interrupts, disable conversion, no conversion start ****** //
}

int adc_measure(int channel)
{
    ADC10CTL0 &= ~ENC;		// Disable ADC to modify ADC Control Registers

    //Analog (Input) Enable for specific channel
    switch (channel){
    case 0:
    	ADC10AE0 = 0x0001;
    	break;
    case 1:
    	ADC10AE0 = 0x0002;
    	break;
    case 2:
    	ADC10AE0 = 0x0004;
    	break;
    case 3:
    	ADC10AE0 = 0x0008;
    	break;
    case 4:
    	ADC10AE0 = 0x0010;
    	break;
    case 5:
    	ADC10AE0 = 0x0020;
    	break;
    case 6:
    	ADC10AE0 = 0x0040;
    	break;
    case 7:
    	ADC10AE0 = 0x0080;
    	break;
    default:
    	ADC10AE0 = 0x0000;
    	break;
    }

//set ADC10CTL1 and modify ADC10CTL0
    ADC10CTL1 = channel << 12;// ****** Input Channel is channel passed into the function (use C bitwise roll-left to Hex MSB), ADC10SC for Sample-and-hold, straight binary format, not inverted, /1, ADC10OSC clock source, single-channel-single-conversion, no active operation ****** //
    ADC10CTL0 |= 0x0003; // ****** Enable ADC and start conversion ****** //

    while ( (ADC10CTL0 & ADC10IFG) == 0);	// Wait until the conversion is finished

    ADC10CTL0 &= ~ENC;		// Disable ADC before retrieving the conversion from memory

    ADC10AE0 = 0x0000;

    return ADC10MEM;		// Return measurement value stored in ACD memory
}
