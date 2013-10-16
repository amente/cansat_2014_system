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
 *   XPIN11 = <<UNUSED>>
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

#include <xbee_config.h>
#include <types.h>
#include <ctype.h>

#include <British.h>



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

/** 
 * The payload code will be compiled with the -D__PAYLOAD__ macro so if there's a section 
 * that only applies to the payload then surround it with something like:
 * 
 * #ifdef __PAYLOAD__
 * 		// do something
 * #endif
 * 
 */

void main(void)
{
	uint32_t t;
	
	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

	repeat (;;)	
	{
		/* App code starts here */
		
		/* BMP085
		printf("Raw Pressure:");
		pressure = readPressure();
		printf("%d",pressure);
			
		printf("Raw Temperature:");	
		temp = readTemperature();
		printf("%d",temp);
		*/
		
		announce("HELLO WORLD!\n");
		
		repeat(t=0; (t++)<4*1000; sys_udelay(250));
		
		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
