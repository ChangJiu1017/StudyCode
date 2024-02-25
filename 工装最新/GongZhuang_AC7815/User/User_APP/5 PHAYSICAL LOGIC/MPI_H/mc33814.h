/*HEAD
************************************************************************************************************************
* Filename     : mc33814.h
* Function     : MC33814芯片头文件
* User         : yangsili
* Date         : 2017/9/26
* Version      : V9.1.0
************************************************************************************************************************
HEAD END*/


#ifndef _MC33814_H_
#define _MC33814_H_

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "PE_Types.h"
#include "FAM.h"

#pragma MESSAGE DISABLE C1106 /* WARNING C1106: Non-standard bitfield type */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
//typedef enum
//{
//    SPI_OK = 0,
//    SPI_RESET,
//    SPI_BUSY,
//    SPI_TIMEOUT,
//}HAL_SPI_StatusTypeDef;

#define  LDO_ON    1U
#define  LDO_OFF   0U

#define MAINOFFONDEF (0x00U)     
#define VRSCONFPARAM (0xF3U)  //2020/05/19修改，由0xE3（1.21V）修改为0XF3(1.715V)


//Control Address/控制地址
#define  _0000_SPI_CHECK        0x0000U
#define  _1000_READ_CONFIG      0x1000U
#define  _2000_WRITE_CONFIG     0x2000U
#define  _3000_READ_STATUS      0x3000U
#define  _4000_WRITE_STATUS     0x4000U
#define  _5000_READ_CONTROL     0x5000U
#define  _6000_WRITE_CONTROL    0x6000U
#define  _7000_SPI_CHECK_RSE    0x7000U


//SPI configuration registers address
#define  _0000_INJ1_CONF        0x0000U
#define  _0100_INJ2_CONF        0x0100U
#define  _0200_RELAY1_CONF      0x0200U
#define  _0300_RELAY2_CONF      0x0300U
#define  _0400_TACHO_CONF       0x0400U
#define  _0500_LAMP_CONF        0x0500U
#define  _0600_BATSW_CONF       0x0600U
#define  _0700_O2H_CONF         0x0700U
#define  _0800_IGN1_CONF        0x0800U
#define  _0900_IGN2_CONF        0x0900U
#define  _0A00_WDG_CONF         0x0A00U
#define  _0B00_VRSENGRUNP_CONF  0x0B00U
#define  _0C00_VRSAUTP_CONF     0x0C00U
#define  _0D00_VRSMISP_CONF     0x0D00U


//SPI control registers
#define _0000_MAIN_OFFON_CONT   0x0000U
#define _0100_OTHER_OFFON_CONT  0x0100U
#define _0200_INJ1_CONT         0x0200U
#define _0300_INJ2_CONT         0x0300U
#define _0400_RELAY1_CONT       0x0400U
#define _0500_RELAY2_CONT       0x0500U
#define _0600_TACHO_CONT        0x0600U
#define _0700_LAMP_CONT         0x0700U
#define _0800_BATSW_CONT        0x0800U
#define _0900_O2H_CONT          0x0900U
#define _0A00_IGN1_CONT         0x0A00U
#define _0B00_IGN2_CONT         0x0B00U
#define _0C00_WDG_CONT          0x0C00U
#define _0D00_VRSENGCRAP_CONT   0x0D00U


//SPI status registers
#define _0000_INJ1_FAULT        0x0000U
#define _0100_INJ2_FAULT        0x0100U
#define _0200_RELAY1_FAULT      0x0200U
#define _0300_RELAY2_FAULT      0x0300U
#define _0400_TACHO_FAULT       0x0400U
#define _0500_LAMP_FAULT        0x0500U
#define _0700_O2H_FAULT         0x0700U
#define _0800_IGN1_FAULT        0x0800U
#define _0900_IGN2_FAULT        0x0900U
#define _0A00_WDG_STATE         0x0A00U
#define _0B00_VRSISO9414_FAULT  0x0B00U
#define _0D00_POWSYS_FAULT      0x0D00U
#define _0E00_SYSONOFF_IND      0x0E00U
#define _0F00_MODELCODE_IND     0x0F00U



//PWM
#define _0000_NONE_EXTPIN       0x0000U
#define _0001_100HZ             0x0001U
#define _0002_1KHZ              0x0002U
#define _0003_EXT_100DC         0x0003U


#define T_MAINTCNT TCNT                   //主定时器值


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
 typedef union 
 {                             //byte                            
    byte BYTE;
    struct 
    {
        byte b0  :1;                                       
        byte b1  :1;                                       
        byte b2  :1;                                       
        byte b3  :1;                                       
        byte b4  :1;                                       
        byte b5  :1;                                       
        byte b6  :1;                                       
        byte b7  :1;                                       
    } Bits;
 }T_BIT_ST;


typedef struct 
{
    byte ShortGND    : 1;
    byte OverTemp    : 1; 
    byte OverCurrent : 1; 
    byte OpenLoad    : 1; 
    byte Undefined   : 3; 
    byte Faults      : 1; 
}T_bitLdoF;


//MC33814状态类型定义
typedef struct  
{
    byte mif_cutoff;
    byte mif_postL;
    byte mif_postH;
    //byte mif_init;
    T_BIT_ST Status;
    byte resetTimes;
    
    union 
    {
        byte BYTE;
        T_bitLdoF BIT;
    }FINJ1;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FINJ2;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FRelay1;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FRelay2;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FTachometer;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FLamp;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FO2H;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FIGN1;
    
    union 
    { 
        byte BYTE;
        T_bitLdoF BIT;
    }FIGN2;
    
    
    union 
    { 
        byte BYTE;
        struct 
        {
            byte WDTimer  : 7;
            byte Enable   : 1; 
        }BIT;
    }WatchdogState;
    
    union 
    { 
        byte BYTE;
        struct 
        {
            byte ISO_OT            : 1;
            byte ClampActiveVRSN   : 1; 
            byte ClampActiveVRSP   : 1; 
            byte Undefined         : 1; 
            byte Peak              : 4; 
        }BIT;
    }VRSConditionFISO9414;
    
    union 
    { 
        byte BYTE;
        struct 
        {
            byte VprotShortGND     : 1;
            byte VprotOverTemp     : 1; 
            byte VprotShortBat     : 1; 
            byte SPIError          : 1; 
            byte Batsw             : 1; 
            byte Pwren             : 1; 
            byte Keysw             : 1; 
            byte AnySysFaults      : 1; 
        }BIT;
    }PowerSupply;
    
    union 
    { 
        byte BYTE;
        struct 
        {
            byte O2H      : 1;
            byte IGN2     : 1; 
            byte IGN1     : 1; 
            byte LAMP     : 1; 
            byte Relay2   : 1; 
            byte Relay1   : 1; 
            byte INJ2     : 1; 
            byte INJ1     : 1; 
        }BIT;
    }SysOnOffInd;
    
    union 
    { 
        byte BYTE;
        struct 
        {
            byte Rev         : 5;
            byte model0      : 1; 
            byte model1      : 1; 
            byte model2      : 1; 
        }BIT;
    }ModelCode;
    
}T_MC33814Status;



#define LDO_33814_SGBIT   0
#define LDO_33814_OTBIT   1
#define LDO_33814_OCBIT   2
#define LDO_33814_OLBIT   3
#define LDO_33814_FTBIT   7


extern volatile T_MC33814Status MC33814Status;



//INJ1
#define P_33814INJ1_ST     MC33814Status.FINJ1.BYTE
#define P_33814INJ1_SHORT  MC33814Status.FINJ1.BIT.ShortGND
#define P_33814INJ1_OT     MC33814Status.FINJ1.BIT.OverTemp
#define P_33814INJ1_OC     MC33814Status.FINJ1.BIT.OverCurrent
#define P_33814INJ1_OPEN   MC33814Status.FINJ1.BIT.OpenLoad
#define P_33814INJ1_FAULT  MC33814Status.FINJ1.BIT.Faults

//INJ2
#define P_33814INJ2_ST     MC33814Status.FINJ2.BYTE
#define P_33814INJ2_SHORT  MC33814Status.FINJ2.BIT.ShortGND
#define P_33814INJ2_OT     MC33814Status.FINJ2.BIT.OverTemp
#define P_33814INJ2_OC     MC33814Status.FINJ2.BIT.OverCurrent
#define P_33814INJ2_OPEN   MC33814Status.FINJ2.BIT.OpenLoad
#define P_33814INJ2_FAULT  MC33814Status.FINJ2.BIT.Faults

//RELAY1
#define P_33814REL1_ST     MC33814Status.FRelay1.BYTE
#define P_33814REL1_SHORT  MC33814Status.FRelay1.BIT.ShortGND
#define P_33814REL1_OT     MC33814Status.FRelay1.BIT.OverTemp
#define P_33814REL1_OC     MC33814Status.FRelay1.BIT.OverCurrent
#define P_33814REL1_OPEN   MC33814Status.FRelay1.BIT.OpenLoad
#define P_33814REL1_FAULT  MC33814Status.FRelay1.BIT.Faults

//RELAY2
#define P_33814REL2_ST     MC33814Status.FRelay2.BYTE
#define P_33814REL2_SHORT  MC33814Status.FRelay2.BIT.ShortGND
#define P_33814REL2_OT     MC33814Status.FRelay2.BIT.OverTemp
#define P_33814REL2_OC     MC33814Status.FRelay2.BIT.OverCurrent
#define P_33814REL2_OPEN   MC33814Status.FRelay2.BIT.OpenLoad
#define P_33814REL2_FAULT  MC33814Status.FRelay2.BIT.Faults

//Tachometer
#define P_33814TACH_SHORT  MC33814Status.FTachometer.BIT.ShortGND
#define P_33814TACH_OT     MC33814Status.FTachometer.BIT.OverTemp
#define P_33814TACH_OC     MC33814Status.FTachometer.BIT.OverCurrent
#define P_33814TACH_OPEN   MC33814Status.FTachometer.BIT.OpenLoad
#define P_33814TACH_FAULT  MC33814Status.FTachometer.BIT.Faults

//LAMP
#define P_33814LAMP_SHORT  MC33814Status.FLamp.BIT.ShortGND
#define P_33814LAMP_OT     MC33814Status.FLamp.BIT.OverTemp
#define P_33814LAMP_OC     MC33814Status.FLamp.BIT.OverCurrent
#define P_33814LAMP_OPEN   MC33814Status.FLamp.BIT.OpenLoad
#define P_33814LAMP_FAULT  MC33814Status.FLamp.BIT.Faults

//O2H
#define P_33814O2H_ST      MC33814Status.FO2H.BYTE
#define P_33814O2H_OC      MC33814Status.FO2H.BIT.OverCurrent
#define P_33814O2H_OPEN    MC33814Status.FO2H.BIT.OpenLoad
#define P_33814O2H_FAULT   MC33814Status.FO2H.BIT.Faults

//IGN1
#define P_33814IGN1_ST     MC33814Status.FIGN1.BYTE
#define P_33814IGN1_OC     MC33814Status.FIGN1.BIT.OverCurrent
#define P_33814IGN1_OPEN   MC33814Status.FIGN1.BIT.OpenLoad
#define P_33814IGN1_FAULT  MC33814Status.FIGN1.BIT.Faults

//IGN2
#define P_33814IGN2_OC     MC33814Status.FIGN2.BIT.OverCurrent
#define P_33814IGN2_OPEN   MC33814Status.FIGN2.BIT.OpenLoad
#define P_33814IGN2_FAULT  MC33814Status.FIGN2.BIT.Faults

//Watchdog
//VRS Conditioner and ISO9141 Faults
//PowerSupply
#define V_33814SYS_TST     MC33814Status.PowerSupply.BIT.AnySysFaults
#define P_33814KEY_TST     MC33814Status.PowerSupply.BIT.Keysw
#define P_33814PWREN_TST   MC33814Status.PowerSupply.BIT.Pwren
#define P_33814BATSW_TST   MC33814Status.PowerSupply.BIT.Batsw
#define P_33814SPI_FAULT   MC33814Status.PowerSupply.BIT.SPIError


#define V_33814PROC_STATUS      MC33814Status.Status.BYTE
#define VF_33814INITERR_TST     MC33814Status.Status.Bits.b0   //初始化错误
#define VF_33814POSTERR_TST     MC33814Status.Status.Bits.b1   //初始化POST错误
#define VF_33814INIT_TST        MC33814Status.Status.Bits.b2   //初始化状态
#define VF_33814SPIBUSY_TST     MC33814Status.Status.Bits.b3   //SPI总线状态
#define VF_33814CLRINITVRS_TST  MC33814Status.Status.Bits.b4   //VRS Control控制是否清空 1：清空，0：未清空



/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
word mc33814_ReadData_def(word w_cmdAdd, word w_regAdd);
byte mc33814_ReadStatus_def(word w_reg);
word mc33814_SPIRwWord_def(word w_cmdAdd, word w_regAdd, word w_value);
bool mc33814_SPIWriteWord_def(word w_cmdAddRead, word w_cmdAddWrite, word w_regAdd, word w_value);
void mc33814_Mc33814Init_init(bool postEn);
void mc33814_ResetInit_init(void);
void mc33814_AutoReadStatus_def(void);
void mc33814_Rel1SetDuty_def(byte b_value);
void mc33814_ClrWdg_50ms(void);
void mc33814_RegBitSet_def(word w_cmdAddRead, word w_cmdAddWrite, word w_regAdd, byte b_bit, byte b_value);
void mc33814_INJ1Set_def(byte b_value);
void mc33814_INJ2Set_def(byte b_value);
void mc33814_Rel1Set_def(byte b_value);
void mc33814_Rel2Set_def(byte b_value);
void mc33814_TachSet_def(byte b_value);
void mc33814_LampSet_def(byte b_value);
void mc33814_IGN1Set_def(byte b_value);
void mc33814_IGN2Set_def(byte b_value);
void mc33814_O2HSet_def(byte b_value);
void mc33814_WriteVRSRuningParam(byte b_value);
void mc33814_WriteVRSCrankingParam(byte b_value);
void mc33814_VRSCrankingClear(void);
void mc33814_PrepareToShutDownSet_def(byte b_value);

#endif //_MC33814_H_
