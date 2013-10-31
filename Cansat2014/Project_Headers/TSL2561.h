/*
 * TSL2561.h
 *
 *  Created on: Oct 20, 2013
 *      Author: spave
 */

#ifndef 	TSL2561_H_
#define 	TSL2561_H_

#define		TSL2561_ADDR				0x39	// 7-bit address when ADDR pin is floating
#define		TSL2561_CMD_TEMPLATE		0xB0	// Template for the command reg. Use the last 4 bits for reg address
#define 	TSL2561_INTEGRATION_TIME	15		// mS

#define 	TSL2561_REG_ID				0x0A	// Contains the Part number/ Rev ID
#define		TSL2561_ID					0x50	// Expected ID of the sensor. Useful for comm test (ie. read(TSL2561_REG_ID) == TSL2561_ID)

#define 	TSL2561_REG_CTRL			0x00	// Control Register. Used to power on/off the sensor
#define 	TSL2561_REG_CTRL_PWR_ON		0x03
#define 	TSL2561_REG_CTRL_PWR_OFF	0x00

#define 	TSL2561_REG_TIMING			0x01
#define 	TSL2561_REG_TIMING_INTEGRATION_TIME 0x00	// Use the shortest integration time of ~14ms
#define 	TSL2561_RAW_SCALE			0.034			// Do the scaling on the ground station

#define 	TSL2561_REG_DATA0LOW		0x0C	// Low bytes for ADC0 (full range)
#define 	TSL2561_REG_DATA0HIGH		0x0D	// High bytes for ADC0 (full range)

bool_t		TSL2561_test(void);
uint16_t	TSL2561_read_raw(void);

#endif /* TSL2561_H_ */