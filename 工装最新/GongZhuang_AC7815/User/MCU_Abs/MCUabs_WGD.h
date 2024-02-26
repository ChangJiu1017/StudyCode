

#ifndef __MCUABS_WGD_H__
#define __MCUABS_WGD_H__
#include "FAM.h"



#define M_WDGTime				50//msÎªµ¥Î»
#define CLR_WDG2					WDOG_Feed();


void WDG_InitHardwr(void);
int32_t WDGCallback(uint32_t wparam, uint32_t lparam);
#endif


