/*===========================================================*/
/*====O2�źŴ���ģ��=====================*/
/*===========================================================*/

//���ܣ���������O2�ɼ���ƽ���õ�O2ֵv_O2_ad�������ϴ�ֵ������v_O2_last��
//      ���ڷ���������ʱ��
//�β�; adres_10bit--��һ��10 AD�ɼ����
//      adres_10bit_2--�ڶ���10 AD�ɼ���� 
extern void FG_OS(word adres_10bit,word adres_10bit_2);      //��¼O2�ɼ���� 

//���ܣ�O2�����ĳ�ʼ��
extern void FI_OS_BeforStarts(void) ;

//���ܣ��ڷ�������ֹʱ��ֻ�ɼ�һ��O2��ֱ�ӷ���O2,����ʹ��ƽ��
extern void FG_OS_WhenStop(word adres_10bit);
extern void FI_OS_Reset(void);

extern void Detect_OS_Flu(byte tps_odr,byte rev_odr,long int t_eng_s,word prd_cas);

extern word ccl_os_fkfast;     //os �����ﵽ���ٵ�ˮƽ��Ȧ��
extern sword t_con_for_oswork;//
extern sword t_con_for_osflu;//
extern word v_O2_trend_DecOrNot;          //Os�Ĳ���ֵ�仯����,��˳��,���һλ�������һ��,λֵΪ0�����½�,λֵΪ1�����½�;
                            //2008-11-17
extern word v_O2_trend_IncOrNot;       //

extern byte v_nb_OS_AD_CD;

extern word v_t_OS_Lt20_to_now;//���һ��OS�źŵ���20�����ڵ�ʱ�䣻2019-1-20 PETER

//20211010 chenwei add ���������ȹ�������
extern byte v_t_O2H_delay_1s;			//ʹ�������ȵ��ӳ�ʱ�䣬���복������
extern byte v_o2h_n_625;					//�����ȿ�ʼ������ת����ֵ
extern byte v_o2h_tpd;					//�����ȿ�ʼ�����Ŀ�����ֵ
extern unsigned long int v_t_EngMove_s;
extern byte VF_O2H_STATUS;		//ָʾ�����ȵ�״̬
extern void F_O2H_FORHUANSONG(void);

