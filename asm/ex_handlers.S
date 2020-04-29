/*
	Exception handlers
*/

//in vectors.S, the exception table is set up to jump to this function for FIQ:
// B  fiq_handler
//Interrupt nesting is not supported

	.global fiq_handler
	.type	fiq_handler @function
fiq_handler:
	/* Save volatile registers. */
	STP		X0, X1, [SP, #-0x10]!
	STP		X2, X3, [SP, #-0x10]!
	STP		X4, X5, [SP, #-0x10]!
	STP		X6, X7, [SP, #-0x10]!
	STP		X8, X9, [SP, #-0x10]!
	STP		X10, X11, [SP, #-0x10]!
	STP		X12, X13, [SP, #-0x10]!
	STP		X14, X15, [SP, #-0x10]!
	STP		X16, X17, [SP, #-0x10]!
	STP		X18, X19, [SP, #-0x10]!
	STP		X29, X30, [SP, #-0x10]!

	MRS		X0, ICC_IAR0_EL1  //Read INTID, acknowledging the interrupt
	MOV		X19, X0
	/* Call the C handler. */
	BL vApplicationFIQHandler

	MSR   ICC_EOIR0_EL1, X19		//End of interrupt handling


	LDP		X29, X30, [SP], #0x10
	LDP		X18, X19, [SP], #0x10
	LDP		X16, X17, [SP], #0x10
	LDP		X14, X15, [SP], #0x10
	LDP		X12, X13, [SP], #0x10
	LDP		X10, X11, [SP], #0x10
	LDP		X8, X9, [SP], #0x10
	LDP		X6, X7, [SP], #0x10
	LDP		X4, X5, [SP], #0x10
	LDP		X2, X3, [SP], #0x10
	LDP		X0, X1, [SP], #0x10

	ERET

