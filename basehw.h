/*
 * basehw.h
 */

#ifndef BASEHW_H_
#define BASEHW_H_

#include <stdint.h>

//Generic timer related:
void setGenTimerFreq(int freq);
uint64_t getPhyCount();
void setEL3PhyTimerCV(uint64_t cv);
void setEL3PhyTimerCtrl(uint32_t ctl);


#endif /* BASEHW_H_ */
