

#ifndef __FAM_H__
#define __FAM_H__

#define M_Debug								1
#define M_DebugApp						0
#include "ac78xx_acmp.h"
#include "ac78xx_adc.h"
#include "ac78xx_can.h"
#include "ac78xx_crc.h"
#include "ac78xx_ctu.h"
#include "ac78xx_dma.h"
#include "ac78xx_gpio.h"
#include "ac78xx_i2c.h"
#include "ac78xx_lin.h"
#include "ac78xx_pwdt.h"
#include "ac78xx_pwm.h"
#include "ac78xx_rtc.h"
#include "ac78xx_spi.h"
#include "ac78xx_serialflash.h"
#include "ac78xx_timer.h"
#include "ac78xx_uart.h"
#include "ac78xx_wdg.h"
#include "ac78xx_eflash.h"
//#include "ac78xx_spm.h"

#define sbyte signed char
#define sword short int
#define sdword long int
#define sddword long long

#define byte unsigned char
#define word unsigned short int
#define dword unsigned long int
#define ddword unsigned long long
typedef unsigned char   bool;
	
typedef char(*char_P)[];

typedef enum
{
    SPI_OK = 0,
    SPI_RESET,
    SPI_BUSY,
    SPI_TIMEOUT,
}HAL_SPI_StatusTypeDef;

#include "MCUabs_GPIO.h"	
#include "MCUabs_SCI.h"
#include "MCUabs_ADC.h"
#include "MCUabs_CAN.h"
#include "MCUabs_IIC.h"
#include "MCUabs_RTC.h"
#include "MCUabs_WGD.h"
#include "Inj_Ing.h"


/*#include "data_sys_macro.h"	
#include "data_sys_standard.h"
#include "Header.h"
#include "public.h"
#include "sys_definition.h"
#include "add.h"
#include "add_ee.h"
#include "add_ee_mpi.h"
#include "add_ee_scr.h"	
#include "add_flash.h"
#include "add_flash_mpi.h"
#include "add_ram.h"
#include "add_ram_MPI.h"
#include "derivative.h"
#include "events.h"
#include "Events_mpi.h"
#include "hard_trig.h"	
#include "MCU.h"
#include "MCU_MPI.h"
#include "PE_Const.h"
#include "PE_Error.h"
#include "PE_Types.h"*/

#define T_MAINTMR 			PWM2->CNT//TCNT//RTC->CNT//                   //主定时器值
#define CLR_WDG 				WDOG_Feed();//{CPMUARMCOP=0x55;CPMUARMCOP=0x55;CPMUARMCOP=0xAA;}

#define FALSE						(0U)
#define TRUE						(1U)	

#define M_AppStartAddr	(0x08006000)

#define BootFlagApp  *((unsigned char *)(0x20003F00))//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
#define BootFlagApp1  *((unsigned char *)(0x20003F01))
#define BootFlagApp2  *((unsigned char *)(0x20003F02))
#define BootFlagApp3  *((unsigned char *)(0x20003F03))
#define BootFlagApp4  *((unsigned char *)(0x20003F04))
#define BootFlagApp5  *((unsigned char *)(0x20003F05))
#define BootFlagApp6  *((unsigned char *)(0x20003F06))
#define BootFlagApp7  *((unsigned char *)(0x20003F07))
#define BootFlagApp8  *((unsigned char *)(0x20003F08))//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
#define BootFlagApp9  *((unsigned char *)(0x20003F09))
#define BootFlagAppA  *((unsigned char *)(0x20003F0A))
#define BootFlagAppB  *((unsigned char *)(0x20003F0B))
#define BootFlagAppC  *((unsigned char *)(0x20003F0C))
#define BootFlagAppD  *((unsigned char *)(0x20003F0D))
#define BootFlagAppE  *((unsigned char *)(0x20003F0E))
#define BootFlagAppF  *((unsigned char *)(0x20003F0F))
#define BootFlagFlash  *(unsigned char *)(0x20003F10)//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
#define BootFlagFlash1  *(unsigned char *)(0x20003F11)
#define BootFlagFlash2  *(unsigned char *)(0x20003F12)
#define BootFlagFlash3  *(unsigned char *)(0x20003F13)
#define BootFlagFlash4  *(unsigned char *)(0x20003F14)
#define BootFlagFlash5  *(unsigned char *)(0x20003F15)
#define BootFlagFlash6  *(unsigned char *)(0x20003F16)
#define BootFlagFlash7  *(unsigned char *)(0x20003F17)
#define BootFlagFlash8  *(unsigned char *)(0x20003F18)//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
#define BootFlagFlash9  *(unsigned char *)(0x20003F19)
#define BootFlagFlashA  *(unsigned char *)(0x20003F1A)
#define BootFlagFlashB  *(unsigned char *)(0x20003F1B)
#define BootFlagFlashC  *(unsigned char *)(0x20003F1C)
#define BootFlagFlashD  *(unsigned char *)(0x20003F1D)
#define BootFlagFlashE  *(unsigned char *)(0x20003F1E)
#define BootFlagFlashF  *(unsigned char *)(0x20003F1F)

void Can_Busoff_Reset(void);
void FUN_Hardwr_Init(void);
void FUN_Var_Init(void);
void FUN_APP_Scheduler(void);
void TIMER_Interrupt_Control_Timer1(void);
void TIMER_Interrupt_Control_Timer0(void);
void CAN_Send_adc(void);
#define PE_low_level_init				FUN_Hardwr_Init


#endif


