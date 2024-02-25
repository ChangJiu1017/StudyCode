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
#include "ac78xx_ckgen_regs.h"

/**
* @param[in]  module: CLK_MODULE type. value can be
*                   - CLK_UART1
*                   - CLK_UART2
*                   - CLK_UART3
*                   - CLK_UART4
*                   - CLK_UART5
*                   - CLK_UART6
*                   - CLK_SPI1
*                   - CLK_SPI2
*                   - CLK_I2C1
*                   - CLK_I2C2
*                   - CLK_PWDT
*                   - CLK_PWM0_APB
*                   - CLK_PWM1_APB
*                   - CLK_PWM2_APB
*                   - CLK_PWM3_APB
*                   - CLK_PWM0_TIMER
*                   - CLK_PWM1_TIMER
*                   - CLK_PWM2_TIMER
*                   - CLK_PWM3_TIMER
*                   - CLK_TIM
*                   - CLK_RTC
*                   - CLK_DMA_APB
*                   - CLK_DMA_AHB
*                   - CLK_GPIO_APB
*                   - CLK_GPIO_AHB
*                   - CLK_WDT_APB
*                   - CLK_CRC
*                   - CLK_LIN
*                   - CLK_CAN1
*                   - CLK_CAN2
*                   - CLK_CAN1_CORE
*                   - CLK_CAN2_CORE
*                   - CLK_SFLASH
*                   - CLK_CTU_APB
*                   - CLK_ADC
*                   - CLK_ACMP
* @param[in]  enable: 0:disable, 1: enable
* @return none
*
* @brief  enbale the module clock
*/
int32_t CKGEN_Enable(CLK_MODULE module, uint8_t enable)
{
    int32_t erro = 0;
    uint32_t addr = 0;
    uint32_t moduleValue = (uint32_t)module;

    if (moduleValue < 32)
    {
        addr = PERI_CLK_EN_0;
    }
    else if (moduleValue < (uint32_t)CLK_MODULE_NUM)
    {
        addr = PERI_CLK_EN_1;
        moduleValue -= 32;
    }
    else
    {
        erro = -1;
    }

    if (addr)
    {
        if (enable)
        {
            WRITEBIT32(addr, (uint32_t)(1 << moduleValue));
        }
        else
        {
            CLEARBIT32(addr, (uint32_t)(1 << moduleValue));
        }
    }

    return erro;
}

/**
* @param[in]  module: the module to do Soft Reset. value can be
*                   - SRST_UART1
*                   - SRST_UART2
*                   - SRST_UART3
*                   - SRST_UART4
*                   - SRST_UART5
*                   - SRST_UART6
*                   - SRST_SPI1
*                   - SRST_SPI2
*                   - SRST_I2C1
*                   - SRST_I2C2
*                   - SRST_PWDT
*                   - SRST_PWM0_APB
*                   - SRST_PWM1_APB
*                   - SRST_PWM2_APB
*                   - SRST_PWM3_APB
*                   - SRST_PWM0_TIMER
*                   - SRST_PWM1_TIMER
*                   - SRST_PWM2_TIMER
*                   - SRST_PWM3_TIMER
*                   - SRST_TIM
*                   - SRST_RTC
*                   - SRST_DMA_APB
*                   - SRST_DMA_AHB
*                   - SRST_GPIO_APB
*                   - SRST_GPIO_AHB
*                   - SRST_WDG
*                   - SRST_CRC
*                   - SRST_LIN
*                   - SRST_CAN1
*                   - SRST_CAN2

*                   - //PERI_SFT_RST2
*                   - SRST_SFLASH
*                   - SRST_CTU
*                   - SRST_ADC
*                   - SRST_ACMP
*                   - SRST_ANA_REG
* @param[in]  active: 1: active, 0: inactive
* @return 0 : no error, -1: not support module
*
* @brief  do soft reset for the module
*/
int32_t CKGEN_SoftReset(SRST_MODULE module, uint8_t active)
{
    int32_t erro = 0;
    uint32_t addr = 0;
    uint32_t moduleValue = (uint32_t)module;

    if (moduleValue < 32)
    {
        addr = PERI_SFT_RST1;
    }
    else if (moduleValue < (uint32_t)SRST_MODULE_NUM)
    {
        addr = PERI_SFT_RST2;
        moduleValue -= 32;
    }
    else
    {
        erro = -1;
    }

    if (addr)
    {
        if (active)
        {
            WRITEBIT32(addr, (uint32_t)(1 << moduleValue));
        }
        else
        {
            CLEARBIT32(addr, (uint32_t)(1 << moduleValue));
        }
    }
    return erro;
}


/**
* @param[in]  clockSource: set system clock source, value can be
*                        - SYSCLK_SRC_INTERNAL_OSC
*                        - SYSCLK_SRC_PLL_OUTPUT
*                        - SYSCLK_SRC_EXTERNAL_OSC
* @return none
*
* @brief  set the sysclck clock source
*/
void CKGEN_SetSysclkSrc(SYSCLK_SRC clockSource)
{
    WriteBitsValue(CKGEN_SRC_SEL, SYSCLK_SRC_POS, SYSCLK_SRC_BITWIDTH, (uint32_t)clockSource);
}

/**
* @param[in]  div: system clock divider set, value can be
*                - SYSCLK_DIVIDER_1
*                - SYSCLK_DIVIDER_2
*                - SYSCLK_DIVIDER_4
*                - SYSCLK_DIVIDER_8
*                - SYSCLK_DIVIDER_16
*                - SYSCLK_DIVIDER_64
*                - SYSCLK_DIVIDER_128
*                - SYSCLK_DIVIDER_256
*                - SYSCLK_DIVIDER_512
* @return none
*
* @brief  set the sysclck divider
*/
void CKGEN_SetSysclkDiv(SYSCLK_DIVIDER div)
{
    WriteBitsValue(CKGEN_SRC_SEL, SYSCLK_DIVIDER_POS, SYSCLK_DIVIDER_BITWIDTH, (uint32_t)div);
}

/**
* @param[in]  div: apb clock divider set, value can be
*                - APBCLK_DIVIDER_1
*                - APBCLK_DIVIDER_2
*                - APBCLK_DIVIDER_4
*                - APBCLK_DIVIDER_8
*                - APBCLK_DIVIDER_16
* @return none
*
* @brief  set the APB clock divider
*/
void CKGEN_SetAPBClockDivider(APBCLK_DIVIDER div)
{
    WriteBitsValue(CKGEN_SRC_SEL, APBCLK_DIVIDER_POS, APBCLK_DIVIDER_BITWIDTH, (uint32_t)div);
}

/**
* @param[in]  ref: set PLL reference clock, value can be
*                - PLL_REF_INTERAL_OSC(8M)
*                - PLL_REF_EXTERNAL_OSC
* @return none
*
* @brief  set the PLL reference
*/
void CKGEN_SetPLLReference(PLL_REF ref)
{
    WriteBitsValue(CKGEN_SRC_SEL, PLL_REF_POS, PLL_REF_BITWIDTH, (uint32_t)ref);
}

/**
* @param[in]  div: set pll PREDIV, value can be
*                - PLL_PREDIV_1
*                - PLL_PREDIV_2
*                - PLL_PREDIV_4
* @return none
*
* @brief  set the PLL Previous Divider
*/
void CKGEN_SetPllPrevDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_PREDIV_POS, RG_MCU_SYSPLL1_PREDIV_BITWIDTH, div);
}

/**
* @param[in]  div: set pll post-divider, value can be
*                - PLL_POSDIV_1
*                - PLL_POSDIV_2
*                - PLL_POSDIV_4
* @return none
*
* @brief  set the PLL post Divider
*/
void CKGEN_SetPllPostDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_POSDIV_POS, RG_MCU_SYSPLL1_POSDIV_BITWIDTH, div);
}

/**
* @param[in]  div: set pll FBKDIV, value can be 0 to 255
* @return none
*
* @brief  set the PLL feedback Divider
*/
void CKGEN_SetPllFeedbackDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_FBKDIV_POS, RG_MCU_SYSPLL1_FBKDIV_BITWIDTH, div);
}

/**
* @param[in]  div: set pll clock divider, value can be 0 to 15
* @return none
*
* @brief  set the PLL Divider
*/
void CKGEN_SetPllDiv(uint8_t div)
{
     WriteBitsValue(PLL_CLK_DIV, PLL_CLK_DIV_POS, PLL_CLK_DIV_BITWIDTH, div);
}

/**
* @param[in]  sel: serial flash clock source select, value can be
*                - SFLASH_CLK_SEL_PLL
*                - SFLASH_CLK_SEL_APB
* @param[in]  div: serial flash clock divider set, value can be
*                - SFLASH_DIVIDER_1
*                - SFLASH_DIVIDER_2
*                - SFLASH_DIVIDER_4
*                - SFLASH_DIVIDER_8
* @return none
*
* @brief  set the sFlash clock
*/
void CKGEN_SetSFlashClock(SFLASH_CLK_SEL sel, SFLASH_DIVIDER div)
{
    WriteBitsValue(CKGEN_SRC_SEL, SFLASH_CLK_SEL_POS, SFLASH_CLK_SEL_BITWIDTH, (uint32_t)sel);
    WriteBitsValue(CKGEN_SRC_SEL, SFLASH_DIVIDER_POS, SFLASH_DIVIDER_BITWIDTH, (uint32_t)div);
}


/**
* @param[in]  canIndex: 0: CAN1, 1: CAN2
* @param[in]  sel: can clock source select, value can be
*                - CAN_CLK_SEL_EXTERNAL_OSC
*                - CAN_CLK_SEL_AHB
* @param[in]  div: can clock divider set, value can be
*                - CAN_CLK_DIVIDER_1
*                - CAN_CLK_DIVIDER_2
*                - CAN_CLK_DIVIDER_4
*                - CAN_CLK_DIVIDER_8
* @return none
*
* @brief  set the CAN clock
*/
void CKGEN_SetCANClock(uint8_t canIndex, CAN_CLK_SEL sel, CAN_CLK_DIVIDER div)
{
    if (0 == canIndex)
    {
        WriteBitsValue(CKGEN_SRC_SEL, CAN1_CLK_SEL_POS, CAN1_CLK_SEL_BITWIDTH, (uint32_t)sel);
        WriteBitsValue(CKGEN_SRC_SEL, CAN1_CLK_DIVIDER_POS, CAN1_CLK_DIVIDER_BITWIDTH, (uint32_t)div);
    }
    else
    {
        WriteBitsValue(CKGEN_SRC_SEL, CAN2_CLK_SEL_POS, CAN2_CLK_SEL_BITWIDTH, (uint32_t)sel);
        WriteBitsValue(CKGEN_SRC_SEL, CAN2_CLK_DIVIDER_POS, CAN2_CLK_DIVIDER_BITWIDTH, (uint32_t)div);
    }
}

