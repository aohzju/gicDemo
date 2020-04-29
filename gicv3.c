/*
 * gicv3.c
 *
 *  Created on: Apr 28, 2020
 *      Author: oscarh
 */

#include "gicv3.h"

void gicInit()
{
	volatile uint32_t regVal;

	//Check GIC features:
	regVal = gicdReadReg(GICD_TYPER);
	//...

	//Global config
	//Enable ARE, interrupt groups, set DS=0:
	gicdWriteReg(GICD_CTLR, 0xB7);
	while(1){ //wait until the reg writing is done
		regVal = gicdReadReg(GICD_CTLR);
		if((regVal & 0x80000000) == 0)
			break;
	}

	//Per-redistributor config:
	regVal = gicrReadReg(GICR_WAKER, 0);
	gicrWriteReg(GICR_WAKER, 0, regVal & 0xFFFFFFFFC);
	while(1){
		regVal = gicrReadReg(GICR_WAKER, 0); //Poll GICR_WAKER.ChildrenAsleep until it reads 0
		if((regVal & 0x4) == 0)
			break;
	}

	//CPU interface config:
	//Enable SRE
	regVal = getICC_SRE_EL3();
	regVal |= 0x01; //Enable SRE
	setICC_SRE_EL3(regVal);

	//the number of priority bits implemented is 5 on the FVP FVP_Base_RevC-2xAEMv8A
	//regVal = getICC_PMR_EL1();
	regVal = 0xFF;		//The minimal priority
	setICC_PMR_EL1(regVal);

	setICC_BPR0_EL1(0x01); //binary point: ggggg.ss. So all 5 bits are group bits, not supporting interrupt preemption.

	//ICC_CTLR_EL3:
	regVal=getICC_CTLR_EL3();
	setICC_CTLR_EL3(regVal & ~0x04); //EOImode_EL3: priority drop and interrupt deactivation together.

	//Enable Group 0 interrupts at CPU interface:
	setICC_IGRPEN0_EL1(0x01);

	//All SGI and PPI interrupts are of secure group 0
	//Set group for interrupts
	gicrWriteReg(GICR_IGROUPR0, 0, 0);
	gicrWriteReg(GICR_IGRPMODR0, 0, 0);

}

void configGenTimerInterrupt()
{
	uint32_t regVal;
	int x;

	//timer interrupt is level sensitive per ARM document:
	regVal = gicrReadReg(GICR_ICFGR1, 0);
	x = GEN_TIMER_INTID - 16;
	x = x*2 + 1;
	regVal &= ~(0x1<<x); //clear to zero
	gicrWriteReg(GICR_ICFGR1, 0, regVal);

	//Set priority:
	setSGIPPIPriority(0, GEN_TIMER_INTID, 0);

	regVal = gicrReadReg(GICR_ISENABLER0, 0);
	regVal |= (1<<GEN_TIMER_INTID); //enable generic timer interrupt
	gicrWriteReg(GICR_ISENABLER0, 0, regVal);
}
