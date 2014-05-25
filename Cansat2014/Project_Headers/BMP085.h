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
#define BMP085_SAMPLE_DELAY         5
 
#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34
 
bool_t BMP085_test(void);
int16_t BMP085_readTemp(void);
int32_t BMP085_readPressure(void);
void BMP085_calibrate(void);
long BMP085_convert_temperature(long);
long BMP085_calc_pressure(long,long);
double BMP085_calc_altitude(double,double);
double BMP085_calc_median_altitude(void);
unsigned long BMP085_calibrate_alt(void);
unsigned long BMP085_get_median_pressure(void);
#define BMP085_init BMP085_calibrate


#pragma pack 1
typedef struct{
        int16_t ac1,ac2,ac3;
        uint16_t ac4,ac5,ac6;
        int16_t b1,b2,mb,mc,md;        
}bmp085_t;

#endif