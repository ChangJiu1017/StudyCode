#include "public.h"
#include "status_st.h"

#include "inj_notystdc.h"
//#include "wait_moving.h"
#include "ad.h"
#include "ld.h"
#include "sci.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_NOTYSTDC_STATUS;
//#pragma DATA_SEG DEFAULT
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte tv3_t_num_need;// equ 0x35	;需要测试T35的次数×8 						
sbyte addtime;//	EQU	5eh					;需要加喷的圈数
byte v_dt1_for_T35;//T35测试之前后，为了防止稀，增加t1；
byte nb_inj_beadd;//累计加喷的次数；
byte nb_t30_added;   //nb_inj_beadd nb_t30_added

byte v_nb_notsyninj_add;  //与角标不同步的加喷的次数； 2012-9-14

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：初始化非喷油圈策略的参数；
void Init_NotYSTDC_beforeStarts(void)
{
    //----Init_TV35Need_afterreset:
	  tv3_t_num_need=0;	    
    V_NOTYSTDC_STATUS=false;// 2008-6-29 add
    v_dt1_for_T35=0;//2009-12-15 add 

}
//*********CTL_NOTYSTDC***********************************************
//功能：非喷油圈的控制
//  放在CI_CO之后 ,必须在本圈触发喷油动作之前，
//  将触发T35测试的动作挪到主程序，通过在调用完该过程之后，判断VF_T35TESTNEED_INJ标志；
//      如果为TRUE，则触发T35;
//  所有的加喷条件和加喷次数的控制都在其中
//  －－－－加喷的条件有－－－－－：
//      1 VF_O2LOWNOTSTARTS_OSDIAG,当发动机不处于STARTS，且 O2LOW时：加一次加喷
//      2 刚刚恢复供油时，加喷次数设置为2次；
//      3 非启动状态下,转速高于900,TPS有误时加喷处理，加喷次数设置为1次；
//      4 当加油门时，增加加喷次数，与油门增量和油门级别有关，并与当前已有的加喷次数相比，取最小者；
//      5 当油量过大拆分时；
//      +6 刚启动时；
//      7 VF_JLEAVEIDLETRANS_ENG,刚离开怠速时；
//-------清除加喷的条件有－－－－－
//      1 VF_JBACKIDLECLRADD_ENG,刚回怠速
//      2 VF_DECSPD_TRANS,急减速时;
//形参：rpm625
//      tps_ddt
//      teng_odr
void CTL_NOTYSTDC(byte rpm625,sbyte tps_ddt)
{
    
    
    if(!VF_FORINJ_INJPHASE)VF_ADDINJLST_NOTYSTDC =false; //2013-5-16
    
    VF_ADDINJ_NOTYSTDC=false;
    
    /*判断加喷的条件*/
    VF_2INJ_ENABLE_INJPHASE=false;
    VF_3INJ_ENABLE_INJPHASE=false;
    VF_4INJ_ENABLE_INJPHASE=false;
    //if(rpm625<REV62D5_ADD1INJ)VF_2INJ_ENABLE_INJPHASE=true;
    //if(rpm625<REV62D5_ADD2INJ)VF_3INJ_ENABLE_INJPHASE=true;
    //if(rpm625<REV62D5_ADD3INJ)VF_4INJ_ENABLE_INJPHASE=true; //2012-10-19 临时删除
    
    //----ADD_INJ_WHEN_Q_TOOLARGE------------------
    //2014-4-6
    if(VF_NEEDDIVFMF_NOTYSTDC)
    {
        addtime=1;//if(addtime==0)addtime=1;
    } else addtime=0;
    
    
    if(VF_JBACKIDLE_ENG||VF_DECSPD_TRANS)addtime=0;//2014-3-15 CLR addtime
    
 
  //2012-10-21 MOVE HERE      //2013-5-17 move here
    VF_ADDINJNEED_NOTYSTDC=false;
    if(addtime)VF_ADDINJNEED_NOTYSTDC=true;

    if(VF_IGNONLY_CFCI&&(!VF_CFLST_CFCI))tv3_t_num_need=0;//2008-10-29 add 
    
    //----------- Deal_with_NOTYSTDC()------------------------------
        if(rpm625>REV62D5_ADD1INJ)addtime=0;
        if(addtime<0)addtime=0;

        if(!VF_FORINJ_INJPHASE){
                
                //VF_T35TESTNEED_NOTYSTDC=false;// 2008-6-29 move in  //2015-3-13 DELETE THIS line，否则会造成T30的T1不对

            		if(addtime) //2014-3-3 ADD CONDICTION，先把拆分的喷完，再进行强制加喷的
            		{
            		    VF_T35TESTNEED_NOTYSTDC=false;//2015-3-13如果有突然加速，需要加喷，则也清除T35标志
            				VF_INJRUN_ENG=true;
            				VF_ADDINJ_NOTYSTDC=true;
            				VF_ADDINJLST_NOTYSTDC=true;//2013-5-16
            				addtime--;
            				nb_inj_beadd++;// nb_t30_added
            		}
            		else{
            		    if(VF_NEWCYCLE_ATMAIN_CAS)
            		    {
            		    
            		        VF_T35TESTNEED_NOTYSTDC=false;//2015-3-13
            		        //2014-4-15 add condiction
                				if(((VF_IGNONLY_CFCI&&VF_CFLST_CFCI&&(rpm625<REV62D5_ADD1INJ))||((rpm625<REV62D5_ADDT35)))&&(( tps_ddt<=8)&&(tps_ddt>=-8)))//2009-12-19       //2009-12-26 降低转速
                				{ //2008-5-28 zhangping modify ,delete condiction PCSET_SCI==REMARK_FOR_T35_HIGHSPEED 
                				    {
                    					if(tv3_t_num_need){//2008-5-26断油期间也T35修正；
                    						tv3_t_num_need--;
                    						//if((tv3_t_num_need&0x07)==0){
                    						if((tv3_t_num_need&0x0f)==0){   //2012-12-27 from 07 to 0f
                        						#ifndef SYS_TRADITIONAL	
													VF_T35TESTNEED_NOTYSTDC=true;
												#endif
                        							VF_INJRUN_ENG=true;
                        							VF_ADDINJ_NOTYSTDC=true;
                                      //2012-12-27 add condiction ，防止影响标定的误判
                                      if(REMARK<=MC_LOADB){
                                          if(VF_E0_TPS)v_dt1_for_T35=M0_FMF_IDLE/8;     //2009-12-15 add //2010-1-26 to 20
                          						    else v_dt1_for_T35=M0_FMF_IDLE/8;
                                      } else v_dt1_for_T35=5;
                      						    nb_t30_added++;
                      						    
                      						    VF_T35_CLS=true;//2013-5-13
                    						} else if((tv3_t_num_need&0x0f)==1) //2012-12-27 from 07 to 0f
                    						{
                    						    if(!VF_E0_TPS)v_dt1_for_T35=M0_FMF_IDLE/8;    //2010-1-26 to 20  //2010-1-29 delete
                    						}        //2013-4-16 add /M0_FMF_IDLE
                    					}
                				    }
                				} else{
                				    //2014-4-6 ADDTIME
                				    //VF_NEEDDIVFMF_NOTYSTDC
                      				    
                      				VF_INJRUN_ENG=false;
                      				VF_ADDINJ_NOTYSTDC=false;
                      				VF_ADDINJLST_NOTYSTDC=false;//2013-5-16
                				}
            		    }
            		}

        } 
        
        //TOBECHECK，为什么 VF_DECSPD_TRANS对加喷的处理放在这里；
        //是不是表示虽然当前的减速动作，但如果有加喷的话，也要再加喷一次；

    //POSITION A  2008-7-31  
}

//************************************************************************************
//所有准备T35的动作都在该过程中
//其条件分别来自：
//  1 时间到；
//  2 暖车到一定时间；
//  3 刚使能怠速搜索；
//  4 对T35测试结果不满意，需要增加一次测试；
//注意，该函数需要放在Search_IdleSpeed之后，因为用到标记VF_JIDLESCEN_IDLE
//形参：    teng
//          t_eng_d1s
void Prep_T35Test(byte teng,byte t_eng_d1s,byte rpm625,byte ccl_cf)
{
    byte b;
    //2008-6-20
    //all time change from 0x1f to 0x2f
    
    //----------TRIG_T35_FOR_5_TIMES_WHEN_TIMEUP------------------------://2008-1-14 MOVE FROM TIME_CALCU
    //这样处理会滞后一个循环，因为原来该过程在 Deal_with_NOTYSTDC之前；

    if(VF_64S_TMCCL&&(rpm625<REV62D5_ADDT35))tv3_t_num_need=0x4f;//2008-5-28;//tv3_t_num_need=0x1f;//when time=**000000, trig t3.5 test
                                                         //2012-12-27 from 2f to 4f
                                                         
    if(VF_JCI_CFCI)tv3_t_num_need=0x1f;//2008-5-8 add  //2008-5-28delete，因为可能会影响供油//2008-11-7 huifu

    if(ccl_cf==8)tv3_t_num_need=0x4f;//断油时T35修? //2008-10-29 增加到9次；?

    //---------PreT35TestWhenWormAndD1s------------------------------------------
    //条件2
    if(VF_WORM_ENG)
    {
        b=(teng>>1)&0x7f;
        if(b<30)b=30-b;else b=0;
        b=b+22;
        //b=b+40;//2008-10-12 add temperory      //2008-11-28 delete
        if(t_eng_d1s==b)tv3_t_num_need=0x2a;
    } 

    //if(v_nb_notsyninj_add>0)tv3_t_num_need=0;//2013-1-7 add 
    
    //----------------------CALL TRIG_T35_FOR_4_TIMES;------------------------------
    if(VF_NEEDT35AGAIN_INJCTL){
      tv3_t_num_need=5;      
      VF_NEEDT35AGAIN_INJCTL=false;
    }
    
}
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/


