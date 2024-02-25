/*===========================================================*/
/*====电池电压处理模块=====================*/
/*===========================================================*/

//功能：根据VBAT的采样结果，计算VBAT的使用值、平均值、级别，以及对油量的修正系数；
//形参：adres_vbat_8bit－－电池电压的8位的AD采集结果
extern void FG_U_bat(word adres_vbat_10bit);

//2008-9-20 ADD
extern void FG_U_bat_BeforeTrig(byte adres_vbat_8bit);

//功能:保存T1触发时刻的vbat
//extern void FM_U_bat_BeforeTrigT1(word vbat);

//功能:保存T3采集时刻的VBAT
//extern void FM_U_bat_whenT3(word vbat);

//功能:保存T1结束时刻的T1
//extern void FM_U_bat_EndT1(word vbat);

extern void FI_U_bat_AfterReset(void);     //2008-12-15

extern byte GetVbat_Tmp(void);//2014-8-12 FROM VOID TO BYTE

extern void VbatSpDiagWhenKeyon(void); 
extern void VbatSpDiagWhenKeyoff(void); 


//----------------------------------------------------------------------------
//v_Vbat_AD
extern sbyte vc_TW1_vbatchg;        // 电池电压变动时，对即将触发的T1的修正系数；
extern sword vc_TW1_vbatdownidle;   // 电池电压下降且怠速时，对即将触发的T1的修正系数； 

extern sword vc_TW1_vbatUpidle;   // 电池电压下降且怠速时，对即将触发的T1的修正系数； //2008-9-20 add

extern byte v_vbat_whenkeyoff;

extern byte v_vbat_atInjOff;//2014-8-12



#define VBAT_FOR_EE 114 //8V,低于这个电压不进行EEPROM操作
//