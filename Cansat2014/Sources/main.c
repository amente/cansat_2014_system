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
#include <types.h>
#include <ctype.h>
#include <drivers.h>
#include <util.h>

void testSetup(void);
void testLoop(void);

#ifdef irq0_irq
static uint8_t got_irq = 0;
void irq0_irq(void)
{
    /* IRQ triggered @ 1Hz */
	got_irq = 1;
}
#endif

#pragma INLINE
void main_setup(void)
{
	SPMSC2 = 2;  // Enable STOP3 MODE
	if(DS1307_config())
		CANSAT_UPTIME = DS1307_get_secs();
}

#pragma INLINE
void main_loop(void)
{
	testLoop();
}

#pragma INLINE
void main_stop_start(void)
{
	pm_set_radio_mode(PM_MODE_STOP);
	while(!got_irq)
		asm stop;  // debounce; stop until there's an actual IRQ
	got_irq = 0;
	pm_set_radio_mode(PM_MODE_RUN);
}

uint16_t CANSAT_UPTIME = 0;
uint16_t CANSAT_PACKET_COUNT  = 0;
void main(void)
{	
	sys_hw_init();
	sys_xbee_init();
	//sys_app_banner();

#ifdef __DEBUG__
	printf("\rCompiled on: %s %s\r", __DATE__, __TIME__);
#endif

	main_setup();

	for(;;)	
	{	
		main_loop();

		sys_xbee_tick();
		++CANSAT_UPTIME;	// increment @ 1HZ
		CANSAT_PACKET_COUNT = CANSAT_UPTIME;  // for now...

		main_stop_start();
		//sys_watchdog_reset();
	}
}
