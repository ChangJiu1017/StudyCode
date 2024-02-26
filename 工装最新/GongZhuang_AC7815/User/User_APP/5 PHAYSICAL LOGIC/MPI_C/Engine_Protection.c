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
 *		1.���ӹ��±�����UP�����ķ��亯��F_UP_OverTMP();
 *		2.���ӶԷ��������ٹ����жϵĺ���F_status_ENG();
 *
 * V1.0.1	2021.2.7	chenwei 
 *		1.���ӵ�����������ʱ���±�����UP�����𲽵ݼ������ֵ;
 *
 * V1.0.1	2021.7.13	chenwei
 *		1.�����Ż����º�up�����ĵ����ݼ���ʽ
 *		2.ȡ��ԭ�ȵ��жϵ��ٵķ���
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
void F_status_ENG(void);   //20210125 chenwei add ���������ٹ����ж�
void F_OVERTMP_ACTION(void);  //20210206 chenwei add




//���±�����״̬��ض���
#define OverTMP_SWITCH   1
#define OverTMP_SPEEDANDTPD     2
#define OverTMP_TE      4
#define OverTMP_TIME    8
#define OverTMP_DTC     16
#define OverTMP_YSTDC   32
#define OverTMP_VBAT    64
#define OverTMP_TRANS    128
#define OverTMP_REMARK   256

//���±����ж�����ȡֵ�Ķ���
#define OverTMPProtection_SPEED62D5 104    //6500rpm
#define OverTMPProtection_TPD 12 //TPD 12��
#define OverTMPProtection_VBAT_min  161
#define OverTMPProtection_VBAT_max  241


word V_OverTMP_st_MskUnEna;   //�ж������㷢�������±�����ԭ��״̬��
    
byte UP_OVERTMP_PROTECTION;  //20210121 chenwei add ���±������õ�up����ֵ,UP��������



/*******���ܣ�����������������صĳ�ʼ��***********/
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
	//20210112 chenwei add ���������±�������
	byte b;
	
	F_OverTMPProtection_Condiction();      //���±������������ж�
	
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
		b=(byte)v_LambdaTg_lookup+UP_OVERTMP_PROTECTION;		//20210710 chenwei add ����up+����up�������յ�Ŀ��
		
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
					v_LambdaTg_break+=1;	//һ�μ�5���Ǿ���̫��
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

	//��������ͻȻ������ʱ
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


/*******���ܣ����������±��������ж�********/
void F_OverTMPProtection_Condiction(void)
{

	VF_OVERTEMPRATRUEPROTECTION_ENG=false;
	VF_OVERTEMANDHIGHLOAD_ENG=false;
	V_OverTMP_st_MskUnEna=0;

//1.���ù�������
	if(BYPTR_M0_OverTMPProtection_SWITCH==0)       //���÷��������ȱ������أ� 0����  1��������  b1�и���Χ  
	{
		V_OverTMP_st_MskUnEna+=OverTMP_SWITCH;
	}
	
//2.���й�������
	if((v_TPS_odr<OverTMPProtection_TPD)||(v_rev_625rpm<OverTMPProtection_SPEED62D5))    //ת��<6500rpm��TPD<12�����������ù���
	{
		V_OverTMP_st_MskUnEna+=OverTMP_SPEEDANDTPD;
	}          
	
//3.��������
	if(v_te<(sword)BYPTR_M0_OverTMPProtection_Te)          //���µ���ĳһֵ��������������  v_te��һ��sword ��1
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TE;
	}       
	
//4.����߸����Ҹ��³�����ֵ��ʱ��Ҫ��
	if(v_t_overTMP_1s<BYPTR_M0_T_OVERTMP)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TIME;	
	}         

//5.�޹�������
	if(cur_code>0)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_DTC;	
	}          

//6.�и�����
	if(!VF_YSTDCGET_INJPHASE)                       
	{
		V_OverTMP_st_MskUnEna+=OverTMP_YSTDC;	
	}          

//7.��ѹ����
	if((v_Vbat_inuse<OverTMPProtection_VBAT_min)||(v_Vbat_inuse>OverTMPProtection_VBAT_max))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_VBAT;
	}   

//8.���ɹ�������
	if(VF_ACCSPD_TRANS||VF_DECSPD_TRANS||VF_TRANSGT15_TRANS||(!VF_TRANSLT10_TRANS))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_TRANS;//���ɻ�Ӽ�����
	}	 

//9.ldb\up�궨ģʽ����
	if((REMARK!=MC_NORMAL)&&((REMARK!=MC_U)||(KESEI!=10)))
	{
		V_OverTMP_st_MskUnEna+=OverTMP_REMARK;//ֻ������ģʽ��upģʽ������������
	}	

	if((v_te>=BYPTR_M0_OverTMPProtection_Te)     //���´��ڹ��±�����ֵ
		&&(v_TPS_odr>=OverTMPProtection_TPD)&&(v_rev_625rpm>=OverTMPProtection_SPEED62D5))  //�ҹ������趨������Χ����
	{
		VF_OVERTEMANDHIGHLOAD_ENG=true;   //��Ϊ���±�����ʱ���ۼ�v_t_overTMP_1s������
	}
		
	
	if((V_OverTMP_st_MskUnEna==0)&&(BYPTR_M0_OverTMPProtection_SWITCH==1))
	{
		VF_OVERTEMPRATRUEPROTECTION_ENG=true;
	}
	
	
}




