/*
 * vadc.h
 *
 *  Created on: Nov 25, 2013
 *      Author: spave
 */

#ifndef VADC_H_
#define VADC_H_

#define VREF 3300  //mV
#define VADC_MUL 2.8f  // VMeasure was stepped down by 1/3 before being measured
#define vadc_read_raw() (adc_ch_read(VMEASURE))
#define vadc_read() ( (int)((adc_ch_read(VMEASURE)/(float)0x0FFF)*VREF*VADC_MUL) )  //12-bit ADC; returns volt in mV

#endif /* VADC_H_ */
