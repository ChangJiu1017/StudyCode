/*===========================================================*/
/*====喷射单元控制模块=====================*/
/*===========================================================*/

extern void FI_IDLE_T3FK(void);//2012-12-25 

//功能：复位后喷油参数的初始化
extern void Init_InjPara_afterReset(void);//后面需要跟上 Check_TW1SCI_afterRDorWR；

//功能：STARTS前喷油参数的初始化
extern void Init_InjPara_beforeStarts(void);

//*******SET_T1_AS_T35GET***************************
//功能：将用于T35测试的T1载入TINJH,L
//返回值：T35测试时使用的T1;
extern word Set_TW1_forT35Get(void);

//功能：发动机静止时，设置T35测试的次数；
//返回值：T35测试的次数
extern byte Set_NB_forT35Get(void);

//***tnooil_to_ti*****SET_T1_FOR_PUMP******************
//功能：载入泵油T1;
//  将泵油T1载入TINJH,L:no oil T1=inj_shift+t3fk0+modi_dcdy
//形参： vbat_odr－－电池电压级别；vbat_mode－电池电压级别余?
//返回值：泵油T1
//  vbat_odr--电压级别
//  vbat_mode--电压级别余
extern word Set_TW1_forPump(byte vbat) ;

//*******SET_TIMES_FOR_PUMP*****************************
//功能：设置泵油的次数；
//返回值：泵油的次数
extern byte Set_NB_forPump(void);

//**********cal_tv3_temp***************************************
//功能：根据T35测试结果计算得到T35对TW3的修正系数
//  calcute T35, and get the rec_co for FLUX correction;
//  cal_tv3_temp:
extern void Cal_T35(sbyte tps_ddt,word tw3,byte vbat,word tw1_synwith_tw3,word tw4,word tw4_2nd,word nb_inj_forDrv);
                                                                                                        //2008-6-30 add tw1
//功能：初始化T1T3测试的起始点
extern void Init_T1Scaning(byte vbat,byte blnIni);//2008-6-25

//功能：获得T1-T3测试的总次数；
extern word Get_NB_T1SCANING(void);

//是否需要在开钥匙后，进行T1扫描；//2008-12-1
extern byte B_T1_Scaning_Needed(void) ;

//功能：设定T1扫描的频率；单位4us//频率设定
extern word Interval_For_T1_Scaning(void);

//功能：
//      获得当前扫描用的TW1，单位4us
extern word Get_TW1_for_Scaning(void);//

extern word Set_TW1_byDT3mode(sword fmf,byte vbat);       //2008-7-25 add

//extern sbyte Cal_dtTw1_bydVbat(byte vbatCal,byte vbatbTrig);

extern word Ctl_DT3(word fmf,byte vbat,
            sbyte vbat_dt,word tw3,byte tps_odr,byte tps_mode,
            byte rev_156ave,byte teng,word tw1p,word pad_cas);
                  //2009-1-17 add 
extern word check_t3ers(word tw3,byte vbat);               //2009-2-19 返回值为修正后的T3

extern void SaveGama_AfterKeyoff(void);

extern byte Check_GamaValid(byte status);

extern sword Limit_TW1(word prd_cas,sword tw1,byte vbat);  //2012-10-19


extern void Init_PN_pp(void);

extern void SaveDtw3Obj(void);//2014-10-16

//-----declearation of public variable--------------------------
extern sword vc_tw1_t3FK_P;         //T3反馈控制值，即根据上圈的T3和本圈T3目标值，计算得到对T1的修正系数，  注意与vc_tw1_t3fk的区别；
extern sbyte v_vbat_dt_TW3FK;       //TW3反馈结果对VBAT_DT的修正系数；
extern sword vc_tw1_t3FK_I;
extern sword v_dt1;

extern word v_dtw3_obj;//dt3 的目标值，即T3减去基能后剩下的值；
extern word v_dtw3;            //计算得到的DTW3；

extern word v_dtw3_obj_lst;//2008-9-24 add

extern sword av_dti_t3fk;//	equ	0x1e5	;运转中怠速偏移量2的平均值,回怠速时，需要将T3反馈值赋予该值。	

extern sword v_T3b;             //2008-11-27 add for disp
extern word v_T1d;             //2009-1-16 

//2009-4-18 
//临时测试
extern byte nb_T35T;
extern sword vc_tw1_t3fk_now;          //save the ti_offset at	idle set by	user，偏移量2当前值

//extern byte status_T4Scan;  //T4扫描的状态,即当前的位置;
extern byte nb_VbatFlu;        //t4扫描过程中,2009-11-28 连续电压波动的次数;

extern sword v_T3d;   //2010-1-11
extern byte v_Vbat_for_gama;//2010-1-11
extern sword v_gama_new;//2010-1-11
extern word v_T4_ave;//2010-1-14
extern sword v_gama0;
extern sword v_gama_tmp;
extern sword v_gama_inuse;
extern sbyte v_gama_delta_div32;//本次被认为可靠的v_gama_new-上一次v_gama_inuse；

extern sbyte v_dDt3;//2010-1-29

//extern byte nb_inj_lst;//byte nb_inj_lst=v_NB_INJDRV  //2010-1-13 add 
extern sbyte v_DT1ToDT3_Cur;
extern word v_T3_scan_lst;
extern byte v_nb_T1Scan;   //测试T1点数
extern byte v_nb_T1di;//扫到的T1Di的次数；
extern byte v_nb_AveScanT3Fail;
extern word v_T1i;//初始化的起始t1

extern byte v_Point_T4Dec;
extern byte v_Point_T4Asc;
extern byte bInjFlag;
extern word v_Tw4_ScanArray[60]; //测量T1-T3关系时，存储16个点的平均T4值
extern byte v_nb_T1Scan;   // v_Tw4_ScanArray [v_nb_T1Scan];
extern sbyte v_nb_T1for1Point;
extern word v_Tw4_CurTw1[8]; //每个T1测试16个次的数据存放
extern byte v_nb_T4Exist;//T4存在的次数
extern word v_T4_inInjctl;//2010-3-6 tmp
extern byte v_Status_T4Dec; //bitx: 1:下降， 0：未下降；
extern byte v_Status_NotDecMore1;//下降不超过1个单位   //v_Status_NotDecMore1  v_Status_T4Dec

extern byte v_ChaPoint_pre;    //T4绝对上升之前的最近一个特征点；
extern word v_T3d_ini;//t3d对应的原始的t3信号；
extern byte v_nb_ScanT4T1fail; //扫描T1T4点数超差的次数

extern byte nb_Piston_bubble;    //同一个T1测试时，高压腔进气泡的次数；
extern sword v_gama_delta_T1d;//2010-1-11
extern word v_T3b0_T1d0;
extern word v_T1d0;
extern byte nb_T3Bubble;//2010-5-22

//2013-3-23
extern byte v_fmf_pump;
extern byte  nb_T3Fk;
extern byte  nb_T3Bubble;

extern byte  v_Vbat_whenT4_min;
extern sword vc_tw1_t3FK_I; 
extern word v_dtw3_obj_lst;


extern byte no_t3_fk_run;//	equ	0x0e	;无T3反馈的次数；在某些条件下，需要终止T3反馈，no_t3_fk_run的值即是终止的次数；	

extern byte v_sta_T3_LT;//T3反馈的状态，T3小于目标时，位1

extern byte vc_tw3_t35;

#define nb_pump  nb_T3Fk
#define nb_pump_constdt3  nb_T3Bubble

#define vbat_lst  v_Vbat_whenT4_min
//#define v_dt1     vc_tw1_t3FK_I  2017-3-28 删除
#define v_dtw3_lst v_dtw3_obj_lst

extern byte v_DTC_T4ScanFail;//2015-12-17 t4扫描错误的故障指示


extern byte v_nb_T30;//2015-8-28 add 
/*  
    2.0     0.18
    1.3     0.14
    1.8     0.135
    2.0     0.135
*/
extern byte v_nb_pump_limit;// 2016-4-28

//extern byte v_fmftot1a;
extern word Ctl_T1_TRAD(word fmf,byte vbat);

