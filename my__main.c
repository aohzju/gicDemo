/*
 */
#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


extern int appmain(void);

#define INIT_RW_REGION(regionName) 						\
{														\
	extern int Load$$##regionName##$$Base; 				\
	extern int Image$$##regionName##$$Base; 			\
	extern int Image$$##regionName##$$Length; 			\
	size_t i, len;										\
	char  *rw_ld_base, *rw_ex_base;						\
	rw_ld_base = (char*)&Load$$##regionName##$$Base;	\
	rw_ex_base = (char*)&Image$$##regionName##$$Base;	\
	len = (size_t)&Image$$##regionName##$$Length;		\
	memcpy(rw_ex_base, rw_ld_base, len);				\
}

#define INIT_ZI_REGION(regionName)						\
{ 														\
	extern int Image$$##regionName##$$ZI$$Base;			\
	extern int Image$$##regionName##$$ZI$$Length;		\
	size_t i, len;										\
	char *bss;											\
	len = (size_t)&Image$$##regionName##$$ZI$$Length;	\
	bss = (char*)&Image$$##regionName##$$ZI$$Base;		\
	memset(bss, 0, len);								\
}


#ifndef USE_ARM_LIB_STARTUP
//This function needs to be called to do scatter loading if ARM library's __main is not used:
//This function does scatter loading and then call applications main function as ARM's __main does:
void my__main()
{
	INIT_RW_REGION(RW_DATA)
	INIT_RW_REGION(configData)

	INIT_ZI_REGION(ZI_DATA);

	//Now call application's main function:
	appmain();
}
#endif
