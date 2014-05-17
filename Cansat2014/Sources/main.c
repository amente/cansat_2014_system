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
 *   XPIN12 = Release [GPIO Pin]
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

#define __DEBUG__

// Team ID is always the first 2 bits in a packet
#define TEAMID 0x2305

// Indices of other values in the packet send buffer
#define PACKET_COUNT_IDX 1
#define MISSION_TIME_IDX 2
#define ALT_IDX 3
#define TEMP_IDX 4
#define SOURCE_VOLT_IDX 5
#define LUX_IDX 6
#define LUX_IR_IDX 7
#define STATUS_IDX 8

#define PACKET_SIZE 18 // 18 Bytes in total 9*16 bits
// The packet send buffer
static uint16_t send_buf[PACKET_SIZE / 2] = { TEAMID, 0, 0, 0, 0, 0, 0, 0, 0 };

uint16_t CANSAT_UPTIME = 0;
uint16_t CANSAT_PACKET_COUNT = 0;

#ifdef __CONTAINER__
#define RELEASE_ALT  500 // Release at 500m
int ascending = 1;
int released = 0;
int umbrella_deployed = 0;
int num_descent_check = 3;
static double alt_buf[3];
#endif

#ifdef irq0_irq
static uint8_t got_irq = 0;
void irq0_irq(void) {
	/* IRQ triggered @ 1Hz */
	got_irq = 1;
}
#endif

static unsigned long P0 = 0;
unsigned long get_avg_pressure(uint8_t);

static unsigned long p_buf[10];
unsigned long do_lowpass(unsigned long p) {
	char i;
	static index = 0;
	unsigned long p_sum = 0;

	p_buf[index++] = p;
	if (index > 9)
		index = 0;
	for (i = 0; i < 10; i++)
		p_sum += p_buf[i];
	return p_sum / 10;
}

#pragma INLINE
void main_setup(void) {
	char buf[48];
	char num;
	char i;
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
	printf("Test %f\r",5.0/3);
#endif
	
	P0 = get_avg_pressure(10);
	
#ifdef __DEBUG__	
	printf("P0 = %lu\r", P0);
#endif

	for (i = 0; i < 10; i++) {
		p_buf[i] = 0;
	}

#ifdef __CONTAINER__
	//send_buf[STATUS_IDX] |=0xFF00; // If MSB of status is 0xFF it indicates container
	//gpio_set(RELEASE, 0);
	//gpio_set(VMEASURE,0);
#endif
}

unsigned long get_avg_pressure(uint8_t n) {
	char i;
	unsigned long ut = BMP085_readTemp();
	unsigned long up = BMP085_readPressure();
	unsigned long p = 0;
	for (i = 0; i < n; i++) {
		p += BMP085_calc_pressure(up, ut);
		ut = BMP085_readTemp();
		up = BMP085_readPressure();
	}
	return p / n;
}

void read_sensors() {

	long ut = BMP085_readTemp();
	long t = BMP085_convert_temperature(ut);
	long pressure = get_avg_pressure(10);
	double alt = BMP085_calc_altitude(pressure, P0);
	uint_16 lux_r, IRlux_r, lux, IRlux;
	
#ifdef __PAYLOAD__
	//TODO: Light Sensor
	TSL2561_read_raw(&lux_r, &IRlux_r);
	send_buf[LUX_IDX] = lux_r;
	send_buf
	send_buf[SOURCE_VOLT_IDX] = vadc_read();	
#endif	
	
	send_buf[MISSION_TIME_IDX] = DS1338_get_secs() - CANSAT_UPTIME;
	send_buf[ALT_IDX] = (long) (alt * 100); // Altitude in 100m
	send_buf[TEMP_IDX] = t;		
}

#ifdef __CONTAINER__
void check_release(){
	unsigned long pressure ;
	double alt;
	int i;
			
	//Take three altitude readings in 100ms interval
	for(i=0;i<3;i++){
		pressure = get_avg_pressure(10);
		alt = BMP085_calc_altitude(pressure, P0);	
		alt_buf[i] = alt;		
		delay(100);
	}
	
	// Compare reading to detect decrease pattern
	if(alt_buf[1]<alt_buf[0] && alt_buf[2]<alt_buf[1]){
		num_descent_check--; // To prevent noise errors , do this num_descent_check times
		if (num_descent_check == 0) {
				// When descent check 
				if(!umbrella_deployed){
					gpio_set(VMEASURE, 1); // VMEASURE in container is UMBRELLA RELEASE
					umbrella_deployed = 1;
					delay(500);
					gpio_set(VMEASURE, 0);
					send_buf[STATUS_IDX] |= 0x00F0; // when first 4 bits of the LSB are set it indicates umbrella deployed  
				}
				ascending = 0;
		}
	}else{
		num_descent_check = 3;
	}	
			
		
	if (!ascending && alt < RELEASE_ALT) {
				if (!released) {
					gpio_set(RELEASE, 1);
					released = 1;
					delay(500);
					gpio_set(RELEASE, 0);
					send_buf[STATUS_IDX] |= 0x000F; // when last 4 bits of the LSB are set it indicates payload released  
		}
	}	
}
#endif

void send_packet() {
	CANSAT_PACKET_COUNT++;
	send_buf[PACKET_COUNT_IDX] = CANSAT_PACKET_COUNT;
	// Put a transmit request to the EMBER 
	xbee_ser_write(&EMBER_SERIAL_PORT, &send_buf, PACKET_SIZE);
#ifdef __DEBUG__
	printf("Sent Packet %d\r",CANSAT_PACKET_COUNT);
#endif

}

#pragma INLINE
void main_loop(void) {
	read_sensors();	
#ifdef __CONTAINER__	
#ifdef __DEBUG__
	printf("Checking release..");
#endif
	check_release();
	delay(200);
#endif
	send_packet();
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
