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



byte CCPS_Pct_DutyCycleMax_C;    //�������̼�޹��������̼�޵�ŷ�ռ�ձ�
byte CCPS_Pct_DutyCycleMin_C;    //�������̼�޹�������С̼�޵�ŷ�ռ�ձ�
byte CCPS_Pct_DutyCycleIdleMax_C;    //�������̼�޹����ĵ���ʱ��̼�޵�ŷ�ռ�ձ�

extern  byte v_cf_1s;         //����ʱ��
extern	sbyte v_UP_M128;     //ȫ���ɵ�U
extern byte v_t_WOT_1s;      //����ȫ����ʱ��
extern word t_after1stStart_s;    //��Կ�׺󣬵�һ��������ʼ��ʱ�������м�ͣ��
extern unsigned long int v_t_eng_t_s;  //����һֵά�ָ�״̬��ʱ�䣬���㿪ʼ�ۻ�


#define CCPS_open 1          //CCPS��������
#define CCPS_ENGSTATUS 2     //����������״̬Ҫ��
#define CCPS_RUNTIME 4       //����ʱ������
#define CCPS_TMP  8          //�������¶�����
#define CCPS_TPS  16          //���ſ�������
#define CCPS_SPEED  32       //ת��Ҫ��
#define CCPS_VBAT   64       //��ѹҪ��
#define CCPS_UP     128       //up<80����

word V_CCPS_st_MskEna;          //̼�޵�ŷ�����״̬��
#define t_CCPS_period 64          //̼�޵�ŷ�һ�����ڵ�ʱ�䵥λms����Ϊռ�ձȵķ�ĸ,64msΪ����¶���
#define CCPS_swt_open 1         //��ʾ����CCPS


word v_t_CCPS_closelast;           //CCPS�ϴιرյ�ʱ��
word v_t_CCPS_opennow;               //CCPS���ο�����ʱ��

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


/*******���ܣ�̼�޵�ŷ���CCPS�߿�����ʹ������********/
void F_CCPSOpen_Condition(void)
{

	VF_CCPS_status=false;  
		V_CCPS_st_MskEna=0;

//1.��CCPS����
	if(BYPTR_M0_CCPS_swt_CCPSStrt_C==CCPS_swt_open)
	{
		V_CCPS_st_MskEna+=CCPS_open;
	}
		
//2.����������״̬Ҫ��(����������һ����)
	if(VF_CLSMODE_IACV                                         //����ջ�
		||(VF_DECSPD_TRANS&&v_cf_1s<BYPTR_M0_CCPS_ti_DurDFCORunTmLim_C)            //���ٶ��ͽ׶μ��ٶ���ʱ��С��ĳһֵ
		||((VF_ACCSPD_TRANS||VF_TRANSGT15_TRANS||(!VF_TRANSLT10_TRANS))&&v_t_eng_t_s<BYPTR_M0_CCPS_ti_DurPERunTmLim_C)   //���ٽ׶���ά�ָ�״̬ʱ��С��
		)  
	{
		V_CCPS_st_MskEna+=CCPS_ENGSTATUS;
	}
	
//3.����������ʱ��Ҫ������������һ������
	if(VF_MMOVE_ENG
		&&((V_Tw<80&&t_after1stStart_s>=BYPTR_M0_CCPS_ti_CldStrTmtMin_C)
			||(V_Tw>80&&t_after1stStart_s>=BYPTR_M0_CCPS_ti_WrmStrtTmMin_C))
		)                       //�����ů�������׶Σ�����������ʱ�����Ҫ����һ��ʱ��
	{
		V_CCPS_st_MskEna+=CCPS_RUNTIME;	
	}  
	
//4.����������Ҫ��
	if((v_te>BYPTR_M0_CCPS_t_EngCydTmpLo_C)&&(v_te<BYPTR_M0_CCPS_t_EngCydTmpHi_C))
	{
		V_CCPS_st_MskEna+=CCPS_TMP;
	}
	
//5.���ſ�������
	if((v_TPS_odr>BYPTR_M0_CCPS_Pct_TPSMin_C)&&(v_TPS_odr<BYPTR_M0_CCPS_Pct_TPSMax_C))
	{
		V_CCPS_st_MskEna+=CCPS_TPS;
	}

//6.ת��Ҫ��
	if((v_rev_625rpm<BYPTR_M0_CCPS_n_EngSpdHi_C)&&(v_rev_625rpm>BYPTR_M0_CCPS_n_EngSpdLo_C))
	{
		V_CCPS_st_MskEna+=CCPS_SPEED;
	}
	
//7.��ѹҪ��
	if(v_Vbat_inuse<BYPTR_M0_CCPS_u_BattVltgMax_C)
	{
		V_CCPS_st_MskEna+=CCPS_VBAT;
	}
	
//8.���������ź�Ҫ�� ���ݵļ�Ũ�׶�
//8.UP<80 ����
	if(v_LambdaTg<80)       //up<80 ����DELPHI ��PEģʽ
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
	
	if(V_CCPS_st_MskEna==255)  //����̼�޵�ŷ����������������й���
	{
		b=(byte)F_2DLookUp(v_rev_625rpm,BYPTR_M1_CCPS_Pct_DutyCycle,7,0,false);
		b1=t_CCPS_period*(100-b)/100;                       //CCPS����ʱ��
		
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
			VF_CCPS_status=false;    //20201209 chenwe add ������ʾ����
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
		VF_CCPS_status=false;    //20201209 chenwe add ������ʾ����
		v_t_CCPS_closelast=T_MAINTMR;   //20201128 chenwei add�����ԣ�Ŀ���ǽ��������϶���㲻��������������ok
	}
	
}


