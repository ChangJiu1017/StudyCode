#include "status_st.h"
#include "public.h"
#include "ign.h"//2010-1-13
#include "sci.h"

#include "add.h"

//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_IDLEMF_STATUS;
//#pragma DATA_SEG DEFAULT
//missf
byte v_dmidle;    //失火检测修正系数	
byte vc_idlemiss_cur;//     当前失火增油的增量							
byte vc_TW1_IDLEMISSF_av; //失火检测平均值								

byte vc_TW1_IDLEMISSF_tmp;  //
byte vc_TW1_IDLEMISSF_sum;  //代表OS工作后，因失火，产生的对怠速反馈量的修正的部分，当

sbyte vc_cls2_idlemissf; //失火检测对怠速氧传感器反馈结果的修正；//张平添加�

//for idle miss fire control
//LOCAL for "q_missfire.c" ONLY
byte rpm156_last;//	equ	0x1f2				;
byte rpm156_last2;//	equ	0x1f3	
byte rpm156_last3;//	equ	0x1f4	
byte rpm156_last4;//	equ	0x1f5	
byte rpm156_limit;//	equ	0x1f6	
byte rpm156_last5;//	equ	0x1f7	
byte rpm156_last6;//	equ	0x1fb	
byte rpm_status;//	equ	0x104			;怠速失火检测是，相邻两圈之间RPM变化的状态；;2007-7-16,	rcord	the	rpm	dec	status	by	15.625		

//LOCAL FOR "Q_MISSFIRE.C"AND "Q_IDLE.C"
sword sum_ti_inc_drpm;//L;//	equ	0x1cc	;怠速失火检测累加值L；疑问											
//sum_ti_inc_drpmh;//	equ	0x1cd	;怠速失火检测累加值H；		
//missf
#define MAX_MODI_FACH 20	//EQU	20						
//#define MIN_CLR_TI_DRPM	60//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
//2009-12-22
#define MIN_CLR_TI_DRPM	40//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
#define MIN_RPM_NO_TI_I	65//EQU	65	;2006-10-21	WHEN	THE	RPM<65*15.6=1014,	NO	TI	INC	FOR	DRPM	
#define OFFSET_FACH 10    //EQU	10		

byte wait_run_drpm_t;//	equ	0x1f8		;wait_run_drpm_t>0, will disable v_dmidle test		

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void miss_fire_doing(void);//失火处理；
void small_fire_doing(void);
void leans_inc_miss_f(void);
void miss_f_inc_ti(sbyte w);
void miss_fire_inc_l_do(sbyte w);

//功能：STARTS前，怠速策略参数的初始化
void FI_IdleMissfire_BeforeStarts(void)
{
    sum_ti_inc_drpm=0;    
    //Init_MissFire_AfterReset:://原来是在复位后，初始化的，我给挪到这里BEFORE STARTS,应该可以的；
    vc_TW1_IDLEMISSF_av=0;//								;
    v_dmidle=0;//								;
    vc_TW1_IDLEMISSF_sum=0;//							;
    rpm_status=0xff;//								;将怠速转速变化标识设置为全部上升；
    //---2008-7-29-----------------------------
    V_IDLEMF_STATUS=false;
    //Init_MF_BeforeStartRun
    wait_run_drpm_t=6;//			;2007-7-17
}

//********************************************************************************************
//注意rpm_ch_status与RPM_STATUS的区别；二者区别在于，RPM_STATUS只用于怠速，而rpm_ch_status用于所有转速；
//定义：
//vc_TW1_IDLEMISSF_tmp:临时失火增油，当圈T1触发前检测到之后马上使用，在第二圈马上清零；
//v_dmidle:失火增油量，增加后衰减都有过程；
//						当O2WORK时，不使用；
//						清零条件：
//										1 temp_rp1<=min_clr_ti_drpm;;//if rpm <min_rpm_no_ti_i, clrf ti_drpm and end miss_fire_test
//										2 START_INITION中
//										3 在av_ti_drpm_cal中,when vc_TW1_IDLEMISSF_av>max_ti_inc_drpm, which not happened normally,clrf
//										4 ANNUATION_TI_INC_DRPM:衰减，直到0；
//						赋值及使用：
//										1 主程序中调用SMISS_FIRE失火检测后，在调用AV_TI_DRPM_CAL；
//										2 搜索怠速偏移量时，需要对将TI_INC_DRPM部分挪到dti_rpm_scedl中去；ADD_TEMPL_TO_PCTI
//										3 SMISS_FIRE中检测到失火后，需要增；
//										4 ANNUATION_TI_INC_DRPM对其衰减；
//										5 失火增油对油量修正；REC_TI_INC_DRPM
//vc_TW1_IDLEMISSF_av:
//										1 主程序中调用SMISS_FIRE失火检测后，在调用AV_TI_DRPM_CAL；
//										2 START_INITION中清零
//										3 搜索怠速偏移量时，需要对将TI_INC_DRPM部分挪到dti_rpm_scedl中去；ADD_TEMPL_TO_PCTI
//										4 AV_TI_DRPM_CAL中,将128次平均值赋给它；
//					是不是可以说，复位后，该值在不出错的情况下都不为0;
//wait_run_drpm_t:当wait_run_drpm_t=0时，set bdrpm_test，使能失火检测；
//													;dec wait_run_drpm_t by 1, when to 0, set bdrpm_test
//sum_ti_inc_drpmh,l:
//										每圈发现油门不在怠速时，sum_ti_inc_drpm-=6;直到为0;
//										每次从怠速起步时，sum_ti_inc_drpm-=128，直到为0；
//										START_INITION中清零
//										ADD_TEMPL_TO_PCTI中，某种情况下，减半；
//										AV_TI_DRPM_CAL中，将ti_inc_drpm-av_ti_drpm值累加到其上；
//2007-7-15反馈程序的失火检测

//功能：怠速失火检测增油策略
void F_Missf_Idle(word rpm156,byte tps_odr,byte c_IdleMissfSum_CLS2)
{
    byte temp_rp1;//	equ	0x1ff	;临时变量_局部变量	
    byte rev_inc_miss;  //失火增油判断的增量;与失火增油相关,2010-1-10
    byte bDrev;
    
    vc_idlemiss_cur=0;
    vc_cls2_idlemissf=0;//必须置零，并且必须放在程序中适当的位置；
    vc_TW1_IDLEMISSF_tmp=0; //每圈都清零；

    //----通过增加变量vc_IdleMissfSum_CLS2，将CLS对TW1_IDLEMISSF_sum的修正挪出；-------
  	if(vc_TW1_IDLEMISSF_sum>=c_IdleMissfSum_CLS2)
  	{
  	    vc_TW1_IDLEMISSF_sum-=c_IdleMissfSum_CLS2;    
  	}
  	else vc_TW1_IDLEMISSF_sum=0;
						
        
    //原来在DO_INJ_WHEN_LEAVE_IDLE中，从中挪过来
    //VF_JLEAVEIDLE_TRANS需要在之前被设置过
    if(VF_JLEAVEIDLETRANS_ENG)//DEC_TI_DPRM_WHEN_LEAVEIDLE:
    {   //离开怠速时，对失火增量的修正�
	      if(sum_ti_inc_drpm>128)sum_ti_inc_drpm-=128;
        else sum_ti_inc_drpm=0;
    }
    //if(VF_JCLOSE_ISC)miss_f_inc_ti(12);//2008-10-12 add   //怠速阀刚关闭时刻增加一部分失火增油
    
    //－－－－smiss_fire－－－－－－－－－－－－－－－－－－－:
    //原smiss_fire过程自此开始 
    if(rpm156>255)temp_rp1=255;else{temp_rp1=(byte)rpm156;}

    //2009-12-15 如果是在启动阶段转速过低，也计算失火增油；
    if(temp_rp1<=MIN_CLR_TI_DRPM&&!VF_STARTS_ENG)
    {
        v_dmidle=0; //2008-5-30 add back, because missfire too at tm125-4-1#
        //2008-5-30 当转速过低，清除失火增量的同时，清除 sum,还需要
    		vc_cls2_idlemissf=vc_cls2_idlemissf-vc_TW1_IDLEMISSF_sum; //2008-5-30 add
    		vc_TW1_IDLEMISSF_sum=0;                  //2008-5-30 add
    }
    //2010-1-10 add 
    rev_inc_miss=v_dmidle/4;  //20101-1-10
    
    rpm156_last6=rpm156_last5;
    rpm156_last5=rpm156_last4;
    rpm156_last4=rpm156_last3;
    rpm156_last3=rpm156_last2;
    rpm156_last2=rpm156_last;
    rpm156_last=rpm156_limit;
    
    /*-----------------------------------------------------------*/
    //当满足以下条件后的开始失火检测
    if((!(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG))&&VF_E0_TPS&&(temp_rp1<rev15d6_target+10)&&temp_rp1<122)   //2009-11-26 add condiction VF_ENAQ_SYSINI
    {               //2010-1-13 form 128 to 106//1650rpm  to rev_target_idle+10
                //2010-7-10由于点火策略更改后，  rev15d6_target在回油门时会很大，这就导致在断油时以为失火，因此，增加 temp_rp1<122
        //---------Annuation_ti_inc_drpm://失火增量衰减过程----------------�
        if(VF_4S_TMCCL){
            VF_WITHMS_IDLEMF=false;//2008-7-29 add
            if(v_dmidle>1)
            {
                v_dmidle--;
                VF_WITHMS_IDLEMF=true;//2008-7-29 add
                //2008-7-29 ADD MISSFRE FLAG
            }
        }
        //2009-12-15 只有失火增油有效时，才进行失火增油衰减；
        rpm156_limit=temp_rp1;
        VF_ENMF_IDLEMF=true;
        if(rpm156_limit>=(rpm156_last+1))rpm_status=rpm_status*2+1;//在汇编中，用是否是进位，然后用左移实现
        else{rpm_status=rpm_status*2;};//当rpm_status的第N位为false时，表示上N圈的转速相对与第N＋1圈是下降的；

        if(((rpm_status&0x03)==0) &&( rpm156_last2>=rpm156_limit))
        {
            //;当最近两圈转速是连续下降，且当圈比上上圈慢;
        		 //2014-1-15
        		 bDrev=(rpm156_last2-rpm156_limit);
        		 #if M0_MFUD1==INJ209
            	  	bDrev=bDrev/2;
        		 #else
                  bDrev=bDrev/4;
        		 #endif
        		 
          		bDrev++;
        		  vc_TW1_IDLEMISSF_tmp=bDrev;
        		 
          		if(RPM_DEC_FOR_MISFIRE+rev_inc_miss>=rpm156_last2-rpm156_limit){
          		    //没超过,不作为失火,进行太稀判断
          			  //一般失火
            			if(RPM_DEC_FOR_LEANS+rev_inc_miss<(rpm156_last2-rpm156_limit)){
            			    //2圈下降小雨8*15.6时不进入
              				if(VF_O2WORK_OSDIAG&&VF_RICH_OSDIAG&&VF_O2DEC_OS){
              					leans_inc_miss_f();//		;O2下降,+4US
              				} 
              				else //2009-12-14 add else    //2009-12-14 原先在这个条件下无失火增油；
              				{
              					leans_inc_miss_f();//		;O2下降,+4US    				    
              				}
            			}
                      //两全下降不多时，再看看下一圈
                      //two_cycle_miss_t
              		//此时已连续两圈下降,是否连续4圈下降?	
              		else if(((rpm_status&0x0f)==0) &&(rpm156_limit<=rpm156_last4)){;//已连续四圈下降   //2009-12-14 add else to here
                			if(RPM_DEC_2C_MISSFIRE+rev_inc_miss<(rpm156_last4-rpm156_limit)){;//进入small_fire_doing：
                					small_fire_doing();//
                			}
                			else{;//three_cycle_miss_t:;没连续四圈超过限值,做三循环(6圈)着火不充分检测
                				if(((rpm_status&0x3f)==0)&&(rpm156_last6>=rpm156_limit)){
                					if(RPM_DEC_3C_MISSFIRE+rev_inc_miss<(rpm156_last6-rpm156_limit)){
                						small_fire_doing()		;//连续三个循环(6圈)下降,失火处理	}
                					}
                				}
                				//else 这种情况下，表示无失火；
                			}
              		}//else 这种情况下，表示无失火；
          		}
          		else{		;//超过,做失火处理:如果当圈转速不是过低,则作为失火,并做过稀判断,2007-7-17 不做过稀检测	
          		    //失火严重
          			miss_fire_doing();//
          		}
        } 
    }
    else
    {

        VF_ENMF_IDLEMF	=false;    
      	rpm_status=0xff;
      	rpm156_limit=0;
      	//离开怠速后，失火增量衰减过程
      	if(tps_odr) //当油门不在怠速时
      	{
              if(sum_ti_inc_drpm>6)sum_ti_inc_drpm-=6;
              else sum_ti_inc_drpm=0;
      	}
    }


    //感觉以下内容都可以放入到上面的大括号之内
    sum_ti_inc_drpm+=(sbyte)v_dmidle-vc_TW1_IDLEMISSF_av;
    if(sum_ti_inc_drpm<0)sum_ti_inc_drpm=0;                 //2010-1-10 
    vc_TW1_IDLEMISSF_av=(byte)((sum_ti_inc_drpm)/128);

    //----------av_ti_drpm_cal-----------------------------------------
    //原av_ti_drpm_cal过程自此开始
    //计算128圈v_dmidle的失火增量平均
    if(!VF_ENMF_IDLEMF){
		    v_dmidle=vc_TW1_IDLEMISSF_av;
    }

    if(vc_TW1_IDLEMISSF_av>MAX_TI_INC_DRPM||VF_ENAQ_SYSINI){;//失火增量平均值过大，认为有问题；
        //2010-5-3 add condiction VF_ENAQ_SYSINI
    		sum_ti_inc_drpm=0;//原刘老师是：sum_ti_inc_drpmh=0
    		vc_TW1_IDLEMISSF_av=0;
    		v_dmidle=0;
    }

    if(vc_idlemiss_cur> MAX_TI_INC_DRPM)vc_idlemiss_cur=MAX_TI_INC_DRPM;
    v_dmidle+=vc_idlemiss_cur;
    

	  if(v_dmidle> MAX_TI_INC_DRPM)v_dmidle= MAX_TI_INC_DRPM ;

    
    return;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//以下是有关不同类别的失火处理miss_fire_doing(大失火),small_fire_doing（小失火）和leans_inc_miss_f（稀）；
//失火处理的同时需要考虑O2是否工作；
//the subroutine related to  smiss_fire;
void miss_fire_doing(void)//失火处理；
{
    if(MIN_RPM_NO_TI_I<rpm156_limit)
    {
        //当圈转速过低不作为失火,when rpm too low no ti_inc inc
        if(VF_O2WORK_OSDIAG){;//miss_fire_o2work
    	    if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)	miss_fire_inc_l_do(STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_fire_inc_l_do;
        }
        else{
    		miss_f_inc_ti( STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_f_inc_ti;
        }
    }
    return;
}


void small_fire_doing(void)
{
    //小失火处理
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        //当圈转速过低不作为失火,w
        if(VF_O2WORK_OSDIAG){;//small_fire_o2work;
    		if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_fire_inc_l_do;
    		//2010-12-6纠正其调用位置；
        }
        else{
    		miss_f_inc_ti(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_f_inc_ti;
    		//2010-12-6纠正其调用位置；
        }
    }
    return;
}

void leans_inc_miss_f(void)
{
    //稀;//进入的条件是if(CONSTSP,O2WORK&motor_status,rich&pdata_status,bo2dec)
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        if(VF_O2WORK_OSDIAG){;//leans_fire_o2work:
        	if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(1);//	w=1;call miss_fire_inc_l_do;
        }
        else{
        	miss_f_inc_ti(1);//	w=1;call miss_f_inc_ti;
        } 
    }
    return;
}

//以下是子过程的子过程；即对失火增油量的处理
void miss_f_inc_ti(sbyte w)
{
    vc_TW1_IDLEMISSF_tmp+=w;
    vc_idlemiss_cur=w;   //replace by vc_idlemiss_cur

    return;
}

void miss_fire_inc_l_do(sbyte w)
{
    vc_cls2_idlemissf=0;
    if(vc_TW1_IDLEMISSF_sum<MAX_MISS_FIRE_DTI){;//累计失火增量过大,只是对临时失火增量增加,不进入
    	//	if(VF_VBATUPCONTINUE_VBAT)w=w+1;//2008-10-23 +2 //2008-10-24 changeto +1
    		vc_TW1_IDLEMISSF_tmp+=w;
    		vc_cls2_idlemissf=w; //ADD_W_TO_leansdt_WHEN_MISSFIRE
    		vc_TW1_IDLEMISSF_sum+=w;
    }
    vc_TW1_IDLEMISSF_tmp+=w;
    return;
}
