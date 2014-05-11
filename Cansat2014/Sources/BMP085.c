/*
 * BMP085.c
 *
 *  Created on: Sep 25, 2013
 *      Author: Amente
 *              Spave
 */
#include <types.h>
#include <math.h>
#include <i2c.h>
#include <rtc.h>
#include <BMP085.h>
#include <util.h>
 
static uint16_t buffer;
static int32_t buffer2;
static uint8_t data[2] = { BMP085_CONTROL, 0 };

static bmp085_t bmp085;
static uint8_t BMP085_isConfiged = 0;

bool_t BMP085_test(void)
{
        uint8_t buffer = 0;
 
        i2c_set_addr(BMP085_I2CADDR);          
        data[1] = BMP085_CHIP_REG;
        i2c_write_no_stop(&data[1], 1);
        i2c_read(&buffer, sizeof(buffer));
       
        return BMP085_CHIP_ID == buffer;
}
 
int16_t BMP085_readTemp()
{
		if (!BMP085_isConfiged)
		{
			BMP085_calibrate();
			BMP085_isConfiged = 1;
		}
        buffer = 0;
       
        i2c_set_addr(BMP085_I2CADDR);
        data[1] = BMP085_READTEMPCMD;
        i2c_write(&data, 2);
        delay(BMP085_SAMPLE_DELAY);
        data[1] = BMP085_TEMPDATA;
        i2c_write_no_stop(&data[1], 1);
        i2c_read(&buffer, sizeof(buffer));  // UT = MSB << 8 + LSB     
      
        return buffer;
}
 
int32_t BMP085_readPressure(void)
{
	if (!BMP085_isConfiged)
	{
		BMP085_calibrate();
		BMP085_isConfiged = 1;
	}
	
	buffer2 = 0;   
	i2c_set_addr(BMP085_I2CADDR);          
	data[1] = BMP085_READPRESSURECMD | (BMP085_PRESSURE_OSS<<6);
	i2c_write(&data, 2);
	delay(BMP085_SAMPLE_DELAY);
	data[1] = BMP085_PRESSUREDATA;
	i2c_write_no_stop(&data[1], 1);
	i2c_read(((char*)&buffer2)+1, 3);	
	
    return buffer2 >> (8-BMP085_PRESSURE_OSS);
}
 
void BMP085_calibrate(){
       
                uint8_t sendReg  = 0xAA;
                i2c_set_addr(BMP085_I2CADDR);  
                i2c_write_no_stop(&sendReg, 1);
	
                i2c_read(&bmp085, sizeof(short)*11);
}
 
 
//short bmp085_calc_temperature(unsigned long ut)
long BMP085_convert_temperature(long ut){
        long temperature;
        long x1,x2, b5;
 
    x1 = (((long) ut - (long) bmp085.ac6) * (long) bmp085.ac5) >> 15;
    x2 = ((long) bmp085.mc << 11) / (x1 + (long)bmp085.md);
    b5 = x1 + x2;
 
  temperature = ((b5 + 8) >> 4);  // temperature in 0.1°C
 
  return (temperature);
}

long BMP085_calc_pressure(long up, long ut)
{	
	int32_t  tval, pval;
	int32_t  x1, x2, x3, b3, b5, b6, p;
	uint32_t  b4, b7;
	uint8_t oss = BMP085_PRESSURE_OSS;
	
	x1 = (ut - bmp085.ac6) * bmp085.ac5 >> 15;
	x2 = ((int32_t) bmp085.mc << 11) / (x1 + bmp085.md);
	b5 = x1 + x2;
	tval = (b5 + 8) >> 4;

	b6 = b5 - 4000;
	x1 = (bmp085.b2 * (b6 * b6 >> 12)) >> 11;
	x2 = bmp085.ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = ((((int32_t) bmp085.ac1 * 4 + x3) << oss) + 2) >> 2;
	x1 = bmp085.ac3 * b6 >> 13;
	x2 = (bmp085.b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (bmp085.ac4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = ((uint32_t) up - b3) * (uint32_t) (50000UL >> oss);
	p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	return pval = p + ((x1 + x2 + 3791) >> 4);
}

double BMP085_calc_altitude(double pressure, double p0)
{
	return 44330.0 * ( 1.0 - pow( (pressure / p0), 0.190295) );
}
