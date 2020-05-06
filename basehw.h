/*
 * basehw.h
 */

#ifndef BASEHW_H_
#define BASEHW_H_

#include <stdint.h>

#define GenCounterFreq  0x5F5E100   //100M
#define TICK_CYCLES     (10*GenCounterFreq/1000) //10ms tick length

//Generic timer related:
void setGenTimerFreq(int freq);
uint64_t getPhyCount();
void setEL3PhyTimerCV(uint64_t cv);
void setEL3PhyTimerCtrl(uint32_t ctl);


#endif /* BASEHW_H_ */
