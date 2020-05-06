/*
 ============================================================================
 */
#include "config.h"
#include "stdio.h"
#include "uart.h"
#include "basehw.h"
#include "gicv3.h"

//Globals:
int array1[16] = {3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12}; //RW Data
int array2[32] = {0}; //ZI, will goes to .bss
int array3[64]; //Global or statically allocated variables not explicitly initialized to any value
				//Will goes to .bss by ARM compiler
				//Will goes to *COM* section by gcc if -fno-common is not used
int array4[16] __attribute__((section("configData"))) = { 100, 101, 102, 103}; //A named data section


int appmain(void) {

	int n, x, y, len;
	x = 20;

	open__stdout();
	gicInit();		//Init and config GIC

	setGenTimerFreq(GenCounterFreq);
	setEL3PhyTimerCV(getPhyCount()+TICK_CYCLES);
	setEL3PhyTimerCtrl(0x01);

	configGenTimerInterrupt(); //Configure and enable the generic timer interrupt

	for(n=0; n<16; n++){
		array1[n] += x+array4[n];
		array2[n] = array1[n]*3;
		array2[n+16] = array1[n];
		array3[n] = array2[n] + 2;
		array3[n+16] = array2[n] + 2;
		array3[n+32] = array2[n] + 2;
	}


	printf("Hello, %s, %d regions initialized by scatter loading\n", "World", 3);

	while(1)
		;

	return 0;
}
