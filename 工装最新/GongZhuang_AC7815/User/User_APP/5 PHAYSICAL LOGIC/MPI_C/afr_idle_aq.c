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
 *		MV_EXHAUST相关预编译字段更改为if
 * V8.3.0   2019.1.15	xielian
 *		根据UP对怠速初始化开环调试最小值限制
 * V8.3.1   2019.1.24	xielian
 *		UP对怠速初始化开环调试最小值限制增大
 * V8.3.2   2019.2.3	xielian
 *		开环调试仅允许在开环模式或初始化时起作用
 * V8.3.3   2019.5.28	xielian
 *		开环调试仅允许在开环模式或初始化时起作用
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
//sbyte vc_tv3_idleAQ;     //怠速偏移量搜索结果对TV3_FK的修正；

sword vc_tw1_IDLESC_inuse;//怠速搜索时的喷油器控制参数的临时增量；
sword vc_tw1_IDLESC;      //怠速搜索后得到的偏移量值；


sbyte vc_MissfireNotidle_IdleSC;//怠速搜索值对发冲指数的修正；//2008-11-5

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

byte nb_aq; //发动机运行之后，找O值的次数；	

//byte for_blnaq ;
byte for_temp;

byte rpm_testMode;//扫描模式 0、初始化 1、调浓 2、调稀 3、调浓 4、细调搜索
byte Mode1_Time;  //下降次数
byte Mode4_time;     //自动搜索次数

//2009-11-25
/*
sbyte n_aq[3];//0:上上次，1：上次，2：本次
byte n_aq_ave[2];//0:shangci,1:benci ;
byte n_aq_ave_max[2];//1:max, 0:2nd max;
sbyte n_aq_min;
sbyte n_aq_max;
*/
//2009-12-24 chang to 
sword n_aq[3];//0:上上次，1：上次，2：本次
sword ign_sum_aq;//2010-1-26
sbyte ign_aq[3];//2010-1-26

word n_aq_ave[2];//0:shangci,1:benci ;
word n_aq_ave_max[2];//1:max, 0:2nd max;
sword n_aq_min;
sword n_aq_max;

sbyte o_aq_ave_max[2];//1:max n, 0:2nd max n;
sbyte o_aq_for_maxn[3];//三次粗扫的结果， 0:上上次，1：上次，2：本次

byte nb_n_GT_llimit;        //最高转速超过M0_REV15D6_LLIMIT_FOR_FULLAQ的次数；

sword ign_aq_sum;      //2010-1-11 
sbyte ign_aq_ave[3];//0:shangci,1:benci ;
//2009-11-18 add 
byte t_s_matchENAQ;//在初始化状态下，满足怠速初始化条件时，按秒计时；
byte t_s_matchAirTest;  //在粗调后,测试怠速气量的时间;，按秒计时；

byte nb_LeanWhenAq;     //2010-3-16
byte nb_OSchange_beforeELean;//nb_OSchange_beforeELean=v_nb_OS_flu
sbyte v_dO_enlean;  //2010-3-24 add    向稀搜索时最小的o值
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define PREOIL_ISV_SEL 4    //		EQU	4		;2006-5-26 NO ISV PRE OIL=4 TIME

#define AQ_PERIOD	60//EQU	120		//2008-8-24 	//单位是s												
//#define AQ_PERIOD_FIRST	8//EQU	8	//2008-10-12 发现启动后O值总是因为SC功能在变，因此，从8调整为30		
//#define AQ_PERIOD_FIRST	5//EQU	8	//2008-10-12 发现启动后O值总是因为SC功能在变，因此，从8调整为30		
//#define AQ_PERIOD_FIRST	9//EQU	8	//2008-10-12 发现启动后O值总是因为SC功能在变，因此，从8调整为30		
#define AQ_PERIOD_FIRST	8//EQU	8	//2008-10-12 发现启动后O值总是因为SC功能在变，因此，从8调整为30		
                    //2010-1-27 chang from 9 to 5,after starts
                    //2010-1-29 from 5 to 8
#define MIN_AV_N_DEC_AQ	78//EQU	78	
#define WAIT_RUN_AQ	    20//EQU	20			

#define STEPM_N_SC	5//EQU	5
#define STEPM_N_SC_2	3//EQU	3
#define WAIT_STEP_N	2//EQU	2
#define AQ_TEMP_NUMBER  30

#define STEP_OF_IDLEAQ  16//粗调的步长
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
#define AQMODE_WAIT_AIR      6//等待调节气量

#define O_DELTA_FOR_NO_3RDFULLAQ    12       //2009-11-30前两次扫描结果差值大于该值则需要扫第三次；
//#define O_DELTA_LIMIT_FOR_LEANAQ    -40      //
#define O_DELTA_LIMIT_FOR_LEANAQ    -60      // 2010-3-16 
#define O_DELTA_LIMIT_FOR_RICHAQ    40       //

//扫描时，每一点的次数2009-12-1
#define NB_REV_SET_FORAQ    38//60     //38-24
#define NB_REV_AVE_FORAQ    32//48     //32-20

#define NB_REV_SET_FORAQ_FULL    38     //38-24 2010 30-38 
#define NB_REV_AVE_FORAQ_FULL    32     //32-20

#define NB_LEAN_LIMIT_FORLEAN    4

#define T_WAIT_TO_CHECKAIR_AFTER_FULLAQ    7     //1200RPM, 128RUNS =128/(1200/60)=128/20=7S

//#if MV_EXHAUST>PL150 || MV_EXHAUST<=PL110  //2013-8-1 ADD
    #define STEP_FOR_FULLENLEANAQ   3       //2010-1-16
    #define STEP_FOR_FULLENRICHAQ   6
    
    //---unit:4us,// 用于作为怠速搜索时，TW1的增量；
    const byte M1_DTW1_AQ[6]= //dti_srpm_form      //2014-1-9 减小for 50CC
    {
        0,0,0,3,6,6
    };
/*
#else 
    #define STEP_FOR_FULLENLEANAQ   4       //2010-1-16
    #define STEP_FOR_FULLENRICHAQ   8


    //---unit:4us,// 用于作为怠速搜索时，TW1的增量；
    const byte M1_DTW1_AQ[6]= //dti_srpm_form      //2014-1-9 MOVE IN
    {
        0,0,0,8,14,14
    };

#endif
*///2016-6-9 暂时保留小步长

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

void Save_AQ_Result(sbyte templ);//add_templ_to_pcti:
void GET_AQ_ENABLE(byte bln_aq);

void GET_AQ_rpmMax_OMax(word for_rev156,sbyte ignca);
//void GET_FALLtoAQ(void);

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void FI_IdleAQ_AfterReset(void)
{
    //复位后自动搜索偏移量的参数的初始化；
    //Init_SAQ_after_start://原来是在复位后，初始化的，我给挪到这里BEFORE STARTS,应该可以的；
    vc_tw1_IDLESC_inuse=0;		

    if(!VF_AQOK_SYSINI||VF_VPOR_HARD) //2010-3-13 add condiction 
    {            //2009-12-26 发现增加 VF_TURNONKEY_HARD条件是有问题的，在气量大时isc的量会一直积累；
        vc_tw1_IDLESC=0;	    
    }
    nb_aq=0;        //2009-4-21
    V_IDLEAQ_STATUS=false;//V_STATUS2_IDLE=0;//2008-8-10 add
    VF_NOTAQING_IDLEAQ=true;
    VF_1STAQ_IDLEAQ=true;
    
    rpm_testMode=AQMODE_INI;

}

//功能：STARTS前，怠速策略参数的初始化
void FI_IdleAQ_BeforeStarts(void)
{
    s_need_for_aq=AQ_PERIOD_FIRST;
}

//********************************************************************************************
//功能：是怠速偏移量搜索过程：依据是‘RPM--油量’的关系
//  使能'怠速偏移量搜索过程'的条件是：
//				氧传感器工作;且 run_need_for_aq=1;且s_need_for_aq=1;
//  与搜索相关的其它事件：
//  1 搜索开始后，通过将wait_run_drpm_t=4：来停止失火检测；
//  2 搜索过程中，发现dcdy_down_too_m（即电压下降过多时），重新来过；
//  搜索结果对油量的影响：（调用add_templ_to_pcti）
//  需要将失火增量减半，并且
//  1 当判断出需要增油时：同时增加T3反馈值，dti_rpm_scedh，dti_rpm_scedl和发冲值；
//  2 当判断需要减油时，且需要减油两与减半后的失火增油相加不大于0，减油（减去二者之后）；
//  关于：run_need_for_aq(已是LOCAL)：初值是在run_need_init中设置为wait_run_aq=20；每次调用av_rpm_64r，且a_search_comp时递减；
//														当每次离开怠速，且与上次离开怠速相距一定时间后，RUN_NEED_FOR_AQ会增一；
//  关于：s_need_for_aq(已是LOCAL)：
//										每1s递减一次；
//										初值：复位后初始化时为AQ_PERIOD_FIRST；
//													在next_sc中，当number_sced==2时，被初始化为aq_period；
//  关于：number_sced(是LOCAL):
//  关于：number_need_sum(是LOCAL):每步探底时的圈数38,其中前6圈无效；
//  关于：brpm_l_for_aq(是LOCAL)：搜索过程中，平均转速AV_RPM低于min_av_n_dec_aq时置位，否则清除；
//  关于：dti_rpm_sc：O值自动搜索过程中的临时增量，以前是直接加在PCTINJ上的；
//  关于：b_1st_search：一次搜索值从开始到结束由number_sced值决定；在此过程中，若不满足结束条件，有可能会重来一遍，
//  b_1st_search：就是确定当前是不是重来过的；
//  关于调用QIDLE_SH_INIT:表示停止搜索；
//在新的循环开始，检测是否离开怠速，计算完转速之后
//－－－－各状态值的对应结果：－－－－－－－－－－－－－－
//    VF_NOTAQING_IDLEAQ;		0	  1   1   1   1   1   0				    
//    number_sced;			  0   5   4   3   2   1 	0			    
//    vc_tw1_IDLESC_inuse;0	  0   0   0   8   14  0					    
                      // rev156  当前转速  teng  温度  fmf_starts 启动增量 rev156ave 平均转速 ，c_idles_cls2 氧传感器反馈前增量，tw1_idlesc_ISC 启动增油
void F_SearchIdleSpeed(word rev156,byte teng,byte fmf_starts,byte rev_156ave,
                    byte c_IdleSc_CLS2,sbyte ignca)
{   //09-9-5  修改整理
    //运行说明：
      /*
         1、O值修正
         2、判断是否离开怠速， s_need_for_aq=1 置位
         3、判断怠速搜索条件 (!VF_O2CHANGED_OSDIAG||VF_OSCHANGELITTLE_OSDIAG)&&(!VF_WORKBACKIDLE_ISC)&&(!VF_CICORECING_CFCI)  
         4、满足条件：
            a、记录转速
            b、判断 VF_NOTAQING_IDLEAQ  是否开始搜索      
               O2=0   run_need_for_aq=1   s_need_for_aq=1 条件满足，进行搜索初始化
            c、每步运行38圈？
               计算平均转速
               判断number_need_sum 到达第几步
         5、搜索步数功能：
            a、第一第二步给的vc_tw1_IDLESC_inuse临时增量为0
            b、第三步给临时增量为0，为第一个记录点，并确定下一次搜索的时间AQ_PERIOD，并判断是否最低转速
            c、第四步vc_tw1_IDLESC_inuse=8，为第二个记录点
               判断1、2点是否下降，是的则number_need_sum=3，从第三、第四步重新测试一次
               两次都是下降趋势，则进行减油，并结束这次搜索
            d、第五步vc_tw1_IDLESC_inuse=14，为第三个记录点
               根据三个点之间的转速变化趋势的不同，得到不同的修正量
               进行增减油修正，并结束搜索              
      */
    byte templ;
    sbyte sw;
    sbyte stemp;

 //   if(VF_E0_TPS)         //2009-12-28 ADD CONDICTION
    //2010-1-10 由于杨老师要求非怠速时,也转速o值,因此,取消这个条件;
    //if(c_IdleSc_CLS2!=0)//2010-1-10 更改为这个条件;  //有问题;
    {
        stemp=c_IdleSc_CLS2;
        stemp=-stemp;
        if(vc_tw1_IDLESC>=c_IdleSc_CLS2)
        {
          vc_tw1_IDLESC=vc_tw1_IDLESC-c_IdleSc_CLS2; 
        }   
        else if(vc_tw1_IDLESC<=stemp)
        {
          vc_tw1_IDLESC+=c_IdleSc_CLS2;	   //向0逼近；
        }
        //-----启动后ISC控制对自动搜索结果的修正
        //vc_tw1_IDLESC=vc_tw1_IDLESC+tw1_IDLESC_ISC;    //2012-11-9 DELETE
    }
    vc_MissfireNotidle_IdleSC=0;//2008-11-5 	    
    //将s_need_for_aq	设置为1保证，回到怠速后马上搜索SET_IDLESEARCH_PARA_WHEN_LEAVEIDLE:
    //2008-2-24 zhangping move to here?
    if(VF_JLEAVEIDLE_ENG)
    {
        s_need_for_aq=1;//;2007-2-14 a start let s_need_for_aq=1 which will start aq imiidiatly when back idle	
        run_need_for_aq++;  //？？？
    }
  
    //09-9-5  修改怠速搜索
    //2009-11-18 add flag VF_AQAFTER_PNINI_SYSINI  , with VF_ENAQ_SYSINI
    //2009-11-18 确定动力喷嘴初始化之后，怠速初始化的条件即VF_ENAQ_SYSINI的条件；
    //  VF_AQAFTER_PNINI_SYSINI是在动力喷嘴初始化成功后置位的；
    //  在关闭钥匙或怠速初始化之后清位；
    //  VF_ENAQ_SYSINI是在VF_AQAFTER_PNINI_SYSINI=true；且怠速条件满足之后置位；否则清位；
    //      即只要怠速初始化未成功，当条件一满足，又会进入状态；
    //这里需要注意一点：
    //       在完成喷嘴初始化之后的指示灯状态：
    //          若发动机停止——指示灯快闪；
    //          若发动机启动——气量正常，怠速初始化未结束，即VF_AQAFTER_PNINI_SYSINI=true，快闪
    //                      ——气量偏低，指示灯亮；
    //                      ——转速偏高？？：暂定不指示
    //                                      看是否有必要增加一个怠速气量调小再调大的过程
    //                                      或者观察转速表  
    //                      ——气量正常，初始化结束：灯灭
    //VF_ENAQ_SYSINI置位的条件：
    //  满足以下条件：
    //      发动机转速在最高和最低之间；
    //      油门怠速；
    //      怠速气量不偏低；
    //      无硬件故障；
    //      VF_AQAFTER_PNINI_SYSINI=true;
    //      满足以上条件n圈之后，置位 ；
    //      VF_WARMUP_ENG:缸头温度采样值大于30，且启动增油消失
    //   
    /*
    //2009-11-20 在未调试情况下,如果怠速转速过高需要进行提示;
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
    {                                 //即若OS反馈则使用OS反馈值
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
    
    if(templ==0)    //条件未满足   //2010-5-3 add 
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
        if(VF_ENAQ_SYSINI)//即,如果原来是开环怠速调试状态,则需要对 vc_tw1_IDLESC进行处理
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
        //2010-2-6 当未满足怠速初始化条件，频繁QA
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
            /*在怠速初始化条件下，如果
                1\不满足学习条件；
                2\氧传感器反馈了，且    
            */    
            VF_ENAQ_SYSINI=true; 
            VF_IDLEAIRTEST_SYSINI=false;//2013-6-2
            rpm_testMode=AQMODE_INI;//2013-6-4
        }
        
         
    }else 
    {
        VF_IDLECONDICTION_MATCH_SYSINI=false;
    }
    
    
    //精确搜索
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
                               //2009-12-24 不限制
        //记录当前转速
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
            2013-6-1 增加说明
            非怠速初始化，且OS没有反馈，且不是U的模式下，且出现过转速回怠速
            或者
            怠速初始化情况下，AQMODE_FINE时 
            
            按旧的方式搜索
            
        */
        
        rpm156_aq=rev156;   
        //rpm7d8_aq=rpm156_aq*2;
        //if(VF_NOTAQING_IDLEAQ||!VF_E0_TPS)//怠速搜索标志,初始1，开始搜索后置0，结束或退出搜索置1
        if(VF_NOTAQING_IDLEAQ||!VF_E0_TPS||VF_NOINJ_ENG||!VF_E0MORETHAN4S_TPS)//怠速搜索标志,初始1，开始搜索后置0，结束或退出搜索置1
                                    //2009-12-26 add condiction VF_NOINJ_ENG
        {                                  //2009-12-10 ADD VF_E0_TPS
              //判断怠速状态，转速  怠速阀
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
            
            //判断 O2   run_need_for_aq   s_need_for_aq  是否进行搜索
            if(!VF_O2CHANGED_OSDIAG&&(run_need_for_aq==1)&&(s_need_for_aq==1))
          	{
          	    GET_AQ_ENABLE(true);    
          	}else
          	{
          	    GET_AQ_ENABLE(false);
          	}
        }else //怠速搜索过程
        {
            number_need_sum--;
            if(number_need_sum!=0) //判断一步的38次是否完成
            {
                //累加后32次的转数
                if(number_need_sum<=NB_REV_AVE_FORAQ)
                {
                    sum_rpm156_64r+=rpm156_aq; 
                    ign_sum_aq+=ignca;
                }
            }
            else  //完成一步
            {  
                number_sced--;  //步数减一
                //根据步数决定vc_tw1_IDLESC_inuse临时增量赋值
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
                            
                //传递转速，存储三个点的转速 tw1+0 ,tw1+8,tw1+14 
                n_aq[0]=n_aq[1];
                n_aq[1]=n_aq[2];
                
                ign_aq[0]=ign_aq[1];
                ign_aq[1]=ign_aq[2];
                
                //av_rpm7D8_64r=(byte)((sum_rpm156_64r>>4)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16与15.6/2=7.8
                //n_aq[2]=(byte)((sum_rpm156_64r>>5)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16与15.6/2=7.8
                n_aq[2]=(byte)(sum_rpm156_64r/NB_REV_AVE_FORAQ) ;  //2009-12-1
                  //ignca
                ign_aq[2]=(byte)(ign_sum_aq/NB_REV_AVE_FORAQ);
                
                if(number_sced!=0)  //5步是否完成
                { //未完成进入
                //是否在倒数第三步，即第一个点
                    if(number_sced==2)
                    {
                      	s_need_for_aq=AQ_PERIOD	;//设置下一次搜索的时间
        			
                				if(rev_156ave>=MIN_AV_N_DEC_AQ)
                				{
                				  VF_LOWRPM_IDLEAQ=false; 
                				}	    
                				else   //转速小于最低转速，VF_LOWRPM_IDLEAQ置1
                				{
                				  VF_LOWRPM_IDLEAQ=true;  
                				}
                    }
                   //是否倒数第二步，即第二个点
                    if((number_sced==1)&&(n_aq[2]<n_aq[1])) //判断第二个点转速是否比第一个点小
                    {
                         
                     	  templ=n_aq[1]-n_aq[2]; //转速差异		
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
                    				if(teng<27||fmf_starts>5)    //温度条件和启动增油量>5
                    				{
                    				  	GET_AQ_ENABLE(false);
                    				}
                    				if(templ>6)  //修正量小于6
                    				{
                    				    templ=6;
                    				}
                    				
                    				sw=templ;
                        		//作二点时，如果发现转速下降，则减油			
                            if(VF_1STAQ_IDLEAQ)   //第一次搜索
                            {
                        			    stemp=-sw;
                        			    Save_AQ_Result(stemp);   //该步目的?？是否进行增减油？
                        			    VF_1STAQ_IDLEAQ=false;     			
                        			    number_sced=STEPM_N_SC_2;         //两个重新测试
                        			    vc_tw1_IDLESC_inuse=M1_DTW1_AQ[STEPM_N_SC-number_sced];	
                        		}
                        		else
                        		{             
                        			    stemp=-sw;
                        			    Save_AQ_Result(stemp);    //确认为下降趋势，则执行减油
                        	      	GET_AQ_ENABLE(false);        //退出搜索              	  	
                        		}   
                				}
                    }
                }
                else
                {
                
                    //五步完成，自动搜索完成，判断增减油
                    //	VF_NOTAQING_IDLEAQ=true;    //置位1，结束搜索
                    
            				sw=(sword)(ign_aq[1])-ign_aq[2];
            				//sw=sw*16/dIgn_perRev_M16;
                            drpm_64_32=n_aq[2]-n_aq[1]+sw;  //3、2两个点转速差值
            				sw=(sword)(ign_aq[0])-ign_aq[1];
            				//sw=sw*16/dIgn_perRev_M16;
                    drpm_32_0=n_aq[1]-n_aq[0]+sw;    //2、1两个点转速差值

                    stemp=drpm_64_32+drpm_32_0;;//由转速变化规律，计算油量增减

                    //三个点转速的不同趋势，决定修正量sw的大小
                    if(drpm_32_0==0) //1、2两个点相同
                    {//2007-7-19有失火检测后,可适当的将自动寻找变稀一点
                        if(stemp<0)sw=-4;else{sw=0;}
                    }
                    else
                    {//rpm32_not_0:
                      	if(drpm_32_0<0)  //1、2两点下降
                      	{
                      		if(drpm_64_32<0)sw=-13;else{sw=-9;}
                      	}
                      	else            //1、2两点上升
                      	{//drpm_32_0>0//rpm32_inc:
                      	    if(drpm_64_32<0)   //2、3两点下降
                            {   //	rpm32i_64d:
                        			if(stemp<0)sw=-13;else{sw=-10;}	
                            }
                        		else if(drpm_64_32==0)  //2、3两点相同
                        		{
                        		  //sw=4;    //2010-1-13 chang from -8 to 4
                        		  //sw=-2;     //2010-1-15  from 4 to -2
                        		  sw=-4;     //2010-1-15  from -2 to -4
                        		}
                        		else       ////2、3两点上升
                        		{
                        		  //sw=8;    //2010-1-13  chang from -7 to 8
                        		  //sw=1;     //2010-1-15 from 8 to 1
                        		  sw=-1;     //2010-1-26 from 1 to -1
                        		}
                      	}
                    }
                    //修正量+两个转速偏差       					
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
                        if(teng<27)  //温度要求
                        {
                            stemp=0;
                        }
                    }
                        
                    stemp=(sbyte)(stemp/4);//修正量
                    Save_AQ_Result(stemp);//	
                    GET_AQ_ENABLE(false);	  //结束、退出
        				
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
        else vc_tw1_IDLESC_inuse=0;//2010-3-19 add ,原来没有这一条；主要是怠速初始化且os反馈动油门后会至此；
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
   //搜索调试过程 
   /*怠速初始化过程*/
   if(VF_ENAQ_SYSINI)
   {       
       if(rpm_testMode==AQMODE_FINE)
       {
           if(VF_NOTAQING_IDLEAQ)
           {
                Mode4_time++;
                if(Mode4_time<=2)//3次
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
                    //2010-1-5 加入温度修正
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
       {    //粗条后,测试怠速气量
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
            number_need_sum=NB_REV_SET_FORAQ_FULL;//每步圈数
            sum_rpm156_64r=0;  //每步转速和，取后32次的值
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
       //if(!VF_AQOK_SYSINI)GET_FALLtoAQ();  //调试失败
       if(rpm_testMode!=AQMODE_INI) //2009-11-26 add 
       {
            rpm_testMode=AQMODE_INI;
           // vc_tw1_IDLESC_inuse=0;//2008-4-18 zhangping add   //2010-1-29
       }
   }
    return;      
}

//怠速自动搜索初始化
void GET_AQ_ENABLE(byte bln_aq)
{
    if(bln_aq)
    {
        VF_NOTAQING_IDLEAQ=false;  //进入搜索置位0
        vc_tw1_IDLESC_inuse=0;  //临时增量值0
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
     
    /*搜索结果处理的思路：
        如果搜索值小于0，需要减油
            先从失火增油中减，不够部分再减o值；
        如果搜索值大于等于0，需要增油
            把失火增油的一半挪到，增油值中一起增；
        由于，不牵牛鼻子了，因此，vc_MissfireNotidle_IdleSC和vc_TW1_MISSF1都不用了；     
    */  

    if(templ>=0||!VF_LOWRPM_IDLEAQ||vc_tw1_IDLESC>=0) //VF_LOWRPM_IDLEAQ转速过低;或自动搜索偏移量>=0;或搜索后需要增油;
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
        		v_dmidle=vc_TW1_IDLEMISSF_av;//失火增量，平均值与当前值;
        		vc_tw1_IDLESC+=vc_TW1_IDLEMISSF_av+templ;
                    
        }
    }

    //2008-7-10 将M8喷油器的自动搜索结果减半；   
//    if(templ>=0|VF_LOWRPM_IDLEAQ|vc_tw1_IDLESCh>=0)
    //2008-8-10 增加对搜索修正的限制条件:
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
byte n_aq[3];//0:上上次，1：上次，2：本次
byte n_aq_ave[2];//0:shangci,1:benci ;
byte n_aq_ave_max[2];//1:max, 0:2nd max;
byte o_aq_ave_max[2];//1:max n, 0:2nd max n;
byte n_aq_min;
byte n_aq_max;
byte o_aq_for_maxn[3];//三次粗扫的结果， 0:上上次，1：上次，2：本次
*/
void GET_AQ_rpmMax_OMax(word for_rev156,sbyte ignca)
{
    sbyte sb;
    byte bSuccess;
    byte bCon_For_finishElean;
    
      //VF_AQINI_FAIL_IDLEAQ 搜索失败标志置1 
      //VF_ENAQ_SYSINI=true 搜索标志位
      //VF_AQOK_SYSINI=true 搜索成功需要置位  //2009-11-19         
    bSuccess=false;
    if(VF_O2WORK_OSDIAG&&!VF_RICH_OSDIAG )nb_LeanWhenAq++;
    bCon_For_finishElean=(rpm_testMode==AQMODE_ENLEAN)&&(nb_LeanWhenAq>NB_LEAN_LIMIT_FORLEAN&&nb_OSchange_beforeELean<v_nb_OS_flu&&(number_sced<=AQ_TEMP_NUMBER-6));
    /*if(for_rev156>127)
    {
        for_rev156=127;
    } *///2009-12-24 DELETE
    
          
    
    number_need_sum--;   //圈数减1
    if(number_need_sum!=0&&!bCon_For_finishElean) //判断一步的38圈是否完成
    {
        //累加后32次的转数
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
        //判断最大最小转速
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
    else  //一步完成
    {  
       
        number_sced--;  //步数减1
        /*粗调的方法：
            前三次的o值一样，
            接下来，每次步长32us
        */      
        //传递转速，存储三个点的转速,对应该点的  
       	n_aq[0]=n_aq[1];
		n_aq[1]=n_aq[2];
		//n_aq[2]=(byte)((sum_rpm156_64r>>5)&0x00ff);    //sum_rpm156_64r/32=sum_rpm156_64r/16与15.6/2=7.8
		n_aq[2]=(byte)(sum_rpm156_64r/NB_REV_AVE_FORAQ_FULL);    //sum_rpm156_64r/32=sum_rpm156_64r/16与15.6/2=7.8
		
		ign_aq_ave[0]=ign_aq_ave[1];//2010-1-11
		ign_aq_ave[1]=ign_aq_ave[2];//2010-1-11
		ign_aq_ave[2]=(sbyte)(ign_aq_sum/NB_REV_AVE_FORAQ_FULL); //2010-1-11
		if(bCon_For_finishElean)n_aq[2]=n_aq[1];
        //三点求平均转速
        //sum_rpm156_64r=(word)((word)n_aq[0]+(word)n_aq[1]+(word)n_aq[2])/3;//三点平均值  

        if(number_sced==AQ_TEMP_NUMBER-1)//第一点
        {
                
        } else if(number_sced==AQ_TEMP_NUMBER-2)//‘第二点
        {
            
        } else if(number_sced==AQ_TEMP_NUMBER-3)//‘第三点
        {
            n_aq_ave[0]=(byte)(((word)n_aq[0]+n_aq[1]+n_aq[2])/3);//三点平均 
            n_aq_ave_max[0]=n_aq_ave_max[1]=n_aq_ave[0]; 
            o_aq_ave_max[0]=o_aq_ave_max[1]=vc_tw1_IDLESC_inuse; 
            if(rpm_testMode==AQMODE_ENLEAN)sb=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENLEANAQ;
            else sb=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENRICHAQ;
            vc_tw1_IDLESC_inuse=sb;          
        } else //
        {
            n_aq_ave[1]=(byte)(((word)n_aq[0]+n_aq[1]+n_aq[2])/3);//三点平均  

            //判断最大转数，并记录对应O值增量
            if(n_aq_ave_max[0]<n_aq_ave[1]&&!bCon_For_finishElean)
            {
                //if((rpm_testMode==AQMODE_ENRICH)||(rpm_testMode==AQMODE_ENRICH_AGAIN))
                if(rpm_testMode==AQMODE_ENLEAN)sb=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENLEANAQ;
                else sb=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENRICHAQ;
                
                if(n_aq_ave_max[1]<n_aq_ave[1])
                {   //又出现一次转速最大值；
                    n_aq_ave_max[0]=n_aq_ave_max[1];
                    n_aq_ave_max[1]=n_aq_ave[1];    
                    o_aq_ave_max[0]=o_aq_ave_max[1];
                    o_aq_ave_max[1]=sb;    
                } else
                {   //又出现一次转速第二大值；
                    n_aq_ave_max[0]=n_aq_ave[1];
                    o_aq_ave_max[0]=sb;
                }
            }                                                //vc_tw1_IDLESC_inuse< o_aq_for_maxn[0]

            if(rpm_testMode==AQMODE_ENRICH)  //模式1，先调浓状态
            { 
                //if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>12))  //浓临界条件
                if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>8)||n_aq_ave[1]<76)  //浓临界条件    //2009-12-4 CHANGE FROM 2 TO 8
                {                                                             //2010-1-4 add n_aq_ave[1]的最低限制 n_aq_ave[1]<76 //
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
                  //达到浓条件
                  rpm_testMode=AQMODE_ENLEAN;//转调稀模式
                  number_sced=AQ_TEMP_NUMBER; //步数
                  Mode1_Time=0;
                  o_aq_for_maxn[0]=o_aq_ave_max[0]/2+o_aq_ave_max[1]/2;
                  if(n_aq_ave_max[1]>=M0_REV15D6_LLIMIT_FOR_FULLAQ)nb_n_GT_llimit++;
                  vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENRICHAQ;//2010-1-5 add ,过浓时转速也会太低
                  nb_OSchange_beforeELean=v_nb_OS_flu;
                    //vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+8;
                } 
                else vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+8;
            }
            else if(rpm_testMode==AQMODE_ENLEAN) //模式2调稀状态
            {
               // if(((n_aq[2]+1)<n_aq[1])&&(number_sced<(AQ_TEMP_NUMBER-3))||((n_aq_max-n_aq_min)>=35)||rpm7d8_aq<140)  //稀临界条件
                //2009-11-19 增加当前转速与最高转速差的判断
            //    if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_max-n_aq_min)>=35)||n_aq_ave[1]<70||((n_aq_ave_max[1]-n_aq_ave[1])>15))  //稀临界条件
                /*if(((n_aq_ave[1]+1)<n_aq_ave[0]) 
                    ||((n_aq_max-n_aq_min)>=20)
                    ||(ign_aq_ave[2]>ign_aq_ave[1])
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12)
                    ||((nb_LeanWhenAq>NB_LEAN_LIMIT_FORLEAN&&v_nb_OS_flu>=1&&(number_sced<=AQ_TEMP_NUMBER-6)))
                )  //稀临界条件  */
                //2010-1-17 加严
                /*
                if(((n_aq_ave[1]+2)<n_aq_ave[0]) 
                    ||(((n_aq_max-n_aq_min)>=20)&&(number_sced<=AQ_TEMP_NUMBER-6))//需要走几步才增加波动的判断；
                    ||(ign_aq_ave[2]>ign_aq_ave[1]+1)
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12)
                    ||(bCon_For_finishElean)
                ) */ //稀临界条件
                //2010-7-8 增加步数限制
                if(
                    ((((n_aq_ave[1]+2)<n_aq_ave[0]) 
                    ||(((n_aq_max-n_aq_min)>=20))//需要走几步才增加波动的判断；
                    ||(ign_aq_ave[2]>ign_aq_ave[1]+1)
                    ||n_aq_ave[1]<76||((n_aq_ave_max[1]-n_aq_ave[1])>12))&&(number_sced<=AQ_TEMP_NUMBER-6))
                    ||(bCon_For_finishElean)
                )  //稀临界条件
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
                
                if(Mode1_Time>=2)//达到稀条件
                { 
                  rpm_testMode=AQMODE_ENRICH_AGAIN;//转调浓模式3
                  Mode1_Time=0;
                  number_sced=AQ_TEMP_NUMBER; //步数
                  o_aq_for_maxn[1]=o_aq_ave_max[0]/2+o_aq_ave_max[1]/2;
                  vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse+STEP_FOR_FULLENRICHAQ;    //怕过稀导致灭车
                  v_dO_enlean=vc_tw1_IDLESC_inuse;//2010-3-24 
                  //if((o_aq_for_maxn[1]-o_aq_for_maxn[0]<O_DELTA_FOR_NO_3RDFULLAQ)
                   //   ||!VF_AQINI_ENRICHTOOSHORT_IDLEAQ      //2010-1-5   //2010-1-16 ADD ! CORRECTION 
                  if((o_aq_for_maxn[1]-o_aq_for_maxn[0]<O_DELTA_FOR_NO_3RDFULLAQ)
                      &&!VF_AQINI_ENRICHTOOSHORT_IDLEAQ      //2010-1-5   //2010-1-16 ADD ! CORRECTION 
                    )
                  {     //如果前两次扫描o值差异较小，则不进行第三次粗扫
                        bSuccess=true;
                        //o_aq_for_maxn[2]=o_aq_for_maxn[1];//2010-1-1
                        o_aq_for_maxn[2]=o_aq_for_maxn[0];//2010-1-5
                  }
                  if(n_aq_ave_max[1]>=M0_REV15D6_LLIMIT_FOR_FULLAQ)nb_n_GT_llimit++;

                } else vc_tw1_IDLESC_inuse=vc_tw1_IDLESC_inuse-STEP_FOR_FULLENLEANAQ;
            }
            else if(rpm_testMode==AQMODE_ENRICH_AGAIN) //模式3调浓状态
            {
               // if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>12))  //浓临界条件
                if(((n_aq_ave[1]+1)<n_aq_ave[0])||((n_aq_ave_max[1]-n_aq_ave[1])>8))  //浓临界条件
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
              //达到浓状态
               //2009-11-20 change to the following
                /*
                vc_tw1_IDLESC_inuse=0;
                Mode1_Time=0;
                Mode4_time=0;
                t_s_matchAirTest=0;
                vc_tw1_IDLESC+=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*5+o_aq_for_maxn[0])/8);
                //vc_tw1_IDLESC+=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*6)/16);
                //vc_tw1_IDLESC=vc_tw1_IDLESC-10;  //2009-12-8   减稀一点
                //vc_tw1_IDLESC=vc_tw1_IDLESC-8;  //2010-1-16 
                vc_tw1_IDLESC=vc_tw1_IDLESC-16;  //2010-3-4 
                */
                //2010-3-23 针对向稀扫描时容易灭车的情况，更改如下：
               
                Mode1_Time=0;
                Mode4_time=0;
                t_s_matchAirTest=0;
                vc_tw1_IDLESC_inuse=(sbyte)(((sword)o_aq_for_maxn[2]*2+o_aq_for_maxn[1]*5+o_aq_for_maxn[0])/8-16);
                //2010-7-15 考虑 v_dO_enlean的影响
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
                else *///2010-5-4 delete 为了保证一致性，不差这点时间了 
                {
                    rpm_testMode=AQMODE_TESTIDLEAIR; 
                }
            }
            
        }
        n_aq_ave[0]=n_aq_ave[1];
        number_need_sum=NB_REV_SET_FORAQ_FULL;//每步长度
        sum_rpm156_64r=0;  //每步转速和，取后32次的值
        ign_sum_aq=0;
        ign_aq_sum=0;//2010-1-11    
        nb_LeanWhenAq=0;    
    }  
    
    return;   
}
/*
void Init_WildAQ_Step()   //初始化粗扫描
{
    number_sced=AQ_TEMP_NUMBER;
    number_need_sum=38;//每步圈数
    sum_rpm156_64r=0;  //每步转速和，取后32次的值
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