

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


extern word v_Im_M51;//T1截止时电流
extern word v_Is_M51;//基准点位置对应的电流

extern dword v_Et_M5000;//INT(Id*Ud)
extern dword v_int_sqI_M5000;//电流积分的
extern word v_Er_M5000;//int(Id^2*r)
extern word v_Ein_M5000;//L/2*Id^2
extern word v_Ef_M5000;//阻力引起的能量损耗的积分

extern word v_Ets_M5000;//基准点能量

extern word v_En_M5000; //单次的能量
extern word v_Ens_M5000;//v_Ens_M5000=v_En_M5000*v_ks_M256/256;标准状态下的能量
extern word v_En0_M5000;//标准状态下，出油点能量，标定得到；

extern word v_W_M5000;//输出的有效功

//总和，用于计算平均值之用
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
extern word v_Kq_MKQ; //E-Q斜率，由标定得到

extern byte v_L_M10000;//电感值

extern word v_t_CalI[5][2];//计算程序的运行时间

extern word v_Wn_tg;//有效功的目标

extern byte v_nb_IIndex_lst;//上一次的AD采集的下标

extern sword v_dT1_wn;//能量模型的dT1 2015-2-2
extern word v_Wn_tg_lst;
extern word v_Wn_r;
  

extern void FS_1inj(void);
extern void Fint_Energy_1pint(byte curPoint,byte blnLst,byte blnCalEnergy);

extern word FC_dTW1_byWnTg(byte index);//计算达到目标T1还需要增加的dT1值；

extern void Adj_Wn_tg(sbyte blnInc);
extern word Disp_dTW1_forWnTg(void);

extern word Disp_wn(void);

extern void Save_Wn_tg(void);      //2015-2-2

extern byte v_ModelType;//喷嘴模型的类型

#define MODEL_TYPE_T3 0
#define MODEL_TYPE_WN 1

#define WN_TG_INI 1000




extern word F_T1Nxt_Wn(word mi,byte vbat);
extern void Save_Wn_tg(void);

