#include "derivative.h"																			
																			
																			
/*------ DATA INPUT ------*/																			
const byte M0_SD0_D12=54;					//	const byte M0_SD0_D12													
const byte M0_IBP=25;					//	const byte M0_IBP													
const byte M0_FBP=160;					//	const byte M0_FBP													
const byte M0_QL_D2=185;					//	const byte M0_QL_D2													
const byte M0_TPS0=23;					//	const byte M0_TPS0													
const byte M0_TPD_LT6=64;					//	const byte M0_TPD_LT6													
const byte M0_TPD_LT11=96;					//	const byte M0_TPD_LT11													
const byte M0_TPD_LT14=112;					//	const byte M0_TPD_LT14													
const byte M0_TPD_LT16=176;					//	const byte M0_TPD_LT16													
const byte M0_OS_LIGHTOFF=56;					//	const byte M0_OS_LIGHTOFF													
const byte M0_CAS_FRONTEDGE=150;					//	const byte M0_CAS_FRONTEDGE													
const byte M0_CAS_TRAILEDGE=64;					//	const byte M0_CAS_TRAILEDGE													
const byte M0_PAIRS_SEALEVEL=170;					//	const byte M0_PAIRS_SEALEVEL													
const byte M0_PAIRS_PER1000M=16;					//	const byte M0_PAIRS_PER1000M													
const byte M0_WIDTH_CDION=250;					//	const byte M0_WIDTH_CDION													
const byte MV_EXHAUST=110;					//	const byte MV_EXHAUST													
const byte M0_BAUDRATE=255;					//	const byte M0_BAUDRATE													
const byte M0_MAP_VERSION=2;					// 														
const byte MV_VEHICLE=3;					// 	1:HIDER 2:SCOOTER 3:CUB 4:WILD													
const byte M0_VBATLIMT=80;					//	const byte M0_VBATLIMT			//										
const byte M0_FMF_CO=115;					//	const byte M0_FMF_CO			//	p0d0.078125,phy=hex/128									
const byte M0_NB_INJDRV=40;					//	const byte M0_NB_INJDRV			//	pd1，phy=hex，def：40									
																			
//project code																			
const byte M0_PC_HH=67;					//	const byte M0_PC_HH			//	C									
const byte M0_PC_HL=67;					//	const byte M0_PC_HL			//	C									
const byte M0_PC_H=48;					//	const byte M0_PC_H			//	0									
const byte M0_PC_L=54;					//	const byte M0_PC_L			//	6									
const byte M0_PC_5=52;					//	const byte M0_PC_5			//	4	新增三个字节，与SCR保持一致								
const byte M0_PC_6=49;					//	const byte M0_PC_6			//	1									
const byte M0_PC_7=52;					//	const byte M0_PC_7			//	4									
																			
//预留																			
const byte M0_NIACVLM=64;					//	const byte M0_NIACVLM			//										
const byte M0_KP_IACV=30;					//	const byte M0_KP_IACV			//										
const byte M0_KI_IACV=9;					//	const byte M0_KI_IACV			//										
const byte M0_STEPBASE=50;					//	const byte M0_STEPBASE			//										
																			
																			
//IACV																			
//formular: p1; x-axis: v_rev_625rpm,y-axis:v_te+40																			
const byte M1_STEPINI[4][2]={																			
	20,	170,																	
	40,	115,																	
	90,	90,																	
	140,	65,																	
};																			
																			
//formular: p15d6;																			
const sbyte M1_STEPDEC[3][2]={																			
	0,	5,																	
	40,	7,																	
	100,	8,																	
};																			
																			
//formular: p1; x-axis: v_step_sm,y-axis:v_rev_625rpm																			
const sbyte M2_STEPLAMBDA[2][8]={																			
	236,	243,	0,	12,	19,	23,	44,	80,											
	238,	243,	0,	12,	22,	28,	49,	80,											
};																			
																			
//formular: p1;																			
const byte STEP_M2LAMBDA[8]={																			
15,	30,	50,	70,	90,	110,	140,	200,												
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
	10	,	3	,															
	96	,	2	,															
};																			
																			
const byte M1_STEPINC_PAIR[8]={																			
	0	,	8	,	18	,	35	,	49	,	59	,	69	,	75	,			
};																			
																			
//X-te Y-stepco formular:p0d02																			
const sbyte M1_STEPCO_PT[4][2]={																			
	236,	85,																	
	0,	62,																	
	50,	28,																	
	100,	16,																	
};																			
																			
//OBD																			
//	E	F	E	A	0	8	C	C	0	6	4	1	4	0	1				
const byte M1_VIN[17]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	54,	52,	49,	52,	48,	49,	0,	0,			
};																			
																			
//	E	F	E	A	0	8	C	C	0	6	4	1	4	0	1		 		
const byte M1_CID[16]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	54,	52,	49,	52,	48,	49,	0,				
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
const byte M0_U_CO_1ST=38;					//	const byte M0_U_CO_1ST													
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
const byte M0_UL_CYCLE=20;					//	const byte M0_UL_CYCLE													
																			
																			
/*------ SPEED CONTROL------*/																			
const byte M0_DN_25=11;					//	const byte M0_DN_25													
const byte M0_DN_153=12;					//	const byte M0_DN_153													
const byte M0_N0_LNOISCV_15D6RPM=97;					//	const byte M0_N0_LNOISCV_15D6RPM													
const byte M0_DN0_HTOLNOISCV_15D6RPM=29;					//	const byte M0_DN0_HTOLNOISCV_15D6RPM				p1									
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
	46,	42,	38,	36,	35,	33,	32,	31,	30,	30,	30,	29,	28,	26,	25,	25,	//	0	
	53,	51,	48,	45,	41,	40,	39,	38,	37,	36,	34,	33,	32,	31,	29,	29,	//	1	
	60,	56,	53,	50,	48,	46,	44,	42,	41,	39,	38,	38,	37,	36,	34,	33,	//	2	
	67,	63,	60,	57,	53,	51,	49,	47,	45,	44,	43,	42,	41,	40,	39,	38,	//	3	
	75,	70,	65,	62,	60,	58,	56,	53,	51,	49,	47,	47,	45,	45,	44,	43,	//	4	
	82,	77,	73,	70,	67,	65,	64,	62,	59,	57,	55,	54,	52,	51,	50,	49,	//	5	
	92,	87,	81,	76,	74,	72,	70,	69,	67,	64,	61,	59,	59,	57,	56,	55,	//	6	
	100,	95,	90,	86,	83,	80,	78,	75,	72,	70,	67,	65,	63,	62,	61,	60,	//	7	
	111,	105,	99,	95,	92,	89,	86,	83,	80,	76,	72,	69,	67,	66,	64,	63,	//	8	
	121,	114,	109,	105,	100,	97,	94,	91,	87,	83,	80,	77,	74,	71,	68,	65,	//	9	
	121,	123,	116,	112,	108,	106,	103,	100,	96,	92,	89,	86,	83,	79,	75,	71,	//	10	
	121,	123,	125,	120,	117,	114,	112,	109,	106,	103,	99,	95,	91,	86,	81,	77,	//	11	
	121,	123,	125,	127,	125,	123,	122,	121,	119,	115,	111,	108,	104,	99,	94,	89,	//	12	
	121,	123,	125,	127,	129,	127,	126,	126,	125,	123,	120,	117,	113,	109,	104,	100,	//	13	
	121,	123,	125,	127,	129,	131,	131,	130,	129,	126,	123,	120,	117,	113,	108,	104,	//	14	
	121,	123,	125,	127,	129,	131,	133,	133,	132,	130,	127,	123,	120,	116,	112,	107,	//	15	
	121,	123,	125,	127,	129,	131,	134,	136,	136,	133,	130,	127,	123,	119,	115,	111,	//	16	
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
																			
const byte M0_T_M64=53;																			
const byte M0_IDLEFMF_CO=129;																			
																			
const sbyte M1_PREINJ[4][2]={																			
	236	,	50	,															
	0	,	32	,															
	40	,	15	,															
	100	,	7	,															
};																			
																			
/*------ LAMBDA CONTROL & FUEL PLAN ------*/																			
																			
const sbyte M1_US_M32[8][2]={																			
	0	,	20	,															
	6	,	12	,															
	14	,	9	,															
	26	,	8	,															
	38	,	4	,															
	50	,	3	,															
	68	,	2	,															
	80	,	1	,															
};																			
																			
const sbyte M1_F_M256[8][2]={																			
	226	,	40	,															
	236	,	38	,															
	246	,	35	,															
	0	,	33	,															
	10	,	32	,															
	20	,	29	,															
	30	,	26	,															
	50	,	24	,															
};																			
																			
const sbyte M1_G0_M128[8][2]={																			
	226	,	49	,															
	236	,	48	,															
	246	,	47	,															
	0	,	46	,															
	10	,	45	,															
	20	,	45	,															
	30	,	43	,															
	50	,	43	,															
};																			
																			
const sbyte M1_E_M4[8][2]={																			
	226	,	48	,															
	236	,	46	,															
	246	,	44	,															
	0	,	42	,															
	10	,	40	,															
	20	,	36	,															
	30	,	31	,															
	50	,	28	,															
};																			
																			
const byte M1_NR0_62D5RPM[16]={																			
44,	88,	129,	142,	149,	153,	157,	157,	157,	157,	158,	159,	160,	161,	161,	161,				
};																			
																			
const sbyte M1_UR_M16[8][2]={																			
	236	,	59	,															
	246	,	51	,															
	0	,	39	,															
	20	,	30	,															
	40	,	22	,															
	60	,	15	,															
	80	,	9	,															
	120	,	3	,															
};																			
																			
const byte M1_NC_62D5[16]={																			
48,	120,	132,	144,	152,	156,	160,	160,	160,	160,	160,	162,	162,	164,	164,	164,				
};																			
																			
const sbyte M1_UA_M32[8][2]={																			
	0	,	37	,															
	6	,	32	,															
	14	,	26	,															
	26	,	22	,															
	38	,	17	,															
	64	,	12	,															
	96	,	7	,															
	120	,	5	,															
};																			
																			
const sbyte M2_UP[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	45,	45,	50,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	//	0	
	60,	65,	70,	70,	70,	70,	70,	81,	80,	75,	75,	80,	80,	80,	80,	81,	//	1	
	80,	75,	70,	70,	70,	30,	30,	30,	30,	30,	30,	30,	30,	30,	80,	81,	//	2	
	80,	75,	70,	70,	70,	30,	30,	20,	20,	20,	20,	20,	20,	40,	65,	81,	//	3	
	88,	75,	65,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	4	
	88,	75,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	5	
	88,	75,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	6	
	88,	75,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	7	
	88,	75,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	8	
	89,	75,	40,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	9	
	89,	75,	60,	60,	60,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	10	
	89,	85,	60,	60,	60,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	11	
	89,	85,	60,	60,	60,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	12	
	89,	85,	60,	60,	60,	30,	30,	30,	0,	0,	0,	0,	0,	40,	65,	81,	//	13	
	89,	80,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	65,	81,	//	14	
	95,	95,	92,	93,	93,	91,	91,	90,	89,	90,	89,	90,	90,	91,	92,	92,	//	15	
	95,	95,	93,	93,	93,	92,	92,	91,	90,	91,	90,	91,	91,	92,	93,	93,	//	16	
};																			
																			
																			
const byte M0_F0_M256=51;					//	const byte M0_F0_M256				51									
const byte M0_g_M128=35;					//	const byte M0_g_M128				19									
const byte M0_X=30;					//	const byte M0_X				0									
const byte M0_Y_M256=85;					//	const byte M0_Y_M256				25									
const byte M0_Z=15;					//	const byte M0_Z				0									
const byte M0_R_D4=8;					//	const byte M0_R_D4				100									
const byte M0_f_M128=3;					//	const byte M0_f_M128				0									
const byte M0_NRT_M64=16;					//	const byte M0_NRT_M64				16									
const byte M0_C_M8=75;					//	const byte M0_C_M8				60									
const byte M0_D_M256=220;					//	const byte M0_D_M256				210									
const byte M0_B_D8=60;					//	const byte M0_B_D8				60									
const byte M0_P_DEC_M64=40;					//	const byte M0_P_DEC_M64				24									
const byte M0_S_DEC_M128=28;					//	const byte M0_S_DEC_M128				81									
const byte M0_L_DEC_M1024=32;					//	const byte M0_L_DEC_M1024				40									
const byte M0_P_ACC_M64=28;					//	const byte M0_P_ACC_M64				24									
const byte M0_S_ACC_M128=22;					//	const byte M0_S_ACC_M128				48									
const byte M0_L_ACC_M1024=35;					//	const byte M0_L_ACC_M1024				40									
																			
/*------IGNITION------*/																			
const byte M2_IGN_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	13,	20,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	0	
	13,	20,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	1	
	14,	20,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	2	
	15,	20,	25,	29,	33,	36,	39,	43,	44,	47,	50,	52,	54,	54,	54,	54,	//	3	
	16,	22,	25,	29,	33,	35,	38,	43,	44,	47,	50,	52,	54,	54,	54,	54,	//	4	
	17,	22,	25,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	5	
	18,	22,	26,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	6	
	19,	22,	26,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	7	
	20,	22,	26,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	54,	54,	54,	//	8	
	20,	22,	26,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	53,	54,	54,	//	9	
	20,	23,	26,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	53,	54,	54,	//	10	
	20,	23,	26,	29,	31,	34,	36,	40,	41,	44,	47,	49,	51,	53,	54,	54,	//	11	
	20,	23,	26,	29,	31,	34,	36,	40,	41,	44,	47,	49,	51,	53,	53,	54,	//	12	
	20,	23,	26,	29,	31,	34,	36,	40,	41,	44,	47,	49,	51,	53,	53,	54,	//	13	
	20,	23,	26,	29,	31,	34,	36,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	14	
	20,	23,	26,	29,	31,	34,	36,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	15	
	20,	23,	26,	29,	31,	34,	36,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	16	
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
