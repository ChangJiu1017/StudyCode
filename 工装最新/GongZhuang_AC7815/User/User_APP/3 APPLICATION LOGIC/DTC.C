/*HEAD
 ***********************************************************************
 * Filename	:DTC.c
 * Function	:DTC control module.		
 * User		:xielian
 * Date		:2017.5.25
 * Version	:V8.3.0
 *
 *** History ***
 * V8.1.0	2017.2.17	xielian 
 *		Base function for PI-Motorcycle.
 *
 * V8.2.0	2017.2.17	xielian 
 *		F_ShowMilFunction()函数中将case的处理改为if，否则在map文件中会增加_CASE_CHECKED，导致升级工具无法升级 
 *
 * V8.3.0	2017.5.25	xielian
 *		增加国四MIL灯规则
 *
 * V9.1.0	2017.12.9	yangsili
 *		Base function for PI-EAEA0122.
 *
 * V9.2.0	2017.12.30	yangsili
 *		1.更改喷油、点火诊断函数
 *    2.增加氧加热、油泵诊断函数
 *    3.增加诊断前准备、诊断后操作函数
 *    4.增加诊断需要的一些变量
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "status_st.h"

#include "add.h"

#include "dtc.h"
#include "ee.h"
#include "sys_init.h"
#include "time_ccl.h"//2010-1-18
#include "sci.h"
#include "hard_trig.h"//2014-1-29 peter, delete

#include "ign.h"

#include "status_eng.h"
#include "events.h"
#include "map_selection.h"
#include "StepMotor.h"
#include "wait_moving.h"

#include "frame.h"//2016-2-23

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/

//2018-1-11,ysl,诊断状态
#define DGN_LSD_ALL       0    //全诊断模式
#define DGN_LSD_NOWSTATE  1    //只诊断当前状态故障模式

byte nb_DTC;
//#pragma DATA_SEG DEFAULT
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 


/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 

byte led_status_con;//	EQU	52H	;确定指示灯状态的变量；when	led_status_con=1,	led	should	change	the	status			
byte nb_con_on;     //需要
byte nb_con_off;
byte nb_con;

byte ca_hardware_er_numb;//2008-8-13 add
byte ca_num_er_numb;

//2009-12-11 add for  CA diag
byte nb_qhy_err;//前后沿时间间隔超标的累计的次数；  VF_QHYCA_INTERR_IGN nb_qhy_err t_qhymax_p
byte t_qhymax_p;//前后沿最大时间间隔单位；
byte t_qhymin_p;//前后沿最小时间间隔单位；
byte v_qhCAint_to_prd_m255;     //2009-8-7 add  //2009-12-11 move from ign.c   

byte v_qhCAint_to_prd_m255_lst;     //2009-12-26 add    
word v_flag_odd_qh_asc;
word v_flag_odd_qh_dec;
word v_flag_even_qh_asc;
word v_flag_even_qh_dec;
byte nb_odd_ystdc;
byte nb_even_ystdc;

byte nb_QYTUBIAN;//QIANYAN TUBIAN DE CISHU
word prd_cas_lst_fordtc; //2009-12-18
//2009-12-28
word prd_cas_lst2_fordtc;
word prd_cas_lst_min;
word prd_cas_lst2_min;

word v_hy_qy_lst;//2009-12-26 
//2009-12-11 ADD FOR DIAG INJ
byte nb_inj_open_er;        //inj er 的次数；
byte nb_inj_short_er;       //inj er 的次数；
byte nb_inj_oc_er;          //inj er 的次数；2017-12-11,ysl,add
byte nb_inj_ot_er;          //inj er 的次数；2017-12-11,ysl,add
byte nb_inj_detect;         //2010-3-20 add ;喷嘴被检测的次数

byte nb_airv_open_er;         //2014-10-28
byte nb_airv_short_er;        
byte nb_airv_detect;         

byte nb_coolv_open_er;         //2014-11-27
byte nb_coolv_short_er;        
byte nb_coolv_detect;         

byte nb_ign_open_er;         //
byte nb_ign_short_er;         //
byte nb_ign_oc_er;         //2017-12-11,ysl,add
byte nb_ign_detect;         //

byte nb_pump_open_er;        //pump er 的次数；2017-12-11,ysl,add
byte nb_pump_short_er;       //pump er 的次数；2017-12-11,ysl,add
byte nb_pump_oc_er;          //pump er 的次数；2017-12-11,ysl,add
byte nb_pump_ot_er;          //pump er 的次数；2017-12-11,ysl,add
byte nb_pump_detect;         //被检测的次数 2017-12-11,ysl,add

byte nb_o2h_open_er;        //o2h er 的次数；2017-12-11,ysl,add
byte nb_o2h_oc_er;          //o2h er 的次数；2017-12-11,ysl,add
byte nb_o2h_detect;         //被检测的次数 2017-12-11,ysl,add

//bool v_diag33814ResetCas;     //2018-1-12,ysl,add
//bool v_diag33814ResetPrd;     //2018-1-12,ysl,add

byte nb_iacv_c1_open_er;        //IACV线圈1的开路er次数；2018-06-23
byte nb_iacv_c2_open_er;        //IACV线圈2的开路er次数；2018-06-23
byte nb_iacv_short_er;       //IACV线圈的短路er次数；2018-06-23
byte nb_iacv_detect;         //被检测的次数 2018-06-23

#define NB_INJ_LIMIT    3//2010-1-1 change from 60 to 3          
#define STEP_QHCAINT    2
#define NB_PUMP_LIMIT   9
#define NB_MIL_LIMIT  5//20210308 chenwei add MIL诊断限值


byte v_AD_cas;//2016-2-28 cas采样值；8bit 2016-2-28
byte nb_AD_cas_err;//cas异常的次数；2016-2-28

byte nb_hy_lack;        //缺少后沿的次数

//====2008-11-14 
byte numbErr[40];//本次ECU上电后，对应故障号下的故障次数；
                 //索引号为：故障号-1          numbErr[b]>M2_DTC_HISTORY[b][1]

//2010-7-2 凸台 判读 采用平均值 ；
byte nb_CA_Ave;
word sum_CA_Int;
byte v_qhCAint_ave;

byte v_CA_break;

//20210923 chenwei add for 环松仪表

#define	DTC_T_INTERVAL_3D2S			25		//环松：定义两个故障间隔时间 3.2s
#define	NUMERICAL_T_INTERVAL_1D2S	7//环松，定义同一故障码内，数值之间的时间间隔	1.2s
#define	MIL_ON_T_INTERVAL_D4S			1//环松，定义同一数值，MIL_ON与MIL_OFF的时间时间，当前一致，均为0.4s
#define	MIL_OFF_T_INTERVAL_D4S			1//环松，定义同一数值，MIL_ON与MIL_OFF的时间时间，当前一致，均为0.4s
#define	T_FROMKEYON_1S					120	//环松，距离开钥匙时间，120s

word v_t_MILGliter_d1s;			//单个故障MIL信号闪烁的时间
word v_pcode_lst;				//环松MIL：上一次执行的故障码




word v_pcode_lookup;			//环松，读取的故障码
byte nb_pcode;					//故障数组中，故障码个数
word v_SPNX_forhuansong[NUM_DTC_SCR];	//存放历史故障

#define PCODE_SYSINIT 0x1111		//20211006 chenwei add 初始化故障码

byte v_pair_ad_input_keyon;		//开钥匙压力输入存储
byte v_pair_ad_input_lst;
byte v_pair_ad_input_max_nmov_eng;	//未启动起见压力采样的最大值
byte v_pair_ad_input_min_nmov_eng;	//未启动起见压力采样的最小值

byte v_Vbat_inuse_keyon;		//开钥匙电压存储
byte v_Vbat_inuse_lst;
byte v_Vbat_inuse_max_nmov_eng;	//未启动起见电压采样的最大值
byte v_Vbat_inuse_min_nmov_eng;	//未启动起见电压采样的最小值

byte VF_NOCA_DIAG;
byte nb_noca_test;		//判断到可能无角标信号的次数


byte test_sb1;
byte test_sb2;
#define P_CrankDeltaMAP_Thrsh	70//107		//压降5kpa(借鉴德尔福)
#define U_CrankDeltaIgn_Thrsh	10		//压降1v(借鉴德尔福)，

#define P_CrankDeltaMAP_NMOVE_Thrsh		3
#define U_CrankDeltaIgn_NMOVE_Thrsh	5//2		//20211010 经过测试，电启动之后，压降大部分3、4，设置为2可能较为苛刻
#define NB_DTC_NOCA	3		//检测故障次数


//word v_DTC_histrory[NUM_DTC_SCR];		//存放历史故障

byte VF_PCODE_INSPNX_forhuansong;		//判断v_SPNX_forhuansong[]中是否已经有某一PCODE

void MIL_ACTION_FORHUANSONG(word pcode);
void SAVE_PCODE_FOROBD4(word pcode);
void DTC_DiagNoCA_def(void);		//20211009 chenwei add


/***20210923 chenwei add 环松MIL显示方式****/
byte TEST_b4,TEST_b3,TEST_b2,TEST_b1;
byte VF_MIL_SWITCH;
byte nb_mil_interval;	//mil信号切换的次数
byte VF_MIL_DTC4;		//MIL执行故障码从右往左第4位
byte VF_MIL_DTC3;		//MIL执行故障码从右往左第3位
byte VF_MIL_DTC2;		//MIL执行故障码从右往左第2位
byte VF_MIL_DTC1;		//MIL执行故障码从右往左第1位



//====对应故障号，以及相应故障需要累加到EE中的次数底线
const byte M2_DTC_HISTORY[40][2] =
{   //故障历史存储位置，累加故障的存储底线
    //索引号为：故障号-1
    SUM_BATTER_ER,200,
    SUM_NOOIL_ER, 200,
    SUM_INJ_SET_ER,1,
    SUM_INJECTOR_ER,2,
    SUM_INIT_ER, 1,
    SUM_RZT_ER,  200,
    SUM_O2_ER,   1,//os
    SUM_JQM_ER,  100,
    EE_DTC_9,    10,
    EE_DTC_10,   10,
    EE_DTC_11,   10,
    EE_DTC_12,   10,
    EE_DTC_13,   10,
    EE_DTC_14,   10,
    EE_DTC_15,   1,//VFF_TENGHIGH_DTC
    EE_DTC_16,   1, //ISCV
    EE_DTC_17,   100, //VFF_OSFK_DTC
    EE_DTC_18,   10,
    EE_DTC_19,   10,
    EE_DTC_20,   10,
    EE_DTC_21,   10,
    EE_DTC_22,   10,
    EE_DTC_23,   10,
    EE_DTC_24,   10,
    EE_DTC_25,   10,
    EE_DTC_26,   10,
    EE_DTC_27,   10,
    EE_DTC_28,   10,
    EE_DTC_29,   10,
    EE_DTC_30,   10,
    EE_DTC_31,   10,
    EE_DTC_32,   10,
    EE_DTC_33,   10,
    EE_DTC_34,   10,
    EE_DTC_35,   10,
    EE_DTC_36,   10,
    EE_DTC_37,   10,     //pair
    EE_DTC_38,   10,
    EE_DTC_39,   10,
    EE_DTC_40,   10,
}; 

byte nb_led;//2009-8-1 add 
byte prd_led;//if(nb_led>=prd_led)nb_led=0; prd_led与显示码有关；
byte lst_code;//
byte cur_code;// if( lst_code==cur_code)nb_led=0;

/*------------DTC_ACT--------------------*/


//关于喷射的参数
word v_t_bInj_toTrig;       //的触发时刻到喷射时刻的时间
word v_t_bInj_toCas;        //的触发用的角标的时刻到喷射时刻的时间
word v_t_bInj_toCas_tmp;        //的触发用的角标的时刻到喷射时刻的时间

word v_t_T1;

word v_t_bInj_toTrig_r;    //实际的触发时刻到喷射时刻的时间
word v_t_bInj_toCas_r;     //实际的触发用的角标的时刻到喷射时刻的时间
word v_t_T1_r;

word v_t_Cas_trigT1;       //触发T1的角标的时间
word v_t_trigT1;           //触发T1比较输出的时刻

//关于点火的参数
word v_t_bIgn_toTrig;       //的触发时刻到点火时刻的时间
word v_t_bIgn_toCas;        //的触发用的角标的时刻到点火时刻的时间
word v_t_IgnOn;				//20210602 cw remark充磁时间

word v_t_IgnOn_LookUp;		//20210602 cw add 查表得到的充磁时间


word v_t_bIgn_toTrig_r;    //时间的触发时刻到点火时刻的时间
word v_t_bIgn_toCas_r;     //实际的触发用的角标的时刻到点火时刻的时间
word v_t_IgnOn_r;

word v_t_Cas_trigIgn;      //触发点火的角标的时间
word v_t_trigIgn;          //触发点火比较输出的时刻
word v_t_IgnCharge_r;       // 实际点火充电的时刻

word v_tl_IgnPreCharge;      //准备点火充电的时间
word v_tl_IgnPreCharge_r;      //准备点火充电的实际时间

//关于执行的次数
//word v_nb_ign_charge;             //触发点火的次数
word v_nb_ign;             //触发点火的次数
word v_nb_inj;             //触发喷油的次数
word v_nb_ignOn_r;           //真实的点火的次数
word v_nb_injOn_r;           //真实的喷油的次数
word v_nb_ignOff_r;           //真实的点火的次数
word v_nb_injOff_r;           //真实的喷油的次数


word v_t_injOn_r;              //INJ_ON的时刻
word v_t_injOff_r;              //INJ_Off的时刻
word v_t_IgnSetOn_r;              //IGN_ON的时刻


sword v_dt_bInj_toCas;
sword v_dt_T1;
sword v_dt_bIgn_toCas;

sword v_dnb_Ign;
sword v_dnb_Inj;
sword v_dnb_T1;                             

sword v_dnb_Ign_YSTDC;
sword v_dnb_Inj_YSTDC;

word v_nb_YSTDC;
word v_nb_IgnInt;   //点火中断的次数

byte v_nb_mmove_eng;
byte VF_STARTS_ENG_LST;

#define M0_NB_CLEARDTC	4	//2017-6-6 启动多少次清除故障

byte nb_Mv[4][2]; //MV最值的类型；0-最小值，1最大值；//2016-2-28


byte v_SPNX_forMIL[8][3]; //2015-11-29 
    /*2015-9-8 专门为MIL设置的锁定故障码用
        2015-9-20 add [2] 0: for frozen frame, 1: for 本次开钥匙发生过的
        MIL 灯是否点亮，由0 和 1一起决定；
    */

byte v_33814LdoOnOff; //2017-12-29,ysl,add
byte b_Count;


//2018.6.23 ysl 怠速步进电机短路诊断使用变量
word v_dgnIacvCoil1AdAv;     //线圈1反馈电压AD采集平均值
word v_dgnIacvCoil2AdAv;     //线圈2反馈电压AD采集平均值
word v_dgnIacvCoil1AdMaxAv;  //线圈1反馈电压AD采集几个最大值平均值
word v_dgnIacvCoil2AdMaxAv;  //线圈2反馈电压AD采集几个最大值平均值
//不易采集到最大值，该数组可以小一些，对应于200的采集点数，虽然是10位采集，但一般都不会很大，所以可以使用byte类型
byte v_dgnIacvCoil1AdMax[5]; //线圈1反馈电压AD采集最大值存放数组
byte v_dgnIacvCoil2AdMax[5]; //线圈2反馈电压AD采集最大值存放数组
byte v_dgnIacvAdcCount;      //AD采集次数计数
byte v_dgnIacvCoil1Ad0Count; //线圈1反馈电压AD采集为0的次数统计
byte v_dgnIacvCoil2Ad0Count; //线圈2反馈电压AD采集为0的次数统计
//短路检测采集的点数
#define IACV_SMPLE_POINT  200  

//20210308 chenwei add
byte nb_MIL_open_er;        //MIL er 的次数；20210308 chenwei add
byte nb_MIL_oc_er;          //MIL er 的次数；20210308 chenwei add
byte nb_MIL_detect;         //被检测的次数 20210308 chenwei add

byte Chk_Mv_failure(byte mv_type,byte limit_type);//2016-2-28    
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void sum_er_inc(byte add);
void Accu_NB_er(byte erNO);
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 






void FI_ActDTC_afterReset(void)
{
	byte i;		//20211004 chenwei add
	
    v_nb_ign=0;             //触发点火的次数
    v_nb_inj=0;             //触发喷油的次数
    v_nb_ignOn_r=0;           //真实的点火的次数
    v_nb_injOn_r=0;           //真实的喷油的次数
    v_nb_ignOff_r=0;           //真实的点火的次数
    v_nb_injOff_r=0;           //真实的喷油的次数
    v_nb_YSTDC=0;   
    v_nb_ccl_r=0;
    v_nb_IgnInt=0;
	
	b_Count=0;
	
		VF_O2H_OPEN=false;  //20210308 chenwei add
		VF_O2H_OC=false;    //20210308 chenwei add
		VF_MIL_OCOROPEN=false;	//20211007 chenwei add

	nb_pcode=0;		//20211004 chenwei add

	if(VF_VPOR_HARD)	//20211004 chenwei 调整
	{
		for(i=0;i<NUM_DTC_SCR;i++)		
		{
			v_SPNX_forhuansong[i]=0;
		}
	}

	
	
    
}

//计算执行器执行的参数的误差
void FC_ACT_Err(void)
{
            
}

//功能：关钥匙时累加故障码到历史故障次数中；
//      当led_status_con>=8时，累加故障次数，并通过调用sum_er_inc，将EE中相应的故障数值累加并保存；
//      sum_er_inc的输入参数是W,也就是W值是地址；
//      在检测到要关钥匙时，被调用；
void FM_SumError_WhenKeyOff(void)
{
    byte b;
    
    
    if(led_status_con<8)return;
    led_status_con=0;

    for(b=0;b<40;b++)
    {
        if(numbErr[b]>=M2_DTC_HISTORY[b][1])
        {
            sum_er_inc(M2_DTC_HISTORY[b][0]);    
        }
    }
    return;  
}

#if SYS==SYS_PI

#pragma CODE_SEG MAX_ROM

    //功能：喷油器开路或短路诊断；并得到code_number,binjector故障码
   

    //2017-12-9,ysl,mod 功能：喷油器开路或短路诊断；并得到code_number,binjector故障码
    //全部诊断时关闭喷油器诊断开路时，需要延时400us，而开启喷油器诊断短路时，需要延时90us.具体请查看33814技术文档16页，下同
    void F_Diag_INJ(byte b_state)
    {
        word b;
        
        mc33814_ReadStatus_def(_0000_INJ1_FAULT);
        
        if(b_state == DGN_LSD_ALL)//全诊断
        {
            if(PORTF_INJO)//如果喷油器开启则关闭喷油器以检测OL
            { 
                //喷油器过流
                if(P_33814INJ1_OC == 1)
                {
                    if(nb_inj_oc_er<255) nb_inj_oc_er++;
                } else nb_inj_oc_er=0;

                INJ_OFF
                for(b=0;b<=1000;b++);
                mc33814_ReadStatus_def(_0000_INJ1_FAULT);
                INJ_ON 
                
                if(P_33814INJ1_OPEN == 1)  //2009-12-18 delete ||VF_SHORT_INJCTL
                {
                    if(nb_inj_open_er<255) nb_inj_open_er++;
                } else nb_inj_open_er=0;//2016-3-3 add   
            }
            else
            {
                if(P_33814INJ1_OPEN == 1)  //2009-12-18 delete ||VF_SHORT_INJCTL
                {
                    if(nb_inj_open_er<255) nb_inj_open_er++;
                } else nb_inj_open_er=0;//2016-3-3 add   

                INJ_ON
                for(b=0;b<=260;b++);
                mc33814_ReadStatus_def(_0000_INJ1_FAULT);
                INJ_OFF
                
                //喷油器过流
                if(P_33814INJ1_OC == 1)
                {
                    if(nb_inj_oc_er<255) nb_inj_oc_er++;
                } else nb_inj_oc_er=0;
            }
        }
        else    //只诊断当前状态故障模式
        {
            //喷油器过流
            if(P_33814INJ1_OC == 1)
            {
                if(nb_inj_oc_er<255) nb_inj_oc_er++;
            } else nb_inj_oc_er=0;
            
            if(P_33814INJ1_OPEN == 1)  //2009-12-18 delete ||VF_SHORT_INJCTL
            {
                if(nb_inj_open_er<255) nb_inj_open_er++;
            } else nb_inj_open_er=0;//2016-3-3 add 
        }
        
        
        //喷油器过热
        if(P_33814INJ1_OT == 1) 
        {
            if(nb_inj_ot_er<255)nb_inj_ot_er++;
        } else nb_inj_ot_er=0;

        //SG
        if(P_33814INJ1_SHORT == 1)
        {
           // VF_SHORT_INJCTL=true;//如果喷油器检测端口为高电平；判定晶体管断路；
           if(nb_inj_short_er<255)nb_inj_short_er++;
        } else nb_inj_short_er=0;//2016-3-3 add
        

        if(nb_inj_open_er>NB_INJ_LIMIT) 
        {
            //VF_ER_INJCTL=true;
            VF_OPEN_INJCTL=true;     //2010-1-1 mos 短路
            VF_OPEN_33814INJ1CTL=true;
        }
        else 
        {
            VF_OPEN_INJCTL=false;
            VF_OPEN_33814INJ1CTL=false;
        }
        
       //2009-12-18 将短路归结为ECU故障 
        if(nb_inj_short_er> NB_INJ_LIMIT)
        {
            VF_SHORT_INJCTL=true;    //2010-1-1 mos 开路
            VF_SHORT_33814INJ1CTL=true;
        }
        else 
        {
            VF_SHORT_INJCTL=false;
            VF_SHORT_33814INJ1CTL=false;
        }
        
        if(nb_inj_oc_er > NB_INJ_LIMIT)
        {
            VF_OC_33814INJ1CTL=true;    //2010-1-1 mos 开路
        } else VF_OC_33814INJ1CTL=false;
        
        if(nb_inj_ot_er > NB_INJ_LIMIT)
        {
            VF_OT_33814INJ1CTL=true;    //2010-1-1 mos 开路
        } else VF_OT_33814INJ1CTL=false;
        
        if(nb_inj_detect<255)nb_inj_detect++;
        //即当喷油器开路或晶体管短路也好，置喷油器故障；
        
        if(P_33814INJ1_ST != 0)
        {
            mc33814_SPIRwWord_def(_4000_WRITE_STATUS, _0000_INJ1_FAULT, 0x0U);
        }
        
    }
    
    
    
    //功能：DTC参数的初始化
    void FI_DTC_AfterReset(void)
    {
        byte b;
        word w;
        //-----Init_ER_Count_AfterReset--------------------:
        //复位后，部分故障累加值的初始化
        //MIL_ON;                             //开启指示灯
        MIL_OFF;//2009-8-4 CHANGE 
        //-----CLR_FAULT_NUM_AFTER_PONRESET;;//注意：由于语法的关系，CLR_FAULT_NUM_AFTER_PONRESET，只是解释部分移过来，而实际程序部分没动；
        //上电复位后，当前故障数的清零
    	if(VF_VPOR_HARD)
    	{
    	    for(b=0;b<40;b++)
    	    {
    	        numbErr[b]=0;
    	    }
    	}

        //-----Init_Code_PORESET:
        //上电复位后部分故障码的初始化	
    	if(VF_VPOR_HARD)
    	{
            led_status_con=0;
            //V_DTC_STATUS=false;//2008-12-15
		VFF_VBAT_DTCbak = 0;         //1
		VFF_T3ER_DTCbak = 0;         //2
		VFF_ECUNOTINIT_DTCbak = 0;     //5  //2009-3-27 b4 to b2
		VFF_INJ_DTCbak = 0;              //4
		VFF_SYSNOTINIT_DTCbak = 0;     //3  //2009-3-27 b2 to b4
		VFF_TES_DTCbak = 0;             //6
		VFF_OS_DTCbak = 0;             //7
		VFF_TPS_DTCbak = 0;
            //V_DTCH_STATUS=false;//2008-12-15
		byte VFF_FUEL_DTCbak = 0;          //9
		byte VFF_INJCTL_DTCbak = 0;         //10
		byte VFF_ECUINIT_DTCbak = 0;          //11
		byte VFF_CAECU_DTCbak = 0;           //12
		byte VFF_ECUPWRON_DTCbak = 0;      //13
		byte VFF_TAS_DTCbak = 0;             //14
		byte VFF_TENGHIGH_DTCbak = 0;     //15
		byte VFF_ISCV_DTCbak = 0;         //16
            //V_DTCHL_STATUS=false;//2008-12-15
		VFF_OSFK_DTCbak = 0;         //17
		VFF_SAV_DTCbak = 0;             //18
		VFF_EGR_DTCbak = 0;              //19
		VFF_TWC_DTCbak = 0;             //20
		VFF_MAPS_DTCbak = 0;         //21
		VFF_AMF_DTCbak = 0;               //22
		VFF_IGN_COIL_SPNbak = 0;              //22
		VFF_VBATSP_ERRbak = 0; 
            //V_DTCHH_STATUS=false;//2008-12-15
		VFF_CYL1INJ_DTCbak = 0;           //33
		VFF_CYL2INJ_DTCbak = 0;           //34
		VFF_CYL1OS_DTCbak = 0;           //35
		VFF_CYL2OS_DTCbak = 0;           //36
		VFF_PAIR_DTCbak = 0;         //37
		VFF_IDLESCLIMIT_DTCbak = 0;      //38
		VFF_7OFDIAG5_DTCbak = 0;      //39
		VFF_8OFDIAG5_DTCbak = 0;      //40
            //V_DTCHHL_STATUS=false;
		VFF_NOZZLEINI_TESHIGH_DTCbak = 0;
		VFF_NOZZLEINI_T1DGOT_DTCbak = 0;
		VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak = 0;
		VFF_NOZZLEINI_ABCGOT_DTCbak = 0;
		VFF_NOZZLEINI_ABCSAVED_DTCbak = 0;
		VFF_NOZZLEINI_NOTDOM_DTCbak = 0;
		VFF_NOZZLEINI_DOMFAILURE_DTCbak = 0;
		VFF_NOZZLEINI_NOTINI_DTCbak = 0;
            //V_DTCHHH_STATUS=false;
		VFF_NOZZLEINI_VBATNOTDOM_DTCbak = 0;//:ECU电压没有dom
		VFF_NOZZLEINI_VBATVALID_DTCbak = 0;//:电压过低导致无法初始化；
		VFF_NOZZLEINI_NOINJ_DTCbak = 0;//:喷油器故障导致无法初始化；
		VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak = 0;//扫描结果的T4<T1d+500，认为结果有问题
		VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak = 0;//T1D扫描之后，计算得到的系数非法；2009-3-5
		VFF_NOZZLEINI_NOT4_DTCbak = 0;//2009-11-29 无t4,导致无法初始化；
            nb_qhy_err= ca_num_er_numb =ca_hardware_er_numb=0;    //2009-12-11
    	      nb_inj_open_er=0;//2009-12-11
    	      nb_inj_short_er=0;
    	      nb_inj_oc_er=0;//2017-12-11,ysl,add
    	      nb_inj_ot_er=0;//2017-12-11,ysl,add
    	      
    	      nb_ign_open_er=0;//2009-12-11
    	      nb_ign_short_er=0;
    	      nb_ign_oc_er=0;//2017-12-11,ysl,add
    	      
    	      nb_pump_open_er=0;        //pump er 的次数；2017-12-11,ysl,add
            nb_pump_short_er=0;       //pump er 的次数；2017-12-11,ysl,add
            nb_pump_oc_er=0;          //pump er 的次数；2017-12-11,ysl,add
            nb_pump_ot_er=0;          //pump er 的次数；2017-12-11,ysl,add

            nb_o2h_open_er=0;        //o2h er 的次数；2017-12-11,ysl,add
            nb_o2h_oc_er=0;          //o2h er 的次数；2017-12-11,ysl,add

            VF_33814DIAGCAS_GEN=false;     //2018-7-13,ysl,add
            VF_33814DIAGPRD_GEN=false;     //2018-7-13,ysl,add
            
            nb_iacv_c1_open_er=0;        //IACV线圈1的开路er次数；2018-06-23
            nb_iacv_c2_open_er=0;        //IACV线圈2的开路er次数；2018-06-23
            nb_iacv_short_er=0;       //IACV线圈的短路er次数；2018-06-23

    	      VF_SHORT_IGN=false;
    	      VF_OPEN_IGN=false;
    	      
    	      nb_AD_cas_err=0;
    	      
    	      V_CAS_STATUS=false; //2014-4-17
			  VF_OBDVER_FAI=false;
			  nb_led=0;
    	      
    	      v_dgnIacvAdcCount=0;  //2018-6-29,ysl,add
    	}

    	  
        //2009-12-11 move from ign.c
        w=(word)BYPTR_M0_CAS_FRONTEDGE-BYPTR_M0_CAS_TRAILEDGE;
                //2010-6-29 对于凸台长度为40度的车，标准值为114；

        w=w*2;
        v_qhCAint_to_prd_m255_lst=v_qhCAint_to_prd_m255=w+8;

        if (BYPTR_MV_VEHICLE== WILD)   //2011-4-8
		{
			t_qhymax_p=w+16;

            t_qhymax_p=w+26;
            t_qhymin_p=w-32;   //2012-1-30  FOR SONGYING 200
		}            
        else
		{
			t_qhymax_p=w+20;

            t_qhymin_p=w-12;   //2011-6-13  FROM -8 TO -12
		}
            

        
        v_flag_odd_qh_asc=v_flag_odd_qh_dec=v_flag_even_qh_asc=v_flag_even_qh_dec=0;
        nb_odd_ystdc=nb_even_ystdc=0;
        nb_QYTUBIAN=0;
        nb_inj_detect=0;
        
        nb_CA_Ave=0;    //nb_CA_Ave sum_CA_Int
    	  nb_hy_lack=0;

        v_hy_qy_lst=0;


        //2016-2-28 ADD 
          Accu_AD_Mv(MV_PAIR,MV_NORMAL);
          Accu_AD_Mv(MV_TES,MV_NORMAL);
          Accu_AD_Mv(MV_TAS,MV_NORMAL);
          Accu_AD_Mv(MV_TPS,MV_NORMAL);


        //2015-6-5
        if(VF_VPOR_HARD)
        {
            V_SPN0_STATUS=0;
            V_SPN1_STATUS=0;
            V_SPN2_STATUS=0;
            V_SPN3_STATUS=0;
            V_SPN4_STATUS=0;
            V_SPN5_STATUS=0;
            V_SPN6_STATUS=0;
            V_SPN7_STATUS=0;
            
            //2015-9-8
            for(b=0;b<=2;b++){
              
                v_SPNX_forMIL[0][b]=0;
                v_SPNX_forMIL[1][b]=0;
                v_SPNX_forMIL[2][b]=0;
                v_SPNX_forMIL[3][b]=0;
                v_SPNX_forMIL[4][b]=0;
                v_SPNX_forMIL[5][b]=0;
                v_SPNX_forMIL[6][b]=0;
                v_SPNX_forMIL[7][b]=0;
            }

        }


        return;
        
    }

    //2016-2-28
    void Accu_AD_Mv(byte mv_type,byte limit_type)
    {
        //MV类型
        if(limit_type==MV_U_LIMIT)
        {
            if(nb_Mv[mv_type][MV_U_LIMIT]<255)nb_Mv[mv_type][MV_U_LIMIT]++;
            nb_Mv[mv_type][MV_L_LIMIT]=0;
        } 
        else if(limit_type==MV_L_LIMIT)
        {
            if(nb_Mv[mv_type][MV_L_LIMIT]<255)nb_Mv[mv_type][MV_L_LIMIT]++;
            nb_Mv[mv_type][MV_U_LIMIT]=0;
        } 
        else
        {
            nb_Mv[mv_type][MV_L_LIMIT]=0;
            nb_Mv[mv_type][MV_U_LIMIT]=0;
        }
        
        
    }
    
    //2016-2-28 判断
    byte Chk_Mv_failure(byte mv_type,byte limit_type)
    {
        if((nb_Mv[mv_type][limit_type]) >50)return true;
        else return false;
    }

    //*************DiagAndDisp*********************************
    //功能：诊断并指示；
    void F_DiagDisp(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy)
    {
        //2008-8-1 诊断角标
        
        //诊断并指示
    	F_SumFailure();//test v_Teng_inuse> rzt_toohot or not, test is oii no?, in sub4
    			//检测温度传感器（缸盖和进气）是否有误，以决定是采用采样值还是预测值；
    			//判断TPS是否有误；
    			//对5种故障的当前故障数目进行累加；
    	
    	F_ShowMilFunction();//故障指示;由led_status_con和实际故障情况决定；
    	

    	return;
    }

    //2008-8-13
    //诊断角标
    void F_Diag_CA(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy)
    {
        long int l;
        word w;
        byte FB;
        byte YSTDC_GET;
        word wCON;
        byte bTUBIAN;
        byte bPcaH;
        byte b;
        //if(nb_hy_inqy!=1||nb_qy_inhy!=1)VFF_CANUM_DTC=true;            //该故障在发动机运转时会保持，发动机停止之后清楚；
        //if(nb_hy_inqy==0)VFF_2ND_DTC=true;else VFF_2ND_DTC=false;//时刻更新 

        bPcaH=(byte)(prd_cas>>8);
		v_CA_break=false;

        /*---判断后沿之间的前沿的数量和前沿之间后沿的数量---*/
        //2013-5-13 add condcition if(v_rev_625rpm<REV62D5_7000)
        if(VF_MMOVE_ENG&&(v_rev_625rpm<REV62D5_7000))
        {
    	/*-------2009-8-7 add-------------*/
    	    bTUBIAN=false;
    	    if(!VF_BFIRE_ENG)
    	    {
    	        prd_cas_lst2_fordtc=prd_cas_lst_fordtc=prd_cas;
    	        prd_cas_lst_min=0;
    	    } else
    	    {   //2009-12-29 add condiction
        	    if((bPcaH<PCA2929)&&VF_TRANSLT10_TRANS&&!VF_CICORECING_CFCI)   //2009-12-25 add condiction
        	    {       //2010-7-2 jiuzheng
            	    l=prd_cas_lst_fordtc/4+(long int)prd_cas_lst_fordtc+prd_cas_lst_fordtc/2;
            	    if(prd_cas>l)
            	    {
            	       bTUBIAN=true;
            	    }
        	    } 
        	    prd_cas_lst_min=prd_cas_lst_fordtc-prd_cas_lst_fordtc/4-prd_cas_lst_fordtc/8; //2009-12-29  -prd_cas_lst_fordtc/8
        	    
        	    if(prd_cas<prd_cas_lst_min&&prd_cas_lst2_fordtc< prd_cas_lst_min)
        	    {
        	        bTUBIAN=true;
        	    }

    	    }
    	    //前沿丢失之后，重新再找
    	    if(bTUBIAN)
    	    {
    	        VF_QYTUBIAN_CAS=true;
    	        VF_YSTDCGET_INJPHASE=false;
    	        nb_QYTUBIAN=nb_QYTUBIAN+10;        
                v_flag_odd_qh_asc=v_flag_odd_qh_dec=v_flag_even_qh_asc=v_flag_even_qh_dec=0;
				v_CA_break=true;
    	    }
    	    prd_cas_lst2_fordtc=prd_cas_lst_fordtc;
    	    prd_cas_lst_fordtc=prd_cas;
        /*-------------判断后沿和前沿的时间间隔------------------------------*/
            //2009-12-11 move from ign.c
            

                w=t_cas_hy-t_cas;
                if(v_hy_qy_lst==0) v_hy_qy_lst=w;
                l=(word)((((long int)v_hy_qy_lst)*1024)/prd_cas);  //2010-7-2
                v_hy_qy_lst=w;
                
                if(l>255)
                {
                    v_qhCAint_to_prd_m255=255;        
                } else v_qhCAint_to_prd_m255=(byte)l;
                

            	
            	//if(!VF_YSTDCGET_INJPHASE&&VF_HAVEHY_CAS)//ADD VF_HAVEHY_CAS
            	//if(!VF_HYER_CAS&&VF_HAVEHY_CAS)//ADD VF_HAVEHY_CAS
            	{
              	    if(VF_YSTDCGET_INJPHASE||VF_QYTUBIAN_CAS)wCON=0xffff; //2009-12-28 add condiction VF_QYTUBIAN_CAS
              	    else wCON=0x001f;
              	    YSTDC_GET=VF_YSTDCGET_INJPHASE;		
    /*          	    if(!VF_YSTDCGET_INJPHASE ||(VF_YSTDCGET_INJPHASE&&VF_NEEDRECBYLOWRPM_IGN
              	                    &&!VF_TRANSGT3_TRANS
              	                    &&(v_qhCAint_to_prd_m255_lst<=t_qhymax_p)   //2009-12-29 add condiction
              	                    &&(v_qhCAint_to_prd_m255_lst>=t_qhymin_p)
              	                    ))   */
              	    if( !VF_YSTDCGET_INJPHASE/*||
              	        ((VF_TRANSLT10_TRANS)&&(v_qhCAint_to_prd_m255_lst<=t_qhymax_p)   //2009-12-29 add condiction
              	                    &&(v_qhCAint_to_prd_m255_lst>=t_qhymin_p)
              	                    )*/)
              	    {
              	        if(VF_RUN2_TMCCL)
                        {   
                      	    FB=VF_YSTDC_INJPHASE;
                            if(((v_flag_odd_qh_asc&wCON)==wCON)&&((v_flag_even_qh_dec&wCON)==wCON))
                            {            //2009-12-29 注意运算优先级的语法
                                VF_YSTDC_INJPHASE=true; 
                                VF_YSTDCGET_INJPHASE=true; 
                                nb_odd_ystdc++;  
                            }
                            v_flag_odd_qh_asc=v_flag_odd_qh_asc<<1;      //255
                            if(v_qhCAint_to_prd_m255>v_qhCAint_to_prd_m255_lst+STEP_QHCAINT)v_flag_odd_qh_asc|=0x0001;
                            else v_flag_odd_qh_asc&=0xfffe;
                            v_flag_odd_qh_dec=v_flag_odd_qh_dec<<1;      //255
                            if(v_qhCAint_to_prd_m255+STEP_QHCAINT<v_qhCAint_to_prd_m255_lst)v_flag_odd_qh_dec|=0x0001;
                            else v_flag_odd_qh_dec&=0xfffe;
                        } else
                        {
                      	    FB=VF_YSTDC_INJPHASE;
                            if(((v_flag_odd_qh_dec&wCON)==wCON)&&((v_flag_even_qh_asc&wCON)==wCON))
                            {
                                VF_YSTDC_INJPHASE=true; 
                                VF_YSTDCGET_INJPHASE=true;   
                                nb_even_ystdc++;
                            }
                            v_flag_even_qh_asc=v_flag_even_qh_asc<<1;
                            if(v_qhCAint_to_prd_m255>v_qhCAint_to_prd_m255_lst+STEP_QHCAINT)v_flag_even_qh_asc|=0x0001;
                            else v_flag_even_qh_asc&=0xfffe;
                            v_flag_even_qh_dec=v_flag_even_qh_dec<<1;
                            if(v_qhCAint_to_prd_m255+STEP_QHCAINT<v_qhCAint_to_prd_m255_lst)v_flag_even_qh_dec|=0x0001;
                            else v_flag_even_qh_dec&=0xfffe;        //1
                        }


                   			if((FB!=VF_YSTDC_INJPHASE)&&!FB)VF_SWITCHPHASE_INJPHASE=true;
                   			if(YSTDC_GET&&(FB!=VF_YSTDC_INJPHASE))VF_PHASEER_CAS=true;   //2009-12-27
              	        //2010-1-26
              	        //在！VF_NOTDELAY_IGN，且未断油情况下，连续16圈
              	        /*if()
              	        {
              	            VF_YSTDCGET_INJPHASE=false;    
              	        } */
              	    } else v_flag_odd_qh_asc=v_flag_odd_qh_dec=v_flag_even_qh_asc=v_flag_even_qh_dec=0;
                    
                    //v_qhCAint_to_prd_m255_lst=v_qhCAint_to_prd_m255;    //2010-7-2 move down
            	}

            //2010-7-4
            //if((VF_E0_TPS&&(bPcaH>PCA2092)&&bPcaH<PCA900&&!VF_STARTS_ENG)||!VF_BFIRE_ENG)//2010-1-31 ADD (prd_cas>PCA2929) FOR FLY125
            if((VF_E0_TPS&&(bPcaH>PCA2092)&&bPcaH<PCA900&&!VF_STARTS_ENG))//2014-4-19 DELETE !VF_BFIRE_ENG
            {                                                 //2010-3-24 add  prd_cas<PCA900
                // nb_qhy_err t_qhymax_p	                  //2010-6-29 add condiciton !VF_BFIRE_ENG
                //nb_CA_Ave sum_CA_Int
               /* if(nb_CA_Ave==2)
                {
                    sum_CA_Int=((word)v_qhCAint_to_prd_m255_lst+v_qhCAint_to_prd_m255)*8;//sum_CA_Int-sum_CA_Int/16+  
                    v_qhCAint_ave=v_qhCAint_to_prd_m255_lst/2+v_qhCAint_to_prd_m255/2;  
                    nb_CA_Ave=3;
                }
                else 
                {
                    if(nb_CA_Ave>2)
                    {
                        sum_CA_Int=sum_CA_Int-v_qhCAint_ave+v_qhCAint_to_prd_m255_lst/2+v_qhCAint_to_prd_m255/2;
                        v_qhCAint_ave=sum_CA_Int/16;
                        if(!(nb_CA_Ave&0x01)) 
                        {
                            if(VF_BFIRE_ENG)
                            {
                                if((v_qhCAint_ave>t_qhymax_p||v_qhCAint_ave<t_qhymin_p) &&!VF_STARTS_ENG)
                                {
                                    if(nb_qhy_err<255)nb_qhy_err++;    
                                } else
                                {
                                    nb_qhy_err=0;            
                                }
                                b=80;
                            }
                            else
                            {
                                if(v_qhCAint_ave>250)
                                {
                                    if(nb_qhy_err<255)nb_qhy_err++;    
                                }
                                else nb_qhy_err=0;
                                b=8;  //2010-6-29 add VF_BFIRE_ENG
                            }
                            if(nb_qhy_err>b)VF_TUTAIER_CAS=true; //2010-3-24 change from 10 to 20
                        }
                    }
                    if(nb_CA_Ave<255)nb_CA_Ave++;   //v_qhCAint_ave
                    else nb_CA_Ave=4; 
                }*/
				
				if(v_nb_tdcerr>60)VF_TUTAIER_CAS=true; 

            } else 
            {
                nb_qhy_err=0;    
				v_nb_tdcerr=0;
            }
           /* //VFF_CANUM_DTC ,VFF_2ND_DTC
            if(nb_hy_inqy!=1||nb_qy_inhy!=1)
            {
                if(ca_num_er_numb<255)ca_num_er_numb++;
            } 
            else ca_num_er_numb=0;
            if(nb_hy_inqy==0)  //连续好多圈，两个前沿之间没有后沿
            {
                if(ca_hardware_er_numb<255)ca_hardware_er_numb++; 
            } 
            else ca_hardware_er_numb=0;
        
            if(!VF_HAVEHY_CAS||VF_HYER_CAS)//2008-10-30
            {     //没有检测到后沿或者后研故障
                if(ca_hardware_er_numb<255)ca_hardware_er_numb++;
            }
            if(ca_num_er_numb>100)VF_CANUMER_CAS=true;
            if(ca_hardware_er_numb>100)VF_CANUMER_CAS=true;*/
			
		//	if(v_nb_tdcerr>100)VF_TUTAIER_CAS=true; 
            
            v_qhCAint_to_prd_m255_lst=v_qhCAint_to_prd_m255;    //2010-7-2 move down
            
        } else{
    	        //2013-5-13 add 
    	        prd_cas_lst2_fordtc=prd_cas_lst_fordtc=prd_cas;
    	        prd_cas_lst_min=0;
        }
        
        
        /*//if(VF_CATCHHY_CAS&&!VF_CATCH_CAS)VF_NOQY_CAS=true; 
        VFF_CAECU_DTC=VFF_CAECU_DTC||VF_NOQY_CAS||VF_TUTAIER_CAS
                ||VF_QYTUBIAN_CAS
                ||VF_PHASEER_CAS //2009-12-27ADD 
                ||VF_HYER_CAS;//2015-6-15 ADD 
        //VFF_CAECU_DTC
        //VFF_2ND_DTC=VFF_CAECU_DTC=VF_HYER_CAS=false;*/

    }


    /*---------------诊断高压包是否连接----------------*/
    //2017-12-11,ysl,mod
    void F_Diag_IGN(byte b_state)
    {
        word b;
        word wt;
        
        mc33814_ReadStatus_def(_0800_IGN1_FAULT);//, false, 0);
        
        /*
            #ifndef IS_DETECT_IGN
                return; //2016-2-28 恢复
                  //2016-3-3 临时因电路的原因，暂时屏蔽
            
            #endif
        */
        //2017-1-21
        if(BYPTR_M0_EN_DTC_IGN==EN_DTC_IGN_NOTEN)
        {
            return;
        }
        
              
        //CDI_ON;//关闭；
        //for(b=0;b<=100;b++);//delay30us
        //if(!IS_INJDRV_HIGH)VF_OPEN_INJCTL=true;//如果喷油器检测端口为低电平；判定喷油器开路
        
        //2016-4-26
        //wt=T_MAINTMR;
        
        //while(!IS_IGNI_DRVHIGH &&( T_MAINTMR-wt<100))
        //{
            //时间没到且端口电平正确        
        //}
        
        if(b_state == DGN_LSD_ALL)//全诊断
        {
            if(!VF_ISCHARGE_IGN)//如果高压包关闭则开启高压包以检测OC
            { 
                if(!IS_IGNI_DRVHIGH)  //2009-12-18 delete ||VF_SHORT_INJCTL
                {
                    if(nb_ign_open_er<255)nb_ign_open_er++;
                } else nb_ign_open_er=0;

                CDI_CHARGE;//开喷油器  
        
                for(b=0;b<=260;b++);//delay100us
        
                mc33814_ReadStatus_def(_0800_IGN1_FAULT);
                CDI_ON;//点火放电
                
                if(P_33814IGN1_OC == 1)
                {
                    if(nb_ign_oc_er<255)nb_ign_oc_er++;
                }  else nb_ign_oc_er=0; 
            }
        }
        else
        {
            if(!IS_IGNI_DRVHIGH)  //2009-12-18 delete ||VF_SHORT_INJCTL
            {
                if(nb_ign_open_er<255)nb_ign_open_er++;
            } else nb_ign_open_er=0;
        
            if(P_33814IGN1_OC == 1)
            {
               // VF_SHORT_INJCTL=true;//如果喷油器检测端口为高电平；判定晶体管断路；
               if(nb_ign_oc_er<255)nb_ign_oc_er++;
            }  else nb_ign_oc_er=0;
        }
        
        
        //2016-4-26
           // wt=T_MAINTMR;
            
            //while(IS_IGNI_DRVHIGH &&( T_MAINTMR-wt<100))
            //{
                //时间没到且端口电平正确        
            //}
            
        

        //deFrame[2]=nb_ign_open_er;
        //deFrame[3]=nb_ign_short_er;
        
        //deVBAT=deFrame[2];
        //deTPS=deFrame[3];
        
        if(nb_ign_open_er>NB_INJ_LIMIT) 
        {
            //VF_ER_INJCTL=true;
            VF_OPEN_IGN=true;     //2010-1-1 mos 短路
        } else {
            //nb_ign_open_er=0;
            VF_OPEN_IGN=false;
        }
        /*if(nb_ign_short_er> NB_INJ_LIMIT)
        {
            VF_SHORT_IGN=true;    //2010-1-1 mos 开路
        }
        else 
        {
            VF_SHORT_IGN=false;
        }*/
        //OC
        if(nb_ign_oc_er> NB_INJ_LIMIT)
        {
            VF_OC_33814IGN=true;    //2010-1-1 mos 开路
        }
        else 
        {
            VF_OC_33814IGN=false;
        }
        
        if(nb_ign_detect<255)nb_ign_detect++;
        
        if(P_33814IGN1_ST != 0)
        {
            mc33814_SPIRwWord_def(_4000_WRITE_STATUS, _0800_IGN1_FAULT, 0x0U);
        }
    }


   
    /*
      保存CAS的AD采样值，
      2016-2-28
    */
    void Save_AD_cas(byte ad)
    {
        v_AD_cas=ad;
        
        if(v_AD_cas>20)//2017-9-19 取消短路故障，防止接地熄火误报||v_AD_cas<2) 
        {
            if(nb_AD_cas_err<255)nb_AD_cas_err++;
        } else nb_AD_cas_err=0;
        
        if(nb_AD_cas_err>20)
        {
            VF_NOQY_CAS=true;    
        } else VF_NOQY_CAS=false;
        
        
        //2016-11-13 恢复 VF_NOQY_CAS=false;//暂时取消 2016-4-26
		//2020-5-20 暂时取消
        VF_NOQY_CAS=false;    
    }

    //*********** F_SumFailure*********************************
    //得到RZT和TPS的CODE_NUMBER，根据CODE_NUMBER，dia_number得到:
    //sensor_erro,jqm_erro //sensor_erro,rzt_erro //SENSOR_ERRO,t_erro  //SENSOR_ERRO,INJECT_ERRO
    //根据code_number累加故障码: //jqm_er_numb  //rzt_er_numb //nooil_er_numb //batter_er_numb  //injector_er_n

    void F_SumFailure(void)
    {

        //将各个故障位合成故障码(编号：1-8)

            VFF_VBAT_DTC=VF_ER_VBAT||(VF_VBATFLU_WHENT4_INJCTL&&VF_1ST_INI_SYSINI);
                    //1    //2009-11-29 add VF_VBATFLU_WHENT4_INJCTL

            //VFF_T3ER_DTC=VF_T3FKLIMIT_INJCTL||VF_T3ER_INJCTL;//2  //暂时没有VF_NOOIL_INJ，
            VFF_T3ER_DTC=VF_OPEN_33814PUMPCTL||VF_SHORT_33814PUMPCTL||VF_OC_33814PUMPCTL;//2020-7-7 CR 油泵故障
			
            VFF_ECUNOTINIT_DTC =VF_ECUNOSET_SYSINI||VF_PNNOINI_SYSINI//;//ECU未编号，导致喷油器标准斜率和截距在EE中的备份值与程序中固有值不一致；
                  ||VF_ECUNOSET_SYSINI
                  ||VFF_NOZZLEINI_DOMFAILURE_DTC
                  ||VFF_NOZZLEINI_VBATNOTDOM_DTC;  //2013-7-23 ADD FOR  VFF_ECUNOTINIT_DTC
			
			VFF_ECUNOTINIT_DTC=false;
			VFF_ECUNOTINIT_DTC=false;
			VF_PNNOINI_SYSINI=false;
			
		

                                               //喷油器参数未设置；//3 //2009-8-3 add VF_PNNOINI_SYSINI

            if((VF_OPEN_INJCTL&&!VF_NOFUSE_VBAT)||VF_SHORT_33814INJ1CTL||VF_OC_33814INJ1CTL
                ||(VF_T3ER_INJCTL&&!VF_ER_VBAT))VFF_INJ_DTC=true;//VF_ER_INJCTL;          //4     //2009-12-31


            VFF_SYSNOTINIT_DTC=VF_NOINIT_SYSINI&&!VF_AQOK_SYSINI;////喷油器O值未设置无偏移量5

            
            if(VF_TESER_TMP  )   //2010-3-25 根据杨老师意见删除VF_TASER_TMP||； 
            {
                VFF_TES_DTC=true; //6 //2009-12-31
            }
			else
			{
				VFF_TES_DTC=false;
			}
            /*
            #ifdef IS_TA_ERR_DISP  //2016-1-15
                if(VF_TASER_TMP)
                {
                    VFF_TES_DTC=true;    
                }
            
            #endif
            */
            if(VF_OSER_OSDIAG)VFF_OS_DTC=VF_OSER_OSDIAG;//||VF_FLEANSIGNAL_OSDIAG;            //7  //2008-11-14 add
                                        //2009-8-17 DELETE TMP
            if(VF_ER_TPS)
			{
				VFF_TPS_DTC=true; //2010-2-6
			}
			else
			{
				VFF_TPS_DTC=false;
			}
            
        //-------故障号：9-16----------------------------
         ///
            VFF_FUEL_DTC =VF_T3ER_INJCTL||VF_T3NOTEXIST_INJCTL;//T3信号有误           //9             _V_STATUS_DIAG2.Bits.vf_diag2_1//ECU喷油计量故障
            
            VFF_INJCTL_DTC=VF_T3FKLIMIT_INJCTL;//T3反馈到头       //10              _V_STATUS_DIAG2.Bits.vf_diag2_2//ECU喷油调节限位故障（气泡）


            if(
                (VF_T3NOTEXIST_INJCTL&&(nb_inj_detect>200)&&!VF_NOFUSE_VBAT&&!VF_OPEN_INJCTL&&!VF_SHORT_INJCTL)//2010-2-6 delete VF_BFIRE_ENG   
                ||VF_SHORT_INJCTL
                ||VF_SHORT_IGN
              //  ||VF_ECUNOSET_SYSINI
              //  ||VFF_NOZZLEINI_DOMFAILURE_DTC
              //  ||VFF_NOZZLEINI_VBATNOTDOM_DTC  //2013-7-23 DELETE   FROM VFF_ECUINIT_DTC
                ||VFF_NOZZLEINI_NOT4_DTC//2009-11-29 add 
                )
            {
                VFF_ECUINIT_DTC=true;  
            }

            
            VFF_ECUPWRON_DTC=false;//VF_OSCFAIL_HARD;//2010-8-20 ADD false;          //13               _V_STATUS_DIAG2.Bits.vf_diag2_5//ECU上电故障

            if(VF_TASER_TMP)
			{
				VFF_TAS_DTC=true;               //14          _V_STATUS_DIAG2.Bits.vf_diag2_6//进气温度传感器故障
			}
			else
			{
				VFF_TAS_DTC=false;
			}

           //过热保护   
            VFF_TENGHIGH_DTC=VF_STOPBYTOOHOT_ENG;          //15              _V_STATUS_DIAG2.Bits.vf_diag2_7//发动机过温保护

        //-------故障号：17-24----------------------------
            if(VF_LIMITLEARN_CLSSL)VFF_OSFK_DTC=true;      //17                     _V_STATUS_DIAG3.Bits.vf_diag3_1//氧传感器反馈异常 ,2008-11-13 增加学习到头
            
            VFF_SAV_DTC=false;       //18                   _V_STATUS_DIAG3.Bits.vf_diag3_2//二次空气阀故障

 //           #if ECU_TYPE==ECU_MOT
            #ifndef IS_GEN

                VFF_EGR_DTC=false;       //19                  _V_STATUS_DIAG3.Bits.vf_diag3_3//EGR阀故障
            #else
                VFF_EGR_DTC=(VF_OPEN0_LDSENSOR||VF_OPEN1_LDSENSOR);       //19                  _V_STATUS_DIAG3.Bits.vf_diag3_3//EGR阀故障
            #endif
            
            VFF_TWC_DTC=false;       //20                 _V_STATUS_DIAG3.Bits.vf_diag3_4//TWC故障
            VFF_MAPS_DTC=VF_ER_MAPS;      //21                   _V_STATUS_DIAG3.Bits.vf_diag3_5//MAP传感器故障
            
            //#if ECU_TYPE==ECU_MOT
            #ifndef IS_GEN
                VFF_AMF_DTC=false;       //22                  _V_STATUS_DIAG3.Bits.vf_diag3_6//AMF传感器故障
            #else
                VFF_AMF_DTC=VF_FAILURE_ETC;       //22                  _V_STATUS_DIAG3.Bits.vf_diag3_6//AMF传感器故障
            #endif
            
        //-------故障号：25-32----------------------------
            VFF_CYL1INJ_DTC=VFF_INJ_DTC; // 25                          _V_STATUS_DIAG4.Bits.vf_diag4_1//1缸喷油器连接错误
            VFF_CYL2INJ_DTC =false;       // 26                  _V_STATUS_DIAG4.Bits.vf_diag4_2//2缸喷油器连接错误?
            
       //氧传感器反馈到头； //2010-3-15 VF_LIMITFK_CLS
            VFF_CYL1OS_DTC =VF_LIMITFK_CLS;//VFF_OS_DTC;  // 27                      _V_STATUS_DIAG4.Bits.vf_diag4_3//1缸氧传感器故障
            
            VFF_CYL2OS_DTC  =false;       // _V_STATUS_DIAG4.Bits.vf_diag4_4//2缸氧传感器故障

            if(VF_ER_PAIR||VF_NOTCAB_PAIR||VF_VALUEERINEE_PAIR||VF_SOFTCFG_ERR_PAIRS)VFF_PAIR_DTC=true;//||VF_VALUEERINEE_PAIR;//   29                      _V_STATUS_DIAG4.Bits.vf_diag4_5//-大气压力故障
            //VFF_PAIR_DTC=false;//2013-9-11   //2014-11-26 取消对压力故障的屏蔽

         //无T4
            VFF_IDLESCLIMIT_DTC=VFF_NOZZLEINI_NOT4_DTC;//VF_LIMITAQ_IDLEAQ;//   30                         _V_STATUS_DIAG4.Bits.vf_diag4_6//--  怠速搜索值达到最大限制值
            
            //2010-5-15 add VF_IDLEAIRTEST_SYSINI
            VFF_7OFDIAG5_DTC=VF_IDLEAIRTEST_SYSINI; //  31                        _V_STATUS_DIAG4.Bits.vf_diag4_7//--
            
            //2010-5-15 add VF_1ST_INI_SYSINI
            VFF_8OFDIAG5_DTC=VF_1ST_INI_SYSINI; //  32                        _V_STATUS_DIAG4.Bits.vf_diag4_8//--
         
        //将故障标志位组合成故障代码；
       /*
        CODE_NUMBER=0;
        if(VF_ER_VBAT)CODE_NUMBER|=0x01;
        if(VF_NOOIL_INJCTL)CODE_NUMBER|=0x02;
        if(VF_T3FKLIMIT_INJCTL||VF_T3ER_INJCTL)CODE_NUMBER|=0x02;//2008-8-19 add VF_T3ER_INJCTL
        if(VF_ECUNOSET_SYSINI)CODE_NUMBER|=0x04; //ECU未编号，导致喷油器标准斜率和截距在EE中的备份值与程序中固有值不一致；
                                           //喷油器参数未设置；
        if(VF_ER_INJCTL)CODE_NUMBER|=0x08;
        if(VF_NOINIT_SYSINI)CODE_NUMBER|=0x10; //喷油器O值未设置无偏移量
        if(VFF_TES_DTC)CODE_NUMBER|=0x20;
        if(VF_O2ER_OSDIAG)CODE_NUMBER|=0x40;
        if(VF_ER_TPS)CODE_NUMBER|=0x80; 
       */
       
       //2013-5-13 add condiction for DTC ACCUMULATE
       if(v_rev_625rpm<REV62D5_7000){
        
            if(VFF_VBAT_DTC)Accu_NB_er(1);
            if(VFF_T3ER_DTC)Accu_NB_er(2);
            if(VFF_ECUNOTINIT_DTC)Accu_NB_er(3);
            if(VFF_INJ_DTC)Accu_NB_er(4);
            if(VFF_SYSNOTINIT_DTC)Accu_NB_er(5);
            if(VFF_TES_DTC)Accu_NB_er(6);
            if(VFF_OS_DTC)Accu_NB_er(7);
            if(VFF_TPS_DTC)Accu_NB_er(8);
            if(VFF_FUEL_DTC)Accu_NB_er(9);
            if(VFF_INJCTL_DTC)Accu_NB_er(10);


            if(VFF_ECUINIT_DTC)Accu_NB_er(11);
            //if((!VF_HAVEHY_CAS||VF_HYER_CAS))Accu_NB_er(12);       //2009-8-7
            if(VFF_ECUPWRON_DTC)Accu_NB_er(13);
            if(VFF_TAS_DTC)Accu_NB_er(14);
            if(VFF_TENGHIGH_DTC)Accu_NB_er(15);

            if(VFF_OSFK_DTC)Accu_NB_er(17);
            if(VFF_SAV_DTC)Accu_NB_er(18);

            #ifndef IS_GEN
//            #if ECU_TYPE==ECU_MOT
                if(VFF_EGR_DTC)Accu_NB_er(19);
            #else
                if(VFF_EGR_DTC)Accu_NB_er(19);
            #endif
            
            if(VFF_TWC_DTC)Accu_NB_er(20);
            if(VFF_MAPS_DTC)Accu_NB_er(21);
    
            #ifndef IS_GEN
            
    //        #if ECU_TYPE==ECU_MOT
                if(VFF_AMF_DTC)Accu_NB_er(22);
            #else
                if(VFF_AMF_DTC)Accu_NB_er(22);
            #endif
            //2009-3-2 23 号故障为点火故障；
            if(VF_CANUMER_CAS)Accu_NB_er(23);          //2009-8-7  
            if(VFF_CAECU_DTC)Accu_NB_er(24);

            if(VFF_CYL1INJ_DTC)Accu_NB_er(33);
            if(VFF_CYL2INJ_DTC)Accu_NB_er(34);
            if(VFF_CYL1OS_DTC)Accu_NB_er(35);
            if(VFF_CYL2OS_DTC)Accu_NB_er(36);
            if(VFF_PAIR_DTC)Accu_NB_er(37);
            if(VFF_IDLESCLIMIT_DTC)Accu_NB_er(38);
            if(VFF_7OFDIAG5_DTC)Accu_NB_er(39);
            if(VFF_8OFDIAG5_DTC)Accu_NB_er(40);
       }



    //======================================================
          VF_PAMB_LB_SPN=VFF_MAPS_DTC;//大气压力传感器故障（电压过低）
          VF_PAMB_UB_SPN=false;//大气压力传感器故障（电压过高、断路等电路故障）
          VF_PAMB_LB_SPN=false;
          
          //VF_PAMB_LB_SPN=Chk_Mv_failure(MV_PAIR,MV_L_LIMIT);
          //VF_PAMB_UB_SPN=Chk_Mv_failure(MV_PAIR,MV_U_LIMIT);
		  
		  //2020-6-19 for CR
		  if((nb_Mv[MV_PAIR][MV_L_LIMIT]) >5)
		  {
			  VF_PAMB_LB_SPN=true;
		  }
		  
		  if((nb_Mv[MV_PAIR][MV_U_LIMIT]) >5)
		  {
			  VF_PAMB_UB_SPN=true;
		  }
          
          VF_TAMB_C_SPN=false;//VF_TASER_TMP;//进气温度传感器故障（间歇性故障、变化不合理）
          VF_TAMB_B_SPN=false;//进气温度传感器故障（信号值超范围）
          VF_TAMB_SHORT_SPN=false;//进气温度传感器故障（短路、电压过低）
          VF_TAMB_OPEN_SPN=false;//进气温度传感器故障（断路、电压过高）
          VF_TAMB_WIRE_SPN=false;//进气温度传感器故障（线路等故障）

          VF_TAMB_SHORT_SPN=VF_TASER_TMP&&Chk_Mv_failure(MV_TAS,MV_U_LIMIT);
          VF_TAMB_OPEN_SPN=VF_TASER_TMP&&Chk_Mv_failure(MV_TAS,MV_L_LIMIT);
          
          VF_TES_WIRE_SPN=false;//发动机（冷却水或缸盖）温度传感器故障（线路等故障）
          VF_TES_B_SPN=false;//发动机（冷却水或缸盖）温度传感器故障（信号值不合理）
          VF_TES_LB_SPN=false;//发动机（冷却水或缸盖）温度传感器故障（短路、电压过低）
          VF_TES_UB_SPN=false;//发动机（冷却水或缸盖）温度传感器故障（断路、电压过高）
          VF_TES_C_SPN=false;//发动机（冷却水或缸盖）温度传感器故障（间歇性故障、变化不合理）


          VF_TES_LB_SPN=VF_TESER_TMP&&Chk_Mv_failure(MV_TES,MV_U_LIMIT);
          VF_TES_UB_SPN=VF_TESER_TMP&&Chk_Mv_failure(MV_TES,MV_L_LIMIT);

          VF_TPS_LB_SPN=false;//TPS故障（输出偏小）
          VF_TPS_UB_SPN=false;//TPS故障（输出偏大）
          VF_TPS_OPEN_SPN=false;//TPS故障（断路）
          VF_TPS_B_SPN=false;//TPS故障（不合理）
          VF_TPS_C_SPN=false;//VF_ER_TPS;//TPS故障（间歇故障）

          VF_TPS_LB_SPN=VF_ER_TPS&&Chk_Mv_failure(MV_TPS,MV_L_LIMIT);
          VF_TPS_UB_SPN=VF_ER_TPS&&Chk_Mv_failure(MV_TPS,MV_U_LIMIT);
          
          //VF_OS_WIRE_SPN=VF_CANNOTWORK_OSDIAG;//2019-1-3 ||VF_CANNOTFLU_OSDIAG;//氧传感器故障（线路不良、失效）
		VF_OS_WIRE_SPN=false;		//20211014 chenwei 环松氧加热功能策略处理，屏蔽上方
		  
          //VF_OS_SHORT_SPN=false;//氧传感器故障（短路、信号偏小）
          //VF_OS_OPEN_SPN=false;//氧传感器故障（断路、信号偏大）	//20211014 chenwei 屏蔽
          VF_OS_OPEN_SPN=VF_OS_HIGHVOLT;		//20211014 chenwei add 环松氧加热
		  
          VF_OS_SLOW_SPN=false;//氧传感器故障（信号变化太慢）
          //VF_OS_C_SPN=VF_OSER_OSDIAG;//氧传感器故障（变化不合理）
          VF_OS_C_SPN=false;//氧传感器故障（变化不合理）   2017-1-10 设置为false;
          VF_OS_HEATER_SPN=false;//氧传感器故障（加热线路不良）
 
          //2016-11-13
          VF_OS_SHORT_SPN=VF_OSSHORTCUT_OSDIAG;//2016-10-19 false;//氧传感器故障（短路、信号偏小）
                       
          /*
          VF_PN_WIRE_SPN=((VF_OPEN_INJCTL&&!VF_NOFUSE_VBAT)
                ||(VF_T3ER_INJCTL&&!VF_ER_VBAT));//喷油器故障（线路不良）
          *///2016-3-5
          //VF_PN_WIRE_SPN=(VF_OPEN_INJCTL&&!VF_ER_VBAT);
		  //VF_PN_WIRE_SPN=VF_SHORT_33814INJ1CTL;
		  VF_PN_WIRE_SPN=VF_OPEN_33814INJ1CTL;
          
          #ifndef SYS_TRADITIONAL
			  VF_PN_T3FK_SPN=VFF_T3ER_DTC;//喷油器故障（反馈T3异常）
		  #else
			  VF_PN_T3FK_SPN=false;
		  #endif
          
          VF_MISSFIRE_SPN=false;//有失火故障
          VF_IGN_COIL_SPN=VF_SHORT_IGN||VF_OPEN_IGN||VF_OC_33814IGN;//||;//点火线圈故障（线路失效）VF_OPEN_IGN--可能不对，需要确认
		  VFF_IGN_COIL_SPN=VF_OC_33814IGN||VF_OPEN_IGN;
          
          VF_CAS_NO_SPN=VF_NOQY_CAS;//无曲轴角标
          VF_CAS_EMI_SPN=VF_TUTAIER_CAS;//曲轴角标被干扰故障（信号不良）
		  
		  VFF_CAECU_DTC=VFF_CAECU_DTC||VF_NOQY_CAS||VF_TUTAIER_CAS
                ||VF_QYTUBIAN_CAS
                ||VF_PHASEER_CAS //2009-12-27ADD 
                ||VF_HYER_CAS;
          
          VF_IGN_SPN=false;//点火正时受到干扰故障
          
          if(VF_1ST_INI_SYSINI)
		  {
			  VF_IDLE_LOW_SPN=VF_IDLEAIRLESS_IGN;//2016-3-1 false;//怠速故障（怠速气量偏小）
		  }
		  else
		  {
			  VF_IDLE_LOW_SPN=false;
		  }
		  
		  
          VF_IDLE_NO_SPN=false;//怠速故障（不能进入怠速状态，最小TPS电压过高，I>=1.0）
          
          VF_VBAT_LB_SPN=VF_ER_VBAT;//2018-12-29 VFF_VBAT_DTC;//电源电压故障（电压过低）
          VF_VBAT_UB_SPN=false;//电源电压故障（电压过高）
          
		  #ifndef SYS_TRADITIONAL
			VF_ECU_NONUM_SPN=VFF_ECUNOTINIT_DTC;//ECU故障（未编号、信息丢失）
			VF_ECU_EEPROM_SPN=VFF_SYSNOTINIT_DTC;//ECU故障（EEPROM失效）
		  #else
			VF_ECU_NONUM_SPN=false;//共轨
			
			
			VF_ECU_EEPROM_SPN=VFF_SYSNOTINIT_DTC;//共轨	
			
			
		  #endif
		  
          VF_ETC_SPN=false;//ETC故障
          VF_LOAD_SPN=false;//发电机负荷传感器故障
		  
		  //VF_IACV_OPEN=VF_C1OPEN_IACV||VF_C2OPEN_IACV;
		  //VF_IACV_SHORT=VF_SHORT_IACV;
		  
		  VF_IACV_OPEN=false;
		  VF_IACV_SHORT=false;


    }


    //**************F_ShowMilFunction****************************
    //if led_status_con<8, flash led
    //when led_status_con>=8, key on time >3 , led on when any erro happened, or led off except flashing when battery erro 
    //故障指示
    void F_ShowMilFunction(void)
    {
        byte b1;
        byte blnShift;
        byte bErr;
        byte bDelay;

		byte i;	//20210924 chenwei add

        VF_DTCOK_CLSSL=true;//2013-3-7
        
        
        //2010-3-26 将VFF_VBAT_DTC分离，作为可恢复的故障；
        bErr=VFF_ECUINIT_DTC
            ||VFF_INJ_DTC
            ||VFF_TPS_DTC
            ||((VF_CANNOTWORK_OSDIAG ||VF_CANNOTFLU_OSDIAG)&&(v_500km<=12))    //2010-1-4//2010-1-18
            ||VFF_TES_DTC //2009-8-3 将温度的作用弱化；
            ||VFF_PAIR_DTC//2009-11-29 add
            ||VF_TUTAIER_CAS||VF_NOQY_CAS//2016-2-28 add 
            //||VF_MILELIMIT_WARN_SYS  //2012-12-6 ADD
            //||VF_MILELIMIT_DUE_SYS   //2012-12-6 ADD
            ||VF_LIMITDUE_LOCK||!VF_KEYON_LOCK||VF_BKEYERR_LOCK  //2013-6-14
            ||VF_OPEN_IGN
            ;


        
        if(bErr||VFF_VBAT_DTC||VFF_TAS_DTC)VF_DTCOK_CLSSL=false;//2013-4-20
        //if(VFF_TAS_DTC)VF_DTCOK_CLSSL=false;//进气温度故障

        //2012-11-2增加
//        #if ECU_TYPE==ECU_GEN
        #ifdef IS_GEN
        
            bErr=bErr||VF_FAILURE_ETC||VF_OPEN1_LDSENSOR||VF_OPEN0_LDSENSOR||VF_LUBEMPTY_ETC;
            
        #endif

        /*
            #ifdef IS_TA_ERR_DISP  //2016-1-15
                bErr=bErr||VF_TASER_TMP;
            #endif
        */
        
        //2017-1-21
        if(BYPTR_M0_EN_DTC_TA==EN_DTC_TA_EN)
        {
                bErr=bErr||VF_TASER_TMP;
        }
        
        
        //2013-9-30 任何情况 都允许进入初始化， 只是需要 屏蔽这些故障
        if(VF_1ST_INI_SYSINI){
            bErr=false;
            VF_DTCOK_CLSSL=true;

        }


        //----INC_led_status_con:-----------------------
        blnShift=false;
        if(VF_PRDMAINTMR_TMR)
        {
            if(led_status_con==255)led_status_con=64;else{
                led_status_con++;
            }
            blnShift=true;

            //2016-2-24
            /*
            v_SPNX_forMIL[0][DTC_TYPE_KEYON]=v_SPNX_forMIL[0][DTC_TYPE_KEYON]|V_SPN0_STATUS;
            v_SPNX_forMIL[1][DTC_TYPE_KEYON]=v_SPNX_forMIL[1][DTC_TYPE_KEYON]|V_SPN1_STATUS;
            v_SPNX_forMIL[2][DTC_TYPE_KEYON]=v_SPNX_forMIL[2][DTC_TYPE_KEYON]|V_SPN2_STATUS;
            v_SPNX_forMIL[3][DTC_TYPE_KEYON]=v_SPNX_forMIL[3][DTC_TYPE_KEYON]|V_SPN3_STATUS;
            v_SPNX_forMIL[4][DTC_TYPE_KEYON]=v_SPNX_forMIL[4][DTC_TYPE_KEYON]|V_SPN4_STATUS;
            v_SPNX_forMIL[5][DTC_TYPE_KEYON]=v_SPNX_forMIL[5][DTC_TYPE_KEYON]|V_SPN5_STATUS;
   
            v_SPNX_forMIL[6][DTC_TYPE_KEYON]=v_SPNX_forMIL[6][DTC_TYPE_KEYON]|V_SPN6_STATUS;
            v_SPNX_forMIL[7][DTC_TYPE_KEYON]=v_SPNX_forMIL[7][DTC_TYPE_KEYON]|V_SPN7_STATUS;
            */
            //2016-3-3 摩托车系统更改为与SCR系统区别，即故障消失后，诊断也立即消失
                v_SPNX_forMIL[0][DTC_TYPE_KEYON]= V_SPN0_STATUS;
                v_SPNX_forMIL[1][DTC_TYPE_KEYON]= V_SPN1_STATUS;
                v_SPNX_forMIL[2][DTC_TYPE_KEYON]= V_SPN2_STATUS;
                v_SPNX_forMIL[3][DTC_TYPE_KEYON]= V_SPN3_STATUS;
                v_SPNX_forMIL[4][DTC_TYPE_KEYON]= V_SPN4_STATUS;
                v_SPNX_forMIL[5][DTC_TYPE_KEYON]= V_SPN5_STATUS;
       
                v_SPNX_forMIL[6][DTC_TYPE_KEYON]= V_SPN6_STATUS;
                v_SPNX_forMIL[7][DTC_TYPE_KEYON]= V_SPN7_STATUS;


        }
 
        //2016-6-5
         //2014-5-28 未激活的情况下
        if(VF_NOTACTIVE_LOCK)
        {
              if(REMARK==MS_PC_SET_SELB&& KESEI==0){
              //cur_code=19;
              MIL_ON;
              return;
              }
        }


        //2015-8-10 add 2016-7-14
        /*
            #ifdef MIL_DELAY
                //bDelay=10;
                bDelay=28;
            
            #else
                bDelay=0;
            
            #endif
        */
        
        //2017-1-21
        if(BYPTR_M0_MIL_DELAY_S==MIL_DELAY_S_NODELAY)
        {
            bDelay=0;    
        } 
        else
        {
            bDelay=BYPTR_M0_MIL_DELAY_S;    
        }

        if(!VF_MMOVE_ENG&&VF_DIAG_FAI&&(t_WOT>=5))
		{			
			VF_OBDVER_FAI=true;
		}
        //#ifndef IS_OBD1  //2016-2-24
        
        if((BYPTR_M0_OBD_VERSION==OBD_VERSION_FAI) ||VF_OBDVER_FAI)
        {
          
        
             //在非诊断模式下：
            //2009-12-8 更改：
            if((led_status_con<10+bDelay)) // VF_INJRDE5_EE:ECU没有编号   //chang from 8 to 10
            {           //由于“2008-11-2 //重新定义VF_NOSET_ECU的含义，不包含喷油器没设定”，因此这里变更为：
            		//&&!(VF_PNNOINI_SYSINI||VF_NOINIT_SYSINI||VF_1ST_INI_SYSINI)
            		
            		if(led_status_con>=bDelay){
            		  
                		if(led_status_con<4+bDelay || (!VF_PNNOINI_SYSINI&&!VF_NOINIT_SYSINI))
                		{
                		    //闪5下；
                    		if(led_status_con&0x1){//led_status_con对2取余
                    			MIL_ON;//开指示灯；
                    		}
                    		else{
                    		    MIL_OFF;
                    		}//关指示灯；
                		} else 
                		{
                		    MIL_OFF; 
                		}
            		} 
            		else
            		{
            		    MIL_OFF;
            		
            		}
            		nb_led=0;
            		cur_code=0;
            } 
            else if(VF_JFINISHINI_WHENMOVING_SYSINI)   //2009-11-25 add
            {
                //如果刚调试好，还在怠速，此时则熄灯，以防止用户误以为没调试好；
                cur_code=0;
                MIL_OFF;    
            }
            else //if(!VF_STARTS_ENG)//2009-11-20 增加故障码显示的条件,启动阶段不显示故障
            {
                //确定显示码
                //2009-8-3 当出现故障码切换时,需要等到头一个故障闪完之后再切换,以免故障码误读.
                if(blnShift)
                {
                    if(nb_led==0) //指示信息从头开始
                    {   
                    //  VFF_TES_DTC=false; //2010-3-14 临时
                    //  VFF_PAIR_DTC=false;
                    //  VFF_ECUINIT_DTC=false;
                        
                        if(bErr||VFF_VBAT_DTC||(VF_15S_AFTERSTART_TMCCL&&VFF_VBATSP_ERR)||VF_ER_REGU)
                        {   //有普通故障，
                            if(VF_ER_VBAT||(VF_15S_AFTERSTART_TMCCL&&VFF_VBATSP_ERR)||VF_ER_REGU)//2018-12-29 VFF_VBAT_DTC
                            {   
                                cur_code=2;
                            } 
                            else if(VFF_ECUINIT_DTC)//2009-8-5 delete tmp||VF_NOTCAB_PAIR)
                            {
                                cur_code=3;    
                            } else if(VFF_INJ_DTC)
                            {
                                cur_code=4;
                            } else if(VFF_TPS_DTC)
                            {
                                cur_code=5;
                            } else if(VF_CANNOTWORK_OSDIAG ||VF_CANNOTFLU_OSDIAG)  //2010-1-4
                            {
                                cur_code=6;
                            }else if(VFF_TES_DTC)
                            {
                                cur_code=7;
                            }
                            else if(VF_TUTAIER_CAS||VF_NOQY_CAS)   //2010-1-4   //2016-2-28 add VF_NOQY_CAS
                            {
                                cur_code=8;    
                            }
                            //     2012-11-10 DELETE
                            else if (VFF_PAIR_DTC)  //2009-11-29 add 
                            {
                                cur_code=9;   
                                //cur_code=0;//2011-11-29 屏蔽压力传感器采样出错时指示灯 
                            } 
                        //#ifdef IS_TA_ERR_DISP  //2016-1-20  2017-1-21
                            else if (VF_TASER_TMP&&(BYPTR_M0_EN_DTC_TA==EN_DTC_TA_EN))  //2009-11-29 add 
                            {
                                cur_code=10;   
                                //cur_code=0;//2011-11-29 屏蔽压力传感器采样出错时指示灯 
                            } 
                        //#endif
                        //#if ECU_TYPE==ECU_GEN
                        #ifdef IS_GEN
                        
                            else if (VF_FAILURE_ETC)  //2012-11-2 
                            {
                                cur_code=11;   
                            }
                            
                            else if (VF_OPEN0_LDSENSOR)  //2012-11-2 
                            {
                                cur_code=12;   
                            }
                            else if (VF_OPEN1_LDSENSOR)  //2012-11-2 
                            {
                                cur_code=13;   
                            }
                            else if (VF_LUBEMPTY_ETC)  //2012-11-2 
                            {
                                cur_code=14;   
                            }
                            
                        #endif
                            else if(!VF_KEYON_LOCK){ //未开锁，指示灯一直亮且不能启动
                                cur_code=19;    
                            } else if(VF_BKEYERR_LOCK){ //快闪 
                                cur_code=21;
                            }
                            else if(VF_LIMITDUE_LOCK){//警告，慢 闪
                                cur_code=20;    
                            } else if(VF_OPEN_IGN){
                                cur_code=15;//2014-4-16
                            }
                            else cur_code=0;
                            
                            //VF_DTCOK_CLSSL=false;//2013-3-6
                        
                        } 
                        else  
                        {   
                            //无
                            //if(((VF_1ST_INI_SYSINI)&&(!VF_NEEDSAVEGAMA_SYSINI))||VF_PNNOINI_SYSINI)//2009-11-24 VF_NOTNEED_DROPSCAN_SYSINI
                            if((VF_1ST_INI_SYSINI)&&(!VF_NEEDSAVEGAMA_SYSINI))//2009-11-24 VF_NOTNEED_DROPSCAN_SYSINI
                            {      
                                //无GAMA，或自检状态下未扫描好出油点（包括扫描后未成功的情况）
                                cur_code=20;    
                            } 
                            else if(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI)      //2009-12-7
                            {   //换件自检，或者强制自检时需要怠速调试，需要快闪,未调试，也快闪； 
                                if(VF_IDLEAIRLESS_IGN)
                                {
                                    cur_code=19;    
                                }else
                                {
                                    cur_code=21;    
                                }
                            }
							/*2018-12-29  非初始化进程怠速气量低暂时不亮灯
                            else if(VF_IDLEAIRLESS_IGN)//&&VF_E0_TPS&&VF_O2CHANGED_OSDIAG)
                            {
                                cur_code=19;    
                            }
                            */							
                            else cur_code=0;
                        }
                        
                    }
                    
                    
                    
                    //deOS=cur_code;
                    /*2009-8-13 注释：
                        nb_led==0时，才继续判断cur_code
                        是否会出现，当
                            nb_led>0，且灯一直亮，而 nb_led在短时间又没有赋0
                    */
                    //2009-8-2 for debug
                    //cur_code=v_PCKESE_SCI;
                    /*
                    if(cur_code!=lst_code)
                    {
                        nb_led=0;//        
                    } */
                    
					/*2017-2-21 更改case为if表达
                    //确定显示内容
                    switch(cur_code)
                    {
                        case 0:MIL_OFF;
                            nb_led=0;
                            break;
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:  //间闪
                        case 8:  //间闪
                        case 9:  //间闪
                        case 10:  //间闪
                        case 11:  //间闪
                        case 12:  //间闪
                        case 13:  //间闪
                        case 14:  //间闪
                        case 15:  //间闪
                            b1=cur_code*2;
                            if(b1<=nb_led)
                            {
                                MIL_OFF;
                            } 
                            else
                            {
                                if(nb_led&0x01)
                                {
                                    MIL_OFF;
                                }else
                                {
                                    MIL_ON; 
                                }
                            }
                            b1=(cur_code+3)*2;
                            nb_led++;
                            if(nb_led>=b1)nb_led=0;
                            break;
                        case 19:MIL_ON;
                            nb_led=0;
                            break;//
                        case 20:  //慢闪
                            b1=nb_led/4;
                            if(b1&0x01)
                            {
                                MIL_OFF;
                            }
                            else
                            {
                                MIL_ON; 
                            }
                            nb_led++;
                            if(nb_led>=8)nb_led=0;
                            break;
                        case 21:   //快闪
                            if(nb_led&0x01)
                            {
                                MIL_OFF;
                            }else
                            {
                                MIL_ON; 
                            }
                            
                            nb_led++;
                            if(nb_led>=2)nb_led=0;
                            break; 
                        default:MIL_OFF;
                            nb_led=0;
                            break;
                    }
                    //lst_code=cur_code;
					*/
					
					if(cur_code==0)
					{
						MIL_OFF;
                        nb_led=0;
					}
					else if((cur_code<=15)&&(cur_code>=1))	//2017-2-22 1-15都是间闪
					{
						b1=cur_code*2;
                            if(b1<=nb_led)
                            {
                                MIL_OFF;
                            } 
                            else
                            {
                                if(nb_led&0x01)
                                {
                                    MIL_OFF;
                                }else
                                {
                                    MIL_ON; 
                                }
                            }
                            b1=(cur_code+3)*2;
                            nb_led++;
                            if(nb_led>=b1)nb_led=0;
					}
					else if(cur_code==19)
					{
						MIL_ON;
                        nb_led=0;
					}
					else if(cur_code==20)//慢闪
					{
						b1=nb_led/4;
                            if(b1&0x01)
                            {
                                MIL_OFF;
                            }
                            else
                            {
                                MIL_ON; 
                            }
                            nb_led++;
                            if(nb_led>=8)nb_led=0;
					}
					else if(cur_code==21)//快闪
					{
						if(nb_led&0x01)
                            {
                                MIL_OFF;
                            }else
                            {
                                MIL_ON; 
                            }
                            
                            nb_led++;
                            if(nb_led>=2)nb_led=0;
					}
					else
					{
						MIL_OFF;
                        nb_led=0;
					}
                }
                
            } 
        //#else
        } 
        else if(BYPTR_M0_OBD_VERSION==OBD_VERSION_1)
        {
                    if(blnShift)
                    {
                            if(nb_led==0) //指示信息从头开始
                            {   
                                if(VF_1ST_INI_SYSINI)
								{
                                    if(!VF_NEEDSAVEGAMA_SYSINI)//2009-11-24 VF_NOTNEED_DROPSCAN_SYSINI
                                    {      
                                        //无GAMA，或自检状态下未扫描好出油点（包括扫描后未成功的情况）
                                        cur_code=20;    
                                    }
									else if(VF_ER_REGU)//2018-12-29 VFF_VBAT_DTC
									{   
										cur_code=2;
									}
                                    else if(VF_NOINIT_SYSINI)      //2009-12-7
                                    {   //换件自检，或者强制自检时需要怠速调试，需要快闪,未调试，也快闪； 
                                        if(VF_IDLEAIRLESS_IGN)
                                        {
                                            cur_code=19;    
                                        }else
                                        {
                                            cur_code=21;    
                                        }
                                    }
                                    else if(VF_IDLEAIRLESS_IGN)//&&VF_E0_TPS&&VF_O2CHANGED_OSDIAG)
                                    {
                                        cur_code=19;    
                                    } 
									else
                                    {
                                        cur_code=0;    
                                    }
                                }
                                else 
								{
                                
                                    cur_code=GetMIL();
									/*if(VFF_VBATSP_ERR&&VF_15S_AFTERSTART_TMCCL)
									{
										cur_code=2;
									}*/
                                    
                                }
                            }
                            
                            if(VF_1ST_INI_SYSINI)
                            {
                                  
										if(cur_code==0)
										{
											MIL_OFF;
											nb_led=0;
											
											//2016-2-29
                                            if(!VF_MMOVE_ENG)
                                            {
                                                MIL_ON;
                                            }
										}										
										else if(cur_code==19)
										{
											MIL_ON;
											nb_led=0;
										}
										else if(cur_code==20)//慢闪
										{
											b1=nb_led/4;
											if(b1&0x01)
											{
												MIL_OFF;
											}
											else
											{
												MIL_ON; 
											}
											nb_led++;
											if(nb_led>=8)nb_led=0;
										}
										else if(cur_code==21)//快闪
										{
											if(nb_led&0x01)
											{
												MIL_OFF;
											}
											else
											{
												MIL_ON; 
											}
                            
											nb_led++;
											if(nb_led>=2)nb_led=0;
										}
										else if((cur_code<=15)&&(cur_code>=1))	//2017-2-22 1-15都是间闪
										{
											b1=cur_code*2;
											if(b1<=nb_led)
											{
												MIL_OFF;
											} 
											else
											{
												if(nb_led&0x01)
												{
													MIL_OFF;
												}else
												{
													MIL_ON; 
												}
											}
											b1=(cur_code+3)*2;
											nb_led++;
											if(nb_led>=b1)nb_led=0;
										}
										else
										{
											MIL_OFF;
											nb_led=0;
										}
                                    
                            }
							else
							{
								if(!VF_MMOVE_ENG||VF_STARTS_ENG)
                                {
                                    MIL_ON;    
                                } 
                                else
                                {
                                    if(cur_code>0)
                                    {
                                        MIL_ON;
                                    }
                                    else
                                    {
                                        MIL_OFF;   
                                    }
                                    
                                    if(cur_code==0 && VFF_VBATSP_ERR&&VF_15S_AFTERSTART_TMCCL)
    								{
    									b1=2*2;//2次间闪
    									if(b1<=nb_led)
    									{
    										MIL_OFF;
    									} 
    									else
    									{
    										if(nb_led&0x01)
    										{
    											MIL_OFF;
    										}else
    										{
    											MIL_ON; 
    										}
    									}
    									b1=(2+3)*2;
    									nb_led++;
    									if(nb_led>=b1)nb_led=0;
    								}
                                }
								
								
							}
                            
                    }                            
        }
		else if((BYPTR_M0_OBD_VERSION==OBD_VERSION_4))	//20210922 chenwei add 环松仪表
        {
								
			AnalysisSPN(DTC_TYPE_CONFIRM,false,false);//解析历史故障码
									
			SAVE_PCODE_FOROBD4(v_pcode);	//20211004 chenwei add
									
			v_pcode_lookup=v_SPNX_forhuansong[nb_pcode];
			
			if(VFF_SYSNOTINIT_DTC)	//20211006 chenwei add 初始化
			{
				v_pcode_lookup=PCODE_SYSINIT;
			}
			
			if((v_pcode_lookup!=0))		//20211005 chenwei add
			{
				VF_MIL_NEMDTC=true;
			}				
			MIL_OFF;		//20211006 17.51 chenwei add 确保原始为高电平,存在一种当启动是正好处于低电平，此处仪表会一直闪烁
			if(((v_pcode_lookup!=0)&&VF_MIL_NEMDTC&&(v_t_fromkeyon_1s>T_FROMKEYON_1S)&&(!VF_MMOVE_ENG))||(VFF_SYSNOTINIT_DTC))
			{
				MIL_ACTION_FORHUANSONG(v_pcode_lookup);
			}	

			if(v_pcode_lookup==0)	//20211005 chenwei add 选择到没有
			{
				nb_pcode=0;
			}  

			
        }
		
		else
        {
                    if(blnShift)
                    {
                            if(nb_led==0) //指示信息从头开始
                            {   
                                if(VF_1ST_INI_SYSINI)
								{
                                    /*if(!VF_NEEDSAVEGAMA_SYSINI)//2009-11-24 VF_NOTNEED_DROPSCAN_SYSINI
                                    {      
                                        //无GAMA，或自检状态下未扫描好出油点（包括扫描后未成功的情况）
                                        cur_code=20;    
                                    }
									else */
									if(VF_ER_REGU)//2018-12-29 VFF_VBAT_DTC
									{   
										cur_code=2;
									}
									else if(VF_CANNOTWORK_OSDIAG ||VF_CANNOTFLU_OSDIAG)//2019-8-12 推行闭环初始化，需要报os故障
									{
										cur_code=6;
									}
                                    else if(VF_NOINIT_SYSINI)      //2009-12-7
                                    {   //换件自检，或者强制自检时需要怠速调试，需要快闪,未调试，也快闪； 
                                        if(VF_IDLEAIRLESS_IGN)
                                        {
                                            cur_code=19;    
                                        }else
                                        {
                                            cur_code=21;    
                                        }
                                    }									
                                    else if(VF_IDLEAIRLESS_IGN)//&&VF_E0_TPS&&VF_O2CHANGED_OSDIAG)
                                    {
                                        cur_code=19;    
                                    }                                     
									else
                                    {
                                        cur_code=0; 
										v_nb_mmove_eng=0;
                                    }
                                }
                                else 
								{                                
                                    cur_code=GetMIL();
									
									//test_20070701=cur_code;
									/*if(VFF_VBATSP_ERR&&VF_15S_AFTERSTART_TMCCL)
									{
										cur_code=2;
									}*/

                                    
                                }
                            }
                            
                            if(VF_1ST_INI_SYSINI)
                            {
                                  
                                    if(cur_code==0)
										{
											MIL_OFF;
											nb_led=0;
											
											//2016-2-29
                                            if(!VF_MMOVE_ENG)
                                            {
                                                MIL_ON;
                                            }
										}										
										else if(cur_code==19)
										{
											MIL_ON;
											nb_led=0;
										}
										else if(cur_code==20)//慢闪
										{
											b1=nb_led/4;
											if(b1&0x01)
											{
												MIL_OFF;
											}
											else
											{
												MIL_ON; 
											}
											nb_led++;
											if(nb_led>=8)nb_led=0;
										}
										else if(cur_code==21)//快闪
										{
											if(nb_led&0x01)
											{
												MIL_OFF;
											}
											else
											{
												MIL_ON; 
											}
                            
											nb_led++;
											if(nb_led>=2)nb_led=0;
										}
										else if((cur_code<=15)&&(cur_code>=1))	//2017-2-22 1-15都是间闪
										{
											b1=cur_code*2;
											if(b1<=nb_led)
											{
												MIL_OFF;
											} 
											else
											{
												if(nb_led&0x01)
												{
													MIL_OFF;
												}else
												{
													MIL_ON; 
												}
											}
											b1=(cur_code+3)*2;
											nb_led++;
											if(nb_led>=b1)nb_led=0;
										}
										else
										{
											MIL_OFF;
											nb_led=0;
										}
                            }
							else
							{
								if(!VF_MMOVE_ENG||(v_t_EngMove_s<1))
                                {
                                    MIL_ON; 
									if((cur_code>0)&&(cur_code<=15))//2020-7-7
									{
										v_nb_mmove_eng=M0_NB_CLEARDTC;
									}
                                } 
                                else
                                {
                                    
									
									if(cur_code>0)
                                    {
                                        MIL_ON;
										if(cur_code<=15)//2017-5-10 出现故障时开始计算发动机启动次数
										{
											v_nb_mmove_eng=M0_NB_CLEARDTC;//2017-5-28 第x次启动时清除故障
										}
                                    }
                                    else
                                    {
                                        MIL_OFF;
										if(VF_STARTS_ENG_LST&&!VF_STARTS_ENG&&(v_nb_mmove_eng>0))
										{
											v_nb_mmove_eng--;
										}
											
										if(v_nb_mmove_eng>0)
										{
											MIL_ON;
										}
                                    }
                                    
                                    if(cur_code==0&&VFF_VBATSP_ERR&&VF_15S_AFTERSTART_TMCCL)
									{
										b1=2*2;//两次间闪
											if(b1<=nb_led)
											{
												MIL_OFF;
											} 
											else
											{
												if(nb_led&0x01)
												{
													MIL_OFF;
												}else
												{
													MIL_ON; 
												}
											}
											b1=(2+3)*2;
											nb_led++;
											if(nb_led>=b1)nb_led=0;
									}
                                }
							}                            
                    }
		}
        //#endif
        VF_STARTS_ENG_LST=VF_STARTS_ENG;
        nb_DTC=cur_code;
        return;//
    }

/******环松仪表显示函数
*******通过故障码转化MIL的翻转信号输入仪表最终仪表显示故障代码**********/
	
	
	void MIL_ACTION_FORHUANSONG(word pcode)
	{
		byte i,i1;
		word w;
		byte b,b1,b2,b3,b4;
		w=pcode;
												
		b4=w/(16*16*16);	//得到故障码第4位
		if(b4==0)b4=10;
		w=w%(16*16*16);
												
		b3=w/(16*16);		//得到故障码第3位
		if(b3==0)b3=10;
		w=w%(16*16);
												
		b2=w/(16);			//得到故障码第2位
		if(b2==0)b2=10;
		w=w%(16);
												
		b1=w;				//得到故障码第1位
		if(b1==0)b1=10;

		//if(VF_MIL_NEMDTC)
		{
				MIL_OFF;
				if((v_t_MILGliter_d1s>DTC_T_INTERVAL_3D2S)&&(!VF_MIL_DTC_3D2S))	//故障间隔，3.2s
				{
					
					VF_MIL_DTC4=true;
					VF_MIL_DTC_3D2S=true;
					v_t_MILGliter_d1s=0;
					VF_MIL_SWITCH=true;
					nb_mil_interval=0;
				}

				if(VF_MIL_DTC4)
				{
					
					if(nb_mil_interval<b4)
					{
						if((v_t_MILGliter_d1s>MIL_ON_T_INTERVAL_D4S)&&(VF_MIL_SWITCH))
						{
							MIL_ON;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=false;
							
						}
						if((v_t_MILGliter_d1s>MIL_OFF_T_INTERVAL_D4S)&&(!VF_MIL_SWITCH))
						{
							MIL_OFF;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=true;
							nb_mil_interval++;
						}
					}
					if((nb_mil_interval>=b4)&&(v_t_MILGliter_d1s>NUMERICAL_T_INTERVAL_1D2S))
					{
						
						v_t_MILGliter_d1s=0;
						VF_MIL_DTC4=false;
						VF_MIL_DTC3=true;
						nb_mil_interval=0;
					}
					
				}

				if(VF_MIL_DTC3)
				{
					
					if(nb_mil_interval<b3)
					{
						if((v_t_MILGliter_d1s>MIL_ON_T_INTERVAL_D4S)&&(VF_MIL_SWITCH))
						{
							
							MIL_ON;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=false;
							
						}
						if((v_t_MILGliter_d1s>MIL_OFF_T_INTERVAL_D4S)&&(!VF_MIL_SWITCH))
						{
							MIL_OFF;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=true;
							nb_mil_interval++;
						}
					}
					if((nb_mil_interval>=b3)&&(v_t_MILGliter_d1s>NUMERICAL_T_INTERVAL_1D2S))
					{
						
						v_t_MILGliter_d1s=0;
						VF_MIL_DTC3=false;
						VF_MIL_DTC2=true;
						nb_mil_interval=0;
					}
					
				}

				if(VF_MIL_DTC2)
				{
					
					if(nb_mil_interval<b2)
					{
						if((v_t_MILGliter_d1s>MIL_ON_T_INTERVAL_D4S)&&(VF_MIL_SWITCH))
						{
							MIL_ON;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=false;
							
						}
						if((v_t_MILGliter_d1s>MIL_OFF_T_INTERVAL_D4S)&&(!VF_MIL_SWITCH))
						{
							MIL_OFF;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=true;
							nb_mil_interval++;
						}
					}
					if((nb_mil_interval>=b2)&&(v_t_MILGliter_d1s>NUMERICAL_T_INTERVAL_1D2S))
					{
						
						v_t_MILGliter_d1s=0;
						VF_MIL_DTC2=false;
						VF_MIL_DTC1=true;
						nb_mil_interval=0;
					}
					
				}

				if(VF_MIL_DTC1)
				{
					
					if(nb_mil_interval<b1)
					{
						if((v_t_MILGliter_d1s>MIL_ON_T_INTERVAL_D4S)&&(VF_MIL_SWITCH))
						{
							MIL_ON;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=false;
							
						}
						if((v_t_MILGliter_d1s>MIL_OFF_T_INTERVAL_D4S)&&(!VF_MIL_SWITCH))
						{
							
							MIL_OFF;
							v_t_MILGliter_d1s=0;
							VF_MIL_SWITCH=true;
							nb_mil_interval++;
						}
					}
					if((nb_mil_interval>=b1)&&(v_t_MILGliter_d1s>NUMERICAL_T_INTERVAL_1D2S))
					{
						
						VF_MIL_DTC1=false;
						VF_MIL_NEMDTC=false;
						VF_MIL_DTC_3D2S=false;
						v_pcode_lst=v_pcode_lookup;
						
						VF_MIL_NEMDTC2=false;

						if(!VFF_SYSNOTINIT_DTC)		//20211006 chenwei add 非初始化才++
						{
							nb_pcode++;		//20211005 chenwei add 配合 存储历史故障的数组
						}

					}
					
				}
		}
		
	}

	
	/****将用于环松仪表显示的历史故障存于数组中*****/	
	void SAVE_PCODE_FOROBD4(word pcode)
	{
		byte i;
		if(v_pcode!=0)
		{
			if(v_SPNX_forhuansong[0]==0||(v_SPNX_forhuansong[0]!=0&&v_SPNX_forhuansong[0]==v_pcode))
			{
				v_SPNX_forhuansong[0]=v_pcode;
			}
										
			else if(((v_SPNX_forhuansong[0]!=0)&&(v_SPNX_forhuansong[0]!=v_pcode)&&(v_SPNX_forhuansong[1]==0))||v_SPNX_forhuansong[1]==v_pcode)
			{
				v_SPNX_forhuansong[1]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[1]!=0)&&(v_SPNX_forhuansong[1]!=v_pcode)&&(v_SPNX_forhuansong[2]==0))||v_SPNX_forhuansong[2]==v_pcode)
			{
				v_SPNX_forhuansong[2]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[2]!=0)&&(v_SPNX_forhuansong[2]!=v_pcode)&&(v_SPNX_forhuansong[3]==0))||v_SPNX_forhuansong[3]==v_pcode)
			{
				v_SPNX_forhuansong[3]=v_pcode;
			}
						
			else if(((v_SPNX_forhuansong[3]!=0)&&(v_SPNX_forhuansong[3]!=v_pcode)&&(v_SPNX_forhuansong[4]==0))||v_SPNX_forhuansong[4]==v_pcode)
			{
				v_SPNX_forhuansong[4]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[4]!=0)&&(v_SPNX_forhuansong[4]!=v_pcode)&&(v_SPNX_forhuansong[5]==0))||v_SPNX_forhuansong[5]==v_pcode)
			{
				v_SPNX_forhuansong[5]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[5]!=0)&&(v_SPNX_forhuansong[5]!=v_pcode)&&(v_SPNX_forhuansong[6]==0))||v_SPNX_forhuansong[6]==v_pcode)
			{
				v_SPNX_forhuansong[6]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[6]!=0)&&(v_SPNX_forhuansong[6]!=v_pcode)&&(v_SPNX_forhuansong[7]==0))||v_SPNX_forhuansong[7]==v_pcode)
			{
				v_SPNX_forhuansong[7]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[7]!=0)&&(v_SPNX_forhuansong[7]!=v_pcode)&&(v_SPNX_forhuansong[8]==0))||v_SPNX_forhuansong[8]==v_pcode)
			{
				v_SPNX_forhuansong[8]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[8]!=0)&&(v_SPNX_forhuansong[8]!=v_pcode)&&(v_SPNX_forhuansong[9]==0))||v_SPNX_forhuansong[9]==v_pcode)
			{
				v_SPNX_forhuansong[9]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[9]!=0)&&(v_SPNX_forhuansong[9]!=v_pcode)&&(v_SPNX_forhuansong[10]==0))||v_SPNX_forhuansong[10]==v_pcode)
			{
				v_SPNX_forhuansong[10]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[10]!=0)&&(v_SPNX_forhuansong[10]!=v_pcode)&&(v_SPNX_forhuansong[11]==0))||v_SPNX_forhuansong[11]==v_pcode)
			{
				v_SPNX_forhuansong[11]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[11]!=0)&&(v_SPNX_forhuansong[11]!=v_pcode)&&(v_SPNX_forhuansong[12]==0))||v_SPNX_forhuansong[12]==v_pcode)
			{
				v_SPNX_forhuansong[12]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[12]!=0)&&(v_SPNX_forhuansong[12]!=v_pcode)&&(v_SPNX_forhuansong[13]==0))||v_SPNX_forhuansong[13]==v_pcode)
			{
				v_SPNX_forhuansong[13]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[13]!=0)&&(v_SPNX_forhuansong[13]!=v_pcode)&&(v_SPNX_forhuansong[14]==0))||v_SPNX_forhuansong[14]==v_pcode)
			{
				v_SPNX_forhuansong[14]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[14]!=0)&&(v_SPNX_forhuansong[14]!=v_pcode)&&(v_SPNX_forhuansong[15]==0))||v_SPNX_forhuansong[15]==v_pcode)
			{
				v_SPNX_forhuansong[15]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[15]!=0)&&(v_SPNX_forhuansong[15]!=v_pcode)&&(v_SPNX_forhuansong[16]==0))||v_SPNX_forhuansong[16]==v_pcode)
			{
				v_SPNX_forhuansong[16]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[16]!=0)&&(v_SPNX_forhuansong[16]!=v_pcode)&&(v_SPNX_forhuansong[17]==0))||v_SPNX_forhuansong[17]==v_pcode)
			{
				v_SPNX_forhuansong[17]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[17]!=0)&&(v_SPNX_forhuansong[17]!=v_pcode)&&(v_SPNX_forhuansong[18]==0))||v_SPNX_forhuansong[18]==v_pcode)
			{
				v_SPNX_forhuansong[18]=v_pcode;
			}

			else if(((v_SPNX_forhuansong[18]!=0)&&(v_SPNX_forhuansong[18]!=v_pcode)&&(v_SPNX_forhuansong[19]==0))||v_SPNX_forhuansong[19]==v_pcode)
			{
				v_SPNX_forhuansong[19]=v_pcode;
			}

		}
	}



    //对上电后，故障号出现的故障次数进行累加；
    void Accu_NB_er(byte erNO)
    {
        //if(VF_DIRECT_INI_SYSINI)return;//2009-8-15 add 
        //2010-3-8 chang to the following  VF_REMOVE_INI_SYSINI
        if(VF_1ST_INI_SYSINI)return;
        if(erNO<=40)
        {
            if(numbErr[erNO-1]<255)numbErr[erNO-1]++;
        }
    }

#pragma CODE_SEG DEFAULT
        
#endif
    
//------------是否显示其他页----------------------------------------------
byte F_DispOtherPageOrNot(void)
{   //除了第一页（）之外的其他页
/*    if(!V_DTC_STATUS)
    {
        return false;
    } 
*/
    if(V_DTCH_STATUS||V_DTCHL_STATUS||V_DTCHH_STATUS) return true;  //2009-11-24 更正；
    else return false;
}

byte FG_DispPage_ForIgn(void)
{
    return 136;
}

byte FG_DispCode_ForCos(void)
{
   // return V_STATUS_DIAG2;
   return V_DTCH_STATUS;//(byte)((V_DTC_STATUS>>8)&0xff);
}

byte FG_DispCode_ForDtfiL(void)
{
   // return V_STATUS_DIAG3;
   return V_DTCHL_STATUS;//(byte)((V_DTC_STATUS>>16)&0xff);
}

byte FG_DispCode_ForTBack(void)
{
   // return V_STATUS_DIAG4;
   return V_DTCHH_STATUS;//(byte)((V_DTC_STATUS>>24)&0xff);
}

byte FG_DispCode_ForPN0Cos(void)
{
   // return V_STATUS_DIAG;
      return V_DTC_STATUS;//(byte)((V_DTC_STATUS)&0xff);

}

//==============================================================================
//******sum_er_inc**********************************
//将EE中地址为W的数据累加一，累加结果最大值限值为255;
void sum_er_inc(byte add)
{
    byte dat;
    
    dat=Load_Data_EE(add);
    if(dat<255)dat++;
    
    Save_Data_EE(add,dat) ;
    
    return;
}


void Clr_DTC_History(void)
{
    byte b;
    
    for(b=0;b<40;b++)
    {
        Save_Data_EE(M2_DTC_HISTORY[b][0],0) ;
    }
}


    /*
        设置对应ID的SPN的状态
        blnSet:是设置还是清除；
        id:顺序号的id
        blnCur:当前的还是历史的；
            
    */
    void SetSpnStatus(byte blnSet,byte id,byte DTC_TYPE)
    {
        byte b;
        byte b1;
        
        
        b1=M1_DTCNO[id]&0x07;
        
        b=1;
        while(b1>0)
        {
            b1--;
            b*=2;
        }
        
        if(DTC_TYPE<=2)
        {
            if(blnSet){
                v_SPNX_forMIL[M1_DTCNO[id]/8][DTC_TYPE]=v_SPNX_forMIL[M1_DTCNO[id]/8][DTC_TYPE]|b;
            } else{
               //(RegName &= (byte)(~(byte)(ClrMask)))
                v_SPNX_forMIL[M1_DTCNO[id]/8][DTC_TYPE]=v_SPNX_forMIL[M1_DTCNO[id]/8][DTC_TYPE]&((byte)(~(byte)(b)));
            }
        }
    }


    /*
        清除SPN状态
        2015-9-20
    */
    void ClearSpnStatus(byte DTC_TYPE)
    {
        byte b;
        
        if(DTC_TYPE<=2){
            for(b=0;b<=7;b++)
            {
                v_SPNX_forMIL[b][DTC_TYPE]=0;
            }
        }
    }        
    //2016-2-23 从SCR移植过来    
    /*
        判断ID对应的位置的SPN是否有故障
        
        blnCur:表示当前位置的故障，否则为历史故障，即存储在EEPROM中的历史
        
        PI与SCR公用
    */
    byte CheckIsSPNNG(byte id,byte DTC_TYPE) 
    {
        byte blnR;
        byte bSPN;
        byte i;
        
        blnR=false;
        
        
            if(DTC_TYPE==DTC_TYPE_PENDING_NOW)
            {
                //switch(id/8)//2015-9-6 发现这个方法不对，更改如下：因为M1_DTCNO才是跟V_SPNX_STATUS对应的 
                switch(M1_DTCNO[id]/8) 
                {
                    case 0:
                        bSPN=V_SPN0_STATUS;
                    break;
                    case 1:
                        bSPN=V_SPN1_STATUS;
                    break;
                    case 2:
                        bSPN=V_SPN2_STATUS;
                    break;
                    case 3:
                        bSPN=V_SPN3_STATUS;
                    break;
                    case 4:
                        bSPN=V_SPN4_STATUS;
                    break;
                    case 5:
                        bSPN=V_SPN5_STATUS;
                    break;
                    case 6:
                        bSPN=V_SPN6_STATUS;
                    break;
                    case 7:
                        bSPN=V_SPN7_STATUS;
                    break;
                  
                }
                
            } 
            else
            {
              
                bSPN=v_SPNX_forMIL[M1_DTCNO[id]/8][DTC_TYPE];
            }
            
            //for(i=0;i<(id&0x07);i++) 2015-9-6 同步地更改如下
            for(i=0;i<(M1_DTCNO[id]&0x07);i++) //
            {
                bSPN=bSPN>>1;        
            }
        
            blnR=bSPN&0x01;
            
        return blnR;
    }


   //2017-12-12,ysl,add,油泵诊断
   void DTC_DiagPump_def(byte b_state)
   {
        word b;

        mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
        
        if(b_state == DGN_LSD_ALL)//全诊断
        {
            if(P_PUMPC_ONOFF_TST)//如果油泵开启则关闭油泵以检测OL
            { 
                if(P_33814REL1_OC == 1)//先检测短路状态
                {
                    if(nb_pump_oc_er<255)nb_pump_oc_er++;
                } else nb_pump_oc_er=0;
        
                P_PUMPC_OFF
                for(b=0;b<=1000;b++);
                mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
                P_PUMPC_ON 
                
                if(P_33814REL1_OPEN == 1)   //先检测开路状态
                {
                    if(nb_pump_open_er<255)nb_pump_open_er++;
                } else nb_pump_open_er=0;
            }
            else
            {
                if(P_33814REL1_OPEN == 1)   //先检测开路状态
                {
                    if(nb_pump_open_er<255)nb_pump_open_er++;
                } else nb_pump_open_er=0;
                
                P_PUMPC_ON //开启油泵以检测OC
                for(b=0;b<=260;b++);
                mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
                P_PUMPC_OFF 
                
                if(P_33814REL1_OC == 1)//先检测短路状态
                {
                    if(nb_pump_oc_er<255)nb_pump_oc_er++;
                } else nb_pump_oc_er=0;
            }
        }
        else    //只诊断当前状态故障模式
        {
            if(P_33814REL1_OPEN == 1)   //先检测开路状态
            {
                if(nb_pump_open_er<255)nb_pump_open_er++;
            } else nb_pump_open_er=0;
            
            //OC
            if(P_33814REL1_OC == 1)
            {
                if(nb_pump_oc_er<255)nb_pump_oc_er++;
            } else nb_pump_oc_er=0;
        }
        
        //过热
        if(P_33814REL1_OT == 1)
        {
            if(nb_pump_ot_er<255)nb_pump_ot_er++;
        } else nb_pump_ot_er=0;
        
        //SG
        if(P_33814REL1_SHORT == 1)
        {
            if(nb_pump_short_er<255)nb_pump_short_er++; 
        } else nb_pump_short_er=0;
        
        
        //以下是累计次数
        if(nb_pump_open_er>NB_PUMP_LIMIT) 
        {
            VF_OPEN_33814PUMPCTL=true;
        } else {
            VF_OPEN_33814PUMPCTL=false;
        }
        
        if(nb_pump_ot_er>NB_PUMP_LIMIT) 
        {
            VF_OT_33814PUMPCTL=true;
        } else {
            VF_OT_33814PUMPCTL=false;
        }
        
     /*   if(nb_pump_short_er>NB_PUMP_LIMIT) 
        {
            VF_SHORT_33814PUMPCTL=true;
        } else {
            VF_SHORT_33814PUMPCTL=false;
        }	*/		//20211002 chenwei 环松接油泵继电器存在该问题故障，先屏蔽掉
        
        if(nb_pump_oc_er>NB_PUMP_LIMIT) 
        {
            VF_OC_33814PUMPCTL=true;
        } else {
            VF_OC_33814PUMPCTL=false;
        }
        
        if(nb_pump_detect<255)nb_pump_detect++;
        
        if(P_33814REL1_ST != 0)//每次完成后要清零
        {
            mc33814_SPIRwWord_def(_4000_WRITE_STATUS, _0200_RELAY1_FAULT, 0U);
        }
   }


#ifdef SYS_O2HEATER_S 
   //2017-12-12,ysl,add,其他驱动诊断氧加热
   void DTC_DiagO2H_def(byte b_state)
   {
        word b;

        mc33814_ReadStatus_def(_0700_O2H_FAULT);//, false, 0);
        VF_O2H_OC=false;	//20211005 chenwei add
		VF_O2H_OPEN=false;
        if(b_state == DGN_LSD_ALL)//全诊断
        {
            if(P_O2H_ONOFF_TST)
            {
                //OC
                if(P_33814O2H_OC == 1)
                {
                    if(nb_o2h_oc_er<255)nb_o2h_oc_er++;
                } else nb_o2h_oc_er=0;
                
                P_O2H_OFF
                for(b=0;b<=1000;b++);

                mc33814_ReadStatus_def(_0700_O2H_FAULT);//, false, 0);
                P_O2H_ON
                
                //开路
                if(P_33814O2H_OPEN == 1)
                {
                    if(nb_o2h_open_er<255) nb_o2h_open_er++;
                } else nb_o2h_open_er=0;
            }
            else
            {
                //开路
                if(P_33814O2H_OPEN == 1)
                {
                    if(nb_o2h_open_er<255) nb_o2h_open_er++;
                } else nb_o2h_open_er=0;
                
                P_O2H_ON
                //for(b=0;b<=260;b++);		//20211015 chenwei 这里G128 O2H拉高拉低耗时154.8us，同时这样7811才28.4us，此处延迟较短读取不到33814状态，下面的OC未能置位
                FUN_IICWait(1400);			//20211015 测试650耗时76.4us OC正常置位，	1000-108.4us		1500-160us	1400-148us

                mc33814_ReadStatus_def(_0700_O2H_FAULT);//, false, 0);
                
                P_O2H_OFF
                //OC
                if(P_33814O2H_OC == 1)
                {
                    if(nb_o2h_oc_er<255)nb_o2h_oc_er++;
                } else nb_o2h_oc_er=0;
            }
        }
        else
        {
            //开路
            if(P_33814O2H_OPEN == 1)
            {
                if(nb_o2h_open_er<255)nb_o2h_open_er++;
            } else nb_o2h_open_er=0;

            //OC
            if(P_33814O2H_OC == 1)
            {
                if(nb_o2h_oc_er<255)nb_o2h_oc_er++;
            } else nb_o2h_oc_er=0;
        }


        if(nb_o2h_open_er>NB_INJ_LIMIT) 
        {
            VF_OPEN_33814O2HCTL=true;
        } else {
            VF_OPEN_33814O2HCTL=false;
        }
        
        if(nb_o2h_oc_er>NB_INJ_LIMIT) 
        {
            VF_OC_33814O2HCTL=true;
        } else {
            VF_OC_33814O2HCTL=false;
        }
		//20210308 chenwei add 
		if(VF_OPEN_33814O2HCTL)//开路或对GND短路
		{
			VF_O2H_OPEN=true;  
		}

		if(VF_OC_33814O2HCTL)//对VCC短路
		{
			VF_O2H_OC=true;
		}
		
        if(nb_o2h_detect<255)nb_o2h_detect++;
        
        if(P_33814O2H_ST != 0)
        {
            mc33814_SPIRwWord_def(_4000_WRITE_STATUS, _0700_O2H_FAULT, 0U);
        }
   }
#endif


//2018-06-23,怠速步进电机开路诊断，对应增加的诊断电路
void DTC_DiagIACVOpen_def()
{
    word b;
    word ptp;
    
    //如果电机未动作，则可以执行诊断，否则直接跳出
    if(v_StepAct!=0) return;
    
    ptp = *((volatile uint32_t *)(GPIOB_IDR_ADDR));//PTP; //保留PTP值，其中包含StepMotorEN1和StepMotorEN2
    
    //关闭1，2线圈输出电流
    StepMotorEN1 = 1;
    StepMotorEN2 = 1;
    
    for(b=0;b<=2800*3;b++);//延时1ms

    if(P_ISCVCOIL1_OPEN == 0)//线圈1
    {
      if(nb_iacv_c1_open_er < 255) 
      {
        nb_iacv_c1_open_er++;
      }
    }else nb_iacv_c1_open_er = 0;
    
    if(P_ISCVCOIL2_OPEN == 0)//线圈2
    {
      if(nb_iacv_c2_open_er < 255) 
      {
        nb_iacv_c2_open_er++;
      }
    }else nb_iacv_c2_open_er = 0;
    
    //还原电机线圈状态
    StepMotorEN1 = (ptp>>12)&0x01;//((ptp&0x01==0) ? 0 : 1);
    StepMotorEN2 = (ptp>>13)&0x01;//((ptp&0x02==0) ? 0 : 1);
    
    if(nb_iacv_c1_open_er>NB_INJ_LIMIT)
    {
       VF_C1OPEN_IACV = true; 
    }
    else
    {
       VF_C1OPEN_IACV = false; 
    }
    if(nb_iacv_c2_open_er>NB_INJ_LIMIT)
    {
       VF_C2OPEN_IACV = true; 
    }
    else
    {
       VF_C2OPEN_IACV = false; 
    }
    //累加诊断次数
    if(nb_iacv_detect<255) nb_iacv_detect++;
}



//2018-06-23,怠速步进电机短路诊断，对应增加的诊断电路，对于目前4拍的驱动，一直都有电流流过H桥，所以不用
//特意开启电机线圈电流，必须使H桥成通路状态才能诊断短路；如果用8拍，那就应当注意没有电流的情况
void DTC_DiagIACVShort_def()
{
    byte i;
    byte divider;
    
    //如果电机未动作，则可以执行诊断，否则直接跳出
    if(v_StepAct!=0) return;
    
    (v_dgnIacvAdcCount >= (IACV_SMPLE_POINT-1)) ? (v_dgnIacvAdcCount = 0) : (v_dgnIacvAdcCount++);

    //新一轮开始，初始化变量
    if(v_dgnIacvAdcCount == 0)
    {
       v_dgnIacvCoil1AdAv = 0;
       v_dgnIacvCoil2AdAv = 0;
       v_dgnIacvCoil1AdMaxAv = 0;
       v_dgnIacvCoil2AdMaxAv = 0;
       v_dgnIacvCoil1Ad0Count = 0;
       v_dgnIacvCoil2Ad0Count = 0;
       for(i = 0; i < 5; i++)
       {
          v_dgnIacvCoil1AdMax[i] = 0;
          v_dgnIacvCoil2AdMax[i] = 0;
       }
    }
    
    v_Coi1_ad10bit = FAD_1Channel(CH_AD_ISCVCOI1,false,2);
    v_Coi2_ad10bit = FAD_1Channel(CH_AD_ISCVCOI2,false,2);
            
    v_dgnIacvCoil1AdAv += v_Coi1_ad10bit;
    v_dgnIacvCoil2AdAv += v_Coi2_ad10bit;
    
    //统计采集为0的次数
    if(v_Coi1_ad10bit <= 1) v_dgnIacvCoil1Ad0Count++;
    if(v_Coi2_ad10bit <= 1) v_dgnIacvCoil2Ad0Count++;
    
    //记录采集的最大值到数组中
    if(v_Coi1_ad10bit >= v_dgnIacvCoil1AdMax[0]) 
    {
        for(i = 4; i > 0; i--)
        {
           v_dgnIacvCoil1AdMax[i] = v_dgnIacvCoil1AdMax[i-1];
        }
        v_dgnIacvCoil1AdMax[0] = v_Coi1_ad10bit;
    }
    if(v_Coi2_ad10bit >= v_dgnIacvCoil2AdMax[0]) 
    {
        for(i = 4; i > 0; i--)
        {
           v_dgnIacvCoil2AdMax[i] = v_dgnIacvCoil2AdMax[i-1];
        }
        v_dgnIacvCoil2AdMax[0] = v_Coi2_ad10bit;
    }

    //采集的点数已足够，则对本轮的采集进行分析
    if(v_dgnIacvAdcCount >= (IACV_SMPLE_POINT - 1))
    {
        for(i = 0; i < 5; i++)
        {
           v_dgnIacvCoil1AdMaxAv += v_dgnIacvCoil1AdMax[i];
           v_dgnIacvCoil2AdMaxAv += v_dgnIacvCoil2AdMax[i];
        }
        divider =  IACV_SMPLE_POINT/10;
        v_dgnIacvCoil1AdAv /= IACV_SMPLE_POINT;
        v_dgnIacvCoil2AdAv /= IACV_SMPLE_POINT;
        
        v_dgnIacvCoil1AdMaxAv = v_dgnIacvCoil1AdMaxAv/5;
        v_dgnIacvCoil2AdMaxAv = v_dgnIacvCoil2AdMaxAv/5;  
        
        //电机正常的判定，根据后期的电路调整，可以适当更改阈值/判定值
        if(
           ((!VF_C1OPEN_IACV&&!VF_C2OPEN_IACV)//非开路时有非开路的条件
           &&((v_dgnIacvCoil1AdAv>=3&&v_dgnIacvCoil1AdAv<=8) && (v_dgnIacvCoil2AdAv>=3&&v_dgnIacvCoil2AdAv<=8))                //平均值比较理想
           &&((v_dgnIacvCoil1Ad0Count<=divider&&v_dgnIacvCoil2Ad0Count<=divider)))  //采集为0的次数极少
           ||
           (VF_C1OPEN_IACV//线圈1开路时有开路的条件
           &&(v_dgnIacvCoil2AdAv==0)
           &&(v_dgnIacvCoil2Ad0Count>=(IACV_SMPLE_POINT-divider))
           &&(v_dgnIacvCoil2AdMaxAv<4))
           ||
           (VF_C2OPEN_IACV//线圈2开路时有开路的条件
           &&(v_dgnIacvCoil1AdAv==0)
           &&(v_dgnIacvCoil1Ad0Count>=(IACV_SMPLE_POINT-divider))
           &&(v_dgnIacvCoil1AdMaxAv<4))
         )
        {
            nb_iacv_short_er = 0;
        }
        else
        {
            if(nb_iacv_short_er < 255) nb_iacv_short_er++; //统计短路故障次数
        }
    }
    
    if(nb_iacv_short_er>NB_INJ_LIMIT)
    {
        VF_SHORT_IACV = true; 
    }
    else
    {
        VF_SHORT_IACV = false; 
    }
}


/***********************************************************************************************************************
* 函数名   : DTC_Diag33814Reset_def
* 功能     : MC33814复位诊断
* 日期     : 2017/12/28
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void DTC_Diag33814Reset_def(void)
{
    byte b_VRSParam = 0;

    b_VRSParam = (byte)mc33814_ReadData_def(_1000_READ_CONFIG, _0B00_VRSENGRUNP_CONF);
    if(b_VRSParam != VRSCONFPARAM) 
    {
        b_VRSParam = (byte)mc33814_ReadData_def(_1000_READ_CONFIG, _0B00_VRSENGRUNP_CONF);
        if(b_VRSParam != VRSCONFPARAM)
        {
            //mc33814_ResetInit_init();
        }
    }
}


//诊断后的保护操作,OC 2018-1-17,add
void DTC_DiagLdoProtect_def(void)
{
    if(P_33814REL1_OC == 1)
    {
        if(P_PUMPC_ONOFF_TST == 1) v_33814LdoOnOff |= 0x10U;   
        else  v_33814LdoOnOff &= 0xefU; 
          
        P_PUMPC_OFF 
    }
    else if((P_PUMPC_ONOFF_TST == 0) && (v_33814LdoOnOff & 0x10U) != 0)//之前是开启的，要还原 
    {
        P_PUMPC_ON
    }
    
    #ifdef SYS_O2HEATER_S 
      if(P_33814O2H_OC == 1)
      {
          if(P_O2H_ONOFF_TST == 1) v_33814LdoOnOff |= 0x20U;   
          else  v_33814LdoOnOff &= 0xdfU; 
          
          P_O2H_OFF 
      }
      else if((P_O2H_ONOFF_TST == 0) && (v_33814LdoOnOff & 0x20U) != 0)//之前是开启的，要还原 
      {
          P_O2H_ON
      }
    #endif
}


//各部件的诊断 2018-1-9,add
void DTC_Diag_def(word prd_cas)
{
    byte b_isDiag33814Reset;// = false;  //诊断33814是否复位的标志
    static word last_prd_cas;
    //static byte b_Count = 0;
    
/*2020-5-20 暂时不进行怠速提高阀的诊断
    //-------------怠速提高阀步进电机诊断---------------
    if(!VF_MMOVE_ENG)
    {
       DTC_DiagIACVShort_def();   //短路诊断需要采集较多的数据来分析，故实时性要求高一些，每个循环都要采集数据诊断
       if(VF_PRDMAINTMR_TMR)
       {
          DTC_DiagIACVOpen_def();  //开路诊断
       }
    }*/
    
    /*----------------33814复位诊断--------------------*/
    //运行过程中发现prd_cas突变，疑似认为33814复位，可以更改条件更为精准些
    /*if(VF_MMOVE_ENG && (prd_cas<(last_prd_cas-last_prd_cas>>2)||prd_cas>(last_prd_cas+last_prd_cas>>2)))
    {2020-6-24 这个条件有点问题——xl
        VF_33814DIAGCAS_GEN = true;
    }
    else
	if(VF_MMOVE_ENG&&VF_PRDMAINTMR_TMR)//发动机运行时达到诊断周期
    {
        VF_33814DIAGPRD_GEN = true;
    }
    
	VF_33814DIAGCAS_GEN=false;
	b_isDiag33814Reset=false;
    if((VF_33814DIAG_RESET&&VF_33814DIAGPRD_GEN)||VF_33814DIAGCAS_GEN)//两个条件同时满足，才诊断33814复位
    {
        DTC_Diag33814Reset_def(); 
        VF_33814DIAG_RESET = false;
        VF_33814DIAGCAS_GEN = false;
        VF_33814DIAGPRD_GEN=false;
        b_isDiag33814Reset = true;
    }
    last_prd_cas = prd_cas;*/
    
    /*----------------33814LSD诊断--------------------*/
    //没有诊断过33814复位，才能诊断其他LSD故障
	b_isDiag33814Reset=false;
	
    if(!b_isDiag33814Reset)
    {
        //单纯开钥匙上电到达260ms周期
        if(!VF_MMOVE_ENG&&VF_PRDMAINTMR_TMR)
        {
            if((VF_PREINJEND_SYSINI&&VF_PREPUMPEND_SYSINI&&VF_NOTNEED_DROPSCAN_SYSINI&&!VF_DOM_SYS&&!VF_PREINJ_2CA)
            ||VFF_INJ_DTC     //2009-12-18 
            )
            {
            	  F_Diag_INJ(DGN_LSD_ALL);//if(INJEND)call	hart_t					;       //检测喷油器故障；
            	  
            	  //如果有异常，全面诊断，否则部分诊断，符合功能异常全面诊断的目的
            	  if(P_33814REL1_ST != 0 || !VF_33814PUMPSELFCHECK)
            	  {
            	      DTC_DiagPump_def(DGN_LSD_ALL);//诊断油泵 2017-12-1,ysl,add
            	  }
            	  else
            	  {
            	      DTC_DiagPump_def(DGN_LSD_NOWSTATE);//诊断油泵 2017-12-1,ysl,add
            	  }
            }

            F_Diag_IGN(DGN_LSD_ALL);//诊断点火电路
            
            #ifdef SYS_O2HEATER_S 
              DTC_DiagO2H_def(DGN_LSD_ALL);//诊断氧传感器 2017-12-1,ysl,add
            #endif
			DTC_DiagMIL_def();//20210308 chenwei add 诊断MIL
            
        }
        //发动机运行中到达间隔100ms和角标周期中间，分步检测，那么其实每个部件的诊断周期是400ms
        else if(VF_33814DIAG_GEN && VF_33814DIAGT_GEN)
        {
            if(b_Count == 0)
            {
                F_Diag_INJ(DGN_LSD_NOWSTATE);   //喷油器经常在两种状态切换DGN_LSD_NOWSTATE也能完全诊断
            }
            else if(b_Count == 1)
            {
                F_Diag_IGN(DGN_LSD_NOWSTATE);    //点火经常在两种状态切换DGN_LSD_NOWSTATE也能完全诊断
            }
            else if(b_Count == 2)
            {
                DTC_DiagPump_def(DGN_LSD_NOWSTATE); //油泵长期保留在一种开关状态 DDGN_LSD_NOWSTATE只能部分诊断
            }
            else if(b_Count == 3)
            {
              #ifdef SYS_O2HEATER_S 
                DTC_DiagO2H_def(DGN_LSD_NOWSTATE);  //氧加热长期保留在一种开关状态 DDGN_LSD_NOWSTATE只能部分诊断
              #endif
            }
            
            if(b_Count >= 3) b_Count = 0;
            else b_Count++;
            
            VF_33814DIAG_GEN = false;
            VF_33814DIAGT_GEN = false;
        }
        //DTC_DiagLdoProtect_def();  //诊断后的保护操作,OC 2018-1-17,add
    } 

	DTC_DiagNoCA_def();		//20211009 chenwei add 角标无信号诊断 
}

/****功能：MIL部分诊断，仅诊断开路、短路*****
*****日期：20210302 chenwei
*****属性：新增代码段*/

void DTC_DiagMIL_def(void)
   {
        word b;

        mc33814_ReadStatus_def(_0500_LAMP_FAULT);
		VF_MIL_OCOROPEN=false;		//20211007 chenwei add
          //只诊断当前状态故障模式
        if(P_33814LAMP_OPEN == 1)   //先检测开路状态
        {
            if(nb_MIL_open_er<255)nb_MIL_open_er++;
        } else nb_MIL_open_er=0;
            
         //OC
        if(P_33814LAMP_OC == 1)
        {
             if(nb_MIL_oc_er<255)nb_MIL_oc_er++;
        } else nb_MIL_oc_er=0;
        
        //以下是累计次数
		if((nb_MIL_oc_er>NB_MIL_LIMIT)||(nb_MIL_open_er>NB_MIL_LIMIT))
        {
            VF_MIL_OCOROPEN=true;
        } else {
            VF_MIL_OCOROPEN=false;
        }
		
        if(nb_MIL_detect<255)nb_MIL_detect++;
   }


/****功能：角标信号无信号诊断*****
*****日期：20211008 chenwei
*****属性：新增代码段
*****通过检测车辆未动作，但电压及压力存在变化，从判断车辆发生了启动*/

void DTC_DiagNoCA_def(void)
{
	byte b1;
	byte b2;
	sbyte sb1;
	sbyte sb2;
	
	if(!VF_MMOVE_ENG)
	{
		if(v_pair_ad_input_max_nmov_eng==0){v_pair_ad_input_max_nmov_eng=v_pair_ad_input;}
		if(v_pair_ad_input_min_nmov_eng==0){v_pair_ad_input_min_nmov_eng=v_pair_ad_input;}

		if(v_Vbat_inuse_max_nmov_eng==0){v_Vbat_inuse_max_nmov_eng=v_Vbat_inuse;}
		if(v_Vbat_inuse_min_nmov_eng==0){v_Vbat_inuse_min_nmov_eng=v_Vbat_inuse;}
		
		if(v_pair_ad_input_max_nmov_eng<v_pair_ad_input_lst)
		{
			v_pair_ad_input_max_nmov_eng=v_pair_ad_input_lst;	//找压力采样最大值
		}
		if(v_pair_ad_input_min_nmov_eng>v_pair_ad_input_lst)
		{
			v_pair_ad_input_min_nmov_eng=v_pair_ad_input_lst;	//找压力采样最小值
		}

		if(v_Vbat_inuse_max_nmov_eng<v_Vbat_inuse_lst)
		{
			v_Vbat_inuse_max_nmov_eng=v_Vbat_inuse_lst;	//找电压采样最大值
		}
		if(v_Vbat_inuse_min_nmov_eng>v_Vbat_inuse_lst)
		{
			v_Vbat_inuse_min_nmov_eng=v_Vbat_inuse_lst;	//找电压采样最小值
		}

		b1=v_pair_ad_input_max_nmov_eng-v_pair_ad_input_min_nmov_eng;
		b2=v_Vbat_inuse_max_nmov_eng-v_Vbat_inuse_min_nmov_eng;
		if(((b1>P_CrankDeltaMAP_Thrsh)&&(b2>U_CrankDeltaIgn_Thrsh))&&(!VF_NOCA_DIAG))	
		{
			VF_NOCA_DIAG=true;
			nb_noca_test++;
		}
		
		sb2=v_Vbat_inuse_max_nmov_eng-v_Vbat_inuse;
		if(sb2<0)	sb2=-sb2;
		if((sb2<U_CrankDeltaIgn_NMOVE_Thrsh)&&VF_NOCA_DIAG)
		{
			VF_NOCA_DIAG=false;
			v_pair_ad_input_max_nmov_eng=v_pair_ad_input;
			v_pair_ad_input_min_nmov_eng=v_pair_ad_input;

			v_Vbat_inuse_max_nmov_eng=v_Vbat_inuse;
			v_Vbat_inuse_min_nmov_eng=v_Vbat_inuse;
		}

		if(nb_noca_test>NB_DTC_NOCA)
		{
			VF_CA_NOSIGNAL=true;
			VF_NOCA_DIAG=false;
			nb_noca_test=0;
		}

		v_pair_ad_input_lst=v_pair_ad_input;
		v_Vbat_inuse_lst=v_Vbat_inuse;
	}

	else VF_CA_NOSIGNAL=false;
	
}

