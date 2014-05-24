/***** XBEE APPLICATION PROJECT *****
 * 
 * Auto-generated header with information about the 
 * relation between the XBee module pins and the 
 * project components.
 * 
 ************ XBEE LAYOUT ***********
 * 
 * This layout represents the XBee ZB (S2B) module 
 * selected for the project with its pin distribution:
 *               _________________
 *              /     ________    \ 
 *             /     |   __   |    \ 
 *            /      | //  \\ |     \ 
 *   XPIN1  -|       | \\__// |      |- XPIN20
 *   XPIN2  -|       |________|      |- XPIN19
 *   XPIN3  -|                       |- XPIN18
 *   XPIN4  -| ===================== |- XPIN17
 *   XPIN5  -| #   # ####  #### #### |- XPIN16
 *   XPIN6  -|  # #  #   # #    #    |- XPIN15
 *   XPIN7  -|   #   ####  ###  ###  |- XPIN14
 *   XPIN8  -|  # #  #   # #    #    |- XPIN13
 *   XPIN9  -| #   # ####  #### #### |- XPIN12
 *   XPIN10 -| ===================== |- XPIN11
 *           |_______________________|
 * 
 ************ PINS LEGEND ***********
 * 
 * The following list displays all the XBee Module pins 
 * with the project component which is using each one:
 * 
 *   XPIN1 = VCC
 *   XPIN2 = uart0 [TX Pin]
 *   XPIN3 = uart0 [RX Pin]
 *   XPIN4 = <<UNUSED>>
 *   XPIN5 = special0 [Reset Pin]
 *   XPIN6 = <<UNUSED>>
 *   XPIN7 = i2c0 [SDA Pin]
 *   XPIN8 = <<UNUSED>>
 *   XPIN9 = power_management0 [Sleep Request Pin]
 *   XPIN10 = GND
 *   XPIN11 = irq0 [IRQ Pin]
 *   XPIN12 = RELEASE [GPIO Pin]
 *   XPIN13 = power_management0 [On Sleep Pin]
 *   XPIN14 = VCC REF
 *   XPIN15 = <<UNUSED>>
 *   XPIN16 = <<UNUSED>>
 *   XPIN17 = <<UNUSED>>
 *   XPIN18 = VMEASURE [ADC Pin]
 *   XPIN19 = i2c0 [SCL Pin]
 *   XPIN20 = <<UNUSED>>
 *
 ************************************/

/** 
 * The container code will be compiled with the -D__CONTAINER__ macro so if there's a section 
 * that only applies to the container then surround it with something like:
 * 
 * #ifdef __CONTAINER__
 * 		// do something
 * #endif
 * 
 */
#include <xbee_config.h>
#include <ctype.h>
#include <stdlib.h>
#include <drivers.h>
#include <util.h>
#include <BMP085.h>
#include <TSL2561.h>
#include <i2c.h>
#include <xbee/serial.h>
#include <math.h>
#include <rtc.h>

// Team ID is always the first 2 bits in a packet
#define TEAMID 0x2305

// Indices of other values in the packet send buffer
#define PACKET_COUNT_IDX 1
#define MISSION_TIME_IDX 2
#define ALT_IDX 3
#define TEMP_IDX 4
#define SOURCE_VOLT_IDX 5
#define LUM_IDX 6
#define STATUS_IDX 7

#define PACKET_SIZE 16 // 16 Bytes in total 8*16 bits
// The packet send buffer
static uint16_t send_buf[PACKET_SIZE / 2] = { TEAMID, 0, 0, 0, 0, 0, 0, 0};

uint16_t CANSAT_UPTIME = 0;
uint16_t CANSAT_PACKET_COUNT = 0;
void send_packet(void);

#ifdef __CONTAINER__
#define RELEASE_ALT  150       // Release at 500m
#define ARM_ALT    100          // Arm umbrella deplyment and release once this alt is reached
#define DESCENT_TRIGGER_DISTANCE 10  // Change cansat state to descent if max_alt-cur_alt is this value 
int armed = 0;
int ascending = 1;
int released = 0;
int umbrella_deployed = 0;
double max_alt = 0;
double cur_alt = 0;
#endif

#ifdef irq0_irq
static uint8_t got_irq = 0;
void irq0_irq(void) {
	/* IRQ triggered @ 1Hz */
	got_irq = 1;
}
#endif

#pragma INLINE
void main_setup(void) {
	char buf[48];	
	unsigned long p = 0;

	// Set xbee baud rate
	xbee_ser_baudrate(&EMBER_SERIAL_PORT, 9600);
	
#ifdef __PAYLOAD__
	SPMSC2 = 2; // Enable STOP3 MODE
#endif

	DS1338_config();
	CANSAT_UPTIME = DS1338_get_secs();
#ifdef __DEBUG__
	printf("Running... %d\r", CANSAT_UPTIME);
#endif

	eeprom_24xxx_read(EEPROM_0, &buf, 12, 7);

#ifdef __DEBUG__
	printf("EEPROM: %s\r", buf);
#endif

	BMP085_init();

#ifdef __DEBUG__
	if(BMP085_test()) printf("BMP085 OK!\r");
	else printf("BMP085 FAIL!\r");
#endif

#ifdef __DEBUG__
	printf("Calibrating altitude..\r");
#endif
	
	BMP085_calibrate_alt();

#ifdef __CONTAINER__
	 send_buf[STATUS_IDX] |=0xFF00; // If MSB of status is 0xFF it indicates container
	 gpio_set(RELEASE, 0);
	 gpio_set(VMEASURE,0);
#endif
}



void read_sensors() {

	long ut = BMP085_readTemp();
	long t = BMP085_convert_temperature(ut);
	long pressure = BMP085_get_median_pressure();
	double alt = BMP085_calc_altitude(pressure);
	
#ifdef __CONTAINER__
	cur_alt = alt;
	if(!armed && cur_alt > RELEASE_ALT){
		armed = 1;
	}
	if(cur_alt>max_alt){
		max_alt=cur_alt;
	}
#endif
	
#ifdef __DEBUG__
	printf("PRESSURE %d\r",pressure)	;	
#endif	
		
#ifdef __PAYLOAD__		
	send_buf[LUM_IDX] = TSL2561_read_raw();	
	send_buf[SOURCE_VOLT_IDX] = vadc_read();	
#endif		
	
	send_buf[MISSION_TIME_IDX] = DS1338_get_secs() - CANSAT_UPTIME;
	send_buf[ALT_IDX]= (int16_t) (alt * 10); // Altitude in dm
#ifdef __DEBUG__
	printf("ALT %d\r",send_buf[ALT_IDX]);
	
#endif
	send_buf[TEMP_IDX] = (uint16_t)t;		
}

#ifdef __CONTAINER__

void check_descent(){	   
		if(ascending){
			if(max_alt - cur_alt > DESCENT_TRIGGER_DISTANCE ){
				ascending = 0;
			}
		}
}

void check_deploy_umbrella(){		
		    // When descent check 
			if(!ascending && !umbrella_deployed){
							gpio_set(VMEASURE, 1); // VMEASURE in container is UMBRELLA RELEASE
							umbrella_deployed = 1;
							delay(500);
							gpio_set(VMEASURE, 0);
							send_buf[STATUS_IDX] |= 0x00F0; // when first 4 bits of the LSB are set it indicates umbrella deployed  
			}	
}

void check_release(){		
	if (!ascending && cur_alt < RELEASE_ALT) {
				if (!released) {
					gpio_set(RELEASE, 1);
					released = 1;
					delay(500);
					gpio_set(RELEASE, 0);
					send_buf[STATUS_IDX] |= 0x000F; // when last 4 bits of the LSB are set it indicates payload released  
		}
	}	
}

void rtc_periodic_task(void)
{
	send_packet();
}

#endif

void send_packet() {
	CANSAT_PACKET_COUNT++;
	send_buf[PACKET_COUNT_IDX] = CANSAT_PACKET_COUNT;
	// Put a transmit request to the EMBER 
	xbee_ser_write(&EMBER_SERIAL_PORT, &send_buf, PACKET_SIZE);
#ifdef __DEBUG__
	printf(">");
#endif

}

#pragma INLINE
void main_loop(void) {
	read_sensors();	
#ifdef __CONTAINER__	
#ifdef __DEBUG__
	printf("Checking release..");
#endif
	if(armed ){		
		check_descent();
	}	
	if(!umbrella_deployed){
		check_deploy_umbrella();
	}	
	if(!released){
		check_release();
	}		
#endif
#ifdef __PAYLOAD__
	send_packet(); // Send packet in payload, container is scheduled with interrupt
#endif	
}

#pragma INLINE
void main_stop_start(void) {
	pm_set_radio_mode(PM_MODE_STOP);
	do
		asm stop;
	while (!got_irq); // debounce; stop until there's an actual IRQ	
	got_irq = 0;
	pm_set_radio_mode(PM_MODE_RUN);
}

void main(void) {
	sys_hw_init();
#ifdef __CONTAINER__
  rtc_set_periodic_task_period(250);
#endif
#ifdef __DEBUG__	
	printf("\rCompiled on: %s %s\r", __DATE__, __TIME__);
#endif

	main_setup();
	for (;;) {
		main_loop();
		sys_xbee_tick();
#ifdef __PAYLOAD__
		main_stop_start();
#endif
	}
}
