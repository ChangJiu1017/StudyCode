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
#include <stdio.h>
#include "ac78xx.h"
#include "ac78xx_gpio.h"
#include "Core_cm3.h"

extern uint8_t CopyData(char *src, int len);

#define MAX_DEBUG_BUFF_SIZE 100

///< debug uart receive buf
uint8_t g_debugBuff[MAX_DEBUG_BUFF_SIZE] = {0};

///< debug init flag
static uint8_t s_debugInit = 0;

#if defined ( __ICCARM__ )

#elif defined ( __GNUC__)

#elif defined (__CC_ARM)
#pragma import(__use_no_semihosting)
#endif


///< define std FILE struct
struct __FILE
{
    int handle;

};

///< refine stdout
FILE __stdout;

/**
* @param[in]  x:  no use
*
* @brief  refine _sys_exit
*/
int _sys_exit(int x)
{
    x = x;
    return 0;
}

#ifdef ITM_DEBUG_OUT
/**
* @param[in]  f:  file pointer for the std input
* @param[in]  ch: the char to put
* @return return the char of be put
*
* @brief  put a char to ITM
*/
int fputc(int ch, FILE *f)
{
    if (s_debugInit)
    {
        ITM_SendChar(ch);
    }
    return ch;
}

///< for ITM use
volatile int ITM_RxBuffer;

/**
* @param[in]  f:  file pointer for the std input
* @return -1: not get char,  other: the char get from UART
*
* @brief  get a char
*/
int fgetc(FILE* f)
{
    int ch = ITM_ReceiveChar();

    return ch;
}
#else
#include "ac78xx_uart.h"

#define DEBUG_UART                UART2
#define DEBUG_UART_IRQ            UART2_IRQn
#define DEBUG_UART_CLK            CLK_UART2
#define DEBUG_UART_SRST           SRST_UART2

/**
* @param[in]  f:  file pointer for the std input
* @param[in]  ch: the char to put
* @return return the char of be put
*
* @brief  put a char to UART
*/
int fputc(int ch, FILE *f)
{
    if (s_debugInit)
    {
        UART_SendData(DEBUG_UART, ch);
    }
    return ch;
}

/**
* @param[in]  f:  file pointer for the std input
* @return -1: not get char,  other: the char get from UART
*
* @brief  get a char
*/
int fgetc(FILE* f)
{
    int ch = -1;
    if (s_debugInit)
    {
        if (UART_RxIsDataReady(DEBUG_UART))
        {
            ch = UART_ReceiveData(DEBUG_UART);
        }
    }

    return ch;
}

#endif

/**
* @return none
*
* @brief  set the debug out is invalid
*/
void DeinitDebug(void)
{
    s_debugInit = 0;
}

/**
* @return none
*
* @brief  init debug out , and set the debug out is valid
*/
void InitDebug(void)
{
#ifdef ITM_DEBUG_OUT
#else

    GPIO_SetFunc(49, 1);
    GPIO_SetFunc(50, 1);
    CKGEN_Enable(DEBUG_UART_CLK, 1);
    CKGEN_SoftReset(DEBUG_UART_SRST, 1);

    UART_SetDivisor(DEBUG_UART, APB_BUS_FREQ / 16.0 / 115200); //apbbus: 48M

    UART_SetDataBits(DEBUG_UART, 8);
    UART_SetStopBit(DEBUG_UART, 1);
    UART_EnableTX(DEBUG_UART, 1);
    UART_EnableRX(DEBUG_UART, 1);
    UART_Set2ByteFIFO(DEBUG_UART, 1);

    UART_SetIntEn(DEBUG_UART, 9);
#endif

    s_debugInit = 1;
}

/**
* @param[in]  stackInfo:  stack infomation
* @return none
*
* @brief  print the stack infomation when occur HardFault
*/
void HardFaultDebug(const unsigned int * stackInfo)
{
    unsigned int stackR0;
    unsigned int stackR1;
    unsigned int stackR2;
    unsigned int stackR3;
    unsigned int stackR12;
    unsigned int stackLR;
    unsigned int stackPC;
    unsigned int stackPSR;

    stackR0 = ((unsigned long) stackInfo[0]);
    stackR1 = ((unsigned long) stackInfo[1]);
    stackR2 = ((unsigned long) stackInfo[2]);
    stackR3 = ((unsigned long) stackInfo[3]);
    stackR12 = ((unsigned long) stackInfo[4]);
    stackLR = ((unsigned long) stackInfo[5]);
    stackPC = ((unsigned long) stackInfo[6]);
    stackPSR = ((unsigned long) stackInfo[7]);

    printf("R0=%x\n", stackR0);
    printf("R1=%x\n", stackR1);
    printf("R2=%x\n", stackR2);
    printf("R3=%x\n", stackR3);
    printf("R12=%x\n", stackR12);
    printf("LR[R14]=%x subroutine call return address\n", stackLR);
    printf("PC[R15]=%x program counter\n", stackPC);
    printf("PSR=%x\n", stackPSR);

    printf("BFAR=%x\n", (*((volatile unsigned long *)(0xE000ED38))));
    printf("CFSR=%x\n", (*((volatile unsigned long *)(0xE000ED28))));
    printf("HFSR=%x\n", (*((volatile unsigned long *)(0xE000ED2C))));
    printf("DFSR=%x\n", (*((volatile unsigned long *)(0xE000ED30))));
    printf("AFSR=%x\n", (*((volatile unsigned long *)(0xE000ED3C))));

    printf("SCB_SHCSR=%x\n", SCB->SHCSR);

    while(1);
}




