/*
 * BMP085.c
 *
 *  Created on: Sep 25, 2013
 *      Author: Amente
 */
#include <types.h>
#include <math.h>
#include <i2c.h>
#include <rtc.h>
#include <BMP085.h>

uint8_t oversampling;

int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;

/**
 * Initialize the device, detect if it is responding
 * Set the mode to BMP085_ULTRALOWPOWER to save power, default is high resolution 
 */


uint8_t BMP085begin(uint8_t mode){
	  oversampling = mode; 

	  //if (BMP085read8(0xD0) != 0x55) return 0;

	  /* read calibration data */
	  ac1 = BMP085read16(BMP085_CAL_AC1);
	  ac2 = BMP085read16(BMP085_CAL_AC2);
	  ac3 = BMP085read16(BMP085_CAL_AC3);
	  ac4 = BMP085read16(BMP085_CAL_AC4);
	  ac5 = BMP085read16(BMP085_CAL_AC5);
	  ac6 = BMP085read16(BMP085_CAL_AC6);

	  b1 = BMP085read16(BMP085_CAL_B1);
	  b2 = BMP085read16(BMP085_CAL_B2);

	  mb = BMP085read16(BMP085_CAL_MB);
	  mc = BMP085read16(BMP085_CAL_MC);
	  md = BMP085read16(BMP085_CAL_MD);
		
	return 1;
}

/**
 * Reads Raw temperature value, no conversion
 */
uint16_t BMP085readRawTemperature(void){	
	BMP085write8(BMP085_CONTROL, BMP085_READTEMPCMD);
	delay_ticks(HZ/200);	
    return BMP085read16(BMP085_TEMPDATA); 	
}
/**
 * Reads Raw pressure value, no conversion
 */
uint32_t BMP085readRawPressure(void){
	  uint32_t raw;

	  //printf("Writing control data \n");
	  BMP085write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6)); // Set the control to Pressure
      //printf("Successfully wrote control data \n");
	  if (oversampling == BMP085_ULTRALOWPOWER) 
	    delay_ticks(HZ/200); // 5 ms  
	  else if (oversampling == BMP085_STANDARD) 
	    delay_ticks(HZ/125); // 8 ms
	  else if (oversampling == BMP085_HIGHRES) 
	    delay_ticks(HZ/71);  // 14 ms
	  else 
	    delay_ticks(HZ/38); // 26 ms
      
	  //printf("Reading from sensor \n");
	  raw = BMP085read16(BMP085_PRESSUREDATA);
	  //printf("Read successful \n");

	  raw <<= 8;
	  raw |= BMP085read8(BMP085_PRESSUREDATA+2);
	  raw >>= (8 - oversampling);
	 
      return raw;
}
/*
float BMP085readTemperature(void) {
  int32_t UT, X1, X2, B5;     // following ds convention
  float temp;

  UT = BMP085readRawTemperature();


  // use datasheet numbers!
  UT = 27898;
  ac6 = 23153;
  ac5 = 32757;
  mc = -8711;
  md = 2868;

  // step 1
  X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) / pow(2,15);
  X2 = ((int32_t)mc * pow(2,11)) / (X1+(int32_t)md);
  B5 = X1 + X2;
  temp = (B5+8)/pow(2,4);
  temp /= 10;
  
  return temp;
}
*/


uint8_t BMP085read8(uint8_t addr){	
	  	  
	  uint8_t data;

	  i2c_set_addr(BMP085_I2CADDR); // start transmission to device
	
	  i2c_write(&addr,1); // sends register address to read from	  
	  	  
	  i2c_read(&data,1);// read a byte no ack	  

	  //printf("Read8 data %X \n",data);
	  return data;
	
}

uint16_t BMP085read16(uint8_t addr){
	uint16_t ret;
	uint8_t bytesRead[2];
	
	i2c_set_addr(BMP085_I2CADDR); // start transmission to device
		
	i2c_write(&addr,1); // sends register address to read from	
			
	i2c_read(bytesRead,2);// read two bytes	

	ret = bytesRead[0];
	ret<<=8;
	ret |=bytesRead[1];	
	
	//printf("Read16 data %X \n",ret);
	return ret;
		
}

void BMP085write8(uint8_t addr, uint8_t data){	
	i2c_set_addr(BMP085_I2CADDR);
	i2c_write(&addr,1);
	i2c_write(&data,1);
}


