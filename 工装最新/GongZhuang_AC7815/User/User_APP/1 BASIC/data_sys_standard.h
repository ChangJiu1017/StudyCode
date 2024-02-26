//==========================================================================================================
//--以下是对系统参数的选型的定义--
//作为编成人员的参考
//---系统参数类别

/*===========================================================================================*/
/*============================喷嘴本构关系定义===============================================*/
/*===========================================================================================*/
#define INJ109_2  52//0x10   //老标准
#define INJ209_2  53//0x20
#define INJ309_2  54//0x30

#define INJ109  49//219 2014-4-25 标准
#define INJ209  50//219 2014-4-25 标准
#define INJ309  51//319 2014-4-25 标准


//2014-8-27
#define INJ109_3  55//0x10  0.6 
#define INJ209_3  56//0x20
#define INJ309_3  57//0x30

#define INJ109_4  58//0x10  0.3 

#define INJ_SCR1  99//2014-10-15 ADD 

//2014-12-5 重新定义喷嘴
#define MFUD1_1 49
#define MFUD1_2 50
#define MFUD1_3 51

#define MFUD3_0 48
#define MFUD3_A 65
#define MFUD3_C 67

#define MFUD6_1 49
#define MFUD6_2 50
#define MFUD6_4 52

#define MFUD4_M10 48//2015-5-18
#define MFUD4_M9  57
#define MFUD4_M8  56

#define MFUD6_SCR 57//2014-12-5


/*===========================================================================================*/
/*============================客户定义=======================================================*/
/*===========================================================================================*/
#define MANUFACTURE_WY  0
#define MANUFACTURE_QJ  1
#define MANUFACTURE_HJ  2//大长江
#define MANUFACTURE_ZS  3//
#define MANUFACTURE_ZY  4//中裕 
#define MANUFACTURE_TE  5
#define MANUFACTURE_LT  6// 连通
#define MANUFACTURE_ML  7// 摩联（白所长）
#define MANUFACTURE_CF  8//  
#define MANUFACTURE_HL  9//

#define MANUFACTURE_LF  10 //A
#define MANUFACTURE_TM  11 // B
#define MANUFACTURE_LIFENG  12 //C
#define MANUFACTURE_XSJ 13     //D
#define MANUFACTURE_LX  14     //E
#define MANUFACTURE_JL  15     //F
#define MANUFACTURE_SL  16     //G
#define MANUFACTURE_QM  17     //H
#define MANUFACTURE_ZX  18     //I
#define MANUFACTURE_HN  19     //J
#define MANUFACTURE_DY  20        //K
#define MANUFACTURE_XY  21           //L
#define MANUFACTURE_WB  22//WY本田（WB）//M
// 23--光阳; 24--慈溪金轮；25_南海轻骑;  26--隆鑫（第二部分） ；27--润通；
#define MANUFACTURE_GY    23    //N
#define MANUFACTURE_GY_SONGYING    23    //N


#define MANUFACTURE_CIJL  24    //O
#define MANUFACTURE_NHQQ    25    //P
#define MANUFACTURE_LX_2ND  26       //Q
#define MANUFACTURE_RT      27          //R
#define MANUFACTURE_BT      28             //S
//#define MANUFACTURE_SF      29//时风        //T
#define MANUFACTURE_T_ZYC       29    //2010-9-30 在用车定义为T系列

#define MANUFACTURE_GZWY    30//2009-3-30 add    //U
#define MANUFACTURE_CG      31//CHANGGE      1:150zh//V
#define MANUFACTURE_HS      32//hengsheng    1:125zh, 2:110zh    //W

//2012-10-29 add
#define MANUFACTURE_GEN_BS  33//X
#define MANUFACTURE_GEN_GENERAC  33//X
                                                                    //X
#define MANUFACTURE_ZJ      34//                                    //Y
#define MANUFACTURE_ZJ_HR      34//                                    //Y
#define MANUFACTURE_ZJ_ZN      34//                               //Y  ZHONGNENG


#define MANUFACTURE_OTHER   35                                     //Z
#define MANUFACTURE_OTHER_YUEJIN  35  //Z
#define MANUFACTURE_OTHER_SF      35//时风   2010-5-13
#define MANUFACTURE_OTHER_DJ      35//时风   2010-5-13
#define MANUFACTURE_OTHER_KT      35//台州凯通，2010-11-24
#define MANUFACTURE_OTHER_YX      35//银翔，2010-11-24
#define MANUFACTURE_OTHER_CT      35//川田，2011-6-1


/*===========================================================================================*/
/*============================程序版本定义===================================================*/
/*===========================================================================================*/
//#define	EASAA	1
//#define	EASAA	2//2.17.2 2016-1-8 准备发GENERAC的程序
//#define	EASAA	3//2.17.3 TPCB, -30
//#define	EASAA	4//2.17.4 纠正TPCB
//#define	EASAA	5//2.17.5 纠正泵油T1

//#define	EASAA	7//2.17.7 增加CAN的版本，可以连接DATAVIEW
//#define	EASAA	8//2.17.8 纠正TW衰减的问题
//#define	EASAA	8//2.17.9 增加对点火线圈的检测，配合ECU硬件
//#define	EASAA	10//2.17.10 更改OS检测的时间
//#define	EASAA	11//2.17.11 纠正TW错误
//#define	EASAA	12//2.17.12 暂时关闭油门全开的自学习
//#define	EASAA	13//2.17.13 暂时关闭CAN
//#define	EASAA	14//2.17.14 增加激活功能
//#define	EASAA	15//2.17.15 压力采样电压条件，增加FUB的厂家信息
//#define	EASAA	16//2.17.16 处理电感线圈烧保险的问题
//#define	EASAA	17//2.17.17 初始化放松
//#define	EASAA	18//2.17.18 解决了因处理烧保险问题，导致不好启动的问题
//#define	EASAA	19//2.17.19 加强了小g的作用
//#define	EASAA	20//2.17.20更改为免激活
#define	EASAA	23//2.17.23 增加UP,IGN策略
#define	EASAB	0
/*
//#define	ECU_RESERVE1	0
#define	ECU_RESERVE1	4   //2015-5-22 电感ECU
//#define	ECU_RESERVE2	  16 //2015-5-22 更改OS采样异常
//#define	ECU_RESERVE2	  17 //2015-6-8 更改
#define	ECU_RESERVE2	  18 //2015-6-13 T3B可以为负值，更改完全使用内部EEPROM
*/

//2015-6-25 与8BIT ECU同步
#define	ECU_RESERVE1	2
//#define	ECU_RESERVE2  10
//#define	ECU_RESERVE2  11     //2.11 加入一键初始化
//#define	ECU_RESERVE2  13     //2.13 ca高限的定义
//#define	ECU_RESERVE2  14     //2.14 发现T30修正偏慢问题之后，改进中
#define	ECU_RESERVE2  17     //2.14 发现T30修正偏慢问题之后，改进中


//--车型----
#define HIDER   1
#define SCOOTER 2
#define CUB     3
#define WILD    4 //2009-11-12 增加的大排量的越野车，200cc及以上

//---一般情况下，编成人员无须对以上配置进行调整


/*===========================================================================================*/
/*============================模式定义=======================================================*/
/*===========================================================================================*/
#define MC_STOP 1

#define MC_ACTUATOR 24

#define MC_DMFC 19

#define MC_DTC 29

#define MC_GENERATOR 10

#define MC_SPEEDCONTROL 26
#define MC_IGNITION 22
#define MC_INI 30
#define MC_LDB 6
#define MC_LDL 7
#define MC_MF 18
#define MC_MI 15
#define MC_MPA 9

#define MC_VMASTER_MAX 11 //2014-11-27 chang from 9 to 11

#define MC_VMASTER_AIRASSIST_DOSING 9
#define MC_VMASTER_DIRECTINJ_DOSING 11

#define MC_VMASTER_FOR_CABNOZZLE 5//2014-10-29标定喷嘴用的REMARK模式，属于VMASTER的模式；

#define MC_WN_TG  2   //目标TG的模式

#define MC_NORMAL 4
#define MC_NOZZLE 8
#define MC_OPENLOOP 3
#define MC_PAIR 11
#define MC_SENSOR 23

#define MC_REMARK5 5

#define MC_U 13
#define MC_MASTER 13

#define MC_REMARK14 14

#define MC_PRONO  31//ECU 编号

#define MC_LOCK 32

//2013-6-6 新老兼容定义
#define MC_SL     MC_LDL
#define MC_LOADB  MC_LDB
#define MC_LAMBDA MC_U//LAMBDA标定时，不使用学习值

//2014-11-10 用于MASTER模式的KESEI
#define KESEI_MASTER_MASTER 0
#define KESEI_MASTER_MDEF_BASE 1
#define KESEI_MASTER_KNH3  2
#define KESEI_MASTER_KAIR  3
#define KESEI_MASTER_MEXH   4
#define KESEI_MASTER_SV    5
#define KESEI_MASTER_DEFMAX 6
#define KESEI_MASTER_AD1     7
#define KESEI_MASTER_AD2     8
#define KESEI_MASTER_PUMP    9

