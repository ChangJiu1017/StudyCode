


#include "MCUabs_WGD.h"



/**
 * WDG_InitHardwr
 *
 * @param[in]  
 * @param[in]  
 * @return 
 *
 * @brief Watchdog function��ʼ��
 */
void WDG_InitHardwr(void)
{
		uint32_t tmpTimeoutValue = 48000*M_WDGTime;      //(ʱ��Ϊ48M)
    WDOG_ConfigType WDOGConfig = {0};
	
		WDOGConfig.control.clkSource = 0;                //0 is 48MHz clk source
    WDOGConfig.control.enableWDOG = ENABLE;
    WDOGConfig.control.intEnable = ENABLE;           //ʹ���жϣ��ӳ�128������ʱ�Ӻ�λ������128����ʱ����ִ���ж�����
	                                                   //����Ҫ��ʱ������λ�������жϼ���
    WDOGConfig.control.prescaler = DISABLE;          //0:disable 256 pre, 1:enable 256 pre
    WDOGConfig.control.updateEnable =ENABLE;        //if update is 0,can't do any setting
    WDOGConfig.control.winEnable = DISABLE;          //window mode watchdog
    WDOGConfig.timeout = tmpTimeoutValue;            //time = pre*timeout/clk source
    WDOGConfig.winTimeout = 0x0;
    WDOG_SetEventCallback(WDGCallback);              //�����жϻص�
    WDOG_Initialize(&WDOGConfig);

    WDOG_Feed();
}

/**
 * WDGCallback
 *
 * @param[in]  wparam: reserved
 * @param[in]  lparam: reserved
 * @return 0: success, other: error
 *
 * @brief Watchdog IRQ callback function
 * �жϷ������128������ʱ�ӣ�ʱ�伫�̣���Ȼ��MCU�ᱻ��λ
 */
int32_t WDGCallback(uint32_t wparam, uint32_t lparam)
{
    
	return 0;
}



