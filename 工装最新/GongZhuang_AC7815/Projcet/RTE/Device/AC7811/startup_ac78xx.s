;/* Copyright Statement:
; *
; * This software/firmware and related documentation ("AutoChips Software") are
; * protected under relevant copyright laws. The information contained herein is
; * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
; * the prior written permission of AutoChips inc. and/or its licensors, any
; * reproduction, modification, use or disclosure of AutoChips Software, and
; * information contained herein, in whole or in part, shall be strictly
; * prohibited.
; *
; * AutoChips Inc. (C) 2016. All rights reserved.
; *
; * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
; * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("AUTOCHIPS SOFTWARE")
; * RECEIVED FROM AUTOCHIPS AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
; * ON AN "AS-IS" BASIS ONLY. AUTOCHIPS EXPRESSLY DISCLAIMS ANY AND ALL
; * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
; * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
; * NONINFRINGEMENT. NEITHER DOES AUTOCHIPS PROVIDE ANY WARRANTY WHATSOEVER WITH
; * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
; * INCORPORATED IN, OR SUPPLIED WITH THE AUTOCHIPS SOFTWARE, AND RECEIVER AGREES
; * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
; * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
; * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN AUTOCHIPS
; * SOFTWARE. AUTOCHIPS SHALL ALSO NOT BE RESPONSIBLE FOR ANY AUTOCHIPS SOFTWARE
; * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
; * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AUTOCHIPS'S
; * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE AUTOCHIPS SOFTWARE
; * RELEASED HEREUNDER WILL BE, AT AUTOCHIPS'S OPTION, TO REVISE OR REPLACE THE
; * AUTOCHIPS SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
; * CHARGE PAID BY RECEIVER TO AUTOCHIPS FOR SUCH AUTOCHIPS SOFTWARE AT ISSUE.
; */

Stack_Size      EQU     0x000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack 0
                DCD     Reset_Handler              ; Reset Handler 04
                DCD     NMI_Handler                ; NMI Handler   08
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     PWDT_IRQHandler
                DCD     UART1_IRQHandler
                DCD     UART2_IRQHandler
                DCD     UART3_IRQHandler
                DCD     UART4_IRQHandler
                DCD     UART5_IRQHandler
                DCD     UART6_IRQHandler
                DCD     EXTI0_IRQHandler
                DCD     EXTI1_IRQHandler
                DCD     EXTI2_IRQHandler
                DCD     EXTI3_IRQHandler
                DCD     EXTI4_IRQHandler
                DCD     EXTI5_9_IRQHandler
                DCD     SPI1_IRQHandler
                DCD     SPI2_IRQHandler
                DCD     I2C1_IRQHandler
                DCD     I2C2_IRQHandler
                DCD     DMA1_IRQHandler
                DCD     DMA2_IRQHandler
                DCD     DMA3_IRQHandler
                DCD     DMA4_IRQHandler
                DCD     DMA5_IRQHandler
                DCD     DMA6_IRQHandler
                DCD     DMA7_IRQHandler
                DCD     DMA8_IRQHandler
                DCD     DMA9_IRQHandler
                DCD     DMA10_IRQHandler
                DCD     DMA11_IRQHandler
                DCD     DMA12_IRQHandler
                DCD     TIMER0_IRQHandler
                DCD     TIMER1_IRQHandler
                DCD     BKP_IRQHandler
                DCD     RTC_IRQHandler
                DCD     Watchdog_IRQHandler
                DCD     PWM0_IRQHandler
                DCD     PWM1_IRQHandler
                DCD     PWM2_IRQHandler
                DCD     PVD_Handler
                DCD     LIN0_Handler
                DCD     EXTI10_15_IRQHandler
                DCD     SPM_IRQHandler
                DCD     0
                DCD     CAN1_Handler
                DCD     CAN2_Handler
                DCD     ADC_IRQHandler
                DCD     ACMP0_IRQHandler
                DCD     ACMP1_IRQHandler
                DCD     TIMER2_IRQHandler
                DCD     TIMER3_IRQHandler
                DCD     TIMER4_IRQHandler
                DCD     TIMER5_IRQHandler
                DCD     TIMER6_IRQHandler
                DCD     TIMER7_IRQHandler
                DCD     PWM3_IRQHandler
                DCD     EFlash_IRQHandler
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit

                ; enable lockup reset
                LDR R1, =0x4000000C
                LDR R0, =0x0301FF00
                STR R0, [R1]

                ; enable xosc monitor
                LDR R1, =0x40000000
                LDR R0, [R1]
                ORR R0, R0,#0x00010000
                STR R0, [R1]

                ; enable xosc ready flag
                LDR R1, =0x400088b0
                LDR R0, [R1]
                BIC  R0, R0,#0x10000000
                STR R0, [R1]
                LDR R1, =0x40008880
                LDR R0, [R1]
                BIC  R0, R0,#0x00FF0000
                STR R0, [R1]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                ;IMPORT  HardFaultDebug
                ;LDR R1, =0xE000ED0C
                ;LDR R0, =0x05FA0004
                ;STR R0, [R1]   ; RESET
                ;TST LR,#4
                ;ITE EQ
                ;MRSEQ R0,MSP
                ;MRSNE R0,PSP
                ;B HardFaultDebug
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  PWDT_IRQHandler      [WEAK]
                EXPORT  UART1_IRQHandler     [WEAK]
                EXPORT  UART2_IRQHandler     [WEAK]
                EXPORT  UART3_IRQHandler     [WEAK]
                EXPORT  UART4_IRQHandler     [WEAK]
                EXPORT  UART5_IRQHandler     [WEAK]
                EXPORT  UART6_IRQHandler     [WEAK]
                EXPORT  EXTI0_IRQHandler     [WEAK]
                EXPORT  EXTI1_IRQHandler     [WEAK]
                EXPORT  EXTI2_IRQHandler     [WEAK]
                EXPORT  EXTI3_IRQHandler     [WEAK]
                EXPORT  EXTI4_IRQHandler     [WEAK]
                EXPORT  EXTI5_9_IRQHandler   [WEAK]
                EXPORT  SPI1_IRQHandler      [WEAK]
                EXPORT  SPI2_IRQHandler      [WEAK]
                EXPORT  I2C1_IRQHandler      [WEAK]
                EXPORT  I2C2_IRQHandler      [WEAK]
                EXPORT  DMA1_IRQHandler      [WEAK]
                EXPORT  DMA2_IRQHandler      [WEAK]
                EXPORT  DMA3_IRQHandler      [WEAK]
                EXPORT  DMA4_IRQHandler      [WEAK]
                EXPORT  DMA5_IRQHandler      [WEAK]
                EXPORT  DMA6_IRQHandler      [WEAK]
                EXPORT  DMA7_IRQHandler      [WEAK]
                EXPORT  DMA8_IRQHandler      [WEAK]
                EXPORT  DMA9_IRQHandler      [WEAK]
                EXPORT  DMA10_IRQHandler     [WEAK]
                EXPORT  DMA11_IRQHandler     [WEAK]
                EXPORT  DMA12_IRQHandler     [WEAK]
                EXPORT  TIMER0_IRQHandler    [WEAK]
                EXPORT  TIMER1_IRQHandler    [WEAK]
                EXPORT  BKP_IRQHandler       [WEAK]
                EXPORT  RTC_IRQHandler       [WEAK]
                EXPORT  Watchdog_IRQHandler  [WEAK]
                EXPORT  PWM0_IRQHandler      [WEAK]
                EXPORT  PWM1_IRQHandler      [WEAK]
                EXPORT  PWM2_IRQHandler      [WEAK]
                EXPORT  PVD_Handler          [WEAK]
                EXPORT  LIN0_Handler         [WEAK]
                EXPORT  EXTI10_15_IRQHandler [WEAK]
                EXPORT  SPM_IRQHandler       [WEAK]
                EXPORT  CAN1_Handler         [WEAK]
                EXPORT  CAN2_Handler         [WEAK]
                EXPORT  ADC_IRQHandler       [WEAK]
                EXPORT  ACMP0_IRQHandler     [WEAK]
                EXPORT  ACMP1_IRQHandler     [WEAK]
                EXPORT  TIMER2_IRQHandler    [WEAK]
                EXPORT  TIMER3_IRQHandler    [WEAK]
                EXPORT  TIMER4_IRQHandler    [WEAK]
                EXPORT  TIMER5_IRQHandler    [WEAK]
                EXPORT  TIMER6_IRQHandler    [WEAK]
                EXPORT  TIMER7_IRQHandler    [WEAK]
                EXPORT  PWM3_IRQHandler      [WEAK]
                EXPORT  EFlash_IRQHandler    [WEAK]
PWDT_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
UART6_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
EXTI5_9_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
BKP_IRQHandler
RTC_IRQHandler
Watchdog_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
PWM2_IRQHandler
PVD_Handler
LIN0_Handler
EXTI10_15_IRQHandler
SPM_IRQHandler
;0
CAN1_Handler
CAN2_Handler
ADC_IRQHandler
ACMP0_IRQHandler
ACMP1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
TIMER4_IRQHandler
TIMER5_IRQHandler
TIMER6_IRQHandler
TIMER7_IRQHandler
PWM3_IRQHandler
EFlash_IRQHandler
                b .
                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

