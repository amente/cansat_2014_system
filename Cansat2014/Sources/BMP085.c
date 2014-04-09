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

/*
typedef struct{
        int16_t ac1,ac2,ac3,b1,b2,mb,mc,md;
        uint16_t ac4,ac5,ac6;  
        long b5;
        short oversampling_setting;
}p_bmp085;
 */

#pragma pack(1)
typedef struct{
        int16_t ac1,ac2,ac3;
        uint16_t ac4,ac5,ac6;
        int16_t b1,b2,mb,mc,md;        
        short oversampling_setting;
}p_bmp085;



static p_bmp085 bmp085;
 
void BMP085_init(void){
        printf("Initializing BMP085\n");
        BMP085_calibrate();  
        bmp085.oversampling_setting = BMP085_PRESSURE_OSS;
}

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
	/*
                printf("ac1: %d\r",bmp085.ac1);
                printf("ac2: %d\r",bmp085.ac2);
                printf("ac3: %d\r",bmp085.ac3);
               
                printf("ac4: %u\r",bmp085.ac4);
                printf("ac5: %u\r",bmp085.ac5);
                printf("ac6: %u\r",bmp085.ac6);
               
                printf("b1: %d\r",bmp085.b1);
                printf("b2: %d\r",bmp085.b2);
                printf("mb: %d\r",bmp085.mb);
                printf("mc: %d\r",bmp085.mc);
                printf("md: %d\r",bmp085.md); */
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
	/*
   long pressure;
   long x1,x2,x3,b3,b6,b5,b7;
   unsigned long b4;
   
   x1 = (((long) ut - (long) bmp085.ac6) * (long) bmp085.ac5) >> 15;
   x2 = ((long) bmp085.mc << 11) / (x1 + (long)bmp085.md);
   b5 = x1 + x2;
   
   b6 = b5 - 4000;
   //*****calculate B3************
   x1 = (b6*b6) >> 12;          
   x1 *= bmp085.b2;
   x1 >>=11;
 
   x2 = (bmp085.ac2*b6);
   x2 >>=11;
 
   x3 = x1 +x2;
 
        b3 = (((((long)bmp085.ac1 )*4 + x3) <<bmp085.oversampling_setting) + 2) >> 2;
 
   //*****calculate B4************
   x1 = (bmp085.ac3* b6) >> 13;
   x2 = (bmp085.b1 * ((b6*b6) >> 12) ) >> 16;
   x3 = ((x1 + x2) + 2) >> 2;
   b4 = (bmp085.ac4 * (unsigned long) (x3 + 32768)) >> 15;
     
   b7 = ((unsigned long)(up - b3) * (50000>>bmp085.oversampling_setting));  
   if (b7 < 0x80000000)
   {
     pressure = (b7 << 1) / b4;
   }
   else
   {
     pressure = (b7 / b4) << 1;
   }
   
   x1 = pressure >> 8;
   x1 *= x1;
   x1 = (x1 * 3038) >> 16;
   x2 = (pressure * -7357) >> 16;
   pressure += (x1 + x2 + 3791) >> 4;        // pressure in Pa  
 
   return (pressure);
   */
	int32_t  tval, pval;
	int32_t  x1, x2, x3, b3, b5, b6, p;
	uint32_t  b4, b7;
	uint8_t oss = BMP085_PRESSURE_OSS;

	/*printf("x1=%ld\r",x1 = (ut - bmp085.ac6) * bmp085.ac5 >> 15);
	printf("x2=%ld\r",x2 = ((int32_t ) bmp085.mc << 11) / (x1 + bmp085.md));
	printf("b5=%ld\r",b5 = x1 + x2);
	printf("tval=%ld\r",tval = (b5 + 8) >> 4);
	
	printf("\rb6=%ld\r",b6 = b5 - 4000);
	printf("x1=%ld\r",x1 = (bmp085.b2 * (b6 * b6 >> 12)) >> 11); 
	printf("x2=%ld\r",x2 = bmp085.ac2 * b6 >> 11);
	printf("x3=%ld\r",x3 = x1 + x2);
	printf("b3=%ld\r",b3 = ((((int32_t ) bmp085.ac1 * 4 + x3)<<oss) + 2) >> 2);
	printf("x1=%ld\r",x1 = bmp085.ac3 * b6 >> 13);
	printf("x2=%ld\r",x2 = (bmp085.b1 * (b6 * b6 >> 12)) >> 16);
	printf("x3=%ld\r",x3 = ((x1 + x2) + 2) >> 2);
	printf("b4=%lu\r",b4 = (bmp085.ac4 * (uint32_t ) (x3 + 32768)) >> 15);
	printf("b7=%lu\r",b7 = ((uint32_t ) up - b3) * (uint32_t)(50000UL >> oss));
	printf("p=%ld\r",p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2);

	printf("x1=%ld\r",x1 = (p >> 8) * (p >> 8));
	printf("x1=%ld\r",x1 = (x1 * 3038) >> 16);
	printf("x2=%ld\r",x2 = (-7357 * p) >> 16);
	return pval = p + ((x1 + x2 + 3791) >> 4);*/
	
	
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
