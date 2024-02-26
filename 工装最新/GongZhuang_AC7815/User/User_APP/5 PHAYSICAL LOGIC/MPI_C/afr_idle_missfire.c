#include "status_st.h"
#include "public.h"
#include "ign.h"//2010-1-13
#include "sci.h"

#include "add.h"

//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_IDLEMF_STATUS;
//#pragma DATA_SEG DEFAULT
//missf
byte v_dmidle;    //ʧ��������ϵ��	
byte vc_idlemiss_cur;//     ��ǰʧ�����͵�����							
byte vc_TW1_IDLEMISSF_av; //ʧ����ƽ��ֵ								

byte vc_TW1_IDLEMISSF_tmp;  //
byte vc_TW1_IDLEMISSF_sum;  //����OS��������ʧ�𣬲����ĶԵ��ٷ������������Ĳ��֣���

sbyte vc_cls2_idlemissf; //ʧ����Ե��������������������������//��ƽ��ӻ

//for idle miss fire control
//LOCAL for "q_missfire.c" ONLY
byte rpm156_last;//	equ	0x1f2				;
byte rpm156_last2;//	equ	0x1f3	
byte rpm156_last3;//	equ	0x1f4	
byte rpm156_last4;//	equ	0x1f5	
byte rpm156_limit;//	equ	0x1f6	
byte rpm156_last5;//	equ	0x1f7	
byte rpm156_last6;//	equ	0x1fb	
byte rpm_status;//	equ	0x104			;����ʧ�����ǣ�������Ȧ֮��RPM�仯��״̬��;2007-7-16,	rcord	the	rpm	dec	status	by	15.625		

//LOCAL FOR "Q_MISSFIRE.C"AND "Q_IDLE.C"
sword sum_ti_inc_drpm;//L;//	equ	0x1cc	;����ʧ�����ۼ�ֵL������											
//sum_ti_inc_drpmh;//	equ	0x1cd	;����ʧ�����ۼ�ֵH��		
//missf
#define MAX_MODI_FACH 20	//EQU	20						
//#define MIN_CLR_TI_DRPM	60//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
//2009-12-22
#define MIN_CLR_TI_DRPM	40//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
#define MIN_RPM_NO_TI_I	65//EQU	65	;2006-10-21	WHEN	THE	RPM<65*15.6=1014,	NO	TI	INC	FOR	DRPM	
#define OFFSET_FACH 10    //EQU	10		

byte wait_run_drpm_t;//	equ	0x1f8		;wait_run_drpm_t>0, will disable v_dmidle test		

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void miss_fire_doing(void);//ʧ����
void small_fire_doing(void);
void leans_inc_miss_f(void);
void miss_f_inc_ti(sbyte w);
void miss_fire_inc_l_do(sbyte w);

//���ܣ�STARTSǰ�����ٲ��Բ����ĳ�ʼ��
void FI_IdleMissfire_BeforeStarts(void)
{
    sum_ti_inc_drpm=0;    
    //Init_MissFire_AfterReset:://ԭ�����ڸ�λ�󣬳�ʼ���ģ��Ҹ�Ų������BEFORE STARTS,Ӧ�ÿ��Եģ�
    vc_TW1_IDLEMISSF_av=0;//								;
    v_dmidle=0;//								;
    vc_TW1_IDLEMISSF_sum=0;//							;
    rpm_status=0xff;//								;������ת�ٱ仯��ʶ����Ϊȫ��������
    //---2008-7-29-----------------------------
    V_IDLEMF_STATUS=false;
    //Init_MF_BeforeStartRun
    wait_run_drpm_t=6;//			;2007-7-17
}

//********************************************************************************************
//ע��rpm_ch_status��RPM_STATUS�����𣻶����������ڣ�RPM_STATUSֻ���ڵ��٣���rpm_ch_status��������ת�٣�
//���壺
//vc_TW1_IDLEMISSF_tmp:��ʱʧ�����ͣ���ȦT1����ǰ��⵽֮������ʹ�ã��ڵڶ�Ȧ�������㣻
//v_dmidle:ʧ�������������Ӻ�˥�����й��̣�
//						��O2WORKʱ����ʹ�ã�
//						����������
//										1 temp_rp1<=min_clr_ti_drpm;;//if rpm <min_rpm_no_ti_i, clrf ti_drpm and end miss_fire_test
//										2 START_INITION��
//										3 ��av_ti_drpm_cal��,when vc_TW1_IDLEMISSF_av>max_ti_inc_drpm, which not happened normally,clrf
//										4 ANNUATION_TI_INC_DRPM:˥����ֱ��0��
//						��ֵ��ʹ�ã�
//										1 �������е���SMISS_FIREʧ������ڵ���AV_TI_DRPM_CAL��
//										2 ��������ƫ����ʱ����Ҫ�Խ�TI_INC_DRPM����Ų��dti_rpm_scedl��ȥ��ADD_TEMPL_TO_PCTI
//										3 SMISS_FIRE�м�⵽ʧ�����Ҫ����
//										4 ANNUATION_TI_INC_DRPM����˥����
//										5 ʧ�����Ͷ�����������REC_TI_INC_DRPM
//vc_TW1_IDLEMISSF_av:
//										1 �������е���SMISS_FIREʧ������ڵ���AV_TI_DRPM_CAL��
//										2 START_INITION������
//										3 ��������ƫ����ʱ����Ҫ�Խ�TI_INC_DRPM����Ų��dti_rpm_scedl��ȥ��ADD_TEMPL_TO_PCTI
//										4 AV_TI_DRPM_CAL��,��128��ƽ��ֵ��������
//					�ǲ��ǿ���˵����λ�󣬸�ֵ�ڲ����������¶���Ϊ0;
//wait_run_drpm_t:��wait_run_drpm_t=0ʱ��set bdrpm_test��ʹ��ʧ���⣻
//													;dec wait_run_drpm_t by 1, when to 0, set bdrpm_test
//sum_ti_inc_drpmh,l:
//										ÿȦ�������Ų��ڵ���ʱ��sum_ti_inc_drpm-=6;ֱ��Ϊ0;
//										ÿ�δӵ�����ʱ��sum_ti_inc_drpm-=128��ֱ��Ϊ0��
//										START_INITION������
//										ADD_TEMPL_TO_PCTI�У�ĳ������£����룻
//										AV_TI_DRPM_CAL�У���ti_inc_drpm-av_ti_drpmֵ�ۼӵ����ϣ�
//2007-7-15���������ʧ����

//���ܣ�����ʧ�������Ͳ���
void F_Missf_Idle(word rpm156,byte tps_odr,byte c_IdleMissfSum_CLS2)
{
    byte temp_rp1;//	equ	0x1ff	;��ʱ����_�ֲ�����	
    byte rev_inc_miss;  //ʧ�������жϵ�����;��ʧ���������,2010-1-10
    byte bDrev;
    
    vc_idlemiss_cur=0;
    vc_cls2_idlemissf=0;//�������㣬���ұ�����ڳ������ʵ���λ�ã�
    vc_TW1_IDLEMISSF_tmp=0; //ÿȦ�����㣻

    //----ͨ�����ӱ���vc_IdleMissfSum_CLS2����CLS��TW1_IDLEMISSF_sum������Ų����-------
  	if(vc_TW1_IDLEMISSF_sum>=c_IdleMissfSum_CLS2)
  	{
  	    vc_TW1_IDLEMISSF_sum-=c_IdleMissfSum_CLS2;    
  	}
  	else vc_TW1_IDLEMISSF_sum=0;
						
        
    //ԭ����DO_INJ_WHEN_LEAVE_IDLE�У�����Ų����
    //VF_JLEAVEIDLE_TRANS��Ҫ��֮ǰ�����ù�
    if(VF_JLEAVEIDLETRANS_ENG)//DEC_TI_DPRM_WHEN_LEAVEIDLE:
    {   //�뿪����ʱ����ʧ�������������
	      if(sum_ti_inc_drpm>128)sum_ti_inc_drpm-=128;
        else sum_ti_inc_drpm=0;
    }
    //if(VF_JCLOSE_ISC)miss_f_inc_ti(12);//2008-10-12 add   //���ٷ��չر�ʱ������һ����ʧ������
    
    //��������smiss_fire��������������������������������������:
    //ԭsmiss_fire�����Դ˿�ʼ 
    if(rpm156>255)temp_rp1=255;else{temp_rp1=(byte)rpm156;}

    //2009-12-15 ������������׶�ת�ٹ��ͣ�Ҳ����ʧ�����ͣ�
    if(temp_rp1<=MIN_CLR_TI_DRPM&&!VF_STARTS_ENG)
    {
        v_dmidle=0; //2008-5-30 add back, because missfire too at tm125-4-1#
        //2008-5-30 ��ת�ٹ��ͣ����ʧ��������ͬʱ����� sum,����Ҫ
    		vc_cls2_idlemissf=vc_cls2_idlemissf-vc_TW1_IDLEMISSF_sum; //2008-5-30 add
    		vc_TW1_IDLEMISSF_sum=0;                  //2008-5-30 add
    }
    //2010-1-10 add 
    rev_inc_miss=v_dmidle/4;  //20101-1-10
    
    rpm156_last6=rpm156_last5;
    rpm156_last5=rpm156_last4;
    rpm156_last4=rpm156_last3;
    rpm156_last3=rpm156_last2;
    rpm156_last2=rpm156_last;
    rpm156_last=rpm156_limit;
    
    /*-----------------------------------------------------------*/
    //����������������Ŀ�ʼʧ����
    if((!(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG))&&VF_E0_TPS&&(temp_rp1<rev15d6_target+10)&&temp_rp1<122)   //2009-11-26 add condiction VF_ENAQ_SYSINI
    {               //2010-1-13 form 128 to 106//1650rpm  to rev_target_idle+10
                //2010-7-10���ڵ����Ը��ĺ�  rev15d6_target�ڻ�����ʱ��ܴ���͵����ڶ���ʱ��Ϊʧ����ˣ����� temp_rp1<122
        //---------Annuation_ti_inc_drpm://ʧ������˥������----------------�
        if(VF_4S_TMCCL){
            VF_WITHMS_IDLEMF=false;//2008-7-29 add
            if(v_dmidle>1)
            {
                v_dmidle--;
                VF_WITHMS_IDLEMF=true;//2008-7-29 add
                //2008-7-29 ADD MISSFRE FLAG
            }
        }
        //2009-12-15 ֻ��ʧ��������Чʱ���Ž���ʧ������˥����
        rpm156_limit=temp_rp1;
        VF_ENMF_IDLEMF=true;
        if(rpm156_limit>=(rpm156_last+1))rpm_status=rpm_status*2+1;//�ڻ���У����Ƿ��ǽ�λ��Ȼ��������ʵ��
        else{rpm_status=rpm_status*2;};//��rpm_status�ĵ�NλΪfalseʱ����ʾ��NȦ��ת��������N��1Ȧ���½��ģ�

        if(((rpm_status&0x03)==0) &&( rpm156_last2>=rpm156_limit))
        {
            //;�������Ȧת���������½����ҵ�Ȧ������Ȧ��;
        		 //2014-1-15
        		 bDrev=(rpm156_last2-rpm156_limit);
        		 #if M0_MFUD1==INJ209
            	  	bDrev=bDrev/2;
        		 #else
                  bDrev=bDrev/4;
        		 #endif
        		 
          		bDrev++;
        		  vc_TW1_IDLEMISSF_tmp=bDrev;
        		 
          		if(RPM_DEC_FOR_MISFIRE+rev_inc_miss>=rpm156_last2-rpm156_limit){
          		    //û����,����Ϊʧ��,����̫ϡ�ж�
          			  //һ��ʧ��
            			if(RPM_DEC_FOR_LEANS+rev_inc_miss<(rpm156_last2-rpm156_limit)){
            			    //2Ȧ�½�С��8*15.6ʱ������
              				if(VF_O2WORK_OSDIAG&&VF_RICH_OSDIAG&&VF_O2DEC_OS){
              					leans_inc_miss_f();//		;O2�½�,+4US
              				} 
              				else //2009-12-14 add else    //2009-12-14 ԭ���������������ʧ�����ͣ�
              				{
              					leans_inc_miss_f();//		;O2�½�,+4US    				    
              				}
            			}
                      //��ȫ�½�����ʱ���ٿ�����һȦ
                      //two_cycle_miss_t
              		//��ʱ��������Ȧ�½�,�Ƿ�����4Ȧ�½�?	
              		else if(((rpm_status&0x0f)==0) &&(rpm156_limit<=rpm156_last4)){;//��������Ȧ�½�   //2009-12-14 add else to here
                			if(RPM_DEC_2C_MISSFIRE+rev_inc_miss<(rpm156_last4-rpm156_limit)){;//����small_fire_doing��
                					small_fire_doing();//
                			}
                			else{;//three_cycle_miss_t:;û������Ȧ������ֵ,����ѭ��(6Ȧ)�Ż𲻳�ּ��
                				if(((rpm_status&0x3f)==0)&&(rpm156_last6>=rpm156_limit)){
                					if(RPM_DEC_3C_MISSFIRE+rev_inc_miss<(rpm156_last6-rpm156_limit)){
                						small_fire_doing()		;//��������ѭ��(6Ȧ)�½�,ʧ����	}
                					}
                				}
                				//else ��������£���ʾ��ʧ��
                			}
              		}//else ��������£���ʾ��ʧ��
          		}
          		else{		;//����,��ʧ����:�����Ȧת�ٲ��ǹ���,����Ϊʧ��,������ϡ�ж�,2007-7-17 ������ϡ���	
          		    //ʧ������
          			miss_fire_doing();//
          		}
        } 
    }
    else
    {

        VF_ENMF_IDLEMF	=false;    
      	rpm_status=0xff;
      	rpm156_limit=0;
      	//�뿪���ٺ�ʧ������˥������
      	if(tps_odr) //�����Ų��ڵ���ʱ
      	{
              if(sum_ti_inc_drpm>6)sum_ti_inc_drpm-=6;
              else sum_ti_inc_drpm=0;
      	}
    }


    //�о��������ݶ����Է��뵽����Ĵ�����֮��
    sum_ti_inc_drpm+=(sbyte)v_dmidle-vc_TW1_IDLEMISSF_av;
    if(sum_ti_inc_drpm<0)sum_ti_inc_drpm=0;                 //2010-1-10 
    vc_TW1_IDLEMISSF_av=(byte)((sum_ti_inc_drpm)/128);

    //----------av_ti_drpm_cal-----------------------------------------
    //ԭav_ti_drpm_cal�����Դ˿�ʼ
    //����128Ȧv_dmidle��ʧ������ƽ��
    if(!VF_ENMF_IDLEMF){
		    v_dmidle=vc_TW1_IDLEMISSF_av;
    }

    if(vc_TW1_IDLEMISSF_av>MAX_TI_INC_DRPM||VF_ENAQ_SYSINI){;//ʧ������ƽ��ֵ������Ϊ�����⣻
        //2010-5-3 add condiction VF_ENAQ_SYSINI
    		sum_ti_inc_drpm=0;//ԭ����ʦ�ǣ�sum_ti_inc_drpmh=0
    		vc_TW1_IDLEMISSF_av=0;
    		v_dmidle=0;
    }

    if(vc_idlemiss_cur> MAX_TI_INC_DRPM)vc_idlemiss_cur=MAX_TI_INC_DRPM;
    v_dmidle+=vc_idlemiss_cur;
    

	  if(v_dmidle> MAX_TI_INC_DRPM)v_dmidle= MAX_TI_INC_DRPM ;

    
    return;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//�������йز�ͬ����ʧ����miss_fire_doing(��ʧ��),small_fire_doing��Сʧ�𣩺�leans_inc_miss_f��ϡ����
//ʧ�����ͬʱ��Ҫ����O2�Ƿ�����
//the subroutine related to  smiss_fire;
void miss_fire_doing(void)//ʧ����
{
    if(MIN_RPM_NO_TI_I<rpm156_limit)
    {
        //��Ȧת�ٹ��Ͳ���Ϊʧ��,when rpm too low no ti_inc inc
        if(VF_O2WORK_OSDIAG){;//miss_fire_o2work
    	    if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)	miss_fire_inc_l_do(STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_fire_inc_l_do;
        }
        else{
    		miss_f_inc_ti( STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_f_inc_ti;
        }
    }
    return;
}


void small_fire_doing(void)
{
    //Сʧ����
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        //��Ȧת�ٹ��Ͳ���Ϊʧ��,w
        if(VF_O2WORK_OSDIAG){;//small_fire_o2work;
    		if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_fire_inc_l_do;
    		//2010-12-6���������λ�ã�
        }
        else{
    		miss_f_inc_ti(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_f_inc_ti;
    		//2010-12-6���������λ�ã�
        }
    }
    return;
}

void leans_inc_miss_f(void)
{
    //ϡ;//�����������if(CONSTSP,O2WORK&motor_status,rich&pdata_status,bo2dec)
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        if(VF_O2WORK_OSDIAG){;//leans_fire_o2work:
        	if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(1);//	w=1;call miss_fire_inc_l_do;
        }
        else{
        	miss_f_inc_ti(1);//	w=1;call miss_f_inc_ti;
        } 
    }
    return;
}

//�������ӹ��̵��ӹ��̣�����ʧ���������Ĵ���
void miss_f_inc_ti(sbyte w)
{
    vc_TW1_IDLEMISSF_tmp+=w;
    vc_idlemiss_cur=w;   //replace by vc_idlemiss_cur

    return;
}

void miss_fire_inc_l_do(sbyte w)
{
    vc_cls2_idlemissf=0;
    if(vc_TW1_IDLEMISSF_sum<MAX_MISS_FIRE_DTI){;//�ۼ�ʧ����������,ֻ�Ƕ���ʱʧ����������,������
    	//	if(VF_VBATUPCONTINUE_VBAT)w=w+1;//2008-10-23 +2 //2008-10-24 changeto +1
    		vc_TW1_IDLEMISSF_tmp+=w;
    		vc_cls2_idlemissf=w; //ADD_W_TO_leansdt_WHEN_MISSFIRE
    		vc_TW1_IDLEMISSF_sum+=w;
    }
    vc_TW1_IDLEMISSF_tmp+=w;
    return;
}
