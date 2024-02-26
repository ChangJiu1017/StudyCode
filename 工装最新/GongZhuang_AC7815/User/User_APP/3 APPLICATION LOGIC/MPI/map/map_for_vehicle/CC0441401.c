#include "derivative.h"																			
																			
																			
/*------ DATA INPUT ------*/																			
const byte M0_SD0_D12=54;					//	const byte M0_SD0_D12													
const byte M0_IBP=25;					//	const byte M0_IBP													
const byte M0_FBP=160;					//	const byte M0_FBP													
const byte M0_QL_D2=185;					//	const byte M0_QL_D2													
const byte M0_TPS0=23;					//	const byte M0_TPS0													
const byte M0_TPD_LT6=48;					//	const byte M0_TPD_LT6													
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
const byte M0_MAP_VERSION=3;					// 														
const byte MV_VEHICLE=3;					// 	1:HIDER 2:SCOOTER 3:CUB 4:WILD													
const byte M0_VBATLIMT=80;					//	const byte M0_VBATLIMT			//										
const byte M0_FMF_CO=128;					//	const byte M0_FMF_CO			//	p0d0.078125,phy=hex/128									
const byte M0_NB_INJDRV=40;					//	const byte M0_NB_INJDRV			//	pd1，phy=hex，def：40									
																			
//project code																			
const byte M0_PC_HH=67;					//	const byte M0_PC_HH			//	C									
const byte M0_PC_HL=67;					//	const byte M0_PC_HL			//	C									
const byte M0_PC_H=48;					//	const byte M0_PC_H			//	0									
const byte M0_PC_L=52;					//	const byte M0_PC_L			//	4									
const byte M0_PC_5=52;					//	const byte M0_PC_5			//	4	新增三个字节，与SCR保持一致								
const byte M0_PC_6=49;					//	const byte M0_PC_6			//	1									
const byte M0_PC_7=52;					//	const byte M0_PC_7			//	4									
																			
//预留																			
const byte M0_NIACVLM=64;					//	const byte M0_NIACVLM			//										
const byte M0_KP_IACV=30;					//	const byte M0_KP_IACV			//										
const byte M0_KI_IACV=10;					//	const byte M0_KI_IACV			//										
const byte M0_STEPBASE=58;					//	const byte M0_STEPBASE			//										
																			
																			
//IACV																			
//formular: p1; x-axis: v_rev_625rpm,y-axis:v_te+40																			
const byte M1_STEPINI[4][2]={																			
	20,	165,																	
	45,	110,																	
	90,	88,																	
	140,	68,																	
};																			
																			
//formular: p15d6;																			
const sbyte M1_STEPDEC[3][2]={																			
	0,	6,																	
	50,	9,																	
	100,	12,																	
};																			
																			
//formular: p1; x-axis: v_step_sm,y-axis:v_rev_625rpm																			
const sbyte M2_STEPLAMBDA[2][8]={																			
	243,	249,	9,	17,	22,	41,	58,	76,											
	243,	249,	9,	17,	22,	41,	58,	76,											
};																			
																			
//formular: p1;																			
const byte STEP_M2LAMBDA[8]={																			
30,	50,	70,	90,	110,	140,	170,	200,												
};																			
																			
//formular: p1;																			
const byte N_M2LAMBDA[2]={																			
90,	128,																		
};																			
																			
//X-te Y-stepincrease																			
const sbyte M1_STEINC_LIDLE[2][2]={																			
	10	,	7	,															
	100	,	3	,															
};																			
//X-te Y-stepincrease																			
const sbyte M1_STEINC_BIDLE[2][2]={																			
	10	,	7	,															
	100	,	4	,															
};																			
																			
const byte M1_STEPINC_PAIR[8]={																			
	0	,	8	,	18	,	35	,	50	,	63	,	75	,	75	,			
};																			
																			
//X-te Y-stepco formular:p0d02																			
const sbyte M1_STEPCO_PT[4][2]={																			
	236,	86,																	
	5,	57,																	
	50,	32,																	
	100,	18,																	
};																			
																			
//OBD																			
//	E	F	E	A	0	8	C	C	0	4	4	1	4	0	1				
const byte M1_VIN[17]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	52,	52,	49,	52,	48,	49,	0,	0,			
};																			
																			
//	E	F	E	A	0	8	C	C	0	4	4	1	4	0	1		 		
const byte M1_CID[16]={																			
69,	70,	69,	65,	48,	56,	67,	67,	48,	52,	52,	49,	52,	48,	49,	0,				
};																			
																			
//	E	C	M	-	J	I	A	N	S	H	E								
const byte M1_ECUNAME[20]={																			
69,	67,	77,	45,	74,	73,	65,	78,	83,	72,	69,	0,	0,	0,	0,	0,	0,	0,	0,	0,
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
const byte M0_U_CO_1ST=46;					//	const byte M0_U_CO_1ST													
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
const byte M0_DN0_HTOLNOISCV_15D6RPM=27;					//	const byte M0_DN0_HTOLNOISCV_15D6RPM				p1									
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
	47,	43,	40,	37,	32,	28,	26,	25,	23,	22,	21,	20,	19,	19,	18,	18,	//	0	
	49,	45,	42,	39,	35,	32,	30,	28,	27,	25,	24,	23,	22,	21,	21,	21,	//	1	
	52,	47,	45,	42,	40,	38,	35,	32,	31,	29,	28,	27,	26,	25,	24,	24,	//	2	
	57,	55,	52,	48,	45,	43,	40,	38,	35,	33,	32,	31,	30,	29,	28,	27,	//	3	
	66,	63,	60,	57,	54,	52,	49,	46,	43,	40,	37,	35,	34,	33,	32,	32,	//	4	
	75,	72,	69,	65,	62,	60,	57,	54,	51,	48,	45,	42,	41,	40,	40,	39,	//	5	
	83,	80,	77,	74,	71,	69,	66,	62,	59,	56,	53,	51,	49,	47,	47,	46,	//	6	
	91,	88,	85,	83,	80,	78,	76,	72,	69,	65,	62,	59,	57,	55,	53,	51,	//	7	
	101,	97,	94,	91,	88,	86,	84,	81,	76,	72,	68,	64,	62,	60,	58,	56,	//	8	
	112,	106,	102,	98,	95,	92,	90,	88,	84,	81,	77,	74,	71,	69,	66,	63,	//	9	
	112,	113,	109,	106,	102,	100,	97,	94,	91,	87,	83,	80,	78,	74,	72,	68,	//	10	
	112,	113,	115,	112,	110,	108,	105,	102,	98,	95,	91,	88,	85,	82,	78,	74,	//	11	
	112,	113,	115,	118,	117,	116,	115,	114,	111,	108,	104,	100,	96,	92,	88,	84,	//	12	
	112,	113,	115,	118,	122,	122,	121,	120,	117,	114,	111,	107,	104,	99,	95,	91,	//	13	
	112,	113,	115,	118,	122,	127,	126,	124,	122,	119,	116,	113,	110,	106,	102,	98,	//	14	
	112,	113,	115,	118,	122,	127,	130,	129,	127,	124,	121,	117,	114,	111,	107,	103,	//	15	
	112,	113,	115,	118,	122,	127,	130,	131,	130,	128,	124,	121,	118,	115,	111,	107,	//	16	
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
																			
const byte M0_T_M64=62;																			
const byte M0_IDLEFMF_CO=130;																			
																			
const sbyte M1_PREINJ[4][2]={																			
	236	,	45	,															
	0	,	30	,															
	40	,	15	,															
	100	,	8	,															
};																			
																			
/*------ LAMBDA CONTROL & FUEL PLAN ------*/																			
																			
const sbyte M1_US_M32[8][2]={																			
	0	,	26	,															
	4	,	19	,															
	8	,	16	,															
	16	,	12	,															
	24	,	8	,															
	32	,	5	,															
	48	,	3	,															
	80	,	1	,															
};																			
																			
const sbyte M1_F_M256[8][2]={																			
	226	,	41	,															
	236	,	38	,															
	246	,	33	,															
	0	,	28	,															
	10	,	26	,															
	20	,	24	,															
	30	,	23	,															
	50	,	22	,															
};																			
																			
const sbyte M1_G0_M128[8][2]={																			
	226	,	47	,															
	236	,	47	,															
	246	,	47	,															
	0	,	47	,															
	10	,	45	,															
	20	,	45	,															
	30	,	45	,															
	50	,	45	,															
};																			
																			
const sbyte M1_E_M4[8][2]={																			
	226	,	43	,															
	236	,	40	,															
	246	,	37	,															
	0	,	34	,															
	10	,	32	,															
	20	,	28	,															
	30	,	26	,															
	50	,	24	,															
};																			
																			
const byte M1_NR0_62D5RPM[16]={																			
44,	88,	129,	142,	149,	153,	157,	157,	157,	157,	158,	159,	160,	161,	161,	161,				
};																			
																			
const sbyte M1_UR_M16[8][2]={																			
	236	,	55	,															
	246	,	48	,															
	0	,	39	,															
	20	,	29	,															
	40	,	19	,															
	60	,	11	,															
	80	,	6	,															
	120	,	2	,															
};																			
																			
const byte M1_NC_62D5[16]={																			
48,	120,	132,	144,	152,	156,	160,	160,	160,	160,	160,	162,	162,	164,	164,	164,				
};																			
																			
const sbyte M1_UA_M32[8][2]={																			
	0	,	22	,															
	4	,	18	,															
	8	,	14	,															
	16	,	10	,															
	32	,	8	,															
	64	,	7	,															
	96	,	5	,															
	120	,	3	,															
};																			
																			
const sbyte M2_UP[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	45,	55,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	81,	//	0	
	45,	60,	75,	75,	75,	75,	75,	70,	80,	75,	75,	80,	80,	80,	80,	81,	//	1	
	65,	65,	75,	75,	75,	75,	75,	70,	60,	60,	60,	61,	62,	63,	80,	81,	//	2	
	75,	65,	75,	75,	75,	75,	70,	20,	20,	20,	20,	20,	20,	40,	65,	81,	//	3	
	80,	65,	65,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	40,	65,	81,	//	4	
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
	88,	88,	88,	89,	89,	89,	89,	89,	89,	89,	89,	89,	89,	89,	89,	89,	//	15	
	91,	91,	90,	90,	89,	88,	86,	86,	87,	87,	89,	90,	91,	93,	94,	95,	//	16	
};																			
																			
																			
const byte M0_F0_M256=51;					//	const byte M0_F0_M256				51									
const byte M0_g_M128=35;					//	const byte M0_g_M128				19									
const byte M0_X=28;					//	const byte M0_X				0									
const byte M0_Y_M256=88;					//	const byte M0_Y_M256				25									
const byte M0_Z=9;					//	const byte M0_Z				0									
const byte M0_R_D4=5;					//	const byte M0_R_D4				100									
const byte M0_f_M128=3;					//	const byte M0_f_M128				0									
const byte M0_NRT_M64=16;					//	const byte M0_NRT_M64				16									
const byte M0_C_M8=70;					//	const byte M0_C_M8				60									
const byte M0_D_M256=205;					//	const byte M0_D_M256				210									
const byte M0_B_D8=60;					//	const byte M0_B_D8				60									
const byte M0_P_DEC_M64=39;					//	const byte M0_P_DEC_M64				24									
const byte M0_S_DEC_M128=30;					//	const byte M0_S_DEC_M128				81									
const byte M0_L_DEC_M1024=41;					//	const byte M0_L_DEC_M1024				40									
const byte M0_P_ACC_M64=30;					//	const byte M0_P_ACC_M64				24									
const byte M0_S_ACC_M128=22;					//	const byte M0_S_ACC_M128				48									
const byte M0_L_ACC_M1024=36;					//	const byte M0_L_ACC_M1024				40									
																			
/*------IGNITION------*/																			
const byte M2_IGN_TPSRPM[17][16]={																			
//	1500	2000	2500	3000	3500	4000	4500	5000	5500	6000	6500	7000	7500	8000	8500	9000			
//	300	500	1000	1500	1800	2000	2200	2400	2600	2800	3000	3200	3400	3600	3800	4000			
	13,	17,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	0	
	13,	17,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	1	
	13,	18,	24,	28,	33,	36,	39,	43,	45,	48,	50,	53,	54,	54,	54,	54,	//	2	
	13,	18,	25,	29,	33,	36,	39,	43,	44,	47,	50,	52,	54,	54,	54,	54,	//	3	
	14,	19,	25,	29,	33,	35,	38,	43,	44,	47,	50,	52,	54,	54,	54,	54,	//	4	
	14,	19,	25,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	5	
	14,	20,	26,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	6	
	14,	20,	26,	30,	32,	35,	38,	42,	43,	46,	49,	51,	53,	54,	54,	54,	//	7	
	14,	20,	26,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	54,	54,	54,	//	8	
	14,	21,	25,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	53,	54,	54,	//	9	
	13,	21,	25,	29,	31,	34,	37,	41,	42,	45,	48,	50,	52,	53,	54,	54,	//	10	
	13,	20,	24,	28,	30,	33,	36,	40,	41,	44,	47,	49,	51,	53,	54,	54,	//	11	
	13,	20,	24,	28,	30,	33,	36,	40,	41,	44,	47,	49,	51,	53,	53,	54,	//	12	
	13,	20,	24,	28,	30,	33,	36,	40,	41,	44,	47,	49,	51,	53,	53,	54,	//	13	
	12,	19,	23,	27,	29,	32,	35,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	14	
	12,	19,	23,	27,	29,	32,	35,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	15	
	12,	19,	23,	27,	29,	32,	35,	38,	40,	43,	46,	48,	50,	52,	53,	54,	//	16	
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
const sbyte M0_IGNMIN_IDLE=4;					//	const sbyte M0_IGNMIN_IDLE				-43									
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
