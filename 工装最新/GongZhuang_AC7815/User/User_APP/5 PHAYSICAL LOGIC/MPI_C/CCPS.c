/*HEAD
 ***********************************************************************
 * Filename	:CCPS.c
 * Function	:CCPS open conditin and control		
 * User		:chenwei
 * Date		:2020.11.26
 * Version	:V1.0.0
 *
 *** History ***
 * V1.0.0	2020.11.26	chenwei 
 *		Base function.
 *
 ***********************************************************************
HEAD END*/



#include "public.h"
#include "status_st.h"
#include "look_up.h"
#include "ign.h"
#include "ad.h"
#include "add_flash_mpi.h"



byte CCPS_Pct_DutyCycleMax_C;    //允许活性碳罐工作的最大碳罐电磁阀占空比
byte CCPS_Pct_DutyCycleMin_C;    //允许活性碳罐工作的最小碳罐电磁阀占空比
byte CCPS_Pct_DutyCycleIdleMax_C;    //允许活性碳罐工作的怠速时的碳罐电磁阀占空比

extern  byte v_cf_1s;         //断油时间
extern	sbyte v_UP_M128;     //全负荷的U
extern byte v_t_WOT_1s;      //油门全开的时间
extern word t_after1stStart_s;    //开钥匙后，第一次启动后开始计时，包括中间停车
extern unsigned long int v_t_eng_t_s;  //车辆一值维持该状态的时间，从零开始累积


#define CCPS_open 1          //CCPS开启条件
#define CCPS_ENGSTATUS 2     //发动机工作状态要求
#define CCPS_RUNTIME 4       //运行时间条件
#define CCPS_TMP  8          //发动机温度条件
#define CCPS_TPS  16          //油门开度条件
#define CCPS_SPEED  32       //转速要求
#define CCPS_VBAT   64       //电压要求
#define CCPS_UP     128       //up<80条件

word V_CCPS_st_MskEna;          //碳罐电磁阀启用状态字
#define t_CCPS_period 64          //碳罐电磁阀一个周期的时间单位ms，此为占空比的分母,64ms为借鉴德尔福
#define CCPS_swt_open 1         //表示开启CCPS


word v_t_CCPS_closelast;           //CCPS上次关闭的时刿
word v_t_CCPS_opennow;               //CCPS本次开启的时刻

void FI_CCPS_afterReset(void);
void F_CCPSOpen_Condition(void);
void F_CCPS_ACTION(void);

extern sword v_te;
extern byte v_Vbat_inuse;
extern sword V_Tw;
extern sbyte v_LambdaTg;

void FI_CCPS_afterReset(void)
{
	CCPS_Pct_DutyCycleMax_C=0;
	CCPS_Pct_DutyCycleMin_C=0;
	CCPS_Pct_DutyCycleIdleMax_C=0;
	v_t_CCPS_closelast=0;
	v_t_CCPS_opennow=0;
	V_CCPS_st_MskEna=0;
	
	
}


/*******功能：碳罐电磁阀（CCPS＿开启的使能条件********/
void F_CCPSOpen_Condition(void)
{

	VF_CCPS_status=false;  
		V_CCPS_st_MskEna=0;

//1.打开CCPS条件
	if(BYPTR_M0_CCPS_swt_CCPSStrt_C==CCPS_swt_open)
	{
		V_CCPS_st_MskEna+=CCPS_open;
	}
		
//2.发动机工作状态要求(满足下列任一条件)
	if(VF_CLSMODE_IACV                                         //进入闭环
		||(VF_DECSPD_TRANS&&v_cf_1s<BYPTR_M0_CCPS_ti_DurDFCORunTmLim_C)            //减速断油阶段减速断油时间小于某一值
		||((VF_ACCSPD_TRANS||VF_TRANSGT15_TRANS||(!VF_TRANSLT10_TRANS))&&v_t_eng_t_s<BYPTR_M0_CCPS_ti_DurPERunTmLim_C)   //加速阶段且维持该状态时间小于
		)  
	{
		V_CCPS_st_MskEna+=CCPS_ENGSTATUS;
	}
	
//3.发动机运行时间要求（满足下列任一条件）
	if(VF_MMOVE_ENG
		&&((V_Tw<80&&t_after1stStart_s>=BYPTR_M0_CCPS_ti_CldStrTmtMin_C)
			||(V_Tw>80&&t_after1stStart_s>=BYPTR_M0_CCPS_ti_WrmStrtTmMin_C))
		)                       //冷机或暖机启动阶段，发动机运行时间均需要大于一定时间
	{
		V_CCPS_st_MskEna+=CCPS_RUNTIME;	
	}  
	
//4.发动机缸温要求
	if((v_te>BYPTR_M0_CCPS_t_EngCydTmpLo_C)&&(v_te<BYPTR_M0_CCPS_t_EngCydTmpHi_C))
	{
		V_CCPS_st_MskEna+=CCPS_TMP;
	}
	
//5.油门开度条件
	if((v_TPS_odr>BYPTR_M0_CCPS_Pct_TPSMin_C)&&(v_TPS_odr<BYPTR_M0_CCPS_Pct_TPSMax_C))
	{
		V_CCPS_st_MskEna+=CCPS_TPS;
	}

//6.转速要求
	if((v_rev_625rpm<BYPTR_M0_CCPS_n_EngSpdHi_C)&&(v_rev_625rpm>BYPTR_M0_CCPS_n_EngSpdLo_C))
	{
		V_CCPS_st_MskEna+=CCPS_SPEED;
	}
	
//7.电压要求
	if(v_Vbat_inuse<BYPTR_M0_CCPS_u_BattVltgMax_C)
	{
		V_CCPS_st_MskEna+=CCPS_VBAT;
	}
	
//8.氧传感器信号要求 短暂的加浓阶段
//8.UP<80 条件
	if(v_LambdaTg<80)       //up<80 类似DELPHI 非PE模式
	{
		V_CCPS_st_MskEna+=CCPS_UP;
	}
	
}

void F_CCPS_ACTION(void)
{	
	byte b1;
	word w;
	byte b;
	
	F_CCPSOpen_Condition();
	
	if(V_CCPS_st_MskEna==255)  //满足碳罐电磁阀工作条件，即进行工作
	{
		b=(byte)F_2DLookUp(v_rev_625rpm,BYPTR_M1_CCPS_Pct_DutyCycle,7,0,false);
		b1=t_CCPS_period*(100-b)/100;                       //CCPS开启时刻
		
		v_t_CCPS_opennow=(T_MAINTMR-v_t_CCPS_closelast)/250;
		
		if(v_t_CCPS_opennow>=b1&&v_t_CCPS_opennow<t_CCPS_period)
		{
			CCPS_ON;
			VF_CCPS_close=false;
			VF_CCPS_status=true;    //20201209 chenwei add
		}
		else
		{
			CCPS_OFF;
			VF_CCPS_status=false;    //20201209 chenwe add 考虑显示问题
			if(!VF_CCPS_close)
			{
				v_t_CCPS_closelast=T_MAINTMR;
				VF_CCPS_close=true;
			}
			else
			{
				VF_CCPS_close=true;
			}
		}
	}	
	else 
	{
		CCPS_OFF;
		VF_CCPS_status=false;    //20201209 chenwe add 考虑显示问题
		v_t_CCPS_closelast=T_MAINTMR;   //20201128 chenwei add待测试，目的是解决条件间隙满足不满足的情况，测试ok
	}
	
}


