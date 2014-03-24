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
 *   XPIN12 = <<UNUSED>>
 *   XPIN13 = power_management0 [On Sleep Pin]
 *   XPIN14 = VCC REF
 *   XPIN15 = <<UNUSED>>
 *   XPIN16 = <<UNUSED>>
 *   XPIN17 = <<UNUSED>>
 *   XPIN18 = <<UNUSED>>
 *   XPIN19 = i2c0 [SCL Pin]
 *   XPIN20 = adc0 [ADC Pin]
 *
 ************************************/

/** 
 * The payload code will be compiled with the -D__PAYLOAD__ macro so if there's a section 
 * that only applies to the payload then surround it with something like:
 * 
 * #ifdef __PAYLOAD__
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

// Team ID is always the first 2 bits in a packet
#define TEAMID 0x2305

// Indices of other values in the packet send buffer
#define PACKET_COUNT_IDX 1
#define MISSION_TIME_IDX 2
#define ALT_IDX 3
#define TEMP_IDX 4
#define SOURCE_VOLT_IDx 5
#define LUX_IDX 6

#define PACKET_SIZE 14 // 14 Bytes in total 7*16 bits

// The send buffer
static uint16_t send_buf[PACKET_SIZE] = {TEAMID,0,0,0,0,0,0};

uint16_t CANSAT_UPTIME = 0;
uint16_t CANSAT_PACKET_COUNT  = 0;

void send_packet(){	
	send_buf[ALT_IDX] = BMP085_readPressure();
	send_buf[TEMP_IDX] = BMP085_readTemp();	
	
	CANSAT_PACKET_COUNT++;
	send_buf[PACKET_COUNT_IDX] =CANSAT_PACKET_COUNT;
	xbee_ser_write(&EMBER_SERIAL_PORT, &send_buf,PACKET_SIZE);	
}

#ifdef irq0_irq
static uint8_t got_irq = 0;
void irq0_irq(void)
{
    /* IRQ triggered @ 1Hz */
	got_irq = 1;
}
#endif

char buf[20] = 0;

//#pragma INLINE
void main_setup(void)
{
	// Set xbee baud rate
	xbee_ser_baudrate(&EMBER_SERIAL_PORT, 9600);	
	SPMSC2 = 2;  // Enable STOP3 MODE
	printf("Running... FP: %f\n", 3.14);
	if (BMP085_test()) printf("Works!\r");
	else printf("No Works!\r");
	
	BMP085_init();

	
	//while(1);
	
	
	
	
	CANSAT_UPTIME = DS1338_get_secs();
}

#pragma INLINE
void main_loop(void)
{
	long ut = BMP085_readTemp();
	long up = BMP085_readPressure();
	printf("Raw Temp: %ld Temp = %ld\r", ut,bmp085_convert_temperature(ut));
	
	printf("Raw Pressure: %ld Pressure = %lu\r",up, bmp085_calc_pressure(up,ut));
	//delay(5);
	//send_packet();
	//printf("TSL2561: %d\r", TSL2561_read_raw());
}

#pragma INLINE
void main_stop_start(void)
{
	pm_set_radio_mode(PM_MODE_STOP);
	do asm stop;
	while(!got_irq);	// debounce; stop until there's an actual IRQ
	++CANSAT_UPTIME;	// increment @ 1HZ
	got_irq = 0;
	pm_set_radio_mode(PM_MODE_RUN);
}


void main(void)
{	
	sys_hw_init();
	//sys_xbee_init();
	

#ifdef __DEBUG__
	printf("\rCompiled on: %s %s\r", __DATE__, __TIME__);
#endif

	main_setup();

	for(;;)	
	{	
		main_loop();		
		sys_xbee_tick();
		//CANSAT_PACKET_COUNT = CANSAT_UPTIME+1;  // for now...		
		//main_stop_start();
		
	}
}
