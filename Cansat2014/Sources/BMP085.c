/*
 * BMP085.c
 *
 *  Created on: Sep 25, 2013
 *      Author: Amente
 *      		Spave
 */
#include <types.h>
#include <math.h>
#include <i2c.h>
#include <rtc.h>
#include <BMP085.h>
#include <util.h>

static uint16_t buffer;
static uint8_t data[2] = { BMP085_CONTROL, 0 };

bool_t BMP085_test(void)
{
	buffer = 0;

	i2c_set_addr(BMP085_I2CADDR);		
	data[1] = BMP085_CHIP_REG;
	i2c_write_no_stop(&data[1], 1);
	i2c_read(&buffer, sizeof(buffer));
	
	return BMP085_CHIP_ID == (buffer >> 8);
}

uint16_t BMP085_readTemp()
{
	buffer = 0;
	
	i2c_set_addr(BMP085_I2CADDR);
	data[1] = BMP085_READTEMPCMD;
	i2c_write(&data, 2);
	delay(BMP085_SAMPLE_DELAY);
	data[1] = BMP085_TEMPDATA;
	i2c_write_no_stop(&data[1], 1);
	i2c_read(&buffer, sizeof(buffer));
	
	return buffer;
}

uint16_t BMP085_readPressure()
{
	buffer = 0;
	
	i2c_set_addr(BMP085_I2CADDR);		
	data[1] = BMP085_READPRESSURECMD | (BMP085_PRESSURE_OSS<<6);
	i2c_write(&data, 2);
	delay(BMP085_SAMPLE_DELAY);
	data[1] = BMP085_PRESSUREDATA;
	i2c_write_no_stop(&data[1], 1);
	i2c_read(&buffer, sizeof(buffer));
	
    return buffer;
}

void BMP085_printCalibrationData(){
	
		uint16_t ac1,ac2,ac3,b1,b2,mb,mc,md;
		int16_t ac4,ac5,ac6;
		uint8_t sendReg  = 0xAA;
		
		i2c_set_addr(BMP085_I2CADDR);	
		i2c_write_no_stop(&sendReg, 1);	
		i2c_read(&ac1,2); sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&ac2,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&ac3,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&ac4,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&ac5,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&ac6,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&b1,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&b2,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&mb,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&mc,2);sendReg+=2;
		i2c_write_no_stop(&sendReg,1);
		i2c_read(&md,2);
		
		printf("ac1: %d\r",ac1);
		printf("ac2: %d\r",ac2);
		printf("ac3: %d\r",ac3);
		
		printf("ac4: %u\r",ac4);
		printf("ac5: %u\r",ac5);
		printf("ac6: %u\r",ac6);
		
		printf("b1: %d\r",b1);
		printf("b2: %d\r",b2);
		printf("mb: %d\r",mb);
		printf("mc: %d\r",mc);
		printf("md: %d\r",md);	
}



