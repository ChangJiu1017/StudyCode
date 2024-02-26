
/*HEAD
 ***********************************************************************
 * Filename	:ld_cls.c
 * Function	:cloose-loop control module.		
 * User		:xielian
 * Date		:2017.2.17
 * Version	:V8.2.0
 *
 *** History ***
 * V8.1.0	2017.2.17	xielian 
 *		Base function for PI-Motorcycle.
 *
 * V8.2.0	2017.2.17	xielian 
 *		1.change #if MV_EXHAUST to if MV_EXHAUST.
 *		2.b1 ��if MV_EXHAUST<=PL50�����¼������������գ�������С��������. 
 *
 * V8.3.0	2019.1.24	xielian 
 *		UP�Աջ����Ƽ�ǿ����
 *
 * V8.4.0	2019.1.25	Peter 
 *		VF_O2DEC_OS�����Ƴټ��ͣ�VF_O2DEC2_OS���ڼ�Ũ
 
 * V8.4.1	2019.1.29	xielian 
 *		strcls_tpd����bugfix
 
 * V8.4.2	2019.2.3	Peter 
 *		ɾ���������ٸ����ڼ�ķ������ƣ���Ϊ��dmlambda���ͽ�������
 *		С����Ũתϡ��������̫ǿ��+5��Ϊ+1
 * V8.4.3	2019.3.1	Peter 
 *		UP��50~60֮��ıջ����Ƽ�Ũ
 * V8.4.4	2019.3.14	xielian 
 *		С���ż�Ũ�ܿ��ŷ�;����ƫŨ����
 * V8.4.5	2019.7.13	xielian 
 *		blnNidle ����ֵ
 * V8.5.0	2019.7.18	xielian 
 *		ת�ٳ���9000�ıջ�������������
 * V8.5.1	2019.12.30	ghz 
 *		UP������������ǿ�ȣ�����ϵͳ�ȶ�
 ***********************************************************************
HEAD END*/

#include "public.h"
#include "status_st.h"
//
#include "add.h"
 
#include "look_up.h"
#include "ee.h"//2008-9-25 v_PCSET_SCIL
#include "ld.h" //2010-3-16
#include "sys_init.h"

#include "data_sys_standard.h"

#include "ad.h"

#include "sci.h"
#include "status_eng.h"

//2019-2-28
#include "add_ram_mpi.h"   //Ϊ������ tes
#include "time_ccl.h"
#include "Events.h"

//��ע��ԭ��O2���ƵĴ��볤����2K;
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
////#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_CLS_STATUS;
volatile byte VF_CCLUP_CLSbak;
volatile byte VF_LEANCON_CLSbak;
volatile byte VF_LIMITFK_CLSbak;
volatile byte VF_ENRICHLIMITFK_CLSbak;
volatile byte VF_T35_CLSbak;  
volatile byte VF_1LEAN_NOTIDLE_CLSbak;
volatile byte VF_RICH_MORE_CLSbak;
volatile byte VF_LEAN_MORE_CLSbak;

volatile V_STATUS_STR _V_OS_STATUS;
volatile V_STATUS_STR _V_OSDIAG_STATUS;
volatile V_STATUS_STR _V_OSDIAGH_STATUS;
volatile V_STATUS_STR _V_OSDIAGHL_STATUS;
//#pragma DATA_SEG DEFAULT

sbyte v_dmLambda;           // ����O2������������
sbyte vc_CLS_1;            //O2��������������AT	v_O2_ad	FEEDBACKCONTROL,MODI TINJ	BY + vc_CLS_1		
sbyte vc_CLS_2;            //same	as	leansdti //���ٷ���������

byte vc_IdleMissfSum_CLS2;     // vc_CLS_2�Ե���ʧ����������������(��Ϊ��)?
byte vc_IdleSc_CLS2;            // vc_CLS_2�Ե����Զ���������������(��Ϊ��)
sbyte leansdtav	;//	1e2h				

sbyte	v_LambdaTg;//2012-10-22

sbyte v_LambdaTg_lookup;//20210130 chenwei add ������õ�up
sbyte v_LambdaTg_break;   //20210206 chenwei add

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define M0_T_CON_FOR_IDLELEARN_S 6//�����������������������������㵡����ѧϰ�����󣬿�ʼ��ѧϰ����Ҫ�ġ���������������ʱ�䣬��λ��s//Ҫ��С��255			
#define MAX_O2FEDBTIME	45//equ	45		
//#define REMARK_MAX_NOO2FK	3//	EQU 	3	//;WHE REMARK<=3 NO v_O2_ad FK
                            //;DEFINE THE REMARK NUMBER FOR NO v_O2_ad FEEDBACK CONTROL

//#define D_TIO2_TO_NOIDLE	-2//equ	-2		;�ǵ���ʱdtio2�ڵ���ƽ���Ļ�����8US(��-8US)
#define D_TIO2_TO_NOIDLE	-6//equ	-2		;�ǵ���ʱdtio2�ڵ���ƽ���Ļ�����8US(��-8US) //2009-9-8

                                    //2008-11-15 add   ������MO_DFMF_LEANTOOLONG_WHENIDLE֮��OS�����Ĵ���
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
//һЩ������ע�ͣ�
        //-----------CONSTSP,O2WORK --------------
        //  �������ǣ��Ƕ��ͣ�ASTK_INJ_AGAIN==0ʱ������!SIGNAL_STATUS,O2_TOOLOW��(v_O2_ad<O2_MAX_WORK��O2_LAST<O2_MAX_WORK)ʱ��λ��
        //		��λ�����ǣ�����������������ʱ������λ��
        //                  ��������������(!VF_STARTS_ENG&&PCSET_SCI>REMARK_MAX_NOO2FK&&VF_O2WORK_OSDIAG&&!VF_FROMO2LOW_OSDIAG)ʱ
        //-----------O2_SEEM_WORK_T:---------------
        //  ��������CONSTSP,O2WORK�������£�ÿ4S��һ����������255
        //-----------O2FEDBTIME:-------------------
        //  ��������CONSTSP,O2WORK�������£�ÿ4S��һ����������MAX_O2FEDBTIME;
        //-----------sensor_status,from_o2_low:----	
		//��λ��������VF_FROMO2LOW_OS��
		//      ��!SENSOR_STATUS,O2_CHANGED&O2AMAX>=O2_REWORK_MINʱ��
		//      ��SENSOR_STATUS,O2_CHANGEDʱ
		//��λ������
		//      VF_O2TOOLOW_OSDIAG=TRUE;
        //      VF_TESTO2LOW_OSDIAG&(o2_change_nooil==0)
        //----------VF_TESTO2LOW_OSDIAG----------------
        //      if(VF_JCI_CFCI) if(ccl_cf>=80)VF_TESTO2LOW_OSDIAG=true;//if(v_ccl_cf>=80)constsp,test_o2_low=true;
        //-----------o2_m_t:-----------------------
        //          ����O2��ֵ�����У���ʱ����ֵ��
        //-----------o2_change_nooil---------------
        //      �����ҵ���ֵ�Ĵ�����
        //      ����ÿ���ҵ�O2������Сֵʱ���ۼ�1;
        //		��CI������������Ҫ�ж�o2_change_nooil�Ƿ�Ϊ0�����Ϊ0�����...;���O2�����Сֵƽ��ֵ��ֵ��
        //-----------av_o2:O2----------------------
        //      ��ʶ������ֵ��ƽ��ֵ����Ҫ��һЩ����
        //  ��λ֮���ʼ��ΪO2_AV_B
        //  ��VF_FROMO2LOW_OSʱ������ǰֵ����AV_O2;
        //  �ڷǶ��ͻ��ٹ��������£���saverage_o2�м���ƽ��ֵ��
        //  ��O2�ҵ���ֵ֮����Ҫ��more_to_o2sum�н���������
        //  ���Ż�ʱ������ǰ����ֵ������ֵ?
        //-----------VF_O2CHANGED_OSDIAG---------------
     	// 1 ��ʱ��!sensor_status,o2_changed��!sensor_status,from_o2_low
     	//          ��(!VF_O2CHANGED_OSDIAG&&O2_SEEM_WORK_T>=0x40)   ʱ��λ��
     	// 2�� o2_change_time==6
        //-----------O2AMAX,O2AMIN-----------------
        //    1 ��λ���ʼ��Ϊ��O2AMAX_MIN��30
        //    2 VF_FROMO2LOW_OSʱ����ֵ��O2_AD��O2_AD-7��
    	//	  3	���o2�仯�����㹻��ƽ����o2������С����������С����ȻҪ����
		//		if(VF_O2CHANGED_OSDIAG){
		//			VF_FROMO2LOW_OSDIAG=false;//	;when v_O2_ad changed more than 6 times, clear from v_O2_ad low
		//			o2amax=av_o2max;o2amin=av_o2min;//templ=av_o2min;�����õģ� }
        //    4 ��!sensor_status,o2_changed��!sensor_status,from_o2_lowʱ��
        //      ��v_nb_OS_flu�����ǽ���������
        //    5 ������4����if(!VF_O2CHANGED_OSDIAG&&O2_SEEM_WORK_T>=0x40){
        //              //;WHEN THE TIME OF v_O2_ad<O2MAX IS MORE THAN (64)*4=256S,ʱ��
        //             �����Ǹ�ֵ����av_o2max��av_o2min
        //    6 ��������������˴��� o2a_init,�Զ��߽����ٳ�ʼ��//��ʱo2amin��o2amax���ݵ�ǰ״̬��ʹ��Ĭ��ֵ��
        //      6.1 ���Ƿ�������
        //      6.2 VF_O2TOOLOW_OSDIAG|(v_O2_ad>=o2_MAX_work&O2_last>=o2_MAX_work);
        //      6.3 (!VF_O2TOOLOW_OSDIAG&(v_O2_ad<o2_MAX_work|O2_last<o2_MAX_work))&!(VF_FROMO2LOW_OSDIAG&!VF_O2CHANGED_OSDIAG)&VF_STARTS_ENG;
        //    7 ��Sget_o2_mx�����ڼ��㵱ǰĿ��ֵ��
        //    8 �ڶ��ͣ��ָ����ͺ���Set_O2Para_After_Finish_CIREC�н��г�ʼ����
        //    TOBECHECK:ע������3��5�Ĺ�ϵ��
        //------------av_o2max/av_o2min------------
        //    O2_AD��ֵO2_MAX��O2_MIN��ƽ��ֵ��
        //    ��o2_change_time=6���������ҵ���ֵ6��֮��ʼ���㣻
        //------------VF_NOTO2DEC_OS---------------
        //    notdectinumÿȦ�ݼ������ǣ�ʱVF_NOTO2DEC_OS��λ�������壻  
		//------------notdectinum------------------
		//    ÿȦ�ݼ������ǣ�ʱVF_NOTO2DEC_OS��λ�������壻
		//    �����������Ҫ����notdectinum����ֱ�Ӹ�ֵ��
		//         1 ���Żص��ٹ����ORDER_ACCEL,deccel_speedly��;��0
		//         2 ���뿪���ٺ���12
    //++++++++++++++++++++++++++++++++++++++++    
    //ע�⣬ԭ������ʦ�м���������ַ��һ���ģ�
    //ע����������α�ʹ�õģ�TOBECHECK
        //sum_o2h	��	av_o2��
        //sum_o2		��	sum_o2l��
        //av_o2max	��	av_o2amax_2
        //sum_o2maxl and sum_o2amax_2l;
        //sum_o2amax_2h and sum_o2maxh;
    //++++++++++++++++++++++++++++++++++++++++    

sbyte max_leansdt	;//	0x1c8				;07-3-13	save	the	max	of	vc_CLS_2	which	should	be	more	than	zero
sbyte min_leansdt	;//	0x1c9				;07-3-13	save	the	min	of	vc_CLS_2	which	should	be	less	than	zero
sbyte LEANSDTsum	;//	1E0H									
sword leansdtsum    ;   

byte mod_dtio2	;//	0x1e8				
byte temp_dmod_dtio2	;//	0x1ee	
byte runs_o2_fk	;//	0x10		;��¼O2�����Ĵ�������Ϊȷ��V_CCL_CLS�Ļ�׼

byte second_ffk_idle	;//	47h	;��ȫ������ĵ��ٳ���ʱ��[S];the  of idle last after full feedback (fedbtime>45)
//byte t_s_idleOS_change; //2010-3-25
sword d_cls_noidle_to_idle;//������ǵ��ٷ������Ĳ���
byte mod_d_cls_noidle_to_idle;

byte v_tes_1st;         //2009-12-17
#define TES_1ST_FOR_RICHCONTROL 20//2009-12-21 change from 25 to 20
#define NB_FOR_LEANTEST_LIMIT   4

//2008-8-13 add
byte numb_fk_m;//NUMB_FK_M;
byte nb_for_leantest;                   //nb_for_leantest   NB_FOR_LEANTEST_LIMIT

//sbyte v_Cls_Limit_PartLoad;//2010-4-10
//byte ccl_leave_idle_for_cls;//2012-7-5 likai daisu de quanshu

//2012-11-9 ADD
sbyte ub_cls_lst;
sbyte lb_cls_lst;

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Test_CLS_Limit(word FMF_frommap);//2012-11-9 delete blnIdle

void fk_run_cacl(void);
void PID_CONTRL(void);//2008-8-13 change byte fmf_trans1  to sbyte fmf_trans1
void sCONTRL_M(sword tw1_idlesc);

void IDLE_FK_WAIT_nRUNS(byte runs);//2008-8-13 add


sbyte temp_xmax;//���ֵ    //2016-12-13 ���Ϊȫ�ֱ�����from Test_CLS_Limit
sbyte temp_xmin;//������Сֵ�ľ���ֵ��

byte run_inc;

#define DMI_OS_EACH 0
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//***********INIT_O2_AFTERRESET***********************
//���ܣ�CLS���Բ����ĳ�ʼ��
void FI_CLS_AfterReset(void)
{
    
    vc_IdleMissfSum_CLS2=0;
    vc_IdleSc_CLS2=0;
    
    leansdtav=MO_FMF_CLS_INIT;  //		;2007-3-30,banked	
    leansdtsum=((MO_FMF_CLS_INIT<<7)&0x7f80);//OFF_SET_SUM*256;//2008-12-3 DELETEOFF_SET_SUM

    vc_CLS_1=MO_FMF_CLS_INIT;   //			;2007-3-30,banked  //2008-10-15 CHANGE FROM 0 TO MO_FMF_CLS_INIT
    vc_CLS_2=MO_FMF_CLS_INIT;   //			;2007-3-30,banked  //2008-10-15 CHANGE FROM 0 TO MO_FMF_CLS_INIT

    V_OS_STATUS=false;//2008-12-15		        
    if(VF_VPOR_HARD)
		{	
			VF_CCLUP_CLSbak = 0;
			VF_LEANCON_CLSbak = 0;
			VF_LIMITFK_CLSbak = 0;
			VF_ENRICHLIMITFK_CLSbak = 0;
			VF_T35_CLSbak = 0;  
			VF_1LEAN_NOTIDLE_CLSbak = 0;
			VF_RICH_MORE_CLSbak = 0;
			VF_LEAN_MORE_CLSbak = 0; 
		}
    //else V_CLS_STATUS&=0x02;//   //���� #define	VF_GETIDLEFK_CLS	_V_CLS_STATUS.Bits.b12013-3-13 DELETE
    

    //2010-7-11
    VF_IDLELEAN1_OS=false;
    //ccl_leave_idle_for_cls=0;
    
    ub_cls_lst=lb_cls_lst=0;//2012-11-14
    
    VF_T35_CLS=false;//2013-3-19
    
    v_dmLambda=MO_FMF_CLS_INIT;//2014-2-27

    
}

//2009-12-17 
void Save_1st_Tes(byte tes_1st)
{
    v_tes_1st=tes_1st;    
}


/*
    �����ʼ��ʱ����ѧϰ�Ľ��
*/
void F_SL_idleIni(sbyte sl,byte blnOK)
{
    sbyte sb_cls2;
    
    //sb_cls2= leansdtav/2+sl/2;
    sb_cls2= sl;
    
    if(blnOK){
        //sb_cls2+=8;
        vc_tw3offset=FS_AQOK_FLAG(vc_tw1_IDLESC,sb_cls2,vc_tw3offset,v_FMF_SCI_idle,true);                    
    } else{
        vc_tw3offset+=sb_cls2;
       // v_FMF_SCI_idle+=sb_cls2;        
    }
    
    v_FMF_SCI_idle=0;
    
		vc_CLS_2-=sb_cls2;
		if(vc_CLS_2>60)vc_CLS_2=60;
		else if(vc_CLS_2<-60)vc_CLS_2=-60;
		
		vc_tw1_IDLESC=0;//
		
		//2010-7-15 add 
		v_nb_flu_lstIdle=0;

    FR_idleClsAve(sb_cls2);//2013-4-11 add 
	
	v_ccl_sl=0;//2013-3-15 ÿ��ѧϰ֮�����¿�ʼ���� 
    //ÿ��ѧϰ֮�󣬲��������ۼ�
    v_OS_flu_SLpoint=0;
    
}

/*����ѧϰ֮�󣬶�ƽ��ֵ�Ĵ���*/
void FR_idleClsAve(sbyte cls)
{
    leansdtav-=cls;  //		;2007-3-30,banked	
		if(leansdtav>60)leansdtav=60;
		else if(leansdtav<-60)leansdtav=-60;
    
    //leansdtsum=0;//OFF_SET_SUM*256;//2008-12-3 DELETEOFF_SET_SUM
    //leansdtsum=(sword)leansdtav*256;
    leansdtsum=(sword)leansdtav*128;//2015-12-15 ����leansdtsum�������ԣ��ڼ����汾�ж�������
        
}

//���ܣ����������������Ʋ��ԣ�����÷������?
/*
            v_rev_625rpm,v_TPS_odr
                        ,v_TPS_mode,v_teng_inuse,v_prd_cash,v_rev_odr,v_rev_mode,v_rev_156ave,

*/
void F_CLS_OS(byte cls2_idlemissf,sword tw1_idlesc,word FMF_frommap)
{                  
    word w;
    
    //����������������������
    vc_IdleMissfSum_CLS2=0;
    vc_IdleSc_CLS2=0;
    
    vc_CLS_2+=cls2_idlemissf;//vc_cls2_idlemissf=w; //ADD_W_TO_leansdt_WHEN_MISSFIRE//ʧ��Ե��ٷ���ֵ������

    VF_LIMITFK_CLS=false;
    VF_ENRICHLIMITFK_CLS=false;
    VF_LEANLIMIT_OSDIAG=false;//

    /*--------------------------------------------------------------*/
    
    if(!VF_STARTS_ENG&&VF_O2WORK_OSDIAG){ //2008-6-28
        if(!VF_NOINJ_ENG)
		{//2013-3-11�������������
        	PID_CONTRL();//PID_CONTRL();	//�������������������ǵ��١�����Ȧ����ʱ��PID����
            Test_CLS_Limit(FMF_frommap);
        }
    	//	if(!VF_CF_CFCI&&!VF_IDLE_ENG){
    		if(!VF_E0_TPS){
    		    if(!VF_NOINJ_ENG){
    		        d_cls_noidle_to_idle=(sword)vc_CLS_1-vc_CLS_2;//2008-6-6 add
    		        if(d_cls_noidle_to_idle>127)d_cls_noidle_to_idle=127;
    		        else if(d_cls_noidle_to_idle<-127)d_cls_noidle_to_idle=-127;
    		        mod_d_cls_noidle_to_idle=0;
    		    }
    		}
    		else{
      			if(!VF_NOINJ_ENG){
  			        //if(VF_BE_ECE_ENG)vc_CLS_1=leansdtav-2; //2014-3-30
  			        //else 
  			        {
  			            vc_CLS_1=leansdtav-D_TIO2_TO_NOIDLE; //2008-6-6 add
  			        }
      			} 
      			else //��������£�
      			{
      			    if(vc_CLS_1<vc_CLS_2)
      			    {   
          			    if(d_cls_noidle_to_idle<0)
          			    {
                        w=mod_d_cls_noidle_to_idle-d_cls_noidle_to_idle;       			        
              			    mod_d_cls_noidle_to_idle=(byte)(w&0xff);
              			    vc_CLS_1=vc_CLS_1+(byte)((w>>8)&0xff);
              			    if(vc_CLS_1>vc_CLS_2)vc_CLS_1=vc_CLS_2;
          			    } 
          			    //2010-6-8 ����Դ��Ϊ�˷�ֹ����ʱ��һ��ƫϡ���������£�
          			    /*vc_CLS_1=vc_CLS_2;*/   //2010-8-24 ���ճ�CO�ߣ�������ȥ
      			    } 
      			    else if(vc_CLS_1>vc_CLS_2)
      			    {
      			        
          			    if(d_cls_noidle_to_idle>0)
          			    {
                        w=mod_d_cls_noidle_to_idle+d_cls_noidle_to_idle;       			        
              			    mod_d_cls_noidle_to_idle=(byte)(w&0xff);
              			    vc_CLS_1=vc_CLS_1-(byte)((w>>8)&0xff);
              			    if(vc_CLS_1<vc_CLS_2)vc_CLS_1=vc_CLS_2;
          			    }
      			    }
      			}
    		}
    }

    
    sCONTRL_M(tw1_idlesc);				//v_O2_ad feedback control�����������������

    
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//�������������������ǵ��١�����Ȧ����ʱ��DITO2�Ĳ���
//����������dtio2��������
void Test_CLS_Limit(word FMF_frommap)
{   
    sbyte b1;
    sbyte b2;
  //  sbyte temp_xmax;//���ֵ    //2016-12-13 ���Ϊȫ�ֱ�����from Test_CLS_Limit
  //  sbyte temp_xmin;//������Сֵ�ľ���ֵ��
    sbyte stemp;

    VF_LEANCON_CLS=false;//2012-12-7
    /*���������ֵ*/
        //2010-10-13 chang to the following:
        b1=(FMF_frommap>>3)&0x1fff;        // FMF_frommap/8
        if(b1<10)b1=10;else if(b1>40)b1=40; // 2009-12-16 CHANGE FROM 10 TO 15
        b1+=10; //CR    //2010-10-23 FROM 10 TO 5
           
        if(VF_O2CHANGED_OSDIAG)
        {
            b2=v_nb_OS_flu/2;
            //if(b2>15)b2=15;
			if(b2>20)b2=20;//CR
            if(BYPTR_MV_EXHAUST<=PL50)
			{
				if(VF_WOT_TPS)b1*=3;
                if(b1>45)b1=45;
				if(b1<15)b1=15;
			}


        } else 
        {
            /*    2013-8-31 DELETE
            #if MV_EXHAUST<=PL50
                b2=3;//2013-8-7   for Test_CLS_Limit
              //  if(VF_WOT_TPS)b1*=3;
              //  if(b1>45)b1=45;
                //2013-8-28 CHANGE TO 
                //2013-8-30 change to the following
                if(!VF_WOT_TPS)b1=0;
                if(b1>25)b1=25;
            #else
            */
                b2=0;
                //b1=b1-6; //2010-1-29
                if(VF_IDLE_ENG)
                {
                    b1=7;
					if(VF_AQAFTER_PNINI_SYSINI)b1=10;
                } 
				else 
                {
                    if(b1>15)b1=15;      //CR 15       
                }
            //#endif
        }
        temp_xmax=b1+b2+15; //CR     //15+50;
        //temp_xmin=-(b1+b2+b2);//15+15+40 //2009-12-18 change to following line   
        temp_xmin=-(b1+b1+b2);//15+50+50    

      /*
        //2016-11-10 ��ʱȷʵ��¡���ź�
        if(VF_O2CHANGED_OSDIAG){
          
            temp_xmax=60;
            temp_xmin=-100;
        }
      */
    //�����ٵ���ʱ������ֵ�����Ʒ��ɣ� 
        if(VF_AQAFTER_PNINI_SYSINI&&VF_IDLE_ENG)
        {
            if(VF_O2CHANGED_OSDIAG)
			{
				temp_xmax=40;
				temp_xmin=-60; 
			}
			else
			{
				if(v_t_eng_t_s>120)
				{
					temp_xmax=25;
					temp_xmin=-40;
				}
				else if(v_t_eng_t_s>60)
				{
					temp_xmax=20;
					temp_xmin=-30;
				}
			}
			       
        }

    //������ڲ���os�Ƿ���Ũʱƫϡ�������ʱ��Ҫ�ſ������� */
        if(!VF_FORINJ_INJPHASE&&VF_ENRICH_FORTESTLEAN_OSDIAG)
        {
            nb_for_leantest++;
            if(VF_E0_TPS)nb_for_leantest++;//2010-2-9
            if(nb_for_leantest<=NB_FOR_LEANTEST_LIMIT)
            {
                //2010-2-8
                if(!VF_E0_TPS||(v_rev_15d6rpm>86))//must higher than >1341 rpm
                {
                    stemp=b1+b1;
                    if(stemp>60)stemp=60;
                    temp_xmax+=stemp;//15+50+60  
                }
                vc_CLS_1=vc_CLS_2=temp_xmax;//ע����Ҫ��֤��ʱ��Ȼ�����ƣ�                           
            } else VF_ENRICH_FORTESTLEAN_OSDIAG=false;
        } 
        else if((VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG&&VF_IDLE_ENG) ||(VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG&&!VF_IDLE_ENG))
        {
            stemp=b1;
            if(stemp<25)stemp=25;
            temp_xmax+=stemp;//15+40+50                             
        }
    //stemp
    //���²����� //2012-12-8   
        //2013-1-14 ADD CONDICTION 
        
        if(!VF_AQAFTER_PNINI_SYSINI){ //2013-1-14 add condiction
            /*
            if(VF_E0_TPS&&V_Ta<16&&REMARK==MC_NORMAL){
                if(v_te<60){
                    temp_xmin=-5;
                    VF_LEANLIMIT_OSDIAG=true;    
                }
            }*/
            //2014-1-16 ADJUST 
            if(VF_E0_TPS&&V_Ta<0&&REMARK==MC_NORMAL){
                if(v_te<10){
                    temp_xmin=-5;
                    VF_LEANLIMIT_OSDIAG=true;    
                }
            }
			
			if(VF_E0_TPS&&(!VF_O2CHANGED_OSDIAG||(((v_t_eng_t_s<120)||(v_te<50))&&(v_O2flu_min_av>v_O2_lambdaE1))))
			//2020-12-12 ȡ��&&(v_LambdaTg>=60)�����ƣ�ȡ��&&(v_O2flu_max_av<(v_O2flu_min_av+10))������
			{
				temp_xmin=-4;
			}
        }
		
		if(!VF_E0_TPS&&(v_te<0)&&(v_TPS_odr<=7)&&(v_nb_OS_flu<50))
		{
			temp_xmin=0;
			VF_LEANLIMIT_OSDIAG=true; 
		}
		
    /*��lambda Ŀ�����mapȷ����    */
        if((v_UP_M128>0) &&(REMARK!=MC_LOADB)){
            VF_LEANLIMIT_OSDIAG=true;
            temp_xmin=0;
        }
        
        if((v_UP_M128<0) &&(REMARK!=MC_LOADB)){
            temp_xmax=0;
            //VF_LIMITFK_CLS=true;
            VF_LEANCON_CLS=true;//2012-12-7
        }
        //2012-11-9 move here
        if(REMARK==MC_U){
            temp_xmin=-2;
            temp_xmax=2;
        }
        
        //��ֹ����ʱ�������ź���
        if(VF_E0_TPS&&v_rev_625rpm>REV62D5_2500){
            temp_xmin/=2;
            temp_xmax/=4;
        }
		
		if(VF_WOT_TPS&&(v_rev_625rpm>REV62D5_9000)&&!VF_OS_LEAN_WHENRICH_OSDIAG)
		{
            if(temp_xmax>45)temp_xmax=45;
        }
		
		/*  2019-2-1,PETER DELETE
		//2018-7-3 ������ȼ�������ڼ�
		if(VF_IDLEFUELCO_ST)
		{
			temp_xmin=-5;
		} */
		
    /*   2016-12-14 DELETE 
        //2015-12-26
        if(VF_FMF_REACHMIN_INJCTL)
        {
            temp_xmin/=4;        
        }
    */    
    /*��������ֵԲ������*/
        /*
            if(temp_xmax>ub_cls_lst)temp_xmax=ub_cls_lst+1;
            else if(temp_xmax<ub_cls_lst)temp_xmax=ub_cls_lst-1;
            ub_cls_lst=temp_xmax;
        
            2016-9-1 DELETE
            
            if(temp_xmin>lb_cls_lst)temp_xmin=lb_cls_lst+1;
            else if(temp_xmin<lb_cls_lst)temp_xmin=lb_cls_lst-1;
            lb_cls_lst=temp_xmin;
        */
    /*ʹ����ֵ*/
    if(VF_IDLE_ENG)
    { 
    		max_leansdt=temp_xmax;
    		min_leansdt=temp_xmin;

        if(vc_CLS_2>=temp_xmax)
        {
            VF_LIMITFK_CLS=true;
            vc_CLS_2=temp_xmax;
            VF_ENRICHLIMITFK_CLS=true;
        } 
        else if(vc_CLS_2<=temp_xmin)
        {
            vc_CLS_2=temp_xmin;
            VF_LIMITFK_CLS=true; //2008-11-17 ԭ���������ᵼ��ȫ���ɲ�ѧϰ     
        }
    } else{
        if(vc_CLS_1>=temp_xmax)
        {
            VF_LIMITFK_CLS=true;
            vc_CLS_1=temp_xmax;
            VF_ENRICHLIMITFK_CLS=true;
        } 
        else if(vc_CLS_1<=temp_xmin)
        {
            vc_CLS_1=temp_xmin;
            VF_LIMITFK_CLS=true; //2008-11-17 ԭ���������ᵼ��ȫ���ɲ�ѧϰ     
        }
    }
    
    //2010-3-15 add
    if(VF_ENAQ_SYSINI)vc_CLS_2=0;

}



//*************fk_run_cacl********************************************************
//2008-8-13 �����ش����
void fk_run_cacl(void)
{

    //-----INC_runs_o2_fk per cycle-------------------------------
		if(VF_CF_CFCI){runs_o2_fk=0;} 
		else runs_o2_fk++;
		
		if(run_inc<255)run_inc++;

    if(VF_RICH_OSDIAG){
        //if((VF_CICORECING_CFCI&&VF_E0_TPS)||!VF_TRANSLT10_TRANS)runs_o2_fk=0;             // VF_CICFGT5_CFCI  VF_CICORECING_CFCI
        //2013-5-18 delete VF_E0_TPS
        if((VF_CICORECING_CFCI)||!VF_TRANSLT10_TRANS)runs_o2_fk=0;             // VF_CICFGT5_CFCI  VF_CICORECING_CFCI
    }
    //2008-8-13
    VF_CCLUP_CLS=false;//2008-12-13
    if(runs_o2_fk>numb_fk_m)
    {
        VF_CCLUP_CLS=true;    ;//NUMB_FK_M                 
        runs_o2_fk=1;
    }
    return;
}

//********PID_CONTRL*********************************************************
//��ʱ��RICHָ������ѭ���ģ�

/*
    �����ķ����ٶ�
        1��Ũʱ�����ͣ�ÿNȦ��һ�Σ����͵Ĳ���sqstep;


        2��ϡʱ�����ͣ�ÿNȦ��һ�Σ����͵Ĳ���sqstep;
        
    ƫϡƫŨ�����Ĵ���
        1��ƫϡ������
            Ũʱ�����ͼӿ� 
        2��ƫŨ������
              ϡʱ�������ٶȼӿ죻
              Ũʱ�������ٶȼ�����
           
*/
void PID_CONTRL(void)
{
    word w;
    sbyte sqstep;
    byte b;
    
    word step_notidle;
	byte nb_inc;
	
	byte strcls_tpd;
	
	byte blnUPrich;
	byte blnNidle;
	
	
	//201902028 �������¶���صļ�Ũ����
	blnUPrich=false;
    if(v_LambdaTg>=65)blnUPrich=true;
  	
	/*if(v_LambdaTg>=55)
	{
	    b=(v_LambdaTg-55);
	    if(b>10)b=10;
	    b=10-b;
	    b=b*8;
	    b+=80;
	    
	    if(v_teng_ad>=b)blnUPrich=true;
	}*/
	
	
	strcls_tpd=BYPTR_M0_STRCLS_TPD;	
	//if(strcls_tpd>7)strcls_tpd=7;
	
	
	blnNidle=false;//2019-7-13 û����䣬�ƺ��и�bug���ڴ����ݸ��true�ˣ�
	if(((!VF_EXH_MATCH)&&(strcls_tpd>0)&&(strcls_tpd<=7)&&(v_TPS_odr<=strcls_tpd))||((v_te<0)&&(v_TPS_odr<=7)))
	{
		blnNidle=true;
	}
	
	if(VF_CICORECING_CFCI)
	{
		blnUPrich=false;
		blnNidle=false;
	}
	


	if(VF_TRANSGT15_TRANS||!VF_TRANSLT10_TRANS||VF_CICORECING_CFCI||(v_t_NOTE0TPS_1s<100))//||VF_EXH_MATCH
		//2019-6-10 ���ɽ׶η�����һЩ
	{
		//2015-5-26 add �ǵ���ʱ�ķ����ٶ�
        step_notidle=v_rev_625rpm/8;
		
		if(step_notidle<10)step_notidle=10;
		if(step_notidle>40)step_notidle=40;
	}
	else
	{		
		step_notidle=v_rev_625rpm/2;//2019-6-10 ���������ٶ�
        
		if(step_notidle<20)step_notidle=20;
		if(step_notidle>70)step_notidle=70;
	}

    if(!VF_E0_TPS){
        vc_CLS_2=MO_FMFENRICH_BACKTOIDLE_CLS +leansdtav;//2008-11-13
    }

    fk_run_cacl();

    sqstep=0;
    if(!VF_RICH_OSDIAG){
    		//ϡŨ�л�ʱ�Է���ֵ�Ĵ���
        		if(VF_RICHLST_OSDIAG)
        		{//�����һѭ��Ũ������ѭ������Ũתϡ�Ĺ���?
          
        		  	if(VF_E0_TPS)
        		  	{
          			    sqstep=MO_STEP_FMF_IDLE_RICHTOLEAN;//2008-7-29 
        						//if((v_LambdaTg>60)&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI)))
        						if((blnUPrich)&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI)))  //2019-2-28
        						{
        							if(v_LambdaTg>70)sqstep+=1;
        							//else sqstep+=1;//B23 
        						}
          			}
          			else if(VF_TRANSLT10_TRANS)
        		    {
        		        sqstep=MO_STEP_FMF_NOTIDLE;    
        		    } 
        		    else
        		    {
        		        sqstep=MO_STEP_FMF_ACCEL;
        					//	if((v_LambdaTg>60)&&(strcls_tpd>0)&&(strcls_tpd<=7)&&(v_TPS_odr<=strcls_tpd))
        					/*	if(blnUPrich&&blnNidle)
        						{
        							sqstep+=1;//2019-2-3 5��Ϊ1������̫ǿ
        						}*/
						
        		    }
        		    VF_T35_CLS=false;//2013-3-19
        		}

        //����Ŀ��Ĵ���
        		
        	//	if(!(VF_GT30_IDLETG_REV&&VF_E0_TPS)||(VF_E0_TPS&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&(v_LambdaTg>60))||((v_LambdaTg>60)&&(strcls_tpd>0)&&(strcls_tpd<=7)&&(v_TPS_odr<=strcls_tpd)))//2018-12-29 ���٣�UP�ϴ������£���Ũ
        		if((!(VF_GT30_IDLETG_REV&&VF_E0_TPS)&&(!VF_CICORECING_CFCI))||(VF_E0_TPS&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&(blnUPrich))||((blnUPrich)&&blnNidle))//2018-12-29 ���٣�UP�ϴ������£���Ũ
				{
            		if(v_LambdaTg>0){  //�ǵ�����Ҫ��Ũ�Ļ�,ʵ�ּ�Ũ
            		    w=(word)mod_dtio2+v_LambdaTg;////2008-6-6 add (word) ,maybe there has some problem before
            		    mod_dtio2=w&0xff;
              			sqstep+=(byte)((w>>8)&0xff);//
            		}
        		}
				
				

    		//�����ٶȵ�����
            if(VF_E0_TPS){
            		numb_fk_m=16;//2008-8-13;2008-4-21 ����������Ũ���͵�Ȧ������numb_fk_m
            		//if(v_rev_625rpm>REV62D5_1750)//2008-8-13;when rpm >max_idle, �����Գ�, 16Ȧһ����
            		/*
					if(!VF_STARTS_ENG&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&VF_O2WORK_OSDIAG&&VF_O2CHANGED_OSDIAG&&VF_15S_AFTERSTART_TMCCL)
					{
						if(v_LambdaTg>75)
						{
							numb_fk_m=6;
						}
						else if(v_LambdaTg>60)
						{
							numb_fk_m=10;
						}
						else if(v_LambdaTg>30)
						{
							numb_fk_m=12;
						}
					}*/
										
					
					if(VF_GT30_IDLETG_REV)//2013-4-17
            		{
            		    //numb_fk_m=8;//2008-8-13   ;2008-4-21 �����ٸ�ʱ����ϡ���͵�Ȧ������numb_fk    
            		    //numb_fk_m=24;//2013-4-14 from 8 to 24  
            		    numb_fk_m=48;//2013-4-14 from 8 to 24  //2014-1-19 from 24 to 48
            		} 
            		else
            		{//;when rpm<max_idle, ����ͬǰ 2008-8-13
            		    if(VF_CICORECING_CFCI){                  //VF_CICFGT5_CFCI  VF_CICORECING_CFCI
            		        //numb_fk_m=12;//2008-8-13           ;2008-4-21 ���ٹ��ͽ���ǰ��������ϡ���͵�Ȧ����?
            		        numb_fk_m=24;//2008-8-13           ;2008-4-21 ���ٹ��ͽ���ǰ��������ϡ���͵�Ȧ����?
            		                        //2014-1-19 from 12 to 24
            		    } 
            		    else
            		    {   
            		        if(v_t_afterbackidle_d1s<50)
							{
								numb_fk_m=16;
							}
							else
							{
								numb_fk_m=24;
							}
							
							/*if(!VF_STARTS_ENG&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&VF_O2WORK_OSDIAG&&VF_O2CHANGED_OSDIAG&&VF_15S_AFTERSTART_TMCCL)
							{
								if(v_LambdaTg>=75)
								{
									numb_fk_m=6;
								}
								else if(v_LambdaTg>=65||blnUPrich) //2020-1-8 60  blnUPrich
								{
									numb_fk_m=10;
								}
								else if(v_LambdaTg>30)
								{
									numb_fk_m=12;
								}
							}*/
            		    }
            		}
					
					if(VF_AQAFTER_PNINI_SYSINI)
					{
						numb_fk_m=10;
					}
										            		
            		
            		if(v_LambdaTg>16){ //2016-6-2 �������� AT PID_CONTRL
                		  
                		//2010-7-11 add for Ԥ����������û����ȫ�����������
                		if(!VF_O2CHANGED_OSDIAG&&!VF_IDLELEAN1_OS)IDLE_FK_WAIT_nRUNS(10);
            		}
            		
            		
            } else{
            		//if(v_rev_625rpm<REV62D5_3500)numb_fk_m=4;
            		//else numb_fk_m=8;
            		//2015-5-26
            		numb_fk_m=step_notidle;
            		if(v_LambdaTg==0)numb_fk_m/2;//2016-6-14  //2016-7-8
            }
        		
        		
        		if(VF_CCLUP_CLS)sqstep++;        		
        		  
    }
    else{//DEC_RICH
    		//ϡŨ�л�ʱ�Է���ֵ�Ĵ���
        		if(!VF_RICHLST_OSDIAG){
          			if(VF_E0_TPS){
          			    sqstep-=MO_STEP_FMF_IDLE_LEANTORICH;//2008-5-7
          			} 
          			else if(VF_TRANSLT10_TRANS)
        		    {
        		        sqstep-=MO_STEP_FMF_NOTIDLE;    
        		    }
        		    else
        		    {
        		        if(!VF_ACCSPD_TRANS)sqstep=-MO_STEP_FMF_ACCEL;  //2010-5-3 add condiction if(!VF_ACCSPD_TRANS)
        		    }

        		}


        //����Ŀ��Ĵ���
        		if(v_LambdaTg<0){
        		    w=(word)mod_dtio2-v_LambdaTg;  //2008-6-6 add (word) ,maybe there has some problem before
        		    mod_dtio2=w&0xff;
          			sqstep-=(byte)((w>>8)&0xff);  //
        		}


    		//�����ٶȵ�����
            if(VF_E0_TPS){
                /*
                #if MV_EXHAUST<=PL50 ||MV_EXHAUST>PL150  //2013-6-15
            		    if(v_nb_flu_lstIdle>2)numb_fk_m=96;
            		    else numb_fk_m=24;
             		#else
            		    numb_fk_m=24;//2008-8-13   ;2008-4-21 ����������Ũ���͵�Ȧ������numb_fk_m
            		#endif
            		*/
            		if(v_t_afterbackidle_d1s<50)
							{
								numb_fk_m=24;
							}
							else
							{
								numb_fk_m=30;
							}
            		//numb_fk_m=24;//2017-1-4
            		
            		//2014-2-13 add 
            		/*
            		if(v_nb_flu_lstIdle>2){
            		    if(numb_fk_m<48)numb_fk_m=48;//2014-2-13
            		}
                */
            	//	if(numb_fk_m<36)numb_fk_m=36;//2014-2-14      2017-1-4 DELETE
            		
            		//2014-2-13 ��ֹ��������������ʱ������̫��
            		if(v_te<V_Ta){
            		    b=0;
            		} else{
            		    b=v_te-V_Ta;
            		}
            		
            		if(b<20){
            		    b=20-b;
            		    numb_fk_m+=b;
            		}
            		
            		//if(VF_AQAFTER_PNINI_SYSINI&&!VF_AQOK_SYSINI&&VF_O2CHANGED_OSDIAG)numb_fk_m/=2;

                //if(!M0_NOTRICH_CLS_FOR_IDLE)
                
                //if(v_LambdaTg>=32)//2013-10-22  add v_LambdaTg condition for IDLE_FK_WAIT_nRUNS
                if(v_LambdaTg>=16)//2014-1-7 FROM 32 TO 16  add v_LambdaTg condition for IDLE_FK_WAIT_nRUNS
                //if(v_LambdaTg>=16||(v_LambdaTg>=8&&v_te>50))//2014-2-12 ADD CONDICTION (v_LambdaTg>=8&&v_te>50)
                {
                	if(!VF_AQAFTER_PNINI_SYSINI&&!VF_GT30_IDLETG_REV)//2013-4-17 add !VF_GT30_IDLETG_REV
                	{    
                        if(VF_DEC_REV)w=3;else w=1;      //2008-8-13
                        w=v_O2_lambdaE1+w;  
                        		                   //2008-8-13
                        if( BYPTR_MV_EXHAUST<=PL50 ||BYPTR_MV_EXHAUST>PL150)
						{
                          
                    		    if(v_nb_flu_lstIdle>1)b=60;
                    		    else b=20;
                        }
						else
						{
                          #ifdef	TE_10K	//20210728 chenwei add 10K���� 
                           if(V_Ta<15 &&v_te<60&&!VF_AQAFTER_PNINI_SYSINI)
						  #endif

						  #ifdef	TE_2K	//20210728 chenwei add 2Kˮ�� 
                           if(V_Ta<15 &&v_te<50&&!VF_AQAFTER_PNINI_SYSINI)
						  #endif
						   	{  //2013-1-14
                                b=10;     //5 to 10
                            } else b=5;//2013-3-13 from 1 to 5
                        }   
                        
							if(v_LambdaTg>60)
							//if(blnUPrich)
							{ //2014-9-2 add 
								if(v_LambdaTg>=60)numb_fk_m+=v_LambdaTg;//2016-6-9 
							  //else numb_fk_m+=v_LambdaTg/2;//B23
							  
								if(VF_O2WORK_OSDIAG&&VF_O2CHANGED_OSDIAG)
								{
									b+=5;
								}
								//if((v_O2_ad<w||VF_O2DEC_OS||VF_O2DEC2_OS)&&(run_inc>200))IDLE_FK_WAIT_nRUNS(b);//2013-1-12 ADD CONDICTION
								if((v_O2_ad<w||VF_O2DEC_OS))IDLE_FK_WAIT_nRUNS(b);//2019-1-25 PETER
							}
							else if(v_LambdaTg>30)
							{
							//	if((v_O2_ad<w||VF_O2DEC2_OS)&&(run_inc>200))IDLE_FK_WAIT_nRUNS(b);
								if((v_O2_ad<w||VF_O2DEC_OS))IDLE_FK_WAIT_nRUNS(b);//2019-1-25 PETER
							}
                	} 
                
                }
				
				if(VF_AQAFTER_PNINI_SYSINI)
				{
					numb_fk_m=10;
				}
				
			//	if((v_LambdaTg>=60)&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI)))
				/*if((blnUPrich)&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))) //2019-2-28
				{
					if(v_LambdaTg>75)
					{
						nb_inc=40;//2020-1-8 30 20
					}
					//else if(v_LambdaTg>65||blnUPrich) //blnUPrich
					else if(v_LambdaTg>65) //2020-1-8 40 b23
					{
						nb_inc=50;//26
					}
					else if(v_LambdaTg>=60)//b23
					{
						nb_inc=50;//32
					}
					else nb_inc=60;//b23										
					
				//	if((VF_O2DEC_OS||VF_O2DEC2_OS)&&(run_inc>nb_inc))
					if((VF_O2DEC2_OS)&&(run_inc>nb_inc))//2019-1-25
					{
												
						run_inc=0;
						
						//sqstep+=DMI_OS_EACH;
						
						if(v_LambdaTg>70)
						{
							sqstep+=2;
						}
						else
						{
							sqstep+=1;
						}
						
						
						
					}
				}*/
				
                
            }
            else{
            		//if(v_rev_625rpm<REV62D5_3500)numb_fk_m=4;
            		//else numb_fk_m=8;

            		//2015-5-26
            		numb_fk_m=step_notidle;

            		VF_T35_CLS=false;
            		
            		//2015-5-26
            		//if(VF_TRANSGT15_TRANS)
            		if(!VF_TRANSLT10_TRANS)//2016-5-21 ����15û�ж���
            		{
            	      numb_fk_m=numb_fk_m+numb_fk_m;	    
            		}

            		if(v_LambdaTg==0)numb_fk_m/2;//2016-6-14  //2016-7-8
            		
            		
            		//2016-8-3
            		if(v_LambdaTg>=70&&v_LambdaTg<80)numb_fk_m+= (v_LambdaTg-60)*3;//2016-8-3
					
					if(blnNidle)
					{	
						#ifdef	TE_10K	//20210728 chenwei add 10K����
						if(V_Ta<15 &&v_te<60)
						#endif

						#ifdef	TE_2K	//20210728 chenwei add 2Kˮ��
						if(V_Ta<15 &&v_te<50)
						#endif
						{  
							b=6; 
						} 
						else 
						{
							b=4;
						}
						
						if(VF_O2WORK_OSDIAG&&VF_O2CHANGED_OSDIAG)
						{
							if(v_LambdaTg>=75)
							{
								b+=5;
							}
							else if(v_LambdaTg>=65)//60
							{
								b+=3;							
							}
							else if(blnUPrich)
							{
							    b+=1;//b23
							}
						}
												
						//if((v_LambdaTg>=30)&&(VF_O2DEC_OS||VF_O2DEC2_OS)&&(run_inc>200))IDLE_FK_WAIT_nRUNS(b);
						if((v_LambdaTg>=30)&&(VF_O2DEC_OS))IDLE_FK_WAIT_nRUNS(b);//2019-1-25
						
						
					//	if(v_LambdaTg>=60)
						/*if(blnUPrich)
						{
							if(v_LambdaTg>75)//75
							{
								nb_inc=20;//10
							}
							else if(v_LambdaTg>=65)//60
							{
								nb_inc=30;//2020-1-8 25 14
							} else nb_inc=40;//b23
										
					
							//if((VF_O2DEC_OS||VF_O2DEC2_OS)&&(run_inc>nb_inc))
							if((VF_O2DEC2_OS)&&(run_inc>nb_inc))//2019-1-25
							{												
								run_inc=0;						
								//sqstep+=DMI_OS_EACH;
								
								if(v_LambdaTg>70)
								{
									sqstep+=2;
								}
								else
								{
									sqstep+=1;
								}
							}
						}*/
						
					}
					
            		
            		
            }

            if(!VF_AQAFTER_PNINI_SYSINI||!VF_E0_TPS){
        	      if(v_LambdaTg>0)numb_fk_m+=v_LambdaTg/8; 
            	//	if(VF_E0_TPS&&!VF_GT30_IDLETG_REV){ //2013-4-17 VF_GT10_IDLETG_REV
            		if(VF_E0_TPS&&!VF_GT30_IDLETG_REV&&v_LambdaTg>16)//2016-6-2 ADD 
            		{ //2013-4-17 VF_GT10_IDLETG_REV
            		    if(v_nb_flu_lstIdle>1)//&&VF_O2CHANGED_OSDIAG)
            		    {
            		        numb_fk_m+=v_LambdaTg/8;    
                        if(VF_T35_CLS){
                            //IDLE_FK_WAIT_nRUNS(20);//2013-3-19
                            IDLE_FK_WAIT_nRUNS(6);//2013-4-19
                            VF_T35_CLS=false;//2013-5-13
                        }
            		    }
            		}
            }
            
        		if(VF_CCLUP_CLS){
        		    sqstep--;  
                if(VF_E0_TPS){
                    vc_IdleMissfSum_CLS2=1;
                    VF_IDLELEAN1_OS=true;
                }
        		}
        //2019-2-1 ,peter move from limit	,b3	
        		if(VF_IDLEFUELCO_ST)sqstep=0;
        		
    }
	
	if((v_t_ci_d1s<5)&&(v_rev_625rpm>REV62D5_9000)&&!VF_OS_LEAN_WHENRICH_OSDIAG)sqstep=0;

/*
    //2013_7_29
    if(VF_E0_TPS){
        if(
              ((v_O2_trend_DecOrNot&0x0007)==0x07)
              &&((v_O2_trend_IncOrNot&0x000f)==0x0)
          ){
              sqstep+=2;
              deDTC+=2;
          } else if(
              ((v_O2_trend_DecOrNot&0x0007)==0x07)
          ){
              sqstep+=1;
              deDTC+=1;
          }
    }
*/    
    if(VF_E0_TPS){
        vc_CLS_2+=sqstep;
		if(vc_CLS_2>100)vc_CLS_2=100;//2019-4-18 ������ܻ����
		if(vc_CLS_2<-100)vc_CLS_2=-100;
    } else {
        vc_CLS_1+=sqstep;
		if(vc_CLS_1>110)vc_CLS_1=110;//2019-4-18 ������ܻ����
		if(vc_CLS_1<-110)vc_CLS_1=-110;
    }

}

//******sCONTRL_M******************************************************
//v_O2_ad feedback control�����������������
//�������������O2������δ����ʱ����O2����ʱ���ֵ��ٺͷǵ��٣�Ȼ���������������
//�ǵ���ʱ��ֱ�ӽ���������DTIO2��
//����ʱ����leansdt����������
		//���е�ת�ٵ���һ��ֵ��ÿ4S���Զ�����ƫ������1/16ת�Ƶ�leansdt;
//��O2������ʱ��
//		leansdt����ΪĬ��ֵ����ʹ�ø�Ĭ��ֵ������������
void sCONTRL_M(sword tw1_idlesc)
{
    byte templ;
    byte temp2;
    
    v_dmLambda=0;

    if(!VF_STARTS_ENG&&(REMARK>MC_OPENLOOP)&&VF_O2WORK_OSDIAG)
    {//2008-5-20 CHANGE fkCONDICTION //2008-6-6 delete &&!VF_MAX_TPS
    	            //2012-11-9 add MC_U
    		if(VF_IDLE_ENG){//2008-11-14 ����VF_FLEANSIGNAL_OSDIAG
      			v_dmLambda=vc_CLS_2;
				/*if((BYPTR_M0_LAMBDA_FACTOR<=250)&&(BYPTR_M0_LAMBDA_FACTOR>0))	//20210922 chenwei add dmlambda��������
				{
					v_dmLambda=v_dmLambda*BYPTR_M0_LAMBDA_FACTOR/32;
				}*/	//20210922 ��ʱ�����ø��ַ�ʽ���İ������֮ǰ��Ҫɾ��20210922 19.39
				
      			if((v_rev_625rpm<=REV62D5_1815))  //2010-1-10 delete thisa condiction , and vf_idle_eng
      			{//2008-11-18 move here
        				leansdtsum+=-leansdtav+vc_CLS_2;//leansdtsumh,leansdtsum+=-leansdtav-vc_CLS_2;
        				leansdtav=(sbyte)(leansdtsum/128);//(byte)((leansdtsum>>7)&0x7f);
      			}
    		}
    		else
    		{
    			  v_dmLambda=vc_CLS_1;
				  /*
				  if((BYPTR_M0_LAMBDA_FACTOR<=250)&&(BYPTR_M0_LAMBDA_FACTOR>0))	//20210922 chenwei add dmlambda��������
					{
						v_dmLambda=v_dmLambda*BYPTR_M0_LAMBDA_FACTOR/32;
					}*/		//20210922 ��ʱ�����ø��ַ�ʽ���İ������֮ǰ��Ҫɾ��20210922 19.39
    		}
			
				//2008-3-21������ʦȷ�ϣ�O2���������󣬲��Զ������ˣ�
				//if(tw1_idlesc!=0){;//���Զ�Ѱ�ҵĵ�λ��Ϊ��,���Զ�Ѱ���뷴���ϳ�   //2010-1-5 add &&!VF_AQAFTER_PNINI_SYSINI&&!VF_AQOK_SYSINI
				if((tw1_idlesc!=0)&&!VF_AQOK_SYSINI
				        &&(!VF_AQAFTER_PNINI_SYSINI||tw1_idlesc>0) //2010-3-17 add condiction
				        &&(VF_O2CHANGED_OSDIAG)
				    //&&(!VFF_OS_DTC||VF_O2CHANGED_OSDIAG) //2010-2-7
				    )
				{
				    //���Զ�Ѱ�ҵĵ�λ��Ϊ��,���Զ�Ѱ���뷴���ϳ�   //2010-1-5 add &&!VF_AQAFTER_PNINI_SYSINI&&!VF_AQOK_SYSINI
					//;ÿ4���Ӻϳ�һ��             //2010-1-5 add !VF_AQOK_SYSINI
					//2010-2-6 add !VFF_OS_DTC
					if(VF_4S_TMCCL){
  						if(tw1_idlesc>=0){
  							//4��������ʱ�ϳ�.��1/16���Զ�Ѱ����.ת�Ƶ���������.���1/16������,ת��4us.
  							//���aq>0,׼����aq��1/16ת�Ƶ�fk��:aq-aq/16,vc_CLS_2+aq/16,���粻�������ֵ.
  							//Ӧ�����ʵ���λ���ж�,�����Զ�Ѱ����������1ms֮��.
  							templ=0;//templ=min{max(0,max_leansdt-vc_CLS_2),max(1,vc_tw1_IDLESC_h,vc_tw1_IDLESC_l/16)};
  							temp2=(byte)(tw1_idlesc/16);
  							if(max_leansdt>vc_CLS_2)templ=max_leansdt-vc_CLS_2; //2008-7-10 ������������⡢
  							        //���½�����������templֵ�ر�󣬿�����44��48����50�ȵȣ�
  							        //�ͻᵼ�� vc_CLS_2ͻ����
  							        //�����ԭ�������max_leansdtֵ���㲻�ԣ�
  							        //Ŷ����仰û���⣬��������Ǿ������⣻
  							if(temp2<1)temp2=1;
  							//if(templ<temp2)templ=temp2;//2008-7-10 �����⣻
  							if(templ<temp2)temp2=templ;//2008-7-10 (max_leansds-leansdt)is the max of what can be transed
  							                            //if templ(=aq/16)>temph, trans temph, elso trans aq/16(templ)
  							vc_IdleSc_CLS2=temp2;//ͨ������ vc_IdleSc_CLS2(ʼ��Ϊ��)
  							vc_CLS_2+=temp2;
  							leansdtav+=temp2;
  							leansdtsum+=((sword)temp2*128/2);// leansdtsumh,leansdtsuml+=templ*128/2;
  						}
  						else{
  							temp2=(byte)((-tw1_idlesc)/16);//templ=0-vc_tw1_IDLESC_h,vc_tw1_IDLESC_l/16;//��ֵ��
  							if(temp2==0)temp2=1;//2008-10-6 ADD   //��Ϊ�з���tw1_idlescС��0ʱ��������ֵת�ƣ�
  							if(vc_CLS_2>min_leansdt)templ=vc_CLS_2-min_leansdt;else templ=0;
  							if(templ>temp2)templ=temp2;//
  							vc_IdleSc_CLS2=templ;//ͨ������ vc_IdleSc_CLS2 (ʼ��Ϊ��)
  							vc_CLS_2-=templ;leansdtav-=templ;
  							leansdtsum-=(sword)templ*128/2;//leansdtsumh,leansdtsuml-=templ*128/2;
  						}
  					}
				}
			
    }
    else{
    		second_ffk_idle=0;

    		leansdtsum=((MO_FMF_CLS_INIT<<7)&0x7f80);//leansdtsumh,leansdtsum=OFF_SET_SUM*256;
    		leansdtav=MO_FMF_CLS_INIT;
    		VF_O2WORK_OSDIAG=false;
    		vc_CLS_2=MO_FMF_CLS_INIT;
    		vc_CLS_1=MO_FMF_CLS_INIT;
    		max_leansdt=MO_FMF_CLS_INIT;
    		min_leansdt=MO_FMF_CLS_INIT;

    		v_dmLambda=vc_CLS_2;
    }
    return;
}

//2008-8-13 add
//�Ƴٷ�����
void IDLE_FK_WAIT_nRUNS(byte runs)
{
    if(runs_o2_fk>runs)runs_o2_fk=runs_o2_fk-runs;
    else runs_o2_fk=0;    
}

