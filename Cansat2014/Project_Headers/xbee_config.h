/* 
 * This is an auto-generated file based on the 
 * configuration of the XBee Programmable project.
 * 
 * Do not edit this file.
 */

#ifndef __XBEE_CONFIG_H_
#define __XBEE_CONFIG_H_

/* Project definitions */

#define CONFIG_XBEE_ZB
#define CONFIG_XBEE_THT
#define CONFIG_XBEE_S2B
#define CONFIG_XBEE_FLASH_LEN           32

/* system0 component */
#define SYS_CFG_CLK_16_MHz
#define SYS_CFG_BUSCLK_SYSCLK_DIV2

/* power_management0 component */
#define ENABLE_POWER_MANAGEMENT
#define ENABLE_SLEEP_RQ_XPIN_9
#define ENABLE_ON_SLEEP_XPIN_13
#define XBEE_ATCMD_PARAM_SM             1
#define POWER_MANAGEMENT_KEEP_UPTIME    0
#define POWER_MANAGEMENT_ENABLE_LVD     0

/* special0 component */
#define ENABLE_RESET_PIN_XPIN_5

/* rtc0 component */
#define ENABLE_RTC

/* network0 component */

/* uart0 component */
#define ENABLE_UART
#define UART_CFG_MODE_2W                1
#define UART_CFG_BAUDRATE               9600
#define UART_CFG_PAR_EN                 UART_CFG_PARITY_DIS
#define UART_CFG_PAR_VAL                UART_CFG_PARITY_ODD
#define UART_CFG_BITS                   UART_CFG_BITS_8
#define UART_CFG_RX_WATERMARK           1
#define UART_CFG_RX_BUF_LEN             32
#define ENABLE_STDIO_PRINTF_SCANF       1

/* i2c0 component */
#define ENABLE_I2C

/* eep24xxx0 component */
#define EEPROM_24XXX_0                  eep24xxx0
#define eep24xxx0                       EEPROM_0
#define EEPROM_24XXX_SLAVE_SUBADDRESS_0 0
#define EEPROM_24XXX_ADDRESS_BYTES_0    2
#define EEPROM_24XXX_PAGE_SIZE_0        64

/* adc_config0 component */
#define ENABLE_ADC
#define ADC_CFG_MODE_BITS               ADC_CFG_MODE_12BIT
#define ADC_CFG_OPTIMIZE                ADC_CFG_OPTIMIZE_FOR_POWER

/* irq0 component */
#define ENABLE_GPIO_IRQ_XPIN_11
#define irq0                            XPIN_11
#define irq0_irq                        gpio_irq_handler_xpin_11
#define GPIO_CFG_IRQ_MODE_11            GPIO_CFG_EDGE_IRQ
#define GPIO_IRQ_LEVEL_EDGE_11          GPIO_IRQ_EDGE_FALL
#define GPIO_IRQ_CFG_PULL_11            GPIO_IRQ_CFG_PULL_HIGH
#define orPD11                          shift_xpin_11
#define andPD11                         ~0
#define orPR11                          shift_xpin_11
#define andPR11                         ~0

/* RELEASE component */
#define ENABLE_GPIO_XPIN_12
#define RELEASE                         XPIN_12
#define GPIO_CFG_DIR_12                 GPIO_CFG_OUTPUT
#define GPIO_CFG_PULL_UP_EN_12          GPIO_CFG_PULL_UP_EN
#define GPIO_CFG_SLEW_RATE_EN_12        GPIO_CFG_SLEW_RATE_EN
#define GPIO_CFG_DRV_STR_12             GPIO_CFG_DRV_STR_HIGH

/* VMEASURE component */
#define ENABLE_ADC_XPIN_18
#define VMEASURE                        XPIN_18

/* Used pins macros */
#define XPIN_13_USED
#define XPIN_9_USED
#define XPIN_5_USED
#define XPIN_3_USED
#define XPIN_2_USED
#define XPIN_7_USED
#define XPIN_19_USED
#define XPIN_11_USED
#define XPIN_12_USED
#define XPIN_18_USED


/* Components includes */
#include <custom.h>
#include <system.h>
#include <pm.h>
#include <rtc.h>
#include <pan_init.h>
#include <endpoints.h>
#include <xbee/discovery.h>
#include <xbee/wpan.h>
#include <xbee/atcmd.h>
#include <uart.h>
#include <i2c.h>
#include <24xxx.h>
#include <adc.h>
#include <gpios.h>

#endif /* __XBEE_CONFIG_H_ */
