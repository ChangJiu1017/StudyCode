/** ###################################################################
**     Filename  : GENERATOR.c
**     Project   : GENERATOR
**     Processor : MC9S12G128CLF
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2013-11-22, 9:35
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
* Version	:V9.1.0
*
*** History ***
* V9.1.0	2017.12.5	yangsili 
*    1.增加油泵初始化
*    2.增加 33814状态变量初始化
*    3.增加油泵控制、诊断函数
*    4.增加33814寄存器外部工具读写
** ###################################################################*/
/* MODULE GENERATOR */
#include "FAM.h"
#include "Service_CAN.h"
/* Including needed modules to compile this module/procedure */
#include "events.h"

//#include "hard_trig.h"

/* Include shared modules, which are used for whole project */

#include "FAM.h"
#include "MCU.H"

#include "status_st.h"
////#include "events.h"
#include "sci.h"
#include "ee.h"

#include "ld.h"

/*----------------*/
#include "Public.h"
//#include "Status_st.h"

#include "wait_moving.h"

//#include "int_serve.h"
#include "ee.h"

//
#include "dtc.h"//　
#include "ign.h"//

#include "ad.h"
#include "inj_phase.h"

#include "time_ccl.h"

#include "look_up.h"

#include "inj_notystdc.h"
#include "inj_ctl.h"

#include "status_eng.h"

#include "sci.h"

#include "sys_init.h"

#include "stepmotor.h"

#include "data_inj_source.h"

#include "add_ee_mpi.h"
#include "add_flash_mpi.h"
#include "add_ram_MPI.h"

//#include "s12sci.h"

#include "frame.h"

#include "pump.h" //2018-4-2,ysl,add
#include "app_rw33814.h"  //2018-4-2,ysl,add
#include "act.h"
#include "CCPS.h"       //20201127 chenwei add 碳罐电磁阀


/* User includes (#include below this line is not maintained by Processor Expert) */
void Shift_Status(void);
void Action_ForWaitingMove(void);   //2014-4-2
void Can_Busoff_Reset(void);			//20210805 chenwei add 优化CANH与CANL短接之后，can-dataview无通讯的情况

word v_t_runtime;
word v_t_runpoint;

word v_nb_rst;
word v_nb_JSTOP;
word v_nb_JMMOVE;

word t_IACVLstT;
//extern byte TachOff_Flag;
extern byte F_ChargeEn;
extern byte F_Trig_IgnEn;
extern byte F_Trig_IgnEn2;
extern word Trig_IgnTmeptime;
void LdActionForTrigT1(void); //2015-5-19
byte charReadIIC;
#pragma CODE_SEG USER_MAIN 
/*----------------------------------------------------------*/
void ECU_main(void)   //2014-7-17
{
  /* Write your local variable definition here */
  word numbr=0;
  word w;
  byte return_inj;
  ch_select=0;
  
  /*
  while(1)
  {
      BOOT_SCI_Sent_Byte(0xAA);
  } */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  //上电后的初始化过程
      //PE_low_level_init();

  /*从这里开始到dowhile循环耗时2ms*/
  pump_PumpInit_init(); //2018-04-03,ysl,在这里放这个函数，可以尽快开启油泵不延时

  /*** End of Processor Expert internal initialization.  ***/
  //    (void)EE_ERAZ_ALL(30);
  //    (void)EE_PROM_ALL(zzq_ee,30);

  //2014-9-3 ADD 
  F_Terminate_Ign();
  F_Terminate_Inj();

  VF_JMMOVE_ENG=false;
  VF_JSTOP_ENG=false;
  VF_JKEYON_HARD=false;
  VF_JKEYOFF_HARD=false;           
  VF_KEYON_HARD=false;

  VF_CAMAIN_TIMEOUT_CAS=false;
  VF_CADIAG_TIMEIN_CAS=false;
  VF_CAHYPREDICT_TIMEIN_CAS=false;
  
  VF_POR_HARD=true;//2014-4-5 ;很奇怪，有时这里没有这一行也可以的
  
    F_ChargeEn = 0;
	F_Trig_IgnEn = 1;		//20210930 此处应该初始化F_Trig_IgnEn=1，原先初始化成了F_Trig_IgnEn=0
	Trig_IgnTmeptime = 0;
	F_Trig_IgnEn2=0;
   
  V_33814_STATUS = 0U; //2017-12-27,ysl,add   
  V_33814INJ1CTL_STATUS = 0U; //2017-12-27,ysl,add    
  V_33814O2HCTL_STATUS = 0U;  //2017-12-27,ysl,add    
  V_33814PUMPCTL_STATUS = 0U; //2017-12-27,ysl,add  
   
  /*
  v_nb_rst=Load_Data_EE(EE_COUNT_FAIL1);v_nb_rst++;Save_Data_EE(EE_COUNT_FAIL1,v_nb_rst);

  nb_clk_er=Load_Data_EE(EE_CLKMON_ER);
  
  nb_PLL_er=Load_Data_EE(EE_PLL_ER);
  */

  FI_TimeOutFlag();
  
  //ch_select=IFsh1_EraseSector(ADD_FLASH_BLOCK(0)-0X68000UL);//2014-5-10 

  /*----------------------------------上电后的主循环----------------------------------*/
  do{  //主循环在缺齿后4ms周期,400us,还有1.5ms
      //BOOT_SCI_Sent_Byte(0xAA);
 
      /*---------判断钥匙和角标的状态，及其变化------------------*/
	      Can_Busoff_Reset();
          Check_KeyStatus();    //校验钥匙的状态
          Shift_Status();       //状态变化，例如钥匙是否开启，发动机是否起停，后需要执行的动作
          FC_TimeOutFlag();     //计算一些时间标志
         
          CLR_WDG//2014-4-13
          /*以上状态及时间标志耗时约30us*/
                            
      /*-------------------------AD采样的处理------------------*/
       //耗时5us
          //if(VF_SAMPATIME_STOP||VF_NEWCYCLE_ATMAIN_CAS)
          if(VF_SAMPATIME_STOP||VF_MMOVE_ENG)//2014-4-28
          {
              if(!VF_MMOVE_ENG)
              {
                  V_AD_STATUS=false; 
                  //2015-3-17 增加条件，如果开钥匙时间不够长，不采样
                 /* if(v_t_eng_d1s<10)
                  {   //开钥匙的1S内不采集PAIR
                      VF_PAIRS_ED_AD=true;    
                  }*/
                  
                     
              }
              //如果新角标来了，或者时间到了，就全部采样
              FAD_ALL((byte)REMARK);  //AD采样，即主循环中进行的，不采用中断； 电流采样采用其他方法；
              if(VF_NEWCYCLE_ATMAIN_CAS||VF_SAMPATIME_STOP)FS_TPSlst();//2014-4-6

          }
          
         // DC_con(KESEI,REMARK);



          //#if ECU_TYPE==ECU_GEN 
          #ifdef IS_GEN
              
              //if((VF_SAMPATIME_STOP&&!VF_MMOVE_ENG)||(VF_MMOVE_ENG&&))
              if(VF_SAMPATIME_STOP||(VF_PRDMAINTMR_TMR&&VF_MMOVE_ENG)) //2016-1-11 add condiction
              {
                  FS_ECT_IdlePosition(v_TPS,v_TPS_odr,v_teng_inuse); //如果发动机未着火，
              }
          #endif
          
      /*---------以上：@不管钥匙是否开启，只要ECU上电后的公共区域------------------*/

      if(!VF_KEYON_HARD){
          /*----------------如果钥匙处于关闭状态，等待中-----------------------*/
          Action_whenKeyOff();    //在这里等到钥匙开启为止
         
          //#if (ECU_TYPE==ECU_GEN )
          #ifdef IS_GEN
          
              //2014-4-27 控制直流电机     
              FS_DCMotor( );//2012-11-3
          #else//2016-9-7    
              #ifdef SYS_STEPMOTOR//2017-11-30
//			  FS_SMotor_IACV();//2016-9-7    
              #endif
          #endif

      } else{
          /*---------------油泵控制 2018-4-2,ysl,add----------------*/
          // 诊断耗时5us
          pump_PumpControl_maincycle();
          
          /*----------------诊断部分放这里，因为需要钥匙开启--------------------*/
          //诊断角标
          if(!VF_CA_BEDIAGED_CAS&&VF_CADIAG_TIMEIN_CAS){
                F_Diag_CA(nb_hy_betweenqy, nb_qy_betweenhy, v_t_cas0,v_t_cashy0,v_prd_cas,v_prd_cas_hy);//2008-8-1 add
                VF_CA_BEDIAGED_CAS=true;
          }

          //各部件诊断 2018-1-11,ysl
          DTC_Diag_def(v_prd_cas);
          
           
          //VF_PRDMAINTMR_TMR 代表一个诊断周期
          if(VF_PRDMAINTMR_TMR){
              //诊断
              F_DiagDisp(nb_hy_betweenqy,nb_qy_betweenhy,v_t_cas0,v_t_cashy0,v_prd_cas,v_prd_cas_hy);//+DIG  
              //if(VF_SAMPATIME_STOP) 
              //2018-1-11,ysl,将下列诊断移到33814诊断函数
              /*if(!VF_MMOVE_ENG)
              {
                  DTC_DiagLdoPrepare_def(); //诊断前的准备 2017-12-1,ysl,add
                  if((VF_PREINJEND_SYSINI&&VF_PREPUMPEND_SYSINI&&VF_NOTNEED_DROPSCAN_SYSINI&&!VF_DOM_SYS)
            	            ||VFF_INJ_DTC     //2009-12-18 
            	    )
                  {
            	      F_Diag_INJ();//if(INJEND)call	hart_t					;       //检测喷油器故障；
            	      DTC_DiagPump_def();//诊断油泵 2017-12-1,ysl,add
                  }
                  F_Diag_IGN();//诊断点火电路
                  DTC_DiagO2H_def();//诊断氧传感器 2017-12-1,ysl,add
                  DTC_DiagLdoAfter_def(); //诊断后的操作 2017-12-29,ysl,add
              }*/
          }

          //2015-12-25 add condiction
          #ifdef IS_GEN
              F_I_ld(VF_NEWCYCLE_ATMAIN_CAS||(VF_PRDMAINTMR_TMR&&!VF_MMOVE_ENG));
          #endif
          
          /*----------------如果钥匙处于开启状态-----------------------*/
          if(!VF_MMOVE_ENG){
              /*----------------@如果钥匙开启，且发动机停机-----------------------*/
              Action_ForWaitingMove();  
              
              //发动机处于未运行 状态的动作              
              Action_WhenStop();
          //20210606 chenwei add
							//if(switch_1==1)
							{
								IGN_CONTROL_BYCAN();
							}
          }
          else{
          /*----------------钥匙开启，但发动机已经运行了-----------------------*/
                //发动机运行过程中，如果角标溢出，则表示停机
                
                if(VF_PCAOV_CAS)VF_JSTOP_ENG=true;
                

                FS_CA_st(v_CA_ignition);//设定基准角标   //2014-4-18 这个函数要挪进来了，在外面有问题，会有 VF_NEWCYCLE_ATMAIN_CAS和VF_OLDCA_CAS不同步的问题
                                       //2014-10-7 add v_CA_ignition parameter;
                
               // Cal_Exe();//2020-12-18                        
                //if(VF_NEWCYCLE_ATMAIN_CAS)    //2015-4-9 发现这个多余，删除
                
                /*
                    主循环与原8BIT的程序差异在于
                        1）将钥匙是否开启、车辆是否运行都融合到主循环中；
                        2）主循环中不等待角标，而是不多循环
                    几个标志位说明
                        VF_NEWCYCLE_ATMAIN_CAS：新角标VF_OLDCA_CAS到来后置位，在下一个主循环时再清理
                        VF_BEINJ_1CYC_INJPHASE：每一圈清一次，执行玩喷射后置位；
                        VF_CAMAIN_TIMEOUT_CAS：在主循环中，按时间预测的，下一次角标要来了
                        VF_AFRAME_NEED_INTSCI：需要进行一次通讯      
                    
                */
                if(VF_NEWCYCLE_ATMAIN_CAS)
                {
                      #ifdef SY_36
	                    TACH_ON
	                  #endif

					  #ifdef SY_24	//20210922 chenwei add 环松转速输出
					  	#ifdef SY_HUANSONG
							TACH_TOGGLE
						#endif

					  #endif
					  
                      //2015-3-17 MOVE HERE，注意一定要保证这句话的位置
                      VF_OLDCA_CAS=true;                     
                
                      //以下2个函数耗时100us 
                      /*---处理转速信号（在触发点火之前）-------*/
                      FG_Rev(v_prd_cas);               //+deal with rev;
                      /*---累计发动机圈数-------*/
                      FG_CCL(); 
					  
					  if(nb_leavidle<255)nb_leavidle++;
					  
					  //----------------------角标门限--------------------------    
            // 一直到FC_DueMain();耗时约700us

					//33814的角标处理
					if(!VF_STARTS_ENG&&VF_MMOVE_ENG)
					{
						if(!VF_33814CLRINITVRS_TST)//VF_TEST
						{
							mc33814_VRSCrankingClear();
							VF_33814CLRINITVRS_TST = true;
							VF_VRSLIMT_H=true;
						}
		
						if((v_rev_625rpm>REV62D5_1750)&&!VF_VRSLIMT_H)
						{
							mc33814_WriteVRSRuningParam(0xF3U);//1.75V
							VF_VRSLIMT_H=true;

							//TEST_CR//P5
						}
						else if((v_rev_625rpm<REV62D5_1250)&&VF_VRSLIMT_H)
						{
							mc33814_WriteVRSRuningParam(0xD3U);//850mv
							VF_VRSLIMT_H=false;
							//TEST_COME//P6
						}
		
					} 
					else 
					{
						//CA_TH_L;
						
					}


					  //2008-8-7 由于近日发现的喷射相位的问题，将该函数挪至此；
                      /*---怠速点火反馈控制-------*/
                      VF_IDLEIGNFK_IGN=false;//2009-12-18
                      /*========触发点火===================*/
                      
                //      #if (ECU_TYPE==ECU_MOT)
                      #ifndef IS_GEN //2015-12-8 纠正，之前少了!
                      
                          //for motorcycle
                          if((VF_NEEDRECBYLOWRPM_IGN)&&!VF_IGNONLY_CFCI&&VF_BFIRE_ENG)
                          {
                              if(!VF_ER_TPS)FC_Idle_Ign_BeforeTrig();//+low speed correction for ign ca
                          }//2009-12-24 ADD CONDICTION VF_BFIRE_ENG;
                      #else
                          //for generator  //2014-4-27 计算点火反馈
                          v_CA_ignition=F_dign(v_rev_3d9,v_CA_ignition);//2012-10-28
                      #endif

                      /*---采集压力传感器信号-------*/
                      VF_PAIR_BESAMP_WHENMOVE_REV=false;
                      
                      /*  2015-3-17 delete
                      if(!(IS_INJDRV_LOW)&&((v_prd_cash>=PCA6500)||VF_T3GET_INTINJ))Samp_Save_PAIR((byte)REMARK);//2008-7-16 zhangping add    //2009-12-9 add condiction
                      else nb_forpair=0;//2010-10-10 
                      */
                
                 //2015-3-17这两部分合成到一起了

                       //2015-3-17 测试，从FS_CA_st获得角标信号开始，到这里，需要120us
                       
                          
                      /*---点火-------*/
                          //w=F_Cal_T_Ign();//v_t_cas0,v_prd_cash,v_prd_cas   2013-6-6
					                 
							VF_IGN_OVER=false;
							Cal_Exe();  //2020-12-18 挪到这个条件外部
							if(VF_IGN_OVER)
							{
								//如果在计算齿号的时候被点火中断打断了，那此处再载入一次点火参数
								v_NO_dwellTooth_trig=v_NO_dwellTooth;
								v_NO_ignTooth_trig=v_NO_ignTooth;
								
								v_t_TrigTooth_To_Ign_trig=v_t_TrigTooth_To_Ign;     //触发齿到点火时刻
								v_t_TrigTooth_To_Dwell_trig=v_t_TrigTooth_To_Dwell;   //触发齿到充电时刻
							}
					 // TEST_COME

                          /*VF_NEED_CHARGE_IGN=false;

                            //pi.c 2015-10-11 change to  
                            if((
                                    !VF_YSTDCGET_INJPHASE                          
                                    ||(VF_YSTDCGET_INJPHASE&&VF_YSTDC_INJPHASE)
                                    ||(REMARK<MC_OPENLOOP)
                                    ||(VF_NOTDELAY_IGN&&v_TPS_odr>4) 
                                )
                                &&(
                                    !VF_IGNONLY_CFCI
                                    ||
                                    (VF_GT10_IDLETG_REV&&(v_revmax_625_cf<112||(v_ccl_cf<30)))//80 for 5000rpm    112　for 7000
                                )
                            )
                            {
                                VF_NEED_CHARGE_IGN=true;
								VF_NEED_CAL_IGNTOOTH=true;
                            } */

                          
                          
                          //触发点火CDI_ON，如果前面有CDI_CHARGE，则就点火了
                              //v_t_trigIgn=T_MAINTMR;
                              //F_Trig_Ign(w);                   //+cal ca for ignition ,then //TRIG IGNITION //2008-6-27      
                             // v_nb_ign++;
                              

                          /*点火诊断*/        //2013-5-13 delete
                          v_IgnCA_act_lst=v_IgnCA_act;
                          v_IgnCA_act=((unsigned long int) v_t_IgnCA_act*360)/v_prd_cas;

                      /*---电压采集-------*/
                      Samp_Save_Vbat();                   //+deal with vbat;//2008-10-5 remove

                      /*---电压的临时变化对T1的修正-------*/
                      /*#ifndef SYS_TRADITIONAL
					  v_tw1=FC_UbatChange_TW1(v_tw1,vc_TW1_vbatchg,vc_TW1_vbatdownidle);    //2008-7-27 delete
					  #endif*/
                      CLR_WDG;    

                      /*---节气门位置采样-------*/
                      Samp_TPS_And_Cal();//+SAMP AND DEAL WITH TPS


                      /*----ETC control---------*/
                      //#if (ECU_TYPE==ECU_GEN)
                      #ifdef IS_GEN
                      
                          //2014-4-27 执行电机控制
                          F_step_ECT(v_rev_3d9,v_TPS);  //2011-11-24 注意这段程序的位置；
                      #endif

                      
         

                      /*---获得发动机状态-------*/
                          FG_EngStatus_Basic();//summerize and move to here by zhangping
                          FG_EngStatus_Just();//
                          FG_CICO();//+DETECT CUT FUEL CONDICTION //vc_cfci2//+累计断油标记
                          F_Trans_FMF(v_TPS_ddt);//过渡工况策略?
                      
                      /*------基于转速的怠速失火检测------------*/
                      F_Missf_Idle(v_rev_15d6rpm,v_TPS_odr,vc_IdleMissfSum_CLS2);//+detect Missfire   //失火策略


                      /*------计算点火参数-------------------------------*/   
                
                      /*-------准备T35测试--------------------*/
					  /*#ifndef SYS_TRADITIONAL
						Prep_T35Test(v_teng_inuse,v_t_eng_d1s,v_rev_625rpm,v_ccl_cf);
					  #endif*/

                }
                

                //不管是否来新角标，只要发动机运行，都需要执行的工作
                if(VF_NEWCYCLE_ATMAIN_CAS){
                      FC_Ld_basic();
                }

                FC_DueMain();//判断MAIN运行循环是否超时了，即新角标是否快要到了

              
                //如果本圈没有进行过喷射
                if(!VF_BEINJ_1CYC_INJPHASE)//4.8mS或者1.5ms 
                //2015-12-25 条件更改为
                /*
                if(
                    !VF_BEINJ_1CYC_INJPHASE&&(m_nbtdc0!=BYPTR_M0_CAS_TRAILEDGE||!VF_OLDCAHY_CAS)
                    &&!VF_BIGN_INTIGN//2015-12-25 add VF_BIGN_INTIGN; 已经充电，但尚未触发点火（不是执行点火）
                    ) 
                */
                {
                
                    /*-----------一次main循环可以执行多次的---------------------*/
                    //if(!VF_CAMAIN_TIMEOUT_CAS &&!VF_BEINJ_1CYC_INJPHASE)
                    //2015-5-1x
                    //if(VF_NEWCYCLE_ATMAIN_CAS)
                    //2015-5-22
                    //if(VF_NEWCYCLE_ATMAIN_CAS||v_ts_binj<1400u||VF_CAMAIN_TIMEOUT_CAS)
                    
                    //2015-5-24 恢复为
                    if(!VF_CAMAIN_TIMEOUT_CAS)
                    {   //本圈的时间还没到,
                        
                         
                        FG_EngStatus_Just();  //发动机状态 
                        LdActionForTrigT1();//2015-5-19 耗时1.5ms                          
                        /*------非喷射圈的喷射动作控制策略------------*/
                        CTL_NOTYSTDC(v_rev_625rpm,v_TPS_ddt);//+Add inj strategy， 非喷油圈的控制策略    //2014-3-3 move 到FC_LD前面
                    
                    
                        /*--------加载最后需要喷射的T1------------------*/
                            //其中包含失火修正
                       /* #ifndef SYS_TRADITIONAL
						if((VF_T35TESTNEED_NOTYSTDC&&!VF_FORINJ_INJPHASE))//2008-6-29 modify    //2014-3-3 晚，DELETE VF_JCI_CFCI  ,因为这可能是导致回油门放炮的原因
                        {   //2012-11-29 18：00   ADD VF_JCI_CFCI                     //2014-3-3 VF_E0_TPS
                            //为了防止再给油那个时刻是喷射圈，上一次油膜补偿量没有算;
                            //刚再给油时不喷射
                            
							v_tw1=Set_TW1_forT35Get();          // CTL_NOTYSTDC过程获得VF_T35TESTNEED_INJ标志，
							
						}
                        else
						#endif
                        {   
                            //2013-7-30 ADD BACK
                            if(!VF_T35TESTNEED_NOTYSTDC){
                                v_dmi+=vc_TW1_IDLEMISSF_tmp;
                                v_tw1+=vc_TW1_IDLEMISSF_tmp;
                                //no_t3_fk_run=2;
                                v_dtw3_obj=v_dtw3_obj+vc_TW1_IDLEMISSF_tmp/2;
                            }

                        }*/
                            
                        /*--------触发T1动作，在一定的喷射提前角之后喷油------------------*/
                    		if(VF_INJRUN_ENG&&!VF_NOINJ_ENG)
                    		{ //2008-10-22 add condiction &&!VF_FORINJ_INJPHASE
                    		  	VF_TURNONINJ_ENG=true;

                    		}
                    		else
							{
								VF_TURNONINJ_ENG=false;//不喷油时，
                    		    //v_tw1=v_T3b;//Set_TW1_forPump(v_Vbat_odr,v_Vbat_mod);  //设定泵油T1参数   4000
                    		
							#ifndef  SYS_TRADITIONAL  
                    		    //2015-12-25 CHANGE TO 
                    		    if(v_T3b<30)v_tw1=30;
                    		    else v_tw1= v_T3b;
                    		#else
								v_tw1=0;
							#endif
                    		}
                    }
                  
                   
                    return_inj=F_Phase_Inj(v_TPS_odr,v_prd_cas,v_Vbat_inuse,v_rev_625rpm,v_t_cas0,v_ystdc_search_t,v_t_cas0);
                    
                    
                    //if(!(IS_INJDRV_LOW))Samp_Save_OS();     //O2采样及计算；  //2009-12-28 move here  ///2014-4-28 DELETE
                    
                    //if(v_ts_binj<600||VF_CAMAIN_TIMEOUT_CAS) //如果喷射时间很临近，或者本圈快结束时
                    //下面if语句耗时约10us
                    if(v_ts_binj<600||VF_CAMAIN_TIMEOUT_CAS||!VF_2INJ_ENABLE_INJPHASE) //如果喷射时间很临近，或者本圈快结束时
                    {
                        //LdActionForTrigT1();//2015-5-19
                        //需要触发的工作要在此执行
                        //if((VF_FORINJ_INJPHASE||!VF_T35TESTNEED_NOTYSTDC)&&VF_TURNONINJ_ENG)FC_mi_trig();
                        if(!VF_FORINJ_INJPHASE)v_FMF_all_lst=v_FMF_all;
                        VF_BEINJ_1CYC_INJPHASE=true;
                        FS_TPSlst();//2014-4-6
                        Save_TPS_basic(v_TPd);//保存本圈最后一次的TPS值，作为预测TPD的基础
                        
                        if(return_inj==1)
                        {   //获得喷油相位，得到当前到驱动喷油器的时间段的长度 v_ts_binj
                            if(VF_TURNONINJ_ENG){
                                //v_t_bInj_toCas=v_t_bInj_toCas_tmp;
                                //v_t_T1=v_tw1;
                                //v_t_Cas_trigT1=v_t_cas0;
                                //v_t_trigT1=T_MAINTMR;
                                
                                //FP_Inj_WhenTMROver(v_ts_binj,v_tw1);//+Trig Injection//  //触发喷油
								//TEST_CR
                                //if(!VF_T35TESTNEED_NOTYSTDC)FS_Film();
                                //2015-5-24 change to ,因为之前FILM衰减的太快
                                if(!VF_T35TESTNEED_NOTYSTDC&&VF_FORINJ_INJPHASE)FS_Film();
                            } else{
                                //未喷射时
                            #ifndef  SYS_TRADITIONAL    
								v_TW1_forTrig=v_T3b;
							#else
								v_TW1_forTrig=0;
							#endif
                            }
                        }
                        /*else if(return_inj==0)
                        {   
                            v_ts_binj=0xffff;
                        } 
                        else if(return_inj==2)
                        {
                            //2008-5-29 等待喷油未结束又来个角标；?
                            nb_reset=RST_BY_INJ_EXPECTION;
                        } 
                        else
                        {   //暂时不做处理
                        }*/
                        
                        
                        if(VF_3INJ_ENABLE_INJPHASE)v_nb_mayadd_notsys=1; 
                    }
                    
        
                }
                /*------发动机运行时的通讯处理-----------------*/

                if(VF_NEWCYCLE_ATMAIN_CAS)
                {
                    if((v_prd_cash>=PCAPER_RUN_19D2K)||!VF_FORINJ_INJPHASE)//2010-4-7
                            //2014-4-27 因为之前运行时，看到的T1都是非喷射的，所以将VF_FORINJ_INJPHASE改为!VF_FORINJ_INJPHASE
                    {
                        VF_AFRAME_NEED_INTSCI=true;
                    };//当转速较低或属于喷油圈时,进行通讯 //从sWFIRST_TEST中剥离出来




                    /*----发动机运行过程中的处理部分----------*/
                    //2015-2-8 ADD 
                    //2015-2-17 move in VF_NEWCYCLE_ATMAIN_CAS,减少时间
                    //
                    FC_F();//call Init_STARTS_Before_StartRun	;//发动机开始运行前，启动增油初始化		
                }


          }
          
          /*---------@钥匙开启状态下的公共区域------------------*/
                  //通讯

                  //采样
          
             //一直到v_t_runpoint=T_MAINTMR;耗时1.6ms(缺齿后),1ms.300us
          //以下2个函数耗时300us或者5us
          if(VF_AFRAME_NEED_INTSCI){
              TOPC(); 
              VF_AFRAME_NEED_INTSCI=false;
              
          }
          
          if(VF_33814RW_GEN) //2018-4-9,ysl,add,33814寄存器外部工具读写
          {
              App_RW33814_def();
              VF_33814RW_GEN = false;
          }
          
          if(TachOff_Flag)
          {
            #ifdef SY_36
				TACH_OFF
	        #endif
            TachOff_Flag=false;	
          }
          /*
          w=T_MAINTMR;
          if(KESEI>0)*/
          //VF_ISCHARGE_IGN
              DisCompFrame();//2015-10-25   //2015-12-26  发现这个函数要占用5MS的时间，影响点火了
          /*
          else deO=0;
          
          deDTFI=T_MAINTMR-w;
          if(deO<deDTFI)deO=deDTFI;
          */
          /*条件满足时耗时500us*/
          if(VF_MMOVE_ENG){
              if(VF_NEWCYCLE_ATMAIN_CAS){
                
                    //2014-5-17 把点火角移动到这个位置，否则，通讯时看到的不是本次实际的点火角，而是与下一次相关的
                    /*------获得基础点火角------------*/
                    //获得点火角并修正＃ǎ?
                    FG_CA_Ign_basic();//V_Tw,v_PCBTDC_SCI,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,v_rev_15d6rpm,REMARK,v_prd_cash,v_TPd
                    /*------海拔对基础点火角------------*/
                    //if(v_rev_625rpm<REV62D5_7000)
                    if(v_rev_625rpm<REV62D5_8500)   //2015-4-16 DY150高原实验时，发现宰割转速限值太低了
                    
                    {
                        FC_IGN_Altitude();//v_pair_odr,v_pair_mode,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode
                    }
              }
          }
      
          /*
		  //---T3 的DOM 修正-------
          //if((VF_T3GET_INTINJ&&!VF_T3BECHECK_INTINJ)&&(!VF_YSTDC_INJPHASE||v_rev_625rpm<REV62D5_5750||!VF_MMOVE_ENG)) //2010-8-30 add 
          if((VF_T3GET_INTINJ&&!VF_T3BECHECK_INTINJ)) //2015-7-5 change to
          {   //2012-11-8 ADD CONDICTION MOVE HERE   Cal_T35
              v_TW3=check_t3ers(v_TW3_in,v_Vbat_inuse);   //2009-3-2 add
              //---计算T35修正-------
              Cal_T35(v_TPS_ddt,v_TW3,v_Vbat_inuse,v_tw1_synwithTW3,v_TW4,v_TW4_2nd,v_NB_INJDRV);//+CAL T35;//IT NEEDS THE ORDER OF VBAT
          }
          */
          
		  //怠速时最高1.2ms,其余时间30us 
          if(!VF_MMOVE_ENG)//{
          //if(!VF_MMOVE_ENG&&!VF_T3GET_INTINJ)
          {
             //2015-8-28 把这一行删除？？？
              //这一行不能删，因为没有得到T3时也要进来，判断初始化的工作，删了就不能初始化了
			 /* #ifndef SYS_TRADITIONAL	
				Cal_T35(v_TPS_ddt,v_TW3,v_Vbat_inuse,v_tw1_synwithTW3,v_TW4,v_TW4_2nd,v_NB_INJDRV);//+CAL T35;//IT NEEDS THE ORDER OF VBAT
			  #endif*/
          } 
          else 
          {
             
              CheckWriting_ext();
              
              if(!VF_OPEREE_1CYCLE_EE&&!VF_ISWRITING_EE){
                  VF_OPEREE_1CYCLE_EE=true;
                  FL_SL_EE_rd();
                  FL_SL_EE_wr();//if(VF_OLDCA_CAS) DELETE CONDICTION    
              }
             

          }
                    v_t_runtime=T_MAINTMR-v_t_runpoint;
                    v_t_runpoint=T_MAINTMR;
					
				/*#ifndef SYS_TRADITIONAL	
					FI_IDLE_T3FK();//2017-2-23 从18pin程序移植
				#endif*/
          
      

      }
      /*---------@不管钥匙是否开启，只要ECU上电后的公共区域------------------*/
            //某些条件下强行关闭喷射
       //以下耗时500us(新角标)或者10us     
          //#if (ECU_TYPE==ECU_GEN )
          #ifdef IS_GEN
          
              //2014-4-27 控制直流电机     
              FS_DCMotor( );//2012-11-3
          #else 
              #ifdef SYS_STEPMOTOR//2017-11-30
		  
		  
		  if(VF_E0_TPS||!VF_MMOVE_ENG)
		  {
			 w=T_MAINTMR-t_IACVLstT;
		  
			 if((w>250)||VF_NEWCYCLE_ATMAIN_CAS||!VF_MMOVE_ENG)
			 {
			//if((v_t_NOTE0TPS_1s<=1)||(v_t_afterbackidle_d1s<1))
			//{
//				FS_SMotor_IACV();//2016-9-7
			//}
			    t_IACVLstT=T_MAINTMR;
			 } 
		  }
		  else if(v_t_NOTE0TPS_1s<=1)
		  {			  
//			  FS_SMotor_IACV();//2016-9-7
			  t_IACVLstT=T_MAINTMR;
		  }
		  //FS_SMotor_IACV();
              #endif  
          #endif
		  
		  /***20201125 chenwei add 欧五碳罐电磁阀***/
		 #ifdef SY_CCPS
		 	F_CCPS_ACTION();   //20201127 chenwei add 碳罐电磁阀功能
		 #endif 

		   F_CC_Diag();		//20210819 chenwei add J16接地熄火	

		   #ifdef SY_BUMPO2_C		//20211003 chenwei add 泵氧
			M_BumpO2_C_ON

			#endif	
			
				#ifdef	SYS_O2HEATER_S
						
					#ifdef SY_HUANSONG	   //20211010 chenwei add 环松氧加热处理方式				   
						F_O2H_FORHUANSONG();	  
					#endif
				
				#endif
  
  }while(1);

  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/


#pragma CODE_SEG DEFAULT 

/* END GENERATOR */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.05 [04.46]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/

//需要触发T1用的负荷计算
void LdActionForTrigT1(void)
{

    Samp_TPS_And_Cal();
    
    FC_Ld_just();
    
    FPredict_Ld(v_TPd);       //预测TPS

    FC_mi_just();
    
    vc_FMF_dtfi_trans=FC_Film_UA(v_TPd_lst,v_TPd,v_FMF);
    v_FMF+=vc_FMF_dtfi_trans;
    
    FC_mi_trig();
    
    
}


//等待发动机启动的动作
void Action_ForWaitingMove(void)
{
      //如果是发动机停机状态
      if(!VF_CATCH_CAS)//Wait_till_1stCACome();  //未得到第一个角标时，判断第一个角标是否来临
      {   //一直没有获取角标
          //if(VF_SAMPATIME_STOP)FI_CAS_WhenCANotCaught();   //钥匙开启后，角标的准备工作
          
      } else{
          //已经有出现角标了
          if(VF_PCAOV_CAS){
              //如果角标溢出，则回到VF_CATCH_CAS=false的状态
              FI_CAS_WhenCANotCaught();//
              VF_JSTOP_ENG=true;
              v_nb_JSTOP=true;
          } else{
              //获取角标了
              if(!VF_2NDCA_STOP) {
                  //之前未获取第二个角标
                  if(!VF_OLDCA_CAS){
                      //如果新来角标了
                      //
                      VF_2NDCA_STOP=true;
                      VF_AFRAME_NEED_INTSCI=true;  
                      
                      
                      #ifndef IS_GEN
                          //if(v_prd_cash<M0_START_SPEED&&M0_MAX_START_RPM<=v_prd_cash){
                          if(v_prd_cash<M0_START_SPEED&&PCA1650>=v_prd_cash)//M0_MAX_START_RPM<=v_prd_cash){
                          {
                              //转速在合理范围内
                                  //转速在1650以下
                                  //可以进入发动机运行模式了
                              
                              VF_JMMOVE_ENG=true;
                              v_nb_JMMOVE++; 
                          } else{
                             // VF_OLDCA_CAS=false;    
                          }
                          
                      #else
                          //2015-12-11 增加对通机的处理，防止停机时角标干扰 t_stop_s>=2
                          
                      
                          if(v_prd_cash<M0_START_SPEED&&PCA1650>=v_prd_cash&&t_stop_s>=2)//M0_MAX_START_RPM<=v_prd_cash){
                          {
                              //转速在合理范围内
                                  //转速在1650以下
                                  //可以进入发动机运行模式了
                              
                              VF_JMMOVE_ENG=true;
                              v_nb_JMMOVE++; 
                          } else{
                             // VF_OLDCA_CAS=false;    
                          }
                          
                      
                      #endif
                         
                  } else{
                      Init_after1stCACome();
                  }
              } else{
                  //获取了第二个角标
                  //等待第三个角标
                  if(!VF_OLDCA_CAS)                                                                                                                               //2010-10-9 add !VF_PCAOV_CAS;
                  {
                      {    
                          VF_JMMOVE_ENG=true;
                          v_nb_JMMOVE++;
                          VF_AFRAME_NEED_INTSCI=true;  
                      }
                  }
                        
              }
          }
      
      }
    
}

/****20210805 chenwei add******************************/
/*****功能：解决CANH与CANL短接之后CAN通讯终端的问题****/
void Can_Busoff_Reset(void)
{
	if((CAN1->BIT.RESET)&&(CAN1->BIT.BUSOFF==0))
	{
		CAN1->BIT.RESET=0;
		CAN1->REG.CFG_STAT|=TPSS;
		CAN1->REG.CFG_STAT|=TSSS;
	}
}

