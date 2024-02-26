


#include "MCUabs_WGD.h"



/**
 * WDG_InitHardwr
 *
 * @param[in]  
 * @param[in]  
 * @return 
 *
 * @brief Watchdog function初始化
 */
void WDG_InitHardwr(void)
{
		uint32_t tmpTimeoutValue = 48000*M_WDGTime;      //(时钟为48M)
    WDOG_ConfigType WDOGConfig = {0};
	
		WDOGConfig.control.clkSource = 0;                //0 is 48MHz clk source
    WDOGConfig.control.enableWDOG = ENABLE;
    WDOGConfig.control.intEnable = ENABLE;           //使能中断，延迟128个总线时钟后复位，可在128总线时钟内执行中断例程
	                                                   //若需要超时立即复位，禁能中断即可
    WDOGConfig.control.prescaler = DISABLE;          //0:disable 256 pre, 1:enable 256 pre
    WDOGConfig.control.updateEnable =ENABLE;        //if update is 0,can't do any setting
    WDOGConfig.control.winEnable = DISABLE;          //window mode watchdog
    WDOGConfig.timeout = tmpTimeoutValue;            //time = pre*timeout/clk source
    WDOGConfig.winTimeout = 0x0;
    WDOG_SetEventCallback(WDGCallback);              //设置中断回调
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
 * 中断服务仅有128个总线时钟（时间极短），然后MCU会被复位
 */
int32_t WDGCallback(uint32_t wparam, uint32_t lparam)
{
    
	return 0;
}



