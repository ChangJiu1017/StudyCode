/*===========================================================*/
/*====ʱ�䴦��ģ��=====================*/
/*===========================================================*/

//���ܣ��ۼ�Ȧ����������Ȧ����ʶ��
extern void FG_CCL(void);

//���ܣ��ۼ�ʱ�䣬����ʱ���ʶ
//�βΣ�prd_cas����ǰ�ؽǱ�����
//  �ۼƷ�������תʱ�������ʱ��
//  �ۼ���������ʱ�䵽��ʱʱ�������TIME_TEMP,TIMEH,TIMEL+=v_prd_cas_h,v_prd_cas_l;��
//  ����ʱʱ������ۼƳ���0.1Sʱ���䳬������ת�Ƶ�D1SECOND��
//  ��D1SECOND���ֳ���1s�󣬽�����ת�Ƶ�SECONDH��SECONDL��SECOND�У�
//  ����SECONDH��SECONDL��SECOND�Ƿ������2,4,16�����ֱ��ñ�ʶ��
//  ���ڼ�������
//  time_flag,f1s����ÿ����1Sʱ��λ��û��ʱ��
//  v_t_eng_d1s:����ʱʱ������ۼƳ���0.1Sʱ,��һ�����޶���255
//  VF_MORE1MIN_TMCCL:��ʱ�䳬��60sʱ��λ
extern void FF_T(word prd_cas,byte blnIni);

//���ܣ�����ǰ��תȦ���ۼӵ�EE�У����ر�Կ��ʱ���ã�?
//  ����������תʱ��secondh��secondl,sum_second�ۼӵ�EE����Ӧ�ĵ�ַ��ee_run_timehl��ee_run_timeh��ee_run_time��
//  ע�⣬�洢֮��û�ж�secondh��secondl,sum_second�������㣬
//  �����ǣ����Ǳ�POWER_TIME���õģ�����ǰ�Ѿ���⵽Կ��Ҫ�رգ����֮�󷢶�����ͣ���ˣ�
//  ����ÿ�η�������������ʱ��Ҫ��secondh��secondl,sum_second�����������㣬
//  ����������תȦ��cycle_numb_hh��cycle_numb_hl��cycle_numb_h�ۼӵ�EE����Ӧ�ĵ�ַ��ee_cycle_h��ee_cycle_hl��ee_cycle_hh��
extern void FM_TimeCCL(void);//KEY_OFF

//���ܣ�Startsǰ��CCL_TIME�����ĳ�ʼ��������ʱ�䶼����
extern void FI_TimeCCL_BeforeStarts(void); //blnPor  //blnBeforeStarts //��⵽�����Ǳ�󣬶�ʱ������ĳ�ʼ��?


//���ܣ���������ֹͣʱ���ۼӱ��ο�Կ�׺��������ʱ��
extern void F_sum_T_ENG_WhenStopS(void);

extern void FI_TMCCL_AfterReset(byte blnForTimeReset);

extern void ResetMiles_AfterIni(void);

extern void FC_TimeOutFlag(void);    ///2014-10-4 add 

extern void F_Clr_SL1s(void); 

//----------------------------------------------------------------------------
extern unsigned long int v_t_EngMove_s;        //��������������ʱ��,��λ:S
extern word t_after1stStart_s;//��Կ�׺󣬵�һ��������ʼ��ʱ�������м�ͣ����
extern word v_ccl_131072;
extern unsigned long int v_t_eng_t_s;//2008-10-27 add

extern byte v_t_d1s_afterCI;       //�ٸ��ͺ��ʱ��   2013-4-22

//extern byte v_mile_limit;//2012-12-6

extern byte v_t_forlstrun_d1s;//��һ�����ŷǵ��ٵ�ʱ�䣻

extern byte v_t_eng_d1s;         //COUNT	0.1S CONTINOUESLY;ֻҪ����������ת��һֱ���ۼ�,���޶���255
                          //2012-12-21���Ӻ��壺����ͣ����ʱ�䣬ע����ʱ�ĵ�λ����

extern word v_t_eng_1s;//����������ά��һ��״̬��ʱ�䣬��������ͣ��������Կ���Ƿ�����

extern byte t_stop_s;//2015-12-11

extern void Clr_t1s_WhenNewState(void); 
//2013-7-16
extern word v_t_pairs_1s;

extern byte v_t_WOT_1s; //����ȫ����ʱ��  2013-8-30
extern byte v_t_NOTE0TPS_1s;//�����뿪���ٵ�ʱ�� 2013-8-30
extern byte v_t_afterbackidle_d1s;//���Żص��ٺ�3s���� 2017-3-7
extern byte v_t_E0_1s;

extern word v_nb_ccl_r;              //����������Ȧ��

extern byte t_SL_d1s;      //������ѧϰ��ʱ�䣬0.1s;
extern byte v_t_ci_d1s;

extern byte v_t_AfterInjON_1ms;//2014-10-8 ����������ʱ��
extern byte v_t_AfterIgnON_1ms;//2014-10-8 ���������ʱ��

extern byte v_t_afterstarts;

extern byte v_t_CA_ncome;

extern void F_Clr_PostInjOnTime(void); 
extern void F_Clr_PostIgnOnTime(void); 

//#define RUNS_FORPAIRS_LIMIT_A 700 //700S
#define RUNS_FORPAIRS_LIMIT_U 800 //700S
#define RUNS_FORPAIRS_LIMIT_L 500 //700S


#define MILE_FOR_LOCK 60
#define M0_MILE_NOLIMIT 250
//20210819 chenwei add for ���
extern byte nb_cc;
//20210923 chenwei add for MIL����
extern word v_t_MILGliter_d1s;
extern byte VF_MIL_SWITCH;
extern byte nb_mil_interval;
extern 	byte VF_MIL_DTC4;
extern 	byte VF_MIL_DTC3;
extern 	byte VF_MIL_DTC2;
extern 	byte VF_MIL_DTC1;
extern word v_pcode_lst;
extern byte nb_pcode;

extern byte v_pair_ad_input_keyon;		//��Կ��ѹ������洢
extern byte v_pair_ad_input_lst;
extern byte v_pair_ad_input_max_nmov_eng;	//δ�������ѹ�����������ֵ
extern byte v_pair_ad_input_min_nmov_eng;	//δ�������ѹ����������Сֵ

extern byte v_Vbat_inuse_keyon;		//��Կ�׵�ѹ�洢
extern byte v_Vbat_inuse_lst;
extern byte v_Vbat_inuse_max_nmov_eng;	//δ���������ѹ���������ֵ
extern byte v_Vbat_inuse_min_nmov_eng;	//δ���������ѹ��������Сֵ

extern byte VF_NOCA_DIAG;
extern byte nb_noca_test;		//�жϵ������޽Ǳ��źŵĴ���
extern byte v_t_fromkeyon_1s;

extern byte SWITCH_IGN;
extern byte switch_1;
