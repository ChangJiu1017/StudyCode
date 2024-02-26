/*HEAD
 ***********************************************************************
 * Filename	:o2.c
 * Function	:os calculation		
 * User		:xielian
 * Date		:2019.1.7
 * Version	:V8.0.0
 *
 *** History ***
 * V8.0.0   2019.1.7	xielian 
 *		Base module.
 * 
 * V8.0.1   2019.1.7	xielian
 *		更新氧传感器不波动，不工作和短地诊断策略.
 *
 * V8.0.2   2019.1.15	xielian
 *		氧传感器变稀趋势增加小油门条件.
 *
 * V8.1.0   2019.1.24	xielian
 *		增加最后一次os信号波动的时间.
 
 * V8.2.0   2019.1.25	peter
 *		调整VF_O2DEC2_OS的定义.
 
 * V8.2.1   2019.1.29	xielian
 *		strcls_tpd计算bugfix.
 
 * V8.3.0   2019.2.14	xielian
 *		VF_O2DEC2_OS和VF_O2DEC_OS条件加严.
 
 * V8.3.1   2019.5.28	xielian
 *		怠速调试判断os波动的幅值不减半.
 * V8.4.0   2019.10.2	xielian
 *		VF_O2DEC2_OS和VF_O2DEC_OS条件加严.
 *		简化初始化
 ***********************************************************************
HEAD END*/

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Public.h"
#include "Status_st.h"
#include "O2.h"
#include "ld_cls.h"
#include "ld_cls_sl.h"

//#include "ign.h"
#include "add.h"
#include "tps.h"

//#include "inj_ctl.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/


byte v_O2_ad;             //O2采样值
byte v_O2_last;
byte v_O2_last2;
byte v_O2_last3;//2019-1-25 PETER
byte v_O2_last4;
byte v_O2_last5;
word v_O2_trend_DecOrNot;          //Os的采样值变化趋势,按顺序,最后一位代表最近一次,
                            //2008-11-17
word v_O2_trend_IncOrNot;       //

//关于 os工作
byte v_O2_notworkLimit;          //os不工作对应的最大采样值
byte v_t_O2work_4s;         //氧传感器工作后，的时间，单位为4秒；

byte v_O2_lambdaE1;          //lambda=1时对应的os采样值，即原来的目标值
//os波动后的参数
byte v_O2flu_max_av;   //波动后最大值的平均值
byte v_O2flu_min_av;   //波动后最小值的平均值
byte v_O2flu_max_lst;   //
byte v_O2flu_min_lst;   //
//sword v_sum_O2flu_max;  //波动后最大之的总和
//sword v_sum_O2flu_min;  //波动后最小之的总和
byte runs_noflu_nocalm;           //氧传感器没有波动的,且未被计入最值计算的次数；

byte v_O2_m_lst;    //最近波动时的极值,即最大或最小值
byte v_nb_OS_flu;   //os波动的次数；

sword t_con_for_oswork; //
sword t_con_for_osflu;  //
byte t_after_jci_s;     //段有后的时间
byte nb_osflu_whencf;   //     断油后os仍然波动的次数
byte t_con_lean;        //os反馈后，出现持续偏西的时间    
byte nb_flusamll;       //os 波动幅度过小的次数；
word t_whenoswork_s;    //os 工作时的时间
byte nb_CI_afteroswork; //os工作后恢复供油的次数;
byte nb_oser_whenrich;  //
byte os_max_afterwork;  //os工作os的最大值；
word t_osconlow_before_osflu_s;   //os波动前，持续低的时间，如果超过一定值，则os有问题；
word t_os_notwork;      //    os不工作的时间2009 -12-16

//2009-12-24 增加对os响应时间的判断
byte ccl_os_conlean;  //os 持续稀的圈数
byte ccl_os_conrich;  //os 持续浓的圈数
word ccl_os_fkfast;     //os 反馈达到快速的水平的圈数
byte nb_ccl_maybenotfast;//  
byte ccl_os_fk_limit;   //2009-12-24 氧传感器响应时间，单位：圈数

byte v_nb_flu_lstIdle;//2010-3-17//最后一次怠速之后，os波动的次数；

byte v_cf_1s;//2018-7-6 断油的时间
//ccl_os_conlean ccl_os_conrich ccl_os_fkfast NB_CCL_OSFKFAST_LEANRICH_LIMIT NB_CCL_OSFKFAST_SUM_LIMIT 
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//#define OS_FORSHORTCUT  10
#define OS_FORSHORTCUT  3	//2018-7-7

#define OS_NOTWORK      56//2013-3-11 56 TO 60

#define OS_LAMBDAE1_INI  45
//#define OS_MAX_AV_INI   49
//#define OS_MAX_AV_INI   54//2014-3-7 

#define OS_MAX_AV_INI   50//2014-3-7  //2014-12-22

//#define OS_MIN_AV_INI   39
#define OS_MIN_AV_INI   26//2010-3-23 FAXIAN DAISU NONG SHI OS BU WANGXIA YA 

#define NB_OSFLU_LIMIT_FORCHANGED  6
#define NB_OSFLU_LIMIT_MAX      50
#define NB_FORAVE_M     4
#define RUNS_NOFLU_NOCALM_LIMIT 120
#define OSMAX_LLIMIT    20
#define DELTA_FLU_BASIC 8           //计算波动的差异的基数
#define DELTA_MAXTOMIN_LIMIT    10   //波动最大值和最小值之间差异的限制；

/*
#define T_FOR_OSNOWORK_S_MAX    1000//如果是怠速，持续这么长时间os不工作，认为有问题；
#define T_FOR_OSNOWORK_S_MIN    600 //在其他情况下，判断os未工作的时间的最小值； //2010-1-18 from 300-600
*/

//2016-3-29 根据森科的要求，欧四更改
#define T_FOR_OSNOWORK_S_MAX    200	//2018-3-23 根据客户要求及过检情况时间再缩短240//如果是怠速，持续这么长时间os不工作，认为有问题；
#define T_FOR_OSNOWORK_S_MIN    180	//2018-3-23 根据客户要求及过检情况时间再缩短200 //在其他情况下，判断os未工作的时间的最小值； //2010-1-18 from 300-600




#define T_STEP_FOR_1JLEAVEIDLE   50 //离开怠速对os工作时间的贡献
#define T_STEP_FOR_1JCI         200 //断油堆os工作时间的贡献

//#define T_FOR_OSNOFLU_S_MAX    3000 //如果是怠速，持续这么长时间os不工作，认为有问题；
#define T_FOR_OSNOFLU_S_MAX    260 //如果是怠速，持续这么长时间os不工作，认为有问题；//2016-11-12
//#define T_FOR_OSNOFLU_S_MIN    600  //在其他情况下，判断os未工作的时间的最小值；   //2010-1-18 from 300-600
#define T_FOR_OSNOFLU_S_MIN    240  //在其他情况下，判断os未工作的时间的最小值；   //2016-10-19 FROM 600 TO 240

#define T_FOR_CON_LEAN_S      2//反馈到头，且持续偏西这个时间后开始放宽；

#define OS_CF_LIMIT         20      //认为断油后 os应该低于该值
#define T_FOR_CI_DETECT_S     2     //断油后对os检测的时间；
#define NB_OSFLU_WHENCF_LIMIT   5   //
#define NB_OSFLUSMALL_LIMIT     60  //     nb_flusamll>NB_OSFLUSMALL_LIMIT 时认为有问题；
#define NB_CILIMIT_FOR_OSNOCHANGE   4
#define T_FOR_CON_LEAN_ENLARGE_S    2
#define T_FOR_TEST_LEAN             6
#define NB_OSER_WHENRICH_LIMIT      3//  nb_oser_whenrich    NB_OSER_WHENRICH_LIMIT
#define OS_SHORT_MLIMIT             8//OS短路后的最大值限制
#define NB_CCL_OSFKFAST_LEANRICH_LIMIT  50     //当持续稀或者持续浓的圈数超过该值之后，可能响应慢
#define NB_CCL_OSFKFAST_LEANRICH2_LIMIT  50   //当持续稀或者持续浓的圈数超过该值之后，表示响应慢
#define NB_CCL_OSFKFAST_SUM_LIMIT   600 
#define NB_MAYBENOTFAST_LIMIT   1 
#define T_OS_RESPONSE_4US       50000//2009-12-24 增加os反应时间200ms；         ccl_os_fk_limit=T_OS_RESPONSE_4US/周期+2；
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 

//byte jqwd_last3_run;//	equ	0X18E		;上上上圈O2值	//2014-7-12 DELETE														
//byte jqwd_last2_run;//	equ	0X18F		;上上圈O2值			//2014-7-12 DELETE																	
byte jqwd_last_run	;//equ	0X190			;上圈O2值	
byte jqwd;//	EQU	32h						;O2的采样值

#define NB_OS_MAYNOTWORK_INI  6
byte nb_os_maynotwork;//os达到未工作的次数

byte v_nb_OS_AD_CD;


word v_t_OS_Lt20_to_now;//最后一次OS信号波动到现在的时间；2019-1-20 PETER

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
byte Ave_by(byte oldValue,byte newValue,byte Avetime );

//20211014 chenwei add
byte os_min_afterstart;		//最小的os采样
byte t_os_highvolt;			//检测到氧传感器断开的计数


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
//****************************************************************************
//复位后的初始化
void FI_OS_Reset(void)
{
    if(VF_VPOR_HARD)
    {
        V_OSDIAGHL_STATUS=false;   //用于记录故障码的
    }
    V_OSDIAG_STATUS=V_OSDIAGH_STATUS=false;  
	
	//20211010 chenwei add
	v_t_O2H_delay_1s=0;
	v_o2h_n_625=0;
	v_o2h_tpd=0;
	VF_O2H_STATUS=false;
	
	//20211014 chenwei add
	os_min_afterstart=255;
	t_os_highvolt=0;
	VF_OS_HIGHVOLT=false;
	
}

//功能：O2参数的初始化
void FI_OS_BeforStarts(void)
{

	jqwd=v_O2_ad;//
	jqwd_last_run=v_O2_ad;//		;2007-3-30		;2007-3-30,BANKED
	//jqwd_last2_run=v_O2_ad;//		;2007-3-30		;2007-3-30,BANKED

    v_O2_trend_DecOrNot=0;       //2008-11-17
    v_O2_trend_IncOrNot=0;
    t_con_lean=0;
    
    v_t_O2work_4s=0;
    t_os_notwork=0;
    t_con_for_oswork=T_FOR_OSNOWORK_S_MAX;
    t_con_for_osflu=T_FOR_OSNOFLU_S_MAX ;
    
    nb_os_maynotwork=NB_OS_MAYNOTWORK_INI;//2014-7-12
    //对信号处理的初始化
    //if(!VF_OSCHANGELITTLE_OSDIAG)v_Nb_osChange=0; //2008-11-17
	
	VF_O2NOTWORK_CF=false;
	v_cf_1s=0;

}

// @Function      void SAVE_ADCRES_TO_O2(void)
//功能：根据两次O2采集，平均得到O2值v_O2_ad，并将上次值保存在v_O2_last中
//      用于发动机运行时；
//形参; adres_10bit--第一次10 AD采集结果
//      adres_10bit_2--第二次10 AD采集结果 
void FG_OS(word adres_10bit,word adres_10bit_2)      //记录O2采集结果 
{ 

    byte strcls_tpd;
	
	strcls_tpd=BYPTR_M0_STRCLS_TPD;	
	//if(strcls_tpd>7)strcls_tpd=7;
	
	//保存O2的ADC结果
    //-----SAVE_ADRES_TO_O2-------------:
    v_O2_last5=v_O2_last4;//2019-1-24
    v_O2_last4=v_O2_last3;//2019-1-24
    v_O2_last3=v_O2_last2;//2019-1-24
    v_O2_last2=v_O2_last;//2010-7-10
    v_O2_last=v_O2_ad;

    //jqwd_last3_run=jqwd_last2_run;
    //jqwd_last2_run=jqwd_last_run;
    jqwd_last_run=jqwd;
    jqwd=(byte)((adres_10bit>>2)&0xff);

    //将第二次O2的采集结果与第一次进行平均；
    //并计算本次平均与上次平均值的差异；
    //ADD_ADRES_TO_O2_AND_AVE_IT:
    jqwd=(byte)((jqwd+(byte)((adres_10bit_2>>2)&0xff))/2);
    v_O2_ad=(byte)((jqwd_last_run+jqwd)/2);

    v_nb_OS_AD_CD++;
    
    //2014-7-24 增加氧传感器信号录取
    /*
    if(VF_O2WORK_OSDIAG){
        if(v_O2_ad>=OS_NOTWORK){
            v_O2_ad=v_O2_last;//
            
              /    
        }
    }
    *///2015-5-24 DELETE 因为这个导致氧传感器短时间出现异常，以为是采样原因引起的；
    
     //------CHECK_O2_TREND_AT_LOWSPEED----------
    //if(VF_NEEDRECBYLOWRPM_IGN)
    //2008-5-10 chang condiction to the following:
    if((VF_IDLE_ENG)||((strcls_tpd>0)&&(strcls_tpd<=7)&&(v_TPS_odr<=strcls_tpd)))
    {
        //校验低速时O2的变化趋势；//2008-1-7从SM_RPM中转移过来；
        //CHECK_O2_TREND_AT_LOWSPEED:
        /*
        if(v_O2_ad<v_O2_last-1){ //2010-3-19 add -1;
        	VF_O2DEC_OS=true;
        }
        else{VF_O2DEC_OS=false;}
        
        //2010-7-10
        if(v_O2_last2>v_O2_last&&v_O2_last>v_O2_ad)VF_O2DEC2_OS=true;
        else VF_O2DEC2_OS=false;
        *///2019-1-25 DELETE
        
        //2019-1-25 REPLACED BY THE FOLLOWING,PETER
        
        if(v_O2_ad<v_O2_last-1){ // 2019-1-25这个标志位用于延迟减油
        	VF_O2DEC_OS=true;
        }
        else{VF_O2DEC_OS=false;}
        
        if(VF_IDLE_ENG)
		{
			if(                       //2019-1-25 这个标志位用于增油
            (v_O2_last>v_O2_ad+5)//4  2
            ||
            (v_O2_last2>v_O2_ad+8)//5  3
            ||
            (v_O2_last3>v_O2_ad+8)//5  3
            ||
            (v_O2_last4>v_O2_ad+8)//5  3
            ||
            (v_O2_last5>v_O2_ad+8)//5  3
            
            )VF_O2DEC2_OS=true;
			else VF_O2DEC2_OS=false;
		}
		else
		{
			if(                       //2019-1-29 非怠速需要加严
            (v_O2_last>v_O2_ad+8)//4
            ||
            (v_O2_last2>v_O2_ad+10)//5
            ||
            (v_O2_last3>v_O2_ad+10)//5
            ||
            (v_O2_last4>v_O2_ad+10)//5
            ||
            (v_O2_last5>v_O2_ad+10)//5
            
            )VF_O2DEC2_OS=true;
			else VF_O2DEC2_OS=false;
		}
		
    }
  
    //2008-11-17 增加对OS采样值趋势的判断
    v_O2_trend_DecOrNot=(v_O2_trend_DecOrNot<<1)&0xfffe;
    v_O2_trend_IncOrNot =(v_O2_trend_IncOrNot<<1)&0xfffe;
    if(v_O2_last>v_O2_ad)
    {
        v_O2_trend_DecOrNot++;  //此时0代表不减,1代表减      
    }
    
    if(v_O2_last<v_O2_ad)
    {
        v_O2_trend_IncOrNot++;  //此时0代表不增,1代表增 
    }
  return;
}

//功能：在发动机静止时，只采集一次O2并直接放入O2,而不使用平均
void FG_OS_WhenStop(word adres_10bit)
{
     v_O2_ad=(byte)((adres_10bit>>2)&0xff);
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//氧传感器的波动的检测?


void Detect_OS_Flu(byte tps_odr,byte rev_odr,long int t_eng_s,word prd_cas)
{   
    byte temph;
    sword sw;
    byte ER_LST; 
    byte o2_work_limit_tmp;
    byte prd_cash;
    
    
    //2011-12-4 add
    prd_cash=(byte)(prd_cas>>8);    
        ER_LST=V_OSDIAGHL_STATUS;
        V_OSDIAGHL_STATUS=false;
    //未工作时的初始化
        if(!VF_O2WORK_OSDIAG){
        		VF_O2CHANGED_OSDIAG=false;
        		//v_O2_m_lst=v_O2_ad;    //2010-3-23 CHANGE TO THE FOLLOWING
        		v_O2_m_lst=OS_MAX_AV_INI;
        		v_O2_last=v_O2_ad;
        		VF_SCHO2MIN_OSDIAG=false;//2010-3-19 true to false
        		VF_SCHO2MAX_OSDIAG=true;//      false to true
        		VF_RCHO2MIN_OSDIAG=false;
        		VF_RCHO2MAX_OSDIAG=false;
        		
        		v_O2flu_max_lst=v_O2flu_max_av=OS_MAX_AV_INI;
        		v_O2flu_min_lst=v_O2flu_min_av=OS_MIN_AV_INI;
        		
        		runs_noflu_nocalm=0;
        		
        		v_nb_OS_flu=0;
    		
    		    /*
            #if MV_EXHAUST<=PL50
                if(VF_WOT_TPS)v_O2_notworkLimit=60;//2013-8-8
                else v_O2_notworkLimit=OS_NOTWORK;
            #else */ //2013-8-31 DELETE
                        v_O2_notworkLimit=OS_NOTWORK;
            //#endif
            //v_t_O2work_4s=0;    
            nb_CI_afteroswork=0;
            nb_oser_whenrich=0;
            nb_osflu_whencf=0;
            //2009-12-15
            os_max_afterwork=0;
            t_osconlow_before_osflu_s=0;
            v_nb_flu_lstIdle=0;
        }
        
        //2010-1-29
        o2_work_limit_tmp=v_O2_notworkLimit;
        if(VF_MAYBE_ECE_ENG&&!VF_E0_TPS)    //怠速不减
        {
            o2_work_limit_tmp=v_O2_notworkLimit+5;
        }
        
    //判断os是否工作
        //if(v_O2_ad<o2_work_limit_tmp&&v_O2_last<o2_work_limit_tmp)
        //if(v_O2_ad<o2_work_limit_tmp)//2014-7-12
        //2014-7-24 CHANGE TO THE FOLLOWING
        if(v_O2_ad<o2_work_limit_tmp)//2014-7-12
        {
            nb_os_maynotwork=0;//2014-7-12
            if(VF_4S_TMCCL)
            {
                if(v_t_O2work_4s<255)v_t_O2work_4s++;
                sw=v_t_O2work_4s;
                sw=sw-4;
                sw=sw/4;
                if(sw<0)sw=0;if(sw>7)sw=7;

                /*#if MV_EXHAUST<=PL50   //2013-8-31 ADD
                    if(VF_WOT_TPS)v_O2_notworkLimit=60+sw; //2013-8-8
                    else v_O2_notworkLimit=56+sw;
                #else*/
                    v_O2_notworkLimit=OS_NOTWORK+sw;
                //#endif

            }
            //2010-7-7 move here
            VF_O2WORK_OSDIAG=true; //氧传感器低于限制值超过
            //if(t_eng_s>T_FOR_OSNOFLU_S_MAX)t_whenoswork_s=T_FOR_OSNOFLU_S_MAX; //2014-7-12 DELETE
            //else t_whenoswork_s=(word)t_eng_s; //2014-7-12 DELETE
        } else 
        {
            //if(nb_os_maynotwork<NB_OS_MAYNOTWORK_INI)
            temph=tps_odr+rev_odr+1;//2014-7-12 generator干扰比较厉害    
            if(temph>NB_OS_MAYNOTWORK_INI)temph=NB_OS_MAYNOTWORK_INI;//2014-7-12 generator干扰比较厉害
			if(VF_AQAFTER_PNINI_SYSINI)
			{
				if(VF_O2WORK_OSDIAG&&(v_O2_ad<=(o2_work_limit_tmp+2)))
				{					
					nb_os_maynotwork=0;
				}
				else if(VF_O2WORK_OSDIAG)
				{
					temph=100;
				}
			}
            if(nb_os_maynotwork<(temph))
            {
                nb_os_maynotwork++;//2014-7-12 generator干扰比较厉害    
            } else{
                v_O2_notworkLimit=OS_NOTWORK;//2014-7-12 ADD 
                
                v_t_O2work_4s=0;
                VF_O2WORK_OSDIAG=false;
            }
        }
        //2010-7-7 moeve
        /*
        if(v_t_O2work_4s>=1)//2010-4-28 add =
        {
            VF_O2WORK_OSDIAG=true; //氧传感器低于限制值超过
            if(t_eng_s>T_FOR_OSNOFLU_S_MAX)t_whenoswork_s=T_FOR_OSNOFLU_S_MAX;
            else t_whenoswork_s=(word)t_eng_s;
        } 
        */
    //v_O2_m_lst:代表本次搜索过程中遇到的极值；
        temph=DELTA_FLU_BASIC+tps_odr/2+((rev_odr/2)&0x0f);
        
        //2010-7-14
        //2019-5-26 取消掉，初始化调试过程幅值太小，容易误判导致学习偏浓if(VF_AQAFTER_PNINI_SYSINI&&VF_E0_TPS&&VF_O2CHANGED_OSDIAG)temph/=2;
        
        
        /*    2014-7-24
        发现增加氧传感器抗干扰之后，
        大的OS值回严重改变OS的目标值
        */
        
        //v_O2_ad;
            
        if(VF_O2WORK_OSDIAG)
        {
            if(VF_SCHO2MAX_OSDIAG||VF_SCHO2MIN_OSDIAG){
                if(v_O2_last>=v_O2_ad){                //goto o2fall;
              			if(VF_SCHO2MIN_OSDIAG){
              				if(v_O2_m_lst>v_O2_ad)v_O2_m_lst=v_O2_ad;//	v_O2_m_lst=min(v_O2_m_lst,v_O2_ad);
              				//2008-10-12 注释：如果正在找最小值，且本次的采样值小等于上次值，
              				//则记住这次的值，作为寻找过程中的极值； 
              				//2009-12-16 add 应对杨老师提到的看到的最大值总比软件显示的要大
              				/*
              				if(v_O2_last>v_O2flu_max_lst&&v_O2_last>v_O2_ad+1)
              				{
              					v_O2flu_max_lst=v_O2_m_lst;//o2_MAX=o2_M_t;
              				    v_O2_m_lst=v_O2_ad;
              					v_O2flu_max_av=Ave_by(v_O2flu_max_av,v_O2_m_lst,NB_FORAVE_M);
              				}*///2010-3-23 delete
              			}
              			else{//找最大值时出现O2值在变小，当减小量大于一定值（考虑TPS和转速因素）时，认为找到最大值了；
              				if(v_O2_ad<v_O2_m_lst-temph){  //2010-3-19发现这里符号有问题，由加更改为减；
              					v_O2flu_max_lst=v_O2_m_lst;//o2_MAX=o2_M_t;
              					//2013-4-11 注释：这里是浓向稀切换时
              					//w=o2_max-o2_min;
              					//more_to_o2sum(o2_max-o2_min);
              					v_O2_m_lst=v_O2_ad;
              					VF_SCHO2MAX_OSDIAG=false;
              					VF_SCHO2MIN_OSDIAG=true;
              					VF_RCHO2MAX_OSDIAG=true;
              					if(v_nb_OS_flu<NB_OSFLU_LIMIT_MAX)v_nb_OS_flu++;
              					if(v_nb_flu_lstIdle<255)v_nb_flu_lstIdle++;//2010-5-4
              					//v_O2flu_max_av=Ave_by(v_O2flu_max_av,v_O2_m_lst,NB_FORAVE_M);
              					v_O2flu_max_av=Ave_by(v_O2flu_max_av,v_O2flu_max_lst,NB_FORAVE_M);  //2010-3-23
              					runs_noflu_nocalm=0;
              					if(nb_osflu_whencf<=NB_OSFLU_WHENCF_LIMIT)nb_osflu_whencf++ ;
								
								v_t_OS_Lt20_to_now=0; 

              					//if(v_nb_flu_forSL<255)v_nb_flu_forSL++; 2013-4-12 DELETE
              					
              				}
              			}
              		}
              		else{//o2rise;可能得到最小值,如果o2比前一个极值大（8+v_TPS_odr/2),就将前一个极值作为o2_min
              			if(VF_SCHO2MAX_OSDIAG){//O2_TO_O2MAX
              				if(v_O2_m_lst<v_O2_ad)v_O2_m_lst=v_O2_ad;//	O2_M_T=max(O2_M_T,v_O2_ad);
              				/*
              				//2009-12-16 add 应对杨老师提到的看到的最大值总比软件显示的要大
              				if(v_O2_last<v_O2flu_min_lst&&v_O2_last+1<v_O2_ad)
              				{
              					v_O2flu_min_lst=v_O2_m_lst;//o2_MAX=o2_M_t;
              				    v_O2_m_lst=v_O2_ad;
              					v_O2flu_min_av=Ave_by(v_O2flu_min_av,v_O2_m_lst,NB_FORAVE_M);
              				}*///2010-3-23 delete
              			}
              			else{
              				if(v_O2_ad>=v_O2_m_lst+temph){
              				    //2008-10-12 注释?
              				    // 如果在找最小值，且采样值大于寻找过程中的极值+某个值；
              				    //则认为找到极小值，即当时的极值作为极小值；?
              					//2013-4-11 注释：这里是稀向浓切换时
              					v_O2flu_min_lst=v_O2_m_lst;
              					//more_to_o2sum(o2_max-o2_min);//w=o2_max-o2_min;
              					v_O2_m_lst=v_O2_ad;
              					VF_SCHO2MAX_OSDIAG=true;
              					VF_SCHO2MIN_OSDIAG=false;
              					VF_RCHO2MIN_OSDIAG=true;
              					if(v_nb_OS_flu<NB_OSFLU_LIMIT_MAX)v_nb_OS_flu++;
              					//v_O2flu_min_av=Ave_by(v_O2flu_min_av,v_O2_m_lst,NB_FORAVE_M);
              					v_O2flu_min_av=Ave_by(v_O2flu_min_av,v_O2flu_min_lst,NB_FORAVE_M);
              					runs_noflu_nocalm=0;
              					if(nb_osflu_whencf<=NB_OSFLU_WHENCF_LIMIT)nb_osflu_whencf++ ;

                  			if(v_nb_flu_forSL<255)v_nb_flu_forSL++;
                  			
                  			//2013-4-12　避开可能是假信号的区域
                  			//if(!(VF_E0_TPS&&v_rev_625rpm>REV62D5_2000)){
                  			if(!(VF_E0_TPS&&VF_GT10_IDLETG_REV&&!VF_AQAFTER_PNINI_SYSINI)){ //2013-4-17
                  			
                  			//rev15d6_target
                  					if(v_OS_flu_SLpoint<255)v_OS_flu_SLpoint++;//2013-3-8 
                  			}
                  			
              					if(v_os_flu_trans<255)v_os_flu_trans++; //2013-4-11 MOVE HERE
								
								v_t_OS_Lt20_to_now=0;

              				}
              			}
              		}
            }
            else{v_O2_m_lst=v_O2_ad;}//;goto O2_M_GET_END;
        //认为刚开始反馈的时刻；
            if(v_nb_OS_flu<=NB_OSFLU_LIMIT_FORCHANGED)
            {
                v_O2flu_min_av=v_O2flu_min_lst;
                v_O2flu_max_av=v_O2flu_max_lst;
                if(v_nb_OS_flu==NB_OSFLU_LIMIT_FORCHANGED)VF_O2CHANGED_OSDIAG=true;
            }
        }
    
    //反馈后，对极值 的特殊处理
        //if(VF_O2CHANGED_OSDIAG)
        if(VF_O2CHANGED_OSDIAG) //2010-3-23 CHANGE 
        {
            //当os没有波动时，根据os在一直偏浓或一直偏稀 的结果来修正最大值和最小值
            if(runs_noflu_nocalm>RUNS_NOFLU_NOCALM_LIMIT)
            {
                //if((VF_SCHO2MIN_OSDIAG||VF_LEANLIMIT_OSDIAG)&&VF_RICH_OSDIAG)
                if((VF_SCHO2MAX_OSDIAG||VF_LEANLIMIT_OSDIAG)&&VF_RICH_OSDIAG&&v_O2flu_max_av>v_O2_ad) //2010-3-24 change 
                {                                 //VF_LEANLIMIT_OSDIAG主要针对不能偏稀的情况，例如油门全开；
                    if(v_O2_ad>=temph+v_O2flu_min_av)v_O2flu_max_av=Ave_by(v_O2flu_max_av,v_O2_ad,NB_FORAVE_M);    
                }
                //else if(VF_SCHO2MAX_OSDIAG)
                else if(VF_SCHO2MIN_OSDIAG)//2010-3-24chage 
                {
                    if(v_O2_ad<v_O2flu_min_av&&!VF_RICH_OSDIAG)
                    {
                        if(v_O2_ad+temph<=v_O2flu_max_av)v_O2flu_min_av=Ave_by(v_O2flu_min_av,v_O2_ad,NB_FORAVE_M);    
                    }
                } 
                runs_noflu_nocalm=0;
            }
            runs_noflu_nocalm++;
        }

        //2014-12-30 CHANGE TO THE FOLLOWING
          if(v_O2flu_max_av<v_O2_ad)v_O2flu_max_av++;
          if(v_O2flu_min_av>v_O2_ad)v_O2flu_min_av--;

    
    //对极大值和极小值得平均值进行校核；
        if(v_O2flu_max_av<=OSMAX_LLIMIT)
        {
            v_O2flu_max_av=OSMAX_LLIMIT;
            //波动幅值过小；
        }

        //2014-12-22 ADD 
            if(v_nb_OS_flu==0){
                if(!VF_STARTS_ENG){
                      if(v_O2_last>40&&v_O2_last<OS_MAX_AV_INI){
                          if(v_O2flu_max_av>v_O2_last)v_O2flu_max_av=v_O2_last;    
                      }
                }
            }


        if(v_O2flu_max_av<v_O2flu_min_av+DELTA_MAXTOMIN_LIMIT)v_O2flu_min_av=v_O2flu_max_av-DELTA_MAXTOMIN_LIMIT;

        ///2014-12-22  //2014-12-24
        
          if(v_O2flu_max_av>OS_MAX_AV_INI)v_O2flu_max_av=OS_MAX_AV_INI;
          if(v_O2flu_min_av>30)v_O2flu_min_av=30;

        
    //计算os工作后的目标值 v_O2_lambdaE1     //temph
        //v_O2_lambdaE1=(v_O2flu_max_av+v_O2flu_min_av)/2;
        //v_O2_lambdaE1=(v_O2flu_max_av+(word)v_O2flu_min_av*3)/4;//2014-3-6
        //v_O2_lambdaE1=(v_O2flu_min_av+(word)v_O2flu_max_av*3)/4;//2014-3-6
        v_O2_lambdaE1=(v_O2flu_min_av+(word)v_O2flu_max_av*2)/4;//2015-3-29 发现上面的有时OS目标还是会比较大，因此改成2/4
        //v_O2_lambdaE1=v_O2flu_max_av-
        
        sw=(sword)v_O2flu_max_av-v_O2flu_min_av;
        if(sw<temph)sw=temph;          //注意：需要防止os在全浓时自身的波动；
        if(sw>16)sw=16;
        sw=sw/2;
        
        
        sw-=2;
        if(sw<3)sw=3;
           
        sw=v_O2flu_max_av-sw;
        if(v_O2_lambdaE1>sw)v_O2_lambdaE1=sw;

       // sw=v_O2flu_min_av+4;
       // if(v_O2_lambdaE1<sw)v_O2_lambdaE1=sw;
        
        /*
        #if MV_EXHAUST<=PL50 //2013-8-8
            if(VF_WOT_TPS){
                if(v_O2_lambdaE1>40)v_O2_lambdaE1=40;    
            }
        #endif
        */
        
        if(!VF_E0_TPS||VF_CICORECING_CFCI||!VF_O2CHANGED_OSDIAG||prd_cash<PCA1831)
        {                                 //2011-12-4 add condiction prd_cash<PCA1831
            v_nb_flu_lstIdle=0;
        } 
    //稀浓判断
          
    
        VF_RICHLST_OSDIAG=VF_RICH_OSDIAG;//  2010-4-26 move to o2.c from cls_sl

        VF_RICH_OSDIAG=false;
        if(v_O2_ad>=v_O2_lambdaE1||VF_STARTS_ENG)VF_RICH_OSDIAG=true;
        
        //2012-6-28
        /*
        if(VF_E0_TPS)VF_1LEAN_NOTIDLE_CLS=false;
        else if(!VF_RICH_OSDIAG&&VF_RICHLST_OSDIAG)VF_1LEAN_NOTIDLE_CLS=true;
        */
        /*
        if(VF_RICHLST_OSDIAG!=VF_RICH_OSDIAG)
        {
            if(v_nb_flu_lstIdle<255)v_nb_flu_lstIdle++ ;
        }
         *///2010-5-4 发现这个条件不合理，更改
         
    //诊断
		//1 偏浓归零；       判断方法，持续偏西后，先给个标志，扩大反馈限制的范围，如果依然偏稀，则认为有问题
        if(VF_O2CHANGED_OSDIAG)
        {
            if(!VF_RICH_OSDIAG&&VF_LIMITFK_CLS&&!VF_LEANCON_CLS
                //&&(no_t3_fk_run==0) //2013-7-15 add for O2.C , 避开T1被限制的情况  //2014-10-8 DELETE ,为了与INJ_CTL.H不关联
                
                )
                //2012-12-7 add VF_LEANCON_CLS
            {
                if(VF_1S_TMCCL)t_con_lean++;
                if(VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG||VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG)
                {
                    if(t_con_lean >=T_FOR_CON_LEAN_ENLARGE_S)   //稀超过2秒后，开始测试
                    {
                        VF_OS_LEAN_WHENRICH_OSDIAG=true; 
                        t_con_lean=0;   
                    }
                } 
                else  //如果反馈没有放大开：
                {     
                    if(!VF_TESTFOROSLEAN_OSDIAG)    //如果没有测试是否偏稀
                    {
                        if(t_con_lean >=T_FOR_CON_LEAN_S)   //稀超过2秒后，开始测试
                        {
                            //t_con_lean=T_FOR_CON_LEAN_S;
                            VF_TESTFOROSLEAN_OSDIAG=true;
                            t_con_lean=0;    
                        }
                    } else
                    {    //当测试稀时，仍出现反馈到头，且偏稀的情况，则认为有问题
                        /*if(t_con_lean>=T_FOR_CON_LEAN_ENLARGE_S)
                        {
                            if(nb_oser_whenrich<255)nb_oser_whenrich++;
                            if(nb_oser_whenrich>NB_OSER_WHENRICH_LIMIT)VF_OS_LEAN_WHENRICH_OSDIAG=true;  
                        } */
                        /*
                        if(VF_MAX_TPS)
                        {
                            VF_OS_LEAN_WHENRICH_OSDIAG=true;
                            t_con_lean=0;    
                        }else*/
                        {
                            if(t_con_lean<T_FOR_TEST_LEAN)
                            {
                                if(((t_con_lean&0x01)==0)&&!VF_ENRICH_FORTESTLEAN_OSDIAG)//每一秒试探一次
                                {
                                    VF_ENRICH_FORTESTLEAN_OSDIAG=true; 
                                    nb_for_leantest=0;   
                                }
                            } 
                            else
                            {   //试探一定时间后，仍没有浓信号，则认为有故障
                                VF_OS_LEAN_WHENRICH_OSDIAG=true; 
                                t_con_lean=0;       
                            }
                        }
                    }
                }
            } else
            {
                t_con_lean=0;
               /* if(VF_TESTFOROSLEAN_OSDIAG&&VF_RICH_OSDIAG)
                {
                    if(VF_IDLE_ENG)VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG=true;
                    //VF_TESTFOROSLEAN_OSDIAG=false;
                }   
                 */
                if(VF_TESTFOROSLEAN_OSDIAG&&VF_ENRICH_FORTESTLEAN_OSDIAG&&VF_RICH_OSDIAG)
                {
                    if(VF_IDLE_ENG)
                    {
                        VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG=true;    
                    } else
                    {
                        VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG=true;    
                    }
                }
            }
            
        }else 
        {
            VF_TESTFOROSLEAN_OSDIAG=false;
            t_con_lean=0;
        }
        if(VF_JLEAVEIDLE_ENG||VF_JBACKIDLE_ENG)VF_TESTFOROSLEAN_OSDIAG=false;  //2009-12-13 离开怠速后
        
        if(VF_MAX_TPS!=VF_MAXLST_TPS)VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG=false;
        
        //2 波动后幅值过小，最大值偏低
        if(VF_O2CHANGED_OSDIAG)
        {   //反馈的最大值偏低
            if(v_O2flu_max_av<=OSMAX_LLIMIT)
            {
                if(VF_1S_TMCCL)
                {
                    if(nb_flusamll<NB_OSFLUSMALL_LIMIT)
                    {
                        // 时认为有问题；
                        nb_flusamll++;
                        if(nb_flusamll>=NB_OSFLUSMALL_LIMIT)
                        {
                            VF_FLUSMALL_OSDIAG=true;
                            nb_flusamll=0;
                        }
                    }
                }
            } else nb_flusamll=0; 
            /*
            //反馈的幅值偏低
            if(!VF_IDLE_ENG&&!VF_LEANLIMIT_OSDIAG)//
            {
                if(v_O2flu_max_lst<v_O2flu_min_lst)
                {
                            
                }
            } */

        } else nb_flusamll=0;
        
        //3 持续高电位   发动机运行超过一定时间，这个时间与发动机工作的负荷有关
		
		//2018-7-6 增加断油时间
		if(VF_IGNONLY_CFCI)
		{
			if(VF_1S_TMCCL&&(v_cf_1s<255))v_cf_1s++;
		}
		else
		{
			v_cf_1s=0;
		}
		
        //if(!VF_O2WORK_OSDIAG&&!VF_WORM_ENG)
		if(!VF_O2WORK_OSDIAG&&!VF_WORM_ENG&&(v_cf_1s<20))//2018-7-6 断油时间超过20s后不判断该故障
        {
            if(VF_1S_TMCCL)
            {
                if(t_os_notwork<=T_FOR_OSNOWORK_S_MAX)t_os_notwork++;
            }
            if(VF_JCI_CFCI)t_con_for_oswork-=T_STEP_FOR_1JLEAVEIDLE;
            if(VF_JLEAVEIDLETRANS_ENG)t_con_for_oswork-=T_STEP_FOR_1JCI;
            if(t_con_for_oswork<T_FOR_OSNOWORK_S_MIN)t_con_for_oswork=T_FOR_OSNOWORK_S_MIN;  
            if(t_os_notwork>=t_con_for_oswork)
            {
                VF_CANNOTWORK_OSDIAG=true; 
                t_os_notwork=0;       
            }
        }
        //4 os一直不波动超过一定时间		
		if(!VF_O2WORK_OSDIAG&&VF_IGNONLY_CFCI)
		{
			VF_O2NOTWORK_CF=true;//2018-7-6 断油过程引起的OS不工作不作为判断条件
		}
		if(VF_O2CHANGED_OSDIAG)
		{
			VF_O2NOTWORK_CF=false;//OS波动后复位
		}
		
        if(VF_O2WORK_OSDIAG&&!VF_WORM_ENG&&!VF_O2CHANGED_OSDIAG&&!VF_O2NOTWORK_CF)
        {
            if(os_max_afterwork<v_O2_ad) os_max_afterwork=v_O2_ad;
           
             
            //2016-11-15 发现，下面这句话怎么是被屏蔽的？？？ 
                if(v_O2_ad<OSMAX_LLIMIT)
                {
                    if(VF_1S_TMCCL)
					{
						if(t_osconlow_before_osflu_s<=T_FOR_OSNOFLU_S_MAX) t_osconlow_before_osflu_s++;
					}
                }
				else 
				{
					t_osconlow_before_osflu_s=0;
				}
            /*
            if(VF_1S_TMCCL)
            {
                if(t_osconlow_before_osflu_s<T_FOR_OSNOFLU_S_MAX) t_osconlow_before_osflu_s++;
            }
			*/

            if(VF_JCI_CFCI)
            {
                t_con_for_osflu-=T_STEP_FOR_1JLEAVEIDLE;
                if(nb_CI_afteroswork<NB_CILIMIT_FOR_OSNOCHANGE)
                {
                    nb_CI_afteroswork++;
                } 
            }
            if(VF_JLEAVEIDLETRANS_ENG)t_con_for_osflu-=T_STEP_FOR_1JCI;
            if(t_con_for_osflu<T_FOR_OSNOFLU_S_MIN)t_con_for_osflu=T_FOR_OSNOFLU_S_MIN;  
            
            if((t_osconlow_before_osflu_s>=t_con_for_osflu)
                    ||(nb_CI_afteroswork>=NB_CILIMIT_FOR_OSNOCHANGE&&t_osconlow_before_osflu_s>T_FOR_OSNOFLU_S_MIN)
                )
            {
                VF_CANNOTFLU_OSDIAG=true;
                nb_CI_afteroswork=0;
                t_osconlow_before_osflu_s=0;
                
                //2016-11-15
                if(os_max_afterwork<=OS_FORSHORTCUT){
                    VF_OSSHORTCUT_OSDIAG=true;
                    VF_CANNOTFLU_OSDIAG=false;
                }
            }
        }
        
        //5 无规则乱变
        if(VF_O2CHANGED_OSDIAG)
        {
            if(VF_CF_CFCI)
            {
                if(VF_1S_TMCCL)
                {
                    if(t_after_jci_s<255)t_after_jci_s++;
                }
                /*if(t_after_jci_s>=T_FOR_CI_DETECT_S)
                {
                    if(v_O2_ad>OS_CF_LIMIT)
                    {
                        if(nb_osflu_whencf<=NB_OSFLU_WHENCF_LIMIT)nb_osflu_whencf++ ;  
                        if(nb_osflu_whencf>=NB_OSFLU_WHENCF_LIMIT)
                        {
                            VF_OUPUT_WHENLEAN_OSDIAG=true; 
                            nb_osflu_whencf=0;//
                            t_after_jci_s=0;   
                        }
                    }    
                } */
                if(t_after_jci_s==T_FOR_CI_DETECT_S)
                {
                    nb_osflu_whencf=0;    
                } 
                else if(t_after_jci_s>T_FOR_CI_DETECT_S)
                {    //2013-3-13 ADD =
                    if(nb_osflu_whencf>= NB_OSFLU_WHENCF_LIMIT)VF_OUPUT_WHENLEAN_OSDIAG=true;//2009-12-18   
                    //2013-3-13 ADD 
                    temph=v_O2flu_min_av+3;
                    //v_O2_last=v_O2_ad
                    if(v_O2_ad>temph&&v_O2_last>temph&&v_O2_last2>temph){
                        VF_O2WORK_OSDIAG=false;    
                    }
                    
                }
            } else 
            {
                t_after_jci_s=0;
            }
        }else t_after_jci_s=0; 
		
		//2021-10-14 新增断路故障
		if(VF_MMOVE_ENG&&(v_t_EngMove_s>200)&&(!VF_O2WORK_OSDIAG))   //50调整为200
        {
        	//if(os_min_afterstart==0)	os_min_afterstart=v_O2_ad;
			
            if(os_min_afterstart>v_O2_ad) os_min_afterstart=v_O2_ad;
           

            if(v_O2_ad>240)
            {
                if(VF_1S_TMCCL)
				{
					if(t_os_highvolt<255) t_os_highvolt++;
				}
            }
			
			
			if((t_os_highvolt>100))
			{
				VF_OS_HIGHVOLT=true;
			}


        }

		else 
		{
			t_os_highvolt=0;
		}
		
        
        VF_OSER_OSDIAG=VF_OUPUT_WHENLEAN_OSDIAG||VF_CANNOTFLU_OSDIAG 
                        ||VF_CANNOTWORK_OSDIAG||VF_FLUSMALL_OSDIAG
                        ||VF_OS_LEAN_WHENRICH_OSDIAG;
        
         
         
        //检测到os故障后的处理；
        if(VF_OSER_OSDIAG)
        {
            //2019-1-7 if(VF_CANNOTFLU_OSDIAG ||VF_CANNOTWORK_OSDIAG||VF_FLUSMALL_OSDIAG)
			if(VF_OSSHORTCUT_OSDIAG ||VF_CANNOTWORK_OSDIAG||VF_OS_HIGHVOLT)
			{
				VF_O2WORK_OSDIAG=false ;
			}			
            t_con_for_oswork=T_FOR_OSNOWORK_S_MAX;
            t_con_for_osflu=T_FOR_OSNOFLU_S_MAX ;
            //v_t_O2work_4s=0;
            v_t_O2work_4s=0;
        }
        
        //2009-12-新老故障或；
        V_OSDIAGHL_STATUS=ER_LST|V_OSDIAGHL_STATUS;

    //2009-12-24
    //对os响应速度的判断
    //ccl_os_conlean ccl_os_conrich ccl_os_fkfast NB_CCL_OSFKFAST_LEANRICH_LIMIT NB_CCL_OSFKFAST_SUM_LIMIT 
    /*
    if(VF_O2CHANGED_OSDIAG&&VF_O2WORK_OSDIAG)
    {
        // ccl_os_fk_limit=T_OS_RESPONSE_4US/周期+2;
        if(prd_cas>=T_OS_RESPONSE_4US)ccl_os_fk_limit=3;
        else ccl_os_fk_limit=T_OS_RESPONSE_4US/prd_cas+2;
        
        if(VF_RICH_OSDIAG)
        {
            if(ccl_os_conlean<=ccl_os_fk_limit)ccl_os_conlean++; 
            ccl_os_conrich=0;       
        } else 
        {
            if(ccl_os_conrich<=ccl_os_fk_limit)ccl_os_conrich++;  
            ccl_os_conlean=0;      
        }
        if(ccl_os_conlean>=ccl_os_fk_limit
                ||ccl_os_conrich>=ccl_os_fk_limit)
        {
            if(ccl_os_conlean>=ccl_os_fk_limit+4
                ||ccl_os_conrich>=ccl_os_fk_limit+4)
            {
                nb_ccl_maybenotfast=0;
                ccl_os_fkfast=0;    
            } else 
            {
                if(nb_ccl_maybenotfast<NB_MAYBENOTFAST_LIMIT)nb_ccl_maybenotfast++ ;
                else 
                {
                    nb_ccl_maybenotfast=0;
                    ccl_os_fkfast=0;    
                }
            }
            
            
        } else 
        {
            nb_ccl_maybenotfast=0;
            if(ccl_os_fkfast<=NB_CCL_OSFKFAST_SUM_LIMIT)ccl_os_fkfast++;
            else 
            {
                VF_O2FKFAST_OS=true;    
            }
        }
    } else 
    {
        ccl_os_conrich=0;    
        ccl_os_conlean=0;   
        nb_ccl_maybenotfast=0;
        ccl_os_fkfast=0; 
        VF_O2FKFAST_OS=false;
    }
    *///2009-12-29 因资源不足，临时删除；
        //驾驶技巧：
        /*如果车辆老是因为偏稀导致怠速呆不住，通过轰油门让os热起来后，
                再回油门，如果在恢复供油时发现偏稀，则增油多一些；
        */
        
    return;
}

//根据新值进行平均，返回值为平均值；
//参数含义：老平均值，新值，平均次数 
byte Ave_by(byte oldValue,byte newValue,byte Avetime )
{
    sword sw;
    
    sw=(sword)oldValue*(Avetime-1);
    sw=sw+newValue;
    sw=sw/Avetime;
    if(sw<0)sw=0;
    if(sw>255)sw=255;
    return (byte)sw;
}

