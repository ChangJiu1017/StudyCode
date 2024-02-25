/*HEAD
 ***********************************************************************
 * Filename	:map_selection.h
 * Function	:map_selectionͷ�ļ�	
 * User		:yangsili
 * Date		:2017.12.30
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.30	yangsili
 *		1.����MV_SYSINFO_21 Ϊ0
 *
 ***********************************************************************
HEAD END*/

/**���ļ���*���������г��͵��б�*******************/
/*
    ��ͷ�ļ�ֻ����������ģ�������
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
   /*����汾*/	
	#define	M0_PRO_1	8				//��Ʒƽ̨ 8��PI-MOTORCYCLE��20��PI-GENERATOR��30��SCR��40��DPF��50��UAV
	#define	M0_PRO_2	50				//ECU������									
	#define	M0_PRO_3	0				//���ƽ̨��汾									
	#define	M0_PRO_4	5				//���ƽ̨С�汾
	#define	M0_PRO_5	12				//�ͻ���Ӧ�������汾		
	#define	M0_PRO_6	0				//�ͻ���Ӧ������С�汾
  #define M0_PRO_7	100				//���԰汾		20211014 chenwei add 
   
   /*System definition*/							
							
	#define 		MV_SYSINFO_0				0//EASA_NOHH
	#define 		MV_SYSINFO_1				BYPTR_M0_PC_L
	#define 		MV_SYSINFO_2				0//EASA_NOHL
	#define 		MV_SYSINFO_3				BYPTR_M0_PC_H
	#define 		MV_SYSINFO_4				BYPTR_M0_PC_HH
	#define 		MV_SYSINFO_5				BYPTR_M0_PC_HL
	#define 		MV_SYSINFO_6				BYPTR_MV_EXHAUST   //M0_EXHAUST
	#define 		MV_SYSINFO_7				M0_PRO_7			//0//EASAA	//20211013 chenwei charge ������԰汾
	#define 		MV_SYSINFO_8				M0_MFUD7		//2017-1-21 ������������׼�汾��
	#define 		MV_SYSINFO_9				M0_PRO_5		//2017-1-21 �������Ӧ�ð汾��1
	#define 		MV_SYSINFO_10				M0_PRO_6		//2017-1-21 �������Ӧ�ð汾��2
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
	#define SYS_MOTOR_33814CAS_S        //33814�Ǳ괦��
    //2018-1-17,ysl,������G�ϵ�?
    #define SYS_O2HEATER_S              //������ϵͳ 
	//#define SYS_TACH
	
	/**TE��ֵѡ��ѡ�����G֮һ***/
	//#define TE_10K	//10k���� �����°�װ�ڸ����ϣ����S��ȴˮѭ��ϵͳ
	#define TE_2K		//2Kˮ��    ��ˮ�°�װ��ˮ���ڣ��G��ȴˮѭ��ϵͳ-20210920 �˴�Ϊ����
	
	/*****************************/
	
	#define SY_CC   //���Ϩ�g
	
	#define SY_CCPS   //̼�S��ŷ�

	#define SY_HUANSONG	//20210922 chenwei add �����Ǳ�ת����ʾ+ѹ��

	//#define	SY_TPS_40mv		//20210923 chenwei add TPS-40mv����֮����Ĭ��Ϊ50mv

	#define SY_BUMPO2_C			//20211003 chenwei add ����ѡ��

	//20210922 15.21��ע ������dmlambda��������BYPTR_M0_LAMBDA_FACTOR

		
    

