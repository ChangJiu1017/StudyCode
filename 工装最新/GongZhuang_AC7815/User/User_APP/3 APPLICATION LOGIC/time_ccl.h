/*===========================================================*/
/*====时间处理模块=====================*/
/*===========================================================*/

//功能：累加圈数，并设置圈数标识；
extern void FG_CCL(void);

//功能：累加时间，设置时间标识
//形参：prd_cas－－前沿角标周期
//  累计发动机运转时间和设置时标
//  累计启动后总时间到临时时间变量：TIME_TEMP,TIMEH,TIMEL+=v_prd_cas_h,v_prd_cas_l;，
//  当临时时间变量累计超过0.1S时，间超过部分转移到D1SECOND，
//  当D1SECOND部分超出1s后，将超过转移到SECONDH，SECONDL，SECOND中；
//  根据SECONDH，SECONDL，SECOND是否是秒的2,4,16倍，分别置标识；
//  关于几个变量
//  time_flag,f1s是在每次增1S时置位，没增时清
//  v_t_eng_d1s:当临时时间变量累计超过0.1S时,增一；并限定在255
//  VF_MORE1MIN_TMCCL:当时间超过60s时置位
extern void FF_T(word prd_cas,byte blnIni);

//功能：将当前运转圈数累加到EE中，（关闭钥匙时调用）?
//  将发动机运转时间secondh，secondl,sum_second累加到EE中相应的地址：ee_run_timehl，ee_run_timeh，ee_run_time中
//  注意，存储之后，没有对secondh，secondl,sum_second进行清零，
//  理由是：它是被POWER_TIME调用的，调用前已经检测到钥匙要关闭，因此之后发动机是停车了；
//  而在每次发动机重新启动时需要对secondh，secondl,sum_second进行重新清零，
//  将发动机运转圈数cycle_numb_hh，cycle_numb_hl，cycle_numb_h累加到EE中相应的地址：ee_cycle_h，ee_cycle_hl，ee_cycle_hh中
extern void FM_TimeCCL(void);//KEY_OFF

//功能：Starts前对CCL_TIME参数的初始化，所有时间都置零
extern void FI_TimeCCL_BeforeStarts(void); //blnPor  //blnBeforeStarts //检测到启动角标后，对时间参数的初始化?


//功能：当发动机停止时，累加本次开钥匙后的总运行时间
extern void F_sum_T_ENG_WhenStopS(void);

extern void FI_TMCCL_AfterReset(byte blnForTimeReset);

extern void ResetMiles_AfterIni(void);

extern void FC_TimeOutFlag(void);    ///2014-10-4 add 

extern void F_Clr_SL1s(void); 

//----------------------------------------------------------------------------
extern unsigned long int v_t_EngMove_s;        //发动机连续运行时间,单位:S
extern word t_after1stStart_s;//开钥匙后，第一次启动后开始计时，包括中间停车；
extern word v_ccl_131072;
extern unsigned long int v_t_eng_t_s;//2008-10-27 add

extern byte v_t_d1s_afterCI;       //再给油后的时间   2013-4-22

//extern byte v_mile_limit;//2012-12-6

extern byte v_t_forlstrun_d1s;//上一次油门非怠速的时间；

extern byte v_t_eng_d1s;         //COUNT	0.1S CONTINOUESLY;只要发动机在运转就一直在累加,并限定在255
                          //2012-12-21增加含义：连续停机的时间，注意这时的单位是秒

extern word v_t_eng_1s;//发动机持续维持一种状态的时间，例如连续停机（不管钥匙是否开启）

extern byte t_stop_s;//2015-12-11

extern void Clr_t1s_WhenNewState(void); 
//2013-7-16
extern word v_t_pairs_1s;

extern byte v_t_WOT_1s; //油门全开的时间  2013-8-30
extern byte v_t_NOTE0TPS_1s;//油门离开怠速的时间 2013-8-30
extern byte v_t_afterbackidle_d1s;//油门回怠速后3s以内 2017-3-7
extern byte v_t_E0_1s;

extern word v_nb_ccl_r;              //发动机工作圈数

extern byte t_SL_d1s;      //用于自学习的时间，0.1s;
extern byte v_t_ci_d1s;

extern byte v_t_AfterInjON_1ms;//2014-10-8 喷射结束后的时间
extern byte v_t_AfterIgnON_1ms;//2014-10-8 点火结束后的时间

extern byte v_t_afterstarts;

extern byte v_t_CA_ncome;

extern void F_Clr_PostInjOnTime(void); 
extern void F_Clr_PostIgnOnTime(void); 

//#define RUNS_FORPAIRS_LIMIT_A 700 //700S
#define RUNS_FORPAIRS_LIMIT_U 800 //700S
#define RUNS_FORPAIRS_LIMIT_L 500 //700S


#define MILE_FOR_LOCK 60
#define M0_MILE_NOLIMIT 250
//20210819 chenwei add for 侧撑
extern byte nb_cc;
//20210923 chenwei add for MIL环松
extern word v_t_MILGliter_d1s;
extern byte VF_MIL_SWITCH;
extern byte nb_mil_interval;
extern 	byte VF_MIL_DTC4;
extern 	byte VF_MIL_DTC3;
extern 	byte VF_MIL_DTC2;
extern 	byte VF_MIL_DTC1;
extern word v_pcode_lst;
extern byte nb_pcode;

extern byte v_pair_ad_input_keyon;		//开钥匙压力输入存储
extern byte v_pair_ad_input_lst;
extern byte v_pair_ad_input_max_nmov_eng;	//未启动起见压力采样的最大值
extern byte v_pair_ad_input_min_nmov_eng;	//未启动起见压力采样的最小值

extern byte v_Vbat_inuse_keyon;		//开钥匙电压存储
extern byte v_Vbat_inuse_lst;
extern byte v_Vbat_inuse_max_nmov_eng;	//未启动起见电压采样的最大值
extern byte v_Vbat_inuse_min_nmov_eng;	//未启动起见电压采样的最小值

extern byte VF_NOCA_DIAG;
extern byte nb_noca_test;		//判断到可能无角标信号的次数
extern byte v_t_fromkeyon_1s;

extern byte SWITCH_IGN;
extern byte switch_1;
