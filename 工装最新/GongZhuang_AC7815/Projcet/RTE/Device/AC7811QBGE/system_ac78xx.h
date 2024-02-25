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



#ifndef _SYSTEM_AC78XX_H
#define _SYSTEM_AC78XX_H
#include "ac78xx_ckgen.h"
#include "ac78xx_spm.h"
#include "debugzone.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    
#define WRITEMEM8(address, value)    *(volatile unsigned char *)(address) = value
#define READMEM8(address)            (*(volatile unsigned char *)(address))

#define WRITEMEM16(address, value)   *(volatile unsigned short *)(address) = value
#define READMEM16(address)           (*(volatile unsigned short *)(address))

#define WRITEMEM32(address, value)   *(volatile unsigned int *)(address) = value
#define READMEM32(address)           (*(volatile unsigned int *)(address))

#define WRITEBIT32(address, bitmask)   *(volatile unsigned int *)(address) |= (bitmask)
#define READBIT32(address, bitmask)    (*(volatile unsigned int *)(address) & (bitmask))
#define CLEARBIT32(address, bitmask)   *(volatile unsigned int *)(address) &= ~(bitmask)
void WriteBitsValue(unsigned int address, unsigned int pos, unsigned int bitWidth, unsigned int bitValue);

typedef struct
{
    unsigned char pos;
    unsigned char bitWidth;
    unsigned int bitValue;
}BITS_VALUE_SET;

void WriteBitsValueArray(unsigned int address, const BITS_VALUE_SET *bitValues, unsigned char count);

#define EnableInterrupts  __ASM(" CPSIE i");
#define DisableInterrupts __ASM(" CPSID i");
#define EnableFatules  __ASM(" CPSIE F");
#define DisableFatules __ASM(" CPSID F");

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

#define SYSCLK_FREQ_27MHz   27000000
#define SYSCLK_FREQ_72MHz   72000000
#define SYSCLK_FREQ_96MHz   96000000
#define SYSCLK_FREQ_100MHz 100000000



#define IC_CLOCK_SRC_HSI  0      //High speed internal RC(HSI): The internal RC OSC to provide 8MHz clock source
#define IC_CLOCK_SRC_HSE  1      //The external OSC to provide 4MHZ ~30MHz  crystal
#define IC_CLOCK_SRC_XOSC 2      //The external OSC to provide 4MHZ ~30MHz  oscillator

#define AC78XX_CLOCK_SRC  IC_CLOCK_SRC_HSE


#define XTAL_8M       8000000
#define XTAL_4M       4000000
#define XTAL_30M     30000000

#define USE_XTAL      XTAL_8M

///<PLL available in HSE & HSI mode
#if USE_XTAL == XTAL_8M
#define PLL_POSDIV   PLL_POSDIV_4
#define PLL_FBKDIV   96
#define PLL_PREDIV   PLL_PREDIV_1

#define SYSCLK_DIV   SYSCLK_DIVIDER_1
#define APBCLK_DIV   APBCLK_DIVIDER_2
#define SYSCLK_FREQ  96000000
#define APB_BUS_FREQ 48000000

#elif USE_XTAL == XTAL_4M
#define PLL_POSDIV   PLL_POSDIV_4
#define PLL_FBKDIV   192
#define PLL_PREDIV   PLL_PREDIV_1
#define SYSCLK_DIV   SYSCLK_DIVIDER_1
#define APBCLK_DIV   APBCLK_DIVIDER_2
#define SYSCLK_FREQ  96000000
#define APB_BUS_FREQ 48000000

#elif USE_XTAL == XTAL_30M
#define PLL_POSDIV   PLL_POSDIV_4
#define PLL_FBKDIV   12
#define PLL_PREDIV   PLL_PREDIV_1
#define SYSCLK_DIV   SYSCLK_DIVIDER_1
#define APBCLK_DIV   APBCLK_DIVIDER_1
#define SYSCLK_FREQ  45000000
#define APB_BUS_FREQ 45000000

#else
#error
#endif

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies             */
#define __SYSTEM_CLOCK    SYSCLK_FREQ

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

extern void BitBandWriteMemory(unsigned long normalAddress,unsigned long bit,unsigned long bitValue);
extern unsigned long BitBandReadMemory(unsigned long normalAddress,unsigned long bit);
extern void BitBandWritePhy(unsigned long normalAddress,unsigned long bit,unsigned long bitValue);
extern unsigned long BitBandReadPhy(unsigned long normalAddress,unsigned long bit);

extern void ndelay(uint32_t ns);
extern void udelay(uint32_t us);
extern void mdelay(uint32_t ms);
extern void InitDelay(void);
extern void SysSleep(void);
extern void SysStop(void);
extern void SysStopLite (void);
extern void SysStandby(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ACM3xxx_H */
