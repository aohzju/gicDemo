/*
 * c_ex_handers.c
 *
 *  Created on: Apr 29, 2020
 *      Author: oscarh
 */

#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "basehw.h"

//The core FIQ handler in c, called assembly function fiq_handler.
//The surrounding logic is in assembly code: context save/restore,
//Interrupt acknowledge by reading ICC_IAR0_EL1
//EOI by writing ICC_EOIR0_EL1
void vApplicationFIQHandler(uint64_t intID)
{
	uint64_t count;
	count = getPhyCount();
	setEL3PhyTimerCV(count+TICK_CYCLES);
	printf("INT %d, Current Generic Timer Count: %llu\n", (uint32_t)intID, (unsigned long long)count);
}
