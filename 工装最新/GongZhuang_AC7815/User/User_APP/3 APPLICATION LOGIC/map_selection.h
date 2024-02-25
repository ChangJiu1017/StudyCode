/*HEAD
 ***********************************************************************
 * Filename	:map_selection.h
 * Function	:map_selection头文件	
 * User		:yangsili
 * Date		:2017.12.30
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.30	yangsili
 *		1.更改MV_SYSINFO_21 为0
 *
 ***********************************************************************
HEAD END*/

/**本文件中*包含了所有车型的列表*******************/
/*
    本头文件只被以下两个模块包含：
        strategy.c
        data_sys_source.c
*/
#include "derivative.h"
#include "data_sys_standard.h"

extern void Set_Flag_EEinFlash(byte blnClear);
extern byte Check_EEinFlash(void);
extern void Copy_Map_fromFlash2EE(byte blnClear);

extern void Check_EE_afterReset(void);//2014-5-17
extern word GetAdr_inEEbyPN(byte PN,word SN); 
extern byte Check_FlashAdd_Valid(word ADD_ROM,byte ADD_PAGE,byte VF_ROM_MCU);
extern unsigned long int GetPtrbyPNandAd(byte n);

//#include "210901-v9.24.H"	
//#include "110902-v9.34F.H"	
//#include "21C902-v9.27C.H"
//#include "21C901-v9.35C.H"	
//#include "31B901-v9.38A.H"	
//#include "31B902-v9.39A.H"	
//#include "41A901-v9.37A.H"	
//#include "41A902-v9.36A.H"
#include "TRAD.H"
#include "setting_conf.h"


//2016-3-5      

//extern 	const byte M1_VIN[17];
//extern  const byte M1_CID[16];
//extern	const byte M1_ECUNAME[20];

/*===================================================================================================*/  
   /*软件版本*/	
	#define	M0_PRO_1	8				//产品平台 8—PI-MOTORCYCLE，20—PI-GENERATOR，30—SCR，40—DPF，50—UAV
	#define	M0_PRO_2	50				//ECU序列码									
	#define	M0_PRO_3	0				//软件平台大版本									
	#define	M0_PRO_4	5				//软件平台小版本
	#define	M0_PRO_5	12				//客户化应用需求大版本		
	#define	M0_PRO_6	0				//客户化应用需求小版本
  #define M0_PRO_7	100				//测试版本		20211014 chenwei add 
   
   /*System definition*/							
							
	#define 		MV_SYSINFO_0				0//EASA_NOHH
	#define 		MV_SYSINFO_1				BYPTR_M0_PC_L
	#define 		MV_SYSINFO_2				0//EASA_NOHL
	#define 		MV_SYSINFO_3				BYPTR_M0_PC_H
	#define 		MV_SYSINFO_4				BYPTR_M0_PC_HH
	#define 		MV_SYSINFO_5				BYPTR_M0_PC_HL
	#define 		MV_SYSINFO_6				BYPTR_MV_EXHAUST   //M0_EXHAUST
	#define 		MV_SYSINFO_7				M0_PRO_7			//0//EASAA	//20211013 chenwei charge 软件测试版本
	#define 		MV_SYSINFO_8				M0_MFUD7		//2017-1-21 增加喷油器标准版本号
	#define 		MV_SYSINFO_9				M0_PRO_5		//2017-1-21 增加软件应用版本号1
	#define 		MV_SYSINFO_10				M0_PRO_6		//2017-1-21 增加软件应用版本号2
	#define 		MV_SYSINFO_11				M0_PRO_1
	#define 		MV_SYSINFO_12				M0_PRO_2
	#define 		MV_SYSINFO_13				M0_PRO_3
	#define 		MV_SYSINFO_14				M0_PRO_4
	#define 		MV_SYSINFO_21			  	BYPTR_M0_MAP_VERSION//M0_MAP//
	#define 		MV_SYSINFO_22		  		BYPTR_MV_VEHICLE//BYPTR_M0_LIMIT_VERSION//	 M0_FF//
							
							
							
	/*Nozzle definition*/							
	#define 		MV_SYSINFO_15				M0_MFUD1
	#define 		MV_SYSINFO_16				M0_MFUD2
	#define 		MV_SYSINFO_17				M0_MFUD3
	#define 		MV_SYSINFO_18				M0_MFUD4
	#define 		MV_SYSINFO_19				M0_MFUD5
	#define 		MV_SYSINFO_20				M0_MFUD6
	
	/*For Project*/
	//#define	IS_GEN
	//#define X_N_9000_TO_9500RPM
	#define SYS_TRADITIONAL
	#define SYS_STEPMOTOR
	//#define SY_CORADJ
	//#define SYS_GEAR
	//#define SY_JSZS
	#define SY_24
	//#define SY_36
	//#define SY_12
	//#define SY_QD
	#define SYS_MOTOR_33814CAS_S        //33814角标处理
    //2018-1-17,ysl,氧加热G拖低?
    #define SYS_O2HEATER_S              //氧加热系统 
	//#define SYS_TACH
	
	/**TE阻值选择，选择其諫之一***/
	//#define TE_10K	//10k缸温 即缸温安装于缸体上，且蜸冷却水循环系统
	#define TE_2K		//2K水温    即水温安装于水套内，覩冷却水循环系统-20210920 此处为环松
	
	/*****************************/
	
	#define SY_CC   //侧撑熄籫
	
	#define SY_CCPS   //碳筍电磁阀

	#define SY_HUANSONG	//20210922 chenwei add 环松仪表转速显示+压力

	//#define	SY_TPS_40mv		//20210923 chenwei add TPS-40mv，反之屏蔽默认为50mv

	#define SY_BUMPO2_C			//20211003 chenwei add 泵氧选择

	//20210922 15.21备注 增加了dmlambda调节因子BYPTR_M0_LAMBDA_FACTOR

		
    

