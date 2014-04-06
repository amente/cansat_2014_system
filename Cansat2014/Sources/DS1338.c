/*
 * DS1338.c
 *
 *  Created on: Oct 31, 2013
 *      Author: spave
 */

#include <i2c.h>
#include <DS1338.h>

int DS1338_config(void)
{
	uint16_t buf = DS1338_REG_CTRL << 8 | 0x10; // The high 8 bit is the control reg while the value of 0x10 is written to it
									   	   	   	// set to SQW to 1Hz
	i2c_set_addr(DS1338_ADDR);
	return i2c_write(&buf, sizeof(buf)) > 0;	
}

static uint16_t DS1338_BCD_to_sec(uint32_t time)
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

uint16_t DS1338_get_secs(void)
{
	uint32_t buf = DS1338_REG_SEC;
	
	i2c_set_addr(DS1338_ADDR);
	i2c_write_no_stop(((char*)&buf)+3, 1);
	i2c_read(&buf, 3);  // This will read in the secs, min and hours in that order

	return DS1338_BCD_to_sec(buf);
}

int DS1338_read_RAM(uint8_t addr, void* buf, uint16_t len)
{
	if ( (addr < 0x08) || (addr + len > 0x3F + 1) )
		 return -EINVAL; 		// Return invalid arguments for out of range addr
	
	i2c_set_addr(DS1338_ADDR);
	i2c_write_no_stop(&addr, 1);	
	return i2c_read(buf, len);
}

int DS1338_write_RAM(uint8_t addr, void* buf, uint16_t len)
{	
	if ( (addr < 0x08) || (addr + len > 0x3F + 1) )
		 return -EINVAL; 		// Return invalid arguments for out of range addr 
	
	i2c_set_addr(DS1338_ADDR);
	i2c_write_no_stop(&addr, 1);	
	return i2c_write(buf, len);
}