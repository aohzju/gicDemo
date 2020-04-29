 #include "config.h"

//Reset handler:

	.global	 reset_handler
	.section .reset,"ax"

//To guarantee no semihosting library functions are called:
 	.global __use_no_semihosting

#ifdef USE_ARM_LIB_STARTUP
//Override ARM library's __rt_entry:
	.global __rt_entry
	.type	__rt_entry @function
__rt_entry:
	B  appmain
#endif

reset_handler:
	// Initialize VBAR_ELx:
	LDR X1, = vector_table_el3
	MSR VBAR_EL3, X1

	// Initialize the register bank:
	.irp	reg, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	MOV		X\reg, XZR
	.endr
	.irp	reg, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
	MOV		X\reg, XZR
	.endr
	.irp	reg, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
	MOV		X\reg, XZR
	.endr

	//Initialize stack pointers:
	//AARCH64 stack is full descending, so set the initial value of SP to Image$$ARM_LIB_STACK$$ZI$$Limit
	LDR		X0, =Image$$ARM_LIB_STACK$$ZI$$Limit  //ARM_LIB_STACK: to be defined in the scatter loading file
	MOV		SP, X0  //SP is SP_EL3 on reset

	//Initialize system control registers:

	//SCTLR_EL3

	//SCR_EL3
	MRS		X0, SCR_EL3
	ORR		X0, X0, #(1<<3)		//EA
	ORR		X0, X0, #(1<<2)		//FIQ
	ORR		X0, X0, #(1<<1)		//IRQ
	ORR		X0, X0, #(1<<8)		//HCE
	ORR		X0, X0, #(1<<7)		//SMD
	MSR		SCR_EL3, X0

	// Enable SError, IRQ and FIQ
	MSR DAIFClr, #0x7

#ifdef USE_ARM_LIB_STARTUP
	B	__main //do startup initialization (copy ZW regions, zero ZI regions, etc. And then call __rt_entry, which calls application's main function
#else
	B	my__main //Alternative: totally bypass ARM library's initialization code. Do it all by yourself
#endif

	.end
