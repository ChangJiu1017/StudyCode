

#ifndef __MCUABS_RTC_H__
#define __MCUABS_RTC_H__
#include "FAM.h"

#if(M_Debug==1)
#define M_RTCtime					0xFFFF
#else
#define M_RTCtime					0xFFFF
#endif


void RTC_Timeoutxus(uint32_t RTCtime);
int32_t RTCCallBackFunc(uint32_t wparam, uint32_t lparam );
void RTC_DisAbleInt(void);
void RTC_TrigeInt(uint32_t time);
uint32_t FUN_SysTick_SET(uint32_t ticks);
void API_10MS_OnInterrupt(void);


#endif


