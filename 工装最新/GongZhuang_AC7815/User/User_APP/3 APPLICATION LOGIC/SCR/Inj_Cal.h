#include "hard_trig.h"	


#define Q_MLPERHOUR_FORPRIMING  7000

//#define Q_MLPERHOUR_FORINJ_MIN  75     //�õ���С��������С��������Ͳ�������
#define Q_MLPERHOUR_FORINJ_MIN  30     //�õ���С��������С��������Ͳ�������,2015-1-7��������ʦҪ��

//#define FMF_MIN  50    //������С��������
#define FMF_MIN  25    //������С��������   //2015-1-8 ��С
#define FMF_ST   280	 //���λ�׼��������ΪĬ��ֵ
//#define FMF_MAX  360	 //�������������
#define FMF_MAX  560	 //�������������  //2015-1-8 �Ӵ�

//#define FMF_ST_U 280   //�����궨ʱ��ѡ������ߣ�2014-10-29
#define FMF_ST_U 300   //�����궨ʱ��ѡ������ߣ�2014-11-13
#define FMF_ST_L 240   //�����궨ʱ��ѡ������ߣ�2014-10-29

#define FRE_FOR_NOZZLECAB 30
//#define FRE_FOR_NOZZLECAB 30    //2014-11-14����60HZ�ȶ��Բ�����30HZ
#define FRE_FOR_NOZZLECAB_FRE 10  //��Ƶ�ʱ궨�ĵڶ���

#define FMF_COMPENSATE_FOR_T30  28

#define NB_COMPENSATE_FOR_T30 10  // FMF_COMPENSATE_FOR_T30*NB_COMPENSATE_FOR_T30=FMF_ST


extern word v_fmf_st_obj;//��׼����������Ŀ��ֵ
extern sword v_dfmf_st_obj;


//#define FRE_MAX  70		 //ִ������ʱ���������Ƶ��70Hz
//#define FRE_MAX  80		 //ִ������ʱ���������Ƶ��80Hz
//#define FRE_MAX  70		 //2014-11-16 �Ļص�70HZ
#define FRE_MAX  100		 //2015-1-31 �Ļص�90HZ
#define FRE_MIN  1		 //ִ������ʱ��������СƵ��1Hz

#define PRD_MAX  250000	 //FRE_MIN 1Hz ��Ӧ�� ����������Сֵ(�붨ʱ����Сֵ4us��Ӧ)
//#define PRD_MIN  3571	 //FRE_MAX 70Hz��Ӧ�� �����������ֵ
#define PRD_MIN  2500	 //FRE_MAX 100Hz��Ӧ�� �����������ֵ
//#define PRD_MIN  3125	 //FRE_MAX 80Hz��Ӧ�� �����������ֵ


#define T1_MIN    250
#define T1_MAX    2000


#define NB_MIN    1
#define NB_MAX    10000

#define QTOTAL_ST 1000

#define T_10MS_FOREXE   3000U

/*
    ����T30���ԵĲ���
*/
#define PER_NB_FOR_T30_100      100U
#define PER_NB_FOR_T30_10       10U
#define PER_NB_FOR_T30_1000    1000U
#define PER_NB_FOR_T30_500      500U

#define NO_FOR_T30    5


#define NUM_FOR_T30_INI       20    //��ʼ����T30�Ĵ�����//2014-11-13 from 10 to 20

/*=======================�ÿ��ƵĲ���======================================================*/
//2014-10-9 move here
//#define T_PURGING_200MS     150    //2014-11-17-2 at kailong ,dayue 33s
//#define T_PRIMING_200MS     150  

//2014-11-17-2 kailong2
#define T_PURGING_200MS     138  
#define T_PRIMING_200MS     138  


extern byte v_nb_compensateForT30;    //��Ϊ����T30����Ҫ����������Ĵ�����
extern long int v_Qtotal_mlPerHour ;      //����������������� 75-7500ml/H
extern word v_fmf_exe;      //ִ������ʱ�ĵ������� 11/250ul ÿ��������

extern dword v_Prd_exe;      //ִ������ʱ�����ڣ�ǰ������������ʼ��ʱ��֮��ļ����
extern dword v_Qtotal_infmfUnitPerSec;       //�����ÿ���������
extern word v_Prd_exe1;
extern dword v_Qtotal_mlPerHour_exe;  //ʵ��ִ�е�����������  ml/H������ʱ����������ں͵��������������ƻ�����������������

extern word v_Qtotal_mlPerHour_forDataview;//����DATAVIEW��ʾ�ĵ�λ
extern word v_Qtotal_mlPerHour_exe_forDataview;//����DATAVIEW��ʾ�ĵ�λ

extern word v_fmf_cal;      //����������ʱ�õ��м����
extern word v_fre_cal;		//����Ƶ�ʼ���ʱ�õ��м����
extern dword v_Prd_cal;     //�������ڼ���ʱ���м����

extern byte v_fre_exe;

extern void Cal_Prd_And_fmf(void);   

extern dword GetQ_ForDataViewUnit(dword Q);

extern word v_fmf_st_r;     //2014-10-29, �����궨֮���FMF_ST�����ⲿд��ECU;

extern sbyte v_kf_M256;//�õ�Ƶ�ʱ궨ϵ�� 2014-11-13

extern dword v_Qtotal_mlPerHour_byFreRec;//��Ƶ��������2014-11-13

/*-----------FOR VIRTUAL MASTER MODE-----------*/
extern sword v_dQtotal_mlPerHour ;      //vmasterģʽ�£��������ĵ���

/*-----------for mode_Q test ------------------*/
#define T1_INIFOR_QTEST   1000//M0_T1_T35;//600      //QTEST�û���T1
#define NB_INIFOR_QTEST   1000
#define FRE_INIFOR_QTEST  10

extern void Init_ForInjCalAfterKeyOn(void);

extern byte v_fre_QTest;
extern word v_nb_QTest;
extern word v_T1_QTest;

extern sword v_dT1_QTest; //v_T1_QTest=v_dT1_QTest+T1_INIFOR_QTEST;
extern sword v_dnb_QTest;
extern sbyte v_dfreQTest;

extern word v_nb_Pump_exe;//��Ҫִ�е�����Ĵ���

extern dword v_t_10ms_for_exe;   ///VMASTERģʽ�£�ִ�еĹ�����ʱ��
extern dword v_t_10ms_setfor_exe;   ///VMASTERģʽ�£���Ҫ���õĹ�����ʱ��

extern byte v_nb_StateCmd[7];//��ȡ�Ľ���״̬�Ĵ���
extern dword v_t_4us_exe;      //2014-11-16

/*---------------------------------------------------*/
extern word v_NOX_CAN;   //����CAN ��NOX�ź�
extern word v_O2_CAN;    //����CAN��O2�ź�,��λΪ1/1000

extern void Set_RunTime4us_by10ms(dword t10ms,byte bln10ms);

extern void Eclapse_Runtime_Exe(void);

extern void Acc_dT1(sword dt1,word nb_T1);

//2014-11-27
#define M0_CONST_MLPERHOUR   false
#define M0_CONST_T1   false
#define CONST_T1_4US    1150
#define CONST_MLPERHOUT 6000

//����ģʽ�Ķ���
//DI:ֱ������;AI��������������
#define K_DEFAULT           0



#define R_QTEST            3
#define K_QTEST            0

#define R_SALVE            4

#define K_SALVE_DI         5//2015-1-11 FROM 1 TO 5

#define R_VMASTER_DI       11

#define R_VMASTER_AI       9

#define K_DOSING            3

#define R_MASTER            13//2015-1-20


#define R_DEFAULT           R_MASTER

