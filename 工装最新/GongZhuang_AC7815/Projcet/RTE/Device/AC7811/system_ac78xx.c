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
#include <stdint.h>
#include "ac78xx.h"

static uint32_t  s_facus = 0;
static uint32_t  s_facms = 0;


#define MAX_SYSTICK_COUNT (SysTick_LOAD_RELOAD_Msk + 1)

/**
* WriteBitsValue
*
* @param[in]  address : 32bit physical address
* @param[in]  pos : the bit pos of the 32 bit address
* @param[in]  bitWidth: the bit width for write
* @param[in]  bitValue: the bit value to write the given pos and bitwidth
* @return none
*
* @brief  Write given Bits Value for given address
*/
void WriteBitsValue(unsigned int address, unsigned int pos, unsigned int bitWidth, unsigned int bitValue)
{
    unsigned int value = *(volatile unsigned int *)(address);
    unsigned int mask = (((1 << bitWidth) - 1) << pos);
    value &= ~mask;
    value |= ((bitValue << pos) & mask);
    *(volatile unsigned int *)(address) = value;
}

/**
* WriteBitsValueArray
*
* @param[in]  address : 32bit physical address
* @param[in]  pos : the bit pos array of the 32 bit address
* @param[in]  bitWidth: the bit width for write
* @param[in]  bitValue: the bit value to write the given pos and bitwidth
* @return none
*
* @brief  Write given Bits Value for given address
*/
void WriteBitsValueArray(unsigned int address, const BITS_VALUE_SET *bitValues, unsigned char count)
{
    unsigned int value = *(volatile unsigned int *)(address);
    unsigned int mask = 0;
    unsigned char i = 0;

    for (i = 0; i < count && bitValues; i++)
    {
        mask = (((1 << bitValues[i].bitWidth) - 1) << bitValues[i].pos);
        value &= ~mask;
        value |= ((bitValues[i].bitValue << bitValues[i].pos) & mask);
    }
    *(volatile unsigned int *)(address) = value;
}

/**
* InitDelay
*
* @param[in]  none
* @return none
*
* @brief  Init Delay count factor, 1s = TICKS * (1/f)  s;
*/
void InitDelay(void)
{
    s_facus = __SYSTEM_CLOCK / 1000000;
    s_facms = s_facus * 1000;
}

/**
* SetHSEBYPClock
*
* @param[in]  none
* @return none
*
* @brief  Set HSE bypass mode
*/
void SetHSEBYPClock(void)
{
    SPM_EnableXOSCBypassMode(1);
    SPM_EnableXOSC(1);
    CKGEN_SetSysclkDiv(SYSCLK_DIV);
    CKGEN_SetSysclkSrc(SYSCLK_SRC_EXTERNAL_OSC);
}


/**
* SetXOSCClock
*
* @param[in]  none
* @return none
*
* @brief  Set XOSC Clock
*/
void SetXOSCClock(void)
{
    SPM_EnableXOSC(1);
    CKGEN_SetSysclkDiv(SYSCLK_DIV);
    CKGEN_SetSysclkSrc(SYSCLK_SRC_EXTERNAL_OSC);
}

/**
* SetHSEClock
*
* @param[in]  none
* @return none
*
* @brief  Use HSE mode, set the clock
*/
void SetHSEClock(void)
{
    SPM_EnableXOSC(1);
    CKGEN_SetPLLReference(PLL_REF_EXTERNAL_OSC);
    CKGEN_SetPllPrevDiv(PLL_PREDIV);
    CKGEN_SetPllPostDiv(PLL_POSDIV);
    CKGEN_SetPllFeedbackDiv(PLL_FBKDIV);
    SPM_EnablePLL(1);
    CKGEN_SetSysclkDiv(SYSCLK_DIV);
    CKGEN_SetSysclkSrc(SYSCLK_SRC_PLL_OUTPUT);
}

/**
* SetHSIClock
*
* @param[in]  none
* @return none
*
* @brief  Use HSI mode, set the clock
*/
void SetHSIClock(void)
{
    CKGEN_SetPLLReference(PLL_REF_INTERAL_OSC);
    CKGEN_SetPllPrevDiv(PLL_PREDIV);
    CKGEN_SetPllPostDiv(PLL_POSDIV);
    CKGEN_SetPllFeedbackDiv(PLL_FBKDIV);
    SPM_EnablePLL(1);
    CKGEN_SetSysclkDiv(SYSCLK_DIV);
    CKGEN_SetSysclkSrc(SYSCLK_SRC_PLL_OUTPUT);
}

/**
* SetSysClock
*
* @param[in]  none
* @return none
*
* @brief  Use HSI mode, set the clock
*/
void SetSysClock(void)
{

#if AC78XX_CLOCK_SRC == IC_CLOCK_SRC_XOSC
    SetXOSCClock();
#elif AC78XX_CLOCK_SRC == IC_CLOCK_SRC_HSI
    SetHSIClock();
#elif AC78XX_CLOCK_SRC == IC_CLOCK_SRC_HSE
    SetHSEClock();
#else
    SetHSEBYPClock();
#endif
}

/**
* EnableQSPI
*
* @param[in]  none
* @return none
*
* @brief  Enable QSPI serial flash.
*
*/
void EnableQSPI(void)
{
    //Enable QSPI serial flash
    WRITEMEM32(0x400100C4, 0x30);
    WRITEMEM32(0x400100AC, 0x7f);
    WRITEMEM32(0x40010034, 0x40);
    WRITEMEM32(0x40010000, 0x20);
    WRITEMEM32(0x40010030, 0xeb);
    WRITEMEM32(0x400100cc, 0xc);
}

/**
* SetEflashClock
*
* @param[in]  none
* @return none
*
* @brief  Set eflash clock.
*
*/
void SetEflashClock(void)
{
    if (72000000 < SYSCLK_FREQ)
    {
        while (READBIT32(0x40002004, (uint32_t)1 << 31))
        {
            WRITEMEM32(0x40002000, 0xac7811);  //unlock eflash
            WRITEMEM32(0x40002000, 0x01234567);
        }
        WRITEMEM32(0x40002014, 0x12067);
        while (READBIT32(0x40002014, 1 << 17));
        WRITEBIT32(0x40002004, (uint32_t)1 << 31);
    }
}

/**
* Initialize the system
*
* @param[in]  none
* @return none
*
* @brief  Setup the microcontroller system.
*         Initialize the System.
*/
void SystemInit (void)
{

    //EnableQSPI();  //enable QSPI if need
    SetEflashClock();
    InitDelay();
    SetSysClock();
    CKGEN_SetAPBClockDivider(APBCLK_DIV);
    CKGEN_SetSFlashClock(SFLASH_CLK_SEL_APB, SFLASH_DIVIDER_2);

    //SPM_EnableLVD(0);  //disable LVD if need
}

/**
* SysTickDelay
*
* @param[in]  param : tick
* @return none
*
* @brief  delay until the syctick count tick to 0
*/
static void SysTickDelay(uint32_t tick)
{
    uint32_t tickFlag = 0;
    SysTick->LOAD = tick - 1;
    SysTick->VAL = 0x00;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    do
    {
        tickFlag = SysTick->CTRL;

    } while(!(tickFlag & SysTick_CTRL_COUNTFLAG_Msk));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

/**
* SysTickRepeatDelay
*
* @param[in]  param : times, the times of delay the tick
* @param[in]  param : tick, each tick to delay
* @return none
*
* @brief  Repeatlly delay the ticks for the given times
*/
static void SysTickRepeatDelay(uint32_t times, uint32_t tick)
{
    uint32_t i = 0;
    for (i = 0; i < times; i++)
    {
        SysTickDelay(tick);
    }
}

/**
* udelay
*
* @param[in]  param : us, us for delay
* @return none
*
* @brief  delay us
*/
void udelay(uint32_t us)
{
    uint32_t tick = us * s_facus;

    SysTickRepeatDelay(tick / MAX_SYSTICK_COUNT, MAX_SYSTICK_COUNT);

    SysTickDelay(tick % MAX_SYSTICK_COUNT);
}

/**
* mdelay
*
* @param[in]  param : ms, us for delay
* @return none
*
* @brief  delay ms
*/
void mdelay(uint32_t ms)
{
    uint32_t tick = ms * s_facms;

    SysTickRepeatDelay(tick / MAX_SYSTICK_COUNT, MAX_SYSTICK_COUNT);

    SysTickDelay(tick % MAX_SYSTICK_COUNT);
}


/**
* getCPUID
*
* @param[in]
* @return return the CPUID
*
* @brief  change the current menu and display
*/
unsigned int GetCPUID(void)
{
    return SCB->CPUID;
}

/**
*
*
* @param[in] normalAddress: memory address to write bit
* @param[in] bit : the bit of the 32 address value to set
* @param[in] bitValue : the value to be write
* @return none
*
* @brief  write memmory bit with BITBAND
*/
void BitbandWriteMemory(unsigned long normalAddress, unsigned long bit, unsigned long bitValue)
{
    unsigned long bitbandAddr = 0;
    if( normalAddress < 0x20000000 || normalAddress > 0x20010000)
    {
        printf("BitbandWriteMemory:Error address!!\n");
        return;
    }

    bitbandAddr = 0x20000000 + 0x2000000 + (normalAddress - 0x20000000) * 32 + bit * 4;
    (*((unsigned long*) bitbandAddr)) = bitValue;

}

/**
*
*
* @param[in] normalAddress: memory address to read bit
* @param[in] bit : the bit of the 32 address value to set
* @return none
*
* @brief  read memmory bit with BITBAND
*/
unsigned long BitbandReadMemory(unsigned long normalAddress, unsigned long bit)
{
    unsigned long bitbandAddr = 0;
    unsigned long bitbandData = 0;


    if( normalAddress < 0x20000000 || normalAddress > 0x20010000)
    {
        printf("BitbandReadMemory:Error address!!\n");
        return 0xFF;
    }
    bitbandAddr = 0x20000000 + 0x2000000 + (normalAddress - 0x20000000) * 32 + bit * 4;
    bitbandData = *(unsigned long *)bitbandAddr;

    return     bitbandData;
}

/**
* @param[in] normalAddress: physical address to write bit
* @param[in] bit : the bit of the 32 address value to set
* @param[in] bitValue : the value to be write
* @return none
*
* @brief  write physical bit with BITBAND
*/
void BitbandWritePhy(unsigned long normalAddress, unsigned long bit, unsigned long bitValue)
{
    unsigned long bitbandAddr = 0;
    if( normalAddress < 0x40000000 || normalAddress > 0x40100000)
    {
        printf("BitbandWritePhy:Error address!!\n");
        return;
    }
    bitbandAddr = 0x40000000 + 0x2000000 + (normalAddress - 0x40000000) * 32 + bit * 4;

    (*((unsigned long*) bitbandAddr)) = bitValue;
}

/**
* BitbandReadPhy
*
* @param[in] normalAddress: physical address to read bit
* @param[in] bit : the bit of the 32 address value to set
* @return none
*
* @brief  read physical bit with BITBAND
*/
unsigned long BitbandReadPhy(unsigned long normalAddress, unsigned long bit)
{
    unsigned long bitbandAddr = 0;
    unsigned long bitbandData = 0;
    if( normalAddress < 0x40000000 || normalAddress > 0x40100000)
    {
        printf("BitbandReadPhy:Error address!!\n");
        return 0xFF;
    }
    bitbandAddr = 0x40000000 + 0x2000000 + (normalAddress - 0x40000000) * 32 + bit * 4;
    bitbandData = *(unsigned long *)bitbandAddr;

    return     bitbandData;
}

/**
* @return none
*
* @brief  enter the Standby mode
*/
void SysStandby(void)
{
    SPM_SetLowPowerMode(LOW_POWER_MODE_STANDBY);
    SPM_EnableModuleWakeup(SPM_MODULE_NMI, 1);
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // If using KEIL's uVision, use the CMSIS intrinsic
    __wfi();
}

/**
* @return none
*
* @brief  enter the Stop mode
*/
void SysStop (void)
{
    SPM_SetLowPowerMode(LOW_POWER_MODE_STOP);
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // If using KEIL's uVision, use the CMSIS intrinsic
    __wfi();
    // Add Debug Interface to enable printf after wakeup   
}

/**
* @return none
*
* @brief  enter the Stop lite mode
*/
void SysStopLite (void)
{
    SPM_SetLowPowerMode(LOW_POWER_MODE_STOP_LITE);
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // If using KEIL's uVision, use the CMSIS intrinsic
    __wfi();
    // Add Debug Interface to enable printf after wakeup
}

/**
* @return none
*
* @brief  enter the Sleep mode
*/
void SysSleep (void)
{
    /* Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep) mode instead
    * of deep sleep.
    */

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __wfi();
}
