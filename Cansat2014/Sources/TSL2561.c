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

void TSL2561_read_raw(uint16_t *ch0, uint16_t *ch1)
{
	uint32_t buf = 0;
	
	if (!TSL2561_isConfiged)
	{
		TSL2561_config();
		TSL2561_isConfiged = 1;
	}
	
	TSL2561_power(1);
	delay(TSL2561_INTEGRATION_TIME);
	TSL2561_set_cmd(TSL2561_REG_DATA0LOW);
	i2c_read(&buf, sizeof(buf));
	TSL2561_power(0);
	
	*ch0 = swap16((unsigned int) buf);		// Data coming in is LE while the MCU is BE so the high/low bytes are swapped
	*ch1 = swap16(((unsigned int*) buf)[1]);
}

void TSL2561_CalculateLux(unsigned int *lux, unsigned int *IRlux, unsigned int ch0, unsigned int ch1)
{
	//------------------------------------------------------------------------
	// first, scale the channel values depending on the gain and integration time
	// 16X, 402mS is nominal.
	// scale if integration time is NOT 402 msec
	unsigned long chScale;
	unsigned long channel1;
	unsigned long channel0;
	
	unsigned long ratio;
	unsigned long ratio1;
	unsigned int b, m;
	
	//unsigned long temp;
	//unsigned long lux;
	
#if (TSL2561_INTEGRATION_TIME == TSL2561_INTEGRATION_TIME_13_7)
		chScale = CHSCALE_TINT0;
#elif (TSL2561_INTEGRATION_TIME == TSL2561_INTEGRATION_TIME_101)
		chScale = CHSCALE_TINT1;
#else
		chScale = (1 << CH_SCALE);
#endif

#ifndef TSL2561_GAIN_16X
	// scale if gain is NOT 16X
	chScale = chScale << 4; // scale 1X to 16X
#endif
	
	// scale the channel values
	channel0 = (ch0 * chScale) >> CH_SCALE;
	channel1 = (ch1 * chScale) >> CH_SCALE;
	//------------------------------------------------------------------------
	// find the ratio of the channel values (Channel1/Channel0)
	// protect against divide by zero
	ratio1 = 0;
	if (channel0 != 0) ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
	// round the ratio value
	ratio = (ratio1 + 1) >> 1;
	// is ratio <= eachBreak ?
	
#ifdef TSL2561_PACKAGE_T
		if ((ratio >= 0) && (ratio <= K1T))
			{b=B1T; m=M1T;}
		else if (ratio <= K2T)
			{b=B2T; m=M2T;}
		else if (ratio <= K3T)
			{b=B3T; m=M3T;}
		else if (ratio <= K4T)
			{b=B4T; m=M4T;}
		else if (ratio <= K5T)
			{b=B5T; m=M5T;}
		else if (ratio <= K6T)
			{b=B6T; m=M6T;}
		else if (ratio <= K7T)
			{b=B7T; m=M7T;}
		else if (ratio > K8T)
			{b=B8T; m=M8T;}
#else
		if ((ratio >= 0) && (ratio <= K1C))
			{b=B1C; m=M1C;}
		else if (ratio <= K2C)
			{b=B2C; m=M2C;}
		else if (ratio <= K3C)
			{b=B3C; m=M3C;}
		else if (ratio <= K4C)
			{b=B4C; m=M4C;}
		else if (ratio <= K5C)
			{b=B5C; m=M5C;}
		else if (ratio <= K6C)
			{b=B6C; m=M6C;}
		else if (ratio <= K7C)
			{b=B7C; m=M7C;}
		else if (ratio > K8C)
			{b=B8C; m=M8C;}
#endif

	/*
	temp = ((channel0 * b) - (channel1 * m));
	// do not allow negative lux value
	if (temp < 0) temp = 0;
	// round lsb (2^(LUX_SCALE-1))
	temp += (1 << (LUX_SCALE-1));
	// strip off fractional portion
	lux = temp >> LUX_SCALE;
	*/
	
	*lux = (unsigned int)((channel0 * b) >> LUX_SCALE);
	*IRlux = (unsigned int)((channel1 * m) >> LUX_SCALE);	
}
