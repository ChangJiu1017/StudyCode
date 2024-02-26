/*HEAD
 ***********************************************************************
 * Filename	:inj_ctl.c
 * Function	:Injection control		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V9.2.1
 *
 *** History ***
 * V9.1.0   2017.2.27	xielian 
 *		Base module for M9 injection control.
 * 
 * V9.2.0   2017.2.27	xielian
 *		vc_tw3_t35默认值更改为M0_TV3_T13D5MS_INT，M0_TV3_T13D5MS_SET和M0_TV3_T1EQ3D5_0为标准值
 *
 * V9.2.1   2017.3.23	xielian
 *		解决油量超大时T3不反馈问题
 *
 * V9.2.2   2017.3.28	xielian
 *		1.8bitMCU上用vc_tw1_t3FK_I复用，在停机时作为v_dt1，在看参数时容易混淆，起始时没有影响，故重新单独定义v_dt1
 *		2.修改预喷参数计算，增加BYPTR_M0_NB_INJDRV作为可标定项
 *
 * V9.2.3   2017.4.18	xielian
 *		1.启动时T3反馈比例项减弱
 *		2.过渡时T3反馈比例和积分项增强
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "inj_ctl.h"

#include "look_up.h"

#include "add.h"

#include "ee.h"
#include "fmf_odr.h"
#include "data_inj_source.H"
#include "sys_init.h"
//#include "events.h"  //2009-11-29   2014-10-14 delete
                                          
//#include "wait_moving.h"

//#include "sci.h"//2010-1-29 linshi

#include "tImE_ccl.h"

//2010-3-13 add 
#include "data_sys_standard.h"

#include "sys_definition.h"   //2014-11-25

//#include "status_eng.h"//2013-7-10

/*
    INJ_CTL.c更改说明
    
    T3反馈控制增加条件
    VF_T3BECHECK_INTINJ=




*/

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_INJCTL_STATUS;
volatile V_STATUS_STR _V_INJCTLH_STATUS;
volatile V_STATUS_STR _V_INJCTLHL_STATUS;
//#pragma DATA_SEG DEFAULT
byte vc_tw3_t35;        //T35修正系数	
                        //2008-3-25 将类型改为SBYTE
sword vc_tw1_t3FK_I;    //T3反馈值  注意与vc_tw1_t3fk的区别；T3反馈的积分项；
sword vc_tw1_t3FK_P;    //T3反馈的比例项；
sword v_dt1;			//2017-3-28 

//2008-7-21 以DT3为目标值得策略的参数；
word v_dtw3_obj;//dt3 的目标值，即T3减去基能后剩下的值；
word v_dtw3;            //计算得到的DTW3；
word v_dtw3_obj_lst;    //上一次

sbyte v_vbat_dt_TW3FK;  //TW3反馈后，对VBAT_DT的修正
sword v_T3b;             //2008-11-27 add for disp

//word t3b1;              //2009-1-16 add for T3B comparision
sbyte v_ccl_fordelay;   //用于推迟T3反馈的圈数
sword vc_tw1_t3fk_now;          //save the ti_offset at	idle set by	user，偏移量2当前值

word v_fmf_max;//2012-10-9
byte v_sta_T3_LT;//T3反馈的状态，T3小于目标时，位1


byte v_nb_T30;//2015-8-28 add 

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//#define DELAY_APUM_MS  0x1C00       //发动机静止状态下，喷油器驱动时，两次喷油之间的时间间隔，单位:4US //2009-7-15 由1E00改为1C00
//#define DELAY_APUM_MS  5500       //发动机静止状态下，喷油器驱动时，两次喷油之间的时间间隔，单位:4US //2009-7-15 由1E00改为1C00
//                            //2009-12-1 将扫描t4时两次脉冲之间的时间间隔DELAY_APUM_MS更改为22ms ;
#define DELAY_APUM_MS  2500       
                        
#define INDEX_T2000 0 //T1=2000对应的T3在v_tw3_t1（M1_TW1_T1T3）中的索引
#define INDEX_T3000 1
#define INDEX_T3400 2//10//2 //T1=3400对应的T3在v_tw3_t1（M1_TW1_T1T3）中的索引
//2008-6-30
#define VBAT_ST     221 //2008-9-18 标准电压采样值的	
//2008-6-21 add
#define NO_T3TEST_3000  1
#define NO_T3TEST_2000  2
#define NO_T3TEST_3400  0// 其他值也算是3400的

#define T1_STARTPOINT   800
//-------------------t4扫描相关参数  09-7-15修改------------------------------
#define T30_3000 265 // 1.06   T30

#define NB_SCANFAIL_LIMIT 5// v_scaningfall>NB_SCANFAIL_LIMIT时停止扫描
//2008-12-3 add T4扫描变量       //2009-7-15修改
#define NB_MODE_ZERO 60  //模式0扫描最大T1点数
#define NB_MODE_ONE 30   //模式1扫描最大点数
#define NB_MODE_TWO 5    //模式2扫描最大点数
#define MODE_0_TOTAL 720// 960 //模式0扫描最大T1总点数  =NB_CT1*NB_MODE_ZERO
#define MODE_1_TOTAL 360//480 //模式1扫描最大T1总点数
#define MODE_2_TOTAL 60//80  //模式2扫描最大T1总点数
//#define TOTAL_NB_T1b 80  //5*16
#define NB_NOT4_LIMIT   5

byte v_nb_T1Scan;   //测试T1点数
sbyte v_nb_T1for1Point;    //16个点测试的完成度
byte v_Point_T4Dec;
byte v_Point_T4Asc;     //T4的第一个上升点
byte v_Flag_For_TestCha;   //特征点T4测试的标志
byte v_ChaPoint_pre;    //T4绝对上升之前的最近一个特征点；

byte nb_T4_exist;   //2009-11-29 add, t4存在的次数

#define NB_CT1 4//每一点T1,需要测试的次数；  //2009-12-1 change from 16 to 12
#define NB_CT1_DIV2 2
#define NB_CT1_M2    8
#define NB_CT1_CUT1 1//每一点T1,需要测试的次数；  //2009-12-1 change from 16 to 12
#define NB_CT1_REMAIN1 2//每一点T1,需要测试的次数；  //2009-12-1 change from 16 to 12

//判断扫描失败对应的点数
#define NB_FOR_SCANT3T1FAIL  40//扫描T1-T3  40*0.1  4ms
#define NB_FOR_SCANT4T1FAIL  55//        1.2MS    0.02*60

#define NB_LIMIE_T1T3FAIL   2
#define NB_LIMIE_T1T4FAIL   4

byte v_nb_ScanT3T1fail; //扫描T1T3失败的次数
byte v_nb_ScanT4T1fail; //扫描T1T4点数超差的次数

byte v_nb_AveScanT3Fail;
byte v_nb_AveScanT4Fail;
byte bInjFlag;

//2010-2-27 重新奠定一
word v_Tw4_ScanArray[NB_MODE_ZERO]; //测量T1-T3关系时，存储16个点的平均T4值
sbyte v_DT1ToDT3_ScanArray[NB_MODE_ZERO]; //测量T1-T3关系时，存储16个点的平均T4值
//word v_Tw1_ScanArray[NB_MODE_ZERO]; //测量T1-T3关系时，存储16个点的平均T1值
word v_Tw4_CurTw1[NB_CT1_M2]; //每个T1测试16个次的数据存放
word v_Tw3_CurTw1[NB_CT1]; //每个T1测试16个次的数据存放
word v_AveResult;

word v_T3_scan_lst;

//2010-3-17 add
byte nb_Piston_bubble;    //电枢进气泡的次数；
byte nb_Retest_forBubble; //因气泡过多导致

byte v_Status_T4Dec; //bitx: 1:下降， 0：未下降；
byte v_Status_T4Inc; //bitx: 1:上升， 0：未上升；
byte v_Status_NotDecMore1;//下降不超过1个单位
//byte v_Status

#define STEP_T1SCAN_FORT4   5
#define STEP_T1SCAN_FORT3   32

#define STEP_2POINT_FORT4   10

sbyte v_DT1ToDT3_Cur;
word v_T4_ave;//2010-1-14
    
#define T3D_MAX  375
#define T3D_MIN  300
#define T3D_MINMIN 275//比
#define T3D_MINMAX 300//比
word v_T1i;//初始化的起始t1
word v_T1i_lst;//初始化的起始t1
word v_T1Scan;//当前扫描用的t1;
byte v_nb_T4Exist;//T4存在的次数
word v_T1Scan_ini_byVbat;//当前扫描用的t1;

word v_T3d_ini;//t3d对应的原始的t3信号；
byte v_nb_T1di;//扫到的T1Di的次数；

word v_T4_inInjctl;//2010-3-6 tmp
//扫描时，第二个T4的信息
word v_T42nd_max;//抛掉不合理的T4点之后，合理的第二个T4的平均值；
word v_T42nd_ave;
word v_T42nd_min;
word v_T42nd_min2; //倒数第二最小的T4值；
word v_T42nd_min3; //倒数第san最小的T4值；
word v_T4_max;
word v_T4_min;

word v_T1d;             //2009-1-16 最终得到的T1D
word v_T1d_prepoint;    //2010-3-9 对应于v_Point_T4Asc的值；
sword v_T3d;   //2010-1-11
//sword v_Gama_lst;   //2010-1-11
byte v_nb_T3d ;//测量t3d的次数
byte v_Vbat_for_gama;//2010-1-11
sword v_gama_new;//2010-1-11
sword v_gama_delta_T1d;//2010-1-11

word v_T1d0;
word v_T3b0_T1d0;

word v_T1d_Tmp;
//初始化后的电压特性修正参数
sword v_gama0;
sword v_gama_tmp;
sword v_gama_inuse;
sbyte v_gama_delta_div32;//本次被认为可靠的（v_gama_new-上一次v_gama_inuse）/32 之后与DT3的量纲一致；；


#define GAMA_DEC_P10    -819//GAMA减小0.1 
//#define GAMA_DEC_P10    -1600//GAMA减小0.2 
#define GAMA_DEC_P06    -491//GAMA减小0.06 
#define GAMA_DEC_P03    -245
#define GAMA_INC_P03    245

#define GAMGA_MIN       -3000
#define GAMGA_MAX       3000

//word v_nb_ScanPointadded;//同一个T1增加的测试点数；

word nb_inj_lst;//byte nb_inj_lst=v_NB_INJDRV  //2010-1-13 add 

#define ADR_GAMA_0      0
#define ADR_GAMA_INUSE  1
#define ADR_GAMA_TMP      2
#define ADR_GAMA_DELTA      3

//2013-3-23 PUMP
byte v_fmf_pump;

   
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte no_t3_fk_run;//	equ	0x0e	;无T3反馈的次数；在某些条件下，需要终止T3反馈，no_t3_fk_run的值即是终止的次数；	
sword sum_tv3_temp_4t;//
sword tv3_t13d5;//
sword av_dti_t3fk;//	equ	0x1e5	;运转中怠速偏移量2的平均值,回怠速时，需要将T3反馈值赋予该值。	
sword sum_dtinj;
//sbyte kDT3_to_T1;       //由DT3反馈，得到的T1的差异；

/*--For dt3 inition-----*/
word v_tw3_t3000;  //2010-2-27 需要赋予初始值 

///2009-1-14 deleted ; for insert lqj logic
byte NO_T3test; //2008-6-20 add
                // NO_T3test=0: T1=3000
                //          =1; T1=2000
                //          =2; T1=3400

//2009-11-27 add
byte v_Vbat_whenT4_max;
byte v_Vbat_whenT4_min;
byte nb_VbatFlu;        //t4扫描过程中,2009-11-28 连续电压波动的次数;
#define VBAT_SMALLFLU_LIMIT_WHENT4    5   //if(v_Vbat_whenT4_max-v_Vbat_whenT4_min)>VBAT_SMALLFLU_LIMIT_WHENT4,退出扫描；
//#define VBAT_GREATFLU_LIMIT_WHENT4    10   //if(v_Vbat_whenT4_max-v_Vbat_whenT4_min)>VBAT_SMALLFLU_LIMIT_WHENT4,退出扫描；
#define VBAT_GREATFLU_LIMIT_WHENT4    20   //2016-6-29 FROM 10 TO 20

#define PNINI_T4_FULLSCAN   0
#define PNINI_T4_FINESCAN   1
#define PNINI_T3_T3B        2
#define NB_VBATFLU_LIMIT    5   //电压波动的次数
#define T4_MAX              4500

#define DIF_FOR_NEIGHBORT4  60//0.24
//byte status_T4Scan;  //T4扫描的状态,即当前的位置;
#define DIF_FOR_2STEPT4     100
#define DIF_FOR_3STEPT4     150
//#define DIF_FOR_1POINT_TESTMORE  12
#define DIF_FOR_1POINT_TESTMORE  10//2010-5-4 chang to 10
#define DIF_FOR_1POINT_AVE  40

/*--For dt3 correction:T35 */
byte T30_beforeCoVat;   //未被修正的减去基础值的T30；
byte T30_beforeCoVat_lst;   //未被修正的减去基础值的T30；
byte vbat_whenT30;      //T30测试时的电压
byte vbat_whenT30_lst;      //T30测试时的电压

//2009-4-18 
//临时测试
byte nb_T35T;
byte nb_vbat_not_predictbytet30;//2009-12-10  t30没有被预测的次数
//2009-12-11 add
byte nb_t3_out;//T3超范围的次数；
byte nb_t3fklimit;  //t3 fankui chaocha
#define NB_T3OUT_LIMIT 20     //nb_t3_out  NB_T3OUT_LIMIT
#define NB_T3FKLLIMIT    100

//2010-1-29 add 
//byte nb_DT3_LTtarget;//DT3连续大于目标值的次数
//byte nb_DT3_GTtarget;//DT3连续小于目标直的次数；
sbyte v_dDt3;//2010-1-29

byte nb_T3Fk;   //启动后t3反馈的次数
byte nb_T3Bubble;//2010-5-22


//实际DOP参数
sword v_mrealDomA;       //a*-1
sword v_mrealDomB;
sword v_mrealDomC;
sword v_mrealVRate; //采样电压和实际电压比率

//v_DTC_T4ScanFail,电压故障
#define DTC_T4_VBAT 1//电压异常
#define DTC_T4_T3   2//T3异常
#define DTC_T4_T4   4//T4异常
#define DTC_T4_DGAMA 8//两次的GAMA差异大
#define DTC_T4_GAMA 16//GAMA超范围
#define DTC_T4_NO_CHA 32//特征点异常

byte v_DTC_T4ScanFail;//2015-12-17 t4扫描错误的故障指示
byte v_nb_pump_limit;//
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Ave_C_TW3_FK(byte tps_odr,byte rev_156ave);
//void check_t3ers(word tw3,word vbat);
void  Cal_ABC(byte vbat);//,word t30,word t3d);//2010-3-2 
word Ctl_DTW3_FK(word tw1n,sbyte vbat_dt,word tw3,byte tps_odr,byte tps_mode,byte rev_156ave,byte teng,word tw1p,byte vbat);
word Set_TW1_byDT3mode(sword fmf,byte vbat);       //2008-7-25 add
sword Cal_T3b(byte vbat,word t1p,byte blnGamaE0);//,byte blnDt3b);//2009-4-14 add 
void Init_NextScanPoint(void);
void Init_BeforeScan(void) ;
byte GetAve(word *Array,byte Num,word Max,word Min,word Scatter);  //平均值存储在v_AveResult中
void Save_Gama(byte AdrGama);
void GetRbyT1d(byte vbat);
byte GetT4Scatter(byte blnReset,word CurVal,word tw42nd);
byte CheckDT1toDT(word tx_cur,word tx_lst,byte index);
void SetFlagAterScanT4(byte vbat);
word Cal_T1d0(byte vbat) ;//2010-3-22 add 

void FD_pump(byte vbat,word tw3,word tw1p);

void FC_DT3(byte vbat,word tw1p,word tw3);

// 载入DOP、电压、和DT3修正数据，并校验
void load_check_memorydate(void);
byte load_check_depart(sword n1,sword n2,sword n3);

//共轨
//byte v_fmftot1a;




/*-------------给程序员的说明------------------*/
//VF_T1SCANINGBREAK_INJCTL：表示是否中止T1扫描；

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
//功能：复位后喷油参数的初始化
void Init_InjPara_afterReset(void)//后面需要跟上 Check_TW1SCI_afterRDorWR；
{
    
    INJ_OFF;//2008-12-16 move here
    //----------Init_T3FKPara_AfterReset;---------------------------
  	sum_dtinj=0;    //	clrf	sum_dtinjh		;2007-3-30,banked
  	av_dti_t3fk=0;  //	clrf	av_dti_t3fk		;2007-3-30,banked
    vc_tw1_t3FK_I=0;  //	clrf	vc_tw1_t3fk_PID_h		;2007-3-30,banked
  	vc_tw1_t3FK_P=0;
    v_vbat_dt_TW3FK=0;//2013-4-23 ADD 
    
    if(VF_VPOR_HARD||vc_tw3_t35==0)//2008-4-21 暂时拿掉
    //2008-5-4 发现去掉这里的VF_POR_MCU条件之后有问题
    {
        //------------Init_T3Para_AfterPONreset:---------------------------
        vc_tw3_t35=(byte)M0_TV3_T13D5MS_INT;//2017-2-23 更改T30初始值//2008-5-20 to byte //movlw	tv3_t13d5ms_set						;2005.3.24movwf	vc_tw3_t35	
        sum_tv3_temp_4t=(sword)vc_tw3_t35*8;  //2008-10-6

        NO_T3test=NO_T3TEST_3400;//2008-6-20 zhangping add

        T30_beforeCoVat=(byte)M0_TV3_T13D5MS_SET;//2008-8-5 
        T30_beforeCoVat_lst=T30_beforeCoVat;	
        vbat_whenT30=M0_VBAT_ST;//2008-8-5 ADD
        vbat_whenT30_lst=vbat_whenT30;
        v_tw3_t3000=M0_TV3_T1EQ3D5_0+(word)M0_TV3_T13D5MS_SET; //2010-2-27
    }
    
    
    if(VF_VPOR_HARD)//2010-3-6 add VF_TURNONKEY_HARD
    {
        VF_T1SCANING_INJCTL=false;//2009-3-3 add 
        nb_t3_out=0;
        nb_t3fklimit=0;
        V_INJCTL_STATUS=false;//  2009-12-18 move here
        V_INJCTLHL_STATUS=false;//2010-2-27

        VF_PUMP1ST_INJCTL=false;//2015-12-17
        v_fmf_pump=M0_FMF_PUMP;
        //2014-10-7 move from sys_init.c
        #ifndef SYS_TRADITIONAL
			load_check_memorydate();//
		#endif

        v_nb_T30=0;
    }
    else
    {
        V_INJCTLHL_STATUS&=0x80;//2010-2-27
                //保留最高位
                //b7 是VF_NEEDSAVEGAMA_SYSINI
        
    }
    //V_INJCTL_STATUS=false;  //2008-12-15
    V_INJCTLH_STATUS=false; //2009-3-4
    
    //==============复位后喷油器动作的设置：2008-11-2
    VF_T1SCANINGBREAK_INJCTL=false;//2009-1-19 根据小乐的建议添加；
    VF_PUMP_INJCTL=false;//2013-3-23

    //2008-11-2 add
    //当系统进入初始化状态时，需要进行一些初始化：
    //1 不进行预喷，泵油等；
    //2 将O值和R值初始化为0；（不把EE中数据清除，若中途退出初始化，这保留原值）

    /*-----显示---------------*/
    //编成人员可以将需要计算机显示的数据放置在以下位置：
    //当处于初始化模式下：
    //T4:DTFI
    
  	v_T1d=0;
    
    nb_vbat_not_predictbytet30=0;
    nb_VbatFlu=0;

    v_fmf_max=M0_QMAX;//2012-10-9
    
    v_sta_T3_LT=0;
}

//功能：STARTS前喷油参数的初始化
void Init_InjPara_beforeStarts(void)
{
  	no_t3_fk_run=0;//	clrf	no_t3_fk_run			;2007-4-4
  	VF_T3ENABLE_INJCTL=true;//2008-7-25 enable;
  	VF_AT3GETAFTERSTART_INTINJ=false;
  	VF_T1SCANING_INJCTL=false;  //2008-11-16
  	vc_tw1_t3FK_I=0;//2008-6-10 ZHANGPING ADD
  	vc_tw1_t3FK_P=0;//2008-6-10 ZHANGPING ADD;
  	nb_T3Fk=0;  //2010-5-22
    nb_T3Bubble=0;
    v_ccl_fordelay=0;
    VF_PUMP_INJCTL=false;//2013-4-22 add to starts
    	
	return;
}





//功能：获得T1-T3测试的总次数；
word Get_NB_T1SCANING(void)
{
    return 5000;
}



//功能：
//      获得当前扫描用的TW1，单位4us
word Get_TW1_for_Scaning(void)
{
    //2009-1-14 from lqj;

    return v_T1Scan;
   
}

//功能：设定T1扫描的频率；单位4us//频率设定
word Interval_For_T1_Scaning(void) //范围：1-0xffff之间
{
   return DELAY_APUM_MS; 
   // if(VF_T1SCANING_INJCTL)return DELAY_APUM_MS;    
   // else return DELAY_APUM_MS/4; //2010-1-31   
}
//*******SET_T1_AS_T35GET***************************
//功能：将用于T35测试的T1载入TINJH,L
//返回值：T35测试时使用的T1;
word Set_TW1_forT35Get(void)
{
    //2008-9-18 将T35测试更改会T1=3000
    NO_T3test=NO_T3TEST_3000;
    //return M0_TW1_CO_T35/4;    // T1L_MTV3_GET    
    return M0_T1_T35;    // T1L_MTV3_GET      //2010-2-25 2014-10-16 from 750 to M0_T1_T35
}

//功能：发动机静止时，设置T35测试的次数；
//返回值：T35测试的次数
byte Set_NB_forT35Get(void)
{
    //return (5);//
    return (6);// 2008-6-20 changge to 6
}

//***tnooil_to_ti*****SET_T1_FOR_PUMP******************
//功能：载入泵油T1;
//  将泵油T1载入TINJH,L:no oil T1=inj_shift+t3fk0+modi_dcdy
//形参： vbat_odr－－电池电压级别；vbat_mode－电池电压级别余?
//返回值：泵油T1
//2014-4-5 add v_T1SCAN
word Set_TW1_forPump(byte vbat)
{
    sword sw;
    
    //2016-1-11 增加中间变量，防止T1超大
    //sw=Set_TW1_byDT3mode(-10,vbat);
    sw=Set_TW1_byDT3mode(10,vbat); //2016-1-11注意流量不能是负数
    
    if(sw<100)sw=100;
    if(sw>2000)sw=2000;
    
    v_T1Scan=sw;
    
    return v_T1Scan;  
   //return 1;                        
}

//*******SET_TIMES_FOR_PUMP*****************************
//功能：设置泵油的次数；
//返回值：泵油的次数
byte Set_NB_forPump(void)
{   /*
    if(VF_RUNTOOS_ENG)return 40;
    else
    {
        return 30;
    } */
    return 40;//2010-2-25 chang for cut down code
} 
 



//初始化下一个扫描点
void Init_NextScanPoint(void)
{
    v_nb_T1for1Point=0;
    GetT4Scatter(true,0,0);
    //v_nb_ScanPointadded=0;
    VF_T4SCATTER_INJCTL=false;
   // nb_Piston_bubble=0;//2010-3-20
    nb_Retest_forBubble=0;
   
}

//开始T1-T3或T1-T4一条曲线的扫描时的初始化
void Init_BeforeScan(void)
{
    v_Point_T4Dec=0;
    v_Point_T4Asc=0; 
    v_Flag_For_TestCha=0;
    v_nb_T1Scan=0; 
    v_nb_T4Exist=0; 
    VF_T4LST_RELIABLE_INJCTL=false; //T4有瑕疵 
    bInjFlag=0;
    v_Status_T4Dec=0;
    v_Status_T4Inc=0;
    v_ChaPoint_pre=0;
    nb_Piston_bubble=0;
    
}

byte GetT4Scatter(byte blnReset,word CurVal,word T42nd)
{
    sword Sc;
    
    if(blnReset)v_T42nd_max=v_T42nd_min=v_T42nd_min2=v_T42nd_min3=v_T4_max=v_T4_min=v_T42nd_ave=0;
    else
    {    
        if(CurVal>0&&CurVal<4000)//16ms
        {
            if(v_T4_max==0)
            {
                v_T4_max=v_T4_min=CurVal;    
            }
            else 
            {
                if(v_T4_min>CurVal)
                {
                    v_T4_min=CurVal;    
                }
                else if(v_T4_max<CurVal)
                {
                    v_T4_max=CurVal;    
                }
            }
        }
        /*  2010-3-25 delete      
        if(T42nd>0&&T42nd<4000)//16ms
        {
            if(VF_T4LST_RELIABLE_INJCTL&&v_nb_T1Scan>0&&v_Tw4_ScanArray[v_nb_T1Scan-1]>0)
            {
                Sc=v_Tw4_ScanArray[v_nb_T1Scan-1]-T42nd;
                if(Sc>-20&&T42nd<20)
                {
                    if(v_T42nd_ave==0)v_T42nd_ave=T42nd;
                    else (v_T42nd_ave=v_T42nd_ave+T42nd)/2;
                }
            }
            else 
            {
                if(VF_T4LST_RELIABLE_INJCTL)
                {
                    if(v_T42nd_ave>T42nd)v_T42nd_ave=T42nd;    
                }
            }
        }*/        
    }
    Sc=v_T4_max-v_T4_min;
    if(Sc>120)Sc=120;
    return (byte)Sc;
}


byte CheckDT1toDT(word tx_cur,word tx_lst,byte index)
{
    sword sw;
    byte bln;
    byte nb_Lleap;//大步跳跃
    byte nb_Sleap;//小步跳跃
    
    bln=false;
    /*
    if(tx_lst+1<tx_cur)
    {
       // sw=((t1_cur-t1_lst)*4)/(tx_cur-tx_lst);
        
    } else
    {
        sw=0;    
    } */
    //if(blnT4)bM=1;
    //else bM=64; 
    
    sw=((tx_cur-(sword)tx_lst)*2);//(t1_cur-(sword)t1_lst);
    
    //STEP_T1SCAN_FORT4
    
    if(sw>60)sw=60;
    else if(sw<-60)sw=-60;
    
    v_DT1ToDT3_ScanArray[index]=sw;
    v_DT1ToDT3_ScanArray[index-1]=v_DT1ToDT3_ScanArray[index]+v_DT1ToDT3_ScanArray[index-1];    
    v_DT1ToDT3_Cur=v_DT1ToDT3_ScanArray[index-1] ;
    if(index>=5)
    {
        nb_Lleap=0;
        nb_Sleap=0;
/* 
         if(v_DT1ToDT3_ScanArray[index-5]+6<=v_DT1ToDT3_ScanArray[index-3])nb_Lleap++;
        if(v_DT1ToDT3_ScanArray[index-4]+6<=v_DT1ToDT3_ScanArray[index-2])nb_Lleap++;
        if(v_DT1ToDT3_ScanArray[index-3]+6<=v_DT1ToDT3_ScanArray[index-1])nb_Lleap++;
        if(v_DT1ToDT3_ScanArray[index-5]+2<=v_DT1ToDT3_ScanArray[index-4])nb_Sleap++;
        if(v_DT1ToDT3_ScanArray[index-4]+2<=v_DT1ToDT3_ScanArray[index-3])nb_Sleap++;
        if(v_DT1ToDT3_ScanArray[index-3]+2<=v_DT1ToDT3_ScanArray[index-2])nb_Sleap++;
        if(v_DT1ToDT3_ScanArray[index-2]+2<=v_DT1ToDT3_ScanArray[index-1])nb_Sleap++;
*/    
          
        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-3],v_DT1ToDT3_ScanArray[index-5],5)==CMP_RESULT_GT)nb_Lleap++;

        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-2],v_DT1ToDT3_ScanArray[index-4],5)==CMP_RESULT_GT)nb_Lleap++;
        
        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-1],v_DT1ToDT3_ScanArray[index-3],5)==CMP_RESULT_GT)nb_Lleap++;

        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-4],v_DT1ToDT3_ScanArray[index-5],1)==CMP_RESULT_GT)nb_Sleap++;
        
        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-3],v_DT1ToDT3_ScanArray[index-4],1)==CMP_RESULT_GT)nb_Sleap++;
        
        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-2],v_DT1ToDT3_ScanArray[index-3],1)==CMP_RESULT_GT)nb_Sleap++;
        
        if(CMP_Sword(v_DT1ToDT3_ScanArray[index-1],v_DT1ToDT3_ScanArray[index-2],1)==CMP_RESULT_GT)nb_Sleap++;
  
         //2010-3-8 小乐发现一例T1-T3扫描出油点太晚的，将10--6，3--2
        if(nb_Lleap>1&&nb_Sleap>1)
        {
            bln=true;    
        }
    }
    return bln;
}
//GetAve
//返回：是否有平均值的标志；
byte GetAve(word *Array,byte Num,word Max,word Min,word Scatter)
{
    byte i;
    byte n1;
    word Sum;
    sword sw;
    word A_min;
    word A_max;
    byte halfNum;
    byte j;
    word tmpResult;
    
    Sum=0;
    n1=0;
    halfNum=Num/2;
    
    for(i=0;i<Num;i++)
    {
        if((Array[i]>Min)&&(Array[i]<Max))
        {
            n1++;
            Sum+=Array[i];    
        }
    }
    tmpResult=0;
    v_AveResult=0;
    if(n1>0)
    {
        Sum=Sum/n1;
        
        for(j=0;j<=2;j++)
        {
            //对于测量，程序到此也没有关系
            //j==0时，SUM为平均值；
            if(j==1)Sum=v_T4_max;
            else if(j==2)Sum=v_T4_min;
            A_max=Scatter+Sum;
            sw=(sword)Sum-Scatter;
            if(sw<0)sw=0;
            A_min=sw;
            
            //add    ,取范围窄的；
            if(A_min<Min)A_min=Min;
            if(A_max>Max)A_max=Max;
            
            n1=0;
            Sum=0;
            for(i=0;i<Num;i++)
            {
                if(Array[i]>=A_min&&Array[i]<=A_max)
                {
                    n1++;
                    Sum+=Array[i];    
                }
            }
            if(n1>0)
            {
                tmpResult=Sum/n1;
            }
            if(n1>=halfNum)break;//超过半数满足条件即可用；
        }
        
        if(tmpResult>0)
        {
            v_AveResult=tmpResult;
            return true;
        } 
        else
        {
            return false;
        }
    } 
    else 
    {
        return false;
    }
}

//2012-12-25 ADD 
void FI_IDLE_T3FK(void){
  
    //油门刚回到怠速或刚恢复供油时,给t3反馈的积分项和比例项赋初值
    if(VF_JBACKIDLE_ENG||VF_JCI_CFCI)
    {
        if(VF_JBACKIDLE_ENG)no_t3_fk_run=4;//call Set_no_t3_fk_run_when_toidle;//设置油门回怠速时，停止T3FK的圈数；//增加VF_JBACKIDLE_ENG，移到INJ_CTL.C
        vc_tw1_t3FK_I= av_dti_t3fk; // //将av_dti_t3fk和sum_dtinjh,l和dtinj_tv3_fkh,l变成局部变量；
        v_vbat_dt_TW3FK=0;          //2008-10-23 ADD
        vc_tw1_t3FK_P=0;            //
    }
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//**************sdtinj_t3fk_av********************************
//获得T3反馈值的平均;GET THE AVERAGE VALUE OF av_dti_t3fk
void Ave_C_TW3_FK(byte tps_odr,byte rev_156ave)
{
    //if(!ystdc_tinj&!starts&v_TPS_odr==0&v_rev_156ave<=max_rpm_conidle){ VF_GT30_IDLETG_REV
    //if(!VF_FORINJ_INJPHASE&&!VF_STARTS_ENG&&!tps_odr && rev_156ave<= M0_MAX_RPM_CONIDLE)//!VF_FORINJ_INJPHASE
    if(!VF_FORINJ_INJPHASE&&!VF_STARTS_ENG&&VF_E0_TPS && !VF_GT30_IDLETG_REV)//2014-10-15
    {
        sum_dtinj=sum_dtinj+vc_tw1_t3FK_I-av_dti_t3fk;//2008-5-10更改，发现av_dti_t3fk的计算不对
        av_dti_t3fk=(sword)(sum_dtinj/16);   //2010-9-14 发现结果被截断，from sbyte to sword
    }
    return;
}

//2009-2-19 变更，返回值为DOM修正后的T3；
word check_t3ers(word tw3,byte vbat)
{
    int stDOM_X;
    long int kaifangshu;
    long int kaifang;
    long int Xfang;
    long int Xfang1;
    byte nb;    //2009-4-18 zhangping add 
    byte bln;   //2009-4-18 zhangping add 
    
   // if(!VF_T3GET_INTINJ)return tw3;
   // VFF_NOZZLEINI_DOMFAILURE_DTC=false;  //2010-3-3 delete
    VF_T3BECHECK_INTINJ=true;//2010-3-3
   // return tw3;
    
    if(VF_DOM_SYS)return tw3;//2014-11-28 add in check_t3ers
    
    //if((tw3>0x8A) && (v_mrealDomB>0) &&(!VFF_NOZZLEINI_DOMFAILURE_DTC))   //2009-4-18 add condiction VFF_NOZZLEINI_DOMFAILURE_DTC
    if((tw3>0x8A) && (v_mrealDomB>0) &&(!VFF_NOZZLEINI_DOMFAILURE_DTC)&&tw3<=T3_DOM_ULIMIT)   //2009-4-18 add condiction VFF_NOZZLEINI_DOMFAILURE_DTC
    {                                              //2010-3-13 add MV_EXHAUST<=PL150
                                    //2010-5-17 add tw3<800
        // kaifangshu=81928192 ;
        kaifangshu= (long int) v_mrealDomB*(long int)v_mrealDomB-4*(long int)v_mrealDomA*(long int)(tw3*40-v_mrealDomC);
        /*
        #if MV_EXHAUST<=PL150
            kaifang=2048;
        #else
            kaifang=4096;//2010-8-30
        #endif
        */
        if(kaifangshu>0)
        {
            kaifang=(long int)v_mrealDomB-(sword)tw3*6;//2010-8-30
            
            stDOM_X=1; 
            nb=0;
            bln=true;
            do
            {    
                kaifang=(kaifangshu/kaifang+kaifang)/2; 
                if(nb>0)
                {
                    Xfang=kaifang*kaifang-kaifangshu;
                    if (Xfang<=0)
                    {
                        Xfang1=Xfang+2*kaifang+1;
                        if(Xfang1>=0)
                        {  
                            stDOM_X=0;
                        }
                    } 
                    else //2010-5-17 恢复，因为昨天发现T3 dom计算在大T3时，时间很长；
                    {
                        Xfang1=Xfang-2*kaifang+1;
                        if(Xfang1<=0)
                        {  
                            stDOM_X=0;
                        }
                    } 
                    
                    if(nb>5)
                    {
                        bln=false;
                        break;
                    }
                }
                nb++;
                CLR_WDG;
            }while(stDOM_X);
              
            if(bln)
            {
                stDOM_X=(long int)((long int)v_mrealDomB-kaifang)*512/(long int)(v_mrealDomA);
                Xfang=(long int)((long int)((long int)m_DOMB-(long int)((long int)m_DOMA*stDOM_X)/1024)*stDOM_X/1024-(long int)m_DOMC)/40;
                tw3=Xfang; 
            }
        }
          
    }
    
    Xfang=(long int)vbat*(long int)v_mrealVRate/1024;
    vbat=Xfang;


    VF_T3ER_INJCTL=false;  //校验T3值是否有误
    //if(tw3>0x900) { VF_T3ER_INJCTL=true;}//移到CTL_TW3和CAL_TV3_TEMP中
    //if(tw3<0xFA) { VF_T3ER_INJCTL=true;}//2008-3-29 zhangping add
    //2008-5-20 这个限制有问题，昨日刚改的喷油器T3(T1=3000)的值小于1.000ms
    //更改如下：
    //if(tw3<0x8A) { VF_T3ER_INJCTL=true;}//2008-3-29 zhangping add
    if(((tw3<0x4A&&tw3>0)||tw3>1250)&&VF_MMOVE_ENG) {  //2010-3-14 ADD CONDICTION tw3>0;
        if(nb_t3_out<255)nb_t3_out++;
        //VF_T3ER_INJCTL=true;
    }//2008-7-9 zhangping add
    else nb_t3_out=0;
    /*
    //2009-11-18 add
    if(tw3<125) {
        //VF_T3ER_INJCTL=true;
        if(nb_t3_out<255)nb_t3_out++;
    }
    */
    if(nb_t3_out>NB_T3OUT_LIMIT)VF_T3ER_INJCTL=true;
    
    return tw3;
}

//**********SMAP_MAX_TEST**********************************************
//功能：对TW1按最值进行限值；
//  TW1限定最大、最小值之间；
//  当转速高于一定值后，最大值与转速相关；
sword Limit_TW1(word prd_cas,sword tw1,byte vbat) 
{
    word w;
    byte prd_cash;
    byte dv;
    sword sw;
    
    prd_cash=(byte)(prd_cas>>8); 	
    w=M0_T1MAX_4US;//MAX_TEMPH,MAX_TEMPL=MAX_TINJH,0;   //MAX_TINJH	EQU	0EH	;IF	MAP_TINJ>0E0H,LET	MAP_TINJ=0E00H	

    if(prd_cash<PCA3907H)
    {
         //w=M0_INJ_BIG_PCAH+prd_cas;   //2009-1-17 临时
         //w=250+prd_cas;   //2009-1-18 临时
         w=prd_cas/2+prd_cas;   //2009-1-18 临时   //2010-2-25 临时，可能有复位的风险
    }

        //2012-10-9
    else if(VF_STARTS_ENG){//2012-10-9 发动机启动时
        if(vbat>M0_V_LOWTH)dv=0;
        else if(vbat<M0_VMIN)dv=M0_V_LOWTH-M0_VMIN; 
        else dv=M0_V_LOWTH-vbat;   
        
        w+=M0_DT1MAX_4US_PER1BITV*(word)dv;
    }
  
    //2013-4-22
    
    if(v_rev_625rpm>REV62D5_8500){
        //w=1800;//1750;//  7000ms
        if(w>1800){
            w=1800;
            //no_t3_fk_run=4;//2013-7-13    
        }
    }

    //#if MV_EXHAUST>PL150   //2014-12-15 DELETE
        //2013-7-7
    #if SYS==SYS_PI   //2014-12-15
        if(v_rev_625rpm>REV62D5_8500){
            sw=F_2DLookUp(vbat,M2_T1MAXD8_V,6,0,false);
            
            sw*=8;//2013-7-10
            

            sw=prd_cas+prd_cas-sw;


            //w=1500;//1750;//  6ms
            //w=1375;//1750;//  6ms
            //1375 FOR  5.5
            if(w>sw){
                
                w=sw;//2013-7-13
            };
        }
   #endif     
   //#endif
  
  
    //if(tw1>M0_T1MAX_4US)tw1=M0_T1MAX_4US;//2013-5-9  //2013-7-12 DELETE ,可能影响三轮车启动；
  
    if(tw1>w) {
        tw1=w;//v_FMF_H,v_FMF_L=min(MAX_TEMPH,MAX_TEMPL,v_FMF_H,v_FMF_L);
        if(!VF_STARTS_ENG) {
            no_t3_fk_run=4;    //2013-7-13
        }
    }
    
    if(tw1<M0_MIN_TINJ) tw1=M0_MIN_TINJ;//v_FMF_H,v_FMF_L=max(MIN_TINJ,0,v_FMF_H,v_FMF_L);
    return tw1;
    
}

//备注：
//存储T4数据的地址定义参考：add_ee.h
//存储的读取函数参考：ee.h}


//2009-4-15
//blnDt3b:代表是否需要的是T3B与标准值之间的差异
/*
函数：Cal_T3b
功能：计算t3b
参数：
    vbat--电池电压，驱动上一次喷油的电池电压，单位：采样值
    tw1p--驱动上一次喷油的t1，单位：4us
    blnDt3b--false:返回结果是t3b，true：返回结果是t3b与标准t3b的差异
返回值：
    与blnDt3b有关，单位为4us；见blnDt3b的说明
相关变量说明：
    以下参数是在t4扫描后获得后存入eeprom,每次ecu上电后,从eeprom中读取
        v_gama0--是个体喷油器的a与标准喷油器之间的差异;
        v_gama_tmp--是个体喷油器的b与标准喷油器之间的差异;
        v_gama_inuse--是个体喷油器的c与标准喷油器之间的差异;
    
    t3b_st--标准t3b
    
    以下参数为常数,取决于t3模型,与个体喷油器无关,以下参数是为了便于运算而被放大的
        #define 	aa10_8	-27
        #define 	ba10_8	-408
        #define 	ca10_8	1459
        #define 	ab_10_6	-356
        #define 	bb_10_6	4642
        #define 	cb_10_6	-10723
        #define 	ac_10_0	48
        #define 	bc_10_0	467
        #define 	cc_10_0	6808
    t3b--返回值    
*/
sword Cal_T3b(byte vbat,word tw1p,byte blnGamaE0)//,byte blnDt3b)
{
    //2009-1-14 added bylqj
    long int aM10_8;
    long int bM10_8;
    long int cM10_8;
 
    long int t3b;
    //long int t3b_st;


    //2009-2-20更换公式
    aM10_8=(((aa10_8*(long int)vbat)/64+ab_10_6)*(long int)vbat/128+ac_10_0);
    bM10_8=(((ba10_8*(long int)vbat)/64+bb_10_6)*(long int)vbat/128+bc_10_0);
    cM10_8=(((ca10_8*(long int)vbat)/64+cb_10_6)*(long int)vbat/128+cc_10_0);


 /*   aM10_8=(MUL((MUL(aa10_8,vbat)/64+ab_10_6),vbat)/128+ac_10_0);
    bM10_8=(((ba10_8*(long int)vbat)/64+bb_10_6)*(long int)vbat/128+bc_10_0);
    cM10_8=(((ca10_8*(long int)vbat)/64+cb_10_6)*(long int)vbat/128+cc_10_0);
    //扩大了8192 , 
   */
    //cM10_8=(cM10_8+v_gama_inuse);   //v_gama_inuse 是 gama
    if(!blnGamaE0)cM10_8+=v_gama_inuse;   //v_gama_inuse 是 gama  //2012-8-22

             
    //t3b=(((aM10_8*tw1p)/50*tw1p)/5+bM10_8*tw1p+cM10_8*250)/8192;
    t3b=((((aM10_8*tw1p)/50*tw1p)/5+bM10_8*tw1p+cM10_8*250)>>13);    //2012-8-23优化
   // t3b+=100;

    
   // if(blnDt3b)t3b=t3b-t3b_st;   //2010-2-23 delete
    
    return (sword)t3b;
}

void  Cal_ABC(byte vbat)//,word t30,word t3d)     //v_tw3_t3000,v_T3d_ini
{
        long int n3;   
        
      // 进入计算T1b的三个参数
      //2010-1-11 临时更改电压特性模型 
        v_nb_T3d++;    
      //未对电压进行判断
        v_Vbat_for_gama=vbat;  //t4扫描，计算gama用的电压采样值；
        n3=CO_G1_M8192*(word)vbat;        //0.04054(模型中的系数)  *0.0625*8192
        //v_T3d=(v_T3d_ini+T30_3000-v_tw3_t3000);   //y3: gama

        //2015-7-12 
        // REPLACED T30_3000 BY M0_TV3_T1EQ3D5_0
       // v_T3d=(v_T3d_ini+M0_TV3_T1EQ3D5_0+M0_TV3_T13D5MS_SET-v_tw3_t3000);   //y3: gama
        v_T3d=(v_T3d_ini+265-v_tw3_t3000);  //2017-2-21 T30修正的基准值1.06，与DT3计算的T30修正值进行区别

        //2010-5-12 CHANG BACK TO THE FOLLOWING
        n3=n3+(((long int)v_T3d+CO_G2_M250)*8192)/250;   // 1.91*    //2010-3-13纠正
        v_gama_new=(sword)n3;//2010-1-11 
}

//功能：存储gama:
//AdrGama代表存储的位置；
//0:gama0,即最近一次ECU开关5次钥匙时存储的值；
//2:gama临时值,即最近一次非开关5次钥匙时存储的值，有待下一次满足条件时在此扫描后方可试用的值；
//1:当前使用的值；
//3:存储本次正式gama与上一次正式gama的差异；
/*
#define ADR_GAMA_0      0
#define ADR_GAMA_INUSE  1
#define ADR_GAMA_TMP      2
#define ADR_GAMA_DELTA      3
*/
//关于GAMA的初值：
//v_gama_new:与开关钥匙无关，在有扫描结果、Cal_ABC时赋值；
//v_gama_tmp，v_gama_inuse，v_gama0都是在load_check_memorydate时赋初值；
//
void Save_Gama(byte AdrGama)
{
    word ABCsave_num;
    byte ABC_save;
    byte adh;
    byte adhb;
    byte adl;
    byte adlb;
    byte valh;
    byte vall;
    sword sw;
    word val_all;


    switch(AdrGama)
    {   //如果是临时存储
        //将临时结果存入原来v_gama_tmp的位置；
        case ADR_GAMA_TMP:
            v_gama_tmp=v_gama_new;
            adh=EE_DT3BH_10;
            adhb=EE_DT3BH_10B;
            adl=EE_DT3BL_11;
            adlb=EE_DT3BL_11B;
            val_all=v_gama_new;        
         //   valh=v_gama_new>>8;
         //   vall=v_gama_new;
            break;
        case ADR_GAMA_INUSE:
            
            sw=(v_gama_new-v_gama_inuse)/32;
            if(sw>125)sw=125;
            else if(sw<-125)sw=-125;
            v_gama_delta_div32=(sbyte)sw;
            
            v_gama_inuse=v_gama_new;
            adh=EE_DT3CH_12;
            adhb=EE_DT3CH_12B;
            adl=EE_DT3CL_13;
            adlb=EE_DT3CL_13B;
            val_all=v_gama_inuse;        
         //   valh=v_gama_inuse>>8;
         //   vall=v_gama_inuse;
            break;
        case ADR_GAMA_0:
            v_gama0=v_gama_new;
            adh=EE_DT3AH_8;
            adhb=EE_DT3AH_8B;
            adl=EE_DT3AL_9;
            adlb=EE_DT3AL_9B;        
            val_all=v_gama0;        
        //    valh=v_gama0>>8;
       //     vall=v_gama0;
            break;
        case ADR_GAMA_DELTA:
            adh=EE_DELTAGAMA;
            adhb=EE_DELTAGAMAB;
            adl=EE_DELTAGAMA;
            adlb=EE_DELTAGAMAB;  
            val_all=v_gama_delta_div32;
                    
      //      valh=4;
     //       vall=v_gama_delta_div32;      
            break;
    }
        if(AdrGama!=ADR_GAMA_DELTA)
        {
            
           valh=val_all>>7;
          // vall=val_all%128;
           vall=val_all&0x7f;
        }
        else
        {
            vall=v_gama_delta_div32;   
        }

    ABCsave_num=0;
    if(AdrGama!=ADR_GAMA_DELTA)
    {
        ABC_save=Save_Data_EE(adh,valh);
        if(ABC_save)
        {
             ABCsave_num++;
        }
        ABC_save=Save_Data_EE(adhb,valh+1);
        if(ABC_save)
        {
             ABCsave_num++;
        }
    }
    ABC_save=Save_Data_EE(adl,vall);
    if(ABC_save)
    {
         ABCsave_num++;
    }
    ABC_save=Save_Data_EE(adlb,vall+1);
    if(ABC_save)
    {
         ABCsave_num++;
    }
}

//T4扫描成功后，置标志位，以便：
//   怠速O值扫描；
//关钥匙时存储GAMA；
void SetFlagAterScanT4(byte vbat)
{
    
    VF_T1SCANINGBREAK_INJCTL=true;//需要推出扫描      //2010-3-13将终止的动作放在这里，而不是校验GAMA之后
    VF_NOTNEED_DROPSCAN_SYSINI=true;//2009-11-25
    VF_RUNENOUGH_FORINI_SYSINI=false;//2010-3-15
    //
    //if(v_gama_new>-3200&&v_gama_new<3200)
    if(Check_GamaValid(CHECK_GAMA_STATUS_NEW))
    {
        VFF_NOZZLEINI_T1DGOT_DTC=true;
        VF_NEEDSAVEGAMA_SYSINI=true;//　//需要在关钥匙时存储扫描值；
    
        if(!VF_SEARCHTMP_SYSINI)
        {   //正式扫描时需要预估R值，即T1的偏移量；
            //if(VF_TIER_SYSINI)2013-4-23 delete
            GetRbyT1d(vbat); //2010-3-13 ADD CONDICITON if(VF_TIER_SYSINI)
            v_gama_inuse=v_gama_new; 
            
            VF_PNNOINI_SYSINI=false;//2009-7-31 add     //2009-8-3 纠正
            FC_ti_set();//2009-7-31      //清除O值已调试的标志； 但未清除O值
        }
                 //  if(AdrGama==ADR_GAMA_0)FS_PNINIT_FLAG();//2009-7-31 //设置为动力喷嘴已初始化；
                   //在没有O值的情况下，修正R值，否则不用修正；
    }
    else 
    {
        VFF_NOZZLEINI_T1DGOT_DTC=false;
        v_DTC_T4ScanFail=DTC_T4_GAMA;


    }
}

//2010-3-5
//应该在关钥匙之后第一个执行，因为其执行结果v_gama_to_cls需要用来修正闭环自学习值；
//2010-3-25 发现写完程序后，直接关钥匙老是要调用这里；
void SaveGama_AfterKeyoff(void)
{                   
    if(VF_NEEDSAVEGAMA_SYSINI)//VF_NEEDSAVEGAMA_SYSINI, VF_SEARCHTMP_SYSINI 
    {   //Set_Miles_NeedT4Flag
        // VF_MILES_NEEDT4_SYSINI||VF_MILES_NEEDT4AGAIN_SYSINI
        if(VF_SEARCHTMP_SYSINI)
        {   //&&!VF_MILES_NEEDT4_SYSINI&&!VF_MILES_NEEDT4AGAIN_SYSINI
            /*
            if(VF_MILES_NEEDT4_SYSINI)
            {
                Save_Gama(ADR_GAMA_TMP);    //VF_SEARCHTMP_SYSINI b//不管是否成，都存储临时GAMA;
                if( Check_GamaValid(CHECK_GAMA_STATUS_NEWTO0))
                {
                 //   Save_Gama(ADR_GAMA_TMP);    //VF_SEARCHTMP_SYSINI  //2010-3-13 move out
                    Set_Miles_NeedT4Flag(2);   
                } else
                {
                    VF_MILE_INI_FAIL=true;//Set_Mile_Fail(true);//增加一次自检失败的次数
                }
            }
            else if(VF_MILES_NEEDT4AGAIN_SYSINI)
            {
                if( Check_GamaValid(CHECK_GAMA_STATUS_NEWTOTMP))
                {
                    v_gama_new=(v_gama_tmp+v_gama_new)/2;
                    if(Check_GamaValid(CHECK_GAMA_STATUS_NEWTO0))
                    {
                        Save_Gama(ADR_GAMA_INUSE);
                        Save_Gama(ADR_GAMA_DELTA);
                        v_gama_to_cls=v_gama_delta_div32;
                        
                        Set_Miles_NeedT4Flag(0);   
                    } else
                    {
                        VF_MILE_INI_FAIL=true;//Set_Mile_Fail(true);//增加一次自检失败的次数
                    }
                    
                } else 
                {
                    VF_MILE_INI_FAIL=true;//Set_Mile_Fail(true);//增加一次自检失败的次数
                }
                v_gama_tmp=v_gama_new;
                Save_Gama(ADR_GAMA_TMP);
                
            } *///2010-5-22 delete
        }
        else//   Check_GamaValid( ,,CHECK_GAMA_STATUS_NEWTO0);
        {
          //  Set_Miles_NeedT4Flag(0);
            Save_Gama(ADR_GAMA_0) ;
            Save_Gama(ADR_GAMA_INUSE);
            Save_Gama(ADR_GAMA_DELTA);
            Save_Gama(ADR_GAMA_TMP);

            FS_PNINIT_FLAG();//2009-7-31 //设置为动力喷嘴已初始化；
            //if(VF_TIER_SYSINI)2013-4-23 delete
            FS_tw1_t3fk((byte)vc_tw1_t3fk);
            ResetMiles_AfterIni();//2010-3-13 add
        }   
    }
    
    //保存GAMA:
    //1 用非初始化的方式获得的的GAMA
    //2 用初始化方式获得的；
    
}

//根据T1D预测R值；
void GetRbyT1d(byte vbat) 
{   
    sword sw;
    //2009-4-16 add 用T1D得到初始的T3反馈值。
    vc_tw1_t3fk=0;
    sw=(sword)v_T1d-Set_TW1_byDT3mode(0,vbat);//
    if(sw>100)sw=100;    //2009-4-20 correction
    if(sw<-100)sw=-100;
    vc_tw1_t3fk=sw; 
}

//校验GAMA是否是有效的；
/*
#define CHECK_GAMA_STATUS_NEWTO0    1 //校验新GAMA与GAMA0(即初始化时测试的GAMA)是否满足要求
#define CHECK_GAMA_STATUS_NEW       2 //校验新GAMA是否满足要求
#define CHECK_GAMA_STATUS_NEWTOTMP  3 //校验自检时两次GAMA的差异是否满足要求；
//  Check_GamaValid( ,,CHECK_GAMA_STATUS_NEWTO0);
//0.01的GAMA差异对应程序中差异为：gama*8192--81.92
//#define GAMA_DEC_P10    -819//GAMA减小0.06 
//#define GAMA_DEC_P06    -491//GAMA减小0.06 
//#define GAMA_DEC_P03    -245
//#define GAMA_INC_P03    245
//#define GAMGA_MIN       -3000
//#define GAMGA_MAX       3000
*/
byte Check_GamaValid(byte status)//Check_GamaValid(CHECK_GAMA_STATUS_NEWTOTMP)
{
    sword sw;
    
    switch(status)
    {
        case CHECK_GAMA_STATUS_NEWTO0:
                sw=v_gama_new- v_gama0;
                if((sw>GAMA_DEC_P06)&&(sw<GAMA_INC_P03))return true;
            break;
        case CHECK_GAMA_STATUS_NEW:
                if((v_gama_new>GAMGA_MIN)&&(v_gama_new<GAMGA_MAX))return true;    
            break;
        case CHECK_GAMA_STATUS_GAMA0:
                if((v_gama0>GAMGA_MIN)&&(v_gama0<GAMGA_MAX))return true;   
            break;
        case CHECK_GAMA_STATUS_NEWTOTMP:
                sw=v_gama_new- v_gama_tmp;
                if((sw>GAMA_DEC_P03)&&(sw<GAMA_INC_P03))return true;
            break;
    }
    return false;
}
 
word Cal_T1d0(byte vbat)
{
    
    return (word)((((M0_T1D0_2_M256M128*(long int)vbat)/128
        +M0_T1D0_1_M256)*(long int)vbat
        +M0_T1D0_0_M256)/256);
        
}


/*
    根据实测的参数，计算DT3
*/
void FC_DT3(byte vbat,word tw1p,word tw3){
  
    sword co_t35_m;     //2008-9-10 增加
    sword dtw3;
    sword sw;

    v_T3b=Cal_T3b(vbat,tw1p,false);//,false);//2009-4-15
    //t3b1=t3b;
    //t30电压修正
    //DT3c=T30-1.06-0.0553*(E30-E)      2009-7-7      //0.0564 2009-12-9
    //              +0.0553*0.0625*250*(E30-E)//采样值
    //              +0.87*(E30-E)             //采样值  
    //2008-9-19 确认：0.019之前的符号应该是减号
    /*  
        v_dtw3+=(sword)vc_tw3_t35-128+(3*(221-(sword)vbat))/10;
    */
    //sw=(sword)vc_tw3_t35-128;
    //2016-3-5 发现这里应该-M0_TV3_T1EQ3D5_0，而不应该-128
    #if M0_MFUD1==MFUD1_3 && M0_MFUD6==51 && M0_MFUD4==MFUD4_M10
        //通机：因为涉及到GENERAC已发货和LONCIN已标定，暂时不改
        sw=(sword)vc_tw3_t35-128;
    #else
        //sw=(sword)vc_tw3_t35-M0_TV3_T1EQ3D5_0;
        sw=(sword)vc_tw3_t35-M0_TV3_T13D5MS_SET; //2016-3-8 纠正为 M0_TV3_T13D5MS_SET
    #endif

    co_t35_m=(sw-(DT3c_M100*(M0_VBAT_ST-(sword)vbat))/100);//2008-9-10 增加，
    //t3b=t3b+co_t35_m;
    dtw3=(sword)tw3-v_T3b-co_t35_m;

    if(dtw3>600)    //2009-12-16 change to 500
    {
        dtw3=600;       //2010-5-22 FROM 500 TO 600
    } else if(dtw3<0)dtw3=0;

    v_dtw3=dtw3;   //获得本次DT3
}

/*
    初始化泵油
*/


void Init_PN_pp(void)
{
    word w;
    sword sw;
    
    nb_pump=0;
    nb_pump_constdt3=0;
    
    #ifdef SYS_TRADITIONAL
		v_dtw3_obj=0;
		v_dt1=0;
	#else
		v_dtw3_obj=FG_DT3_Target(v_fmf_pump);//2009-4-14 add   //2009-5-3
		//v_dt1=(sword)vc_tw1_t3fk-100;
		v_dt1=(sword)vc_tw1_t3fk-200; //2013-4-18 from 100 to 200
	#endif  

		
		#ifdef	TE_10K	//20210728 chenwei add 10K缸温 
   		 //2015-12-17 add 
        if(v_te>80)w=80;
        else if(v_te<5)w=5;
        else w=v_te;
		
		#endif

		#ifdef	TE_2K	//20210728 chenwei add 2K水温 
		
        if(v_te>55)w=55;
        else if(v_te<5)w=5;
        else w=v_te;
		
		#endif
        
        //if(VF_PUMP1ST_INJCTL)w=80;//2016-4-28 delete
        //w=M0_FMF_PUMP;
        
        sw=(sword)M0_FMF_PUMP-w;
        
        //if(sw<10)sw=10;
        if(sw<1)sw=1;//2016-4-18 v_fmf_pump
        v_fmf_pump=(byte)sw;
        
        
       
       /* sw=(sword)M0_NB_PUMP_FAIL-w/2;  //2016-4-28 不知道是不是这个写法有问题，改成下面的写法
        //if(sw<10)sw=10;
        if(sw<1)sw=1; //2016-4-26 ADD 
        v_nb_pump_limit=(sbyte)sw;
       */
       
       w=w/2;
       if(w>M0_NB_PUMP_FAIL)
       { 
          v_nb_pump_limit=0;
       } 
       else
       {
          v_nb_pump_limit=BYPTR_M0_NB_INJDRV-w;   
       }
    
         
}

/*目标泵油
      功能：
          按一定流量泵油，当T3稳定时，就表示泵油成功；
          并达到目标值；
      退出条件：
          泵油成功；
          泵油失败但是达到一定次数；
          有T3故障
      标志位：
    
      变量：
          扫描用T1，
          扫描用T3,
          扫描的次数；
          扫描用油量；
          扫描得到的DT3，4个
      
      流程：
          DT1=R值;
          T1=T1()+DT1;
          
          DT3_TG
          
          执行一次T1之后
          DT3_TG
          DT3_NOW
          
      原则：
          满油时，DT3/DT1=0.3--0.4
               
              
*/  

void FD_pump(byte vbat,word tw3,word tw1p){
    sword sw;
    
    nb_pump++;
    
    
    FC_DT3(vbat,tw1p,tw3);
    
    if(nb_pump>1)
    {
        sw=(sword)v_dtw3_obj-v_dtw3;
        if((nb_pump_constdt3&0x01)==0x01)sw=0;
        //sw/=2;
        if(sw>50)sw=50;
        if(sw<-50)sw=-50;
        //if(sw>8||sw<-8)
        if(sw>1||sw<-1)//2015-5-20
        {
            v_dt1+=sw;
            nb_pump_constdt3=0;
        }else{
            //DT3接近目标值
            v_Tw3_CurTw1[nb_pump_constdt3]=v_dtw3;
            
            nb_pump_constdt3++;
            
            sw=(sword)vbat_lst-vbat;
            if(sw<=2&&sw>=-2){//
                if(nb_pump_constdt3>=4)//再判断两次DT3之间
                {
                    //sw=(sword)v_dtw3_lst-v_dtw3;
                    /*sw=(sword)v_Tw3_CurTw1[1]+(sword)v_Tw3_CurTw1[3]
                        -(sword)v_Tw3_CurTw1[0]-(sword)v_Tw3_CurTw1[2]; */
                    sw=(sword)v_Tw3_CurTw1[0]+(sword)v_Tw3_CurTw1[2]
                      -(sword)v_Tw3_CurTw1[1]-(sword)v_Tw3_CurTw1[3];
                    
                    //if(sw<=48&&sw>=12){
                    if(sw<=96&&sw>=24){//是两次，需要成倍
                        //泵油结束
                        sw=v_Tw3_CurTw1[1]-(sword)v_Tw3_CurTw1[3];
                        
                        if(sw>=-3&&sw<=3){
                            sw=v_Tw3_CurTw1[0]-(sword)v_Tw3_CurTw1[2];
                            if(sw>=-3&&sw<=3)VF_PUMP_INJCTL=false;
                        }
                        
                        
                    } 
                    
                    if(VF_PUMP_INJCTL){
                        nb_pump_constdt3=0;
                    }
                }
            } else{
                nb_pump_constdt3=0;    
            }
        }
        
        
        //if(nb_pump>M0_NB_PUMP_FAIL)
        if(nb_pump>v_nb_pump_limit)//2015-12-17
        {
            //泵油次数过多，泵油失败，退出
            VF_PUMP_INJCTL=false;
        }
    }else nb_pump_constdt3=0;
    
    
    vbat_lst=vbat;

    
    v_dtw3_lst=v_dtw3;
    sw=v_dt1+Set_TW1_byDT3mode(v_fmf_pump,vbat);//
    
    //2016-1-11 for FD_pump 
    if(sw<100)sw=100;
    v_T1Scan=(word)sw;
    
    if((nb_pump_constdt3&0x01)==0x01){
        //如果nb_pump_constdt3为偶数
        //T1增加0.25ms，即62.5，使用64
        //v_T1Scan+=64;
        v_T1Scan-=64;
    }
    
    if(v_T1Scan>2000)v_T1Scan=2000;//2015-7-10 limit t1 for pump

}

/*
    2014-10-6 move from sys_init to inj_ctl.c
    
*/
void load_check_memorydate(void)
{ 
 // word checkfir;
 // word checksec;
  
    sword loadswone;
    sword loadswtwo;
    sword m_datesw;
    byte checkerr; 
    byte numforcase;
    byte i;
    byte m_addrtodate[4];   //2009-3-5 change for 3 to 4
    byte b1_1;
    byte b2_1;

 checkerr=0;
 numforcase=0;
 
 for(i=0;i<=25;i=i+4)
 {
    numforcase++;
    
    switch(numforcase)
    {
              case 1 : m_addrtodate[0]= EE_DOMAH_1  ; 
                       m_addrtodate[1]= EE_DOMAH_1B ; 
                       m_addrtodate[2]= EE_DOMAL_2  ;
                       m_addrtodate[3]= EE_DOMAL_2B ; 
                       break;
              case 2 : m_addrtodate[0]= EE_DOMBH_3  ; 
                       m_addrtodate[1]= EE_DOMBH_3B ; 
                       m_addrtodate[2]= EE_DOMBL_4  ;
                       m_addrtodate[3]= EE_DOMBL_4B ; 
                       break;
              case 3 : m_addrtodate[0]= EE_DOMCH_5  ; 
                       m_addrtodate[1]= EE_DOMCH_5B ; 
                       m_addrtodate[2]= EE_DOMCL_6  ;
                       m_addrtodate[3]= EE_DOMCL_6B ; 
                       break;
              case 4 : m_addrtodate[0]=  EE_DT3AH_8  ; 
                       m_addrtodate[1]= EE_DT3AH_8B ; 
                       m_addrtodate[2]= EE_DT3AL_9  ;
                       m_addrtodate[3]= EE_DT3AL_9B ; 
                       break;
              case 5 : m_addrtodate[0]= EE_DT3BH_10  ; 
                       m_addrtodate[1]= EE_DT3BH_10B ; 
                       m_addrtodate[2]= EE_DT3BL_11  ;
                       m_addrtodate[3]= EE_DT3BL_11B ; 
                       break;
              case 6 : m_addrtodate[0]= EE_DT3CH_12  ; 
                       m_addrtodate[1]= EE_DT3CH_12B ; 
                       m_addrtodate[2]= EE_DT3CL_13  ;
                       m_addrtodate[3]= EE_DT3CL_13B ; 
                       break;
              case 7 : m_addrtodate[0]= EE_VRATE_7  ; 
                       m_addrtodate[1]= EE_VRATE_7B ;  
                       break;
            } 
    
    m_datesw=0;
    b1_1=Load_Data_EE(m_addrtodate[0]);
    b2_1=Load_Data_EE(m_addrtodate[1]);
    loadswone=b1_1;
    loadswtwo=b2_1;
   // if(((byte)(b1_1)==b2_1)
   //     ||
    if((byte)(b1_1+1)==b2_1) //2010-5-6
    {
        loadswone=b1_1;
        loadswtwo=b2_1;
        
        if(loadswone>127)
        {
           loadswone=loadswone-256;
        }
        
        if(i<=23)
        {   
            m_datesw=loadswone*128; 
            b1_1=Load_Data_EE(m_addrtodate[2]);
            b2_1=Load_Data_EE(m_addrtodate[3]);
            loadswone=b1_1;
            loadswtwo=b2_1;
           
          //if(((byte)(b1_1)==b2_1)
          //if(((byte)(b1_1)==b2_1)
          if((byte)(b1_1+1)==b2_1)
          {
              if(loadswone>127)
              {
                 loadswone=loadswone-256;
              }
             m_datesw=m_datesw+loadswone;
          }else
          {
            checkerr++;
          }  
        }else
        {
         m_datesw=loadswone;
        }
        
    }else
    {
     checkerr++;
    }
     
     //m_datesw=0;//2009-3-5
     
     switch(numforcase)
            {
              case 1 : v_mrealDomA=m_datesw;
                       break;
              case 2 : v_mrealDomB=m_datesw;
                       break;
              case 3 : v_mrealDomC=m_datesw;
                       m_datesw=load_check_depart(v_mrealDomA,v_mrealDomB,v_mrealDomC);

                       if(checkerr>0||m_datesw)
                        {
                          VFF_NOZZLEINI_DOMFAILURE_DTC=true;
                          v_mrealDomA=m_DOMA;
                          v_mrealDomB=m_DOMB;
                          v_mrealDomC=m_DOMC;
                          checkerr=0;
                          VFF_NOZZLEINI_NOTDOM_DTC=true;// //判断dom参数的读取不成功为false  
                        } else
                        {
                          v_mrealDomA=m_DOMA+v_mrealDomA;
                          v_mrealDomB=m_DOMB+v_mrealDomB;
                          v_mrealDomC=m_DOMC+v_mrealDomC;
                        }

                        
                       break;
              case 4 : v_gama0=m_datesw;
                       break;
              case 5 : v_gama_tmp=m_datesw;
                       break;
              case 6 : v_gama_inuse=m_datesw;
                       //if(checkerr>0)
                       // m_datesw=load_check_depart(v_gama0,v_gama_tmp,v_gama_inuse,true);
                        //m_datesw=load_check_depart(0,0,v_gama_inuse,true);
                       if(checkerr>0||(!Check_GamaValid(CHECK_GAMA_STATUS_GAMA0)))
                       {
                           v_gama0=0;
                           v_gama_tmp=0;
                           v_gama_inuse=0;
                           checkerr=0;
                           VFF_NOZZLEINI_NOTINI_DTC=true;// //判断T3b参数偏移量的读取不成功为false    
                           VF_PNNOINI_SYSINI=true;//2010-3-8 add 
                       }
                       v_gama_new=v_gama_inuse;//2016-7-31 保证v_gama_new有初值
                          
                       break;
              case 7:  
                       if(m_datesw>-70 && m_datesw<70)
                       {
                            v_mrealVRate=m_datesw+1024;
                       } else
                       {
                            v_mrealVRate=1024;
                            VFF_NOZZLEINI_VBATNOTDOM_DTC=true; //判断电压比率的读取不成功为false 
                       }
                       break  ;
            }                   
 }


  return;
 }
 
//09-8-28  增加对DOM和初始化结果的限值
 byte load_check_depart(sword n1,sword n2,sword n3)
 {
    long int m1;
    

//    n3=n3+910;//m_DOMC*2
    n3=n3+m_DOMCM2;//   2010-3-14修正后合并；
    
    m1=((long int)n1*(-4)+(long int)n2*2+(long int)n3)/40;
   // if(m1>0||m1<-50)       //超出范围为1
    if(m1>5||m1<-50)       //超出范围为1    //2010-5-4 ADD
    {
        return 1; //2010-1-11 临时取消
    }

    return 0;
 }
 
 
 //保存DTW3的目标值；2014-10-16
void SaveDtw3Obj(void)
{
    v_dtw3_obj_lst=v_dtw3_obj;          //2008-12-3 add 
}

//#define M0_FMFTOT1A1	0
//#define M0_FMFTOT1A0	0
//#define M0_FMFTOT1B1	0
//#define M0_FMFTOT1B0	0	

const byte M2_T1MOD_VBAT[4][2]=					//		
    {	125	,	82 	,	//	
    	160	,	70 	,	//	
    	195	,	64 	,	//	
    	225	,	60 	,	//	
    };

word Ctl_T1_TRAD(word fmf,byte vbat)
{
	word tw1;
	//unsigned long int l;
	
	tw1=(word)(fmf<<3);	//2017-10-23 1fmf对应脉宽=4*8/1000=0.032ms
	
	tw1=tw1+4*(220-vbat);
	//v_fmftot1a=vbat*M0_FMFTOT1A1+M0_FMFTOT1A0;
	//v_fmftot1b=vbat*M0_FMFTOT1B1+M0_FMFTOT1B0;
	
	//v_fmftot1a=F_2DLookUp(vbat,M2_T1MOD_VBAT,3,0,false);
	//l=(unsigned long int)tw1*v_fmftot1a/64;
	//tw1=(word)l;

	//tw1=Limit_TW1_TRAD();
	
		
	return tw1;
}


   