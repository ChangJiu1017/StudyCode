#include "derivative.h"																			
																			
																			
/*------ DATA INPUT ------*/																			
const byte M0_SD0_D12=54;					//	const byte M0_SD0_D12													
const byte M0_IBP=25;					//	const byte M0_IBP													
const byte M0_FBP=160;					//	const byte M0_FBP													
const byte M0_QL_D2=185;					//	const byte M0_QL_D2													
const byte M0_TPS0=23;					//	const byte M0_TPS0													
const byte M0_TPD_LT6=55;					//	const byte M0_TPD_LT6													
const byte M0_TPD_LT11=96;					//	const byte M0_TPD_LT11													
const byte M0_TPD_LT14=112;					//	const byte M0_TPD_LT14													
const byte M0_TPD_LT16=176;					//	const byte M0_TPD_LT16													
const byte M0_OS_LIGHTOFF=56;					//	const byte M0_OS_LIGHTOFF													
const byte M0_CAS_FRONTEDGE=150;					//	const byte M0_CAS_FRONTEDGE													
const byte M0_CAS_TRAILEDGE=64;					//	const byte M0_CAS_TRAILEDGE													
const byte M0_PAIRS_SEALEVEL=170;					//	const byte M0_PAIRS_SEALEVEL													
const byte M0_PAIRS_PER1000M=16;					//	const byte M0_PAIRS_PER1000M													
const byte M0_WIDTH_CDION=250;					//	const byte M0_WIDTH_CDION													
const byte MV_EXHAUST=125;					//	const byte MV_EXHAUST													
const byte M0_BAUDRATE=255;					//	const byte M0_BAUDRATE													
const byte M0_MAP_VERSION=2;					// 														
const byte MV_VEHICLE=3;					// 	1:HIDER 2:SCOOTER 3:CUB 4:WILD													
const byte M0_VBATLIMT=80;					//	const byte M0_VBATLIMT			//										
const byte M0_FMF_CO=110;					//	const byte M0_FMF_CO			//	p0d0.078125,phy=hex/128									
const byte M0_NB_INJDRV=40;					//	const byte M0_NB_INJDRV			//	pd1，phy=hex，def：40									
																			
//project code																			
const byte M0_PC_HH=67;					//	const byte M0_PC_HH			//	C									
const byte M0_PC_HL=67;					//	const byte M0_PC_HL			//	C									
const byte M0_PC_H=48;					//	const byte M0_PC_H			//	0									
const byte M0_PC_L=54;					//	const byte M0_PC_L			//	6									
const byte M0_PC_5=52;					//	const byte M0_PC_5			//	4	新增三个字节，与SCR保持一致								
const byte M0_PC_6=49;					//	const byte M0_PC_6			//	1									
const byte M0_PC_7=54;					//	const byte M0_PC_7			//	6									
																			
//预留																			
const byte M0_NIACVLM=64;					//	const byte M0_NIACVLM			//										
const byte M0_KP_IACV=30;					//	const byte M0_KP_IACV			//										
const byte M0_KI_IACV=10;					//	const byte M0_KI_IACV			//										
const byte M0_STEPBASE=60;					//	const byte M0_STEPBASE			//										
																			
																			
//IACV																			
//formular: p1; x-axis: v_rev_625rpm,y-axis:v_te+40																			
const byte M1_STEPINI[4][2]={																			
	20,	140,																	
	40,	110,																	
	90,	90,																	
	140,	70,																	
};																			
																			
//formular: p15d6;																			
const sbyte M1_STEPDEC[3][2]={																			
	0,	5,																	
	40,	4,																	
	100,	3,																	
};																			
																			
//formular: p1; x-axis: v_step_sm,y-axis:v_rev_625rpm																			
const sbyte M2_STEPLAMBDA[2][8]={																			
	226,	236,	0,	14,	20,	33,	52,	93,											
	220,	231,	0,	15,	20,	35,	56,	93,											
};																			
																			
//formular: p1;																			
const byte STEP_M2LAMBDA[8]={																			
20,	40,	60,	80,	100,	120,	140,	200,												
};																			
																			
//formular: p1;																			
const byte N_M2LAMBDA[2]={																			
96,	128,																		
};																			
																			
//X-te Y-stepincrease																			
const sbyte M1_STEINC_LIDLE[2][2]={																			
	10	,	4	,															
	96	,	3	,															
};																			
//X-te Y-stepincrease																			
const sbyte M1_STEINC_BIDLE[2][2]={																			
	10	,	4	,															
	96	,	3	,															
};																			
																			
const byte M1_STEPINC_PAIR[8]={																			
	0	,	8	,	13	,	28	,	45	,	55	,	65	,	75	,			
};																			
																			
//X-te Y-stepco formular:p0d02																			
const sbyte M1_STEPCO_PT[4][2]={																			
	236,	72,																	
	0,	55,																	
	50,	25,																	
	100,	12,																	
};																			
																			
//OBD																			
//	E	F	E	A	0	8	C	C	0	6	4	1	6	0	1				
const byte M1_VIN[17]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	54,	52,	49,	54,	48,	49,	0,	0,			
};																			
																			
//	E	F	E	A	0	8	C	C	0	6	4	1	6	0	1		 		
const byte M1_CID[16]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	54,	52,	49,	54,	48,	49,	0,				
};																			
																			
//	E	C	M	-	S	H	U	A	N	G	S	H	I						
const byte M1_ECUNAME[20]={																			
69,	67,	77,	45,	83,	72,	85,	65,	78,	71,	83,	72,	73,	0,	0,	0,	0,	0,	0,	0,
};																			
																			
//Setting																			
const byte M0_OBD_VERSION=2;					//	const byte M0_OBD_VERSION													
const byte M0_EN_DTC_TA=1;					//	const byte M0_EN_DTC_TA													
const byte M0_EN_DTC_IGN=1;					//	const byte M0_EN_DTC_IGN													
const byte M0_EN_IGN_FK_PARTLOAD=1;					//	const byte M0_EN_IGN_FK_PARTLOAD													
const byte M0_UL_REV_62D5RPM_IGNFK=62;					//	const byte M0_UL_REV_62D5RPM_IGNFK													
const byte M0_EN_UP_FK_PARTLOAD=255;					//	const byte M0_EN_UP_FK_PARTLOAD													
const byte M0_UL_REV_62D5RPM_UPFK=255;					//	const byte M0_UL_REV_62D5RPM_UPFK													
const byte M0_MODE_REV_LIMIT=255;					//	const byte M0_MODE_REV_LIMIT													
const byte M0_MIL_DELAY_S=255;					//	const byte M0_MIL_DELAY_S													
const byte M0_SOURCE_TA=255;					//	const byte M0_SOURCE_TA													
const byte M0_EN_PW=255;					//	const byte M0_EN_PW													
const byte M0_EN_VSPEED=255;					//	const byte M0_EN_VSPEED													
const byte M0_EN_PAIRS=1;					//	const byte M0_EN_PAIRS													
const byte M0_MODE_IDLECONTROL=255;					//	const byte M0_MODE_IDLECONTROL													
const byte M0_MODE_CA=255;					//	const byte M0_MODE_CA													
const byte M0_MODE_ACTIVE=255;					//	const byte M0_MODE_ACTIVE													
const byte M0_STRCLS_TPD=255;					//	const byte M0_STRCLS_TPD													
const byte M0_ENGTYPE=4;					//	const byte M0_ENGTYPE			1：CG  2：CB  3：GY										
const byte M0_U_CO_1ST=48;					//	const byte M0_U_CO_1ST													
const byte M0_URNIDLE_CO=255;					//	const byte M0_URNIDLE_CO													
const byte M0_TOOTH_TDC=93;					//	const byte M0_TOOTH_TDC													
const byte M0_INJFLOW=31;					//	const byte M0_INJFLOW													
const byte M0_PC_8=1;					//	const byte M0_PC_8													
const byte M0_LDBMUL=255;					//	const byte M0_LDBMUL													
const byte M0_RESERVE9=255;					//	const byte M0_RESERVE9													
const byte M0_RESERVE10=255;					//	const byte M0_RESERVE10													
const byte M0_RESERVE11=255;					//	const byte M0_RESERVE11													
const byte M0_RESERVE12=255;					//	const byte M0_RESERVE12													
const byte M0_RESERVE13=255;					//	const byte M0_RESERVE13													
const byte M0_RESERVE14=255;					//	const byte M0_RESERVE14													
const byte M0_RESERVE15=255;					//	const byte M0_RESERVE15													
const byte M0_RESERVE16=255;					//	const byte M0_RESERVE16													
const byte M0_RESERVE17=255;					//	const byte M0_RESERVE17													
const byte M0_RESERVE18=255;					//	const byte M0_RESERVE18													
const byte M0_RESERVE19=255;					//	const byte M0_RESERVE19													
const byte M0_RESERVE20=255;					//	const byte M0_RESERVE20													
const byte M0_RESERVE21=255;					//	const byte M0_RESERVE21													
const byte M0_RESERVE22=255;					//	const byte M0_RESERVE22													
const byte M0_RESERVE23=255;					//	const byte M0_RESERVE23													
const byte M0_RESERVE24=255;					//	const byte M0_RESERVE24													
const byte M0_RESERVE25=255;					//	const byte M0_RESERVE25													
const byte M0_RESERVE26=255;					//	const byte M0_RESERVE26													
const byte M0_RESERVE27=255;					//	const byte M0_RESERVE27													
const byte M0_RESERVE28=255;					//	const byte M0_RESERVE28													
const byte M0_RESERVE29=255;					//	const byte M0_RESERVE29													
const byte M0_RESERVE30=255;					//	const byte M0_RESERVE30													
const byte M0_RESERVE31=255;					//	const byte M0_RESERVE31													
const byte M0_RESERVE32=255;					//	const byte M0_RESERVE32													
const byte M0_RESERVE33=255;					//	const byte M0_RESERVE33													
const byte M0_RESERVE34=255;					//	const byte M0_RESERVE34													
const byte M0_RESERVE35=255;					//	const byte M0_RESERVE35													
const byte M0_RESERVE36=255;					//	const byte M0_RESERVE36													
const byte M0_RESERVE37=255;					//	const byte M0_RESERVE37													
const byte M0_RESERVE38=255;					//	const byte M0_RESERVE38													
const byte M0_RESERVE39=255;					//	const byte M0_RESERVE39													
const byte M0_RESERVE40=255;					//	const byte M0_RESERVE40													
const byte M0_RESERVE41=255;					//	const byte M0_RESERVE41													
const byte M0_RESERVE42=255;					//	const byte M0_RESERVE42													
const byte M0_RESERVE43=255;					//	const byte M0_RESERVE43													
const byte M0_RESERVE44=255;					//	const byte M0_RESERVE44													
const byte M0_RESERVE45=255;					//	const byte M0_RESERVE45													
const byte M0_RESERVE46=255;					//	const byte M0_RESERVE46													
																			
//Parameters																			
const byte M0_FMF_IDLE_RICHTOLEAN=255;					//	const byte M0_FMF_IDLE_RICHTOLEAN													
const byte M0_FMF_IDLE_LEANTORICH=255;					//	const byte M0_FMF_IDLE_LEANTORICH													
const byte M0_T_SPRAY_FLY_TO_INLET_D1MS=255;					//	const byte M0_T_SPRAY_FLY_TO_INLET_D1MS													
const byte M0_UL_CYCLE=255;					//	const byte M0_UL_CYCLE													
																			
																			
/*------ SPEED CONTROL------*/																			
const byte M0_DN_25=11;					//	const byte M0_DN_25													
const byte M0_DN_153=12;					//	const byte M0_DN_153													
const byte M0_N0_LNOISCV_15D6RPM=99;					//	const byte M0_N0_LNOISCV_15D6RPM													
const byte M0_DN0_HTOLNOISCV_15D6RPM=22;					//	const byte M0_DN0_HTOLNOISCV_15D6RPM				p1									
const byte M0_N0_LISCV_15D6RPM=80;					//	const byte M0_N0_LISCV_15D6RPM													
const byte M0_N0_HISCV_15D6RPM=102;					//	const byte M0_N0_HISCV_15D6RPM													
const byte M0_NSTAND_62D5RPM=32;					//	const byte M0_NSTAND_62D5RPM													
const byte M0_NFULL_62D5RPM=61;					//	const byte M0_NFULL_62D5RPM													
const byte M0_KP_M8=255;					//	const byte M0_KP_M8													
const byte M0_KI_M8=255;					//	const byte M0_KI_M8													
const byte M0_KD_M8=255;					//	const byte M0_KD_M8													
const sbyte M0_K_15D6RPM=252;					//	const sbyte M0_K_15D6RPM													
const byte M0_J_M4=30;					//	const byte M0_J_M4													
																			
																			
/*------ BASIC LOAD ------*/																			
const byte M2_LDB_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	47,	41,	35,	32,	31,	30,	29,	29,	29,	29,	29,	28,	28,	28,	27,	26,	//	0	
	53,	49,	44,	41,	39,	36,	34,	33,	32,	31,	31,	31,	31,	31,	30,	29,	//	1	
	60,	57,	53,	50,	45,	42,	39,	37,	36,	35,	34,	34,	34,	34,	33,	32,	//	2	
	67,	65,	62,	59,	55,	51,	48,	45,	42,	40,	39,	38,	38,	38,	38,	37,	//	3	
	77,	75,	72,	69,	66,	63,	59,	56,	53,	50,	48,	45,	45,	45,	44,	42,	//	4	
	86,	83,	80,	78,	74,	71,	68,	65,	62,	59,	56,	54,	53,	52,	50,	47,	//	5	
	94,	91,	88,	85,	82,	79,	75,	72,	69,	67,	64,	62,	61,	58,	56,	53,	//	6	
	105,	100,	96,	92,	89,	86,	83,	81,	79,	77,	75,	72,	70,	67,	65,	61,	//	7	
	117,	111,	106,	101,	97,	94,	92,	90,	87,	84,	81,	79,	77,	74,	71,	67,	//	8	
	130,	122,	116,	112,	108,	105,	103,	100,	97,	94,	90,	87,	84,	81,	77,	73,	//	9	
	130,	133,	127,	122,	119,	115,	112,	110,	106,	103,	98,	93,	89,	86,	83,	79,	//	10	
	130,	133,	135,	132,	130,	127,	122,	118,	114,	110,	106,	102,	98,	93,	89,	85,	//	11	
	130,	133,	135,	137,	138,	137,	136,	133,	129,	124,	119,	115,	111,	106,	102,	96,	//	12	
	130,	133,	135,	137,	142,	142,	141,	139,	136,	132,	128,	125,	120,	116,	111,	105,	//	13	
	130,	133,	135,	137,	142,	144,	144,	142,	140,	137,	134,	131,	126,	122,	117,	111,	//	14	
	130,	133,	135,	137,	142,	144,	146,	146,	144,	141,	138,	134,	130,	125,	120,	115,	//	15	
	130,	133,	135,	137,	142,	144,	146,	148,	147,	144,	141,	137,	133,	128,	124,	120,	//	16	
};																			
																			
																			
/*------ BASIC LOAD CORRECTION ------*/																			
const byte M2_MPA_M128[8][16]={																			
	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,			
	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,			
	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,	108,			
	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,	96,			
	84,	84,	84,	84,	83,	83,	83,	83,	83,	83,	83,	83,	83,	83,	83,	83,			
	71,	71,	70,	69,	68,	68,	68,	68,	68,	68,	68,	68,	68,	68,	68,	68,			
	62,	61,	60,	59,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,	58,			
	54,	54,	54,	54,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,	48,			
};																			
																			
const byte M0_T_M64=55;																			
const byte M0_IDLEFMF_CO=128;																			
																			
const sbyte M1_PREINJ[4][2]={																			
	236	,	65	,															
	0	,	48	,															
	40	,	20	,															
	100	,	6	,															
};																			
																			
/*------ LAMBDA CONTROL & FUEL PLAN ------*/																			
																			
const sbyte M1_US_M32[8][2]={																			
	0	,	16	,															
	6	,	12	,															
	12	,	9	,															
	24	,	6	,															
	38	,	4	,															
	50	,	2	,															
	72	,	1	,															
	96	,	1	,															
};																			
																			
const sbyte M1_F_M256[8][2]={																			
	226	,	44	,															
	236	,	40	,															
	246	,	38	,															
	0	,	34	,															
	10	,	29	,															
	20	,	25	,															
	30	,	22	,															
	50	,	20	,															
};																			
																			
const sbyte M1_G0_M128[8][2]={																			
	226	,	58	,															
	236	,	55	,															
	246	,	53	,															
	0	,	49	,															
	10	,	47	,															
	20	,	46	,															
	30	,	45	,															
	50	,	45	,															
};																			
																			
const sbyte M1_E_M4[8][2]={																			
	226	,	54	,															
	236	,	48	,															
	246	,	42	,															
	0	,	35	,															
	10	,	30	,															
	20	,	27	,															
	30	,	25	,															
	50	,	20	,															
};																			
																			
const byte M1_NR0_62D5RPM[16]={																			
40,	88,	129,	142,	149,	153,	157,	157,	157,	157,	158,	159,	160,	161,	161,	161,				
};																			
																			
const sbyte M1_UR_M16[8][2]={																			
	236	,	51	,															
	246	,	45	,															
	0	,	30	,															
	20	,	25	,															
	40	,	20	,															
	60	,	16	,															
	80	,	10	,															
	120	,	2	,															
};																			
																			
const byte M1_NC_62D5[16]={																			
48,	120,	132,	144,	152,	156,	160,	160,	160,	160,	160,	162,	162,	164,	164,	164,				
};																			
																			
const sbyte M1_UA_M32[8][2]={																			
	0	,	30	,															
	4	,	24	,															
	8	,	20	,															
	16	,	18	,															
	32	,	15	,															
	64	,	12	,															
	96	,	8	,															
	120	,	3	,															
};																			
																			
const sbyte M2_UP[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	45,	45,	50,	75,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	//	0	
	50,	50,	75,	75,	75,	75,	75,	81,	80,	75,	75,	80,	80,	80,	80,	81,	//	1	
	65,	65,	60,	55,	55,	55,	55,	70,	60,	60,	60,	61,	62,	63,	80,	81,	//	2	
	75,	65,	60,	35,	35,	35,	35,	20,	20,	20,	20,	20,	20,	40,	65,	81,	//	3	
	80,	65,	60,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	4	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	5	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	6	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	7	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	8	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	9	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	10	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	11	
	80,	65,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	12	
	80,	65,	40,	30,	30,	30,	30,	30,	0,	0,	0,	0,	0,	40,	65,	81,	//	13	
	80,	70,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	65,	81,	//	14	
	87,	87,	87,	87,	85,	86,	85,	85,	86,	85,	86,	85,	85,	84,	87,	89,	//	15	
	87,	87,	87,	87,	85,	86,	85,	85,	86,	85,	86,	85,	85,	84,	87,	89,	//	16	
};																			
																			
																			
const byte M0_F0_M256=50;					//	const byte M0_F0_M256				51									
const byte M0_g_M128=15;					//	const byte M0_g_M128				19									
const byte M0_X=30;					//	const byte M0_X				0									
const byte M0_Y_M256=85;					//	const byte M0_Y_M256				25									
const byte M0_Z=10;					//	const byte M0_Z				0									
const byte M0_R_D4=15;					//	const byte M0_R_D4				100									
const byte M0_f_M128=4;					//	const byte M0_f_M128				0									
const byte M0_NRT_M64=16;					//	const byte M0_NRT_M64				16									
const byte M0_C_M8=65;					//	const byte M0_C_M8				60									
const byte M0_D_M256=230;					//	const byte M0_D_M256				210									
const byte M0_B_D8=60;					//	const byte M0_B_D8				60									
const byte M0_P_DEC_M64=25;					//	const byte M0_P_DEC_M64				24									
const byte M0_S_DEC_M128=30;					//	const byte M0_S_DEC_M128				81									
const byte M0_L_DEC_M1024=23;					//	const byte M0_L_DEC_M1024				40									
const byte M0_P_ACC_M64=35;					//	const byte M0_P_ACC_M64				24									
const byte M0_S_ACC_M128=18;					//	const byte M0_S_ACC_M128				48									
const byte M0_L_ACC_M1024=24;					//	const byte M0_L_ACC_M1024				40									
																			
/*------IGNITION------*/																			
const byte M2_IGN_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	13,	17,	24,	28,	33,	36,	39,	41,	44,	47,	49,	51,	53,	54,	54,	54,	//	0	
	13,	17,	24,	28,	33,	36,	39,	41,	44,	47,	49,	51,	53,	54,	54,	54,	//	1	
	13,	18,	24,	28,	33,	36,	39,	41,	44,	47,	49,	51,	53,	54,	54,	54,	//	2	
	13,	18,	25,	29,	33,	36,	39,	41,	43,	46,	49,	51,	53,	54,	54,	54,	//	3	
	14,	19,	25,	29,	33,	35,	38,	41,	43,	46,	49,	51,	52,	53,	53,	53,	//	4	
	14,	19,	25,	30,	32,	35,	38,	40,	42,	45,	48,	50,	52,	53,	53,	53,	//	5	
	14,	20,	26,	30,	32,	35,	38,	40,	42,	45,	48,	50,	51,	52,	53,	53,	//	6	
	14,	20,	26,	30,	31,	34,	38,	40,	42,	45,	48,	50,	51,	52,	52,	52,	//	7	
	14,	20,	26,	29,	31,	34,	37,	39,	41,	44,	47,	49,	51,	52,	52,	52,	//	8	
	14,	21,	25,	29,	31,	33,	35,	37,	39,	42,	45,	47,	49,	50,	51,	52,	//	9	
	14,	21,	25,	29,	31,	33,	35,	37,	39,	42,	45,	47,	49,	50,	51,	51,	//	10	
	14,	20,	24,	27,	29,	32,	34,	36,	38,	41,	44,	46,	48,	49,	51,	51,	//	11	
	14,	20,	24,	27,	29,	32,	34,	36,	38,	41,	44,	46,	48,	49,	50,	51,	//	12	
	14,	20,	21,	24,	25,	28,	30,	32,	34,	36,	39,	41,	43,	44,	44,	44,	//	13	
	14,	20,	21,	24,	25,	28,	30,	32,	34,	36,	39,	41,	43,	44,	44,	44,	//	14	
	14,	20,	21,	22,	23,	26,	28,	30,	32,	34,	37,	39,	41,	42,	42,	42,	//	15	
	14,	20,	21,	22,	23,	26,	28,	30,	32,	34,	37,	39,	41,	42,	42,	42,	//	16	
};																			
																			
const sbyte M2_IGC[8][2]={																			
	216	,	236	,															
	226	,	238	,															
	236	,	240	,															
	246	,	242	,															
	0	,	244	,															
	10	,	246	,															
	20	,	248	,															
	40	,	250	,															
};																			
																			
const sbyte M2_IGT[10][2]={																			
	236	,	17	,															
	252	,	13	,															
	12	,	10	,															
	28	,	7	,															
	44	,	5	,															
	60	,	2	,															
	76	,	0	,															
	92	,	254	,															
	108	,	252	,															
	124	,	251	,															
};																			
																			
const sbyte M0_IGNMAX_IDLE=21;					//	const sbyte M0_IGNMAX_IDLE				21									
const sbyte M0_IGNMIN_IDLE=0;					//	const sbyte M0_IGNMIN_IDLE				-43									
const byte M0_IGS=0;					//	const byte M0_IGS				14									
const byte M0_O=0;					//	const byte M0_O				0									
const byte M0_M=15;					//	const byte M0_M				34									
																			
																			
/*------COORDINATES------*/																			
//2016-8-11 从Data_sys_source.c中挪到这儿																			
//2015-11-20增加																			
//N_62D5_M2LDB:所有3D的X坐标，*62.5为实际单位rpm，表示为n；PAIR_DEG--M2_MPA_M128中纵坐标，表示为Pdeg，TPD_M2LDB--其他3D的纵坐标，表示为TPD																			
//			1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000	
//			0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
//			0	1	2	3	4	5	6	7									
																			
const byte N_62D5_M2LDB[16]={																			
24,	32,	40,	48,	56,	64,	72,	80,	88,	96,	104,	112,	120,	128,	136,	144,				
};																			
																			
const byte TPD_M2LDB[17]={																			
0,	16,	32,	48,	64,	80,	96,	112,	128,	144,	160,	176,	192,	208,	224,	240,	255,			
};																			
																			
const byte PAIR_DEG[8]={																			
0,	16,	32,	48,	64,	80,	96,	112,												
};																			
