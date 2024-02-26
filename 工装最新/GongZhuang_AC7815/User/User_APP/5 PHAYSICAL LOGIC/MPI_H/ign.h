/*===========================================================*/
/*====点火控制模块=====================*/
/*===========================================================*/

//功能：点火参数的初始化
extern void FI_Ign_BeforeStarts(void);

//功能：获得基础点火角：v_CA_ignition
//    并进行温度等修正
//2008-9-25 add ,byte v_rev,byte v_rev_ave_512,byte remark
extern void FG_CA_Ign_basic(void);
//********************************
//低速点火角修正
//功能：在触发点火之前，判断如果是低速，则对点火角进行修正；
//形参： rev156--v_rev_15d6rpm;
//       tps_odr--v_TPS_odr;
extern void FC_Idle_Ign_BeforeTrig(void) ;//word rev156,byte tps_odr,byte tps_mode,sword tes,word rev3d9,byte rev156ave_16

//***************************************************************
//功能：根据点火角计算点火时刻，并触发：v_tl_ignition
//返回值：对应于主定时器的，从现在开始点火之间的时间长度
//形参： t_cas0 --v_t_cas0
//       prd_cash--v_prd_cash
//       prd_cas--v_prd_cas
extern word F_Cal_T_Ign(void);

extern void FI_Ign_afterReset(void);//2009-8-7 add 

extern void FC_IGN_Altitude(void);//byte pair_odr,byte pair_mode,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode

//----------------------------------------------------------------------------
//ign
extern word v_tl_ignition;  //根据角标周期计算得到的点火角对应的时间长度，量纲：4US      
extern word v_t_cas;//volatile v_t_casSTR _v_t_cas;             //计算点火时刻的时间基准，根据情况选择前沿或后沿	

extern sbyte v_CA_ignition;  //点火提前角，量纲：0.7CA     								
extern sword ign_lst8run_sum;

extern word rev15d6_target;//转速目标值；2010-7-6
extern word v_rev15d6tg_nocorr;

extern sbyte co_IdleIgn_mod;   //2010-1-13

//2013-1-14
extern sbyte ign_lst8run_ave; //ign_lst8run_ave=0；ign_lst8run_sum=0；
extern byte dRev_forAirLess;               //当怠速比标准怠速低于该值时，认为气量低
extern sbyte ignMax_forAirLess;

extern sbyte v_igs;
extern byte v_M;
extern byte v_O;

extern byte v_n0_high;
extern byte v_n0_low;
extern byte v_J;
extern byte v_K;
extern sbyte inc_rev_backidle;


