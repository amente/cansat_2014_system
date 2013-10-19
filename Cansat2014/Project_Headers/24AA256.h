/*
 * 24AA256.h
 *
 *  Created on: Oct 18, 2013
 *      Author: spave
 */

#ifndef EEPROM_24AA256_H_
#define EEPROM_24AA256_H_

#define EEPROM_24AA256_ADDR 0x50  // 7-bit address

int EEPROM_24AA256_write(uint16_t addr, uint8_t *buf, uint16_t len);
int EEPROM_24AA256_read(uint16_t addr, uint8_t *buf, uint16_t len);

#endif /* 24AA256_H_ */