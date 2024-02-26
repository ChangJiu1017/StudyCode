/*===========================================================*/
/*====转速处理模块=====================*/
/*===========================================================*/

//-------------functions----------------------------------
//功能：获得转速及其变化规律，相位判断，低速时转速平均；
//形参：prd_cas－－前沿角标周期
//extern void FG_Rev_BeforeIgn(word prd_cas);

//功能：若在Get_Rev_BeforeIgn中没有计算转速，则计算，冲程检测
//形参：prd_cas－－前沿角标周期
//extern void FG_Rev_AfterIgn(word prd_cas) ;

//功能：复位后，转速和角标参数的初始化
extern void FI_Rev_AfterReset(void);//

extern void FG_Rev(word prd_cas);//2014-3-31

extern void FC_DueMain(void);
//-------------variables----------------------------------
extern word vlst_prd_cas;   //本次两个角标间的时间间隔，即角标周期 	
extern word v_prd_cas_min;  //下一圈角标周期的最小域值，由vlst_prd_cas计算得到
extern word v_rev_15d6rpm;  //转速rpm[量纲 ：15.6RPM];
extern word v_rev_3d9;//2010-1-25

extern byte v_rev_deg;//转速级别  2013-1-4
extern byte v_rev_625rpm;        //save	RPM	using	62.5 unit

extern word v_t_CAHY_timeout;//