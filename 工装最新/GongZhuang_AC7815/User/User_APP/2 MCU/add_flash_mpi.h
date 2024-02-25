/*HEAD
 ***********************************************************************
 * Filename	:add_flash_mpi.h
 * Function	:Flash address configuration.		
 * User		:xielian
 * Date		:2017.1.20
 * Version	:V1.1.2
 *
 *** History ***
 * V1.1.0	2017.1.20	xielian 
 *		Base function.
 *
 * V1.1.1	2017.1.20	xielian 
 *		add flash address configuration.
 *
 * V1.1.2	2017.3.24	xielian 
 *		add M0_FMF_CO.
 * V1.1.3	2017.3.28	xielian 
 *		add M0_NB_INJDRV,M0_VBATLIMT.
 * V1.2.0	2018.9.26	xielian 
 *		add IACV relative,MV_VEHICLE.
 ***********************************************************************
HEAD END*/

//#include "public.h"
#include "derivative.h"
#include "sys_definition.h"   //2014-10-29
							
#define	ADD_FLASH_START	ATC7811MapAddr//(unsigned long int)0xe200	//2015-3-22 -1000				
//#define	ADD_FLASH_START	0x88000UL		//0X3C000   88000-20000=68000������ʱ�õ�			
//#define ADD_FLASH_START_REF 0x20000UL//��Ե�ַ��������ʹ�õģ� 
//#define	ADD_FLASH_START	0x20000UL					

#define	ADD_FLASH_BLOCK(N)	(unsigned long int)(ADD_FLASH_START+(unsigned long int)N*0x200)					
#define	SIZE_FLASH_BLOCK	(unsigned long int)512					
#define	NO_MAX_BLOCK	(unsigned long int)14					
#define	NO_MIN_BLOCK	(unsigned long int)0					
#define ADD_FLASH_BYTE(N)	(unsigned long int)(ADD_FLASH_START+(unsigned long int)N*0x200)					

//MAP PAGE							
#define	DATAANDSPEED	0					
#define	LDB	1					
#define	LDBC	2					
#define	LAMBDAANDFUEL	3					
#define	PN_IGN	4					

/*
    MAP �Ĵ洢
    ԭʼmap�����FLASH  ADD_FLASH_STARTλ��
    ͨ��һ�����ƣ����Ƶ�EEPROM��
    ��
    0xf200 ��Ӧ�� 0x800 =ea00
        ����ַΪ0x400
*/
#define DADD_FLASH_REF_END  0x9FFu
#define DADD_ST_MAPINEE0  0x400u  //����Ӧ������Ե�ַ������EEPROM�е�λ�ã����Ե�ַ��Ҫ���ϻ���ַ0x400��

#define DADD_MAP_INEE_FLG(x) DADD_ST_MAPINEE0+(word)0x200*x+(word)0x1feu

#define DADD_FLASH2EE  0//0xdA00u//  2015-3-22 -1000


/*----------------------------------------------------------*/
//�Ա����ͺ����Ķ���							
extern byte * p_byte_array;//2014-5-16 add
extern sbyte * p_sbyte_array;//2014-5-16 add

extern byte ByteData_FlashAtEE(byte * ptr); //��EEPROM�л�ȡFLASH map������,��һ����ַƫ��DADD_FLASH2EE=0xf200-0x800=EA00
extern byte * Ptr_FlashAtEE(byte * ptr); 
extern byte Get_Map_byPtr(byte * ptr,byte x_num,byte i_y,byte i_x);


/*-----------------------------------------------------------*/
//normal in flash
#define M0_EGR 0

//#pragma CODE_SEG USER_MAP

#if SYS==SYS_SCR

    #define BYPTR_M0_C_CABNOZZLE ByteData_FlashAtEE(&M0_TPS0)
    
    

#endif

/*----------data input & speed control-----------------*/							
	extern 	const	byte	M0_SD0_D12;//	@ ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(0);//		;							
	#define BYPTR_M0_SD0_D12 M0_SD0_D12//ByteData_FlashAtEE(&M0_SD0_D12)													
														
	extern 	const	byte	M0_IBP/* __attribute__((section (".ARM.__at_0x080C001")))*/;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(1)	;							
	#define BYPTR_M0_IBP M0_IBP//ByteData_FlashAtEE(&M0_IBP)													
														
	extern 	const	byte	M0_FBP;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(2)	;							
	#define BYPTR_M0_FBP M0_FBP//ByteData_FlashAtEE(&M0_FBP)													
														
	extern 	const	byte	M0_QL_D2;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(3)	;							
	#define BYPTR_M0_QL_D2 M0_QL_D2//ByteData_FlashAtEE(&M0_QL_D2)													
														
	extern 	const	byte	M0_TPS0;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(4)	;							
	#define BYPTR_M0_TPS0 M0_TPS0//ByteData_FlashAtEE(&M0_TPS0)													
														
	extern 	const	byte	M0_TPD_LT6;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(5)	;							
	#define BYPTR_M0_TPD_LT6 M0_TPD_LT6//ByteData_FlashAtEE(&M0_TPD_LT6)													
														
	extern 	const	byte	M0_TPD_LT11;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(6)	;							
	#define BYPTR_M0_TPD_LT11 M0_TPD_LT11//ByteData_FlashAtEE(&M0_TPD_LT11)													
														
	extern 	const	byte	M0_TPD_LT14;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(7)	;							
	#define BYPTR_M0_TPD_LT14 M0_TPD_LT14//ByteData_FlashAtEE(&M0_TPD_LT14)													
														
	extern 	const	byte	M0_TPD_LT16;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(8)	;							
	#define BYPTR_M0_TPD_LT16 M0_TPD_LT16//ByteData_FlashAtEE(&M0_TPD_LT16)													
														
	extern 	const	byte	M0_OS_LIGHTOFF;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(9)	;							
	#define BYPTR_M0_OS_LIGHTOFF M0_OS_LIGHTOFF//ByteData_FlashAtEE(&M0_OS_LIGHTOFF)													
														
	extern 	const	byte	M0_CAS_FRONTEDGE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(10)	;							
	#define BYPTR_M0_CAS_FRONTEDGE M0_CAS_FRONTEDGE//(ByteData_FlashAtEE(&M0_CAS_FRONTEDGE))													
														
	extern 	const	byte	M0_CAS_TRAILEDGE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(11)	;							
	#define BYPTR_M0_CAS_TRAILEDGE M0_CAS_TRAILEDGE//ByteData_FlashAtEE(&M0_CAS_TRAILEDGE)													
														
	extern 	const	byte	M0_PAIRS_SEALEVEL;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(12)	;							
	#define BYPTR_M0_PAIRS_SEALEVEL M0_PAIRS_SEALEVEL//ByteData_FlashAtEE(&M0_PAIRS_SEALEVEL)													
														
	extern 	const	byte	M0_PAIRS_PER1000M;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(13)	;							
	#define BYPTR_M0_PAIRS_PER1000M M0_PAIRS_PER1000M//ByteData_FlashAtEE(&M0_PAIRS_PER1000M)													
														
	extern 	const	byte	M0_WIDTH_CDION;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(14)	;							
	#define BYPTR_M0_WIDTH_CDION M0_WIDTH_CDION//ByteData_FlashAtEE(&M0_WIDTH_CDION)													
																												
	extern 	const	byte	M0_DN_25;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(15)	;							
	#define BYPTR_M0_DN_25 M0_DN_25//ByteData_FlashAtEE(&M0_DN_25)													
														
	extern 	const	byte	M0_DN_153;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(16)	;							
	#define BYPTR_M0_DN_153 M0_DN_153//ByteData_FlashAtEE(&M0_DN_153)													
														
	extern 	const	byte	M0_N0_LNOISCV_15D6RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(17)	;							
	#define BYPTR_M0_N0_LNOISCV_15D6RPM M0_N0_LNOISCV_15D6RPM//ByteData_FlashAtEE(&M0_N0_LNOISCV_15D6RPM)													
														
	extern 	const	byte	M0_DN0_HTOLNOISCV_15D6RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(18)	;							
	#define BYPTR_M0_DN0_HTOLNOISCV_15D6RPM M0_DN0_HTOLNOISCV_15D6RPM//ByteData_FlashAtEE(&M0_DN0_HTOLNOISCV_15D6RPM)													
														
	extern 	const	byte	M0_N0_LISCV_15D6RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(19)	;							
	#define BYPTR_M0_N0_LISCV_15D6RPM M0_N0_LISCV_15D6RPM//ByteData_FlashAtEE(&M0_N0_LISCV_15D6RPM)													
														
	extern 	const	byte	M0_N0_HISCV_15D6RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(20)	;							
	#define BYPTR_M0_N0_HISCV_15D6RPM M0_N0_HISCV_15D6RPM//ByteData_FlashAtEE(&M0_N0_HISCV_15D6RPM)													
														
	extern 	const	byte	M0_NSTAND_62D5RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(21)	;							
	#define BYPTR_M0_NSTAND_62D5RPM M0_NSTAND_62D5RPM//ByteData_FlashAtEE(&M0_NSTAND_62D5RPM)													
														
	extern 	const	byte	M0_NFULL_62D5RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(22)	;							
	#define BYPTR_M0_NFULL_62D5RPM M0_NFULL_62D5RPM//ByteData_FlashAtEE(&M0_NFULL_62D5RPM)													
														
	extern 	const	byte	M0_KP_M8;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(23)	;							
	#define BYPTR_M0_KP_M8 M0_KP_M8//ByteData_FlashAtEE(&M0_KP_M8)													
														
	extern 	const	byte	M0_KI_M8;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(24)	;							
	#define BYPTR_M0_KI_M8 M0_KI_M8//ByteData_FlashAtEE(&M0_KI_M8)													
														
	extern 	const	byte	M0_KD_M8;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(25)	;							
	#define BYPTR_M0_KD_M8 M0_KD_M8//ByteData_FlashAtEE(&M0_KD_M8)													
														
	extern 	const	sbyte	M0_K_15D6RPM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(26)	;							
	#define BYPTR_M0_K_15D6RPM M0_K_15D6RPM//(sbyte)ByteData_FlashAtEE(&M0_K_15D6RPM)													
														
	extern 	const	byte	M0_J_M4;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(27)	;							
	#define BYPTR_M0_J_M4 M0_J_M4//ByteData_FlashAtEE(&M0_J_M4)													
														
	extern 	const	byte	M0_VBATLIMT;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(28)	;									
	#define BYPTR_M0_VBATLIMT M0_VBATLIMT//ByteData_FlashAtEE(&M0_VBATLIMT)															
																
	extern 	const	byte	M0_FMF_CO;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(29)	;									
	#define BYPTR_M0_FMF_CO M0_FMF_CO//ByteData_FlashAtEE(&M0_FMF_CO)															
														
	extern 	const	byte	M0_MAP_VERSION;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(30)	;	 //2015-6-23 ADD						
	#define BYPTR_M0_MAP_VERSION M0_MAP_VERSION//ByteData_FlashAtEE(&M0_MAP_VERSION)													
														
	//extern 	const	byte	M0_LIMIT_VERSION	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(31)	;//2015-6-23 ADD 							
	//#define BYPTR_M0_LIMIT_VERSION ByteData_FlashAtEE(&M0_LIMIT_VERSION)
	
	extern 	const	byte	MV_VEHICLE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(31)	;//2015-6-23 ADD 							
	#define BYPTR_MV_VEHICLE MV_VEHICLE//ByteData_FlashAtEE(&MV_VEHICLE)
	
	extern 	const	byte	M1_VIN[17];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(32)	;							
	#define BYPTR_M1_VIN M1_VIN//Ptr_FlashAtEE(&M1_VIN)													
														
	extern 	const	byte	M1_CID[16];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(49)	;							
	#define BYPTR_M1_CID M1_CID//Ptr_FlashAtEE(&M1_CID)													
														
	extern 	const	byte	M1_ECUNAME[20];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(65)	;							
	#define BYPTR_M1_ECUNAME M1_ECUNAME//Ptr_FlashAtEE(&M1_ECUNAME)													
														
	extern 	const	byte	M0_PC_HH;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(85)	;							
	#define BYPTR_M0_PC_HH M0_PC_HH//ByteData_FlashAtEE(&M0_PC_HH)													
														
	extern 	const	byte	M0_PC_HL;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(86)	;							
	#define BYPTR_M0_PC_HL M0_PC_HL//ByteData_FlashAtEE(&M0_PC_HL)													
														
	extern 	const	byte	M0_PC_H;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(87)	;							
	#define BYPTR_M0_PC_H M0_PC_H//ByteData_FlashAtEE(&M0_PC_H)													
														
	extern 	const	byte	M0_PC_L;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(88)	;							
	#define BYPTR_M0_PC_L M0_PC_L//ByteData_FlashAtEE(&M0_PC_L)													
														
	extern 	const	byte	M0_PC_5;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(89)	;							
	#define BYPTR_M0_PC_5 M0_PC_5//ByteData_FlashAtEE(&M0_PC_5)													
														
	extern 	const	byte	M0_PC_6;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(90)	;							
	#define BYPTR_M0_PC_6 M0_PC_6//ByteData_FlashAtEE(&M0_PC_6)													
														
	extern 	const	byte	M0_PC_7;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(91)	;							
	#define BYPTR_M0_PC_7 M0_PC_7//ByteData_FlashAtEE(&M0_PC_7)													
														
	extern 	const	byte	MV_EXHAUST;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(92)	;									
	#define BYPTR_MV_EXHAUST MV_EXHAUST//ByteData_FlashAtEE(&MV_EXHAUST)															
																
	extern 	const	byte	M0_BAUDRATE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(93)	;									
	#define BYPTR_M0_BAUDRATE M0_BAUDRATE//ByteData_FlashAtEE(&M0_BAUDRATE)															
																
	extern 	const	byte	M0_NB_INJDRV;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(94)	;									
	#define BYPTR_M0_NB_INJDRV M0_NB_INJDRV//ByteData_FlashAtEE(&M0_NB_INJDRV)															
																
	extern 	const	byte	M0_NIACVLM;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(95)	;
	#define BYPTR_M0_NIACVLM M0_NIACVLM//ByteData_FlashAtEE(&M0_NIACVLM)						
						
	extern 	const	byte	M0_KP_IACV;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(96)	;
	#define BYPTR_M0_KP_IACV M0_KP_IACV//ByteData_FlashAtEE(&M0_KP_IACV)						
						
	extern 	const	byte	M0_KI_IACV;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(97)	;
	#define BYPTR_M0_KI_IACV M0_KI_IACV//ByteData_FlashAtEE(&M0_KI_IACV)																			
														
	//Setting 2017-1-20													
	extern 	const	byte	M0_OBD_VERSION;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(98)	;							
	#define BYPTR_M0_OBD_VERSION M0_OBD_VERSION//ByteData_FlashAtEE(&M0_OBD_VERSION)													
														
	extern 	const	byte	M0_EN_DTC_TA;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(99)	;							
	#define BYPTR_M0_EN_DTC_TA M0_EN_DTC_TA//ByteData_FlashAtEE(&M0_EN_DTC_TA)													
														
	extern 	const	byte	M0_EN_DTC_IGN;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(100)	;							
	#define BYPTR_M0_EN_DTC_IGN M0_EN_DTC_IGN//ByteData_FlashAtEE(&M0_EN_DTC_IGN)													
														
	extern 	const	byte	M0_EN_IGN_FK_PARTLOAD;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(101)	;							
	#define BYPTR_M0_EN_IGN_FK_PARTLOAD M0_EN_IGN_FK_PARTLOAD//ByteData_FlashAtEE(&M0_EN_IGN_FK_PARTLOAD)													
														
	extern 	const	byte	M0_UL_REV_62D5RPM_IGNFK;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(102)	;							
	#define BYPTR_M0_UL_REV_62D5RPM_IGNFK M0_UL_REV_62D5RPM_IGNFK//ByteData_FlashAtEE(&M0_UL_REV_62D5RPM_IGNFK)													
														
	extern 	const	byte	M0_EN_UP_FK_PARTLOAD;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(103)	;							
	#define BYPTR_M0_EN_UP_FK_PARTLOAD M0_EN_UP_FK_PARTLOAD//ByteData_FlashAtEE(&M0_EN_UP_FK_PARTLOAD)													
														
	extern 	const	byte	M0_UL_REV_62D5RPM_UPFK;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(104)	;							
	#define BYPTR_M0_UL_REV_62D5RPM_UPFK M0_UL_REV_62D5RPM_UPFK//ByteData_FlashAtEE(&M0_UL_REV_62D5RPM_UPFK)													
														
	extern 	const	byte	M0_MODE_REV_LIMIT;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(105)	;							
	#define BYPTR_M0_MODE_REV_LIMIT M0_MODE_REV_LIMIT//ByteData_FlashAtEE(&M0_MODE_REV_LIMIT)													
														
	extern 	const	byte	M0_MIL_DELAY_S;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(106)	;							
	#define BYPTR_M0_MIL_DELAY_S M0_MIL_DELAY_S//ByteData_FlashAtEE(&M0_MIL_DELAY_S)													
														
	extern 	const	byte	M0_SOURCE_TA;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(107)	;							
	#define BYPTR_M0_SOURCE_TA M0_SOURCE_TA//ByteData_FlashAtEE(&M0_SOURCE_TA)													
														
	extern 	const	byte	M0_EN_PW;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(108)	;							
	#define BYPTR_M0_EN_PW M0_EN_PW//ByteData_FlashAtEE(&M0_EN_PW)													
														
	extern 	const	byte	M0_EN_VSPEED;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(109)	;							
	#define BYPTR_M0_EN_VSPEED M0_EN_VSPEED//ByteData_FlashAtEE(&M0_EN_VSPEED)													
														
	extern 	const	byte	M0_EN_PAIRS;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(110)	;							
	#define BYPTR_M0_EN_PAIRS M0_EN_PAIRS//ByteData_FlashAtEE(&M0_EN_PAIRS)													
														
	extern 	const	byte	M0_MODE_IDLECONTROL;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(111)	;							
	#define BYPTR_M0_MODE_IDLECONTROL M0_MODE_IDLECONTROL//ByteData_FlashAtEE(&M0_MODE_IDLECONTROL)													
														
	extern 	const	byte	M0_MODE_CA;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(112)	;							
	#define BYPTR_M0_MODE_CA M0_MODE_CA//ByteData_FlashAtEE(&M0_MODE_CA)													
														
	extern 	const	byte	M0_MODE_ACTIVE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(113)	;							
	#define BYPTR_M0_MODE_ACTIVE M0_MODE_ACTIVE//ByteData_FlashAtEE(&M0_MODE_ACTIVE)													
														
	extern 	const	byte	M0_STRCLS_TPD;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(114)	;							
	#define BYPTR_M0_STRCLS_TPD M0_STRCLS_TPD//ByteData_FlashAtEE(&M0_STRCLS_TPD)													
														
	extern 	const	byte	M0_ENGTYPE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(115)	;							
	#define BYPTR_M0_ENGTYPE M0_ENGTYPE//ByteData_FlashAtEE(&M0_ENGTYPE)													
														
	extern 	const	byte	M0_U_CO_1ST;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(116)	;							
	#define BYPTR_M0_U_CO_1ST M0_U_CO_1ST//ByteData_FlashAtEE(&M0_U_CO_1ST)													
														
	extern 	const	byte	M0_URNIDLE_CO;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(117)	;							
	#define BYPTR_M0_URNIDLE_CO M0_URNIDLE_CO//ByteData_FlashAtEE(&M0_URNIDLE_CO)													
														
	extern 	const	byte	M0_TOOTH_TDC;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(118)	;							
	#define BYPTR_M0_TOOTH_TDC M0_TOOTH_TDC//ByteData_FlashAtEE(&M0_RESERVE5)													
														
	extern 	const	byte	M0_INJFLOW;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(119)	;							
	#define BYPTR_M0_INJFLOW M0_INJFLOW//ByteData_FlashAtEE(&M0_RESERVE6)													
														
	extern 	const	byte	M0_PC_8;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(120)	;							
	#define BYPTR_M0_PC_8 M0_PC_8//ByteData_FlashAtEE(&M0_RESERVE7)													
														
	extern 	const	byte	M0_LDBMUL;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(121)	;							
	#define BYPTR_M0_LDBMUL M0_LDBMUL//ByteData_FlashAtEE(&M0_RESERVE8)													
														
	extern 	const	byte	M0_RESERVE9;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(122)	;							
	#define BYPTR_M0_RESERVE9 M0_RESERVE9//ByteData_FlashAtEE(&M0_RESERVE9)													
														
	extern 	const	byte	M0_RESERVE10;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(123)	;							
	#define BYPTR_M0_RESERVE10 M0_RESERVE10//ByteData_FlashAtEE(&M0_RESERVE10)													
														
	extern 	const	byte	M0_RESERVE11;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(124)	;							
	#define BYPTR_M0_RESERVE11 M0_RESERVE11//ByteData_FlashAtEE(&M0_RESERVE11)													
														
	extern 	const	byte	M0_RESERVE12;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(125)	;							
	#define BYPTR_M0_RESERVE12 M0_RESERVE12//ByteData_FlashAtEE(&M0_RESERVE12)													
														
	extern 	const	byte	M0_RESERVE13;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(126)	;							
	#define BYPTR_M0_RESERVE13 M0_RESERVE13//ByteData_FlashAtEE(&M0_RESERVE13)													
														
	extern 	const	byte	M0_RESERVE14;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(127)	;							
	#define BYPTR_M0_RESERVE14 M0_RESERVE14//ByteData_FlashAtEE(&M0_RESERVE14)													
														
	extern 	const	byte	M0_RESERVE15;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(128)	;							
	#define BYPTR_M0_RESERVE15 M0_RESERVE15//ByteData_FlashAtEE(&M0_RESERVE15)													
														
	extern 	const	byte	M0_RESERVE16;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(129)	;							
	#define BYPTR_M0_RESERVE16 M0_RESERVE16//ByteData_FlashAtEE(&M0_RESERVE16)													
														
	extern 	const	byte	M0_RESERVE17;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(130)	;							
	#define BYPTR_M0_RESERVE17 M0_RESERVE17//ByteData_FlashAtEE(&M0_RESERVE17)													
														
	extern 	const	byte	M0_RESERVE18;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(131)	;							
	#define BYPTR_M0_RESERVE18 M0_RESERVE18//ByteData_FlashAtEE(&M0_RESERVE18)													
														
	extern 	const	byte	M0_RESERVE19;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(132)	;							
	#define BYPTR_M0_RESERVE19 M0_RESERVE19//ByteData_FlashAtEE(&M0_RESERVE19)													
														
	extern 	const	byte	M0_RESERVE20;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(133)	;							
	#define BYPTR_M0_RESERVE20 M0_RESERVE20//ByteData_FlashAtEE(&M0_RESERVE20)													
														
	extern 	const	byte	M0_RESERVE21;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(134)	;							
	#define BYPTR_M0_RESERVE21 M0_RESERVE21//ByteData_FlashAtEE(&M0_RESERVE21)													
														
	extern 	const	byte	M0_RESERVE22;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(135)	;							
	#define BYPTR_M0_RESERVE22 M0_RESERVE22//ByteData_FlashAtEE(&M0_RESERVE22)													
														
	extern 	const	byte	M0_RESERVE23;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(136)	;							
	#define BYPTR_M0_RESERVE23 M0_RESERVE23//ByteData_FlashAtEE(&M0_RESERVE23)													
														
	extern 	const	byte	M0_RESERVE24;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(137)	;							
	#define BYPTR_M0_RESERVE24 M0_RESERVE24//ByteData_FlashAtEE(&M0_RESERVE24)													
														
	extern 	const	byte	M0_RESERVE25;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(138)	;							
	#define BYPTR_M0_RESERVE25 M0_RESERVE25//ByteData_FlashAtEE(&M0_RESERVE25)													
														
	extern 	const	byte	M0_RESERVE26;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(139)	;							
	#define BYPTR_M0_RESERVE26 M0_RESERVE26//ByteData_FlashAtEE(&M0_RESERVE26)													
														
	extern 	const	byte	M0_RESERVE27;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(140)	;							
	#define BYPTR_M0_RESERVE27 M0_RESERVE27//ByteData_FlashAtEE(&M0_RESERVE27)													
														
	extern 	const	byte	M0_RESERVE28;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(141)	;							
	#define BYPTR_M0_RESERVE28 M0_RESERVE28//ByteData_FlashAtEE(&M0_RESERVE28)													
														
	extern 	const	byte	M0_CCPS_ti_DurDFCORunTmLim_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(142)	;							
	#define BYPTR_M0_CCPS_ti_DurDFCORunTmLim_C M0_CCPS_ti_DurDFCORunTmLim_C// ByteData_FlashAtEE(&M0_CCPS_ti_DurDFCORunTmLim_C)	//20201201 chenwei add CCPS:142~154												
														
	extern 	const	byte	M0_CCPS_ti_DurPERunTmLim_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(143)	;							
	#define BYPTR_M0_CCPS_ti_DurPERunTmLim_C M0_CCPS_ti_DurPERunTmLim_C// ByteData_FlashAtEE(&M0_CCPS_ti_DurPERunTmLim_C)													
														
	extern 	const	byte	M0_CCPS_ti_CldStrTmtMin_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(144)	;							
	#define BYPTR_M0_CCPS_ti_CldStrTmtMin_C M0_CCPS_ti_CldStrTmtMin_C// ByteData_FlashAtEE(&M0_CCPS_ti_CldStrTmtMin_C)													
														
	extern 	const	byte	M0_CCPS_ti_WrmStrtTmMin_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(145)	;							
	#define BYPTR_M0_CCPS_ti_WrmStrtTmMin_C M0_CCPS_ti_WrmStrtTmMin_C//ByteData_FlashAtEE(&M0_CCPS_ti_WrmStrtTmMin_C)													
														
	extern 	const	byte	M0_CCPS_t_EngCydTmpHi_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(146)	;							
	#define BYPTR_M0_CCPS_t_EngCydTmpHi_C M0_CCPS_t_EngCydTmpHi_C//ByteData_FlashAtEE(&M0_CCPS_t_EngCydTmpHi_C)													
														
	extern 	const	byte	M0_CCPS_t_EngCydTmpLo_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(147)	;							
	#define BYPTR_M0_CCPS_t_EngCydTmpLo_C M0_CCPS_t_EngCydTmpLo_C//ByteData_FlashAtEE(&M0_CCPS_t_EngCydTmpLo_C)													
														
	extern 	const	byte	M0_CCPS_Pct_TPSMax_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(148)	;							
	#define BYPTR_M0_CCPS_Pct_TPSMax_C M0_CCPS_Pct_TPSMax_C//ByteData_FlashAtEE(&M0_CCPS_Pct_TPSMax_C)													
														
	extern 	const	byte	M0_CCPS_Pct_TPSMin_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(149)	;							
	#define BYPTR_M0_CCPS_Pct_TPSMin_C M0_CCPS_Pct_TPSMin_C//ByteData_FlashAtEE(&M0_CCPS_Pct_TPSMin_C)													
														
	extern 	const	byte	M0_CCPS_n_EngSpdHi_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(150)	;							
	#define BYPTR_M0_CCPS_n_EngSpdHi_C M0_CCPS_n_EngSpdHi_C//ByteData_FlashAtEE(&M0_CCPS_n_EngSpdHi_C)													
														
	extern 	const	byte	M0_CCPS_n_EngSpdLo_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(151)	;							
	#define BYPTR_M0_CCPS_n_EngSpdLo_C M0_CCPS_n_EngSpdLo_C//ByteData_FlashAtEE(&M0_CCPS_n_EngSpdLo_C)													
														
	extern 	const	byte	M0_CCPS_u_BattVltgMax_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(152)	;							
	#define BYPTR_M0_CCPS_u_BattVltgMax_C M0_CCPS_u_BattVltgMax_C//ByteData_FlashAtEE(&M0_CCPS_u_BattVltgMax_C)													
														
	extern 	const	byte	M0_CCPS_ti_O2RichTmMax_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(153)	;							
	#define BYPTR_M0_CCPS_ti_O2RichTmMax_C M0_CCPS_ti_O2RichTmMax_C//ByteData_FlashAtEE(&M0_CCPS_ti_O2RichTmMax_C)													
														
	extern 	const	byte	M0_CCPS_swt_CCPSStrt_C;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(154)	;							
	#define BYPTR_M0_CCPS_swt_CCPSStrt_C M0_CCPS_swt_CCPSStrt_C//ByteData_FlashAtEE(&M0_CCPS_swt_CCPSStrt_C)													
														
	extern 	const	byte	M0_OverTMPProtection_Te;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(155)	;							
	#define BYPTR_M0_OverTMPProtection_Te M0_OverTMPProtection_Te//ByteData_FlashAtEE(&M0_RESERVE42)													
														
	extern 	const	byte	M0_T_OVERTMP;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(156)	;							
	#define BYPTR_M0_T_OVERTMP M0_T_OVERTMP//ByteData_FlashAtEE(&M0_RESERVE43)													
														
	extern 	const	byte	M0_OverTMPProtection_SWITCH;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(157)	;							
	#define BYPTR_M0_OverTMPProtection_SWITCH M0_OverTMPProtection_SWITCH//ByteData_FlashAtEE(&M0_RESERVE44)													
														
	extern 	const	byte	M0_CC_NC_SPEED;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(158)	;							
	#define BYPTR_M0_CC_NC_SPEED M0_CC_NC_SPEED//ByteData_FlashAtEE(&M0_RESERVE45)		//20211008 chenwei add���ɰ�ȫ�����ٶ���ת��											
														
	extern 	const	byte	M0_CC_NR_SPEED;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(159)	;							
	#define BYPTR_M0_CC_NR_SPEED M0_CC_NR_SPEED//ByteData_FlashAtEE(&M0_RESERVE46)													
														
	//Parameters
	extern 	const	byte	M0_FMF_IDLE_RICHTOLEAN;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(160)	;							
	#define BYPTR_M0_FMF_IDLE_RICHTOLEAN M0_FMF_IDLE_RICHTOLEAN//ByteData_FlashAtEE(&M0_FMF_IDLE_RICHTOLEAN)													
														
	extern 	const	byte	M0_FMF_IDLE_LEANTORICH;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(161)	;							
	#define BYPTR_M0_FMF_IDLE_LEANTORICH M0_FMF_IDLE_LEANTORICH//ByteData_FlashAtEE(&M0_FMF_IDLE_LEANTORICH)													
														
	extern 	const	byte	M0_T_SPRAY_FLY_TO_INLET_D1MS;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(162)	;							
	#define BYPTR_M0_T_SPRAY_FLY_TO_INLET_D1MS M0_T_SPRAY_FLY_TO_INLET_D1MS//ByteData_FlashAtEE(&M0_T_SPRAY_FLY_TO_INLET_D1MS)													
														
	extern 	const	byte	M0_UL_CYCLE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(163)	;							
	#define BYPTR_M0_UL_CYCLE M0_UL_CYCLE//ByteData_FlashAtEE(&M0_UL_CYCLE)
	
	extern 	const	byte	M0_STEPBASE;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(164)	;							
	#define BYPTR_M0_STEPBASE M0_STEPBASE//ByteData_FlashAtEE(&M0_STEPBASE)
	
	extern 	const	byte	M0_PARAMETER_RES2;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(165)	;							
	#define BYPTR_M0_PARAMETER_RES2 M0_PARAMETER_RES2//ByteData_FlashAtEE(&M0_PARAMETER_RES2)
	
	extern 	const	byte	M0_PARAMETER_RES3;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(166)	;							
	#define BYPTR_M0_PARAMETER_RES3 M0_PARAMETER_RES3//ByteData_FlashAtEE(&M0_PARAMETER_RES3)
	
	extern 	const	byte	M0_PARAMETER_RES4;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(167)	;							
	#define BYPTR_M0_PARAMETER_RES4 M0_PARAMETER_RES4//ByteData_FlashAtEE(&M0_PARAMETER_RES4)
	
	extern 	const	byte	M0_PARAMETER_RES5;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(168)	;							
	#define BYPTR_M0_PARAMETER_RES5 M0_PARAMETER_RES5//ByteData_FlashAtEE(&M0_PARAMETER_RES5)
	
	extern 	const	byte	M0_PARAMETER_RES6;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(169)	;							
	#define BYPTR_M0_PARAMETER_RES6 M0_PARAMETER_RES6//ByteData_FlashAtEE(&M0_PARAMETER_RES6)
	
	extern 	const	byte	M0_PARAMETER_RES7;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(170)	;							
	#define BYPTR_M0_PARAMETER_RES7 M0_PARAMETER_RES7//ByteData_FlashAtEE(&M0_PARAMETER_RES7)
	
	extern 	const	byte	M0_PARAMETER_RES8;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(171)	;							
	#define BYPTR_M0_PARAMETER_RES8 M0_PARAMETER_RES8//ByteData_FlashAtEE(&M0_PARAMETER_RES8)
	
	extern 	const	byte	M0_PARAMETER_RES9;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(171)	;							
	#define BYPTR_M0_PARAMETER_RES9 M0_PARAMETER_RES9//ByteData_FlashAtEE(&M0_PARAMETER_RES9)
	
	extern 	const	byte	M0_PARAMETER_RES10;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(173)	;							
	#define BYPTR_M0_PARAMETER_RES10 M0_PARAMETER_RES10//ByteData_FlashAtEE(&M0_PARAMETER_RES10)
	
	extern 	const	byte	M0_PARAMETER_RES11;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(174)	;							
	#define BYPTR_M0_PARAMETER_RES11 M0_PARAMETER_RES11//ByteData_FlashAtEE(&M0_PARAMETER_RES11)
	
	extern 	const	byte	M0_PARAMETER_RES12;//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(175)	;							
	#define BYPTR_M0_PARAMETER_RES12 M0_PARAMETER_RES12//ByteData_FlashAtEE(&M0_PARAMETER_RES12)
	
	//IACV
	extern 	const	byte	M1_STEPINC_PAIR[8];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(176)	;
	#define BYPTR_M1_STEPINC_PAIR M1_STEPINC_PAIR//Ptr_FlashAtEE(M1_DN0_HTOLNOISCV_15D6RPM)
	
	extern 	const	sbyte	M1_STEPCO_PT[4][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(184)	;
	#define BYPTR_M1_STEPCO_PT M1_STEPCO_PT//Ptr_FlashAtEE(M1_STEPCO_PT)
	
	extern 	const	byte	M1_STEPINI[4][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(192)	;									
	#define BYPTR_M1_STEPINI M1_STEPINI//;Ptr_FlashAtEE(M1_STEPINI)

	extern 	const	sbyte	M1_STEINC_LIDLE[2][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(200)	;									
	#define BYPTR_M1_STEINC_LIDLE M1_STEINC_LIDLE//Ptr_FlashAtEE(M1_STEINC_LIDLE)	

	extern 	const	sbyte	M1_STEINC_BIDLE[2][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(204)	;									
	#define BYPTR_M1_STEINC_BIDLE M1_STEINC_BIDLE//Ptr_FlashAtEE(M1_STEINC_BIDLE)		
																
	extern 	const	sbyte	M1_STEPDEC[3][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(208)	;									
	#define BYPTR_M1_STEPDEC M1_STEPDEC//Ptr_FlashAtEE(M1_STEPDEC)

	extern 	const	byte	N_M2LAMBDA[2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(214)	;									
	#define BYPTR_N_M2LAMBDA N_M2LAMBDA//Ptr_FlashAtEE(N_M2LAMBDA)	
																
	//extern 	const	byte	TE_M2STEPINI[4]	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(212)	;									
	//#define BYPTR_TE_M2STEPINI Ptr_FlashAtEE(TE_M2STEPINI)
	
	extern 	const	byte	STEP_M2LAMBDA[8];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(216)	;									
	#define BYPTR_STEP_M2LAMBDA STEP_M2LAMBDA//Ptr_FlashAtEE(STEP_M2LAMBDA)															

	extern 	const	sbyte	M2_STEPLAMBDA[2][8];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(224)	;									
	#define BYPTR_M2_STEPLAMBDA M2_STEPLAMBDA//Ptr_FlashAtEE(M2_STEPLAMBDA)															
	extern 	const	byte	M1_CCPS_Pct_DutyCycle[8][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(272)	;							
	#define BYPTR_M1_CCPS_Pct_DutyCycle M1_CCPS_Pct_DutyCycle//Ptr_FlashAtEE(M1_CCPS_Pct_DutyCycle)  //20201126 chenwei add
	extern 	const	byte	M1_T_O2H_DELAY[8][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(288)	;							
	#define BYPTR_M1_T_O2H_DELAY M1_T_O2H_DELAY//Ptr_FlashAtEE(M1_T_O2H_DELAY)  //20211010 chenwei add

	extern 	const	byte	M1_SPEED_O2H_THRESH[8][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(304)	;							
	#define BYPTR_M1_SPEED_O2H_THRESH M1_SPEED_O2H_THRESH//Ptr_FlashAtEE(M1_SPEED_O2H_THRESH)  //20211010 chenwei add

	extern 	const	byte	M1_TPD_O2H_THRESH[8][2];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(320)	;							
	#define BYPTR_M1_TPD_O2H_THRESH M1_TPD_O2H_THRESH//Ptr_FlashAtEE(M1_TPD_O2H_THRESH)  //20211010 chenwei add
																																												
	/*----------basic load-----------------*/													
	extern 	const	byte	M2_LDB_TPSRPM[17][16];/*272 A35-P51*///	@	ADD_FLASH_BYTE(LDB)+(unsigned long int)(0)	;							
	#define BYPTR_M2_LDB_TPSRPM M2_LDB_TPSRPM//Ptr_FlashAtEE(M2_LDB_TPSRPM)													
														
														
	/*----------basic load correction-----------------*/													
	extern 	const	byte	M2_MPA_M128[8][16];/*128 A68-P75*///	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(0)	;							
	#define BYPTR_M2_MPA_M128 M2_MPA_M128//Ptr_FlashAtEE(M2_MPA_M128)													
														
	extern 	const	byte	M0_T_M64;//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(8*16+0)	;							
	#define BYPTR_M0_T_M64 M0_T_M64//ByteData_FlashAtEE(&M0_T_M64)

	extern 	const	byte	M0_IDLEFMF_CO;//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(8*16+1)	;							
	#define BYPTR_M0_IDLEFMF_CO M0_IDLEFMF_CO//ByteData_FlashAtEE(&M0_IDLEFMF_CO)
	
	/*extern 	const	sbyte	M1_DT1_INI[3][2]	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(8*16+2)	;							
	#define BYPTR_M1_DT1_INI Ptr_FlashAtEE(M1_DT1_INI)*/

	extern 	const	byte	M2_V_T_IGNON[16][16];//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(11*16+0)	;							
	#define BYPTR_M2_V_T_IGNON M2_V_T_IGNON//Ptr_FlashAtEE(M2_V_T_IGNON)		//20210601 cw add ���,20210925 ��Ϊ16*16

	//20210926 chennwei add �������
	extern	const byte	N_62D5_M2IGNON[16];//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(27*16+0) 	;							
	#define 	BYPTR_N_62D5_M2IGNON	N_62D5_M2IGNON//Ptr_FlashAtEE(N_62D5_M2IGNON)											
														
	extern	const byte	VBAT_M2IGNON[16];//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(28*16+0) 	;						
	#define 	BYPTR_VBAT_M2IGNON	VBAT_M2IGNON//Ptr_FlashAtEE(VBAT_M2IGNON)
														
	/*----------lambda control & fuel plan-----------------*/													
	extern 	const	sbyte	M1_US_M32[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(0)	;							
	#define BYPTR_M1_US_M32 M1_US_M32//Ptr_FlashAtEE(M1_US_M32)													
														
	extern 	const	sbyte	M1_F_M256[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(1*16+0)	;							
	#define BYPTR_M1_F_M256 M1_F_M256//Ptr_FlashAtEE(M1_F_M256)													
														
	extern 	const	sbyte	M1_G0_M128[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(2*16+0)	;							
	#define BYPTR_M1_G0_M128 M1_G0_M128//Ptr_FlashAtEE(M1_G0_M128)													
														
	extern 	const	sbyte	M1_E_M4[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(3*16+0)	;							
	#define BYPTR_M1_E_M4 M1_E_M4//Ptr_FlashAtEE(M1_E_M4)													
														
	extern 	const	byte	M1_NR0_62D5RPM[16];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(4*16+0)	;							
	#define BYPTR_M1_NR0_62D5RPM M1_NR0_62D5RPM//Ptr_FlashAtEE(M1_NR0_62D5RPM)													
														
	extern 	const	sbyte	M1_UR_M16[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(5*16+0)	;							
	#define BYPTR_M1_UR_M16 M1_UR_M16//Ptr_FlashAtEE(M1_UR_M16)													
														
	extern 	const	byte	M1_NC_62D5[16];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(6*16+0)	;							
	#define BYPTR_M1_NC_62D5 M1_NC_62D5//Ptr_FlashAtEE(M1_NC_62D5)													
														
	extern 	const	sbyte	M1_UA_M32[8][2];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(7*16+0)	;							
	#define BYPTR_M1_UA_M32 M1_UA_M32//Ptr_FlashAtEE(M1_UA_M32)													
														
	extern 	const	sbyte	M2_UP[17][16];//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(8*16+0)	;							
	#define BYPTR_M2_UP M2_UP//Ptr_FlashAtEE(M2_UP)													
														
	extern 	const	byte	M0_F0_M256;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+0)	;							
	#define BYPTR_M0_F0_M256 M0_F0_M256//ByteData_FlashAtEE(&M0_F0_M256)													
														
	extern 	const	byte	M0_g_M128;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+1)	;							
	#define BYPTR_M0_g_M128 M0_g_M128//ByteData_FlashAtEE(&M0_g_M128)													
														
	extern 	const	byte	M0_X;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+2)	;							
	#define BYPTR_M0_X M0_X//ByteData_FlashAtEE(&M0_X)													
														
	extern 	const	byte	M0_Y_M256;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+3)	;							
	#define BYPTR_M0_Y_M256 M0_Y_M256//ByteData_FlashAtEE(&M0_Y_M256)													
														
	extern 	const	byte	M0_Z;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+4)	;							
	#define BYPTR_M0_Z M0_Z//ByteData_FlashAtEE(&M0_Z)													
														
	extern 	const	byte	M0_R_D4;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+5)	;							
	#define BYPTR_M0_R_D4 M0_R_D4//ByteData_FlashAtEE(&M0_R_D4)													
														
	extern 	const	byte	M0_f_M128;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+6)	;							
	#define BYPTR_M0_f_M128 M0_f_M128//ByteData_FlashAtEE(&M0_f_M128)													
														
	extern 	const	byte	M0_NRT_M64;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+7)	;							
	#define BYPTR_M0_NRT_M64 M0_NRT_M64//ByteData_FlashAtEE(&M0_NRT_M64)													
														
	extern 	const	byte	M0_C_M8;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+8)	;							
	#define BYPTR_M0_C_M8 M0_C_M8//ByteData_FlashAtEE(&M0_C_M8)													
														
	extern 	const	byte	M0_D_M256;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+9)	;							
	#define BYPTR_M0_D_M256 M0_D_M256//ByteData_FlashAtEE(&M0_D_M256)													
														
	extern 	const	byte	M0_B_D8;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+10)	;							
	#define BYPTR_M0_B_D8 M0_B_D8//ByteData_FlashAtEE(&M0_B_D8)													
														
	extern 	const	byte	M0_P_DEC_M64;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+11)	;							
	#define BYPTR_M0_P_DEC_M64 M0_P_DEC_M64//ByteData_FlashAtEE(&M0_P_DEC_M64)													
														
	extern 	const	byte	M0_S_ACC_M128;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+12)	;							
	#define BYPTR_M0_S_ACC_M128 M0_S_ACC_M128//ByteData_FlashAtEE(&M0_S_ACC_M128)													
														
	extern 	const	byte	M0_L_ACC_M1024;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+13)	;							
	#define BYPTR_M0_L_ACC_M1024 M0_L_ACC_M1024//ByteData_FlashAtEE(&M0_L_ACC_M1024)													
														
	extern 	const	byte	M0_P_ACC_M64;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+14)	;							
	#define BYPTR_M0_P_ACC_M64 M0_P_ACC_M64//ByteData_FlashAtEE(&M0_P_ACC_M64)													
														
	extern 	const	byte	M0_S_DEC_M128;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+15)	;							
	#define BYPTR_M0_S_DEC_M128 M0_S_DEC_M128//ByteData_FlashAtEE(&M0_S_DEC_M128)													
														
	extern 	const	byte	M0_L_DEC_M1024;//	@	ADD_FLASH_BYTE(LAMBDAANDFUEL)+(unsigned long int)(25*16+16)	;							
	#define BYPTR_M0_L_DEC_M1024 M0_L_DEC_M1024//ByteData_FlashAtEE(&M0_L_DEC_M1024)													
														
	/*----------ignition-----------------*/													
	extern 	const	byte	M2_IGN_TPSRPM[17][16];//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(0)	;							
	#define BYPTR_M2_IGN_TPSRPM M2_IGN_TPSRPM//Ptr_FlashAtEE(M2_IGN_TPSRPM)													
														
	extern 	const	sbyte	M2_IGC[8][2];//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(17*16+0)	;							
	#define BYPTR_M2_IGC M2_IGC//Ptr_FlashAtEE(M2_IGC)													
														
	extern 	const	sbyte	M2_IGT[10][2];//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(18*16+0)	;							
	#define BYPTR_M2_IGT M2_IGT//Ptr_FlashAtEE(M2_IGT)													
														
	extern 	const	sbyte	M0_IGNMAX_IDLE;//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(20*16+0)	;							
	#define BYPTR_M0_IGNMAX_IDLE M0_IGNMAX_IDLE//(sbyte)ByteData_FlashAtEE(&M0_IGNMAX_IDLE)													
														
	extern 	const	sbyte	M0_IGNMIN_IDLE;//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(20*16+1)	;							
	#define BYPTR_M0_IGNMIN_IDLE M0_IGNMIN_IDLE//(sbyte)ByteData_FlashAtEE(&M0_IGNMIN_IDLE)													
														
	extern 	const	byte	M0_IGS;//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(20*16+2)	;							
	#define BYPTR_M0_IGS M0_IGS//ByteData_FlashAtEE(&M0_IGS)													
														
	extern 	const	byte	M0_O;//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(20*16+3)	;							
	#define BYPTR_M0_O M0_O//ByteData_FlashAtEE(&M0_O)													
														
	extern 	const	byte	M0_M;//	@	ADD_FLASH_BYTE(PN_IGN)+(unsigned long int)(20*16+4)	;							
	#define BYPTR_M0_M M0_M//ByteData_FlashAtEE(&M0_M)													
														
														
	extern	const byte	N_62D5_M2LDB[16];//	@	ADD_FLASH_BYTE(LDB)+(unsigned long int)(288) 	;	//	18						
	#define 	BYPTR_N_62D5_M2LDB	N_62D5_M2LDB//Ptr_FlashAtEE(N_62D5_M2LDB)											
														
	extern	const byte	TPD_M2LDB[17];//	@	ADD_FLASH_BYTE(LDB)+(unsigned long int)(304) 	;	//	304						
	#define 	BYPTR_TPD_M2LDB	TPD_M2LDB//Ptr_FlashAtEE(TPD_M2LDB)											
														
	extern	const byte	PAIR_DEG[8];//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(160) 	;	//	128�ǳ�ͻ ������Ϊ144	10		ע�⣺����ǲ���FLASH--SCR�е�			
	#define 	BYPTR_PAIR_DEG	PAIR_DEG//Ptr_FlashAtEE(PAIR_DEG)																							

	extern	const sbyte	M1_PREINJ[4][2];//	@	ADD_FLASH_BYTE(LDBC)+(unsigned long int)(168) 	;				
	#define 	BYPTR_M1_PREINJ	M1_PREINJ//Ptr_FlashAtEE(M1_PREINJ)

	extern 	const	byte	M2_UP_OVERTEP_PROTECTION[5][6];//	@	ADD_FLASH_BYTE(DATAANDSPEED)+(unsigned long int)(15*16+0)	;							
	#define BYPTR_M2_UP_OVERTEP_PROTECTION M2_UP_OVERTEP_PROTECTION//Ptr_FlashAtEE(M2_UP_OVERTEP_PROTECTION)    //20210112 chenwei add ���±���up����

	extern	const byte	N_62D5_M2LDB1[6];//	@	ADD_FLASH_BYTE(LDB)+(unsigned long int)(321) 	;							
	#define 	BYPTR_N_62D5_M2LDB1	N_62D5_M2LDB1//Ptr_FlashAtEE(N_62D5_M2LDB1)											
														
	extern	const byte	TPD_M2LDB1[5];//	@	ADD_FLASH_BYTE(LDB)+(unsigned long int)(327) 	;						
	#define 	BYPTR_TPD_M2LDB1	TPD_M2LDB1//Ptr_FlashAtEE(TPD_M2LDB1)							
																																																
/*================================================================================================*/


//=====================================================================================
//�����Ƕ���ɢmap��������
//��Ӧ��map�ļ�Ϊmap_PNX_Motor.c
extern const word   INJ_REACH_BMS ;//     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
extern const byte   PRE_OIL_INC  ;//           50;//Ԥ���������������4US       EQU     50 ;20
//extern const byte   M0_FMF_IDLE    ;//             72;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder;//0.2/1500rpm is 86*0.004+4.344;//4.
extern const byte   WAIT_RUN_STARTB  ;//      	30;//�뿪����ʱ�м��ͺͼ�����ԣ���ֹ���������뿪���ٵ�ʱ������̣���������֮�����С���ΪWAIT_RUN_STARTB������Ȧ			

//����������for idle missfire������������������������������������������
extern const byte   MAX_TI_INC_DRPM	;//		25;//ʧ��������������������4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
extern const byte   MAX_MISS_FIRE_DTI;//		12;//���ۼ�ʧ���������ڸ�ֵʱ��ֻ�Խ�����ʱʧ������//equ		12		;2007-7-17����ʱ���ۼ�ʧ�����48US
extern const byte   RPM_DEC_FOR_LEANS;//		8;//2008-5-30 change from 5 to 8//��Ȧ��ת���½�>;//93 (6*15.625),��O2dec,��Ϊϡ,��4US�����٣�15.6rpm
extern const byte   RPM_DEC_FOR_MISFIRE	;//	10;//2008-5-30 7//��Ȧ��ת���½�>;//109 (7*15.625),��Ϊʧ�� �����٣�15.6rpm
extern const byte   RPM_DEC_2C_MISSFIRE;//		13;//2008-5-30 11//����ѭ�����½�>;//156��Ϊȼ�ղ��ã����٣�15.6rpm
extern const byte   RPM_DEC_3C_MISSFIRE	;//	15;//2008-5-30 13//������ѭ�����½�>;//187��Ϊȼ�ղ��ã����٣�15.6rpm
extern const byte   STEP_TI_INC_AMISF;//		1;//2008-5-30 3//һ��ʧ���������,������4US; v_dmidle inc by step_ti_inc_amisf each misfire, 
extern const byte   STEP_A_SMALL_FIRE;//		1;//2008-5-30 2//һ��ȼ�ղ��õ����� ������4US;

//--for cf������������������������
extern const byte M0_MIN_STOPOIL_RUN;//7;//���ۼƶ���Ȧ��������ֵʱ������ҪCI����?	
extern const byte M0_INJ_AG_INC ;//             75;//CI�͸��뿪����ʱ������Ȧ�������Ļ���ֵ������4US;equ	75	;the first inj after stpoil inc oil by75*4us
extern const byte M0_ACCELDT_MAX;//	           150;//������������ʱ��v_Ka_b1���˵�����ϵ���ܺ͵����ֵ��	
extern const byte M0_MAX_RPM_AQ;//  118;//�����Զ��������ܵ�ת�����ޣ�����15.6RPM��ת��Ϊ1843
extern const word M0_TIME_TO_CDI_H ;// 0x0032 ;      //CDI�ŵ�֮�󣬵��´γ��֮���ʱ��;;0.8MS����λ:4US
extern const byte  M0_MAX_RPM_CONIDLE;// 106;         //���ٿ��Ƶ����ת�٣���λ15.6RPM// equ	106	;when the average of rpm156 over 16 runs>;//106,clear conti_idle_time
extern const byte  M0_MIN_RPM_CONIDLE;//	77;          //���ٿ��Ƶ����ת�٣���λ15.6RPM//equ	77	;1203 rpm 2004.10.28;82 ;03.9.1 ;77	;03.8.22;71	;when the v_rev_156ave<which,1109rpm, clear conti_idle_time 
extern const byte  M0_NBTDCO_USED_IGN;//	50;          //�����ŵ���Ƕ�С�ڸ�ֵ��ת�ٸ���PCAH2929_IGNʱ��һ��⵽�Ǳ�͵�𣬵�λ0.7��CA?

extern const byte  M0_RZT_HOT_NOOIL;//		72;  //	equ		72		;��RZT>;//72,��Ϊ�ȳ�,���ٶ���ת�ٲ��ټ�С�����٣����¶Ȳ���ֵһ��
extern const byte  M0_RPM_DEC_REOIL	;//	20 ; //	equ		20		;�����ٹ���ת�ٵļ�С��Ϊ(20-(72-v_Teng_inuse)/4),�ȳ���Ϊ<2250�����٣����¶Ȳ���ֵһ��
extern const byte  M0_RPM_NOOIL_DEC_TOILDE;// 18; //	equ		18		;TOIDLEʱ�Ķ���ת�ټ�С��(18-(72-v_Teng_inuse)/4), �ȳ������Ϊ>;//2375�����٣����¶Ȳ���ֵһ��
extern const byte  M0_IDLE_STOP_SP;// 0x75  ;     //��STARTS�׶ε���͹���ת��	EQU	075H	//;	when	v_prd_cas_h>;//75h:rpm<500	not	inj	when	if	motor	started
extern const byte  M0_START_SPEED  ;//  0xD1;     //STARTS�׶ε���͹���ת��
extern const byte  M0_MAX_START_RPM ;// 0x06;     //�ж������Ǳ�ʱ�����ܴﵽ���������ת��?
extern const byte  M0_JQM_DRY_IGTOR;// 15 ;       //���������Զ�Ӧ�����TPS���𣬼�������NOT MMOVEʱ���ж����ż�����ڵ��ڸ�ֵʱ��������EQU	15	;WHEN	JQM>;//15,	FOR	DRY	INGITOR
extern const byte  M0_TES_MAX;//	235     ;        //����ͷ�¶ȴ���������ֵ���ڸ�ֵ����Ϊ�������й��ϣ�����ͣ��//EQU	235	;WHEN	V_TENG_INUSE>M0_TES_MAX,	STOP	OIL	NOT	CONSIDER	THE	MOTOR	STATUS				
extern const byte  M0_TES_FOR_IDLENOTTOOLONG;//	210;         //����ͷ�¶ȴ���������ֵ���ڸ�ֵ����������ʱ�䳬��һ��ֵ֮��ͣ��//EQU	200	;2004.6.22,V_TENG_INUSE>;//200	AND	RUNNING	IN	IDLE	FOR	TWO	MINITE	STOP	INJ			
                                                //2008-11-7 CHANGE FROM 200 TO 210
//2008-11-2 move here
extern const byte  M0_MIN_RZT_TI_SH;//	50//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND;//0,	NO	TI_SHIFT	GET	
extern const byte  M0_MAX_RZT_TI_SH;//	192//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND;//0,	NO	TI_SHIFT	GET	
//����for tmp
extern const byte  M0_TAS_MAX;//	            90;//�����¶ȴ���������ֵ�������ֵ�����ڸ�ֵ�ж�Ϊ�й��ϣ�EQU	90	;WHEN	AIR	TEMPERATURE	MEASURED	>63DEGREE,	SNSOR	ERRO		
extern const byte  M0_TAS_MIN;//	            2;//�����¶ȴ���������ֵ�������ֵ��С�ڸ�ֵ�ж�Ϊ�й��ϣ�//EQU	2		
extern const byte  M0_TES_MIN;//	            1;//ů��״̬�£���ͷ�¶ȴ���������С��ֵ��С�ڸ�ֵ�ж�Ϊ�й��ϣ�/EQU	1	;-30DC,IF	V_TENG_INUSE<M0_TES_MIN	OR	V_TENG_INUSE>RZT_MAX	FOR	15	TIMES,		
extern const byte  M0_TAS_MAX_INUSE	;//       60;//�����¶ȴ���������ֵ�������ֵ��ʹ��ʱ�����ֵ�������ڸ�ֵ������ֵʹ�ã�EQU	60	;2005.6.1	THE	MAX	OF	V_TAIR_INUSE	IS	45DEGREE	FOR	V_TAIR_AD	SAMPLING		
extern const byte  M0_TAS_MAX_PREDICTED	;//   50;//�����¶ȴ�����Ԥ��ֵֵ�������ֵ��EQU	50	;2004.9.20,37	DEGREE	;45	;2004.8.29	34	DEGREE	;60	47	DEGREE		
extern const byte  MO_TES_LLIMITFORPUMP;  //130 //��TES���ڸ��¶�ֵ���ҷ���������ʱ��̣ܶ�����������û������ͣ��ʱ������Ҫ���� ��      

extern const byte M0_PCAH_DOWNRPM   ;       //   29;  //;2007-4-9 when rpm<2020,down the rpm to pc by some value  ,����ͬPCAH
extern const byte  M0_TENG_LT_GUOSAN       ;    //=    25;//��������ʱ���¶��������
extern const byte M0_IGN_MAX_L6000RPM;      //=50;							//35		��ת��ʱ,�Ե��ٵ��ǵ�����							
extern const byte M0_IGN_MAX_GT6000RPM;     //=57;							//40		57.14285714							

/*��������map*/ 
extern const word M0_FMF_1ST_BASIC;//=128;//������һȦ�Ļ�������	
extern const word M0_FMF_PREINJ;//=128;   //����Ԥ������ :						5.632
//2010-1-18 ADD ADD FOT IT	

extern const byte M0_TES_FOR_1STRUN_FMF_CORRECTION;//27,������ʱ,�¶ȵ��ڸ�ֵʱ,��Ҫ��������,����������ΪM0_TES_FOR_1STRUN_FMF_CORRECTION-teng							

/*��������ֹʱ��һЩ����*/
extern const word  M0_T_DELAYED_AFTER_KEYOFF_S;//  3600    //ECU��Կ�׹رպ��M0_T_DELAYED_AFTER_KEYOFF_S��ʱ,��ȫ�رյ�Դ
extern const word  M0_T_INTERVAL_COM_WHENSTOP_4US;//  5000  //��λ:4us,������δ����ʱ,ͨѶ���ٶ�;��������֮���ʱ����;
extern const word  M0_T_KEYOFF_SHORTLY_S;//   1800          //��λ:s,Կ�׹ر�ʱ��С�ڸ�ֵʱ,�� VF_OFFSHORT_KEYλ

/*os�ջ����Ʋ���*/
extern const byte MO_STEP_FMF_IDLELEAN;// 1         //���ٷ���������ϡʱ�����ڵ�����������λ��ͬFMF	
extern const byte MO_STEP_FMF_IDLE_RICHTOLEAN;// 4         //���ٷ�������Ũ��ϡ����Ҫ���ӵ���������λ��ͬFMF 
extern const byte MO_STEP_FMF_IDLE_LEANTORICH;// 4      //���ٷ�������ϡ��Ũ����Ҫ��������������λ��ͬFMF
extern const byte MO_FMF_RICHLIMIT_WOT_NOTFK;//	4       //����ȫ������������ʱ����������ļ��������෴���������Ҫ���ǡ�=0
extern const byte MO_FMF_CLS_INIT;// 0          //�����󣬵��ٷ��������ĳ�ʼֵ
extern const byte MO_FMFENRICH_BACKTOIDLE_CLS    ;// 2       //2008-11-13 ADD 
extern const byte MO_DFMF_LEANTOOLONG_WHENIDLE;// 25//����1s�ڣ����ٷ���������֮ǰ��ƽ��ֵ�߳���ֵ������OSһ��ָʾƫϡ������ΪOS�����⣻ 
extern const byte MO_DFMF_LIMIT_LEANTOOLONG_WHENIDLE;// 4   //2008-11-14 ��������������ʱ���Ե���������ֵ������ֵ
extern const byte MO_SUBDFMF_LIMIT_LEANTOOLONG_WHENIDLE;// 10   //2008-11-14 ��������������ʱ���Ե��ټ�����ֵ������ֵ

extern const byte M0_CCL_ENRICH_CNTLEAN_IDLE ;//      16//;��������ϡ,���͵�����Ȧ��,����?6Ȧ,��һ����
extern const byte M0_CCL_ENRICH_CNTLEAN_IDLE_HSP;//        24//;����ת�ٸ�ʱ����ϡ,�����͵�Ȧ��
extern const byte M0_CCL_ENRICH_CNTLEAN_IDLE_CI;//         16//;����ʱ,�ٹ��ͽ���ǰ,����ϡ,��Ҫ���͵�Ȧ��,
extern const byte M0_CCL_DERICH_CNTRICH_IDLE  ;//     24//;��������Ũ,���͵�����Ȧ��,����??6Ȧ,����һ��
extern const byte MO_STEP_FMF_NOTIDLE    ;// 4           //�Ǽ����ڼ�,�ǵ��ٷ���ʱ��ϡŨ�仯ʱ������������ֵ��
extern const byte MO_STEP_FMF_ACCEL  ;//  5           //�����ڼ䣬ϡŨ�仯ʱ������������ֵ��
extern const byte MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST ;//  6   //������������������Ϊϡ�Ĵ���ʱ����Ҫ���������ٲ������������ָ�����
                                                //��nb_o2change_afteridletoofast��>MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST֮��ָ�����
extern const byte MO_DFMF_LIMIT_WHENWOT_CHANGELITTLE  ;// 20   //VF_OSCHANGELITTLE_OSDIAG=TRUE������ȫ��ʱ���������ֵ;//2008-11-17
extern const sbyte MO_DFMF_LLIMIT_WHEN_ACCEL;//2009-8-7 =-5; //����ʱ���Լ������������ƣ�  //

/*������Ȧ�Ĳ���*/
extern const byte M0_TES_ADDINJ_ENRICH;//=50; //����Ȧ�������¶������������¶ȵ��ڸ�ֵʱ���������ͣ�������Ϊ�� M0_TES_ADDINJ_ENRICH-TES����λ��ͬFMF
extern const byte M0_INJPHASE_FORIDLE;      //����ʱ��������λ  // M0_INJPHASE_FORIDLE/16*����

/*------ISCV����-------*/
extern const byte M0_TENG_L_ISCV ;//27     //2008-11-4   ISCV��ʱ��ů�������������¶�
extern const byte M0_D1S_ISCVON_BASE ;//50 //2008-11-4   ISCV����ʱ��Ļ���ֵ
extern const byte MO_TENG_U_ISCV;// 33     //2008-11-4   ����ISCV��ʱ�������¶�
extern const byte M0_TES_U_FOR_CI_ISCVON;//=110;//���¶ȵ��ڸ�ֵʱ��������ʱ��Ҫ�򿪵�����߷�

/*--���ٲ���-----------*/
extern const byte M0_CO_LSPEED_TO16;//=12;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;
extern const byte M0_TPS_IDLE_L;// 7	//���ٵ�������ż��𣬶�Ӧ���ż���Ϊ��"0.NM",���У�N=ȡ����M0_TPS_IDLE_L/4����M=M0_TPS_IDLE_L MOD 4

/*--����ϵͳ��ʼ������-----------*/
extern const byte M0_T_KEYON_FOR_INI_S;     //    3 //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��ÿ��Կ�׿�������ʱ�����ƣ�
extern const byte M0_T_KEYOFF_FOR_INI_S;    //  3  //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��ÿ��Կ�׹رպ���ʱ�����ƣ� 
extern const byte M0_NB_KEYONOFF_FOR_INI;   //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��Կ�׷������صĴ��� 
extern const byte M0_TES_ULIMIT_FOR_INI;//=60;//��ʼ��ʱ����ͷ�¶�������ޣ�
extern const byte M0_REV15D6_LLIMIT_FOR_FULLAQ;//��ɨʱ�������ת�ٸ��ڸ�ֵ�����ж����� 


/*--for cabrication----*/
extern const byte  M0_KESEI_CCLHHHL_PER500KM;//2009-9-28 add 							
							

/*------------------------dimension---------------------------*/							
    #define LDB_X_UB        15
    #define LDB_Y_UB        16
    #define LDB_XMODE_NUM   32
    #define LDB_YMODE_NUM   16
    
    #define IGN_X_UB        15
    #define IGN_Y_UB        16
    #define IGN_XMODE_NUM   32
    #define IGN_YMODE_NUM   16
    
    #define UP_X_UB        15
    #define UP_Y_UB        16
    #define UP_XMODE_NUM   32
    #define UP_YMODE_NUM   16
    
    #define MPA_X_UB        15
    #define MPA_Y_UB        7
    #define MPA_XMODE_NUM   16
    #define MPA_YMODE_NUM   16
    
    #define SL_X_UB        1
    #define SL_Y_UB        1
    #define SL_XMODE_NUM   32
    #define SL_YMODE_NUM   16
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
							
