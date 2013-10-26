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
 *   XPIN6 = special0 [RSSI PWM Pin]
 *   XPIN7 = i2c0 [SDA Pin]
 *   XPIN8 = special0 [BKGD Pin]
 *   XPIN9 = <<UNUSED>>
 *   XPIN10 = GND
 *   XPIN11 = <<UNUSED>>
 *   XPIN12 = <<UNUSED>>
 *   XPIN13 = <<UNUSED>>
 *   XPIN14 = VCC REF
 *   XPIN15 = special0 [Association Pin]
 *   XPIN16 = <<UNUSED>>
 *   XPIN17 = <<UNUSED>>
 *   XPIN18 = <<UNUSED>>
 *   XPIN19 = i2c0 [SCL Pin]
 *   XPIN20 = special0 [Commissioning Pin]
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
#include <types.h>
#include <ctype.h>
#include <util.h>
#include <24xxx.h>
#include <BMP085.h>

#if defined(RTC_ENABLE_PERIODIC_TASK)
void rtc_periodic_task(void)
{
    /*
     * Function call every RTC_CFG_PERIODIC_TASK_PERIOD * 8 ms.
     * This function is called from the timer ISR, please be brief
     * and exit, or just set a flag and do your home work in the 
     * main loop
     */

    /* Implement your code here */
}
#endif

//float temp;
//uint32_t pressure;

void main(void)
{
	unsigned char buffer[32];
	ssize_t rc;
	
	sys_hw_init();
	sys_xbee_init();
	//sys_app_banner();
	printf("\rCompiled Time: %s %s\r", __DATE__, __TIME__);
	
	
	for(;;)	
	{
		rc=eeprom_24xxx_read(EEPROM_0, buffer, 0, 25);
		printf("RC=%d Data=%s\r", rc, buffer);
		
		
		//printf("Temperature: ");
		//temp = BMP085readTemperature();
		//printf("%f\n",temp);
		
		//printf("Pressure: ");
		//pressure = BMP085readRawTemperature();
		//printf("%d\n",pressure);
		
		delay(1000);  // delay 1s
		//sys_watchdog_reset();
		//sys_xbee_tick();
	}
}
