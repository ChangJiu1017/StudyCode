/*HEAD
 ***********************************************************************
 * Filename	:tmp.c
 * Function	:temperature calculation.		
 * User		:xielian
 * Date		:2017.1.17
 * Version	:V8.1.1
 *
 *** History ***
 * V8.0.0	2017.1.17	zhangping 
 *		Base function.
 * V8.1.0	2017.1.17	zhangping 
 *		TA来源根据M0_SOURCE_TA配置，删除IS_TA_ONBOARD.
 *
 * V8.1.1	2017.1.17	xielian 
 *		bugfix for v_twb calculation at engine stoped condition. 
 * V8.2.0	2019.10.29	xielian 
 *		兼容江苏宗申温度传感器. 
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "add.h"

#include "tmp.h"
#include "time_ccl.h"
#include "ee.h"
#include "look_up.h"
#include "dtc.h"

#include "afr_starts.h"

#define TAS_100K 0
#define TAS_50K 1

#define TAS_TYPE  TAS_50K

#define M0_N0_M16 1516//1500rpm
#define M0_N0 95//1500rpm
//#define M0_IBP			25
#define M0_m0 100


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_TMP_STATUS;
//#pragma DATA_SEG DEFAULT

volatile byte v_tair_ad;// @0x200;           //空气温度采样值

byte v_tair_inuse;        //空气温度使用值：save the v_Tair_inuse	used for	map_tinj modify	
byte v_teng_ad;           //RZT采样值	
byte v_teng_inuse;        //发动机缸头温度使用值
byte v_teng_odrh5;
sword v_te;         //对应于AD值的温度值
sword v_twb;        //用于计算V_TW的基础值

word v_teng_10bit;
word v_tair_10bit;

sbyte V_Ta;      //单位：度数
sword V_Tw;      //单位：度数

byte v_G_M128;    //按G-TA表查到的G

sbyte v_dXZ;   //2012-10-10

byte v_Gx_M128;//实际的G

byte VF_TW_1ST;

/*---------THE FOLLOWING ARE local macros--------------------------------------------------*/
/*
    原始公式：XZ=(0.00024*n+0.107237)(0.000152*TPSD+0.000396)

    M0_B1_M8192=2
    M0_B2_M128=14
    M0_B3_M8192=1
    M0_B4_M8192=3

    XZ_M131072=(M0_B1_M8192_M4*V_rev/64+M0_B2_M128_M4)*(M0_B3_M8192*V_TPS_odr+M0_B4_M8192)/(8*4)                                                      
*/
    
/*    
#define    M0_XZ_M131072 5//M0_XZ_M16384   8   //5 TO 20

#define   M0_C1_M33554432 5
#define   M0_C2_M131072 5
#define   M0_C3_M33554432 9
#define   M0_C4_M131072 8
#define M0_XX_M4096_M273  59//27
*/

/*
#define    M0_B1_M8192_M4  10//11//2
#define    M0_B2_M128_M4  38//39//14
#define    M0_B3_M8192 4//2
#define    M0_B4_M8192 7//3
*/

/*
    XZ=(C1*TPS+C3)*n+C2*TPS+C4
    C1=0.0000001525	  M0_C1_M33554432=5
    C2=0.0000371	    M0_C2_M131072=5
    C3=0.00000026535	M0_C3_M33554432=9
    C4=0.000064554	  M0_C4_M131072=8
    2012-9-14 CHANGE FROM B1 TO C1
    
XZ_M131072=(M0_C1_M33554432*V_TPS_odr+M0_C3_M33554432)*V_rev/256+M0_C2_M131072*V_TPS_odr+M0_C4_M131072

*/


//#define M0_N0_M16     1538
#define M0_N0_M2     192         //2012-10-19
//#define M0_FMF0_D4    25
#define M0_FMF0_D4    32  //2012-10-31 为了简化运算，定义为from 25 to 32
#define M0_TMIN       60

//#define M0_XX_M1024_M273  8
/*DATA FOR MODEL TW*/			

/*  2012-8-17 更改为按循环计
XX=0.0000528   M0_XX_M273_M4096=59
XZ=(B1*n+B2)(B3*TPSD+B4)
B1=0.000305    M0_B1_M32768=10
B1=0.0742      M0_B2_M512=38
B1=0.0005      M0_B3_M8192=4
B1=0.00087     M0_B4_M8192=7			
*/	
//	M2_TW_STABLE（火花塞热平衡温度theta,转速单位为15.6RPM）：		
const byte M2_TW_STABLE=200;			

/*TES 度数与采样值之间的转换*/			
#define M0_TES_S_DEG_UB 8
const byte M0_TES_DEG_BASIC=30;			

#ifdef TE_10K	//10k缸温

	const byte M1_TES_S_DEG[9][2]=			
	{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
	    3	,9,//-21	
	    4	,12,//	-18
	    5	,17,//	-13
	    8	,	26 	,//-4
	    15	,	38 	,//8
	    40	,	61 	,//31
	    180	,	138 	,//108
	    210	,	169 	,//139
	    234	,	220 ,	//190		211 181    
	};
#endif

/*		
#ifdef	TE_2K	//2k水温
	const byte M1_TES_S_DEG[9][2]=			
	{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
	    3	,	9	,//-21	
	    4	,	12	,//	-18
	    5	,	17	,//	-13
	    8	,	26 	,//-4
	    15	,	38 	,//8
	    40	,	61 	,//31
	    108	,	90	,			//20210720 chenwei add 中间一档，提高分辨率
	    180	,	138 	,//108
	    210	,	169 	,//139
		 
	};

#endif*/

//20210920 chenwei add 环松2K水温
#ifdef	TE_2K

	const byte M1_TES_S_DEG[9][2]=			
	{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
	    3	,	9	,//-21	
	    4	,	12	,//	-18
	    5	,	17	,//	-13
	    8	,	26 	,//-4
	    15	,	38 	,//8
	    40	,	61 	,//31
	    108	,	90	,			//20210720 chenwei add 中间一档，提高分辨率
	    180	,	138 	,//108
	    210	,	169 	,//139
		 
	};

#endif


//#endif	

/*#ifdef SY_JSZS

const byte M1_TES_INUSE_JSZS[9][2]=			
{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
    9	,2,//-30
    25	,6,//-10
    53	,18,//10
    93	,	40 	,//30
    176	,	115 	,//70
    214	,	169 	,//100
    238	,	212 	,//140
    247	,	231 	,//180
    248	,	234 ,	//190
};

#endif*/

#ifdef SYS_TRADITIONAL
//这个表格是新温度采样和老温度采样的对应关系
	#ifdef TE_10K	//10k缸温
		const byte M1_TES_INUSE_JSZS[9][2]=			
		{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
			   5	,	2,		//-25
			   8	,	5,		//-15
			   23	,	13,//5
			   35	,	23 ,//15
			   58	,	40 	,//30
			   168	,	135 	,//80
			   214	,	183 	,//110
			   238	,	218 	,//150
			   247	,	234 ,	//190	
		};

	#endif

	/*#ifdef	TE_2K	//2k水温
		const byte M1_TES_INUSE_JSZS[9][2]=			
		
		{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
			   30, 		2, 
			   46, 		5,		
			   92, 		13,		
			   118, 	23,		
			   155, 	40,	
			   208,		108,
			   226,		135,
			   242, 	183,	
			   250, 	218,
			   
		};

	#endif*/

	#ifdef	TE_2K	//20210920 chenwei add 环松2K水温
	
		const byte M1_TES_INUSE_JSZS[9][2]=			
		
		{//  AD 8BIT, DEG+M0_TES_DEG_BASIC			
			/*   12, 		2, 
			   23, 		5,		
			   88, 		13,	//92, 	13,		
			   100, 	23,	//118, 	23,		
			   142, 	40,	//155, 	40,	
			   205,		108,
			   220,		135,
			   240, 	183,	
			   250, 	218,*/

				10,		 2, 	  			//-25
				21, 	 5, 	 			//-18
				70, 	 13, //92,	 13,	//-4	 
				90,	 	 23, //118,  23,	//15
				142,	 40, //155,  40, 	//31
				205,	 108,				//68
				225,	 135,				//83
				243,	 183,	 			//108
				252,	 218,				//139

			   
		};

	#endif


#endif		



/*
//2013-2-22 R75=100K+100KAT25C				B=3700
const byte M1_TAS_S_DEG[9][2]=				
{				
    23	,	9	,//	-21
    40	,	20	,//	-10
    61	,	30	,//	0
    86	,	40	,//	10
    113	,	50	,//	20
    139	,	60	,//	30
    163	,	70	,//	40
    199	,	90	,//	60
    221	,	110	,//	80
};				
*/
//2013-2-22 R75=100K+100KAT25C				B=3700
//2013-3-3 手工调整
/*const byte M1_TAS_S_DEG[9][2]=				
{				
    23	,	9	,//	-21　　// FROM 23 TO 27 //2013-3-25 CHANGE BACK
    40	,	20	,//	-10  //FROM 40 TO 43
    61	,	30	,//	0
    86	,	40	,//	10
    113	,	50	,//	20
    139	,	60	,//	30
    163	,	70	,//	40
    199	,	90	,//	60
    221	,	110	,//	80
};				
*/
//2013-3-25 R75=100K+100KAT25C				B=3900
/*const byte M1_TAS_S_DEG[9][2]=				
{				
20	,	9	,//	-21
37	,	20	,//	-10
58	,	30	,//	0
84	,	40	,//	10
112	,	50	,//	20
140	,	60	,//	30
165	,	70	,//	40
202	,	90	,//	60
224	,	110	,//	80
};*/

//CR 系统
const byte M1_TAS_S_DEG[9][2]=				
{				
39	,	5	,//	-25
73	,	20	,//	-10
100	,	30	,//	0
128	,	40	,//	10
153	,	50	,//	20
176	,	60	,//	30
194	,	70	,//	40
219	,	90	,//	60
233	,	110	,//	80
};				

//2016-1-11
const byte M1_TPCB_S_DEG_PI[9][2]=				
{//采样值，TAMB温度+40度；即查表后需要减40				
7 	,	16	,//	-20
12 	,	26	,//	-10
16 	,	31	,//	-5
21 	,	36	,//	0
27 	,	41	,//	5
34 	,	46	,//	10
51 	,	56	,//	20
95 	,	76	,//	40
181 	,	116	,//	80
};				


//2012-12-12 ADD      2013-6-18
const byte M1_TAS_MCU_DEG[9][2]=				
{			//	deg
    44	,	9	,//	-21
    51	,	20	,//	-10
    58	,	30	,//	0
    65	,	40	,//	10
    71	,	50	,//	20
    78	,	60	,//	30
    86	,	70	,//	40
    101	,	90	,//	60
    116	,	110	,//	80
};				

//2012-11-19
#define M0_TAS_MAX_50K  242
#define M0_TAS_MIN_50K  5
#define M0_TAS_MAX_INUSE_50K  230
#define M0_TAS_DEFAULT_50K   80
#define M0_D_TES_TAS  15//TES与TAS截距的偏移

#define TES_DEG_MIN -25//-20
#define TES_DEG_MAX 190
#define TW_DEG_MAX  170 //预测的最大值；

#define M0_TMP_TH_M4  15//  TMP采样值小于该值时，需要乘4

#define TA_DEG_MAX_ADD30 90


//#define  M0_D2_M4096 11	
#define  M0_D2_M4096 68	
//温度查表结果需要减去M0_TES_DEG_BASIC			
#define M0_TW_MAX 230
           
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte tamp_yc_last;	//equ	0x46	    //;上次次由RZT预测的空气温度，从EE中读取的		
//byte RZT_st;	        //EQU	6Dh					//;疑问？	STORING	THE	v_Teng_inuse	OF	LAST	STOP	LOADED	WHEN	INJECTING	AT	ST	
//byte RZT_1st_afterKeyOn;//2008-11-6 开钥匙后获得的第一次RZT采样值，用于缸头温度预测；

//2009-12-11 ADD 
byte nb_teser;    
byte nb_taser;

byte nb_tpcber;//2016-1-9

//byte nb_tasGTtes;   //空气温度大于岗头温度
#define NB_TMPER_LIMIT 5
//byte teng_whenKeyOn;//2010-1-29

byte Ni_M16 ;
unsigned long int N_add_M16;
word X_M64;


word exp_M128;
sword dtw0;
word Tw_brf_M16;
sword Tw_af_M16;
byte Tw_stable;

word X_add_M64;

word Xi_M65536;
unsigned long int X_add_M65536;

//#define    M0_XZ_M131072 5//M0_XZ_M16384   8   //5 TO 20

/*----------2013-1-21　模型------------------------------------------*/
//#define M0_XZ_MT0_M256 348
//#define M0_XZ_MT0_M128 174//348  2013-1-23 replaced by M0_UA_M128

#define M0_UB_SQ  59

const sword M1_SQ_M2048[60][2]={					
	1	,	2048	,	//
	50	,	289	,	//
	100	,	204	,	//
	150	,	167	,	//
	200	,	144	,	//
	250	,	129	,	//
	300	,	118	,	//
	350	,	109	,	//
	400	,	102	,	//
	450	,	96	,	//
	500	,	91	,	//
	550	,	87	,	//
	600	,	83	,	//
	650	,	80	,	//
	700	,	77	,	//
	750	,	74	,	//
	800	,	72	,	//
	850	,	70	,	//
	900	,	68	,	//
	950	,	66	,	//
	1000	,	64	,	//
	1050	,	63	,	//
	1100	,	61	,	//
	1150	,	60	,	//
	1200	,	59	,	//
	1250	,	57	,	//
	1300	,	56	,	//
	1350	,	55	,	//
	1400	,	54	,	//
	1450	,	53	,	//
	1500	,	52	,	//
	1600	,	51	,	//
	1650	,	50	,	//
	1700	,	49	,	//
	1750	,	48	,	//
	1850	,	47	,	//
	1900	,	46	,	//
	2000	,	45	,	//
	2100	,	44	,	//
	2200	,	43	,	//
	2300	,	42	,	//
	2400	,	41	,	//
	2500	,	40	,	//
	2650	,	39	,	//
	2800	,	38	,	//
	2950	,	37	,	//
	3100	,	36	,	//
	3250	,	35	,	//
	3450	,	34	,	//
	3650	,	33	,	//
	3900	,	32	,	//
	4100	,	31	,	//
	4400	,	30	,	//
	4700	,	29	,	//
	5000	,	28	,	//
	5350	,	27	,	//
	5800	,	26	,	//
	6250	,	25	,	//
	6750	,	24	,	//
	7300	,	23	,	//
	};//				

long int v_tao_M16;
long int v_tao_s_M16 ;

long int v_sigma_M2048;
sword v_sq;//2013-1-21

sword tw_stop;//2013-1-21

//2013-1-27
#define 	M0_W_M64	33			
//#define 	M0_XZ_27PER	95	//	0.27	352
#define 	M0_XZ_25PER	88	//	0.27	352
//#define 	M0_XZ_17PER	59	//	0.17	352


#define 	M0_XZ_10PER	35	//	0.1	352
#define 	M0_XZ_15PER	53	//	0.15	352        

#define 	M0_XZ_20PER	70	//	0.1	352

//#define 	M0_XZ_15PER	43	//?	352        

#define 	M0_TCR	25			
#define 	M0_EF_FACTOR	60			

//#define 	M0_TCR_M64	1600
//#define   M0_TCR_M128 3200
#define   M0_TCR_M256 6400

long int v_Tg_Sum256; //v_Tg_Sum256=M0_TCR_M128;

sword tw_whenKeyoff;//2016-8-13


/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/
void FI_TMP_AfterReset()
{
    byte w;    

    //2010-1-19 
    if(VF_POR_HARD)
    {   
        VF_1STGET_TMP=false;//2008-11-6     //2010-1-10 delete
        VF_TESLTTASYC_TMP=false;
        v_tao_M16=0;//2013-1-25
        
        v_tao_s_M16=0;//2016-4-3 add 
        
        dtw0=0;
        v_twb=0;
        tw_whenKeyoff=25; //VF_POR_HARD
		
		VF_TW_1ST=true;
    }
    
    //2008-11-6 add condiction
    if(VF_VPOR_HARD)
    {
        nb_teser=nb_taser=0;//nb_tasGTtes=0;
        nb_tpcber=0;
        V_TMP_STATUS&=0x01;//2010-1-19 
        VF_TESSTABLE_TMP=false;//TES未达到稳定状态  

        v_R_d4=BYPTR_M0_R_D4;//M0_R_d4;
        V_Tw=tw_whenKeyoff;//2016-8-13  VF_VPOR_HARD
        
    }
    else{
        V_TMP_STATUS&=0x41;//2010-1-10  //　2010-3-13　VF_TESHIGHFORPUMP_TMP也需要保留  
    }

    //功能：上电复位后，从EE中获得TAMP_YC_LAST值；如果从两个地址读出的数值一致，则使用读出值，否则使用默认值25;
    if(VF_VPOR_HARD||tamp_yc_last==0)//2008-3-12张平增加条件
    {
        w=Load_Data_EE(EE_TAMP_SAVE);
        tamp_yc_last=Load_Data_EE(EE_TAMP_SAVE+1)-1;//2010-1-4 add data(xb)=data(x)+1;
        if(w!=tamp_yc_last||tamp_yc_last==0){
          
            #if TAS_TYPE==TAS_100K
                tamp_yc_last=25;   
            #else
                tamp_yc_last=M0_TAS_DEFAULT_50K; 
            #endif
        }
        VF_TCHANGED_TMP=false;
    }

        X_add_M65536=0;
    
    Xi_M65536=2;

    v_dXZ=0;
    v_sq=0;
    
    return;
}

//发动机启动时的初始化
void FI_TMP_beforeStart(void)
{
    //Tw0=V_Tw;        
    X_M64=0;
    Tw_brf_M16=0;
    
    N_add_M16=0;

    Tw_stable=M2_TW_STABLE;
    VF_TESSTABLE_TMP=false;

    //v_tao_M16=v_tao0_M16；
    v_sigma_M2048=0;
    
    v_Tg_Sum256=M0_TCR_M256;
    
    v_tao_M16=0;//2016-4-5
    

}

//功能：根据空气和发动机温度采样值，诊断温度信号是否有误，并获得空气和发动机温度的使用值；
//形参dres_tair_8bit－－空气温度8位采集结果?
//形参dres_teng_8bit－－发动机温度8位采集结果
//形成　rev　=rpm/15.6 //2012-8-1 add 
void FG_TMP(word adres_tair_10bit,word adres_teng_10bit,word rev,word fmf,byte tpd)
{
    //byte w;
    word w;
    sword sw;  //2010-3-13 纠错
    
    byte blnM4;
    
    if (BYPTR_MV_EXHAUST>PL150)
        {fmf*=2;}//2012-11-10 温度计算时，大流量需要乘2

    
    v_teng_10bit=1023-adres_teng_10bit;
    v_tair_10bit=1023-adres_tair_10bit;
    
    if(VF_MMOVE_ENG)
    {
       //获取温度的AD采集结果
        v_tair_ad=(byte)((v_tair_10bit/4+v_tair_ad)/2);
        v_teng_ad=(byte)((v_teng_10bit/4+v_teng_ad)/2);

    }
    else
    {
        //在发动机静止时，获取温度的AD采集结果
        v_tair_ad=v_tair_10bit/4;
        v_teng_ad=v_teng_10bit/4;

    }
    
    
    //测试空气温度采样值TAIR,若超限，则使用预测值tamp_yc as 使用值tamp；
    //空气温度使用值
    #if TAS_TYPE==TAS_100K

        if(v_tair_ad>M0_TAS_MAX||v_tair_ad<M0_TAS_MIN){
        		v_tair_inuse=tamp_yc_last;
        		if(nb_taser<255)nb_taser++; //,NB_TMPER_LIMIT
        }
        else{
            nb_taser=0;
            v_tair_inuse=v_tair_ad;
        }
        //考虑到故障的情况，确定AIR使用的AD值；
        if(v_tair_inuse>M0_TAS_MAX_INUSE) v_tair_inuse=M0_TAS_MAX_INUSE;//2010-3-21 move here
    #else   //2012-11-19 TAS限值
        if(v_tair_ad>M0_TAS_MAX_50K||v_tair_ad<M0_TAS_MIN_50K){
        		v_tair_inuse=tamp_yc_last;
        		if(nb_taser<255)nb_taser++; //,NB_TMPER_LIMIT
        }
        else{
            nb_taser=0;
            v_tair_inuse=v_tair_ad;
        }
        //考虑到故障的情况，确定AIR使用的AD值；
        if(v_tair_inuse>M0_TAS_MAX_INUSE_50K) v_tair_inuse=M0_TAS_MAX_INUSE_50K;//2010-3-21 move here
   
        if(v_tair_ad<M0_TAS_MIN_50K)Accu_AD_Mv(MV_TAS,MV_L_LIMIT);
        else if(v_tair_ad>M0_TAS_MAX_50K)Accu_AD_Mv(MV_TAS,MV_U_LIMIT);
        else Accu_AD_Mv(MV_TAS,MV_NORMAL);

   
    #endif
                 
    //#ifdef IS_TA_ONBOARD
    //2017-1-21
    if(BYPTR_M0_SOURCE_TA==SOURCE_TA_ONBOARD){
      
         //2016-1-9 add GEN需要判断TPCB
        if(v_TPCB>M0_TAS_MAX||v_TPCB<M0_TAS_MIN){
            if(nb_tpcber<255)nb_tpcber++;
        }
        else
        {
            nb_tpcber=0;    
        }
        
        v_tair_inuse=v_TPCB;//2016-1-10
        if(v_tair_inuse>M0_TAS_MAX_INUSE_50K) v_tair_inuse=M0_TAS_MAX_INUSE_50K;//2010-3-21 move here
    
        if(v_TPCB<M0_TAS_MIN)Accu_AD_Mv(MV_TAS,MV_L_LIMIT);
        else if(v_TPCB>M0_TAS_MAX)Accu_AD_Mv(MV_TAS,MV_U_LIMIT);
        else Accu_AD_Mv(MV_TAS,MV_NORMAL);
    }
    //#endif
      
      

    //发动机温度诊断
        if(!VF_WORM_ENG)w=2;else{w=M0_TES_MIN;}
        if(v_teng_ad<w || v_teng_ad>=M0_TES_MAX)
        {
            if(nb_teser<255)nb_teser++; //,NB_TMPER_LIMIT
            
            #if TAS_TYPE==TAS_100K
                v_teng_inuse=v_tair_inuse;
            #else
                v_teng_inuse=v_tair_inuse-M0_D_TES_TAS;//2012-11-19 
            #endif
        }else 
        {
            nb_teser=0;
			//#ifdef SY_JSZS
				v_teng_inuse=F_2DLookUp(v_teng_ad,M1_TES_INUSE_JSZS,M0_TES_S_DEG_UB,0,false);
			//#else
				//v_teng_inuse=v_teng_ad;
			//#endif
        }

        //2016-2-28
        if(v_teng_ad<2)Accu_AD_Mv(MV_TES,MV_L_LIMIT);
        else if(v_teng_ad>M0_TES_MAX)Accu_AD_Mv(MV_TES,MV_U_LIMIT);
        else Accu_AD_Mv(MV_TES,MV_NORMAL);
        
    /*诊断*/
        //#ifdef IS_TA_ONBOARD
        if(BYPTR_M0_SOURCE_TA==SOURCE_TA_ONBOARD) {
          if(nb_tpcber>NB_TMPER_LIMIT)VF_TASER_TMP=true;
            else VF_TASER_TMP=false;//2016-3-1
        }
        else {
          if(nb_taser>NB_TMPER_LIMIT)VF_TASER_TMP=true;
            else VF_TASER_TMP=false;//2016-3-1
        }
        //#endif
        
        
        
        if(nb_teser>NB_TMPER_LIMIT)VF_TESER_TMP=true;
            else VF_TESER_TMP=false;//2016-3-1


    /*计算得到对应于发动机AD值的温度摄氏度度值*/
        //2012-11-19
        #if TAS_TYPE==TAS_100K
            blnM4=false;
            if((v_tair_inuse<M0_TMP_TH_M4)&&(nb_taser==0)){
                blnM4=true;
            }
            
            V_Ta=F_2DLookUp(v_tair_inuse,M1_TES_S_DEG,M0_TES_S_DEG_UB,v_tair_10bit,blnM4);
        #else
            V_Ta=F_2DLookUp(v_tair_inuse,M1_TAS_S_DEG,M0_TES_S_DEG_UB,0,false);
        #endif
        V_Ta-=M0_TES_DEG_BASIC;//2012-9-5 纠正

        //2013-6-18
        //#if ECU_TYPE==ECU_GEN
        /*
        #ifdef IS_GEN
        
            V_Ta=F_2DLookUp(v_Tmp_bymcu,M1_TAS_MCU_DEG,M0_TES_S_DEG_UB,0,false);
        #endif
        */
        
        //2016-1-9 IS_GEN
        //#ifdef IS_TA_ONBOARD
        if((BYPTR_M0_SOURCE_TA==SOURCE_TA_ONBOARD)){
          
            //V_Ta=F_2DLookUp(v_TPCB,M1_TAS_MCU_DEG,M0_TES_S_DEG_UB,0,false);
            V_Ta=F_2DLookUp(v_TPCB,M1_TPCB_S_DEG_PI,M0_TES_S_DEG_UB,0,false);
            V_Ta-=40;//
        }
        //#endif
        
        
        if(V_Ta<TES_DEG_MIN)V_Ta=TES_DEG_MIN;//最低温度
        
        /*teng*/
        blnM4=false;
        if((v_teng_inuse<M0_TMP_TH_M4)&&(nb_teser==0)){
            blnM4=true;
        }
		/*#ifdef SY_JSZS
		if((v_teng_inuse>230)&&(nb_teser==0))
		{
			 blnM4=true;
		}
		#endif*/
		//#ifdef SY_JSZS
			blnM4=false;		
		//#endif
		
        v_te=F_2DLookUp(v_teng_inuse,M1_TES_S_DEG,M0_TES_S_DEG_UB,v_teng_10bit,blnM4) ;
        
        //根据采样值，计算得到温度值；
        v_te-=M0_TES_DEG_BASIC;
        //v_te-=3;//2012-11-23     //2013-1-16 delete
        if(v_te<TES_DEG_MIN)v_te=TES_DEG_MIN;//最低温度
        
        //if(v_te<V_Ta)v_te= V_Ta;//2012-9-20 delete

		 //20211014 chenwei add
			  if(!VF_MMOVE_ENG)
			  {
		        v_te_keyon=v_te;
				V_Ta_keyon=V_Ta;
			  }
        
        if(!VF_1STGET_TMP)
        {
            //Tw0=V_Tw=v_te;

            //2012-11-17 add condiction 那个温度低就用哪个，往保险方向取
            //#ifdef IS_TA_ONBOARD
            //2017-1-21
            if(BYPTR_M0_SOURCE_TA==SOURCE_TA_ONBOARD){
              
                if(((v_te>V_Ta)&&(v_te<0)&&(nb_tpcber==0))||(nb_teser!=0))
                {
                    //2012-11-19 add  (nb_teser!=0)
                    v_twb=V_Tw=V_Ta;//2013-3-26 add 
                } else{
                    v_twb=V_Tw=v_te;//2013-3-26 add 
                }
            }
            else {
              if(((v_te>V_Ta)&&(v_te<0)&&(nb_taser==0))||(nb_teser!=0))
                {
                    //2012-11-19 add  (nb_teser!=0)
                    v_twb=V_Tw=V_Ta;//2013-3-26 add 
                } else{
                    v_twb=V_Tw=v_te;//2013-3-26 add 
                }
            }
            //#endif
            
            tw_stop=V_Tw;
            
            //if(nb_teser==0)   //这个条件会导致TES出错时TW计算不了；
            VF_1STGET_TMP=true; //2012-9-24 add condiction TES不出错时，得到一次即可
                                  
            //if(tamp_yc_last>v_teng_inuse)
            //2012-11-19 更改为用温度值作比较
            if(V_Ta>V_Tw)
          	{
          	    tamp_yc_last-=2;
          	}else
          	{
          	    tamp_yc_last+=2;
          	}
        }								
        
        if(v_twb>v_te)v_twb=v_te;
        //if(v_twb<V_Ta)v_twb=V_Ta;
        if(v_twb<V_Ta-5)v_twb=V_Ta-5;//2013-4-18
		
		
		if(VF_TW_1ST)
		//2019-12-24  发现上电第一次采样计算的twb或te可能有问题，因此对于第一次上电后没启动的tw持续赋值为twb
		{
			tw_stop=V_Tw=v_twb;
			if(VF_MMOVE_ENG)VF_TW_1ST=false;
		}
		
		
        //26H8增加
        //D26H9 ADD CONDICTION V_TW; VR_1S_TMCCL
        //V_TWB的过度处理
        if(v_te>0||V_Tw>10){ //认为0度以上TE基本就正确了
            if(v_twb<v_te)
			{
                if(VF_MMOVE_ENG)
				{
					if(VF_1S_TMCCL)v_twb++;
				}
				else
				{
					v_twb++;
				}
            }
        }
        
    /*在此处计算温度预测值：vp_teng_ad*/
        
        //if(VF_MMOVE_ENG&&!VF_YSTDC_INJPHASE&&VF_BFIRE_ENG)//2012-8-15 ADD CONDICITON
        if(VF_MMOVE_ENG&&VF_RUN2_TMCCL&&VF_BFIRE_ENG)//2020-9-24 发现有时候!VF_YSTDC_INJPHASE无法满足
		{
           // if(!VF_TESSTABLE_TMP)  //本括号以内的运行时间是256us;
            if(!VF_IGNONLY_CFCI)
            {
                if(rev<M0_N0)rev=M0_N0;//
                
                w=M0_N0_M16/rev;
                
                /*
                if(VF_STARTS_ENG) {
                    w/=2;           //  /4 rich 0.8 
                                    //D26H7 FROM /2 TO /4 , XZ FROM 60 TO 80  
                                    //STARTS之后，温度上升的优点快FROM /4 TO W/=2;
                    if(w<1)w=1;
                }//2013-3-25 add condiction
                *///2014-12-15 DELETE  FOR TMP CAL
                if(v_tao_M16<128000)//2020-9-27 时间长了tao可能会溢出，因为v_sq查表的数最大就到7300，按照8000*16计算设定限值
				{
					v_tao_M16=v_tao_M16+w;//V_rev=ni/15.6,量纲为15.6rpm
				
                
                v_sq=(word)(v_tao_M16>>4);//;v_tao_M16/16;
                
                //#if MV_EXHAUST<=PL150   //2014-2-20 add for tmp
                //    if(VF_STARTS_ENG)v_sq+=150;  //  /40 rich 0.8
               
                 if(VF_STARTS_ENG)v_sq+=v_X;//2014-2-21
                //#endif
                //if(VF_STARTS_ENG)v_sq+=100;  //  2013-12-22 FROM 150 TO 100
                                            //+100显得太浓
                                           //H21 30
                                           //H22 50
                                           //D26H7 TWstarts 涨要快，更改50 TO 100
                                           //D26H8 根据张建测试，STARTS后面XZ=80，sq=100，TW上升太快，
                                           //    starts时，温度可以升的再快一点，SQ FROM 100 TO 50
                                            //D26H12 from 50 to 150
                v_tao_s_M16=v_tao_M16;
                
                v_sq=F_2DLookUpSword(v_sq,M1_SQ_M2048,M0_UB_SQ);//sq_M2048=M1_sq_M2048[X];
                //if(v_sq<=M1_SQ_M2048[M0_UB_SQ][1])VF_TESSTABLE_TMP=true;    2013-1-25 delete
                
                }
                
                
                /*
                    G=(n0/n-1)*G0*g+G0;
                     =G0*(n0/n*g-g+1)
                     =G0*(n0/n*g_m128-g_m128+128);
                    
                */
                //2014-2-12 需要X M0_g_M128
                /*
                    v_sigma_M2048+=(long int)fmf*v_sq/M0_m0;                   //v_sigma_M2048=v_sigma_M2048+mi/M0_m0*sq_M2048;

                    w=(word)N0_62D5*M0_g_M128/v_rev_625rpm;
                    
                    blnM4=(byte)((v_G_M128*(w+128-M0_g_M128))>>7);
                    
                    if(blnM4<2)blnM4=2;

                    w=(word)(((v_G_M128*(unsigned long int)v_sigma_M2048)>>16)&0x0000ffff); //2014-2-13 恢复
                    //w=(word)(((blnM4*(unsigned long int)v_sigma_M2048)>>16)&0x0000ffff);//2014-2-12 add M0_g_M128
                */
                //2014-2-13 更改如下
                //2014-2-24  发现着火后，如果v_rev_625rpm为几百rpm时，G算的就不对了，因此需要限制v_rev_625rpm
                    
                    
                    if(v_rev_625rpm>N0_62D5){
                      
                        w=(word)N0_62D5*v_g/v_rev_625rpm;
                        blnM4=(byte)((v_G_M128*(w+128-v_g))>>7);
                        if(blnM4<2)blnM4=2;
                    } else blnM4=v_G_M128;
                
                    v_Gx_M128=blnM4;//2015-2-3
                    
                //if(v_sigma_M2048<0x9fffffff)
				if(v_sigma_M2048<0x03ffffff)//2020-9-27 条件挪到这个地方，否则时间长了sigma会溢出;条件设小一些，大了也没有意义
				{
					v_sigma_M2048+=((unsigned long int)fmf*v_sq/M0_m0)*blnM4; 
				}	
					                  
					//v_sigma_M2048=v_sigma_M2048+mi/M0_m0*sq_M2048;

                    //2015-2-3 add 
                   // w=(word)((((unsigned long int)v_sigma_M2048)>>16)&0x0000ffff); //2014-2-13 恢复
                   // w=(word)((((unsigned long int)v_sigma_M2048)>>18)&0x00003fff); //2014-2-22 G M4
                   // w=(word)((((unsigned long int)v_sigma_M2048)>>20)&0x00000fff); //2015-2-3 G
                
                   //2015-3-8 找到TMP.C 中TW上升快是因为一圈被多次调用的，因此恢复
                    w=(word)((((unsigned long int)v_sigma_M2048)>>18)&0x00003fff); 

                if(w>200)w=200; //2013-12-30
                V_Tw=w+dtw0+v_twb;// 2013-12-5
                //deDTFI=V_Tw;
                  
                if(V_Tw>M0_TW_MAX){
                    V_Tw=M0_TW_MAX;
                    VF_TESSTABLE_TMP=true;
                }
            }
                
                /*
                //计算 v_tg
                 // sw=(M0_W_M64*((long int)fmf*rev-M0_M0_M_N0))/M0_M0_M_N0+64;
                  //2013-1-29 离开怠速时的作用减半
                  sw=(M0_W_M64*((long int)fmf*rev-M0_M0_M_N0))/M0_M0_M_N0_M4+64;
                  
                  if(sw<0)sw=0;
                  //w=(((word)sw*M0_TCR)>>6)&0x03ff;
                  //v_Y_M256
                  w=(((word)sw*v_Y_M256)>>6)&0x03ff;
                  if(w>500)w=500;
                  //if(VF_IGNONLY_CFCI)w=v_Y_M256/4;//2013-1-29
                  if(VF_IGNONLY_CFCI)w=v_Y_M256/8;//2013-3-26
                  
                  //deT3=w;
                  v_Tg_Sum256=v_Tg_Sum256-v_Tg+w;
                  
                  
                  v_Tg=(word)(v_Tg_Sum256>>8);
                  if(v_Tg<v_Y_M256){
                      v_Tg=v_Y_M256;
                      v_Tg_Sum256=(word)((v_Tg<<8)&0xff00);//2013-2-1
                  }

                 *///2013-5-17 delete v_tg;
                 
                  //2013-3-26 CHANGE TO 
                  //v_Tg=v_Y_M256;
                  //sw=v_te+v_Tg;
                  //2013-3-26 CHANGE TO 
                  sw=v_twb+v_Y_M256;//2015-10-27 替代v_Tg;
                  
               
                  if(V_Tw>sw)V_Tw=sw;
                  if(V_Tw>M0_TW_MAX)V_Tw=M0_TW_MAX;
                  

        } 
        else
        {
            if(!VF_MMOVE_ENG)Attu_TMP(); //2012-8-21 更正
        }
        
        
        //if(V_Tw<v_te)V_Tw=v_te; //2012-9-20 MOVE HERE
        //2012-11-17 因为温度传感器误差大，当V_Tw<10时，都不使用这个 ,因此增加条件
        if(V_Tw>40){//2012-11-23  FROM 10 TO 40
            if(V_Tw<v_te)V_Tw++; //2012-9-20 MOVE HERE
        }
        
        if(VF_MMOVE_ENG){
            tw_stop=(V_Tw); //2013-1-24
        
        }
    /*在此处确定所使用的温度值：v_teng_inuse　*/
        v_teng_odrh5=(V_Tw-TES_DEG_MIN)/8;//保证在0-30之间
        
        //if((v_teng_inuse>=25)&&(v_teng_inuse<38))VF_TENG_FORECE_TMP=true;
        //2012-11-19
        if((V_Tw>=18)&&(V_Tw<30))VF_TENG_FORECE_TMP=true;
        else VF_TENG_FORECE_TMP=false;        
    
    /*其它*/
        //sw=v_teng_inuse-(sword)v_tair_ad;//2010-3-25 模拟里程测试时发现应该使用v_teng_inuse
        //2012-11-19
        /*
        sw=V_Tw-(sword)V_Ta;//2010-3-25 模拟里程测试时发现应该使用v_teng_inuse
        if((sw>40))VFF_NOZZLEINI_TESHIGH_DTC=true;
        else VFF_NOZZLEINI_TESHIGH_DTC=false;
        */

        //2013-3-3
        //sw=V_Tw-(sword)M0_T_UC;//2010-3-25 模拟里程测试时发现应该使用v_teng_inuse
        if((V_Tw>=M0_TW_FORINI))VFF_NOZZLEINI_TESHIGH_DTC=true;
        else VFF_NOZZLEINI_TESHIGH_DTC=false;



}


//********SAVE_TAIR_WHEN_KEYOFF_OR_STOPRUN********************************************
//功能：将空气温度预测值tamp_yc保存到地址ee_Tamp_save和ee_Tamp_save+1的EE中；
//      当停车或关闭钥匙时被调用；
void FM_TMP_air_StopOrKeyoff()
{
    if(VF_MORE1MIN_TMCCL&&!VF_TESER_TMP){
        Save_Data_EE(EE_TAMP_SAVE,tamp_yc_last);
        Save_Data_EE(EE_TAMP_SAVE+1,tamp_yc_last+1);   //2010-1-4 data(xb)=data(x)+1; 
    }           // DEBUG
    return;    
}

void Attu_TMP(void)
{
    word w;
    
    //2013-3-26 v_te replaced by v_twb
    
    if(tw_stop<v_twb)tw_stop=v_twb;
    
    if(V_Tw>v_twb)
    {

        //if((((V_Tw-v_te)>16)&&VF_2S_TMCCL)||VF_8S_TMCCL) V_Tw--;
        
        w=v_R_d4*v_t_eng_1s;
        w=F_EXPM100_Lookup(w);
        
        //dtw0=(tw_stop-v_twb)*(long int)w/4096;
        dtw0=((tw_stop-v_twb)*(long int)w)/4096;
        
        V_Tw=v_twb+dtw0;
        
        
        
    } else{
        
        V_Tw=v_twb;
        dtw0=0;
        
        
    }
    if(v_t_eng_1s<=64){
        v_tao_M16=(v_tao_s_M16*(64-v_t_eng_1s))/64;
        
        //v_tao_M16=0;//2016-4-3
    } else v_tao_M16=0;
    
    if(!VF_KEYON_HARD)   //Attu_TMP
    {
       tw_whenKeyoff=V_Tw;
    }
      
}



/*
    2016-4-4 发现，
    如果钥匙开启或者关闭那一下，时间从0开始计算，调用Attu_TMP
    是有问题的，等于前面的衰减作用都被取消的
    因此，增加这个函数
*/
void ResetTwParaWhenKeyonOroff(void) 
{
    v_tao_s_M16=v_tao_M16;
    tw_stop=V_Tw;        
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
