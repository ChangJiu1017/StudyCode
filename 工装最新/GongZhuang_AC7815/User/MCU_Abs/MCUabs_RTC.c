
#include "MCUabs_RTC.h"

byte RTC_ST;

/**
 * RTC_Timeoutxus
 *
 * @param[in]  
 * @param[in]  
 * @return 
 *
 * @brief RTCTimer function��ʼ��
 */
void RTC_Timeoutxus(uint32_t RTCtime)
{
    uint32_t tmpMod = 999;
    uint32_t tmpPrescalerValue = 47;
    RTC_ConfigType RTCConfig = {0};

    //RTC_SetEventCallback(RTCCallBackFunc);			   //���ûص�����
    NVIC_SetPriority(RTC_IRQn, 1);
		
    RTCConfig.clockSource = 0;                         //ʱ��Դѡ��0:Bus clk (demo��������ʱ��Ϊ48M)��
	  RTCConfig.moduloValue = tmpMod;
    RTCConfig.interruptEn = ENABLE;
    RTCConfig.prescalerValue = tmpPrescalerValue;      //time = (pre+1)*(mod+1)/clk
    RTCConfig.RTCOut = ENABLE;
    RTC_Initialize(&RTCConfig);                        //����RTC
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

