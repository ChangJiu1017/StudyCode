/* Copyright Statement:
 *
 * This software/firmware and related documentation ("AutoChips Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
 * the prior written permission of AutoChips inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of AutoChips Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * AutoChips Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("AUTOCHIPS SOFTWARE")
 * RECEIVED FROM AUTOCHIPS AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. AUTOCHIPS EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES AUTOCHIPS PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE AUTOCHIPS SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN AUTOCHIPS
 * SOFTWARE. AUTOCHIPS SHALL ALSO NOT BE RESPONSIBLE FOR ANY AUTOCHIPS SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AUTOCHIPS'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE AUTOCHIPS SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT AUTOCHIPS'S OPTION, TO REVISE OR REPLACE THE
 * AUTOCHIPS SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO AUTOCHIPS FOR SUCH AUTOCHIPS SOFTWARE AT ISSUE.
 */
#include "ac78xx.h"
#include "ac78xx_spm_regs.h"


///< for IRQ Event call back function
static SPM_IRQEventFunc s_eventFunc = 0;


/**
* SPM_SetLowPowerMode
*
* @param[in]  mode: stop or sleep mode, value can be
*                 - LOW_POWER_MODE_STOP_LITE
*                 - LOW_POWER_MODE_STOP
*                 - LOW_POWER_MODE_STANDBY
* @return none
*
* @brief  set the low power mode
*/
void SPM_SetLowPowerMode(LOW_POWER_MODE mode)
{
    WriteBitsValue(REG_PWR_MGR_CFG0, REG_SLEEP_MODE_POS, REG_SLEEP_MODE_BITWIDTH, (uint32_t)mode);
    WRITEBIT32(REG_PWR_MGR_CFG0, 1 << REG_PWR_EN_POS);
    WriteBitsValue(0x40008880, 14, 2, 3); //config ldo voltage in stop mode
    WriteBitsValue(0x40008000, 3, 1, 0);

    NVIC_EnableIRQ(SPM_IRQn);
}

/**
* SPM_EnableFastBoot
*
* @param[in]  enable: 1: enable, 0 disable
* @return none
*
* @brief  enable fast boot mode
*/
void SPM_EnableFastBoot(uint8_t enable)
{
    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_FAST_BOOT_POS);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_FAST_BOOT_POS);
    }
}

/**
* SPM_EnablePVD
*
* @param[in]  enable: 1: enable, 0 disable
* @return none
*
* @brief  enable PVD
*/
void SPM_EnablePVD(uint8_t enable)
{
    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_PVD_POS);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_PVD_POS);
    }
}

/**
* SPM_SetLVDTriggerLevel
*
* @param[in]  level: value can be 0: VLVDL[2.65v], 1: VLVDH[4.3]
* @return none
*
* @brief  set LVD trigger level
*/
void SPM_SetLVDTriggerLevel(uint8_t level)
{
    if (level)
    {
        WRITEBIT32(REG_PWR_MGR_CFG1, 1 << REG_LVD_TRIGGER_LEVEL_POS);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG1, 1 << REG_LVD_TRIGGER_LEVEL_POS);
    }
}

/**
* SPM_SetPVDTriggerLevel
*
* @param[in]  level: VPVD level, 0 ~ 3,  the voltage depends on the VLVD level, as following:
*                  - VLVDL=2.65V VPVDL_0=2.7V
*                  - VLVDL=2.65V VPVDL_1=2.8V
*                  - VLVDL=2.65V VPVDL_2=2.9V
*                  - VLVDL=2.65V VPVDL_3=3.0V
*                  - VLVDH=4.3V  VPVDH_0=4.4V
*                  - VLVDH=4.3V  VPVDH_1=4.5V
*                  - VLVDH=4.3V  VPVDH_2=4.6V
*                  - VLVDH=4.3V  VPVDH_3=4.7V
* @return none
*
* @brief  set PVD trigger level
*/
void SPM_SetPVDTriggerLevel(uint8_t level)
{
    WriteBitsValue(REG_PWR_MGR_CFG1, REG_PVD_TRIGGER_LEVEL_POS, REG_PVD_TRIGGER_LEVEL_BITWIDTH, level);
}

/**
* SPM_EnableLVD
*
* @param[in]  enable: 1: enable, 0 disable
* @return none
*
* @brief  enable PVD
*/
void SPM_EnableLVD(uint8_t enable)
{
    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_LVD_POS);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG0, 1 << REG_EN_LVD_POS);
    }
}


/**
* SPM_EnableXOSCBypassMode
*
* @param[in]  enable: 1: enable, 0 disable
* @return none
*
* @brief  enable XOSC bypass mode
*/
void SPM_EnableXOSCBypassMode(uint8_t enable)
{
    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG1, 1 << REG_XOSC_HSEBYP_POS);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG1, 1 << REG_XOSC_HSEBYP_POS);
    }
}

/**
* SPM_EnableXOSC
*
* @param[in]  enable: 0:disable, 1: enable
* @return none
*
* @brief  enable XOSC and wait the XOSC status is OK
*/
void SPM_EnableXOSC(uint8_t enable)
{
    uint32_t status = 0;

    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG1, 1 << REG_XOSC_HSEON_POS);
        do
        {
            status = READBIT32(REG_PWR_MGR_CFG1, (unsigned int)1 << XOSC_RDY_POS);
        } while (!status);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG1, 1 << REG_XOSC_HSEON_POS);
        do
        {
            status = READBIT32(REG_PWR_MGR_CFG1, (unsigned int)1 << XOSC_RDY_POS);
        } while (status);
    }
}

/**
* SPM_EnablePLL
*
* @param[in]  enable: 0:disable, 1: enable
* @return none
*
* @brief  enable PLL and wait for the status is OK
*/
void SPM_EnablePLL(uint8_t enable)
{
    uint32_t status = 0;
    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG1, 1 << REG_SYSPLL_ON_POS);
        do
        {
            status = READBIT32(REG_PWR_MGR_CFG1, (unsigned int)1 << SYSPLL_RDY_POS);
        } while (!status);
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG1, 1 << REG_SYSPLL_ON_POS);
        do
        {
            status = READBIT32(REG_PWR_MGR_CFG1, (unsigned int)1 << SYSPLL_RDY_POS);
        } while (status);
    }
}

/**
* SPM_EnableCANLowpassFilter
*
* @param[in]  canIndex: value can be 1:CAN1, 2: CAN2
* @param[in]    enable: 0:disable, 1: enable
* @return none
*
* @brief  enable CAN lowpass filter
*/
void SPM_EnableCANLowpassFilter(uint8_t canIndex, uint8_t enable)
{
    uint8_t filterPos = REG_EN_CAN1_FILTER_POS;
    if (2 == canIndex)
    {
        filterPos = REG_EN_CAN2_FILTER_POS;
    }

    if (enable)
    {
        WRITEBIT32(REG_PWR_MGR_CFG0, (uint32_t)(1 << filterPos));
    }
    else
    {
        CLEARBIT32(REG_PWR_MGR_CFG0, (uint32_t)(1 << filterPos));
    }
}

/**
* SPM_EnableModuleWakeup
*
* @param[in]  module: module flag in SPM, value can be
*                   - SPM_MODULE_ACMP0
*                   - SPM_MODULE_ACMP1
*                   - SPM_MODULE_I2C1
*                   - SPM_MODULE_I2C2
*                   - SPM_MODULE_SPI1
*                   - SPM_MODULE_SPI2
*                   - SPM_MODULE_LIN
*                   - SPM_MODULE_CAN1
*                   - SPM_MODULE_CAN2
*                   - SPM_MODULE_UART1
*                   - SPM_MODULE_UART2
*                   - SPM_MODULE_UART3
*                   - SPM_MODULE_UART4
*                   - SPM_MODULE_UART5
*                   - SPM_MODULE_UART6
*                   - SPM_MODULE_RTC
*                   - SPM_MODULE_ADC
*                   - SPM_MODULE_GPIO
*                   - SPM_MODULE_NMI
*                   - SPM_MODULE_PVD
*                   - SPM_MODULE_SPM_OVER_COUNT
* @param[in]  enable: 0:disable, 1: enable
* @return 0: success,  -1: error
*
* @brief  Config Module Wakeup Function
*/
int32_t SPM_EnableModuleWakeup(SPM_MODULE module, uint8_t enable)
{
    int32_t ret = -1;

    if (module < SPM_MODULE_NUM)
    {
        if (enable)
        {
            enable = 1;
        }
        if (module <= SPM_MODULE_SPM_OVER_COUNT)
        {
            WriteBitsValue(REG_EN_PERIPH_WAKEUP, (uint32_t)module, 1, enable);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }

    return ret;
}

/**
* SPM_EnableModuleSleepACK
*
* @param[in]  module: module flag in SPM, value can be
*                   - SPM_MODULE_ACMP0
*                   - SPM_MODULE_ACMP1
*                   - SPM_MODULE_I2C1
*                   - SPM_MODULE_I2C2
*                   - SPM_MODULE_SPI1
*                   - SPM_MODULE_SPI2
*                   - SPM_MODULE_LIN
*                   - SPM_MODULE_CAN1
*                   - SPM_MODULE_CAN2
*                   - SPM_MODULE_UART1
*                   - SPM_MODULE_UART2
*                   - SPM_MODULE_UART3
*                   - SPM_MODULE_UART4
*                   - SPM_MODULE_UART5
*                   - SPM_MODULE_UART6
*                   - SPM_MODULE_RTC
*                   - SPM_MODULE_ADC
*                   - SPM_MODULE_GPIO
*                   - SPM_MODULE_NMI
*                   - SPM_MODULE_PVD
*                   - SPM_MODULE_SPM_OVER_COUNT
* @param[in]  enable: 0:disable, 1: enable
* @return 0: success,  -1: error
*
* @brief  Enable which module the SPM wait the ack when do stop
*/
int32_t SPM_EnableModuleSleepACK(SPM_MODULE module, uint8_t enable)
{
    int32_t ret = -1;

    if (module < SPM_MODULE_NUM)
    {
        ret = 0;
        if (enable)
        {
            WRITEBIT32(REG_EN_PERIPH_SLEEP_ACK, (uint32_t)(1 << (uint32_t)module));
        }
        else
        {
            CLEARBIT32(REG_EN_PERIPH_SLEEP_ACK, (uint32_t)(1 << (uint32_t)module));
        }
    }

    return ret;
}

/**
* SPM_EnableModuleSPMIRQ
*
* @param[in]  module: module flag in SPM, value can be
*                   - SPM_MODULE_ACMP0
*                   - SPM_MODULE_ACMP1
*                   - SPM_MODULE_I2C1
*                   - SPM_MODULE_I2C2
*                   - SPM_MODULE_SPI1
*                   - SPM_MODULE_SPI2
*                   - SPM_MODULE_LIN
*                   - SPM_MODULE_CAN1
*                   - SPM_MODULE_CAN2
*                   - SPM_MODULE_UART1
*                   - SPM_MODULE_UART2
*                   - SPM_MODULE_UART3
*                   - SPM_MODULE_UART4
*                   - SPM_MODULE_UART5
*                   - SPM_MODULE_UART6
*                   - SPM_MODULE_RTC
*                   - SPM_MODULE_ADC
*                   - SPM_MODULE_GPIO
*                   - SPM_MODULE_NMI
*                   - SPM_MODULE_PVD
*                   - SPM_MODULE_SPM_OVER_COUNT
* @param[in]  enable: 0:disable, 1: enable
* @return 0: success,  -1: error
*
* @brief  Enable the module trigger SPM IRQ or it's self IRQ
*/
int32_t SPM_EnableModuleSPMIRQ(SPM_MODULE module, uint8_t enable)
{
    int32_t ret = -1;

    if (module < SPM_MODULE_NUM)
    {
        ret = 0;
        if (enable)
        {
            WRITEBIT32(REG_EN_TRIGGER_IRQ, (uint32_t)(1 << (uint32_t)module));
        }
        else
        {
            CLEARBIT32(REG_EN_TRIGGER_IRQ, (uint32_t)(1 << (uint32_t)module));
        }
    }

    return ret;
}

/**
* SPM_GetModuleWakeupSourceFlag
*
* @return the wakeup source flag
*
* @brief  get the wakeup source flag
*/
uint32_t SPM_GetModuleWakeupSourceFlag(void)
{
    uint32_t flag = READMEM32(REG_SPM_WAKEUP_IRQ);

    return flag;
}

/**
* SPM_ClearWakeupSourceFlag
*
* @return the current wakeup source flag
*
* @brief  clear the wakeup source flag
*/
uint32_t SPM_ClearWakeupSourceFlag(void)
{
    uint32_t flag = READMEM32(REG_SPM_WAKEUP_IRQ);
    WRITEMEM32(REG_SPM_WAKEUP_IRQ, flag);

    return flag;
}


/**
* SPM_SetModuleAckCounter
*
* @param[in]  sleepAckCount: wait the module ack count when do stop
* @param[in]  wakeupAckCount: wait the module ack count when do wakeup
* @return none
*
* @brief  set the wakeup ack tick count when do stop,  if wait the module ack
*/
void SPM_SetModuleAckCounter(uint16_t sleepAckCount, uint16_t wakeupAckCount)
{
    uint32_t count = (sleepAckCount << 16) | wakeupAckCount;

    WRITEMEM32(REG_PWR_MGR_PG_MISC0, count);
}

/**
* SPM_SetSPMIRQHandler
*
* @param[in]  eventFunc: the pointer of the call back function, which will be called in SPM_IRQHandler
* @return none
*
* @brief  set SPM IRQHandler callback function
*/
void SPM_SetSPMIRQHandler(SPM_IRQEventFunc eventFunc)
{
    s_eventFunc = eventFunc;
}

/**
* SPM_IRQHandler
*
* @return none
*
* @brief  SPM IRQHandler
*/
void SPM_IRQHandler(void)
{
    uint32_t wakeupFlag = SPM_GetModuleWakeupSourceFlag();
    WRITEMEM32(REG_SPM_WAKEUP_IRQ, wakeupFlag);
    NVIC_ClearPendingIRQ(SPM_IRQn);

    if (s_eventFunc)
    {
        s_eventFunc(wakeupFlag, READMEM32(0x4000800C));
    }
}
