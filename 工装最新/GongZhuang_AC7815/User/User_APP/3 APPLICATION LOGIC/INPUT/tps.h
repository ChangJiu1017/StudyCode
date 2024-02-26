/*===========================================================*/
/*====TPS处理模块=====================*/
/*===========================================================*/


//功能：复位后TPS参数的初始化
extern void FI_TPS_AfterReset(void);

//功能：根据AD采集结果计算TPS其它参数及变化量；
//      寻找怠速零点位置对应的TPS采样值；
//      诊断TPS;
//      当TPS有误时，采用转速值预测TPS，进入跛行程序
//形参：rev156--v_rev_15d6rpm;
extern void FG_TPS(word rev156);

extern void Check_BOS(void) ;

extern void FS_TPSlst(void);//2014-4-6

extern void SaveADForM(word tps);//2015-4-9

//----------------------------------------------------------------------------
extern byte v_TPS;     //上次TPS采样值平均结果的高8BITS  		
extern sbyte v_TPS_ddt;        //TPS的增量	
extern byte v_TPS_odr;         //TPS级别  	
extern byte v_TPS_mode;        //TPS级别余

//2010-5-11
extern byte v_TPS0_min;
extern byte v_TPS0_AD10	;//	5AH					;节气门位置的0点；MEMORY	FOR	SAVE	JQMWZ0	
extern byte t_1s_NIncTps;
extern byte nb_BH;                           
extern byte nb_BH1;                           

extern byte v_TPd;//油门级别　2012-8-20
extern byte v_TPd_lst;//油门级别　2012-11-22
extern byte v_TPd_lst2;//油门级别　2012-11-22

extern byte v_TPS_lst; //2012-11-14
extern word v_TPS_AD10;

extern word v_TPS0_256sum;

//extern byte nb_tps_back;  //2013-4-8
//extern byte v_TPS0_min_back;
extern byte v_bTPS_AD10_lst;

extern word v_TPS_16sum;//2013-4-9
extern byte v_TPS0_ub;

extern byte v_TPS_bTrans;       //上一次JQMWZ采样值的高8位		

#define M0_ETC_TPS_MIN 25
#define M0_ETC_TPS_MAX 200
extern byte v_T_percent;//2016-3-2 增加负荷百分比

extern byte v_TPd_lst_in1cycle;
extern byte v_TPd_lst2_in1cycle;

extern byte nb_leavidle;
