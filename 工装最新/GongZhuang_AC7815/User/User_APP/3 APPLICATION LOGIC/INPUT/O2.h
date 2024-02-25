/*===========================================================*/
/*====O2信号处理模块=====================*/
/*===========================================================*/

//功能：根据两次O2采集，平均得到O2值v_O2_ad，并将上次值保存在v_O2_last中
//      用于发动机运行时；
//形参; adres_10bit--第一次10 AD采集结果
//      adres_10bit_2--第二次10 AD采集结果 
extern void FG_OS(word adres_10bit,word adres_10bit_2);      //记录O2采集结果 

//功能：O2参数的初始化
extern void FI_OS_BeforStarts(void) ;

//功能：在发动机静止时，只采集一次O2并直接放入O2,而不使用平均
extern void FG_OS_WhenStop(word adres_10bit);
extern void FI_OS_Reset(void);

extern void Detect_OS_Flu(byte tps_odr,byte rev_odr,long int t_eng_s,word prd_cas);

extern word ccl_os_fkfast;     //os 反馈达到快速的水平的圈数
extern sword t_con_for_oswork;//
extern sword t_con_for_osflu;//
extern word v_O2_trend_DecOrNot;          //Os的采样值变化趋势,按顺序,最后一位代表最近一次,位值为0代表下降,位值为1代表不下降;
                            //2008-11-17
extern word v_O2_trend_IncOrNot;       //

extern byte v_nb_OS_AD_CD;

extern word v_t_OS_Lt20_to_now;//最后一次OS信号低于20到现在的时间；2019-1-20 PETER

//20211010 chenwei add 环松氧加热工作策略
extern byte v_t_O2H_delay_1s;			//使用氧加热的延迟时间，距离车辆运行
extern byte v_o2h_n_625;					//氧加热开始工作的转速限值
extern byte v_o2h_tpd;					//氧加热开始工作的开度限值
extern unsigned long int v_t_EngMove_s;
extern byte VF_O2H_STATUS;		//指示氧加热的状态
extern void F_O2H_FORHUANSONG(void);

