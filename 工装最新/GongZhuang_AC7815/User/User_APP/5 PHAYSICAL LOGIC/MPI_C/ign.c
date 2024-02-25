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
 * V8.1.1	2017.2.15	xielian 	
 *		#if MV_EXHAUSTԤ�����ֶθ���Ϊif.
 * V8.1.2	2017.4.1	xielian 	
 *		���ٻָ����ͺ��һ��Ȧ���ڷſ�������������
 * V8.2.0	2017.4.28	xielian 	
 *		�����ŵ�����߽׶ν�����һ��ʱ���ڵ��Ǽ�СԲ������
 * V8.3.0	2016.11.15	xielian
 *		change highidle taget speed relative to v_tw.
 * V8.4.0	2018.4.2	xielian
 *		��������߷�����ʱ���������ٵ��������
 * V8.4.1   2018.9.26	xielian
 *		�����͸�Ϊ��ƥ�������뽫Ԥ���벿�ִ���������. 
 * V8.5.0   2019.11.11	xielian
 *		�򻯳�ʼ��������Ҫ���������������ϲ���.
 * V8.6.0   2019.12.13	Peter
 *		���Ƽ��ٹ��̵�������ǲ��ܹ�С��С���ŵ���������¶����������ٵ���Ŀ��ת�ٱ���1900.
 ***********************************************************************			
HEAD END*/

#include "Public.h"
#include "events.h"
#include "look_up.h"
#include "status_st.h"

#include "ee.h"     //2008-12-4                          PL150
#include "sci.h"    //2009-12-5 

//2010-4-7 add ��ʱ
#include "time_ccl.h"

#include "afr_cfci.h"


//2010-3-13 add 
#include "data_sys_standard.h"

#include "ad.h"

//2013-7-15
#include "status_eng.h"

#include "DTC.H"


#include "map_selection.h"

#include "add.h"

#include "stepmotor.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_IGN_STATUS;
//#pragma DATA_SEG DEFAULT
word v_tl_ignition;     //���Ƕ�Ӧ��ʱ��=PCA*(m_NBTDC0-v_CA_ignition)/256/2
sbyte v_CA_ignition;    //����	
sbyte v_CA_ignition_lst;    //����	 2010-1-3

byte tps_all_lst;       //2010-1-3
sbyte v_CA_limit_byrpm; //2010-1-3
byte nb_lose_hy;        //��ʧ���صĴ��� 2010-1-6

sbyte v_Ign_I_notidle;   //�ǵ��ٵ����ֵ��

byte nb_MayAirLess; //2010-5-3 

sbyte v_Ign_Map;    //2012-8-22 v_Ign_Map
sbyte v_Ign_UlimitBaseTPS;  //2012-11-2����TPS��õ������ǣ�

sbyte v_ign_idletg;

sbyte ign_lst8run_ave; //ign_lst8run_ave=0��ign_lst8run_sum=0��
byte dRev_forAirLess;               //�����ٱȱ�׼���ٵ��ڸ�ֵʱ����Ϊ������
sbyte ignMax_forAirLess;
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
word tigni;

byte m_nbtdc0;//	equ	0x0d			;�ŵ���Ƕȣ��������ѡ��ǰ�ػ���أ�		

word v_t_cas;//volatile v_t_casSTR _v_t_cas;             //������ʱ�̵�ʱ���׼���������ѡ��ǰ�ػ����

sbyte co_IdleIgn_byRPM;             //os�Ե��ٵ��ǵ�����  co_IdleIgn_byRPM STEP_DIGN_ANN  STEP_DIGN_LEAN_TO_RICH
sbyte co_IdleIgn_byRPM_lst;
sbyte co_IdleIgn_I;//2009-12-28
sbyte co_IdleIgn_mod;   //2010-1-13

byte inc_revobj_worm;       //���ǿ���Ŀ��ת�ٵ�������
sbyte inc_rev_backidle;     //������ʱ��ת��Ŀ�������

word rev15d6_target;//ת��Ŀ��ֵ��2010-7-6
word v_rev15d6tg_nocorr;

byte v_rev15d6_target_idle;//����Ŀ��

sword ign_lst8run_sum;

sbyte ign_step_mode;     //2010-1-11

byte dIgn_perRev_M16;   //2010-1-25

byte t_idleNormal_s;//���������ָ�������ʱ��

byte prd_HYIgn_Chizhi;//PCA2400_2092  ,,2092-1650:  0X23-0X1C=

#define REV_FOR_MUSTFKIGN       77  ////1200:77

sbyte v_igs;
byte v_M;
byte v_O;

byte v_n0_high;
byte v_n0_low;

byte v_J;
byte v_K;

byte v_ccl_inc_ing;

byte ign_nb_backidle;

byte v_n0_low_airless;
byte nb_airrecover;


#define TE_FOR_NOTDELAY 40


/*----------THE FOLLOWING ARE public parameters------------------------------*/
//2012-4-11 �����ǰ�ǵĸ�ԭ������
sbyte vc_Ign_Altitude;//VC_IGN_ALTITUDE

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void hy_sel(void);//word prd_cas,word t_cas0// 2008-5-30 change "byte prd_cash" to "word prd_cas"
void Get_IgnLimit(byte prd_h, sbyte ign_ofMap,word rev_15d6);
void Get_RevTarget(void);//byte tps_all,word v_rev
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
//2009-8-7 add
void FI_Ign_afterReset(void)
{
   
   if(VF_VPOR_HARD)
   {
        V_IGN_STATUS=false;     
        nb_lose_hy=0;
   } else V_IGN_STATUS&=0x10;  //���� VF_QHYCA_INTERR_IGN

   prd_HYIgn_Chizhi=PCA2400_2092; //PCA2092_1650;  //2010-7-1
   
   VF_QHYCA_INTERR_IGN=false;     //2015-3-24
   
}
//********************************
//INITION
//���ܣ��������ĳ�ʼ��
void FI_Ign_BeforeStarts(void)
{
    v_CA_ignition=BYPTR_M0_IGS;
    
    V_IGN_STATUS=false;//2008-12-15
    
    v_tl_ignition=0x300;	//�����ڻ��CA���CA���ڵ�У��


    rev15d6_target=v_n0_low;

    inc_revobj_worm=0;
    co_IdleIgn_I=0;
    co_IdleIgn_mod=0;
    ign_lst8run_ave=0;//2009-12-30
    ign_lst8run_sum=0;
    ign_step_mode=0;
    dIgn_perRev_M16=16;
    v_Ign_I_notidle=BYPTR_M0_CAS_TRAILEDGE;
    
    nb_MayAirLess=0;
    
    v_CA_limit_byrpm=0;
    
    ignMax_forAirLess=BYPTR_M0_CAS_TRAILEDGE;
    v_Ign_UlimitBaseTPS=BYPTR_M0_CAS_FRONTEDGE;
	
	ign_nb_backidle=0;
	nb_airrecover=0;
    
}

//**************************************************************
//���ܣ��ڴ������֮ǰ���ж�����ǵ��٣���Ե��ǽ���������
//�βΣ� rev156--v_rev_15d6rpm;
//       tps_odr--v_TPS_odr;
//��ת�ٵ���3000rpmʱ���ڽǱ굽��֮�󣬵��֮ǰ���ú���������//2008-12-8 ��ע
void FC_Idle_Ign_BeforeTrig()//2010-1-25 add 
{//word rev156,byte tps_odr,byte tps_mode,sword tes,word rev3d9,byte rev156ave_16
////v_rev_15d6rpm,v_TPS_odr,v_TPS_mode,V_Tw,v_rev_3d9,v_rev_156ave

    sbyte sb;

    sword sw;

    byte blnIniCondiction;//M0_CAS_TRAILEDGE
    
    if(VF_1S_TMCCL&&(t_idleNormal_s<255))t_idleNormal_s++;

    if(v_ccl_ci<7)return;//2017-3-29

    if(REMARK==MC_IGNITION&&KESEI==2){
        return;//2013-9-2����ģʽȡ�����ٵ����
    }

     if(VF_E0_TPS&&VF_BFIRE_ENG) 
     {                              
        
        sw=v_J+v_K*(sword)V_Tw/16;
        if(sw<2)sw=2;
        if(sw>30)sw=30;
        dRev_forAirLess=sw;
        
        
        /*���㵡�ٶԵ��ķ���ֵ*/
        sw=rev15d6_target-(sword)v_rev_15d6rpm;
        
        if(V_Tw<=25)dIgn_perRev_M16=BYPTR_M0_DN_25;
        else if(V_Tw>=153)dIgn_perRev_M16=BYPTR_M0_DN_153;
        else {
          dIgn_perRev_M16=
              (((sword)BYPTR_M0_DN_153-BYPTR_M0_DN_25)*(V_Tw-25))/128
              +BYPTR_M0_DN_25;    
        }

        /**/
		//#ifndef SYS_STEPMOTOR
			sw=dIgn_perRev_M16*sw/16;
		
			if(sw>20)sw=20;     //2010-1-13 from 45 to 15
			if(sw<-20)sw=-20;   //2010-1-13 from -30 to -15
		/*#else
			if((sw>1)||(sw<-1))	//2018-3-28 ��������������ڳ�ͻ,����ת������
			{
				sw=dIgn_perRev_M16*sw/32;	//16;
				if(VF_CICORECING_CFCI||(v_t_afterbackidle_d1s<50))
				{
					sw=sw*2;
					if(sw>20)sw=20;     
					if(sw<-20)sw=-20;   
				}
				else
				{
			
					if(sw>5)sw=5;     //2016-11-29 ������IACV���ƣ�Ϊ��������IACV�����ظ������������ֵ��С
					if(sw<-5)sw=-5;  
				}
			}
			else
			{
				sw=0;
			}
		#endif*/

		
        if(VF_REVTARGET_INC_IGN)
        {
            if(sw<-7)sw=-7;
			v_ccl_inc_ing=0;
        }
		else
		{
			if(v_ccl_inc_ing<255)v_ccl_inc_ing++;
		}				
   
		co_IdleIgn_byRPM=sw;
		
		//2017-4-27 �Ե�����Ƶ��л�����Բ��������VF_REVTARGET_INC_IGN��λ��һ��Ȧ�����ڣ�Ȧ�����˹���������������������Լ�����������
		if((v_ccl_inc_ing<30)&&(v_ccl_inc_ing>0)&&(co_IdleIgn_byRPM<co_IdleIgn_byRPM_lst))
		{
			if(VF_RUN4_TMCCL)
			{
				co_IdleIgn_byRPM=co_IdleIgn_byRPM_lst-1;				
			}
			else
			{
				co_IdleIgn_byRPM=co_IdleIgn_byRPM_lst;
			}
		}
		
		co_IdleIgn_byRPM_lst=co_IdleIgn_byRPM;

        /*���㵡�ٶԵ��ķ���ֵ�Ļ���*/
        if((!VF_ENAQ_SYSINI)||VF_IDLEAIRTEST_SYSINI||v_rev_15d6rpm<REV_FOR_MUSTFKIGN)     //2010-1-6 add ת��
        {                                                   //1150rpm:74
           /* #if M0_MFUD1==INJ109
                //2014-1-13 add condiction  delete &&VF_E0MORETHAN4S_TPS
                //δ���е��ٳ�ʼ�������ߵ��ٳ�ʼ��ʱ���������жϣ���ת�ٵ������ƺ���Ҫ���е����
                if(!VF_STARTS_ENG&& ( ((inc_rev_backidle<=1))||(v_rev_15d6rpm<v_n0_low) ))
                {              //2010-1-14 add rev_target_inc_backidle==0
                    if(co_IdleIgn_byRPM>0)
                    {   //2010-6-26 changfrom ++ to the following;
                        co_IdleIgn_mod+=4;
                        if(v_rev_15d6rpm<v_n0_low)
                        {
                            sb=v_n0_low-(sbyte)v_rev_15d6rpm;
                            if(sb>16)sb=16;
                            co_IdleIgn_mod+=sb;
                        }
                    }
                    else if(co_IdleIgn_byRPM<0)co_IdleIgn_mod-=2;//2010-6-26 chang from -- to -=2

                    if(co_IdleIgn_mod>=16||co_IdleIgn_mod<-16)   //2010-9-17  �����ԭ�ȡco_IdleIgn_mod=-16ʱ���cco_IdleIgn_I������������
                    {
                        co_IdleIgn_I+=co_IdleIgn_mod/16;
                        if(co_IdleIgn_mod>0)co_IdleIgn_mod=co_IdleIgn_mod&0x0f;  // ????
                        if(co_IdleIgn_mod<0)co_IdleIgn_mod=co_IdleIgn_mod|0xf0;  // ????
                    }
                     
                    if(co_IdleIgn_I>20)co_IdleIgn_I=20; //2010-6-26 chang from 5 to 20
                    if(co_IdleIgn_I<-40)co_IdleIgn_I=-40;
                }
                v_CA_ignition+=co_IdleIgn_byRPM;//+co_IdleIgn_I;        

           #else*/
                //δ���е��ٳ�ʼ�������ߵ��ٳ�ʼ��ʱ���������жϣ���ת�ٵ������ƺ���Ҫ���е����
                if(!VF_STARTS_ENG&& ( ((inc_rev_backidle<=1)&&VF_E0MORETHAN4S_TPS)||(v_rev_15d6rpm<v_n0_low) ))
                {              //2010-1-14 add rev_target_inc_backidle==0
                    if(co_IdleIgn_byRPM>0)
                    {   //2010-6-26 changfrom ++ to the following;
                        co_IdleIgn_mod+=4;
                        if(v_rev_15d6rpm<v_n0_low)
                        {
                            sb=v_n0_low-(sbyte)v_rev_15d6rpm;
                            if(sb>16)sb=16;
                            co_IdleIgn_mod+=sb;
                        }
                    }
                    else if(co_IdleIgn_byRPM<0)co_IdleIgn_mod-=2;//2010-6-26 chang from -- to -=2

                    if(co_IdleIgn_mod>=16||co_IdleIgn_mod<-16)   //2010-9-17  �����ԭ�ȡco_IdleIgn_mod=-16ʱ���cco_IdleIgn_I������������
                    {
                        co_IdleIgn_I+=co_IdleIgn_mod/16;
                        if(co_IdleIgn_mod>0)co_IdleIgn_mod=co_IdleIgn_mod&0x0f;  // ????
                        if(co_IdleIgn_mod<0)co_IdleIgn_mod=co_IdleIgn_mod|0xf0;  // ????
                    }
                     
                    if(co_IdleIgn_I>20)co_IdleIgn_I=20; //2010-6-26 chang from 5 to 20
                    if(co_IdleIgn_I<-40)co_IdleIgn_I=-40;
                }
                v_CA_ignition+=co_IdleIgn_byRPM;//+co_IdleIgn_I;        

          // #endif
        } 
        else 
        {   //�����е���������ǲ���ƽ��ֵ
            co_IdleIgn_byRPM=co_IdleIgn_byRPM/4;   //2010-1-4
            if(co_IdleIgn_byRPM>4)co_IdleIgn_byRPM=4;
            if(co_IdleIgn_byRPM<-4)co_IdleIgn_byRPM=-4;
            v_CA_ignition=ign_lst8run_ave+co_IdleIgn_byRPM;
        }

        
        VF_IDLEIGNFK_IGN=true;//2009-12-18
        
        /*���Ƶ����ں���Χ֮��*/
            if(v_CA_ignition<v_CA_limit_byrpm)v_CA_ignition=v_CA_limit_byrpm; //2010-1-3
            if(v_CA_ignition>BYPTR_M0_IGNMAX_IDLE)v_CA_ignition=BYPTR_M0_IGNMAX_IDLE;//;

            //2015-3-24 ADD ����������أ����������֮�����ϵ��
            if(v_CA_ignition>=M0_CAS_TRAILEDGE)
            {
                VF_QHYCA_INTERR_IGN=true;        
            }

        /*���㵡�ٵ��ǵ�ƽ��ֵ*/
            //2010-11-30   ign_lst8run_ave�ļ���λ�ò��� ,�ƶ�����
            if(VF_E0_TPS)
            {
                if((VF_NOTAQING_IDLEAQ&&!VF_ENAQ_SYSINI)||VF_IDLEAIRTEST_SYSINI||v_rev_15d6rpm<REV_FOR_MUSTFKIGN)  //2010-1-5 ADD CONDICTION
                {
                    ign_lst8run_sum=ign_lst8run_sum+v_CA_ignition-ign_lst8run_ave;
                    ign_lst8run_ave=ign_lst8run_sum/32; //2010-2-6 from 8 to 32
                }
            }
        
        
        /*���������ж�*/
            //if((v_CA_ign_TransAfterStarts<=BYPTR_M0_IGNMIN_IDLE&&VF_E0MORETHAN4S_TPS&&(inc_rev_backidle==0))
            //    &&((VF_NOTAQING_IDLEAQ&&!VF_ENAQ_SYSINI)||VF_IDLEAIRTEST_SYSINI)
            if((VF_E0MORETHAN4S_TPS)
                &&((VF_NOTAQING_IDLEAQ&&!VF_ENAQ_SYSINI)||VF_IDLEAIRTEST_SYSINI)
                &&(!VF_REVTARGET_INC_IGN)
            )      
            {
                //2010-2-6
                if(!VF_STARTS_ENG)
                {
                    blnIniCondiction=VF_1ST_INI_SYSINI&&VF_AQAFTER_PNINI_SYSINI;//(VF_AQAFTER_PNINI_SYSINI&&(VF_IDLEAIRTEST_SYSINI||VF_O2CHANGED_OSDIAG||VF_AQOK_SYSINI)); //�ڳ�ʼ��״̬�� //2010-3-13 add condiction &&(MV_EXHAUST<=PL150)
                    //�ڷǳ�ʼ��״̬��
                    {

                        if( BYPTR_MV_EXHAUST>PL150)   //2010-11-30 PL>150�ĳ������ж�����ʱ�������Ǹߵ���Ŀ�ꣻ
                        {
                          
                            sw=((sword)v_rev_156ave+dRev_forAirLess);   //2010-11-30
                            //2010-11-30
                        } 
                        else
                        {
                            sw=((sword)v_rev_156ave+dRev_forAirLess-inc_revobj_worm);   //2010-7-6 -inc_revobj_worm
                        }
                        
                        //������ڵ��ٳ�ʼ��״̬��
                        if(blnIniCondiction)
                        {
                            if(!VF_IDLEAIRLESS_IGN)
                            {
                            #ifdef SYS_STEPMOTOR
								if(v_step_sm==STEP_SM_MAX)									
							#endif								
								{
                                	if((ign_lst8run_ave>ignMax_forAirLess)&&(v_rev_156ave<(v_n0_low_airless+5))) //2013-1-14 DELETE=
                                	{  
                                    	if(nb_MayAirLess>100)
                                    	{
                                        	VF_IDLEAIRLESS_IGN=true;    
                                    	} else nb_MayAirLess++;
										t_idleNormal_s=0;
									}else nb_MayAirLess=0;
                                } 
                            }
                            //2010-2-4 ���ĵ��ٵ�ָʾ�ı�׼��
                            else
							{
                                if(v_rev_156ave>(v_n0_low_airless+10))
								{
									if(nb_airrecover<=100)nb_airrecover++;
								}
								else
								{
									if(nb_airrecover>0)nb_airrecover--;
								}
															
								if((ign_lst8run_ave<(ignMax_forAirLess-5))||(nb_airrecover>100))
								{
									VF_IDLEAIRLESS_IGN=false;//2009-7-9 add
									nb_airrecover=0;
								}
                            }
                        } 
                        else{

                            if(VF_IDLEAIRLESS_IGN) //2010-7-6 add blnIniCondiction
                            {
                                    if((ign_lst8run_ave<ignMax_forAirLess-5)
                                        &&(sw>v_n0_low_airless+2)  //2010-5-5 for xy, chang from M0_N0_LNOISCV_15D6RPM to 93
                                    )
                                    {
                                        if(t_idleNormal_s>=5)VF_IDLEAIRLESS_IGN=false;
                                        
                                    }   
                            }
                            else
                            {   //���������ж�������
                                //1��������û����ߣ��ҵ��Ǵ�����ֵ����ת�ٱȣ�Ŀ��ת��+156����
                                //2����ת��ƽ��ֵ��Ŀ��ת�ٵ���һ����ֵ��
								
                                if((((ign_lst8run_ave>ignMax_forAirLess)&&(v_rev_156ave<v_n0_low_airless+10))          //2010-7-6 ���Ǵ������е��������������
                                    ||(sw<v_n0_low_airless-1))) //const byte M0_N0_LNOISCV_15D6RPM    //2010-5-5 for xy, chang from M0_N0_LNOISCV_15D6RPM to 93
                                {                                    //2013-1-14 DELETE=
                                #ifdef SYS_STEPMOTOR
									if(v_step_sm==STEP_SM_MAX)									
								#endif
									{
									if(VF_15S_AFTERSTART_TMCCL)
                                    {
                                        //2010-5-3 add nb_MayAirLess;
                                        if(nb_MayAirLess>100)
                                        {
                                            VF_IDLEAIRLESS_IGN=true;    
                                        } else nb_MayAirLess++;
                                        t_idleNormal_s=0;
                                    }
									}
                                } else nb_MayAirLess=0;
                            }
                        }
                    }
                } else VF_IDLEAIRLESS_IGN=false;
            }
            v_CA_ignition_lst=v_CA_ignition;//2010-7-6 FOR LEAVE IDLE
     } 
     else
     {
        t_idleNormal_s=0;
     }

    //VF_IDLEAIRLESS_IGN=false;//2012-12-8
     
    return;
}

//***************************************************************
//���ܣ����ݵ��Ǽ�����ʱ�̣�������
//����ֵ����Ӧ������ʱ���ģ������ڿ�ʼ���֮���ʱ�䳤��
//�βΣ� t_cas0 --v_t_cas0
//       prd_cash--v_prd_cash
//       prd_cas--v_prd_cas
//word F_Cal_T_Ign(word t_cas0,byte prd_cash,word prd_cas) //v_t_cas0,v_prd_cash,v_prd_cas
word F_Cal_T_Ign(void) //v_t_cas0,v_prd_cash,v_prd_cas
{
    byte b;
    word w; 
    word w1;
    
    //2012-11-2
//    #if ECU_TYPE==ECU_GEN
    #ifdef IS_GEN
    
        //2014-4-27 �������ƣ�
        if(v_CA_ignition>v_Ign_UlimitBaseTPS)v_CA_ignition=v_Ign_UlimitBaseTPS;
    #endif

    //2014-4-17 �Ƴ��Ժ��صĵȴ�
      
    if(!VF_BFIRE_ENG)v_CA_ignition=BYPTR_M0_IGS;                                               

    //-----STIGNI_NEW_C--------------------------------------------
    //prd_cas=v_prd_cas;//2008-7-12 ��������ʱǰ���ؽǱ��໥У��֮��,����   2013-6-3 delete

    
    if(m_nbtdc0>v_CA_ignition)b=m_nbtdc0-v_CA_ignition;else b=0;
    if(b>0)
    {
        v_tl_ignition=(word)(((unsigned long int)v_prd_cas*b)/512);//(word)((prd_cas*b)/512);
        //�Ǳ굽���ʱ�̵�ʱ��,=(�Ǳ�Ƕ�-����)*�����Ǳ�֮���ʱ��/360+�Ǳ�ʱ��,�Ƕȵ�λ0.7CA
		
		tigni=v_t_cas +v_tl_ignition;//���Ƕ�Ӧ��ʱ��
    }
    else
    {
        tigni=v_t_cas+2;
        v_tl_ignition=2;
    }
    
    v_t_bIgn_toCas=v_tl_ignition;
    v_t_Cas_trigIgn=v_t_cas;
    
    //------TIGNI_TESTS();--------------------------------------
    w1=T_MAINTMR;
    if(((tigni<(unsigned long int)w1+2&&tigni>v_t_cas)&& w1>=v_t_cas)//�Ӳ�׽�Ǳ꿪ʼ��������ʱ��û�����
        || ((v_t_cas>w1)&& (tigni>v_t_cas||tigni<(unsigned long int)w1+2)))                  //�Ӳ�׽�Ǳ꿪ʼ��������ʱ�������
    {   //����������£�Ӧ��������� //�����ǰ���ǣ�����ӽǱ겶��ʼ����ǰ��ʱ�䲻�ᳬ������ʱ����һ������
        w=2;
    }
    else
    {
        w=tigni-w1;//��ǰʱ�̵����ʱ�̵�ʱ��
    }
    if(w<=4)
    {
        w=4;
        //CDI_ON; //2008-8-7 remove ,��Ϊ�����ܻᵼ�º���û�г��Ķ�����?
    }
    
    w1=v_prd_cas/6;
    if(w>w1)//2008-3-19 ���� ����CA_Toospeed��λ�����ڴ�������ʱ�̼������������ɵģ���˽�����ֵ��
    {
        w=w1;
            //2008-3-19 ���ֹ�һ��  ,������ֵ��Ч��
    }

    //----PreditIgnTimeForNextCycle:--------------------
    tigni+=v_prd_cas;
   
    return w;
}

//************FG_CA_Ign_basic****************************************
//���ܣ���û������ǲ�����?
//�βΣ� t_eng_s --v_t_EngMove_s
//       teng_odrh5--v_teng_odrh5
//       teng-v_teng_inuse
//       pcbtdc--v_PCBTDC_SCI
//       rev_odr
//       rev_mode
//       tps_odr
//       tps_mode
//void FG_CA_Ign_basic(sword stw,byte pcbtdc,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode,word v_rev,byte remark,byte prd_cash,byte TPd)
void FG_CA_Ign_basic(void)
//V_Tw,v_PCBTDC_SCI,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,v_rev_15d6rpm,REMARK,v_prd_cash,v_TPd
{


    sbyte dIgn;
    sword stw;
    sword sw;
    byte tpd;
    
        VF_NOTDELAY_IGN=true;//2010-1-26

    /*------------------*/
        v_O=Adj_byte(BYPTR_M0_O,ADJ_O);
        v_M=Adj_byte(BYPTR_M0_M,ADJ_M);
		/*
        #ifdef SYS_STEPMOTOR
		sw=V_Tw;
		if(sw>125)sw=125;//2016-12-8 ��������
		v_n0_high=Adj_byte((byte)F_2DLookUpSbyte((sbyte)sw,(char_P)BYPTR_M1_DN0_HTOLNOISCV_15D6RPM,7),ADJ_n0_high);//2016-10-25 ����Ϊ1D
        #else*/
		v_n0_high=Adj_byte(BYPTR_M0_DN0_HTOLNOISCV_15D6RPM,ADJ_n0_high);//2015-10-27
		//#endif
		v_n0_low=Adj_byte(BYPTR_M0_N0_LNOISCV_15D6RPM,ADJ_n0_low);//2015-10-27   //2015-12-10 ����
		
		v_n0_low_airless=v_n0_low;
		
		if(VF_1ST_INI_SYSINI&&!VF_OSER_OSDIAG)//&&VF_AQAFTER_PNINI_SYSINI
		{
			v_n0_low=122;
		}

        v_J=Adj_byte(BYPTR_M0_J_M4,ADJ_j);
        v_K=Adj_byte(BYPTR_M0_K_15D6RPM,ADJ_k);
        
    


    /*����ȡmap���ǡ�*/
        
        if(VF_UB_TPS)tpd=16;else tpd=v_TPS_odr;//2015-12-13
        v_Ign_Map=v_CA_ignition=//(byte)FG_Map_basic(TIGN,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode);
            //(byte)FG_3DMap(BYPTR_M2_IGN_TPSRPM,v_rev_odr,v_rev_mode,tpd,v_TPS_mode,IGN_X_UB,IGN_Y_UB,IGN_XMODE_NUM,IGN_YMODE_NUM,false,false);
	  	      (byte)FG_3DMap_xyz((byte *)BYPTR_M2_IGN_TPSRPM,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB,(byte *)BYPTR_TPD_M2LDB,15,16,0x00);//2017-1-21
        
        //2015-10-27
        v_Ign_Map=Adj_sbyte(v_Ign_Map,ADJ_Igb);

		v_CA_ignition=v_Ign_Map;
       //2017-1-17
        if(Is_RunPointMatch(1))
        {
            if(v_Ign_Map<21)v_CA_ignition=v_Ign_Map+10;//2017-1-17    
        }

     /*   
        //2012-11-2���Ӷ������ǵ�����
        if(VF_WOT_TPS)v_Ign_UlimitBaseTPS=M1_IGN_ULIMIT[16];
        else v_Ign_UlimitBaseTPS= M1_IGN_ULIMIT[v_TPS_odr];
     */  
        
        //2015-12-11 ʹ��ת����ߵ���һ����Ϊ��𽻵�����
        
      #ifdef IS_GEN
          //v_Ign_UlimitBaseTPS=*(byte *)( (BYPTR_M2_IGN_TPSRPM)+v_rev_odr+(word)v_TPS_odr*16);
          v_Ign_UlimitBaseTPS=*(byte *)( (BYPTR_M2_IGN_TPSRPM)+15+(word)v_TPS_odr*16);
      #endif
        
//    #if ECU_TYPE==ECU_MOT
      #ifndef IS_GEN

        
            stw=v_te;//2013-4-30
            
            #ifdef	TE_10K	//20210728 chenwei add 10K����
            if(stw>125)stw=125;
			#endif

			#ifdef	TE_2K	//20210728 chenwei add 2K����
            if(stw>80)stw=80;
			#endif

			
            dIgn=F_2DLookUpSbyte((sbyte)stw,(char_P)BYPTR_M2_IGT,9);
    
    /*����ȡ���ٵ��ǵ����� ����׼���ٵ��ǡ�*/
        //if(VF_E0_TPS)
        if(VF_E0_TPS&&!VF_IGNONLY_CFCI)//2014-12-9
        
        {
            if(v_ccl_ci>10) //2017-3-29
            {
              
                if(v_Ign_Map>BYPTR_M0_IGNMAX_IDLE)v_Ign_Map=BYPTR_M0_IGNMAX_IDLE;//2013-1-5 //2014-2-14 FROM 10 TO M0_CAS_TRAILEDGE
            }

            v_Ign_Map+=dIgn;
			
			v_ign_idletg=v_Ign_Map;
            
            
            /*ȷ��������������ʱ�����ǵı�׼*/ //2012-12-5
            //if(v_te>80)ignMax_forAirLess=v_Ign_Map+7;//��������¶ȳ���80����ignMax_forAirLessΪĬ��ֵ��

            /*ȷ��������������ʱ�����ǵı�׼*/ //2012-12-5
            //2013-4-29
            //ignMax_forAirLess=v_Ign_Map+3;//��������¶ȳ���80����ignMax_forAirLessΪĬ��ֵ��
            ignMax_forAirLess=v_Ign_Map;// 2013-4-29 delete +3;//��������¶ȳ���80����ignMax_forAirLessΪĬ��ֵ��

            if(!VF_1ST_INI_SYSINI)ignMax_forAirLess+=10;
            //2013-4-24
            //if(ignMax_forAirLess>M0_CAS_TRAILEDGE)ignMax_forAirLess=M0_CAS_TRAILEDGE;
            //2014-2-17 CHANGE FROM M0_CAS_TRAILEDGE TO BYPTR_M0_IGNMAX_IDLE
            
            if(v_ccl_ci>10)
            {
                if(ignMax_forAirLess>BYPTR_M0_IGNMAX_IDLE)ignMax_forAirLess=BYPTR_M0_IGNMAX_IDLE;
            }

            v_Ign_I_notidle=co_IdleIgn_I+10+dIgn;    
        }
    
    /*��ȡĿ��ת��*/
        Get_RevTarget();//v_TPd,v_rev_15d6rpm
    
      v_igs=Adj_sbyte(BYPTR_M0_IGS,ADJ_igs);
      //---------rec_gin_withrzt--------------------------------------------//
      if(!VF_BFIRE_ENG)
      {
          v_CA_ignition_lst=v_CA_ignition=v_igs;
          tps_all_lst=v_TPd;

      } 
      //else if(VF_E0_TPS&&(!VF_ER_TPS)) //2009-9-4 ADD VF_NEEDRECBYLOWRPM_IGN //2010-2-6 add (!VF_ER_TPS)
      else if(VF_E0_TPS&&(!VF_ER_TPS)&&!VF_IGNONLY_CFCI) //2014-12-9
      {
          #ifdef SYS_STEPMOTOR
			  v_CA_ignition=v_Ign_Map;
		  #else
			  v_CA_ignition=v_Ign_Map+co_IdleIgn_I;//2008-12-7 ADD    //2009-12-30 MOVE co_IdleIgn_I HERE
		  #endif
          

          VF_NOTDELAY_IGN=false;//2010-1-26 
          
      } 
      else
      {   //���ٵ��ǿ��ƶԷǵ��ٲ��ֹ�����xiuzheng 
          /*
          if((VF_TRANSLT10_TRANS&&(!(VF_ACCMID_TRANS&&VF_ACCMIDLST_TRANS)&&!VF_ACCSPD_TRANS))
                  ||VF_ER_TPS
              )//2010-2-5 VFF_TPS_DTC
          {                                            //2010-1-11 ���ּ��ٲ��ã���������
              if(REMARK!=MC_OPENLOOP)
              {
                  if((tps_odr<3)||(VF_ER_TPS)) //2010-1-9 from 2 to 3 
                  {
                      sw=rev15d6_target-(sword)v_rev; 
                      if(sw<-70)sw=-70;
                      if(!VF_ER_TPS)
                      {
                          if(sw>5)sw=5;
                      } else
                      {
                          if(sw>60)sw=60;
                      }
                      if(v_Ign_I_notidle<sw)v_Ign_I_notidle=sw;
                      else if(v_Ign_I_notidle>sw)v_Ign_I_notidle--;
               
                      if(v_Ign_I_notidle<-2)VF_NOTDELAY_IGN=false;//2010-1-26
                      v_CA_ignition+=v_Ign_I_notidle; 
                  }
              } else v_Ign_I_notidle=0;
              //if(v_CA_ignition<0)v_CA_ignition=0; //2010-1-2
          } else 
          {
              v_Ign_I_notidle=0;//�ǵ��٣����ſ춯ʱ��λ������mapֵ��
          }   
          *///2012-12-20 delete
          //v_Ign_I_notidle=0;
          
          VF_IDLEAIRLESS_IGN=false;

          nb_MayAirLess=0;//2010-5-4
      }


      /*С����ʱ���ǵĹ��ɴ���*/
      //#if MV_VEHICLE!=SCOOTER

      //2017-1-21
      if(BYPTR_M0_EN_IGN_FK_PARTLOAD==EN_IGN_FK_PARTLOAD_FBCTR)
	  {
        
          if((REMARK!=MC_IGNITION)&&(v_te>TE_FOR_NOTDELAY)) //2014-3-12
          { //2013-4-10 ADD CONDICTION 
            
            
              if(!Is_RunPointMatch(1))//2017-1-17 add  
              {
                  if(!VF_E0_TPS&&(v_TPS_odr<=4)&&(!(VF_ACCMID_TRANS&&VF_ACCMIDLST_TRANS)&&!VF_ACCSPD_TRANS)&&(!VF_JCI_CFCI))//2010-1-11 ���ּ��ٲ��ã�����
                  {
                      //2013-4-10
                      //v_CA_ignition=((80-v_TPd)*(sword)v_Ign_I_notidle+(sword)v_CA_ignition*v_TPd)/80;
                                       //  v_CA_ignition+=(sbyte)(((co_IdleIgn_I+dIgn)*(sword)(80-v_TPd))/80) ;
                                         v_CA_ignition+=(sbyte)(((co_IdleIgn_I/2+dIgn)*(sword)(80-v_TPd))/80) ;//2019-12-9

                  }
              }
            
     
          }
      }
     /*
         // #else
              //����̤�峵 2013-8-5 add ignition leave idle for SCOOTER
              
              if(v_Ign_I_notidle<0)v_Ign_I_notidle=0;
              if(REMARK==MC_NORMAL)
              { //2013-4-10 ADD CONDICTION 
                  if(!VF_E0_TPS&&(v_TPS_odr<=4)&&(!(VF_ACCMID_TRANS&&VF_ACCMIDLST_TRANS)&&!VF_ACCSPD_TRANS)&&(!VF_JCI_CFCI))//2010-1-11 ���ּ��ٲ��ã�����
                  {
                      //2013-4-10
                      //v_CA_ignition=((80-v_TPd)*(sword)v_Ign_I_notidle+(sword)v_CA_ignition*v_TPd)/80;
                                         v_CA_ignition+=(sbyte)(((co_IdleIgn_I+dIgn)*(sword)(80-v_TPd))/80) ;

                  }
              }
          //#endif
       */
      tps_all_lst=v_TPd;


      //2013-7-15
        //2013-7-8 PL150���ϵĵ������
      if( BYPTR_MV_EXHAUST<=PL50){
          if(v_rev_625rpm>REV62D5_4000&&!VF_IGNONLY_CFCI&&v_TPS_odr>=1){ //2013-8-28 CHANGE FROM 5 TO 1
              //2013-7-12
              if(v_rev_625rpm>v_rev62d5rpm_cf-8)   //152--9500 rpm yishang    12-for 750rpm
              {
              
                    sw=(sword)v_rev_625rpm-v_rev62d5rpm_cf+8;  //8--for 500rpm
                    
                    //2013-9-5 FROM 4 TO 6
                    sw*=6;
                    if(sw>48)sw=48;
                    
                    sw=-sw;
                    stw=v_CA_ignition+sw;  
                    //stw-=10;
                    if(stw<0)stw=0;     
                   
                   //2013-8-31
                   if(VF_RUN8_TMCCL){
                        if(stw<v_CA_ignition_lst)v_CA_ignition=v_CA_ignition_lst-1;
                        else if(stw>v_CA_ignition_lst)v_CA_ignition=v_CA_ignition_lst+1;
                        else v_CA_ignition=v_CA_ignition_lst;
                   } else{
                        v_CA_ignition=v_CA_ignition_lst;
                   }
                   
                   
              } 
          }

      } else{ 
	  //if MV_EXHAUST>PL150
          if( BYPTR_MV_EXHAUST<=250){
			  //2014-2-14 add ����������500rpm
              if(v_rev_625rpm>REV62D5_8000&&!VF_IGNONLY_CFCI&&v_TPS_odr>=5){
                  //2013-7-12
                  if(v_rev_625rpm>v_rev62d5rpm_cf-8)   //152--9500 rpm yishang    12-for 750rpm
                  {
                       //if(v_rev_625rpm<v_rev62d5rpm_cf)
                       
                       if(v_rev_625rpm>v_rev62d5rpm_cf-4){
                            v_CA_ignition=7;
                       
                       } else
                       {
                            sw=(sword)v_rev_625rpm-v_rev62d5rpm_cf+4;  //8--for 500rpm
                            
                            //sw*=4;//32*0.7 for 500rpm
                            //if(sw<-32)sw=-32;
                            //sw+=32;
                            
                            sw*=4;
                            if(sw<-20)sw=-20;
                            sw+=20;
                            
                            sw=-sw;
                            v_CA_ignition+=sw;  
                            v_CA_ignition-=20;
                       }
                       
                  } 
              }

          } else{    
              if(v_rev_625rpm>REV62D5_8000&&!VF_IGNONLY_CFCI&&v_TPS_odr>=5){
                  //2013-7-12
                  if(v_rev_625rpm>v_rev62d5rpm_cf-16)   //152--9500 rpm yishang    12-for 750rpm
                  {
                       //if(v_rev_625rpm<v_rev62d5rpm_cf)
                       
                       if(v_rev_625rpm>v_rev62d5rpm_cf-8){
                            v_CA_ignition=7;
                       
                       } else
                       {
                            sw=(sword)v_rev_625rpm-v_rev62d5rpm_cf+4;  //8--for 500rpm
                            
                            //sw*=4;//32*0.7 for 500rpm
                            //if(sw<-32)sw=-32;
                            //sw+=32;
                            
                            sw*=2;
                            if(sw<-20)sw=-20;
                            sw+=20;
                            
                            sw=-sw;
                            v_CA_ignition+=sw;  
                             v_CA_ignition-=20;
                       }
                       
                  } 
              }
		  }
	  }
    
    #endif        
    
    //------------------------------------

        Get_IgnLimit(v_prd_cash,v_Ign_Map,v_rev_15d6rpm);
    //------------------------------------
    //2010-7-6 ������ǵ��ٷ����õĻ���ֵ����ת����أ���������أ�
    //����Ӧ��������Ǳ�mapֵ��
    
    
    /*���ǵı߽��жϺ��쳣����*/
        if(v_CA_ignition<v_CA_limit_byrpm)v_CA_ignition=v_CA_limit_byrpm;
        if(v_CA_ignition>BYPTR_M0_CAS_FRONTEDGE)v_CA_ignition=BYPTR_M0_CAS_FRONTEDGE;       //;

        //if(VF_BOC_TPS||VF_HOLD_TPS)v_CA_ignition=0;//2011-1-18   2013-4-14 delete

        v_CA_ignition_lst=v_CA_ignition;
        
        //2010-2-6
        if(VF_ER_TPS)
        {
            if(v_CA_ignition>v_Ign_Map+15)v_CA_ignition=v_Ign_Map+15;
        }
    //2012-11-2
//    #if ECU_TYPE==ECU_GEN
    #ifdef IS_GEN
    
        //2014-4-27 ��������
        if(v_CA_ignition>v_Ign_UlimitBaseTPS)v_CA_ignition=v_Ign_UlimitBaseTPS;
    #endif
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

//2010-7-6
//��õ��ǵ���ֵֵ
//(v_CA_limit_byrpm>BYPTR_M0_IGNMIN_IDLE)
void Get_IgnLimit(byte prd_h, sbyte ign_ofMap,word rev_15d6)
{
    sword sw;

    sw=(sword)rev_15d6-v_n0_low;

    if(sw<0)sw=0;
    
    //sw=BYPTR_M0_M+(BYPTR_M0_O*sw)>>7;
    //2014-5-17 �������£�
    sw=v_O+((v_M*sw)>>7);//���ֽ��治�䣬�ڳ�������ߵ���һ��
    
    if(sw>90)sw=90;
	#ifndef SYS_STEPMOTOR
    	v_CA_limit_byrpm=sw;
    	//if(v_CA_limit_byrpm<0)v_CA_limit_byrpm=0;
    	v_CA_limit_byrpm+=BYPTR_M0_IGNMIN_IDLE; 
	#else
		v_CA_limit_byrpm=BYPTR_M0_IGNMIN_IDLE;	
	#endif
	//2019-12-9 ���ӣ�����С������ʱ����С����
	if(v_te<TE_FOR_NOTDELAY) 
	{
	    if(v_TPd>=14&&v_TPd<64)
	    {
    	    if(v_CA_limit_byrpm<20)v_CA_limit_byrpm=20 ;
    	    v_CA_limit_byrpm+=(v_TPd-14)/2;
	    }
	      
	    	      
	}
	
	
	
	#ifndef SYS_STEPMOTOR
	if(VF_IDLEFUELCO_ST)
	{		
		ign_nb_backidle=7;
	}
    else
	{
		if(VF_RUN8_TMCCL)
		{
			if(ign_nb_backidle>0)ign_nb_backidle--;
		}		
		
	}
	v_CA_limit_byrpm=v_CA_limit_byrpm-ign_nb_backidle;
	#endif
	
    if(!VF_E0_TPS)
    {
        if(v_CA_limit_byrpm>ign_ofMap)v_CA_limit_byrpm=ign_ofMap;    
        if(v_CA_limit_byrpm>35)v_CA_limit_byrpm=35;//��ӦԼΪ25�ȣ�
    }

}
/*
    ��ȡ���ŵ���ʱ��Ŀ��ת��
*/
// rev15d6_target
void Get_RevTarget(void)//byte tpd, word v_rev    //v_TPd,v_rev_15d6rpm
{
        //if(rev156>M0_N0_LNOISCV_15D6RPM+INC_REVOBJ_FORWARMUP)//2010-6-26 delete�ӿ�rev_target_inc_backidle˥���ٶ�
    sword sw;
    sword s_target;

    /*���Żص��ٵĴ���*/
        if((VF_JCI_CFCI&&VF_E0_TPS)||!VF_E0_TPS)
        {
            sw=(sword)v_rev_15d6rpm-v_n0_low;          //VF_JCI_CFCI
            //sw/=4;  //2012-12-6 �����Ǻ��ˣ����ǵ�������
            //sw/=2;  //2012-12-6 �����Ǻ��ˣ����ǵ�������  //2012-12-8 ��ֹ������ڣ���ʱɾ��
            
            if(sw<0)sw=0;
            //#if (MV_VEHICLE== HIDER)||(MV_VEHICLE== WILD)
            if(BYPTR_MV_VEHICLE== WILD)
			{
				if(sw>120)sw=120;
			}               
            else
			{
				if(sw>15)sw=15; //2012-11-24 FROM 30 TO 15
			}
                

            inc_rev_backidle=(sbyte)sw;
        }
        
        if(inc_rev_backidle<=0)inc_rev_backidle=0;
        else if(inc_rev_backidle>120)inc_rev_backidle=120;
        else if(inc_rev_backidle>60)
        {
            if(BYPTR_MV_VEHICLE== WILD)
			{
				inc_rev_backidle-=2;
                //inc_rev_backidle-=2;
			}               
            else //(MV_VEHICLE== HIDER)2012-11-23
			{
				inc_rev_backidle-=4;
			}
                

        }
        else if(inc_rev_backidle>30)
        {
            if(BYPTR_MV_VEHICLE== WILD)
			{
				inc_rev_backidle--;
			}                
            else //2012-11-20 �ӿ���ʽ��
			{
				inc_rev_backidle-=2;
			}                
        }
        else if(inc_rev_backidle>=1) 
        {
            if(BYPTR_MV_VEHICLE== WILD)
			{
				if(VF_RUN4_TMCCL)inc_rev_backidle--;
			}                
            else //2012-11-20 �ӿ���ʽ��
			{
				inc_rev_backidle--;
			}                
        }

    /*�����ߵ��ٵĴ���*/
        if(VF_WORM_ENG&&!VF_HOTSTART_ENG) //2009-12-31 ADD 
        {
            inc_revobj_worm=v_n0_high;//BYPTR_M0_DN0_HTOLNOISCV_15D6RPM;       
        } else 
        {
            if(VF_RUN4_TMCCL)
            {
                if(inc_revobj_worm>0)inc_revobj_worm--;
            }
        }
    
        if(inc_revobj_worm>0||inc_rev_backidle>0)VF_REVTARGET_INC_IGN=true;
        else VF_REVTARGET_INC_IGN=false;

    //rev15d6_target
    s_target=v_n0_low+inc_revobj_worm;
	v_rev15d6tg_nocorr=s_target;

    //2012-4-19 ��ע��
    /*
         tpd2   TPD
            4   0.1
            8   0.2
            12  0.3
            15   0.33
    */
    //2012-4-19 CHANGE FROM  4 TO 6
/*
    if(v_TPd>12)
    {   //����0.2��
        sw=((word)v_TPd-12)*8;
        s_target+=sw;
    }
*/    

    //2019-12-9
    if(v_TPd>8)
    {   //����0.2��
       // sw=((word)v_TPd-8)*8;
        sw=((word)v_TPd-8)*6;
        s_target+=sw;
    }
    
    if(VF_ER_TPS)
    {
        if(VF_RUN_MORE5S_TMCCL)s_target =192;//3000rpm, ����ʱĿ��ת��Ϊ3500rpm  
        else s_target=v_n0_low;//rev_target_notidle=M0_N0_LNOISCV_15D6RPM
    } else if(VF_E0_TPS)s_target+=inc_rev_backidle;
    
    //s_target
    if(!VF_BFIRE_ENG)rev15d6_target=s_target;
    else
    {   
        rev15d6_target=(word)(((word)rev15d6_target*3+s_target)/4);   
    }
    
    
    v_rev15d6_target_idle=255;
    if(rev15d6_target<256)v_rev15d6_target_idle=rev15d6_target;
    
}


/*
    ���ζԵ����ǰ�ǵ�������
*/
//byte pair_odr,byte pair_mode,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode
//v_pair_odr,v_pair_mode,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode
void FC_IGN_Altitude(void)
{
    byte v_TPS_odr_HalfLoad;   
    byte rev_t;
    sword sw;
    byte ign_halfld;
    
    
    v_TPS_odr_HalfLoad=5+v_rev_odr/4;
    if(v_TPS_odr_HalfLoad>9)v_TPS_odr_HalfLoad=9;
    

    
    vc_Ign_Altitude=0;

    if((v_TPS_odr>v_TPS_odr_HalfLoad)&&(v_rev_odr>1) )
    {
        rev_t=v_rev_odr+1;
        if(rev_t>15)rev_t=15;
        
        //ign_halfld=BYPTR_M2_IGN_TPSRPM [v_TPS_odr_HalfLoad][rev_t];
        ign_halfld=Get_Map_byPtr((byte *)BYPTR_M2_IGN_TPSRPM,16,v_TPS_odr_HalfLoad,rev_t);
        
        //sw=((sword)ign_halfld-BYPTR_M2_IGN_TPSRPM [v_TPS_odr][rev_t] ) ;
        sw=((sword)ign_halfld-Get_Map_byPtr((byte *)BYPTR_M2_IGN_TPSRPM,16,v_TPS_odr,rev_t) ) ;

        if(sw<0)sw=0;
        if(sw>15)sw=15;
        sw=sw*v_Pair_deg/64;
        
        vc_Ign_Altitude=(byte)sw;
        
        if(vc_Ign_Altitude>0)
        {
            v_CA_ignition+=vc_Ign_Altitude;
            ign_halfld+=3;
            if(v_CA_ignition>ign_halfld)v_CA_ignition=ign_halfld;
            if(v_CA_ignition>BYPTR_M0_CAS_FRONTEDGE)v_CA_ignition=BYPTR_M0_CAS_FRONTEDGE; 
        }
              //;
    }
     
}
