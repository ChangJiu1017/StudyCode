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
 *		�������Ƶ�ʰ���¡��Ҫ�����Ϊ200HZ
 * V8.2.3	2018.3.23	xielian
 *		�����Ż�
 * V8.2.4 2018.5.19 yangsili
 *    ȥ����ʼ������
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
sbyte v_nbStep_DC;//����PID�õ�����Ҫִ�еĽǶȣ�

sbyte v_dnb_byIgn; //���ǶԵ�����˶�������

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
//sword v_d_rev_to_tg_lst5 ;//lcw2013-2-1    2013-2-4 Ϊ��ʡRAM��ɾ�� 

sword v_d_AVrpm_to_tg    ;//2013-1-20ƽ��ת����Ŀ��Ĳ�ֵ
sword v_d_AVrpm_to_tg_lst    ;//2013-1-20��ƽ��ת����Ŀ��Ĳ�ֵ
sword v_d_AVrpm_to_tg_lst2    ;//2013-1-20ƽ��ת����Ŀ��Ĳ�ֵ
sword v_d_AVrpm_to_tg_lst3   ;//2013-1-20��ƽ��ת����Ŀ��Ĳ�ֵ


sbyte v_acc_rev;//���ٶ�//2012-11-15
sbyte v_acc_rev_lst;//��һ�μ��ٶ�//2012-11-17
 
byte VF_STA_LD ;

byte v_ccl_StartsProcess;

byte v_step_formin; //�����ſ�����С���ۻ��Ĳ�����

//2011-11-19 i detection
byte v_I_ld;
byte v_I_ld_lst;
byte v_I_ld_lst2,v_I_ld_lst3,v_I_ld_lst4;

byte v_I2_ld,v_I2_ld_lst;
byte v_I2_ld_lst2,v_I2_ld_lst3,v_I2_ld_lst4;


byte v_I_ini[2];
byte v_I_ini_lst[2];          //2012-11-15 ��һ�β�����ʼֵ��
byte v_I_ini_lst2[2];          //2013-1-13 ͷ���β�����ʼֵ



byte v_I_stable_lst;//��һ���ȶ�������ƽ��ֵ��
byte v_I_stable;    //�ȶ�����ֵ 
byte v_I_lst;//��һ�ε���ֵ��
byte v_I;    //����ֵ 



byte v_TPS_I;//2012-11-18 �и��ص�������õ���TPSֵ��
byte v_TPS_I_lst;//2012-11-18 �и��ص�������õ���TPSֵ��

byte v_TPS_I_stable;//2013-1-27ƽ����������õ���TPSֵ��
byte v_TPS_I_stable_lst;//2013-1-27�ϴ�ƽ����������õ���TPSֵ��

byte v_ccl_Istable; //��������ȶ������ڵ�Ȧ����
//byte ccl_disableN;// ��ʹ��ת�ٿ��Ƶ�Ȧ����

byte  st_I_Not_cap   ;//LCW����1û�в����״̬
byte  st_I2_Not_cap   ;//LCW����2û�в����״̬
byte  st_tps_tg_to_max  ;//lcw2013-1-15Ŀ��TPS�ﵽ���ֵ��״̬
byte  st_tps_tg_to_min  ;//lcw2013-1-15Ŀ��TPS�ﵽ��Сֵ��״̬ 
byte  st_DC_ACC_8       ;//DCȫ���ٵ�״̬
byte  st_DC_DEC_8       ;//DCȫ���ٵ�״̬             



byte v_EngineOil;

word v_rev3d9_tg;

byte v_TPS_st;//����ʱ����λ��

byte  n_nb_I ;
      
byte v_ld0[2];//2012-10-12, ����Ϊ0ʱ�Ĳ���ֵ��ͣ��ʱ

sbyte drev_tg_i;// ת����Ŀ�����Ļ��֣�


//2013-1-12 lcw
sword drev_tg_rp;     //ת��ƫ��������Ƶ��ۼ���;
//sword drev_tg_ri;     //ת��ƫ����ֿ��Ƶ��ۼ���;
long int  drev_tg_ri;     //ת��ƫ����ֿ��Ƶ��ۼ���;
//sword drev_tg_rd;     //ת��ƫ��΢�ֿ��Ƶ��ۼ���;
long int drev_tg_rd;     //ת��ƫ��΢�ֿ��Ƶ��ۼ���;


sbyte v_nb_DC;      //DC ON��ʱ��Ϊ�v_nb_DC/nb_RTI*����  //2012-11-14 from byte to sbyte
sbyte v_nb_DC;      //DC ON��ʱ��Ϊ�v_nb_DC/nb_RTI*����  //2012-11-14 from byte to sbyte

sbyte v_nb_DCPrd;   //DC���Ƶ�������

sbyte v_nb_step_pre;    //2013-1-13 LCW ���ڵ����Ӽ����ж�Ϊ�棬Ԥ�����������ƶ��Ĳ���
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
//�������״̬
#define VF_TOTPSE0_STALD  0
#define VF_TOTPSMIN_STALD 1
#define VF_SETINIT_STALD 2
#define VF_TPS_READY    3
#define VF_STEA_STALD   4
#define VF_TRAN_STALD   5

// �����ȶ�״̬���ж�
#define M0_REV_TH_STEADY 25//22//15//22     //3.9rpm
#define M0_ACC_TH_STEADY 14//10//6//8    //10

#define M0_CCL_LAG  4//������Ҫ�ӳٵ�ѭ����
#define M0_CCL_LAG_FOR_SM  16
#define MO_dTPS_TRAN 15

#define M0_DIGN_TH_ECT_P 20     //10
#define M0_DIGN_TH_ECT_N -30     //10

#define M0_DIGN_TH_ECT_MAX_P 8 //20
#define M0_DIGN_TH_ECT_MAX_N -8     //10

#define M0_THRE_LD_OPEN_RUN 250   //����������ʱ����������ֵ���ڸ�ֵ����ʾ��������·
#define M0_THRE_LD_OPEN_STOP 180   //������ͣ��ʱ����������ֵ���ڸ�ֵ����ʾ��������·

#define M0_THRE_LD_ZERO 130   //Ĭ�ϵĵ���Ϊ0��ֵ�� //2013-1-25 120 to 130
/*-map to be open-------------------------------------------------------------------*/
//#define M0_TPS_ST 55     //����ʱ��������λ�� //2012-11-15 from 45 to 55
//BS--55, GENERAC--45

//ÿȦ���Զ��������Ĳ���
#define M0_STEPS_MAX_DC 16
#define M0_STEPS_MAX_DC_trip 150   //2013-1-19


#define M0_STEPS_MAX_SM 13
#define STEP_LEAVEIDLE	12
#define STEP_BACKIDLE	30


// ʹ��v_KP_M8��ʱ�Ĳ���
#define M0_DRPM_MAX_pDC 60       //��������ʱһѭ���ڵ�ת��ƫ��仯���ֵ������40*3.9
//#define M0_DAVRPM_MAX_pDC 120    //2013-1-16��������ʱ,3ѭ���ڵ�ת��ƫ��仯���ֵ������100*3.9

#define M0_DRPM_MAX_iDC 30  //2013-1-22 80        //���ֿ���ʱ��ת�����ƫ��������96*3.9:374rpm
//#define M0_DRPM_MAX_dDC 50       //΢�ֿ���ʱ��ת��ƫ��仯�Ĳ�ֵ���ƫ������30*3.9
//2015-12-27 CHANGE TO 100
#define M0_DRPM_MAX_dDC 100       //΢�ֿ���ʱ��ת��ƫ��仯�Ĳ�ֵ���ƫ������30*3.9

//#define M0_DREV_TG_KP_ASTEP 256 //2013-1-30ʹ��v_KP_M8��v_KP_M8*drpm/256,256����һ��
#define M0_DREV_TG_KP_ASTEP 256 //2015-12-27 ����Ϊ128
    
      
#define M0_DREV_TG_KI_ASTEP  3200 //2013-1-30 *v_KI_M8/3200
//#define M0_DREV_TG_KI_ASTEP  1024 //2013-1-30 *v_KI_M8/3200
       //���ֿ���ʱ��ת��ƫ���ۼƵ�M0_DREV_TG_kI_ASTEP=50���ۺϵ����һ��
//#define M0_DREV_TG_KD_ASTEP 32 //2013-1-30 ΢�ֳ���=v_KD_M8/32
#define M0_DREV_TG_KD_ASTEP 16 //2013-1-30 ΢�ֳ���=v_KD_M8/32 2015-12-27
       //ת��΢�ֿ���ʱ��ת��ƫ��仯���Ĳ�ֵ�ۼƵ�M0_DREV_TG_KD_ASTEP=32���ۺϵ����һ��



#define M_X 1

#define  M0_RPM_H0 35/M_X  //25           //��ת��97.5��ETC��M0_ETC_DEC_to_H0=5
#define  M0_ETC_DEC_to_H0 3 //5;
#define  M0_RPM_H1 45/M_X//35           //��ת��97.5��ETC��M0_ETC_DEC_to_H0=10
#define  M0_ETC_DEC_to_H1 5 //10;      
#define  M0_RPM_H2 55/M_X //45           //��ת��136��ETC��M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_H2 10;
#define  M0_RPM_H3 65/M_X //55           //��ת��214��ETC��M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_H3 15 //20;
#define  M0_RPM_H4 75/M_X //65           //��ת��253��ETC��M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_H4 15 //30; 
#define  M0_RPM_H5 85/M_X //75           //��ת��292��ETC��M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_H5 20 //31;
#define  M0_RPM_H6 95/M_X //80           //��ת��312��ETC��M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_H6 25 //31;
#define  M0_RPM_H7 130/M_X           //��ת��507��ETC��M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_H7 31 

#define  M0_RPM_L0 -30/M_X           //��ת��97.5��ETC��M0_ETC_DEC_to_H0=5
#define  M0_ETC_DEC_to_L0 3
#define  M0_RPM_L1 -40/M_X           //��ת��97.5��ETC��M0_ETC_DEC_to_H0=10
#define  M0_ETC_DEC_to_L1 5 //7;      
#define  M0_RPM_L2 -50/M_X           //��ת��136��ETC��M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_L2 9 //10;
#define  M0_RPM_L3 -60/M_X           //��ת��214��ETC��M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_L3 10 //15;
#define  M0_RPM_L4 -70/M_X           //��ת��253��ETC��M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_L4 15 //20; 
#define  M0_RPM_L5 -75/M_X           //��ת��292��ETC��M0_ETC_DEC_to_H0=15
#define  M0_ETC_DEC_to_L5 15 //31;
#define  M0_RPM_L6 -80/M_X           //��ת��312��ETC��M0_ETC_DEC_to_H3=16
#define  M0_ETC_DEC_to_L6 20 //31;
#define  M0_RPM_L7 -130/M_X           //��ת��507��ETC��M0_ETC_DEC_to_H4=32
#define  M0_ETC_DEC_to_L7 31 

/*-------For IACV---------*/
#define  M0_DNSTEP_KP 512//1024
#define  M0_DNSTEP_KI 8192//16384
#define  M0_STOFCORADJ 6//5 2020-3-24 �ֵ�����Ҫ������ͣһ��������Ϊ3�ı���
#define  M0_TCORSTP 5	//��΢�ߵ���ִ�дֵ��ڵ�ʱ������	p1
#define  M0_DNCORSTP 20	//��΢�ߵ���ִ�дֵ��ڵ�ת������	p15d6
 

//���ص��������ŵĹ�ϵ������0��
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
byte v_KD_M8; //KDδ��ʹ�ã�����Ϊ��ʼTPSλ��
//2013-2-1 
#define M0_TPS_ST 50 //2013-1-28,BYPTR_M0_KD_M8  //2013-2-2 liulaoshi zoule
//#define M0_TPS_ST 60 //2013-1-28,BYPTR_M0_KD_M8

word v_tmr_DC;

byte levelO;

//������ת�ٵ�״̬
byte st_rev_DEC;
byte st_rev_ACC;

//���ٶȵı仯״̬��
byte st_a_DEC;
byte st_a_ACC;

//2012-11-4 add
byte v_TPS_lst_DC;
byte v_TPS_lst2_DC;
byte v_dTPS_DC;            //2013-1-13 lcw TPS��TPS_tg֮��
byte v_dTPS_lst_DC;
byte v_dTPS_lst2_DC;


byte v_TPS_tg;//2012-11-14 add TPS��Ŀ��ֵ
byte v_TPS_tg_lst;      //lcw 2013-1-13



void StepMotor_Init(void);

byte t_forMinTPS;//2012-12-13

byte nb_er_lds0; //�������������ϵĴ�����
byte nb_er_lds1; //�������������ϵĴ�����

//�����źŲɼ�
#define IDX_LD_MX 19
//byte I_ad[2][2][IDX_LD_MX+1]; //DIM0--CHANNEL NO  ,DIM1--IDX  ,DIM2--  ,
word tick_lst_ad_ld;//��һ�θ���ad��ʱ��
byte idx_ld;
byte v_I_max[2];
byte v_I_max_lst[2];

byte v_I_min[2];
byte v_I_min_lst[2];

byte v_I_ldx_lst2[2];
byte v_I_ldx_lst[2];
byte v_I_ldx[2];

sword v_dI_ld[2];//ld�ı仯��ӳ��di�ϵı仯

byte point_ld;

byte v_nb_ldAd_1cycle;//1Ȧ�����ld�ɼ�������

void SetAsTpstg(sword sw) ;

byte SM_Position;
void FC_SM(byte blnIni);
//#define STEP_SM_MAX 125
//#define STEP_SM_MAX 160//2016-9-13
//#define STEP_SM_MAX 180
//#define STEP_SM_MAX 200
//#define STEP_SM_MIN 10
//#define STEP_SM_MIN 20	//2017-11-8

sword v_step_sm;//step ��λ��
sword v_StepAct;
sword v_step_sm_tg;//step ��Ŀ��λ��
sword v_StepAct_lst;//2017-11-11

sbyte v_t_IACV_on;//2016-9-10 IACV����������
byte v_step_IACV_on;//2016-9-10 IACV�����Ĳ�����
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
byte VF_OPENLOOP_IACV;		//��ȥIACV�������Ƶı�־

//20211010 chenwei add ���������ȹ�������
byte v_t_O2H_delay_1s;			//ʹ�������ȵ��ӳ�ʱ�䣬���복������
byte v_o2h_n_625;					//�����ȿ�ʼ������ת����ֵ
byte v_o2h_tpd;					//�����ȿ�ʼ�����Ŀ�����ֵ
byte VF_O2H_STATUS;		//ָʾ�����ȵ�״̬

void F_O2H_FORHUANSONG(void);

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
//////////////////////Initialize/////////////////////////
void StepMotor_Init(void)
{

    //����������ʼ��
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
			if(!VF_1ST_INI_SYSINI)	//2018-3-22 ��ʼ�����̷�����reset����Ҫ���ѧ��
			{
				Fset_SM(255);	//���ѧ��λ 2018-3-22 Ϩ�𿪹ػ����쳣Ϩ������Ҳ����ѧ���Ƿ����
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
			v_step_sm_tg=10;//2020-12-18 �����Կ��ǰѧ����ɣ��͸���ֵ�����򱣳�ԭֵ����
			v_StepAct=0;
			v_step_sm=0;
		}
		
		v_IACV_ini_st=102;
		Save_Data_EE(0x7f,102);
		
		/*if(v_IACV_ini_st!=100)
		{			
			if(!VF_1ST_INI_SYSINI)	//2018-3-22 ��ʼ�����̷�����reset����Ҫ���ѧ��
			{
				Fset_SM(255);	//���ѧ��λ 2018-3-22 Ϩ�𿪹ػ����쳣Ϩ������Ҳ����ѧ���Ƿ����
			}				
		}*/
		#endif
		
        VF_LEARZRO_KEYOFF=false;

		
    }
	
	
	/*#ifdef SYS_STEPMOTOR
	if(VF_JSTOP_ENG)
	{		
		if(!VF_1ST_INI_SYSINI)	//2018-3-22 ��ʼ�����̷�����reset����Ҫ���ѧ��
		{
			Fset_SM(255);	//���ѧ��λ 2018-3-22 Ϩ�𿪹ػ����쳣Ϩ������Ҳ����ѧ���Ƿ����
		}			
	}
	#endif*/
	
    
    /*if(VF_POR_HARD){
        
    }*///20210227 chenwei �˴���ʱû�õ��˴����ε�
    
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
    �����źŲɼ�
    blnReset:���µĻ�׼ʱ�䵽��ʱ�����¿�ʼһ�����ݣ�����ѡ��Ǳ�ǰ����Ϊ��׼ʱ��
    

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
    {   //1MS ÿ��
    
        for(j=0;j<=1;j++)
        {
            if(blnReset)
            {
                //��ʱ���׼����֮�󣬱���ad����
                idx_ld++;
                if(idx_ld>1)idx_ld=0;
                
                v_I_ldx_lst2[j]=v_I_ldx_lst[j];
                v_I_ldx_lst[j]=v_I_ldx[j];
                
                //������һ������
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
                
                //2015-11-20 ��������
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
        //���㸺�ɱ仯
        v_nb_ldAd_1cycle=point_ld;
        point_ld=0;
        
        v_dI_ld[1]=v_dI_ld[0];
        
        v_dI_ld[0]=(sword)v_I_ldx[0]+v_I_ldx[1]-(sword)v_I_ldx_lst[0]-v_I_ldx_lst[1];
    
        if(VF_MMOVE_ENG)
        {
            //2015-11-20 ���ӿ���
            //����������ͻ�䣬�����������Ž���׼��������
            
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
    
    
        //2016-1-4 ���Ӷ�LD�Ƿ���ϵ�У��
    
    
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

/*��ȡ1ͨ���ĵ�������*/
void Save_I_Load_12(word iLoad,word iLoad2)
{
    sword sw;
    
    
    
    v_I_ini_lst2[0]=v_I_ini_lst[0];          //2013-1-13 �����ʼֵ      
    v_I_ini_lst[0]=v_I_ini[0];               //2013-1-13�����ʼֵ 
 
  
    v_I_ini[0]=(byte)((iLoad+iLoad2)>>3);
    temp_I1_init_lst_cap=temp_I1_init_cap;     
    temp_I1_init_cap=v_I_ini[0];

    
    
}

/*��ȡ2ͨ���ĵ�������*/
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

/*����λ�ü��*/
void FD_LUB()    
{  
    word w1,w2;
    /*
    
    if(VF_MMOVE_ENG){       //ֻ����תʱ�ż����ͻ�Ϩ�𿪹�
      
        F_SelCh_ADC(CH_AD_EngineOil);	
        w1=F_WaitSample_ADC_result();
        F_SelCh_ADC(CH_AD_EngineOil);	
        w2=F_WaitSample_ADC_result();
    
        w1=(byte)(w1>>2);
        w2=(byte)(w2>>2); 
        w1=(w1+w2)/2;
//    v_EngineOil=(w1+w2)/2;
    

        if(w1<50){       //������Ͳ���ֵ<50,�����޻��ͣ���ػ�
          if(VF_LUBEMPTY_ETC_lst)VF_LUBEMPTY_ETC=true;
          VF_LUBEMPTY_ETC_lst=true;
        }else {
         VF_LUBEMPTY_ETC_lst=false;  
        VF_LUBEMPTY_ETC=false;    
        }
    }
    
    if(REMARK!=4)VF_LUBEMPTY_ETC=false;//2013-6-21 ��Ϊ LAMBDA��������λ���빫���ˣ�����ֻ����λ��REMARK=4ʱ������ 
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

      
   // �Ȳɼ������ź�1 
     
      
     // F_SelCh_ADC(CH_AD_CURRENT1,0);
      
      sw=FAD_1Channel(CH_AD_CURRENT1,false,2);
      //sw=F_WaitSample_ADC_result(); 
      b_I0=b=sw>>2; 
      
      //2013-2-4
      if(b<10){
          nb_er_lds0++;    
      } else nb_er_lds0=0;
      if(nb_er_lds0>200)VF_OPEN0_LDSENSOR=true;
      
      //VF_OPEN0_LDSENSOR=true;           //����������С��Ҳ�ǿ�·
      
      
     /*�����������ĵ����ź�*/
      if(!VF_MMOVE_ENG){
            if(v_ld0[0]>v_I_ini[0])v_ld0[0]=v_I_ini[0];
      }else{
 
             //2013-1-31��������������ò���Ч &&(!VF_OPEN0_LDSENSOR)
             if(!VF_CAP0_LDSENSOR){

                if(VF_I_CAP_lst||VF_I_CAP_Twns_lst){
                    if(b>v_ld0[0])v_I_ini[0]=b_I0;   //����ϴβ���ɹ����ò���ֵ��Ч 
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
             if(!VF_CAP0_LDSENSOR)st_I_Not_cap++;         //���û�������1��st_I_No_cap��bit0��λ
             
             VF_CAP0_LDSENSOR=false;
             VF_I_CAP_Twns=false;
             
          
      }
      
      
      
      if(VF_OPEN0_LDSENSOR)v_ld0[0]=0;  //2012-11-11 add 

      
   // �Ȳɼ������ź� 2          
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
            //2013-1-31��������������ò���Ч     &&(!VF_OPEN1_LDSENSOR) 
          if(!VF_CAP1_LDSENSOR){

                if(VF_I2_CAP_lst||VF_I2_CAP_Twns_lst){
                    if(b>v_ld0[1])v_I_ini[1]=b_I2;   //����ϴβ���ɹ����ò���ֵ��Ч 
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
          if(!VF_CAP1_LDSENSOR)st_I2_Not_cap++;         //���û�������1��st_I_No_cap��bit0��λ
             
          VF_CAP1_LDSENSOR=false;
          VF_I2_CAP_Twns=false; 
 
          
      }
      


  
       if(VF_OPEN1_LDSENSOR)v_ld0[1]=0;
   


      
      /*����������Ϊ��Ը���*/  
          
      b=0;      

      if(!VF_OPEN0_LDSENSOR){
          if(v_I_ini[0]>v_ld0[0])b=v_I_ini[0]-v_ld0[0];
      } else {               //2013-1-31 �����������й���ʱҲ�㣬����Ч�����𽥱�Ϊ0
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
      
      
      
      b=0;//2012-11-12 ������

      if(!VF_OPEN1_LDSENSOR){
          if(v_I_ini[1]>v_ld0[1])b=v_I_ini[1]-v_ld0[1];
      } 
      else {         //2013-1-31 �����������й���ʱҲ�㣬����Ч�����𽥱�Ϊ0
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
      
      /*���������׽��־*/
      
           
      
      temp_st_I2_not_cap=st_I2_Not_cap;        //lcw test           
      temp_st_I1_not_cap=st_I_Not_cap ;


/*2013-1-31
//2013-1-25��������С��TPS�ĳ����Ƶ��˴�
        //��������
      //�����ȶ������� û��ʵ������
      sw_v=(sword)v_I_ld_lst-v_I_ld;
      sw_a=(sword)v_I2_ld_lst-v_I2_ld;
 */           

//2013-1-27��ƽ��������Ϊ��ֵ̬��¼
      v_I_lst=v_I; 
      v_I_stable_lst=v_I_stable;
      v_TPS_I_lst=v_TPS_I; 
      v_TPS_I_stable_lst=v_TPS_I_stable;                 


                //2013-1-27 ��Ϊ˲̬��ƽ�����������
                //ͻ��ʱʹ��˲ֵ̬����Ȼʹ��ƽ��ֵ

      v_I=(word)v_I_ld+(word)v_I2_ld;
      v_I_stable=((word)v_I_ld_lst+v_I_ld+v_I_ld_lst2+v_I_ld_lst3+v_I_ld_lst4
                  +(word)v_I2_ld_lst+v_I2_ld+v_I2_ld_lst2+v_I2_ld_lst3+v_I2_ld_lst4)/5;
                //2013-1-31ƽ����������  
      v_TPS_I=F_2DLookUp(v_I,M2_TPS_BYLD,6,0,false);        //˲̬������Ӧ��TPS
      v_TPS_I_stable=F_2DLookUp(v_I_stable,M2_TPS_BYLD,6,0,false);    //2013-1-27 ƽ��������Ӧ��TPS      
 
                //v_I_stable  20 for 1440w
            /*
                ��Ϊv_I_stable���˲������´Ӽ�⵽����ͻ�䣬
                    �������ʼִ�У���Ҫ��1-2Ȧ��
                    �ٴε��ִ�У��������������Ҫ��1-2Ȧ��
                    ��ˣ��������ѭ����
                    �ٴӶ�����ֱ������Ŀ�꣬��Ҫ���ѭ����
                    ��ˣ���ȻҪ��ת���µ��Ĺ��̣�
                    
                ֱ�����������ٶȣ�30-40����ֵ/ѭ����    
            */
            
            /*����������ʼ���õ���*/

      VF_INC_LDSENSOR=false;
      VF_DEC_LDSENSOR=false;
      VF_MAYACC0_LDSENSOR=false;
      VF_MAYDEC0_LDSENSOR=false;
      VF_MAYACC1_LDSENSOR=false;
      VF_MAYDEC1_LDSENSOR=false;          
      if(VF_STA_LD>VF_TPS_READY){
                
          if(v_ccl_Istable<255)v_ccl_Istable++;
                
          sw_v_p=((sword)v_TPS_I-v_TPS_I_lst);        //����˲̬������ӦTPS�Ĳ���
                 //�����仯��������ȡ1/2ȥ��������
          if((sw_v_p<=20)&&(sw_v_p>=-20))sw_v_p/=2;

                                                      //����˲̬�����������ʱ��Ҫ���⴦��
          if(sw_v_p>20){
              if(Act_Step<0)Act_Step=0;
              v_nbStep_ETC+=sw_v_p;   //1���������仯�����TPS���죬��Ϊ����
                                      //2������������10ʱ���õ���������־����Ԥ�ӵ�
              if(v_nbStep_ETC>100)v_nbStep_ETC=100;

              VF_MAYACC0_LDSENSOR=true;              //2013-1-27Ԥ�ӵ�
              DC_INC;
              DC_ON; 
              //2013-1-28              VF_INC_LDSENSOR=true;  
              v_ccl_Istable=0;         
          }else if(sw_v_p<-20){        //3��������С��-10ʱ���õ�������־����Ԥ�ӵ�
              if(Act_Step>0)Act_Step=0;
              v_nbStep_ETC+=sw_v_p;           
              if(v_nbStep_ETC<-100)v_nbStep_ETC=-100;
              VF_MAYDEC0_LDSENSOR=true;        //2013-1-27Ԥ�ӵ�
              DC_DEC;
              DC_ON; 
              //2013-1-28              VF_DEC_LDSENSOR=true; 
              v_ccl_Istable=0;          
          }else if(v_ccl_Istable<8){ 
              v_nbStep_ETC+=sw_v_p;          //ͻ���ڼ佫˲̬������Ӧ��TPS�仯��һ�룬�ӵ�������
          
          }else{                             //4��ͻ�������ֵ̬�ı仯
              sw_v_p=((sword)v_TPS_I_stable-v_TPS_I_stable_lst);        //���ε����Ĳ���
              //2013-1-31             sw_v_p/=2;        //�����仯��������ȡ1/2ȥ��������
              v_nbStep_ETC+=sw_v_p;   //1���������仯�����TPS���죬��Ϊ����
          }
      }
     
      
      
      temp_TPS_I_lst=v_TPS_I_lst;   //test    
      temp_TPS_I=v_TPS_I;           //test      
 
      
}

/*
    ����Ŀ�����ţ�ȷ�������������
    ����TPSλ�ã�����DC MOTOR��������
    
    ��Ҫ���Ƕ�̬��������;
    
    
    ���õ����������ʱ����Ҫ��������
    1��Ŀ�������뵱ǰ���ŵĲ��죻
    2����ǰ���ת�٣�
*/

void FS_DCMotor(void)
{             //ÿȦ����

      byte bClk;
      byte TPS_now;
      sword sw;

      //v_TPS_tg=128;
      if(!VF_BYTPD_ETC)v_Adj[ADJ_TPDtg]=0;

      if(VF_KEYON_HARD)
      {
          if((word)(T_MAINTMR-t_lst_Drive)>2500)
          {   //10MS ÿ��
              
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
              
              //2015-11-8 ������仰����������Ŀ�����20ʱ����Ӧ��ȫ�����е����
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
          ����
          TPS���Ƶ�P,I,D����
          
          I:ֱ�Ӷ�TPS_TG������
          P:
          D:
      
      
      
      */
 




      return;
      
} 


//2016-9-7
void FS_SMotor_IACV(void)
{ 
         //2016-1-25 ���Ӳ����������
       
       /* 
        if(KESEI==1)  
            v_StepAct=100;
        else
            v_StepAct=-100;// 
       */
	   if(!VF_MMOVE_ENG||VF_NEWCYCLE_ATMAIN_CAS)
	   {
		   IACV_con();//����������֮��ÿȦִ��һ��
	   }
       
        
       
        //if(VF_KEYON_HARD)//2020-8-17 ͣ����������ѧ��
        {
            //if(T_MAINTMR-t_lst_Drive>500)//2MS ÿ��
            //if(T_MAINTMR-t_lst_Drive>600)
            if(v_step_sm_tg==0)//2017-10-23 ��ʼ��ʱ������������ֽϿ�Ĳ���Ƶ��
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
					v_IACV_ini_st=101;//2020-12-18 ����ѧ0��
					Save_Data_EE(0x7f,101);			
				}
				
				//2017-11-8 ǰ��252�����ýϸߵ�Ƶ�ʿ���,���9�����ýϵ�Ƶ��
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
							Save_Data_EE(0x7f,100);//ѧ�����д״̬��,��Կ��ѧ�㣺100��
						}						
					}
					
					
				}								
			}
			else
			{
				if(VF_MMOVE_ENG)
				{
					if((word)(T_MAINTMR-t_lst_Drive)>1250)//2017-6-26 loncinҪ���޸�Ϊ200HZ ���ݵ���ֲᣬ����������Ҫ�ܿ�70~140step/sec����
					{   
                
						#ifdef SY_CORADJ
						if(VF_CORADJ&&(v_StepAct<0))
						{
							F_StepMT_3T1();//2020-3-24 �ֵ��ڹ�����Ȼ����������ͣһ���ķ�ʽ
						}
						else
						#endif
						{
							FC_SM(false);//2016-1-25
						}						
						t_lst_Drive=T_MAINTMR;
					}
				}
				else if(VF_KEYON_HARD)//2020-8-17 ��Կ�ײ�ҪԤ��λ��ֻѧ�㼴��
				{
					if(T_MAINTMR-t_lst_Drive>600)//2019-5-7 Ԥ��λƵ������
					{   
                
						FC_SM(false);
						t_lst_Drive=T_MAINTMR;
						if((v_StepAct!=0)&&(v_IACV_ini_st!=103))
						{
							v_IACV_ini_st=103;//2021-2-26 �����ڿ�ʼԤ��λ�ˣ�Ԥ��λ��״̬Ϊ103
							Save_Data_EE(0x7f,103);
						}
					}
				}
				
				/*if(VF_KEYON_HARD&&(v_IACV_ini_st!=102))
				{
					v_IACV_ini_st=102;
					//Save_Data_EE(0x7f,102);	//��Կ��״̬��ѧ����ɣ��������й���д102״̬		
				}*/
			}
        }
		//else
		//{
			//FC_SM(true);
		//}
		
		if(IS_KEY_OFF&&!VF_KEYON_HARD&&(v_t_eng_t_s>10)&&(v_IACV_ini_st==100))
			//ѧ������ص��
		{
			FC_SM(true);
		}
      

/*****************2017-3-10 ���¸��ݷ���������״���ж��Ƿ���Ҫ���дֵ���******************************/
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
    ����H-BRIDGE���������ԣ�
    �����DC_ON��DC_OFF,��������DC_DEC����DC_INC��
    �������Ѹ�ٹرգ����ߵ��Ǹ���ƽ�����������㣻
*/
/*
    DC MOTOR PARAMETER
    120RPM--2RPS
    125MS--90DEG--225samp        0.4DEG/samp
    1ms--2samp��ʹ�ò������ʱ��Ӧ2v_nbStep_ETC��
              ��1samp��Ӧһ��v_nbStep_ETC;
              
   
   ���ڷ������PID����
      TPS(K)+=KP[E(K)-E(K-1)]
          +KI*TS*E(K)
          +KD*{E(K)+E(K-2)-2*E(K-1)}/TS
      
      
      TPS(K)=KP*E(K)+KI*SIGMA{E(N),N FROM 0 TO K}*TS+KD*{E(K)-E(K-1)}/TS    
    
*/
/*
    �Ե���ĵ��ڲ��ã�ռ�ձ�+�������ķ�ʽ
        D/8����ռ�ձ�
        nP����������
            ����
*/

//=========================================================================
/*
    �趨VF_TPS_READY״̬ǰ��Ŀ��TPS
    
    ECT��ʼ��
    ��VF_STA_LD<=VF_SETINIT_STALDʱ�����г�ʼ������
    �뷢�����Ƿ������޹أ�
    
    ��VF_TPS_READY״̬�󣬽�����������
    
    ��Ҫ����TPS���ϵ������
    
    
*/
//=========================================================================
void FS_ECT_IdlePosition(byte tps,byte tps_odr,byte tes)
{
  sword w1,w2;
  sword sw;  
    
    
    //�����¶ȶ��������ŵ�������2012-6-6
    
    w1=tes;
    if(w1<70)
    {
        w1=70-tes;
        w1=w1/4;
    } else w1=0;
    
    
    v_TPS_st=M0_TPS_ST;          //2013-2-1 ��ʼ�������¶��޹�  

    
    VF_ACC_ETC=false;
    VF_INC_LDSENSOR=false;
    VF_DEC_ETC=false;
    VF_DEC_LDSENSOR=false;
    
     //������ֹͣʱ��ֱ����������Ŷ�����������ı��Ҫȷ�ϣ���ʱv_nbStep_ETC==0
     //Ӧ����Ŀ��TPS����һ��ʱ�仹û�ӽ���Ϊ�����⡣
    
    //���õ����ʼλ��
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
            {   //����������
                VF_FAILURE_ETC=1; //2012-11-2       
            }
            else
            {//��������λ�ñƽ�
               v_step_formin+=1; 
            }
        }
        
        //������Ź��ϣ���ֱ�ӿ�����С����λ�ã�
        if(VF_ER_TPS){
            VF_STA_LD=VF_TOTPSMIN_STALD;
        }
    }
    else if(VF_STA_LD==VF_TOTPSMIN_STALD)
    {   //������Сλ��
        v_TPS_tg=M0_ETC_TPS_MIN+2;       //to min+2
        sw=(v_TPS_tg-(sword)tps);
        if(sw>-3&&sw<3){
            //������Сλ��
             VF_STA_LD=VF_SETINIT_STALD;
             
             v_step_formin=0;      //for the next period   
        }
        else  {
            if(v_step_formin>250) //2012-10-27 from 400 to 200
            {   //����������
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
                    //��������λ��
                  VF_STA_LD=VF_TPS_READY;
                  v_TPS_tg=v_TPS_st+1;    //to v_TPS_st+1
                  v_step_formin=0;      //for the next period  
               
              }
              else  
              {
                  if(v_step_formin>250) //2012-10-27 from 400 to 200
                  {   //����������
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
      
      		sw=(sword) (v_rev3d9_tg- (sword)rev_3d9);// ��������ֵ��Χ�ڣ�


           //2013-1-20ת����10��λ���²�ѹ
          if(sw<0)
          {               //sw<0,ת�ٸ���Ŀ��ת��ʱ
              sw+=20;               //sw+20
              if(sw>0)sw=0;          //+20��>0,����ǰ
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
      		
      		    v_dnb_byIgn++;//��
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
     
    
    if(v_d_ign_fb>10)v_d_ign_fb=10;  //2012-10-28  ��ʱ
    if(v_d_ign_fb<-15)v_d_ign_fb=-15;//5;  //2012-10-28  ��ʱ    
    
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
        sword d_ETC_PID;                //���Ϊ013-1-13 ��ת��PID�Ľ���ۼӺ��ټӵ������ϣ������ж�
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
            //������ٶȺ�Ŀ��ת�ٲ���
            v_acc_rev=(sword)rev_3d9-rev_3d9_lst;  //���ٶ�
            v_d_rev_to_tg=(sword)rev_3d9-v_rev3d9_tg;                   //v_rev3d9_tg����һѭ����RPm

        /*-----------------------------------------------------------------------------*/ 
        //�� ���������ȹ��̵�״̬�ж�������8Ȧ����3400�ŵ�����״̬
            if(VF_STA_LD<=VF_TPS_READY)     //�ж��Ƿ�������������2012-6-6 change to <=
            {                              //�������������з���rpm_3d9�м������Ŀ��ܣ�
                v_ccl_StartsProcess=v_ccl_StartsProcess<<1;      //����Ҫ��8����������3000ת������Ϊ��������������˲̬���̡�
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


        /*--------------------------����ת�ټ����ٶȶԵ���Ŀ���------------------------*/
        //2013-1-30 lcw��Ӧ����VF_TPS_READYǰ����������
            //if(VF_STA_LD<=VF_TPS_READY)  //�������ٿ���   2012-6-6 change to <=
            if(VF_STA_LD==VF_TPS_READY)    //�������ٿ���   
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
            if(!VF_YSTDC_INJPHASE) return;     //������Ȧ����ó���β�������
    
            if(!VF_BFIRE_ENG)return; 


        /*----------״̬�ж�-------------------------------------------------------------*/ 
            st_rev_DEC=(st_rev_DEC<<1)&0xfe;
            st_rev_ACC=(st_rev_ACC<<1)&0xfe;
            st_a_DEC=(st_a_DEC<<1)&0xfe;
            st_a_ACC=(st_a_ACC<<1)&0xfe;
            
            if(v_d_rev_to_tg>(v_d_rev_to_tg_lst+1))st_rev_ACC++;       //2013-1-13��1���ϣ��������
            if(v_d_rev_to_tg<(v_d_rev_to_tg_lst-1))st_rev_DEC++;       //2013-1-13��1���ϣ��������            
 
            if(v_acc_rev>v_acc_rev_lst)st_a_ACC++;
            if(v_acc_rev<v_acc_rev_lst)st_a_DEC++;
            
        /*--------------------------------------------------------------------------------*/ 
            if(v_d_rev_to_tg<10)V_ETC3_STATUS_RPMto_H=0 ; //ת�ٵ���Ŀ��ת�ٺ�������Ե�����ٱ��
            if(v_d_rev_to_tg>-10)V_ETC4_STATUS_RPMto_L=0 ; //ת����Ŀ��ת�ٺ�������Ե�����ٱ��

            //2013-1-20����ƽ��ת�� 4,3,2,1

            v_d_AVrpm_to_tg=(v_d_rev_to_tg+v_d_rev_to_tg+v_d_rev_to_tg+v_d_rev_to_tg
                   +v_d_rev_to_tg_lst+v_d_rev_to_tg_lst+v_d_rev_to_tg_lst
                   +v_d_rev_to_tg_lst2+v_d_rev_to_tg_lst2
                   +v_d_rev_to_tg_lst3)/10;
              

            //V_ETC3_STATUS_RPMto_H=0;
            //V_ETC4_STATUS_RPMto_L=0;

            //2012-11-18 add ccl_disableN
            if((VF_STA_LD>VF_TPS_READY))
            {   //2012-5-30 ADD CONDICTION

                d_ETC_PID=0;                                               //2013-1-13ת�� PID�������ƽ������
                
                //v_nbStep_ETC=0;   //2015-10-23
                
                /*-----------��������-------------------------------------------------*/
                      //2013-1-17 ׼����ƽ��ת����ת�ٱ�������
                      if(v_d_AVrpm_to_tg>20||v_d_AVrpm_to_tg<-20
                              ||((st_rev_DEC&0x0f)==0x0f)||((st_rev_ACC&0x0f)==0x0f)){
                          sw_v_p=v_d_AVrpm_to_tg-v_d_AVrpm_to_tg_lst;
                           //ƫ��10���⣬�����ӻ�����3ѭ�����ϣ���ƫ��ı仯��Ϊ��������
                      } else{
                          sw_v_p=(v_d_AVrpm_to_tg-v_d_AVrpm_to_tg_lst); ///4; 
                          //ƫ��10���ڣ��Ҳ�����Ҳ������3ѭ�����ϣ���ƫ��ı仯��1/4��Ϊ������          
                         
                      }
                 
                 
                      if(sw_v_p>M0_DRPM_MAX_pDC)sw_v_p=M0_DRPM_MAX_pDC;        //ƫ��仯����M0_DRPM_MAX_PDC=60ʱ��ȡ60
                      if(sw_v_p<-M0_DRPM_MAX_pDC)sw_v_p=-M0_DRPM_MAX_pDC;       //ƫ��仯С��-60ʱ��ȡ-60             
                      
                      //2013-1-30ʹ��v_KP_M8
                      sw_v_p=sw_v_p*v_KP_M8;
                      drev_tg_rp-=sw_v_p;                               //2013-1-30 ��v_KP_M8����
                      
                      
                      sb=0;
                      while(drev_tg_rp>M0_DREV_TG_KP_ASTEP)
                      {                    //��ƫ��仯���ۼƻ��ɲ�����M0_DREV_TG_P_ASTEPΪһ��
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
                          //ת��������Ŀ��ת��M0_RPM_H0���趨VF_RPM_TO_H0��ETC��M0_ETC_DEC_to_H0
                          sb-=M0_ETC_DEC_to_H0;
                          VF_RPM_TO_H0=true;
                       }
                       if(v_d_AVrpm_to_tg>M0_RPM_H1&&!VF_RPM_TO_H1){
                          //ת��������Ŀ��ת��M0_RPM_H1���趨VF_RPM_TO_H1��ETC��M0_ETC_DEC_to_H1
                          sb-=M0_ETC_DEC_to_H1;
                          VF_RPM_TO_H1=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H2&&!VF_RPM_TO_H2){
                          //ת��������Ŀ��ת��M0_RPM_H2���趨VF_RPM_TO_H2��ETC��M0_ETC_DEC_to_H2
                          sb-=M0_ETC_DEC_to_H2;
                          VF_RPM_TO_H2=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H3&&!VF_RPM_TO_H3){
                          //ת��������Ŀ��ת��M0_RPM_H3���趨VF_RPM_TO_H3��ETC��M0_ETC_DEC_to_H3
                          sb-=M0_ETC_DEC_to_H3;
                          VF_RPM_TO_H3=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H4&&!VF_RPM_TO_H4){
                          //ת��������Ŀ��ת��M0_RPM_H4���趨VF_RPM_TO_H4��ETC��M0_ETC_DEC_to_H4
                          sb-=M0_ETC_DEC_to_H4;
                          VF_RPM_TO_H4=true;
                       } 
                       if(v_d_AVrpm_to_tg>M0_RPM_H5&&!VF_RPM_TO_H5){
                          //ת��������Ŀ��ת��M0_RPM_H2���趨VF_RPM_TO_H2��ETC��M0_ETC_DEC_to_H2
                          sb-=M0_ETC_DEC_to_H5;
                          VF_RPM_TO_H5=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H6&&!VF_RPM_TO_H6){
                          //ת��������Ŀ��ת��M0_RPM_H3���趨VF_RPM_TO_H3��ETC��M0_ETC_DEC_to_H3
                          sb-=M0_ETC_DEC_to_H6;
                          VF_RPM_TO_H6=true;
                       }                 
                       if(v_d_AVrpm_to_tg>M0_RPM_H7&&!VF_RPM_TO_H7){
                          //ת��������Ŀ��ת��M0_RPM_H4���趨VF_RPM_TO_H4��ETC��M0_ETC_DEC_to_H4
                          sb-=M0_ETC_DEC_to_H7;
                          VF_RPM_TO_H7=true;
                       }                  
                       
                       
                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L0&&!VF_RPM_TO_L0){
                          //ת���½���Ŀ��ת������M0_RPM_L0���趨VF_RPM_TO_L0��ETC��M0_ETC_DEC_to_L0
                          sb+=M0_ETC_DEC_to_L0;
                          VF_RPM_TO_L0=true;
                       }
                       if(v_d_AVrpm_to_tg<M0_RPM_L1&&!VF_RPM_TO_L1){
                          //ת���½���Ŀ��ת������M0_RPM_L1���趨VF_RPM_TO_L1��ETC��M0_ETC_DEC_to_L1  
                          sb+=M0_ETC_DEC_to_L1; 
                          VF_RPM_TO_L1=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L2&&!VF_RPM_TO_L2){
                          //ת���½���Ŀ��ת������M0_RPM_L2���趨VF_RPM_TO_L2��ETC��M0_ETC_DEC_to_L2    
                          sb+=M0_ETC_DEC_to_L2; 
                          VF_RPM_TO_L2=true;                 }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L3&&!VF_RPM_TO_L3){
                          //ת���½���Ŀ��ת������M0_RPM_L3���趨VF_RPM_TO_L3��ETC��M0_ETC_DEC_to_L3       ;
                          sb+=M0_ETC_DEC_to_L3; 
                          VF_RPM_TO_L3=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L4&&!VF_RPM_TO_L4){
                          //ת���½���Ŀ��ת������M0_RPM_L4���趨VF_RPM_TO_L4��ETC��M0_ETC_DEC_to_L4        
                          sb+=M0_ETC_DEC_to_L4; 
                          VF_RPM_TO_L4=true;
                       }       
                       if(v_d_AVrpm_to_tg<M0_RPM_L5&&!VF_RPM_TO_L5){
                          //ת���½���Ŀ��ת������M0_RPM_L2���趨VF_RPM_TO_L2��ETC��M0_ETC_DEC_to_L2       
                          sb+=M0_ETC_DEC_to_L5; 
                          VF_RPM_TO_L5=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L6&&!VF_RPM_TO_L6){
                          //ת���½���Ŀ��ת������M0_RPM_L3���趨VF_RPM_TO_L3��ETC��M0_ETC_DEC_to_L3         
                          sb+=M0_ETC_DEC_to_L6; 
                          VF_RPM_TO_L6=true;
                       }                 
                       if(v_d_AVrpm_to_tg<M0_RPM_L7&&!VF_RPM_TO_L7){
                          //ת���½���Ŀ��ת������M0_RPM_L4���趨VF_RPM_TO_L4��ETC��M0_ETC_DEC_to_L4          
                          sb+=M0_ETC_DEC_to_L7; 
                          VF_RPM_TO_L7=true;
                       }                                 
                       
                       
                       //d_ETC_p+=sb;
                       v_d_ETC_p=sb;

                       d_ETC_PID+=sb;    //2013-1-13 ���������Ʋ����ӵ�d_ETC_PID  
                        
                /*-----------���ֿ���-------------------------------------------------*/
                        //lcw2013-1-12ת�ٻ��ֿ����޸�Ϊ��ƫ���ۼӣ�ÿv_nb_I_rpm����һ��
         
                        sw_v_i=v_d_AVrpm_to_tg; 

                        if(sw_v_i>M0_DRPM_MAX_iDC)sw_v_i=M0_DRPM_MAX_iDC;        //ƫ�����50ʱ��ȡ50
                        if(sw_v_i<-M0_DRPM_MAX_iDC)sw_v_i=-M0_DRPM_MAX_iDC;        //ƫ��С��-50ʱ��ȡ-50             
                      
                        //2013-1-30 ʹ��v_KI_M8                
                        sw_v_i=sw_v_i*v_KI_M8;

                        drev_tg_ri-=sw_v_i;                                          //ƫ���ۼ�

                       
                        sb=0;
                        while (drev_tg_ri>(long int)M0_DREV_TG_KI_ASTEP){                    //��ƫ���ۼƻ��ɲ�����M0_DREV_TG_I_ASTEPΪһ��
                            drev_tg_ri-=(long int)M0_DREV_TG_KI_ASTEP;
                            sb++;
                        }
                        while (drev_tg_ri<-(long int)M0_DREV_TG_KI_ASTEP){
                            drev_tg_ri+=(long int)M0_DREV_TG_KI_ASTEP;
                            sb--;
                        }
                        
                        if(v_d_rev_to_tg>75)sb-=1;          // ת�ٹ���ʱ�������                 
                       
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
                        d_ETC_PID+=sb;                                            //2013-1-13 �����ֿ��Ʋ����ӵ�d_ETC_PID                


                /*-----------΢�ֿ���-------------------------------------------------*/
                        sw_v_p=(v_d_rev_to_tg-v_d_rev_to_tg_lst)-(v_d_rev_to_tg_lst-v_d_rev_to_tg_lst2);  //��ƫ��仯�Ĳ�ֵ
          
                        if(sw_v_p>M0_DRPM_MAX_dDC)sw_v_p=M0_DRPM_MAX_dDC;        //ƫ��仯�Ĳ�ֵ�޶��ڴ���M0_DRPM_MAX_DDC
                        if(sw_v_p<-M0_DRPM_MAX_dDC)sw_v_p=-M0_DRPM_MAX_dDC;            
                      
                      //2013-1-30 ʹ��v_KD_M8        
                        sw_v_p=sw_v_p*v_KD_M8;
                        drev_tg_rd-=sw_v_p;                                       //ƫ��仯��ֵ�ۼ�
         
                      //2013-1-14ת�ٱȽϵͺ����ͻ����΢�ּӱ�
                      //2013-1-19              if(v_d_rev_to_tg<-10&&sw_v_p<-2)drev_tg_rd=drev_tg_rd-sw_v_p;
         
                        sb=0;
                        while (drev_tg_rd>M0_DREV_TG_KD_ASTEP){                    //��ƫ���ۼƻ��ɲ�����M0_DREV_TG_P_ASTEPΪһ��
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
                
               //2013-1-13 ��΢�ֿ��Ʋ����ӵ�d_ETC_PID                
               /*d_ETC_PIDΪת��PID�ļ���������Ҫ�����ֳ����������ʵ����޶���Ȼ������Ϊ���ڵĲ���
                    1.Ŀ��ת��ƫ��̫��ʱ�����Ƶ����ٶȣ�
                    2.������TPS������һ�� 
                  2013-1-15�󸺺�ͻ������ʱ����ȹ�����ת�ٵ���Ŀ��ֵʱ���ܿ���й�.�󸺺��ر���ȫ����.
                  �󸺺��ر���ȫ����ʱ��ת�ٺܿ��ܵ���Ŀ��ֵ��ͻй���ɣ�ת�ٿ������ӣ�TPS���ܿ�����ͻ����ܶࡣ
               */
                                
                  if(v_d_AVrpm_to_tg<-100&&d_ETC_PID<0)d_ETC_PID=0;   //2013-1-18��390�⣬���ȱ�׼ת�ٵ�̫�࣬�����Ʋ���
                  if(v_d_AVrpm_to_tg>100&&d_ETC_PID>0)d_ETC_PID=0;    //2013-1-18��390�⣬���ȱ�׼ת�ٸ�̫�࣬�����Ʋ���
     
                
                  //lcw2013-1-14������ƽ�����������
                  #if MOTOR_TYPE==MOTOR_SM
                      //13�Ǹ��ݲ����������������Ƶģ�
                      if(d_ETC_PID>13)d_ETC_PID=13;
                      if(d_ETC_PID<-13)d_ETC_PID=-13;
                  #else

                      if(d_ETC_PID>M0_STEPS_MAX_DC_trip)d_ETC_PID=M0_STEPS_MAX_DC_trip;
                      if(d_ETC_PID<-M0_STEPS_MAX_DC_trip)d_ETC_PID=-M0_STEPS_MAX_DC_trip;

                  #endif
                                

                  v_d_ETC_all=d_ETC_PID;

                  v_nbStep_ETC=0;

                  d_ETC_PID=v_d_ETC_i;
                /*-------------������������----------------------------------------------*/
 
                     //2013-1-25��d_ETC_PID��Сʱ��ֱ�Ӽ���
                        if(d_ETC_PID<3&&d_ETC_PID>-3)v_nbStep_ETC+=d_ETC_PID;
                        else{
                            if(d_ETC_PID>0){
                                if(v_nbStep_ETC<d_ETC_PID)v_nbStep_ETC+=d_ETC_PID;
                                else v_nbStep_ETC=v_nbStep_ETC+d_ETC_PID/2;           //���PID��������
                            }else{
                                if(v_nbStep_ETC>d_ETC_PID)v_nbStep_ETC+=d_ETC_PID;
                                else v_nbStep_ETC=v_nbStep_ETC+d_ETC_PID/2;  
                            }
                        }
                     

                        if(tps<M0_TPS_ST||v_TPS_tg<M0_TPS_ST){ 
                            //2013-1-17 ��ΪTPS��С��TPS-TG��С������ټ�һ���Ĳ���
                            if(v_nbStep_ETC<-2)v_nbStep_ETC=-2;  
                        }
                        
                /*-------------����Ŀ������----------------------------------------------*/
                     
                     if(VF_BYTPD_ETC){
                          sw=(sword)v_TPS_st+2+v_Adj[ADJ_TPDtg];
                          
                          SetAsTpstg(sw);
                     } 
                     else
                     {
                      
                        //v_TPS_tg
                        //sw=(sword)v_TPS+v_nbStep_ETC; 
                        sw=(sword)v_TPS_tg+v_d_ETC_i+v_d_ETC_p+v_d_ETC_d; 
                        
                        sw+=v_dnb_byIgn;      //2015-10-23 ���ǶԵ��������
                        
                        v_dnb_byIgn=0;        //2015-11-14
                        
                        SetAsTpstg(sw);
                     }
                    /*    
                      //��Ŀ�����ŵ�����
                        //2013-1-20 ����ʹ�ã���������ѭ�������ӣ��Ҳ���ת��
                        if((v_TPS_tg>v_TPS_last_cy)&&((st_rev_ACC&0xff)==0xff)){
                           v_TPS_tg=v_TPS_last_cy;    // ת��һֱ����������£�Ŀ��TPS��Ӧ������һѭ����TPS
                        }
                                            
                        
                        if((v_TPS_tg<v_TPS_last_cy)&&((st_rev_DEC&0xff)==0xff)){                //lcw2013-1-11  <39������3ѭ��
                           v_TPS_tg=v_TPS_last_cy;    // ת��һֱ����������£�Ŀ��TPS��ӦС����һѭ����TPS
                        }


                        //2013-1-16 �������6ѭ����ת�ٵ���Ŀ��ֵ��TPS_tg�����ڣ�  
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
        v_d_rev_to_tg_lst=v_d_rev_to_tg;      //lcw 2013-1-11��Ȧһ��
        rev_3d9_lst3=rev_3d9_lst2;
        rev_3d9_lst2=rev_3d9_lst;
        rev_3d9_lst=rev_3d9;
        v_acc_rev_lst=v_acc_rev;//2012-11-17
        v_TPS_last_cy=v_TPS;  //lcw2013-1-11
        v_d_AVrpm_to_tg_lst3=v_d_AVrpm_to_tg_lst2;
        v_d_AVrpm_to_tg_lst2=v_d_AVrpm_to_tg_lst;
        v_d_AVrpm_to_tg_lst=v_d_AVrpm_to_tg;        
        
}

//20120907   P=10/9   I=5/32   D=7/64   ������ ��v_ign_fb_limit-3    �͸��ɲ����ȶ����γ�
//20120911   P=18/39  I=7/64   D=10/128 ������ ��v_ign_fb_limit-4    ��̬ת���ȶ� 
           //���ǵ���Ϊ27������  MO_MAGNETOCA_QY=87; 
                  
//20121018   ��Samp_All_ADC_whenStop�ڼ��˵��������ͻ��Ͳ��� 

//#endif

/*
    2015-12-27
    ��ֵ����ΪTPS Ŀ��
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
    ����v_StepAct���Ƶ����ת    
*/
void FC_SM(byte blnIni)
{
    /*2016-9-9 �������ĵ�����߷���Ӧ��
        v_StepActΪ-ʱ��������С�ķ��򿪣�
        ���򣬷�����
    
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
			t_stepcon_delay=4;	//2017-11-11 ���״̬�ı�ʱDelay 20ms,������������Ϊ5ms
		}	

		if(t_stepcon_delay==0)
		{
        if(v_StepAct>0)                  //Act_Step>0 ��������ת
        {
             v_StepAct--;
             
             if(v_step_sm_tg!=0)v_step_sm++;

             //if(SM_Position>6)SM_Position=0;	//8��
			 if(SM_Position>2)SM_Position=0;	//4��
             else SM_Position++;
        }
        else if(v_StepAct<0)           //Act_Step<0 ��������ת
        {
             v_StepAct++;
             
             if(v_step_sm_tg!=0)v_step_sm--;

             if(SM_Position>0)SM_Position--;
             //else  SM_Position=7;	//8��
			 else  SM_Position=3;	//4��
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
                StepMotorEN1=0; //1B            //StepMotorEN=1,Ϊʹ�ܹر�
                StepMotorEN2=1;             //StepMotorPhase1=0��������OUT1B����OUT1A
                StepMotorPhase1=0;          //�������������Ϊ�ߵ�ƽ
                break;                              //���ͨ��˳��ΪA-AB-B-BC-C-CD-D-DA
        case 1: //6
                StepMotorEN1=0; //1B-2B            //��ϸ��Ϣ�μ�PCB�弰Microchip MTS62C19A�����������оƬ
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
                StepMotorEN1=0; //1B-2B            //��ϸ��Ϣ�μ�PCB�弰Microchip MTS62C19A�����������оƬ
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
    ���ò��������λ��
    
    position��Ŀ��λ��
*/
void Fset_SM(byte position)
{
    sword sw_sm;
    sword sw_exe;



    if(position==255)
		//�ϵ�������λ��ѧϰ
	{
       // Opr_SM(-400);
        v_StepAct=-261;		//-300 2017-11-8 �������г�250������ѧ��ʱcapnut����ĥ��
        v_step_sm=0;   
        v_step_sm_tg=0;
    } 
    else
    {
        if((v_StepAct==0)||(VF_CORADJ&&(!VF_CORADJ_LST)))
        {   //���ֹͣʱ�Ž����µ�����
			//2017-3-10 ���ߴֵ��ڱ�־λ�����ش���������
            //v_step_sm_tg=v_step_sm;//2016-9-11
      
            sw_sm=position;
			
			if(VF_CORADJ&&(!VF_CORADJ_LST)&&(v_ntcoradj<40))
			{
				v_step_sm+=M0_STOFCORADJ;//2017-3-10 ʧ�������쳣״��ʱ�Ĵֵ���
				v_ntcoradj++;
				//xl1+=1;
			}
            //20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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
	
//--------------------------2016-12-10 ����Ŀ��ת����ʵ��ת�ٲ���Ͳ���ƽ��------------------------------
	v_dn_iacv=rev15d6_target-(sword)v_rev_15d6rpm;//2017-4-12 �ָ�
	//v_dn_iacv=v_rev15d6tg_nocorr-(sword)v_rev_15d6rpm;
	
	//ת�ٲ���ƽ��,4,3,2,1  ��ETC��������
	v_dnave_iacv=(v_dn_iacv+v_dn_iacv+v_dn_iacv+v_dn_iacv
                   +v_dn_iacv_lst+v_dn_iacv_lst+v_dn_iacv_lst
                   +v_dn_iacv_lst2+v_dn_iacv_lst2
                   +v_dn_iacv_lst3)/10;			   


				   
//---------------------------------2016-12-10 ����IACV���Ŀ�경��----------------------------------------
   
   //2018-5-19,YSL,ȥ����ʼ������
   #ifdef SYS_TRADITIONAL
   	if(REMARK==6&&KESEI==0)
   #else
   	if(VFF_SYSNOTINIT_DTC||VFF_ECUNOTINIT_DTC||VF_1ST_INI_SYSINI||(REMARK==6&&KESEI==0))
   #endif
    //if(VFF_SYSNOTINIT_DTC||VFF_ECUNOTINIT_DTC||VF_1ST_INI_SYSINI||(REMARK==6&&KESEI==0))//2018-3-21 ��ʼ�����߻������ɱ궨ģʽ�²���Ϊ�궨ֵ
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
	//if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG)&&(v_t_eng_t_s>12))||VF_CLSMODE_IACV)	//20210929 chenwei �� �����ɳ̲����Ŀ������Ҫ��ߵ��ٲ�����3s
	if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG)&&(v_t_eng_t_s>4))||VF_CLSMODE_IACV)		//3��5
	//if((((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))||((VF_O2WORK_OSDIAG)&&(VF_O2CHANGED_OSDIAG)))&&(v_t_eng_t_s>=20))
	//2019-4-25����ʱ������
	//�����ɹ���OS��ʼ��������ջ����ƺ�
	{
		VF_CLSMODE_IACV=true;
		//if(VF_E0_TPS&&(v_rev_15d6rpm<160)&&(!VF_CICORECING_CFCI))
		//if(VF_E0_TPS&&!VF_IGNONLY_CFCI&&(!VF_CICORECING_CFCI)&&(v_t_afterbackidle_d1s>=30)&&(!VF_CORADJ))
		if(VF_E0_TPS&&!VF_IGNONLY_CFCI&&(!VF_CORADJ))
		//if(VF_E0_TPS&&(v_rev_15d6rpm<160)&&(!VF_CORADJ))
			//2017-2-23 �����Żص��٣���ת�ٸߵ�״̬�²�����
			//2017-3-6 �����Ŵ����ٸ��������׶Σ�������
			//2017-3-7 ���Żص���2~3sʱ�����ڲ�����-����δ����
			//2019-3-25 ȥ��ת�ٸߵ����������Ӷ�������
			//2019-4-25 ȥ���ٸ������������Żص�������
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
			
			
			
			//ת�ٲ�������15.6*3=46.2rpm��ŵ��ڲ������
			//2017-3-6 ע�⵱����ƫС����ָʾ����˸ʱ���Ƿ���Ҫ����������
			if((v_dnave_iacv>3)||(v_dnave_iacv<-8))//2020-12-12 -6��Ϊ-8������140.4rpm���ܵ���
			{
				//��������
				sw_p=v_dnave_iacv-v_dnave_iacv_lst;
				

				if(sw_p>10)sw_p=10;
				if(sw_p<-10)sw_p=-10;
				sw_p=(sword)(sw_p*(sword)v_kp_iacv);				
				
				//2017-4-12 �����ź󣬵���Ŀ�������ڼ䣬��������������
				//2019-4-12 ���Ŀ������ڼ��������������
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
				
				
				//���ֿ���
				sw_i=v_dnave_iacv;
				
				if(sw_i>8)sw_i=8;
				if(sw_i<-6)sw_i=-6;
				sw_i=(sword)(sw_i*(sword)v_ki_iacv);
				
				//2017-4-12 �����ź󣬵���Ŀ�������ڼ䣬��������������
				//2019-4-12 ���Ŀ������ڼ��������������
				//2019-4-25 �����ṩ�׶ν�����һ��ʱ���ڻ��ּӿ�
				
				if((inc_rev_backidle==0)&&(v_t_afterbackidle_d1s<100))
				{
					sw_i=sw_i*8;//�ӿ������������һ��
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

			
				//2017-3-9 PI���ƽ��			
				v_steptmp=v_steptmp+v_steptmp_p+v_steptmp_i;
				
				//20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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
	//�����׶�
	//�������ƽ׶�
	{
		
		//if(VF_STARTS_ENG||!VF_MMOVE_ENG)//ͣ���������׶�
		if(((VF_STARTS_ENG&&v_rev_625rpm<25)||!VF_MMOVE_ENG)&&!VF_OPENLOOP_IACV)//ͣ���������׶�	//20210929 chenwei add &&v_rev_625rpm<25
		{
			sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			//tmp_step=(byte)FG_3DMap_xyz(BYPTR_M2_STEPINI,v_rev_15d6rpm,sw_te,BYPTR_N_M2STEPINI,BYPTR_TE_M2STEPINI,7,7,false);
			//tmp_step=(byte)FG_3DMap_xyz(BYPTR_M2_STEPINI,v_rev_15d6rpm,sw_te,BYPTR_N_M2STEPINI,BYPTR_TE_M2STEPINI,1,3,false);//2018-3-21 MAPά�ȵ���Ϊ4X4
			v_stepini=(sword)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);//(sword)F_2DLookUpSbyte((sbyte)sw_te,BYPTR_M1_STEPINI,3); 
			v_steptmp=v_stepini;
			v_step_bcidlimt=v_stepini;
			
		}
		else//�����ɹ���Ŀ������ƽ׶�
		{
			
			VF_OPENLOOP_IACV=true;		//20210929 chenwei add
			v_stepdec_fact=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEPDEC),2);
			//if(v_stepdec_fact>64)v_stepdec_fact=64;	//20210929 chenwei ���Σ��ſ���100
			if(v_stepdec_fact>100)v_stepdec_fact=100;
			
			//w=(word)v_t_eng_d1s;
			//if(b>250)b=250;
			w=(word)v_stepdec_fact*v_t_afterstarts/64;//v_t_eng_d1s
			//if(w>50)w=50;		//20210929 chenwei ���Σ��ſ���80
			if(w>80)w=80;		
			v_steptmp=v_stepini-(sword)w;
			
			//20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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
		
		//��ԭ����
		/*b=14745/(v_step_alt);//15564=128*128*0.95  14745:0.9
		if(b<128)b=128;
		v_steptmp=(sword)(v_steptmp*b/128);*///2018-6-28 ��ԭʵ�鷢������������̫�󣬰��ո�ԭ����ϵ����95%�ٽ�������
		//2020-10-15 ������ԭ����
		step_pair=(char *)BYPTR_M1_STEPINC_PAIR;
		b=F_LinearInter(*(step_pair+v_pair_odr),*(step_pair+v_pair_odr+1),v_pair_mode,16,false);
		v_stepco_pt=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEPCO_PT),3);
		
		v_stepco_pt=(word)v_stepco_pt*b/50;
		
		v_steptmp=v_steptmp+v_stepco_pt;
		
		
		//if(v_steptmp<(sword)STEP_SM_MIN)v_steptmp=STEP_SM_MIN;
		//if(v_steptmp>(sword)STEP_SM_MAX)v_steptmp=STEP_SM_MAX;
		
		//�ջ�����ǰ���ȶ����ٲ�����ֵ
		v_step_idle=v_steptmp;
		

	}
    
	//stw=(sword)v_step_IACV_on+(sword)v_Adj[ADJ_TPDtg];

		//v_step_idle=v_step_IACV_on;
		//
		
	}
	
	
	
//------------------------------2019-3-25 ����뿪���ٺͻص����ٵĴ���---------------------------------

	step_leavidle=(sword)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEINC_LIDLE),1);
	step_backidle=(sword)F_2DLookUpSbyte((sbyte)v_te,(char_P)(BYPTR_M1_STEINC_BIDLE),1);
	
	if(VF_MMOVE_ENG&&!VF_STARTS_ENG&&(v_t_eng_t_s>10)&&VF_NEWCYCLE_ATMAIN_CAS)
	{				
		w=(word)v_step_idle+50;
		if(w>STEP_SM_MAX)w=STEP_SM_MAX;
		
		//�������Ӳ���
		if(!VF_E0_TPS&&VF_STEP_TRANS)
		{		
			if(v_t_afterbackidle_d1s>100)
			{
				v_step_idle=v_step_IACV_on;
			}
		
			if((word)v_steptmp<w)//������Ӳ���������50
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

			if((word)v_steptmp<w)//������Ӳ���������50
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
			if((word)v_steptmp<w)//������Ӳ���������50
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
		
		if(VF_JCI_CFCI&&VF_E0_TPS&&(v_TPS_mode<=7))//2019-4-26 ������ʱ��tpdΪ0������tps_mode�ϴ��Ѿ��ٸ�����
		{
			if((word)v_steptmp<w)v_steptmp+=v_step_bidle;
			if((word)v_steptmp>w)v_steptmp=w;
		}
	
		//������ӵĲ�����˥��
		if(!VF_STEP_STEADY&&!VF_IGNONLY_CFCI)//2019-4-25 �� �����ڼ䲻��
		{
			sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			v_step_bcidlimt=(byte)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);
			v_step_bcidlimt+=(byte)v_stepco_pt;   //20210402 chenwei ��ԭ�궨����v_step_bcidlimt����M1_STEPINI���ң���ԭ��δ���ϸ�ԭ����
			if(v_step_bcidlimt>v_step_idle)v_step_bcidlimt=v_step_idle;
			
			if(v_steptmp>(v_step_bcidlimt+30))//�����ȶ�����30������˥��
			{

				if(VF_RUN8_TMCCL)
				{
					v_steptmp--;
				}
				
				if(VF_RUN2_TMCCL&&(v_dn_iacv<=-8))//ת�ٸ���Ŀ��100r�ӿ�˥��
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
								
					if((v_rev_625rpm<REV62D5_3000)&&VF_RUN4_TMCCL&&(v_dn_iacv<-12))//���С���Ŷ�����������ص���ʱ�������Ӳ��࣬��Ҫ�ӿ첽��˥��
					{
						v_steptmp--;
					}
				}
				
				if(((v_steptmp<(v_step_bcidlimt+10))&&(v_dn_iacv>-8))||((v_t_afterbackidle_d1s>150)&&(v_dn_iacv>-8)&&(v_dn_iacv<5))||(v_steptmp<=v_step_bcidlimt))
				//�жϵ��ٵ�������ȶ���������1�������ﵽ����ǰ�ȶ�����+10,����ת�ٲ�����Ŀ��ת��100rpm  �� 2���ص���ʱ�����15s��ת����Ŀ��ת��-80~120֮��
				{
					VF_STEP_STEADY=true;
				}
			}			
		}

		//if(v_t_afterbackidle_d1s<50)
		if((v_t_afterbackidle_d1s<100)||(v_t_d1s_afterCI<80))	//2021-1-27 �����ٸ��͵�ʱ�����������⻬�к�����С̫��
		{
		/*	sw_te=v_te+40;
			if(sw_te>165)sw_te=165;
			if(sw_te<0)sw_te=0;
			v_step_bcidlimt=(byte)F_2DLookUp((byte)sw_te,BYPTR_M1_STEPINI,3,0,false);
			if(v_step_bcidlimt>v_step_idle)v_step_bcidlimt=v_step_idle;//2021-1-28 ȡstepini��stepidleС���Ǹ���Ϊ����*/
			//if(v_t_afterbackidle_d1s<30)
			if((v_t_afterbackidle_d1s<80)||(v_t_d1s_afterCI<50))	//2021-1-27 �����ٸ��͵�ʱ�����������⻬�к�����С̫��
			{
				//if(v_steptmp<v_step_idle)v_steptmp=v_step_idle;
				if(v_steptmp<v_step_bcidlimt)v_steptmp=v_step_bcidlimt;
			}
			else
			{
				//sw=(sword)v_step_idle-10;
				sw=(sword)v_step_bcidlimt-10;
				//if(sw<10)sw=10;
				if(sw<20)sw=20;//2021-1-27 ��С�����趨Ϊ20
				if(v_steptmp<sw)v_steptmp=sw;
				//if((v_steptmp<v_step_idle)&&(v_dn_iacv>-20))v_steptmp=v_step_idle;
				
			}
			
			//if((v_t_afterbackidle_d1s<15)&&(v_steptmp<(v_step_idle+10)))
		}

		//20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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
	


/*--------------------------------------����Ŀ����ǶԲ����Ŀ���--------------------------------------------
���ԣ�
1���趨���Ŀ��Ϊ���Ǳ����-5
2��������Ǵﵽ�������������ƣ���������15s�����ڲ�����ֱ���ﵽĿ�����

����Ŀ����ǿ��Ƶ�������
1�����������г���1min
2���ܿ����Żص����ڼ�
3�����Ͳ������ﵽ��С(����ת��ƫ�ߵ����)��Ӧ�ÿ��ƣ����Ͳ������ﵽ���(����ת��ƫ�͵����)��Ӧ�ÿ���

2020-11-27 ��������
1����v_Ign_Map+dIgn��Ϊ����Ŀ�����
2����co_IdleIgn_I��Ϊ���ٲ��������������������

*/

if(VF_CLSMODE_IACV&&VF_NEWCYCLE_ATMAIN_CAS)
{
	
	//if(VF_1S_TMCCL&&(v_t_ignlim<255))v_t_ignlim++;

	#ifdef	TE_10K	//20210728 chenwei add 10K����
	if(VF_E0_TPS&&VF_MORE1MIN_TMCCL&&!VF_CICORECING_CFCI&&(v_t_afterbackidle_d1s>=100)&&(v_te>60))
	#endif	

	#ifdef	TE_2K	//20210728 chenwei add 2Kˮ��
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
			if(v_CA_ignition>v_ign_stptg)v_steptmp++;//���Ǵ���Ŀ����ǣ�ͨ������������ʹ�������
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
		
		if((co_IdleIgn_I>6)||(co_IdleIgn_I<-12))//&&(v_step_ignco<=20)&&(v_t_ignlim>=60)���20��,����20������Ҫ���1min�ſ����ٵ���
		{
			if((co_IdleIgn_I>6)&&VF_8S_TMCCL)
			{
				v_steptmp++;//���Ǵ���Ŀ����ǣ�ͨ������������ʹ�������
				v_step_ignco++;
			}
			
			if((co_IdleIgn_I<-12)&&VF_8S_TMCCL)
			{
				v_steptmp--;
				v_step_ignco--;
			}
			
			
			//v_step_ignco++;

			//20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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

	//20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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
    
//------------------------------------2016-12-12 IACV�����������Ĺ�ϵ------------------------------------        
    //��Ҫ�ж�ʲô����²���Ҫ����
    //v_t_IACV_on=F_2DLookUpSbyte((sbyte)v_step_sm,BYPTR_M2_IGC,7);
	v_t_IACV_on=(sbyte)FG_3DMap_xyz((byte *)BYPTR_M2_STEPLAMBDA,v_step_sm,v_rev_15d6rpm,(byte *)BYPTR_STEP_M2LAMBDA,(byte *)BYPTR_N_M2LAMBDA,7,1,0x80);
	
	v_t_IACV_on=(sword)(v_t_IACV_on*v_step_alt)/128;
	
	
	//2018-6-27 ��ԭʵ�鲹��ƫŨ�����Ӹ�ԭ����
	/*if(((v_rev_15d6rpm>=BYPTR_M0_NIACVLM)&&(!VF_STARTS_ENG))||(V_Ta<0))
	{
		v_t_IACV_on=(sword)(v_t_IACV_on*v_step_alt)/128;
	}
	else if(v_Pair_deg>32)
	{
		v_t_IACV_on=0;//���������׶�,��ԭ2000m������ʱ����������
	}*/
	


    //��Ϊ����nstand4ģʽΪ,��ʹ������
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

/*2016-11-19 �ú�������ֵ�����⣬ɾ��
//IACV��Ӧ��LD������
byte Ldinc_byIACV(void)
{
    //return v_step_IACV_on/2;//
    //��Ҫ���е���������
    
        
    return v_t_IACV_on;    
}
*/


/*2016-11-19 �������Ժ󣬵��иߵ�������ʱ�������Ŀ�굡�ټ��㲽�������������ɾ���ú���
    ����Ŀ��ת�٣��ᵽ��������

byte Step_inc(void)
{
    sword sw;
    
    
    if(rev15d6_target>v_n0_low)sw=(sword)rev15d6_target-v_n0_low;
    else sw=0;
    
    sw=sw+sw;
    
    if(sw>40)sw=40;
    
    
    return (byte)sw;

}*/
     
//��ת


/*
    switch(SM_Position)//
    { // 
        case 0: //6
                StepMotorEN1=0; //1B-2B            //��ϸ��Ϣ�μ�PCB�弰Microchip MTS62C19A�����������оƬ
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
	  �ֵ��ڵ������
	  1.���ظߵ��٣��������ֶ����ٸ���ʱ���в�������ֵ���
	  2.��΢�ߵ��٣�ʵ�ʲ����Ѿ���С�����Դ��ڷ������ߵ���:|Ŀ��-ʵ��|>һ����ֵ
	  
	  �жϣ�
	  1.�������ظߵ���ʱ������--�ٸ���--���ͣ��������֣�����Ҫ����ִ�дֵ��ڹ��ܣ�
	  2.��΢�ߵ���ʱ��ÿ������ִ��һ�δֵ��ڣ�ֱ���ظ��������٣�
	  3.�Դֵ��ڴ�����������,��Ҫ��֤һ����ʻѭ���ֵ��ڵĲ������ڵ���ܵ��ڵ��������Χ
	  */

//---------------------------------------------״̬����------------------------------------------------------	  
	  //���������У��������޽�����λ�ù���ʱ����λ�ֵ��ڼ����־
	  if(VF_MMOVE_ENG&&VF_E0_TPS&&(!VFF_TPS_DTC)&&(v_t_afterbackidle_d1s>=100))
		  //2019-3-25 �����ȶ�����10s���ϣ��ֵ��ڶ����Ӳ�����С����
	  {
		  VF_CORACT=true;
	  }
	  else
	  {
		  VF_CORACT=false;
	  }
	  
	  //�ֵ��ڹ��ܼ����¼�������͵�״̬	  
	  if(!VF_CORACT)
	  {
		  VF_CFST=false;
	  }
	  else if(VF_IGNONLY_CFCI)
	  {
		  VF_CFST=true;
	  }
	  
	  //�ֵ��ڹ��ܼ����¼����һ�ζ��ͺ��ٸ��͵�״̬	  
	  if(!VF_CORACT)//||(VF_IGNONLY_CFCI&&VF_CIRECINJST))
	  //if(!VF_CORACT)
	  //2017-4-10 ����������������һ���ٸ��ͺ󣬲��ҵ�ǰΪ����ʱ����Ҫ��λ״̬
	  {
		  VF_CIRECINJST=false;
	  }	  
	  else if(VF_CFST&&(!VF_IGNONLY_CFCI))
	  //else if(VF_CICORECING_CFCI)
	  //2017-4-10  VF_CICORECING_CFCI��Ϊ�ٸ����ж�ʱ�����⣬�������Ϊ��������һ�ζ��ͣ��ҵ�ǰΪ�Ƕ���״̬
	  {
		  VF_CIRECINJST=true;
	  }
	  
	  //�ж���΢�ߵ���
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
	  
//------------------------2019-3-25 ��Իص����������ظߵ��ٴֵ���--------------------------------------------
	  if(!VF_CORADJ&&VF_IGNONLY_CFCI&&VF_CFST&&VF_CIRECINJST&&(v_step_sm>STEP_SM_MIN))
	  {
		  v_step_IACV_on=v_step_IACV_on-M0_STOFCORADJ;
		  
		  VF_CFST=false;
		  VF_CIRECINJST=false;
		  //20210331 chenwei add ��������΢����TPD0.2��0.3�ĶԶ���С��������ֵ
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

//------------------------ ��Ե����������С����Ȼ�ߵ��٣�ʧ�����Ĵֵ���-------------------------------------	  
	  //�ж��Ƿ���Ҫ�ֵ���
	  if(VF_CORADJ&&(v_StepAct==0))
	  {
		  //�ֵ�����ɺ󣬸�λ��־�����쳣������������ִ��PI���ơ�
		  VF_CORADJ=false;
		  v_t_corstp_s=0;
		  VF_CFST=false;
		  VF_CIRECINJST=false;
	  }
	  else if((VF_IGNONLY_CFCI&&VF_CFST&&VF_CIRECINJST&&(v_step_sm==STEP_SM_MIN))||(v_t_corstp_s>M0_TCORSTP)) 	  
	  {
		  //�ֵ��ڵ���������ظߵ��ٻ���΢�ߵ��ٳ���һ��ʱ��
		  VF_CORADJ=true;
		  VF_CFST=false;
		  VF_CIRECINJST=false;
	  }
}

/********************************************************************
*
*�������3��ͣ1��
*
*StepMotor_ST�������������״̬
*StepMotortimer������������ʱ��
*M_StepMotorPowerTime�������������ʱ��
*F_L_R����������˶�����0������  1���ر�
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
					StepMotorEN1=0; //1B-2B            //��ϸ��Ϣ�μ�PCB�弰Microchip MTS62C19A�����������оƬ
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



/********���ܣ�������������������**********
*********ʱ�䣺2021.10.10*******************
*********ʹ�ã�chenwei**********************/
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
			//20211014 chenwei С����Ҳ���ȣ�3500��5���£�
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