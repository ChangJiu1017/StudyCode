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
*    1.�����ͱó�ʼ��
*    2.���� 33814״̬������ʼ��
*    3.�����ͱÿ��ơ���Ϻ���
*    4.����33814�Ĵ����ⲿ���߶�д
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
#include "dtc.h"//��
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
#include "CCPS.h"       //20201127 chenwei add ̼�޵�ŷ�


/* User includes (#include below this line is not maintained by Processor Expert) */
void Shift_Status(void);
void Action_ForWaitingMove(void);   //2014-4-2
void Can_Busoff_Reset(void);			//20210805 chenwei add �Ż�CANH��CANL�̽�֮��can-dataview��ͨѶ�����

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
  //�ϵ��ĳ�ʼ������
      //PE_low_level_init();

  /*�����￪ʼ��dowhileѭ����ʱ2ms*/
  pump_PumpInit_init(); //2018-04-03,ysl,�������������������Ծ��쿪���ͱò���ʱ

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
  
  VF_POR_HARD=true;//2014-4-5 ;����֣���ʱ����û����һ��Ҳ���Ե�
  
    F_ChargeEn = 0;
	F_Trig_IgnEn = 1;		//20210930 �˴�Ӧ�ó�ʼ��F_Trig_IgnEn=1��ԭ�ȳ�ʼ������F_Trig_IgnEn=0
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

  /*----------------------------------�ϵ�����ѭ��----------------------------------*/
  do{  //��ѭ����ȱ�ݺ�4ms����,400us,����1.5ms
      //BOOT_SCI_Sent_Byte(0xAA);
 
      /*---------�ж�Կ�׺ͽǱ��״̬������仯------------------*/
	      Can_Busoff_Reset();
          Check_KeyStatus();    //У��Կ�׵�״̬
          Shift_Status();       //״̬�仯������Կ���Ƿ������������Ƿ���ͣ������Ҫִ�еĶ���
          FC_TimeOutFlag();     //����һЩʱ���־
         
          CLR_WDG//2014-4-13
          /*����״̬��ʱ���־��ʱԼ30us*/
                            
      /*-------------------------AD�����Ĵ���------------------*/
       //��ʱ5us
          //if(VF_SAMPATIME_STOP||VF_NEWCYCLE_ATMAIN_CAS)
          if(VF_SAMPATIME_STOP||VF_MMOVE_ENG)//2014-4-28
          {
              if(!VF_MMOVE_ENG)
              {
                  V_AD_STATUS=false; 
                  //2015-3-17 ���������������Կ��ʱ�䲻������������
                 /* if(v_t_eng_d1s<10)
                  {   //��Կ�׵�1S�ڲ��ɼ�PAIR
                      VF_PAIRS_ED_AD=true;    
                  }*/
                  
                     
              }
              //����½Ǳ����ˣ�����ʱ�䵽�ˣ���ȫ������
              FAD_ALL((byte)REMARK);  //AD����������ѭ���н��еģ��������жϣ� ����������������������
              if(VF_NEWCYCLE_ATMAIN_CAS||VF_SAMPATIME_STOP)FS_TPSlst();//2014-4-6

          }
          
         // DC_con(KESEI,REMARK);



          //#if ECU_TYPE==ECU_GEN 
          #ifdef IS_GEN
              
              //if((VF_SAMPATIME_STOP&&!VF_MMOVE_ENG)||(VF_MMOVE_ENG&&))
              if(VF_SAMPATIME_STOP||(VF_PRDMAINTMR_TMR&&VF_MMOVE_ENG)) //2016-1-11 add condiction
              {
                  FS_ECT_IdlePosition(v_TPS,v_TPS_odr,v_teng_inuse); //���������δ�Ż�
              }
          #endif
          
      /*---------���ϣ�@����Կ���Ƿ�����ֻҪECU�ϵ��Ĺ�������------------------*/

      if(!VF_KEYON_HARD){
          /*----------------���Կ�״��ڹر�״̬���ȴ���-----------------------*/
          Action_whenKeyOff();    //������ȵ�Կ�׿���Ϊֹ
         
          //#if (ECU_TYPE==ECU_GEN )
          #ifdef IS_GEN
          
              //2014-4-27 ����ֱ�����     
              FS_DCMotor( );//2012-11-3
          #else//2016-9-7    
              #ifdef SYS_STEPMOTOR//2017-11-30
//			  FS_SMotor_IACV();//2016-9-7    
              #endif
          #endif

      } else{
          /*---------------�ͱÿ��� 2018-4-2,ysl,add----------------*/
          // ��Ϻ�ʱ5us
          pump_PumpControl_maincycle();
          
          /*----------------��ϲ��ַ������Ϊ��ҪԿ�׿���--------------------*/
          //��ϽǱ�
          if(!VF_CA_BEDIAGED_CAS&&VF_CADIAG_TIMEIN_CAS){
                F_Diag_CA(nb_hy_betweenqy, nb_qy_betweenhy, v_t_cas0,v_t_cashy0,v_prd_cas,v_prd_cas_hy);//2008-8-1 add
                VF_CA_BEDIAGED_CAS=true;
          }

          //��������� 2018-1-11,ysl
          DTC_Diag_def(v_prd_cas);
          
           
          //VF_PRDMAINTMR_TMR ����һ���������
          if(VF_PRDMAINTMR_TMR){
              //���
              F_DiagDisp(nb_hy_betweenqy,nb_qy_betweenhy,v_t_cas0,v_t_cashy0,v_prd_cas,v_prd_cas_hy);//+DIG  
              //if(VF_SAMPATIME_STOP) 
              //2018-1-11,ysl,����������Ƶ�33814��Ϻ���
              /*if(!VF_MMOVE_ENG)
              {
                  DTC_DiagLdoPrepare_def(); //���ǰ��׼�� 2017-12-1,ysl,add
                  if((VF_PREINJEND_SYSINI&&VF_PREPUMPEND_SYSINI&&VF_NOTNEED_DROPSCAN_SYSINI&&!VF_DOM_SYS)
            	            ||VFF_INJ_DTC     //2009-12-18 
            	    )
                  {
            	      F_Diag_INJ();//if(INJEND)call	hart_t					;       //������������ϣ�
            	      DTC_DiagPump_def();//����ͱ� 2017-12-1,ysl,add
                  }
                  F_Diag_IGN();//��ϵ���·
                  DTC_DiagO2H_def();//����������� 2017-12-1,ysl,add
                  DTC_DiagLdoAfter_def(); //��Ϻ�Ĳ��� 2017-12-29,ysl,add
              }*/
          }

          //2015-12-25 add condiction
          #ifdef IS_GEN
              F_I_ld(VF_NEWCYCLE_ATMAIN_CAS||(VF_PRDMAINTMR_TMR&&!VF_MMOVE_ENG));
          #endif
          
          /*----------------���Կ�״��ڿ���״̬-----------------------*/
          if(!VF_MMOVE_ENG){
              /*----------------@���Կ�׿������ҷ�����ͣ��-----------------------*/
              Action_ForWaitingMove();  
              
              //����������δ���� ״̬�Ķ���              
              Action_WhenStop();
          //20210606 chenwei add
							//if(switch_1==1)
							{
								IGN_CONTROL_BYCAN();
							}
          }
          else{
          /*----------------Կ�׿��������������Ѿ�������-----------------------*/
                //���������й����У�����Ǳ���������ʾͣ��
                
                if(VF_PCAOV_CAS)VF_JSTOP_ENG=true;
                

                FS_CA_st(v_CA_ignition);//�趨��׼�Ǳ�   //2014-4-18 �������ҪŲ�����ˣ������������⣬���� VF_NEWCYCLE_ATMAIN_CAS��VF_OLDCA_CAS��ͬ��������
                                       //2014-10-7 add v_CA_ignition parameter;
                
               // Cal_Exe();//2020-12-18                        
                //if(VF_NEWCYCLE_ATMAIN_CAS)    //2015-4-9 ����������࣬ɾ��
                
                /*
                    ��ѭ����ԭ8BIT�ĳ����������
                        1����Կ���Ƿ����������Ƿ����ж��ںϵ���ѭ���У�
                        2����ѭ���в��ȴ��Ǳ꣬���ǲ���ѭ��
                    ������־λ˵��
                        VF_NEWCYCLE_ATMAIN_CAS���½Ǳ�VF_OLDCA_CAS��������λ������һ����ѭ��ʱ������
                        VF_BEINJ_1CYC_INJPHASE��ÿһȦ��һ�Σ�ִ�����������λ��
                        VF_CAMAIN_TIMEOUT_CAS������ѭ���У���ʱ��Ԥ��ģ���һ�νǱ�Ҫ����
                        VF_AFRAME_NEED_INTSCI����Ҫ����һ��ͨѶ      
                    
                */
                if(VF_NEWCYCLE_ATMAIN_CAS)
                {
                      #ifdef SY_36
	                    TACH_ON
	                  #endif

					  #ifdef SY_24	//20210922 chenwei add ����ת�����
					  	#ifdef SY_HUANSONG
							TACH_TOGGLE
						#endif

					  #endif
					  
                      //2015-3-17 MOVE HERE��ע��һ��Ҫ��֤��仰��λ��
                      VF_OLDCA_CAS=true;                     
                
                      //����2��������ʱ100us 
                      /*---����ת���źţ��ڴ������֮ǰ��-------*/
                      FG_Rev(v_prd_cas);               //+deal with rev;
                      /*---�ۼƷ�����Ȧ��-------*/
                      FG_CCL(); 
					  
					  if(nb_leavidle<255)nb_leavidle++;
					  
					  //----------------------�Ǳ�����--------------------------    
            // һֱ��FC_DueMain();��ʱԼ700us

					//33814�ĽǱ괦��
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


					  //2008-8-7 ���ڽ��շ��ֵ�������λ�����⣬���ú���Ų���ˣ�
                      /*---���ٵ��������-------*/
                      VF_IDLEIGNFK_IGN=false;//2009-12-18
                      /*========�������===================*/
                      
                //      #if (ECU_TYPE==ECU_MOT)
                      #ifndef IS_GEN //2015-12-8 ������֮ǰ����!
                      
                          //for motorcycle
                          if((VF_NEEDRECBYLOWRPM_IGN)&&!VF_IGNONLY_CFCI&&VF_BFIRE_ENG)
                          {
                              if(!VF_ER_TPS)FC_Idle_Ign_BeforeTrig();//+low speed correction for ign ca
                          }//2009-12-24 ADD CONDICTION VF_BFIRE_ENG;
                      #else
                          //for generator  //2014-4-27 ��������
                          v_CA_ignition=F_dign(v_rev_3d9,v_CA_ignition);//2012-10-28
                      #endif

                      /*---�ɼ�ѹ���������ź�-------*/
                      VF_PAIR_BESAMP_WHENMOVE_REV=false;
                      
                      /*  2015-3-17 delete
                      if(!(IS_INJDRV_LOW)&&((v_prd_cash>=PCA6500)||VF_T3GET_INTINJ))Samp_Save_PAIR((byte)REMARK);//2008-7-16 zhangping add    //2009-12-9 add condiction
                      else nb_forpair=0;//2010-10-10 
                      */
                
                 //2015-3-17�������ֺϳɵ�һ����

                       //2015-3-17 ���ԣ���FS_CA_st��ýǱ��źſ�ʼ���������Ҫ120us
                       
                          
                      /*---���-------*/
                          //w=F_Cal_T_Ign();//v_t_cas0,v_prd_cash,v_prd_cas   2013-6-6
					                 
							VF_IGN_OVER=false;
							Cal_Exe();  //2020-12-18 Ų����������ⲿ
							if(VF_IGN_OVER)
							{
								//����ڼ���ݺŵ�ʱ�򱻵���жϴ���ˣ��Ǵ˴�������һ�ε�����
								v_NO_dwellTooth_trig=v_NO_dwellTooth;
								v_NO_ignTooth_trig=v_NO_ignTooth;
								
								v_t_TrigTooth_To_Ign_trig=v_t_TrigTooth_To_Ign;     //�����ݵ����ʱ��
								v_t_TrigTooth_To_Dwell_trig=v_t_TrigTooth_To_Dwell;   //�����ݵ����ʱ��
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
                                    (VF_GT10_IDLETG_REV&&(v_revmax_625_cf<112||(v_ccl_cf<30)))//80 for 5000rpm    112��for 7000
                                )
                            )
                            {
                                VF_NEED_CHARGE_IGN=true;
								VF_NEED_CAL_IGNTOOTH=true;
                            } */

                          
                          
                          //�������CDI_ON�����ǰ����CDI_CHARGE����͵����
                              //v_t_trigIgn=T_MAINTMR;
                              //F_Trig_Ign(w);                   //+cal ca for ignition ,then //TRIG IGNITION //2008-6-27      
                             // v_nb_ign++;
                              

                          /*������*/        //2013-5-13 delete
                          v_IgnCA_act_lst=v_IgnCA_act;
                          v_IgnCA_act=((unsigned long int) v_t_IgnCA_act*360)/v_prd_cas;

                      /*---��ѹ�ɼ�-------*/
                      Samp_Save_Vbat();                   //+deal with vbat;//2008-10-5 remove

                      /*---��ѹ����ʱ�仯��T1������-------*/
                      /*#ifndef SYS_TRADITIONAL
					  v_tw1=FC_UbatChange_TW1(v_tw1,vc_TW1_vbatchg,vc_TW1_vbatdownidle);    //2008-7-27 delete
					  #endif*/
                      CLR_WDG;    

                      /*---������λ�ò���-------*/
                      Samp_TPS_And_Cal();//+SAMP AND DEAL WITH TPS


                      /*----ETC control---------*/
                      //#if (ECU_TYPE==ECU_GEN)
                      #ifdef IS_GEN
                      
                          //2014-4-27 ִ�е������
                          F_step_ECT(v_rev_3d9,v_TPS);  //2011-11-24 ע����γ����λ�ã�
                      #endif

                      
         

                      /*---��÷�����״̬-------*/
                          FG_EngStatus_Basic();//summerize and move to here by zhangping
                          FG_EngStatus_Just();//
                          FG_CICO();//+DETECT CUT FUEL CONDICTION //vc_cfci2//+�ۼƶ��ͱ��
                          F_Trans_FMF(v_TPS_ddt);//���ɹ�������?
                      
                      /*------����ת�ٵĵ���ʧ����------------*/
                      F_Missf_Idle(v_rev_15d6rpm,v_TPS_odr,vc_IdleMissfSum_CLS2);//+detect Missfire   //ʧ�����


                      /*------���������-------------------------------*/   
                
                      /*-------׼��T35����--------------------*/
					  /*#ifndef SYS_TRADITIONAL
						Prep_T35Test(v_teng_inuse,v_t_eng_d1s,v_rev_625rpm,v_ccl_cf);
					  #endif*/

                }
                

                //�����Ƿ����½Ǳֻ꣬Ҫ���������У�����Ҫִ�еĹ���
                if(VF_NEWCYCLE_ATMAIN_CAS){
                      FC_Ld_basic();
                }

                FC_DueMain();//�ж�MAIN����ѭ���Ƿ�ʱ�ˣ����½Ǳ��Ƿ��Ҫ����

              
                //�����Ȧû�н��й�����
                if(!VF_BEINJ_1CYC_INJPHASE)//4.8mS����1.5ms 
                //2015-12-25 ��������Ϊ
                /*
                if(
                    !VF_BEINJ_1CYC_INJPHASE&&(m_nbtdc0!=BYPTR_M0_CAS_TRAILEDGE||!VF_OLDCAHY_CAS)
                    &&!VF_BIGN_INTIGN//2015-12-25 add VF_BIGN_INTIGN; �Ѿ���磬����δ������𣨲���ִ�е��
                    ) 
                */
                {
                
                    /*-----------һ��mainѭ������ִ�ж�ε�---------------------*/
                    //if(!VF_CAMAIN_TIMEOUT_CAS &&!VF_BEINJ_1CYC_INJPHASE)
                    //2015-5-1x
                    //if(VF_NEWCYCLE_ATMAIN_CAS)
                    //2015-5-22
                    //if(VF_NEWCYCLE_ATMAIN_CAS||v_ts_binj<1400u||VF_CAMAIN_TIMEOUT_CAS)
                    
                    //2015-5-24 �ָ�Ϊ
                    if(!VF_CAMAIN_TIMEOUT_CAS)
                    {   //��Ȧ��ʱ�仹û��,
                        
                         
                        FG_EngStatus_Just();  //������״̬ 
                        LdActionForTrigT1();//2015-5-19 ��ʱ1.5ms                          
                        /*------������Ȧ�����䶯�����Ʋ���------------*/
                        CTL_NOTYSTDC(v_rev_625rpm,v_TPS_ddt);//+Add inj strategy�� ������Ȧ�Ŀ��Ʋ���    //2014-3-3 move ��FC_LDǰ��
                    
                    
                        /*--------���������Ҫ�����T1------------------*/
                            //���а���ʧ������
                       /* #ifndef SYS_TRADITIONAL
						if((VF_T35TESTNEED_NOTYSTDC&&!VF_FORINJ_INJPHASE))//2008-6-29 modify    //2014-3-3 ��DELETE VF_JCI_CFCI  ,��Ϊ������ǵ��»����ŷ��ڵ�ԭ��
                        {   //2012-11-29 18��00   ADD VF_JCI_CFCI                     //2014-3-3 VF_E0_TPS
                            //Ϊ�˷�ֹ�ٸ����Ǹ�ʱ��������Ȧ����һ����Ĥ������û����;
                            //���ٸ���ʱ������
                            
							v_tw1=Set_TW1_forT35Get();          // CTL_NOTYSTDC���̻��VF_T35TESTNEED_INJ��־��
							
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
                            
                        /*--------����T1��������һ����������ǰ��֮������------------------*/
                    		if(VF_INJRUN_ENG&&!VF_NOINJ_ENG)
                    		{ //2008-10-22 add condiction &&!VF_FORINJ_INJPHASE
                    		  	VF_TURNONINJ_ENG=true;

                    		}
                    		else
							{
								VF_TURNONINJ_ENG=false;//������ʱ��
                    		    //v_tw1=v_T3b;//Set_TW1_forPump(v_Vbat_odr,v_Vbat_mod);  //�趨����T1����   4000
                    		
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
                    
                    
                    //if(!(IS_INJDRV_LOW))Samp_Save_OS();     //O2���������㣻  //2009-12-28 move here  ///2014-4-28 DELETE
                    
                    //if(v_ts_binj<600||VF_CAMAIN_TIMEOUT_CAS) //�������ʱ����ٽ������߱�Ȧ�����ʱ
                    //����if����ʱԼ10us
                    if(v_ts_binj<600||VF_CAMAIN_TIMEOUT_CAS||!VF_2INJ_ENABLE_INJPHASE) //�������ʱ����ٽ������߱�Ȧ�����ʱ
                    {
                        //LdActionForTrigT1();//2015-5-19
                        //��Ҫ�����Ĺ���Ҫ�ڴ�ִ��
                        //if((VF_FORINJ_INJPHASE||!VF_T35TESTNEED_NOTYSTDC)&&VF_TURNONINJ_ENG)FC_mi_trig();
                        if(!VF_FORINJ_INJPHASE)v_FMF_all_lst=v_FMF_all;
                        VF_BEINJ_1CYC_INJPHASE=true;
                        FS_TPSlst();//2014-4-6
                        Save_TPS_basic(v_TPd);//���汾Ȧ���һ�ε�TPSֵ����ΪԤ��TPD�Ļ���
                        
                        if(return_inj==1)
                        {   //���������λ���õ���ǰ��������������ʱ��εĳ��� v_ts_binj
                            if(VF_TURNONINJ_ENG){
                                //v_t_bInj_toCas=v_t_bInj_toCas_tmp;
                                //v_t_T1=v_tw1;
                                //v_t_Cas_trigT1=v_t_cas0;
                                //v_t_trigT1=T_MAINTMR;
                                
                                //FP_Inj_WhenTMROver(v_ts_binj,v_tw1);//+Trig Injection//  //��������
								//TEST_CR
                                //if(!VF_T35TESTNEED_NOTYSTDC)FS_Film();
                                //2015-5-24 change to ,��Ϊ֮ǰFILM˥����̫��
                                if(!VF_T35TESTNEED_NOTYSTDC&&VF_FORINJ_INJPHASE)FS_Film();
                            } else{
                                //δ����ʱ
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
                            //2008-5-29 �ȴ�����δ�����������Ǳꣻ?
                            nb_reset=RST_BY_INJ_EXPECTION;
                        } 
                        else
                        {   //��ʱ��������
                        }*/
                        
                        
                        if(VF_3INJ_ENABLE_INJPHASE)v_nb_mayadd_notsys=1; 
                    }
                    
        
                }
                /*------����������ʱ��ͨѶ����-----------------*/

                if(VF_NEWCYCLE_ATMAIN_CAS)
                {
                    if((v_prd_cash>=PCAPER_RUN_19D2K)||!VF_FORINJ_INJPHASE)//2010-4-7
                            //2014-4-27 ��Ϊ֮ǰ����ʱ��������T1���Ƿ�����ģ����Խ�VF_FORINJ_INJPHASE��Ϊ!VF_FORINJ_INJPHASE
                    {
                        VF_AFRAME_NEED_INTSCI=true;
                    };//��ת�ٽϵͻ���������Ȧʱ,����ͨѶ //��sWFIRST_TEST�а������




                    /*----���������й����еĴ�����----------*/
                    //2015-2-8 ADD 
                    //2015-2-17 move in VF_NEWCYCLE_ATMAIN_CAS,����ʱ��
                    //
                    FC_F();//call Init_STARTS_Before_StartRun	;//��������ʼ����ǰ���������ͳ�ʼ��		
                }


          }
          
          /*---------@Կ�׿���״̬�µĹ�������------------------*/
                  //ͨѶ

                  //����
          
             //һֱ��v_t_runpoint=T_MAINTMR;��ʱ1.6ms(ȱ�ݺ�),1ms.300us
          //����2��������ʱ300us����5us
          if(VF_AFRAME_NEED_INTSCI){
              TOPC(); 
              VF_AFRAME_NEED_INTSCI=false;
              
          }
          
          if(VF_33814RW_GEN) //2018-4-9,ysl,add,33814�Ĵ����ⲿ���߶�д
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
              DisCompFrame();//2015-10-25   //2015-12-26  �����������Ҫռ��5MS��ʱ�䣬Ӱ������
          /*
          else deO=0;
          
          deDTFI=T_MAINTMR-w;
          if(deO<deDTFI)deO=deDTFI;
          */
          /*��������ʱ��ʱ500us*/
          if(VF_MMOVE_ENG){
              if(VF_NEWCYCLE_ATMAIN_CAS){
                
                    //2014-5-17 �ѵ����ƶ������λ�ã�����ͨѶʱ�����Ĳ��Ǳ���ʵ�ʵĵ��ǣ���������һ����ص�
                    /*------��û�������------------*/
                    //��õ��ǲ���������?
                    FG_CA_Ign_basic();//V_Tw,v_PCBTDC_SCI,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,v_rev_15d6rpm,REMARK,v_prd_cash,v_TPd
                    /*------���ζԻ�������------------*/
                    //if(v_rev_625rpm<REV62D5_7000)
                    if(v_rev_625rpm<REV62D5_8500)   //2015-4-16 DY150��ԭʵ��ʱ�������׸�ת����ֵ̫����
                    
                    {
                        FC_IGN_Altitude();//v_pair_odr,v_pair_mode,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode
                    }
              }
          }
      
          /*
		  //---T3 ��DOM ����-------
          //if((VF_T3GET_INTINJ&&!VF_T3BECHECK_INTINJ)&&(!VF_YSTDC_INJPHASE||v_rev_625rpm<REV62D5_5750||!VF_MMOVE_ENG)) //2010-8-30 add 
          if((VF_T3GET_INTINJ&&!VF_T3BECHECK_INTINJ)) //2015-7-5 change to
          {   //2012-11-8 ADD CONDICTION MOVE HERE   Cal_T35
              v_TW3=check_t3ers(v_TW3_in,v_Vbat_inuse);   //2009-3-2 add
              //---����T35����-------
              Cal_T35(v_TPS_ddt,v_TW3,v_Vbat_inuse,v_tw1_synwithTW3,v_TW4,v_TW4_2nd,v_NB_INJDRV);//+CAL T35;//IT NEEDS THE ORDER OF VBAT
          }
          */
          
		  //����ʱ���1.2ms,����ʱ��30us 
          if(!VF_MMOVE_ENG)//{
          //if(!VF_MMOVE_ENG&&!VF_T3GET_INTINJ)
          {
             //2015-8-28 ����һ��ɾ��������
              //��һ�в���ɾ����Ϊû�еõ�T3ʱҲҪ�������жϳ�ʼ���Ĺ�����ɾ�˾Ͳ��ܳ�ʼ����
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
					FI_IDLE_T3FK();//2017-2-23 ��18pin������ֲ
				#endif*/
          
      

      }
      /*---------@����Կ���Ƿ�����ֻҪECU�ϵ��Ĺ�������------------------*/
            //ĳЩ������ǿ�йر�����
       //���º�ʱ500us(�½Ǳ�)����10us     
          //#if (ECU_TYPE==ECU_GEN )
          #ifdef IS_GEN
          
              //2014-4-27 ����ֱ�����     
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
		  
		  /***20201125 chenwei add ŷ��̼�޵�ŷ�***/
		 #ifdef SY_CCPS
		 	F_CCPS_ACTION();   //20201127 chenwei add ̼�޵�ŷ�����
		 #endif 

		   F_CC_Diag();		//20210819 chenwei add J16�ӵ�Ϩ��	

		   #ifdef SY_BUMPO2_C		//20211003 chenwei add ����
			M_BumpO2_C_ON

			#endif	
			
				#ifdef	SYS_O2HEATER_S
						
					#ifdef SY_HUANSONG	   //20211010 chenwei add ���������ȴ���ʽ				   
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

//��Ҫ����T1�õĸ��ɼ���
void LdActionForTrigT1(void)
{

    Samp_TPS_And_Cal();
    
    FC_Ld_just();
    
    FPredict_Ld(v_TPd);       //Ԥ��TPS

    FC_mi_just();
    
    vc_FMF_dtfi_trans=FC_Film_UA(v_TPd_lst,v_TPd,v_FMF);
    v_FMF+=vc_FMF_dtfi_trans;
    
    FC_mi_trig();
    
    
}


//�ȴ������������Ķ���
void Action_ForWaitingMove(void)
{
      //����Ƿ�����ͣ��״̬
      if(!VF_CATCH_CAS)//Wait_till_1stCACome();  //δ�õ���һ���Ǳ�ʱ���жϵ�һ���Ǳ��Ƿ�����
      {   //һֱû�л�ȡ�Ǳ�
          //if(VF_SAMPATIME_STOP)FI_CAS_WhenCANotCaught();   //Կ�׿����󣬽Ǳ��׼������
          
      } else{
          //�Ѿ��г��ֽǱ���
          if(VF_PCAOV_CAS){
              //����Ǳ��������ص�VF_CATCH_CAS=false��״̬
              FI_CAS_WhenCANotCaught();//
              VF_JSTOP_ENG=true;
              v_nb_JSTOP=true;
          } else{
              //��ȡ�Ǳ���
              if(!VF_2NDCA_STOP) {
                  //֮ǰδ��ȡ�ڶ����Ǳ�
                  if(!VF_OLDCA_CAS){
                      //��������Ǳ���
                      //
                      VF_2NDCA_STOP=true;
                      VF_AFRAME_NEED_INTSCI=true;  
                      
                      
                      #ifndef IS_GEN
                          //if(v_prd_cash<M0_START_SPEED&&M0_MAX_START_RPM<=v_prd_cash){
                          if(v_prd_cash<M0_START_SPEED&&PCA1650>=v_prd_cash)//M0_MAX_START_RPM<=v_prd_cash){
                          {
                              //ת���ں���Χ��
                                  //ת����1650����
                                  //���Խ��뷢��������ģʽ��
                              
                              VF_JMMOVE_ENG=true;
                              v_nb_JMMOVE++; 
                          } else{
                             // VF_OLDCA_CAS=false;    
                          }
                          
                      #else
                          //2015-12-11 ���Ӷ�ͨ���Ĵ�����ֹͣ��ʱ�Ǳ���� t_stop_s>=2
                          
                      
                          if(v_prd_cash<M0_START_SPEED&&PCA1650>=v_prd_cash&&t_stop_s>=2)//M0_MAX_START_RPM<=v_prd_cash){
                          {
                              //ת���ں���Χ��
                                  //ת����1650����
                                  //���Խ��뷢��������ģʽ��
                              
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
                  //��ȡ�˵ڶ����Ǳ�
                  //�ȴ��������Ǳ�
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
/*****���ܣ����CANH��CANL�̽�֮��CANͨѶ�ն˵�����****/
void Can_Busoff_Reset(void)
{
	if((CAN1->BIT.RESET)&&(CAN1->BIT.BUSOFF==0))
	{
		CAN1->BIT.RESET=0;
		CAN1->REG.CFG_STAT|=TPSS;
		CAN1->REG.CFG_STAT|=TSSS;
	}
}

