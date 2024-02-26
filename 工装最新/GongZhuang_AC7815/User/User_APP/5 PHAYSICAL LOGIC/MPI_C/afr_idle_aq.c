/*HEAD			
 ***********************************************************************			
 * Filename	:ign.c		
 * Function	:ignition calculation.		
 * User		:xielian	
 * Date		:2017.1.20	
 * Version	:V8.1.1		
 *			
 *** History ***			
 * V8.1.0	2017.1.20	xielian 	
 *		Base. 
 * V8.2.0   2018.9.26	xielian
 *		MV_EXHAUST���Ԥ�����ֶθ���Ϊif
 * V8.3.0   2019.1.15	xielian
 *		����UP�Ե��ٳ�ʼ������������Сֵ����
 * V8.3.1   2019.1.24	xielian
 *		UP�Ե��ٳ�ʼ������������Сֵ��������
 * V8.3.2   2019.2.3	xielian
 *		�������Խ������ڿ���ģʽ���ʼ��ʱ������
 * V8.3.3   2019.5.28	xielian
 *		�������Խ������ڿ���ģʽ���ʼ��ʱ������
 ***********************************************************************			
HEAD END*/

#include "public.h"
#include "status_st.h"
#include "sys_init.h"  //2009-12-12 ad 
#include "ign.h"//2010-1-26
#include "ld.h"
#include "ad.h"
//2010-3-13 add 
#include "data_sys_standard.h"

//2010-7-14add
#include "time_ccl.h"

#include "sci.h"    

#include "add.h"
#include "events.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_IDLEAQ_STATUS;
//#pragma DATA_SEG DEFAULT
//sbyte vc_tv3_idleAQ;     //����ƫ�������������TV3_FK��������

sword vc_tw1_IDLESC_inuse;//��������ʱ�����������Ʋ�������ʱ������
sword vc_tw1_IDLESC;      //����������õ���ƫ����ֵ��


sbyte vc_MissfireNotidle_IdleSC;//��������ֵ�Է���ָ����������//2008-11-5

//for idle search
word rpm156_aq;//	equ	0x1d1	;when	rpm156_aq=rpm156	but	limited	to	the	max	of	127			
byte number_need_sum;//	equ	0x1d2												
byte number_sced;//	equ	0x1d3												
byte run_need_for_aq;//	equ	0x1d4												
byte s_need_for_aq;//	equ	0x1d5																						
word sum_rpm156_64r;//	equ	0x1d9//2008-3-9 change from sbyte to byte									
                    //byte sum_rpm156_64rh;//	equ	0x1da
                    //2008-3-6 change to word									
                    //2008-3-6 change from sbyte to byte									
sword drpm_64_32;//	equ	0x1dc									
sword drpm_32_0;//	equ	0x1dd	

byte nb_aq; //����������֮����Oֵ�Ĵ�����	

//byte for_blnaq ;
byte for_temp;

byte rpm_testMode;//ɨ��ģʽ 0����ʼ�� 1����Ũ 2����ϡ 3����Ũ 4��ϸ������
byte Mode1_Time;  //�½�����
byte Mode4_time;     //�Զ���������

//2009-11-25
/*
sbyte n_aq[3];//0:���ϴΣ�1���ϴΣ�2������
byte n_aq_ave[2];//0:shangci,1:benci ;
byte n_aq_ave_max[2];//1:max, 0:2nd max;
sbyte n_aq_min;
sbyte n_aq_max;
*/
//2009-12-24 chang to 
sword n_aq[3];//0:���ϴΣ�1���ϴΣ�2������
sword ign_sum_aq;//2010-1-26
sbyte ign_aq[3];//2010-1-26

word n_aq_ave[2];//0:shangci,1:benci ;
word n_aq_ave_max[2];//1:max, 0:2nd max;
sword n_aq_min;
sword n_aq_max;

sbyte o_aq_ave_max[2];//1:max n, 0:2nd max n;
sbyte o_aq_for_maxn[3];//���δ�ɨ�Ľ���� 0:���ϴΣ�1���ϴΣ�2������

byte nb_n_GT_llimit;        //���ת�ٳ���M0_REV15D6_LLIMIT_FOR_FULLAQ�Ĵ�����

sword ign_aq_sum;      //2010-1-11 
sbyte ign_aq_ave[3];//0:shangci,1:benci ;
//2009-11-18 add 
byte t_s_matchENAQ;//�ڳ�ʼ��״̬�£����㵡�ٳ�ʼ������ʱ�������ʱ��
byte t_s_matchAirTest;  //�ڴֵ���,���Ե���������ʱ��;�������ʱ��

byte nb_LeanWhenAq;     //2010-3-16
byte nb_OSchange_beforeELean;//nb_OSchange_beforeELean=v_nb_OS_flu
sbyte v_dO_enlean;  //2010-3-24 add    ��ϡ����ʱ��С��oֵ
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define PREOIL_ISV_SEL 4    //		EQU	4		;2006-5-26 NO ISV PRE OIL=4 TIME

#define AQ_PERIOD	60//EQU	120		//2008-8-24 	//��λ��s												
//#define AQ_PERIOD_FIRST	8//EQU	8	//2008-10-12 ����������Oֵ������ΪSC�����ڱ䣬��ˣ���8����Ϊ30		
//#define AQ_PERIOD_FIRST	5//EQU	8	//2008-10-12 ����������Oֵ������ΪSC�����ڱ䣬��ˣ���8����Ϊ30		
//#define AQ_PERIOD_FIRST	9//EQU	8	//2008-10-12 ����������Oֵ������ΪSC�����ڱ䣬��ˣ���8����Ϊ30		
#define AQ_PERIOD_FIRST	8//EQU	8	//2008-10-12 ����������Oֵ������ΪSC�����ڱ䣬��ˣ���8����Ϊ30		
                    //2010-1-27 chang from 9 to 5,after starts
                    //2010-1-29 from 5 to 8
#define MIN_AV_N_DEC_AQ	78//EQU	78	
#define WAIT_RUN_AQ	    20//EQU	20			

#define STEPM_N_SC	5//EQU	5
#define STEPM_N_SC_2	3//EQU	3
#define WAIT_STEP_N	2//EQU	2
#define AQ_TEMP_NUMBER  30

#define STEP_OF_IDLEAQ  16//�ֵ��Ĳ���
#define MIN_NB_FORAQ_ENOUGH 6
//2008-8-10 add
#define MAX_IDLESC  10
#define MIN_IDLESC  -10
//2008-11-4 DEFINE FOR LOGIC CHART
//2009-11-18 add
#define AQMODE_INI           0
#define AQMODE_ENRICH        1
#define AQMODE_ENLEAN        2
#define AQMODE_ENRICH_AGAIN  3
#define AQMODE_TESTIDLEAIR   4 //2009-11-20 VF_IDLEAIRTEST_SYSINI
#define AQMODE_FINE          5
#define AQMODE_WAIT_AIR      6//�ȴ���������

#define O_DELTA_FOR_NO_3RDFULLAQ    12       //2009-11-30ǰ����ɨ������ֵ���ڸ�ֵ����Ҫɨ�����Σ�
//#define O_DELTA_LIMIT_FOR_LEANAQ    -40      //
#define O_DELTA_LIMIT_FOR_LEANAQ    -60      // 2010-3-16 
#define O_DELTA_LIMIT_FOR_RICHAQ    40       //

//ɨ��ʱ��ÿһ��Ĵ���2009-12-1
#define NB_REV_SET_FORAQ    38//60     //38-24
#define NB_REV_AVE_FORAQ    32//48     //32-20

#define NB_REV_SET_FORAQ_FULL    38     //38-24 2010 30-38 
#define NB_REV_AVE_FORAQ_FULL    32     //32-20

#define NB_LEAN_LIMIT_FORLEAN    4

#define T_WAIT_TO_CHECKAIR_AFTER_FULLAQ    7     //1200RPM, 128RUNS =128/(1200/60)=128/20=7S

//#if MV_EXHAUST>PL150 || MV_EXHAUST<=PL110  //2013-8-1 ADD
    #define STEP_FOR_FULLENLEANAQ   3       //2010-1-16
    #define STEP_FOR_FULLENRICHAQ   6
    
    //---unit:4us,// ������Ϊ��������ʱ��TW1��������
    const byte M1_DTW1_AQ[6]= //dti_srpm_form      //2014-1-9 ��Сfor 50CC
    {
        0,0,0,3,6,6
    };
/*
#else 
    #define STEP_FOR_FULLENLEANAQ   4       //2010-1-16
    #define STEP_FOR_FULLENRICHAQ   8


    //---unit:4us,// ������Ϊ��������ʱ��TW1��������
    const byte M1_DTW1_AQ[6]= //dti_srpm_form      //2014-1-9 MOVE IN
    {
        0,0,0,8,14,14
    };

#endif
*///2016-6-9 ��ʱ����С����

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

void Save_AQ_Result(sbyte templ);//add_templ_to_pcti:
void GET_AQ_ENABLE(byte bln_aq);

void GET_AQ_rpmMax_OMax(word for_rev156,sbyte ignca);
//void GET_FALLtoAQ(void);

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void FI_IdleAQ_AfterReset(void)
{
    //��λ���Զ�����ƫ�����Ĳ����ĳ�ʼ����
    //Init_SAQ_after_start://ԭ�����ڸ�λ�󣬳�ʼ���ģ��Ҹ�Ų������BEFORE STARTS,Ӧ�ÿ��Եģ�
    vc_tw1_IDLESC_inuse=0;		

    if(!VF_AQOK_SYSINI||VF_VPOR_HARD) //2010-3-13 add condiction 
    {            //2009-12-26 �������� VF_TURNONKEY_HARD������������ģ���������ʱisc������һֱ���ۣ�
        vc_tw1_IDLESC=0;	    
    }
    nb_aq=0;        //2009-4-21
    V_IDLEAQ_STATUS=false;//V_STATUS2_IDLE=0;//2008-8-10 add
    VF_NOTAQING_IDLEAQ=true;
    VF_1STAQ_IDLEAQ=true;
    
    rpm_testMode=AQMODE_INI;

}

//���ܣ�STARTSǰ�����ٲ��Բ����ĳ�ʼ��
void FI_IdleAQ_BeforeStarts(void)
{
    s_need_for_aq=AQ_PERIOD_FIRST;
}

//********************************************************************************************
//���ܣ��ǵ���ƫ�����������̣������ǡ�RPM--�������Ĺ�ϵ
//  ʹ��'����ƫ������������'�������ǣ�
//				������������;�� run_need_for_aq=1;��s_need_for_aq=1;
//  ��������ص������¼���
//  1 ������ʼ��ͨ����wait_run_drpm_t=4����ֹͣʧ���⣻
//  2 ���������У�����dcdy_down_too_m������ѹ�½�����ʱ��������������
//  ���������������Ӱ�죺������add_templ_to_pcti��
//  ��Ҫ��ʧ���������룬����
//  1 ���жϳ���Ҫ����ʱ��ͬʱ����T3����ֵ��dti_rpm_scedh��dti_rpm_scedl�ͷ���ֵ��
//  2 ���ж���Ҫ����ʱ������Ҫ�������������ʧ��������Ӳ�����0�����ͣ���ȥ����֮�󣩣�
//  ���ڣ�run_need_for_aq(����LOCAL)����ֵ����run_need_init������Ϊwait_run_aq=20��ÿ�ε���av_rpm_64r����a_search_compʱ�ݼ���
//														��ÿ���뿪���٣������ϴ��뿪�������һ��ʱ���RUN_NEED_FOR_AQ����һ��
//  ���ڣ�s_need_for_aq(����LOCAL)��
//										ÿ1s�ݼ�һ�Σ�
//										��ֵ����λ���ʼ��ʱΪAQ_PERIOD_FIRST��
//													��next_sc�У���number_sced==2ʱ������ʼ��Ϊaq_period��
//  ���ڣ�number_sced(��LOCAL):
//  ���ڣ�number_need_sum(��LOCAL):ÿ��̽��ʱ��Ȧ��38,����ǰ6Ȧ��Ч��
//  ���ڣ�brpm_l_for_aq(��LOCAL)�����������У�ƽ��ת��AV_RPM����min_av_n_dec_aqʱ��λ�����������
//  ���ڣ�dti_rpm_sc��Oֵ�Զ����������е���ʱ��������ǰ��ֱ�Ӽ���PCTINJ�ϵģ�
//  ���ڣ�b_1st_search��һ������ֵ�ӿ�ʼ��������number_scedֵ�������ڴ˹����У�������������������п��ܻ�����һ�飬
//  b_1st_search������ȷ����ǰ�ǲ����������ģ�
//  ���ڵ���QIDLE_SH_INIT:��ʾֹͣ������
//���µ�ѭ����ʼ������Ƿ��뿪���٣�������ת��֮��
//����������״ֵ̬�Ķ�Ӧ���������������������������������
//    VF_NOTAQING_IDLEAQ;		0	  1   1   1   1   1   0				    
//    number_sced;			  0   5   4   3   2   1 	0			    
//    vc_tw1_IDLESC_inuse;0	  0   0   0   8   14  0					    
                      // rev156  ��ǰת��  teng  �¶�  fmf_starts �������� rev156ave ƽ��ת�� ��c_idles_cls2 ������������ǰ������tw1_idlesc_ISC ��������
void F_SearchIdleSpeed(word rev156,byte teng,byte fmf_starts,byte rev_156ave,
                    byte c_IdleSc_CLS2,sbyte ignca)
{   //09-9-5  �޸�����
    //����˵����
      /*
         1��Oֵ����
         2���ж��Ƿ��뿪���٣� s_need_for_aq=1 ��λ
         3���жϵ����������� (!VF_O2CHANGED_OSDIAG||VF_OSCHANGELITTLE_OSDIAG)&&(!VF_WORKBACKIDLE_ISC)&&(!VF_CICORECING_CFCI)  
         4������������
            a����¼ת��
            b���ж� VF_NOTAQING_IDLEAQ  �Ƿ�ʼ����      
               O2=0   run_need_for_aq=1   s_need_for_aq=1 �������㣬����������ʼ��
            c��ÿ������38Ȧ��
               ����ƽ��ת��
               �ж�number_need_sum ����ڼ���
         5�������������ܣ�
            a����һ�ڶ�������vc_tw1_IDLESC_inuse��ʱ����Ϊ0
            b������������ʱ����Ϊ0��Ϊ��һ����¼�㣬��ȷ����һ��������ʱ��AQ_PERIOD�����ж��Ƿ����ת��
            c�����Ĳ�vc_tw1_IDLESC_inuse=8��Ϊ�ڶ�����¼��
               �ж�1��2���Ƿ��½����ǵ���number_need_sum=3���ӵ��������Ĳ����²���һ��
               ���ζ����½����ƣ�����м��ͣ��������������
            d�����岽vc_tw1_IDLESC_inuse=14��Ϊ��������¼��
               ����������֮���ת�ٱ仯���ƵĲ�ͬ���õ���ͬ��������
               ��������������������������              
      */
    byte templ;
    sbyte sw;
    sbyte stemp;

 //   if(VF_E0_TPS)         //2009-12-28 ADD CONDICTION
    //2010-1-10 ��������ʦҪ��ǵ���ʱ,Ҳת��oֵ,���,ȡ���������;
    //if(c_IdleSc_CLS2!=0)//2010-1-10 ����Ϊ�������;  //������;
    {
        stemp=c_IdleSc_CLS2;
        stemp=-stemp;
        if(vc_tw1_IDLESC>=c_IdleSc_CLS2)
        {
          vc_tw1_IDLESC=vc_tw1_IDLESC-c_IdleSc_CLS2; 
        }   
        else if(vc_tw1_IDLESC<=stemp)
        {
          vc_tw1_IDLESC+=c_IdleSc_CLS2;	   //��0�ƽ���
        }
        //-----������ISC���ƶ��Զ��������������
        //vc_tw1_IDLESC=vc_tw1_IDLESC+tw1_IDLESC_ISC;    //2012-11-9 DELETE
    }
    vc_MissfireNotidle_IdleSC=0;//2008-11-5 	    
    //��s_need_for_aq	����Ϊ1��֤���ص����ٺ����������SSET_IDLESEARCH_PARA_WHEN_LEAVEIDLE:
    //2008-2-24 zhangping move to here?
    if(VF_JLEAVEIDLE_ENG)
    {
        s_need_for_aq=1;//;2007-2-14 a start let s_need_for_aq=1 which will start aq imiidiatly when back idle	
        run_need_for_aq++;  //������
    }
  
    //09-9-5  �޸ĵ�������
    //2009-11-18 add flag VF_AQAFTER_PNINI_SYSINI  , with VF_ENAQ_SYSINI
    //2009-11-18 ȷ�����������ʼ��֮�󣬵��ٳ�ʼ����������VF_ENAQ_SYSINI��������
    //  VF_AQAFTER_PNINI_SYSINI���ڶ��������ʼ���ɹ�����λ�ģ�
    //  �ڹر�Կ�׻��ٳ�ʼ��֮����λ��
    //  VF_ENAQ_SYSINI����VF_AQAFTER_PNINI_SYSINI=true���ҵ�����������֮����λ��������λ��
    //      ��ֻҪ���ٳ�ʼ��δ�ɹ���������һ���㣬�ֻ����״̬��
    //������Ҫע��һ�㣺
    //       ����������ʼ��֮���ָʾ��״̬��
    //          ��������ֹͣ����ָʾ�ƿ�����
    //          �����������������������������ٳ�ʼ��δ��������VF_AQAFTER_PNINI_SYSINI=true������
    //                      ��������ƫ�ͣ�ָʾ������
    //                      ����ת��ƫ�ߣ������ݶ���ָʾ
    //                                      ���Ƿ��б�Ҫ����һ������������С�ٵ���Ĺ���
    //                                      ���߹۲�ת�ٱ�  
    //                      ����������������ʼ������������
    //VF_ENAQ_SYSINI��λ��������
    //  ��������������
    //      ������ת������ߺ����֮�䣻
    //      ���ŵ��٣�
    //      ����������ƫ�ͣ�
    //      ��Ӳ�����ϣ�
    //      VF_AQAFTER_PNINI_SYSINI=true;
    //      ������������nȦ֮����λ ��
    //      VF_WARMUP_ENG:��ͷ�¶Ȳ���ֵ����30��������������ʧ
    //   
    /*
    //2009-11-20 ��δ���������,�������ת�ٹ�����Ҫ������ʾ;
    if(rev_156ave>120&&VF_NOINIT_SYSINI&&VF_IDLE_ENG)
    {
        VF_AQIDLETOOHIGH_WHENNOINI_IDLEAQ=true;
    }
    else 
    {
        if(VF_AQIDLETOOHIGH_WHENNOINI_IDLEAQ)
        {
            if((rev_156ave<113&&VF_IDLE_ENG)||!VF_NOINIT_SYSINI)
            {
                VF_AQIDLETOOHIGH_WHENNOINI_IDLEAQ=false;
            }
        }
    } */
    templ=0;
    if(VF_AQAFTER_PNINI_SYSINI&&VF_WARMUP_ENG
          	          //&&VFF_NOZZLEINI_TESHIGH_DTC//2013-3-3
          	          &&(V_Tw>=50)//2013-6-4
          )//2010-3-13 add condiction !VF_O2CHANGED_OSDIAG
    {                                 //����OS������ʹ��OS����ֵ
        if(!(VFF_INJ_DTC||VFF_TPS_DTC||((VF_CANNOTWORK_OSDIAG ||VF_CANNOTFLU_OSDIAG)&&(v_500km<=12))||VFF_TES_DTC ||VF_NOTCAB_PAIR||VFF_VBAT_DTC))
        {   //  2010-10-9 delete VFF_T3ER_DTC
            if(VF_IDLE_ENG&&(!VF_IDLEAIRLESS_IGN||VF_IDLEAIRTEST_SYSINI)&&(rev_156ave<128))  //2010-5-24 delete (rev_156ave>70)
            {
                {
                    templ=1;
                    if(VF_1S_TMCCL)
                    {
                        if(t_s_matchENAQ<255)t_s_matchENAQ++;        
                    }   
                } //else statu_aq=4;
            } 
        }
    }
    
    //2010-3-23 add 
    if(VF_ADJUSTO_WHEN_AQ&&VF_ENAQ_SYSINI)
    {
        VF_ADJUSTO_WHEN_AQ=false;
        templ=0;        
    }
    
    // if(VF_MATCH_INI_CLSSL)templ=0;//2013-3-12 add   2013-6-2 delete
    
    if(templ==0)    //����δ����   //2010-5-3 add 
    {
        t_s_matchENAQ=0;
    }
    
//    if((templ==0)||VF_O2CHANGED_OSDIAG)   //2010-5-3 add condiction VF_O2CHANGED_OSDIAG
    //2010-7-14 chang to the following
    if((templ==0)||
        //(VF_O2CHANGED_OSDIAG&&(rpm_testMode==AQMODE_ENRICH||rpm_testMode==AQMODE_INI))
        (VF_MATCH_INI_CLSSL&&(rpm_testMode==AQMODE_ENRICH||rpm_testMode==AQMODE_INI))
        //2013-6-4
        )   //2010-5-3 add condiction VF_O2CHANGED_OSDIAG
    {
        //2010-5-3
        //if(VF_O2CHANGED_OSDIAG) VF_ENAQ_SYSINI=false;

        //2010-3-18 zengjia 
        if(VF_ENAQ_SYSINI)//��,���ԭ���ǿ������ٵ���״̬,����Ҫ�� vc_tw1_IDLESC���д���
        {
            if(rpm_testMode==AQMODE_ENLEAN)
            {   //vc_tw1_IDLESC+=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*5+o_aq_for_maxn[0])/8);
                vc_tw1_IDLESC+=(vc_tw1_IDLESC_inuse+o_aq_for_maxn[0])/2;    
            } else if(rpm_testMode==AQMODE_ENRICH_AGAIN)
            {
                vc_tw1_IDLESC+=(vc_tw1_IDLESC_inuse+o_aq_for_maxn[1])/2;    
            }
        }
        VF_ENAQ_SYSINI=false;
        //2010-2-6 ��δ���㵡�ٳ�ʼ��������Ƶ��QA
        //if(VF_AQAFTER_PNINI_SYSINI)
        {
            if(VF_IDLEAIRLESS_IGN||(VF_E0_TPS&&rev_156ave<(rev15d6_target-10)))
            {
                if(s_need_for_aq>10)s_need_for_aq=10;        
            }
        }
    }
    
    //v_nb_flu_lstIdle
    if(t_s_matchENAQ>4)//2010-1-5
    {
        VF_IDLECONDICTION_MATCH_SYSINI=true;   
        //if(!VF_AQOK_SYSINI&&!VF_O2CHANGED_OSDIAG)VF_ENAQ_SYSINI=true; 
        //2013-3-9 CHANGE TO THE FOLLOWING
        
        if(!VF_AQOK_SYSINI&&!VF_MATCH_INI_CLSSL&&!VF_ENAQ_SYSINI){ //2013-6-4
        //if(!VF_AQOK_SYSINI&&!VF_O2CHANGED_OSDIAG){ //2013-6-4
            /*�ڵ��ٳ�ʼ�������£����
                1\������ѧϰ������
                2\�������������ˣ���    
            */    
            VF_ENAQ_SYSINI=true; 
            VF_IDLEAIRTEST_SYSINI=false;//2013-6-2
            rpm_testMode=AQMODE_INI;//2013-6-4
        }
        
         
    }else 
    {
        VF_IDLECONDICTION_MATCH_SYSINI=false;
    }
    
    
    //��ȷ����
    //if((!(VF_O2CHANGED_OSDIAG&&VF_O2WORK_OSDIAG)||VF_OSCHANGELITTLE_OSDIAG)&&((!VF_ENAQ_SYSINI)||rpm_testMode==AQMODE_FINE))  //2008-8-24 replace  //2008-11-12 add condiction VF_OSCHANGELITTLE_OSDIAG
    //if(((!(VF_O2CHANGED_OSDIAG&&VF_O2WORK_OSDIAG)||VF_OSCHANGELITTLE_OSDIAG)&&!VF_ENAQ_SYSINI)
    //REMARK==MC_U
    //2012-11-9 add
    //if(((!(VF_O2CHANGED_OSDIAG&&VF_O2WORK_OSDIAG)||VF_OSER_OSDIAG)&&!VF_ENAQ_SYSINI)
    if((
        (!(VF_O2CHANGED_OSDIAG&&VF_O2WORK_OSDIAG)||VF_OSER_OSDIAG)
            &&!VF_ENAQ_SYSINI
            &&((REMARK==MC_OPENLOOP)||VF_1ST_INI_SYSINI)//2019-2-3 (REMARK!=MC_U)
            //&&(v_t_forlstrun_d1s<12||VF_LE_IDLETG_ONCE_REV)
            &&(VF_LE_IDLETG_ONCE_REV)
        )
        ||(VF_ENAQ_SYSINI&&(rpm_testMode==AQMODE_FINE)&&VF_1ST_INI_SYSINI))  //2008-8-24 replace  //2008-11-12 add condiction VF_OSCHANGELITTLE_OSDIAG
    {
                               //2009-12-24 ������
        //��¼��ǰת��
        /*
        if(rev156<127)
        {
            rpm156_aq=(byte)rev156;
        }else 
        {
            rpm156_aq=127; 
        }
        */
        /*
            2013-6-1 ����˵��
            �ǵ��ٳ�ʼ������OSû�з������Ҳ���U��ģʽ�£��ҳ��ֹ�ת�ٻص���
            ����
            ���ٳ�ʼ������£�AQMODE_FINEʱ 
            
            ���ɵķ�ʽ����
            
        */
        
        rpm156_aq=rev156;   
        //rpm7d8_aq=rpm156_aq*2;
        //if(VF_NOTAQING_IDLEAQ||!VF_E0_TPS)//����������־,��ʼ1����ʼ��������0���������˳�������1
        if(VF_NOTAQING_IDLEAQ||!VF_E0_TPS||VF_NOINJ_ENG||!VF_E0MORETHAN4S_TPS)//����������־,��ʼ1����ʼ��������0���������˳�������1
                                    //2009-12-26 add condiction VF_NOINJ_ENG
        {                                  //2009-12-10 ADD VF_E0_TPS
              //�жϵ���״̬��ת��  ���ٷ�
           	//if(!VF_IDLE_ENG ||(rpm156_aq>=M0_MAX_RPM_AQ)||VF_WORKSTART_ISC)
           	//if(!VF_IDLE_ENG ||(rpm156_aq>=126)||VF_WORKSTART_ISC)  //2009-11-20
           	//if(!VF_E0_TPS ||(rpm156_aq>=126)||VF_WORKSTART_ISC)  //2009-12-10
           	//if(!VF_E0_TPS||VF_CICFGT5_CFCI||VF_NOINJ_ENG||!VF_E0MORETHAN4S_TPS)  //2009-12-24
           	if(!VF_E0_TPS||VF_CICORECING_CFCI||VF_NOINJ_ENG||!VF_E0MORETHAN4S_TPS||(rpm156_aq>=126))  //2010-1-14
          	{
          	    run_need_for_aq=WAIT_RUN_AQ;//run_need_init;
          	}
          	if(run_need_for_aq>=2)
          	{
          	    run_need_for_aq--; 
          	}
            
            //�ж� O2   run_need_for_aq   s_need_for_aq  �Ƿ��������
            if(!VF_O2CHANGED_OSDIAG&&(run_need_for_aq==1)&&(s_need_for_aq==1))
          	{
          	    GET_AQ_ENABLE(true);    
          	}else
          	{
          	    GET_AQ_ENABLE(false);
          	}
        }else //������������
        {
            number_need_sum--;
            if(number_need_sum!=0) //�ж�һ����38���Ƿ����
            {
                //�ۼӺ�32�ε�ת��
                if(number_need_sum<=NB_REV_AVE_FORAQ)
                {
                    sum_rpm156_64r+=rpm156_aq; 
                    ign_sum_aq+=ignca;
                }
            }
            else  //���һ��
            {  
                number_sced--;  //������һ
                //���ݲ�������vc_tw1_IDLESC_inuse��ʱ������ֵ
                if(number_sced>STEPM_N_SC)
                {
                    sw=0;
                }else
                {
                    sw=STEPM_N_SC-number_sced;
                }
                if(sw>6)
                {
                    sw=6;
                }
                vc_tw1_IDLESC_inuse=M1_DTW1_AQ[sw];
                            
                //����ת�٣��洢�������ת�� tw1+0 ,tw1+8,tw1+14 
                n_aq[0]=n_aq[1];
                n_aq[1]=n_aq[2];
                
                ign_aq[0]=ign_aq[1];
                ign_aq[1]=ign_aq[2];
                
                //av_rpm7D8_64r=(byte)((sum_rpm156_64r>>4)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16��15.6/2=7.8
                //n_aq[2]=(byte)((sum_rpm156_64r>>5)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16��15.6/2=7.8
                n_aq[2]=(byte)(sum_rpm156_64r/NB_REV_AVE_FORAQ) ;  //2009-12-1
                  //ignca
                ign_aq[2]=(byte)(ign_sum_aq/NB_REV_AVE_FORAQ);
                
                if(number_sced!=0)  //5���Ƿ����
                { //δ��ɽ���
                //�Ƿ��ڵ���������������һ����
                    if(number_sced==2)
                    {
                      	s_need_for_aq=AQ_PERIOD	;//������һ��������ʱ��
        			
                				if(rev_156ave>=MIN_AV_N_DEC_AQ)
                				{
                				  VF_LOWRPM_IDLEAQ=false; 
                				}	    
                				else   //ת��С�����ת�٣�VF_LOWRPM_IDLEAQ��1
                				{
                				  VF_LOWRPM_IDLEAQ=true;  
                				}
                    }
                   //�Ƿ����ڶ��������ڶ�����
                    if((number_sced==1)&&(n_aq[2]<n_aq[1])) //�жϵڶ�����ת���Ƿ�ȵ�һ����С
                    {
                         
                     	  templ=n_aq[1]-n_aq[2]; //ת�ٲ���		
                				sw=(sword)(ign_aq[2])-ign_aq[1];
                				
                				if((BYPTR_MV_EXHAUST<=PL50) || (BYPTR_MV_EXHAUST>PL150))
								{
									//2013-6-20
                    				if(sw<0)sw/=4; //for 50cc idle aq;
                    				//sw=sw*16/dIgn_perRev_M16;
								}                 				
                				
                				sw=templ+sw;
                				if(sw>0)     //2010-1-26
                				{
                    				////
                    				templ=(byte)sw;
                    				if(teng<27||fmf_starts>5)    //�¶�����������������>5
                    				{
                    				  	GET_AQ_ENABLE(false);
                    				}
                    				if(templ>6)  //������С��6
                    				{
                    				    templ=6;
                    				}
                    				
                    				sw=templ;
                        		//������ʱ���������ת���½��������			
                            if(VF_1STAQ_IDLEAQ)   //��һ������
                            {
                        			    stemp=-sw;
                        			    Save_AQ_Result(stemp);   //�ò�Ŀ��?���Ƿ���������ͣ�
                        			    VF_1STAQ_IDLEAQ=false;     			
                        			    number_sced=STEPM_N_SC_2;         //�������²���
                        			    vc_tw1_IDLESC_inuse=M1_DTW1_AQ[STEPM_N_SC-number_sced];	
                        		}
                        		else
                        		{             
                        			    stemp=-sw;
                        			    Save_AQ_Result(stemp);    //ȷ��Ϊ�½����ƣ���ִ�м���
                        	      	GET_AQ_ENABLE(false);        //�˳�����              	  	
                        		}   
                				}
                    }
                }
                else
                {
                
                    //�岽��ɣ��Զ�������ɣ��ж�������
                    //	VF_NOTAQING_IDLEAQ=true;    //��λ1����������
                    
            				sw=(sword)(ign_aq[1])-ign_aq[2];
            				//sw=sw*16/dIgn_perRev_M16;
                            drpm_64_32=n_aq[2]-n_aq[1]+sw;  //3��2������ת�ٲ�ֵ
            				sw=(sword)(ign_aq[0])-ign_aq[1];
            				//sw=sw*16/dIgn_perRev_M16;
                    drpm_32_0=n_aq[1]-n_aq[0]+sw;    //2��1������ת�ٲ�ֵ

                    stemp=drpm_64_32+drpm_32_0;;//��ת�ٱ仯���ɣ�������������

                    //������ת�ٵĲ�ͬ���ƣ�����������sw�Ĵ�С
                    if(drpm_32_0==0) //1��2��������ͬ
                    {//2007-7-19��ʧ�����,���ʵ��Ľ��Զ�Ѱ�ұ�ϡһ��
                        if(stemp<0)sw=-4;else{sw=0;}
                    }
                    else
                    {//rpm32_not_0:
                      	if(drpm_32_0<0)  //1��2�����½�
                      	{
                      		if(drpm_64_32<0)sw=-13;else{sw=-9;}
                      	}
                      	else            //1��2��������
                      	{//drpm_32_0>0//rpm32_inc:
                      	    if(drpm_64_32<0)   //2��3�����½�
                            {   //	rpm32i_64d:
                        			if(stemp<0)sw=-13;else{sw=-10;}	
                            }
                        		else if(drpm_64_32==0)  //2��3������ͬ
                        		{
                        		  //sw=4;    //2010-1-13 chang from -8 to 4
                        		  //sw=-2;     //2010-1-15  from 4 to -2
                        		  sw=-4;     //2010-1-15  from -2 to -4
                        		}
                        		else       ////2��3��������
                        		{
                        		  //sw=8;    //2010-1-13  chang from -7 to 8
                        		  //sw=1;     //2010-1-15 from 8 to 1
                        		  sw=-1;     //2010-1-26 from 1 to -1
                        		}
                      	}
                    }
                    //������+����ת��ƫ��       					
                    sw+=drpm_32_0+drpm_64_32;

            				if((BYPTR_MV_EXHAUST<=PL50) || (BYPTR_MV_EXHAUST>PL150))
							{
								//2013-7-29 idle aq
                				if(sw<0)sw/=4; //for 50cc idle aq;
                				//sw=sw*16/dIgn_perRev_M16;
							}

                    stemp=sw;//w_to_templ:
                    if(stemp<0)
                    {
                        if(teng<27)  //�¶�Ҫ��
                        {
                            stemp=0;
                        }
                    }
                        
                    stemp=(sbyte)(stemp/4);//������
                    Save_AQ_Result(stemp);//	
                    GET_AQ_ENABLE(false);	  //�������˳�
        				
                }
                number_need_sum=NB_REV_SET_FORAQ;
                sum_rpm156_64r=0;
                ign_sum_aq=0;
            }
           
        }

    }
//    else if(!VF_ENAQ_SYSINI||rpm_testMode==AQMODE_FINE)
    else if(!VF_ENAQ_SYSINI)//||rpm_testMode==AQMODE_FINE)
    {  
        //vc_tw1_IDLESC_inuse=0;//   //2010-1-29
        if(vc_tw1_IDLESC_inuse>0)vc_tw1_IDLESC_inuse--;//2010-1-30
        else vc_tw1_IDLESC_inuse=0;//2010-3-19 add ,ԭ��û����һ������Ҫ�ǵ��ٳ�ʼ����os���������ź�����ˣ�
        VF_LIMITAQ_IDLEAQ=false;
        VF_NOTAQING_IDLEAQ=true;//2008-12-9
    }
      
    if(VF_1S_TMCCL&&(s_need_for_aq>1)&&!VF_ENAQ_SYSINI)
    {
        s_need_for_aq--; 
    }

    /*
    if(nb_aq>=MIN_NB_FORAQ_ENOUGH)
    {
        VF_AQSEVERALTIMES_IDLEAQ=true;//2009-4-21 ADD 
    }*///2010-3-19 DELETE
    
    //2009-11-18 add flag VF_AQAFTER_PNINI_SYSINI  , with VF_ENAQ_SYSINI
   //�������Թ��� 
   /*���ٳ�ʼ������*/
   if(VF_ENAQ_SYSINI)
   {       
       if(rpm_testMode==AQMODE_FINE)
       {
           if(VF_NOTAQING_IDLEAQ)
           {
                Mode4_time++;
                if(Mode4_time<=2)//3��
                {
                    GET_AQ_ENABLE(true);
                    VF_NOTAQING_IDLEAQ=false; 
                } 
                else
                {   //
                    
                    rpm_testMode=AQMODE_INI;
                    Mode4_time=0;

                    //2009-12-12 add 
                    //vc_tw3offset= vc_tw1_IDLESC
                    //2010-1-5 �����¶�����
                    //    sw=((sword)teng-160)/6;
                        sw=((sword)teng-160)/8;//2010-1-18 
                        if(sw>5)sw=5;
                        //else if(sw<-20)sw=-20;
                        else if(sw<-15)sw=-15; //2010-1-18
                        
                        //2013-10-31 add for fmf_aq
                        if((BYPTR_MV_EXHAUST>PL150) || (BYPTR_MV_EXHAUST<PL125))
						{
							sw/=2;
						}
                        
                        sw=vc_tw1_IDLESC+sw;
                        if(sw>100)sw=100;
                        else if(sw<-100)sw=-100;
						
						if(v_LambdaTg>=75)
						{
							if(sw<-15)sw=-15;//-40
						}
						else if(v_LambdaTg>=60)
						{
							if(sw<-20)sw=-20;//-50
						}
						else if(v_LambdaTg>=30)
						{
							if(sw<-40)sw=-40;//-60
						}



                        vc_tw1_IDLESC=(sbyte)sw;
                    //------------------
                    vc_tw3offset=FS_AQOK_FLAG(vc_tw1_IDLESC,0,vc_tw3offset,v_FMF_SCI_idle,true);
                    v_FMF_SCI_idle=0;                    
                    /*
                    sw=vc_tw3offset+vc_tw1_IDLESC;
                    if(sw>100)sw=100;
                    if(sw<-100)sw=-100;
                    vc_tw3offset=(sbyte)sw;
                    */
                    vc_tw1_IDLESC=0;
                                                    
                }
           }
       } 
       else if(rpm_testMode==AQMODE_TESTIDLEAIR)
       {    //������,���Ե�������
            VF_IDLEAIRTEST_SYSINI=true;
            VF_NOTAQING_IDLEAQ=true;

            if(VF_1S_TMCCL)
            {
                if(t_s_matchAirTest<255)t_s_matchAirTest++;    
            }
            if(t_s_matchAirTest>T_WAIT_TO_CHECKAIR_AFTER_FULLAQ)
            {
                if(!VF_IDLEAIRLESS_IGN)
                {
                    VF_IDLEAIRTEST_SYSINI=false;
                    rpm_testMode=AQMODE_FINE;    
                }
            }
       } 
       else if(rpm_testMode==AQMODE_INI) 
       {
            number_sced=AQ_TEMP_NUMBER;
            number_need_sum=NB_REV_SET_FORAQ_FULL;//ÿ��Ȧ��
            sum_rpm156_64r=0;  //ÿ��ת�ٺͣ�ȡ��32�ε�ֵ
            ign_sum_aq=0;
            ign_aq_sum=0;//2010-1-11
            Mode1_Time=0;
            Mode4_time=0;
            vc_tw1_IDLESC_inuse=0;
            rpm_testMode=AQMODE_ENRICH;
            nb_n_GT_llimit=0;//2009-11-30 add 
            nb_LeanWhenAq=0;//2010-1-16
       }
       else if(rpm_testMode<AQMODE_TESTIDLEAIR&&rpm_testMode>AQMODE_INI)//AQMODE_FINE)   //2009-11-20 change to AQMODE_TESTIDLEAIR
       { 

           GET_AQ_rpmMax_OMax(rev156,ignca);
       }
   } 
   else
   {    //
       //if(!VF_AQOK_SYSINI)GET_FALLtoAQ();  //����ʧ��
       if(rpm_testMode!=AQMODE_INI) //2009-11-26 add 
       {
            rpm_testMode=AQMODE_INI;
           // vc_tw1_IDLESC_inuse=0;//2008-4-18 zhangping add   //2010-1-29
       }
   }
    return;      
}

//�����Զ�������ʼ��
void GET_AQ_ENABLE(byte bln_aq)
{
    if(bln_aq)
    {
        VF_NOTAQING_IDLEAQ=false;  //����������λ0
        vc_tw1_IDLESC_inuse=0;  //��ʱ����ֵ0
        n_aq[0]=0;
        n_aq[1]=0;
        n_aq[2]=0;
        ign_aq[0]=0;
        ign_aq[1]=0;
        ign_aq[2]=0;
    }else
    {
        VF_NOTAQING_IDLEAQ=true;
        //vc_tw1_IDLESC_inuse=0;//   //2010-1-29
        if(vc_tw1_IDLESC_inuse>0)vc_tw1_IDLESC_inuse--;//2010-1-30
        else vc_tw1_IDLESC_inuse=0; //2010-3-19
    }
    VF_1STAQ_IDLEAQ=true; 
    number_need_sum=NB_REV_SET_FORAQ;
    sum_rpm156_64r=0;
    nb_LeanWhenAq=0;
    number_sced=STEPM_N_SC-2;      //2010-1-27 add -2
    ign_sum_aq=0;
    ign_aq_sum=0;//2010-1-11
    
    return ;
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
void Save_AQ_Result(sbyte templ)//add_templ_to_pcti:
{
    sbyte sw;
     
    /*������������˼·��
        �������ֵС��0����Ҫ����
            �ȴ�ʧ�������м������������ټ�oֵ��
        �������ֵ���ڵ���0����Ҫ����
            ��ʧ�����͵�һ��Ų��������ֵ��һ������
        ���ڣ���ǣţ�����ˣ���ˣ�vc_MissfireNotidle_IdleSC��vc_TW1_MISSF1�������ˣ�     
    */  

    if(templ>=0||!VF_LOWRPM_IDLEAQ||vc_tw1_IDLESC>=0) //VF_LOWRPM_IDLEAQת�ٹ���;���Զ�����ƫ����>=0;����������Ҫ����;
    {
        if(templ<0)
        {
            sw=templ+(sword)vc_TW1_IDLEMISSF_av;
            if(sw<=0)
            {
                sum_ti_inc_drpm=vc_TW1_IDLEMISSF_av=v_dmidle=0;
                vc_tw1_IDLESC+=(sbyte)sw;    
            } else
            {
                v_dmidle=vc_TW1_IDLEMISSF_av=(sbyte)sw;
                sum_ti_inc_drpm=(sword)vc_TW1_IDLEMISSF_av*128;    
            }
        }
        else
        {
        		sum_ti_inc_drpm=sum_ti_inc_drpm/2;//sum_ti_inc_drpmh,sum_ti_inc_drpml/=2;
        		vc_TW1_IDLEMISSF_av=(byte)(vc_TW1_IDLEMISSF_av/2);
        		v_dmidle=vc_TW1_IDLEMISSF_av;//ʧ��������ƽ��ֵ�뵱ǰֵ;
        		vc_tw1_IDLESC+=vc_TW1_IDLEMISSF_av+templ;
                    
        }
    }

    //2008-7-10 ��M8���������Զ�����������룻   
//    if(templ>=0|VF_LOWRPM_IDLEAQ|vc_tw1_IDLESCh>=0)
    //2008-8-10 ���Ӷ�������������������:
    if(nb_aq<255)  nb_aq++;
    if(templ>MAX_IDLESC||templ<MIN_IDLESC)
    {
      VF_LIMITAQ_IDLEAQ=true;
    }
    else 
    {
      VF_LIMITAQ_IDLEAQ=false;
    }
    if(templ>MAX_IDLESC)templ=MAX_IDLESC;
    if(templ<MIN_IDLESC)templ=MIN_IDLESC;
}

/*
byte n_aq[3];//0:���ϴΣ�1���ϴΣ�2������
byte n_aq_ave[2];//0:shangci,1:benci ;
byte n_aq_ave_max[2];//1:max, 0:2nd max;
byte o_aq_ave_max[2];//1:max n, 0:2nd max n;
byte n_aq_min;
byte n_aq_max;
byte o_aq_for_maxn[3];//���δ�ɨ�Ľ���� 0:���ϴΣ�1���ϴΣ�2������
*/
void GET_AQ_rpmMax_OMax(word for_rev156,sbyte ignca)
{
    sbyte sb;
    byte bSuccess;
    byte bCon_For_finishElean;
    
      //VF_AQINI_FAIL_IDLEAQ ����ʧ�ܱ�־��1 
      //VF_ENAQ_SYSINI=true ������־λ
      //VF_AQOK_SYSINI=true �����ɹ���Ҫ��λ  //2009-11-19         
    bSuccess=false;
    if(VF_O2WORK_OSDIAG&&!VF_RICH_OSDIAG )nb_LeanWhenAq++;
    bCon_For_finishElean=(rpm_testMode==AQMODE_ENLEAN)&&(nb_LeanWhenAq>NB_LEAN_LIMIT_FORLEAN&&nb_OSchange_beforeELean<v_nb_OS_flu&&(number_sced<=AQ_TEMP_NUMBER-6));
    /*if(for_rev156>127)
    {
        for_rev156=127;
    } *///2009-12-24 DELETE
    
          
    
    number_need_sum--;   //Ȧ����1
    if(number_need_sum!=0&&!bCon_For_finishElean) //�ж�һ����38Ȧ�Ƿ����
    {
        //�ۼӺ�32�ε�ת��
        if(number_need_sum<=NB_REV_AVE_FORAQ_FULL)
        {
            sum_rpm156_64r+=for_rev156; 
            ign_aq_sum+=ignca;    //2010-1-11
        }

        if(number_need_sum==NB_REV_AVE_FORAQ_FULL)
        {
            n_aq_min=for_rev156;
            n_aq_max=for_rev156;
        }
        //�ж������Сת��
        else if(number_need_sum<NB_REV_AVE_FORAQ_FULL&&number_need_sum>0)
        {
            if(n_aq_min>for_rev156)
            {
                n_aq_min=for_rev156;
            }
            if(n_aq_max<for_rev156)
            {
                n_aq_max=for_rev156;
            }
        }          
      
    }
    else  //һ�����
    {  
       
        number_sced--;  //������1
        /*�ֵ��ķ�����
            ǰ���ε�oֵһ����
            ��������ÿ�β���32us
        */      
        //����ת�٣��洢�������ת��,��Ӧ�õ��  
       	n_aq[0]=n_aq[1];
		n_aq[1]=n_aq[2];
		//n_aq[2]=(byte)((sum_rpm156_64r>>5)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16��15.6/2=7.8
		n_aq[2]=(byte)(sum_rpm156_64r/NB_REV_AVE_FORAQ_FULL);    //sum_rpm156_64r/32=sum_rpm156_64r/16��15.6/2=7.8
		
		ign_aq_ave[0]=ign_aq_ave[1];//2010-1-11
		ign_aq_ave[1]=ign_aq_ave[2];//2010-1-11
		ign_aq_ave[2]=(sbyte)(ign_aq_sum/NB_REV_AVE_FORAQ_FULL); //2010-1-11
		if(bCon_For_finishElean)n_aq[2]=n_aq[1];
        //������ƽ��ת��
        //sum_rpm156_64r=(word)((word)n_aq[0]+(word)n_aq[1]+(word)n_aq[2])/3;//����ƽ��ֵ  

        if(number_sced==AQ_TEMP_NUMBER-1)//��һ��
        {
                
        } else if(number_sced==AQ_TEMP_NUMBER-2)//���ڶ���
        {
            
        } else if(number_sced==AQ_TEMP_NUMBER-3)//��������
        {
            n_aq_ave[0]=(byte)(((word)n_aq[0]+n_aq[1]+n_aq[2])/3);//����ƽ�� 
            n_aq_ave_max[0]=n_aq_ave_max[1]=n_aq_ave[0]; 
            o_aq_ave_max[0]=o_aq_ave_max[1]=vc_tw1_IDLESC_inuse; 
            if(rpm_testMode==AQMODE_ENLEAN)sb=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENLEANAQ;
            else sb=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENRICHAQ;
            vc_tw1_IDLESC_inuse=sb;          
        } else //
        {
            n_aq_ave[1]=(byte)(((word)n_aq[0]+n_aq[1]+n_aq[2])/3);//����ƽ��  

            //�ж����ת��������¼��ӦOֵ����
            if(n_aq_ave_max[0]<n_aq_ave[1]&&!bCon_For_finishElean)
            {
                //if((rpm_testMode==AQMODE_ENRICH)||(rpm_testMode==AQMODE_ENRICH_AGAIN))
                if(rpm_testMode==AQMODE_ENLEAN)sb=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENLEANAQ;
                else sb=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENRICHAQ;
                
                if(n_aq_ave_max[1]<n_aq_ave[1])
                {   //�ֳ���һ��ת�����ֵ��
                    n_aq_ave_max[0]=n_aq_ave_max[1];
                    n_aq_ave_max[1]=n_aq_ave[1];    
                    o_aq_ave_max[0]=o_aq_ave_max[1];
                    o_aq_ave_max[1]=sb;    
                } else
                {   //�ֳ���һ��ת�ٵڶ���ֵ��
                    n_aq_ave_max[0]=n_aq_ave[1];
                    o_aq_ave_max[0]=sb;
                }
            }                                                //vc_tw1_IDLESC_inuse< o_aq_for_maxn[0]

            if(rpm_testMode==AQMODE_ENRICH)  //ģʽ1���ȵ�Ũ״̬
            { 
                //if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>12))  //Ũ�ٽ�����
                if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>8)||n_aq_ave[1]<76)  //Ũ�ٽ�����    //2009-12-4 CHANGE FROM 2 TO 8
                {                                                             //2010-1-4 add n_aq_ave[1]��������� n_aq_ave[1]<76 //
                  Mode1_Time++;
                } 
                else if(vc_tw1_IDLESC_inuse+vc_tw1_IDLESC>O_DELTA_LIMIT_FOR_RICHAQ)Mode1_Time++;  //2009-11-20
                                                        //  0.2/4us=50     //2009-11-25 change from 20 to 
                                                    //2010-1-1 ADD vc_tw1_IDLESC
                if(Mode1_Time>=2)
                {
                    //2010-1-5 add 
                    if(number_sced>=AQ_TEMP_NUMBER-8)//2010-3-15 change from -6 to -8
                    {
                        VF_AQINI_ENRICHTOOSHORT_IDLEAQ=true;
                    } else VF_AQINI_ENRICHTOOSHORT_IDLEAQ=false;
                  //�ﵽŨ����
                  rpm_testMode=AQMODE_ENLEAN;//ת��ϡģʽ
                  number_sced=AQ_TEMP_NUMBER; //����
                  Mode1_Time=0;
                  o_aq_for_maxn[0]=o_aq_ave_max[0]/2+o_aq_ave_max[1]/2;
                  if(n_aq_ave_max[1]>=M0_REV15D6_LLIMIT_FOR_FULLAQ)nb_n_GT_llimit++;
                  vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENRICHAQ;//2010-1-5 add ,��Ũʱת��Ҳ��̫��
                  nb_OSchange_beforeELean=v_nb_OS_flu;
                    //vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+8;
                } 
                else vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+8;
            }
            else if(rpm_testMode==AQMODE_ENLEAN) //ģʽ2��ϡ״̬
            {
               // if(((n_aq[2]+1)<n_aq[1])&&(number_sced<(AQ_TEMP_NUMBER-3))||((n_aq_max-n_aq_min)>=35)||rpm7d8_aq<140)  //ϡ�ٽ�����
                //2009-11-19 ���ӵ�ǰת�������ת�ٲ���ж�
            //    if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_max-n_aq_min)>=35)||n_aq_ave[1]<70||((n_aq_ave_max[1]-n_aq_ave[1])>15))  //ϡ�ٽ�����
                /*if(((n_aq_ave[1]+1)<n_aq_ave[0]) 
                    ||((n_aq_max-n_aq_min)>=20)
                    ||(ign_aq_ave[2]>ign_aq_ave[1])
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12)
                    ||((nb_LeanWhenAq>NB_LEAN_LIMIT_FORLEAN&&v_nb_OS_flu>=1&&(number_sced<=AQ_TEMP_NUMBER-6)))
                )  //ϡ�ٽ�����  */
                //2010-1-17 ����
                /*
                if(((n_aq_ave[1]+2)<n_aq_ave[0]) 
                    ||(((n_aq_max-n_aq_min)>=20)&&(number_sced<=AQ_TEMP_NUMBER-6))//��Ҫ�߼��������Ӳ������жϣ�
                    ||(ign_aq_ave[2]>ign_aq_ave[1]+1)
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12)
                    ||(bCon_For_finishElean)
                ) */ //ϡ�ٽ�����
                //2010-7-8 ���Ӳ�������
                if(
                    ((((n_aq_ave[1]+2)<n_aq_ave[0]) 
                    ||(((n_aq_max-n_aq_min)>=20))//��Ҫ�߼��������Ӳ������жϣ�
                    ||(ign_aq_ave[2]>ign_aq_ave[1]+1)
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12))&&(number_sced<=AQ_TEMP_NUMBER-6))
                    ||(bCon_For_finishElean)
                )  //ϡ�ٽ�����
                {   //                                                    //2010-1-5 chang from 35 to 20 ,70 
                    Mode1_Time++;                                       //2010-1-11 add (ign_aq_ave[1]>=ign_aq_ave[0]+3)
                    if((n_aq_max-n_aq_min)>=25)Mode1_Time++;//2010-1-5
                    if((n_aq_ave[1])<70)Mode1_Time++;      //2010-1-5
                    if(
                        (ign_aq_ave[1]>ign_aq_ave[0])
                            &&(number_sced<AQ_TEMP_NUMBER-6)
                            &&(vc_tw1_IDLESC_inuse<o_aq_for_maxn[0])
                      )Mode1_Time++;//2010-1-11
                }
                else if(vc_tw1_IDLESC_inuse+vc_tw1_IDLESC<O_DELTA_LIMIT_FOR_LEANAQ)Mode1_Time++;  //2009-11-20
                                                               // 2010-1-1 ADD  vc_tw1_IDLESC
                if(bCon_For_finishElean)Mode1_Time++;//2010-3-15 
                
                if(Mode1_Time>=2)//�ﵽϡ����
                { 
                  rpm_testMode=AQMODE_ENRICH_AGAIN;//ת��Ũģʽ3
                  Mode1_Time=0;
                  number_sced=AQ_TEMP_NUMBER; //����
                  o_aq_for_maxn[1]=o_aq_ave_max[0]/2+o_aq_ave_max[1]/2;
                  vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENRICHAQ;    //�¹�ϡ������
                  v_dO_enlean=vc_tw1_IDLESC_inuse;//2010-3-24 
                  //if((o_aq_for_maxn[1]-o_aq_for_maxn[0]<O_DELTA_FOR_NO_3RDFULLAQ)
                   //   ||!VF_AQINI_ENRICHTOOSHORT_IDLEAQ      //2010-1-5   //2010-1-16 ADD ! CORRECTION 
                  if((o_aq_for_maxn[1]-o_aq_for_maxn[0]<O_DELTA_FOR_NO_3RDFULLAQ)
                      &&!VF_AQINI_ENRICHTOOSHORT_IDLEAQ      //2010-1-5   //2010-1-16 ADD ! CORRECTION 
                    )
                  {     //���ǰ����ɨ��oֵ�����С���򲻽��е����δ�ɨ
                        bSuccess=true;
                        //o_aq_for_maxn[2]=o_aq_for_maxn[1];//2010-1-1
                        o_aq_for_maxn[2]=o_aq_for_maxn[0];//2010-1-5
                  }
                  if(n_aq_ave_max[1]>=M0_REV15D6_LLIMIT_FOR_FULLAQ)nb_n_GT_llimit++;

                } else vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENLEANAQ;
            }
            else if(rpm_testMode==AQMODE_ENRICH_AGAIN) //ģʽ3��Ũ״̬
            {
               // if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>12))  //Ũ�ٽ�����
                if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>8))  //Ũ�ٽ�����
                {
                  Mode1_Time++;
                }
                else if(vc_tw1_IDLESC_inuse+vc_tw1_IDLESC>O_DELTA_LIMIT_FOR_RICHAQ)Mode1_Time++;  //2009-11-20
                                                         //2010-1-1 
                if(Mode1_Time>=2)
                {
                    bSuccess=true;
                    o_aq_for_maxn[2]=o_aq_ave_max[0]/2+o_aq_ave_max[1]/2;
                } else vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENRICHAQ;
            }
            
            if(bSuccess)
            {
              //�ﵽŨ״̬
               //2009-11-20 change to the following
                /*
                vc_tw1_IDLESC_inuse=0;
                Mode1_Time=0;
                Mode4_time=0;
                t_s_matchAirTest=0;
                vc_tw1_IDLESC+=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*5+o_aq_for_maxn[0])/8);
                //vc_tw1_IDLESC+=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*6)/16);
                //vc_tw1_IDLESC=vc_tw1_IDLESC-10;  //2009-12-8   ��ϡһ��
                //vc_tw1_IDLESC=vc_tw1_IDLESC-8;  //2010-1-16 
                vc_tw1_IDLESC=vc_tw1_IDLESC-16;  //2010-3-4 
                */
                //2010-3-23 �����ϡɨ��ʱ�����𳵵�������������£�
               
                Mode1_Time=0;
                Mode4_time=0;
                t_s_matchAirTest=0;
                vc_tw1_IDLESC_inuse=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*5+o_aq_for_maxn[0])/8-16);
                //2010-7-15 ���� v_dO_enlean��Ӱ��
                //vc_tw1_IDLESC_inuse=(sbyte)(((sword)o_aq_for_maxn[2]+v_dO_enlean*2+o_aq_for_maxn[1]*4+o_aq_for_maxn[0])/8-16);
                //if(vc_tw1_IDLESC_inuse<o_aq_for_maxn[1]-8)vc_tw1_IDLESC_inuse=o_aq_for_maxn[1]-8;
                
                if((BYPTR_MV_EXHAUST<=PL150) && (BYPTR_MV_EXHAUST>=PL125)) //2013-10-31 ADD CONDICTION
				{
					vc_tw1_IDLESC_inuse-=16;
				}
                
                if(vc_tw1_IDLESC_inuse<v_dO_enlean)vc_tw1_IDLESC_inuse=v_dO_enlean;
                //2010-4-26
                if(vc_tw1_IDLESC_inuse>v_dO_enlean+20)vc_tw1_IDLESC_inuse=v_dO_enlean+20;

                vc_tw1_IDLESC+=vc_tw1_IDLESC_inuse;  //2010-3-4 
                vc_tw1_IDLESC_inuse=0;
                 
                /*
                if((nb_n_GT_llimit>=2)&&!VF_AIRLESS_WHENIDLEAQ_IGN) //2010-1-10 add VF_AIRLESS_WHENIDLEAQ_IGN
                {
                    rpm_testMode=AQMODE_FINE;   
                    VF_IDLEAIRTEST_SYSINI=false;
                } 
                else *///2010-5-4 delete Ϊ�˱�֤һ���ԣ��������ʱ���� 
                {
                    rpm_testMode=AQMODE_TESTIDLEAIR; 
                }
            }
            
        }
        n_aq_ave[0]=n_aq_ave[1];
        number_need_sum=NB_REV_SET_FORAQ_FULL;//ÿ������
        sum_rpm156_64r=0;  //ÿ��ת�ٺͣ�ȡ��32�ε�ֵ
        ign_sum_aq=0;
        ign_aq_sum=0;//2010-1-11    
        nb_LeanWhenAq=0;    
    }  
    
    return;   
}
/*
void Init_WildAQ_Step()   //��ʼ����ɨ��
{
    number_sced=AQ_TEMP_NUMBER;
    number_need_sum=38;//ÿ��Ȧ��
    sum_rpm156_64r=0;  //ÿ��ת�ٺͣ�ȡ��32�ε�ֵ
}
*/
/*
void GET_FALLtoAQ(void)
{
   VF_ENAQ_SYSINI=false;
   VF_AQOK_SYSINI=false ;
   rpm_testMode=AQMODE_INI;
   Mode4_time=0;
   VF_AQINI_FAIL_IDLEAQ =1;
} 
*/