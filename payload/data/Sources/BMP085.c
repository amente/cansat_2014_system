/*
 * BMP085.c
 *
 *  Created on: Sep 25, 2013
 *      Author: Amente
 */
#include <types.h>
#include <newi2c.h>
#include <rtc.h>
#include <BMP085.h>

uint8_t oversampling;

int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;

/**
 * Initialize the device, detect if it is responding
 * Set the mode to BMP085_ULTRALOWPOWER to save power, default is high resolution 
 */


uint8_t begin(uint8_t mode){
	  oversampling = mode; 

	  if (read8(0xD0) != 0x55) return 0;

	  /* read calibration data */
	  ac1 = read16(BMP085_CAL_AC1);
	  ac2 = read16(BMP085_CAL_AC2);
	  ac3 = read16(BMP085_CAL_AC3);
	  ac4 = read16(BMP085_CAL_AC4);
	  ac5 = read16(BMP085_CAL_AC5);
	  ac6 = read16(BMP085_CAL_AC6);

	  b1 = read16(BMP085_CAL_B1);
	  b2 = read16(BMP085_CAL_B2);

	  mb = read16(BMP085_CAL_MB);
	  mc = read16(BMP085_CAL_MC);
	  md = read16(BMP085_CAL_MD);
		
	return 1;
}
#define begin()		begin(BMP085_ULTRAHIGHRES)

/**
 * Reads Raw temperature value, no conversion
 */
uint16_t readTemperature(void){	
	write8(BMP085_CONTROL, BMP085_READTEMPCMD);
	delay_ticks(HZ/200);	
    return read16(BMP085_TEMPDATA); 	
}
/**
 * Reads Raw pressure value, no conversion
 */
uint32_t readPressure(void){
	  uint32_t raw;

	  write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6)); // Set the control to Pressure

	  if (oversampling == BMP085_ULTRALOWPOWER) 
	    delay_ticks(HZ/200); // 5 ms  
	  else if (oversampling == BMP085_STANDARD) 
	    delay_ticks(HZ/125); // 8 ms
	  else if (oversampling == BMP085_HIGHRES) 
	    delay_ticks(HZ/71);  // 14 ms
	  else 
	    delay_ticks(HZ/38); // 26 ms

	  raw = read16(BMP085_PRESSUREDATA);

	  raw <<= 8;
	  raw |= read8(BMP085_PRESSUREDATA+2);
	  raw >>= (8 - oversampling);
	 
      return raw;
}


uint8_t read8(const uint8_t addr){
	
	  uint8_t ret;

	  i2c_set_addr(BMP085_I2CADDR); // start transmission to device
	
	  newi2c_wr_byte(addr); // sends register address to read from
	
	  i2c_stop(); // end transmission
	  
	  i2c_set_addr(BMP085_I2CADDR);
	  
	  ret = i2c_rd_byte(0);// read a byte no ack
	
	  i2c_stop(); // end transmission

	  return ret;
	
}

uint16_t read16(const uint8_t addr){
	
	uint8_t bytesRead[2];
	uint16_t ret;

	i2c_set_addr(BMP085_I2CADDR); // start transmission to device
		
	newi2c_wr_byte(addr); // sends register address to read from
		
	i2c_stop(); // end transmission
		  
	i2c_set_addr(BMP085_I2CADDR);
	
	i2c_read(bytesRead,2);// read two bytes
	
	i2c_stop(); // end transmission

	ret = bytesRead[0];
	ret<<=8;
	ret |=bytesRead[1];	
	
	return ret;
		
}

void write8(const uint8_t addr, const uint8_t data){
	
	i2c_set_addr(BMP085_I2CADDR);
	
	newi2c_wr_byte(addr);  //  send register address to write to
	newi2c_wr_byte(data);  // send the data
		
	i2c_stop();
}


