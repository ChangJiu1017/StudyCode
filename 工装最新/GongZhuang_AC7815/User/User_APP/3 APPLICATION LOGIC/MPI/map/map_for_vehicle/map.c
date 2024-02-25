#include "derivative.h"																			
																			
																			
/*------ DATA INPUT ------*/																			
//2																			
CAL const byte M0_SD0_D12=54;					//	CAL const byte M0_SD0_D12													
CAL const byte M0_IBP=25;					//	CAL const byte M0_IBP													
CAL const byte M0_FBP=160;					//	CAL const byte M0_FBP													
CAL const byte M0_QL_D2=185;					//	CAL const byte M0_QL_D2													
CAL const byte M0_TPS0=23;					//	CAL const byte M0_TPS0													
CAL const byte M0_TPD_LT6=64;					//	CAL const byte M0_TPD_LT6													
CAL const byte M0_TPD_LT11=96;					//	CAL const byte M0_TPD_LT11													
CAL const byte M0_TPD_LT14=112;					//	CAL const byte M0_TPD_LT14													
CAL const byte M0_TPD_LT16=176;					//	CAL const byte M0_TPD_LT16													
CAL const byte M0_OS_LIGHTOFF=56;					//	CAL const byte M0_OS_LIGHTOFF													
CAL const byte M0_CAS_FRONTEDGE=150;					//	CAL const byte M0_CAS_FRONTEDGE													
CAL const byte M0_CAS_TRAILEDGE=64;					//	CAL const byte M0_CAS_TRAILEDGE													
CAL const byte M0_PAIRS_SEALEVEL=0;					//	CAL const byte M0_PAIRS_SEALEVEL													
CAL const byte M0_PAIRS_PER1000M=16;					//	CAL const byte M0_PAIRS_PER1000M													
CAL const byte M0_WIDTH_CDION=250;					//	CAL const byte M0_WIDTH_CDION													
CAL const byte M0_DN_25=15;					//	CAL const byte M0_DN_25													
																			
//3																			
/*------ SPEED CONTROL------*/																			
CAL const byte M0_DN_153=20;					//	CAL const byte M0_DN_153													
CAL const byte M0_N0_LNOISCV_15D6RPM=90;					//	CAL const byte M0_N0_LNOISCV_15D6RPM													
CAL const byte M0_DN0_HTOLNOISCV_15D6RPM=6;					//	CAL const byte M0_DN0_HTOLNOISCV_15D6RPM				p1									
CAL const byte M0_N0_LISCV_15D6RPM=80;					//	CAL const byte M0_N0_LISCV_15D6RPM													
CAL const byte M0_N0_HISCV_15D6RPM=102;					//	CAL const byte M0_N0_HISCV_15D6RPM													
CAL const byte M0_NSTAND_62D5RPM=40;					//	CAL const byte M0_NSTAND_62D5RPM													
CAL const byte M0_NFULL_62D5RPM=61;					//	CAL const byte M0_NFULL_62D5RPM													
CAL const byte M0_KP_M8=255;					//	CAL const byte M0_KP_M8													
CAL const byte M0_KI_M8=255;					//	CAL const byte M0_KI_M8													
CAL const byte M0_KD_M8=255;					//	CAL const byte M0_KD_M8													
CAL const sbyte M0_K_15D6RPM=252;					//	CAL const sbyte M0_K_15D6RPM													
CAL const byte M0_J_M4=30;					//	CAL const byte M0_J_M4													
CAL const byte M0_VBATLIMT=80;					//	CAL const byte M0_VBATLIMT			//										
CAL const byte M0_FMF_CO=255;					//	CAL const byte M0_FMF_CO			//	p0d0.078125,phy=hex/128									
CAL const byte M0_MAP_VERSION=1;					// 														
CAL const byte MV_VEHICLE=1;					// 	1:HIDER 2:SCOOTER 3:CUB 4:WILD													
																			
																			
//OBD																			
//4	E	F	E	A	0	8	C	C	0	2	B	1	7	0	1				
CAL const byte M1_VIN[17]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	50,	66,	49,	55,	48,	49,	0,	0,			
};																			
																			
//5	E	F	E	A	0	8	C	C	0	2	B	1	7	0	1				
CAL const byte M1_CID[16]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	50,	66,	49,	55,	48,	49,	0,				
};																			
																			
//6-7	E	C	M	-	H	U	A	N	S	O	N	G							
CAL const byte M1_ECUNAME[20]={																			
69,	67,	77,	45,	72,	85,	65,	78,	83,	79,	78,	71,	0,	0,	0,	0,	0,	0,	0,	0,
};																			
																			
//project code																			
//7																			
CAL const byte M0_PC_HH=67;					//	CAL const byte M0_PC_HH			//	C									
CAL const byte M0_PC_HL=67;					//	CAL const byte M0_PC_HL			//	C									
CAL const byte M0_PC_H=48;					//	CAL const byte M0_PC_H			//	0									
CAL const byte M0_PC_L=50;					//	CAL const byte M0_PC_L			//	2									
CAL const byte M0_PC_5=66;					//	CAL const byte M0_PC_5			//	B	??????,?SCR????								
CAL const byte M0_PC_6=49;					//	CAL const byte M0_PC_6			//	1									
CAL const byte M0_PC_7=55;					//	CAL const byte M0_PC_7			//	7									
CAL const byte MV_EXHAUST=244;					//	CAL const byte MV_EXHAUST													
CAL const byte M0_BAUDRATE=255;					//	CAL const byte M0_BAUDRATE													
CAL const byte M0_NB_INJDRV=40;					//	CAL const byte M0_NB_INJDRV			//	pd1,phy=hex,def:40									
CAL const byte M0_NIACVLM=64;					//	CAL const byte M0_NIACVLM			//										
																			
//??																			
//8																			
CAL const byte M0_KP_IACV=30;					//	CAL const byte M0_KP_IACV			//										
CAL const byte M0_KI_IACV=30;					//	CAL const byte M0_KI_IACV			//										
																			
//Setting																			
//8																			
CAL const byte M0_OBD_VERSION=4;					//	CAL const byte M0_OBD_VERSION													
CAL const byte M0_EN_DTC_TA=1;					//	CAL const byte M0_EN_DTC_TA													
CAL const byte M0_EN_DTC_IGN=1;					//	CAL const byte M0_EN_DTC_IGN													
CAL const byte M0_EN_IGN_FK_PARTLOAD=1;					//	CAL const byte M0_EN_IGN_FK_PARTLOAD													
CAL const byte M0_UL_REV_62D5RPM_IGNFK=62;					//	CAL const byte M0_UL_REV_62D5RPM_IGNFK													
CAL const byte M0_EN_UP_FK_PARTLOAD=255;					//	CAL const byte M0_EN_UP_FK_PARTLOAD													
CAL const byte M0_UL_REV_62D5RPM_UPFK=255;					//	CAL const byte M0_UL_REV_62D5RPM_UPFK													
CAL const byte M0_MODE_REV_LIMIT=255;					//	CAL const byte M0_MODE_REV_LIMIT													
CAL const byte M0_MIL_DELAY_S=255;					//	CAL const byte M0_MIL_DELAY_S													
CAL const byte M0_SOURCE_TA=255;					//	CAL const byte M0_SOURCE_TA													
CAL const byte M0_EN_PW=255;					//	CAL const byte M0_EN_PW													
CAL const byte M0_EN_VSPEED=255;					//	CAL const byte M0_EN_VSPEED													
CAL const byte M0_EN_PAIRS=1;					//	CAL const byte M0_EN_PAIRS													
CAL const byte M0_MODE_IDLECONTROL=2;					//	CAL const byte M0_MODE_IDLECONTROL													
//9																			
CAL const byte M0_MODE_CA=255;					//	CAL const byte M0_MODE_CA													
CAL const byte M0_MODE_ACTIVE=255;					//	CAL const byte M0_MODE_ACTIVE													
CAL const byte M0_STRCLS_TPD=255;					//	CAL const byte M0_STRCLS_TPD													
CAL const byte M0_ENGTYPE=1;					//	CAL const byte M0_ENGTYPE			1:CG  2:CB  3:GY										
CAL const byte M0_U_CO_1ST=39;					//	CAL const byte M0_U_CO_1ST													
CAL const byte M0_URNIDLE_CO=255;					//	CAL const byte M0_URNIDLE_CO													
CAL const byte M0_TOOTH_TDC=92;					//	CAL const byte M0_TOOTH_TDC													
CAL const byte M0_INJFLOW=97;					//	CAL const byte M0_INJFLOW													
CAL const byte M0_PC_8=1;					//	CAL const byte M0_PC_8													
CAL const byte M0_LDBMUL=90;					//	CAL const byte M0_LDBMUL													
CAL const byte M0_RESERVE9=255;					//	CAL const byte M0_RESERVE9													
CAL const byte M0_RESERVE10=255;					//	CAL const byte M0_RESERVE10													
CAL const byte M0_RESERVE11=255;					//	CAL const byte M0_RESERVE11													
CAL const byte M0_RESERVE12=255;					//	CAL const byte M0_RESERVE12													
CAL const byte M0_RESERVE13=255;					//	CAL const byte M0_RESERVE13													
CAL const byte M0_RESERVE14=255;					//	CAL const byte M0_RESERVE14													
//10																			
CAL const byte M0_RESERVE15=255;					//	CAL const byte M0_RESERVE15													
CAL const byte M0_RESERVE16=255;					//	CAL const byte M0_RESERVE16													
CAL const byte M0_RESERVE17=255;					//	CAL const byte M0_RESERVE17													
CAL const byte M0_RESERVE18=255;					//	CAL const byte M0_RESERVE18													
CAL const byte M0_RESERVE19=255;					//	CAL const byte M0_RESERVE19													
CAL const byte M0_RESERVE20=255;					//	CAL const byte M0_RESERVE20													
CAL const byte M0_RESERVE21=255;					//	CAL const byte M0_RESERVE21													
CAL const byte M0_RESERVE22=255;					//	CAL const byte M0_RESERVE22													
CAL const byte M0_RESERVE23=255;					//	CAL const byte M0_RESERVE23													
CAL const byte M0_RESERVE24=255;					//	CAL const byte M0_RESERVE24													
CAL const byte M0_RESERVE25=255;					//	CAL const byte M0_RESERVE25													
CAL const byte M0_RESERVE26=255;					//	CAL const byte M0_RESERVE26													
CAL const byte M0_RESERVE27=255;					//	CAL const byte M0_RESERVE27													
CAL const byte M0_RESERVE28=255;					//	CAL const byte M0_RESERVE28													
CAL const byte M0_CCPS_ti_DurDFCORunTmLim_C=255;					//	CAL const byte M0_CCPS_ti_DurDFCORunTmLim_C													
CAL const byte M0_CCPS_ti_DurPERunTmLim_C=255;					//	CAL const byte M0_CCPS_ti_DurPERunTmLim_C													
//11																			
CAL const byte M0_CCPS_ti_CldStrTmtMin_C=255;					//	CAL const byte M0_CCPS_ti_CldStrTmtMin_C													
CAL const byte M0_CCPS_ti_WrmStrtTmMin_C=255;					//	CAL const byte M0_CCPS_ti_WrmStrtTmMin_C													
CAL const byte M0_CCPS_t_EngCydTmpHi_C=255;					//	CAL const byte M0_CCPS_t_EngCydTmpHi_C													
CAL const byte M0_CCPS_t_EngCydTmpLo_C=255;					//	CAL const byte M0_CCPS_t_EngCydTmpLo_C													
CAL const byte M0_CCPS_Pct_TPSMax_C=255;					//	CAL const byte M0_CCPS_Pct_TPSMax_C													
CAL const byte M0_CCPS_Pct_TPSMin_C=255;					//	CAL const byte M0_CCPS_Pct_TPSMin_C													
CAL const byte M0_CCPS_n_EngSpdHi_C=255;					//	CAL const byte M0_CCPS_n_EngSpdHi_C													
CAL const byte M0_CCPS_n_EngSpdLo_C=255;					//	CAL const byte M0_CCPS_n_EngSpdLo_C													
CAL const byte M0_CCPS_u_BattVltgMax_C=255;					//	CAL const byte M0_CCPS_u_BattVltgMax_C													
CAL const byte M0_CCPS_ti_O2RichTmMax_C=255;					//	CAL const byte M0_CCPS_ti_O2RichTmMax_C													
CAL const byte M0_CCPS_swt_CCPSStrt_C=0;					//	CAL const byte M0_CCPS_swt_CCPSStrt_C													
CAL const byte M0_OverTMPProtection_Te=95;					//	CAL const byte M0_OverTMPProtection_Te													
CAL const byte M0_T_OVERTMP=150;					//	CAL const byte M0_T_OVERTMP													
CAL const byte M0_OverTMPProtection_SWITCH=1;					//	CAL const byte M0_OverTMPProtection_SWITCH													
CAL const byte M0_CC_NC_SPEED=52;					//	CAL const byte M0_CC_NC_SPEED													
CAL const byte M0_CC_NR_SPEED=50;					//	CAL const byte M0_CC_NR_SPEED													
																			
																			
//12																			
//Parameters																			
CAL const byte M0_FMF_IDLE_RICHTOLEAN=255;					//	CAL const byte M0_FMF_IDLE_RICHTOLEAN													
CAL const byte M0_FMF_IDLE_LEANTORICH=255;					//	CAL const byte M0_FMF_IDLE_LEANTORICH													
CAL const byte M0_T_SPRAY_FLY_TO_INLET_D1MS=255;					//	CAL const byte M0_T_SPRAY_FLY_TO_INLET_D1MS													
CAL const byte M0_UL_CYCLE=255;					//	CAL const byte M0_UL_CYCLE													
CAL const byte M0_STEPBASE=45;					//	CAL const byte M0_STEPBASE			//										
																			
//none1																			
CAL const byte Tony_mapnone1[11];																			
																			
//13																			
CAL const byte M1_STEPINC_PAIR[8]={																			
	0	,	11	,	26	,	40	,	55	,	66	,	75	,	85	,			
};																			
																			
//X-te Y-stepco formular:p0d02																			
CAL const sbyte M1_STEPCO_PT[4][2]={																			
	236,	75,																	
	0,	60,																	
	20,	50,																	
	90,	40,																	
};																			
																			
//IACV																			
//formular: p1; x-axis: v_rev_625rpm,y-axis:v_te+40																			
//14																			
CAL const byte M1_STEPINI[4][2]={																			
	20,	150,																	
	40,	110,																	
	70,	80,																	
	120,	65,																	
};																			
																			
//X-te Y-stepincrease																			
CAL const sbyte M1_STEINC_LIDLE[2][2]={																			
	25	,	3	,															
	100	,	2	,															
};																			
//X-te Y-stepincrease																			
CAL const sbyte M1_STEINC_BIDLE[2][2]={																			
	25	,	2	,															
	100	,	2	,															
};																			
																			
																			
//formular: p15d6;																			
//15																			
CAL const sbyte M1_STEPDEC[3][2]={																			
	236,	32,																	
	0,	28,																	
	80,	20,																	
};																			
																			
//15formular: p1;																			
CAL const byte N_M2LAMBDA[2]={																			
92,	128,																		
};																			
																			
//formular: p1;																			
CAL const byte STEP_M2LAMBDA[8]={																			
20,	40,	60,	80,	100,	130,	160,	200,												
};																			
																			
																			
//formular: p1; x-axis: v_step_sm,y-axis:v_rev_625rpm																			
//16																			
CAL const sbyte M2_STEPLAMBDA[2][8]={																			
	221,	244,	9,	25,	40,	55,	70,	85,											
	221,	244,	9,	25,	40,	55,	70,	85,											
};																			
																			
//17-18																			
//20210112 chenwei add ????????up??																			
CAL const byte M2_UP_OVERTEP_PROTECTION[5][6]={																			
//	6500	7000	7500	8000	8500	9000													
	45,	45,	45,	40,	30,	10,	//												
	35,	35,	35,	35,	30,	10,	//												
	20,	20,	20,	20,	20,	10,	//												
	10,	10,	10,	10,	10,	10,	//												
	8,	8,	8,	8,	8,	8,	//												
};																			
																			
//none11																			
CAL const byte Tony_mapnone2[2];																			
																			
//formular: p1; x-axis: CCPS_Pct_DutyCycle,y-axis:v_rev_625rpm																			
CAL const byte M1_CCPS_Pct_DutyCycle[8][2]={																			
	255,	255,																	
	255,	255,																	
	255,	255,																	
	255,	255,																	
	255,	255,																	
	255,	255,																	
	255,	255,																	
	255,	255,																	
};																			
																			
//formular: p1; x-axis: te,y-axis:v_t_O2H_delay_1s																			
CAL const byte M1_T_O2H_DELAY[8][2]={																			
	20,	240,																	
	30,	240,																	
	40,	180,																	
	60,	5,																	
	70,	5,																	
	90,	5,																	
	120,	5,																	
	140,	5,																	
};																			
																			
//formular: p1; x-axis: te,y-axis:v_rev_625rpm																			
CAL const byte  M1_SPEED_O2H_THRESH[8][2]={																			
	20,	64,																	
	30,	64,																	
	40,	64,																	
	60,	64,																	
	70,	64,																	
	90,	64,																	
	120,	64,																	
	140,	64,																	
};																			
																			
//formular: p1; x-axis: te,y-axis:v_TPd																			
CAL const byte M1_TPD_O2H_THRESH[8][2]={																			
	20,	144,																	
	30,	144,																	
	40,	144,																	
	60,	144,																	
	70,	144,																	
	90,	144,																	
	120,	144,																	
	140,	144,																	
};																			
																			
																			
																			
																			
																			
//none2																			
CAL const byte Tony_mapnone3[176+512];				//224?176															
																			
																			
//35-51																			
/*------ BASIC LOAD ------*/																			
CAL const byte M2_LDB_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	56,	55,	52,	47,	41,	37,	33,	30,	27,	27,	27,	26,	26,	26,	25,	24,	//	0	
	67,	65,	61,	55,	50,	45,	40,	35,	31,	28,	28,	28,	28,	28,	27,	27,	//	1	
	78,	75,	70,	63,	57,	52,	47,	42,	37,	33,	31,	31,	31,	31,	30,	29,	//	2	
	89,	82,	76,	72,	66,	62,	56,	51,	47,	42,	40,	37,	36,	35,	35,	34,	//	3	
	101,	91,	85,	80,	75,	70,	63,	59,	53,	50,	47,	46,	44,	42,	42,	41,	//	4	
	111,	102,	92,	86,	81,	75,	69,	65,	61,	56,	53,	51,	50,	48,	48,	46,	//	5	
	123,	110,	100,	93,	86,	80,	74,	70,	66,	63,	60,	57,	56,	54,	52,	52,	//	6	
	138,	122,	110,	100,	92,	85,	79,	75,	72,	69,	65,	63,	61,	60,	58,	56,	//	7	
	153,	135,	118,	106,	98,	90,	85,	80,	77,	74,	71,	69,	67,	66,	64,	63,	//	8	
	170,	149,	132,	117,	106,	98,	91,	87,	84,	82,	79,	77,	75,	73,	71,	70,	//	9	
	170,	173,	154,	136,	121,	109,	101,	97,	95,	91,	90,	87,	83,	81,	78,	77,	//	10	
	170,	173,	176,	160,	145,	130,	119,	115,	111,	107,	102,	99,	96,	92,	89,	86,	//	11	
	170,	173,	176,	180,	168,	160,	151,	145,	141,	138,	132,	123,	115,	109,	106,	102,	//	12	
	170,	173,	176,	180,	186,	177,	171,	164,	158,	152,	148,	143,	135,	126,	119,	112,	//	13	
	170,	173,	176,	180,	186,	191,	188,	183,	178,	172,	165,	156,	146,	137,	128,	123,	//	14	
	170,	173,	176,	180,	186,	191,	198,	197,	196,	192,	184,	174,	163,	155,	145,	138,	//	15	
	170,	173,	176,	180,	186,	191,	198,	202,	203,	198,	191,	181,	171,	160,	151,	145,	//	16	
};																			
																			
//none3																			
CAL const byte Tony_mapnone4[16];																			
																			
/*------COORDINATES------*/																			
//2016-8-11 ?Data_sys_source.c?????																			
//2015-11-20??																			
//N_62D5_M2LDB:??3D?X??,*62.5?????rpm,???n;PAIR_DEG--M2_MPA_M128????,???Pdeg,TPD_M2LDB--??3D????,???TPD																			
//			1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000	
//			0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
//			0	1	2	3	4	5	6	7									
//53																			
CAL const byte N_62D5_M2LDB[16]={																			
24,	32,	40,	48,	56,	64,	72,	80,	88,	96,	104,	112,	120,	128,	136,	144,				
};																			
//54																			
CAL const byte TPD_M2LDB[17]={																			
0,	16,	32,	48,	64,	80,	96,	112,	128,	144,	160,	176,	192,	208,	224,	240,	255,			
};																			
//55																			
CAL const byte N_62D5_M2LDB1[6]={																			
104,	112,	120,	128,	136,	144,														
};																			
CAL const byte TPD_M2LDB1[5]={																			
192,	208,	224,	240,	255,															
};																			
																			
//none4																			
CAL const byte Tony_mapnone5[180+512];																			
																			
//68-75																			
/*------ BASIC LOAD CORRECTION ------*/																			
CAL const byte M2_MPA_M128[8][16]={																			
	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,			
	119,	119,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,			
	113,	111,	108,	108,	108,	108,	107,	106,	106,	106,	106,	106,	106,	106,	105,	105,			
	100,	96,	95,	94,	94,	94,	94,	94,	93,	93,	93,	93,	93,	93,	93,	93,			
	90,	89,	84,	82,	81,	81,	80,	79,	79,	79,	79,	79,	79,	79,	79,	79,			
	77,	74,	70,	67,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,			
	65,	63,	60,	59,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,			
	56,	56,	54,	54,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,			
};																			
																			
//76																			
CAL const byte M0_T_M64=48;																			
CAL const byte M0_IDLEFMF_CO=150;																			
																			
//none5																			
CAL const byte Tony_mapnone6[30];																			
																			
//78																			
CAL const byte PAIR_DEG[8]={																			
0,	16,	32,	48,	64,	80,	96,	112,												
};																			
																			
CAL const sbyte M1_PREINJ[4][2]={																			
	236	,	80	,															
	0	,	50	,															
	35	,	20	,															
	80	,	10	,															
};																			
																			
CAL const byte M2_V_T_IGNON[16][16]={																			
//																			
	250,	246,	212,	87,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	250,	167,	145,	87,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	200,	129,	112,	87,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	153,	106,	93,	87,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	125,	90,	79,	82,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	106,	81,	71,	71,	57,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	93,	71,	64,	62,	51,	46,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	84,	67,	59,	56,	48,	43,	39,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	76,	60,	54,	53,	43,	40,	37,	34,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	70,	56,	50,	48,	42,	37,	35,	32,	31,	28,	25,	23,	21,	20,	18,	17,	//	0	
	65,	53,	48,	45,	39,	35,	32,	31,	29,	28,	25,	23,	21,	20,	18,	17,	//	0	
	60,	50,	45,	43,	37,	34,	31,	29,	28,	26,	21,	20,	18,	18,	17,	17,	//	0	
	57,	48,	43,	40,	35,	32,	29,	28,	26,	25,	20,	18,	17,	18,	15,	15,	//	0	
	54,	45,	40,	39,	34,	31,	29,	26,	25,	23,	20,	17,	14,	14,	14,	12,	//	0	
	51,	43,	39,	37,	32,	29,	28,	26,	25,	23,	18,	15,	14,	14,	12,	12,	//	0	
	51,	43,	39,	37,	32,	29,	28,	26,	25,	23,	18,	15,	14,	14,	12,	12,	//	0	
};																			
																			
//20210926 chenwei add ?????																			
CAL const byte N_62D5_M2IGNON[16]={																			
13,	26,	38,	51,	64,	77,	90,	102,	115,	128,	141,	154,	166,	179,	192,	205,				
};																			
																			
CAL const byte VBAT_M2IGNON[16]={																			
94,	107,	120,	133,	145,	158,	171,	184,	197,	210,	222,	235,	248,	254,	254,	254,				
};																			
																			
																			
																			
//none6																			
CAL const byte Tony_mapnone7[48+512];																			
																			
/*------ LAMBDA CONTROL & FUEL PLAN ------*/																			
//101																			
CAL const sbyte M1_US_M32[8][2]={																			
	0	,	22	,															
	4	,	20	,															
	8	,	16	,															
	16	,	14	,															
	24	,	12	,															
	32	,	10	,															
	48	,	8	,															
	80	,	6	,															
};																			
//102																			
CAL const sbyte M1_F_M256[8][2]={																			
	226	,	45	,															
	236	,	40	,															
	246	,	35	,															
	0	,	32	,															
	10	,	30	,															
	20	,	28	,															
	30	,	27	,															
	50	,	26	,															
};																			
//103																			
CAL const sbyte M1_G0_M128[8][2]={																			
	226	,	40	,															
	236	,	46	,															
	246	,	48	,															
	0	,	48	,															
	10	,	48	,															
	20	,	48	,															
	30	,	48	,															
	50	,	50	,															
};																			
//104																			
CAL const sbyte M1_E_M4[8][2]={																			
	226	,	48	,															
	236	,	43	,															
	246	,	39	,															
	0	,	36	,															
	10	,	34	,															
	20	,	32	,															
	30	,	30	,															
	50	,	29	,															
};																			
//105																			
CAL const byte M1_NR0_62D5RPM[16]={																			
44,	89,	117,	119,	119,	119,	119,	119,	119,	119,	119,	119,	119,	119,	119,	119,				
};																			
//106																			
CAL const sbyte M1_UR_M16[8][2]={																			
	236	,	48	,															
	246	,	33	,															
	0	,	20	,															
	20	,	15	,															
	40	,	13	,															
	60	,	12	,															
	80	,	10	,															
	120	,	4	,															
};																			
//107																			
CAL const byte M1_NC_62D5[16]={																			
48,	110,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,				
};																			
//108																			
CAL const sbyte M1_UA_M32[8][2]={																			
	0	,	14	,															
	4	,	12	,															
	8	,	10	,															
	16	,	8	,															
	32	,	6	,															
	64	,	4	,															
	96	,	2	,															
	120	,	1	,															
};																			
//109-125																			
CAL const sbyte M2_UP[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	65,	65,	80,	80,	80,	80,	80,	80,	80,	80,	80,	80,	80,	80,	80,	85,	//	0	
	65,	65,	65,	65,	65,	65,	65,	65,	65,	65,	69,	69,	80,	80,	80,	85,	//	1	
	80,	60,	55,	55,	55,	55,	55,	55,	40,	40,	40,	60,	60,	60,	80,	85,	//	2	
	80,	60,	55,	55,	55,	55,	55,	55,	0,	30,	30,	50,	50,	50,	69,	85,	//	3	
	80,	60,	55,	55,	55,	55,	55,	55,	0,	0,	20,	40,	40,	40,	60,	85,	//	4	
	80,	60,	55,	55,	55,	55,	0,	0,	0,	0,	0,	30,	30,	30,	50,	85,	//	5	
	80,	60,	55,	55,	55,	55,	0,	0,	0,	0,	0,	20,	20,	20,	40,	85,	//	6	
	81,	60,	56,	0,	0,	0,	0,	0,	0,	0,	0,	10,	10,	10,	40,	85,	//	7	
	81,	80,	57,	10,	0,	0,	0,	0,	0,	0,	0,	10,	10,	10,	30,	85,	//	8	
	81,	80,	58,	10,	10,	0,	0,	0,	0,	0,	4,	10,	10,	10,	20,	85,	//	9	
	81,	80,	59,	20,	10,	10,	0,	0,	82,	81,	81,	81,	81,	81,	81,	81,	//	10	
	81,	80,	80,	40,	30,	10,	10,	10,	83,	82,	82,	82,	82,	82,	82,	82,	//	11	
	81,	80,	80,	50,	40,	30,	30,	30,	84,	83,	83,	83,	83,	83,	83,	83,	//	12	
	81,	80,	80,	60,	50,	40,	40,	40,	85,	84,	84,	84,	84,	84,	84,	84,	//	13	
	83,	83,	83,	83,	83,	83,	83,	83,	89,	89,	89,	89,	89,	89,	89,	89,	//	14	
	89,	89,	89,	89,	89,	88,	88,	90,	91,	91,	91,	91,	91,	92,	93,	93,	//	15	
	93,	93,	93,	93,	93,	92,	92,	92,	93,	93,	93,	92,	92,	93,	94,	94,	//	16	
};																			
																			
																			
//126																			
CAL const byte M0_F0_M256=45;					//	CAL const byte M0_F0_M256				51									
CAL const byte M0_g_M128=35;					//	CAL const byte M0_g_M128				19									
CAL const byte M0_X=40;					//	CAL const byte M0_X				0									
CAL const byte M0_Y_M256=60;					//	CAL const byte M0_Y_M256				25									
CAL const byte M0_Z=7;					//	CAL const byte M0_Z				0									
CAL const byte M0_R_D4=20;					//	CAL const byte M0_R_D4				100									
CAL const byte M0_f_M128=5;					//	CAL const byte M0_f_M128				0									
CAL const byte M0_NRT_M64=16;					//	CAL const byte M0_NRT_M64				16									
CAL const byte M0_C_M8=70;					//	CAL const byte M0_C_M8				60									
CAL const byte M0_D_M256=240;					//	CAL const byte M0_D_M256				210									
CAL const byte M0_B_D8=60;					//	CAL const byte M0_B_D8				60									
CAL const byte M0_P_DEC_M64=20;					//	CAL const byte M0_P_DEC_M64				24									
CAL const byte M0_S_ACC_M128=40;					//	CAL const byte M0_S_ACC_M128				48									
CAL const byte M0_L_ACC_M1024=44;					//	CAL const byte M0_L_ACC_M1024				40									
CAL const byte M0_P_ACC_M64=34;					//	CAL const byte M0_P_ACC_M64				24									
CAL const byte M0_S_DEC_M128=36;					//	CAL const byte M0_S_DEC_M128				81									
//127																			
CAL const byte M0_L_DEC_M1024=230;					//	CAL const byte M0_L_DEC_M1024				40									
																			
//none7																			
CAL const byte Tony_mapnone8[95+512];																			
																			
//134-150																			
/*------IGNITION------*/																			
CAL const byte M2_IGN_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	21,	30,	34,	38,	40,	40,	41,	42,	43,	44,	44,	45,	46,	46,	46,	46,	//	0	
	21,	30,	33,	37,	40,	40,	41,	42,	43,	44,	44,	45,	46,	46,	46,	46,	//	1	
	21,	30,	33,	37,	40,	40,	41,	42,	43,	44,	44,	45,	46,	46,	46,	46,	//	2	
	21,	30,	33,	37,	40,	40,	41,	43,	43,	44,	44,	45,	46,	46,	46,	46,	//	3	
	21,	30,	33,	37,	40,	40,	41,	43,	43,	44,	44,	45,	46,	46,	46,	46,	//	4	
	21,	30,	33,	37,	40,	40,	41,	43,	43,	44,	44,	43,	46,	46,	46,	46,	//	5	
	20,	30,	33,	37,	40,	40,	41,	43,	43,	44,	44,	41,	44,	44,	44,	44,	//	6	
	20,	30,	31,	36,	40,	40,	43,	44,	44,	44,	44,	41,	44,	44,	44,	44,	//	7	
	20,	30,	31,	36,	40,	40,	43,	44,	44,	44,	44,	41,	44,	44,	44,	44,	//	8	
	20,	30,	31,	36,	40,	40,	43,	46,	46,	44,	44,	41,	44,	44,	44,	44,	//	9	
	20,	29,	31,	36,	40,	40,	43,	46,	46,	44,	44,	41,	43,	44,	44,	44,	//	10	
	20,	29,	30,	34,	39,	40,	44,	46,	46,	44,	44,	43,	43,	44,	44,	44,	//	11	
	20,	29,	30,	34,	39,	40,	44,	46,	46,	44,	44,	43,	43,	43,	43,	43,	//	12	
	20,	27,	30,	34,	39,	40,	44,	47,	47,	46,	46,	43,	43,	43,	43,	43,	//	13	
	20,	27,	29,	33,	37,	40,	46,	48,	48,	46,	46,	44,	43,	43,	43,	43,	//	14	
	20,	26,	29,	33,	37,	40,	46,	49,	49,	46,	46,	44,	43,	43,	43,	43,	//	15	
	20,	24,	29,	33,	37,	40,	46,	49,	49,	46,	46,	44,	43,	43,	43,	43,	//	16	
};																			
//151																			
CAL const sbyte M2_IGC[8][2]={																			
	216	,	236	,															
	226	,	238	,															
	236	,	240	,															
	246	,	242	,															
	0	,	244	,															
	10	,	246	,															
	20	,	248	,															
	40	,	250	,															
};																			
//152-153																			
CAL const sbyte M2_IGT[10][2]={																			
	236	,	14	,															
	252	,	12	,															
	12	,	10	,															
	28	,	7	,															
	44	,	5	,															
	60	,	2	,															
	76	,	254	,															
	92	,	250	,															
	108	,	246	,															
	124	,	242	,															
};																			
																			
//none8																			
CAL const byte Tony_mapnone9[12];																			
																			
//154																			
CAL const sbyte M0_IGNMAX_IDLE=13;					//	CAL const sbyte M0_IGNMAX_IDLE				21									
CAL const sbyte M0_IGNMIN_IDLE=3;					//	CAL const sbyte M0_IGNMIN_IDLE				-43									
CAL const byte M0_IGS=14;					//	CAL const byte M0_IGS				14									
CAL const byte M0_O=0;					//	CAL const byte M0_O				0									
CAL const byte M0_M=20;					//	CAL const byte M0_M				34									
																			
//none9																			
CAL const byte Tony_mapnone10[187];																			
