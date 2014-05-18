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
	char timing = TSL2561_REG_TIMING_INTEGRATION_TIME | TSL2561_GAIN_16X;	// Leave the rest at default
	TSL2561_set_cmd(TSL2561_REG_TIMING);
	i2c_write(&timing, 1);
}

void TSL2561_read_raw(uint16_t *ch0, uint16_t *ch1)
{
	//uint32_t buf = 0;
	
	if (!TSL2561_isConfiged)
	{
		TSL2561_config();
		TSL2561_isConfiged = 1;
	}
	
	TSL2561_power(1);
	delay(TSL2561_INTEGRATION_TIME);
	TSL2561_set_cmd(TSL2561_REG_DATA0LOW);
	//i2c_read(&buf, sizeof(buf));
	i2c_read(ch0, 2);
	TSL2561_set_cmd(TSL2561_REG_DATA1LOW);
	i2c_read(ch1, 2);
	TSL2561_power(0);

	*ch0 = swap16(*ch0);		// Data coming in is LE while the MCU is BE so the high/low bytes are swapped
	*ch1 = swap16(*ch1);
	
#ifdef __DEBUG__
	 printf("\rCH0 %X\r",*ch0);
	 printf("\rCH1 %X\r",*ch1);
#endif
	
}
