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
	i2c_write_no_stop(&reg, 1);
	i2c_read(&buf, 2);	
	
	return DS1307_REG_CTRL_DEF == (buf >> 8);	
}