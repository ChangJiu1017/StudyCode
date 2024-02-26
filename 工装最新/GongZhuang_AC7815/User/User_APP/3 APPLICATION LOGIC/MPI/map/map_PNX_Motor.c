/*HEAD			
 ***********************************************************************			
 * Filename	:map_PNX_Motor.c		
 * Function	:parameter relative to vehicle type.		
 * User		:xielian	
 * Date		:2017.1.20	
 * Version	:V8.1.1		
 *			
 *** History ***			
 * V8.1.0	2017.1.20	xielian 	
 *		Base. 
 * V8.2.0   2018.9.26	xielian
 *		车型相关预编译参数定义的调整
 ***********************************************************************			
HEAD END*/

#include "public.h"
#include "add.h"

#include "map_selection.h"//2013-4-16

const byte MO_STEP_FMF_IDLE_RICHTOLEAN= 2;//4 ;        //怠速反馈，由浓变稀，需要增加的油量，单位：同FMF 
const byte MO_STEP_FMF_IDLE_LEANTORICH= 2;//4 ;     //怠速反馈，由稀变浓，需要减掉的油量，单位：同FMF



/*--跟模块相关的map----------------------------*/

     const word   M0_MS_RPM_DOWN  =           0;//怠速计算时，REMARK被调到六，此时上位机对转速的限制范围为1350-1550;
                                        //而对于不同车型，需要调整的怠速范围不同，就是通过实际值减该值来区分； 
    // #ifndef SYS_TRADITIONAL
	// const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
     //#else
	 const word   INJ_REACH_BMS =     0x03e8;//  共轨系统喷嘴无效时间约1mm，燃油飞行时间约30mm/ms
	 //#endif
	 const byte   PRE_OIL_INC  =           50;//预喷基础油量，量纲4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//怠速油量，量纲4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	30;//离开怠速时有加油和加喷策略，防止连续两次离开怠速的时间间距过短，设置两次之间的最小间隔为WAIT_RUN_STARTB，量纲圈			
                                              //2010-1-11 from 40 to 10
                                              //2012-12-7 FROM 10 TO 20，抖动油门过浓
    //－－－－－for idle missfire－－－－－－－－－－－－－－－－－－－－－
     const byte   MAX_TI_INC_DRPM	=		40;//25;//失火检测的最大增油量，量纲4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		12;//当累计失火增量大于该值时，只对进行临时失火增油//equ		12		;2007-7-17反馈时的累计失火最大48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//两圈内转速下降>=93 (6*15.625),且O2dec,作为稀,增4US，量纲：15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//两圈内转速下降>=109 (7*15.625),作为失火 ，量纲：15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//两个循环内下降>=156作为燃烧不好，量纲：15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//三两个循环内下降>=187作为燃烧不好，量纲：15.6rpm
     const byte   STEP_TI_INC_AMISF=	3;//2009-12-14 	1;//2008-5-30 3//一次失火的增油量,，量纲4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=	2;//2009-12-14	1;//2008-5-30 2//一次燃烧不好的增油 ，量纲4US;
     const byte M0_IGN_MAX_L6000RPM=50;							//35		高转速时,对怠速点火角的限制							
     const byte M0_IGN_MAX_GT6000RPM=57;							//40		57.14285714							
     //const byte M0_CO_LSPEED_TO16=12;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;
     //const byte M0_CO_LSPEED_TO16=16;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
     //const byte M0_CO_LSPEED_TO16=2;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
     const byte M0_CO_LSPEED_TO16=8;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
                                   //2013-1-27 BACKTO 8
    

    const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;怠速连续浓,减油的正常圈数,连续??6圈,减增一次

/*
//2010-3-17 ADD 
#if(MV_VEHICLE== WILD)
    const word   M0_MS_RPM_DOWN  =           0;//怠速计算时，REMARK被调到六，此时上位机对转速的限制范围为1350-1550;
                                        //而对于不同车型，需要调整的怠速范围不同，就是通过实际值减该值来区分； 
    //const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
    #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	const byte   PRE_OIL_INC  =           50;//预喷基础油量，量纲4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//怠速油量，量纲4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
    const byte WAIT_RUN_STARTB  =      40;//离开怠速时有加油和加喷策略，防止连续两次离开怠速的时间间距过短，设置两次之间的最小间隔为WAIT_RUN_STARTB，量纲圈			
                                              //2010-1-11 from 40 to 10
                                              //2010-6-4 chang from 10 to 40
    //－－－－－for idle missfire－－－－－－－－－－－－－－－－－－－－－
    const byte MAX_TI_INC_DRPM	=		40;//25;//失火检测的最大增油量，量纲4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
    const byte MAX_MISS_FIRE_DTI=		12;//当累计失火增量大于该值时，只对进行临时失火增油//equ		12		;2007-7-17反馈时的累计失火最大48US
    const byte RPM_DEC_FOR_LEANS=	10;//	9;//2008-5-30 change from 5 to 8//两圈内转速下降>=93 (6*15.625),且O2dec,作为稀,增4US，量纲：15.6rpm
                                       
    const byte RPM_DEC_FOR_MISFIRE	=12;//	11;//2008-5-30 7//两圈内转速下降>=109 (7*15.625),作为失火 ，量纲：15.6rpm
    const byte RPM_DEC_2C_MISSFIRE=	15;//	14;//2008-5-30 11//两个循环内下降>=156作为燃烧不好，量纲：15.6rpm
    const byte RPM_DEC_3C_MISSFIRE	=17;//	16;//2008-5-30 13//三两个循环内下降>=187作为燃烧不好，量纲：15.6rpm
    const byte STEP_TI_INC_AMISF=	3;//2009-12-14 	1;//2008-5-30 3//一次失火的增油量,，量纲4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
    const byte STEP_A_SMALL_FIRE=	2;//2009-12-14	1;//2008-5-30 2//一次燃烧不好的增油 ，量纲4US;
    const byte M0_IGN_MAX_L6000RPM=50;							//35		高转速时,对怠速点火角的限制							
    const byte M0_IGN_MAX_GT6000RPM=57;							//40		57.14285714							
     //const byte M0_CO_LSPEED_TO16=12;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;
    const byte M0_CO_LSPEED_TO16=8;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi

    
    const byte M0_CCL_DERICH_CNTRICH_IDLE  =     24;//;怠速连续浓,减油的正常圈数,连续??6圈,减增一次
                                                    //2010-7-10 from 24 to 48　　2010-7-30 from 48 to 24

#endif

#if(MV_VEHICLE== CUB)
     const word   M0_MS_RPM_DOWN  =           0;//怠速计算时，REMARK被调到六，此时上位机对转速的限制范围为1350-1550;
                                        //而对于不同车型，需要调整的怠速范围不同，就是通过实际值减该值来区分； 
     //const word   INJ_REACH_BMS =     0x0753;//从喷油器开始加T1到燃油到达进气门之间的时间 
     #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	 const byte   PRE_OIL_INC  =           50;//预喷基础油量，量纲4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//怠速油量，量纲4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	40;//离开怠速时有加油和加喷策略，防止连续两次离开怠速的时间间距过短，设置两次之间的最小间隔为WAIT_RUN_STARTB，量纲圈			

    //－－－－－for idle missfire－－－－－－－－－－－－－－－－－－－－－
     const byte   MAX_TI_INC_DRPM	=		15;//失火检测的最大增油量，量纲4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		8;//当累计失火增量大于该值时，只对进行临时失火增油//equ		12		;2007-7-17反馈时的累计失火最大48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//两圈内转速下降>=93 (6*15.625),且O2dec,作为稀,增4US，量纲：15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//两圈内转速下降>=109 (7*15.625),作为失火 ，量纲：15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//两个循环内下降>=156作为燃烧不好，量纲：15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//三两个循环内下降>=187作为燃烧不好，量纲：15.6rpm
     const byte   STEP_TI_INC_AMISF=		1;//2008-5-30 3//一次失火的增油量,，量纲4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=		1;//2008-5-30 2//一次燃烧不好的增油 ，量纲4US;
    const byte M0_IGN_MAX_L6000RPM=43;								//30			高转速时,对怠速点火角的限制							
    const byte M0_IGN_MAX_GT6000RPM=50;								//35										
    const byte M0_CO_LSPEED_TO16=8;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;

     const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;怠速连续浓,减油的正常圈数,连续??6圈,减增一次

#endif

#if(MV_VEHICLE== SCOOTER)
     const word   M0_MS_RPM_DOWN  =           0x200;//怠速计算时，REMARK被调到六，此时上位机对转速的限制范围为1350-1550;
                                        //而对于不同车型，需要调整的怠速范围不同，就是通过实际值减该值来区分； 
     //const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
     #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//从喷油器开始加T1到燃油到达进气门之间的时间 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	 const byte   PRE_OIL_INC  =           50;//预喷基础油量，量纲4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE    =             72;//怠速油量，量纲4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     //const byte   M0_FMF_IDLE    =             90;//怠速油量，量纲4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	30;//离开怠速时有加油和加喷策略，防止连续两次离开怠速的时间间距过短，设置两次之间的最小间隔为WAIT_RUN_STARTB，量纲圈			
    
    //－－－－－for idle missfire－－－－－－－－－－－－－－－－－－－－－
     const byte   MAX_TI_INC_DRPM	=		25;//失火检测的最大增油量，量纲4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		12;//当累计失火增量大于该值时，只对进行临时失火增油//equ		12		;2007-7-17反馈时的累计失火最大48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//两圈内转速下降>=93 (6*15.625),且O2dec,作为稀,增4US，量纲：15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//两圈内转速下降>=109 (7*15.625),作为失火 ，量纲：15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//两个循环内下降>=156作为燃烧不好，量纲：15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//三两个循环内下降>=187作为燃烧不好，量纲：15.6rpm
     const byte   STEP_TI_INC_AMISF=		1;//2008-5-30 3//一次失火的增油量,，量纲4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=		1;//2008-5-30 2//一次燃烧不好的增油 ，量纲4US;
    const byte M0_IGN_MAX_L6000RPM=43;								//30			高转速时,对怠速点火角的限制							
    const byte M0_IGN_MAX_GT6000RPM=50;								//35										
    const byte M0_CO_LSPEED_TO16=8;// 低速修正油量=((-转速+1500)/15.6)*M0_CO_LSPEED_TO16/16;

     const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;怠速连续浓,减油的正常圈数,连续??6圈,减增一次

#endif
*/
//--for cf－－－－－－－－－－－－
const byte M0_MIN_STOPOIL_RUN=7;//当累计断油圈数超过该值时，才需要CI修正?	
const byte M0_INJ_AG_INC =             75;//CI和刚离开怠速时，加喷圈增油量的基础值，量纲4US;equ	75	;the first inj after stpoil inc oil by75*4us
//const byte M0_ACCELDT_MAX=	           150;//过渡油量修正时，v_Ka_b1所乘的增油系数总和的最大值；	
const byte M0_ACCELDT_MAX=	           250;//过渡油量修正时，v_Ka_b1所乘的增油系数总和的最大值；	
const byte M0_MAX_RPM_AQ=  118;//怠速自动搜索功能的转速上限：量纲15.6RPM；转速为1843
const word M0_TIME_TO_CDI_H = 0x0032 ;      //CDI放电之后，到下次充电之间的时间;;0.8MS，单位:4US
//const word M0_TIME_TO_CDI_H = 500 ;      //CDI放电之后，到下次充电之间的时间;;0.8MS，单位:4US
const byte  M0_MAX_RPM_CONIDLE= 116;         //怠速控制的最高转速，单位15.6RPM// equ	106	;when the average of rpm156 over 16 runs>=106,clear conti_idle_time
const byte  M0_MIN_RPM_CONIDLE=	77;          //怠速控制的最低转速，单位15.6RPM//equ	77	;1203 rpm 2004.10.28;82 ;03.9.1 ;77	;03.8.22;71	;when the v_rev_156ave<which,1109rpm, clear conti_idle_time 
const byte  M0_NBTDCO_USED_IGN=	50;          //当点火磁电机角度小于该值，转速高于PCAH2929_IGN时，一检测到角标就点火，单位0.7度CA?

const byte  M0_RZT_HOT_NOOIL=		72;  //	equ		72		;当RZT>=72,作为热车,怠速断油转速不再减小，量纲：与温度采样值一致
const byte  M0_RPM_DEC_REOIL	=	20 ; //	equ		20		;怠速再供油转速的减小量为(20-(72-v_Teng_inuse)/4),热车后为<2250，量纲：与温度采样值一致
//const byte  M0_RPM_NOOIL_DEC_TOILDE= 18; //	equ		18		;TOIDLE时的断油转速减小量(18-(72-v_Teng_inuse)/4), 热车后断油为>=2375，量纲：与温度采样值一致
const byte  M0_RPM_NOOIL_DEC_TOILDE= 8; //	equ		18		;TOIDLE时的断油转速减小量(18-(72-v_Teng_inuse)/4), 热车后断油为>=2375，量纲：与温度采样值一致
                  //2013-1-12 FROM 18 TO 8
//const byte  M0_IDLE_STOP_SP= 0x75  ;     //非STARTS阶段的最低供油转速	EQU	075H	//;	when	v_prd_cas_h>=75h:rpm<500	not	inj	when	if	motor	started
//const byte  M0_IDLE_STOP_SP= 0xFE  ;     //非STARTS阶段的最低供油转速	EQU	075H	//;	when	v_prd_cas_h>=75h:rpm<500	not	inj	when	if	motor	started
                               // 2012-5-29

//2015-12-6 把M0_IDLE_STOP_SP改回0x75 ，因为原来会导致通机停不了机
const byte  M0_IDLE_STOP_SP= 0x75  ;


//const byte  M0_START_SPEED  =  0xD1;     //STARTS阶段的最低供油转速
const byte  M0_START_SPEED  =  0xFE;     //STARTS阶段的最低供油转速
const byte  M0_MAX_START_RPM = 0x06;     //判断启动角标时，可能达到的最高启动转速?
const byte  M0_JQM_DRY_IGTOR= 15 ;       //吹火花塞策略对应的最低TPS级别，即发动机NOT MMOVE时，判断油门级别大于等于该值时，不喷油EQU	15	;WHEN	JQM>=15,	FOR	DRY	INGITOR

#ifdef TE_10K						//20210720 chenwei add 

const byte  M0_TES_MAX=	247     ;  //2019-9-26 缸体上需要提高 235      //当缸头温度传感器采样值大于该值，认为传感器有故障，并且停油//EQU	235	;WHEN	V_TENG_INUSE>M0_TES_MAX,	STOP	OIL	NOT	CONSIDER	THE	MOTOR	STATUS				
#endif

#ifdef TE_2K
const byte  M0_TES_MAX=	253  	;	//20210720 chenwei add 2K水温传感器限值设为253

#endif


const byte  M0_TES_FOR_IDLENOTTOOLONG=	210;         //当缸头温度传感器采样值大于该值，连续怠速时间超过一定值之后，停油//EQU	200	;2004.6.22,V_TENG_INUSE>=200	AND	RUNNING	IN	IDLE	FOR	TWO	MINITE	STOP	INJ			
                                        //2008-11-7 CHANGE FROM 200 TO 210
//2008-11-2 move here
const byte  M0_MIN_RZT_TI_SH=	50;//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND=0,	NO	TI_SHIFT	GET	
const byte  M0_MAX_RZT_TI_SH=	192;//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND=0,	NO	TI_SHIFT	GET	
//－－for tmp
const byte  M0_TAS_MAX=	            90;//空气温度传感器采样值的最高域值；大于该值判断为有故障；EQU	90	;WHEN	AIR	TEMPERATURE	MEASURED	>63DEGREE,	SNSOR	ERRO		
const byte  M0_TAS_MIN=	            2;//空气温度传感器采样值的最低域值；小于该值判断为有故障；//EQU	2		
const byte  M0_TES_MIN=	            1;//暖车状态下，缸头温度传感器的最小域值，小于该值判断为有故障；/EQU	1	;-30DC,IF	V_TENG_INUSE<M0_TES_MIN	OR	V_TENG_INUSE>RZT_MAX	FOR	15	TIMES,		
const byte  M0_TAS_MAX_INUSE	=       60;//空气温度传感器采样值的最高域值；使用时的最大值；若高于该值，按该值使用；EQU	60	;2005.6.1	THE	MAX	OF	V_TAIR_INUSE	IS	45DEGREE	FOR	V_TAIR_AD	SAMPLING		
const byte  M0_TAS_MAX_PREDICTED	=   50;//空气温度传感器预测值值的最高域值；EQU	50	;2004.9.20,37	DEGREE	;45	;2004.8.29	34	DEGREE	;60	47	DEGREE		
const byte  MO_TES_LLIMITFORPUMP;  //130   //当TES高于该温度值，且发动机运行时间很短，即启动过程没结束就停车时，就需要泵油 ；     

const byte M0_PCAH_DOWNRPM   =   29;  //;2007-4-9 when rpm<2020,down the rpm to pc by some value  ,量纲同PCAH
const byte  M0_TENG_LT_GUOSAN       =    25;//国三测试时的温度最低限制

/*----------以下是启动策略-----------------------------------------*/
//量纲与DTFI一致,是Q*250/11;


const word M0_FMF_PREINJ=128;   //单次预喷油量 :						5.632	


const word M0_FMF_1ST_BASIC=128;//启动第一圈的基本油量	
const byte M0_TES_FOR_1STRUN_FMF_CORRECTION=60;////27,当启动时,温度低于该值时,需要额外增油,额外增油量为M0_TES_FOR_1STRUN_FMF_CORRECTION-teng	
                                              //2009-12-10 change from 27 to 60
/*发动机静止时的一些策略*/
const word  M0_T_DELAYED_AFTER_KEYOFF_S=500  ;  //ECU在钥匙关闭后的M0_T_DELAYED_AFTER_KEYOFF_S秒时,完全关闭电源
                                      //201282 CHANGE FROM 3600 TO 500
const word  M0_T_INTERVAL_COM_WHENSTOP_4US= 5000 ; //单位:4us,发动机未启动时,通讯的速度;即两个桢之间的时间间隔;
const word  M0_T_KEYOFF_SHORTLY_S= 1800   ;       //单位:s,钥匙关闭时间小于该值时,置 VF_OFFSHORT_KEY位

/*--os闭环控制策略-----*/
 const byte MO_STEP_FMF_IDLELEAN= 1    ;     //怠速反馈，持续稀时，定期的增油量，单位：同FMF	
 //const byte MO_STEP_FMF_IDLE_RICHTOLEAN= 4 ;        //怠速反馈，由浓变稀，需要增加的油量，单位：同FMF 
 //2011-3-17 4 to 6
 
 
 const byte MO_FMF_RICHLIMIT_WOT_NOTFK=	4 ;      //油门全开，开环工作时，允许的最大的减油量的相反数；这个数要求是〉=0
 //const byte MO_FMF_CLS_INIT=5   ;       //启动后，怠速反馈油量的初始值
 //const byte MO_FMF_CLS_INIT=0   ;       //启动后，怠速反馈油量的初始值
 //const byte MO_FMF_CLS_INIT=5   ;       //启动后，怠速反馈油量的初始值
 const byte MO_FMF_CLS_INIT=0   ;       //启动后，怠速反馈油量的初始值 //2015-12-15 FROM 5 TO 0
      //2013-3-1 FROM 0 TO 5
      //2013-9-16 from 5 to 0
      //2014-1-11 FROM 0 TO 5
 const byte MO_FMFENRICH_BACKTOIDLE_CLS    = 2   ;    //2008-11-13 ADD 
 const byte MO_DFMF_LEANTOOLONG_WHENIDLE= 25;//当在1s内，怠速反馈油量比之前的平均值高出该值，并且OS一致指示偏稀，则认为OS有问题； 
 const byte MO_DFMF_LIMIT_LEANTOOLONG_WHENIDLE= 4  ; //2008-11-14 当出现增长过快时，对怠速增反馈值的限制值
 const byte MO_SUBDFMF_LIMIT_LEANTOOLONG_WHENIDLE= 10 ;  //2008-11-14 当出现增长过快时，对怠速减反馈值的限制值

 const byte M0_CCL_ENRICH_CNTLEAN_IDLE =      16;//;怠速连续稀,增油的正常圈数,连续?6圈,增一次油
 const byte M0_CCL_ENRICH_CNTLEAN_IDLE_HSP=        24;//;怠速转速高时连续稀,需增油的圈数
 const byte M0_CCL_ENRICH_CNTLEAN_IDLE_CI=         16;//;怠速时,再供油结束前,连续稀,需要增油的圈数,
 const byte MO_STEP_FMF_NOTIDLE    = 2 ;          //非加速期间,非怠速反馈时，稀浓变化时，油量的跳变值；
                                                //2009-9-8 change from 4 to 3;
 const byte MO_STEP_FMF_ACCEL  =  2;//5    ;       //加速期间，稀浓变化时，油量的跳变值；
 const byte MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST =  6 ;  //当氧传感器出现误判为稀的错误时，需要氧传感器再波动这个次数后恢复反馈
                                                //当nb_o2change_afteridletoofast〉>MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST之后恢复正常
 const byte MO_DFMF_LIMIT_WHENWOT_CHANGELITTLE  = 20 ;  //VF_OSCHANGELITTLE_CLS=TRUE且油门全开时的最大限制值;//2008-11-17
 const sbyte MO_DFMF_LLIMIT_WHEN_ACCEL=-15; //加速时，对减反馈量的限制；
/*--非喷油圈的策略------*/
const byte M0_TES_ADDINJ_ENRICH=50; //加喷圈油量的温度修正，即当温度低于该值时，进行增油，增油量为： M0_TES_ADDINJ_ENRICH-TES，单位：同FMF
const byte M0_INJPHASE_FORIDLE=8;         //2008-12-3   //怠速喷射相位
const byte M0_NB_ADDINJ_JCI=1;       //刚恢复供油时，加喷的次数；

/*------ISCV策略-------*/
const byte  M0_TENG_L_ISCV =27 ;    //2008-11-4   ISCV延时到暖车结束的上限温度
const byte  M0_D1S_ISCVON_BASE= 50 ;//2008-11-4   ISCV开启时间的基础值
const byte  MO_TENG_U_ISCV= 33  ;   //2008-11-4   计算ISCV延时的上限温度
const byte M0_TES_U_FOR_CI_ISCVON=110;//当温度低于该值时，回油门时需要打开怠速提高阀

/*-----怠速策略-------*/
const byte M0_TPS_IDLE_L=5;// 7	//怠速的最低油门级别，对应油门级别为："0.NM",其中，N=取整（M0_TPS_IDLE_L/4），M=M0_TPS_IDLE_L MOD 4

/*--怠速系统初始化策略-----------*/
const byte M0_T_KEYON_FOR_INI_S=1;     //    3 //系统需要进入初始化状态，对应的每次钥匙开启后的最长时间限制；
//const byte M0_T_KEYOFF_FOR_INI_S=1;    //  3  //系统需要进入初始化状态，对应的每次钥匙关闭后的最长时间限制； 
const byte M0_T_KEYOFF_FOR_INI_S=2;    //  3  //系统需要进入初始化状态，对应的每次钥匙关闭后的最长时间限制；   //2016-7-30

const byte M0_NB_KEYONOFF_FOR_INI=8;   //系统需要进入初始化状态，对应的钥匙反复开关的次数 
const byte M0_TES_ULIMIT_FOR_INI=180;//初始化时，缸头温度最高上限；
const byte M0_REV15D6_LLIMIT_FOR_FULLAQ=94;//粗扫时，若最高转速高于该值1480rpm，则不判断气量

