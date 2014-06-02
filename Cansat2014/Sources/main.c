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
 *   XPIN8 = special0 [BKGD Pin]
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
#include <wpan/types.h>
#include <rtc.h>

typedef struct {
	uint8_t reset_cause;
	uint16_t T0;  // Initial time
	uint16_t pkt_cnt;	
	unsigned long P0;  //Initial pressure
#ifdef __CONTAINER__
	uint8_t armed;
	uint8_t ascending;
	uint8_t released;
	uint8_t umbrella_deployed;
	double max_alt;
	double cur_alt;
#endif
}status_t;

static status_t status;

#define POR_RESET   0xAB //POR Reset indicator 
#define STATUS_RAM_ADR 0x08

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

void send_packet(void);
void post(void);

#ifdef __CONTAINER__
#define RELEASE_ALT  10    // Release at 500m
#define ARM_ALT    10        // Arm umbrella deplyment and release once this alt is reached
#define DESCENT_TRIGGER_DISTANCE 3  // Change cansat state to descent if max_alt-cur_alt above is this value 
#endif

#ifdef irq0_irq
static uint8_t got_irq = 0;
void irq0_irq(void) {
	/* IRQ triggered @ 1Hz */
	got_irq = 1;
}
#endif

void read_status() {
	DS1338_read_RAM(STATUS_RAM_ADR, &status, sizeof(status));
	printf("RESET CAUSE 0x%x",status.reset_cause);
	if (status.reset_cause != POR_RESET) {
#ifdef __DEBUG__
		printf("Calibrating altitude..\r");
#endif	
		status.P0 = BMP085_calibrate_alt();
#ifdef __DEBUG__
		printf("Setting initial time..\r");
#endif	
		status.T0 = DS1338_get_secs();
#ifdef __DEBUG__
		printf("Init pkt_cnt..\r");
#endif	
		status.pkt_cnt = 0;
		
#ifdef __CONTAINER__
		status.armed = 0;
		status.ascending = 1;
		status.released = 0;
		status.umbrella_deployed = 0;
		status.max_alt = 0;
		status.cur_alt = 0;
#endif		
		status.reset_cause = POR_RESET;
		post();		
	}
}

void post(void){
	char test[] = "Hello";	
	char test2[sizeof(test)];
	char i = 0;
	
	char buf[10];	
	buf[0] = 0xFF;
	
	DS1338_write_RAM(0x32,&test,sizeof(test));
	DS1338_read_RAM(0x32,&test2,sizeof(test2));
	if(strcmp(test,test2) ==0){
		buf[1] = 0xFF;
	}else{buf[1] = 0;}
	
	if(BMP085_test()){
		buf[2] = 0xFF;
	}else{buf[2] = 0;}
	
   memset(test2,0,sizeof(test2));	
   eeprom_24xxx_write(EEPROM_0, &test, 0x7FF0, sizeof(test));
   eeprom_24xxx_read(EEPROM_0, &test2, 0x7FF0, sizeof(test2));
   if(strcmp(test,test2) ==0){
   		buf[3] = 0xFF;
   	}else{buf[3] = 0;}
	 
#ifdef __PAYLOAD__
   	   buf[0] = 0x00;
   	   if(TSL2561_test()){
   		   buf[4] = 0xFF;
   	   }else{buf[4] = 0;}
   	   
   	   if(vadc_read()>2800){
   		  buf[5] = 0xFF;
   	   }else{buf[5] = 0;}   	   
#endif
   
  *(unsigned long *)&buf[6] = status.P0;
	printf("POST");
   for(i =0;i<sizeof(buf);i++){
	   printf("%X",buf[i]);
   }
  
}

void write_status(){
	DS1338_write_RAM(STATUS_RAM_ADR, &status, sizeof(status));	
}

#pragma INLINE
void main_setup(void) {
	char buf[48];	
	unsigned long p = 0;
		
#ifdef __CONTAINER__
	 send_buf[STATUS_IDX] |=0xFF00; // If MSB of status is 0xFF it indicates container
	 gpio_set(RELEASE, 0);
	 gpio_set(VMEASURE,0);
#endif
	
	
	DS1338_config();	
	
    #ifdef __DEBUG__
		printf("Reading status... \r");
	#endif
			
	#ifdef __DEBUG__
		printf("Running... \r %d",DS1338_get_secs());
	#endif

	// Set xbee baud rate
	xbee_ser_baudrate(&EMBER_SERIAL_PORT, 9600);
	
#ifdef __PAYLOAD__
	SPMSC2 = 2; // Enable STOP3 MODE
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

	read_status();	  // Read cansat status from RTC RAM	
}

void read_sensors() {

	long ut = BMP085_readTemp();
	long t = BMP085_convert_temperature(ut);
	long pressure = BMP085_get_median_pressure();
	double alt = BMP085_calc_altitude(pressure,status.P0);
	
#ifdef __CONTAINER__
	status.cur_alt = alt;
	if(!status.armed && status.cur_alt > RELEASE_ALT){
		status.armed = 1;
	}
	if(status.cur_alt>status.max_alt){
		status.max_alt=status.cur_alt;
	}
#endif
	
		
#ifdef __PAYLOAD__		
	send_buf[LUM_IDX] = TSL2561_read_raw();	
	send_buf[SOURCE_VOLT_IDX] = vadc_read();	
#endif		
	
	send_buf[MISSION_TIME_IDX] = DS1338_get_secs() - status.T0;
	send_buf[ALT_IDX]= (int16_t) (alt * 10); // Altitude in dm

	send_buf[TEMP_IDX] = (uint16_t)t;		
}

#ifdef __CONTAINER__

void check_descent(){	   
		if(status.ascending && status.cur_alt>0){
			if(status.max_alt - status.cur_alt > DESCENT_TRIGGER_DISTANCE ){
				status.ascending = 0;
			}
		}
}

void check_deploy_umbrella(){		
		    // When descent check 
			if(!status.ascending && !status.umbrella_deployed){
							gpio_set(VMEASURE, 1); // VMEASURE in container is UMBRELLA RELEASE
							status.umbrella_deployed = 1;
							delay(500);
							gpio_set(VMEASURE, 0);							
			}	
}

void check_release(){		
	if (!status.ascending && status.cur_alt < RELEASE_ALT) {
				if (!status.released) {
					gpio_set(RELEASE, 1);
					status.released = 1;
					delay(500);
					gpio_set(RELEASE, 0);					
		}
	}	
}

void rtc_periodic_task(void)
{
	send_packet();
}

#endif

void send_packet() {
	status.pkt_cnt++;
	send_buf[PACKET_COUNT_IDX] = status.pkt_cnt;
	// Put a transmit request to the EMBER 
	xbee_ser_write(&EMBER_SERIAL_PORT, &send_buf, PACKET_SIZE);
	write_status();
	eeprom_24xxx_write(EEPROM_0, &send_buf, PACKET_SIZE*(status.pkt_cnt-1),PACKET_SIZE );
	
#ifdef __DEBUG__
	printf(">");
#endif
}
static uint32_t cur_time, last_time = 0;
#pragma INLINE
void main_loop(void) {
	read_sensors();	
#ifdef __CONTAINER__	
#ifdef __DEBUG__
	printf("Checking release..");
#endif
	if(status.armed ){		
		check_descent();
	}	
	if(!status.umbrella_deployed){
		check_deploy_umbrella();
	}	
	if(!status.released){
		check_release();
	}	
	if ((cur_time = rtc_get_uptime()) > last_time)
	{
		send_packet();
		last_time=cur_time;
	}
	send_buf[STATUS_IDX] |=( (status.umbrella_deployed)?0x00F0:0x0000);
	send_buf[STATUS_IDX] |=( (status.released)?0x000F:0x0000);
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
