/*
 * util.c
 *
 *  Created on: Oct 18, 2013
 *      Author: spave
 */
#include <system.h>
#include <types.h>
#include <util.h>

#pragma INLINE
void delay(uint32_t t)
{
	for(t*=4; t--; sys_udelay(250));	
}
