/*HEAD
 ***********************************************************************
 * Filename	:time_ccl.c
 * Function	:time calculation		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.25	xielian
 *		增加0.1s计时标志位VF_D1S_TMCCL.
 *
 * V8.3.0   2019.1.24	xielian
 *		增加最后一次os信号波动的时间.
  *		增加怠速运行时间.
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "time_ccl.h"
#include "add_ee.h"
#include "ee.h"


#include "o2.h"//2019-1-20

//#include "add.h"
//#include "rev.h"
//#include "sci.h"
//#include "sys_init.h"


//#include "ld.h"
//#include "dtc_act.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/

unsigned long int v_t_EngMove_s;           //S为单位的时间累加值//一次开钥匙后发动机累计运行的时间；
unsigned long int v_t_eng_t_s;//2008-10-27 add  //车辆一直维持该状态的时间，从零开始累计；

byte v_t_eng_d1s;         //COUNT	0.1S CONTINOUESLY;只要发动机在运转就一直在累加,并限定在255
                          //2012-12-21增加含义：连续停机的时间
byte v_ccl_eng_byte;    	//运行圈数
word v_t_eng_1s;

byte v_500km;              //车辆运行了500km的倍数；

byte v_t_d1s_afterCI;       //再给油后的时间 

word v_nb_ccl_r;              //发动机工作圈数        2014-10-6

    //一旦状态有变化，即清0
    //状态包括，停车，运行，关钥匙，开钥匙后停车等

//2012-6-26
byte v_d1s_TPDE0_to_2500rpm;
byte v_d1s_TPDE0_to_4000rpm;


byte v_t_forlstrun_d1s;//上一次油门非怠速的时间；

word v_t_pairs_1s;//2013-7-16

byte t_SL_d1s;      //用于自学习的时间，0.1s;

byte v_t_AfterInjON_1ms;//2014-10-8 喷射结束后的时间
byte v_t_AfterIgnON_1ms;//2014-10-8 点火结束后的时间

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/
long t_acc_4us;//TIMEL	;//	3bH						;时间（4us）累加值低位;tIMEH		;//	3cH						;时间（4us）累加值高位	
byte d1second		;//	3dh	;0.1s的累加值；当累加值超过10时，转移到second；COUNT	0.1S	NOT	CONTINOUSLY	RISING	FROM	THE	CARRY	TO		WHEN	IT>10

byte cycle_numb_h    ;//	153h		;运行圈数												
byte cycle_numb_hl	;//	154h		;运行圈数				
byte cycle_numb_hh	;//	155h		;运行圈数					cycle_numb是他们的低位；为PUBLIC 

word t_after1stStart_s;//开钥匙后，第一次启动后开始计时，包括中间停车；

byte t_stop_s;//2010-1-29
word v_ccl_131072;   //一个单位对应大约23.8km;     //2010-3-8 change to word

word v_ccl_131072_afterini;//初始化之后运行的里程；

sbyte v_fmf_Grind_all;

byte v_t_WOT_1s; //油门全开的时间  2013-8-30
byte v_t_NOTE0TPS_1s;//油门离开怠速的时间 2013-8-30

byte v_t_afterbackidle_d1s;
byte v_t_E0_1s;
byte v_t_ci_d1s;
byte v_t_afterstarts;

byte v_t_CA_ncome;

//20210115 chenwei add
word v_t_overTMP_1s;     //达到过温保护启用缸温限值及工况要求后持续运行时间
word t_UPOVERTMP_start_1s;  //20210125 chenwei add 某一工况满足等速条件后过温保护开始的时间，0时刻代表up补偿开始分开段加入，不满足等速条件该值清零；

//20210925 chenwei add 环松仪表
byte v_t_fromkeyon_1s;	//距离开钥匙的时间


void Get_Miles(void);

word nb_d5ms;		//20210606 chednwei add
byte nb_d5ms_INJ;

//2010-3-13 关于里程的数据
#define MILES_KM_761    34//32
#define MILES_KM_6K     258//256
#define MILES_KM_12K    514//512
/*
v_ccl_131072    km
2	47.6
4	95.2
8	190.4
16	380.8
32	761.6
64	1523.2
128	3046.4
256	6092.8
512	12185.6
1024	24371.2
2048	48742.4
4096	97484.8
8192	194969.6
*/
//0km、50 300 km、500km、800 km、3000 km、8000km各测一次
#define MILESKM_PER_ if((v_ccl_131072_afterini&0xffc0)!=(ccl_131072_lst&0xffc0)) b=true
#define MILESKM_PER_23      if((v_ccl_131072_afterini)!=(ccl_131072_lst)) b=true
#define MILESKM_PER_48      if((v_ccl_131072_afterini&0xfffe)!=(ccl_131072_lst&0xfffe)) b=true
#define MILESKM_PER_95      if((v_ccl_131072_afterini&0xfffc)!=(ccl_131072_lst&0xfffc)) b=true
#define MILESKM_PER_190     if((v_ccl_131072_afterini&0xfff8)!=(ccl_131072_lst&0xfff8)) b=true
#define MILESKM_PER_380     if((v_ccl_131072_afterini&0xfff0)!=(ccl_131072_lst&0xfff0)) b=true
#define MILESKM_PER_761     if((v_ccl_131072_afterini&0xffe0)!=(ccl_131072_lst&0xffe0)) b=true
#define MILESKM_PER_1520     if((v_ccl_131072_afterini&0xffc0)!=(ccl_131072_lst&0xffc0)) b=true
#define MILESKM_PER_3040     if((v_ccl_131072_afterini&0xff80)!=(ccl_131072_lst&0xff80)) b=true
#define MILESKM_PER_6090     if((v_ccl_131072_afterini&0xff00)!=(ccl_131072_lst&0xff00)) b=true
#define MILESKM_PER_12000     if((v_ccl_131072_afterini&0xfe00)!=(ccl_131072_lst&0xfe00)) b=true
//部分参数的注释：
//runs_numb_rzt_c：为每256圈增一，直至达到255：runs_numb_rzt_c用于温度预测；
//runs_o2_fk:在主循环中，每运行一圈加一；
//VF_T3ENABLE_INJCTL:运行两圈后使能；

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/
//2008-7-5 add the following
//当开关钥匙是
void FI_TMCCL_AfterReset(byte blnForTimeReset)
{
    v_500km=0;   //2009-9-28
    
    if(!blnForTimeReset&&(VF_VPOR_HARD||VF_POR_HARD))//2010-3-25 ADD VF_POR_HARD
    {   //运行时的参数只有在开钥匙时清除
    	cycle_numb_h=0;
    	cycle_numb_hl=0;
    	cycle_numb_hh=0 ;
    	t_after1stStart_s=0;//2010-1-29
    	Get_Miles();

    	v_t_EngMove_s=0;

		v_t_overTMP_1s=0;  //20210115 chenwei add

		VF_MIL_NEMDTC=false;	//20210923 chenwei add
		VF_MIL_DTC4=false;
		VF_MIL_DTC3=false;
		VF_MIL_DTC2=false;
		VF_MIL_DTC1=false;
		VF_MIL_DTC_3D2S=false;
		v_t_MILGliter_d1s=0;
		VF_MIL_SWITCH=0;
		nb_mil_interval=0;
		VF_MIL_DTC_LSTOVER=false;	//20210924 chenwei add
		v_pcode_lst=0;
		nb_pcode=0;
		v_t_fromkeyon_1s=0;
		VF_MIL_NEMDTC2=false;
		
    }
    
    if(VF_POR_HARD){
        v_t_eng_d1s=0;   //2012-12-21
        v_t_eng_1s=0;
    }
    if(VF_VPOR_HARD){
        //ccl_forMile=0;
        //Get_MileLimit(true);
        VF_MILELIMIT_WARN_SYS=false;
        VF_MILELIMIT_DUE_SYS=false;
		
		v_t_CA_ncome=30;

		v_t_afterstarts=0;		//20210930 chenwei add

		//20211009 chenwei add
			v_pair_ad_input_min_nmov_eng=0;
			v_pair_ad_input_max_nmov_eng=0;
			v_pair_ad_input_lst=0;
			
			v_Vbat_inuse_max_nmov_eng=0;
			v_Vbat_inuse_min_nmov_eng=0;
			v_Vbat_inuse_lst=0;
			nb_noca_test=0;
			VF_NOCA_DIAG=false;
			VF_CA_NOSIGNAL=true;
    }
    
    t_stop_s=0;//

    //复位后对CCL_TIME参数的初始化，即累计圈数和累计时间都置零；
    //2008-7-5 add the following
    t_acc_4us=0;
    v_t_eng_t_s=0;//2008-10-27

    d1second=0;
    V_TMCCL_STATUS=false;//2008-12-15--
    V_TMCCLH_STATUS=false;//2008-12-15--
	
	  v_t_forlstrun_d1s=0;
	  v_t_WOT_1s=0;
}

//功能：累加圈数，并设置圈数标识；
void FG_CCL(void)
{
    v_nb_ccl_r++;//2014-4-6
    
    if(++v_ccl_eng_byte==0){
		if(++cycle_numb_h==0){
				if(++cycle_numb_hl==0){
						cycle_numb_hh++;
				}
		}
    }
    //累计运转圈数；
    V_TMCCLH_STATUS&=0xE0;
    if(v_ccl_eng_byte>2)VF_T3ENABLE_INJCTL=true;//允许T3反馈；
    if((v_ccl_eng_byte&0x01)==0){VF_RUN2_TMCCL=true;};//判断当前圈数是不是2的倍数；即奇偶圈
    if((v_ccl_eng_byte&0x03)==0){VF_RUN4_TMCCL=true;};//判断当前圈数是不是4的倍数；
    if((v_ccl_eng_byte&0x07)==0){VF_RUN8_TMCCL=true;};//判断当前圈数是不是8的倍数；
    if((v_ccl_eng_byte&0x0f)==0){VF_RUN16_TMCCL=true;};//判断当前圈数是不是16的倍数；
    if((v_ccl_eng_byte&0x3f)==0){VF_RUN64_TMCCL=true;};//判断当前圈数是不是64的倍数；
    

      
    return;    
}


//功能：累加时间，设置时间标识
//形参：prd_cas－－前沿角标周期
//  累计发动机运转时间和设置时标
//  累计启动后总时间到临时时间变量：TIME_TEMP,TIMEH,TIMEL+=v_prd_cas_h,v_prd_cas_l;，
//  当临时时间变量累计超过0.1S时，间超过部分转移到D1SECOND，
//  当D1SECOND部分超出1s后，将超过转移到SECONDH，SECONDL，SECOND中；
//  根据SECONDH，SECONDL，SECOND是否是秒的2,4,16倍，分别置标识；
//  关于几个变量
//  time_flag,f1s是在每次增1S时置位，没增时清
//  v_t_eng_255s:每次SECOND溢出时，h_second增一，也就是说h_second基本单位是256秒，并限定在256*255S;
//  v_t_eng_d1s:当临时时间变量累计超过0.1S时,增一；并限定在255
//  VF_MORE1MIN_TMCCL:当时间超过60s时置位
//2008-7-4 扩展该函数的功能；
//之前用途：
//  1 发动机运转时的时标
//增加两个用途
//  2 关钥匙后的时标；
//  3 开钥匙，但发动机未运转时的时标；
void FF_T(word prd_cas,byte blnIni)
{
    /*
    VF_16S_TMCCL=false;		//					;16标识；														
    VF_8S_TMCCL=false;		//					;8标识；														
    VF_2S_TMCCL=false;		//				;2S标识；												
    VF_1S_TMCCL=false;		//					;1s标识												
    VF_4S_TMCCL=false;
    VF_64S_TMCCL=false;         //
    VF_3OS_TMCCL=false;         //
    */
    V_TMCCL_STATUS&=0x80;//2010-3-9
	VF_D1S_TMCCL=false;
    
    if(blnIni)return;//2010-1-26 add
    
    
    if(!VF_BFIRE_ENG&&VF_MMOVE_ENG)v_t_eng_d1s=0;   //2009-12-18
    t_acc_4us+=prd_cas;
    //IF TIME>25000(61A8H), INC D1SECOND AND -25000,TILL TIME<25000
        
    while(t_acc_4us>=0x0061A8){;//0x00,61H,A8H代表0.1s
    		t_acc_4us-=0x0061A8;
    		d1second++;
			VF_D1S_TMCCL=true;
    		if((v_t_eng_d1s<255)&&VF_MMOVE_ENG)v_t_eng_d1s++;
			if((v_t_afterstarts<255)&&VF_MMOVE_ENG)v_t_afterstarts++;
        if(v_t_d1s_afterCI<255)v_t_d1s_afterCI++;
		if(v_t_ci_d1s<255)v_t_ci_d1s++;
        //2013-3-26 d27h10 move here
        if(!VF_E0_TPS){
            if(v_t_forlstrun_d1s<255)v_t_forlstrun_d1s++;
            
        }
        if(t_SL_d1s<255)t_SL_d1s++;//2013-4-1

        //if(t_SL_d1s==255)t_SL_d1s=0;//时间在25秒后重新计时  //2014-10-6

    	//	CLR_WDG; //2014-1-28 peter, delete
    		//call CHECK_TIMEFLAG_fcacul_idle_od;//
		
		if(v_t_afterbackidle_d1s<255)
		{
			v_t_afterbackidle_d1s++;
		}
		
		if(v_t_CA_ncome<30)v_t_CA_ncome++;

		//20210819 chenwei add
		if(VF_CC_CONNECT&&(nb_cc<255))
		{
			nb_cc++;
		}

		//20210923 chenwei add 
		if(VF_MIL_NEMDTC&&(v_t_MILGliter_d1s<65535))
		{
			v_t_MILGliter_d1s++;
		}	
		else	
		{
			v_t_MILGliter_d1s=0;
		}	//20210924 这种方式时间不准，暂时屏蔽掉，采用实时计算的方式，
		
    }
			//20210606 chenwei add 0.5ms
	 while((t_acc_4us>=0x00007D)&&(switch_1==1)){;//0x00,61H,A8H??0.1s
    		t_acc_4us-=0x00007D;
    		//nb_d5ms++;
	 		nb_d5ms_INJ++;
			
		
    }
    while(d1second>=10){
    		d1second-=10;
    		v_t_eng_t_s++;//2008-10-27
    		if(VF_MMOVE_ENG)v_t_EngMove_s++;
    		//call CHECK_TIME_FLAG_FOR_IGN;
    		VF_1S_TMCCL=true;          
    		if((v_t_eng_t_s & 0x1)==0)VF_2S_TMCCL=true;
    		if((v_t_eng_t_s & 0x3)==0)VF_4S_TMCCL=true;
    		if((v_t_eng_t_s & 0xf)==0)VF_16S_TMCCL=true;
    		if((v_t_eng_t_s & 0x7)==0)VF_8S_TMCCL=true;//2010-1-26 add back 	
    		//if((v_t_eng_t_s & 0x1f)==0)VF_64S_TMCCL=true;	
    		if((v_t_eng_t_s & 0x3f)==0)VF_64S_TMCCL=true;	 //2010-1-6 纠正
    		if((v_t_eng_t_s & 29)==0)VF_3OS_TMCCL=true;	
//    	    if((v_t_eng_t_s&0x0f)==3)VF_16SDELAY3S_TMCCL=true;//即16S之后的第三秒；
            //2009-9-7
    	  if(v_t_eng_d1s<255&&!VF_MMOVE_ENG)v_t_eng_d1s++;//2012-12-21
    	  if(v_t_eng_1s<3600)v_t_eng_1s++;
		  
			//20210115 chenwei add
			if(VF_OVERTEMANDHIGHLOAD_ENG)   //过温保护达到缸温限值与负荷要求
			{
				if(v_t_overTMP_1s<65535)
				{
					v_t_overTMP_1s++;
				}
			}
		/*	if(VF_OVERTEMPRATRUEPROTECTION_ENG&&VF_STATUS_SPEED_ENG&&VF_STATUS_TPD_ENG)
				{
					t_UPOVERTMP_start_1s++;
				}
			else t_UPOVERTMP_start_1s=0;      //不满足时清零  20210123 chenwei 屏蔽 再满足再加需测试*/

			if(VF_KEYON_HARD)	//20210925 chenwei add
			{
				if(v_t_fromkeyon_1s<255)
				{
					v_t_fromkeyon_1s++;
				}
				
			}
			else
			{
				v_t_fromkeyon_1s=0;
			}
    	    
        if(VF_MMOVE_ENG)
        {
            t_stop_s=0;
        } else 
        {
            if(t_stop_s<255)t_stop_s++;
        }
    		//CLR_WDG;//2014-1-28 peter, delete
    		//2010-1-29
    		if(t_after1stStart_s>0||VF_MMOVE_ENG)
    		{
    		    if(t_after1stStart_s<3000)t_after1stStart_s++;    
    		}


        if(v_t_OS_Lt20_to_now<0xffff)v_t_OS_Lt20_to_now++;//2019-1-20
        
        if(VF_MMOVE_ENG){
            if(v_t_eng_t_s>=60)VF_MORE1MIN_TMCCL=true;;//v_t_EngMove_s>=60, SET morethan_1m
            if((v_t_eng_t_s>=15))VF_15S_AFTERSTART_TMCCL=true; //2010-3-13 ADD CONDICTION VF_MMOVE_ENG
            //if((v_t_eng_t_s>20))VF_RUNENOUGH_FORINI_SYSINI=true;//2010-3-13
            if(VF_BFIRE_ENG&&v_t_eng_t_s<5)VF_RUNENOUGH_FORINI_SYSINI=false;//2015-12-9 for CBG8 INITION
            if((v_t_eng_t_s>20))VF_RUNENOUGH_FORINI_SYSINI=true;//2010-3-13
            
            if((v_t_eng_t_s>=5))VF_RUN_MORE5S_TMCCL=true; //2010-3-13 ADD CONDICTION VF_MMOVE_ENG
            
            //2013-7-16
            if(v_t_pairs_1s>=1&&v_t_pairs_1s<=RUNS_FORPAIRS_LIMIT_U)v_t_pairs_1s++; //2013-4-24  //注意<=

            //2013-8-30
            if(VF_WOT_TPS){
                if(v_t_WOT_1s<255)v_t_WOT_1s++;
            } else v_t_WOT_1s=0;
            
            if(!VF_E0_TPS) {
                if(v_t_NOTE0TPS_1s<255)v_t_NOTE0TPS_1s++; 
            }
            else{
                v_t_NOTE0TPS_1s=0;
            }
			
			if(VF_E0_TPS)
			{
				if(v_t_E0_1s<255)v_t_E0_1s++;
			}
			else
			{
				v_t_E0_1s=0;
			}
            
            
            
        }
        
        
    }

    
    return;
}

//功能：将当前运转圈数累加到EE中，（关闭钥匙时调用）?
//  将发动机运转时间secondh，secondl,sum_second累加到EE中相应的地址：ee_run_timehl，ee_run_timeh，ee_run_time中
//  注意，存储之后，没有对secondh，secondl,sum_second进行清零，
//  理由是：它是被POWER_TIME调用的，调用前已经检测到钥匙要关闭，因此之后发动机是停车了；
//  而在每次发动机重新启动时需要对secondh，secondl,sum_second进行重新清零，
//  将发动机运转圈数cycle_numb_hh，cycle_numb_hl，cycle_numb_h累加到EE中相应的地址：ee_cycle_h，ee_cycle_hl，ee_cycle_hh中
void FM_TimeCCL()//KEY_OFF
{
    word temp_data;
    byte b;

    unsigned long int sec_ee;
    
    //v_t_EngMove_s;
    
    sec_ee=Load_Data_EE(EE_RUN_TIME)
            +(((word)Load_Data_EE(EE_RUN_TIMEH)<<8)&0xff00)
            +(((unsigned long int)Load_Data_EE(EE_RUN_TIMEHL)<<16)&0xff0000);

    sec_ee+=v_t_EngMove_s;
    Save_Data_EE(EE_RUN_TIME,(byte)(sec_ee));
    Save_Data_EE(EE_RUN_TIMEH,(byte)(sec_ee>>8));
    Save_Data_EE(EE_RUN_TIMEHL,(byte)(sec_ee>>16));
    //2010-3-15 ADD 防止写程序之后老是里程自检
    Get_Miles();

    //SAVE_RUNCYCLESUM_WHEN_KEYOFF:
    if(cycle_numb_h!=0){
    		temp_data=Load_Data_EE(EE_CYCLE_H);
    		temp_data+=cycle_numb_h;
    		if(temp_data>255){
    		     if(++cycle_numb_hl==0)cycle_numb_hh++;
    		}
    	    temp_data=temp_data&0xff;	
    		b=Save_Data_EE(EE_CYCLE_H,((byte)(temp_data)));
    }
    if(cycle_numb_hl!=0){
    		temp_data=Load_Data_EE(EE_CYCLE_HL);
    		temp_data+=cycle_numb_hl;
    		if(temp_data>255){cycle_numb_hh++;}
    	    temp_data=temp_data&0xff;	
    		b=Save_Data_EE(EE_CYCLE_HL,((byte)(temp_data)));	
    }
    if(cycle_numb_hh!=0){
    		temp_data=Load_Data_EE(EE_CYCLE_HH);
    		temp_data+=cycle_numb_hh;
    	    temp_data=temp_data&0xff;	
    		b=Save_Data_EE(EE_CYCLE_HH,((byte)(temp_data)));
    }

    //----CLR_RUNTIME_RUNCYCLE_WHEN_KEYOFF------

    cycle_numb_h=0;
    cycle_numb_hl=0;
    cycle_numb_hh=0 ;   
    v_t_EngMove_s=0;

      
    return;
}

//功能：Starts前对CCL_TIME参数的初始化，所有时间都置零
void FI_TimeCCL_BeforeStarts() //blnPor  //blnBeforeStarts //检测到启动角标后，对时间参数的初始化?
{
    t_acc_4us=0;
	d1second=0;
	//v_ccl_eng_byte=0; //2008-8-7更改如下：
	                //2008-1 刘老师将启动时圈数初始化为1   //
	v_ccl_eng_byte=1;//?
	v_t_eng_d1s=0;
	v_t_eng_1s=0;

	//V_CCL_ENGINE=0;//2008-3-28 zhangping add
	V_TMCCL_STATUS=0;//
	V_TMCCLH_STATUS=0;//
    v_t_eng_t_s=0;//2008-10-27

}  

//********SUM_SECOND_TO_ALL*********************************
//功能：当发动机停止时，累加本次开钥匙后的总运行时间
void F_sum_T_ENG_WhenStopS()
{
    


}

//喷嘴强制自检之后，的里程的初始化
void ResetMiles_AfterIni(void)
{
    byte b;
    
    b=Load_Data_EE(EE_CYCLE_HH);
    Save_Data_EE(EE_CYCLE_HH_INI,b); 
            
    b=Load_Data_EE(EE_CYCLE_HL);
    Save_Data_EE(EE_CYCLE_HL_INI,b); 
            
}
//获得与里程相关的数据
void Get_Miles(void)
{
    word w;
    	//2009-9-28 
    	w=Load_Data_EE(EE_CYCLE_HH);
    	w=(w<<8)&0xff00;
    	w=w+Load_Data_EE(EE_CYCLE_HL);
    	
    	/*
    	if(M0_KESEI_CCLHHHL_PER500KM==0)
    	{
    	    v_500km=w/47;    
    	    //v_500km=w/16;    
    	}else
    	{
    	    
    	    v_500km=w/M0_KESEI_CCLHHHL_PER500KM;
    	} 
    	*/
  	    v_500km=w/47;  //2010-2-23 linshi  
    	
    	
    	//2010-2-3 add 
    	if(w>500)w=500;
    	v_ccl_131072=w/2;//2010-2-3
    	if(v_ccl_131072<13)
    	{
    	    v_fmf_Grind_all=12-v_ccl_131072/2;
    	} else if(v_ccl_131072<=32)
    	{
    	    v_fmf_Grind_all=6-((sbyte)v_ccl_131072-9)/4;    
    	}else
    	{
    	    v_fmf_Grind_all=0;       
    	}
    	if(v_fmf_Grind_all<0)v_fmf_Grind_all=0;
    	else if(v_fmf_Grind_all>12)v_fmf_Grind_all=12;
    	
    	/*
    	w=Load_Data_EE(EE_CYCLE_HH_INI);
    	v_ccl_131072_afterini=(((w<<8)&0xff00)+Load_Data_EE(EE_CYCLE_HL_INI))/2;
    	if(v_ccl_131072_afterini>v_ccl_131072)v_ccl_131072_afterini=0;
    	else v_ccl_131072_afterini=v_ccl_131072 -v_ccl_131072_afterini;
        *///2010-7-3 delete
}

//2014-10-6
void F_Clr_SL1s(void) 
{
    t_SL_d1s=0;    
}

void F_Clr_PostInjOnTime(void) 
{
    v_t_AfterInjON_1ms=0;
}

void F_Clr_PostIgnOnTime(void) 
{
    v_t_AfterIgnON_1ms=0;
}

//2016-3-3
//每次新状态时，清零
void Clr_t1s_WhenNewState(void) 
{
    v_t_eng_1s=0;    
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
/*
里程    v_ccl_131072(24km)    

        512     

*/
