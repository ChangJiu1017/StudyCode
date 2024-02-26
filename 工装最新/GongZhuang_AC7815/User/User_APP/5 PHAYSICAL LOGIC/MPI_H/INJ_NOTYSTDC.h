/*===========================================================*/
/*====非喷油圈的控制模块=====================*/
/*===========================================================*/

//功能：初始化非喷油圈策略的参数；
extern void Init_NotYSTDC_beforeStarts(void);

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
extern void CTL_NOTYSTDC(byte rpm625,sbyte tps_ddt) ;

//************************************************************************************
//功能：准备T35测试
//所有准备T35的动作都在该过程中
//其条件分别来自：
//  1 时间到；
//  2 暖车到一定时间；
//  3 刚使能怠速搜索；
//  4 对T35测试结果不满意，需要增加一次测试；
//注意，该函数需要放在Search_IdleSpeed之后，因为用到标记VF_JIDLESCEN_IDLE
//形参：    teng
//          t_eng_d1s
extern void Prep_T35Test(byte teng,byte t_eng_d1s,byte rpm625,byte ccl_cf);

#define REMARK_FOR_T35_HIGHSPEED  1
extern byte v_dt1_for_T35;//T35测试之前后，为了防止稀，增加t1；
extern byte nb_inj_beadd;//累计加喷的次数；
extern byte nb_t30_added;

extern byte tv3_t_num_need;// equ 0x35	;需要测试T35的次数×8 						

