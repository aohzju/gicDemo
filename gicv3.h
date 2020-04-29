/*
 * gicv3.h
 *
 *  Created on: Apr 28, 2020
 *      Author: oscarh
 */

#ifndef _GICV3_H_
#define _GICV3_H_

#include <stdint.h>

//Base addresses:

//Base of Distributor:
#define	GICD_BASE 	0x002F000000

//Bases of redistributors:
/*Each redistributor occupies 128K memory spaces for its memory mapped registers.
  N redistributors' memory is a contiguous N*128K space with one by one. So the
  first redistributor base is GIC_BASE, the 2nd is GICR_BASE+128K and so on.

  All GIC registers are 32-bits wide.
*/
#define	GICR_BASE			0x002F100000
#define GICR_SPACE_SIZE		0x20000		//128K size
#define GICR_BASE_SHIFT		17			//1<<17==128k

//GIC Distributor registers:
#define GICD_CTLR		0x0000
#define GICD_TYPER		0x0004
#define GICD_IIDR		0x0008
#define GICD_STATUSR	0x0010
#define GICD_SETSPI_NSR	0x0040
#define GICD_CLRSPI_NSR	0x0048
#define GICD_SETSPI_SR	0x0050
#define	GICD_CLRSPI_SR	0x0058

//Redistributor registers:
#define GICR_CTLR		0x0000
#define GICR_IIDR		0x0004
#define GICR_TYPER		0x0008
#define GICR_STATUSR	0x0010
#define GICR_WAKER		0x0014

#define GICR_SETLPIR	0x0040
#define GICR_CLRLPIR	0x0048
#define GICR_PROPBASER	0x0070
#define GICR_PENDBASER	0x0078
#define GICR_INVLPIR	0x00A0
#define GICR_INVALLR	0x00B0
#define GICR_SYNCR		0x00C0

//registers relative to SGI_base, so add 0x10000:
#define GICR_IGROUPR0			(0x10000+0x0080)
#define GICR_ISENABLER0			(0x10000+0x0100)
#define GICR_ICENABLER0			(0x10000+0x0180)
#define GICR_ISPENDR0			(0x10000+0x0200)
#define GICR_ICPENDR0			(0x10000+0x0280)
#define GICR_ISACTIVER0			(0x10000+0x0300)
#define GICR_ICACTIVER0			(0x10000+0x0380)
#define GICR_IPRIORITYR			(0x10000+0x0400)	//byte-accessible register
#define GICR_ICFGR0				(0x10000+0x0C00)
#define GICR_ICFGR1				(0x10000+0x0C04)
#define GICR_IGRPMODR0			(0x10000+0x0D00)
#define GICR_NSACR				(0x10000+0x0E00)

#define GEN_TIMER_INTID			29		//EL3 Physical Timer interrupt ID


//Register read/write functions:
static inline void gicdWriteReg(uintptr_t offset, uint32_t data)
{
    (*((volatile uint32_t *)(offset + GICD_BASE))) = data;
}

static inline uint32_t gicdReadReg(uintptr_t offset)
{
    return (*((volatile uint32_t *)(offset + GICD_BASE)));
}

static inline void gicrWriteReg(uintptr_t offset, uint32_t gicrId, uint32_t data)
{
    uintptr_t gicrAddr;
    gicrAddr = (GICR_BASE + ((uintptr_t)gicrId << GICR_BASE_SHIFT));
    (*((volatile uint32_t *)(offset + gicrAddr))) = data;
}

static inline uint32_t gicrReadReg(uintptr_t offset, uint32_t gicrId)
{
    uintptr_t gicrAddr;
    gicrAddr = (GICR_BASE + ((uintptr_t)gicrId << GICR_BASE_SHIFT));
    return (*((volatile uint32_t *)(offset + gicrAddr)));
}

//CPU interface registers r/w functions:
//------------------------------------------------------------
#define GET_ICC_REG_FUNC(regName) \
static inline uint32_t get##regName() \
{ \
	uint64_t val; \
	__asm("MRS %[val], " #regName \
				: [val] "=r" (val) \
				: ); \
	return (uint32_t)val; \
}

#define SET_ICC_REG_FUNC(regName) \
static inline void set##regName(uint32_t val) \
{ \
	uint64_t reg = val; \
	__asm("MSR " #regName ", %[val]\n" \
		  "ISB" \
			: \
			: [val] "r" (reg)); \
}
//-----------------------------------------------------------
static inline uint32_t getICC_SRE_EL3()
{
	uint64_t val;
	__asm("MRS %[val], ICC_SRE_EL3"
			: [val] "=r" (val)
			: );

	return (uint32_t)val;
}

static inline void setICC_SRE_EL3(uint32_t val)
{
	uint64_t reg = val;
	__asm("MSR ICC_SRE_EL3, %[val]\n"
		  "ISB"
			:
			: [val] "r" (reg));
}

//Get SGI or PPI's priority:
//intID: 0~31, SGI or PPI interrupt ID.
static inline uint8_t getSGIPPIPriority(uint32_t gicrId, uint32_t intID)
{
	volatile uint8_t *regAddr;

	uintptr_t gicrAddr;
	gicrAddr = (GICR_BASE + ((uintptr_t)gicrId << GICR_BASE_SHIFT));

	regAddr = (volatile uint8_t*)(gicrAddr + GICR_IPRIORITYR + intID);
	return *regAddr;
}

//Set SGI or PPI's priority:
//intID: 0~31, SGI or PPI interrupt ID.
static inline void setSGIPPIPriority(uint32_t gicrId, uint32_t intID, uint8_t priority)
{
	volatile uint8_t *regAddr;

	uintptr_t gicrAddr;
	gicrAddr = (GICR_BASE + ((uintptr_t)gicrId << GICR_BASE_SHIFT));

	regAddr = (volatile uint8_t*)(gicrAddr + GICR_IPRIORITYR + intID);
	*regAddr = priority;
}


GET_ICC_REG_FUNC(ICC_PMR_EL1)
SET_ICC_REG_FUNC(ICC_PMR_EL1)

GET_ICC_REG_FUNC(ICC_BPR0_EL1)
SET_ICC_REG_FUNC(ICC_BPR0_EL1)

GET_ICC_REG_FUNC(ICC_CTLR_EL3)
SET_ICC_REG_FUNC(ICC_CTLR_EL3)

GET_ICC_REG_FUNC(ICC_IGRPEN0_EL1)
SET_ICC_REG_FUNC(ICC_IGRPEN0_EL1)


/////////////////////////////////////////////////////////////////////////////////////////
void gicInit();
void configGenTimerInterrupt();

#endif /* _GICV3_H_ */
