/*HEAD
 ***********************************************************************
 * Filename	:dtc.h
 * Function	:故障诊断模块头文件	
 * User		:yangsili
 * Date		:2017.12.30
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.30	yangsili
 *		1.添加喷油开路诊断、油泵、O2H诊断函数声明
 *    2.添加诊断前准备，后操作函数声明
 *
 ***********************************************************************
HEAD END*/

/*===========================================================*/
/*====故障诊断模块=====================*/
/*===========================================================*/

#include "FAM.h"
#include "sys_definition.h"

//功能：DTC参数的初始化
extern void FI_DTC_AfterReset(void);

//功能：诊断/设置故障码并指示；
extern void F_DiagDisp(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy); //2008-8-1 add 形参

//功能：喷油器开路或短路诊断；并得到code_number,binjector故障码
extern void F_Diag_INJ(byte b_state);

void DTC_DiagINJOpen_def(void);//2017-12-29,ysl,add 功能：喷油器状态实时读取，主要用于开路条件刷新

extern void F_Diag_IGN(byte b_state);//诊断点火电路

extern void DTC_DiagPump_def(byte b_state);//油泵诊断，2017-12-12,ysl,add

extern void DTC_DiagO2H_def(byte b_state);//氧加热诊断，2017-12-12,ysl,add?

extern void DTC_Diag33814Reset_def(void);//33814复位诊断,2017-12-28,ysl,add

extern void DTC_Diag_def(word prd_cas);  //各部件的诊断

extern void F_Diag_AirV(void);//2014-10-28
extern void F_Diag_CoolV(void);//2014-11-27

//功能：关钥匙时累加故障码到历史故障次数中；
//      当led_status_con>=8时，累加故障次数，并通过调用sum_er_inc，将EE中相应的故障数值累加并保存；
//      sum_er_inc的输入参数是W,也就是W值是地址；
//      在检测到要关钥匙时，被调用；
extern void FM_SumError_WhenKeyOff(void);

//功能：根据诊断结果设置故障码--CODE_NUMBER
extern void F_SumFailure(void);

//功能：根据诊断结果进行指示
extern void F_ShowMilFunction(void);

extern void F_Diag_CA(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy);//2008-8-1 诊断角标
//是否显示其他页
extern byte F_DispOtherPageOrNot(void);

extern byte FG_DispPage_ForIgn(void);

extern byte FG_DispCode_ForCos(void);

extern byte FG_DispCode_ForDtfiL(void);

extern byte FG_DispCode_ForTBack(void);

//extern void F_Diag_ISCV(void);//2008-8-5 添加

extern byte FG_DispCode_ForPN0Cos(void);//2008-11-14 

extern void Clr_DTC_History(void);

extern void F_Diag_INJ_atOFF(void); //2014-11-12
extern void F_Diag_INJ_atON(void);

//-----------public variables--------------------------------
//extern byte CODE_NUMBER;//故障码：与刘老师程序的兼容
extern word v_flag_odd_qh_asc;
extern word v_flag_odd_qh_dec;
extern word v_flag_even_qh_asc;
extern word v_flag_even_qh_dec;
extern byte nb_odd_ystdc;
extern byte nb_even_ystdc;

extern byte nb_DTC;
extern byte nb_QYTUBIAN;//QIANYAN TUBIAN DE CISHU

//2009-12-11 add for  CA diag
extern byte nb_qhy_err;//前后沿时间间隔超标的累计的次数；  VF_QHYCA_INTERR_IGN nb_qhy_err t_qhymax_p

extern byte nb_ign_open_er;         //
extern byte nb_ign_short_er;         //
extern byte nb_ign_detect;         //

extern byte v_rev62d5_hyUL;
extern byte nb_hy_lack;        //缺少后沿的次数

//extern void FS_CA_st(void); 2014-10-7
extern void FS_CA_st(sbyte ignCa);

//对时间基准的诊断部分
extern unsigned long int t_acc_for10ms;
extern unsigned long int t_acc_forCAOV;
extern unsigned long int t_acc_StopAD;
extern unsigned long int t_acc_StopCom;
extern unsigned long int t_acc_MIL;

#define FCLEAR_CAOV       t_acc_forCAOV=0;
#define FCLEAR_ADATSTOP   t_acc_StopAD=0;
#define FCLEAR_COMATSTOP  t_acc_StopCom=0;
#define FCLEAR_MIL        t_acc_MIL=0;

extern void FI_TimeOutFlag(void);
extern void Shift_Status(void);
extern void Check_KeyStatus(void);



/*======DTC_ACT============================================================================================*/
//    typedef int8_t          sbyte ;
//    typedef int16_t         sword ;
//    typedef unsigned char   bool;
    


//关于喷射的参数
extern word v_t_bInj_toTrig;       //的触发时刻到喷射时刻的时间
extern word v_t_bInj_toCas;        //的触发用的角标的时刻到喷射时刻的时间
extern word v_t_bInj_toCas_tmp;        //的触发用的角标的时刻到喷射时刻的时间

extern word v_t_T1;

extern word v_t_bInj_toTrig_r;    //实际的触发时刻到喷射时刻的时间
extern word v_t_bInj_toCas_r;     //实际的触发用的角标的时刻到喷射时刻的时间
extern word v_t_T1_r;

extern word v_t_Cas_trigT1;       //触发T1的角标的时间
extern word v_t_trigT1;           //触发T1比较输出的时刻

//关于点火的参数
extern word v_t_bIgn_toTrig;       //的触发时刻到点火时刻的时间
extern word v_t_bIgn_toCas;        //的触发用的角标的时刻到点火时刻的时间
extern word v_t_IgnOn;
extern word v_t_IgnOn_LookUp;		//20210602 cw add 查表得到的充磁时间


extern word v_t_bIgn_toTrig_r;    //时间的触发时刻到点火时刻的时间
extern word v_t_bIgn_toCas_r;     //实际的触发用的角标的时刻到点火时刻的时间
extern word v_t_IgnOn_r;

extern word v_t_Cas_trigIgn;      //触发点火的角标的时间
extern word v_t_trigIgn;          //触发点火比较输出的时刻
extern word v_t_IgnCharge_r;       // 实际点火充电的时刻

extern word v_tl_IgnPreCharge;      //准备点火充电的时间
extern word v_tl_IgnPreCharge_r;      //准备点火充电的实际时间

//关于执行的次数
extern word v_nb_ign;             //触发点火的次数
extern word v_nb_inj;             //触发喷油的次数
extern word v_nb_ignOn_r;           //真实的点火的次数
extern word v_nb_injOn_r;           //真实的喷油的次数
extern word v_nb_ignOff_r;           //真实的点火的次数
extern word v_nb_injOff_r;           //真实的喷油的次数

extern word v_nb_ccl_r;              //发动机工作圈数

extern word v_t_injOn_r;              //INJ_ON的时刻
extern word v_t_injOff_r;              //INJ_Off的时刻
extern word v_t_IgnSetOn_r;              //IGN_ON的时刻

extern sword v_dt_bInj_toCas;
extern sword v_dt_T1;
extern sword v_dt_bIgn_toCas;

extern sword v_dnb_Ign;
extern sword v_dnb_Inj;
extern sword v_dnb_T1;

extern sword v_dnb_Ign_YSTDC;
extern sword v_dnb_Inj_YSTDC;

extern word v_nb_YSTDC;
extern word v_nb_IgnInt;   //点火中断的次数

extern void FI_ActDTC_afterReset(void);

extern byte nb_airv_open_er;         //2014-11-12
extern byte nb_airv_short_er;        

extern byte CheckIsSPNNG(byte id,byte DTC_TYPE);
extern void SetSpnStatus(byte blnSet,byte id,byte DTC_TYPE);
extern void ClearSpnStatus(byte DTC_TYPE);
extern dword rd_byAddGrp(byte nbbytes,word add_1st);

extern void Save_AD_cas(byte ad);//2016-2-28
extern byte v_AD_cas;//2016-2-28 cas采样值；8bit 2016-2-28

extern byte v_CA_break;


#define MV_PAIR 0
#define MV_TES 1
#define MV_TAS 2
#define MV_TPS 3


#define MV_L_LIMIT 0
#define MV_U_LIMIT 1
#define MV_NORMAL  2


extern void Accu_AD_Mv(byte mv_type,byte limit_type);//累加各个采样值的最值；2016-2-28

extern void DTC_DiagMIL_def(void);  //20210308 chenwei add MIL灯诊断
//20210923 chenwei add
extern void MIL_ACTION_FORHUANSONG(word pcode);
extern void SAVE_PCODE_FOROBD4(word pcode);
extern void DTC_DiagNoCA_def(void);		//20211009 chenwei add

extern byte v_n_SPN_ng;
extern byte v_t_fromkeyon_1s;

