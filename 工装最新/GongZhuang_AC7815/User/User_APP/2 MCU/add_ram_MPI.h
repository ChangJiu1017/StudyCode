

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

extern	byte	v_nb_notsyninj_add	;//ADD_RAM_CAB_ST_B	+	22	;	//����ѭ������Ĵ�����

							
/*================================================================*/							
//�����Ǳ궨�������б�							
//byte	or	sbyte��					
//COS ;// KESEI=18||KESEI=255 ,ADJUST BY COS UP/DOWN , "+"���������Ӧ����REMARK							
/*fuel*/							
extern	volatile byte	v_tair_ad	;//ADD_RAM_CAB_ST_B	+	2	;	//�����¶Ȳ���ֵ
extern	sbyte	V_Ta	;//ADD_RAM_CAB_ST_B	+	3	;	//�����¶ȣ���λ������
extern	byte	v_teng_ad	;//ADD_RAM_CAB_ST_B	+	4	;	//��ͷ�¶Ȳ���ֵ
						;	
extern	byte	v_G_M128	;//ADD_RAM_CAB_ST_B	+	5	;	//v_G_M128=M0_XZ_M16384
//extern	byte	v_XX	;//ADD_RAM_CAB_ST_B	+	6	;	//v_XX=M0_XX_M1024_M273
						;	
//extern	byte	vc_A_mf	;//ADD_RAM_CAB_ST_B	+	7	;	//��Ĥģ���е�A
extern	byte	vc_B_mf	;//ADD_RAM_CAB_ST_B	+	8	;	//��Ĥģ���е�B
extern	byte	vc_C_mf	;//ADD_RAM_CAB_ST_B	+	9	;	//��Ĥģ���е�C
extern	byte	vc_D_mf	;//ADD_RAM_CAB_ST_B	+	10	;	//��Ĥģ���е�D
extern	byte	vc_SD0_mf	;//ADD_RAM_CAB_ST_B	+	11	;	//��Ĥģ���е�SD0
						;	
extern	byte	v_M0_E_M8	;//ADD_RAM_CAB_ST_B	+	12	;	//����ģ���е�E
extern	byte	v_M0_F_M256	;//ADD_RAM_CAB_ST_B	+	13	;	//����ģ���е�F
						;	
extern	sbyte	vc_FMF_teng	;//ADD_RAM_CAB_ST_B	+	14	;	//�����ĸ�ͷ�¶�����ϵ��
//extern	byte	vc_FMF_tair	;//ADD_RAM_CAB_ST_B	+	15	;	//�����¶ȶ�����������ϵ��
						;	
extern	byte	v_P_DEC_M64	;//ADD_RAM_CAB_ST_B	+	16	;	//����ʱ��Ĥ�����ķ���ϵ��
extern	byte	v_P_ACC_M64	;//ADD_RAM_CAB_ST_B	+	17	;	//����ʱ��Ĥ�����ķ���ϵ��
extern	byte	v_L_M1024	;//ADD_RAM_CAB_ST_B	+	18	;	//��Ĥ����˥��ϵ��
extern	byte	v_S_M128	;//ADD_RAM_CAB_ST_B	+	19	;	//��Ĥ����˥��ϵ��
//extern	sbyte	v_M0_DUKK	;//ADD_RAM_CAB_ST_B	+	20	;	//����ʱ��С����
						;	
extern	byte	v_LAMBDA_byMAPS	;//ADD_RAM_CAB_ST_B	+	21	;	//������������ֵ


extern	sbyte	v_dXZ	;//ADD_RAM_CAB_ST_B	+	23	;	//v_G_M128=M0_XZ_M16384
extern	byte	v_T_M64	;//ADD_RAM_CAB_ST_B	+	24	;	//�뿪����ʱU
extern	byte	v_UR_M16	;//ADD_RAM_CAB_ST_B	+	25	;	//�ָ�����ʱU
extern	byte	v_UA_M128	;//ADD_RAM_CAB_ST_B	+	26	;	//����ʱU
extern	byte	v_Y_M256	;//ADD_RAM_CAB_ST_B	+	27	;	//������ʱU


extern	sbyte	v_UP_M128	;//ADD_RAM_CAB_ST_B	+	30	;	//ȫ����U
extern	sbyte	v_LambdaTg	;//ADD_RAM_CAB_ST_B	+	31	;	//ȫ����U������

/*ALTITUDE*/						;	
//extern	byte	v_kDt	;//ADD_RAM_CAB_ST_B	+	38	;	//���ι�������ϵ����
//extern	byte	v_Altitude_beta	;//ADD_RAM_CAB_ST_B	+	39	;	//beta*8
////extern	byte	v_Altitude_nta	;//ADD_RAM_CAB_ST_B	+	40	;	//2008-9-11
/*cls*/						;	
extern	sbyte	leansdtav	;//ADD_RAM_CAB_ST_B	+	41	;	//
							
extern	byte	nb_for_leantest	;//ADD_RAM_CAB_ST_B	+	43	;	
extern	sbyte	v_dmLambda	;//ADD_RAM_CAB_ST_B	+	44	;	//����O2�������������������ϵ��
//extern	sbyte	vc_CLS_IDLE_av	;//ADD_RAM_CAB_ST_B	+	45	;	//O2������ƽ��
						;	
/*cls	learned*/					;	
//extern	byte	nb_Lean_max	;//ADD_RAM_CAB_ST_B	+	46	;	//��һ���ڼ��ڣ�����ϡ�����Ȧ����
//extern	byte	nb_Rich_max	;//ADD_RAM_CAB_ST_B	+	47	;	//��һ���ڼ��ڣ�����Ũ�����Ȧ����
extern	sbyte	c_cls_max	;//ADD_RAM_CAB_ST_B	+	48	;	//��һ���ڼ��ڣ��ջ������������ֵ
extern	sbyte	c_cls_min	;//ADD_RAM_CAB_ST_B	+	49	;	//��һ���ڼ��ڣ��ջ�����������Сֵ��

extern	sbyte	v_cls_sl_cur	;//ADD_RAM_CAB_ST_B	+	57	;	//��ǰѧϰֵ��
extern	sbyte	v_SL_cls	;//ADD_RAM_CAB_ST_B	+	58	;	//�ǵ��ٵ���ѧϰֵ

							
/*DTC*/						;	
extern	byte	t_qhymax_p	;//ADD_RAM_CAB_ST_B	+	60	;	//ǰ�������ʱ������λ��
extern	byte	t_qhymin_p	;//ADD_RAM_CAB_ST_B	+	61	;	//ǰ������Сʱ������λ��
extern	byte	v_qhCAint_ave	;//ADD_RAM_CAB_ST_B	+	62	;	//v_qhCAint_to_prd_m255��ƽ��ֵ
extern	byte	v_qhCAint_to_prd_m255	;//ADD_RAM_CAB_ST_B	+	63	;	//2009-8-7
						;	
/*IGN*/						;	
extern	byte	inc_revobj_worm	;//ADD_RAM_CAB_ST_B	+	70	;	//���ǿ���Ŀ��ת�ٵ�������
extern	sbyte	co_IdleIgn_I	;//ADD_RAM_CAB_ST_B	+	71	;	//���ٵ��Ƿ���ֵ�Ļ��֣�0.7��CA
extern	sbyte	co_IdleIgn_byRPM	;//ADD_RAM_CAB_ST_B	+	72	;	//���ٵ��Ƿ���ֵ��0.7��CA
						;	
extern	sbyte	ign_lst8run_ave	;//ADD_RAM_CAB_ST_B	+	73	;	//�����Ȧ���ٵ��ǵ�ƽ����0.7��CA
extern	byte	nb_ign	;//ADD_RAM_CAB_ST_B	+	74	;	//������
extern	byte	nb_lose_hy	;//ADD_RAM_CAB_ST_B	+	75	;	//��ʧ���صĴ���
extern	byte	v_rev15d6_target_idle	;//ADD_RAM_CAB_ST_B	+	76	;	//����Ŀ��
						;	
extern	sbyte	v_CA_ignition_lst	;//ADD_RAM_CAB_ST_B	+	77	;	//�����ǰ�ǣ����٣�0.7CA
extern	sbyte	ign_step_mode	;//ADD_RAM_CAB_ST_B	+	78	;	//2010-1-11
extern	sbyte	v_CA_limit_byrpm	;//ADD_RAM_CAB_ST_B	+	79	;	//����ת�ټ���ĵ��ǵ���ֵ��0.7CA
extern	sbyte	v_Ign_Map	;//ADD_RAM_CAB_ST_B	+	80	;	//map�ĵ��ǣ����ڵ���ʱ��Ҫ���ǵ��������
extern	byte	dIgn_perRev_M16	;//ADD_RAM_CAB_ST_B	+	81	;	//����ת�ٶԵ��ǵķ���ǿ�ȣ�0.7CA/15.6rpm*16
extern	byte	m_nbtdc0	;//ADD_RAM_CAB_ST_B	+	82	;	//��𴥷��صļල��0.7CA���������ѡ��ǰ�ػ���أ�
						;	
extern	sbyte	v_Ign_I_notidle	;//ADD_RAM_CAB_ST_B	+	83	;	//�ǵ��ٵ����ֵ��
extern	sbyte	vc_Ign_Altitude	;//ADD_RAM_CAB_ST_B	+	84	;	//VC_IGN_ALTITUDE
						;	
extern	sbyte	v_IgnCA_act	;//ADD_RAM_CAB_ST_B	+	86	;	//ʵ�ʲ�õĵ��ǣ�0.7CA
extern	sbyte	v_IgnCA_act_lst	;//ADD_RAM_CAB_ST_B	+	87	;	//��һ��ʵ�ʲ�õĵ��ǣ�0.7CA
						;	
/*sensor*/						;	
//extern word nb_forpair;//2014-1-28 peter, delete 
extern	byte	v_pair_ad	;//ADD_RAM_CAB_ST_B	+	100	;	//pair��AD
extern	byte	v_pair_odr	;//ADD_RAM_CAB_ST_B	+	101	;	//���ݺ�һ�ܲ��Խ�������US1119PAIRS��
	//����ÿ����1000m������ֵ����15.2������ȡ16��					;	
extern	byte	v_pair_mode	;//ADD_RAM_CAB_ST_B	+	102	;	
extern	byte	v_pair_co	;//ADD_RAM_CAB_ST_B	+	103	;	//������Ļ���ѹ��ֵ?
extern	byte	v_pair_ee	;//ADD_RAM_CAB_ST_B	+	104	;	//���һ�ζ�ȡ�Ĵ洢��EE�е�PAIRS��
extern	byte	v_pair_ad_input	;//ADD_RAM_CAB_ST_B	+	105	;	//PAIRS,A/D�ɼ���ֱ������ֵ����Ҫ�������֮��ſ��ã�
						;	
extern	byte	v_pair_ad_input_min	;//ADD_RAM_CAB_ST_B	+	107	;	//��VF_PERIOD_PAIRS�ڼ��ڲɼ�����PAIRS����Сֵ
extern	byte	v_pair_ad_input_max	;//ADD_RAM_CAB_ST_B	+	108	;	//��VF_PERIOD_PAIRS�ڼ��ڲɼ�����PAIRS�����ֵ
extern	byte	v_vbat_maxwhenpair	;//ADD_RAM_CAB_ST_B	+	109	;	//v_vbat_maxwhenpair=MAX_VBAT
extern	byte	v_vbat_minwhenpair	;//ADD_RAM_CAB_ST_B	+	110	;	
extern	byte	v_pair_max_afterkeyon	;//ADD_RAM_CAB_ST_B	+	111	;	
extern	byte	v_pair_min_afterkeyon	;//ADD_RAM_CAB_ST_B	+	112	;	
extern	byte	v_pair_aveinlast10min	;//ADD_RAM_CAB_ST_B	+	113	;	
extern	sbyte	v_KPT	;//ADD_RAM_CAB_ST_B	+	114	;	
extern	byte	v_Tmp_KPT	;//ADD_RAM_CAB_ST_B	+	115	;	//k,b�궨ʱ����Ӧ�ı�׼�¶ȣ�
extern	sbyte	v_KPT_L	;//ADD_RAM_CAB_ST_B	+	116	;	//pairs�ĵ����¶�����ϵ����v_KPT=(Xs1-Xs2)/(Ta1-Ta2)
extern	byte	v_pair_co_er	;//ADD_RAM_CAB_ST_B	+	117	;	
						;	
extern	byte	v_O2_ad	;//ADD_RAM_CAB_ST_B	+	120	;	//O2����ֵ
extern	byte	v_O2_last	;//ADD_RAM_CAB_ST_B	+	121	;	//�ϴε�O2����ֵ
						;	
extern	byte	v_nb_OS_flu	;//ADD_RAM_CAB_ST_B	+	123	;	//os�����Ĵ�����
extern	byte	v_nb_flu_lstIdle	;//ADD_RAM_CAB_ST_B	+	124	;	
extern	byte	v_O2_lambdaE1	;//ADD_RAM_CAB_ST_B	+	125	;	//lambda=1ʱ��Ӧ��os����ֵ����ԭ����Ŀ��ֵ
extern	byte	v_O2flu_max_av	;//ADD_RAM_CAB_ST_B	+	126	;	//���������ֵ��ƽ��ֵ
extern	byte	v_O2flu_min_av	;//ADD_RAM_CAB_ST_B	+	127	;	//��������Сֵ��ƽ��ֵ
extern	byte	v_O2flu_max_lst	;//ADD_RAM_CAB_ST_B	+	128	;	//
extern	byte	v_O2flu_min_lst	;//ADD_RAM_CAB_ST_B	+	129	;	//
extern	byte	v_O2_notworkLimit	;//ADD_RAM_CAB_ST_B	+	130	;	//os��������Ӧ��������ֵ
extern	byte	v_t_O2work_4s	;//ADD_RAM_CAB_ST_B	+	131	;	//�������������󣬵�ʱ�䣬��λΪ4�룻
						;	
extern	byte	runs_noflu_nocalm	;//ADD_RAM_CAB_ST_B	+	132	;	//��������û�в�����,��δ��������ֵ����Ĵ�����
						;	
extern	byte	v_O2_m_lst	;//ADD_RAM_CAB_ST_B	+	133	;	//�������ʱ�ļ�ֵ,��������Сֵ
						;	
extern	byte	t_after_jci_s	;//ADD_RAM_CAB_ST_B	+	134	;	//���к��ʱ��
extern	byte	nb_osflu_whencf	;//ADD_RAM_CAB_ST_B	+	135	;	//���ͺ�os��Ȼ�����Ĵ���
extern	byte	t_con_lean	;//ADD_RAM_CAB_ST_B	+	136	;	//os�����󣬳��ֳ���ƫ����ʱ��
extern	word	t_osconlow_before_osflu_s	;//ADD_RAM_CAB_ST_B	+	137	;	//os����ǰ�������͵�ʱ�䣬�������һ��ֵ����os�����⣻
						;	
extern	byte	ccl_os_conlean	;//ADD_RAM_CAB_ST_B	+	139	;	//os����ϡ��Ȧ��
extern	byte	ccl_os_conrich	;//ADD_RAM_CAB_ST_B	+	140	;	//os����Ũ��Ȧ��
extern	byte	nb_ccl_maybenotfast	;//ADD_RAM_CAB_ST_B	+	141	;	//
extern	byte	ccl_os_fk_limit	;//ADD_RAM_CAB_ST_B	+	142	;	//����������Ӧʱ�䣬��λ��Ȧ��
						;	
extern	byte	nb_CI_afteroswork	;//ADD_RAM_CAB_ST_B	+	144	;	//os������ָ����͵Ĵ���
						;	
extern	byte	v_rev_156ave	;//ADD_RAM_CAB_ST_B	+	150	;	//v_rev_15d6rpm��ƽ��ֵ
extern	byte	v_rev_625rpm	;//ADD_RAM_CAB_ST_B	+	151	;	//ת��rpm[����,v_rev_15d6rpm/4
extern	byte	v_rev_odr	;//ADD_RAM_CAB_ST_B	+	152	;	//ת�ټ���[0-15]
extern	byte	v_rev_mode	;//ADD_RAM_CAB_ST_B	+	153	;	//ת�ټ�����[0-31]
extern	byte	v_ystdc_search_t	;//ADD_RAM_CAB_ST_B	+	154	;	//���ҵ�ѹ��TDC֮ǰ����������λ������
extern	byte	v_rev_156ave_512	;//ADD_RAM_CAB_ST_B	+	155	;	//ת�ٵ�1024��ƽ��
						;	
extern	byte	v_vbat_bIdleTrig	;//ADD_RAM_CAB_ST_B	+	160	;	//���ٴ�������ǰ�ĵ�ѹ����ֵ��
extern	byte	dcdy_adresult	;//ADD_RAM_CAB_ST_B	+	161	;	
extern	byte	v_Vbat_ad	;//ADD_RAM_CAB_ST_B	+	162	;	
extern	byte	v_Vbat_inuse	;//ADD_RAM_CAB_ST_B	+	163	;	//2008-6-10
						;	
/*STATUS*/						;	
extern	byte	v_t_eng_d1s	;//ADD_RAM_CAB_ST_B	+	170	;	//��������������ʱ��,��λ:0.1S
extern	byte	v_ccl_eng_byte	;//ADD_RAM_CAB_ST_B	+	171	;	//�������������е�Ȧ����
//extern	byte	v_t_eng_d1s_afterfire	;//ADD_RAM_CAB_ST_B	+	172	;	//d1s_in_a_key,��ISCV�����б��õ������������Ż�ʼ���������е�ʱ��,��λ:0.1S
extern	byte	v_500km	;//ADD_RAM_CAB_ST_B	+	173	;	//����������500km�ı�����
extern	sbyte	v_fmf_Grind_all	;//ADD_RAM_CAB_ST_B	+	174	;	//
extern	byte	cycle_numb_h	;//ADD_RAM_CAB_ST_B	+	175	;	//����Ȧ��
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
extern	byte	v_dmidle	;//ADD_RAM_CAB_ST_B	+	210	;	//����ʧ��������ϵ������T1��������
extern	byte	vc_TW1_IDLEMISSF_sum	;//ADD_RAM_CAB_ST_B	+	211	;	//����ʧ��������ϵ�����ۼ�ֵ
extern	byte	vc_TW1_IDLEMISSF_av	;//ADD_RAM_CAB_ST_B	+	212	;	//ʧ����ƽ��ֵ
						;	
extern	byte	vc_TW1_IDLEMISSF_tmp	;//ADD_RAM_CAB_ST_B	+	214	;	//��ʧ�����ʱ����T1����ʱ����
extern	byte	rpm_status	;//ADD_RAM_CAB_ST_B	+	215	;	//����ʧ�����ǣ�������Ȧ֮��RPM�仯��״̬��
						;	
/*=============================================================*/						;	
/*===word	or	sword��=====================================*/				;	
/*=============================================================*/						;	
//DTFI/0.004	;//	KESEI=18	#NAME?			;	
extern	sword	V_Tw	;//ADD_RAM_CAB_ST_W	+	2	;	//��ͷ�¶ȣ���λ������
extern	sword	v_te	;//ADD_RAM_CAB_ST_W	+	4	;	//��Ӧ��v_teng_ad���¶�ֵ
						;	
//extern	sword	Tc	;//ADD_RAM_CAB_ST_W	+	6	;	//TC
extern	sword	v_u_M128	;//ADD_RAM_CAB_ST_W	+	8	;	//U
extern	word	Tw_brf_M16	;//ADD_RAM_CAB_ST_W	+	10	;	
						;	
extern	long int	v_mf_remain	;//ADD_RAM_CAB_ST_W	+	12	;	//��Ĥ�����
extern	long int	v_mf_evap	;//ADD_RAM_CAB_ST_W	+	16	;	//��Ĥ�����
							
extern	word	v_mi	;//ADD_RAM_CAB_ST_W	+	20	;	//��Ϊmap��������������������׼״̬��ע����v_FMF_frommap������
						;	
extern	word	v_ldb	;//ADD_RAM_CAB_ST_W	+	22	;	//��map�ϲ��ҵ���fmf
extern	word	v_msMu	;//ADD_RAM_CAB_ST_W	+	24	;	//����˷ѵ�ȼ�ͣ�
extern	word	v_Film_preinj	;//ADD_RAM_CAB_ST_W	+	26	;	//Ԥ�����Ĥ��
extern	word	v_ld_NotAltitude	;//ADD_RAM_CAB_ST_W	+	28	;	//���ڼ�����Ĥ�����������Ļ�������
extern	word	v_ld	;//ADD_RAM_CAB_ST_W	+	30	;	//��Ϊmap��������������������׼״̬��ע����v_FMF_frommap������
						;	
extern	word	V_mf	;//ADD_RAM_CAB_ST_W	+	32	;	//��Ĥ��ȣ�������fmfͬ
extern	sword	v_dmfL	;//ADD_RAM_CAB_ST_W	+	34	;	//��Ȧ����Ĥ����
extern	sword	v_dmfS	;//ADD_RAM_CAB_ST_W	+	36	;	//��Ȧ����Ĥ����
						;	
extern	sword	v_dmf_SumL	;//ADD_RAM_CAB_ST_W	+	38	;	//�ܵ���Ĥ������
extern	sword	v_dmf_SumS	;//ADD_RAM_CAB_ST_W	+	40	;	//�ܵ���Ĥ������
extern	word	v_fmf_max	;//ADD_RAM_CAB_ST_W	+	42	;	//���쵥Ȧ��������������
extern	sword	v_mf_rf	;//ADD_RAM_CAB_ST_W	+	44	;	//������
extern	sword	v_dld	;//ADD_RAM_CAB_ST_W	+	46	;	//load ��������
//extern	sword	v_mpa	;//ADD_RAM_CAB_ST_W	+	48	;	//���ζԸ��ɵ�����
extern	sword	v_dmi	;//ADD_RAM_CAB_ST_W	+	50	;	//������������
extern	word	v_u_M128_Cold	;//ADD_RAM_CAB_ST_W	+	52	;	//������������
extern	word	t_Cal_1cycle	;//ADD_RAM_CAB_ST_W	+	56	;	//mcu����һȦ��Ҫ��ʱ�䣻
extern	word	v_TW3_in	;//ADD_RAM_CAB_ST_W	+	58	;	//mcu�Ӷ˿ڲ����T3
extern	word	t_left_1cycle	;//ADD_RAM_CAB_ST_W	+	60	;	//һȦʣ���ʱ�䣻
extern	word	t_left_1cycle_lst	;//ADD_RAM_CAB_ST_W	+	62	;	//��һȦʣ���ʱ�䣻
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
