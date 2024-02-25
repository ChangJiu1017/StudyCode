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
 *		低电压喷射和启动需要，在Init_beforeWaitingFor1stCA中增加InitVbat_remain()调用.
 * 
 * V9.2.0   2018.4.2 yangsili
 *     1.增加油泵复位初始化
 *     2.增加33814初始化
 *     3.增加油泵时间计算函数
 *     4.增加钥匙关闭后油泵关闭的功能
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
word v_NB_INJDRV;         //停车状态下，喷油器需要工作的次数     //2008-6-27 changgeo to word
word v_preinj_ca;

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
word tw1_wm;
word nb_now;
byte nb_keyonoff_forT1T3;//钥匙连续开关
word t_com_lst;             //2008-7-29 add,上一次通讯的时间，以T_MAINTMR为基准；
byte nb_keyoffloop;         //2009-8-30
word t_delayforkeyofftest_4us;//2009-12-20
//byte HARD_RST_FLG1;
//byte HARD_RST_FLG2;
//byte v_vbat_whenkeyoff; DELETE

byte v_Tmp_bymcu;
//word t_1stDetectKeyoff;     //第一次检测到关钥匙时的时间  t_1stDetectKeyoff=T_MAINTMR;

byte v_rev62d5_hyUL;
byte nb_vbat_times;
//byte VFF_VBATSP_ERR; 放到status_st.h中去定义

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
//功能：复位后的初始化
//      这里的复位是广义的，不仅指单片机自身的复位；
//      而且还指每次角标从有到消失时，程序从头再来的过程；
void Init_afterReset(void)
{
    
    CLR_WDG;
    
    //==============================2008-10-30 add for 初始化================
    //#ifndef SYS_TRADITIONAL
	FG_SysInit_AfterReset();//2008-11-2 add
	//#endif
    //======================================================================= 
    
    FI_TMCCL_AfterReset(false);//              2008-7-5 add  //2015-3-17 注意顺序，因为对后面会有影响
    
    FI_SCI_AfterReset();
    Init_EE_afterReset();
    Init_Int_afterReset();              //
    FI_DTC_AfterReset();                         //因为 Init_InjPara_afterReset有进行诊断，因此Init_DTC_afterReset必需在Init_InjPara_afterReset之前
    CLR_WDG;          //2008-3-9 zhangping add ; 
    
    //PWR_ON;//2012-11-19 ADD; //2014-8-13 DELETE
    //PWR_OFF;//2013-3-23
    //2014-4-7 ADD 
    F_Terminate_Inj();//2012-11-20 发电机上发现一例烧喷嘴的，原因不明，临时增加
    F_Terminate_Ign();  //add by zhangping 
    
    Init_InjPara_afterReset();          //后面需要跟上Check_TW1SCI_afterRDorWR，因为这里可能更改通讯标志位；//-call	tv3_idle_cac;//计算得到怠速流量对应的T3:tv3_idle 移到Init_InjPara_afterReset中  
    //Check_TW1SCI_afterRDorWR();       //2008-12-16 暂时删除，因为stop循环中有
   
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
    FI_status_AfterReset();     //2008-10-30 硬件复位后，对状态的初始化

    Init_StopStatus_afterReset();
    FI_Trans_AfterReset();      //2009-1-13 
    //FI_IdleAQ_BeforeStarts();   //2009-4-22 move to afterreset
    FI_IdleAQ_AfterReset();   //2009-8-25 add , devide from  FI_IdleAQ_BeforeStarts
    FI_Ign_BeforeStarts();      //2009-7-9 move here
    FI_OS_Reset();              //2009-12-12
    
    FI_Altitude_AfterReset();   //2010-4-30
    
    Init_FilmAfterReset();      //2012-8-9

    //2013-4-23 MOVE FROM STARTS TO RESET
    FI_CI();//CALL Init_CFCI_BeforeStartRun									;//发动机开始运行前，CFCI策略初始化	

//    #if ECU_TYPE==ECU_GEN
   /*
    #ifdef IS_GEN
    
        //2014-4-27 对ECT控制的初始化
    #endif
    */ //2016-9-10 去除这个条件
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
  
  	//#ifndef	SY_HUANSONG		//20211010 chenwei add 此处是否可以不区分环松，初始化均关掉氧加热，更合适
    	P_O2H_OFF
    	VF_O2H_STATUS=false;
   // #endif
	
  #endif
  
    pump_PumpInit_init(); //2018-04-16,ysl,add
	
	Init_Act();
    
    //2014-10-14 MOVE HERE	      //2014-4-17
    v_rev62d5_hyUL=REV62D5_HYST_L;

	FI_EngProtection_AfterReset();    //20210112 chenwei add 发动机保护功能相关初始化
	
	#ifdef SY_CCPS
		FI_CCPS_afterReset();  //20201126 chenwei add 碳罐电磁阀
	#endif
	
	if(VF_VPOR_HARD||VF_POR_HARD)		//20211009 chenwei add开钥匙是存储记录的
	{
		v_pair_ad_input_keyon=0;
		v_Vbat_inuse_keyon=0;
		VF_CA_NOSIGNAL=false;
	}

}

/*=================================================*/
void Init_beforeStarts()
{
    
    //--------以下开始进入准备喷油、点火的阶段--------------------------------------------------------

    Init_NotYSTDC_beforeStarts();////Call Set_Addtime_Before_StartRun;							;//发动机开始运行前，设置加喷次数
    FI_Starts_BeforeStarts(V_Tw,V_Ta);//call Init_STARTS_Before_StartRun	;//发动机开始运行前，启动增油初始化		
    FI_OS_BeforStarts();//call Init_O2_Before_StartRun;									;//发动机开始运行前，O2状态初始化

    Init_InjPara_beforeStarts();//call Init_T3FK_BeforeStartRun									;//发动机开始运行前，T3反馈策略初始化
    FI_Trans_BeforeStarts();//call Init_TRANS_BeforeStartRun									;//发动机开始运行前，过渡策略初始化	

    FI_TimeCCL_BeforeStarts();  						;//发动机开始运行前，初始化时间和圈数
    FI_status_ENG_BeforeStarts();//CALL Init_RUNSTATUS_BeforeStartRun	;//发动机开始运行前，运转状态初始化	

    //FI_Ign_BeforeStarts();//CALL SET_IGNCA_AFTER_2CACATCHED			  				;//设置发动机刚运行起来时的点火角；
    FI_SysInit_BeforeStarts();//2008-11-2;

    FI_IdleAQ_BeforeStarts();   //2009-4-22 move to afterreset   //2009-8-25 RESUME
    FI_IdleMissfire_BeforeStarts();
    FI_Ign_afterReset();//2009-8-7 add 

    
    FI_TMP_beforeStart();//2012-8-2
    
    Init_FilmBeforeStarts();//2012-8-9
	
	#ifdef SYS_O2HEATER_S
	
		#ifndef	SY_HUANSONG		//20211010 chenwei 区分是否是环松，非环松则用常规策略
   			P_O2H_ON  //打开氧加热	
   			VF_O2H_STATUS=true;		//20211012 打开氧加热的标志
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
/*---------功能：当发动机静止时的通讯操作，包括EE的读写---------------------------------*/
void Com_whenStop(void)
{
    
    if(VF_DOM_SYS&&v_NB_INJDRV!=0)
    {
        if(nb_now!=v_NB_INJDRV&&VF_T3GET_INTINJ)
        {
            nb_now=v_NB_INJDRV;
            VF_T3GET_INTINJ=false;
            VF_AFRAME_NEED_INTSCI=true;//TOPC();	//TOBECHECK，为什么原来刘老师要用两个TOPC    	//为PC准备数据，并发送 
        }
    }
    else
    {
        v_PCSET_SCI_TOCOM=v_PCSET_SCI;
        v_PCKESE_SCI_TOCOM=v_PCKESE_SCI;
        v_PCBTDC_SCI_TOCOM=v_PCBTDC_SCI;
        v_PCTBACK_SCI_TOCOM=v_PCTBACK_SCI;
        
        FS_ti_pc(v_PCKESE_SCI_TOCOM,REMARK,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,vc_tw1_IDLESC,v_FMF_SCI_idle,vc_tw3offset);

        write_eeprom(v_PCKESE_SCI_TOCOM,v_PCSET_SCI_TOCOM,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,PCCQJSJD);//+call	write_eeprom//判断条件合适时，将参数写入EE
        
       	if(VF_JBEWR_EE){
     	        v_PCBTDC_SCI=0;
              v_PCTBACK_SCI=0;
              v_PCKESE_SCI=20;
       	}


      	//2008-7-13 更改为
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
      	{   //2008-7-13 当喷油器工作时,每获取一个T3通讯一次;
           // if(nb_now!=v_NB_INJDRV&&VF_T3GET_INTINJ)  //VF_T4GET_INTINJ
            if(nb_now!=v_NB_INJDRV)  //VF_T4GET_INTINJ
            {
                nb_now=v_NB_INJDRV;
                VF_AFRAME_NEED_INTSCI=true;//TOPC();	//TOBECHECK，为什么原来刘老师要用两个TOPC    	//为PC准备数据，并发送 
            }
      	}
    }
    
}                 

/*=================================================*/
//功能：当发动机静止时，准备喷油操作，包括预喷，泵油，T35测试等
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
	
    if(VF_INJENDWHENSTOP_STOP)              //当T35测试结束，进入此处
    { 
          //return;
          VF_T35TEST_HARD=false;           //清除T35测试标识
          //if(nb_t3_whenstop<=1) 
          VF_T3NOTEXIST_INJCTL=false;    //2009-12-18  //2012-10-28 临时
          if(!VF_PREPUMPEND_SYSINI)	    //进入此处，开始泵油，泵油次数与上次发动机运行状态有关
          {	
          /*#ifndef SYS_TRADITIONAL  
			  tw1_wm=Set_TW1_forPump(v_Vbat_inuse);  //设定泵油T1参数   4000    
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
              
          	  Trig_Inj_whenStop(tw1_wm);               //启动喷油器动作
              */
              //2020-5-25 如果不对以下这几个赋值为false，串口通信无法读写map数据
			  //VF_PUMPING_SYSINI=true;     //这个不影响
			  VF_PUMPING_SYSINI=false;
			  VF_PUMP_INJCTL=false;
			  VF_INJENDWHENSTOP_STOP=false;


              //VF_PUMP_INJCTL=true;   //有这个不行
              F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_PUMP);//有这个不行
              

               
                                        //设置泵油开始标识
          } 
		  /*#ifndef SYS_TRADITIONAL
          else if(!VF_NOTNEED_DROPSCAN_SYSINI&&!VFF_VBAT_DTC&&(VF_RUNENOUGH_FORINI_SYSINI))//2008-6-25      //2010-3-26 add VFF_VBAT_DTC
          {   //开始T1T3特性测试
              //2008-12-1 add 用于保证，当一组T1扫描之后，如果再需要进行扫描时 仍然可以扫描；B_T1_Scaning_Needed()
              //if(v_teng_inuse<=M0_TES_ULIMIT_FOR_INI)     //2008-6-29 add condiction
              {
                  Init_T1Scaning(v_Vbat_inuse,true);   //对T1扫描进行初始化；
                  v_NB_INJDRV=Get_NB_T1SCANING();//
                  nb_now=v_NB_INJDRV;
                  Trig_Inj_whenStop(tw1_wm);
                  FI_AfterInjAction_SCI_AfterReset();//2008-6-25 add 
              }

          }
		  #endif*/
          else if(!VF_PREINJEND_SYSINI)	    	//进入此处，开始预喷，一组预喷的次数与温度RZT、钥匙状态、上次发动机运行状态有关
          {
            /*#ifndef SYS_TRADITIONAL    
              //需要计算y=aln(x)+b的系数；
              tw1_wm=Set_TW1_byDT3mode(FMF_PreInj(),v_Vbat_inuse);//2008-11-27 merge into one;   
              //tw1_wm=300;  
              v_NB_INJDRV=NB_Preinj(V_Tw,V_Ta); //SET_TIMES_FOR_PREINJ的返回值；
              
              //if(V_Tw>20)v_NB_INJDRV=0;//2016-4-27 for preinj
              if(V_Tw>10)v_NB_INJDRV=0;//2016-4-28
            #else			   
              tw1_wm=0;
			  v_NB_INJDRV=0;
			#endif*/
			
			  v_NB_INJDRV=NB_Preinj(V_Tw,V_Ta);//2
			  tw1_wm=625;//2020-5-26 2.5ms 怠速通常是2.5~3
			  
			  nb_now=v_NB_INJDRV;//2008-7-13 add
          	  if(v_NB_INJDRV>0)Trig_Inj_whenStop(tw1_wm);            //启动喷油器动作
              F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_PREINJ);      //设置预喷结束标识
              //此时将remark切换到正常状态，之前，都为1
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
                    //2014-11-28 add condiciton,要求T1满足一定范围
                      
                        tw1_wm=v_fmf_O;////2014-8-10 vc_tw1_IDLESC+v_FMF_SCI_idle+vc_tw3offset;//v_FMF_SCI_idle;//+1024;//2009-3-10 ADD +1024
                    
                        VF_DOM_SYS=true; //2008-7-13 add
                        v_NB_INJDRV=26;
                        nb_now=v_NB_INJDRV;
                        Trig_Inj_whenStop(tw1_wm);    //启动喷油动作;
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
    {   //2008-12-16 移动了v_NB_INJDRV--在喷油触发中的位置，以保证VF_INJENDWHENSTOP_STOP是最后一次驱动的T4都获得之后才置位
        //2008-12-16 也更改了INJ_AFTER_SOMETIME的含义；
        //      原来INJ_AFTER_SOMETIME是指，固定时间后触发T1；
        //      现在更改为跟正常喷射一样，固定时间后触发“触发T1的前0.n”
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
//功能：当发动机静止时触发喷油动作
//形参：teng_inuse－－发动机温度，作为是否喷射的一个依据；
//      t1－－需要触发的喷油脉冲宽度
void Trig_Inj_whenStop(word t1)    
{

    //2010-3-14 chang as following:
    VF_INJENDWHENSTOP_STOP=false;
    FP_Inj_WhenTMROver(4,t1);

    return;
}

/*=================================================*/
//功能：
//  钥匙关闭后，使用备用电源工作的情况下，发生的动作
void Action_whenKeyOff(void)
{
    
    CLR_WDG;


    F_Terminate_Inj();//2012-11-20 发电机上发现一例烧喷嘴的，原因不明，临时增加
    F_Terminate_Ign();  //add by zhangping 
    P_PUMPC_OFF     //2018-4-16,ysl,关闭油泵
	P_O2H_OFF

   // #if ECU_TYPE==ECU_GEN
    #ifdef IS_GEN
    
        //2014-4-27 发电机在关钥匙后短时间即切断电源
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
				Fset_SM(255);//2020-12-18 关钥匙后进行学零操作
				VF_LEARZRO_KEYOFF=true;
			}
        }//
        else{
            PWR_OFF;

        }//

         
    #endif
    /*----------------------------------------------------------------------*/
    //关闭要是后的处理工作在此进行

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
//复位之后，对stopstatus的状态进行初始化
void Init_StopStatus_afterReset(void)
{
    VF_INJENDWHENSTOP_STOP=true;          //设置为"当前处于不喷油的状态"
}

/*=================================================*/
//功能：
//  复位之后，等待第一个角标之前的初始化
void Init_beforeWaitingFor1stCA(void)
{
    CLR_WDG;          //2008-3-9 zhangping add ; 

    //Samp_All_ADC_whenStop();		    //采集所有模拟信号，其中包括TPS4次，其它信号各1次
    //－－－触发T35动作，具体是否成功需要在过程Trig_Inj_whenStop里进行判断
    //tw1_wm=Set_TW1_forT35Get();    
    tw1_wm=0;
	//v_NB_INJDRV=Set_NB_forT35Get();  //2008-3-6遇到返回值赋值有问题，需要解决TOBECHECK
	v_NB_INJDRV=0;

    nb_now=v_NB_INJDRV;
    
    //Trig_Inj_whenStop(tw1_wm);    //启动喷油动作;当VF_ALLOWINJAFTERRESET_INJ=false时，不能启动T35测试
	InitVbat_remain();
	
	v_preinj_ca=750;//3ms
}

//
    
/*=================================================*/
//功能：
//第一个角标到来之前的等待
void Action_WhenStop(void)
{
    
  //停机状态下的动作
      //未解锁时，对REMARK做限制；
      if(!VF_KEYON_LOCK){
          if(KESEI<20){
              REMARKW=MC_LOCK;    
          }
      }
    	Com_whenStop();
      InjAction_whenStop();
      FC_IdleSC_FMF(0,vc_tw1_IDLESC_inuse,vc_tw1_IDLESC,v_FMF_SCI_idle,v_FMF_SCI_Nidle,vc_tw3offset); 
      FC_F();//call Init_STARTS_Before_StartRun	;//发动机开始运行前，启动增油初始化		
	

	  v_preinj_ca=(byte)F_2DLookUpSbyte((sbyte)v_te,(char_P)BYPTR_M1_PREINJ,3);
	  //if(v_preinj_ca>30)v_preinj_ca=30;	//20211009 chenwei 放开至100
	 if(v_preinj_ca>100)v_preinj_ca=100;
	 
	  if(v_preinj_ca<2)v_preinj_ca=2;
	  v_preinj_ca=v_preinj_ca*250;//ms--4us
	  
      if(VF_INJENDWHENSTOP_STOP)OperLock();//2013-6-14  //2013-7-26 add condiction VF_INJENDWHENSTOP_STOP
      
      //2013-3-4 
      //清楚自学习值
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
      
      //2014-5-9 ADD 激活
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
//功能：
//  第一个角标到来之后的初始化
void Init_after1stCACome(void)
{
    if(!VF_KEYON_HARD)return;    //2009-1-15 add 
    VF_INJENDWHENSTOP_STOP=true;
    
    F_FinishSysInitAction_whenStop(ACTION_WHENSTOP_ALL );
    
    VF_3RDCA_STOP=false;
    VF_STARTFROMHIGHSPEED_STOP=false;//2009-12-25

    //VF_OLDCA_CAS=true;				                //设置角标为老角标,2014-4-17 DELETE THIS LINE
    //VF_OLDCAHY_CAS=true;                      ,2014-4-17 DELETE THIS LINE
    VF_T35TEST_HARD=false;				                    	//清除T35测试标识
    VF_VPOR_HARD=false;
   // VF_POR_HARD=false;
    
    v_NB_INJDRV=0;                                  //清除发动机静止状态时的喷油次数
    VF_INJENDWHENSTOP_STOP=true;

}


byte F_act_AfterKeyoff(void)
{
    
    VF_KEYON_HARD=false;

    VF_MMOVE_ENG=false;//2013-3-1
    
    #ifndef SYS_TRADITIONAL
		SaveGama_AfterKeyoff();//2010-3-6 add   //应该在关钥匙之后第一个执行，因为其执行结果v_gama_to_cls需要用来修正闭环自学习值；
    #endif
	
    F_Terminate_Ign();  //add by zhangping 
    F_Terminate_Inj();  //call	TERMINATE_INJECTION;        
    //2009-7-31 change to the following
    //2009-9-11 怠速初始化成功，并结束之后
    if(VF_AQOK_SYSINI)  //怠速调试成功后
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
    F_Terminate_Inj();//2012-11-20 发电机上发现一例烧喷嘴的，原因不明，临时增加
    F_Terminate_Ign();  //add by zhangping 

    VF_RUNTOOS_ENG=true;
    //VF_POR_HARD=false;                  //2009-3-15
    if(!VF_STARTS_ENG)VF_RUNTOOS_ENG=false;     //当STARTS阶段停车，置Brun_tooshort标志；
}

/*=============================================================*/
//状态之间切换，进行的动作
/*=============================================================*/
void Shift_Status(void){
      
      if(VF_JKEYON_HARD)
      {
          mc33814_Mc33814Init_init(true);//2018-4-16,ysl,33814初始化最好放在  PWR_ON 之前，PWR_ON是表示开启延时关闭CPU电源的语句
          PWR_ON;//2014-4-15在这里把电源打开，
          mc33814_WriteVRSCrankingParam(0xC0);
		  //E0:1.2     B0:425mv	 70:110mv	  D0:850mv     C0：600mv
          
          SCI0CR2_RIE(DISABLE);
          
          VF_MAY_UPDATE_FLASH=false;
          if(nb_keyon_forIni==0){
              //2014-9-3 增加条件，即快速开钥匙时，不等待
              if(0/*Check_UpdateCmd1()*/)
              {
                  
                  VF_MAY_UPDATE_FLASH=true;//2014-8-13 add  
                 // _asm jmp _EntryPoint
                             
              } 
              
          }
          
          SCI0CR2_RIE(ENABLE);
          
          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          FI_status_AfterReset();               //2012-8-2 add   保证钥匙关闭时，为停机状态；
          //MIL_OFF;        

          
          //钥匙刚开启时，需要进行的动作
          //这以下的代码需要移动到合适的位置
          FI_Hard_AfterReset();   //这个必须紧跟其后
          

          Init_afterReset();      //钥匙开启后的初始化过程
  
          FI_CAS_WhenCANotCaught();   //钥匙开启后，角标的准备工作

          //在这里测量一次电压
          Samp_Save_Vbat(); //注意在测量电压之前，一定要保证PWR_ON了；
          Samp_Save_Vbat(); //注意在测量电压之前，一定要保证PWR_ON了；
          Samp_Save_Vbat(); //注意在测量电压之前，一定要保证PWR_ON了；
          Samp_Save_Vbat(); //注意在测量电压之前，一定要保证PWR_ON了；
          
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
      {   //停机时需要马上进行的动作
          VF_MMOVE_ENG=false;//2015-12-8 解决通机初始化不停机的问题，at vf_jstop_eng
          F_act_AfterStop();              
          
          VF_NEWCYCLE_ATMAIN_CAS=false;

          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          FI_status_AfterReset();               //2012-8-2 add   保证钥匙关闭时，为停机状态；

          Init_afterReset();      //钥匙开启后的初始化过程
          FI_CAS_WhenCANotCaught();   //钥匙开启后，角标的准备工作

          Clr_t1s_WhenNewState();//2016-3-3 
          
          //v_t_eng_1s=0;   
      }
      
      
      if(VF_JKEYOFF_HARD)
      {
          FI_TMCCL_AfterReset(true);            //              2008-8-7 add  ,FOR CALCULATION WHEN KEY OFF
          F_act_AfterKeyoff();
          FI_CAS_WhenCANotCaught();   //钥匙开启后，角标的准备工作
          
          VF_MMOVE_ENG=false;//2014-8-13 add
          
          Frame_action_whenKeyoff();//2016-2-27 
          Clr_t1s_WhenNewState();//2016-3-3
          
          //v_t_eng_1s=0;  
          
          ResetTwParaWhenKeyonOroff();//2016-4-4   
		  
		 // Fset_SM(255);//2020-8-17 关钥匙后进行学零操作
  
      }
      


      VF_JMMOVE_ENG=false;
      VF_JSTOP_ENG=false;
      
      VF_JKEYON_HARD=false;
      VF_JKEYOFF_HARD=false;
      
      VF_POR_HARD=false; 
      VF_VPOR_HARD=false; //2014-4-10 ADD          

}

/*=============================================================*/
//校验钥匙的状态
//需要考虑电压的因素
/*=============================================================*/
void Check_KeyStatus(void)
{
    byte b;
    word w;

    //2016-7-31 MOVE HERE
    if(!VF_JKEYOFF_HARD||VF_JKEYON_HARD)
    {
        FC_KeyStatus();//判断钥匙开启或关闭后的时间，及相关的状态       2016-7-31 把这个移走，不能放这里，否则关钥匙时判断不对，
    }
    //
    if(VF_KEYON_HARD){
        //如果原来为钥匙开启状态
        if(!IS_KEY_OFF){
            //记录最近一次钥匙开启的时间
            t_1stDetectKeyoff=T_MAINTMR;
            
        } else{
            //发现钥匙开始有关闭迹象
            w=T_MAINTMR-t_1stDetectKeyoff;

            if(v_Vbat_inuse>20){
                //如果保险丝好的
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
        //如果原来为钥匙关闭状态  
        b=0;
        if(!IS_KEY_OFF){
            for(b=0;b<=10;b++)
            {
                
                if(!IS_KEY_OFF)
                {
                    
                    //2013-10-12 开钥匙，需要判断稳定一定时间后，再进行一下其他工作
                    w=T_MAINTMR;
                    while((word)(T_MAINTMR-w)<250){ //延时1ms
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
    //FC_KeyStatus();//判断钥匙开启或关闭后的时间，及相关的状态       2016-7-31 把这个移走，不能放这里，否则关钥匙时判断不对，
    
}

//初始化超时标识
void FI_TimeOutFlag(void)
{
    VF_PRDMAINTMR_TMR=false;
    VF_SAMPATIME_STOP=true;
    VF_PCAOV_CAS=false;
    VF_1COM_STOP=false;
    
}

//功能：计算超时时间基准
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
    
        //对时间的处理
    if(VF_MMOVE_ENG)
	{
        //V_TMCCL_STATUS&=0x80;  清0后不计时了？
		if(VF_NEWCYCLE_ATMAIN_CAS)
		{
			FF_T(v_prd_cas,false); 
		}
		//else
		//{
			//FF_T(0,true); 清0后不计时了？
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

    //停车期间，采样的时间标识
    if(t_acc_StopAD>TL_FOR_STOPAD)
    {
        if(!VF_MMOVE_ENG)VF_SAMPATIME_STOP=true;
        FCLEAR_ADATSTOP    
    }
    
    //指示灯驱动的时间
    if(t_acc_MIL>TL_FOR_MIL)
    {
        VF_PRDMAINTMR_TMR=true;
        FCLEAR_MIL    
    }
    
    //角标溢出的时间标识
    if(t_acc_forCAOV>TL_FOR_PCAOV)
    {
        
        VF_PCAOV_CAS=true;
    }
    
    //停车时通讯的时间标识 
    if(t_acc_StopCom>TL_FOR_STOPCOM)
    {
        if(!VF_MMOVE_ENG)VF_1COM_STOP=true;
        FCLEAR_COMATSTOP
    }
    
    
    //2015-10-30 需要增加10ms的
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


//设置用于触发点火的角标
//void FS_CA_st(void)
void FS_CA_st(sbyte ignCa)
{

    CLR_WDG;

   
    VF_NEWCYCLE_ATMAIN_CAS=false;
    

    //需要区分是以前沿为基准，还是后沿为基准
    if(!VF_OLDCA_CAS){
        VF_CAST_BESET_CAS=false;
        VF_CAHYPREDICT_TIMEIN_CAS=false;//2015-3-2 发现VF_CAHYPREDICT_TIMEIN_CAS与VF_OLDCA_CAS之间为自锁的自相矛盾的关系；
        
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
        
        VF_CASHY_BECHECK1_CAS=false;//2015-3-3 等不到后沿的情况下，不能点火
        
        //VF_BEINJ_1CYC_INJPHASE=false;//这个最好在后沿来了之后置位  2014-5-18 DELETE
        //V_AD_STATUS=false;
    }

    /*
            //直接使用前沿
        //if(!VF_OLDCA_CAS)
        {
            m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
            v_t_cas=v_t_cas0;           //备份角标捕获的时刻  v_t_cas0是在角标中断中获得的
            VF_CAST_BESET_CAS=true;
        }
        v_rev62d5_hyUL=REV62D5_HYST_L;


     return;
    */
     
    //如果用角标预测的CAHY应该到了
    if(!VF_CAST_BESET_CAS){

            //2015-3-2 MOVE HERE
            if(!VF_CAHYPREDICT_TIMEIN_CAS) {
                //if((word)(T_MAINTMR-v_t_cas0)>v_prd_cas/4)
                if((word)(T_MAINTMR-v_t_cas0)>v_prd_cas/3) //2015-3-3 保险起见，取1/3；
                {
                    v_t_CAHY_timeout=T_MAINTMR;
                      
                    VF_CAHYPREDICT_TIMEIN_CAS=true;
                    //VF_CAHYPREDICT_TIMEIN_CAS=false;
                    //deDTC++;

                }

            }


            //如果发动机转速低
         /*   if(
                (!VF_HYER_CAS)
                &&
                //(v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||((VF_E0_TPS||ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&v_rev_625rpm<v_rev62d5_hyUL))
                //2015-3-2 避免非前沿点火时太早
                //(v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||((ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&(v_rev_625rpm<v_rev62d5_hyUL||VF_E0_TPS))) //会导致怠速时点火角切换时，引起前后沿切换
                (v_rev_625rpm<REV62D5_1000||!VF_BFIRE_ENG||(VF_E0_TPS&&!VF_IGNONLY_CFCI)||((ignCa<=BYPTR_M0_CAS_TRAILEDGE)&&(v_rev_625rpm<v_rev62d5_hyUL||VF_E0_TPS)))
              ) 
              {
                
                //
                if(!VF_OLDCAHY_CAS) {
                    VF_NEWCYCLE_ATMAIN_CAS=true;//新的一圈开始
                    v_t_cas=v_t_cashy0;//采用中断得到的后沿；
                		m_nbtdc0=BYPTR_M0_CAS_TRAILEDGE;
                		VF_CAST_BESET_CAS=true;
                		
                }
                else if(VF_CAHYPREDICT_TIMEIN_CAS)
                {

                    
                    if(VF_OLDCAHY_CAS){
                        //如果新的后沿还没有来
                        if(nb_hy_lack<255)nb_hy_lack++;//说明没有后沿
                        if(nb_hy_lack>10){  //2015-3-2 from 3 to 10
                            VF_HAVEHY_CAS=false;
                            VF_HYER_CAS=true;
                        }
                    }

                    VF_NEWCYCLE_ATMAIN_CAS=true;      //新的一圈开始
                    m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
                    v_t_cas=v_t_cas0;                 //备份角标捕获的时刻  v_t_cas0是在角标中断中获得的
                    VF_CAST_BESET_CAS=true;
                    
                    
                    //2014-8-18 说明：这个时候就不能点火了，否则可能反转：
                    VF_CASHY_BECHECK1_CAS=true; //2015-3-3 ,这个动作在点火中断中来执行
                    
                }                
        
                
                v_rev62d5_hyUL=REV62D5_HYST_H;
                
            } else*///20210223 因为CR系统不需要后沿做为基准，如果保留一定概率可能存在问题，谢总提出屏蔽掉
            {
                //直接使用前沿
                if(!VF_OLDCA_CAS)
                {
                    VF_NEWCYCLE_ATMAIN_CAS=true;//新的一圈开始
                    
                    
					
                    m_nbtdc0=BYPTR_M0_CAS_FRONTEDGE;  //M0_NBTDCO_USED;
                    v_t_cas=v_t_cas0;           //备份角标捕获的时刻  v_t_cas0是在角标中断中获得的
                    VF_CAST_BESET_CAS=true;
                }
                v_rev62d5_hyUL=REV62D5_HYST_L;
                
                
            }

    }
    
    ///01C33 IDLE IDLE IGN
    if(VF_NEWCYCLE_ATMAIN_CAS)//2014-5-18 move here
    {
        //2015-3-8 MOVE HERE，之前的方法会导致一圈内多次置位，导致多次采集，对TMP.C而言，是不允许的；
        VF_TPS_ED_AD=false;
        VF_VBAT_ED_AD=false;

        /*
        if(VF_E0_TPS){//2014-10-8
            if(VF_YSTDC_INJPHASE)VF_OS_ED_AD=false;
        }
        else VF_OS_ED_AD=false;
        */
        
        //2015-5-22
        //发现怠速OS采样有时不太对，因此更改条件
        VF_OS_ED_AD=false;
        
        VF_PAIRS_ED_AD=false;
        VF_TES_ED_AD=false;
        VF_TAS_ED_AD=false;

        VF_BEINJ_1CYC_INJPHASE=false;//这个最好在后沿来了之后置位  2014-5-18 DELETE
        
        //V_TEST_OFF    
    }
    
}