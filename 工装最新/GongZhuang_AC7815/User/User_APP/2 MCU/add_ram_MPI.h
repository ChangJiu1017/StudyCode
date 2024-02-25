

//#include "hard_trig.h"
//#include "PE_Types.h"
#include "FAM.h"
/*
#define	M0_T0	273					
#define	M0_T0_M2048	559104					
#define	M0_T0_M8192	2236416					
							
#define	ADD_RAM_CAB_ST_B	0x2343					
#define	ADD_RAM_CAB_ST_W	0x241a					
#define	ADD_STATUS_ST	0x2301					
*/

extern	byte	v_nb_notsyninj_add	;//ADD_RAM_CAB_ST_B	+	22	;	//单个循环喷射的次数；

							
/*================================================================*/							
//以下是标定参数的列表，							
//byte	or	sbyte型					
//COS ;// KESEI=18||KESEI=255 ,ADJUST BY COS UP/DOWN , "+"后面的数对应的是REMARK							
/*fuel*/							
extern	volatile byte	v_tair_ad	;//ADD_RAM_CAB_ST_B	+	2	;	//空气温度采样值
extern	sbyte	V_Ta	;//ADD_RAM_CAB_ST_B	+	3	;	//空气温度，单位：度数
extern	byte	v_teng_ad	;//ADD_RAM_CAB_ST_B	+	4	;	//缸头温度采样值
						;	
extern	byte	v_G_M128	;//ADD_RAM_CAB_ST_B	+	5	;	//v_G_M128=M0_XZ_M16384
//extern	byte	v_XX	;//ADD_RAM_CAB_ST_B	+	6	;	//v_XX=M0_XX_M1024_M273
						;	
//extern	byte	vc_A_mf	;//ADD_RAM_CAB_ST_B	+	7	;	//油膜模型中的A
extern	byte	vc_B_mf	;//ADD_RAM_CAB_ST_B	+	8	;	//油膜模型中的B
extern	byte	vc_C_mf	;//ADD_RAM_CAB_ST_B	+	9	;	//油膜模型中的C
extern	byte	vc_D_mf	;//ADD_RAM_CAB_ST_B	+	10	;	//油膜模型中的D
extern	byte	vc_SD0_mf	;//ADD_RAM_CAB_ST_B	+	11	;	//油膜模型中的SD0
						;	
extern	byte	v_M0_E_M8	;//ADD_RAM_CAB_ST_B	+	12	;	//启动模型中的E
extern	byte	v_M0_F_M256	;//ADD_RAM_CAB_ST_B	+	13	;	//启动模型中的F
						;	
extern	sbyte	vc_FMF_teng	;//ADD_RAM_CAB_ST_B	+	14	;	//油量的缸头温度修正系数
//extern	byte	vc_FMF_tair	;//ADD_RAM_CAB_ST_B	+	15	;	//空气温度对油量的修正系数
						;	
extern	byte	v_P_DEC_M64	;//ADD_RAM_CAB_ST_B	+	16	;	//减速时油膜向短项的分配系数
extern	byte	v_P_ACC_M64	;//ADD_RAM_CAB_ST_B	+	17	;	//加速时油膜向短项的分配系数
extern	byte	v_L_M1024	;//ADD_RAM_CAB_ST_B	+	18	;	//油膜长项衰减系数
extern	byte	v_S_M128	;//ADD_RAM_CAB_ST_B	+	19	;	//油膜短项衰减系数
//extern	sbyte	v_M0_DUKK	;//ADD_RAM_CAB_ST_B	+	20	;	//分配时最小步数
						;	
extern	byte	v_LAMBDA_byMAPS	;//ADD_RAM_CAB_ST_B	+	21	;	//线性氧传感器值


extern	sbyte	v_dXZ	;//ADD_RAM_CAB_ST_B	+	23	;	//v_G_M128=M0_XZ_M16384
extern	byte	v_T_M64	;//ADD_RAM_CAB_ST_B	+	24	;	//离开怠速时U
extern	byte	v_UR_M16	;//ADD_RAM_CAB_ST_B	+	25	;	//恢复供油时U
extern	byte	v_UA_M128	;//ADD_RAM_CAB_ST_B	+	26	;	//加速时U
extern	byte	v_Y_M256	;//ADD_RAM_CAB_ST_B	+	27	;	//急加速时U


extern	sbyte	v_UP_M128	;//ADD_RAM_CAB_ST_B	+	30	;	//全负荷U
extern	sbyte	v_LambdaTg	;//ADD_RAM_CAB_ST_B	+	31	;	//全负荷U的增量

/*ALTITUDE*/						;	
//extern	byte	v_kDt	;//ADD_RAM_CAB_ST_B	+	38	;	//海拔过渡修正系数；
//extern	byte	v_Altitude_beta	;//ADD_RAM_CAB_ST_B	+	39	;	//beta*8
////extern	byte	v_Altitude_nta	;//ADD_RAM_CAB_ST_B	+	40	;	//2008-9-11
/*cls*/						;	
extern	sbyte	leansdtav	;//ADD_RAM_CAB_ST_B	+	41	;	//
							
extern	byte	nb_for_leantest	;//ADD_RAM_CAB_ST_B	+	43	;	
extern	sbyte	v_dmLambda	;//ADD_RAM_CAB_ST_B	+	44	;	//本次O2反馈结果对油量的修正系数
//extern	sbyte	vc_CLS_IDLE_av	;//ADD_RAM_CAB_ST_B	+	45	;	//O2反馈的平均
						;	
/*cls	learned*/					;	
//extern	byte	nb_Lean_max	;//ADD_RAM_CAB_ST_B	+	46	;	//在一定期间内，连续稀的最大圈数；
//extern	byte	nb_Rich_max	;//ADD_RAM_CAB_ST_B	+	47	;	//在一定期间内，连续浓的最大圈数；
extern	sbyte	c_cls_max	;//ADD_RAM_CAB_ST_B	+	48	;	//在一定期间内，闭环修正量的最大值
extern	sbyte	c_cls_min	;//ADD_RAM_CAB_ST_B	+	49	;	//在一定期间内，闭环修正量的最小值；

extern	sbyte	v_cls_sl_cur	;//ADD_RAM_CAB_ST_B	+	57	;	//当前学习值；
extern	sbyte	v_SL_cls	;//ADD_RAM_CAB_ST_B	+	58	;	//非怠速的自学习值

							
/*DTC*/						;	
extern	byte	t_qhymax_p	;//ADD_RAM_CAB_ST_B	+	60	;	//前后沿最大时间间隔单位；
extern	byte	t_qhymin_p	;//ADD_RAM_CAB_ST_B	+	61	;	//前后沿最小时间间隔单位；
extern	byte	v_qhCAint_ave	;//ADD_RAM_CAB_ST_B	+	62	;	//v_qhCAint_to_prd_m255的平均值
extern	byte	v_qhCAint_to_prd_m255	;//ADD_RAM_CAB_ST_B	+	63	;	//2009-8-7
						;	
/*IGN*/						;	
extern	byte	inc_revobj_worm	;//ADD_RAM_CAB_ST_B	+	70	;	//点火角控制目标转速的修正量
extern	sbyte	co_IdleIgn_I	;//ADD_RAM_CAB_ST_B	+	71	;	//怠速点火角反馈值的积分，0.7度CA
extern	sbyte	co_IdleIgn_byRPM	;//ADD_RAM_CAB_ST_B	+	72	;	//怠速点火角反馈值，0.7度CA
						;	
extern	sbyte	ign_lst8run_ave	;//ADD_RAM_CAB_ST_B	+	73	;	//最近几圈怠速点火角的平均；0.7度CA
extern	byte	nb_ign	;//ADD_RAM_CAB_ST_B	+	74	;	//点火次数
extern	byte	nb_lose_hy	;//ADD_RAM_CAB_ST_B	+	75	;	//丢失后沿的次数
extern	byte	v_rev15d6_target_idle	;//ADD_RAM_CAB_ST_B	+	76	;	//怠速目标
						;	
extern	sbyte	v_CA_ignition_lst	;//ADD_RAM_CAB_ST_B	+	77	;	//点火提前角，量纲：0.7CA
extern	sbyte	ign_step_mode	;//ADD_RAM_CAB_ST_B	+	78	;	//2010-1-11
extern	sbyte	v_CA_limit_byrpm	;//ADD_RAM_CAB_ST_B	+	79	;	//根据转速计算的点火角的限值，0.7CA
extern	sbyte	v_Ign_Map	;//ADD_RAM_CAB_ST_B	+	80	;	//map的点火角，对于怠速时需要考虑点火修正；
extern	byte	dIgn_perRev_M16	;//ADD_RAM_CAB_ST_B	+	81	;	//怠速转速对点火角的反馈强度，0.7CA/15.6rpm*16
extern	byte	m_nbtdc0	;//ADD_RAM_CAB_ST_B	+	82	;	//点火触发沿的监督，0.7CA，根据情况选择前沿或后沿；
						;	
extern	sbyte	v_Ign_I_notidle	;//ADD_RAM_CAB_ST_B	+	83	;	//非怠速点火反馈值；
extern	sbyte	vc_Ign_Altitude	;//ADD_RAM_CAB_ST_B	+	84	;	//VC_IGN_ALTITUDE
						;	
extern	sbyte	v_IgnCA_act	;//ADD_RAM_CAB_ST_B	+	86	;	//实际测得的点火角，0.7CA
extern	sbyte	v_IgnCA_act_lst	;//ADD_RAM_CAB_ST_B	+	87	;	//上一次实际测得的点火角，0.7CA
						;	
/*sensor*/						;	
//extern word nb_forpair;//2014-1-28 peter, delete 
extern	byte	v_pair_ad	;//ADD_RAM_CAB_ST_B	+	100	;	//pair的AD
extern	byte	v_pair_odr	;//ADD_RAM_CAB_ST_B	+	101	;	//根据何一杰测试结果，针对US1119PAIRS，
	//海拔每升高1000m，采样值降低15.2，近似取16；					;	
extern	byte	v_pair_mode	;//ADD_RAM_CAB_ST_B	+	102	;	
extern	byte	v_pair_co	;//ADD_RAM_CAB_ST_B	+	103	;	//修正后的环境压力值?
extern	byte	v_pair_ee	;//ADD_RAM_CAB_ST_B	+	104	;	//最近一次读取的存储在EE中的PAIRS；
extern	byte	v_pair_ad_input	;//ADD_RAM_CAB_ST_B	+	105	;	//PAIRS,A/D采集的直接输入值，需要经过诊断之后才可用；
						;	
extern	byte	v_pair_ad_input_min	;//ADD_RAM_CAB_ST_B	+	107	;	//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最小值
extern	byte	v_pair_ad_input_max	;//ADD_RAM_CAB_ST_B	+	108	;	//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最大值
extern	byte	v_vbat_maxwhenpair	;//ADD_RAM_CAB_ST_B	+	109	;	//v_vbat_maxwhenpair=MAX_VBAT
extern	byte	v_vbat_minwhenpair	;//ADD_RAM_CAB_ST_B	+	110	;	
extern	byte	v_pair_max_afterkeyon	;//ADD_RAM_CAB_ST_B	+	111	;	
extern	byte	v_pair_min_afterkeyon	;//ADD_RAM_CAB_ST_B	+	112	;	
extern	byte	v_pair_aveinlast10min	;//ADD_RAM_CAB_ST_B	+	113	;	
extern	sbyte	v_KPT	;//ADD_RAM_CAB_ST_B	+	114	;	
extern	byte	v_Tmp_KPT	;//ADD_RAM_CAB_ST_B	+	115	;	//k,b标定时，对应的标准温度；
extern	sbyte	v_KPT_L	;//ADD_RAM_CAB_ST_B	+	116	;	//pairs的低温温度修正系数；v_KPT=(Xs1-Xs2)/(Ta1-Ta2)
extern	byte	v_pair_co_er	;//ADD_RAM_CAB_ST_B	+	117	;	
						;	
extern	byte	v_O2_ad	;//ADD_RAM_CAB_ST_B	+	120	;	//O2采样值
extern	byte	v_O2_last	;//ADD_RAM_CAB_ST_B	+	121	;	//上次的O2采样值
						;	
extern	byte	v_nb_OS_flu	;//ADD_RAM_CAB_ST_B	+	123	;	//os波动的次数；
extern	byte	v_nb_flu_lstIdle	;//ADD_RAM_CAB_ST_B	+	124	;	
extern	byte	v_O2_lambdaE1	;//ADD_RAM_CAB_ST_B	+	125	;	//lambda=1时对应的os采样值，即原来的目标值
extern	byte	v_O2flu_max_av	;//ADD_RAM_CAB_ST_B	+	126	;	//波动后最大值的平均值
extern	byte	v_O2flu_min_av	;//ADD_RAM_CAB_ST_B	+	127	;	//波动后最小值的平均值
extern	byte	v_O2flu_max_lst	;//ADD_RAM_CAB_ST_B	+	128	;	//
extern	byte	v_O2flu_min_lst	;//ADD_RAM_CAB_ST_B	+	129	;	//
extern	byte	v_O2_notworkLimit	;//ADD_RAM_CAB_ST_B	+	130	;	//os不工作对应的最大采样值
extern	byte	v_t_O2work_4s	;//ADD_RAM_CAB_ST_B	+	131	;	//氧传感器工作后，的时间，单位为4秒；
						;	
extern	byte	runs_noflu_nocalm	;//ADD_RAM_CAB_ST_B	+	132	;	//氧传感器没有波动的,且未被计入最值计算的次数；
						;	
extern	byte	v_O2_m_lst	;//ADD_RAM_CAB_ST_B	+	133	;	//最近波动时的极值,即最大或最小值
						;	
extern	byte	t_after_jci_s	;//ADD_RAM_CAB_ST_B	+	134	;	//段有后的时间
extern	byte	nb_osflu_whencf	;//ADD_RAM_CAB_ST_B	+	135	;	//断油后os仍然波动的次数
extern	byte	t_con_lean	;//ADD_RAM_CAB_ST_B	+	136	;	//os反馈后，出现持续偏西的时间
extern	word	t_osconlow_before_osflu_s	;//ADD_RAM_CAB_ST_B	+	137	;	//os波动前，持续低的时间，如果超过一定值，则os有问题；
						;	
extern	byte	ccl_os_conlean	;//ADD_RAM_CAB_ST_B	+	139	;	//os持续稀的圈数
extern	byte	ccl_os_conrich	;//ADD_RAM_CAB_ST_B	+	140	;	//os持续浓的圈数
extern	byte	nb_ccl_maybenotfast	;//ADD_RAM_CAB_ST_B	+	141	;	//
extern	byte	ccl_os_fk_limit	;//ADD_RAM_CAB_ST_B	+	142	;	//氧传感器响应时间，单位：圈数
						;	
extern	byte	nb_CI_afteroswork	;//ADD_RAM_CAB_ST_B	+	144	;	//os工作后恢复供油的次数
						;	
extern	byte	v_rev_156ave	;//ADD_RAM_CAB_ST_B	+	150	;	//v_rev_15d6rpm的平均值
extern	byte	v_rev_625rpm	;//ADD_RAM_CAB_ST_B	+	151	;	//转速rpm[量纲,v_rev_15d6rpm/4
extern	byte	v_rev_odr	;//ADD_RAM_CAB_ST_B	+	152	;	//转速级别[0-15]
extern	byte	v_rev_mode	;//ADD_RAM_CAB_ST_B	+	153	;	//转速级别余[0-31]
extern	byte	v_ystdc_search_t	;//ADD_RAM_CAB_ST_B	+	154	;	//在找到压缩TDC之前，对喷油相位的修正
extern	byte	v_rev_156ave_512	;//ADD_RAM_CAB_ST_B	+	155	;	//转速的1024次平均
						;	
extern	byte	v_vbat_bIdleTrig	;//ADD_RAM_CAB_ST_B	+	160	;	//怠速触发喷油前的电压采样值；
extern	byte	dcdy_adresult	;//ADD_RAM_CAB_ST_B	+	161	;	
extern	byte	v_Vbat_ad	;//ADD_RAM_CAB_ST_B	+	162	;	
extern	byte	v_Vbat_inuse	;//ADD_RAM_CAB_ST_B	+	163	;	//2008-6-10
						;	
/*STATUS*/						;	
extern	byte	v_t_eng_d1s	;//ADD_RAM_CAB_ST_B	+	170	;	//发动机连续运行时间,单位:0.1S
extern	byte	v_ccl_eng_byte	;//ADD_RAM_CAB_ST_B	+	171	;	//发动机连续运行的圈数；
//extern	byte	v_t_eng_d1s_afterfire	;//ADD_RAM_CAB_ST_B	+	172	;	//d1s_in_a_key,在ISCV控制中被用到；发动机从着火开始后连续运行的时间,单位:0.1S
extern	byte	v_500km	;//ADD_RAM_CAB_ST_B	+	173	;	//车辆运行了500km的倍数；
extern	sbyte	v_fmf_Grind_all	;//ADD_RAM_CAB_ST_B	+	174	;	//
extern	byte	cycle_numb_h	;//ADD_RAM_CAB_ST_B	+	175	;	//运行圈数
						;	
//2012-6-26						;	
extern	byte	v_d1s_TPDE0_to_2500rpm	;//ADD_RAM_CAB_ST_B	+	180	;	
extern	byte	v_d1s_TPDE0_to_4000rpm	;//ADD_RAM_CAB_ST_B	+	181	;	
						;	
//extern	byte	v_ccl_TPDE0_to_2500rpm	;//ADD_RAM_CAB_ST_B	+	182	;	
//extern	byte	v_ccl_TPDE0_to_4000rpm	;//ADD_RAM_CAB_ST_B	+	183	;	
						;	
extern	byte	v_Tmp_bymcu	;//ADD_RAM_CAB_ST_B	+	190	;	
extern	byte	v_vbat_whenkeyoff	;//ADD_RAM_CAB_ST_B	+	191	;	
						;	
/*system*/						;	
extern	byte	vc_tw3_t35	;//ADD_RAM_CAB_ST_B	+	200	;	

						;	
/*idle*/						;	
extern	byte	v_dmidle	;//ADD_RAM_CAB_ST_B	+	210	;	//怠速失火检测修正系数（对T1的修正）
extern	byte	vc_TW1_IDLEMISSF_sum	;//ADD_RAM_CAB_ST_B	+	211	;	//怠速失火检测修正系数的累加值
extern	byte	vc_TW1_IDLEMISSF_av	;//ADD_RAM_CAB_ST_B	+	212	;	//失火检测平均值
						;	
extern	byte	vc_TW1_IDLEMISSF_tmp	;//ADD_RAM_CAB_ST_B	+	214	;	//当失火过多时，对T1的临时修正
extern	byte	rpm_status	;//ADD_RAM_CAB_ST_B	+	215	;	//怠速失火检测是，相邻两圈之间RPM变化的状态；
						;	
/*=============================================================*/						;	
/*===word	or	sword型=====================================*/				;	
/*=============================================================*/						;	
//DTFI/0.004	;//	KESEI=18	#NAME?			;	
extern	sword	V_Tw	;//ADD_RAM_CAB_ST_W	+	2	;	//缸头温度，单位：度数
extern	sword	v_te	;//ADD_RAM_CAB_ST_W	+	4	;	//对应于v_teng_ad的温度值
						;	
//extern	sword	Tc	;//ADD_RAM_CAB_ST_W	+	6	;	//TC
extern	sword	v_u_M128	;//ADD_RAM_CAB_ST_W	+	8	;	//U
extern	word	Tw_brf_M16	;//ADD_RAM_CAB_ST_W	+	10	;	
						;	
extern	long int	v_mf_remain	;//ADD_RAM_CAB_ST_W	+	12	;	//油膜沉积项；
extern	long int	v_mf_evap	;//ADD_RAM_CAB_ST_W	+	16	;	//油膜蒸发项；
							
extern	word	v_mi	;//ADD_RAM_CAB_ST_W	+	20	;	//作为map的油量，考虑修正到标准状态，注意与v_FMF_frommap的区别
						;	
extern	word	v_ldb	;//ADD_RAM_CAB_ST_W	+	22	;	//从map上查找到的fmf
extern	word	v_msMu	;//ADD_RAM_CAB_ST_W	+	24	;	//冷机浪费的燃油；
extern	word	v_Film_preinj	;//ADD_RAM_CAB_ST_W	+	26	;	//预喷的油膜；
extern	word	v_ld_NotAltitude	;//ADD_RAM_CAB_ST_W	+	28	;	//用于计算油膜和启动油量的基础油量
extern	word	v_ld	;//ADD_RAM_CAB_ST_W	+	30	;	//作为map的油量，考虑修正到标准状态，注意与v_FMF_frommap的区别
						;	
extern	word	V_mf	;//ADD_RAM_CAB_ST_W	+	32	;	//油膜厚度，量纲与fmf同
extern	sword	v_dmfL	;//ADD_RAM_CAB_ST_W	+	34	;	//当圈的油膜增量
extern	sword	v_dmfS	;//ADD_RAM_CAB_ST_W	+	36	;	//当圈的油膜增量
						;	
extern	sword	v_dmf_SumL	;//ADD_RAM_CAB_ST_W	+	38	;	//总的油膜增量；
extern	sword	v_dmf_SumS	;//ADD_RAM_CAB_ST_W	+	40	;	//总的油膜增量；
extern	word	v_fmf_max	;//ADD_RAM_CAB_ST_W	+	42	;	//喷嘴单圈可喷射的最大流量
extern	sword	v_mf_rf	;//ADD_RAM_CAB_ST_W	+	44	;	//返流项
extern	sword	v_dld	;//ADD_RAM_CAB_ST_W	+	46	;	//load 的修正量
//extern	sword	v_mpa	;//ADD_RAM_CAB_ST_W	+	48	;	//海拔对负荷的修正
extern	sword	v_dmi	;//ADD_RAM_CAB_ST_W	+	50	;	//油量分配修正
extern	word	v_u_M128_Cold	;//ADD_RAM_CAB_ST_W	+	52	;	//油量分配修正
extern	word	t_Cal_1cycle	;//ADD_RAM_CAB_ST_W	+	56	;	//mcu计算一圈需要的时间；
extern	word	v_TW3_in	;//ADD_RAM_CAB_ST_W	+	58	;	//mcu从端口捕获的T3
extern	word	t_left_1cycle	;//ADD_RAM_CAB_ST_W	+	60	;	//一圈剩余的时间；
extern	word	t_left_1cycle_lst	;//ADD_RAM_CAB_ST_W	+	62	;	//上一圈剩余的时间；
extern  word  v_index_stack ;//ADD_RAM_CAB_ST_W	+	64;
extern  word  v_index_stack2 ;//ADD_RAM_CAB_ST_W	+	66;

/*							
2012-10-22							
	v_FMF_frommap	to 	v_ldb				
							
	vc_FMF_cls	to	v_dmLambda				
							
	v_dld	added					
							
	v_FMF_TransBasic	to	v_ld_NotAltitude				
							
	vc_FMF_StartWaste	to	v_msMu				
							
	v_ld	to	v_FMF_asmap				
							
	v_mpa	ADDED					
	v_dmi	ADDED					
	v_FMF_inj	to	v_mi				
	vc_TW1_IDLEMISSF		v_dmidle				
							
	v_Tw_AD_deg	to	v_te				
							
*/							
