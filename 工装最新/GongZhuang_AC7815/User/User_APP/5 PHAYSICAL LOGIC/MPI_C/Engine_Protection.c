/*HEAD
 ***********************************************************************
 * Filename	:Engine_Protection.c
 * Function	:Some code for engine protection, include condiction \ diagnose AfterReset,and so on
 * User		:chenwei
 * Date		:2021.1.12
 * Version	:V1.0.0
 *
 *** History ***
 * V1.0.0	2021.1.12	chenwei 
 *		Base function.
 *
 * V1.0.1	2021.1.16	chenwei 
 *		1.增加过温保护对UP补偿的分配函数F_UP_OverTMP();
 *		2.增加对发动机等速工况判断的函数F_status_ENG();
 *
 * V1.0.1	2021.2.7	chenwei 
 *		1.增加当条件不满足时过温保护对UP补偿逐步递减至查表值;
 *
 * V1.0.1	2021.7.13	chenwei
 *		1.调整优化过温后up增量的递增递减方式
 *		2.取消原先的判断等速的方法
 *
 ***********************************************************************
HEAD END*/

#include "public.h"
#include "status_st.h"
#include "look_up.h"
#include "ign.h"
#include "ad.h"
#include "add_flash_mpi.h"
#include "Engine_Protection.h"
#include "data_sys_standard.h"
#include "sci.h"

void FI_EngProtection_AfterReset(void);
void F_OverTMPProtection_Condiction(void);
sbyte F_UP_OverTMP(byte data);
void F_status_ENG(void);   //20210125 chenwei add 发动机等速工况判定
void F_OVERTMP_ACTION(void);  //20210206 chenwei add




//过温保护的状态相关定义
#define OverTMP_SWITCH   1
#define OverTMP_SPEEDANDTPD     2
#define OverTMP_TE      4
#define OverTMP_TIME    8
#define OverTMP_DTC     16
#define OverTMP_YSTDC   32
#define OverTMP_VBAT    64
#define OverTMP_TRANS    128
#define OverTMP_REMARK   256

//过温保护判定参数取值的定义
#define OverTMPProtection_SPEED62D5 104    //6500rpm
#define OverTMPProtection_TPD 12 //TPD 12级
#define OverTMPProtection_VBAT_min  161
#define OverTMPProtection_VBAT_max  241


word V_OverTMP_st_MskUnEna;   //判定不满足发动机过温保护的原因，状态字
    
byte UP_OVERTMP_PROTECTION;  //20210121 chenwei add 过温保护查表得到up补偿值,UP补偿总量



/*******功能：发动机保护策略相关的初始化***********/
void FI_EngProtection_AfterReset(void)
{
	VF_OVERTEMPRATRUEPROTECTION_ENG=false;
	VF_OVERTEMANDHIGHLOAD_ENG=false;
	V_OverTMP_st_MskUnEna=0;
	UP_OVERTMP_PROTECTION=0;  //20210206 chenwei add
	v_LambdaTg_break=0;
	v_LambdaTg_lookup=0;
}


void F_OVERTMP_ACTION(void)
{
	//20210112 chenwei add 发动机过温保护策略
	byte b;
	
	F_OverTMPProtection_Condiction();      //过温保护启用条件判定
	
	if(VF_OVERTEMANDHIGHLOAD_ENG&&VF_OVERTEMPRATRUEPROTECTION_ENG)
	{
		UP_OVERTMP_PROTECTION=FG_3DMap_xyz((byte *)BYPTR_M2_UP_OVERTEP_PROTECTION,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB1,(byte *)BYPTR_TPD_M2LDB1,5,4,false);	
		if(UP_OVERTMP_PROTECTION==255)
		{
			UP_OVERTMP_PROTECTION=0;
		}
		else if(UP_OVERTMP_PROTECTION>50)
		{
			UP_OVERTMP_PROTECTION=50;
		}
		b=(byte)v_LambdaTg_lookup+UP_OVERTMP_PROTECTION;		//20210710 chenwei add 基础up+过温up，即最终的目标
		
		if(b>126)
		{
			b=126;
			
		}

		if(v_LambdaTg_break<126)
		{
			
			if(v_LambdaTg_break<b)
			{
				if(VF_1S_TMCCL)
				{
					v_LambdaTg_break+=1;	//一次加5还是觉得太快
				}
			}
			else 
			{
				v_LambdaTg_break=b;
			}
		}

		else
		{
			v_LambdaTg_break=126;
		}
		
		v_LambdaTg=v_LambdaTg_break;
			
	}

	//过温条件突然不满足时
	else
	{	
		if(v_LambdaTg_break>v_LambdaTg_lookup)
		{
			
			if(VF_1S_TMCCL)
			{
				v_LambdaTg_break-=5;
			}
		}
		else
		{
			v_LambdaTg_break=v_LambdaTg_lookup;
		}
		v_LambdaTg=v_LambdaTg_break;
		
	}

	if(v_LambdaTg>126)v_LambdaTg=126;
	if(v_LambdaTg<v_LambdaTg_lookup)v_LambdaTg=v_LambdaTg_lookup;

	v_LambdaTg_break=v_LambdaTg;
	
}


/*******功能：发动机过温保护条件判断********/
void F_OverTMPProtection_Condiction(void)
{

	VF_OVERTEMPRATRUEPROTECTION_ENG=false;
	VF_OVERTEMANDHIGHLOAD_ENG=false;
	V_OverTMP_st_MskUnEna=0;

//1.启用功能条件
	if(BYPTR_M0_OverTMPProtection_SWITCH==0)       //启用发动机过热保护开关（ 0：关  1：代表开）  b1有个范围  
	{
		V_OverTMP_st_MskUnEna+=OverTMP_SWITCH;
	}
	
//2.运行工况条件
	if((v_TPS_odr<OverTMPProtection_TPD)||(v_rev_625rpm<OverTMPProtection_SPEED62D5))    //转速<6500rpm或TPD<12均不属于启用工况
	{
		V_OverTMP_st_MskUnEna+=OverTMP_SPEEDANDTPD;
	}          
	
//3.缸温条件
	if(v_te<(sword)BYPTR_M0_OverTMPProtection_Te)          //缸温低于某一值不满足启用条件  v_te是一个sword 改1
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TE;
	}       
	
//4.满足高负荷且缸温超过限值的时间要求
	if(v_t_overTMP_1s<BYPTR_M0_T_OVERTMP)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TIME;	
	}         

//5.无故障条件
	if(cur_code>0)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_DTC;	
	}          

//6.判缸条件
	if(!VF_YSTDCGET_INJPHASE)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_YSTDC;	
	}          

//7.电压条件
	if((v_Vbat_inuse<OverTMPProtection_VBAT_min)||(v_Vbat_inuse>OverTMPProtection_VBAT_max))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_VBAT;
	}   

//8.过渡工况条件
	if(VF_ACCSPD_TRANS||VF_DECSPD_TRANS||VF_TRANSGT15_TRANS||(!VF_TRANSLT10_TRANS))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TRANS;//过渡或加减油门
	}	 

//9.ldb\up标定模式条件
	if((REMARK!=MC_NORMAL)&&((REMARK!=MC_U)||(KESEI!=10)))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_REMARK;//只有正常模式和up模式满足启用条件
	}	

	if((v_te>=BYPTR_M0_OverTMPProtection_Te)     //缸温大于过温保护限值
		&&(v_TPS_odr>=OverTMPProtection_TPD)&&(v_rev_625rpm>=OverTMPProtection_SPEED62D5))  //且工况在设定工况范围以内
	{
		VF_OVERTEMANDHIGHLOAD_ENG=true;   //作为过温保护的时间累计v_t_overTMP_1s的条件
	}
		
	
	if((V_OverTMP_st_MskUnEna==0)&&(BYPTR_M0_OverTMPProtection_SWITCH==1))
	{
		VF_OVERTEMPRATRUEPROTECTION_ENG=true;
	}
	
	
}




