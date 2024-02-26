
#include "MCUabs_RTC.h"

byte RTC_ST;

/**
 * RTC_Timeoutxus
 *
 * @param[in]  
 * @param[in]  
 * @return 
 *
 * @brief RTCTimer function初始化
 */
void RTC_Timeoutxus(uint32_t RTCtime)
{
    uint32_t tmpMod = 999;
    uint32_t tmpPrescalerValue = 47;
    RTC_ConfigType RTCConfig = {0};

    //RTC_SetEventCallback(RTCCallBackFunc);			   //设置回调函数
    NVIC_SetPriority(RTC_IRQn, 1);
		
    RTCConfig.clockSource = 0;                         //时钟源选择：0:Bus clk (demo板上总线时钟为48M)；
	  RTCConfig.moduloValue = tmpMod;
    RTCConfig.interruptEn = ENABLE;
    RTCConfig.prescalerValue = tmpPrescalerValue;      //time = (pre+1)*(mod+1)/clk
    RTCConfig.RTCOut = ENABLE;
    RTC_Initialize(&RTCConfig);                        //配置RTC
		RTC_DisAbleInt();
}

void RTC_TrigeInt(uint32_t time)
{
		RTC->RTCPS = 0;
		RTC->MOD = time;
		RTC->SC.RTIE = 1;
		RTC->RTCPS = 191;
}

void RTC_DisAbleInt(void)
{
		RTC->SC.RTIE = 0;
		RTC->RTCPS = 0;
}

