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
* @param[in]  module: the pointer of the call back function
* @param[in]  enable: 0:disable, 1: enable
* @return none
*
* @brief  enbale the module clock
*/
int32_t CKGEN_Enable(CLK_MODULE module, uint8_t enable)
{
    int32_t erro = 0;
    uint32_t addr = 0;

    if (module < 32)
    {
        addr = PERI_CLK_EN_0;
    }
    else if (module < CLK_MODULE_NUM)
    {
        addr = PERI_CLK_EN_1;
        module -= 32;
    }
    else
    {
        erro = -1;
    }

    if (addr)
    {
        if (enable)
        {
            WRITEBIT32(addr, 1 << module);
        }
        else
        {
            CLEARBIT32(addr, 1 << module);
        }
    }

    return erro;
}

/**
* @param[in]  module: the module to do Soft Reset
* @param[in]  active: 1: active, 0: inactive
* @return 0 : no error, -1: not support module
*
* @brief  do soft reset for the module
*/
int32_t CKGEN_SoftReset(SRST_MODULE module, uint8_t active)
{
    int32_t erro = 0;
    uint32_t addr = 0;

    if (module < 32)
    {
        addr = PERI_SFT_RST1;
    }
    else if (module < SRST_MODULE_NUM)
    {
        addr = PERI_SFT_RST2;
        module -= 32;
    }
    else
    {
        erro = -1;
    }

    if (addr)
    {
        if (active)
        {
            WRITEBIT32(addr, 1 << module);
        }
        else
        {
            CLEARBIT32(addr, 1 << module);
        }
    }
    return erro;
}


/**
* @param[in]  clockSource: clock source
* @return none
*
* @brief  set the sysclck clock source
*/
void CKGEN_SetSysclkSrc(SYSCLK_SRC clockSource)
{
    BITS_VALUE_SET bitValues[2] = {
        {SYSCLK_SRC_POS, SYSCLK_SRC_BITWIDTH, clockSource},
        {PLL_REF_ENABLE_POS, 1, 1},
    };
    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 2);
}

/**
* @param[in]  div: clock divider
* @return none
*
* @brief  set the sysclck divider
*/
void CKGEN_SetSysclkDiv(SYSCLK_DIVIDER div)
{
    BITS_VALUE_SET bitValues[2] = {
        {SYSCLK_DIVIDER_POS, SYSCLK_DIVIDER_BITWIDTH, div},
        {PLL_REF_ENABLE_POS, 1, 1},
    };
    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 2);
}

/**
* @param[in]  div: clock divider
* @return none
*
* @brief  set the APB clock divider
*/
void CKGEN_SetAPBClockDivider(APBCLK_DIVIDER div)
{
    BITS_VALUE_SET bitValues[2] = {
        {APBCLK_DIVIDER_POS, APBCLK_DIVIDER_BITWIDTH, div},
        {PLL_REF_ENABLE_POS, 1, 1},
    };
    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 2);
}

/**
* @param[in]  ref: PLL reference
* @return none
*
* @brief  set the PLL reference
*/
void CKGEN_SetPLLReference(PLL_REF ref)
{
    BITS_VALUE_SET bitValues[2] = {
        {PLL_REF_POS, PLL_REF_BITWIDTH, ref},
        {PLL_REF_ENABLE_POS, 1, 1},
    };
    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 2);
}

/**
* @param[in]  div: divider
* @return none
*
* @brief  set the PLL Previous Divider
*/
void CKGEN_SetPllPrevDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_PREDIV_POS, RG_MCU_SYSPLL1_PREDIV_BITWIDTH, div);
}

/**
* @param[in]  div: divider
* @return none
*
* @brief  set the PLL post Divider
*/
void CKGEN_SetPllPostDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_POSDIV_POS, RG_MCU_SYSPLL1_POSDIV_BITWIDTH, div);
}

/**
* @param[in]  div: divider
* @return none
*
* @brief  set the PLL feedback Divider
*/
void CKGEN_SetPllFeedbackDiv(uint8_t div)
{
    WriteBitsValue(REG_MCU_SYSPLL1_CFG0, RG_MCU_SYSPLL1_FBKDIV_POS, RG_MCU_SYSPLL1_FBKDIV_BITWIDTH, div);
}

/**
* @param[in]  div: divider
* @return none
*
* @brief  set the PLL Divider
*/
void CKGEN_SetPllDiv(uint8_t div)
{
     WriteBitsValue(PLL_CLK_DIV, PLL_CLK_DIV_POS, PLL_CLK_DIV_BITWIDTH, div);
}

/**
* @param[in]  sel: clock selelct
* @param[in]  div: divider
* @return none
*
* @brief  set the sFlash clock
*/
void CKGEN_SetSFlashClock(SFLASH_CLK_SEL sel, SFLASH_DIVIDER div)
{
    BITS_VALUE_SET bitValues[3] = {
        {SFLASH_CLK_SEL_POS, SFLASH_CLK_SEL_BITWIDTH, sel},
        {SFLASH_DIVIDER_POS, SFLASH_DIVIDER_BITWIDTH, div},
        {PLL_REF_ENABLE_POS, 1, 1},
    };
    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 3);
}


/**
* @param[in]  canIndex: 0: CAN1, 1: CAN2
* @param[in]  sel: clock selelct
* @param[in]  div: divider
* @return none
*
* @brief  set the CAN clock
*/
void CKGEN_SetCANClock(uint8_t canIndex, CAN_CLK_SEL sel, CAN_CLK_DIVIDER div)
{

    BITS_VALUE_SET bitValues[3] = {
        {CAN1_CLK_SEL_POS, CAN1_CLK_SEL_BITWIDTH, sel},
        {CAN1_CLK_DIVIDER_POS, CAN1_CLK_DIVIDER_BITWIDTH, div},
        {PLL_REF_ENABLE_POS, 1, 1},
    };

    if (canIndex)
    {
        bitValues[0].pos = CAN2_CLK_SEL_POS;
        bitValues[1].pos = CAN2_CLK_DIVIDER_POS;
    }

    WriteBitsValueArray(CKGEN_SRC_SEL, bitValues, 3);
}

