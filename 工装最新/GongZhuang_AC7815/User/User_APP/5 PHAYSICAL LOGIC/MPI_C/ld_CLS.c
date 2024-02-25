
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
 *		2.b1 在if MV_EXHAUST<=PL50条件下计算存在溢出风险，增加最小限制条件. 
 *
 * V8.3.0	2019.1.24	xielian 
 *		UP对闭环控制加强功能
 *
 * V8.4.0	2019.1.25	Peter 
 *		VF_O2DEC_OS用于推迟减油，VF_O2DEC2_OS用于加浓
 
 * V8.4.1	2019.1.29	xielian 
 *		strcls_tpd计算bugfix
 
 * V8.4.2	2019.2.3	Peter 
 *		删除回油门再给油期间的反馈限制，改为对dmlambda减油进行限制
 *		小油门浓转稀增油作用太强，+5改为+1
 * V8.4.3	2019.3.1	Peter 
 *		UP在50~60之间的闭环控制加浓
 * V8.4.4	2019.3.14	xielian 
 *		小油门加浓避开排放;低温偏浓反馈
 * V8.4.5	2019.7.13	xielian 
 *		blnNidle 赋初值
 * V8.5.0	2019.7.18	xielian 
 *		转速超过9000的闭环反馈量的限制
 * V8.5.1	2019.12.30	ghz 
 *		UP调节油量增量强度，增加系统稳定
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
#include "add_ram_mpi.h"   //为了引用 tes
#include "time_ccl.h"
#include "Events.h"

//备注：原来O2控制的代码长度是2K;
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

sbyte v_dmLambda;           // 本次O2反馈的修正量
sbyte vc_CLS_1;            //O2反馈的修正量；AT	v_O2_ad	FEEDBACKCONTROL,MODI TINJ	BY + vc_CLS_1		
sbyte vc_CLS_2;            //same	as	leansdti //怠速反馈修正量

byte vc_IdleMissfSum_CLS2;     // vc_CLS_2对怠速失火检测总量的修正；(总为正)?
byte vc_IdleSc_CLS2;            // vc_CLS_2对怠速自动搜索量的修正；(总为正)
sbyte leansdtav	;//	1e2h				

sbyte	v_LambdaTg;//2012-10-22

sbyte v_LambdaTg_lookup;//20210130 chenwei add 查表所得的up
sbyte v_LambdaTg_break;   //20210206 chenwei add

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define M0_T_CON_FOR_IDLELEARN_S 6//怠速氧传感器正常反馈，并且满足怠速自学习条件后，开始自学习所需要的、连续满足条件的时间，单位：s//要求小于255			
#define MAX_O2FEDBTIME	45//equ	45		
//#define REMARK_MAX_NOO2FK	3//	EQU 	3	//;WHE REMARK<=3 NO v_O2_ad FK
                            //;DEFINE THE REMARK NUMBER FOR NO v_O2_ad FEEDBACK CONTROL

//#define D_TIO2_TO_NOIDLE	-2//equ	-2		;非怠速时dtio2在怠速平均的基础上8US(减-8US)
#define D_TIO2_TO_NOIDLE	-6//equ	-2		;非怠速时dtio2在怠速平均的基础上8US(减-8US) //2009-9-8

                                    //2008-11-15 add   当出现MO_DFMF_LEANTOOLONG_WHENIDLE之后，OS波动的次数
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
//一些变量的注释：
        //-----------CONSTSP,O2WORK --------------
        //  的条件是：非断油，ASTK_INJ_AGAIN==0时，满足!SIGNAL_STATUS,O2_TOOLOW和(v_O2_ad<O2_MAX_WORK或O2_LAST<O2_MAX_WORK)时置位；
        //		清位条件是：当上述条件不满足时，又清位；
        //                  当不满足条件：(!VF_STARTS_ENG&&PCSET_SCI>REMARK_MAX_NOO2FK&&VF_O2WORK_OSDIAG&&!VF_FROMO2LOW_OSDIAG)时
        //-----------O2_SEEM_WORK_T:---------------
        //  是在满足CONSTSP,O2WORK的条件下，每4S增一；并限制在255
        //-----------O2FEDBTIME:-------------------
        //  是在满足CONSTSP,O2WORK的条件下，每4S增一；并限制在MAX_O2FEDBTIME;
        //-----------sensor_status,from_o2_low:----	
		//清位条件：（VF_FROMO2LOW_OS）
		//      当!SENSOR_STATUS,O2_CHANGED&O2AMAX>=O2_REWORK_MIN时；
		//      或当SENSOR_STATUS,O2_CHANGED时
		//置位条件：
		//      VF_O2TOOLOW_OSDIAG=TRUE;
        //      VF_TESTO2LOW_OSDIAG&(o2_change_nooil==0)
        //----------VF_TESTO2LOW_OSDIAG----------------
        //      if(VF_JCI_CFCI) if(ccl_cf>=80)VF_TESTO2LOW_OSDIAG=true;//if(v_ccl_cf>=80)constsp,test_o2_low=true;
        //-----------o2_m_t:-----------------------
        //          跟踪O2最值过程中，临时的最值；
        //-----------o2_change_nooil---------------
        //      连续找到最值的次数；
        //      是在每次找到O2最大或最小值时，累加1;
        //		当CI修正结束后，需要判断o2_change_nooil是否为0；如果为0则表明...;则给O2最大、最小值平均值赋值；
        //-----------av_o2:O2----------------------
        //      标识：采样值的平均值，需要加一些修正
        //  复位之后初始化为O2_AV_B
        //  当VF_FROMO2LOW_OS时，将当前值赋给AV_O2;
        //  在非断油或再供油条件下，在saverage_o2中计算平均值；
        //  当O2找到最值之后，需要在more_to_o2sum中进行修正；
        //  刚着火时，将当前采样值赋给该值?
        //-----------VF_O2CHANGED_OSDIAG---------------
     	// 1 此时是!sensor_status,o2_changed且!sensor_status,from_o2_low
     	//          且(!VF_O2CHANGED_OSDIAG&&O2_SEEM_WORK_T>=0x40)   时置位；
     	// 2或 o2_change_time==6
        //-----------O2AMAX,O2AMIN-----------------
        //    1 复位后初始化为：O2AMAX_MIN和30
        //    2 VF_FROMO2LOW_OS时，赋值：O2_AD和O2_AD-7；
    	//	  3	如果o2变化次数足够，平均的o2最大和最小就是最大和最小，不然要计算
		//		if(VF_O2CHANGED_OSDIAG){
		//			VF_FROMO2LOW_OSDIAG=false;//	;when v_O2_ad changed more than 6 times, clear from v_O2_ad low
		//			o2amax=av_o2max;o2amin=av_o2min;//templ=av_o2min;过渡用的； }
        //    4 当!sensor_status,o2_changed且!sensor_status,from_o2_low时，
        //      由v_nb_OS_flu对他们进行修正；
        //    5 当满足4，且if(!VF_O2CHANGED_OSDIAG&&O2_SEEM_WORK_T>=0x40){
        //              //;WHEN THE TIME OF v_O2_ad<O2MAX IS MORE THAN (64)*4=256S,时，
        //             将他们赋值给：av_o2max和av_o2min
        //    6 有三中情况会进入此处： o2a_init,对二者进行再初始化//此时o2amin和o2amax根据当前状态，使用默认值；
        //      6.1 不是反馈程序；
        //      6.2 VF_O2TOOLOW_OSDIAG|(v_O2_ad>=o2_MAX_work&O2_last>=o2_MAX_work);
        //      6.3 (!VF_O2TOOLOW_OSDIAG&(v_O2_ad<o2_MAX_work|O2_last<o2_MAX_work))&!(VF_FROMO2LOW_OSDIAG&!VF_O2CHANGED_OSDIAG)&VF_STARTS_ENG;
        //    7 在Sget_o2_mx中用于计算当前目标值；
        //    8 在断油，恢复供油后，在Set_O2Para_After_Finish_CIREC中进行初始化；
        //    TOBECHECK:注意条件3和5的关系；
        //------------av_o2max/av_o2min------------
        //    O2_AD最值O2_MAX和O2_MIN的平均值；
        //    当o2_change_time=6；即连续找到最值6次之后开始计算；
        //------------VF_NOTO2DEC_OS---------------
        //    notdectinum每圈递减；当非０时VF_NOTO2DEC_OS置位，否则清；  
		//------------notdectinum------------------
		//    每圈递减；当非０时VF_NOTO2DEC_OS置位，否则清；
		//    有两种情况需要，对notdectinum进行直接赋值：
		//         1 油门回怠速过快或ORDER_ACCEL,deccel_speedly后;清0
		//         2 当离开怠速后，置12
    //++++++++++++++++++++++++++++++++++++++++    
    //注意，原来刘老师有几个变量地址是一样的：
    //注意他们是如何被使用的，TOBECHECK
        //sum_o2h	与	av_o2；
        //sum_o2		与	sum_o2l；
        //av_o2max	与	av_o2amax_2
        //sum_o2maxl and sum_o2amax_2l;
        //sum_o2amax_2h and sum_o2maxh;
    //++++++++++++++++++++++++++++++++++++++++    

sbyte max_leansdt	;//	0x1c8				;07-3-13	save	the	max	of	vc_CLS_2	which	should	be	more	than	zero
sbyte min_leansdt	;//	0x1c9				;07-3-13	save	the	min	of	vc_CLS_2	which	should	be	less	than	zero
sbyte LEANSDTsum	;//	1E0H									
sword leansdtsum    ;   

byte mod_dtio2	;//	0x1e8				
byte temp_dmod_dtio2	;//	0x1ee	
byte runs_o2_fk	;//	0x10		;记录O2反馈的次数，作为确定V_CCL_CLS的基准

byte second_ffk_idle	;//	47h	;完全反馈后的怠速持续时间[S];the  of idle last after full feedback (fedbtime>45)
//byte t_s_idleOS_change; //2010-3-25
sword d_cls_noidle_to_idle;//怠速与非怠速反馈量的差异
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


sbyte temp_xmax;//最大值    //2016-12-13 变更为全局变量，from Test_CLS_Limit
sbyte temp_xmin;//负的最小值的绝对值；

byte run_inc;

#define DMI_OS_EACH 0
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//***********INIT_O2_AFTERRESET***********************
//功能：CLS策略参数的初始化
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
    //else V_CLS_STATUS&=0x02;//   //保留 #define	VF_GETIDLEFK_CLS	_V_CLS_STATUS.Bits.b12013-3-13 DELETE
    

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
    处理初始化时，自学习的结果
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
	
	v_ccl_sl=0;//2013-3-15 每次学习之后，重新开始计数 
    //每次学习之后，波动重新累计
    v_OS_flu_SLpoint=0;
    
}

/*怠速学习之后，对平均值的处理*/
void FR_idleClsAve(sbyte cls)
{
    leansdtav-=cls;  //		;2007-3-30,banked	
		if(leansdtav>60)leansdtav=60;
		else if(leansdtav<-60)leansdtav=-60;
    
    //leansdtsum=0;//OFF_SET_SUM*256;//2008-12-3 DELETEOFF_SET_SUM
    //leansdtsum=(sword)leansdtav*256;
    leansdtsum=(sword)leansdtav*128;//2015-12-15 发现leansdtsum倍数不对，在几个版本中都会体现
        
}

//功能：氧传感器反馈控制策略，并获得反馈结果?
/*
            v_rev_625rpm,v_TPS_odr
                        ,v_TPS_mode,v_teng_inuse,v_prd_cash,v_rev_odr,v_rev_mode,v_rev_156ave,

*/
void F_CLS_OS(byte cls2_idlemissf,sword tw1_idlesc,word FMF_frommap)
{                  
    word w;
    
    //－－－－增加这两个参数
    vc_IdleMissfSum_CLS2=0;
    vc_IdleSc_CLS2=0;
    
    vc_CLS_2+=cls2_idlemissf;//vc_cls2_idlemissf=w; //ADD_W_TO_leansdt_WHEN_MISSFIRE//失火对怠速反馈值的修正

    VF_LIMITFK_CLS=false;
    VF_ENRICHLIMITFK_CLS=false;
    VF_LEANLIMIT_OSDIAG=false;//

    /*--------------------------------------------------------------*/
    
    if(!VF_STARTS_ENG&&VF_O2WORK_OSDIAG){ //2008-6-28
        if(!VF_NOINJ_ENG)
		{//2013-3-11补回这个条件；
        	PID_CONTRL();//PID_CONTRL();	//氧传感器正常反馈，非怠速、非上圈断油时，PID控制
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
      			else //断油情况下，
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
          			    //2010-6-8 在鑫源，为了防止换档时下一档偏稀，更改如下；
          			    /*vc_CLS_1=vc_CLS_2;*/   //2010-8-24 因金刚车CO高，调整回去
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

    
    sCONTRL_M(tw1_idlesc);				//v_O2_ad feedback control结果对油量的修正；

    
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//氧传感器正常反馈，非怠速、非上圈断油时，DITO2的测试
//正常反馈量dtio2的上下限
void Test_CLS_Limit(word FMF_frommap)
{   
    sbyte b1;
    sbyte b2;
  //  sbyte temp_xmax;//最大值    //2016-12-13 变更为全局变量，from Test_CLS_Limit
  //  sbyte temp_xmin;//负的最小值的绝对值；
    sbyte stemp;

    VF_LEANCON_CLS=false;//2012-12-7
    /*计算基础限值*/
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
        //2016-11-10 临时确实给隆鑫张宏
        if(VF_O2CHANGED_OSDIAG){
          
            temp_xmax=60;
            temp_xmin=-100;
        }
      */
    //当怠速调试时，对最值的限制放松； 
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

    //如果正在测试os是否有浓时偏稀现象或怠速时需要放宽反馈，则： */
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
                vc_CLS_1=vc_CLS_2=temp_xmax;//注意需要保证此时仍然是限制；                           
            } else VF_ENRICH_FORTESTLEAN_OSDIAG=false;
        } 
        else if((VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG&&VF_IDLE_ENG) ||(VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG&&!VF_IDLE_ENG))
        {
            stemp=b1;
            if(stemp<25)stemp=25;
            temp_xmax+=stemp;//15+40+50                             
        }
    //stemp
    //低温不反馈 //2012-12-8   
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
			//2020-12-12 取消&&(v_LambdaTg>=60)的限制；取消&&(v_O2flu_max_av<(v_O2flu_min_av+10))的限制
			{
				temp_xmin=-4;
			}
        }
		
		if(!VF_E0_TPS&&(v_te<0)&&(v_TPS_odr<=7)&&(v_nb_OS_flu<50))
		{
			temp_xmin=0;
			VF_LEANLIMIT_OSDIAG=true; 
		}
		
    /*有lambda 目标控制map确定的    */
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
        
        //防止滑行时传感器信号误导
        if(VF_E0_TPS&&v_rev_625rpm>REV62D5_2500){
            temp_xmin/=2;
            temp_xmax/=4;
        }
		
		if(VF_WOT_TPS&&(v_rev_625rpm>REV62D5_9000)&&!VF_OS_LEAN_WHENRICH_OSDIAG)
		{
            if(temp_xmax>45)temp_xmax=45;
        }
		
		/*  2019-2-1,PETER DELETE
		//2018-7-3 回油门燃油修正期间
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
    /*将反馈限值圆滑处理*/
        /*
            if(temp_xmax>ub_cls_lst)temp_xmax=ub_cls_lst+1;
            else if(temp_xmax<ub_cls_lst)temp_xmax=ub_cls_lst-1;
            ub_cls_lst=temp_xmax;
        
            2016-9-1 DELETE
            
            if(temp_xmin>lb_cls_lst)temp_xmin=lb_cls_lst+1;
            else if(temp_xmin<lb_cls_lst)temp_xmin=lb_cls_lst-1;
            lb_cls_lst=temp_xmin;
        */
    /*使用限值*/
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
            VF_LIMITFK_CLS=true; //2008-11-17 原来的条件会导致全负荷不学习     
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
            VF_LIMITFK_CLS=true; //2008-11-17 原来的条件会导致全负荷不学习     
        }
    }
    
    //2010-3-15 add
    if(VF_ENAQ_SYSINI)vc_CLS_2=0;

}



//*************fk_run_cacl********************************************************
//2008-8-13 进行重大调整
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
//此时的RICH指的是上循环的；

/*
    基本的反馈速度
        1、浓时，减油；每N圈减一次，减油的步长sqstep;


        2、稀时，增油：每N圈增一次，增油的步长sqstep;
        
    偏稀偏浓反馈的处理
        1、偏稀反馈：
            浓时，减油加快 
        2、偏浓反馈：
              稀时，加油速度加快；
              浓时，减油速度减慢；
           
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
	
	
	//201902028 增加与温度相关的加浓处理
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
	
	
	blnNidle=false;//2019-7-13 没有这句，似乎有个bug，内存数据搞成true了？
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
		//2019-6-10 过渡阶段反馈快一些
	{
		//2015-5-26 add 非怠速时的反馈速度
        step_notidle=v_rev_625rpm/8;
		
		if(step_notidle<10)step_notidle=10;
		if(step_notidle>40)step_notidle=40;
	}
	else
	{		
		step_notidle=v_rev_625rpm/2;//2019-6-10 减慢反馈速度
        
		if(step_notidle<20)step_notidle=20;
		if(step_notidle>70)step_notidle=70;
	}

    if(!VF_E0_TPS){
        vc_CLS_2=MO_FMFENRICH_BACKTOIDLE_CLS +leansdtav;//2008-11-13
    }

    fk_run_cacl();

    sqstep=0;
    if(!VF_RICH_OSDIAG){
    		//稀浓切换时对反馈值的处理
        		if(VF_RICHLST_OSDIAG)
        		{//如果上一循环浓，即本循环是由浓转稀的过程?
          
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
        							sqstep+=1;//2019-2-3 5改为1，作用太强
        						}*/
						
        		    }
        		    VF_T35_CLS=false;//2013-3-19
        		}

        //反馈目标的处理
        		
        	//	if(!(VF_GT30_IDLETG_REV&&VF_E0_TPS)||(VF_E0_TPS&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&(v_LambdaTg>60))||((v_LambdaTg>60)&&(strcls_tpd>0)&&(strcls_tpd<=7)&&(v_TPS_odr<=strcls_tpd)))//2018-12-29 怠速，UP较大的情况下，加浓
        		if((!(VF_GT30_IDLETG_REV&&VF_E0_TPS)&&(!VF_CICORECING_CFCI))||(VF_E0_TPS&&(!(VF_NOINIT_SYSINI&&VF_1ST_INI_SYSINI))&&(blnUPrich))||((blnUPrich)&&blnNidle))//2018-12-29 怠速，UP较大的情况下，加浓
				{
            		if(v_LambdaTg>0){  //非怠速需要加浓的话,实现加浓
            		    w=(word)mod_dtio2+v_LambdaTg;////2008-6-6 add (word) ,maybe there has some problem before
            		    mod_dtio2=w&0xff;
              			sqstep+=(byte)((w>>8)&0xff);//
            		}
        		}
				
				

    		//反馈速度的设置
            if(VF_E0_TPS){
            		numb_fk_m=16;//2008-8-13;2008-4-21 将怠速连续浓减油的圈数赋予numb_fk_m
            		//if(v_rev_625rpm>REV62D5_1750)//2008-8-13;when rpm >max_idle, 反馈对称, 16圈一增减
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
            		    //numb_fk_m=8;//2008-8-13   ;2008-4-21 将怠速高时连续稀增油的圈数赋予numb_fk    
            		    //numb_fk_m=24;//2013-4-14 from 8 to 24  
            		    numb_fk_m=48;//2013-4-14 from 8 to 24  //2014-1-19 from 24 to 48
            		} 
            		else
            		{//;when rpm<max_idle, 反馈同前 2008-8-13
            		    if(VF_CICORECING_CFCI){                  //VF_CICFGT5_CFCI  VF_CICORECING_CFCI
            		        //numb_fk_m=12;//2008-8-13           ;2008-4-21 将再供油结束前怠速连续稀增油的圈数赋?
            		        numb_fk_m=24;//2008-8-13           ;2008-4-21 将再供油结束前怠速连续稀增油的圈数赋?
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
										            		
            		
            		if(v_LambdaTg>16){ //2016-6-2 增加条件 AT PID_CONTRL
                		  
                		//2010-7-11 add for 预防氧传感器没有完全热起来的情况
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
    		//稀浓切换时对反馈值的处理
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


        //反馈目标的处理
        		if(v_LambdaTg<0){
        		    w=(word)mod_dtio2-v_LambdaTg;  //2008-6-6 add (word) ,maybe there has some problem before
        		    mod_dtio2=w&0xff;
          			sqstep-=(byte)((w>>8)&0xff);  //
        		}


    		//反馈速度的设置
            if(VF_E0_TPS){
                /*
                #if MV_EXHAUST<=PL50 ||MV_EXHAUST>PL150  //2013-6-15
            		    if(v_nb_flu_lstIdle>2)numb_fk_m=96;
            		    else numb_fk_m=24;
             		#else
            		    numb_fk_m=24;//2008-8-13   ;2008-4-21 将怠速连续浓减油的圈数赋予numb_fk_m
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
            		
            		//2014-2-13 防止氧传感器不够热时，减的太快
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
                          #ifdef	TE_10K	//20210728 chenwei add 10K缸温 
                           if(V_Ta<15 &&v_te<60&&!VF_AQAFTER_PNINI_SYSINI)
						  #endif

						  #ifdef	TE_2K	//20210728 chenwei add 2K水温 
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
            		if(!VF_TRANSLT10_TRANS)//2016-5-21 发现15没有定义
            		{
            	      numb_fk_m=numb_fk_m+numb_fk_m;	    
            		}

            		if(v_LambdaTg==0)numb_fk_m/2;//2016-6-14  //2016-7-8
            		
            		
            		//2016-8-3
            		if(v_LambdaTg>=70&&v_LambdaTg<80)numb_fk_m+= (v_LambdaTg-60)*3;//2016-8-3
					
					if(blnNidle)
					{	
						#ifdef	TE_10K	//20210728 chenwei add 10K缸温
						if(V_Ta<15 &&v_te<60)
						#endif

						#ifdef	TE_2K	//20210728 chenwei add 2K水温
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
		if(vc_CLS_2>100)vc_CLS_2=100;//2019-4-18 否则可能会溢出
		if(vc_CLS_2<-100)vc_CLS_2=-100;
    } else {
        vc_CLS_1+=sqstep;
		if(vc_CLS_1>110)vc_CLS_1=110;//2019-4-18 否则可能会溢出
		if(vc_CLS_1<-110)vc_CLS_1=-110;
    }

}

//******sCONTRL_M******************************************************
//v_O2_ad feedback control结果对油量的修正；
//分两种情况，当O2工作和未工作时；当O2工作时，分怠速和非怠速，然后进行油量的修正
//非怠速时：直接将油量增加DTIO2；
//怠速时：用leansdt增减油量；
		//其中当转速低于一定值后，每4S将自动搜索偏移量的1/16转移到leansdt;
//当O2不工作时，
//		leansdt设置为默认值，并使用该默认值对油量修正；
void sCONTRL_M(sword tw1_idlesc)
{
    byte templ;
    byte temp2;
    
    v_dmLambda=0;

    if(!VF_STARTS_ENG&&(REMARK>MC_OPENLOOP)&&VF_O2WORK_OSDIAG)
    {//2008-5-20 CHANGE fkCONDICTION //2008-6-6 delete &&!VF_MAX_TPS
    	            //2012-11-9 add MC_U
    		if(VF_IDLE_ENG){//2008-11-14 增加VF_FLEANSIGNAL_OSDIAG
      			v_dmLambda=vc_CLS_2;
				/*if((BYPTR_M0_LAMBDA_FACTOR<=250)&&(BYPTR_M0_LAMBDA_FACTOR>0))	//20210922 chenwei add dmlambda调节因子
				{
					v_dmLambda=v_dmLambda*BYPTR_M0_LAMBDA_FACTOR/32;
				}*/	//20210922 暂时不采用该种方式，改版程序定性之前需要删除20210922 19.39
				
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
				  if((BYPTR_M0_LAMBDA_FACTOR<=250)&&(BYPTR_M0_LAMBDA_FACTOR>0))	//20210922 chenwei add dmlambda调节因子
					{
						v_dmLambda=v_dmLambda*BYPTR_M0_LAMBDA_FACTOR/32;
					}*/		//20210922 暂时不采用该种方式，改版程序定性之前需要删除20210922 19.39
    		}
			
				//2008-3-21与杨老师确认，O2正常工作后，不自动搜索了；
				//if(tw1_idlesc!=0){;//当自动寻找的低位不为零,将自动寻找与反馈合成   //2010-1-5 add &&!VF_AQAFTER_PNINI_SYSINI&&!VF_AQOK_SYSINI
				if((tw1_idlesc!=0)&&!VF_AQOK_SYSINI
				        &&(!VF_AQAFTER_PNINI_SYSINI||tw1_idlesc>0) //2010-3-17 add condiction
				        &&(VF_O2CHANGED_OSDIAG)
				    //&&(!VFF_OS_DTC||VF_O2CHANGED_OSDIAG) //2010-2-7
				    )
				{
				    //当自动寻找的低位不为零,将自动寻找与反馈合成   //2010-1-5 add &&!VF_AQAFTER_PNINI_SYSINI&&!VF_AQOK_SYSINI
					//;每4秒钟合成一次             //2010-1-5 add !VF_AQOK_SYSINI
					//2010-2-6 add !VFF_OS_DTC
					if(VF_4S_TMCCL){
  						if(tw1_idlesc>=0){
  							//4秒整数倍时合成.将1/16的自动寻找量.转移到反馈量中.如果1/16等于零,转移4us.
  							//如果aq>0,准备将aq的1/16转移到fk中:aq-aq/16,vc_CLS_2+aq/16,假如不超过最大值.
  							//应该在适当的位置判断,保障自动寻找量在正负1ms之间.
  							templ=0;//templ=min{max(0,max_leansdt-vc_CLS_2),max(1,vc_tw1_IDLESC_h,vc_tw1_IDLESC_l/16)};
  							temp2=(byte)(tw1_idlesc/16);
  							if(max_leansdt>vc_CLS_2)templ=max_leansdt-vc_CLS_2; //2008-7-10 这条语句有问题、
  							        //导致结果是所计算的templ值特别大，可能是44，48，或50等等；
  							        //就会导致 vc_CLS_2突跳；
  							        //其根本原因可能是max_leansdt值计算不对；
  							        //哦，这句话没问题，是下面的那句有问题；
  							if(temp2<1)temp2=1;
  							//if(templ<temp2)templ=temp2;//2008-7-10 有问题；
  							if(templ<temp2)temp2=templ;//2008-7-10 (max_leansds-leansdt)is the max of what can be transed
  							                            //if templ(=aq/16)>temph, trans temph, elso trans aq/16(templ)
  							vc_IdleSc_CLS2=temp2;//通过增加 vc_IdleSc_CLS2(始终为正)
  							vc_CLS_2+=temp2;
  							leansdtav+=temp2;
  							leansdtsum+=((sword)temp2*128/2);// leansdtsumh,leansdtsuml+=templ*128/2;
  						}
  						else{
  							temp2=(byte)((-tw1_idlesc)/16);//templ=0-vc_tw1_IDLESC_h,vc_tw1_IDLESC_l/16;//正值；
  							if(temp2==0)temp2=1;//2008-10-6 ADD   //因为有发现tw1_idlesc小于0时，不向反馈值转移；
  							if(vc_CLS_2>min_leansdt)templ=vc_CLS_2-min_leansdt;else templ=0;
  							if(templ>temp2)templ=temp2;//
  							vc_IdleSc_CLS2=templ;//通过增加 vc_IdleSc_CLS2 (始终为正)
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
//推迟反馈胡
void IDLE_FK_WAIT_nRUNS(byte runs)
{
    if(runs_o2_fk>runs)runs_o2_fk=runs_o2_fk-runs;
    else runs_o2_fk=0;    
}

