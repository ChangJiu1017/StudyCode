#define ACTION_WHENSTOP_PREINJ  0
#define ACTION_WHENSTOP_PUMP    1
#define ACTION_WHENSTOP_T1T3    2
#define ACTION_WHENSTOP_ALL     3

//2009-3-2在sys_init.h中的对初始化码的定义；
#define NOZZLEINI_CODE_ING_STOP 9               //正在对动力喷嘴进行初始化动作；
#define NOZZLEINI_CODE_TESHIGH_STOP 10          //动力喷嘴初始化的温度条件不满足；
//#define NOZZLEINI_CODE_//#define	VFF_NOZZLEINI_TESHIGH_DTC               _V_DTCHHL_STATUS.Bits.b0           //41  //2009-3-2   //喷嘴初始化时,缸头温度过高
#define NOZZLEINI_CODE_T1DGOT                  11 //#define	VFF_NOZZLEINI_T1DGOT_DTC                _V_DTCHHL_STATUS.Bits.b1//        //：获得了T1D
#define NOZZLEINI_CODE_FAILURE_BYNOINFLEXION   12 //#define	VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTC _V_DTCHHL_STATUS.Bits.b2//        //：没有找到T4拐点而导致扫描T1D失败；
#define NOZZLEINI_CODE_ABCGOT                  13 //#define	VFF_NOZZLEINI_ABCGOT_DTC                _V_DTCHHL_STATUS.Bits.b3//    //：获得了计算T3B所需的ABC系数；
#define NOZZLEINI_CODE_ABCSAVED                14 //#define VFF_NOZZLEINI_ABCSAVED_DTC              _V_DTCHHL_STATUS.Bits.b4//    //：成功保存ABC测试结果；
#define NOZZLEINI_CODE_NOTDOM                  15 //#define	VFF_NOZZLEINI_NOTDOM_DTC                _V_DTCHHL_STATUS.Bits.b5//    //：没有DOM
#define NOZZLEINI_CODE_DOMFAILURE              16 //#define	VFF_NOZZLEINI_DOMFAILURE_DTC            _V_DTCHHL_STATUS.Bits.b6//:DOM数据有问题；
#define NOZZLEINI_CODE_NOTINI                  17 //#define	VFF_NOZZLEINI_NOTINI_DTC                _V_DTCHHL_STATUS.Bits.b7//:喷嘴没有初始化；
#define NOZZLEINI_CODE_VBATNOTDOM              18 //#define	VFF_NOZZLEINI_VBATNOTDOM_DTC            _V_DTCHHH_STATUS.Bits.b0//:ECU电压没有dom
#define NOZZLEINI_CODE_VBATVALID               19 //#define	VFF_NOZZLEINI_VBATVALID_DTC             _V_DTCHHH_STATUS.Bits.b1//:电压过低导致无法初始化；
#define NOZZLEINI_CODE_NOINJ                   20 //#define	VFF_NOZZLEINI_NOINJ_DTC                 _V_DTCHHH_STATUS.Bits.b2//:喷油器故障导致无法初始化；
#define NOZZLEINI_CODE_T1DFAIL_T4LET1          21 //#define	VFF_NOZZLEINI_T1DFAIL_T4LET1_DTC        _V_DTCHHH_STATUS.Bits.b3//扫描结果的T4<T1d+500，认为结果有问题
#define NOZZLEINI_CODE_ABCSCAN_RESULTINVALID   22 //#define	VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTC _V_DTCHHH_STATUS.Bits.b4//T1D扫描之后，计算得到的系数非法；2009-3-5

#define IDLEINI_CODE_PNINI  8                //动力喷嘴是否初始化(相当于以前的喷油器设定)//2009-4-7
#define IDLEINI_CODE_TES    7                //2009-4-16 TES条件中包括非暖车过程；
#define IDLEINI_CODE_TPS    4
#define IDLEINI_CODE_N      4
#define IDLEINI_CODE_I      5                //T3故障
#define IDLEINI_CODE_VBAT   6
#define IDLEINI_CODE_OS             1
#define IDLEINI_CODE_OS_LEARNED     1
#define IDLEINI_AIRRICH     2
#define IDLEINI_AIRLESS     3

//2010-3-9
//校验GAMA是否是有效的；
#define CHECK_GAMA_STATUS_NEWTO0    1 //校验新GAMA与GAMA0(即初始化时测试的GAMA)是否满足要求
#define CHECK_GAMA_STATUS_NEW       2 //校验新GAMA是否满足要求
#define CHECK_GAMA_STATUS_NEWTOTMP  3 //校验自检时两次GAMA的差异是否满足要求；
#define CHECK_GAMA_STATUS_GAMA0     4


//NEET4的形参   2010-3-11 
#define NEEDT4_CLR_MILE     0 //清除里程自检的标志
#define NEEDT4_NEED_MILE    1 //需要里程自检的标志
#define NEEDT4_MILE_AGAIN   2 //需要重复里程自检的标志
#define NEEDT4_CLR_REMOVE   3//清除换件自检的标志；
#define NEEDT4_NEED_REMOVE   4//清除换件自检的标志；
#define NEEDT4_REMOVE_AGAIN   5//清除换件自检的标志；



//2008-11-2 new
extern void FG_SysInit_AfterReset(void);//2013-4-2 delete t 
//extern void FR_SysInit_AfterKeyoff(unsigned long int t_eng_s_whenidle);
extern void FI_SysInit_BeforeStarts(void);//；

extern void F_FinishSysInitAction_whenStop(byte whatAction);          //2008-12-16 add

//功能：设置偏移量：计算所说的诊断仪的计算功能
//  注意：偏移量２:DTI_T3FK0是在计算过程中，REMARK=6时，上位机对DTINJ_TV3_FK+vc_tw1_t3fk+DCDY_DT计算平均值而得到；
//				然后上位机，将DTI_T3FK0按写正常的EE的方式写入到地址E_Dti_t3fk0_ADR和E_DTI_T3FK0_AB中；
//				因此，写偏移量２的动作是一个普通的写EE的动作，而与写偏移量1(O值)的动作有所区别。
//  pckese, pcset, pcbtdc, pctback：分别为控制位
//  tw1_idlesc：怠速搜索值对T1的修正系数
//  tw3_sci：上位机对T3的调整量；
//  tw3_offset：T3的偏移量；即偏移量1（O值）
extern void FS_ti_pc(byte pckese,byte pcset,byte pcbtdc,byte pctback,
                sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset);


//功能：保存初始化结果
//2008-10-30
//extern void FM_PNInit_Result(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte CLS_IDLE_av);
extern void FM_PNInit_Result(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte CLS_IDLE_av,sword t1_offset);

extern void Init_EE_SL(void);  //2014-10-4

extern void Save_DT1_whenKeyoff(sword t1_offset);//SCR专用；

extern sbyte vc_tw1_t3fk;           //偏移量2，即T1的偏移量
extern sbyte vc_tw3offset;          //T3零点save the value of	ti shift set by	pc，T3的零点-调试得到//即O值
extern byte nb_keyon_forIni;       // //2008-10-30 ADD
extern byte nb_keyon;              //2009-8-28
//extern void FC_SysInit_whenStop(unsigned long int t_eng_s_whenidle);
extern void FC_ti_set(void);//2009-7-31
extern void FS_PNINIT_FLAG(void);//2009-7-31
//2009-8-6 add
//存储T1的T3反馈结果
extern void FS_tw1_t3fk(byte b)  ;
//extern void Set_Miles_NeedT4Flag(byte NeedT4);
extern sbyte FS_AQOK_FLAG(sword idle_sc,sbyte fmf_cls,sbyte t3_offset,sbyte v_o,byte blnOk); //2010-7-8 add condiciton ok

extern void FC_KeyStatus(void);

//2014-3-29 add in sys_init.h
extern byte Load_Key(word adr); //2014-11-26 更正，frombyte to word
extern void Save_Key(word adr,byte dat);
extern byte blnIdleIni;//2016-7-31 增加，怠速初始化进行中
