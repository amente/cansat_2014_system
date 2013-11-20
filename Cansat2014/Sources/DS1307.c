/*
 * DS1307.c
 *
 *  Created on: Oct 31, 2013
 *      Author: spave
 */

#include <i2c.h>
#include <DS1307.h>

static uint16_t	buf;
static uint8_t	reg;


bool_t DS1307_test(void)
{
	reg = DS1307_REG_CTRL;
	
	i2c_set_addr(DS1307_ADDR);
	i2c_write_no_stop(&reg, sizeof(reg));
	i2c_read(&buf, 2);	
	
	return DS1307_REG_CTRL_DEF == (buf >> 8);	
}

void DS1307_enable(void)
{
	const uint8_t ZERO[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // First 0 for the reg addr follow by 8 more to zero the RTC and set HTL to false (start the clock)
	i2c_set_addr(DS1307_ADDR);
	i2c_write(&ZERO, 1+8);  
}

static uint16_t DS1307_BCD_to_sec(uint32_t time)
{
	// The last byte of time is not used
	uint8_t  bcd_sec, bcd_min, bcd_hour;
	uint16_t secs = 0;
	
	bcd_sec  =    *(uint8_t*)&time;
	bcd_min  =  *(((uint8_t*)&time)+1);
	bcd_hour =  *(((uint8_t*)&time)+2);
		
	secs  = ((0x0F & bcd_sec));                          //    sec
	secs += ((0x70 & bcd_sec) >> 4)  * 10;               // 10 sec

	secs += ((0x0F & bcd_min))       * 60;               //    min
	secs += ((0x70 & bcd_min) >> 4)  * 60 * 10;          // 10 min
		
	secs += ((0x0F & bcd_hour))      * 60 * 60;          //    hour
	secs += ((0x30 & bcd_hour) >> 4) * 60 * 60 * 10;     // 10 hour
	
	return secs;
}

uint16_t DS1307_get_secs(void)
{
	uint32_t time;
	reg = DS1307_REG_SEC;
	
	i2c_set_addr(DS1307_ADDR);
	i2c_write_no_stop(&reg, sizeof(reg));
	i2c_read(&time, 3);  // This will read in the secs, min and hours in that order
		
	return DS1307_BCD_to_sec(time);
}

int DS1307_read_RAM(uint8_t addr, void* buf, uint16_t len)
{
	if ( (addr < 0x08) || (addr + len > 0x3F + 1) )
		 return -EINVAL; 		/* Return invalid arguments for out of range addr */
	
	i2c_set_addr(DS1307_ADDR);
	i2c_write_no_stop(&addr, sizeof(addr));
	
	return i2c_read(buf, len);
}

int DS1307_write_RAM(uint8_t addr, void* buf, uint16_t len)
{
	uint8_t data[56+1];  // TODO: can I do better than this?
	
	if ( (addr < 0x08) || (addr + len > 0x3F + 1) )
		 return -EINVAL; 		/* Return invalid arguments for out of range addr */
	
	data[0] = addr;
	memcpy(&data[1], buf, len);
	
	i2c_set_addr(DS1307_ADDR);
	return i2c_write(&data, len+1);
}