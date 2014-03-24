/*
 * BMP085.h
 *
 *  Created on: Sep 25, 2013
 *      Author: Amente
 *                      Spave
 */
 
#ifndef BMP085_H_
#define BMP085_H_
 
#define BMP085_I2CADDR		0x77
#define BMP085_CHIP_REG		0xD0
#define BMP085_CHIP_ID		0x55
 
 
#define BMP085_PRESSURE_OSS         0
#define BMP085_SAMPLE_DELAY         5//26
 
#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34
 
bool_t BMP085_test(void);
int16_t BMP085_readTemp(void);
int32_t BMP085_readPressure(void);
void BMP085_calibrate(void);
long bmp085_convert_temperature(long);
long bmp085_calc_pressure(long,long);
void BMP085_init(void);
 
#endif