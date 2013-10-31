/*
 * DS1307.h
 *
 *  Created on: Oct 31, 2013
 *      Author: spave
 */

#ifndef DS1307_H_
#define DS1307_H_

#define DS1307_ADDR 			0x68  // 7-bit address

#define DS1307_REG_CTRL			0x07
#define DS1307_REG_CTRL_DEF		0x03  // The default value of the control reg upon startup. Will use to test comm


bool_t DS1307_test(void);

#endif /* DS1307_H_ */
