/*===========================================================*/
/*====��ص�ѹ����ģ��=====================*/
/*===========================================================*/

//���ܣ�����VBAT�Ĳ������������VBAT��ʹ��ֵ��ƽ��ֵ�������Լ�������������ϵ����
//�βΣ�adres_vbat_8bit������ص�ѹ��8λ��AD�ɼ����
extern void FG_U_bat(word adres_vbat_10bit);

//2008-9-20 ADD
extern void FG_U_bat_BeforeTrig(byte adres_vbat_8bit);

//����:����T1����ʱ�̵�vbat
//extern void FM_U_bat_BeforeTrigT1(word vbat);

//����:����T3�ɼ�ʱ�̵�VBAT
//extern void FM_U_bat_whenT3(word vbat);

//����:����T1����ʱ�̵�T1
//extern void FM_U_bat_EndT1(word vbat);

extern void FI_U_bat_AfterReset(void);     //2008-12-15

extern byte GetVbat_Tmp(void);//2014-8-12 FROM VOID TO BYTE

extern void VbatSpDiagWhenKeyon(void); 
extern void VbatSpDiagWhenKeyoff(void); 


//----------------------------------------------------------------------------
//v_Vbat_AD
extern sbyte vc_TW1_vbatchg;        // ��ص�ѹ�䶯ʱ���Լ���������T1������ϵ����
extern sword vc_TW1_vbatdownidle;   // ��ص�ѹ�½��ҵ���ʱ���Լ���������T1������ϵ���� 

extern sword vc_TW1_vbatUpidle;   // ��ص�ѹ�½��ҵ���ʱ���Լ���������T1������ϵ���� //2008-9-20 add

extern byte v_vbat_whenkeyoff;

extern byte v_vbat_atInjOff;//2014-8-12



#define VBAT_FOR_EE 114 //8V,���������ѹ������EEPROM����
//