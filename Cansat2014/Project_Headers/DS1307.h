/*
 * DS1307.h
 *
 *  Created on: Oct 31, 2013
 *      Author: spave
 */

#ifndef DS1307_H_
#define DS1307_H_

#define DS1307_ADDR			0x68  // 7-bit address

#define DS1307_REG_SEC			0x00
#define DS1307_REG_SEC_CH		0x80  // The MSB of the sec reg is set if the clock is _DISABLED_

#define DS1307_REG_MIN			0x01

#define DS1307_REG_HOUR			0x02

#define DS1307_REG_CTRL			0x07
#define DS1307_REG_CTRL_DEF		0x03  // The default value of the control reg upon startup. Will use to test comm


bool_t DS1307_test(void);
void DS1307_enable(void);
uint16_t DS1307_get_secs(void);
int DS1307_read_RAM(uint8_t addr, void* buf, uint16_t len);
int DS1307_write_RAM(uint8_t addr, void* buf, uint16_t len);

#endif /* DS1307_H_ */
