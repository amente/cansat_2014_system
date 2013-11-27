/*
 * vadc.h
 *
 *  Created on: Nov 25, 2013
 *      Author: spave
 */

#ifndef VADC_H_
#define VADC_H_

#define VADC_PIN XPIN_20

#define vadc_read_raw() adc_ch_read(VADC_PIN)

#endif /* VADC_H_ */
