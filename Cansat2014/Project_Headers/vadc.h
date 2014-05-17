/*
 * vadc.h
 *
 *  Created on: Nov 25, 2013
 *      Author: spave
 */

#ifndef VADC_H_
#define VADC_H_

#define VCC 3300  //mV
#define vadc_read_raw() (adc_ch_read(VMEASURE))
#define vadc_read() ( (int)((adc_ch_read(VMEASURE)/(float)0x0FFF)*VCC) )  //12-bit ADC, returns volt in mV

#endif /* VADC_H_ */
