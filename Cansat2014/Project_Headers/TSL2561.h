/*
 * TSL2561.h
 *
 *  Created on: Oct 20, 2013
 *      Author: spave
 */

#ifndef 	TSL2561_H_
#define 	TSL2561_H_

#define		TSL2561_ADDR				0x39	// 7-bit address when ADDR pin is floating
#define		TSL2561_CMD_TEMPLATE		0xA0	// Template for the command reg. Use the last 4 bits for reg address

#define 	TSL2561_INTEGRATION_TIME_13_7	15		// mS
#define 	TSL2561_INTEGRATION_TIME_101	105		// mS
#define 	TSL2561_INTEGRATION_TIME_NONE	0		// mS

#define 	TSL2561_INTEGRATION_TIME	TSL2561_INTEGRATION_TIME_13_7

#define		TSL2561_GAIN_1X		0x00
#define 	TSL2561_GAIN_16X	0x10

#define 	TSL2561_REG_TIMING			0x01
#define 	TSL2561_REG_TIMING_INTEGRATION_TIME 0x00	// Use the shortest integration time of ~14ms

#define 	TSL2561_REG_ID				0x0A	// Contains the Part number/ Rev ID
#define		TSL2561_ID					0x50	// Expected ID of the sensor. Useful for comm test (ie. read(TSL2561_REG_ID) == TSL2561_ID)

#define 	TSL2561_REG_CTRL			0x00	// Control Register. Used to power on/off the sensor
#define 	TSL2561_REG_CTRL_PWR_ON		0x03
#define 	TSL2561_REG_CTRL_PWR_OFF	0x00

#define 	TSL2561_REG_DATA0LOW		0x0C	// Low bytes for ADC0 (full range)
#define 	TSL2561_REG_DATA0HIGH		0x0D	// High bytes for ADC0 (full range)
#define 	TSL2561_REG_DATA1LOW		0x0E	// Low bytes for ADC0 (full range)
#define 	TSL2561_REG_DATA1HIGH		0x0F	// High bytes for ADC0 (full range)

#define		TSL2561_PACKAGE_CS
//#define		TSL2561_PACKAGE_T

//****************************************************************************
//
// Copyright (R) 2004-2005 TAOS, Inc.
//
//****************************************************************************
#define LUX_SCALE 14 // scale by 2^14
#define RATIO_SCALE 9 // scale ratio by 2^9
//---------------------------------------------------
// Integration time scaling factors
//---------------------------------------------------
#define CH_SCALE 10 // scale channel values by 2^10
#define CHSCALE_TINT0 0x7517 // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1 0x0fe7 // 322/81 * 2^CH_SCALE
//---------------------------------------------------
// T Package coefficients
//---------------------------------------------------
// For Ch1/Ch0=0.00 to 0.50
// Lux/Ch0=0.0304-0.062*((Ch1/Ch0)^1.4)
// piecewise approximation
// For Ch1/Ch0=0.00 to 0.125:
// Lux/Ch0=0.0304-0.0272*(Ch1/Ch0)
//
// For Ch1/Ch0=0.125 to 0.250:
// Lux/Ch0=0.0325-0.0440*(Ch1/Ch0)
//
// For Ch1/Ch0=0.250 to 0.375:
// Lux/Ch0=0.0351-0.0544*(Ch1/Ch0)
//
// For Ch1/Ch0=0.375 to 0.50:
// Lux/Ch0=0.0381-0.0624*(Ch1/Ch0)
//
// For Ch1/Ch0=0.50 to 0.61:
// Lux/Ch0=0.0224-0.031*(Ch1/Ch0)
//
// For Ch1/Ch0=0.61 to 0.80:
// Lux/Ch0=0.0128-0.0153*(Ch1/Ch0)
//
// For Ch1/Ch0=0.80 to 1.30:
// Lux/Ch0=0.00146-0.00112*(Ch1/Ch0)
//
// For Ch1/Ch0>1.3:
// Lux/Ch0=0
//---------------------------------------------------
#define K1T 0x0040 // 0.125 * 2^RATIO_SCALE
#define B1T 0x01f2 // 0.0304 * 2^LUX_SCALE
#define M1T 0x01be // 0.0272 * 2^LUX_SCALE
#define K2T 0x0080 // 0.250 * 2^RATIO_SCALE
#define B2T 0x0214 // 0.0325 * 2^LUX_SCALE
#define M2T 0x02d1 // 0.0440 * 2^LUX_SCALE
#define K3T 0x00c0 // 0.375 * 2^RATIO_SCALE
#define B3T 0x023f // 0.0351 * 2^LUX_SCALE
#define M3T 0x037b // 0.0544 * 2^LUX_SCALE
#define K4T 0x0100 // 0.50 * 2^RATIO_SCALE
#define B4T 0x0270 // 0.0381 * 2^LUX_SCALE
#define M4T 0x03fe // 0.0624 * 2^LUX_SCALE
#define K5T 0x0138 // 0.61 * 2^RATIO_SCALE
#define B5T 0x016f // 0.0224 * 2^LUX_SCALE
#define M5T 0x01fc // 0.0310 * 2^LUX_SCALE
#define K6T 0x019a // 0.80 * 2^RATIO_SCALE
#define B6T 0x00d2 // 0.0128 * 2^LUX_SCALE
#define M6T 0x00fb // 0.0153 * 2^LUX_SCALE
#define K7T 0x029a // 1.3 * 2^RATIO_SCALE
#define B7T 0x0018 // 0.00146 * 2^LUX_SCALE
#define M7T 0x0012 // 0.00112 * 2^LUX_SCALE
#define K8T 0x029a // 1.3 * 2^RATIO_SCALE
#define B8T 0x0000 // 0.000 * 2^LUX_SCALE
#define M8T 0x0000 // 0.000 * 2^LUX_SCALE
//---------------------------------------------------
// CS package coefficients
//---------------------------------------------------
// For 0 <= Ch1/Ch0 <= 0.52
// Lux/Ch0 = 0.0315-0.0593*((Ch1/Ch0)^1.4)
// piecewise approximation
// For 0 <= Ch1/Ch0 <= 0.13
// Lux/Ch0 = 0.0315-0.0262*(Ch1/Ch0)
// For 0.13 <= Ch1/Ch0 <= 0.26
// Lux/Ch0 = 0.0337-0.0430*(Ch1/Ch0)
// For 0.26 <= Ch1/Ch0 <= 0.39
// Lux/Ch0 = 0.0363-0.0529*(Ch1/Ch0)
// For 0.39 <= Ch1/Ch0 <= 0.52
// Lux/Ch0 = 0.0392-0.0605*(Ch1/Ch0)
// For 0.52 < Ch1/Ch0 <= 0.65
// Lux/Ch0 = 0.0229-0.0291*(Ch1/Ch0)
// For 0.65 < Ch1/Ch0 <= 0.80
// Lux/Ch0 = 0.00157-0.00180*(Ch1/Ch0)
// For 0.80 < Ch1/Ch0 <= 1.30
// Lux/Ch0 = 0.00338-0.00260*(Ch1/Ch0)
// For Ch1/Ch0 > 1.30
// Lux = 0
//---------------------------------------------------
#define K1C 0x0043 // 0.130 * 2^RATIO_SCALE
#define B1C 0x0204 // 0.0315 * 2^LUX_SCALE
#define M1C 0x01ad // 0.0262 * 2^LUX_SCALE
#define K2C 0x0085 // 0.260 * 2^RATIO_SCALE
#define B2C 0x0228 // 0.0337 * 2^LUX_SCALE
#define M2C 0x02c1 // 0.0430 * 2^LUX_SCALE
#define K3C 0x00c8 // 0.390 * 2^RATIO_SCALE
#define B3C 0x0253 // 0.0363 * 2^LUX_SCALE
#define M3C 0x0363 // 0.0529 * 2^LUX_SCALE
#define K4C 0x010a // 0.520 * 2^RATIO_SCALE
#define B4C 0x0282 // 0.0392 * 2^LUX_SCALE
#define M4C 0x03df // 0.0605 * 2^LUX_SCALE
#define K5C 0x014d // 0.65 * 2^RATIO_SCALE
#define B5C 0x0177 // 0.0229 * 2^LUX_SCALE
#define M5C 0x01dd // 0.0291 * 2^LUX_SCALE
#define K6C 0x019a // 0.80 * 2^RATIO_SCALE
#define B6C 0x0101 // 0.0157 * 2^LUX_SCALE
#define M6C 0x0127 // 0.0180 * 2^LUX_SCALE
#define K7C 0x029a // 1.3 * 2^RATIO_SCALE
#define B7C 0x0037 // 0.00338 * 2^LUX_SCALE
#define M7C 0x002b // 0.00260 * 2^LUX_SCALE
#define K8C 0x029a // 1.3 * 2^RATIO_SCALE
#define B8C 0x0000 // 0.000 * 2^LUX_SCALE
#define M8C 0x0000 // 0.000 * 2^LUX_SCALE
// lux equation approximation without floating point calculations
//////////////////////////////////////////////////////////////////////////////
// Routine: unsigned int CalculateLux(unsigned int ch0, unsigned int ch0, int iType)
//
// Description: Calculate the approximate illuminance (lux) given the raw
// channel values of the TSL2560. The equation if implemented
// as a piece-wise linear approximation.
//
// Arguments: unsigned int iGain - gain, where 0:1X, 1:16X
// unsigned int tInt - integration time, where 0:13.7mS, 1:100mS, 2:402mS,
// 3:Manual
// unsigned int ch0 - raw channel value from channel 0 of TSL2560
// unsigned int ch1 - raw channel value from channel 1 of TSL2560
// unsigned int iType - package type (T or CS)
//
// Return: unsigned int - the approximate illuminance (lux)
//
//////////////////////////////////////////////////////////////////////////////

bool_t		TSL2561_test(void);
void 		TSL2561_read_raw(uint16_t *ch0, uint16_t *ch1);

#endif /* TSL2561_H_ */
