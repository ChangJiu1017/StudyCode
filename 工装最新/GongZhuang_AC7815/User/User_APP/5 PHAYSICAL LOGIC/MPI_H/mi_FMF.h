/*===========================================================*/
/*====油量修正模块=====================*/
/*===========================================================*/
#include "map_selection.h"

//#define M0_FMF_FOR1CYCLE  1800//单圈的最大喷射量；　2012-10-9
//#define M0_FMF_FOR1CYCLE  3000//单圈的最大喷射量；　2014-12-9
#define M0_FMF_FOR1CYCLE  10000


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/

extern sword v_FMF_remain;  //当一圈的油量超过某个限值时需要拆分油量_FMF_remain为拆分后多余的油量
extern sword v_FMF;         //本圈需要执行的最终油量
extern sword v_FMF_lst; //上一次实际喷射量
extern sword v_FMF_all_lst; //上一次总喷射量

extern sword v_tw1;         //本圈需要执行的T1

//2008-5-29 add

extern sword v_fmf_O;//2009-8-3 重新定义的O值
extern byte   M0_FMF_IDLE    ;//2009-8-20 add 
extern byte v_fmf_grind;//2010-2-3

extern sword vc_FMF_dtfi_trans; //2010-5-14 纯粹的过渡部分的dtfi 

extern word v_dms;//2013-6-22

/*----------the following are declaration of public sub---------------*/
//功能：初始化FMF模块
extern void FI_FMF_AfterReset(void); //油量模块的初始化

//功能：低速增油修正；
//+需要保证低速修正后if(v_FMF_H,v_FMF_L>=256)v_FMF_H,v_FMF_L=256;
//     FMF_lspd--低速油量修正系数
extern sword FC_FMF_LowSpeed(sword fmf,byte FMF_lspd);

extern sword v_FMF_all;//一个循环内需要喷射的总油量  2014-4-14

//-----------rec_by_astk_tmp:--------------------------------------------------------------
//功能：温度、启动增油对油量的共同修正；
//  +map_ti*(128+vc_FMF_STARTS/2)/128+vc_FMF_STARTS+map_ti*coefficient_rzt/128=map_tinj*(coefficien+vc_FMF_STARTS/2)/128+vc_FMF_STARTS/2
//  温度修正
//  获得RZT和TAIR对油量的修正系数，
//  物理含义：
//		v_Tair_ad:直接按理想气体状态方程折算；
//		v_Teng_inuse:考虑对进气的加热效应；
//返回值：修正后的油量
extern sword FC_FMF_TMP(sword fmf,byte fmf_tair,sbyte fmf_teng) ; //2008-11-11 add ,byte fmf_restart

//功能：闭环控制修正；
//返回值：修正后的油量
extern sword FC_CLS(sword fmf,sbyte fmf_cls);

//功能：过渡和断油控制修正；
//返回值：修正后的油量
//extern sword FC_FMF_TransCI(sword fmf,byte tair,byte teng,sbyte fmf_cfci,sbyte fmf_trans1,byte fmf_trans2,byte fmf_starts);

//功能：怠速失火对油量的修正；
//返回值：修正后的油量
extern sword FC_MissF_FMF_Idle(sword fmf,sbyte tw1_idlemissf);

//功能：怠速失火对即将触发的TW1的修正；
//形参：IDLEMISSF_tmp－－vc_TW1_IDLEMISSF_tmp
extern sword FC_MissF_TW1_Idle(sword fmf, sbyte IDLEMISSF_tmp);

//功能：怠速失火对DTFI的修正；
//形参：IDLEMISSF_tmp－－vc_TW1_IDLEMISSF_tmp
extern sword FC_MissFireForDtfi_Idle(sword dtfi,sbyte IDLEMISSF_tmp);

//功能：本圈为非喷油圈（即所准备的是喷油圈的油）时，过大油量的拆分
//      TOBECHECK,需要判断本圈是非喷油圈
//返回值：拆分后的主油量
extern sword F_remain_FMF(sword fmf) ;

//功能：怠速搜索临时量对FMF的修正
//返回值：修正后的油量
extern sword FC_IdleSC_FMF(sword fmf,sbyte tw1_IDLESC_inuse,sword tw1_idle_sc,sword fmf_sci_idle,sword fmf_sci_nidle,sbyte tw3offset);

//功能：T3反馈对T1的修正；
//返回值：修正后的TW1
//       tw1_t3--偏移量2
//extern sword CO_TW1_T3FK(sword tw1,sword tw1_t3_fk_pid,sbyte tw1_t3,sbyte vbat_dt,sword tw1_t3FK_I);

//功能：电池电压变动对TW1的修正
//返回值：修正后的TW1
extern sword FC_UbatChange_TW1(sword tw1,sbyte tw1_vbatchg,sword tw1_vbatdownidle) ;


extern byte v_nb_rinj;     //喷射之前，对油量进行修正的次数；
extern byte v_nb_rinj_y;   //喷射之前，对油量进行修正的次数；
