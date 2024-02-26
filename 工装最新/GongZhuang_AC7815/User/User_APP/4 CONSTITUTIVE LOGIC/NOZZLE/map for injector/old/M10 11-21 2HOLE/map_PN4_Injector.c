#include "derivative.h"
#include "data_inj_source.h"

const word M0_INJ_SLOP_MAP=0x287;//2008-6-10	; t1map_slop_center=115(us/ul)*11(ul)/1000*512=647=0x287, the real can be modified by dkt1*0.055
const word M0_INJ_SHIFT_MAP= 0x3f9;//	; t10map_shift=4.07ms/0.004us=1017=0x3f9//2008-6-10
const word M0_TW1_CO_T35= 3000;//;USING T1=3.5MS FOR TV3_MODIFY
const word M0_TV3_T1EQ3D5_0=0;//;2008-7-23 for new pn model T3.0 �Ļ�׼��0.5=0+125*0.004 ,the base of tv3|t1=3.0ms is

const word M0_TV3_T13D5MS_SET= 125;

//2009-8-23 CHANGE TO 128
//const word M0_SUM_TV3_TEMP_SET=(word)1000;//	;SUMH_TV3_TEMP_SET=M0_TV3_T13D5MS_SET*4
const word M0_SUM_TV3_TEMP_SET=(word)500;//	;SUMH_TV3_TEMP_SET=M0_TV3_T13D5MS_SET*4

const byte M0_VBAT_ST=221;              //2008-8-5 ��������׼��ص�ѹ

//2008-12-3 ADD FOR 
const word M0_KT3_MAP=0x025c;
const word M0_DT30_MAP=0x0219;

const byte M0_PROG_NUM_HL_INJ=0x80;//2009-4-1 0xa0;               //#define M0_PROG_NUM_HL_INJ 0xA0//	;2008-3-5 0x90	;THE BIT4`BIT7 OF PRO_NUM_HL define the injector type,2008-3-5M8��׼����
const byte M0_NUMBER_T1=0x58;//#define M0_NUMBER_T1 	0x58//	;2008-3-21��ʼ ר�����ڴ�����������ϸС�����޸�
const byte M0_TYPE_INJCTOR=0x9;//#define M0_TYPE_INJCTOR 8	//;2008-2-22 ��������ΪM8:M7��T3 ,t30=2.15,kt3=1.144,t10=4.529,kt1=1.276
const word M0_MAX_TINJ=0xe00;//#define M0_MAX_TINJ 0xe00//
const word M0_MIN_TINJ=0x300;//#define M0_MIN_TINJ	0x300//EQU	03H	;IF	V_FMF_H<MIN_TINJ,LET	MAP_TINJ=0300H		
const byte M0_INJ_BIG_PCAH=2;// ��������ת�ٸ���3900ʱ�����T1�����Ƶ�ϵ����#define M0_INJ_BIG_PCAH	2//EQU	2	;WHEN	RPM>3907,	INJ	PULSE	CAN	BE	MORE	THAN	PCA	BY	2.048MS		
const byte M0_INJ_SHIFTH_REF=4;//#define M0_INJ_SHIFTH_REF	4//EQU	4	;INJ_SHIFT_REF=0X43E=4.344MS												
const byte M0_INJ_SHIFTL_REF=0x3e;//#define M0_INJ_SHIFTL_REF	0x3e//EQU	3EH													


const word M0_A0 =143;
const word M0_B0 =(sword)(-689);

const sbyte M1_MTV3_T1_V[12]=//mtv3_t1_v
{
 /*   -88 ,//2007-7-19 ,//-82 ,//-88 ,//2005.4.21 ,//2005.3.18 qj ,//-64	,//v=11	,larger 0.253,0x17f1
    -88 ,//-69 ,//-77 	,//v=11.5, larger 0.24 ,0x17f2
    -65 ,//-57 ,//-66 	,//v=12, larger 0.227,0x17f3
    -53 ,//-47 ,//-54 	,//v=12.5, larger 0.206,0x17f4
    -41 ,//-36 ,//-42	,//v=13, larger 0.169,0x17f5
    -29 ,//-25 ,//-30	,//v=13.5, larger 0.122 ,0x17f6
    -16 ,//-15 ,//-17	,//v=14, larger 0.07,0x17f7
    -4	,//-4 ,//-5	,//v=14.5, larger 0.02,0x17f8
    8 ,//6 ,//6	,//v=15, smaller 0.024,0x17f9
    20 ,//16 ,//20	,//v=15.5, smaller 0.079,0x17fa
    32 ,//27 ,//32	,//v=16.0, smaller 0.129,0x17fb
    32 //27 ,//32	,//v=16.0, smaller 0.129,0x17fc    */
	-56,//-54,//-67, //;2008-3-21	;-66 ;2007-12-27 ;-61 ;2007-10-31 ;-88  	;v=11	,larger 0.253,0x17f1
	-49,//-46,//-59,	//	;-57 		;-52 ;-88 	;v=11.5, larger 0.24 ,0x17f2
	-41,//-39,//-51,	//	;-50 		;-43 ;-65 	;v=12, larger 0.227,0x17f3
	-35,//-31,//-42,	//	;-39 		;-35 ;-53 	;v=12.5, larger 0.206,0x17f4
	-27,//-25,//-33,	//	;-28 		;-26 ;-41 	;v=13, larger 0.169,0x17f5
	-19,//-18,//-24,	//	;-18 		;-17 ;-29 	;v=13.5, larger 0.122 ,0x17f6
	-12,//-10,//-14,	//	;-9  		;-10 ;-16 	;v=14, larger 0.07,0x17f7
	-4,//-3,//-4,	//	;-4  		;-2  ;-4 	;v=14.5, larger 0.02,0x17f8
	4,//4,//6,	//	;6   		;5   ;8 	;v=15, smaller 0.024,0x17f9
	10,//15,	//	;15  		;12  ;20 	;v=15.5, smaller 0.079,0x17fa
	15,//25,	//	;25  		;20  ;32 	;v=16.0, smaller 0.129,0x17fb
	15,//25	//	;25  		;20  ;32 	;v=16.0, smaller 0.129,0x17fc

};//

//---
const byte ALTITUDE_K[8]=
{
	255,
	230,
	207,
	186,
	168,
	152,
	152,
	152    
};

//---add (the return value*8+m_tidcdyl)*4us
const sword M1_C_FMF_VBAT[8][10]=
{
    324	,//	ti=4.200,	v_Vbat_odr=11.0,	m_tidcdy=1.300,	addr=0x1101	40	4
    268	,//	ti=4.200,	v_Vbat_odr=11.5,	m_tidcdy=1.072,	addr=0x1102	33	4
    211	,//	ti=4.200,	v_Vbat_odr=12.0,	m_tidcdy=0.848,	addr=0x1103	26	3
    163	,//	ti=4.200,	v_Vbat_odr=12.5,	m_tidcdy=0.655,	addr=0x1104	20	3
    119	,//	ti=4.200,	v_Vbat_odr=13.0,	m_tidcdy=0.477,	addr=0x1105	14	7
    78	,//	ti=4.200,	v_Vbat_odr=13.5,	m_tidcdy=0.315,	addr=0x1106	9	6
    42	,//	ti=4.200,	v_Vbat_odr=14.0,	m_tidcdy=0.171,	addr=0x1107	5	2
    10	,//	ti=4.200,	v_Vbat_odr=14.5,	m_tidcdy=0.041,	addr=0x1108	1	2
    -10	,//	ti=4.200,	v_Vbat_odr=15.0,	m_tidcdy=-0.043,	addr=0x1109	-1	-2
    -10	,//	ti=4.200,	v_Vbat_odr=15.5,	m_tidcdy=-0.043,	addr=0x110a	-1	-2
    351	,//	ti=5.600,	v_Vbat_odr=11.0,	m_tidcdy=1.404,	addr=0x110b	43	7
    288	,//	ti=5.600,	v_Vbat_odr=11.5,	m_tidcdy=1.155,	addr=0x110c	36	0
    231	,//	ti=5.600,	v_Vbat_odr=12.0,	m_tidcdy=0.924,	addr=0x110d	28	7
    180	,//	ti=5.600,	v_Vbat_odr=12.5,	m_tidcdy=0.720,	addr=0x110e	22	4
    131	,//	ti=5.600,	v_Vbat_odr=13.0,	m_tidcdy=0.527,	addr=0x110f	16	3
    86	,//	ti=5.600,	v_Vbat_odr=13.5,	m_tidcdy=0.346,	addr=0x1110	10	6
    46	,//	ti=5.600,	v_Vbat_odr=14.0,	m_tidcdy=0.185,	addr=0x1111	5	6
    11	,//	ti=5.600,	v_Vbat_odr=14.5,	m_tidcdy=0.045,	addr=0x1112	1	3
    -14	,//	ti=5.600,	v_Vbat_odr=15.0,	m_tidcdy=-0.056,	addr=0x1113	-1	-6
    -14	,//	ti=5.600,	v_Vbat_odr=15.5,	m_tidcdy=-0.056,	addr=0x1114	-1	-6
    377	,//	ti=7.000,	v_Vbat_odr=11.0,	m_tidcdy=1.510,	addr=0x1115	47	1
    311	,//	ti=7.000,	v_Vbat_odr=11.5,	m_tidcdy=1.245,	addr=0x1116	38	7
    248	,//	ti=7.000,	v_Vbat_odr=12.0,	m_tidcdy=0.993,	addr=0x1117	31	0
    193	,//	ti=7.000,	v_Vbat_odr=12.5,	m_tidcdy=0.775,	addr=0x1118	24	1
    143	,//	ti=7.000,	v_Vbat_odr=13.0,	m_tidcdy=0.573,	addr=0x1119	17	7
    94	,//	ti=7.000,	v_Vbat_odr=13.5,	m_tidcdy=0.379,	addr=0x111a	11	6
    50	,//	ti=7.000,	v_Vbat_odr=14.0,	m_tidcdy=0.201,	addr=0x111b	6	2
    12	,//	ti=7.000,	v_Vbat_odr=14.5,	m_tidcdy=0.049,	addr=0x111c	1	4
    -15	,//	ti=7.000,	v_Vbat_odr=15.0,	m_tidcdy=-0.063,	addr=0x111d	-1	-7
    -15	,//	ti=7.000,	v_Vbat_odr=15.5,	m_tidcdy=-0.063,	addr=0x111e	-1	-7
    396	,//	ti=8.400,	v_Vbat_odr=11.0,	m_tidcdy=1.587,	addr=0x111f	49	4
    326	,//	ti=8.400,	v_Vbat_odr=11.5,	m_tidcdy=1.307,	addr=0x1120	40	6
    259	,//	ti=8.400,	v_Vbat_odr=12.0,	m_tidcdy=1.040,	addr=0x1121	32	3
    202	,//	ti=8.400,	v_Vbat_odr=12.5,	m_tidcdy=0.808,	addr=0x1122	25	2
    149	,//	ti=8.400,	v_Vbat_odr=13.0,	m_tidcdy=0.598,	addr=0x1123	18	5
    99	,//	ti=8.400,	v_Vbat_odr=13.5,	m_tidcdy=0.397,	addr=0x1124	12	3
    52	,//	ti=8.400,	v_Vbat_odr=14.0,	m_tidcdy=0.209,	addr=0x1125	6	4
    14	,//	ti=8.400,	v_Vbat_odr=14.5,	m_tidcdy=0.057,	addr=0x1126	1	6
    -17	,//	ti=8.400,	v_Vbat_odr=15.0,	m_tidcdy=-0.070,	addr=0x1127	-2	-1
    -17	,//	ti=8.400,	v_Vbat_odr=15.5,	m_tidcdy=-0.070,	addr=0x1128	-2	-1
    411	,//	ti=9.800,	v_Vbat_odr=11.0,	m_tidcdy=1.647,	addr=0x1129	51	3
    337	,//	ti=9.800,	v_Vbat_odr=11.5,	m_tidcdy=1.351,	addr=0x112a	42	1
    268	,//	ti=9.800,	v_Vbat_odr=12.0,	m_tidcdy=1.076,	addr=0x112b	33	4
    210	,//	ti=9.800,	v_Vbat_odr=12.5,	m_tidcdy=0.844,	addr=0x112c	26	2
    155	,//	ti=9.800,	v_Vbat_odr=13.0,	m_tidcdy=0.623,	addr=0x112d	19	3
    103	,//	ti=9.800,	v_Vbat_odr=13.5,	m_tidcdy=0.414,	addr=0x112e	12	7
    56	,//	ti=9.800,	v_Vbat_odr=14.0,	m_tidcdy=0.224,	addr=0x112f	7	0
    14	,//	ti=9.800,	v_Vbat_odr=14.5,	m_tidcdy=0.060,	addr=0x1130	1	6
    -18	,//	ti=9.800,	v_Vbat_odr=15.0,	m_tidcdy=-0.075,	addr=0x1131	-2	-2
    -18	,//	ti=9.800,	v_Vbat_odr=15.5,	m_tidcdy=-0.075,	addr=0x1132	-2	-2
    427	,//	ti=11.200,	v_Vbat_odr=11.0,	m_tidcdy=1.710,	addr=0x1133	53	3
    345	,//	ti=11.200,	v_Vbat_odr=11.5,	m_tidcdy=1.381,	addr=0x1134	43	1
    277	,//	ti=11.200,	v_Vbat_odr=12.0,	m_tidcdy=1.111,	addr=0x1135	34	5
    217	,//	ti=11.200,	v_Vbat_odr=12.5,	m_tidcdy=0.872,	addr=0x1136	27	1
    160	,//	ti=11.200,	v_Vbat_odr=13.0,	m_tidcdy=0.641,	addr=0x1137	20	0
    107	,//	ti=11.200,	v_Vbat_odr=13.5,	m_tidcdy=0.429,	addr=0x1138	13	3
    58	,//	ti=11.200,	v_Vbat_odr=14.0,	m_tidcdy=0.234,	addr=0x1139	7	2
    15	,//	ti=11.200,	v_Vbat_odr=14.5,	m_tidcdy=0.063,	addr=0x113a	1	7
    -21	,//	ti=11.200,	v_Vbat_odr=15.0,	m_tidcdy=-0.085,	addr=0x113b	-2	-5
    -21	,//	ti=11.200,	v_Vbat_odr=15.5,	m_tidcdy=-0.085,	addr=0x113c	-2	-5
    438	,//	ti=12.700,	v_Vbat_odr=11.0,	m_tidcdy=1.753,	addr=0x113d	54	6
    353	,//	ti=12.700,	v_Vbat_odr=11.5,	m_tidcdy=1.415,	addr=0x113e	44	1
    286	,//	ti=12.700,	v_Vbat_odr=12.0,	m_tidcdy=1.146,	addr=0x113f	35	6
    223	,//	ti=12.700,	v_Vbat_odr=12.5,	m_tidcdy=0.894,	addr=0x1140	27	7
    165	,//	ti=12.700,	v_Vbat_odr=13.0,	m_tidcdy=0.663,	addr=0x1141	20	5
    111	,//	ti=12.700,	v_Vbat_odr=13.5,	m_tidcdy=0.447,	addr=0x1142	13	7
    60	,//	ti=12.700,	v_Vbat_odr=14.0,	m_tidcdy=0.243,	addr=0x1143	7	4
    17	,//	ti=12.700,	v_Vbat_odr=14.5,	m_tidcdy=0.068,	addr=0x1144	2	1
    -21	,//	ti=12.700,	v_Vbat_odr=15.0,	m_tidcdy=-0.085,	addr=0x1145	-2	-5
    -21	,//	ti=12.700,	v_Vbat_odr=15.5,	m_tidcdy=-0.085,	addr=0x1146	-2	-5
    447	,//	ti=14.100,	v_Vbat_odr=11.0,	m_tidcdy=1.790,	addr=0x1147	55	7
    363	,//	ti=14.100,	v_Vbat_odr=11.5,	m_tidcdy=1.455,	addr=0x1148	45	3
    293	,//	ti=14.100,	v_Vbat_odr=12.0,	m_tidcdy=1.172,	addr=0x1149	36	5
    228	,//	ti=14.100,	v_Vbat_odr=12.5,	m_tidcdy=0.912,	addr=0x114a	28	4
    167	,//	ti=14.100,	v_Vbat_odr=13.0,	m_tidcdy=0.670,	addr=0x114b	20	7
    114	,//	ti=14.100,	v_Vbat_odr=13.5,	m_tidcdy=0.458,	addr=0x114c	14	2
    63	,//	ti=14.100,	v_Vbat_odr=14.0,	m_tidcdy=0.254,	addr=0x114d	7	7
    17	,//	ti=14.100,	v_Vbat_odr=14.5,	m_tidcdy=0.068,	addr=0x114e	2	1
    -20	,//	ti=14.100,	v_Vbat_odr=15.0,	m_tidcdy=-0.082,	addr=0x114f	-2	-4
    -20	,//	ti=14.100,	v_Vbat_odr=15.5,	m_tidcdy=-0.082,	addr=0x1150	-2	-4
};

//2015-4-14					
const word M1_DT3_Q[29]={//DT3_Q_ST_4US,FMF_ODR,Q,DT3_Q_ST				Q	DT3	Q*250/11	DT3/0.004	��DT3/��FMF
24	,//	0	0	0.098453513	0	24	0.3125
34	,//	1	1.408	0.136884539	32	34	0.28125
43	,//	2	2.816	0.175960766	64	43	0.3125
53	,//	3	4.224	0.215682191	96	53	0.34375
64	,//	4	5.632	0.256048816	128	64	0.3125
74	,//	5	7.04	0.29706064	160	74	0.3125
84	,//	6	8.448	0.338717663	192	84	0.34375
95	,//	7	9.856	0.381019885	224	95	0.3125
105	,//	8	11.264	0.423967307	256	105	0.34375
116	,//	9	12.672	0.467559928	288	116	0.34375
127	,//	10	14.08	0.511797748	320	127	0.375
139	,//	11	15.488	0.556680768	352	139	0.34375
150	,//	12	16.896	0.602208986	384	150	0.375
162	,//	13	18.304	0.648382404	416	162	0.34375
173	,//	14	19.712	0.695201022	448	173	0.375
185	,//	15	21.12	0.742664838	480	185	0.375
197	,//	16	22.528	0.790773854	512	197	0.375
209	,//	17	23.936	0.839528069	544	209	0.40625
222	,//	18	25.344	0.888927483	576	222	0.375
234	,//	19	26.752	0.938972097	608	234	0.40625
247	,//	20	28.16	0.989661909	640	247	0.40625
260	,//	21	29.568	1.040996921	672	260	0.40625
273	,//	22	30.976	1.092977133	704	273	0.40625
286	,//	23	32.384	1.145602543	736	286	0.40625
299	,//	24	33.792	1.198873153	768	299	0.4375
313	,//	25	35.2	1.252788962	800	313	0.40625
326	,//	26	36.608	1.30734997	832	326	0.4375
340	,//	27	38.016	1.362556178	864	340	0.4375
354	,//	28	39.424	1.418407584	896	354	
};	

