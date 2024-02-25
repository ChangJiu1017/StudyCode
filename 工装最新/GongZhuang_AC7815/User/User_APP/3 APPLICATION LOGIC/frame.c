/*HEAD
 ***********************************************************************
 * Filename	:frame.c
 * Function	:CAN comunication		
 * User		:xielian
 * Date		:2017.1.17
 * Version	:V8.2.0
 *
 *** History ***
 * V8.1.0   2017.1.17	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.1.17	xielian
 *		1.整合变量发送参数
 *		2.增加Remark和Kesei模式发送，兼容串口通讯模式 
 *		3.调整CAN帧发送周期
 *
 * V8.2.1   2018.9.8	xielian
 *		调整诊断仪读取VIN，CID，ECUNAME的地址
 ***********************************************************************
HEAD END*/
#include "Events_mpi.h"
#include "Service_CAN.h"
#include "public.h"

//#include "can.h"

#include "frame.h"

#include "ad_scr.h"//2015-4-4 

#include "dtc.h"   //20105-6-4
#include "data_inj_source.h"

#include "sci.h"//2015-10-25 add 

#include "ld.h"// 2015-10-26 for SYS_PI
#include "ad.h"//2015-10-26
#include "ign.h"
#include "inj_ctl.h"
#include "sys_init.h"
#include "time_ccl.h"
#include "stepmotor.h"
#include "Status_eng.h"

#include "mc33814.h"
#include "EE.H"

//20201118 chenwei add
//20201109 chenwei add 欧五OS诊断策略相关

extern byte nb_schmax;                     //成功穿越稀到浓的次数
extern byte nb_schmin;                     //成功穿越浓到稀的次数
extern byte t_os_diagperiod;               //OS的诊断周期
extern byte v_OS_leanthres;                //OS正常工作的稀值（类似与常说的300mv）
extern byte v_OS_richthres;                //OS正常工作的浓值（类似与常说的600mv）


extern word sum_tschmax_ms;               //成功穿越稀-浓的累计时间，以ms为单位
extern word sum_tschmin_ms;               //成功穿越浓-稀的累计时间，以ms为单位
extern word sum_tschmax;                  //下面两个应该与上面带ms重复了？？
extern word sum_tschmin;
extern long int v_dt_tschmax_4us;
extern long int v_dt_tschmin_4us;
extern word v_t_schmax_last;            //上一次进入函数Detect_OS_rationality()的时间
extern byte v_dt_schmaxave;
extern byte v_dt_schminave;





//#include "frame.h" //2015-1-5 add   这里 include frame.h时，会出错
/*
volatile STRU_CAN8 _STRU_NOX_OUT   ;
volatile STRU_CAN8 _STRU_NOX_IN    ;

volatile STRU_CAN8 _STRU_ACU_OUT   ;
volatile STRU_CAN8 _STRU_ACU_IN    ;

volatile STRU_CAN8 _STRU_CAN_DEBUG ;//作为调试用

volatile STRU_CAN8 _STRU_ECU_T_N ;//2014-11-8

//2014-12-7
volatile STRU_CAN8 _STRU_ECU_SCR1   ;//-
volatile STRU_CAN8 _STRU_ECU_INCON   ;//-
volatile STRU_CAN8 _STRU_ECU_AMCON   ;//2014
volatile STRU_CAN8 _STRU_ECU_EEC2   ;//2014
volatile STRU_CAN8 _STRU_ECU_EEC1   ;//2014

volatile STRU_CAN8 _STRU_ACU_IN_MGPERS    ;
*/
volatile STRU_CAN8 STRU_CAN8_FRAME[FRAME_NUM];
volatile STRU_FROZEN _FROZEN_FRAME;
volatile FRAME_15765 _FRAME_15765;//2015-9-16

//====================================================
volatile V_STATUS_STR    _V_NOX_STATUS;    //2014-10-28 空气电磁阀状态
volatile V_STATUS_STR    _V_COOLV_STATUS;    //2014-10-28 冷却水阀状态
volatile V_STATUS_STR    _V_TU_STATUS;    //2014-10-28 尿素温度状态
volatile V_STATUS_STR    _V_ADB_STATUS;    //2014-10-28 液位状态
volatile V_STATUS_STR    _V_TEIN_STATUS;    //2014-10-28 进口排气状态
volatile V_STATUS_STR    _V_TEOUT_STATUS;    //2014-10-28 出口排气状态


#define FRAME15765ONLY  false//2015-9-20
//2014-11-17

word v_t_CAN_200ms_4us;

dword v_t_1s_4us;//2015-8-22

dword v_t_4us_PURGING;
dword v_t_4us_PRIMING;

dword v_t_4us_PURGING_valid;//有效的清扫时间2015-7-27
dword v_t_4us_PRIMING_valid;//有效的填充时间

dword v_t_4us_PURGING_whenkeyon;  //2015-1-11钥匙开启时已经清扫的时间；


dword v_t_ACUOut_timeout_4us;
dword v_t_ACUIn_timeout_4us;

dword v_t_Pumpoff_afterPURGING;   //清洗后，泵停止的时间；

dword v_ID_toBeSend;              //等待发送的帧

byte v_vin[17];
byte v_cid[16];
byte v_ecuname[20];

//帧时间控制用
    void WorkFrameOut(void);
    byte tick_10ms_forNOx;  //NOX帧用的时间
    byte tick_10ms_forDEF;  //液位帧用的时间
    byte tick_10ms_forMETER;//仪表用的时间

    byte tick_10ms_forDMX;     //用于DMX的时间控制
    byte tick_10ms_forPacket; //用于包的时间控制

    

    unsigned long int tick_15765Packet_lst;//上一次发送包的时间
    unsigned long int dt_for_15765Packet_4us;//时间间隔

//临时声明
extern word v_Misf_n_EnaEngSpdHi;  //失火检测允许的发动机转速上限 62.5rpm
extern byte v_Misf_n_EnaEngSpdLo;  //失火检测允许的发动机转速下限 15.6rpm
extern sword v_Misf_AgAccnReplVal;//采样段角加速度特征值
extern word v_Misf_MisfThres;//失火阈值
extern byte nb_Misftime_cyc;//一个block中发生失火的次数;
extern byte nb_Misftime_block;//一个array中发生失火的block数
extern word nb_cycnum_cyc;//cycle计数
extern word nb_cycnum_block;//block计数
extern byte v_map_ad;//临时定义
extern word v_Misf_st_MskUnEnaMisf;         //20201019 chenwei 失火状态字
extern word nb_Misfcyc_del;                 //20201019 chenwei 延迟失火检测的循环数
extern word MISF_SPEED;  
extern word MISF_TRANS;
extern word MISF_TMP;
extern word MISF_VBAT;
extern word MISF_ZROTRQ;
extern word MISF_ALT;
extern word MISF_CF;
extern word MISF_YSTDC; 
extern word MISF_RUNTIME;
extern word MISF_HIDLE;
extern word MISF_DTC;
extern word MISF_TPS;

extern byte nb_EropVMisf_cyc;
extern word v_dt_cutign;
extern word v_dt_cutign_lst;
extern word Misf_AgAccnReplVal_CUR;   //20201022 chenwei 转速-特征值系数
extern byte cut_cy;                   //失火计数

//20201127 chenwei add CCPS碳罐电磁阀添加参数
extern word v_t_CCPS_closelast;						
extern byte v_t_CCPS_opennow;			 
//extern byte t_CCPS_period; 			   		   
extern byte V_CCPS_st_MskEna;		 

//20201202 chenwei add SAIC二次补气阀
extern byte v_SAIC_st_UnEnaSAIC;						 			  					  
extern word t_after1stStart_s;

//20210201 zhouxinrui add ITSD进气温度合理性诊断
extern sbyte v_ITSD_ED_Ta_Last;			
extern sbyte v_ITSD_ED_Ta_Diff;		
extern byte nb_ITSD_ED_CycCnt;		
extern byte nb_ITSD_ED_FaultCnt;

extern sbyte V_Ta;
extern sword v_te;
extern unsigned long int v_t_eng_t_s;	
extern word t_EngSoak_s;
extern byte DTC_FORM;

extern sbyte v_ITSD_SD_Ta_MIN;			
extern sbyte v_ITSD_SD_Ta_MAX;			
extern sbyte v_ITSD_SD_Ta_LimDiff;		
extern word v_ITSD_SD_ti_EngWrm;		
extern word v_ITSD_SD_ti_EngStop;		
extern byte V_ITSD_SD_MskEna;			

extern word v_CLTD_HSD_ti_EngStop;
extern sbyte v_CLTD_HSD_IntkTmp;
extern sword v_CLTD_HSD_ClntTmp;
extern byte v_CLTD_HSD_TmpDiff;
extern byte V_CLTD_HSD_MskEna;

extern word v_CLTD_SD_ti_EngStop;
extern word v_CLTD_SD_ti_EngRun;
extern sword v_CLTD_SD_TmpStrt;
extern byte v_CLTD_SD_TmpDiff;
extern byte V_CLTD_SD_MskEna;







    
//2015-8-5
//ECM6,对最大T的定义
//const byte M0_MAXT_ECM6_H=0x21;
//const byte M0_MAXT_ECM6_L=0xFC;//(370+500)*10=8700=0X  //370-256= 114

/*=====================================================================*/
//对CANVIEW的定义
      void Init_CanViewFrame(void);
      byte CheckCanOutTime(void);
      void DiscomposeCmd(void);
      void ParaFrameOut(void);        //向CAN输出PARAMETER
      /*
          关于PARA OUT帧外发的说明
          
          当v_t1ms_CanView_NotOut时间到达INT1MS_CANPARAOUT时，发送对应的的ID_CANPARAOUT的帧
          
      */

      word v_t10ms_CanView_NotOut[NUM_FRAME_PARAOUT];   //上一次的发送到现在的时间

      word v_tick_lstCanout;//上次CAN发送出去的时间；//保证两次CAN发送时间间隔>500US,对于250k的BPS

      const byte INT10MS_CANPARAOUT[NUM_FRAME_PARAOUT]=//;       //两次之间的时间间隔
      {
50	,	//0
1	,	//1
1	,	//2
1	,	//3
50	,	//4
25	,	//5
25	,	//6
50	,	//7
50	,	//8
50	,	//9
1	,	//10
50	,	//11
50	,	//12
50	,	//13
25	,	//14
20	,	//15
40	,	//16
250	,	//17
50	,	//18
125	,	//19
125	,	//20
125	,	//21
125	,	//22
20	,	//23
20	,	//24
5	,	//25
250	,	//26
5	,	//27
5	,	//28
5	,	//29
125	,	//30
40	,	//31
25	,	//32
50	,	//33
125	,	//34
50	,	//35
125	,	//36
50	,	//37
50	,	//38
125	,	//39
250	,	//40
250	,	//41
250	,	//42
250	,	//43
250	,	//44
250	,	//45
250	,	//46
1	,	//47
1	,	//48
1	,	//49
1	,	//50
1	,	//51
1	,	//52
1	,	//53
1   ,   //54 //20201118 add EUROPVOS_1
1   ,   //55 //20201118 add EUROPVOS_2
1   ,   //56 //20201127 add CCPS_1
1   ,   //57 //20201127 add SAIC_1
1   ,   //58 //20210201 add ITSD_1
1   ,   //59 //20210201 add ITSD_2
1   ,   //60 //20210201 add ITSD_3
1   ,   //61 //20210201 add CLTD_1
1   ,   //62 //20210201 add CLTD_2
1   ,   //63 //20210201 add CLTD_3
5	,	//64
      };


      const dword ID_CANPARAOUT[NUM_FRAME_PARAOUT]=  //PARAOUT相关的ID号，用数组下标来查询，与INT10MS_CANPARAOUT对应
      {
FRAME_STATUS	,	//0
FRAME_LD_LDB	,	//1
FRAME_LD_LDL	,	//2
FRAME_LD_MPA	,	//3
FRAME_LD_MTI	,	//4
FRAME_LAMBDA_U	,	//5
FRAME_LAMBDA_UC	,	//6
FRAME_LAMBDA_UC2	,	//7
FRAME_LAMBDA_UP	,	//8
FRAME_LAMBDA_UA	,	//9
FRAME_LAMBDA_UR	,	//10
FRAME_FILM_1	,	//11
FRAME_FILM_2	,	//12
FRAME_FILM_3	,	//13
FRAME_MI	,	//14
FRAME_IGN_1	,	//15
FRAME_IGN_2	,	//16
FRAME_IGN_3	,	//17
FRAME_INJPHASE	,	//18
FRAME_SPEED_1	,	//19
FRAME_SPEED_2	,	//20
FRAME_SPEED_3	,	//21
FRAME_INITION	,	//22
FRAME_NOZZLE1	,	//23
FRAME_NOZZLE2	,	//24
FRAME_TPS1	,	//25
FRAME_TPS2	,	//26
FRAME_N	,	//27
FRAME_OS1	,	//28
FRAME_OS2	,	//29
FRAME_TMP1	,	//30
FRAME_TMP2	,	//31
FRAME_VBAT	,	//32
FRAME_CAS1	,	//33
FRAME_CAS2	,	//34
FRAME_PAIRS1	,	//35
FRAME_PAIRS2	,	//36
FRAME_ETC1	,	//37
FRAME_ETC2	,	//38
FRAME_ACTUATOR	,	//39
FRAME_FAI_DTC	,	//40
OVERTPM_PRO	,	//41
RUNTIME_2	,	//42
RUNTIME_3	,	//43
RUNTIME_4	,	//44
RUNTIME_5	,	//45
RUNTIME_6	,	//46
MISF_1	,	//47
MISF_2	,	//48
MISF_3	,	//49
MISF_4	,	//50
MISF_5	,	//51
MISF_6	,	//52
MISF_7	,	//53
EUROPVOS_1 ,  //54   //20201118 chenwei add
EUROPVOS_2 ,  //55   //20201118 chenwei add
CCPS_1   ,    //56   //20201127 chenwei add
SAIC_1   ,    //57   //20201202 chenwei add
ITSD_1   ,    //58   //20210201 zhouxinrui add
ITSD_2   ,    //59   //20210201 zhouxinrui add
ITSD_3   ,    //60   //20210201 zhouxinrui add
CLTD_1   ,    //61   //20210201 zhouxinrui add
CLTD_2   ,    //62   //20210201 zhouxinrui add
CLTD_3   ,    //63   //20210201 zhouxinrui add
FRAME_FAI_FLASH	,	//64

      };

      byte index_lstCanParaOut;//上一次CAN PARAOUT 的索引

      #define NUM_PARACANOUT_ENBALE 4


      /*
          与ID_CANPARAOUT的对应关系
          v_paraCanOutEnable[0]，开始；
          对于每个
          v_paraCanOutEnable，又从低位开始
          
      */
      byte v_paraCanOutEnable[NUM_PARACANOUT_ENBALE];//定义CAN OUT para是否输出
      
      byte CheckIdxEnable(byte idx);

      word v_add_forDataview;//与DATAVIEW建立连接后的地址，2015-7-12
      
      
      byte v_BAM_for;//多帧包发送的内容
      #define BAM_FOR_NONE  0
      #define BAM_FOR_DM1   1
      
      #define NUM_DTC_OUT  28
      byte b_DTC_out[NUM_DTC_OUT];//用于多包发送的数据的存储
      byte p_DTCout;//DTC out的指针指向
      
      void SaveToDTCout(byte blnFromst,byte blnSave,byte * pd);

      /*
    	const M1_VIN[17]={	102,	97,	105,	121,	117,	110,	110,	101,	105,	45,	121,	99,	0,	0,	0,	0,	0,	};			
    	const M1_CID[16]={	67,	73,	68,	79,	70,	70,	65,	73,	83,	67,	82,	0,	0,	0,	0,	0,	};				
    	const M1_ECUNAME[20]={	85,	68,	77,	48,	0x2D,	65,	73,	83,	67,	82,	69,	67,	85,	0,	0,	0,	0,	0,	0,	0,	};
      */
//--------------------------------------------------------------------------------
//--关于SPN的定义
/*
      注意：数量需要与 frame.h中的NUM_DTC_SCR对应    
*/
const word M1_SPN[]={	
168	,
168	,
3362	,
3362	,
3362	,
3360	,
3363	,
3472	,
3472	,
3472	,
4335	,
3360	,
3361	,
3361	,
3361	,
639	,
1761	,
1761	,
1761	,
1761	,
1761	,
1761	,
3031	,
3031	,
3031	,
3031	,
3363	,
4360	,
4360	,
4363	,
4363	,
3226	,
3226	,
3226	,
3226	,
4225	,
4225	,
4090	,
639	,
639	,
108	,
171	,
110	,
4091	,
4092	,
4093	,
4094	,
4095	,
4096	,
4091	,
4092	,
4093	,
4094	,
4095	,
4096	,
4090	,
};	

const byte M1_FMI[]={	
3	,
4	,
12	,
7	,
14	,
12	,
7	,
3	,
4	,
5	,
19	,
14	,
12	,
14	,
7	,
11	,
17	,
18	,
1	,
2	,
3	,
4	,
2	,
3	,
4	,
17	,
5	,
3	,
4	,
3	,
4	,
12	,
1	,
2	,
5	,
16	,
19	,
16	,
19	,
11	,
12	,
12	,
12	,
16	,
16	,
16	,
16	,
16	,
16	,
19	,
19	,
19	,
19	,
19	,
19	,
19	,
};	
const byte M1_DTCNO[NUM_DTC_SCR]={		
0	,	
1	,	
2	,	
3	,	
4	,	
5	,	
6	,	
7	,	
8	,	
9	,	
10	,	
11	,	
12	,	
13	,	
14	,	
15	,	
16	,	
17	,	
18	,	
19	,	
20	,	
21	,	
22	,	
23	,	
24	,	
25	,	
26	,	
27	,	
28	,	
29	,	
30	,	
31	,	
32	,	
33	,	
34	,	
35	,	
36	,	
37	,
38	,
39	,
40	,
41	,
42	,
43	,
44	,	
45	,	//20210308 chenwei add		
46	,
47	,
48	,
49	,	//20211005 chenwei add

		
};		

const byte SCRDTC_ST[]={		
36 	,	//VF_VOLT_U_SPN:Y/N
36 	,	//VF_VOLT_L_SPN:Y/N
168 	,	//VF_NOTBECAL_SPN:N
180 	,	//VF_PUMP_E_SPN:Y/N
180 	,	//VF_PUMP_M_SPN:Y/N
176 	,	//VF_ACU_SPN:N
164 	,	//VF_FROZEN_SPN:Y/N
180 	,	//VF_AIRV_POWER_SPN:Y/N
180 	,	//VF_AIRV_GND_SPN:Y/N
180 	,	//VF_AIRV_OPEN_SPN:Y/N
182 	,	//VF_PRESSURE_SPN:Y
176 	,	//VF_ACUEE_SPN:N
164 	,	//VF_PUMP2_E_SPN:Y/N
164 	,	//VF_PUMP2_M_SPN:Y/N
164 	,	//VF_PUMP2_DEICING_SPN:Y/N
160 	,	//VF_NOLINK_DCU_SPN:N
32 	,	//VF_ADB_LB_SPN:N
168 	,	//VF_ADB_LLB_SPN:N
204 	,	//VF_ADB_LLLB_SPN:Y/N
34 	,	//VF_ADB_ERR_SPN:Y50
34 	,	//VF_ADB_UU_SPN:Y50
34 	,	//VF_ADB_UL_SPN:Y50
32 	,	//VF_TU_ERR_SPN:N
32 	,	//VF_TU_UU_SPN:N
32 	,	//VF_TU_UL_SPN:N
4 	,	//VF_TU_DEICING_SPN:Y/N
32 	,	//VF_COOLV_ERR_SPN:N
178 	,	//VF_TEIN_U_SPN:Y50
178 	,	//VF_TEIN_L_SPN:Y50
178 	,	//VF_TEOUT_U_SPN:Y50
178 	,	//VF_TEOUT_L_SPN:Y50
162 	,	//VF_NOX_HEATER_SPN:Y50
162 	,	//VF_NOX_U_SPN:Y50
162 	,	//VF_NOX_VALUE_SPN:Y50
162 	,	//VF_NOX_LINK_SPN:Y50
176 	,	//VF_EXH_U_SPN:N
212 	,	//VF_EXH_UU_SPN:Y/N
176 	,	//VF_CATALYST_SPN:N
160 	,	//VF_NOLINK_ECU_SPN:N
160 	,	//VF_NOLINK_ACU_SPN:N
32 	,	//VF_NOPAMB_SPN:N
32 	,	//VF_NOTAMB_SPN:N
0 	,	//VF_NOET1_SPN:N
176 	,	//:N
176 	,	//:N
176 	,	//:N
176 	,	//:N
176 	,	//:N
176 	,	//:N
212 	,	//:Y/N
212 	,	//:Y/N
212 	,	//:Y/N
212 	,	//:Y/N
212 	,	//:Y/N
212 	,	//:Y/N
214 	,	//VF_CATALYST_SPN:Y
214 	,	//:Y/N chenwei add 20210226
214 	,	//:Y/N chenwei add 20210226
214 	,	//:Y/N chenwei add 20210226

};	

const word P_CODE[NUM_DTC_SCR]={			
		0x0107	,//byte0-0
		0x0108	,//byte0-1
		0x0110	,//byte0-2
		0x0111	,//byte0-3
		0x0112	,//byte0-4
		0x0113	,//byte0-5
		0x0114	,//byte0-6
		0x0115	,//byte0-7
		0x0116	,//byte1-0
		0x0117	,//byte1-1
		0x0118	,//byte1-2
		0x0119	,//byte1-3
		0x0122	,//byte1-4
		0x0123	,//byte1-5
		0x0122	,//byte1-6
		0x0123	,//byte1-7
		0x0124	,//byte2-0
		0x0132	,//byte2-1
		0x0131	,//byte2-2
		0x0132	,//byte2-3
		0x0133	,//byte2-4
		0x0134	,//byte2-5
		0x0135	,//byte2-6
		0x0201	,//0x0261	,//byte2-7	//20211005 CR环松，根据环松要求调整
		0x0231	,//byte3-0
		0x0300	,//byte3-1
		0x0351	,//0x2300	,//byte3-2	//20211005 CR环松，根据环松要求调整
		0x0335	,//byte3-3
		0x0336	,//byte3-4
		0x0371	,//byte3-5
		0x0506	,//byte3-6
		0x0510	,//byte3-7
		0x0562	,//byte4-0
		0x0563	,//byte4-1
		0x0602	,//byte4-2
		0x14AC	,//byte4-3	//
		0x14AD	,//byte4-4
		0x1501	,//byte4-5
		0x0230	,//0x0627	,//byte4-6	//20211005 CR环松，根据环松要求调整
		0x0628	,//byte4-7
		0x0232	,//0x0629	,//byte5-0	//20211005 CR环松，根据环松要求调整
		0x0509	,//byte5-1
		0x0508	,//byte5-2
		0x0200	,//byte5-3
		0x0262	,//byte5-4
		0x0505  ,//byte5-5 20210308 chenwei add
		0x0650  ,//byte5-6 20210308 chenwei add	
		0x0031  ,//byte5-7 20210308 chenwei add
		0x0032  ,//byte6-0 20210308 chenwei add
		0x0337  ,//byte6-1 20211004 chenwei add		无曲轴信号
		};	
	

const byte DTC_CON_2[]={			
	0	,
	0	,
	0	,
	3	,
	0	,
	1	,
	1	,
	0	,
	0	,
	0	,
	0	,
	3	,
	0	,
	0	,
	0	,
	0	,
	0	,
	1	,
	4	,
	0	,
	1	,
	1	,
	0	,
	1	,
	1	,
	0	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	3	,
	7	,
	3	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	1	,
	5	,
	5	,
	5	,
	5	,
	5	,
	5	,
	7	,
	5	,
		};	
	
	

const byte MIL_FOR_DTC[]={	
9	,//VF_PAMB_LB_SPN
9	,//VF_PAMB_UB_SPN
10	,//VF_TAMB_C_SPN
10	,//VF_TAMB_B_SPN
10	,//VF_TAMB_SHORT_SPN
10	,//VF_TAMB_OPEN_SPN
10	,//VF_TAMB_WIRE_SPN
7	,//VF_TES_WIRE_SPN
7	,//VF_TES_B_SPN
7	,//VF_TES_LB_SPN
7	,//VF_TES_UB_SPN
7	,//VF_TES_C_SPN
5	,//VF_TPS_LB_SPN
5	,//VF_TPS_UB_SPN
5	,//VF_TPS_OPEN_SPN
5	,//VF_TPS_B_SPN
5	,//VF_TPS_C_SPN
6	,//VF_OS_WIRE_SPN
6	,//VF_OS_SHORT_SPN
6	,//VF_OS_OPEN_SPN
6	,//VF_OS_SLOW_SPN
6	,//VF_OS_C_SPN
6	,//VF_OS_HEATER_SPN
4	,//VF_PN_WIRE_SPN,VF_OPEN_33814INJ1CTL,VF_OPEN_INJCTL
4	,//VF_PN_T3FK_SPN
15	,//VF_MISSFIRE_SPN  VF_Misf_st_EmiDmgConf   
10	,//VF_IGN_COIL_SPN
0	,//VF_CAS_NO_SPN
8	,//VF_CAS_EMI_SPN
8	,//VF_IGN_SPN
19	,//VF_IDLE_LOW_SPN
5	,//VF_IDLE_NO_SPN
2	,//VF_VBAT_LB_SPN
2	,//VF_VBAT_UB_SPN
3	,//VF_ECU_NONUM_SPN
3	,//VF_ECU_EEPROM_SPN
11	,//VF_ETC_SPN
12	,//VF_LOAD_SPN
14	,//VF_OPEN_33814PUMPCTL
14	,//VF_SHORT_33814PUMPCTL
14	,//VF_OC_33814PUMPCTL
11	,//VF_IACV_OPEN
11	,//VF_IACV_SHORT
4	,//VF_SHORT_33814INJ1CTL
4	,//VF_OC_33814INJ1CTL
5	,//VF_IACS_ERR 20210308 chenwei add	
13	,//VF_MIL_OCOROPEN 20210308 chenwei add 
6	,//VF_O2H_OPEN 20210308 chenwei add 
6	,//VF_O2H_OC 20210308 chenwei add 
3	,//VF_INI_SYSINI 20211005 chenwei add

	
};	
	
																													
        //byte AnalysisSPN(byte blnCur);//2015-6-4
        byte AnalysisSPN(byte blnCur,byte blnGetOnly1,byte blnFromSt);
        	
        //byte Get_Num_SPNNG(void); 2015-9-16
        byte Get_Num_SPNNG(byte blnCur);
        word Cal_Num_pack(void);

        //word v_spn;
        unsigned long int v_spn;//2015-8-29
        word v_pcode;//2015-9-16 add pcode
        
        byte v_fmi;
        
        //以下4个状态
        #define IDX_CUR 0//当前值
        #define IDX_MAX 1//当前故障中的最大值
        #define IDX_MAX_AFTER_PWRON 2  //开钥匙后的最大值
        byte v_st_mil;
        byte v_st_scr;
        byte v_st_miloff_con;
        byte v_pumpoff;
        byte st_scr_tmp;
        
        byte v_st_mil_disp;//2016-2-24 应该显示的MIL
          
        
        byte v_n_SPN_ng;//故障的数量
        byte v_NO_SpnPack_CanOut;//从CAN发出的SPN的PACK的号；
        
        byte v_id_cur;
        sbyte CurPgnInx;//当前显示
        
        
        #define NUM_BYTPGN  20

        byte bytPgn[NUM_BYTPGN];//需要作为DM1后面帧输出的
        //byte GetCurPgnbyte(byte blnFromst,byte blnCur);//2015-9-16
        byte GetCurPgnbyte(byte blnFromst,byte blnCur,byte blnPgn);//2015-9-16 

        word v_n_SPN_pack;//spn故障的PACK数量
        word v_n_SPN_pack_disp;  //当前显示的位置
 
 
        byte v_nb_DTC_history;//历史故障的数量；
        byte v_nb_DTC_history_milon;//历史中需要亮灯的故障的数量；
        
 
        void DiagBy15765(void); //2015-9-16
 
        byte SaveFrame_whenKeyoff(void);//2015-9-17
        byte Search_pos_to_saveDTC(byte id);//

        byte Read_Num_FrozenFrame(byte blnChkMILLamp) ;
        
        //void ClearDTCHistory(void);        

        void Save_FrozenFrame(void) ;//2015-11-28 保存冻结帧

        #define DTC_HISTORY_BLOCK_ALL    0
        #define DTC_HISTORY_BLOCK_CLR    1
        #define DTC_HISTORY_BLOCK_NOCLR  2
        
        byte v_pos_dtc;//=0;
        byte v_pos_curid;//=0;

        byte GetDTCStauts(void);

        volatile STRU_FROZEN_SHORT _TMP_DTC[NUM_DTC_HISTORY];
        byte Opr_Tmp_DTC(byte action,byte pos);//1:APPEND,2:DELETE,3:MODIFY
        
        byte v_nb_dtc_noneclr;//当前HISTORY LIST中，不可擦除的故障的数量
        byte v_nb_dtc_clr;  //当前HISTORY LIST中，可擦除的故障的数量
        byte v_ub_tmp_dtc;//DTC 数组的已有的数据的个数；

        void Clr_DTC_list(void); 
        
        byte v_DC; //2016-2-24 临时
        byte v_OBDC;

byte deFrame[8];

		//20210224 chenwei add欧五综合功能相关
		byte test_2021022401;
		word test_2021022402;
		byte test_2021022601;
		byte v_bpos_misfDTC;  //20210226 chenwei add 失火历史故障码P0300的起始存储ID
		byte v_bpos_OSSDRLDTC;	//20210227 chenwei add OSSD-LR历史故障码P014C的起始存储ID
		byte v_bpos_OSSDLRDTC;	//20210227 chenwei add OSSD-RL历史故障码P014D的起始存储ID
		
		void Clear_EURV_DrivingCycle_count(void);//20210218 chenwei add清除历史故障吗是将EEPROM中存储驾驶循环的数写0
		void Clear_SPN7_PCODE(void);//20210224 chenwei 清除SPN7涉及的故障码
		void Clear_PCODE_Specific(byte bposDTC,byte blnReset,word adr);//20210226 chenwei add 清除该位置的特定故障码
		#define PCODE_OSSD_RL P_CODE[56]   //20210226 chenwei add 失火故障码P014C
		#define PCODE_OSSD_LR P_CODE[57]   //20210226 chenwei add 失火故障码P014D
		#define PCODE_MISF P_CODE[58]   //20210226 chenwei add 失火故障码P0300
        
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
void Frame_action_whenKeyoff(void)
{
    SaveFrame_whenKeyoff();    
}


/*
    ACU/DCU数据帧的初始化
*/
void FI_Frame_afterReset(void)
{
    byte i;
    
    byte j;

    
    for(j=0;j<FRAME_NUM;j++){
      
        for(i=0;i<=7;i++){
            STRU_CAN8_FRAME[j].Byte[i]=0;
        }
        STRU_CAN8_FRAME[j].COMMON.TimeOut_Limit=10;
        STRU_CAN8_FRAME[j].COMMON.TimeOut_200ms=100;//初始设置为TIME OUT状态
        STRU_CAN8_FRAME[j].COMMON.VF_RECNEW_FRAME=false;//初始设置为帧未接收的状态
    }


    //开关打开时，发送一次地址声明 SCR Address Claim
    //(void) CAN_SendFrameExt(0x98EEFF3D, 0, 8, &qdata[0]);  

    //V_NOX_STATUS=false;
		VF_TOBEON_NOX=0;
		VF_VOLTINRANGE_NOX=0;
		VF_TMPINRANGE_NOX=0;
		VF_NOXVALID_NOX=0;
		VF_O2VALID_NOX=0;
		VF_HEATEROK_NOX=0;
		VF_NOXOK_NOX=0;
		VF_O2_NOX=0;
		
    //V_COOLV_STATUS=false;
		VF_TOBEON_COOLV=0;
		VF_ISOPEN_COOLV=0;
		VF_ISSHORT_COOLV=0;
		VF_NOXON_ATSLAVE_NOX=0;
		VF_NOXOFF_ATSLAVE_NOX=0;
		VF_LOW_TSCR=0;
    
    //V_TU_STATUS=false;
		VF_UB_TU=0;
		VF_LB_TU=0;
		VF_DECING_TU=0;
		VF_UB_VOLT=0;
		VF_LB_VOLT=0;
		VF_UERR_TU=0;
		VF_LERR_TU=0;
		
		//V_ADB_STATUS=false;
		VF_LB_ADB=0;
		VF_LERR_ADB=0;
		VF_UERR_ADB=0;
		VF_LLB_ADB=0;
		VF_LLLB_ADB=0;
		VF_UL_ADB=0;
		
		//V_TEIN_STATUS=false;
		VF_UB_TEIN=0;
		VF_LB_TEIN=0;
		VF_ERR_TEIN=0;
		VF_IS_TSCR_SET_TEIN=0;
		VF_UB_TAMB_OUTER=0;
		VF_LB_TAMB_OUTER=0;
		VF_ERR_TAMB_OUTER=0;
		
    
		//V_TEOUT_STATUS=false;
		VF_UB_TEOUT=0;
		VF_LB_TEOUT=0;
		VF_ERR_TEOUT=0;
		VF_UB_TAMB_INNER=0;
		VF_LB_TAMB_INNER=0;
		VF_ERR_TAMB_INNER=0;
		VF_NOTAMB_FROMCAN=0;
		VF_NOPAMB_FROMCAN=0;
    
    //2015-6-13
        VF_NOXON_ATSLAVE_NOX=false;
        VF_NOXOFF_ATSLAVE_NOX=false;
    
    //时间
    v_t_CAN_200ms_4us=0;
    
    v_t_1s_4us=0;//2015-8-22

    v_t_4us_PURGING=0;//>=T_PURGING_4US  
    
    v_t_4us_PRIMING=0;//>=T_PRIMING_4US
    v_t_4us_PURGING_valid=0;
    v_t_4us_PRIMING_valid=0;
    
    if(VF_POR_HARD)v_t_4us_PURGING_whenkeyon=0;//如果ECU刚上电，则清零，2015-1-11
    
    v_t_ACUOut_timeout_4us=T_ACUOUT_TIMEOUT_4US;
    v_t_ACUIn_timeout_4us=T_ACUIN_TIMEOUT_4US;
    
    Init_CanViewFrame();//2015-6-3
    
    //2016-2-24
    v_DC=0;
    v_OBDC=0;
    
    //2016-2-28
    for(i=0;i<=7;i++){
        deFrame[i]=0;
    }
    

}

word v_tl_can;

//
/*
    功能：解析数据帧的内容
    来自CAN先上的数据帧，
    不包含与开发系统的那个部分的
    
*/
void DisCompFrame(void) 
{
      dword Qtmp;
      word w;

    	VF_NOLINK_FROMECU_SCRDTC=false;    	           // _V_SCRDTC3_STATUS.Bits.b7             //没有来自ECU的连接
    	VF_NOLINK_FROMACU_SCRDTC=false;    	           // _V_SCRDTC3_STATUS.Bits.b5             //泵
    	VF_NOLINK_FROMDCU_SCRDTC=false;    	           // _V_SCRDTC3_STATUS.Bits.b4             //泵
      VF_NOLINK_FROMNOX_SCRDTC=false;

        VF_NOLINK_FROMEEC1_SCRDTC=false;
        VF_NOLINK_FROMEEC2_SCRDTC=false;
        VF_NOLINK_FROMAMCON_SCRDTC=false;
        VF_NOLINK_FROMINCON_SCRDTC=false;
        VF_NOLINK_FROMSCR1_SCRDTC=false;
        
        VF_NOLINK_T_SCRDTC=false;
        VF_NOLINK_N_SCRDTC=false;
        
        VF_NOLINK_FROMDCU_MGPERS_SCRDTC=false;
        
        VF_NOLINK_FROMPT_SCRDTC=false;///2015-4-4


      if(!Is_can_ok())
      {
          if(!VF_1ST_INI_SYSINI)
          {
              CAN_Init();//开启CAN    
          }
      } 
      else
      {   //CAN可用
          if(VF_1ST_INI_SYSINI) 
          {
              Close_Can();
              
          } 
          else
          {
               //2016-6-6
              //====================================================================================
              if(VF_KEYON_HARD)//2015-9-21 ADD THIS CONDICTION
              {
                    w=T_MAINTMR;
                        
              //来自FAIDATAVIEW的数据处理
                    //if(KESEI==1||KESEI>5)
                        DiscomposeCmd();
                    
                    
              //向FAIDATAVIEW输出参数：
                    //if(KESEI==2||KESEI>5)
                    if(!VF_ISCHARGE_IGN)//2016-3-5
                        ParaFrameOut();
                    
              //一些工作帧的发送      
                    //if(KESEI==3||KESEI>5)
                        WorkFrameOut();
                    
                    
              //2015-9-16 增加诊断仪部分的交互
                    //if(KESEI==4||KESEI>5)
                        DiagBy15765(); 
                    
                    w=T_MAINTMR-w;
                    
                    if(v_tl_can<w)v_tl_can=w;//2016-2-20

                  //2015-11-30 诊断DCU状态      
                    GetDTCStauts();     


              }
          }
      }

}

/*
    按照15765协议实现的诊断
*/

//byte n_nb_frame_forRqst;
//byte v_Rqst_for;

void Set_dt_for_15765Packet_4us(byte ms) 
{
    if(ms>127)ms=127;
    dt_for_15765Packet_4us=250*(unsigned long int)ms;
    
}


void DiagBy15765(void) 
{
     byte i;
     
     byte b;
     byte b1;
     word w;
     unsigned long int lt;
     byte blnCur;
     byte DTC_TYPE;
	 

	for(i=0;i<17;i++){
        v_vin[i]=*(byte *)(BYPTR_M1_VIN+i);
    }
	
	for(i=0;i<16;i++){
        v_cid[i]=*(byte *)(BYPTR_M1_CID+i);
    }
	
	for(i=0;i<20;i++){
        v_ecuname[i]=*(byte *)(BYPTR_M1_ECUNAME+i);
    }

     
      
     //_FRAME_15765.COMMON.IS_SENDING=false;
     //=============================================
     //对接收到的新
     //if(v_mode_DCU==DCU_MASTER)
     {
         if(_FRAME_15765.COMMON.IS_NEW)
         {
              _FRAME_15765.COMMON.IS_NEW=false;
              for(i=0;i<=7;i++)
              {
                  _STRU_PARAOUT.Byte[i]= 0;
              }
              
              
              tick_15765Packet_lst=T_MAINTMR;
              
              Set_dt_for_15765Packet_4us(0);
              
                    //for(i=0;i<8;i++)_FRAME_15765.Byte[i]=Data[i];
              
              //2015-12-17 把以下两行挪走，注意流控帧是不需要回复的，如果在流控帧时v_n_SPN_pack_disp=0，则会导致后续回复帧出错
              //_FRAME_15765.COMMON.IS_SENDING=true;
              //v_n_SPN_pack_disp=0;
              
              
              //第一个帧的回复
              switch(_FRAME_15765.COMMON.N_PCITYPE)
              {
                  case N_PCITYPE_SF:
                      //2015-12-17 MOVE HERE
                      _FRAME_15765.COMMON.IS_SENDING=true;
                      v_n_SPN_pack_disp=0;
                      
                      
                      //switch(_FRAME_15765.Byte[1]) 
                      {
                          if(_FRAME_15765.Byte[1]==ASK_01){
                            
                          //case ASK_01:
                              
                              if(_FRAME_15765.Byte[2]==0) //请求第0组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x00;  
                                  
                                  //注意，这里是低位在先
                                  /*
                                  _STRU_PARAOUT.Byte[3]=0x98;  //
                                  _STRU_PARAOUT.Byte[4]=0x10;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x13;  //2015-11-26 from 11 to 13
                                  SCR xitong */
                                  
                                  //PI系统如下
                                  _STRU_PARAOUT.Byte[3]=0x98;  //
                                  _STRU_PARAOUT.Byte[4]=0x10;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x1B;  //
                                 
                                  
                                  //后面几个都是0 
                              } 
                              if(_FRAME_15765.Byte[2]==0x01)
                              {    //排放相关故障个数，MIL灯状态
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x01;  
                                  
                                  _STRU_PARAOUT.Byte[3]=Get_Num_SPNNG(DTC_TYPE_CONFIRM); //MIL状态+故障个数 
                                  if(GetMIL())_STRU_PARAOUT.Byte[3]+=128;
                                                                 //v_n_SPN_ng=Get_Num_SPNNG(true);

                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x04;  //
                                  //_STRU_PARAOUT.Byte[5]=0x00;  //
                                  //_STRU_PARAOUT.Byte[5]=0x01;  //2015-11-26 from 0 to 1
                                  //_STRU_PARAOUT.Byte[6]=0x01;  //
                                  //2016-3-5 PI
                                  _STRU_PARAOUT.Byte[5]=0x00;  //2016-3-5 PI
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              } 
                              else if(_FRAME_15765.Byte[2]==0x02)
                              {   //请求故障码,验证的故障代码(P code, 16bits )
                                  _STRU_PARAOUT.Byte[0]=0x05;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x02;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  //_STRU_PARAOUT.Byte[4]=0x20;  //第一个8个，一共4*8=32个
                                  //_STRU_PARAOUT.Byte[5]=0x40;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=GetCurPgnbyte(true,DTC_TYPE_PENDING_NOW,false);  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=GetCurPgnbyte(false,DTC_TYPE_PENDING_NOW,false);  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              else if(_FRAME_15765.Byte[2]==0x03) //2015-11-24 for ecu
                              {
                                  _STRU_PARAOUT.Byte[0]=0x04;//
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x03;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x0;  //
                                  //后面几个都是0 
                              }
                              
                              else if(_FRAME_15765.Byte[2]==0x04) {
                                  //发动机负荷% (8bits, scaling:0.4, offset:0)
                                  _STRU_PARAOUT.Byte[0]=0x03;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x04;  
                                  
                                  _STRU_PARAOUT.Byte[4]=0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=v_T_percent;//v_TPS;//v_Pedal_percentM2D5;//0x80;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                                  
                              }
                              else if(_FRAME_15765.Byte[2]==0x05) {
                                  //冷却液温度℃(8bits, scaling:1, offset:-40)
                                  _STRU_PARAOUT.Byte[0]=0x03;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x05;  
                                  
                                  _STRU_PARAOUT.Byte[3]=v_te+40;//v_Eng_CoolTmp+40;//0x8e;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }
                              else if(_FRAME_15765.Byte[2]==0x0c) {
                                  //发动机转速rpm(16bits, scaling:0.25, offset:0)
                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x0c;  
                                  
                                  //2016-7-26
                                  /*
                                  _STRU_PARAOUT.Byte[3]=(v_rev_15d6rpm/4)>>8;//(byte)((v_n_rpm*4)>>8);//0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=(byte)(v_rev_15d6rpm/4);//(byte)(v_n_rpm*4);//0x00;  //第一个8个，一共4*8=32个
                                  */
                                  //2016-7-26 change to   ASK_01,0x0c service
                                  w=v_rev_15d6rpm;
                                  w=w*64;//2016-7-26
                                  
                                  
                                  _STRU_PARAOUT.Byte[3]=(w)>>8;//(byte)((v_n_rpm*4)>>8);//0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=(byte)(w);//(byte)(v_n_rpm*4);//0x00;  //第一个8个，一共4*8=32个
                                  
                                  
                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }

                              else if(_FRAME_15765.Byte[2]==0x13) {
                                  //未知，填充ELM用
                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x13;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }
                              
                              else if(_FRAME_15765.Byte[2]==0x1c)
                              {    //执行的认证标准
                                  _STRU_PARAOUT.Byte[0]=0x03;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x1c;  
                                  
                                  //_STRU_PARAOUT.Byte[3]=0x06; //
                                  _STRU_PARAOUT.Byte[3]=0x04; //2016-3-5 ADD PI
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              else if(_FRAME_15765.Byte[2]==0x1D)
                              {    //氧传感器
                                
                                  _STRU_PARAOUT.Byte[0]=0x03;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x1D;  
                                  
                                  //_STRU_PARAOUT.Byte[3]=0x06; //
                                  _STRU_PARAOUT.Byte[3]=0x01; //2016-3-5 ADD PI
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              else if(_FRAME_15765.Byte[2]==0x1f)//2015-11-28
                              {    //v_KEY_t_s,min
                                  _STRU_PARAOUT.Byte[0]=0x04;//
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x1f;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0;//HBYTE(v_ENG_t_s_cur/60); //
                                  _STRU_PARAOUT.Byte[4]=0;//LBYTE(v_ENG_t_s_cur/60);  //
                                  
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }

                              else if(_FRAME_15765.Byte[2]==0x20) //请求第X组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x20;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=0x01;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x20;  //第一个8个，一共4*8=32个
                                  //_STRU_PARAOUT.Byte[6]=0x11;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x01;  //2016-3-5 PI
                                  //后面几个都是0 
                              }
                              else if(_FRAME_15765.Byte[2]==0x30) {
                                  //DTC清除后的WUC                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[0]=0x03;//
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x30;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0;//v_WUC;  // 需要是清除DTC之后后，待改正
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }
                              else if(_FRAME_15765.Byte[2]==0x33) {
                                  //请求 大气压力（绝对压力） 1Byte，1 kPa/bit
                                  _STRU_PARAOUT.Byte[0]=0x03;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x33;  
                                  
                                  _STRU_PARAOUT.Byte[3]=v_p_kpa;//v_BaroPre_D5Kpa/2;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x00;  
                                  _STRU_PARAOUT.Byte[5]=0x00;  
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }
                              else if(_FRAME_15765.Byte[2]==0x3c) {
                                  //scr温度SCR温度， 2Byte ，0.1℃/bit-40
                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x3c;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0;//HBYTE((v_TSCR+40)*10);  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0;//LBYTE((v_TSCR+40)*10);  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }

                              else if(_FRAME_15765.Byte[2]==0x40) //请求第X组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x40;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=0x84;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=0x06;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //第一个8个，一共4*8=32个
                                  //后面几个都是0 
                              }
                              else if(_FRAME_15765.Byte[2]==0x41) //请求第X组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x41;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=0x00;  
                                  _STRU_PARAOUT.Byte[4]=0x04;  
                                  _STRU_PARAOUT.Byte[5]=0x01;  
                                  _STRU_PARAOUT.Byte[6]=0x00;  
                                  _STRU_PARAOUT.Byte[7]=0x00;  
                                  //后面几个都是0 
                              }
                              else if(_FRAME_15765.Byte[2]==0x46) {
                                  //环境温度：(8bits, scaling:1, offset:-40)
                                  _STRU_PARAOUT.Byte[0]=0x03;
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x46;  
                                  _STRU_PARAOUT.Byte[3]=V_Ta+40;//v_AirTmp_Deg+40;  //
                                  
                                  _STRU_PARAOUT.Byte[4]=0x00;  //
                                  
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }

                              else if(_FRAME_15765.Byte[2]==0x4d)
                              {
                                  //MIL 亮之后发动机运行时间min（16bits，scaling:1,offset:0）
                                  _STRU_PARAOUT.Byte[0]=0x04;//
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x4d;  
                                  
                                  //w=v_ENG_t_s_MILOn/60;
                                  w=0;
                                  _STRU_PARAOUT.Byte[3]=HBYTE(w);  
                                  _STRU_PARAOUT.Byte[4]=LBYTE(w);  

                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                                  
                              } 
                              else if(_FRAME_15765.Byte[2]==0x4e)
                              {
                                  //故障码清除后的运行时间min（16bits，scaling:1,offset:0）
                                  _STRU_PARAOUT.Byte[0]=0x04;//
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  _STRU_PARAOUT.Byte[2]=0x4e;  
                                  
                                  //w=v_ENG_t_s_afterDTCclr/60;
                                  w=0;//
                                  
                                  _STRU_PARAOUT.Byte[3]=HBYTE(w);  
                                  _STRU_PARAOUT.Byte[4]=LBYTE(w);  

                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              

                              else if(_FRAME_15765.Byte[2]==0x60) //请求第X组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x60;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x0;  //第一个8个，一共4*8=32个
                                  //后面几个都是0 
                              }
                              else if(_FRAME_15765.Byte[2]==0x80) //请求第X组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_01;
                                  
                                  _STRU_PARAOUT.Byte[2]=0x80;  
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[3]=0x28;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[4]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x0;  //第一个8个，一共4*8=32个
                                  //后面几个都是0 
                              }
                              
                              v_n_SPN_pack=1;        
                          //break;
                          } else if(_FRAME_15765.Byte[1]==ASK_02){
                          //case ASK_02:
                              if(_FRAME_15765.Byte[2]==0) //请求第0组
                              {
                                  _STRU_PARAOUT.Byte[0]=0x07;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x00;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0xd8;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[5]=0x10;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[6]=0x00;  //第一个8个，一共4*8=32个
                                  _STRU_PARAOUT.Byte[7]=0x00;  //第一个8个，一共4*8=32个
                                  //后面几个都是0 
                                  // n_nb_frame_forRqst=1;
                              } else if(_FRAME_15765.Byte[2]==0x01)
                              {    //排放相关故障个数，MIL灯状态
                                  _STRU_PARAOUT.Byte[0]=0x06;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x01;  
                                  
                                  _STRU_PARAOUT.Byte[3]=Get_Num_SPNNG(DTC_TYPE_PENDING); //MIL状态+故障个数 
                                  if(GetMIL())_STRU_PARAOUT.Byte[3]+=128;
                                                                 //v_n_SPN_ng=Get_Num_SPNNG(true);

                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=0x04;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x01;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              } 
                              else if(_FRAME_15765.Byte[2]==0x02)
                              {   //请求故障码,验证的故障代码(P code, 16bits )
                                  _STRU_PARAOUT.Byte[0]=0x05;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x02;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //第一个8个，一共4*8=32个
                                  
                                  //注意，这里是低位在先
                                  //_STRU_PARAOUT.Byte[4]=0x20;  //第一个8个，一共4*8=32个
                                  //_STRU_PARAOUT.Byte[5]=0x40;  //第一个8个，一共4*8=32个
                                  //_STRU_PARAOUT.Byte[4]=GetCurPgnbyte(true,DTC_TYPE_CONFIRM,false);  //
                                  //_STRU_PARAOUT.Byte[5]=GetCurPgnbyte(false,DTC_TYPE_CONFIRM,false);  //
                                  
                                  //2016-3-5 导致冻结帧的故障PCODE，对应的SCR程序可能也要调整
                                  if(_FROZEN_FRAME.COMMON.FAI_ID_FROZEN==INVALID_FAI_ID) {
                                      _STRU_PARAOUT.Byte[4]=0;//GetCurPgnbyte(true,DTC_TYPE_CONFIRM,false);  //
                                      _STRU_PARAOUT.Byte[5]=0;//GetCurPgnbyte(false,DTC_TYPE_CONFIRM,false);  //
                                  } 
                                  else
                                  {
                                      _STRU_PARAOUT.Byte[4]=(byte)((P_CODE[_FROZEN_FRAME.COMMON.FAI_ID_FROZEN])>>8);//GetCurPgnbyte(true,DTC_TYPE_CONFIRM,false);  //
                                      _STRU_PARAOUT.Byte[5]=(byte)(P_CODE[_FROZEN_FRAME.COMMON.FAI_ID_FROZEN]);//GetCurPgnbyte(false,DTC_TYPE_CONFIRM,false);  //
                                  }
                                 
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              else if(_FRAME_15765.Byte[2]==0x04) {
                                  //发动机负荷% (8bits, scaling:1, offset:0)
                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x04;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=_FROZEN_FRAME.COMMON.T_2D5;//0x80;
                                  //_STRU_PARAOUT.Byte[4]=0x80;  //
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                                  
                              }
                              else if(_FRAME_15765.Byte[2]==0x05) {
                                  //冷却液温度℃(8bits, scaling:1, offset:-40)
                                  _STRU_PARAOUT.Byte[0]=0x04;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x05;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=_FROZEN_FRAME.COMMON.ET+40;//
                                  _STRU_PARAOUT.Byte[5]=0x00;  //
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              
                              }
                              else if(_FRAME_15765.Byte[2]==0x0c) {
                                  //发动机转速rpm(16bits, scaling:0.25, offset:0)
                                  _STRU_PARAOUT.Byte[0]=0x05;//按一组 pid（32个）来考虑
                                  _STRU_PARAOUT.Byte[1]=RQST_02;
                                  _STRU_PARAOUT.Byte[2]=0x0c;  
                                  
                                  _STRU_PARAOUT.Byte[3]=0x00;  //
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[4]=(byte)((_FROZEN_FRAME.COMMON.N*4)>>8);//
                                  
                                  //注意，这里是低位在先
                                  _STRU_PARAOUT.Byte[5]=(byte)(_FROZEN_FRAME.COMMON.N*4);//
                                  _STRU_PARAOUT.Byte[6]=0x00;  //
                                  _STRU_PARAOUT.Byte[7]=0x00;  //
                              }
                              
                              
                              v_n_SPN_pack=1;        
                          //break;
                          } else if(
                                _FRAME_15765.Byte[1]==ASK_03
                                ||_FRAME_15765.Byte[1]==ASK_07
                                ||_FRAME_15765.Byte[1]==ASK_0A
                                )
                                {
                          
                          //case ASK_03:
                              //2015-11-28 ASK_03需要查看不可清除故障
                          
                          //case ASK_07:
                          //case ASK_0A:
                                                    
                          

                              if(_FRAME_15765.Byte[1]==ASK_03)
                              {
                                  //blnCur=false;
                                  DTC_TYPE=DTC_TYPE_CONFIRM;
                              }
                              else if(_FRAME_15765.Byte[1]==ASK_07)
                              {
                                  //blnCur=true;
                                  DTC_TYPE=DTC_TYPE_PENDING_NOW;//DTC_TYPE_PENDING;
                              } 
                              else
                              {
                                  DTC_TYPE=DTC_TYPE_PENDING_NOW;//DTC_TYPE_KEYON;  2016-2-29 change to keyon
                              }
                              
                              //请求被确认的DTC P码
                              //
                              //    1）先要确认PCODE的数量；
                              //    2）计算帧数；
                              //    3）
                              
                              v_n_SPN_ng=Get_Num_SPNNG(DTC_TYPE);
                      
                              if(v_n_SPN_ng<=2)
                              {   //单帧
                                  v_n_SPN_pack=1; 
                                  
                              } 
                              else
                              {
                                  b=(v_n_SPN_ng-2)*2;
                                  v_n_SPN_pack=1;
                                  
                                  while(b>0)
                                  {
                                      v_n_SPN_pack++;
                                      if(b>=7)b-=7; 
                                      else b=0;   
                                  }
                                  
                              }

                              //GetCurPgnbyte(byte blnFromst,byte blnCur,byte blnPgn)
                              if(v_n_SPN_pack<=1)
                              {
                                  _STRU_PARAOUT.Byte[0]=2+v_n_SPN_ng*2;
                                  _STRU_PARAOUT.Byte[1]=_FRAME_15765.Byte[1]+0x40;
                                  _STRU_PARAOUT.Byte[2]=v_n_SPN_ng;//故障数量
                                  
                                  _STRU_PARAOUT.Byte[3]=GetCurPgnbyte(true,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[4]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[5]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[6]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[7]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  

                              } 
                              else
                              {
                                  _STRU_PARAOUT.Byte[0]=0x10;//N_PCITYPE_FF*16;
                                  _STRU_PARAOUT.Byte[1]=2+v_n_SPN_ng*2;
                                  
                                  _STRU_PARAOUT.Byte[2]=_FRAME_15765.Byte[1]+0x40;
                                  
                                  _STRU_PARAOUT.Byte[3]=v_n_SPN_ng;//故障数量

                                  _STRU_PARAOUT.Byte[4]=GetCurPgnbyte(true,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[5]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[6]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  _STRU_PARAOUT.Byte[7]=GetCurPgnbyte(false,DTC_TYPE,false);
                                  
                                  
                                  //SaveToDTCout(byte blnFromst,byte blnSave,byte * pd);
                                  //把后续帧的数据存入
                                  b=(v_n_SPN_pack-1)*7;
                                  b1=GetCurPgnbyte(false,DTC_TYPE,false);
                                  SaveToDTCout(true,true,&b1);//从头开始
                                  
                                  for(i=1;i<b;i++)
                                  {
                                      b1=GetCurPgnbyte(false,DTC_TYPE,false);
                                      SaveToDTCout(false,true,&b1);
                                  }
                              }
                          //break;
                          } else if(
                                _FRAME_15765.Byte[1]==ASK_04
                                )
                                {
                          
                          //case ASK_04:
                              //清除故障内存
                              v_n_SPN_pack=1;        
                              _STRU_PARAOUT.Byte[0]=1;
                              //正响应
                              _STRU_PARAOUT.Byte[1]=RQST_04;
                              
                              //这里需要增加故障码清除的动作
                              ClearDTCHistory(false);
                              ClearSpnStatus(DTC_TYPE_CONFIRM); //2016-2-23 这个临时删除
                              
                              //2016-3-2 add 
                              ClearSpnStatus(DTC_TYPE_PENDING); //2016-3-2
                              ClearSpnStatus(DTC_TYPE_KEYON); //2016-3-2
                              
                          //break;
                         } else if(
                                _FRAME_15765.Byte[1]==ASK_09
                                )
                                {

                          //case ASK_09:
                              
                              //switch(_FRAME_15765.Byte[2])
                              
                                    //2015-9-19 设置成跟艾可兰的一样
                                if(_FRAME_15765.Byte[2]== 0x00){
                                  
                                    //询问的是支持哪个PID
                                    v_n_SPN_pack=1;        

                                    _STRU_PARAOUT.Byte[0]=6;
                                    _STRU_PARAOUT.Byte[1]=RQST_09;
                                    _STRU_PARAOUT.Byte[2]=0;

                                    _STRU_PARAOUT.Byte[3]=0x54;
                                    _STRU_PARAOUT.Byte[4]=0x40;
                                    

                                } else if(_FRAME_15765.Byte[2]==0x02){
                                  

                                    //车辆识别号，17个ASC码
                                    _STRU_PARAOUT.Byte[0]=0x10;
                                    _STRU_PARAOUT.Byte[1]=0x14;
                                    _STRU_PARAOUT.Byte[2]=RQST_09;
                                    _STRU_PARAOUT.Byte[3]=0x02; //正反应
                                    _STRU_PARAOUT.Byte[4]=0x01; //数据项目编号
                                
                                                                    
                                    //接下来就是车辆识别号
                                    _STRU_PARAOUT.Byte[5]=v_vin[0]; //
                                    _STRU_PARAOUT.Byte[6]=v_vin[1]; //
                                    _STRU_PARAOUT.Byte[7]=v_vin[2]; //

                                    v_n_SPN_pack=3;

                                    b1=v_vin[3];
                                    SaveToDTCout(true,true,&b1);//从头开始
                                    
                                    for(i=4;i<17;i++) 
                                    {
                                        b1=v_vin[i];
                                        SaveToDTCout(false,true,&b1);//从头开始
                                    }
                                    
                                }
                                 else if(
                                      _FRAME_15765.Byte[2]==0x04 
                                      ){
                                  
                                    //CALIBRATION IDENTIFICATION，16个ASC码
                                    _STRU_PARAOUT.Byte[0]=0x10;
                                    _STRU_PARAOUT.Byte[1]=0x13;
                                    _STRU_PARAOUT.Byte[2]=RQST_09;
                                    _STRU_PARAOUT.Byte[3]=0x04; //正反应
                                    _STRU_PARAOUT.Byte[4]=0x01; //数据项目编号
                                
                                    //接下来就是车辆识别号
                                    _STRU_PARAOUT.Byte[5]=v_cid[0]; //
                                    _STRU_PARAOUT.Byte[6]=v_cid[1]; //
                                    _STRU_PARAOUT.Byte[7]=v_cid[2]; //

                                    v_n_SPN_pack=3;
                                    
                                    b1=v_cid[3];
                                    SaveToDTCout(true,true,&b1);//从头开始
                                    
                                    for(i=4;i<16;i++) 
                                    {
                                        b1=v_cid[i];
                                        SaveToDTCout(false,true,&b1);//从头开始
                                    }
                                } 
                                 else if(
                                      _FRAME_15765.Byte[2]==0x06 
                                      ){   //CVN, 4 BYTES,2015-11-28
                                  
                                    _STRU_PARAOUT.Byte[0]=0x07;
                                    _STRU_PARAOUT.Byte[1]=RQST_09;
                                    _STRU_PARAOUT.Byte[2]=0X06;
                                    _STRU_PARAOUT.Byte[3]=0x01; //正反应
                                    _STRU_PARAOUT.Byte[4]=0x01; //数据项目编号
                                
                                    //接下来就是车辆识别号
                                    _STRU_PARAOUT.Byte[5]=0x02; //
                                    _STRU_PARAOUT.Byte[6]=0x03; //
                                    _STRU_PARAOUT.Byte[7]=0x04; //

                                    v_n_SPN_pack=1;
                                } 
                                else if(_FRAME_15765.Byte[2]==0x0A){
                                  
                                    //发动机名，20个ASC码
                                    _STRU_PARAOUT.Byte[0]=0x10;
                                    _STRU_PARAOUT.Byte[1]=0x17;
                                    _STRU_PARAOUT.Byte[2]=RQST_09;
                                    _STRU_PARAOUT.Byte[3]=0x0a; //正反应
                                    _STRU_PARAOUT.Byte[4]=0x01; //数据项目编号
                                
                                    //接下来就是车辆识别号
                                    _STRU_PARAOUT.Byte[5]=v_ecuname[0]; //
                                    _STRU_PARAOUT.Byte[6]=v_ecuname[1]; //
                                    _STRU_PARAOUT.Byte[7]=v_ecuname[2]; //

                                    v_n_SPN_pack=4;
                                
                                    b1=v_ecuname[3];
                                    SaveToDTCout(true,true,&b1);//从头开始
                                    
                                    for(i=4;i<20;i++) 
                                    {
                                        b1=v_ecuname[i];
                                        SaveToDTCout(false,true,&b1);//从头开始
                                    }
                                
                                } else {
                                    v_n_SPN_pack=0;
                                
                                }
                          
                          //break;
                                }
                          /*
                          default:
                              v_n_SPN_pack=0;    
                          break;
                          */
                          else v_n_SPN_pack=0;
                      }
                      
                      
                          
                  break;
                  
                  case N_PCITYPE_FF:
                      
                  break;
                  
                  case N_PCITYPE_CF:
                      
                  break;
                  
                  case N_PCITYPE_FC:
                      //2015-9-20 接收到流控制帧
                      b=(_FRAME_15765.Byte[0]&0x0f); //流控制
                      
                      if(b==0)
                      {   //继续连续发送
                          
                          Set_dt_for_15765Packet_4us(_FRAME_15765.Byte[2]); 
                          
                          if(_FRAME_15765.Byte[1]==0)VF_FS_BS_CONTINUE_CANCTL=true;
                          else VF_FS_BS_CONTINUE_CANCTL=false;
                             
                      } 
                      else if(b==1)
                      {
                          //暂停发送
                          Set_dt_for_15765Packet_4us(50);    
                      } 
                      else if(b==2)
                      {
                          //停止发送
                          v_n_SPN_pack=0;
                          _FRAME_15765.COMMON.IS_SENDING=false;
                      }
                      
                      
                      
                  break;
                
              }
              
              
         }
         
     }
         
     //=============================================
     //对诊断仪请求的回复：
         if(_FRAME_15765.COMMON.IS_SENDING)
         {
             if(v_n_SPN_pack_disp<v_n_SPN_pack)
             {

                 if((word)(T_MAINTMR-dt_for_15765Packet_4us)>tick_15765Packet_lst||dt_for_15765Packet_4us==0)
                 {
                      v_n_SPN_pack_disp++;
                      
                      if(v_n_SPN_pack_disp>=2)
                      {
                          //第二以上的帧数据在此赋值
                          //if(v_Rqst_for==ASK_03)
                          {
                              //_STRU_PARAOUT.Byte[0]=0x20+v_n_SPN_pack_disp-2;
                              _STRU_PARAOUT.Byte[0]=0x20+v_n_SPN_pack_disp-1;//2015-9-19 根据艾可兰的纠正
                              
                              for(i=1;i<=7;i++){
                                
                                  if(v_n_SPN_pack_disp==2&&i==1)
                                  {
                                      SaveToDTCout(true,false,&b1);
                                  } 
                                  else
                                  {
                                      SaveToDTCout(false,false,&b1);
                                  }
                                  _STRU_PARAOUT.Byte[i]=b1;
                              }
                          }
                      }
                      

                      //if(v_Type_Master==MASTER_ECM)
                          CAN_SendFrameExt( ID_15765_RQST_ST,0,8,(byte *)_STRU_PARAOUT.Byte);//2015-12-15
                     /*
                      else 
                          CAN_SendFrameExt( ID_15765_RQST|CAN_EXTENDED_FRAME_ID,0,8,&_STRU_PARAOUT.Byte);
                       */
                      tick_15765Packet_lst=T_MAINTMR;
                      
                      if(!VF_FS_BS_CONTINUE_CANCTL)
                      {     //如果不连续发送，则设置长时间
                            Set_dt_for_15765Packet_4us(4);    
                      }
                      else 
                      {     //如果是连续发送
                            if(dt_for_15765Packet_4us==0)
                            {   //如果没有设置连续发送的时间，则将时间设置为5MS
                                Set_dt_for_15765Packet_4us(5);
                            }
                      }
                         
                 }
             } 

             if(v_n_SPN_pack_disp>=v_n_SPN_pack)
             {
                  _FRAME_15765.COMMON.IS_SENDING=false;
             }
         }
     
     

}
/*
  ---------------------------------------
    模式          |CAN输出      |CAN输入  
  ---------------------------------------
    DCU_QTEST     |无           |无
    DCU_SLAVE     |ACU_OUT      |ACU_IN
    DCU_VMASTER   |ACU_IN       |ACU_OUT
    DCU_MASTER    |ACU_IN       |ACU_OUT
  ---------------------------------------
*/


void Set_CabIndex(byte n)
{
    //v_Cab_Index=n;    
}



/*
    初始化帧的发送频次
*/
void Init_CanViewFrame(void)
{
    byte i;
    
    index_lstCanParaOut=0;
    
    
    for(i=0;i<NUM_FRAME_PARAOUT;i++)
    {
          
        v_t10ms_CanView_NotOut[i]=i*2;
    
    }
    
    //一开始为禁止输出的，需要靠CAN发来的数据触发
    for(i=0;i<NUM_PARACANOUT_ENBALE;i++)
    {
        v_paraCanOutEnable[i]=0xff;//暂时都设置为开启状态
    }
    
    v_NO_SpnPack_CanOut=0;
    
    for(i=0;i<=2;i++)
    {
      
        v_st_mil=0;
        v_st_scr=0;
        v_st_miloff_con=0;
        v_pumpoff=0;
        
        v_st_mil_disp=0;
    }


    VF_CALPARA_REQUEST_CANCTL=true;
    VF_DTC_REQUEST_CANCTL=true;
    VF_PNPARA_REQUEST_CANCTL=true;
    
    VF_FLASHLINK_DATAVIEW=false;//2015-7-12
    v_add_forDataview=0;
    
    VF_BY_BYTE_DATAVIEW=true;
    VF_BY_1UNIT_DATAVIEW=true;
    
    VF_LINK_DATAVIEW=false;

    VF_TPCM_BAM_SENT_CANCTL=false;//2015-8-29
    VF_DIAG_RQST_CANCTL=false;

      v_BAM_for=BAM_FOR_NONE;
      
    
    //VF_CALPARA_AUTOOUT_SYS=true;
    VF_CALPARA_AUTOOUT_SYS=false;
      
    //顺序上，这句话要在这里靠后一点  
      v_ub_tmp_dtc=0; //2016-3-3 v_ub_tmp_dtc需要Read_Num_FrozenFrame之前，因为Read_Num_FrozenFrame里面需要加载故障码，用到v_ub_tmp_dtc
      Read_Num_FrozenFrame(true);

    //2016-2-23 
        v_pos_dtc=0;
        v_pos_curid=0;
        
      //  V_CTLBYDTC_STATUS=0;
        
    
}



/*
    PARA OUT的CAN的时间计时
    10ms调用一次
*/
void Accu_CanParaTime_Per10ms(void)
{
    byte i;
    
    for(i=0;i<NUM_FRAME_PARAOUT;i++) 
    {
        if(v_t10ms_CanView_NotOut[i]<255)
        v_t10ms_CanView_NotOut[i]++;    
    }
    
    //2015-8-28
    tick_10ms_forNOx++;
    tick_10ms_forDEF++;
    tick_10ms_forMETER++;
    
    tick_10ms_forDMX++;
    tick_10ms_forPacket++;
    
    //t_lst_tick_can1++;
            
}


/*
    每次进来只发送一个帧,发送后记住这里的位置，下次进来后再从这个位置开始
    
    本函数主要针对标定及诊断帧，发送与否受OBD开关或者CAL开关限制
    
*/


void ParaFrameOut(void)
{
    byte i;
    
    if(CheckCanOutTime())
    {
        
        //deFrame[2]++;
        for(i=0;i<NUM_FRAME_PARAOUT;i++)
        {
            index_lstCanParaOut++;
            
            if(index_lstCanParaOut>=NUM_FRAME_PARAOUT){
                index_lstCanParaOut=0;
            }
            
            //deFrame[3]++;
            
            //如果这个帧被使能了
            //if(CheckIdxEnable(index_lstCanParaOut)&&(VF_LINK_DATAVIEW||VF_CALPARA_AUTOOUT_SYS))//2015-7-13 ADD //2015-7-30 add VF_CALPARA_AUTOOUT_SYS
            if(CheckIdxEnable(index_lstCanParaOut))//2015-7-17  ADD  临时
            {
            
                //deFrame[4]++;
                //如果时间到了
                if(v_t10ms_CanView_NotOut[index_lstCanParaOut]>=INT10MS_CANPARAOUT[index_lstCanParaOut]){
                    //时间到，发送一帧
                    //准备帧数据
                    
                    //deFrame[5]++;
                    if(ComposeParaFrame(ID_CANPARAOUT[index_lstCanParaOut]))
                    {   // 如果需要这个帧外发
                      
                    
                        //deFrame[6]++;
                       if(VF_LINK_DATAVIEW||VF_CALPARA_AUTOOUT_SYS) //2015-7-30把条件移动到这里//2016-2-20 DELETE
                       //2016-3-5 把这个条件恢复回来
                        //发送
                        {
                            //deFrame[7]++;
                            CAN_SendFrameExt(ID_CANPARAOUT[index_lstCanParaOut]|CAN_EXTENDED_FRAME_ID,0,8,(byte *)_STRU_PARAOUT.Byte);
                        }
                        //CAN已发数据的时间清0
                        ResetCanTime();

                        //这组参数的时间清0
                        v_t10ms_CanView_NotOut[index_lstCanParaOut]=0;
                        
                        
                        break;
                    }
                }
            } 
            else
            {   //2015-7-30 ADD
                //在未与DCU保持连接时，由于需要处理OBD的工作，因此，还需要ComposeParaFrame起作用
                
            }
        
        }
    }
    
}



/*
    本函数主要负责发送不受标定及诊断条件限制的帧
    2015-8-28
*/
void WorkFrameOut(void) 
{
    word w;
    byte i;
    
    //2015-9-19 用于测试，这4出的CAN发送暂时取消
   
    //PACKECT
    //PACKECT传输的内容都是在FRAME_TPCM_BAM时刻算好的
    if(tick_10ms_forPacket>=5)
    {
        
          if(v_BAM_for==BAM_FOR_NONE)VF_TPCM_BAM_SENT_CANCTL=false;
        
          if(VF_MASTEROBD_SYS&&VF_TPCM_BAM_SENT_CANCTL) 
          {   //如果头部已经发送
                  
                  if(v_BAM_for==BAM_FOR_DM1){
                  
                      v_n_SPN_pack_disp++;
                      //第一个包
                      if(v_n_SPN_pack_disp==1)
                      {
                      //
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.P01=1;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PROTECT_LAMP_STATUS=DM1_LAMP_OFF;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.AMBER_WARNING_LAMP_STATUS =DM1_LAMP_OFF;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.RED_STOP_LAMP_STATUS =DM1_LAMP_OFF;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.MIL_LAMP_STATUS =DM1_LAMP_OFF;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.FLASHLAMPSTATUS=0;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PGNL=GetCurPgnbyte(true,true,true);//(byte)v_spn;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PGNH =GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PGNHL=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PGNHH=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_1ST.PGN_NXT=GetCurPgnbyte(false,true,true);
                      } 
                      else if(v_n_SPN_pack_disp<=v_n_SPN_pack)
                      {
                          
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.P0X=v_n_SPN_pack_disp;
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX1=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX2=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX3=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX4=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX5=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX6=GetCurPgnbyte(false,true,true);
                            _STRU_PARAOUT.TPCM_BAM_PACKECT_X.PGNX7=GetCurPgnbyte(false,true,true);
                          
                      } 
                      else
                      {   //包结束
                            VF_TPCM_BAM_SENT_CANCTL=false;
                          
                      }
                  }

                  CAN_SendFrameExt(FRAME_TPCM_BAM_PACKECT|CAN_EXTENDED_FRAME_ID,0,8,(byte *)_STRU_PARAOUT.Byte);
          }
    }
    
    
    if(tick_10ms_forMETER>20)
    {
        
        
        _STRU_PARAOUT.Byte[0]= v_tl_can>>8;
        _STRU_PARAOUT.Byte[1]= v_tl_can;
        _STRU_PARAOUT.Byte[2]=P_33814IGN1_ST;
        for(i=2;i<=7;i++){
            _STRU_PARAOUT.Byte[i]=deFrame[i];
        }
				
//				_STRU_PARAOUT.Byte[4] = InterCHV[2];
//				_STRU_PARAOUT.Byte[5] = (InterCHV[2]>>8);
//        _STRU_PARAOUT.Byte[6] = v_prd_cas;
//        _STRU_PARAOUT.Byte[7] = (v_prd_cas>>8);
        CAN_SendFrameExt(0x18ff0001ul|CAN_EXTENDED_FRAME_ID,0,8,(byte *)_STRU_PARAOUT.Byte);
        
        v_tl_can=0;
        tick_10ms_forMETER=0;
    }
    
}

//复位CAN发送时间
void ResetCanTime(void)
{
    v_tick_lstCanout=T_MAINTMR;    
}

/*
    判断两次CAN发送的时间间隔是否够了
*/
byte CheckCanOutTime(void)
{
    word w;
    
    w=T_MAINTMR-v_tick_lstCanout;
    
    //return w>125;
    return w>1250;
    
}

/*
    PARA CAN是否发出，
    需要通过v_paraCanOutEnable使能    
*/      
byte CheckIdxEnable(byte idx)
{
    byte i1;
    byte i2; 
    
    byte i3;     
    byte i;
      
    i1=idx/8;
    
    i2=idx&0x7;
    
    i3=1;
    
    for(i=0;i<i2;i++)
    {
        i3=i3*2;
    }
    
    /*
    if(
        (ID_CANPARAOUT[idx]==FRAME_FAI_FLASH)
        &&!VF_FLASHLINK_DATAVIEW
        )return 0;
    */
    if(
        ((ID_CANPARAOUT[idx]==FRAME_FAI_FLASH)&&!VF_FLASHLINK_DATAVIEW)
        ||
        ((ID_CANPARAOUT[idx]!=FRAME_FAI_FLASH)&&VF_FLASHLINK_DATAVIEW)
        )return 0;
    
    return 1;
    //return v_paraCanOutEnable[i1]&i3;
            
}


/*
    解析来自开发系统的命令

*/
void DiscomposeCmd(void)
{
    dword cmd_ID;
    byte cmd_h;
    dword times;
    byte cmd_l;
    word w;
    byte b;
    
    byte bsw;

    word ADD_ROM;
    byte ADD_PAGE;
    
      if(_STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME)
      {
          _STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME=false;
          VF_LINK_DATAVIEW=true;//2015-7-13
          
          //等待接收下个命令
          /*
          cmd_ID=
              (((dword)_STRU_CMDIN.CMD_IN.ID_HH<<24)&0xff000000u)+
              (((dword)_STRU_CMDIN.CMD_IN.ID_HL<<16)&0x00ff0000u)+
              (((dword)_STRU_CMDIN.CMD_IN.ID_H<<8)&0x0000ff00u)+
              (dword)_STRU_CMDIN.CMD_IN.ID_L;
              
          */
          times=
              (((dword)_STRU_CMDIN.CMD_TEST.TEST_TIMES_HH<<24)&0xff000000u)+
              (((dword)_STRU_CMDIN.CMD_TEST.TEST_TIMES_HL<<16)&0x00ff0000u)+
              (((dword)_STRU_CMDIN.CMD_TEST.TEST_TIMES_H<<8)&0x0000ff00u)+
                (dword)_STRU_CMDIN.CMD_TEST.TEST_TIMES_L;
              
          
          cmd_h=_STRU_CMDIN.CMD_TEST.CMD;
          
          cmd_h=(cmd_h>>4)&0x0f;
          cmd_l=(_STRU_CMDIN.CMD_TEST.CMD&0x0f);
          

          /*
              不能有：_CASE_DIRECT_BYTE 
          */
          bsw=_STRU_CMDIN.CMD_TEST.CMD;
          
         // deFrame[2]++;
          //switch(bsw)
          {
                      
                      if(bsw==0x00){
                        //停止VMASTER
                          cmdAdjO--;
                          //deFrame[3]++;
                          //break;
                      } else if(bsw== 0x01){
                        
                          
                          if(_STRU_CMDIN.CMD_IN.BYTE1==0x02) 
                          {
                              b=_STRU_CMDIN.CMD_IN.BYTE2;
                          
                          
                              if(b>=1&&b<=39){
                                
                                  switch (_STRU_CMDIN.CMD_IN.BYTE3) 
                                  {
                                    case 0x00:
                                        Adj_para(b,0);           
                                    break;
                                    case 0x11:
                                        Adj_para(b,1);
                                        if(b==1){ //2015-10-30
                                            if(VF_E0_TPS)
                                            {
                                                v_FMF_SCI_idle+=2;//v_FMF_SCI_Nidle=v_FMF_SCI_idle; 
                                            } 
                                            else
                                            {
                                                v_FMF_SCI_Nidle+=2;
                                            }

                                        }
                                    break;
                                    case 0x22:
                                        Adj_para(b,-1);
                                        if(b==1){ //2015-10-30
                                            if(VF_E0_TPS)
                                            {
                                                v_FMF_SCI_idle-=2;//v_FMF_SCI_Nidle=v_FMF_SCI_idle; 
                                            } 
                                            else
                                            {
                                                v_FMF_SCI_Nidle-=2;
                                            }

                                        }
                                    break;
                                    
                                  }
                              }
                          }
                            
                      }
                      
                     
                      //2015-10-12
                      else if(bsw== 0x50){
                        
                          if(_STRU_CMDIN.CMD_IN.BYTE1==0x01){
                              if(_STRU_CMDIN.CMD_IN.BYTE2==0x11)VF_NO_TRANS_EN_SYS=false;
                              else if(_STRU_CMDIN.CMD_IN.BYTE2==0x00)VF_NO_TRANS_EN_SYS=true;    
                          }
                          
                          if(_STRU_CMDIN.CMD_IN.BYTE1==0x02){
                              if(_STRU_CMDIN.CMD_IN.BYTE2==0x11)VF_NO_NH3_EN_SYS=false;
                              else if(_STRU_CMDIN.CMD_IN.BYTE2==0x00)VF_NO_NH3_EN_SYS=true;    
                          }
                          
                          //2015-10-15 使能标定开关
                          if(_STRU_CMDIN.CMD_IN.BYTE1==0x03){
                              if(_STRU_CMDIN.CMD_IN.BYTE2==0x11)VF_CALPARA_AUTOOUT_SYS=true;
                              else if(_STRU_CMDIN.CMD_IN.BYTE2==0x00)VF_CALPARA_AUTOOUT_SYS=false;    
                          }
						  
						  //2017-2-17 增加标定模式，兼容串口通讯模式
						  if(_STRU_CMDIN.CMD_IN.BYTE1=0xA0)
						  {						  
							  v_PCSET_SCIL=_STRU_CMDIN.CMD_IN.BYTE2;	//2017-2-17 Remark
							  v_PCKESE_SCI=_STRU_CMDIN.CMD_IN.BYTE3;		//2017-2-17 KESEI							  
						  }
                      
                          
                      }
                      
                      //2015-7-12
                      else if(bsw== 0xa0){
                          //deFrame[4]++;
                        
                          //建立连接
                          VF_FLASHLINK_DATAVIEW=true;
                      } else if(bsw== 0xa1){
                        
                          //取消连接
                          VF_FLASHLINK_DATAVIEW=false;
                      } else if(bsw== 0xa2){
                        
                         // deFrame[5]++;

                                                
                          b=(_STRU_CMDIN.CMD_FLASH.CMD2>>4)&0x0f;
                          
                          if(b==0x0a)VF_BY_BYTE_DATAVIEW=true; //按byte 写入
                          if(b==0x0b)VF_BY_BYTE_DATAVIEW=false;//按word 写入

                          //移动指针
                          if(b==0x0d)
                          {   //2015-10-16接收长整型，dword
                              VF_BY_1UNIT_DATAVIEW=false;            
                          } else
                          {
                              VF_BY_1UNIT_DATAVIEW=true;
                          }
                          
                          b=_STRU_CMDIN.CMD_FLASH.CMD2&0x0f;
                          
                          //2015-7-22 张本西要求
                          if(b==0x05){
                              //2015-10-16 移动指针后都按dword
                              VF_BY_1UNIT_DATAVIEW=false;
                              VF_BY_BYTE_DATAVIEW=false;
                          }
                          
                          if(
                              //b==0x0a||b==0x0B||b==0x05
                              b==0x0a||b==0x05 //2015-10-16 取消||b==0x0B
                          ) 
                          {
                            
                              v_add_forDataview
                                  =
                                    (word)_STRU_CMDIN.CMD_FLASH.ADD_H*256
                                    +
                                    _STRU_CMDIN.CMD_FLASH.ADD_L ;
                                    
                              w=(v_add_forDataview>>15)&0x0001;
                              
                              if(w)VF_LINKTOEE_DATAVIEW=false;
                              else VF_LINKTOEE_DATAVIEW=true;
                              

                              if(!VF_LINKTOEE_DATAVIEW)
                              {   //FLASH的地址
                                  ADD_ROM=v_add_forDataview&0x03ff;
                                  ADD_PAGE=((v_add_forDataview)>>10)&0x1f; //,用于ADD与实际FLASH地址的映射

                                  w=DADD_ST_MAPINEE0+(word)ADD_PAGE*0x200+ADD_ROM;
                                                                
                                  //要写入FALSH的内容，其实也是写入EERPOM的，只是需要偏移DADD_ST_MAPINEE0
                                  //w=DADD_ST_MAPINEE0;
                              }
                              else
                              {   //eeprom的地址
                                  
                                  //写入地址,w在这里是基础地址
                                  w=v_add_forDataview&0x7fff;
                              }
                              
                              if(b==0x0a)//||b==0x0b
                              {
                                  
                                  //ClearRTPara(true,w);//2015-10-21
                                  
                                  Save_Data_EE(w, _STRU_CMDIN.CMD_FLASH.DATA1_L);
                                   
                                  if(!VF_BY_BYTE_DATAVIEW) 
                                  {
                                       Save_Data_EE(w+1, _STRU_CMDIN.CMD_FLASH.DATA1_H);
                                  }
                               
                                  //if(b==0x0b)
                                  {
                                      //2015-7-16 更正
                                      if(!VF_BY_1UNIT_DATAVIEW)//操作两个位置 
                                      {
                                          {
                                              Save_Data_EE(w+2, _STRU_CMDIN.CMD_FLASH.DATA2_L);
                                              Save_Data_EE(w+3, _STRU_CMDIN.CMD_FLASH.DATA2_H);
                                          }
                                      }
                                  }
                           
                              }
                             
                                  
                          }
                      
                      }

          }
          
      }
      
      //2015-7-13
      if(_STRU_CMDIN.CMD_IN.TimeOut_200ms>_STRU_CMDIN.CMD_IN.TimeOut_Limit) {
          VF_LINK_DATAVIEW=false;
      } 
    
}


/*
    定义外发帧
    
    根据ID
    这里只管准备数据，不管发送频次


        		 case	FRAME_PARA_CAL	:																			
        		 case	FRAME_PARA_PUMP	:																			
        		 case	FRAME_WORK1_DOSER	:																			
        		 case	FRAME_WORK2_DOSER	:																			
        		 case	FRAME_WORK3_DOSER	:																			
        		 case	FRAME_SENSOR1	:																			
        		 case	FRAME_SENSOR2	:																			
        		 case	FRAME_ENGINE	:																			
        		 case	FRAME_LINK_TIMEOUT	:																			
        		 case	FRAME_PARA1_MASTER	:																			
        		 case	FRAME_PARA2_MASTER	:																			
        		 case	FRAME_PARA3_MASTER	:																			
        		 case	FRAME_FAI_DTC	:																			

    2015-7-21 增加为

             #define	FRAME_CMD_TEST	0x18EF3DFA
             #define	FRAME_PARA_CAL	0x18FF403D
             #define	FRAME_PARA_CAL2	0x18FF413D
             #define	FRAME_PARA_PUMP	0x18FF423D
             #define	FRAME_PARA_CAL3	0x18FF433D
             #define	FRAME_PARA_CAL4	0x18FF443D
             #define	FRAME_PARA_CAL5	0x18FF453D
             #define	FRAME_PARA_CAL6	0x18FF463D
             #define	FRAME_PARA_CAL7	0x18FF473D
             #define	FRAME_PARA_CAL8	0x18FF483D
             #define	FRAME_PARA_CAL9	0x18FF493D
             #define	FRAME_WORK1_DOSER	0x18FF4A3D
             #define	FRAME_WORK2_DOSER	0x18FF4B3D
             #define	FRAME_WORK3_DOSER	0x18FF4C3D
             #define	FRAME_WORK0_DOSER	0x18FF4D3D
             #define	FRAME_SENSOR1	0x18FF4E3D
             #define	FRAME_SENSOR2	0x18FF4F3D
             #define	FRAME_SENSOR3	0x18FF503D
             #define	FRAME_SENSOR4	0x18FF623D
             #define	FRAME_SENSOR5	0x18FF633D
             #define	FRAME_SENSOR6	0x18FF643D
             #define	FRAME_ENGINE	0x18FF513D
             #define	FRAME_LINK_TIMEOUT	0x18FF523D
             #define	FRAME_PARA1_MASTER	0x18FF533D
             #define	FRAME_PARA2_MASTER	0x18FF543D
             #define	FRAME_PARA3_MASTER	0x18FF553D
             #define	FRAME_PARA4_MASTER	0x18FF573D
             #define	FRAME_PARA5_MASTER	0x18FF583D
             #define	FRAME_PARA6_MASTER	0x18FF593D
             #define	FRAME_PARA7_MASTER	0x18FF603D
             #define	FRAME_PARA8_MASTER	0x18FF613D
             #define	FRAME_FAI_DTC	0x18FF563D
             #define	FRAME_FAI_FLASH	0x18FFA03D
    

    返回值：
        这个帧是否需要外发
        
        true：表示需要发出，
        false，表示不发
              主要针对PGN帧，没有故障时，有个帧是不发的    
*/

//#pragma CODE_SEG MAX_ROM  //2016-4-23 FOR COMPOSEPARAFRAME

byte ComposeParaFrame(dword id) 
{
    word w;
    byte blnr;
    
      blnr=VF_CALPARA_REQUEST_CANCTL;//默认情况为该帧需要发出

        		 //LBYTE(v_T1_EOBD[0])
        		 //HBYTE(v_T1_EOBD[0])
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	

      switch(id) 
      {
        		  case FRAME_STATUS:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=REMARK;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=KESEI;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LD_LDB:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_ldb);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_ldb);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_ld);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_ld);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_dmLambda;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=LBYTE(v_fmf_O);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HBYTE(v_fmf_O);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LD_LDL:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_SL_cls);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_SL_cls);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_cls_sl_learn;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_nb_SL;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_TPd_rd;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_REV_rd;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LD_MPA:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(Co_FMF_Altitude);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(Co_FMF_Altitude);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_mpa;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LD_MTI:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_ld_NotAltitude);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_ld_NotAltitude);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_mti;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_T_M64;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LAMBDA_U:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_msMu);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_msMu);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_u_M128);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_u_M128);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_ss;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_us_M16;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_f0;	
          		 break;
              case FRAME_LAMBDA_UC:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_u_M128_Cold);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_u_M128_Cold);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_M0_E_M8;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_M0_F_M256;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_G_M128;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_g;	
          		 break;
          		 
              case FRAME_LAMBDA_UC2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_Gx_M128;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_X;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_Y_M256;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_Z;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_R_d4;	
          		 break;
              case FRAME_LAMBDA_UP:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_LambdaTg;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_LambdaTg_SL;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_UP_M128;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LAMBDA_UA:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(vc_FMF_dtfi_trans+v_TPS_addt);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(vc_FMF_dtfi_trans+v_TPS_addt);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_UA_M128;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_LAMBDA_UR:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_UR_M16;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_FILM_1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(V_mf);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(V_mf);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_mf_evap);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_mf_evap);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_mf_remain);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_mf_remain);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_mf_area_M256);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_mf_area_M256);	
          		 break;
              case FRAME_FILM_2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=vc_B_mf;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=vc_C_mf;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=vc_D_mf;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_P_ACC_M64;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_P_DEC_M64;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_S_M128;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_L_M1024;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_L_DEC_M1024;	
          		 break;
              case FRAME_FILM_3:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_dmfS+v_dmfL);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_dmfS+v_dmfL);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_dmf_SumL);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_dmf_SumL);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_dmf_SumS);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_dmf_SumS);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_dmfL);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_dmfL);	
          		 break;
              case FRAME_MI:
                 // _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_mi);	
                  //_STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_mi);
			  	  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_t_T1);		//20211010 chenwei 环松显示t1
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_t_T1);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_ld+v_msMu);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_ld+v_msMu);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_dmf_SumS);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_dmf_SumS);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_dmi+v_dld);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_dmi+v_dld);	
          		 break;
              case FRAME_IGN_1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_CA_ignition;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_Ign_Map;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_Adj[ADJ_Igb];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_igs;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;//v_M/v_O	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_IGN_2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_CA_limit_byrpm;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_M;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_O;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=vc_Ign_Altitude;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_tl_ignition);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_tl_ignition);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_d_ign_fb;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_IGN_3:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_INJPHASE:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_t_bInj_toCas_r);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_t_bInj_toCas_r);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_SPEED_1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=(v_n0_low+v_n0_high);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_n0_low;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_K;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_J;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_SPEED_2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_rev62d5rpm_cf;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_SPEED_3:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_nfull);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_nfull);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_nstand);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_nstand);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_INITION:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_NOZZLE1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_dtw3);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_dtw3);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_dtw3_obj_lst);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_dtw3_obj_lst);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_gama_inuse);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_gama_inuse);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE((sword)vc_tw1_t3FK_I+vc_tw1_t3FK_P+v_vbat_dt_TW3FK+vc_tw1_t3fk);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE((sword)vc_tw1_t3FK_I+vc_tw1_t3FK_P+v_vbat_dt_TW3FK+vc_tw1_t3fk);	
          		 break;
              case FRAME_NOZZLE2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_n0_low;//LBYTE(v_TW3);		//20211010 chenwei 调整环松显示怠速转速
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;//HBYTE(v_TW3);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_TW3_in);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_TW3_in);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(vc_tw3_t35);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(vc_tw3_t35);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_TW1_forTrig);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_TW1_forTrig);	
          		 break;
              case FRAME_TPS1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_TPS_AD10);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_TPS_AD10);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_TPS0_AD10);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_TPd;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_TPS_odr;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_TPS_mode;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_TPS2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_N:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_rev_3d9);//v_n;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_rev_3d9);//v_n;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_OS1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_O2_ad;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_O2_lambdaE1;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_O2flu_max_av;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_O2flu_min_av;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_nb_OS_AD_CD;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=VF_O2H_STATUS;	//20211012 chenwei add 氧加热的标志	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_OS2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_LAMBDA_byMAPS;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_TMP1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_te);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_te);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_teng_inuse;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=V_Ta;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_tair_inuse;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_teng_ad;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_tair_ad;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_TMP2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(V_Tw);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(V_Tw);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_VBAT:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_Vbat_inuse;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_Vbat_inuse;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_CAS1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=nb_qy_betweenhy;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=nb_hy_betweenqy;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_qhCAint_ave;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_qhCAint_to_prd_m255;	
          		 break;
              case FRAME_CAS2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=m_nbtdc0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=nb_hy_lack;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=nb_qy_betweenhy;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=nb_hy_betweenqy;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_AD_cas;//2016-2-28 add 	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_PAIRS1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_pair_ad_input;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_pair_ad;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_pair_co;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=nb_forpair;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_t_pairs_1s;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_pair_odr;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_pair_mode;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_pair_ad_input_max;	
          		 break;
              case FRAME_PAIRS2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_pair_ad_input_min;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_pair_max_afterkeyon;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_pair_min_afterkeyon;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_Pair_deg;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_pair_ee;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_ETC1:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_TPS_tg;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_KP_M8;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_KI_M8;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_KD_M8;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_step_IACV_on;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=v_t_IACV_on;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;//LBYTE(v_step_sm);		
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_step_sm;//HBYTE(v_step_sm);	//20211008 环松数据流
          		 break;
              case FRAME_ETC2:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_I_ld;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_I2_ld;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_I_ini[0];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_I_ini[1];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;	
          		 break;
              case FRAME_ACTUATOR:
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=v_LambdaTg_lookup;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=v_LambdaTg_break;	
          		 break;
          		 case	FRAME_FAI_DTC	:		
                        
                        _STRU_PARAOUT.FAI_DTC.DTC0 =V_SPN0_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC1 =V_SPN1_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC2 =V_SPN2_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC3 =V_SPN3_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC4 =V_SPN4_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC5 =V_SPN5_STATUS;
                        _STRU_PARAOUT.FAI_DTC.DTC6 =V_SPN6_STATUS;
                        
                        //海拔的指示
                        //这个不作为故障
                        _STRU_PARAOUT.FAI_DTC.DTC7 =V_SPN7_STATUS;
                        if(VF_HIGHALTITUDE_SCRDTC) _STRU_PARAOUT.FAI_DTC.DTC7 =128;//海拔过高
          		          //2015-10-15 ADD 
                        if(VF_CALPARA_AUTOOUT_SYS) _STRU_PARAOUT.FAI_DTC.DTC7 +=64;//标定数据自动输出
                        if(!VF_NO_TRANS_EN_SYS) _STRU_PARAOUT.FAI_DTC.DTC7 +=32;//使用过渡模型
                        if(!VF_NO_NH3_EN_SYS) _STRU_PARAOUT.FAI_DTC.DTC7 +=16;//使能NH3模型
                        
          		 																	
          		 break;
              case OVERTPM_PRO:    //20210121 chenwei add 过温保护相关参数显示
					_STRU_PARAOUT.PARA_OUT_ST.BYTE0=UP_OVERTMP_PROTECTION;	//过温保护的up补偿
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=LBYTE(v_t_overTMP_1s);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HBYTE(v_t_overTMP_1s);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=BYPTR_M0_OverTMPProtection_SWITCH;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=VF_OVERTEMANDHIGHLOAD_ENG;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=VF_OVERTEMPRATRUEPROTECTION_ENG;	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(V_OverTMP_st_MskUnEna);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(V_OverTMP_st_MskUnEna);	

				   
          		 break;
              case RUNTIME_2:

			  	  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(_FROZEN_FRAME.COMMON.FAI_ID);	                      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(_FROZEN_FRAME.COMMON.FAI_ID); 
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=GetCurPgnbyte(true,DTC_TYPE_PENDING_NOW,false);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=GetCurPgnbyte(false,DTC_TYPE_PENDING_NOW,false);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=GetCurPgnbyte(false,DTC_TYPE_PENDING_NOW,false);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=GetCurPgnbyte(false,DTC_TYPE_PENDING_NOW,false);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(0); 	
          		 break;
              case RUNTIME_3:
			  	 _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(0);	                      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(0); 
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(0);
          		 break;
              case RUNTIME_4:
			  	 _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(0);	                      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(0); 
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(0);
          		 break;
              case RUNTIME_5:
			  	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(0);	                      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(0); 
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(0);
          		 break;
              case RUNTIME_6:
			  	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(0);	                      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(0); 
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(0);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(0);
          		 break;
              case MISF_1:
			  	/*_STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_map_ad;	                      //临时定义
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=nb_Misftime_block;              //一个Array中存在失火故障的Block数	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=nb_Misftime_cyc;                //一个Block中发生失火的次数
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_Misf_n_EnaEngSpdLo;   	      //失火检测允许的发动机转速下限 15.6rpm
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=VF_Misf_st_EmiDmgConf;	      //判断失火故障
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=VF_Misf_st_MisfSig;    	      //发生失火
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=BYPTR_Misf_n_MaxWMTCEngSpd_C;	  //WMTC最大转速
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=BYPTR_Misf_n_MaxEngSpd_C;       //发动机最大转速  */
          		 break;
              /*case RUNTIME_8:
                  //v_tl_can
                  //v_tl_can
                
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_tl_can);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_tl_can);	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=deFrame[2];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=deFrame[3];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=deFrame[4];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=deFrame[5];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=deFrame[6];	
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=deFrame[7];	
              
          		 break;  */   //20201022 chenwei 添加失火显示参数暂用RUNTIME_8，暂不知此处用途先把它屏蔽
				 case MISF_2:
			  /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=BYPTR_Misf_num_EmiDmgCntInBlock_C;	                     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=BYPTR_Misf_num_EmiDmgBlockInArrar_C;             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_Misf_AgAccnReplVal);               
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_Misf_AgAccnReplVal);    //采样段角加速度特征值   	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_Misf_MisfThres);	       //失火阈值
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_Misf_MisfThres);     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=cut_cy;                         //失火计数
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=nb_EropVMisf_cyc;          */
          		 break;
				 case MISF_3:
			  	/*_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_dt_cutign);	         //当前循环周期            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_dt_cutign);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_dt_cutign_lst);    //上次循环周期         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_dt_cutign_lst);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(nb_cycnum_block);	  //Block计数    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(nb_cycnum_block);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(nb_cycnum_cyc);      //clcye计数
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(nb_cycnum_cyc);   */        
          		 break;
				 case MISF_4:
			  	/*_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(nb_Misfcyc_del);	         //延迟失火检测的循环计数            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(nb_Misfcyc_del);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;                
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=0;    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_Misf_n_EnaEngSpdHi);	  //失火检测允许的发动机转速上限 62.5rpm    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_Misf_n_EnaEngSpdHi);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_Misf_st_MskUnEnaMisf);      //失火状态字
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_Misf_st_MskUnEnaMisf);  */      
          		 break;
				 case MISF_5:
			  /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(MISF_SPEED);	         //失火转速条件            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(MISF_SPEED);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(MISF_TRANS);       //失火过渡条件         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(MISF_TRANS);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(MISF_TMP);	       //失火发动机温度条件    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(MISF_TMP);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(MISF_VBAT);         //失火电压条件
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(MISF_VBAT);    */    
          		 break;
				 case MISF_6:
			  /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(MISF_ZROTRQ);	         //失火零扭矩线条件            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(MISF_ZROTRQ);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(MISF_ALT);           //失火海拔条件         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(MISF_ALT);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(MISF_CF);	        //失火断油条件    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(MISF_CF);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(MISF_YSTDC);         //失火判缸条件
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(MISF_YSTDC);   */     
          		 break;
				 case MISF_7:
			  /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(MISF_RUNTIME);	         //失火发动机运行条件            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(MISF_RUNTIME);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(MISF_DTC);	        //失火存在故障条件         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(MISF_DTC);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=nb_schmin;          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=VF_SCHMIN_SUM;          //20201118 chenwei 进入欧五os诊断函数的次数   
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=VF_OSSD_st_FalEdgSlwRespErrSt;   */     
          		 break;
				 case EUROPVOS_1:
			  	/*_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_dt_tschmax_4us);	                     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_dt_tschmax_4us);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(sum_tschmax_ms);                    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(sum_tschmax_ms);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_dt_schmaxave);	            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_dt_schmaxave);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=nb_schmax;         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=t_os_diagperiod;     */   
          		 break;
				 case EUROPVOS_2:
			  /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_dt_tschmin_4us);	                     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_dt_tschmin_4us);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(sum_tschmin_ms);                    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(sum_tschmin_ms);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_dt_schminave);	            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_dt_schminave);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=VF_OSSD_st_RisEdgSlwRespErrSt;         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=VF_SCHMAX_SUM;        */
          		 break;
				 case CCPS_1:
			          	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_t_CCPS_closelast);	                     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_t_CCPS_closelast);             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_t_CCPS_opennow);                    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_t_CCPS_opennow);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=0;	            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=LBYTE(V_CCPS_st_MskEna);          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HBYTE(V_CCPS_st_MskEna);         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=VF_CCPS_status;        
          		 break;
				 case SAIC_1:
			          /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=v_SAIC_st_UnEnaSAIC;	                     
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE1=VF_SAIC_status;             
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE2=0;                    
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE3=LBYTE(t_after1stStart_s);    	      
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE4=HBYTE(t_after1stStart_s);	            
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE5=0;          
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE6=0;         
                  _STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;   */     
          		 break;
				 
				 case ITSD_1:
			  	/*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=VF_ITSD_st_ErtcFaultConf;	                     
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_ITSD_ED_Ta_Last;             
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_ITSD_ED_Ta_Diff;                    
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE3=nb_ITSD_ED_CycCnt;    	      
                 	_STRU_PARAOUT.PARA_OUT_ST.BYTE4=nb_ITSD_ED_FaultCnt;	            
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE5=V_Ta;          
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE6=DTC_FORM;         
                  	_STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;      */  
          		 break;
			   	 case ITSD_2:
			  	/*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=VF_ITSD_st_StuctFaultConf; 					   
					_STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_ITSD_SD_Ta_MIN;			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_ITSD_SD_Ta_MAX;					 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE3=v_ITSD_SD_Ta_LimDiff;			
					_STRU_PARAOUT.PARA_OUT_ST.BYTE4=V_ITSD_SD_MskEna; 			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE5=LBYTE(v_te);		  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE6=HBYTE(v_te);		 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE7=0;		*/  
			  	 break;
				 case ITSD_3:
			  	/*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(t_EngSoak_s); 					   
					_STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(t_EngSoak_s);			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_ITSD_SD_ti_EngStop);					 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_ITSD_SD_ti_EngStop);			
					_STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_t_eng_t_s); 			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_t_eng_t_s);		  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_ITSD_SD_ti_EngWrm);		 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_ITSD_SD_ti_EngWrm);		*/  
			  	 break;
				 
				 case CLTD_1:
			  	/*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=VF_CLTD_st_HiStrtFaultConf; 					   
					_STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_CLTD_HSD_IntkTmp;			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_CLTD_HSD_ClntTmp);					 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_CLTD_HSD_ClntTmp);			
					_STRU_PARAOUT.PARA_OUT_ST.BYTE4=v_CLTD_HSD_TmpDiff; 			  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE5=V_CLTD_HSD_MskEna;		  
					_STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_CLTD_HSD_ti_EngStop);		 
					_STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_CLTD_HSD_ti_EngStop);		 */ 
			  	 break;
			  	 case CLTD_2:
				 /*	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(t_EngSoak_s); 					
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(t_EngSoak_s); 		   
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE2=LBYTE(v_t_eng_t_s); 				  
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE3=HBYTE(v_t_eng_t_s); 		 
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE4=DTC_FORM; 		   
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE5=V_Ta; 	   
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_te); 	  
				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_te); 	   */
			  	break;
			  	case CLTD_3:
    				/* 	_STRU_PARAOUT.PARA_OUT_ST.BYTE0=VF_CLTD_st_StuctFaultConf; 					
    					_STRU_PARAOUT.PARA_OUT_ST.BYTE1=v_CLTD_SD_TmpStrt; 		   
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE2=v_CLTD_SD_TmpDiff; 				  
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE3=V_CLTD_SD_MskEna; 		 
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_CLTD_SD_ti_EngStop); 		   
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_CLTD_SD_ti_EngStop); 	   
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE6=LBYTE(v_CLTD_SD_ti_EngRun); 	  
    				 	_STRU_PARAOUT.PARA_OUT_ST.BYTE7=HBYTE(v_CLTD_SD_ti_EngRun); 	 */  
			  	break;
				
              case FRAME_FAI_FLASH: //2015-7-12
                      #define CMD_FLASH_LINKED 0xA0
                      #define CMD_FLASH_LINKED 0xA0
                      //2015-7-16 UPDATED
                      
                      if(VF_FLASHLINK_DATAVIEW)
                      {
                          _STRU_PARAOUT.CMD_FLASH.CMD=CMD_FLASH_LINKED;
                          
                          if(!VF_BY_1UNIT_DATAVIEW){
                              //DWORD
                              _STRU_PARAOUT.CMD_FLASH.CMD2=0xDA;
															if(((v_add_forDataview>>15)&0x01)==1)
															{		
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview+1,0,0,0);//show_eeprom(20,v_add_forDataview+1,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview+0,0,0,0);//show_eeprom(20,v_add_forDataview  ,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=show_eeprom(20,v_add_forDataview+3,0,0,0);//show_eeprom(20,v_add_forDataview+3,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=show_eeprom(20,v_add_forDataview+2,0,0,0);//show_eeprom(20,v_add_forDataview+2,0,0,0);
															
//																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview+2,0,0,0);//show_eeprom(20,v_add_forDataview+1,0,0,0);
//																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview+3,0,0,0);//show_eeprom(20,v_add_forDataview  ,0,0,0);
//																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=show_eeprom(20,v_add_forDataview  ,0,0,0);//show_eeprom(20,v_add_forDataview+3,0,0,0);
//																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=show_eeprom(20,v_add_forDataview+1,0,0,0);//show_eeprom(20,v_add_forDataview+2,0,0,0);
															}
															else
															{		
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview+1,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview  ,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=show_eeprom(20,v_add_forDataview+3,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=show_eeprom(20,v_add_forDataview+2,0,0,0);
															}
													} else if(VF_BY_1UNIT_DATAVIEW)
                          {   //一个字节
                              _STRU_PARAOUT.CMD_FLASH.CMD2=0xAA;

															if(((v_add_forDataview>>15)&0x01)==1)
															{
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=0;// show_eeprom(20,v_add_forDataview+1,0,0,0);
															}
															else
															{
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L= show_eeprom(20,v_add_forDataview,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=0;// show_eeprom(20,v_add_forDataview+1,0,0,0);
                              }    
                          } 
                          else 
                          {    //一个字
                              _STRU_PARAOUT.CMD_FLASH.CMD2=0xBA;//2015-7-21 FROM BB TO BA
															if(((v_add_forDataview>>15)&0x01)==1)
															{
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview+1,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview+0,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=0;//2版本
																
//																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview,0,0,0);
//																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview+1,0,0,0);
//																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=0;
//																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=0;//1版本
															}
															else
															{
																	_STRU_PARAOUT.CMD_FLASH.DATA1_H=show_eeprom(20,v_add_forDataview+1,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA1_L=show_eeprom(20,v_add_forDataview,0,0,0);
																	_STRU_PARAOUT.CMD_FLASH.DATA2_H=0;
																	_STRU_PARAOUT.CMD_FLASH.DATA2_L=0;

															}
                          }
                          
                          _STRU_PARAOUT.CMD_FLASH.ADD_L=(byte)v_add_forDataview;
                          _STRU_PARAOUT.CMD_FLASH.ADD_H= (byte)(v_add_forDataview>>8);
                          
                          
                                            
                      }
        
             break;


        		 																			
        		 
        	/*	 
            case RUNTIME_1:      //2015-8-24
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_DC);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_DC);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_DC);

                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=LBYTE(v_WUC);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=HBYTE(v_WUC);
                      
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=LBYTE(v_OBDC);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HBYTE(v_OBDC);
                
            break;
            
            case RUNTIME_2:
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_ENG_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_ENG_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_ENG_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HHBYTE(v_ENG_t_s);
            
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_KEY_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_KEY_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HLBYTE(v_KEY_t_s);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HHBYTE(v_KEY_t_s);
            
            break;
            
            case RUNTIME_3:
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_nb_DOSER);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_nb_DOSER);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_nb_DOSER);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HHBYTE(v_nb_DOSER);
            
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_nb_Supplier);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_nb_Supplier);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HLBYTE(v_nb_Supplier);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HHBYTE(v_nb_Supplier);
            
            break;
            
            case RUNTIME_4:
                    //2015-9-20 add 
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_ENG_t_s_MILOn);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_ENG_t_s_MILOn);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_ENG_t_s_MILOn);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HHBYTE(v_ENG_t_s_MILOn);
            
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_ENG_t_s_afterDTCclr);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_ENG_t_s_afterDTCclr);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HLBYTE(v_ENG_t_s_afterDTCclr);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HHBYTE(v_ENG_t_s_afterDTCclr);
            
            break;
            case RUNTIME_5:
                    //2015-10-16 add 
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_Fuel_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_Fuel_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_Fuel_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HHBYTE(v_Fuel_c_ml_sum);
            
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_DEF_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_DEF_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HLBYTE(v_DEF_c_ml_sum);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HHBYTE(v_DEF_c_ml_sum);
            
            break;
            case RUNTIME_6:
                    //2015-10-16 add 
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE0=LBYTE(v_Fuel_c_ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE1=HBYTE(v_Fuel_c_ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE2=HLBYTE(v_Fuel_c_ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE3=HHBYTE(v_Fuel_c_ul);
            
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE4=LBYTE(v_DEF_c_d1ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE5=HBYTE(v_DEF_c_d1ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE6=HLBYTE(v_DEF_c_d1ul);
                      _STRU_PARAOUT.PARA_OUT_ST.BYTE7=HHBYTE(v_DEF_c_d1ul);
            
            break;
            //v_Fuel_c_ul

           */
      }
      
      return blnr;
}
//#pragma CODE_SEG DEFAULT


byte Get_DTCNO_byID(byte id)
{
    return M1_DTCNO[id];    
}


/*
    2015-12-3 根据PCODE_ANOTHER选择P CODE
    
    2016-2-23 move from SCR TO PI
*/
word GetPcode(byte id)
{
    #ifndef PCODE_ANOTHER
        return P_CODE[id];
    
    #else
        return P_CODE2[id];
    #endif    
}

/*
    SPN解析
    
    
    blnFromSt : true  从头开始
              : false 非从头开始
    
    blnCur: true,当前
            false,历史
    
    
    blnGetOnly1: 
            true  :每次进来只解析一次，获得一个spn，下次的话，就获得下一个
            false :每次遍历一遍，把所有故障码都看一遍
            
    返回值，如果是true，表示是第一个package
            
    
    
    2015-8-27 需要在这里考虑故障历史        
*/
byte AnalysisSPN(byte DTC_TYPE,byte blnGetOnly1,byte blnFromSt)
{
    byte i;
    
    byte id;
    
    byte bSPN;
    byte bSPN_NG;
    
    byte bln1st;
    
    
    id=0;
    
    v_spn=0;
    v_fmi=0;
    
    {
        v_pcode=0;
    }
    
    bln1st=false;
    
    
    if(blnFromSt)v_id_cur=0;//2015-9-16
    //v_id_cur;
    //
    for(id=0;id<NUM_DTC_SCR;id++)
    {
      
        if(v_id_cur>=NUM_DTC_SCR)
        {
            v_id_cur=0;
        }
        
        if(v_id_cur==0){
            v_NO_SpnPack_CanOut=0; 
            bln1st=true;   
        }
        
        if(CheckIsSPNNG(v_id_cur,DTC_TYPE))
        {
            v_NO_SpnPack_CanOut++;
            //对应id的有故障
            v_pcode=P_CODE[v_id_cur];//2015-9-21 纠正
            v_spn=  M1_SPN[v_id_cur];
            v_fmi=  M1_FMI[v_id_cur];
            /*
            v_st_mil[0]=   (SCRDTC_ST[v_id_cur]>>7)&0x01;
            v_st_scr[0]=   (SCRDTC_ST[v_id_cur]>>5)&0x03;
            v_st_miloff_con[0]=  (SCRDTC_ST[v_id_cur]>>3)&0x03;
            v_pumpoff[0]=  (SCRDTC_ST[v_id_cur]>>1)&0x03;
            */
            if(v_spn>0){
                v_id_cur++;
                if(blnGetOnly1)break;//暂时取最靠前的故障    //2015-9-16 仅仅获得一次
            }
        }
        v_id_cur++;
        
    }
    
    return bln1st;
}



/*
    获得当前需要传输的PGN的字节
    blnFromst：是否从头开始
    blnCur:是否是当前的
    
    blnPgn: true-PGN模式
            false--PCODE模式
            
*/


//byte GetCurPgnbyte(byte blnFromst,byte blnCur) 
byte GetCurPgnbyte(byte blnFromst,byte DTC_TYPE,byte blnPgn)//2015-9-16 
{
    byte bytR;
    byte i;
    
    byte blnFromst2;
    
   // blnPgn
    
    if(blnFromst){
        CurPgnInx=0;
        
        v_id_cur=0;
        //需要载入bytPgn
        
        for(i=0;i<=NUM_BYTPGN-1;i++)
        {
            bytPgn[i]=0xff;            
        }
        
        //deOS++;
        blnFromst2=1;
        
        v_NO_SpnPack_CanOut=0;//2015-9-21
        
    } 
    else 
    {
        blnFromst2=0;
    }
    //while(CurPgnInx<5) 
    while(CurPgnInx<1) 
    {
      
        //if(v_n_SPN_ng>=v_NO_SpnPack_CanOut)AnalysisSPN(blnCur,true,blnFromst2);//2015-9-21 增加注意这里的改动，blnFromst2
        if(v_n_SPN_ng>v_NO_SpnPack_CanOut)AnalysisSPN(DTC_TYPE,true,blnFromst2);//2015-9-21-2 >
        else {
            //break;
            //2015-8-29 更改为
            v_spn=0xfffffffful;
            v_pcode=0;//PCODE默认需要补充时为0
        }
        
        if(blnFromst2>0)blnFromst2=0;
        
        //for(i=0;i<4;i++) //2015-6-10 发现这个条件是错的
        if(blnPgn)
        {
            bytPgn[CurPgnInx] =(byte)v_spn;
            CurPgnInx++;
            
            bytPgn[CurPgnInx] =(byte)(v_spn>>8);
            CurPgnInx++;
            
            bytPgn[CurPgnInx] =(byte)(v_spn>>16);
            CurPgnInx++;
            
            bytPgn[CurPgnInx] =(byte)(v_spn>>24);
            CurPgnInx++;
        } 
        else
        {   //2015-9-16 add 
            bytPgn[CurPgnInx] =(byte)(v_pcode>>8);
            CurPgnInx++;
            
            //2015-9-21 PCODE,杨老师说顺序错了，改成这样
            bytPgn[CurPgnInx] =(byte)v_pcode;              
            CurPgnInx++;
            
           
        }
    }
    
    bytR=bytPgn[0]; //返回当前值
    
    if(CurPgnInx>0)CurPgnInx--;//2015-6-9 ADD
    
    if(CurPgnInx<0)CurPgnInx=0;
    
    //后面的往前移位
    for(i=0;i<=NUM_BYTPGN-2;i++)
    {
        bytPgn[i]=bytPgn[i+1];            
    }
    
    if(blnPgn) bytPgn[NUM_BYTPGN-1]=0xff;
    else bytPgn[NUM_BYTPGN-1]=0;
    
    return bytR;
}

/*
    获取当前SPN故障的数量；

    同时获得mil,scr,miloff,pumpoff等灯的最大状态
    
    blnCur: 
        true  _当前故障
        false 非当前故障
*/
//byte Get_Num_SPNNG(void)
byte Get_Num_SPNNG(byte DTC_TYPE)
{
    byte n;
    word id;
    byte mil;
    byte scr;
    byte offtoon;
    byte pumpoff;

    
    n=0;
    for(id=0;id<NUM_DTC_SCR;id++){
      
        if(CheckIsSPNNG(id,DTC_TYPE)){
            if(M1_SPN[id]>0) 
            {
                  n++; //SPN >0才是有效的

            
            }
        }
    }
    
    return n;
}

/*
    GET MIL STATUS
    
    获取MIL和SCR等的状态
    //2015-9-16
*/
byte GetMIL(void)
{
    //2015-9-21 add 
    
    if(!VF_KEYON_HARD)return false;
    
    //else return VF_MILON_CTLBYDTC;
    else return v_st_mil_disp;   
}

byte GetSCRLamp(void)
{

    if(!VF_KEYON_HARD)return false;
    else return v_st_scr;//VF_SCRON_CTLBYDTC;


}


/*
    2015-6-13 
    是否需要因为故障而停止喷射
*/
byte GetIsPumpoffByDTC(void)
{
    if(!VF_KEYON_HARD)return false;
    //else return VF_PUMPOFF_CTLBYDTC;
    else return 0;
}


//2016-6-23 增加以下内容

byte v_pos_dtc;
byte v_pos_curid;

//#define TYPE_PRE_DTC 10
#define TYPE_PRE_DTC 5//2016-3-2 ADD 
/*
    获得DTC状态：SCRLAMP/MIL/PUMPOFF
    
    为了节省时间，每次获取一个DTC HISTORY位置，轮流获取
*/
byte GetDTCStauts(void)
{
    byte n;
    word id;
    byte mil;
    byte scr;
    byte offtoon;
    byte pumpoff;
    byte pending_chk;
    byte torque_limit;
    byte bunremove;
    
    
      

    //对于被确认的故障，和待确认的故障，都应该从EEPROM中直接查询，因为需要得到DC等信息
    //for(pos_dtc=0;pos_dtc<NUM_DTC_HISTORY;pos_dtc++)
        if(v_pos_dtc==0){
            //v_st_mil=0;//2016-3-3 DELETE
            //2016-3-3 注意PI的MIL显示和SCR的MIL显示不同，pi的仅仅对应当前的故障
        }
        
             
        
        {
          
            mil=   (SCRDTC_ST[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]>>7)&0x01;
            scr=   (SCRDTC_ST[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]>>5)&0x03;
            offtoon=  (SCRDTC_ST[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]>>3)&0x03;
            pumpoff=  (SCRDTC_ST[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]>>1)&0x03;
            
            bunremove=(SCRDTC_ST[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ])&0x01;
            
            pending_chk=DTC_CON_2[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]&0x03;
            torque_limit=(DTC_CON_2[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID ]>>2)&0x03; //限扭位
            
            mil=MIL_FOR_DTC[_TMP_DTC[v_pos_dtc].COMMON.FAI_ID];//2016-2-24
        
        //EEPROM中存储的故障的处理，    
            /* 2016-2-24 临时屏蔽
            if(
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_CONFIRM)
                  //&&!CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_NOW)
               )
            {   //对于已确认的故障，需要判断是不是当前已经没有故障了

                  if(!CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW))
                  {
                      if(
                          offtoon==0
                         ||(offtoon==1&&v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC)
                         ||(offtoon==2&&v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC+2) 
                         //||(offtoon==3&&v_OBDC>_TMP_DTC[v_pos_dtc].COMMON.NB_OBDC+2) 
                         ||(offtoon==3&&v_OBDC>_TMP_DTC[v_pos_dtc].COMMON.NB_OBDC)//2015-12-25 
                         )
                      {  //故障消失后，MIL马上恢复
                          
                          if(v_KEY_t_s_cur>TYPE_PRE_DTC) 
                          {//2015-12-1
                            
                              //SetSpnStatus(byte blnSet,byte id,byte DTC_TYPE)
                              SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_CONFIRM);

                              _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CLR;
                              _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                              _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                              _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                              
                              if(bunremove) Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                              else Opr_Tmp_DTC(2,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                              //
                          }
                      }
                      else 
                      {
                          if(v_st_mil<mil)v_st_mil=mil;    
                      }
                  } 
                  else
                  {
                          if(v_st_mil<mil)v_st_mil=mil;
                      
                      //2015-12-25 故障CONFIRM时，应该要不断更新故障对应那个时候的DC和OBDC，因为DC和OBDC会一直在(￣︶￣)↗ 涨
                          if(v_KEY_t_s_cur>TYPE_PRE_DTC)
                          {
                              _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CONFIRM;
                              _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                              _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                              _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                              
                              Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                          }
                             
                  }
                
            }
            else if
            (
               // _FROZEN_FRAME.COMMON.DTC_STA==DTC_STA_PENDING
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_PENDING)
                  &&CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW)
            ) 
            {   //待确认的故障，需要判断，是否已经满足确认的条件
                  if(
                      pending_chk==0//立即确认
                      ||(pending_chk==1&&v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC)
                      ||(pending_chk==2&&v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC+2)
                      //||(pending_chk==3&&v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_OBDC+2)
                      ||(pending_chk==3&&v_OBDC>_TMP_DTC[v_pos_dtc].COMMON.NB_OBDC+2)  //2015-12-26 纠正
                    ) 
                    {   //故障被确认

                        if(v_KEY_t_s_cur>TYPE_PRE_DTC) 
                        {   //2015-12-1
                            if(v_st_mil<mil)v_st_mil=mil;    
                            SetSpnStatus(true,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_CONFIRM);        
                            SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING);
                                    
                            _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CONFIRM;
                            _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                            _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                            _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                          
                            Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                        }
                    } 
                    else
                    {
                           // if(v_st_mil<mil)v_st_mil=mil;    
                    }
                   
            } 
            else if
            (
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_PENDING)
                  &&!CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW)
            ) 
            {    //pending，并发现没有故障，则
                
            
                  //if(v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC)
                  {
                        if(v_KEY_t_s_cur>TYPE_PRE_DTC) 
                        {
                        
                            SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING);        
                            //_TMP_DTC[v_pos_dtc].COMMON.DTC_STA=DTC_STA_CONFIRM;
                            Opr_Tmp_DTC(2,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                        }
                  }
            }
             */



            if(
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_CONFIRM)
               )
            {   //对于已确认的故障，需要判断是不是当前已经没有故障了
                  /*  2016-3-2 DELETE
                  if(!CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW))
                  {
                      {  //故障消失后，MIL马上恢复
                          
                          //if(v_t_eng_1s>TYPE_PRE_DTC) 
                          {//2015-12-1
                            
                              //SetSpnStatus(byte blnSet,byte id,byte DTC_TYPE)
                              SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_CONFIRM);

                              _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CLR;
                              _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                              _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                              _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                              
                              if(bunremove) Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                              else Opr_Tmp_DTC(2,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                              //
                          }
                      }
                  } 
                  else
                  {

                        //if((v_st_mil==0)||(v_st_mil>mil&&mil!=0))v_st_mil=mil;//记录最小的mil；//2016-3-2
                        //  if(v_st_mil<mil)v_st_mil=mil;
                      
                      //2015-12-25 故障CONFIRM时，应该要不断更新故障对应那个时候的DC和OBDC，因为DC和OBDC会一直在(￣︶￣)↗ 涨
                          //if(v_t_eng_1s>TYPE_PRE_DTC)
                          {
                              _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CONFIRM;
                              _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                              _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                              _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                              
                              Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                          }
                             
                  } */
                
            }
            else if
            (
               // _FROZEN_FRAME.COMMON.DTC_STA==DTC_STA_PENDING
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_PENDING)
                  &&CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW)
            ) 
            {   //待确认的故障，需要判断，是否已经满足确认的条件
                    {   //故障被确认


                        if(v_t_eng_1s>TYPE_PRE_DTC) 
                        {   //2015-12-1
                            //if(v_st_mil<mil)v_st_mil=mil;    
                            //if((v_st_mil==0)||(v_st_mil>mil&&mil!=0))v_st_mil=mil;//记录最小的mil；//2016-3-2 move out

                            SetSpnStatus(true,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_CONFIRM);        
                            SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING);
                                    
                            _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_CONFIRM;
                            _FROZEN_FRAME.COMMON.NB_DC=v_DC;
                            _FROZEN_FRAME.COMMON.NB_OBDC=v_OBDC;
                            _FROZEN_FRAME.COMMON.FAI_ID=_TMP_DTC[v_pos_dtc].COMMON.FAI_ID;
                          
                            Opr_Tmp_DTC(3,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                        }
                    } 
                   
            } 
            else if
            (
                (_TMP_DTC[v_pos_dtc].COMMON.DTC_STA==DTC_STA_PENDING)
                  &&!CheckIsSPNNG(_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING_NOW)
            ) 
            {    //pending，并发现没有故障，则
                

            
                  //if(v_DC>_TMP_DTC[v_pos_dtc].COMMON.NB_DC)
                  {
                        if(v_t_eng_1s>TYPE_PRE_DTC) 
                        {
                        
                            SetSpnStatus(false,_TMP_DTC[v_pos_dtc].COMMON.FAI_ID,DTC_TYPE_PENDING);        
                            //_TMP_DTC[v_pos_dtc].COMMON.DTC_STA=DTC_STA_CONFIRM;
                            Opr_Tmp_DTC(2,v_pos_dtc);//1:APPEND,2:DELETE,3:MODIFY 
                        }
                  }
            }            
        }
        
        v_pos_dtc++;
        if(v_pos_dtc>=v_ub_tmp_dtc)
        {
              //v_st_mil_disp=v_st_mil;  //2016-3-3 delete
                     
              if(v_st_mil>0)
              {
                  //置灯亮标志
                  
                  //VF_MILON_CTLBYDTC=true; 
              } 
              else 
              {
                  //置灯不亮标志
                  //VF_MILON_CTLBYDTC=false;
              }
              v_pos_dtc=0;
        }

    //====================================================================================
    //对SCR LAMP状态的确认
        //看当前故障
        if(v_pos_curid==0)
        {
            st_scr_tmp=0;
            v_pumpoff=0;
        }
        
        {
          
            if(
                CheckIsSPNNG(v_pos_curid,DTC_TYPE_PENDING_NOW)
                &&!CheckIsSPNNG(v_pos_curid,DTC_TYPE_CONFIRM)
                &&!CheckIsSPNNG(v_pos_curid,DTC_TYPE_PENDING)
                )
            {     //对于不属于PENDING和CONFIRM的，需要确认一下是否属于在EEPROM中被清除的故障
                  //如果属于被清除的故障，则把状态更改过来，否则，新增一个

                  _FROZEN_FRAME.COMMON.FAI_ID=v_pos_curid;
                  _FROZEN_FRAME.COMMON.DTC_STA=DTC_STA_PENDING;
                  _FROZEN_FRAME.COMMON.NB_DC=v_DC;

                  //===================
                  //if(v_KEY_t_s_cur>TYPE_PRE_DTC) 
                  if(v_t_eng_1s>TYPE_PRE_DTC) 

                  {
                      if(Opr_Tmp_DTC(4,0))
                      {

                          SetSpnStatus(true,v_pos_curid,DTC_TYPE_PENDING);        
                      }//1:APPEND,2:DELETE,3:MODIFY ,4:REPLACE IF EXIST,ELSE APPEND  
                      /**/
                      //注意对于APPEND的类的，一定要确定APPEND是否成功
                      //一定要保证状态的同步
                  }
            } 
            
        
        }


     
    //对PUMP_OFF状态的确认
        //看
        {
          
            if(CheckIsSPNNG(v_pos_curid,DTC_TYPE_PENDING_NOW))
            {
 
                 //scr lamp
                       //当前有故障，
                      scr=   (SCRDTC_ST[v_pos_curid]>>5)&0x03;
                      if(scr>st_scr_tmp)
                      {
                          st_scr_tmp=scr;        
                      }

                      if((v_st_mil==0)||(v_st_mil>mil&&mil!=0))v_st_mil=mil;//记录最小的mil；//2016-3-2 move in

                      //if(v_KEY_t_s_cur>TYPE_PRE_DTC) 
                      {
                            //在此判断是否有以往的冻结帧，如果没有就冻结当前
                            if(_FROZEN_FRAME.COMMON.FAI_ID_FROZEN==INVALID_FAI_ID)
                            {
                                _FROZEN_FRAME.COMMON.T_2D5=v_T_percent;//v_Pedal_percentM2D5;
                                _FROZEN_FRAME.COMMON.N=(word)v_rev_15d6rpm*16;;//v_n_rpm;
                                _FROZEN_FRAME.COMMON.ET=v_te;//v_Eng_CoolTmp;
                                _FROZEN_FRAME.COMMON.TSCR=0;//v_TSCR;
                                _FROZEN_FRAME.COMMON.FAI_ID_FROZEN=v_pos_curid; //2015-11-28
                            }
                      }
                  //pumpoff
                      pumpoff=   (SCRDTC_ST[v_pos_curid]>>1)&0x03;
                      if(pumpoff>v_pumpoff)
                      {
                          v_pumpoff=pumpoff;        
                      }
            }
        
        }
        
        v_pos_curid++;
        
        if(v_pos_curid>=NUM_DTC_SCR)
        {
            v_pos_curid=0;
            
            if(v_pumpoff>1)//1: FOR 50H
            {
                //VF_PUMPOFF_CTLBYDTC =true;
            } 
            else
            {
                //VF_PUMPOFF_CTLBYDTC =false;
            }
            
            //2015-12-4
            v_st_scr=st_scr_tmp;
            
            if(st_scr_tmp) 
            {
                //VF_SCRON_CTLBYDTC =true;
            } 
            else
            {
                //VF_SCRON_CTLBYDTC =false;
            }
            
            //2016-3-3 ADD HERE
                v_st_mil_disp=v_st_mil;
                v_st_mil=0;
                
        }
        
        
    //2015-12-2    
        if(!VF_MODE_OBDWORK_CAL)
        {
            //VF_MILON_CTLBYDTC=false;
            //VF_SCRON_CTLBYDTC=false;
            //VF_PUMPOFF_CTLBYDTC=false;    
        }
        
}


/*
    根据故障的pgn数，计算包数
*/
word Cal_Num_pack(void)
{
    word w;
    word ni;
    
    
    if(v_n_SPN_ng>0){
      
        w=(word)v_n_SPN_ng*4+2;

        
        ni=w/7;
        
        if(ni*7<w)
        {
            ni++;    
        }
    } else ni=0;
    
    
    v_n_SPN_pack=ni;
    
    return ni;
    
}


/*
    在确定位置保存故障冻结帧
    
    blnLoad:需要加载，还是保存
    frame_pos:位置
    blnIDonly：
        0:只操作ID,
        1:ID+DTC_STA
        2:
        其他:all
        
    //block_type:0-全部，1-可清除区，2-不可清除区,,byte block_type 暂时不考虑
         
    2015-11-28 变更长度    
*/


void Load_FrozenFrame(byte blnLoad,word frame_pos,byte blnIDonly)
{
    //冻结帧需要存储到EEPROM的位置
    
    
    word add_st;
    word add_w;
    
    add_st=frame_pos*BYTES_PER_FRAME_FROZEN;
    
    add_st+=(word)ADD_1ST_DTC; //2015-12-21
    
    add_w=add_st+POS_OF_FAI_ID;
    
    
    
    if(blnLoad)
    {
        //_FROZEN_FRAME.COMMON.FAI_ID=rd_byAddGrp(1,add_w);
        //_FROZEN_FRAME.COMMON.FAI_ID=rd_byAddGrp(1,add_w);//2015-9-21
        //_FROZEN_FRAME.COMMON.FAI_ID=rdrive_iic(add_w);//2015-12-1 注意要更改为rdrive_iic，因为rd_byAddGrp会把255变为0
				_FROZEN_FRAME.COMMON.FAI_ID=rdrive_iic_ext(add_w);
        
        if(!blnIDonly)
        {
            CLR_WDG
              
            add_w++;
            _FROZEN_FRAME.COMMON.NB_DC=rd_byAddGrp(3,add_w);
            
            add_w+=3;
            //_FROZEN_FRAME.COMMON.NB_WUC=rd_byAddGrp(2,add_w);
            
            add_w+=2;
            _FROZEN_FRAME.COMMON.NB_OBDC=rd_byAddGrp(2,add_w);
            
            add_w+=2;
            //_FROZEN_FRAME.COMMON.ENG_RT=rd_byAddGrp(4,add_w);
            //2015-11-29 REPLACE BY
            _FROZEN_FRAME.COMMON.DTC_STA=rd_byAddGrp(1,add_w);
            
            add_w+=4;
            _FROZEN_FRAME.COMMON.KEYON_T=rd_byAddGrp(4,add_w);
            
            
                add_w+=4;
            //    _FROZEN_FRAME.COMMON.NB_PUMP=rd_byAddGrp(4,add_w);
                
                add_w+=4;
              //  _FROZEN_FRAME.COMMON.T_2D5=rd_byAddGrp(1,add_w);
                
                add_w+=1;
                //_FROZEN_FRAME.COMMON.N=rd_byAddGrp(2,add_w);
                
                add_w+=2;
               // _FROZEN_FRAME.COMMON.ET=rd_byAddGrp(1,add_w);
                
                add_w+=1;
                //_FROZEN_FRAME.COMMON.TSCR=rd_byAddGrp(1,add_w);
                
              //  _FROZEN_FRAME.COMMON.FAI_ID_FROZEN=INVALID_FAI_ID; //2016-3-5 DELETE IN Load_FrozenFrame
             
        }
            
            
        //读取的时候，把冻结帧的信息也读出,2015-11-28
                _FROZEN_FRAME.COMMON.FAI_ID_FROZEN=rd_byAddGrp(1,ADD_FROZEN_ID);//2016-3-5 MOVE OUT
           //if(_FROZEN_FRAME.COMMON.FAI_ID_FROZEN==INVALID_FAI_ID)
           if(_FROZEN_FRAME.COMMON.FAI_ID_FROZEN!=INVALID_FAI_ID)//2016-3-5 CHANGE IN Load_FrozenFrame
           {
                _FROZEN_FRAME.COMMON.T_2D5=rd_byAddGrp(1,ADD_FROZEN_T);
                _FROZEN_FRAME.COMMON.N=rd_byAddGrp(2,ADD_FROZEN_N);
                _FROZEN_FRAME.COMMON.ET=rd_byAddGrp(1,ADD_FROZEN_ET);
                _FROZEN_FRAME.COMMON.TSCR=rd_byAddGrp(2,ADD_FROZEN_TSCR);
           }
                
    } 
    else 
    {
        if(_FROZEN_FRAME.COMMON.FAI_ID<NUM_DTC_SCR)
        {   //2015-
              
            if(SCRDTC_ST[_FROZEN_FRAME.COMMON.FAI_ID]&0x01) 
            {
                v_nb_dtc_noneclr++;
            } 
            else
            {
                v_nb_dtc_clr++;
            }
        
            //保存次数
            wdrive_iic_ext(ADD_DTC_NUM_NONECLR,v_nb_dtc_noneclr);//
            wdrive_iic_ext(ADD_DTC_NUM_CLR,v_nb_dtc_clr);//

        } 
            wdrive_iic_ext(add_w,_FROZEN_FRAME.COMMON.FAI_ID);//2015-9-21 偏移128

            //
            if(!blnIDonly)
            {
                CLR_WDG
                
                add_w++;
                wdrive_iic_ext(add_w,LBYTE(_FROZEN_FRAME.COMMON.NB_DC));
                add_w++;
                wdrive_iic_ext(add_w,HBYTE(_FROZEN_FRAME.COMMON.NB_DC));
                add_w++;
                wdrive_iic_ext(add_w,HLBYTE(_FROZEN_FRAME.COMMON.NB_DC));

                add_w++;
                //wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.NB_WUC));
                add_w++;
                //wdrive_iic(add_w,HBYTE(_FROZEN_FRAME.COMMON.NB_WUC));

                add_w++;
                wdrive_iic_ext(add_w,LBYTE(_FROZEN_FRAME.COMMON.NB_OBDC));
                add_w++;
                wdrive_iic_ext(add_w,HBYTE(_FROZEN_FRAME.COMMON.NB_OBDC));
                
                add_w++;
                //wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.ENG_RT));
                //2015-11-29 ENGINE RUNTIME
                wdrive_iic_ext(add_w,(_FROZEN_FRAME.COMMON.DTC_STA));
                
                add_w++;
                //wdrive_iic(add_w,HBYTE(_FROZEN_FRAME.COMMON.ENG_RT));
                add_w++;
                //wdrive_iic(add_w,HLBYTE(_FROZEN_FRAME.COMMON.ENG_RT));
                add_w++;
                //wdrive_iic(add_w,HHBYTE(_FROZEN_FRAME.COMMON.ENG_RT));
                
                add_w++;
                wdrive_iic_ext(add_w,LBYTE(_FROZEN_FRAME.COMMON.KEYON_T));
                add_w++;
                wdrive_iic_ext(add_w,HBYTE(_FROZEN_FRAME.COMMON.KEYON_T));
                add_w++;
                wdrive_iic_ext(add_w,HLBYTE(_FROZEN_FRAME.COMMON.KEYON_T));
                add_w++;
                wdrive_iic_ext(add_w,HHBYTE(_FROZEN_FRAME.COMMON.KEYON_T));
                
                /*
                add_w++;
                wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.NB_PUMP));
                add_w++;
                wdrive_iic(add_w,HBYTE(_FROZEN_FRAME.COMMON.NB_PUMP));
                add_w++;
                wdrive_iic(add_w,HLBYTE(_FROZEN_FRAME.COMMON.NB_PUMP));
                add_w++;
                wdrive_iic(add_w,HHBYTE(_FROZEN_FRAME.COMMON.NB_PUMP));
                
                add_w++;
                wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.T_2D5));
                
                add_w++;
                wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.N));
                add_w++;
                wdrive_iic(add_w,HBYTE(_FROZEN_FRAME.COMMON.N));

                add_w++;
                wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.ET));

                add_w++;
                wdrive_iic(add_w,LBYTE(_FROZEN_FRAME.COMMON.TSCR));
                */
            }
    }
}

/*
    读取冻结帧的总数
    
    并把状态载入到数组v_SPNX_forMIL[0][X]
    
    返回值：历史故障（冻结帧）的总数
*/


byte Read_Num_FrozenFrame(byte blnChkMILLamp) 
{
     byte i;
     byte nb_frame;
     byte  b;
     
    // ADD_1ST_DTC
     
     nb_frame=0;
     v_nb_DTC_history_milon=0;
     v_nb_DTC_history=0;


     //NUM_DTC_HISTORY
     for(i=0;i<NUM_DTC_HISTORY;i++)
     {


          Load_FrozenFrame(true,i,false);
          
          if(_FROZEN_FRAME.COMMON.FAI_ID<=NUM_DTC_SCR-1)
          {
              nb_frame++;  //需要考虑是否已经被清除的情况
          
              //Opr_Tmp_DTC(1,0);//追加最近一组数据
              /*
                  2015-9-17
                  这里有几个事情
                  1、需要判断故障是属于当前故障，还是历史故障，这个取决于MIL灯熄灭条件；
                  2、把当前故障标志位体现到：v_SPNX_forMIL中；
                          这里需要考虑一下EE中的当前故障，是放到v_SPNX_forMIL
                              还是仅仅把MIL点亮就够了
                              
                          因为放到v_SPNX_forMIL有个问题是，开钥匙后运行产生新的故障和之前的故障就分不清了
                          再关钥匙存储时，就区分不开了；    
              
              
              */

                  SetSpnStatus(false,_FROZEN_FRAME.COMMON.FAI_ID,DTC_TYPE_PENDING);
                  SetSpnStatus(false,_FROZEN_FRAME.COMMON.FAI_ID,DTC_TYPE_CONFIRM);


                  if(
                      _FROZEN_FRAME.COMMON.DTC_STA==DTC_STA_PENDING
                      ||
                      _FROZEN_FRAME.COMMON.DTC_STA==DTC_STA_CONFIRM
                      )
                  {
                      if(_FROZEN_FRAME.COMMON.DTC_STA==DTC_STA_PENDING) 
                      {
                           b=DTC_TYPE_PENDING;
                      } else b=DTC_TYPE_CONFIRM;
                      
                      SetSpnStatus(true,_FROZEN_FRAME.COMMON.FAI_ID,b);
                      
                  } 

                  //DTC_STA_CLR的都载入
                  Opr_Tmp_DTC(1,0);//1:APPEND,2:DELETE,3:MODIFY 
              
              v_nb_DTC_history++;
          
          }
          
     }
     
     return nb_frame;
     
/*
 _FROZEN_FRAME.FAI_ID;
    //_FROZEN_FRAME.NB_SPN;
 _FROZEN_FRAME.NB_DC;
 _FROZEN_FRAME.NB_WUC;
 _FROZEN_FRAME.NB_OBDC;
 _FROZEN_FRAME.ENG_RT;//发动机运行时间
 _FROZEN_FRAME.KEYON_T;//上电时间
 _FROZEN_FRAME.NB_PUMP;
 _FROZEN_FRAME.T_2D5;
 _FROZEN_FRAME.N;
 _FROZEN_FRAME.ET;
 _FROZEN_FRAME.TSCR;
    //_FROZEN_FRAME.type_dtc;//故障类型

*/          
}

//2016-2-23 追加

/*
  action
      1：追加
      2：删除
      3: 修改
  
  pos:
      删除的位置，或者追加的位置，即被操作的ID处于这个位置
      
  数据来源  
      _FROZEN_FRAME    
          
*/
byte Opr_Tmp_DTC(byte action,byte pos)//1:APPEND,2:DELETE,3:MODIFY,4:REPLACE IF EXIST,ELSE APPEND 
{
     byte i;
     byte blnR;
     byte blnE;


     blnR=true;
     
     if(action==1||action==4)
     {
          blnE=false;
          if(v_ub_tmp_dtc>0&&(action==4))
          {
              for(i=0;i<v_ub_tmp_dtc;i++)
              {
                  if(_TMP_DTC[i].COMMON.FAI_ID==_FROZEN_FRAME.COMMON.FAI_ID)
                  {   //已经存在了，直接更改原信息
                      _TMP_DTC[i].COMMON.DTC_STA =_FROZEN_FRAME.COMMON.DTC_STA;
                      _TMP_DTC[i].COMMON.NB_DC   =_FROZEN_FRAME.COMMON.NB_DC;
                      _TMP_DTC[i].COMMON.NB_OBDC   =_FROZEN_FRAME.COMMON.NB_OBDC;
                                  
                      blnE=true;
                  }
              }
          }

          if(v_ub_tmp_dtc<NUM_DTC_HISTORY&&!blnE)
          {
                  
              _TMP_DTC[v_ub_tmp_dtc].COMMON.FAI_ID  =_FROZEN_FRAME.COMMON.FAI_ID;
              _TMP_DTC[v_ub_tmp_dtc].COMMON.DTC_STA =_FROZEN_FRAME.COMMON.DTC_STA;
              _TMP_DTC[v_ub_tmp_dtc].COMMON.NB_DC   =_FROZEN_FRAME.COMMON.NB_DC;
              _TMP_DTC[v_ub_tmp_dtc].COMMON.NB_OBDC   =_FROZEN_FRAME.COMMON.NB_OBDC;
              
              v_ub_tmp_dtc++;
          } 
          else 
          {
              if(!blnE)blnR=false;
          }
     } 
     else if(action==2)
     {    //DELETE
          if(pos<v_ub_tmp_dtc){
            
              if(pos<NUM_DTC_HISTORY-1) 
              {
                  for(i=pos;i<NUM_DTC_HISTORY-1;i++)
                  {
                      _TMP_DTC[i].COMMON.FAI_ID  =_TMP_DTC[i+1].COMMON.FAI_ID;
                      _TMP_DTC[i].COMMON.DTC_STA =_TMP_DTC[i+1].COMMON.DTC_STA;
                      _TMP_DTC[i].COMMON.NB_DC   =_TMP_DTC[i+1].COMMON.NB_DC;
                      _TMP_DTC[i].COMMON.NB_OBDC   =_TMP_DTC[i+1].COMMON.NB_OBDC;
                  }
                  
                  v_ub_tmp_dtc--;
                  
              }
          }
     } 
     else if(action==3)
     {    //MODIFY
          if(pos<v_ub_tmp_dtc)
          {
              _TMP_DTC[pos].COMMON.FAI_ID  =_FROZEN_FRAME.COMMON.FAI_ID;
              _TMP_DTC[pos].COMMON.DTC_STA =_FROZEN_FRAME.COMMON.DTC_STA;
              _TMP_DTC[pos].COMMON.NB_DC   =_FROZEN_FRAME.COMMON.NB_DC;
              _TMP_DTC[pos].COMMON.NB_OBDC =_FROZEN_FRAME.COMMON.NB_OBDC;
          }
     } 
     
     return blnR;
     
}



/*
    根据EE中的情况，确定MIL灯的状态
    1）MIL是否需要点亮；
    2）MIL是否满足熄灯条件；
        0:无故障后立即
        1:1DC后
        2:3DC后
        3:3OBD后
        
        
    _FROZEN_FRAME.COMMON.FAI_ID    
    
    v_st_mil[IDX_MAX_AFTER_PWRON]
*/
/*
void CheckMILOn_by_FROZEN_FRAME(void)
{
    byte mil;
    byte scr;
    byte miloff_on;
    
    mil=        (SCRDTC_ST[_FROZEN_FRAME.COMMON.FAI_ID]>>7)&0x01;
    scr=        (SCRDTC_ST[_FROZEN_FRAME.COMMON.FAI_ID]>>5)&0x03;
    miloff_on=  (SCRDTC_ST[_FROZEN_FRAME.COMMON.FAI_ID]>>3)&0x03;


    switch(miloff_on)
    {
        case 0:
            mil=0;    
        break;
        
        case 1:
           // if(v_DC>_FROZEN_FRAME.COMMON.NB_DC+1)mil=0;
        break;
        
        case 2:
           // if(v_DC>_FROZEN_FRAME.COMMON.NB_DC+3)mil=0;
        break;
        
        case 3:
           // if(v_OBDC>_FROZEN_FRAME.COMMON.NB_OBDC+3)mil=0;
        break;
      
    }
    
    if(mil>0)
    {
        v_nb_DTC_history_milon++;
        
    }
    
    //SetSpnStatus(byte blnSet,byte id,byte blnCur);
    SetSpnStatus(true,_FROZEN_FRAME.COMMON.FAI_ID,false);
    
    if(v_st_mil[IDX_MAX_AFTER_PWRON]<mil)v_st_mil[IDX_MAX_AFTER_PWRON]=mil;        
}
*/

/*
    判断在故障历史中是否有当前故障号的故障
    
    返回值：在存储区中的位置
            255--表示不存在

*/

/*
    判断在故障历史中是否有当前故障号的故障
    
    返回值：在存储区中的位置
            255--表示不存在

*/
byte Search_DTC_inHistory(byte dtc_tobe_search)
{
     byte i;
     
     byte st_pos;
     byte ed_pos;
     
     //需要先判断搜索的范围，与是否是可擦除相关
         if(SCRDTC_ST[dtc_tobe_search]&0x01)
         {
            //不可擦除
            st_pos=ID_DTC_HISTORY_ST_NOCLR;
            ed_pos=ID_DTC_HISTORY_ED_NOCLR;
         } 
         else
         {
            //可擦除
            st_pos=ID_DTC_HISTORY_ST_CLR;
            ed_pos=ID_DTC_HISTORY_ED_CLR;
         }
     
     
     //NUM_DTC_HISTORY
         for(i=st_pos;i<=ed_pos;i++)
         {
              /*
              Load_FrozenFrame(true,i,true);//读取对应帧的ID
              
              //if(_FROZEN_FRAME.COMMON.FAI_ID!=INVALID_FAI_ID)
              //if(_FROZEN_FRAME.COMMON.FAI_ID!=dtc_tobe_search)//2015-9-7 CHANGE 
              //if(_FROZEN_FRAME.COMMON.FAI_ID==dtc_tobe_search)//2015-9-21 纠正
              //if(_FROZEN_FRAME.COMMON.FAI_ID==M1_DTCNO[dtc_tobe_search])//2015-9-21 纠正
              if(_FROZEN_FRAME.COMMON.FAI_ID==dtc_tobe_search)//2015-11-30
              {
                    return i;
              }
              */
              
              
         }
     
     //return 255;
     //2015-9-7
     return INVALID_FAI_ID;
     
}




/*
    关钥匙时，保存故障码

    先遍历故障码，按FAI DTC号，从小到大，
    每遍历到一个，就
        1）先确认是否已经在EEPROM中；
        2）把他存储到当前的空位；
        3）没有空位就按顺序覆盖；
    2015-9-17
*/
byte SaveFrame_whenKeyoff(void)
{
    byte bpos;
    byte id;

    byte i;
    
    Save_FrozenFrame();//2015-11-28
              
        _FROZEN_FRAME.COMMON.NB_DC=0;//v_DC;//rd_byAddGrp(3,add_w);
    
        _FROZEN_FRAME.COMMON.NB_WUC=0;//v_WUC;//rd_byAddGrp(2,add_w);
        
        _FROZEN_FRAME.COMMON.NB_OBDC=0;//v_OBDC;//rd_byAddGrp(2,add_w);
        
        _FROZEN_FRAME.COMMON.ENG_RT=0;//v_ENG_t_s;//0;//rd_byAddGrp(4,add_w);
        
        _FROZEN_FRAME.COMMON.KEYON_T=0;//v_KEY_t_s;//rd_byAddGrp(4,add_w);
        
        _FROZEN_FRAME.COMMON.NB_PUMP=0;//v_nb_DOSER;//rd_byAddGrp(4,add_w);
       
   
    

    Clr_DTC_list();
    
    if(v_ub_tmp_dtc>0)
    {
        for(i=0;i<v_ub_tmp_dtc;i++){
            
            _FROZEN_FRAME.COMMON.FAI_ID   =_TMP_DTC[i].COMMON.FAI_ID;
            _FROZEN_FRAME.COMMON.DTC_STA  =_TMP_DTC[i].COMMON.DTC_STA ;
            _FROZEN_FRAME.COMMON.NB_DC    =_TMP_DTC[i].COMMON.NB_DC ;
            _FROZEN_FRAME.COMMON.NB_OBDC    =_TMP_DTC[i].COMMON.NB_OBDC ;
            
            bpos=Search_pos_to_saveDTC(_FROZEN_FRAME.COMMON.FAI_ID);

            if(bpos !=INVALID_FAI_ID)
            {
                Load_FrozenFrame(false,bpos,false);
            }
        }
    } 
    else
    {
        
    }

    
}


//2016-2-23 ADD FROM SCR

/*
    存储冻结帧
    在关闭钥匙时，优先存储冻结帧
    2015-11-28
*/
void Save_FrozenFrame(void) 
{
    if(_FROZEN_FRAME.COMMON.FAI_ID_FROZEN!=INVALID_FAI_ID)
    {   //说明当前有新的冻结帧
        
          //wdrive_iic(ADD_FROZEN_ID,   (_FROZEN_FRAME.COMMON.FAI_ID_FROZEN+128));
          wdrive_iic(ADD_FROZEN_ID,   (_FROZEN_FRAME.COMMON.FAI_ID_FROZEN));//2016-3-5 AT Save_FrozenFrame
          wdrive_iic(ADD_FROZEN_T,    (_FROZEN_FRAME.COMMON.T_2D5));
          wdrive_iic(ADD_FROZEN_N,    LBYTE(_FROZEN_FRAME.COMMON.N));
          wdrive_iic(ADD_FROZEN_N+1,  HBYTE(_FROZEN_FRAME.COMMON.N));
          wdrive_iic(ADD_FROZEN_ET,   (_FROZEN_FRAME.COMMON.ET));
          wdrive_iic(ADD_FROZEN_TSCR,   LBYTE(_FROZEN_FRAME.COMMON.TSCR));
          wdrive_iic(ADD_FROZEN_TSCR+1, HBYTE(_FROZEN_FRAME.COMMON.TSCR));
                    
    }
        
}



/*
    寻找存储DTC的位置    
*/
byte Search_pos_to_saveDTC(byte id) 
{
    byte i;
     
    unsigned long int t_min;
    byte p_oldest;

     byte st_pos;
     byte ed_pos;
     
     //需要先判断搜索的范围，与是否是可擦除相关
        if(SCRDTC_ST[id]&0x01)
        {
            //不可擦除
            st_pos=ID_DTC_HISTORY_ST_NOCLR;
            ed_pos=ID_DTC_HISTORY_ED_NOCLR;

            p_oldest=v_nb_dtc_noneclr+st_pos;
          
        } 
        else
        {
            //可擦除
            st_pos=ID_DTC_HISTORY_ST_CLR;
            ed_pos=ID_DTC_HISTORY_ED_CLR;
            
            p_oldest=v_nb_dtc_clr+st_pos;
        }
         
        if(p_oldest>ed_pos)p_oldest=ed_pos;
     
    
    return p_oldest;     

    
    /*
    p_oldest=st_pos;
    
    
    //没有出现在EE中，则找空位填入
    if(Read_Num_FrozenFrame(false)< NUM_DTC_HISTORY)
    {
        //位置不满
        for(i=st_pos;i<=ed_pos;i++)
        {
            Load_FrozenFrame(true,i,true);//读取对应帧的ID
            
            if(_FROZEN_FRAME.COMMON.FAI_ID>NUM_DTC_SCR-1)//2015-9-7 CHANGE ,2015-11-28 COR, 2015-11-30
            {
                  p_oldest= i;
                  break;
            }
            
        }
        
        
    } 
    else 
    {
        //位置已满
        t_min=0xfffffffful;
        for(i=st_pos;i<=ed_pos;i++)
        {
            Load_FrozenFrame(true,i,true);//读取对应帧的ID
            
            if(_FROZEN_FRAME.COMMON.ENG_RT<t_min)//2015-9-7 CHANGE 
            {
                  t_min=_FROZEN_FRAME.COMMON.ENG_RT;
                  p_oldest=i;
            }
            
        }
        
        
    }
    
    return p_oldest;
    */
}

/*
    blnFromst--从头开始
    blnSave----保存还是取出
    data 
    
    返回值：取出的数据    

*/
void SaveToDTCout(byte blnFromst,byte blnSave,byte * pd) 
{
    byte i;
    
    if(blnFromst) {
        p_DTCout=0;
        if(blnSave)
        {
            for(i=0;i<NUM_DTC_OUT;i++){
                b_DTC_out[p_DTCout]=0;    
            }
        }
    }
    
    if(blnSave)
    {
        b_DTC_out[p_DTCout]=*pd;
    } 
    else
    {
        *pd=b_DTC_out[p_DTCout];
    }
    
    p_DTCout++;
    if(p_DTCout>=NUM_DTC_OUT)p_DTCout=NUM_DTC_OUT-1;    
    
            
}


/*
    清除故障码历史
    
    所有的都清除
    2015-9-20，暂时设置为清除所有故障码


    blnReset:
        0--清除可清除故障，并把不可清除故障时间赋0
        1--包括不可清除故障在内，所有都复位

*/
void ClearDTCHistory(byte blnReset) 
{
    byte i;
    
    word add_st;
    word add_w;


    for(i=0;i<NUM_DTC_HISTORY;i++)
    {
          CLR_WDG
          add_st=(word)i*BYTES_PER_FRAME_FROZEN;

          add_st+=(word)ADD_1ST_DTC; //2015-12-21
          
          add_w=add_st+POS_OF_FAI_ID;
          
          _FROZEN_FRAME.COMMON.FAI_ID=INVALID_FAI_ID;

          //把ID都写为255
          if(blnReset|| (i>=ID_DTC_HISTORY_ST_CLR&&i<=ID_DTC_HISTORY_ED_CLR))
          {
              wdrive_iic(add_w,_FROZEN_FRAME.COMMON.FAI_ID);
          }
          //把ENG 时间都写为0，因此判断故障时，需要
          else
          {
            
              add_w+=12;
              wdrive_iic(add_w,0);
              add_w++;
              wdrive_iic(add_w,0);
              add_w++;
              wdrive_iic(add_w,0);
              add_w++;
              wdrive_iic(add_w,0);
          }
          
    }
    
    
    //清除冻结帧
    wdrive_iic(ADD_FROZEN_ID, (INVALID_FAI_ID));
    _FROZEN_FRAME.COMMON.FAI_ID_FROZEN=INVALID_FAI_ID;//2016-3-5 add in ClearDTCHistory
    
    v_ub_tmp_dtc=0;
    
    v_nb_dtc_noneclr=0;        
    v_nb_dtc_clr=0;

    wdrive_iic(ADD_DTC_NUM_NONECLR,v_nb_dtc_noneclr);//
    wdrive_iic(ADD_DTC_NUM_CLR,v_nb_dtc_clr);//
}

/*
    关于SCR灯与MIL灯点亮的说明
    
    
    SCR灯由v_st_scr[1]控制；1：代表当前值
    MIL灯由v_st_MIL[2]控制；2：代表本次上电后的最大值；
    
    注意，SCR和MIL用的是不一样的，
    

*/

//2016-2-27 ADD

/*
    读取在内部EEPROM存储的长整型数据
    
    2015-10-20 改为外部EEPROM
*/
dword rd_byAddGrp(byte nbbytes,word add_1st) 
{
    byte i;
    byte badd;
    
    dword ladd;
    
    ladd=0;
    for(i=0;i<nbbytes;i++)
    {
        //badd=rdrive_iic(add_1st+i); //可以用       
        badd=rdrive_iic_ext(add_1st+i); //???
        if(i==0)ladd+=badd;
        if(i==1)ladd+=(((dword)badd<<8)&0x0000ff00);
        if(i==2)ladd+=(((dword)badd<<16)&0x00ff0000);
        if(i==3)ladd+=(((dword)badd<<24)&0xff000000);
        
    }
    
    if(nbbytes==1)
    {
        if(ladd==0xff)ladd=0;
    }
    if(nbbytes==2)
    {
        if(ladd==0xfffful)ladd=0;
    }
    if(nbbytes==3)
    {
        if(ladd==0xfffffful)ladd=0;
    }
    if(nbbytes==4)
    {
        if(ladd==0xfffffffful)ladd=0;
    }
    
    
    return ladd;
    
}

/*
    
*/
void Clr_DTC_list(void) 
{ 
    byte i;
    
    _FROZEN_FRAME.COMMON.FAI_ID=INVALID_FAI_ID;
    
    //清除EEPROM的所有故障
    for(i=0;i<NUM_DTC_HISTORY;i++){
      
        Load_FrozenFrame(false,i,true);
    }
    
    //v_ub_tmp_dtc=0;
    
    v_nb_dtc_noneclr=0;        
    v_nb_dtc_clr=0;

    wdrive_iic(ADD_DTC_NUM_NONECLR,v_nb_dtc_noneclr);//
    wdrive_iic(ADD_DTC_NUM_CLR,v_nb_dtc_clr);//
    
}

/*
=================================================================
============命令定义=============================================
=================================================================
*/

