/*
 * DS1338.h
 *
 *  Created on: Oct 31, 2013
 *      Author: spave
 */

#ifndef DS1338_H_
#define DS1338_H_

#define DS1338_ADDR			0x68  // 7-bit address

#define DS1338_REG_SEC			0x00
#define DS1338_REG_SEC_CH		0x80  // The MSB of the sec reg is set if the clock is _DISABLED_

#define DS1338_REG_MIN			0x01

#define DS1338_REG_HOUR			0x02

#define DS1338_REG_CTRL			0x07
#define DS1338_REG_CTRL_DEF		0xB3  // The default value of the control reg upon startup. Will use to test comm


int DS1338_config(void);
uint16_t DS1338_get_secs(void);
int DS1338_read_RAM(uint8_t addr, void* buf, uint16_t len);
int DS1338_write_RAM(uint8_t addr, void* buf, uint16_t len);

#endif /* DS1338_H_ */
