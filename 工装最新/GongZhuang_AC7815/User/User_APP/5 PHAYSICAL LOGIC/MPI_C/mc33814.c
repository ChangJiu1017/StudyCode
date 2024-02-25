/*HEAD
************************************************************************************************************************
* Filename     : mc33814.c
* Function     : MC33814оƬ
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
#define CUTOFFMAX    (30)    //MC33814�Ͽ���������������
#define WRITEMAX     (10)    //MC33814���д����


//POST����Ͷ�ȡ������
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
volatile T_MC33814Status MC33814Status;//״̬�Ĵ����ṹ����


//#define VF_33814SPIBusy_TST    V_33814ST1_TST.b0
//#define VF_33814ClrVRSCtl_TST  V_33814ST1_TST.b1


/***********************************************************************************************************************
* ������   : mc33814_delayTleadTlag_def
* ����     : MC33814SPI������ʱ
* ����     : 2017/9/28
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
static void mc33814_delayTleadTlag_def(void)
{  
    __NOP;
}


/***********************************************************************************************************************
* ������   : mc33814_SPIRwWord_def
* ����     : MC33814����SPI����,д���
* ����     : 2017/9/27
* ������� : w_cmdAdd�����Ƶ�ַ
*            w_regAdd���Ĵ�����ַ
*            w_value�����͵�����
* ����ֵ   : w_recData����ȡ��SPI����
* �޸ļ�¼ :  
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
* ������   : mc33814_PostCheck_def
* ����     : MC33814POST���
* ����     : 2017/9/28
* ������� : ��
* ����ֵ   : 
* �޸ļ�¼ :  
***********************************************************************************************************************/
static void mc33814_PostCheck_def(void) 
{
    byte b_i = 0;
    byte b_readData = 0;
    MC33814Status.mif_postL = 0;
    MC33814Status.mif_postH = 0;
    
    //ʹ��POST
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 0x52U);
    
    for(b_i = 0; b_i < FAULT_REG_COUNT; b_i++) 
    {
        mc33814_SPIRwWord_def(_4000_WRITE_STATUS, sb_StatusReg[b_i][0], sb_StatusReg[b_i][1]);
        b_readData = (byte)mc33814_ReadData_def(_3000_READ_STATUS, sb_StatusReg[b_i][0]);
        if(b_readData != sb_StatusReg[b_i][1])   //���������������д��Ĳ����
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
* ������   : mc33814_ReadData_def
* ����     : MC33814��ȡָ����ַ����
* ����     : 2017/9/27
* ������� : w_cmdAdd�����Ƶ�ַ
*            w_regAdd���Ĵ�����ַ
* ����ֵ   : w_data����ȡ��SPI����
* �޸ļ�¼ :  
***********************************************************************************************************************/
word mc33814_ReadData_def(word w_cmdAdd, word w_regAdd) 
{
    word w_data = 0;
    mc33814_SPIRwWord_def(w_cmdAdd, w_regAdd, 0U);
    w_data = mc33814_SPIRwWord_def(w_cmdAdd, w_regAdd, 0U);
    return w_data;
}


/***********************************************************************************************************************
* ������   : mc33814_SPIWriteWord_def
* ����     : MC33814дָ����ַ���ݣ���У��
* ����     : 2017/11/29
* ������� : w_cmdAddRead������������
             w_cmdAddWrite��д��������
*            w_regAdd���Ĵ�����ַ
* ����ֵ   : bool���ͣ�falseд��ʧ�ܣ�trueд����ȷ
* �޸ļ�¼ :  
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
* ������   : mc33814_RegBitSet_def
* ����     : MC33814дĳ���Ĵ���Bitλ,�ȶ�ȡ���ֽڵ����ݣ���������λ���ı���Ҫ���õ�λ����һ��д��
* ����     : 2017/11/13
* ������� : w_cmdAddRead:������
*            w_cmdAddWrite:д����
*            w_regAdd���Ĵ�����ַ
*            b_bit���Ĵ���Bitλ
*            b_value:ֵ,0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
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
* ������   : mc33814_ReadStatus_def
* ����     : MC33814��ȡ״̬�Ĵ���
* ����     : 2017/10/7
* ������� : w_reg���Ĵ���
* ����ֵ   : value:��ȡ��ֵ
* �޸ļ�¼ :  
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
* ������   : mc33814_AutoReadStatus_def
* ����     : MC33814���ʱ���ȡ״̬�Ĵ������ú���ִ��һ�δ����Ҫ500usʱ��
* ����     : 2017/10/7
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
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
* ������   : mc33814_Mc33814Init_init
* ����     : MC33814��ʼ��
* ����     : 2017/9/28
* ������� : postEn:�Ƿ����Post���
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_Mc33814Init_init(bool postEn) 
{
   V_33814PROC_STATUS = 0;
   
   //POST���
   if(postEn)
   {
       mc33814_PostCheck_def();
   }
   
   //IGN1ģʽ
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0800_IGN1_CONF, 0x90U)) 
       VF_33814INITERR_TST = 1;
   //�ͱÿ���RELAY1ģʽ
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0200_RELAY1_CONF, 0x98U)) 
       VF_33814INITERR_TST = 1;
   
   //����߼��������
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0600_BATSW_CONF, 0x00U)) 
       VF_33814INITERR_TST = 1;
   
   //���Ź� ��ʹ�� 1S
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0A00_WDG_CONF, 0x2AU))
       VF_33814INITERR_TST = 1;
   
   //��
   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0500_LAMP_CONF, 0x18U))
       VF_33814INITERR_TST = 1;

   
   
   //ת�ٱ�
   #ifdef SY_24

   	   #ifndef	SY_HUANSONG
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x17U))//2020-10-9 24�ݲ��ýǱ��Ƶ���ת�٣�17�����ƵN=23
       VF_33814INITERR_TST = 1;

	   #else	//20210922 chenwei add �����Ǳ����IO����ת��
	   
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x58U))//01��58
       VF_33814INITERR_TST = 1;
   	   #endif
		
   #endif
   #ifdef SY_36
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x58U))//01��58
       VF_33814INITERR_TST = 1;
   #endif
   #ifdef SY_12
	   if(!mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0400_TACHO_CONF, 0x09U))//2020-10-9 12�ݲ��ýǱ��Ƶ���ת�٣�17�����ƵN=9
       VF_33814INITERR_TST = 1;
   #endif
    
   
   //VRS����
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
   

   //LDO���
   if(!mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, MAINOFFONDEF))
       VF_33814INITERR_TST = 1; 
   //���ùػ�׼��
   if(!mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 0x92U))
       VF_33814INITERR_TST = 1; 
  
   VF_33814INIT_TST = 1;
}


/***********************************************************************************************************************
* ������   : mc33814_ResetInit_init
* ����     : MC33814��λ��ʼ��
* ����     : 2017/12/28
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
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


//LDO��SPI�������������ʹ��INPUTֱ��������������ʹ��SPI����

/***********************************************************************************************************************
* ������   : mc33814_Rel1SetDuty_def
* ����     : ��Relay1PWMռ�ձ�
* ����     : 2017/9/28
* ������� : b_value��ռ�ձȴ�С
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_Rel1SetDuty_def(byte b_value) 
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0400_RELAY1_CONT, b_value);  
}


/***********************************************************************************************************************
* ������   : mc33814_ClrWdg_50ms
* ����     : MC33814ι���Ź�
* ����     : 2017/9/28
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_ClrWdg_50ms(void)     //50msι��һ��
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0C00_WDG_CONT, 0x80U);   
}
 

/***********************************************************************************************************************
* ������   : mc33814_INJ1Set_def
* ����     : MC33814����INJ1��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/ 
void mc33814_INJ1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 7, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_INJ2Set_def
* ����     : MC33814����INJ1��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/ 
void mc33814_INJ2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 6, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_Rel1Set_def
* ����     : MC33814����Rel1��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/ 
void mc33814_Rel1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 5, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_Rel2Set_def
* ����     : MC33814����Relay2��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/ 
void mc33814_Rel2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 4, b_value);
}
 

/***********************************************************************************************************************
* ������   : mc33814_LampSet_def
* ����     : MC33814����Lamp��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/  
void mc33814_LampSet_def(byte b_value) 
{
    //��ΪĿǰ��Lamp����֮�⣬�����ʹ��I/Oֱ����SPI�������ķ�ʽ������û��Ҫ�˼���������������״̬��Ҳ��û��Ҫ��ȡ��ǰ��main on/off�Ĵ���ֵ
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
* ������   : mc33814_TachSet_def
* ����     : MC33814����Tach��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
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
* ������   : mc33814_IGN1Set_def
* ����     : MC33814����IGN1��������
* ����     : 2017/10/7
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/  
void mc33814_IGN1Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 2, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_IGN2Set_def
* ����     : MC33814����IGN2��������
* ����     : 2017/10/7
* ������� : b_value:״̬   0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/  
void mc33814_IGN2Set_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 1, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_O2HSet_def
* ����     : MC33814���������ȿ���
* ����     : 2017/10/7
* ������� : b_value:״̬  0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/  
void mc33814_O2HSet_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0000_MAIN_OFFON_CONT, 0, b_value);
} 
 
 
 /***********************************************************************************************************************
* ������   : mc33814_WriteVRSRuningParam
* ����     : д����ʱ�����޼��˲�����
* ����     : 2017/11/29
* ������� : b_value������������λ�����ޣ�����λ���˲���ռ
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_WriteVRSRuningParam(byte b_value)
{
    mc33814_SPIWriteWord_def(_1000_READ_CONFIG, _2000_WRITE_CONFIG, _0B00_VRSENGRUNP_CONF, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_WriteVRSCrankingParam
* ����     : д����ʱ�����޼��˲�����
* ����     : 2017/11/29
* ������� : b_value������������λ�����ޣ�����λ���˲���ռ
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_WriteVRSCrankingParam(byte b_value)
{
    mc33814_SPIWriteWord_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0D00_VRSENGCRAP_CONT, b_value);
}


/***********************************************************************************************************************
* ������   : mc33814_VRSCrankingClear
* ����     : ������ʱ�����޼��˲�����
* ����     : 2017/11/29
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void mc33814_VRSCrankingClear(void)
{
    mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0D00_VRSENGCRAP_CONT, 0);
}


/***********************************************************************************************************************
* ������   : mc33814_PrepareToShutDownSet_def
* ����     : MC33814��ʱ�رյ�Դ����
* ����     : 2018/4/16
* ������� : b_value:״̬ 0Ϊ�رգ�1~255Ϊ����
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/  
void mc33814_PrepareToShutDownSet_def(byte b_value) 
{
    mc33814_RegBitSet_def(_5000_READ_CONTROL, _6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 7, b_value);
}



