/*
 * TSL2561.c
 *
 *  Created on: Oct 16, 2013
 *      Author: spave
 */

#include <i2c.h>
#include <TSL2561.h>
#include <util.h>

static uint8_t TSL2561_isConfiged = 0;

static int TSL2561_set_cmd(uint8_t reg)
{
	i2c_set_addr(TSL2561_ADDR);
	reg |= TSL2561_CMD_TEMPLATE;
	return i2c_write_no_stop(&reg, 1);
}

bool_t TSL2561_test(void)
{
	uint16_t id;
	TSL2561_set_cmd(TSL2561_REG_ID);
	i2c_read(&id, 2);
	return TSL2561_ID == (id >> 8);	
}

static int TSL2561_power(bool_t state)  // 1 = ON, 0= OFF
{
	state = state ? TSL2561_REG_CTRL_PWR_ON : TSL2561_REG_CTRL_PWR_OFF;	
	TSL2561_set_cmd(TSL2561_REG_CTRL);	
	return i2c_write(&state, 1);
}

#pragma INLINE
static void TSL2561_config(void)
{
	char timing = TSL2561_REG_TIMING_INTEGRATION_TIME;	// Leave the rest at default
	TSL2561_set_cmd(TSL2561_REG_TIMING);
	i2c_write(&timing, 1);
}

uint16_t TSL2561_read_raw(void)
{
	uint16_t raw = 0;
	
	if (!TSL2561_isConfiged)
	{
		TSL2561_config();
		TSL2561_isConfiged = 1;
	}
	
	TSL2561_power(1);
	delay(TSL2561_INTEGRATION_TIME);
	TSL2561_set_cmd(TSL2561_REG_DATA0LOW);
	i2c_read(&raw, 2);
	TSL2561_power(0);
	
	return swap16(raw);	// Data coming in is LE while the MCU is BE so the high/low bytes are swapped  
}
