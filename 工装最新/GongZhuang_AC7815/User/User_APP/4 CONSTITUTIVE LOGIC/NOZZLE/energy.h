

#define QTEST_ENERGY_ST   0
#define QTEST_ENERGY_ACCU 1
#define QTEST_ENERGY_ED   2

extern void F_StartQtest_ForEnergy(byte sta); 

extern void FC_Energy_1Inj(word index,word nb_point);
extern void Init_EnergyReset(void); 

extern byte Get_NxtDispPoint(void);
extern word Disp_t_I(void);
extern byte Disp_IM12D5_I(void);
extern byte Disp_VBAT_I(void);


extern word v_Im_M51;//T1��ֹʱ����
extern word v_Is_M51;//��׼��λ�ö�Ӧ�ĵ���

extern dword v_Et_M5000;//INT(Id*Ud)
extern dword v_int_sqI_M5000;//�������ֵ�
extern word v_Er_M5000;//int(Id^2*r)
extern word v_Ein_M5000;//L/2*Id^2
extern word v_Ef_M5000;//���������������ĵĻ���

extern word v_Ets_M5000;//��׼������

extern word v_En_M5000; //���ε�����
extern word v_Ens_M5000;//v_Ens_M5000=v_En_M5000*v_ks_M256/256;��׼״̬�µ�����
extern word v_En0_M5000;//��׼״̬�£����͵��������궨�õ���

extern word v_W_M5000;//�������Ч��

//�ܺͣ����ڼ���ƽ��ֵ֮��
extern dword v_Et_ave;
extern dword v_int_sqI_ave;
extern word v_Er_ave;
extern word v_Ein_ave;
extern word v_Ef_ave;

extern word v_En_ave;

extern word v_Ens_ave;
extern word v_W_ave;
extern word v_Is_ave;
extern word v_Im_ave;

extern word v_ks_M256;
extern word v_Kq_MKQ; //E-Qб�ʣ��ɱ궨�õ�

extern byte v_L_M10000;//���ֵ

extern word v_t_CalI[5][2];//������������ʱ��

extern word v_Wn_tg;//��Ч����Ŀ��

extern byte v_nb_IIndex_lst;//��һ�ε�AD�ɼ����±�

extern sword v_dT1_wn;//����ģ�͵�dT1 2015-2-2
extern word v_Wn_tg_lst;
extern word v_Wn_r;
  

extern void FS_1inj(void);
extern void Fint_Energy_1pint(byte curPoint,byte blnLst,byte blnCalEnergy);

extern word FC_dTW1_byWnTg(byte index);//����ﵽĿ��T1����Ҫ���ӵ�dT1ֵ��

extern void Adj_Wn_tg(sbyte blnInc);
extern word Disp_dTW1_forWnTg(void);

extern word Disp_wn(void);

extern void Save_Wn_tg(void);      //2015-2-2

extern byte v_ModelType;//����ģ�͵�����

#define MODEL_TYPE_T3 0
#define MODEL_TYPE_WN 1

#define WN_TG_INI 1000




extern word F_T1Nxt_Wn(word mi,byte vbat);
extern void Save_Wn_tg(void);

