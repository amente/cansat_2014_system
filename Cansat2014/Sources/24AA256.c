/*
 * 24AA256.c
 *
 *  Created on: Oct 16, 2013
 *      Author: spave
 */

#include <i2c.h>
#include <24AA256.h>
#include <util.h>

int rc; // return code

//TODO: BOUNDARY CHECKS!!

int EEPROM_24AA256_write(uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint8_t writebuf[2+64];  // 2byte address + 64byte data
	i2c_set_addr(EEPROM_24AA256_ADDR);
	
	memcpy(writebuf, &addr, 2);
	memcpy(writebuf, buf, len);
	
	rc = i2c_write(writebuf, len+2);
	if (rc < 0) return rc;	
	delay(5);
	
	return rc;
}

int EEPROM_24AA256_read(uint16_t addr, uint8_t *buf, uint16_t len)
{
	i2c_set_addr(EEPROM_24AA256_ADDR);
	
	rc = i2c_write_no_stop(&addr, 2);
	if (rc < 0) return rc;
	
	return i2c_read(buf, len);
}