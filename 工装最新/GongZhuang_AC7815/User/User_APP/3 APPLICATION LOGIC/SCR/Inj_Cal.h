#include "hard_trig.h"	


#define Q_MLPERHOUR_FORPRIMING  7000

//#define Q_MLPERHOUR_FORINJ_MIN  75     //泵的最小喷射量，小于这个数就不喷射了
#define Q_MLPERHOUR_FORINJ_MIN  30     //泵的最小喷射量，小于这个数就不喷射了,2015-1-7根据杨老师要求

//#define FMF_MIN  50    //单次最小喷射油量
#define FMF_MIN  25    //单次最小喷射油量   //2015-1-8 减小
#define FMF_ST   280	 //单次基准喷射量，为默认值
//#define FMF_MAX  360	 //单次最大喷射量
#define FMF_MAX  560	 //单次最大喷射量  //2015-1-8 加大

//#define FMF_ST_U 280   //流量标定时，选择的上线，2014-10-29
#define FMF_ST_U 300   //流量标定时，选择的上线，2014-11-13
#define FMF_ST_L 240   //流量标定时，选择的下线，2014-10-29

#define FRE_FOR_NOZZLECAB 30
//#define FRE_FOR_NOZZLECAB 30    //2014-11-14发现60HZ稳定性差，因此用30HZ
#define FRE_FOR_NOZZLECAB_FRE 10  //泵频率标定的第二点

#define FMF_COMPENSATE_FOR_T30  28

#define NB_COMPENSATE_FOR_T30 10  // FMF_COMPENSATE_FOR_T30*NB_COMPENSATE_FOR_T30=FMF_ST


extern word v_fmf_st_obj;//基准喷射油量的目标值
extern sword v_dfmf_st_obj;


//#define FRE_MAX  70		 //执行喷射时的喷射最大频率70Hz
//#define FRE_MAX  80		 //执行喷射时的喷射最大频率80Hz
//#define FRE_MAX  70		 //2014-11-16 改回到70HZ
#define FRE_MAX  100		 //2015-1-31 改回到90HZ
#define FRE_MIN  1		 //执行喷射时的喷射最小频率1Hz

#define PRD_MAX  250000	 //FRE_MIN 1Hz 对应的 喷射周期最小值(与定时器最小值4us对应)
//#define PRD_MIN  3571	 //FRE_MAX 70Hz对应的 喷射周期最大值
#define PRD_MIN  2500	 //FRE_MAX 100Hz对应的 喷射周期最大值
//#define PRD_MIN  3125	 //FRE_MAX 80Hz对应的 喷射周期最大值


#define T1_MIN    250
#define T1_MAX    2000


#define NB_MIN    1
#define NB_MAX    10000

#define QTOTAL_ST 1000

#define T_10MS_FOREXE   3000U

/*
    关于T30测试的策略
*/
#define PER_NB_FOR_T30_100      100U
#define PER_NB_FOR_T30_10       10U
#define PER_NB_FOR_T30_1000    1000U
#define PER_NB_FOR_T30_500      500U

#define NO_FOR_T30    5


#define NUM_FOR_T30_INI       20    //初始进行T30的次数；//2014-11-13 from 10 to 20

/*=======================泵控制的参数======================================================*/
//2014-10-9 move here
//#define T_PURGING_200MS     150    //2014-11-17-2 at kailong ,dayue 33s
//#define T_PRIMING_200MS     150  

//2014-11-17-2 kailong2
#define T_PURGING_200MS     138  
#define T_PRIMING_200MS     138  


extern byte v_nb_compensateForT30;    //因为做了T30，需要补偿的喷射的次数；
extern long int v_Qtotal_mlPerHour ;      //需求的尿素喷射流量 75-7500ml/H
extern word v_fmf_exe;      //执行喷射时的单次油量 11/250ul 每单次喷射

extern dword v_Prd_exe;      //执行喷射时的周期；前后两次喷射起始的时间之间的间隔；
extern dword v_Qtotal_infmfUnitPerSec;       //这个是每秒的喷射量
extern word v_Prd_exe1;
extern dword v_Qtotal_mlPerHour_exe;  //实际执行的尿素喷射量  ml/H；根据时间的喷射周期和单次喷射量，反推回来的尿素喷射量；

extern word v_Qtotal_mlPerHour_forDataview;//用于DATAVIEW显示的单位
extern word v_Qtotal_mlPerHour_exe_forDataview;//用于DATAVIEW显示的单位

extern word v_fmf_cal;      //喷射量计算时用的中间变量
extern word v_fre_cal;		//喷射频率计算时用的中间变量
extern dword v_Prd_cal;     //喷射周期计算时用中间变量

extern byte v_fre_exe;

extern void Cal_Prd_And_fmf(void);   

extern dword GetQ_ForDataViewUnit(dword Q);

extern word v_fmf_st_r;     //2014-10-29, 经过标定之后的FMF_ST，由外部写入ECU;

extern sbyte v_kf_M256;//泵的频率标定系数 2014-11-13

extern dword v_Qtotal_mlPerHour_byFreRec;//泵频率修正后，2014-11-13

/*-----------FOR VIRTUAL MASTER MODE-----------*/
extern sword v_dQtotal_mlPerHour ;      //vmaster模式下，对流量的调整

/*-----------for mode_Q test ------------------*/
#define T1_INIFOR_QTEST   1000//M0_T1_T35;//600      //QTEST用基础T1
#define NB_INIFOR_QTEST   1000
#define FRE_INIFOR_QTEST  10

extern void Init_ForInjCalAfterKeyOn(void);

extern byte v_fre_QTest;
extern word v_nb_QTest;
extern word v_T1_QTest;

extern sword v_dT1_QTest; //v_T1_QTest=v_dT1_QTest+T1_INIFOR_QTEST;
extern sword v_dnb_QTest;
extern sbyte v_dfreQTest;

extern word v_nb_Pump_exe;//需要执行的喷射的次数

extern dword v_t_10ms_for_exe;   ///VMASTER模式下，执行的工作的时间
extern dword v_t_10ms_setfor_exe;   ///VMASTER模式下，需要设置的工作的时间

extern byte v_nb_StateCmd[7];//获取的进入状态的次数
extern dword v_t_4us_exe;      //2014-11-16

/*---------------------------------------------------*/
extern word v_NOX_CAN;   //来自CAN 的NOX信号
extern word v_O2_CAN;    //来自CAN的O2信号,单位为1/1000

extern void Set_RunTime4us_by10ms(dword t10ms,byte bln10ms);

extern void Eclapse_Runtime_Exe(void);

extern void Acc_dT1(sword dt1,word nb_T1);

//2014-11-27
#define M0_CONST_MLPERHOUR   false
#define M0_CONST_T1   false
#define CONST_T1_4US    1150
#define CONST_MLPERHOUT 6000

//工作模式的定义
//DI:直接喷射;AI：空气辅助喷射
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

