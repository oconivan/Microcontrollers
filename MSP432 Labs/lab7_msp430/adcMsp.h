/*
 * ADC_msp.h
 *
 *  Created on: May 23, 2012
 *      Author: Erioto
 */

#ifndef ADC_MSP_H_
#define ADC_MSP_H_


// External user Functions
// *********************************************************
void adc_init();	// Initialize ADC peripheral
int adc_measure(int channel);	// measure selected ADC Channel
// *********************************************************

#endif /* ADC_MSP_H_ */
