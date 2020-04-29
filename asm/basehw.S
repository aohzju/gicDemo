	.global setGenTimerFreq
	.type	setGenTimerFreq	@function

setGenTimerFreq:
	MSR		CNTFRQ_EL0, X0
	RET

	.global getPhyCount
	.type	getPhyCount @function
getPhyCount:
	MRS		X0, CNTPCT_EL0
	RET

	.global setEL3PhyTimerCV
	.type	setEL3PhyTimerCV @function
setEL3PhyTimerCV:
	MSR		CNTPS_CVAL_EL1, X0
	RET

	.global setEL3PhyTimerCtrl
	.type	setEL3PhyTimerCtrl @function
setEL3PhyTimerCtrl:
	MSR CNTPS_CTL_EL1, X0
	RET

	.end
