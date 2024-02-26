/*HEAD
 ***********************************************************************
 * Filename	:wait_moving.c
 * Function	:wait moving		
 * User		:xielian
 * Date		:2017.3.29
 * Version	:V8.1.1
 *
 *** History ***
 * V8.1.0   2017.2.27	xielian 
 *		Base module.
 * 
 * V8.1.1   2017.3.28	xielian
 *		�͵�ѹ�����������Ҫ����Init_beforeWaitingFor1stCA������InitVbat_remain()����.
 * 
 * V9.2.0   2018.4.2 yangsili
 *     1.�����ͱø�λ��ʼ��
 *     2.����33814��ʼ��
 *     3.�����ͱ�ʱ����㺯��
 *     4.����Կ�׹رպ��ͱùرյĹ���
 ***********************************************************************
HEAD END*/

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "FAM.h"
#include "Service_SCI.h"
#include "Public.h"
#include "Status_st.h"

#include "ad.h"


#include "events.h"

#include "ign.h"
#include "sci.h"
#include "inj_ctl.h"

#include "dtc.h"
#include "status_eng.h"
#include "time_ccl.h"

#include "inj_notystdc.h"

#include "add.h"

#include "ee.h"

#include "Sys_Init.h"//2008-11-2

#include "inj_phase.h"

#include "stepmotor.h"

#include "lock.h"
#include "wait_Moving.h"//2014-3-30

#include "ld.h"

#include "sci.h"

//#include "s12sci.h"

#include "frame.h"

#include "pump.h"

#include "mc33814.h"

#include "CCPS.h"	//20210922 chenwei add

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_STOP_STATUS;//2014-1-29 peter,delete
//#pragma DATA_SEG DEFAULT

#define TIME_KEYON_SHORT_T1T3   5

//////////////////////////////////////////////////////////////////////////////////////////
unsigned long int t_acc_forCAOV;
unsigned long int t_acc_StopAD;
unsigned long int t_acc_StopCom;
unsigned long int t_acc_MIL;
unsigned long int t_acc_for10ms;
word t_acc_for100ms; //2018-7-3,ysl,add
byte t_WOT;

word v_t_lstForFlag;

#define TL_FOR_PCAOV    0xffff
#define TL_FOR_STOPAD   25000//100ms
#define TL_FOR_STOPCOM  25000//100ms
#define TL_FOR_MIL      0xffff//


#define REV62D5_HYST_L  35
#define REV62D5_HYST_H  45

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
word v_NB_INJDRV;         //ͣ��״̬�£���������Ҫ�����Ĵ���     //2008-6-27 changgeo to word
word v_preinj_ca;

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
word tw1_wm;
word nb_now;
byte nb_keyonoff_forT1T3;//Կ����������
word t_com_lst;             //2008-7-29 add,��һ��ͨѶ��ʱ�䣬��T_MAINTMRΪ��׼��
byte nb_keyoffloop;         //2009-8-30
word t_delayforkeyofftest_4us;//2009-12-20
//byte HARD_RST_FLG1;
//byte HARD_RST_FLG2;
//byte v_vbat_whenkeyoff; DELETE

byte v_Tmp_bymcu;
//word t_1stDetectKeyoff;     //��һ�μ�⵽��Կ��ʱ��ʱ��  t_1stDetectKeyoff=T_MAINTMR;

byte v_rev62d5_hyUL;
byte nb_vbat_times;
//byte VFF_VBATSP_ERR; �ŵ�status_st.h��ȥ����

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
     //
void Com_whenStop(void);
void InjAction_whenStop(void);
void Trig_Inj_whenStop(word t1);//2008-12-16 move from hardtrig to here
void Action_whenKeyOff(void);//2008-12-16 add 
void Init_StopStatus_afterReset(void);// 

void Action_WhenStop(void);// 

void Init_after1stCACome(void);
void Init_beforeWaitingFor1stCA(void);
void Samp_MCUTmp(void);
//void F_Remove_Detect(void);


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//���ܣ���λ��ĳ�ʼ��
//      ����ĸ�λ�ǹ���ģ�����ָ��Ƭ������ĸ�λ��
//      ���һ�ָÿ�νǱ���е���ʧʱ�������ͷ�����Ĺ��̣�
void Init_afterReset(void)
{
    
    CLR_WDG;
    
    //==============================2008-10-30 add for ��ʼ��================
    //#ifndef SYS_TRADITIONAL
	FG_SysInit_AfterReset();//2008-11-2 add
	//#endif
    //======================================================================= 
    
    FI_TMCCL_AfterReset(false);//              2008-7-5 add  //2015-3-17 ע��˳����Ϊ�Ժ������Ӱ��
    
    FI_SCI_AfterReset();
    Init_EE_afterReset();
    Init_Int_afterReset();              //
    FI_DTC_AfterReset();                         //��Ϊ Init_InjPara_afterReset�н�����ϣ����Init_DTC_afterReset������Init_InjPara_afterReset֮ǰ
    CLR_WDG;          //2008-3-9 zhangping add ; 
    
    //PWR_ON;//2012-11-19 ADD; //2014-8-13 DELETE
    //PWR_OFF;//2013-3-23
    //2014-4-7 ADD 
    F_Terminate_Inj();//2012-11-20 ������Ϸ���һ��������ģ�ԭ��������ʱ����
    F_Terminate_Ign();  //add by zhangping 
    
    Init_InjPara_afterReset();          //������Ҫ����Check_TW1SCI_afterRDorWR����Ϊ������ܸ���ͨѶ��־λ��//-call	tv3_idle_cac;//����õ�����������Ӧ��T3:tv3_idle �Ƶ�Init_InjPara_afterReset��  
    //Check_TW1SCI_afterRDorWR();       //2008-12-16 ��ʱɾ������Ϊstopѭ������
   
    FI_TMP_AfterReset();               //2008-10-6 ADD
    FI_Rev_AfterReset();              //
    FI_TPS_AfterReset();              //     
    FI_FMF_AfterReset();                //
    FI_CLS_AfterReset();                //
    FI_P_air_AfterReset();              //2008-7-16 
    FI_SLCLS_AfterReset();              //2008-11-9
    FI_Maps_AfterReset();               //2008-12-15
    Init_MissfireNotIdle_afterReset();              //2008-11-5
    FI_Phase_Inj_AfterReset();  //2008-12-15
    FI_U_bat_AfterReset();      //2008-12-15
    FI_Starts_AfterReset();     //2008-12-15
    FI_status_AfterReset();     //2008-10-30 Ӳ����λ�󣬶�״̬�ĳ�ʼ��

    Init_StopStatus_afterReset();
    FI_Trans_AfterReset();      //2009-1-13 
    //FI_IdleAQ_BeforeStarts();   //2009-4-22 move to afterreset
    FI_IdleAQ_AfterReset();   //2009-8-25 add , devide from  FI_IdleAQ_BeforeStarts
    FI_Ign_BeforeStarts();      //2009-7-9 move here
    FI_OS_Reset();              //2009-12-12
    
    FI_Altitude_AfterReset();   //2010-4-30
    
    Init_FilmAfterReset();      //2012-8-9

    //2013-4-23 MOVE FROM STARTS TO RESET
    FI_CI();//CALL Init_CFCI_BeforeStartRun									;//��������ʼ����ǰ��CFCI���Գ�ʼ��	

//    #if ECU_TYPE==ECU_GEN
   /*
    #ifdef IS_GEN
    
        //2014-4-27 ��ECT���Ƶĳ�ʼ��
    #endif
    */ //2016-9-10 ȥ���������
        #ifdef IS_GEN
			FI_ECTafterReset();//2012-10-28
		#else
			#ifdef SYS_STEPMOTOR
				FI_ECTafterReset();
			#else	
				StepMotorEN1=1; 
				StepMotorEN2=1;
				StepMotorPhase2=0;
				StepMotorPhase1=0;
			#endif
		#endif
    
    InitLockWhenKeyON(); //2013-6-14
    
    FI_ActDTC_afterReset();//2014-4-5
    
    FI_Ld_AfterReset();//2014-4-7;
    
  #ifdef SYS_O2HEATER_S
  
  	//#ifndef	SY_HUANSONG		//20211010 chenwei add �˴��Ƿ���Բ����ֻ��ɣ���ʼ�����ص������ȣ�������
    	P_O2H_OFF
    	VF_O2H_STATUS=false;
   // #endif
	
  #endif
  
    pump_PumpInit_init(); //2018-04-16,ysl,add
	
	Init_Act();
    
    //2014-10-14 MOVE HERE	      //2014-4-17
    v_rev62d5_hyUL=REV62D5_HYST_L;

	FI_EngProtection_AfterReset();    //20210112 chenwei add ����������������س�ʼ��
	
	#ifdef SY_CCPS
		FI_CCPS_afterReset();  //20201126 chenwei add ̼�޵�ŷ�
	#endif
	
	if(VF_VPOR_HARD||VF_POR_HARD)		//20211009 chenwei add��Կ���Ǵ洢��¼��
	{
		v_pair_ad_input_keyon=0;
		v_Vbat_inuse_keyon=0;
		VF_CA_NOSIGNAL=false;
	}

}

/*=================================================*/
void Init_beforeStarts()
{
    
    //--------���¿�ʼ����׼�����͡����Ľ׶�--------------------------------------------------------

    Init_NotYSTDC_beforeStarts();////Call Set_Addtime_Before_StartRun;							;//��������ʼ����ǰ�����ü������
    FI_Starts_BeforeStarts(V_Tw,V_Ta);//call Init_STARTS_Before_StartRun	;//��������ʼ����ǰ���������ͳ�ʼ��		
    FI_OS_BeforStarts();//call Init_O2_Before_StartRun;									;//��������ʼ����ǰ��O2״̬��ʼ��

    Init_InjPara_beforeStarts();//call Init_T3FK_BeforeStartRun									;//��������ʼ����ǰ��T3�������Գ�ʼ��
    FI_Trans_BeforeStarts();//call Init_TRANS_BeforeStartRun									;//��������ʼ����ǰ�����ɲ��Գ�ʼ��	

    FI_TimeCCL_BeforeStarts();  						;//��������ʼ����ǰ����ʼ��ʱ���Ȧ��
    FI_status_ENG_BeforeStarts();//CALL Init_RUNSTATUS_BeforeStartRun	;//��������ʼ����ǰ����ת״̬��ʼ��	

    //FI_Ign_BeforeStarts();//CALL SET_IGNCA_AFTER_2CACATCHED			  				;//���÷���������������ʱ�ĵ��ǣ�
    FI_SysInit_BeforeStarts();//2008-11-2;

    FI_IdleAQ_BeforeStarts();   //2009-4-22 move to afterreset   //2009-8-25 RESUME
    FI_IdleMissfire_BeforeStarts();
    FI_Ign_afterReset();//2009-8-7 add 

    
    FI_TMP_beforeStart();//2012-8-2
    
    Init_FilmBeforeStarts();//2012-8-9
	
	#ifdef SYS_O2HEATER_S
	
		#ifndef	SY_HUANSONG		//20211010 chenwei �����Ƿ��ǻ��ɣ��ǻ������ó������
   			P_O2H_ON  //��������	
   			VF_O2H_STATUS=true;		//20211012 �������ȵı�־
   		#endif
		
	#endif
	
    
    return;
    
}
void Samp_MCUTmp(void)
{
    
    /*
        //2009-12-29 add  
    F_SelCh_ADC(CH_AD_MCUTMP);
    w=F_WaitSample_ADC_result();
    if(w>330)w=330;
    else if(w<210)w=210;
    v_Tmp_bymcu=(byte)(360-w);
    *///2014-1-29 peter,delete
}
/*=================================================*/


/*=================================================*/
/*---------���ܣ�����������ֹʱ��ͨѶ����������EE�Ķ�д---------------------------------*/
void Com_whenStop(void)
{
    
    if(VF_DOM_SYS&&v_NB_INJDRV!=0)
    {
        if(nb_now!=v_NB_INJDRV&&VF_T3GET_INTINJ)
        {
            nb_now=v_NB_INJDRV;
            VF_T3GET_INTINJ=false;
            VF_AFRAME_NEED_INTSCI=true;//TOPC();	//TOBECHECK��Ϊʲôԭ������ʦҪ������TOPC    	//ΪPC׼�����ݣ������� 
        }
    }
    else
    {
        v_PCSET_SCI_TOCOM=v_PCSET_SCI;
        v_PCKESE_SCI_TOCOM=v_PCKESE_SCI;
        v_PCBTDC_SCI_TOCOM=v_PCBTDC_SCI;
        v_PCTBACK_SCI_TOCOM=v_PCTBACK_SCI;
        
        FS_ti_pc(v_PCKESE_SCI_TOCOM,REMARK,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,vc_tw1_IDLESC,v_FMF_SCI_idle,vc_tw3offset);

        write_eeprom(v_PCKESE_SCI_TOCOM,v_PCSET_SCI_TOCOM,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,PCCQJSJD);//+call	write_eeprom//�ж���������ʱ��������д��EE
        
       	if(VF_JBEWR_EE){
     	        v_PCBTDC_SCI=0;
              v_PCTBACK_SCI=0;
              v_PCKESE_SCI=20;
       	}


      	//2008-7-13 ����Ϊ
      	if(VF_INJENDWHENSTOP_STOP)
      	{
      	    //2008-7-29 ADD CONDICTION
      	    //if(T_MAINTMR-t_com_lst>M0_T_INTERVAL_COM_WHENSTOP_4US)
            if(
                  v_PCSET_SCI_TOCOM==v_PCSET_SCI&&
                  v_PCKESE_SCI_TOCOM==v_PCKESE_SCI&&
                  v_PCBTDC_SCI_TOCOM==v_PCBTDC_SCI&&
                  v_PCTBACK_SCI_TOCOM==v_PCTBACK_SCI)
      	    {
          	    VF_AFRAME_NEED_INTSCI=true;//TOPC();    
          	    t_com_lst=T_MAINTMR;
      	    }
      	}
      	else
      	{   //2008-7-13 ������������ʱ,ÿ��ȡһ��T3ͨѶһ��;
           // if(nb_now!=v_NB_INJDRV&&VF_T3GET_INTINJ)  //VF_T4GET_INTINJ
            if(nb_now!=v_NB_INJDRV)  //VF_T4GET_INTINJ
            {
                nb_now=v_NB_INJDRV;
                VF_AFRAME_NEED_INTSCI=true;//TOPC();	//TOBECHECK��Ϊʲôԭ������ʦҪ������TOPC    	//ΪPC׼�����ݣ������� 
            }
      	}
    }
    
}                 

/*=================================================*/
//���ܣ�����������ֹʱ��׼�����Ͳ���������Ԥ�磬���ͣ�T35���Ե�
void InjAction_whenStop(void)
{
	sword sw,sw2;//20210426 chenwei add
    sbyte sb_t;
	
	if(v_TPS_odr>=15)
	{
		if(VF_1S_TMCCL&&(t_WOT<255))t_WOT++;
	}
	else
	{
		t_WOT=0;
	}
	
	/*if((REMARK==MC_INI)&&(KESEI==0)&&(v_TPS_odr>=15))
	{
		Save_Data_EE(0x7f,true);
	}*/
	
    if(VF_INJENDWHENSTOP_STOP)              //��T35���Խ���������˴�
    { 
          //return;
          VF_T35TEST_HARD=false;           //���T35���Ա�ʶ
          //if(nb_t3_whenstop<=1) 
          VF_T3NOTEXIST_INJCTL=false;    //2009-12-18  //2012-10-28 ��ʱ
          if(!VF_PREPUMPEND_SYSINI)	    //����˴�����ʼ���ͣ����ʹ������ϴη���������״̬�й�
          {	
          /*#ifndef SYS_TRADITIONAL  
			  tw1_wm=Set_TW1_forPump(v_Vbat_inuse);  //�趨����T1����   4000    
          	  if(tw1_wm>800)tw1_wm=800;       //2009-9-29 add //2013-5-17

              //v_NB_INJDRV=200;
              //v_NB_INJDRV=20;
              v_NB_INJDRV=60;  //2015-5-20  FROM 20 TO 60
		  #else
			  tw1_wm=0;
			  v_NB_INJDRV=0;
		  #endif
              Init_PN_pp();
              
              //if(V_Tw>25)v_NB_INJDRV=2;
              
              if(v_NB_INJDRV>v_nb_pump_limit)v_NB_INJDRV=v_nb_pump_limit;//2016-4-28
              
          	  Trig_Inj_whenStop(tw1_wm);               //��������������
              */
              //2020-5-25 ������������⼸����ֵΪfalse������ͨ���޷���дmap����
			  //VF_PUMPING_SYSINI=true;     //�����Ӱ��
			  VF_PUMPING_SYSINI=false;
			  VF_PUMP_INJCTL=false;
			  VF_INJENDWHENSTOP_STOP=false;


              //VF_PUMP_INJCTL=true;   //���������
              F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_PUMP);//���������
              

               
                                        //���ñ��Ϳ�ʼ��ʶ
          } 
		  /*#ifndef SYS_TRADITIONAL
          else if(!VF_NOTNEED_DROPSCAN_SYSINI&&!VFF_VBAT_DTC&&(VF_RUNENOUGH_FORINI_SYSINI))//2008-6-25      //2010-3-26 add VFF_VBAT_DTC
          {   //��ʼT1T3���Բ���
              //2008-12-1 add ���ڱ�֤����һ��T1ɨ��֮���������Ҫ����ɨ��ʱ ��Ȼ����ɨ�裻B_T1_Scaning_Needed()
              //if(v_teng_inuse<=M0_TES_ULIMIT_FOR_INI)     //2008-6-29 add condiction
              {
                  Init_T1Scaning(v_Vbat_inuse,true);   //��T1ɨ����г�ʼ����
                  v_NB_INJDRV=Get_NB_T1SCANING();//
                  nb_now=v_NB_INJDRV;
                  Trig_Inj_whenStop(tw1_wm);
                  FI_AfterInjAction_SCI_AfterReset();//2008-6-25 add 
              }

          }
		  #endif*/
          else if(!VF_PREINJEND_SYSINI)	    	//����˴�����ʼԤ�磬һ��Ԥ��Ĵ������¶�RZT��Կ��״̬���ϴη���������״̬�й�
          {
            /*#ifndef SYS_TRADITIONAL    
              //��Ҫ����y=aln(x)+b��ϵ����
              tw1_wm=Set_TW1_byDT3mode(FMF_PreInj(),v_Vbat_inuse);//2008-11-27 merge into one;   
              //tw1_wm=300;  
              v_NB_INJDRV=NB_Preinj(V_Tw,V_Ta); //SET_TIMES_FOR_PREINJ�ķ���ֵ��
              
              //if(V_Tw>20)v_NB_INJDRV=0;//2016-4-27 for preinj
              if(V_Tw>10)v_NB_INJDRV=0;//2016-4-28
            #else			   
              tw1_wm=0;
			  v_NB_INJDRV=0;
			#endif*/
			
			  v_NB_INJDRV=NB_Preinj(V_Tw,V_Ta);//2
			  tw1_wm=625;//2020-5-26 2.5ms ����ͨ����2.5~3
			  
			  nb_now=v_NB_INJDRV;//2008-7-13 add
          	  if(v_NB_INJDRV>0)Trig_Inj_whenStop(tw1_wm);            //��������������
              F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_PREINJ);      //����Ԥ�������ʶ
              //��ʱ��remark�л�������״̬��֮ǰ����Ϊ1
              FI_SCI_AfterReset();
              
          }
					//20210426 chenwei add ???????
			else if((switch_1==1))
			{
		          
				T1=T_MAINTMR;
				v_NB_INJDRV=1;
				//M_MilLed_TOGGLE;
				//M_ISVC_TOGGLE;//2010427 chenwei add
				v_nb_T3d=0;
				sw=T1-T1_lst;
				//if(sw>t_interval_4us)
				{	
					tw1_wm=Set_TW1_byDT3mode(flow_1time_ul,v_Vbat_inuse);	//??20ul
					//Trig_Inj_whenStop(tw1_wm);//???400??,??16ul????150?
					//M_FAN_TOGGLE;//20210427 chenwei add
					T1_lst=T1;
								
				}

							
			}
          else// if(VF_DOM_SYS)  2008-7-13 delete
          {   //_TOCOM
               
             // if((v_PCKESE_SCI==9&&(byte)v_PCSET_SCI==9)||(v_PCKESE_SCI==11&&(byte)v_PCSET_SCI==11))
              if((KESEI==9&&REMARK==9)||(KESEI==11&&REMARK==11))
              {
                    if(v_fmf_O>50&&v_fmf_O<1600){
                    //2014-11-28 add condiciton,Ҫ��T1����һ����Χ
                      
                        tw1_wm=v_fmf_O;////2014-8-10 vc_tw1_IDLESC+v_FMF_SCI_idle+vc_tw3offset;//v_FMF_SCI_idle;//+1024;//2009-3-10 ADD +1024
                    
                        VF_DOM_SYS=true; //2008-7-13 add
                        v_NB_INJDRV=26;
                        nb_now=v_NB_INJDRV;
                        Trig_Inj_whenStop(tw1_wm);    //�������Ͷ���;
                        //v_PCKESE_SCI=10;     //2009-3-10 delete 
                        //v_PCSET_SCI=10;
                        VF_T3GET_INTINJ=false; 
                        VF_T4GET_INTINJ=false;//2008-12-1
                    } 
              }
              else
              {
                    VF_DOM_SYS=false;// 2008-7-13 add
              }
          }
          
          
          
    } 
    else
    {   //2008-12-16 �ƶ���v_NB_INJDRV--�����ʹ����е�λ�ã��Ա�֤VF_INJENDWHENSTOP_STOP�����һ��������T4�����֮�����λ
        //2008-12-16 Ҳ������INJ_AFTER_SOMETIME�ĺ��壻
        //      ԭ��INJ_AFTER_SOMETIME��ָ���̶�ʱ��󴥷�T1��
        //      ���ڸ���Ϊ����������һ�����̶�ʱ��󴥷�������T1��ǰ0.n��
        if((v_NB_INJDRV==0)&&VF_INJSTOP_HARD)//2014-4-5 add condiction
        //if((v_NB_INJDRV==0))//2014-4-5 add condiction
        {
            VF_PUMPING_SYSINI=false;
            VF_INJENDWHENSTOP_STOP=true;
            
            VF_PUMP_INJCTL=false;//2013-4-22
            //2009-3-10 
            //if((v_PCKESE_SCI==9&&(byte)v_PCSET_SCI==9)||(v_PCKESE_SCI==11&&(byte)v_PCSET_SCI==11))
            if((KESEI==9&&REMARK==9)||(KESEI==11&&REMARK==11))
            {
                v_PCKESE_SCI=10;     //2009-3-10
                REMARKW=10;
            }
        }
        
        
        //VF_1ST_INI_SYSINI
        //if(VF_T1SCANINGBREAK_INJCTL||(!VF_1ST_INI_SYSINI&&VF_PUMPING_SYSINI&&!VF_PUMP_INJCTL))
        if(VF_T1SCANINGBREAK_INJCTL||(VF_PUMPING_SYSINI&&!VF_PUMP_INJCTL))
        {
            VF_PUMPING_SYSINI=false;
            VF_INJENDWHENSTOP_STOP=true;//2009-2-19 add    
            v_NB_INJDRV=0;//2009-8-26 ADD 
            if(VF_T1SCANINGBREAK_INJCTL)VF_NOTNEED_DROPSCAN_SYSINI=true;//2010-3-4 add 
            
            INJ_OFF;//2010-3-13
        }
    }
    

    
}

/*=================================================*/
//****************************************************************************
//���ܣ�����������ֹʱ�������Ͷ���
//�βΣ�teng_inuse�����������¶ȣ���Ϊ�Ƿ������һ�����ݣ�
//      t1������Ҫ����������������
void Trig_Inj_whenStop(word t1)    
{

    //2010-3-14 chang as following:
    VF_INJENDWHENSTOP_STOP=false;
    FP_Inj_WhenTMROver(4,t1);

    return;
}

/*=================================================*/
//���ܣ�
//  Կ�׹رպ�ʹ�ñ��õ�Դ����������£������Ķ���
void Action_whenKeyOff(void)
{
    
    CLR_WDG;


    F_Terminate_Inj();//2012-11-20 ������Ϸ���һ��������ģ�ԭ��������ʱ����
    F_Terminate_Ign();  //add by zhangping 
    P_PUMPC_OFF     //2018-4-16,ysl,�ر��ͱ�
	P_O2H_OFF

   // #if ECU_TYPE==ECU_GEN
    #ifdef IS_GEN
    
        //2014-4-27 ������ڹ�Կ�׺��ʱ�伴�жϵ�Դ
        if(v_t_eng_t_s<10
                )  //2008-10-27 REPLACED WITH v_t_eng_t_s
        {
           // PWR_ON;   
        }//
        else{
            PWR_OFF;
        }// 
    #else
        //if(v_t_eng_t_s<M0_T_DELAYED_AFTER_KEYOFF_S&&(!VF_MAY_UPDATE_FLASH))//2014-8-13 add this condiction  

        if(v_t_eng_t_s<M0_T_DELAYED_AFTER_KEYOFF_S&&(!VF_MAY_UPDATE_FLASH))//2014-8-13 add this condiction  
        {
			if(!VF_LEARZRO_KEYOFF&&(v_t_eng_t_s>5))
			{
				Fset_SM(255);//2020-12-18 ��Կ�׺����ѧ�����
				VF_LEARZRO_KEYOFF=true;
			}
        }//
        else{
            PWR_OFF;

        }//

         
    #endif
    /*----------------------------------------------------------------------*/
    //�ر�Ҫ�Ǻ�Ĵ������ڴ˽���

    Attu_TMP();             //2010-1-19
   
    //2013-3-9
    F_ActionSL_AfterKeyoff();           

    //2014-4-13 add 
    VF_INJENDWHENSTOP_STOP=true;
    v_NB_INJDRV=0;
	
	VF_MMOVE_ENG=false;
    
	VbatSpDiagWhenKeyoff();

}
       




/*=================================================*/
//��λ֮�󣬶�stopstatus��״̬���г�ʼ��
void Init_StopStatus_afterReset(void)
{
    VF_INJENDWHENSTOP_STOP=true;          //����Ϊ"��ǰ���ڲ����͵�״̬"
}

/*=================================================*/
//���ܣ�
//  ��λ֮�󣬵ȴ���һ���Ǳ�֮ǰ�ĳ�ʼ��
void Init_beforeWaitingFor1stCA(void)
{
    CLR_WDG;          //2008-3-9 zhangping add ; 

    //Samp_All_ADC_whenStop();		    //�ɼ�����ģ���źţ����а���TPS4�Σ������źŸ�1��
    //����������T35�����������Ƿ�ɹ���Ҫ�ڹ���Trig_Inj_whenStop������ж�
    //tw1_wm=Set_TW1_forT35Get();    
    tw1_wm=0;
	//v_NB_INJDRV=Set_NB_forT35Get();  //2008-3-6��������ֵ��ֵ�����⣬��Ҫ���TOBECHECK
	v_NB_INJDRV=0;

    nb_now=v_NB_INJDRV;
    
    //Trig_Inj_whenStop(tw1_wm);    //�������Ͷ���;��VF_ALLOWINJAFTERRESET_INJ=falseʱ����������T35����
	InitVbat_remain();
	
	v_preinj_ca=750;//3ms
}

//
    
/*=================================================*/
//���ܣ�
//��һ���Ǳ굽��֮ǰ�ĵȴ�
void Action_WhenStop(void)
{
    
  //ͣ��״̬�µĶ���
      //δ����ʱ����REMARK�����ƣ�
      if(!VF_KEYON_LOCK){
          if(KESEI<20){
              REMARKW=MC_LOCK;    
          }
      }
    	Com_whenStop();
      InjAction_whenStop();
      FC_IdleSC_FMF(0,vc_tw1_IDLESC_inuse,vc_tw1_IDLESC,v_FMF_SCI_idle,v_FMF_SCI_Nidle,vc_tw3offset); 
      FC_F();//call Init_STARTS_Before_StartRun	;//��������ʼ����ǰ���������ͳ�ʼ��		
	

	  v_preinj_ca=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)BYPTR_M1_PREINJ,3);
	  //if(v_preinj_ca>30)v_preinj_ca=30;	//20211009 chenwei �ſ���100
	 if(v_preinj_ca>100)v_preinj_ca=100;
	 
	  if(v_preinj_ca<2)v_preinj_ca=2;
	  v_preinj_ca=v_preinj_ca*250;//ms--4us
	  
      if(VF_INJENDWHENSTOP_STOP)OperLock();//2013-6-14  //2013-7-26 add condiction VF_INJENDWHENSTOP_STOP
      
      //2013-3-4 
      //�����ѧϰֵ
      if(REMARK==MC_SL&&KESEI==10){
          if(v_PCBTDC_SCI_TOCOM==12&&v_PCTBACK_SCI_TOCOM==12){
              Init_EE_SL();

          }
          v_PCBTDC_SCI=v_PCTBACK_SCI=0;
          KESEI=v_PCKESE_SCI=0;
      }

      if(VF_CLR_SL_SYSINI&&VF_AQAFTER_PNINI_SYSINI){
          Init_EE_SL();
          VF_CLR_SL_SYSINI=false;
      }
      
      //2014-5-9 ADD ����
      if(
          (REMARK==REMARK_CLRACT&&KESEI==KESEI_CLRACT)
          ||
          (REMARK==1&&KESEI==1)//2016-6-5
          )
      {
          ClrActive();
          KESEI=v_PCKESE_SCI=0;
          v_PCSET_SCI_TOCOM=v_PCSET_SCI=MS_PC_SET_SELB;
          VF_NOTACTIVE_LOCK=false; 

          //if(VF_KEYON_FOR1STLOCK_LOCK)VF_1ST_INI_SYSINI=true;   
      }

      
      
            
}

/*=================================================*/
//���ܣ�
//  ��һ���Ǳ굽��֮��ĳ�ʼ��
void Init_after1stCACome(void)
{
    if(!VF_KEYON_HARD)return;    //2009-1-15 add 
    VF_INJENDWHENSTOP_STOP=true;
    
    F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_ALL );
    
    VF_3RDCA_STOP=false;
    VF_STARTFROMHIGHSPEED_STOP=false;//2009-12-25

    //VF_OLDCA_CAS=true;				                //���ýǱ�Ϊ�ϽǱ�,2014-4-17 DELETE THIS LINE
    //VF_OLDCAHY_CAS=true;                      ,2014-4-17 DELETE THIS LINE
    VF_T35TEST_HARD=false;				                    	//���T35���Ա�ʶ
    VF_VPOR_HARD=false;
   // VF_POR_HARD=false;
    
    v_NB_INJDRV=0;                                  //�����������ֹ״̬ʱ�����ʹ���
    VF_INJENDWHENSTOP_STOP=true;

}


byte F_act_AfterKeyoff(void)
{
    
    VF_KEYON_HARD=false;

    VF_MMOVE_ENG=false;//2013-3-1
    
    #ifndef SYS_TRADITIONAL
		SaveGama_AfterKeyoff();//2010-3-6 add   //Ӧ���ڹ�Կ��֮���һ��ִ�У���Ϊ��ִ�н��v_gama_to_cls��Ҫ���������ջ���ѧϰֵ��
    #endif
	
    F_Terminate_Ign();  //add by zhangping 
    F_Terminate_Inj();  //call	TERMINATE_INJECTION;        
    //2009-7-31 change to the following
    //2009-9-11 ���ٳ�ʼ���ɹ���������֮��
    if(VF_AQOK_SYSINI)  //���ٵ��Գɹ���
    {     
        FM_PNInit_Result(0,0,vc_tw3offset,0,vc_tw1_t3fk_now);  //2008-10-26 add 
        FC_SL_EE_afterIni();
        Clr_DTC_History();
    } 
    
    F_sum_T_ENG_WhenStopS();            //CALL  SUM_SECOND_TO_ALL
    FM_SumError_WhenKeyOff();           //call	FM_SumError_WhenKeyOff	;no need for bank,return in bank0
    FM_TMP_air_StopOrKeyoff();          //call	SAVE_TAIR_WHEN_KEYOFF_OR_STOPRUN
    FM_TimeCCL();                       //call	SAVE_RUNCYCLESUM_WHEN_KEYOFF//call	SAVE_RUNTIMESUM_WHEN_KEYOFF
    Save_MissfireNotIdle();             //2008-11-5 add
    FM_P_air_WhenKeyOff();              //2008-11-5 add 
 
    ActionAtKeyoff();//2013-6-14
 
    ActionWhenKeyoff();//2013-10-16
 
    return 1;
}

void F_act_AfterStop(void) 
{
  
    F_sum_T_ENG_WhenStopS();                     //CALL SUM_SECOND_TO_ALL
    FM_TMP_air_StopOrKeyoff();                   //call	SAVE_TAIR_WHEN_KEYOFF_OR_STOPRUN;
    //+call	clr_wr_enable
    F_Terminate_Inj();//2012-11-20 ������Ϸ���һ��������ģ�ԭ��������ʱ����
    F_Terminate_Ign();  //add by zhangping 

    VF_RUNTOOS_ENG=true;
    //VF_POR_HARD=false;                  //2009-3-15
    if(!VF_STARTS_ENG)VF_RUNTOOS_ENG=false;     //��STARTS�׶�ͣ������Brun_tooshort��־��
}

/*=============================================================*/
//״̬֮���л������еĶ���
/*=============================================================*/
void Shift_Status(void){
      
      if(VF_JKEYON_HARD)
      {
          mc33814_Mc33814Init_init(true);//2018-4-16,ysl,33814��ʼ����÷���  PWR_ON ֮ǰ��PWR_ON�Ǳ�ʾ������ʱ�ر�CPU��Դ�����
          PWR_ON;//2014-4-15������ѵ�Դ�򿪣�
          mc33814_WriteVRSCrankingParam(0xC0);
		  //E0:1.2     B0:425mv	 70:110mv	  D0:850mv     C0��600mv
          
          SCI0CR2_RIE(DISABLE);
          
          VF_MAY_UPDATE_FLASH=false;
          if(nb_keyon_forIni==0){
              //2014-9-3 ���������������ٿ�Կ��ʱ�����ȴ�
              if(0/*Check_UpdateCmd1()*/)
              {
                  
                  VF_MAY_UPDATE_FLASH=true;//2014-8-13 add  
                 // _asm jmp _EntryPoint
                             
              } 
              
          }
          
          SCI0CR2_RIE(ENABLE);
          
          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          FI_status_AfterReset();               //2012-8-2 add   ��֤Կ�׹ر�ʱ��Ϊͣ��״̬��
          //MIL_OFF;        

          
          //Կ�׸տ���ʱ����Ҫ���еĶ���
          //�����µĴ�����Ҫ�ƶ������ʵ�λ��
          FI_Hard_AfterReset();   //�������������
          

          Init_afterReset();      //Կ�׿�����ĳ�ʼ������
  
          FI_CAS_WhenCANotCaught();   //Կ�׿����󣬽Ǳ��׼������

          //���������һ�ε�ѹ
          Samp_Save_Vbat(); //ע���ڲ�����ѹ֮ǰ��һ��Ҫ��֤PWR_ON�ˣ�
          Samp_Save_Vbat(); //ע���ڲ�����ѹ֮ǰ��һ��Ҫ��֤PWR_ON�ˣ�
          Samp_Save_Vbat(); //ע���ڲ�����ѹ֮ǰ��һ��Ҫ��֤PWR_ON�ˣ�
          Samp_Save_Vbat(); //ע���ڲ�����ѹ֮ǰ��һ��Ҫ��֤PWR_ON�ˣ�
          
          //deTPS=v_Vbat_inuse;
          Init_beforeWaitingFor1stCA();

          VF_NEWCYCLE_ATMAIN_CAS=false;
          
          
          FI_Frame_afterReset();//2016-2-27 ADD 
          Clr_t1s_WhenNewState();//2016-3-3   
          ResetTwParaWhenKeyonOroff();//2016-4-4   
          
         // v_t_eng_1s=0;

		  VbatSpDiagWhenKeyon();
		  
		  VF_VRSLIMT_H=false;
      }
      
      if(VF_JMMOVE_ENG)
      {
          FI_status_ENG_BeforeStarts();
          Init_beforeStarts();
          
          Clr_t1s_WhenNewState();//2016-3-3 
          
          //v_t_eng_1s=0;   

		  //20210708 chenwei add
		
        F_ChargeEn = 0;
		F_Trig_IgnEn = 1;
		Trig_IgnTmeptime = 0;
		F_Trig_IgnEn2=0;
      }
      
      if(VF_JSTOP_ENG)
      {   //ͣ��ʱ��Ҫ���Ͻ��еĶ���
          VF_MMOVE_ENG=false;//2015-12-8 ���ͨ����ʼ����ͣ�������⣬at vf_jstop_eng
          F_act_AfterStop();              
          
          VF_NEWCYCLE_ATMAIN_CAS=false;

          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          FI_status_AfterReset();               //2012-8-2 add   ��֤Կ�׹ر�ʱ��Ϊͣ��״̬��

          Init_afterReset();      //Կ�׿�����ĳ�ʼ������
          FI_CAS_WhenCANotCaught();   //Կ�׿����󣬽Ǳ��׼������

          Clr_t1s_WhenNewState();//2016-3-3 
          
          //v_t_eng_1s=0;   
      }
      
      
      if(VF_JKEYOFF_HARD)
      {
          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          F_act_AfterKeyoff();
          FI_CAS_WhenCANotCaught();   //Կ�׿����󣬽Ǳ��׼������
          
          VF_MMOVE_ENG=false;//2014-8-13 add
          
          Frame_action_whenKeyoff();//2016-2-27 
          Clr_t1s_WhenNewState();//2016-3-3
          
          //v_t_eng_1s=0;  
          
          ResetTwParaWhenKeyonOroff();//2016-4-4   
		  
		 // Fset_SM(255);//2020-8-17 ��Կ�׺����ѧ�����
  
      }
      


      VF_JMMOVE_ENG=false;
      VF_JSTOP_ENG=false;
      
      VF_JKEYON_HARD=false;
      VF_JKEYOFF_HARD=false;
      
      VF_POR_HARD=false; 
      VF_VPOR_HARD=false; //2014-4-10 ADD          

}

/*=============================================================*/
//У��Կ�׵�״̬
//��Ҫ���ǵ�ѹ������
/*=============================================================*/
void Check_KeyStatus(void)
{
    byte b;
    word w;

    //2016-7-31 MOVE HERE
    if(!VF_JKEYOFF_HARD||VF_JKEYON_HARD)
    {
        FC_KeyStatus();//�ж�Կ�׿�����رպ��ʱ�䣬����ص�״̬       2016-7-31 ��������ߣ����ܷ���������Կ��ʱ�жϲ��ԣ�
    }
    //
    if(VF_KEYON_HARD){
        //���ԭ��ΪԿ�׿���״̬
        if(!IS_KEY_OFF){
            //��¼���һ��Կ�׿�����ʱ��
            t_1stDetectKeyoff=T_MAINTMR;
            
        } else{
            //����Կ�׿�ʼ�йرռ���
            w=T_MAINTMR-t_1stDetectKeyoff;

            if(v_Vbat_inuse>20){
                //�������˿�õ�
                if(w>2500){
                    VF_KEYON_HARD=false;
                    VF_JKEYOFF_HARD=true;
                    v_nb_off++;    
                }
            } else{
                if(w>1000){
                    VF_JKEYOFF_HARD=true;
                    VF_KEYON_HARD=false;
                    v_nb_off++;    
                }
            }
        }
    
    } else{
        //���ԭ��ΪԿ�׹ر�״̬  
        b=0;
        if(!IS_KEY_OFF){
            for(b=0;b<=10;b++)
            {
                
                if(!IS_KEY_OFF)
                {
                    
                    //2013-10-12 ��Կ�ף���Ҫ�ж��ȶ�һ��ʱ����ٽ���һ����������
                    w=T_MAINTMR;
                    while((word)(T_MAINTMR-w)<250){ //��ʱ1ms
                        CLR_WDG;
                    }

                } else{
                    break;
                }
            }
            
        } 
        
        if(b>=10){
            VF_KEYON_HARD=true;
            VF_JKEYON_HARD=true;
        }
    }
    //FC_KeyStatus();//�ж�Կ�׿�����رպ��ʱ�䣬����ص�״̬       2016-7-31 ��������ߣ����ܷ���������Կ��ʱ�жϲ��ԣ�
    
}

//��ʼ����ʱ��ʶ
void FI_TimeOutFlag(void)
{
    VF_PRDMAINTMR_TMR=false;
    VF_SAMPATIME_STOP=true;
    VF_PCAOV_CAS=false;
    VF_1COM_STOP=false;
    
}

//���ܣ����㳬ʱʱ���׼
void FC_TimeOutFlag(void)
{
    word w;
    word wt;
    
    wt=T_MAINTMR;
    
    w=wt-v_t_lstForFlag;
    
    t_acc_forCAOV+=w;
    t_acc_StopAD+=w;
    t_acc_StopCom+=w;
    t_acc_MIL+=w;
    t_acc_for10ms+=w;
    t_acc_for100ms+=w;//2018-7-3,ysl,add
    
        //��ʱ��Ĵ���
    if(VF_MMOVE_ENG)
	{
        //V_TMCCL_STATUS&=0x80;  ��0�󲻼�ʱ�ˣ�
		if(VF_NEWCYCLE_ATMAIN_CAS)
		{
			FF_T(v_prd_cas,false); 
		}
		//else
		//{
			//FF_T(0,true); ��0�󲻼�ʱ�ˣ�
		//}
		
    }
    else{
        FF_T(w,false);
    }
    
    v_t_lstForFlag=wt;    

    VF_PRDMAINTMR_TMR=false;
    VF_SAMPATIME_STOP=false;
    VF_1COM_STOP=false;
    //VF_PCAOV_CAS=true;

    //ͣ���ڼ䣬������ʱ���ʶ
    if(t_acc_StopAD>TL_FOR_STOPAD)
    {
        if(!VF_MMOVE_ENG)VF_SAMPATIME_STOP=true;
        FCLEAR_ADATSTOP    
    }
    
    //ָʾ��������ʱ��
    if(t_acc_MIL>TL_FOR_MIL)
    {
        VF_PRDMAINTMR_TMR=true;
        FCLEAR_MIL    
    }
    
    //�Ǳ������ʱ���ʶ
    if(t_acc_forCAOV>TL_FOR_PCAOV)
    {
        
        VF_PCAOV_CAS=true;
    }
    
    //ͣ��ʱͨѶ��ʱ���ʶ 
    if(t_acc_StopCom>TL_FOR_STOPCOM)
    {
        if(!VF_MMOVE_ENG)VF_1COM_STOP=true;
        FCLEAR_COMATSTOP
    }
    
    
    //2015-10-30 ��Ҫ����10ms��
    if(t_acc_for10ms>(word)2500) 
    {
        Accu_CanParaTime_Per10ms();
        t_acc_for10ms=0;
        
    }
     
    //2018-7-3,ysl,add
    if(t_acc_for100ms>(word)25000) 
    {
        VF_33814DIAGT_GEN = true;
        pump_PumpTimeAcc_100ms(); 
        t_acc_for100ms=0;
    }
}


//�������ڴ������ĽǱ�
//void FS_CA_st(void)
void FS_CA_st(sbyte ignCa)
{

    CLR_WDG;

   
    VF_NEWCYCLE_ATMAIN_CAS=false;
    

    //��Ҫ��������ǰ��Ϊ��׼�����Ǻ���Ϊ��׼
    if(!VF_OLDCA_CAS){
        VF_CAST_BESET_CAS=false;
        VF_CAHYPREDICT_TIMEIN_CAS=false;//2015-3-2 ����VF_CAHYPREDICT_TIMEIN_CAS��VF_OLDCA_CAS֮��Ϊ����������ì�ܵĹ�ϵ��
        
        VF_CAMAIN_TIMEOUT_CAS=false;
        VF_CA_BEDIAGED_CAS=false;
        VF_CADIAG_TIMEIN_CAS=false;
        
        VF_OPEREE_1CYCLE_EE=false;
        
        VF_CASHY_BECHECK1_CAS=false;
        VF_CASHY_BECHECK2_CAS=false;
        VF_CASHY_BECHECK3_CAS=false;
        
        /*
        VF_TPS_ED_AD=false;
        VF_VBAT_ED_AD=false;

        if(VF_E0_TPS){//2014-10-8
            if(VF_YSTDC_INJPHASE)VF_OS_ED_AD=false;
        }
        else VF_OS_ED_AD=false;
        
        VF_PAIRS_ED_AD=false;
        VF_TES_ED_AD=false;
        VF_TAS_ED_AD=false;
        */
        //2015-3-8 MOVE DOWN
        
        VF_CASHY_BECHECK1_CAS=false;//2015-3-3 �Ȳ������ص�����£����ܵ��
        
        //VF_BEINJ_1CYC_INJPHASE=false;//�������ں�������֮����λ  2014-5-18 DELETE
        //V_AD_STATUS=false;
    }

    /*
            //ֱ��ʹ��ǰ��
        //if(!VF_OLDCA_CAS)
        {
            m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
            v_t_cas=v_t_cas0;           //���ݽǱ겶���ʱ��  v_t_cas0���ڽǱ��ж��л�õ�
            VF_CAST_BESET_CAS=true;
        }
        v_rev62d5_hyUL=REV62D5_HYST_L;


     return;
    */
     
    //����ýǱ�Ԥ���CAHYӦ�õ���
    if(!VF_CAST_BESET_CAS){

            //2015-3-2 MOVE HERE
            if(!VF_CAHYPREDICT_TIMEIN_CAS) {
                //if((word)(T_MAINTMR-v_t_cas0)>v_prd_cas/4)
                if((word)(T_MAINTMR-v_t_cas0)>v_prd_cas/3) //2015-3-3 ���������ȡ1/3��
                {
                    v_t_CAHY_timeout=T_MAINTMR;
                      
                    VF_CAHYPREDICT_TIMEIN_CAS=true;
                    //VF_CAHYPREDICT_TIMEIN_CAS=false;
                    //deDTC++;

                }

            }


            //���������ת�ٵ�
         /*   if(
                (!VF_HYER_CAS)
                &&
                //(v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||((VF_E0_TPS||ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&v_rev_625rpm<v_rev62d5_hyUL))
                //2015-3-2 �����ǰ�ص��ʱ̫��
                //(v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||((ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&(v_rev_625rpm<v_rev62d5_hyUL||VF_E0_TPS))) //�ᵼ�µ���ʱ�����л�ʱ������ǰ�����л�
                (v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||(VF_E0_TPS&&!VF_IGNONLY_CFCI)||((ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&(v_rev_625rpm<v_rev62d5_hyUL||VF_E0_TPS)))
              ) 
              {
                
                //
                if(!VF_OLDCAHY_CAS) {
                    VF_NEWCYCLE_ATMAIN_CAS=true;//�µ�һȦ��ʼ
                    v_t_cas=v_t_cashy0;//�����жϵõ��ĺ��أ�
                		m_nbtdc0=BYPTR_M0_CAS_TRAILEDGE;
                		VF_CAST_BESET_CAS=true;
                		
                }
                else if(VF_CAHYPREDICT_TIMEIN_CAS)
                {

                    
                    if(VF_OLDCAHY_CAS){
                        //����µĺ��ػ�û����
                        if(nb_hy_lack<255)nb_hy_lack++;//˵��û�к���
                        if(nb_hy_lack>10){  //2015-3-2 from 3 to 10
                            VF_HAVEHY_CAS=false;
                            VF_HYER_CAS=true;
                        }
                    }

                    VF_NEWCYCLE_ATMAIN_CAS=true;      //�µ�һȦ��ʼ
                    m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
                    v_t_cas=v_t_cas0;                 //���ݽǱ겶���ʱ��  v_t_cas0���ڽǱ��ж��л�õ�
                    VF_CAST_BESET_CAS=true;
                    
                    
                    //2014-8-18 ˵�������ʱ��Ͳ��ܵ���ˣ�������ܷ�ת��
                    VF_CASHY_BECHECK1_CAS=true; //2015-3-3 ,��������ڵ���ж�����ִ��
                    
                }                
        
                
                v_rev62d5_hyUL=REV62D5_HYST_H;
                
            } else*///20210223 ��ΪCRϵͳ����Ҫ������Ϊ��׼���������һ�����ʿ��ܴ������⣬л��������ε�
            {
                //ֱ��ʹ��ǰ��
                if(!VF_OLDCA_CAS)
                {
                    VF_NEWCYCLE_ATMAIN_CAS=true;//�µ�һȦ��ʼ
                    
                    
					
                    m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
                    v_t_cas=v_t_cas0;           //���ݽǱ겶���ʱ��  v_t_cas0���ڽǱ��ж��л�õ�
                    VF_CAST_BESET_CAS=true;
                }
                v_rev62d5_hyUL=REV62D5_HYST_L;
                
                
            }

    }
    
    ///01C33 IDLE IDLE IGN
    if(VF_NEWCYCLE_ATMAIN_CAS)//2014-5-18 move here
    {
        //2015-3-8 MOVE HERE��֮ǰ�ķ����ᵼ��һȦ�ڶ����λ�����¶�βɼ�����TMP.C���ԣ��ǲ�����ģ�
        VF_TPS_ED_AD=false;
        VF_VBAT_ED_AD=false;

        /*
        if(VF_E0_TPS){//2014-10-8
            if(VF_YSTDC_INJPHASE)VF_OS_ED_AD=false;
        }
        else VF_OS_ED_AD=false;
        */
        
        //2015-5-22
        //���ֵ���OS������ʱ��̫�ԣ���˸�������
        VF_OS_ED_AD=false;
        
        VF_PAIRS_ED_AD=false;
        VF_TES_ED_AD=false;
        VF_TAS_ED_AD=false;

        VF_BEINJ_1CYC_INJPHASE=false;//�������ں�������֮����λ  2014-5-18 DELETE
        
        //V_TEST_OFF    
    }
    
}