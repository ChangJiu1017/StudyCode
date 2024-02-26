/*HEAD
 ***********************************************************************
 * Filename	:stepmotor.c
 * Function	:motor control.		
 * User		:xielian
 * Date		:2017.6.26
 * Version	:V1.2.1
 *
 *** History ***
 * V8.1.0	2016.11.28	xielian 
 *		Base function.
 *
 * V8.2.0	2016.11.28	xielian 
 *		Update the IACV control logic. 
 * V8.2.1	2017.6.26	xielian 
 *		步进电机频率按照隆鑫要求调整为200HZ
 * V8.2.3	2018.3.23	xielian
 *		策略优化
 * V8.2.4 2018.5.19 yangsili
 *    去掉初始化条件
 ***********************************************************************
HEAD END*/
#include "FAM.h"
#include "StepMotor.h"
#include "ee.h"
#include "Status_st.h"


//#include "math.h"

#include "ad.h"
#include "add.h"

#include "time_ccl.h"
#include "ign.h"

#include "sci.h"

#include "public.h"

#include "look_up.h"

#include "ld_altitude.h"

//#if ECU_TYPE==ECU_GEN

//volatile V_STATUS_STR _V_IACV_STATUS;//none
byte StepMotor_Phase_L;

sword v_nbStep_ETC,Act_Step;
sbyte v_nbStep_DC;//根据PID得到的需要执行的角度；

sbyte v_dnb_byIgn; //点火角对电机的运动的修正

word rev_3d9_lst;
word rev_3d9_lst2;
word rev_3d9_lst3;

/*2011-11-8 peter*/
volatile sbyte v_d_ign_fb;
sbyte v_d_ign_fb_ave;
byte nb_n_limit;
byte nb_p_limit;

sword v_d_rev_to_tg ;
sword v_d_rev_to_tg_lst ;
sword v_d_rev_to_tg_lst2 ;//lcw2013-1-12
sword v_d_rev_to_tg_lst3 ;//lcw2013-1-12
sword v_d_rev_to_tg_lst4 ;//lcw2013-1-12
//sword v_d_rev_to_tg_lst5 ;//lcw2013-2-1    2013-2-4 为节省RAM，删除 

sword v_d_AVrpm_to_tg    ;//2013-1-20平均转速与目标的差值
sword v_d_AVrpm_to_tg_lst    ;//2013-1-20上平均转速与目标的差值
sword v_d_AVrpm_to_tg_lst2    ;//2013-1-20平均转速与目标的差值
sword v_d_AVrpm_to_tg_lst3   ;//2013-1-20上平均转速与目标的差值


sbyte v_acc_rev;//加速度//2012-11-15
sbyte v_acc_rev_lst;//上一次加速度//2012-11-17
 
byte VF_STA_LD ;

byte v_ccl_StartsProcess;

byte v_step_formin; //将油门开到最小，累积的部步数

//2011-11-19 i detection
byte v_I_ld;
byte v_I_ld_lst;
byte v_I_ld_lst2,v_I_ld_lst3,v_I_ld_lst4;

byte v_I2_ld,v_I2_ld_lst;
byte v_I2_ld_lst2,v_I2_ld_lst3,v_I2_ld_lst4;


byte v_I_ini[2];
byte v_I_ini_lst[2];          //2012-11-15 上一次采样初始值；
byte v_I_ini_lst2[2];          //2013-1-13 头两次采样初始值



byte v_I_stable_lst;//上一次稳定电流的平均值；
byte v_I_stable;    //稳定电流值 
byte v_I_lst;//上一次电流值；
byte v_I;    //电流值 



byte v_TPS_I;//2012-11-18 有负载电流估算得到的TPS值；
byte v_TPS_I_lst;//2012-11-18 有负载电流估算得到的TPS值；

byte v_TPS_I_stable;//2013-1-27平均电流估算得到的TPS值；
byte v_TPS_I_stable_lst;//2013-1-27上次平均电流估算得到的TPS值；

byte v_ccl_Istable; //电流最后稳定到现在的圈数；
//byte ccl_disableN;// 不使能转速控制的圈数；

byte  st_I_Not_cap   ;//LCW电流1没有捕获的状态
byte  st_I2_Not_cap   ;//LCW电流2没有捕获的状态
byte  st_tps_tg_to_max  ;//lcw2013-1-15目标TPS达到最大值的状态
byte  st_tps_tg_to_min  ;//lcw2013-1-15目标TPS达到最小值的状态 
byte  st_DC_ACC_8       ;//DC全加速的状态
byte  st_DC_DEC_8       ;//DC全减速的状态             



byte v_EngineOil;

word v_rev3d9_tg;

byte v_TPS_st;//启动时油门位置

byte  n_nb_I ;
      
byte v_ld0[2];//2012-10-12, 电流为0时的采样值，停机时

sbyte drev_tg_i;// 转速与目标差异的积分；


//2013-1-12 lcw
sword drev_tg_rp;     //转速偏差比例控制的累计量;
//sword drev_tg_ri;     //转速偏差积分控制的累计量;
long int  drev_tg_ri;     //转速偏差积分控制的累计量;
//sword drev_tg_rd;     //转速偏差微分控制的累计量;
long int drev_tg_rd;     //转速偏差微分控制的累计量;


sbyte v_nb_DC;      //DC ON的时间为_nb_DC/nb_RTI*周期  //2012-11-14 from byte to sbyte
sbyte v_nb_DC;      //DC ON的时间为_nb_DC/nb_RTI*周期  //2012-11-14 from byte to sbyte

sbyte v_nb_DCPrd;   //DC控制的周期数

sbyte v_nb_step_pre;    //2013-1-13 LCW 由于电流加减速判断为真，预先启动可能移动的步数
sbyte v_fmf_ETC;      //2013-1-28

//2013-1-9
byte v_stp_inc;
byte v_stp_dec;
byte v_stp_cal;

byte v_tps_tg_2;     //lcw
byte v_nbStep_ETC_1;  //lc
byte v_nbStep_ETC_2;   //lcw
sbyte  temp_d_rpmPID;

sbyte temp_d_rpmI;
sbyte  temp_dc_b_lst;
sbyte temp_dc_b;   //lcw
byte temp_Step;   //lcw
byte  temp_TPS_I;
byte  temp_TPS_I_lst;
byte temp_t3_16us;
byte temp_t3_16us_lst;
byte temp_t1_32us;
byte temp_t1_32us_lst;

byte temp_I_cac_Pos;
byte temp_I_cac_Pos_lst;
byte  temp_Pos;
byte  temp_Pos_Pinj;
byte  temp_Pos_Pinj_lst;

byte temp_st_I2_not_cap;
byte  temp_st_I1_not_cap;
byte temp_nb_I2_cap;
byte  temp_nb_I1_cap;
byte  temp_I1_init_cap;
byte  temp_I1_init_lst_cap;
byte  temp_I2_init_cap;
byte  temp_I2_init_lst_cap;
byte  temp_I1_ld;
byte  temp_I1_ld_lst;
byte  temp_I2_ld;
byte  temp_I2_ld_lst;


byte v_nb_dcprd_t;  //lc
byte v_nb_dcStep_t;   //lcw

//2013-6-18
byte v_TPS_last_cy;  //lcw2013-1-11

word v_nfull;
word v_nstand;

sword v_d_ETC_p;
sword v_d_ETC_i;
sword v_d_ETC_d;
sword v_d_ETC_all;

word t_lst_Drive;
byte t_TPS_lst;

byte v_nb_IncLd;
byte v_nb_DecLd;

//----------IACV------------------
byte v_ki_iacv;
byte v_kp_iacv;
sword v_dn_iacv;
sword v_dn_iacv_lst;
sword v_dn_iacv_lst2;
sword v_dn_iacv_lst3;
sword v_dnave_iacv;
sword v_dnave_iacv_lst;
long int v_dnave_sigma_i;
sword v_dnave_sigma_p;
byte v_t_corstp_s;
byte v_ntcoradj;
byte v_stepbase;
byte VF_STEP_TRANS;
byte VF_STEP_STEADY;
sword v_stepini;
byte v_step_bidle;

byte VF_IGN_SUIT;

byte StepMotorStep;

byte v_IACV_ini_st;

char *step_pair;
word v_stepco_pt;





/*---------------------Local vairables--------------------------------*/
//步进电机状态
#define VF_TOTPSE0_STALD  0
#define VF_TOTPSMIN_STALD 1
#define VF_SETINIT_STALD 2
#define VF_TPS_READY    3
#define VF_STEA_STALD   4
#define VF_TRAN_STALD   5

// 用于稳定状态的判断
#define M0_REV_TH_STEADY 25//22//15//22     //3.9rpm
#define M0_ACC_TH_STEADY 14//10//6//8    //10

#define M0_CCL_LAG  4//控制需要延迟的循环数
#define M0_CCL_LAG_FOR_SM  16
#define MO_dTPS_TRAN 15

#define M0_DIGN_TH_ECT_P 20     //10
#define M0_DIGN_TH_ECT_N -30     //10

#define M0_DIGN_TH_ECT_MAX_P 8 //20
#define M0_DIGN_TH_ECT_MAX_N -8     //10

#define M0_THRE_LD_OPEN_RUN 250   //发动机运行时，电流采样值大于该值，表示传感器开路
#define M0_THRE_LD_OPEN_STOP 180   //发动机停机时，电流采样值大于该值，表示传感器开路

#define M0_THRE_LD_ZERO 130   //默认的电流为0的值； //2013-1-25 120 to 130
/*-map to be open-------------------------------------------------------------------*/
//#define M0_TPS_ST 55     //启动时基本油门位置 //2012-11-15 from 45 to 55
//BS--55, GENERAC--45

//每圈可以动作的最大的步数
#define M0_STEPS_MAX_DC 16
#define M0_STEPS_MAX_DC_trip 150   //2013-1-19


#define M0_STEPS_MAX_SM 13
#define STEP_LEAVEIDLE	12
#define STEP_BACKIDLE	30


// 使用v_KP_M8等时的参数
#define M0_DRPM_MAX_pDC 60       //比例控制时一循环内的转速偏差变化最大值限制在40*3.9
//#define M0_DAVRPM_MAX_pDC 120    //2013-1-16比例控制时,3循环内的转速偏差变化最大值限制在100*3.9

#define M0_DRPM_MAX_iDC 30  //2013-1-22 80        //积分控制时的转速最大偏差限制在96*3.9:374rpm
//#define M0_DRPM_MAX_dDC 50       //微分控制时的转速偏差变化的差值最大偏限制在30*3.9
//2015-12-27 CHANGE TO 100
#define M0_DRPM_MAX_dDC 100       //微分控制时的转速偏差变化的差值最大偏限制在30*3.9

//#define M0_DREV_TG_KP_ASTEP 256 //2013-1-30使用v_KP_M8后，v_KP_M8*drpm/256,256核算一步
#define M0_DREV_TG_KP_ASTEP 256 //2015-12-27 更改为128
    
      
#define M0_DREV_TG_KI_ASTEP  3200 //2013-1-30 *v_KI_M8/3200
//#define M0_DREV_TG_KI_ASTEP  1024 //2013-1-30 *v_KI_M8/3200
       //积分控制时的转速偏差累计到M0_DREV_TG_kI_ASTEP=50，折合到电机一步
//#define M0_DREV_TG_KD_ASTEP 32 //2013-1-30 微分常数=v_KD_M8/32
#define M0_DREV_TG_KD_ASTEP 16 //2013-1-30 微分常数=v_KD_M8/32 2015-12-27
       //转速微分控制时，转速偏差变化量的差值累计到M0_DREV_TG_KD_ASTEP=32，折合到电机一步



#define M_X 1

#define  M0_RPM_H0 35/M_X  //25           //高转速97.5，ETC减M0_ETC_DEC_to_H0=5
#define  M0_ETC_DEC_to_H0 3 //5;
#define  M0_RPM_H1 45/M_X//35           //高转速97.5，ETC减M0_ETC_DEC_to_H0=10
#define  M0_ETC_DEC_to_H1 5 //10;      
#define  M0_RPM_H2 55/M_X //45           //高转速136，ETC减M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_H2 10;
#define  M0_RPM_H3 65/M_X //55           //高转速214，ETC减M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_H3 15 //20;
#define  M0_RPM_H4 75/M_X //65           //高转速253，ETC减M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_H4 15 //30; 
#define  M0_RPM_H5 85/M_X //75           //高转速292，ETC减M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_H5 20 //31;
#define  M0_RPM_H6 95/M_X //80           //高转速312，ETC减M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_H6 25 //31;
#define  M0_RPM_H7 130/M_X           //高转速507，ETC减M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_H7 31 

#define  M0_RPM_L0 -30/M_X           //高转速97.5，ETC减M0_ETC_DEC_to_H0=5
#define  M0_ETC_DEC_to_L0 3
#define  M0_RPM_L1 -40/M_X           //高转速97.5，ETC减M0_ETC_DEC_to_H0=10
#define  M0_ETC_DEC_to_L1 5 //7;      
#define  M0_RPM_L2 -50/M_X           //高转速136，ETC减M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_L2 9 //10;
#define  M0_RPM_L3 -60/M_X           //高转速214，ETC减M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_L3 10 //15;
#define  M0_RPM_L4 -70/M_X           //高转速253，ETC减M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_L4 15 //20; 
#define  M0_RPM_L5 -75/M_X           //高转速292，ETC减M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_L5 15 //31;
#define  M0_RPM_L6 -80/M_X           //高转速312，ETC减M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_L6 20 //31;
#define  M0_RPM_L7 -130/M_X           //高转速507，ETC减M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_L7 31 

/*-------For IACV---------*/
#define  M0_DNSTEP_KP 512//1024
#define  M0_DNSTEP_KI 8192//16384
#define  M0_STOFCORADJ 6//5 2020-3-24 粗调节需要走三步停一步，设置为3的倍数
#define  M0_TCORSTP 5	//轻微高怠速执行粗调节的时间门限	p1
#define  M0_DNCORSTP 20	//轻微高怠速执行粗调节的转速门限	p15d6
 

//负载电流与油门的关系不包括0点
const byte M2_TPS_BYLD[7][2]={				
	1	,	0	,
	24	,	14	,
	46	,	30	,
	68	,	43	,
	90	,	62	,
	114	,	89	,
	138	,	130	,
};

//pid control paratmeters for ETC
byte v_KP_M8;
byte v_KI_M8;
byte v_KD_M8; //KD未被使用，复用为初始TPS位置
//2013-2-1 
#define M0_TPS_ST 50 //2013-1-28,BYPTR_M0_KD_M8  //2013-2-2 liulaoshi zoule
//#define M0_TPS_ST 60 //2013-1-28,BYPTR_M0_KD_M8

word v_tmr_DC;

byte levelO;

//发动机转速的状态
byte st_rev_DEC;
byte st_rev_ACC;

//加速度的变化状态；
byte st_a_DEC;
byte st_a_ACC;

//2012-11-4 add
byte v_TPS_lst_DC;
byte v_TPS_lst2_DC;
byte v_dTPS_DC;            //2013-1-13 lcw TPS与TPS_tg之差
byte v_dTPS_lst_DC;
byte v_dTPS_lst2_DC;


byte v_TPS_tg;//2012-11-14 add TPS的目标值
byte v_TPS_tg_lst;      //lcw 2013-1-13



void StepMotor_Init(void);

byte t_forMinTPS;//2012-12-13

byte nb_er_lds0; //电流传感器故障的次数；
byte nb_er_lds1; //电流传感器故障的次数；

//电流信号采集
#define IDX_LD_MX 19
//byte I_ad[2][2][IDX_LD_MX+1]; //DIM0--CHANNEL NO  ,DIM1--IDX  ,DIM2--  ,
word tick_lst_ad_ld;//上一次负荷ad的时刻
byte idx_ld;
byte v_I_max[2];
byte v_I_max_lst[2];

byte v_I_min[2];
byte v_I_min_lst[2];

byte v_I_ldx_lst2[2];
byte v_I_ldx_lst[2];
byte v_I_ldx[2];

sword v_dI_ld[2];//ld的变化反映在di上的变化

byte point_ld;

byte v_nb_ldAd_1cycle;//1圈里面的ld采集的数量

void SetAsTpstg(sword sw) ;

byte SM_Position;
void FC_SM(byte blnIni);
//#define STEP_SM_MAX 125
//#define STEP_SM_MAX 160//2016-9-13
//#define STEP_SM_MAX 180
//#define STEP_SM_MAX 200
//#define STEP_SM_MIN 10
//#define STEP_SM_MIN 20	//2017-11-8

sword v_step_sm;//step 的位置
sword v_StepAct;
sword v_step_sm_tg;//step 的目标位置
sword v_StepAct_lst;//2017-11-11

sbyte v_t_IACV_on;//2016-9-10 IACV补偿油量，
byte v_step_IACV_on;//2016-9-10 IACV开启的步数，
byte v_step_idle;
sword v_steptmp;
byte v_step_alt;
sword v_steptmp_i;
sword v_steptmp_p;
byte v_stepdec_fact;

byte v_step_bcidlimt;

void IACV_con(void);
void F_CORADJ_IACV(void);
byte Ldinc_byIACV(void);
void F_StepMT_3T1(void);

byte Step_inc(void);
//byte test_180322;
byte t_stepcon_delay;
byte v_step_ignco;
sbyte v_ign_stptg;
byte v_t_ignlim;


//2016-1-4 add 
        byte v_nb_ld_total_err;//2016-1-4
        byte v_nb_ld_err[2];//2016-1-4
        
//20210929 chenwei add
byte VF_OPENLOOP_IACV;		//进去IACV开环控制的标志

//20211010 chenwei add 环松氧加热工作策略
byte v_t_O2H_delay_1s;			//使用氧加热的延迟时间，距离车辆运行
byte v_o2h_n_625;					//氧加热开始工作的转速限值
byte v_o2h_tpd;					//氧加热开始工作的开度限值
byte VF_O2H_STATUS;		//指示氧加热的状态

void F_O2H_FORHUANSONG(void);

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
//////////////////////Initialize/////////////////////////
void StepMotor_Init(void)
{

    //变量参数初始化
    StepMotor_OFF;
    StepMotor_Phase_L=0;    

    v_ccl_StartsProcess=0;
	VF_CORACT=false;
	VF_CORADJ=false;
	VF_CORADJ_LST=false;
	VF_CFST=false;
	VF_CIRECINJST=false;
	v_ntcoradj=0;
	v_t_corstp_s=0;
	v_step_idle=STEP_SM_MIN;
	VF_STEP_STEADY=true;
	
	v_ign_stptg=BYPTR_M0_CAS_TRAILEDGE-5;
	if(v_ign_stptg<0)v_ign_stptg=0;
	
	v_t_ignlim=0;
	VF_STEP_IGNCO=false;
	v_step_ignco=0;
	VF_CLSMODE_IACV=false;
	
	StepMotorStep=0;
  
}
                                        
/*
    2011-11-9 PETER FOR INT after reset (Hardreset or Softreset)
*/
void FI_ECTafterReset()
{
    v_d_ign_fb_ave=0;
    nb_p_limit=0;
    nb_n_limit=0;
    
    VF_STA_LD= VF_TOTPSE0_STALD;

    v_step_formin=0;
    VF_FAILURE_ETC=false; //2013-1-29 lcw

    v_TPS_st=M0_TPS_ST;
    v_TPS_tg=v_TPS_st-2;    //2013-1-26lcw    
    
    StepMotor_Init();
	
	if(VF_POR_HARD)
	{
        #ifdef SYS_STEPMOTOR
		v_step_sm_tg=10;
		v_StepAct=0;	
		v_step_sm=0;
		
		v_IACV_ini_st = Load_Data_EE(0x7f);
		if(v_IACV_ini_st!=100)
		{			
			if(!VF_1ST_INI_SYSINI)	//2018-3-22 初始化过程发生的reset不需要电机学零
			{
				Fset_SM(255);	//电机学零位 2018-3-22 熄火开关或者异常熄火的情况也进行学零是否合理？
			}				
		}		
		
		#endif
    }

	
  
    if(VF_VPOR_HARD)
    {
        v_ld0[0]=M0_THRE_LD_ZERO;
        v_ld0[1]=M0_THRE_LD_ZERO;
        
        V_LDSENSOR_STATUS=0;
        V_ETC_STATUS=0;
        
        v_KP_M8=BYPTR_M0_KP_M8;
        v_KI_M8=BYPTR_M0_KI_M8;
        v_KD_M8=BYPTR_M0_KD_M8;
        
        v_rev3d9_tg=(word)BYPTR_M0_NFULL_62D5RPM*16 ; //2013-6-19
        
        nb_er_lds1=0;
        nb_er_lds0=0;
        
        v_d_ETC_d=v_d_ETC_p=v_d_ETC_i=0;

        
        drev_tg_rd=0;

        
        //Fset_SM(255);// FOR RESET
		#ifdef SYS_STEPMOTOR
		if(v_IACV_ini_st==100)
		{
			v_step_sm_tg=10;//2020-12-18 如果开钥匙前学零完成，就赋初值，否则保持原值不变
			v_StepAct=0;
			v_step_sm=0;
		}
		
		v_IACV_ini_st=102;
		Save_Data_EE(0x7f,102);
		
		/*if(v_IACV_ini_st!=100)
		{			
			if(!VF_1ST_INI_SYSINI)	//2018-3-22 初始化过程发生的reset不需要电机学零
			{
				Fset_SM(255);	//电机学零位 2018-3-22 熄火开关或者异常熄火的情况也进行学零是否合理？
			}				
		}*/
		#endif
		
        VF_LEARZRO_KEYOFF=false;

		
    }
	
	
	/*#ifdef SYS_STEPMOTOR
	if(VF_JSTOP_ENG)
	{		
		if(!VF_1ST_INI_SYSINI)	//2018-3-22 初始化过程发生的reset不需要电机学零
		{
			Fset_SM(255);	//电机学零位 2018-3-22 熄火开关或者异常熄火的情况也进行学零是否合理？
		}			
	}
	#endif*/
	
    
    /*if(VF_POR_HARD){
        
    }*///20210227 chenwei 此处暂时没用到此处屏蔽掉
    
    Act_Step=0;//2012-11-6 ADD
    
    v_nbStep_ETC=0;//2012-11-19 ADD
    
    t_forMinTPS=2;
    
    v_stp_cal=v_stp_inc=v_stp_dec=0;
    v_nbStep_DC=0;//2013-1-9
    
    
    v_dnb_byIgn=0;
    
    VF_BYTPD_ETC=false;
    
    v_nb_IncLd=0;
    v_nb_DecLd=0;
    v_nb_ldAd_1cycle=0;
    
    v_nb_ld_err[0]=0;//2016-1-4
    v_nb_ld_err[1]=0;//2016-1-4
    v_nb_ld_total_err=0;//2016-1-4
	
	VF_OPENLOOP_IACV=false;		//20210929 chenwei add
}

void DC_con(sbyte clk,byte pos) 
{
/*    
		byte b;
    
    if(clk==0)
    {
         DC_MOTOR_OFF
         setReg8(PWMDTY2, 127);            
    } else if(clk<0)//2015-10-22 changeto <0 
    {
        b=pos;
        //if(b<10)b=10;
        //if(b<40)b=40;
        if(b<20)b=20;//2015-12-16
        
        if(b>127)b=127;
        b+=128;

        setReg8(PWMDTY2, b);            
         DC_MOTOR_ON
    } 
    else
    {
        b=pos;
        if(b>127)b=127;
        b=128-b;

        //if(b<10)b=10;
        //if(b<40)b=40;
        if(b<20)b=20;//2015-12-16

        setReg8(PWMDTY2, b);            
         DC_MOTOR_ON
        
    }
*/    
}
/*
    电流信号采集
    blnReset:在新的基准时间到来时，重新开始一组数据，这里选择角标前沿作为基准时间
    

*/

void F_I_ld(byte blnReset)
{
    byte i;
    byte j;
    
    byte idx_lst;
    byte b1;
    
    byte bad;
    
    sword sw; 
    
    if((word)(T_MAINTMR-tick_lst_ad_ld)>250||blnReset)
    {   //1MS 每次
    
        for(j=0;j<=1;j++)
        {
            if(blnReset)
            {
                //新时间基准到了之后，备份ad数据
                idx_ld++;
                if(idx_ld>1)idx_ld=0;
                
                v_I_ldx_lst2[j]=v_I_ldx_lst[j];
                v_I_ldx_lst[j]=v_I_ldx[j];
                
                //处理上一组数据
                /*
                
                b1=0;
                for(i=0;i<=IDX_LD_MX;i++)
                {
                    if(I_ad[j][idx_lst][i]>b1)b1=I_ad[j][idx_lst][i];
                    //I_ad[j][idx_lst][i]=0;
                    
                } */
                v_I_max_lst[j]=v_I_max[j];
                v_I_max[j]=0;// b1;
                
                v_I_min_lst[j]=v_I_min[j];
                v_I_min[j]=255;// b1;
                
                
                sw=(sword)v_I_max_lst[j]-128;
                if(sw<0)sw=0;
                
                
                v_I_ldx[j]=(byte)sw;
                
                
                sw=128-(sword)v_I_min_lst[j];
                if(sw<0)sw=0;
                //sw=-sw;
                
                bad=(sword)sw;
                
                if(v_I_ldx[j]<bad)v_I_ldx[j]=bad;
                
                
                
                
                //==================
                
                
            }
            
            
            {
              
                if(j==0)b1= CH_AD_CURRENT1;
                else b1= CH_AD_CURRENT2;
                
                //I_ad[j][idx_ld][point_ld]
                bad=FAD_1Channel(b1,true,2);
                
                if(bad>v_I_max[j])v_I_max[j]=bad;
                if(bad<v_I_min[j])v_I_min[j]=bad;
                
                //2015-11-20 纠正错误
                if(j==1){
                    tick_lst_ad_ld=T_MAINTMR;
                }
                
                //if(point_ld>IDX_LD_MX)point_ld=IDX_LD_MX;
            }
            
        }
        
        point_ld++;
        

    }
    
    
    if(blnReset)
    {
        //计算负荷变化
        v_nb_ldAd_1cycle=point_ld;
        point_ld=0;
        
        v_dI_ld[1]=v_dI_ld[0];
        
        v_dI_ld[0]=(sword)v_I_ldx[0]+v_I_ldx[1]-(sword)v_I_ldx_lst[0]-v_I_ldx_lst[1];
    
        if(VF_MMOVE_ENG)
        {
            //2015-11-20 增加控制
            //如果发现如何突变，则立即对油门进行准备动作；
            
            if(v_dI_ld[0]>20){
                DC_con(-1,126);
                t_lst_Drive=T_MAINTMR;
                //deTPS++;
                v_nb_IncLd++;
                
                //v_TPS_tg+=20;
                SetAsTpstg((sword)v_TPS_tg+v_dI_ld[0]);
                
            } 
            else if(v_dI_ld[0]<(sword)-20)
            {
                DC_con(1,126);
                t_lst_Drive=T_MAINTMR;
                v_nb_DecLd--;
                
                //v_TPS_tg-=20;
                SetAsTpstg((sword)v_TPS_tg+v_dI_ld[0]);
            }
        }
    
    
        //2016-1-4 增加对LD是否故障的校核
    
    
        if(!VF_MMOVE_ENG) 
        {   
            //2016-1-4
            if(v_I_ldx[0]>20){
                if(v_nb_ld_err[0]<255)v_nb_ld_err[0]++;
            }
            else
            {
                v_nb_ld_err[0]=0;
            }
            
            if(v_I_ldx[1]>20){
                if(v_nb_ld_err[1]<255)v_nb_ld_err[1]++;
            }
            else
            {
                v_nb_ld_err[1]=0;
            }
        } 
        else
        {
            
            if(
                (v_TPS_odr>11&&v_dI_ld[0]<20)
                ||
                (v_TPS_odr<=11&&v_dI_ld[0]>80)
            )
            {
                if(v_nb_ld_total_err<255)v_nb_ld_total_err++;
            } 
            else
            {
                v_nb_ld_total_err=0;
            }
            
        }
    
    
        //VF_OPEN0_LDSENSOR,VF_OPEN1_LDSENSOR
    
        if(v_nb_ld_err[0]>100||v_nb_ld_err[1]>100||v_nb_ld_total_err>100)
        {
            VF_OPEN0_LDSENSOR=true;
            VF_OPEN1_LDSENSOR=true;    
        }
        else
        {
            VF_OPEN0_LDSENSOR=false;
            VF_OPEN1_LDSENSOR=false;    
        }
    
    }
    
    
    
}

/*获取1通道的电流采样*/
void Save_I_Load_12(word iLoad,word iLoad2)
{
    sword sw;
    
    
    
    v_I_ini_lst2[0]=v_I_ini_lst[0];          //2013-1-13 保存初始值      
    v_I_ini_lst[0]=v_I_ini[0];               //2013-1-13保存初始值 
 
  
    v_I_ini[0]=(byte)((iLoad+iLoad2)>>3);
    temp_I1_init_lst_cap=temp_I1_init_cap;     
    temp_I1_init_cap=v_I_ini[0];

    
    
}

/*获取2通道的电流采样*/
void Save_I_Load_34(word iLoad,word iLoad2)
{  
    sword sw;
    
    
 //   v_nbtemp3++;        //lcw test
    
    v_I_ini_lst2[1]=v_I_ini_lst[1];    
    v_I_ini_lst[1]=v_I_ini[1];     

    v_I_ini[1]=(byte)((iLoad+iLoad2)>>3);
    
    temp_I2_init_lst_cap=temp_I2_init_cap;     
    temp_I2_init_cap=v_I_ini[1];

//2013-1-27    VF_MAYACC1_LDSENSOR=false;
//2013-1-27    VF_MAYDEC1_LDSENSOR=false;  
      
//lcw2013-1-13    sw=(sword)v_I_ini[1]-(sword)v_I_stable-v_ld0[1];
    sw=(sword)v_I_ini[1]-(sword)v_I_ini_lst[1];


}

/*机油位置检测*/
void FD_LUB()    
{  
    word w1,w2;
    /*
    
    if(VF_MMOVE_ENG){       //只有运转时才检测机油或熄火开关
      
        F_SelCh_ADC(CH_AD_EngineOil);	
        w1=F_WaitSample_ADC_result();
        F_SelCh_ADC(CH_AD_EngineOil);	
        w2=F_WaitSample_ADC_result();
    
        w1=(byte)(w1>>2);
        w2=(byte)(w2>>2); 
        w1=(w1+w2)/2;
//    v_EngineOil=(w1+w2)/2;
    

        if(w1<50){       //如果机油采样值<50,表明无机油，或关机
          if(VF_LUBEMPTY_ETC_lst)VF_LUBEMPTY_ETC=true;
          VF_LUBEMPTY_ETC_lst=true;
        }else {
         VF_LUBEMPTY_ETC_lst=false;  
        VF_LUBEMPTY_ETC=false;    
        }
    }
    
    if(REMARK!=4)VF_LUBEMPTY_ETC=false;//2013-6-21 因为 LAMBDA输入与油位输入公用了，所以只让油位在REMARK=4时起作用 
    *///2014-1-27 peter ,delete
}


/*
    Function:calculate the Target TPS by Current
*/
void F_CAL_TPS_TG_BY_I(word rev_3d9,byte tps)
{
      byte b, b_I0,b_I2;
      sword sw,sw_I1,sw_I2,sw_v_p;    //sw_v,sw_a,
      temp_I_cac_Pos_lst=temp_I_cac_Pos;     //test
      temp_I_cac_Pos=temp_Pos;     //test

      
   // 先采集电流信号1 
     
      
     // F_SelCh_ADC(CH_AD_CURRENT1,0);
      
      sw=FAD_1Channel(CH_AD_CURRENT1,false,2);
      //sw=F_WaitSample_ADC_result(); 
      b_I0=b=sw>>2; 
      
      //2013-2-4
      if(b<10){
          nb_er_lds0++;    
      } else nb_er_lds0=0;
      if(nb_er_lds0>200)VF_OPEN0_LDSENSOR=true;
      
      //VF_OPEN0_LDSENSOR=true;           //电流采样过小，也是开路
      
      
     /*处理并诊断输入的电流信号*/
      if(!VF_MMOVE_ENG){
            if(v_ld0[0]>v_I_ini[0])v_ld0[0]=v_I_ini[0];
      }else{
 
             //2013-1-31加入电流传感器好才有效 &&(!VF_OPEN0_LDSENSOR)
             if(!VF_CAP0_LDSENSOR){

                if(VF_I_CAP_lst||VF_I_CAP_Twns_lst){
                    if(b>v_ld0[0])v_I_ini[0]=b_I0;   //如果上次捕获成功，该采样值有效 
                } else{
                    if(b>v_ld0[0])b=b-v_ld0[0];
                    else b=0;
                    if(b<4)v_I_ini[0]=v_ld0[0];                

                VF_MAYACC0_LDSENSOR=false;
                VF_MAYDEC0_LDSENSOR=false; 
                }

             }
                
             if(VF_CAP0_LDSENSOR)VF_I_CAP_lst=true;
             else VF_I_CAP_lst=false; 
             if(VF_I_CAP_Twns)VF_I_CAP_Twns_lst=true;
             else VF_I_CAP_Twns_lst=true;      
   
             st_I_Not_cap=(st_I_Not_cap<<1)&0xfe;         //test
             if(!VF_CAP0_LDSENSOR)st_I_Not_cap++;         //如果没捕获电流1，st_I_No_cap的bit0置位
             
             VF_CAP0_LDSENSOR=false;
             VF_I_CAP_Twns=false;
             
          
      }
      
      
      
      if(VF_OPEN0_LDSENSOR)v_ld0[0]=0;  //2012-11-11 add 

      
   // 先采集电流信号 2          
      //F_SelCh_ADC(CH_AD_CURRENT2,0);
      //sw=F_WaitSample_ADC_result(); 

      sw=FAD_1Channel(CH_AD_CURRENT2,false,2);

      b_I2=b=sw>>2; 
      
      //2013-2-4
      if(b<10){
          nb_er_lds1++;    
      } else nb_er_lds1=0;
      if(nb_er_lds1>200)VF_OPEN1_LDSENSOR=true;
      
      
      if(!VF_MMOVE_ENG){
          if(v_ld0[1]>v_I_ini[1])v_ld0[1]=v_I_ini[1];
          //if(v_I_ini[1]>M0_THRE_LD_OPEN_STOP)VF_OPEN1_LDSENSOR=true;  //2013-2-4 delete
          
      } else{
          //if(v_I_ini[1]>M0_THRE_LD_OPEN_RUN)VF_OPEN1_LDSENSOR=true;   //2013-2-4 delete
          
                             //I2
            //2013-1-31加入电流传感器好才有效     &&(!VF_OPEN1_LDSENSOR) 
          if(!VF_CAP1_LDSENSOR){

                if(VF_I2_CAP_lst||VF_I2_CAP_Twns_lst){
                    if(b>v_ld0[1])v_I_ini[1]=b_I2;   //如果上次捕获成功，该采样值有效 
                } else{
                    if(b>v_ld0[1])b=b-v_ld0[1];
                    else b=0;
                    if(b<4)v_I_ini[1]=v_ld0[1];                
                VF_MAYACC1_LDSENSOR=false;
                VF_MAYDEC1_LDSENSOR=false; 
                }

          }
                
          if(VF_CAP0_LDSENSOR)VF_I_CAP_lst=true;
          else VF_I_CAP_lst=false;  
          if(VF_I2_CAP_Twns)VF_I2_CAP_Twns_lst=true;
          else VF_I2_CAP_Twns_lst=true;     
 
          st_I2_Not_cap=(st_I2_Not_cap<<1)&0xfe;         //test
          if(!VF_CAP1_LDSENSOR)st_I2_Not_cap++;         //如果没捕获电流1，st_I_No_cap的bit0置位
             
          VF_CAP1_LDSENSOR=false;
          VF_I2_CAP_Twns=false; 
 
          
      }
      


  
       if(VF_OPEN1_LDSENSOR)v_ld0[1]=0;
   


      
      /*将电流换算为相对负荷*/  
          
      b=0;      

      if(!VF_OPEN0_LDSENSOR){
          if(v_I_ini[0]>v_ld0[0])b=v_I_ini[0]-v_ld0[0];
      } else {               //2013-1-31 电流传感器有故障时也算，将有效负荷逐渐变为0
          if((v_I_ld>0)&&VF_RUN8_TMCCL)b=v_I_ld-1;
          else   b=v_I_ld;         //2013-2-1

      }
      
      v_I_ld_lst4=v_I_ld_lst3;
      v_I_ld_lst3=v_I_ld_lst2;
      v_I_ld_lst2=v_I_ld_lst;
      v_I_ld_lst=v_I_ld;       
      v_I_ld=b;
      temp_I1_ld_lst=temp_I1_ld;    //test      
      temp_I1_ld=b;                  //test
      
      
      
      b=0;//2012-11-12 纠正；

      if(!VF_OPEN1_LDSENSOR){
          if(v_I_ini[1]>v_ld0[1])b=v_I_ini[1]-v_ld0[1];
      } 
      else {         //2013-1-31 电流传感器有故障时也算，将有效负荷逐渐变为0
          if((v_I2_ld>0)&&VF_RUN8_TMCCL)b=v_I2_ld-1;
          else b=v_I2_ld;     //2013-2-1
      }

      
      v_I2_ld_lst4=v_I2_ld_lst3;
      v_I2_ld_lst3=v_I2_ld_lst2;
      v_I2_ld_lst2=v_I2_ld_lst;
      v_I2_ld_lst=v_I2_ld;
      v_I2_ld=b; 
      temp_I2_ld_lst=temp_I2_ld;    //test      
      temp_I2_ld=b;                  //test
      
      /*清除电流捕捉标志*/
      
           
      
      temp_st_I2_not_cap=st_I2_Not_cap;        //lcw test           
      temp_st_I1_not_cap=st_I_Not_cap ;


/*2013-1-31
//2013-1-25将电流大小插TPS的程序移到此处
        //电流策略
      //计算稳定电流， 没有实质意义
      sw_v=(sword)v_I_ld_lst-v_I_ld;
      sw_a=(sword)v_I2_ld_lst-v_I2_ld;
 */           

//2013-1-27将平均电流作为稳态值记录
      v_I_lst=v_I; 
      v_I_stable_lst=v_I_stable;
      v_TPS_I_lst=v_TPS_I; 
      v_TPS_I_stable_lst=v_TPS_I_stable;                 


                //2013-1-27 改为瞬态和平均电流都查表
                //突变时使用瞬态值，不然使用平均值

      v_I=(word)v_I_ld+(word)v_I2_ld;
      v_I_stable=((word)v_I_ld_lst+v_I_ld+v_I_ld_lst2+v_I_ld_lst3+v_I_ld_lst4
                  +(word)v_I2_ld_lst+v_I2_ld+v_I2_ld_lst2+v_I2_ld_lst3+v_I2_ld_lst4)/5;
                //2013-1-31平均次数增多  
      v_TPS_I=F_2DLookUp(v_I,M2_TPS_BYLD,6,0,false);        //瞬态电流对应的TPS
      v_TPS_I_stable=F_2DLookUp(v_I_stable,M2_TPS_BYLD,6,0,false);    //2013-1-27 平均电流对应的TPS      
 
                //v_I_stable  20 for 1440w
            /*
                因为v_I_stable的滤波，导致从检测到电流突变，
                    到电机开始执行，需要差1-2圈；
                    再次电机执行，到电机启动，又要差1-2圈；
                    因此，会差两个循环，
                    再从动作，直到到达目标，又要差几个循环，
                    因此，必然要有转速下掉的过程；
                    
                直流电机的最大速度：30-40采样值/循环；    
            */
            
            /*启动结束后开始采用电流*/

      VF_INC_LDSENSOR=false;
      VF_DEC_LDSENSOR=false;
      VF_MAYACC0_LDSENSOR=false;
      VF_MAYDEC0_LDSENSOR=false;
      VF_MAYACC1_LDSENSOR=false;
      VF_MAYDEC1_LDSENSOR=false;          
      if(VF_STA_LD>VF_TPS_READY){
                
          if(v_ccl_Istable<255)v_ccl_Istable++;
                
          sw_v_p=((sword)v_TPS_I-v_TPS_I_lst);        //两次瞬态电流对应TPS的差异
                 //电流变化超过不大，取1/2去控制油门
          if((sw_v_p<=20)&&(sw_v_p>=-20))sw_v_p/=2;

                                                      //两次瞬态电流差异过大时，要特殊处理
          if(sw_v_p>20){
              if(Act_Step<0)Act_Step=0;
              v_nbStep_ETC+=sw_v_p;   //1、将电流变化引起的TPS差异，作为步数
                                      //2、当步数大于10时，置电流增减标志，并预加电
              if(v_nbStep_ETC>100)v_nbStep_ETC=100;

              VF_MAYACC0_LDSENSOR=true;              //2013-1-27预加电
              DC_INC;
              DC_ON; 
              //2013-1-28              VF_INC_LDSENSOR=true;  
              v_ccl_Istable=0;         
          }else if(sw_v_p<-20){        //3、当步数小于-10时，置电流减标志，并预加电
              if(Act_Step>0)Act_Step=0;
              v_nbStep_ETC+=sw_v_p;           
              if(v_nbStep_ETC<-100)v_nbStep_ETC=-100;
              VF_MAYDEC0_LDSENSOR=true;        //2013-1-27预加电
              DC_DEC;
              DC_ON; 
              //2013-1-28              VF_DEC_LDSENSOR=true; 
              v_ccl_Istable=0;          
          }else if(v_ccl_Istable<8){ 
              v_nbStep_ETC+=sw_v_p;          //突变期间将瞬态电流对应的TPS变化的一半，加到步数上
          
          }else{                             //4、突变后，用稳态值的变化
              sw_v_p=((sword)v_TPS_I_stable-v_TPS_I_stable_lst);        //两次电流的差异
              //2013-1-31             sw_v_p/=2;        //电流变化超过不大，取1/2去控制油门
              v_nbStep_ETC+=sw_v_p;   //1、将电流变化引起的TPS差异，作为步数
          }
      }
     
      
      
      temp_TPS_I_lst=v_TPS_I_lst;   //test    
      temp_TPS_I=v_TPS_I;           //test      
 
      
}

/*
    根据目标油门，确定电机工作参数
    根据TPS位置，设置DC MOTOR工作参数
    
    需要考虑动态油量修正;
    
    
    设置电机工作参数时，需要考虑因素
    1）目标油门与当前油门的差异；
    2）当前电机转速；
*/

void FS_DCMotor(void)
{             //每圈都做

      byte bClk;
      byte TPS_now;
      sword sw;

      //v_TPS_tg=128;
      if(!VF_BYTPD_ETC)v_Adj[ADJ_TPDtg]=0;

      if(VF_KEYON_HARD)
      {
          if((word)(T_MAINTMR-t_lst_Drive)>2500)
          {   //10MS 每次
              
              TPS_now=FAD_1Channel(CH_AD_TPS,true,2);
              
              
              sw=TPS_now-(sword)v_TPS_tg;
              
              
              if(sw>0){
                  bClk=1;
              } else if(sw<0)
              {
                  bClk=255;
              }else bClk=0;
              
              //sw=sw/2;
              if(sw<0)sw=-sw;
              
              //2015-11-8 增加这句话，即油门与目标差异20时，就应该全速运行电机了
              //sw=sw*4;
              sw=sw*3; //2015-12-16
              if(sw>127)sw=127;
              
              DC_con(bClk,sw);
              
              t_TPS_lst=TPS_now;
              
              t_lst_Drive=T_MAINTMR;
          }
      } 
      else
      {   //2015-11-15 add 
          DC_con(0,0);
      }
      
      
      /*
          考虑
          TPS控制的P,I,D三项
          
          I:直接对TPS_TG起作用
          P:
          D:
      
      
      
      */
 




      return;
      
} 


//2016-9-7
void FS_SMotor_IACV(void)
{ 
         //2016-1-25 增加步进电机控制
       
       /* 
        if(KESEI==1)  
            v_StepAct=100;
        else
            v_StepAct=-100;// 
       */
	   if(!VF_MMOVE_ENG||VF_NEWCYCLE_ATMAIN_CAS)
	   {
		   IACV_con();//发动机运行之后，每圈执行一次
	   }
       
        
       
        //if(VF_KEYON_HARD)//2020-8-17 停机过程允许学零
        {
            //if(T_MAINTMR-t_lst_Drive>500)//2MS 每次
            //if(T_MAINTMR-t_lst_Drive>600)
            if(v_step_sm_tg==0)//2017-10-23 初始化时，步进电机保持较快的步进频率
			{
				/*
				if(T_MAINTMR-t_lst_Drive>600)
				{   
                
                	FC_SM(false);//2016-1-25
                	t_lst_Drive=T_MAINTMR;
            	}
				*/
				if((v_IACV_ini_st!=101))
				{
					v_IACV_ini_st=101;//2020-12-18 正在学0中
					Save_Data_EE(0x7f,101);			
				}
				
				//2017-11-8 前面252步采用较高的频率控制,最后9步采用较低频率
				if(v_StepAct<(-9))
				{
					if((word)(T_MAINTMR-t_lst_Drive)>600)//2.4ms 
					{   
                
						//FC_SM(false);//2016-1-25
						F_StepMT_3T1();
						t_lst_Drive=T_MAINTMR;
					}
				}
				else if(v_StepAct<0)
				{
					if((word)(T_MAINTMR-t_lst_Drive)>8250)//33ms
					{   
                
						//FC_SM(false);//2016-1-25
						F_StepMT_3T1();
						t_lst_Drive=T_MAINTMR;
					}
					
					if(v_StepAct==0)
					{
						//if(!VF_KEYON_HARD)
						{
							v_IACV_ini_st=100;
							Save_Data_EE(0x7f,100);//学零完成写状态字,关钥匙学零：100；
						}						
					}
					
					
				}								
			}
			else
			{
				if(VF_MMOVE_ENG)
				{
					if((word)(T_MAINTMR-t_lst_Drive)>1250)//2017-6-26 loncin要求修改为200HZ 根据电机手册，控制周期需要避开70~140step/sec区间
					{   
                
						#ifdef SY_CORADJ
						if(VF_CORADJ&&(v_StepAct<0))
						{
							F_StepMT_3T1();//2020-3-24 粗调节过程仍然采用走三步停一步的方式
						}
						else
						#endif
						{
							FC_SM(false);//2016-1-25
						}						
						t_lst_Drive=T_MAINTMR;
					}
				}
				else if(VF_KEYON_HARD)//2020-8-17 关钥匙不要预走位，只学零即可
				{
					if(T_MAINTMR-t_lst_Drive>600)//2019-5-7 预走位频率增大
					{   
                
						FC_SM(false);
						t_lst_Drive=T_MAINTMR;
						if((v_StepAct!=0)&&(v_IACV_ini_st!=103))
						{
							v_IACV_ini_st=103;//2021-2-26 代表在开始预走位了，预走位的状态为103
							Save_Data_EE(0x7f,103);
						}
					}
				}
				
				/*if(VF_KEYON_HARD&&(v_IACV_ini_st!=102))
				{
					v_IACV_ini_st=102;
					//Save_Data_EE(0x7f,102);	//开钥匙状态下学零完成，正常运行过程写102状态		
				}*/
			}
        }
		//else
		//{
			//FC_SM(true);
		//}
		
		if(IS_KEY_OFF&&!VF_KEYON_HARD&&(v_t_eng_t_s>10)&&(v_IACV_ini_st==100))
			//学零结束关电机
		{
			FC_SM(true);
		}
      

/*****************2017-3-10 以下根据发动机运行状况判断是否需要进行粗调节******************************/
		#ifdef SY_CORADJ
		F_CORADJ_IACV();
		#endif

/*****************************************************************************************************/		  
	  
     //2016-9-9   
        if(!VF_MMOVE_ENG||VF_NEWCYCLE_ATMAIN_CAS)
	    {
			Fset_SM(v_step_IACV_on);
		}
      //      Fset_SM((word)KESEI*10);
	  
	  
	  
          
}
/*
    关于H-BRIDGE的驱动特性：
    如果从DC_ON到DC_OFF,而不操作DC_DEC或者DC_INC，
    电机不会迅速关闭，即高的那个电平不会立即归零；
*/
/*
    DC MOTOR PARAMETER
    120RPM--2RPS
    125MS--90DEG--225samp        0.4DEG/samp
    1ms--2samp，使用步进电机时对应2v_nbStep_ETC，
              即1samp对应一个v_nbStep_ETC;
              
   
   关于发电机的PID控制
      TPS(K)+=KP[E(K)-E(K-1)]
          +KI*TS*E(K)
          +KD*{E(K)+E(K-2)-2*E(K-1)}/TS
      
      
      TPS(K)=KP*E(K)+KI*SIGMA{E(N),N FROM 0 TO K}*TS+KD*{E(K)-E(K-1)}/TS    
    
*/
/*
    对电机的调节采用：占空比+周期数的方式
        D/8代表占空比
        nP代表周期数
            步数
*/

//=========================================================================
/*
    设定VF_TPS_READY状态前的目标TPS
    
    ECT初始化
    当VF_STA_LD<=VF_SETINIT_STALD时，进行初始化动作
    与发动机是否启动无关，
    
    到VF_TPS_READY状态后，进入正常控制
    
    需要考虑TPS故障的情况；
    
    
*/
//=========================================================================
void FS_ECT_IdlePosition(byte tps,byte tps_odr,byte tes)
{
  sword w1,w2;
  sword sw;  
    
    
    //增加温度对启动油门的修正　2012-6-6
    
    w1=tes;
    if(w1<70)
    {
        w1=70-tes;
        w1=w1/4;
    } else w1=0;
    
    
    v_TPS_st=M0_TPS_ST;          //2013-2-1 初始油门与温度无关  

    
    VF_ACC_ETC=false;
    VF_INC_LDSENSOR=false;
    VF_DEC_ETC=false;
    VF_DEC_LDSENSOR=false;
    
     //发动机停止时，直流电机的油门动作及其结束的标记要确认：何时v_nbStep_ETC==0
     //应该设目标TPS，过一定时间还没接近认为有问题。
    
    //设置电机初始位置
    if(VF_STA_LD==VF_TOTPSE0_STALD) 
    {
        
        v_TPS_tg=v_TPS_st+10;       //to v_TPS_st+10
        sw=(v_TPS_tg-(sword)tps);
        if(sw>-3&&sw<3){
            VF_STA_LD=VF_TOTPSMIN_STALD;
            v_step_formin=0;      //for the next period        
        }        
        
        else
        {
            if(v_step_formin>250) //2012-10-27 from 400 to 200
            {   //油门有问题
                VF_FAILURE_ETC=1; //2012-11-2       
            }
            else
            {//油门向怠速位置逼近
               v_step_formin+=1; 
            }
        }
        
        //如果油门故障，则直接开到最小油门位置；
        if(VF_ER_TPS){
            VF_STA_LD=VF_TOTPSMIN_STALD;
        }
    }
    else if(VF_STA_LD==VF_TOTPSMIN_STALD)
    {   //到达最小位置
        v_TPS_tg=M0_ETC_TPS_MIN+2;       //to min+2
        sw=(v_TPS_tg-(sword)tps);
        if(sw>-3&&sw<3){
            //到达最小位置
             VF_STA_LD=VF_SETINIT_STALD;
             
             v_step_formin=0;      //for the next period   
        }
        else  {
            if(v_step_formin>250) //2012-10-27 from 400 to 200
            {   //油门有问题
                VF_FAILURE_ETC=1; //2012-11-2       
            }
            else v_step_formin+=1; 
        }
            
    }
    else if( (VF_STA_LD==VF_SETINIT_STALD)) //2012-11-13 ADD
    {
        if(VF_ER_TPS){
            VF_STA_LD=VF_TPS_READY;
            v_nbStep_ETC=20;
        } 
        else {
        
              //deTPS++;
              
              v_TPS_tg=v_TPS_st+2;       //to v_TPS_st+1
              sw=(v_TPS_tg-(sword)tps);
              if(sw>-3&&sw<3){
                    //到达启动位置
                  VF_STA_LD=VF_TPS_READY;
                  v_TPS_tg=v_TPS_st+1;    //to v_TPS_st+1
                  v_step_formin=0;      //for the next period  
               
              }
              else  
              {
                  if(v_step_formin>250) //2012-10-27 from 400 to 200
                  {   //油门有问题
                      VF_FAILURE_ETC=1;  
                  }
                  else v_step_formin+=1; 
              }
        }
        
    }  
    else if(VF_STA_LD>=VF_TPS_READY)
    {
        //2015-12-16
        if(!VF_MMOVE_ENG||VF_BYTPD_ETC)
        {
            //v_TPS_tg=v_TPS;
              //v_TPS_tg=v_TPS_st+2;       //to v_TPS_st+1
              sw=(sword)v_TPS_st+2+v_Adj[ADJ_TPDtg];
              
              if(sw>M0_ETC_TPS_MAX)sw=M0_ETC_TPS_MAX;
              if(sw<M0_ETC_TPS_MIN)sw=M0_ETC_TPS_MIN;
              
              v_TPS_tg=(byte)sw;
        
        }
    }
    
}

//================================================================
//Function: Ignition CA Control for steady condiction
//================================================================
sbyte F_dign(word rev_3d9,sbyte ign)
{
    sbyte sb1;
    sbyte sb_lst;
    sword sw;
    
  
  
 // return;
  
      if(VF_STA_LD==VF_STEA_STALD || VF_STA_LD==VF_TRAN_STALD)          
     {
      
      		sw=(sword) (v_rev3d9_tg- (sword)rev_3d9);// 限制在最值范围内；


           //2013-1-20转速在10单位以下不压
          if(sw<0)
          {               //sw<0,转速高于目标转速时
              sw+=20;               //sw+20
              if(sw>0)sw=0;          //+20后>0,不提前
          }
  
      		if(sw>M0_DIGN_TH_ECT_P)sw=M0_DIGN_TH_ECT_P;    //2011-11-24 peter add 10
      		else if(sw<M0_DIGN_TH_ECT_N)sw=M0_DIGN_TH_ECT_N;
 
      		
      		v_d_ign_fb=(sbyte)sw;
      		
      		
      		sb_lst=v_d_ign_fb_ave;
      		
      		v_d_ign_fb_ave=((sword)v_d_ign_fb_ave*7+ v_d_ign_fb)/8;
      
      		if(v_d_ign_fb_ave==sb_lst)
      		{
      		    if(v_d_ign_fb>0)v_d_ign_fb_ave++;
      		    else if(v_d_ign_fb<0) v_d_ign_fb_ave--;
      		}
      		
      		if(v_d_ign_fb_ave>= M0_DIGN_TH_ECT_MAX_P)
      		{   //ECT INC
      		    nb_p_limit++;//v_nbStep_ETC++;
      		    nb_n_limit=0;
      		}
      		else if(v_d_ign_fb_ave<=(M0_DIGN_TH_ECT_MAX_N))//+7) )
      		{
      		    //ECT DEC
      		    nb_n_limit++;//v_nbStep_ETC--;
      		    nb_p_limit=0;
      		}
      		else
      		{
      		    nb_n_limit=nb_p_limit=0;
      		}
      		
      		
      		if(v_d_ign_fb_ave>M0_DIGN_TH_ECT_MAX_P+20) v_d_ign_fb_ave=M0_DIGN_TH_ECT_MAX_P+20;
      		if(v_d_ign_fb_ave<M0_DIGN_TH_ECT_MAX_N-20) v_d_ign_fb_ave=M0_DIGN_TH_ECT_MAX_N-20;
      		
      		if(nb_p_limit>7)
      		{
      		    nb_p_limit=0;
      		    //v_nbStep_ETC++;//+=v_d_ign_fb_ave/2;
      		    v_d_ign_fb_ave=0;//v_d_ign_fb_ave/2;
      		
      		    v_dnb_byIgn++;//；
      		}
      		
      		//if(nb_p_limit>7)
      		if(nb_n_limit>7)//2015-11-14
      		{    //  ??
      		    nb_n_limit=0;
      		    //v_nbStep_ETC--;//+=v_d_ign_fb_ave/2;
      		    v_d_ign_fb_ave=0;//v_d_ign_fb_ave/2;
      		    
      		    v_dnb_byIgn--;//2015-10-23
      		}
     }
     else v_d_ign_fb=0;
     
    
    if(v_d_ign_fb>10)v_d_ign_fb=10;  //2012-10-28  临时
    if(v_d_ign_fb<-15)v_d_ign_fb=-15;//5;  //2012-10-28  临时    
    
    //2012-10-28 return
    ign+=v_d_ign_fb;
    
    return ign;
          
}


//==================================================================
//Function: ECT control for Load Transit condiciton
//Output:v_nbStep_ETC , + for Inc, - for Dec, 0.7deg per step
//==================================================================
void F_step_ECT(word rev_3d9, byte tps)
{
        sword sw,sw1;

        //sbyte sb;
        sword sb;
        sword d_ETC_PID;                //另存为013-1-13 将转速PID的结果累加后再加到步数上，便于判断
        //byte bfk;
        //byte b_ddrev,b_dacc,b_dtps,b_rev_tg;
        byte i;

        sword sw_v,sw_a,sw_v_p;
        long int sw_v_i;


        /*-----------------------------------------------------------------------------*/ 
            v_KP_M8=Adj_byte(BYPTR_M0_KP_M8,ADJ_kp);
            v_KI_M8=Adj_byte(BYPTR_M0_KI_M8,ADJ_ki);
            v_KD_M8=Adj_byte(BYPTR_M0_KD_M8,ADJ_kd);

            v_nfull=Adj_byte(BYPTR_M0_NFULL_62D5RPM,ADJ_nfull)*(word)16;
            v_nstand=Adj_byte(BYPTR_M0_NSTAND_62D5RPM,ADJ_nstand)*(word)16;
             
            v_rev3d9_tg=v_nfull; //2013-6-19

        /*-----------------------------------------------------------------------------*/ 
            //计算加速度和目标转速差异
            v_acc_rev=(sword)rev_3d9-rev_3d9_lst;  //加速度
            v_d_rev_to_tg=(sword)rev_3d9-v_rev3d9_tg;                   //v_rev3d9_tg是上一循环的RPm

        /*-----------------------------------------------------------------------------*/ 
        //从 启动到过度过程的状态判定：连续8圈大于3400才到正常状态
            if(VF_STA_LD<=VF_TPS_READY)     //判断是否已正常启动。2012-6-6 change to <=
            {                              //因在启动过程中发现rpm_3d9有计算错误的可能，
                v_ccl_StartsProcess=v_ccl_StartsProcess<<1;      //所以要求8次连续大于3000转，则认为正常启动，进入瞬态过程。
                v_ccl_StartsProcess&=0xFE;
 
                if( rev_3d9>(v_rev3d9_tg-50))v_ccl_StartsProcess++;
                
                if(v_ccl_StartsProcess==0xFF)
                {      
                    if(v_t_eng_d1s>20)
                    {   //2012-5-30 ADD CONDICTION
                        //VF_STA_LD=VF_TRAN_STALD;//VF_STEA_STALD;       //2011-11-19  rex
                        VF_STA_LD=VF_STEA_STALD;       //2015-10-22
                    }
                } 
            }


        /*--------------------------根据转速及加速度对电机的控制------------------------*/
        //2013-1-30 lcw：应该在VF_TPS_READY前不能启动。
            //if(VF_STA_LD<=VF_TPS_READY)  //启动加速控制   2012-6-6 change to <=
            if(VF_STA_LD==VF_TPS_READY)    //启动加速控制   
            {
                i=30;
                if(v_teng_inuse<70)i=100-v_teng_inuse;
                if(i>30)i=30;
                
                if(VF_RUN8_TMCCL) //2013-2-2 liulaoshi zoule
                {
                    if((v_TPS<125)&&(rev_3d9<v_rev3d9_tg)){
                        //v_nbStep_ETC+=2;//2013-2-1 from 1to 2
                        //v_TPS_tg=v_TPS+2;
                        //v_TPS_tg=v_TPS+2;
                        //2015-12-27 change to the following
                        SetAsTpstg((sword)v_TPS_tg+2);
                    }
                    else if(rev_3d9>v_rev3d9_tg){
                        //v_nbStep_ETC-=2;//v_nbStep_ETC--;
                        //v_TPS_tg=v_TPS-2;
                        //2015-12-27 change to the following
                        SetAsTpstg((sword)v_TPS_tg-2);
                    }
                }
            }
            
        /*-------------------------------------------------------------------------------*/
            if(!VF_YSTDC_INJPHASE) return;     //非喷油圈，则该程序段不起作用
    
            if(!VF_BFIRE_ENG)return; 


        /*----------状态判断-------------------------------------------------------------*/ 
            st_rev_DEC=(st_rev_DEC<<1)&0xfe;
            st_rev_ACC=(st_rev_ACC<<1)&0xfe;
            st_a_DEC=(st_a_DEC<<1)&0xfe;
            st_a_ACC=(st_a_ACC<<1)&0xfe;
            
            if(v_d_rev_to_tg>(v_d_rev_to_tg_lst+1))st_rev_ACC++;       //2013-1-13增1以上，才算加速
            if(v_d_rev_to_tg<(v_d_rev_to_tg_lst-1))st_rev_DEC++;       //2013-1-13减1以上，才算减速            
 
            if(v_acc_rev>v_acc_rev_lst)st_a_ACC++;
            if(v_acc_rev<v_acc_rev_lst)st_a_DEC++;
            
        /*--------------------------------------------------------------------------------*/ 
            if(v_d_rev_to_tg<10)V_ETC3_STATUS_RPMto_H=0 ; //转速低于目标转速后，清除所以到达高速标记
            if(v_d_rev_to_tg>-10)V_ETC4_STATUS_RPMto_L=0 ; //转高于目标转速后，清除所以到达低速标记

            //2013-1-20计算平局转速 4,3,2,1

            v_d_AVrpm_to_tg=(v_d_rev_to_tg+v_d_rev_to_tg+v_d_rev_to_tg+v_d_rev_to_tg
                   +v_d_rev_to_tg_lst+v_d_rev_to_tg_lst+v_d_rev_to_tg_lst
                   +v_d_rev_to_tg_lst2+v_d_rev_to_tg_lst2
                   +v_d_rev_to_tg_lst3)/10;
              

            //V_ETC3_STATUS_RPMto_H=0;
            //V_ETC4_STATUS_RPMto_L=0;

            //2012-11-18 add ccl_disableN
            if((VF_STA_LD>VF_TPS_READY))
            {   //2012-5-30 ADD CONDICTION

                d_ETC_PID=0;                                               //2013-1-13转速 PID增量控制结果置零
                
                //v_nbStep_ETC=0;   //2015-10-23
                
                /*-----------比例控制-------------------------------------------------*/
                      //2013-1-17 准备用平均转速做转速比例控制
                      if(v_d_AVrpm_to_tg>20||v_d_AVrpm_to_tg<-20
                              ||((st_rev_DEC&0x0f)==0x0f)||((st_rev_ACC&0x0f)==0x0f)){
                          sw_v_p=v_d_AVrpm_to_tg-v_d_AVrpm_to_tg_lst;
                           //偏差10以外，或连加或连减3循环以上，将偏差的变化作为比例增量
                      } else{
                          sw_v_p=(v_d_AVrpm_to_tg-v_d_AVrpm_to_tg_lst); ///4; 
                          //偏差10以内，且不连加也不连减3循环以上，将偏差的变化的1/4作为比例增          
                         
                      }
                 
                 
                      if(sw_v_p>M0_DRPM_MAX_pDC)sw_v_p=M0_DRPM_MAX_pDC;        //偏差变化大于M0_DRPM_MAX_PDC=60时，取60
                      if(sw_v_p<-M0_DRPM_MAX_pDC)sw_v_p=-M0_DRPM_MAX_pDC;       //偏差变化小于-60时，取-60             
                      
                      //2013-1-30使用v_KP_M8
                      sw_v_p=sw_v_p*v_KP_M8;
                      drev_tg_rp-=sw_v_p;                               //2013-1-30 将v_KP_M8用上
                      
                      
                      sb=0;
                      while(drev_tg_rp>M0_DREV_TG_KP_ASTEP)
                      {                    //将偏差变化的累计换成步数，M0_DREV_TG_P_ASTEP为一步
                          drev_tg_rp-=M0_DREV_TG_KP_ASTEP;
                          sb++;
                      }
                      while(drev_tg_rp<-M0_DREV_TG_KP_ASTEP)
                      {
                          drev_tg_rp+=M0_DREV_TG_KP_ASTEP;
                          sb--;
                      }
                      //sb=0;
                          
                      if(v_d_AVrpm_to_tg>M0_RPM_H0&&!VF_RPM_TO_H0){
                          //转速上升到目标转速M0_RPM_H0后，设定VF_RPM_TO_H0，ETC减M0_ETC_DEC_to_H0
                          sb-=M0_ETC_DEC_to_H0;
                          VF_RPM_TO_H0=true;
                       }
                       if(v_d_AVrpm_to_tg>M0_RPM_H1&&!VF_RPM_TO_H1){
                          //转速上升到目标转速M0_RPM_H1后，设定VF_RPM_TO_H1，ETC减M0_ETC_DEC_to_H1
                          sb-=M0_ETC_DEC_to_H1;
                          VF_RPM_TO_H1=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H2&&!VF_RPM_TO_H2){
                          //转速上升到目标转速M0_RPM_H2后，设定VF_RPM_TO_H2，ETC减M0_ETC_DEC_to_H2
                          sb-=M0_ETC_DEC_to_H2;
                          VF_RPM_TO_H2=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H3&&!VF_RPM_TO_H3){
                          //转速上升到目标转速M0_RPM_H3后，设定VF_RPM_TO_H3，ETC减M0_ETC_DEC_to_H3
                          sb-=M0_ETC_DEC_to_H3;
                          VF_RPM_TO_H3=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H4&&!VF_RPM_TO_H4){
                          //转速上升到目标转速M0_RPM_H4后，设定VF_RPM_TO_H4，ETC减M0_ETC_DEC_to_H4
                          sb-=M0_ETC_DEC_to_H4;
                          VF_RPM_TO_H4=true;
                       } 
                       if(v_d_AVrpm_to_tg>M0_RPM_H5&&!VF_RPM_TO_H5){
                          //转速上升到目标转速M0_RPM_H2后，设定VF_RPM_TO_H2，ETC减M0_ETC_DEC_to_H2
                          sb-=M0_ETC_DEC_to_H5;
                          VF_RPM_TO_H5=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H6&&!VF_RPM_TO_H6){
                          //转速上升到目标转速M0_RPM_H3后，设定VF_RPM_TO_H3，ETC减M0_ETC_DEC_to_H3
                          sb-=M0_ETC_DEC_to_H6;
                          VF_RPM_TO_H6=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H7&&!VF_RPM_TO_H7){
                          //转速上升到目标转速M0_RPM_H4后，设定VF_RPM_TO_H4，ETC减M0_ETC_DEC_to_H4
                          sb-=M0_ETC_DEC_to_H7;
                          VF_RPM_TO_H7=true;
                       }                  
                       
                       
                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L0&&!VF_RPM_TO_L0){
                          //转速下降到目标转速以下M0_RPM_L0后，设定VF_RPM_TO_L0，ETC增M0_ETC_DEC_to_L0
                          sb+=M0_ETC_DEC_to_L0;
                          VF_RPM_TO_L0=true;
                       }
                       if(v_d_AVrpm_to_tg<M0_RPM_L1&&!VF_RPM_TO_L1){
                          //转速下降到目标转速以下M0_RPM_L1后，设定VF_RPM_TO_L1，ETC增M0_ETC_DEC_to_L1  
                          sb+=M0_ETC_DEC_to_L1; 
                          VF_RPM_TO_L1=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L2&&!VF_RPM_TO_L2){
                          //转速下降到目标转速以下M0_RPM_L2后，设定VF_RPM_TO_L2，ETC增M0_ETC_DEC_to_L2    
                          sb+=M0_ETC_DEC_to_L2; 
                          VF_RPM_TO_L2=true;                 }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L3&&!VF_RPM_TO_L3){
                          //转速下降到目标转速以下M0_RPM_L3后，设定VF_RPM_TO_L3，ETC增M0_ETC_DEC_to_L3       ;
                          sb+=M0_ETC_DEC_to_L3; 
                          VF_RPM_TO_L3=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L4&&!VF_RPM_TO_L4){
                          //转速下降到目标转速以下M0_RPM_L4后，设定VF_RPM_TO_L4，ETC增M0_ETC_DEC_to_L4        
                          sb+=M0_ETC_DEC_to_L4; 
                          VF_RPM_TO_L4=true;
                       }       
                       if(v_d_AVrpm_to_tg<M0_RPM_L5&&!VF_RPM_TO_L5){
                          //转速下降到目标转速以下M0_RPM_L2后，设定VF_RPM_TO_L2，ETC增M0_ETC_DEC_to_L2       
                          sb+=M0_ETC_DEC_to_L5; 
                          VF_RPM_TO_L5=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L6&&!VF_RPM_TO_L6){
                          //转速下降到目标转速以下M0_RPM_L3后，设定VF_RPM_TO_L3，ETC增M0_ETC_DEC_to_L3         
                          sb+=M0_ETC_DEC_to_L6; 
                          VF_RPM_TO_L6=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L7&&!VF_RPM_TO_L7){
                          //转速下降到目标转速以下M0_RPM_L4后，设定VF_RPM_TO_L4，ETC增M0_ETC_DEC_to_L4          
                          sb+=M0_ETC_DEC_to_L7; 
                          VF_RPM_TO_L7=true;
                       }                                 
                       
                       
                       //d_ETC_p+=sb;
                       v_d_ETC_p=sb;

                       d_ETC_PID+=sb;    //2013-1-13 将比例控制步数加到d_ETC_PID  
                        
                /*-----------积分控制-------------------------------------------------*/
                        //lcw2013-1-12转速积分控制修改为：偏差累加，每v_nb_I_rpm增减一步
         
                        sw_v_i=v_d_AVrpm_to_tg; 

                        if(sw_v_i>M0_DRPM_MAX_iDC)sw_v_i=M0_DRPM_MAX_iDC;        //偏差大于50时，取50
                        if(sw_v_i<-M0_DRPM_MAX_iDC)sw_v_i=-M0_DRPM_MAX_iDC;        //偏差小于-50时，取-50             
                      
                        //2013-1-30 使用v_KI_M8                
                        sw_v_i=sw_v_i*v_KI_M8;

                        drev_tg_ri-=sw_v_i;                                          //偏差累计

                       
                        sb=0;
                        while (drev_tg_ri>(long int)M0_DREV_TG_KI_ASTEP){                    //将偏差累计换成步数，M0_DREV_TG_I_ASTEP为一步
                            drev_tg_ri-=(long int)M0_DREV_TG_KI_ASTEP;
                            sb++;
                        }
                        while (drev_tg_ri<-(long int)M0_DREV_TG_KI_ASTEP){
                            drev_tg_ri+=(long int)M0_DREV_TG_KI_ASTEP;
                            sb--;
                        }
                        
                        if(v_d_rev_to_tg>75)sb-=1;          // 转速过高时必须减快                 
                       
                        /* 
                            sb=0;
                            sw_v_i=v_d_AVrpm_to_tg;
                            sw_v_i=-sw_v_i;
                            if(sw_v_i>0){
                                sb=sw_v_i/64;
                                sb++;    
                            } else if(sw_v_i<0){
                                sb=sw_v_i/64;
                                sb--;
                            }
                        */
                        
                        v_d_ETC_i=sb;
                        temp_d_rpmI=sb ;//test
                        d_ETC_PID+=sb;                                            //2013-1-13 将积分控制步数加到d_ETC_PID                


                /*-----------微分控制-------------------------------------------------*/
                        sw_v_p=(v_d_rev_to_tg-v_d_rev_to_tg_lst)-(v_d_rev_to_tg_lst-v_d_rev_to_tg_lst2);  //求偏差变化的差值
          
                        if(sw_v_p>M0_DRPM_MAX_dDC)sw_v_p=M0_DRPM_MAX_dDC;        //偏差变化的差值限定在大于M0_DRPM_MAX_DDC
                        if(sw_v_p<-M0_DRPM_MAX_dDC)sw_v_p=-M0_DRPM_MAX_dDC;            
                      
                      //2013-1-30 使用v_KD_M8        
                        sw_v_p=sw_v_p*v_KD_M8;
                        drev_tg_rd-=sw_v_p;                                       //偏差变化差值累计
         
                      //2013-1-14转速比较低后减速突减，微分加倍
                      //2013-1-19              if(v_d_rev_to_tg<-10&&sw_v_p<-2)drev_tg_rd=drev_tg_rd-sw_v_p;
         
                        sb=0;
                        while (drev_tg_rd>M0_DREV_TG_KD_ASTEP){                    //将偏差累计换成步数，M0_DREV_TG_P_ASTEP为一步
                            drev_tg_rd-=M0_DREV_TG_KD_ASTEP;
                            sb++;
                        }
                        while (drev_tg_rd<-M0_DREV_TG_KD_ASTEP){
                            drev_tg_rd+=M0_DREV_TG_KD_ASTEP;
                            sb--;
                        }                
          

                        //d_ETC_d+=sb;
                        v_d_ETC_d=sb;
                        d_ETC_PID+=sb;
                        temp_d_rpmPID=d_ETC_PID;                //test
                
               //2013-1-13 将微分控制步数加到d_ETC_PID                
               /*d_ETC_PID为转速PID的计算结果。需要根据现场条件，做适当的限定，然后再作为调节的步数
                    1.目标转速偏差太大时，限制调节速度，
                    2.到怠速TPS后最多减一步 
                  2013-1-15大负荷突降负荷时大幅度过冲与转速低于目标值时不能快减有关.大负荷特别是全负荷.
                  大负荷特别是全负荷时，转速很可能低于目标值，突泄负荷，转速快速增加，TPS不能快减，就会过冲很多。
               */
                                
                  if(v_d_AVrpm_to_tg<-100&&d_ETC_PID<0)d_ETC_PID=0;   //2013-1-18在390外，即比标准转速低太多，则限制不减
                  if(v_d_AVrpm_to_tg>100&&d_ETC_PID>0)d_ETC_PID=0;    //2013-1-18在390外，即比标准转速高太多，则限制不加
     
                
                  //lcw2013-1-14引入张平的最大步数限制
                  #if MOTOR_TYPE==MOTOR_SM
                      //13是根据步进电机的能力而设计的；
                      if(d_ETC_PID>13)d_ETC_PID=13;
                      if(d_ETC_PID<-13)d_ETC_PID=-13;
                  #else

                      if(d_ETC_PID>M0_STEPS_MAX_DC_trip)d_ETC_PID=M0_STEPS_MAX_DC_trip;
                      if(d_ETC_PID<-M0_STEPS_MAX_DC_trip)d_ETC_PID=-M0_STEPS_MAX_DC_trip;

                  #endif
                                

                  v_d_ETC_all=d_ETC_PID;

                  v_nbStep_ETC=0;

                  d_ETC_PID=v_d_ETC_i;
                /*-------------计算油门增量----------------------------------------------*/
 
                     //2013-1-25在d_ETC_PID较小时，直接加入
                        if(d_ETC_PID<3&&d_ETC_PID>-3)v_nbStep_ETC+=d_ETC_PID;
                        else{
                            if(d_ETC_PID>0){
                                if(v_nbStep_ETC<d_ETC_PID)v_nbStep_ETC+=d_ETC_PID;
                                else v_nbStep_ETC=v_nbStep_ETC+d_ETC_PID/2;           //如果PID增，加上
                            }else{
                                if(v_nbStep_ETC>d_ETC_PID)v_nbStep_ETC+=d_ETC_PID;
                                else v_nbStep_ETC=v_nbStep_ETC+d_ETC_PID/2;  
                            }
                        }
                     

                        if(tps<M0_TPS_ST||v_TPS_tg<M0_TPS_ST){ 
                            //2013-1-17 改为TPS过小或TPS-TG过小，最多再减一步的策略
                            if(v_nbStep_ETC<-2)v_nbStep_ETC=-2;  
                        }
                        
                /*-------------计算目标油门----------------------------------------------*/
                     
                     if(VF_BYTPD_ETC){
                          sw=(sword)v_TPS_st+2+v_Adj[ADJ_TPDtg];
                          
                          SetAsTpstg(sw);
                     } 
                     else
                     {
                      
                        //v_TPS_tg
                        //sw=(sword)v_TPS+v_nbStep_ETC; 
                        sw=(sword)v_TPS_tg+v_d_ETC_i+v_d_ETC_p+v_d_ETC_d; 
                        
                        sw+=v_dnb_byIgn;      //2015-10-23 点火角对电机的修正
                        
                        v_dnb_byIgn=0;        //2015-11-14
                        
                        SetAsTpstg(sw);
                     }
                    /*    
                      //对目标油门的限制
                        //2013-1-20 可以使用，将量增的循环数增加，且不管转速
                        if((v_TPS_tg>v_TPS_last_cy)&&((st_rev_ACC&0xff)==0xff)){
                           v_TPS_tg=v_TPS_last_cy;    // 转速一直在连增情况下，目标TPS不应大于上一循环的TPS
                        }
                                            
                        
                        if((v_TPS_tg<v_TPS_last_cy)&&((st_rev_DEC&0xff)==0xff)){                //lcw2013-1-11  <39且连减3循环
                           v_TPS_tg=v_TPS_last_cy;    // 转速一直连减的情况下，目标TPS不应小于上一循环的TPS
                        }


                        //2013-1-16 如果连减6循环且转速低于目标值，TPS_tg不低于，  
                        if(((st_rev_DEC&0x3f)==0x3f)&&(v_d_rev_to_tg<25)&&(v_TPS_tg<M0_TPS_ST)){
                            v_TPS_tg=M0_TPS_ST; 
                                
                        }
                     */
            }
            
     
            
            v_nbStep_ETC_2=v_nbStep_ETC;           //lcw   test
        
        /*------------------------------------------------------------------------------------------*/ 
       // v_d_rev_to_tg_lst5=v_d_rev_to_tg_lst4;  //2013-2-4 delete
        v_d_rev_to_tg_lst4=v_d_rev_to_tg_lst3; 
        v_d_rev_to_tg_lst3=v_d_rev_to_tg_lst2; 
        v_d_rev_to_tg_lst2=v_d_rev_to_tg_lst; 
        v_d_rev_to_tg_lst=v_d_rev_to_tg;      //lcw 2013-1-11两圈一次
        rev_3d9_lst3=rev_3d9_lst2;
        rev_3d9_lst2=rev_3d9_lst;
        rev_3d9_lst=rev_3d9;
        v_acc_rev_lst=v_acc_rev;//2012-11-17
        v_TPS_last_cy=v_TPS;  //lcw2013-1-11
        v_d_AVrpm_to_tg_lst3=v_d_AVrpm_to_tg_lst2;
        v_d_AVrpm_to_tg_lst2=v_d_AVrpm_to_tg_lst;
        v_d_AVrpm_to_tg_lst=v_d_AVrpm_to_tg;        
        
}

//20120907   P=10/9   I=5/32   D=7/64   点火角限 ：v_ign_fb_limit-3    低负荷不很稳定有游车
//20120911   P=18/39  I=7/64   D=10/128 点火角限 ：v_ign_fb_limit-4    稳态转速稳定 
           //点火角调整为27度上下  MO_MAGNETOCA_QY=87; 
                  
//20121018   在Samp_All_ADC_whenStop内加了电流采样和机油采样 

//#endif

/*
    2015-12-27
    将值设置为TPS 目标
*/

void SetAsTpstg(sword sw) 
{
        /*
        if(sw<10)sw=10;
        if(sw>240)sw=240;

        */
        if(sw>M0_ETC_TPS_MAX)sw=M0_ETC_TPS_MAX;
        if(sw<M0_ETC_TPS_MIN)sw=M0_ETC_TPS_MIN;
        
        v_TPS_tg=(byte)sw;
        
        //sw=(sword)v_TPS+30;
        sw=(sword)v_TPS+60;
        if(v_TPS_tg>sw){
            v_TPS_tg=sw;
            //deVBAT++;
        }
        //sw=(sword)v_TPS-30;
        sw=(sword)v_TPS-60;
        if(sw<M0_TPS_ST)sw=M0_TPS_ST;
        if(v_TPS_tg<sw){
            //deOS++;
            v_TPS_tg=sw;
        }

}

/*
    根据v_StepAct控制电机运转    
*/
void FC_SM(byte blnIni)
{
    /*2016-9-9 在汽车的怠速提高阀上应用
        v_StepAct为-时，往阀关小的方向开，
        否则，阀开大
    
    */
    
    if(blnIni){
        v_StepAct=0;    
        SM_Position=0;
		
		StepMotorEN1=1; 
        StepMotorEN2=1;
        StepMotorPhase2=0;
        StepMotorPhase1=0;
    } 
	else 
	{
        if((v_StepAct_lst==0)&&(v_StepAct!=0))
		{
			t_stepcon_delay=4;	//2017-11-11 电机状态改变时Delay 20ms,函数调用周期为5ms
		}	

		if(t_stepcon_delay==0)
		{
        if(v_StepAct>0)                  //Act_Step>0 代表电机正转
        {
             v_StepAct--;
             
             if(v_step_sm_tg!=0)v_step_sm++;

             //if(SM_Position>6)SM_Position=0;	//8拍
			 if(SM_Position>2)SM_Position=0;	//4拍
             else SM_Position++;
        }
        else if(v_StepAct<0)           //Act_Step<0 代表电机反转
        {
             v_StepAct++;
             
             if(v_step_sm_tg!=0)v_step_sm--;

             if(SM_Position>0)SM_Position--;
             //else  SM_Position=7;	//8拍
			 else  SM_Position=3;	//4拍
        }
		}
		
		if(t_stepcon_delay>0)	
		{                   
			t_stepcon_delay--;
		}
    }
	
	
	
	if((VF_T1SCANING_INJCTL&&(!VF_MMOVE_ENG)&&VF_1ST_INI_SYSINI))//||(v_StepAct==0))
	 {
		 StepMotorEN1=1; 
         StepMotorEN2=1;
         StepMotorPhase2=0;
         StepMotorPhase1=0;
	 }
	 else
	 {
	/*
	switch(SM_Position)//
    { // 
        case 0: 
                StepMotorEN1=0; //1B            //StepMotorEN=1,为使能关闭
                StepMotorEN2=1;             //StepMotorPhase1=0，电流从OUT1B流向OUT1A
                StepMotorPhase1=0;          //步进电机公共端为高电平
                break;                              //电机通电顺序为A-AB-B-BC-C-CD-D-DA
        case 1: //6
                StepMotorEN1=0; //1B-2B            //详细信息参见PCB板及Microchip MTS62C19A步进电机驱动芯片
                StepMotorEN2=0;              
                StepMotorPhase2=0;
                StepMotorPhase1=0;              
                break;
        case 2: //
                StepMotorEN1=1; //2B
                StepMotorEN2=0;
                StepMotorPhase2=0;
                break;
        case 3: StepMotorEN1=0; //2B-1A
                StepMotorEN2=0;
                StepMotorPhase2=0;
                StepMotorPhase1=1;               
                break;
        case 4: StepMotorEN1=0;  //1A
                StepMotorEN2=1;
                StepMotorPhase1=1;
                break;
        case 5: StepMotorEN1=0; //1A-2A
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=1;               
                break;
        case 6: 
                StepMotorEN1=1;  //2A
                StepMotorEN2=0;
                StepMotorPhase2=1;
                break;
        case 7: 
                StepMotorEN1=0; //2A-1B
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=0;
                break;
        
        default: break;
   
     }
	 */
	 switch(SM_Position)//
    { // 
        case 0: //6
                StepMotorEN1=0; //1B-2B            //详细信息参见PCB板及Microchip MTS62C19A步进电机驱动芯片
                StepMotorEN2=0;              
                StepMotorPhase2=0;
                StepMotorPhase1=0;              
                break;
        case 1: StepMotorEN1=0; //2B-1A
                StepMotorEN2=0;
                StepMotorPhase2=0;
                StepMotorPhase1=1;               
                break;
        case 2: StepMotorEN1=0; //1A-2A
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=1;               
                break;
        case 3: 
                StepMotorEN1=0; //2A-1B
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=0;
                break;

        default: break;
   
     }
	 }
	 
	 v_StepAct_lst=v_StepAct;


}



/*
    设置步进电机的位置
    
    position是目标位置
*/
void Fset_SM(byte position)
{
    sword sw_sm;
    sword sw_exe;



    if(position==255)
		//上电后进行零位自学习
	{
       // Opr_SM(-400);
        v_StepAct=-261;		//-300 2017-11-8 电机最大行程250，避免学零时capnut过多磨损
        v_step_sm=0;   
        v_step_sm_tg=0;
    } 
    else
    {
        if((v_StepAct==0)||(VF_CORADJ&&(!VF_CORADJ_LST)))
        {   //电机停止时才接受新的命令
			//2017-3-10 或者粗调节标志位上升沿触发新命令
            //v_step_sm_tg=v_step_sm;//2016-9-11
      
            sw_sm=position;
			
			if(VF_CORADJ&&(!VF_CORADJ_LST)&&(v_ntcoradj<40))
			{
				v_step_sm+=M0_STOFCORADJ;//2017-3-10 失步引起异常状况时的粗调节
				v_ntcoradj++;
				//xl1+=1;
			}
            //20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
            if((v_TPd>=8)&&(v_TPd<=12))
            {
            	if(sw_sm<(STEP_SM_MIN+STEP_SM_MIN_INCREMENT))sw_sm=(STEP_SM_MIN+STEP_SM_MIN_INCREMENT);
            	else if(sw_sm>STEP_SM_MAX)sw_sm=STEP_SM_MAX;
            }
			else
			{
            	if(sw_sm<STEP_SM_MIN)sw_sm=STEP_SM_MIN;
            	else if(sw_sm>STEP_SM_MAX)sw_sm=STEP_SM_MAX;
			}
			
            sw_exe=sw_sm-v_step_sm;  
      
           // v_step_sm=sw_sm;
            
            v_step_sm_tg=sw_sm;
            
            v_StepAct=sw_exe;   
        }
    }
	
	VF_CORADJ_LST=VF_CORADJ;
}



void IACV_con(void)
{
    sword sw_i;
	sword sw_p;
	//byte tmp_step;
	sword sw_te;
	sword sw_stepinc;
	byte b;
	word w;
	sword step_leavidle;
	sword step_backidle;
	sword sw;
	sbyte sb;
	
	v_stepbase=BYPTR_M0_STEPBASE+v_Adj[ADJ_STEPBASE];//+v_Adj[ADJ_TPDtg];
	v_step_alt=(byte)FG_3DMap_xyz((byte *)BYPTR_M2_MPA_M128,v_TPd,v_Pair_deg,(byte *)BYPTR_TPD_M2LDB,(byte *)BYPTR_PAIR_DEG,15,7,0x00);	
	
	/*
	stw=v_te;//2013-4-30
    if(stw>125)stw=125;
    v_step_IACV_on=F_2DLookUpSbyte((sbyte)stw,BYPTR_M2_IGT,9);
    
    
    stw=(sword)v_step_IACV_on+v_Adj[ADJ_TPDtg];
    
    stw+=Step_inc();
	*/
	
	v_ki_iacv=Adj_byte(BYPTR_M0_KI_IACV,ADJ_KI_IACV);
	v_kp_iacv=Adj_byte(BYPTR_M0_KP_IACV,ADJ_KP_IACV);
	
//--------------------------2016-12-10 计算目标转速与实际转速差异和差异平均------------------------------
	v_dn_iacv=rev15d6_target-(sword)v_rev_15d6rpm;//2017-4-12 恢复
	//v_dn_iacv=v_rev15d6tg_nocorr-(sword)v_rev_15d6rpm;
	
	//转速差异平均,4,3,2,1  与ETC控制类似
	v_dnave_iacv=(v_dn_iacv+v_dn_iacv+v_dn_iacv+v_dn_iacv
                   +v_dn_iacv_lst+v_dn_iacv_lst+v_dn_iacv_lst
                   +v_dn_iacv_lst2+v_dn_iacv_lst2
                   +v_dn_iacv_lst3)/10;			   


				   
//---------------------------------2016-12-10 计算IACV电机目标步数----------------------------------------
   
   //2018-5-19,YSL,去掉初始化条件
   #ifdef SYS_TRADITIONAL
   	if(REMARK==6&&KESEI==0)
   #else
   	if(VFF_SYSNOTINIT_DTC||VFF_ECUNOTINIT_DTC||VF_1ST_INI_SYSINI||(REMARK==6&&KESEI==0))
   #endif
    //if(VFF_SYSNOTINIT_DTC||VFF_ECUNOTINIT_DTC||VF_1ST_INI_SYSINI||(REMARK==6&&KESEI==0))//2018-3-21 初始化或者基础负荷标定模式下步数为标定值
	//if(REMARK==6&&KESEI==0)
	{
		step_pair=(char *)BYPTR_M1_STEPINC_PAIR;
		b=F_LinearInter(*(step_pair+v_pair_odr),*(step_pair+v_pair_odr+1),v_pair_mode,16,false);
		
		//v_step_IACV_on=(byte)(v_stepbase*128/v_step_alt);
		v_step_IACV_on=v_stepbase+b;
		v_steptmp=v_step_IACV_on;
		//v_step_IACV_on=v_stepbase;
	}
	else
	{
	//if((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))	
	//if((((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))||((VF_O2WORK_OSDIAG)&&(VF_O2CHANGED_OSDIAG)))&&(!VFF_SYSNOTINIT_DTC)&&(!VFF_ECUNOTINIT_DTC))
	//if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG)&&(v_t_eng_t_s>12))||VF_CLSMODE_IACV)	//20210929 chenwei 改 杨海环松沙滩车项目，启动要求高怠速不超过3s
	if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG)&&(v_t_eng_t_s>4))||VF_CLSMODE_IACV)		//3改5
	//if((((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))||((VF_O2WORK_OSDIAG)&&(VF_O2CHANGED_OSDIAG)))&&(v_t_eng_t_s>=20))
	//2019-4-25增加时间条件
	//启动成功或OS开始工作进入闭环控制后
	{
		VF_CLSMODE_IACV=true;
		//if(VF_E0_TPS&&(v_rev_15d6rpm<160)&&(!VF_CICORECING_CFCI))
		//if(VF_E0_TPS&&!VF_IGNONLY_CFCI&&(!VF_CICORECING_CFCI)&&(v_t_afterbackidle_d1s>=30)&&(!VF_CORADJ))
		if(VF_E0_TPS&&!VF_IGNONLY_CFCI&&(!VF_CORADJ))
		//if(VF_E0_TPS&&(v_rev_15d6rpm<160)&&(!VF_CORADJ))
			//2017-2-23 在油门回怠速，但转速高的状态下不计算
			//2017-3-6 回油门处于再给油修正阶段，不计算
			//2017-3-7 油门回怠速2~3s时间以内不计算-逗车未断油
			//2019-3-25 去掉转速高的条件，增加断油条件
			//2019-4-25 去掉再给油条件和有门回怠速条件
		{
			/*
			sw=v_dnave_iacv;

			if(v_dnave_iacv<0)
			{
				sw=-v_dnave_iacv;
			}
			*/
			sb=(sbyte)(v_ign_stptg-10);
			if(sb<v_CA_limit_byrpm)sb=v_CA_limit_byrpm;
			if((v_CA_ignition>sb)&&(v_CA_ignition<v_ign_stptg))
			{
				VF_IGN_SUIT=true;
			}
			else
			{
				VF_IGN_SUIT=false;
			}
			
			
			
			//转速波动超过15.6*3=46.2rpm后才调节步进电机
			//2017-3-6 注意当气量偏小引起指示灯闪烁时，是否需要处理？？？？
			if((v_dnave_iacv>3)||(v_dnave_iacv<-8))//2020-12-12 -6改为-8，超过140.4rpm才能调节
			{
				//比例控制
				sw_p=v_dnave_iacv-v_dnave_iacv_lst;
				

				if(sw_p>10)sw_p=10;
				if(sw_p<-10)sw_p=-10;
				sw_p=(sword)(sw_p*(sword)v_kp_iacv);				
				
				//2017-4-12 回油门后，怠速目标修正期间，减弱比例项作用
				//2019-4-12 点火目标控制期间减弱比例项作用
				if((inc_rev_backidle>0)||VF_STEP_IGNCO)
				{
					sw_p=sw_p/8;
				}
				//xl4=sw_p;
				
				v_dnave_sigma_p+=sw_p;
		
				sw_stepinc=0;
				while(v_dnave_sigma_p>(sword)M0_DNSTEP_KP)
				{
					v_dnave_sigma_p-=M0_DNSTEP_KP;
					sw_stepinc++;
				}
				while(v_dnave_sigma_p<-(sword)M0_DNSTEP_KP)
				{
					v_dnave_sigma_p+=M0_DNSTEP_KP;
					sw_stepinc--;
				}
		
				
				sw_p=sw_stepinc;
				/*if((sw_stepinc!=0)&&((v_dnave_iacv>15)||(v_dnave_iacv<-15)))
				{
					sw_p=sw_stepinc+v_dnave_iacv/4;
				}*/
				
				v_steptmp_p=sw_p;
				
				
				//积分控制
				sw_i=v_dnave_iacv;
				
				if(sw_i>8)sw_i=8;
				if(sw_i<-6)sw_i=-6;
				sw_i=(sword)(sw_i*(sword)v_ki_iacv);
				
				//2017-4-12 回油门后，怠速目标修正期间，减弱积分项作用
				//2019-4-12 点火目标控制期间减弱积分项作用
				//2019-4-25 怠速提供阶段结束后一段时间内积分加快
				
				if((inc_rev_backidle==0)&&(v_t_afterbackidle_d1s<100))
				{
					sw_i=sw_i*8;//加快积分项作用试一下
				}				
				else if(VF_STEP_IGNCO||VF_IGN_SUIT)//(inc_rev_backidle>0)||
				{
					sw_i=sw_i/16;
				}
				

				
				v_dnave_sigma_i+=sw_i;
			
				sw_stepinc=0;
				while(v_dnave_sigma_i>(long int)M0_DNSTEP_KI)
				{
					v_dnave_sigma_i-=M0_DNSTEP_KI;
					sw_stepinc++;
				}
				while(v_dnave_sigma_i<-(long int)M0_DNSTEP_KI)
				{
					v_dnave_sigma_i+=M0_DNSTEP_KI;
					sw_stepinc--;
				}
				v_steptmp_i=sw_stepinc;

			
				//2017-3-9 PI控制结果			
				v_steptmp=v_steptmp+v_steptmp_p+v_steptmp_i;
				
				//20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
				 if((v_TPd>=8)&&(v_TPd<=12))
				 {
				 	if(v_steptmp<((sword)STEP_SM_MIN+STEP_SM_MIN_INCREMENT))v_steptmp=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
				 }
				 else
				 {
					if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
				 }
				if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;
				
				
				//v_step_idle=v_steptmp;
			}
		}
	}
	else
	//启动阶段
	//开环控制阶段
	{
		
		//if(VF_STARTS_ENG||!VF_MMOVE_ENG)//停机及启动阶段
		if(((VF_STARTS_ENG&&v_rev_625rpm<25)||!VF_MMOVE_ENG)&&!VF_OPENLOOP_IACV)//停机及启动阶段	//20210929 chenwei add &&v_rev_625rpm<25
		{
			sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			//tmp_step=(byte)FG_3DMap_xyz(BYPTR_M2_STEPINI,v_rev_15d6rpm,sw_te,BYPTR_N_M2STEPINI,BYPTR_TE_M2STEPINI,7,7,false);
			//tmp_step=(byte)FG_3DMap_xyz(BYPTR_M2_STEPINI,v_rev_15d6rpm,sw_te,BYPTR_N_M2STEPINI,BYPTR_TE_M2STEPINI,1,3,false);//2018-3-21 MAP维度调整为4X4
			v_stepini=(sword)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);//(sword)F_2DLookUpSbyte((sbyte)sw_te,BYPTR_M1_STEPINI,3); 
			v_steptmp=v_stepini;
			v_step_bcidlimt=v_stepini;
			
		}
		else//启动成功后的开环控制阶段
		{
			
			VF_OPENLOOP_IACV=true;		//20210929 chenwei add
			v_stepdec_fact=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEPDEC),2);
			//if(v_stepdec_fact>64)v_stepdec_fact=64;	//20210929 chenwei 屏蔽，放开到100
			if(v_stepdec_fact>100)v_stepdec_fact=100;
			
			//w=(word)v_t_eng_d1s;
			//if(b>250)b=250;
			w=(word)v_stepdec_fact*v_t_afterstarts/64;//v_t_eng_d1s
			//if(w>50)w=50;		//20210929 chenwei 屏蔽，放开到80
			if(w>80)w=80;		
			v_steptmp=v_stepini-(sword)w;
			
			//20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
			if((v_TPd>=8)&&(v_TPd<=12))
			{
				if(v_steptmp<(sword)(STEP_SM_MIN+STEP_SM_MIN_INCREMENT))v_steptmp=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
			}
			else
			{
				if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
			}
			
			if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;
		}
		
		//高原修正
		/*b=14745/(v_step_alt);//15564=128*128*0.95  14745:0.9
		if(b<128)b=128;
		v_steptmp=(sword)(v_steptmp*b/128);*///2018-6-28 高原实验发现修正后气量太大，按照高原修正系数的95%再进行修正
		//2020-10-15 调整高原修正
		step_pair=(char *)BYPTR_M1_STEPINC_PAIR;
		b=F_LinearInter(*(step_pair+v_pair_odr),*(step_pair+v_pair_odr+1),v_pair_mode,16,false);
		v_stepco_pt=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEPCO_PT),3);
		
		v_stepco_pt=(word)v_stepco_pt*b/50;
		
		v_steptmp=v_steptmp+v_stepco_pt;
		
		
		//if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
		//if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;
		
		//闭环控制前的稳定怠速步数初值
		v_step_idle=v_steptmp;
		

	}
    
	//stw=(sword)v_step_IACV_on+(sword)v_Adj[ADJ_TPDtg];

		//v_step_idle=v_step_IACV_on;
		//
		
	}
	
	
	
//------------------------------2019-3-25 针对离开怠速和回到怠速的处理---------------------------------

	step_leavidle=(sword)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEINC_LIDLE),1);
	step_backidle=(sword)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEINC_BIDLE),1);
	
	if(VF_MMOVE_ENG&&!VF_STARTS_ENG&&(v_t_eng_t_s>10)&&VF_NEWCYCLE_ATMAIN_CAS)
	{				
		w=(word)v_step_idle+50;
		if(w>STEP_SM_MAX)w=STEP_SM_MAX;
		
		//过渡增加步数
		if(!VF_E0_TPS&&VF_STEP_TRANS)
		{		
			if(v_t_afterbackidle_d1s>100)
			{
				v_step_idle=v_step_IACV_on;
			}
		
			if((word)v_steptmp<w)//最大增加步数不超过50
			{
				if(v_t_afterbackidle_d1s>30)
				{
					if(v_dnave_iacv>=-6)
					{
						v_steptmp=v_steptmp+step_leavidle;	
					}
					else
					{
						v_steptmp=v_steptmp+step_leavidle/4;
					}
					
					if((word)v_steptmp>w)v_steptmp=w;									
				}
				
			}					
			VF_STEP_TRANS=false;
			VF_STEP_STEADY=false;
		}
	/*
		if(VF_E0_TPS&&VF_NEWCYCLE_ATMAIN_CAS&&VF_STEP_TRANS)
		{

			if((word)v_steptmp<w)//最大增加步数不超过50
			{
				
				if(v_rev_625rpm>=REV62D5_6000)
				{
					v_steptmp=v_steptmp+step_backidle;
				}
				else if(v_rev_625rpm>=REV62D5_4500)
				{
					v_steptmp=v_steptmp+step_backidle/2;//6
				}
				else if(v_rev_625rpm>REV62D5_3000)
				{
					v_steptmp=v_steptmp+step_backidle/6;
				}
				
				if((word)v_steptmp>w)v_steptmp=w;	
				
			}		
			VF_STEP_TRANS=false;
		}*/
		
		if(VF_E0_TPS&&VF_STEP_TRANS)
		{
			if((word)v_steptmp<w)//最大增加步数不超过50
			{
				
				if(v_rev_625rpm>=REV62D5_6000)
				{
					v_step_bidle=step_backidle;
				}
				else if(v_rev_625rpm>=REV62D5_4500)
				{
					v_step_bidle=step_backidle/2;//6
				}
				else if(v_rev_625rpm>REV62D5_3000)
				{
					v_step_bidle=step_backidle/6;
				}
				
				//if((word)v_steptmp>w)v_steptmp=w;	
				
			}		
			VF_STEP_TRANS=false;
			VF_STEP_STEADY=false;  //20210129 chenwei add
		}
		
		if(VF_JCI_CFCI&&VF_E0_TPS&&(v_TPS_mode<=7))//2019-4-26 动油门时候tpd为0，但是tps_mode较大，已经再给油了
		{
			if((word)v_steptmp<w)v_steptmp+=v_step_bidle;
			if((word)v_steptmp>w)v_steptmp=w;
		}
	
		//针对增加的步数的衰减
		if(!VF_STEP_STEADY&&!VF_IGNONLY_CFCI)//2019-4-25 晚 断油期间不减
		{
			sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			v_step_bcidlimt=(byte)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);
			v_step_bcidlimt+=(byte)v_stepco_pt;   //20210402 chenwei 高原标定发现v_step_bcidlimt按照M1_STEPINI查找，但原先未加上高原修正
			if(v_step_bcidlimt>v_step_idle)v_step_bcidlimt=v_step_idle;
			
			if(v_steptmp>(v_step_bcidlimt+30))//大于稳定步数30步快速衰减
			{

				if(VF_RUN8_TMCCL)
				{
					v_steptmp--;
				}
				
				if(VF_RUN2_TMCCL&&(v_dn_iacv<=-8))//转速高于目标100r加快衰减
				{
					v_steptmp--;
				}

			}
			else if(VF_E0_TPS)
			{
				if((v_steptmp>=(v_step_bcidlimt+10))||((v_steptmp>v_step_bcidlimt)&&(v_dn_iacv<=-8)&&(v_t_d1s_afterCI>10)))
				{
					if(VF_RUN16_TMCCL)
					{
						v_steptmp--;
					}
								
					if((v_rev_625rpm<REV62D5_3000)&&VF_RUN4_TMCCL&&(v_dn_iacv<-12))//针对小油门逗车的情况，回怠速时候步数增加不多，需要加快步数衰减
					{
						v_steptmp--;
					}
				}
				
				if(((v_steptmp<(v_step_bcidlimt+10))&&(v_dn_iacv>-8))||((v_t_afterbackidle_d1s>150)&&(v_dn_iacv>-8)&&(v_dn_iacv<5))||(v_steptmp<=v_step_bcidlimt))
				//判断怠速电机步数稳定的条件：1：步数达到加速前稳定步数+10,并且转速不高于目标转速100rpm  或 2：回怠速时间大于15s，转速再目标转速-80~120之间
				{
					VF_STEP_STEADY=true;
				}
			}			
		}

		//if(v_t_afterbackidle_d1s<50)
		if((v_t_afterbackidle_d1s<100)||(v_t_d1s_afterCI<80))	//2021-1-27 增加再给油的时间条件，避免滑行后步数减小太多
		{
		/*	sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			v_step_bcidlimt=(byte)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);
			if(v_step_bcidlimt>v_step_idle)v_step_bcidlimt=v_step_idle;//2021-1-28 取stepini和stepidle小的那个作为限制*/
			//if(v_t_afterbackidle_d1s<30)
			if((v_t_afterbackidle_d1s<80)||(v_t_d1s_afterCI<50))	//2021-1-27 增加再给油的时间条件，避免滑行后步数减小太多
			{
				//if(v_steptmp<v_step_idle)v_steptmp=v_step_idle;
				if(v_steptmp<v_step_bcidlimt)v_steptmp=v_step_bcidlimt;
			}
			else
			{
				//sw=(sword)v_step_idle-10;
				sw=(sword)v_step_bcidlimt-10;
				//if(sw<10)sw=10;
				if(sw<20)sw=20;//2021-1-27 最小步数设定为20
				if(v_steptmp<sw)v_steptmp=sw;
				//if((v_steptmp<v_step_idle)&&(v_dn_iacv>-20))v_steptmp=v_step_idle;
				
			}
			
			//if((v_t_afterbackidle_d1s<15)&&(v_steptmp<(v_step_idle+10)))
		}

		//20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
        if((v_TPd>=8)&&(v_TPd<=12))
        {
        	if(v_steptmp<(sword)(STEP_SM_MIN+STEP_SM_MIN_INCREMENT))v_steptmp=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
        }
		else
		{
			if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
		}
		
		if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;

		
	}
	


/*--------------------------------------根据目标点火角对步数的控制--------------------------------------------
策略：
1、设定点火目标为：角标后沿-5
2、如果点火角达到最大或最晚点火限制，持续超过15s，调节步数，直到达到目标点火角

进行目标点火角控制的条件：
1、发动机运行超过1min
2、避开油门回怠速期间
3、点火和步数都达到最小(但是转速偏高的情况)不应该控制；点火和步数都达到最大(但是转速偏低的情况)不应该控制

2020-11-27 调整策略
1、以v_Ign_Map+dIgn作为怠速目标点火角
2、将co_IdleIgn_I作为怠速步进电机步数增减的依据

*/

if(VF_CLSMODE_IACV&&VF_NEWCYCLE_ATMAIN_CAS)
{
	
	//if(VF_1S_TMCCL&&(v_t_ignlim<255))v_t_ignlim++;

	#ifdef	TE_10K	//20210728 chenwei add 10K缸温
	if(VF_E0_TPS&&VF_MORE1MIN_TMCCL&&!VF_CICORECING_CFCI&&(v_t_afterbackidle_d1s>=100)&&(v_te>60))
	#endif	

	#ifdef	TE_2K	//20210728 chenwei add 2K水温
	if(VF_E0_TPS&&VF_MORE1MIN_TMCCL&&!VF_CICORECING_CFCI&&(v_t_afterbackidle_d1s>=100)&&(v_te>45))
	#endif
	{
		//test0413++;
		/*if(((v_CA_ignition==BYPTR_M0_IGNMAX_IDLE)&&(v_step_sm<(sword)STEP_SM_MAX))||((v_CA_ignition==v_CA_limit_byrpm)&&(v_step_sm>(sword)STEP_SM_MIN)))
		{
			if(VF_1S_TMCCL&&(v_t_ignlim<255))v_t_ignlim++;
		}
		else
		{
			if(VF_1S_TMCCL&&(v_t_ignlim>0))v_t_ignlim--;
		}
		
		if(v_t_ignlim>50)
		{
			VF_STEP_IGNCO=true;
		}
	
		if(VF_STEP_IGNCO&&VF_2S_TMCCL&&(v_step_ignco<=15))
		{
			if(v_CA_ignition>v_ign_stptg)v_steptmp++;//点火角大于目标点火角，通过增加气量来使点火降下来
			if(v_CA_ignition<v_ign_stptg)v_steptmp--;
			
			if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
			if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;
			
		
			v_step_ignco++;
		
		}
		else if((v_step_ignco>15)||(VF_STEP_IGNCO&&(v_CA_ignition<=v_ign_stptg+2)&&(v_CA_ignition>=v_ign_stptg-2)))
		{
			v_step_ignco=0;
			VF_STEP_IGNCO=false;
			v_t_ignlim=0;
		}*/
		
		if((co_IdleIgn_I>6)||(co_IdleIgn_I<-12))//&&(v_step_ignco<=20)&&(v_t_ignlim>=60)最大20步,超过20步后，需要间隔1min才可以再调节
		{
			if((co_IdleIgn_I>6)&&VF_8S_TMCCL)
			{
				v_steptmp++;//点火角大于目标点火角，通过增加气量来使点火降下来
				v_step_ignco++;
			}
			
			if((co_IdleIgn_I<-12)&&VF_8S_TMCCL)
			{
				v_steptmp--;
				v_step_ignco--;
			}
			
			
			//v_step_ignco++;

			//20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
        	if((v_TPd>=8)&&(v_TPd<=12))
        	{
        		if(v_steptmp<(sword)(STEP_SM_MIN+STEP_SM_MIN_INCREMENT))v_steptmp=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
        	}
			else
			{
				if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
			}
			
			if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;		
		}
		
		/*if(v_step_ignco>20)
		{
			v_step_ignco=0;
			v_t_ignlim=0;
		}		*/
				
	}
	else
	{
		v_step_ignco=0;
		//VF_STEP_IGNCO=false;
	}

}

	

//-------------------------------------------------END------------------------------------------------------

	
	
	v_step_IACV_on=v_steptmp+v_Adj[ADJ_TPDtg];

	//20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
    if((v_TPd>=8)&&(v_TPd<=12))
    {
    	if(v_step_IACV_on<(STEP_SM_MIN+STEP_SM_MIN_INCREMENT))v_step_IACV_on=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
    }
	else
	{
		if(v_step_IACV_on<STEP_SM_MIN)v_step_IACV_on=STEP_SM_MIN;
	}
	
	if(v_step_IACV_on>STEP_SM_MAX)v_step_IACV_on=STEP_SM_MAX;
    
    /*
    if(REMARK==6&&KESEI==0) 
    {
        v_step_IACV_on=(byte)(v_stepbase*128/v_step_alt);
		//v_step_IACV_on=v_stepbase;   
    }
	*/
    
//------------------------------------2016-12-12 IACV步数与气量的关系------------------------------------        
    //需要判断什么情况下不需要开阀
    //v_t_IACV_on=F_2DLookUpSbyte((sbyte)v_step_sm,BYPTR_M2_IGC,7);
	v_t_IACV_on=(sbyte)FG_3DMap_xyz((byte *)BYPTR_M2_STEPLAMBDA,v_step_sm,v_rev_15d6rpm,(byte *)BYPTR_STEP_M2LAMBDA,(byte *)BYPTR_N_M2LAMBDA,7,1,0x80);
	
	v_t_IACV_on=(sword)(v_t_IACV_on*v_step_alt)/128;
	
	
	//2018-6-27 高原实验补偿偏浓，增加高原修正
	/*if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))||(V_Ta<0))
	{
		v_t_IACV_on=(sword)(v_t_IACV_on*v_step_alt)/128;
	}
	else if(v_Pair_deg>32)
	{
		v_t_IACV_on=0;//常温启动阶段,高原2000m以上暂时不补偿油量
	}*/
	


    //因为定义nstand4模式为,不使用修正
    if(REMARK==10&&KESEI==5)
    {
        v_t_IACV_on=0;    
    }
    else
    {
        v_Adj[ADJ_TPDtg]=0;
    }
	
	
	v_dn_iacv_lst3=v_dn_iacv_lst2;
	v_dn_iacv_lst2=v_dn_iacv_lst;
	v_dn_iacv_lst=v_dn_iacv;
	v_dnave_iacv_lst=v_dnave_iacv;
    
    
}

/*2016-11-19 该函数返回值有问题，删除
//IACV对应的LD的增量
byte Ldinc_byIACV(void)
{
    //return v_step_IACV_on/2;//
    //需要进行的油量补偿
    
        
    return v_t_IACV_on;    
}
*/


/*2016-11-19 升级策略后，当有高怠速需求时，会根据目标怠速计算步进电机步数，故删除该函数
    根据目标转速，提到怠速气量

byte Step_inc(void)
{
    sword sw;
    
    
    if(rev15d6_target>v_n0_low)sw=(sword)rev15d6_target-v_n0_low;
    else sw=0;
    
    sw=sw+sw;
    
    if(sw>40)sw=40;
    
    
    return (byte)sw;

}*/
     
//不转


/*
    switch(SM_Position)//
    { // 
        case 0: //6
                StepMotorEN1=0; //1B-2B            //详细信息参见PCB板及Microchip MTS62C19A步进电机驱动芯片
                StepMotorEN2=0;              
                StepMotorPhase2=0;
                StepMotorPhase1=0;              
                break;
        case 1: StepMotorEN1=0; //2B-1A
                StepMotorEN2=0;
                StepMotorPhase2=0;
                StepMotorPhase1=1;               
                break;
        case 2: StepMotorEN1=0; //1A-2A
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=1;               
                break;
        case 3: 
                StepMotorEN1=0; //2A-1B
                StepMotorEN2=0;
                StepMotorPhase2=1;
                StepMotorPhase1=0;
                break;

        default: break;
   
     }
*/

void F_CORADJ_IACV(void)
{
	/*
	  粗调节的情况：
	  1.严重高怠速：反复出现断油再给油时进行步进电机粗调节
	  2.轻微高怠速：实际步数已经最小，但仍存在非正常高怠速:|目标-实际|>一定阀值
	  
	  判断：
	  1.发生严重高怠速时（断油--再给油--断油，反复出现），需要立即执行粗调节功能；
	  2.轻微高怠速时，每隔几秒执行一次粗调节，直到回复正常怠速；
	  3.对粗调节次数进行限制,但要保证一个驾驶循环粗调节的步数大于电机能调节的最大步数范围
	  */

//---------------------------------------------状态计算------------------------------------------------------	  
	  //发动机运行，怠速且无节气门位置故障时，置位粗调节激活标志
	  if(VF_MMOVE_ENG&&VF_E0_TPS&&(!VFF_TPS_DTC)&&(v_t_afterbackidle_d1s>=100))
		  //2019-3-25 油门稳定运行10s以上，粗调节都增加步数最小条件
	  {
		  VF_CORACT=true;
	  }
	  else
	  {
		  VF_CORACT=false;
	  }
	  
	  //粗调节功能激活，记录发生断油的状态	  
	  if(!VF_CORACT)
	  {
		  VF_CFST=false;
	  }
	  else if(VF_IGNONLY_CFCI)
	  {
		  VF_CFST=true;
	  }
	  
	  //粗调节功能激活，记录发生一次断油后再给油的状态	  
	  if(!VF_CORACT)//||(VF_IGNONLY_CFCI&&VF_CIRECINJST))
	  //if(!VF_CORACT)
	  //2017-4-10 增加条件：当发生一次再给油后，并且当前为断油时，需要复位状态
	  {
		  VF_CIRECINJST=false;
	  }	  
	  else if(VF_CFST&&(!VF_IGNONLY_CFCI))
	  //else if(VF_CICORECING_CFCI)
	  //2017-4-10  VF_CICORECING_CFCI作为再给油判断时有问题，条件变更为：当发生一次断油，且当前为非断油状态
	  {
		  VF_CIRECINJST=true;
	  }
	  
	  //判断轻微高怠速
	  if(VF_CORACT&&(v_step_sm==STEP_SM_MIN)&&(v_dnave_iacv<-(sword)M0_DNCORSTP))
	  {
		  if(VF_1S_TMCCL)
		  {
			  v_t_corstp_s++;
		  }
	  }
	  else
	  {
		  v_t_corstp_s=0;
	  }
	  
//------------------------2019-3-25 针对回调较慢的严重高怠速粗调节--------------------------------------------
	  if(!VF_CORADJ&&VF_IGNONLY_CFCI&&VF_CFST&&VF_CIRECINJST&&(v_step_sm>STEP_SM_MIN))
	  {
		  v_step_IACV_on=v_step_IACV_on-M0_STOFCORADJ;
		  
		  VF_CFST=false;
		  VF_CIRECINJST=false;
		  //20210331 chenwei add 增加油门微开，TPD0.2、0.3的对对最小步数的限值
    	  if((v_TPd>=8)&&(v_TPd<=12))
    	  {
    	  	if(v_step_IACV_on<STEP_SM_MIN+STEP_SM_MIN_INCREMENT)v_step_IACV_on=STEP_SM_MIN+STEP_SM_MIN_INCREMENT;
    	  }
		  else
		  {
		  	if(v_step_IACV_on<STEP_SM_MIN)v_step_IACV_on=STEP_SM_MIN;
		  }
		  if(v_step_IACV_on>STEP_SM_MAX)v_step_IACV_on=STEP_SM_MAX;
		  
	  }

//------------------------ 针对电机步数已最小但仍然高怠速（失步）的粗调节-------------------------------------	  
	  //判断是否需要粗调节
	  if(VF_CORADJ&&(v_StepAct==0))
	  {
		  //粗调节完成后，复位标志。无异常情况，步进电机执行PI控制。
		  VF_CORADJ=false;
		  v_t_corstp_s=0;
		  VF_CFST=false;
		  VF_CIRECINJST=false;
	  }
	  else if((VF_IGNONLY_CFCI&&VF_CFST&&VF_CIRECINJST&&(v_step_sm==STEP_SM_MIN))||(v_t_corstp_s>M0_TCORSTP)) 	  
	  {
		  //粗调节的情况：严重高怠速或轻微高怠速超过一定时间
		  VF_CORADJ=true;
		  VF_CFST=false;
		  VF_CIRECINJST=false;
	  }
}

/********************************************************************
*
*步进电机3步停1步
*
*StepMotor_ST：步进电机步数状态
*StepMotortimer：步进电机换项定时器
*M_StepMotorPowerTime：步进电机换项时间
*F_L_R：步进电机运动方向：0：开启  1：关闭
*
********************************************************************/
void F_StepMT_3T1(void)
{
   
	if(StepMotorStep == 3) 
	{	
	    StepMotorStep = 0;		
	    return;
	}
        
    if(SM_Position>0)SM_Position--;
	else  SM_Position=3;

    if((VF_T1SCANING_INJCTL&&(!VF_MMOVE_ENG)&&VF_1ST_INI_SYSINI))//||(v_StepAct==0))
	{
		 StepMotorEN1=1; 
         StepMotorEN2=1;
         StepMotorPhase2=0;
         StepMotorPhase1=0;
	}
	else
	{
		switch(SM_Position)
		{
			case 0: //6
					StepMotorEN1=0; //1B-2B            //详细信息参见PCB板及Microchip MTS62C19A步进电机驱动芯片
					StepMotorEN2=0;              
					StepMotorPhase2=0;
					StepMotorPhase1=0;
				
					StepMotorStep++;				
					break;
			case 1:               
					StepMotorEN1=0; //2B-1A
					StepMotorEN2=0;
					StepMotorPhase2=0;
					StepMotorPhase1=1;
				
					StepMotorStep++;				
					break;
			case 2: 
					StepMotorEN1=0; //1A-2A
					StepMotorEN2=0;
					StepMotorPhase2=1;
					StepMotorPhase1=1;
				
					StepMotorStep++; 
					break;
			case 3: 
					StepMotorEN1=0; //2A-1B
					StepMotorEN2=0;
					StepMotorPhase2=1;
					StepMotorPhase1=0;
				
					StepMotorStep++;
					break;        

			default: break;
		}
	}
	
	v_StepAct++;
	if(v_step_sm_tg!=0)v_step_sm--;
}



/********功能：环松氧加热条件策略**********
*********时间：2021.10.10*******************
*********使用：chenwei**********************/
void F_O2H_FORHUANSONG(void)
{
	
	if(VF_MMOVE_ENG)
	{
		byte b_te;
		b_te=v_te+40;
		v_t_O2H_delay_1s=(byte)F_2DLookUp(b_te,BYPTR_M1_T_O2H_DELAY,7,0,false);
		v_o2h_n_625=(byte)F_2DLookUp(b_te,BYPTR_M1_SPEED_O2H_THRESH,7,0,false);
		v_o2h_tpd=(byte)F_2DLookUp(b_te,BYPTR_M1_TPD_O2H_THRESH,7,0,false);
		
		//if((v_t_EngMove_s>v_t_O2H_delay_1s)&&(v_rev_625rpm<=v_o2h_n_625)&&(v_TPd<=v_o2h_tpd))
		if((v_t_EngMove_s>v_t_O2H_delay_1s))		  
		{
			//20211014 chenwei 小负荷也加热（3500，5以下）
			if(((v_rev_625rpm<=v_o2h_n_625)&&(v_TPd<=v_o2h_tpd))||((v_rev_625rpm<56)&&(v_TPd<80)))
			{
				P_O2H_ON;
				VF_O2H_STATUS=true;
			}
			else
			{
				P_O2H_OFF;
				VF_O2H_STATUS=false;
			}
		}
		else
		{
			P_O2H_OFF;
			VF_O2H_STATUS=false;
		}
	}
	else
	{
		P_O2H_OFF;
		VF_O2H_STATUS=false;
	}
}