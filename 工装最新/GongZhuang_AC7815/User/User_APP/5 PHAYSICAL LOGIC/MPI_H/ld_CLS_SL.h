
extern void FI_SLCLS_AfterReset(void);
extern void F_ActionSL_AfterKeyoff(void);
extern void F_SL_ByCLS(void) ;

extern void FL_SL_EE_rd(void);
extern void FL_SL_EE_wr(void);

extern void FC_SL_EE_afterIni(void);


extern sbyte v_SL_map[2][2];     //����ʹ�õ���ѧϰmap��ֵ��
extern byte v_SL_map_AD[2][2];     //����ʹ�õ���ѧϰmap��ֵ��

extern sbyte v_SL_map_wr[4];  //׼��д�ڵ�EE�еġ�ѧϰ����ֵ��
extern byte v_SL_add_WR[4];      //��Ҫд�����ѧϰ���ĸ���ĵ�ַ

extern byte v_flag_eewr;

extern byte v_add_sl;          //��ѭ����ѧϰ�ĵ㣻
extern sbyte v_SL_cls_lst;         //��һ�ε���ѧϰֵ�����ڹ�˳

extern sbyte v_map_SL_basic;    //��ѧϰMAP�Ļ���ֵ  

extern byte v_ccl_sl;          //��ǰ��ѧϰ��ѧϰ��Ȧ����
extern byte v_nb_SL  ;    // ��ѧϰ�Ĵ���

extern byte v_addr_for1st_eeRD;

extern sbyte v_cls_sl_learn;     //��ǰѧϰֵ��

extern byte v_OS_flu_SLpoint;    //���ٵ������жϵ�ʱ�䴰���ڣ�OS�����Ĵ���

extern sbyte v_fmf_sl_idle; //���ٵ���ѧϰֵ��

extern byte v_TPd_sl_rd;
extern byte v_TPmod_sl_rd;
extern byte v_Rev_sl_rd;
extern byte v_Revmod_sl_rd;

extern byte v_TPd_rd;
extern byte v_REV_rd;

extern byte nb_ccl_r;  //Ũ �Ĵ���
extern byte nb_ccl_l;  //ϡ �Ĵ���
extern byte v_u_sl;    //v_UP_M128
extern byte v_t_aftersl_s; //����ϡ����Ũ֮���ʱ��
extern byte v_os_flu_trans;//����ϡ����Ũ

extern word v_relate_sl_sum;//2013-5-9


extern byte v_add_sl_steady;      //��һѭ����ѧϰ�ĵ㣻

extern byte v_nb_wr_fail;  //д��ʧ�ܵĴ���

extern byte v_nb_flu_forSL;//��һ���ڼ��ڣ��������������Ĵ���
extern byte v_flag_chkwr;         //EE��Ҫд��ı�־����׼��д��ʱ��v_flag_chkwr

extern sbyte CheckSlLimit(word fmf,sword swl,byte sladd);

extern byte v_nb_wr_fail;  //д��ʧ�ܵĴ���

//extern sbyte v_SLtg_sum;      //2013-5-16  ѧϰĿ���ۼ�
extern sbyte v_LambdaTg_SL;//2013-5-16

extern byte v_DTC_SL;//2015-12-17 δ������ѧϰ������ԭ��

extern byte v_status_IDLESL;  //2019-1-20


/*
��Ҫ��ʾ����Ϣ��
    V_CLSSLH_STATUS
    V_CLSSL_STATUS
*/

//extern void FR_SL_test(void);
