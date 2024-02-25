/*HEAD
************************************************************************************************************************
* Filename     : mc33814.c
* Function     : MC33814芯片
* User         : yangsili
* Date         : 2017/9/26
* Version      : V9.1.0
************************************************************************************************************************
HEAD END*/


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "mc33814.h"
#include "hard_trig.h"
#include "public.h"
//#include "hidef.h"
//#include "SPI.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CUTOFFMAX    (30)    //MC33814断开连接最大次数限制
#define WRITEMAX     (10)    //MC33814最大写次数


//POST待检和读取的数据
const word sb_StatusReg[][2] = 
{
   {_0000_INJ1_FAULT,        0x8FU},   //0
   {_0100_INJ2_FAULT,        0x8FU},   //1
   {_0200_RELAY1_FAULT,      0x8FU},   //2
   {_0300_RELAY2_FAULT,      0x8FU},   //3
   {_0400_TACHO_FAULT,       0x8EU},   //4
   {_0500_LAMP_FAULT,        0x8FU},   //5
   {_0700_O2H_FAULT,         0x8CU},   //7
   {_0800_IGN1_FAULT,        0x8CU},   //8
   {_0900_IGN2_FAULT,        0x8CU},   //9
   {_0A00_WDG_STATE,         0xFFU},   //10
   {_0B00_VRSISO9414_FAULT,  0xF7U},   //11
   {_0D00_POWSYS_FAULT,      0xFFU},   //13
   {_0E00_SYSONOFF_IND,      0xFFU},   //14
   //{_0F00_MODELCODE_IND,     0xE0U}    //15
};


typedef enum
{
   FAULT_INJ1 = 0,
   FAULT_INJ2,
   FAULT_RELAY1,
   FAULT_RELAY2,
   FAULT_TACHO,
   FAULT_LAMP,
   FAULT_O2H,
   FAULT_IGN1,
   FAULT_IGN2,
   FAULT_WDG,
   FAULT_VRSIOS9414,
   FAULT_POWSYS,
   FAULT_SYSONOFF,
   //FAULT_MODELCODE,
   
   FAULT_REG_COUNT,
}t_StatusReg;


//volatile T_bits V_33814ST1_TST;
volatile T_MC33814Status MC33814Status;//状态寄存器结构变量


//#define VF_33814SPIBusy_TST    V_33814ST1_TST.b0
//#define VF_33814ClrVRSCtl_TST  V_33814ST1_TST.b1


/***********************************************************************************************************************
* 函数名   : mc33814_delayTleadTlag_def
* 功能     : MC33814SPI命令延时
* 日期     : 2017/9/28
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
static void mc33814_delayTleadTlag_def(void)
{  
    __NOP;
}


/***********************************************************************************************************************
* 函数名   : mc33814_SPIRwWord_def
* 功能     : MC33814发送SPI命令,写与读
* 日期     : 2017/9/27
* 输入参数 : w_cmdAdd：控制地址
*            w_regAdd：寄存器地址
*            w_value：发送的数据
* 返回值   : w_recData：读取的SPI数据
* 修改记录 :  
***********************************************************************************************************************/
word mc33814_SPIRwWord_def(word w_cmdAdd, word w_regAdd, word w_value)
{
    word w_recData = 0;
    HAL_SPI_StatusTypeDef recSt;
    static word w_Count = 0;
    //P_MC33814_CS = 0U;
    //mc33814_delayTleadTlag_def();
    recSt = spi_RwWord_def(w_cmdAdd | w_regAdd | w_value, &w_recData);
    //mc33814_delayTleadTlag_def();
    //P_MC33814_CS = 1U;
    
    if(recSt != 0) w_Count++; 
    else  w_Count = 0; 

    if(w_Count > CUTOFFMAX) MC33814Status.mif_cutoff = 1;
    else MC33814Status.mif_cutoff = 0;
    
    return w_recData;    
}


/***********************************************************************************************************************
* 函数名   : mc33814_PostCheck_def
* 功能     : MC33814POST检测
* 日期     : 2017/9/28
* 输入参数 : 无
* 返回值   : 
* 修改记录 :  
***********************************************************************************************************************/
static void mc33814_PostCheck_def(void) 
{
    byte b_i = 0;
    byte b_readData = 0;
    MC33814Status.mif_postL = 0;
    MC33814Status.mif_postH = 0;
    
    //使能POST
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 0x52U);
    
    for(b_i = 0; b_i < FAULT_REG_COUNT; b_i++) 
    {
        mc33814_SPIRwWord_def(_4000_WRITE_STATUS, sb_StatusReg[b_i][0], sb_StatusReg[b_i][1]);
        b_readData = (byte)mc33814_ReadData_def(_3000_READ_STATUS, sb_StatusReg[b_i][0]);
        if(b_readData != sb_StatusReg[b_i][1])   //如果读出的数据与写入的不相等
        {
            VF_33814POSTERR_TST = 1;
            if(b_i < 8)
                MC33814Status.mif_postL |= 1<<b_i;
            else
                MC33814Status.mif_postH |= 1<<(b_i-8); 
        }
    }
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 0x12U);
}


/***********************************************************************************************************************
* 函数名   : mc33814_ReadData_def
* 功能     : MC33814读取指定地址数据
* 日期     : 2017/9/27
* 输入参数 : w_cmdAdd：控制地址
*            w_regAdd：寄存器地址
* 返回值   : w_data：读取的SPI数据
* 修改记录 :  
***********************************************************************************************************************/
word mc33814_ReadData_def(word w_cmdAdd, word w_regAdd) 
{
    word w_data = 0;
    mc33814_SPIRwWord_def(w_cmdAdd, w_regAdd, 0U);
    w_data = mc33814_SPIRwWord_def(w_cmdAdd, w_regAdd, 0U);
    return w_data;
}


/***********************************************************************************************************************
* 函数名   : mc33814_SPIWriteWord_def
* 功能     : MC33814写指定地址数据，并校验
* 日期     : 2017/11/29
* 输入参数 : w_cmdAddRead：读控制命令
             w_cmdAddWrite：写控制命令
*            w_regAdd：寄存器地址
* 返回值   : bool类型：false写入失败，true写入正确
* 修改记录 :  
***********************************************************************************************************************/
bool mc33814_SPIWriteWord_def(word w_cmdAddRead, word w_cmdAddWrite, word w_regAdd, word w_value)
{
    word w_readValue;
    word w_count = 0;
    while(w_count < WRITEMAX)
    {
        mc33814_SPIRwWord_def(w_cmdAddWrite, w_regAdd, w_value);
        w_readValue = mc33814_ReadData_def(w_cmdAddRead, w_regAdd);
        w_readValue &= 0x00FFU;
        if(w_readValue == w_value) return true; 
        else w_count++;     
    }
    return false;
}


/***********************************************************************************************************************
* 函数名   : mc33814_RegBitSet_def
* 功能     : MC33814写某个寄存器Bit位,先读取该字节的数据，保留其他位，改变需要设置的位，再一并写入
* 日期     : 2017/11/13
* 输入参数 : w_cmdAddRead:读命令
*            w_cmdAddWrite:写命令
*            w_regAdd：寄存器地址
*            b_bit：寄存器Bit位
*            b_value:值,0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void mc33814_RegBitSet_def(word w_cmdAddRead, word w_cmdAddWrite, word w_regAdd, byte b_bit, byte b_value)
{
    byte b_oldv;
    byte b_writev;
    b_oldv = mc33814_ReadData_def(w_cmdAddRead, w_regAdd);
    if(b_value == LDO_OFF) 
    {
        b_writev = (b_oldv & (~(1<<b_bit)));
    } 
    else 
    {
        b_writev = b_oldv | (1<<b_bit);
    }
    mc33814_SPIRwWord_def(w_cmdAddWrite, w_regAdd, b_writev);
}


/***********************************************************************************************************************
* 函数名   : mc33814_ReadStatus_def
* 功能     : MC33814读取状态寄存器
* 日期     : 2017/10/7
* 输入参数 : w_reg：寄存器
* 返回值   : value:读取的值
* 修改记录 :  
***********************************************************************************************************************/
byte mc33814_ReadStatus_def(word w_reg)
{
    byte b_value;
    b_value = (byte)mc33814_ReadData_def(_3000_READ_STATUS, w_reg);
    switch(w_reg) 
    {
        case _0000_INJ1_FAULT: 
        {
            MC33814Status.FINJ1.BYTE = b_value;
            break;
        }
        case _0100_INJ2_FAULT: 
        {
            MC33814Status.FINJ2.BYTE = b_value;
            break;
        }
        case _0200_RELAY1_FAULT: 
        {
            MC33814Status.FRelay1.BYTE = b_value;
            break;
        }
        case _0300_RELAY2_FAULT: 
        {
            MC33814Status.FRelay2.BYTE = b_value;
            break;
        }
        case _0400_TACHO_FAULT: 
        {
            MC33814Status.FTachometer.BYTE = b_value;
            break;
        }
        case _0500_LAMP_FAULT: 
        {
            MC33814Status.FLamp.BYTE = b_value;
            break;
        }
        case _0700_O2H_FAULT: 
        {
            MC33814Status.FO2H.BYTE = b_value;
            break;
        }
        case _0800_IGN1_FAULT: 
        {
            MC33814Status.FIGN1.BYTE = b_value;
            break;
        }
        case _0900_IGN2_FAULT: 
        {
            MC33814Status.FIGN2.BYTE = b_value;
            break;
        }
        case _0A00_WDG_STATE: 
        {
            MC33814Status.WatchdogState.BYTE = b_value;
            break;
        }
        case _0B00_VRSISO9414_FAULT: 
        {
            MC33814Status.VRSConditionFISO9414.BYTE = b_value;
            break;
        }
        case _0D00_POWSYS_FAULT: 
        {
            MC33814Status.PowerSupply.BYTE = b_value;
            break;
        }
        case _0E00_SYSONOFF_IND: 
        {
            MC33814Status.SysOnOffInd.BYTE = b_value;
            break;
        }
        case _0F00_MODELCODE_IND: 
        {
            MC33814Status.ModelCode.BYTE = b_value;
            break;
        }
        default:
            break;
    }
    
    return b_value;
}


/***********************************************************************************************************************
* 函数名   : mc33814_AutoReadStatus_def
* 功能     : MC33814间隔时间读取状态寄存器，该函数执行一次大概需要500us时间
* 日期     : 2017/10/7
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_AutoReadStatus_def(void)
{
    byte b_i;
    byte b_num;
    b_num = FAULT_REG_COUNT;
    for(b_i = 0; b_i < b_num; b_i++)
    {
        mc33814_ReadStatus_def(sb_StatusReg[b_i][0]);//, false, 0); 
    }
}


/***********************************************************************************************************************
* 函数名   : mc33814_Mc33814Init_init
* 功能     : MC33814初始化
* 日期     : 2017/9/28
* 输入参数 : postEn:是否进行Post检查
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_Mc33814Init_init(bool postEn) 
{
   V_33814PROC_STATUS = 0;
   
   //POST检查
   if(postEn)
   {
       mc33814_PostCheck_def();
   }
   
   //IGN1模式
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0800_IGN1_CONF, 0x90U)) 
       VF_33814INITERR_TST = 1;
   //油泵控制RELAY1模式
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0200_RELAY1_CONF, 0x98U)) 
       VF_33814INITERR_TST = 1;
   
   //电池逻辑输出控制
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0600_BATSW_CONF, 0x00U)) 
       VF_33814INITERR_TST = 1;
   
   //看门狗 不使能 1S
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0A00_WDG_CONF, 0x2AU))
       VF_33814INITERR_TST = 1;
   
   //灯
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0500_LAMP_CONF, 0x18U))
       VF_33814INITERR_TST = 1;

   
   
   //转速表
   #ifdef SY_24

   	   #ifndef	SY_HUANSONG
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x17U))//2020-10-9 24齿采用角标分频输出转速，17代表分频N=23
       VF_33814INITERR_TST = 1;

	   #else	//20210922 chenwei add 环松仪表采用IO进行转速
	   
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x58U))//01改58
       VF_33814INITERR_TST = 1;
   	   #endif
		
   #endif
   #ifdef SY_36
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x58U))//01改58
       VF_33814INITERR_TST = 1;
   #endif
   #ifdef SY_12
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x09U))//2020-10-9 12齿采用角标分频输出转速，17代表分频N=9
       VF_33814INITERR_TST = 1;
   #endif
    
   
   //VRS设置
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0B00_VRSENGRUNP_CONF, VRSCONFPARAM))
       VF_33814INITERR_TST = 1; 
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0C00_VRSAUTP_CONF, 0x21U))
       VF_33814INITERR_TST = 1; 
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0D00_VRSMISP_CONF, 0x8FU))
       VF_33814INITERR_TST = 1; 
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0D00_VRSMISP_CONF, 0x89U))
       VF_33814INITERR_TST = 1; 
   
   /*SPI control registers*/
   //VRS Engine Cranking Parameter
   //if(!mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0D00_VRSENGCRAP_CONT, 0xE0U))
   //    VF_33814INITERR_TST = 1; 
   

   //LDO输出
   if(!mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, MAINOFFONDEF))
       VF_33814INITERR_TST = 1; 
   //启用关机准备
   if(!mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 0x92U))
       VF_33814INITERR_TST = 1; 
  
   VF_33814INIT_TST = 1;
}


/***********************************************************************************************************************
* 函数名   : mc33814_ResetInit_init
* 功能     : MC33814复位初始化
* 日期     : 2017/12/28
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_ResetInit_init(void) 
{
    //if(VF_33814INIT_TST == 1)
    {
        mc33814_Mc33814Init_init(false);
        if(MC33814Status.resetTimes < 255)
        {
            MC33814Status.resetTimes++;
        }
    }
}


//LDO的SPI驱动函数，如果使用INPUT直接驱动，就无需使用SPI驱动

/***********************************************************************************************************************
* 函数名   : mc33814_Rel1SetDuty_def
* 功能     : 设Relay1PWM占空比
* 日期     : 2017/9/28
* 输入参数 : b_value：占空比大小
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_Rel1SetDuty_def(byte b_value) 
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0400_RELAY1_CONT, b_value);  
}


/***********************************************************************************************************************
* 函数名   : mc33814_ClrWdg_50ms
* 功能     : MC33814喂看门狗
* 日期     : 2017/9/28
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_ClrWdg_50ms(void)     //50ms喂狗一次
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0C00_WDG_CONT, 0x80U);   
}
 

/***********************************************************************************************************************
* 函数名   : mc33814_INJ1Set_def
* 功能     : MC33814设置INJ1驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void mc33814_INJ1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 7, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_INJ2Set_def
* 功能     : MC33814设置INJ1驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void mc33814_INJ2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 6, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_Rel1Set_def
* 功能     : MC33814设置Rel1驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void mc33814_Rel1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 5, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_Rel2Set_def
* 功能     : MC33814设置Relay2驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void mc33814_Rel2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 4, b_value);
}
 

/***********************************************************************************************************************
* 函数名   : mc33814_LampSet_def
* 功能     : MC33814设置Lamp驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_LampSet_def(byte b_value) 
{
    //因为目前除Lamp驱动之外，其余的使用I/O直驱与SPI驱动相或的方式，所以没必要顾及其它的驱动开关状态，也就没必要读取当前的main on/off寄存器值
    //mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 3, b_value);
    byte b_oldv;
    byte b_writev;
    b_oldv = 0U;//MAINOFFONDEF;
    if(b_value == LDO_OFF) 
    {
        b_writev = (b_oldv & (~(1<<3)));
    } 
    else 
    {
        b_writev = b_oldv | (1<<3);
    }
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, b_writev);
}


/***********************************************************************************************************************
* 函数名   : mc33814_TachSet_def
* 功能     : MC33814设置Tach驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_TachSet_def(byte b_value) 
{
    /*byte b_oldv;
    byte b_writev;
    b_oldv = 0U;//MAINOFFONDEF;
    if(b_value == LDO_OFF) 
    {
        b_writev = (b_oldv & (~(1<<1)));
    } 
    else 
    {
        b_writev = b_oldv | (1<<1);
    }
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, b_writev);*/
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 1, b_value);
}
 
/***********************************************************************************************************************
* 函数名   : mc33814_IGN1Set_def
* 功能     : MC33814设置IGN1驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_IGN1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 2, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_IGN2Set_def
* 功能     : MC33814设置IGN2驱动开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态   0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_IGN2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 1, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_O2HSet_def
* 功能     : MC33814设置氧加热开关
* 日期     : 2017/10/7
* 输入参数 : b_value:状态  0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_O2HSet_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 0, b_value);
} 
 
 
 /***********************************************************************************************************************
* 函数名   : mc33814_WriteVRSRuningParam
* 功能     : 写运行时的门限及滤波参数
* 日期     : 2017/11/29
* 输入参数 : b_value：参数，高四位是门限，低四位是滤波比占
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_WriteVRSRuningParam(byte b_value)
{
    mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0B00_VRSENGRUNP_CONF, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_WriteVRSCrankingParam
* 功能     : 写启动时的门限及滤波参数
* 日期     : 2017/11/29
* 输入参数 : b_value：参数，高四位是门限，低四位是滤波比占
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_WriteVRSCrankingParam(byte b_value)
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0D00_VRSENGCRAP_CONT, b_value);
}


/***********************************************************************************************************************
* 函数名   : mc33814_VRSCrankingClear
* 功能     : 清启动时的门限及滤波参数
* 日期     : 2017/11/29
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void mc33814_VRSCrankingClear(void)
{
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0D00_VRSENGCRAP_CONT, 0);
}


/***********************************************************************************************************************
* 函数名   : mc33814_PrepareToShutDownSet_def
* 功能     : MC33814延时关闭电源设置
* 日期     : 2018/4/16
* 输入参数 : b_value:状态 0为关闭，1~255为开启
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/  
void mc33814_PrepareToShutDownSet_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 7, b_value);
}



