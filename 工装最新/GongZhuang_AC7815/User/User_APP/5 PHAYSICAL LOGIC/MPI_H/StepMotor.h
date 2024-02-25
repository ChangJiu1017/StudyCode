#ifndef STEPMOTOR_H
#define STEPMOTOR_H
#include "public.h"
 

/*
 StepMotor_A_ON    OUT 1A
 StepMotor_AB_ON   OUT 1A 2A
 StepMotor_B_ON    OUT 2A
 StepMotor_BC_ON   OUT 2A 1B
 StepMotor_C_ON    OUT 1B
 StepMotor_CD_ON   OUT 1B 2B
 StepMotor_D_ON    OUT 2B
 StepMotor_DA_ON   OUT 2B 1A
*/
/*
#define StepMotorPINA PTED_PTED4 
#define StepMotorPIND PTED_PTED5 
#define StepMotorPINB PTED_PTED6 
#define StepMotorPINC PTGD_PTGD1 

#define StepMotorPhase2 PTGD_PTGD2
#define StepMotorPhase1 PTCD_PTCD5
#define StepMotorEN2 PTGD_PTGD1
#define StepMotorEN1 PTCD_PTCD3

#define StepMotor_A_ON  {StepMotorEN1=0;StepMotorEN2=1;StepMotorPhase1=1;StepMotorPhase2=0;}
#define StepMotor_AB_ON {StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=1;StepMotorPhase2=1;}
#define StepMotor_B_ON  {StepMotorEN1=1;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=1;}
#define StepMotor_BC_ON {StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=1;}
#define StepMotor_C_ON  {StepMotorEN1=0;StepMotorEN2=1;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_CD_ON {StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_D_ON  {StepMotorEN1=1;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_DA_ON {StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=1;StepMotorPhase2=0;}
#define StepMotor_OFF   {StepMotorEN1=1;StepMotorEN2=1;StepMotorPhase1=0;StepMotorPhase2=0;}
*/
//2014-1-27 peter,change to the following:
#define StepMotor_A_ON  0//{StepMotorEN1=0;StepMotorEN2=1;StepMotorPhase1=1;StepMotorPhase2=0;}
#define StepMotor_AB_ON 0//{StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=1;StepMotorPhase2=1;}
#define StepMotor_B_ON  0//{StepMotorEN1=1;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=1;}
#define StepMotor_BC_ON 0//{StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=1;}
#define StepMotor_C_ON  0//{StepMotorEN1=0;StepMotorEN2=1;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_CD_ON 0//{StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_D_ON  0//{StepMotorEN1=1;StepMotorEN2=0;StepMotorPhase1=0;StepMotorPhase2=0;}
#define StepMotor_DA_ON 0//{StepMotorEN1=0;StepMotorEN2=0;StepMotorPhase1=1;StepMotorPhase2=0;}
#define StepMotor_OFF  0// {StepMotorEN1=1;StepMotorEN2=1;StepMotorPhase1=0;StepMotorPhase2=0;}

#define StepMotorPINA 0//PTED_PTED4 
#define StepMotorPIND 0//PTED_PTED5 
#define StepMotorPINB 0//PTED_PTED6 
#define StepMotorPINC 0//PTGD_PTGD1 

//2016-5-26 FOR STEPMOTOR
#define StepMotorPhase2 M_MotorPh2//PTP_PTP3
#define StepMotorPhase1 M_MotorPh1//PTP_PTP2
#define StepMotorEN2    M_MotorEN2//PTP_PTP1
#define StepMotorEN1    M_MotorEN1//PTP_PTP0

#define M0_REV_TARGET_3D9 923  // 2500rpm-641 3000rpm-769 3500rpm-897  3600rpm-923 4000rpm-1025 
#define M0_REV_TARGET_3D9_HIGH 950
#define M0_REV_TARGET_3D9_STANDBY 512 //FROM 2000RPM

#define HIGHSPEED 1
#define LOWSPEED  0


/*
#define DC_ON StepMotorEN1=0;
#define DC_OFF StepMotorEN1=1;

#define DC_INC StepMotorPhase1=1;
#define DC_DEC StepMotorPhase1=0;
*/
//2014-1-27 peter, modify to the following
#define DC_ON 0;
#define DC_OFF 1;

#define DC_INC 1;
#define DC_DEC 0;


/*
//2012-11-4 临时测试
#define DC_ON StepMotorEN2=0;
#define DC_OFF StepMotorEN2=1;

#define DC_INC StepMotorPhase2=1;
#define DC_DEC StepMotorPhase2=0;
*/

#define NB_DC_PRD 7 //直流电机控制周期　为（NB_DC_PRD+1）*中断周期
//#define NB_DC_PRD 15 //直流电机控制周期　为（NB_DC_PRD+1）*中断周期

#define T_DC_PRD_4US 1000//=(NB_DC_PRD+1)*500us/4

#define NB_SM_PRD 3 //步进电机控制周期　为（NB_SM_PRD+1）*中断周期

#define STEP_SM_MAX 200
//#define STEP_SM_MIN 20	//2017-11-8
#define STEP_SM_MIN 20	//2020-11-27 改为20 12 2017-12-12 仅测试

#define STEP_SM_MIN_INCREMENT 25  //20210331 chenwei add 最小步数的额外增量，用于特殊情况下需要在STEP_SM_MIN基础上增加一定值

extern sbyte v_nb_DC;//DC ON的时间为_nb_DC/nb_RTI*周期
extern sbyte v_nb_DCPrd;//DC控制的周期数；

extern byte nb_RTI;
extern sbyte v_nbStep_DC;//根据PID得到的需要执行的角度；


extern byte StepMotor_Phase_L;

extern sword v_nbStep_ETC,Act_Step;

/*Subroutine*/
extern void StepMotorControl(void); 
extern void FI_ECTafterReset() ;

extern sbyte F_dign(word rev_15d6,sbyte ign);
extern void FS_ECT_IdlePosition(byte tps,byte tps_odr,byte tes);
extern void F_step_ECT(word rev_3d9, byte tps);

extern void Save_I_Load_12(word iLoad,word iLoad2);
extern void Save_I_Load_34(word iLoad,word iLoad2);

extern void FD_LUB();


/*2012-10-30 ADD ETC*/						;	
extern byte v_I_ld 	;
extern byte v_I2_ld;
extern byte v_I_ld_lst;

extern byte v_I2_ld_lst2,v_I2_ld_lst3,v_I2_ld_lst4;

extern byte v_I_ini[2];

extern byte v_EngineOil;

extern volatile sbyte v_d_ign_fb;
extern volatile sbyte v_d_ign_trans; 

extern  sbyte v_dTPS_tg_a   ;
extern  sbyte v_dTPS_tg_n  ;
extern  sbyte v_acc_rev    ;
extern  sword v_d_rev_to_tg ; 
extern  byte v_ccl_RPMTemp;
extern  byte VF_STA_LD ;
extern  byte v_ccl_StartsProcess;

extern byte v_TPS_tg;//2012-11-14 add TPS的目标值
extern byte v_TPS_I_lst;//2012-11-18 有负载电流估算得到的TPS值；

extern  byte v_ccl_I;

extern sbyte v_d_ign_fb_ave;

extern word v_rev3d9_tg;

extern byte v_TPS_st;//启动时油门位置

extern sbyte v_dnb_byIgn; //点火角对电机的运动的修正

extern byte v_KP_M8;
extern byte v_KI_M8;
extern byte v_KD_M8;
extern sbyte drev_tg_i;// 转速与目标差异的积分；

extern byte v_I_stable_lst;//上一次稳定电流的平均值；

extern void F_CAL_TPS_TG_BY_I(word rev_3d9,byte tps);

extern void FS_DCMotor(void);

extern word v_nfull;
extern word v_nstand;

extern sword v_d_ETC_p;
extern sword v_d_ETC_i;
extern sword v_d_ETC_d;
extern sword v_d_ETC_all;
extern sword v_d_AVrpm_to_tg    ;//2013-1-20平均转速与目标的差值


extern void F_I_ld(byte blnReset); //2015-11-14
extern byte v_I_max[2];
extern byte v_I_max_lst[2];
extern byte v_I_min_lst[2];
extern byte v_I_ldx[2];


extern    byte v_nb_IncLd;
extern     byte v_nb_DecLd;

extern sword v_dI_ld[2];//ld的变化反映在di上的变化
extern byte v_nb_ldAd_1cycle;//1圈里面的ld采集的数量

extern void FS_SMotor_IACV(void);//2016-9-7
extern byte SM_Position;

extern sword v_step_sm;//step 的位置
extern void Fset_SM(byte position);

extern sword v_step_sm;//step 的位置
extern sword v_StepAct;

extern sbyte v_t_IACV_on;//2016-9-10 IACV开启的时间，
extern byte v_step_IACV_on;//2016-9-10 IACV开启的步数，
extern sword v_steptmp;
extern sword v_steptmp_p;
extern sword v_steptmp_i;

extern byte Ldinc_byIACV(void);

extern byte v_ki_iacv;
extern byte v_kp_iacv;

extern sword v_dn_iacv;
extern sword v_dn_iacv_lst;
extern sword v_dn_iacv_lst2;
extern sword v_dn_iacv_lst3;
extern sword v_dnave_iacv;
extern sword v_dnave_iacv_lst;
extern long int v_dnave_sigma_i;
extern sword v_dnave_sigma_p;
extern byte v_t_corstp_s;
extern byte v_ntcoradj;

//extern byte test_171106;
extern byte t_stepcon_delay;
extern byte v_stepbase;


#endif
 
 
 
 
 