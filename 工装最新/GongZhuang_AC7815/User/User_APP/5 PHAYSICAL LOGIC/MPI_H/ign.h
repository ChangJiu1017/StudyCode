/*===========================================================*/
/*====������ģ��=====================*/
/*===========================================================*/

//���ܣ��������ĳ�ʼ��
extern void FI_Ign_BeforeStarts(void);

//���ܣ���û������ǣ�v_CA_ignition
//    �������¶ȵ�����
//2008-9-25 add ,byte v_rev,byte v_rev_ave_512,byte remark
extern void FG_CA_Ign_basic(void);
//********************************
//���ٵ�������
//���ܣ��ڴ������֮ǰ���ж�����ǵ��٣���Ե��ǽ���������
//�βΣ� rev156--v_rev_15d6rpm;
//       tps_odr--v_TPS_odr;
extern void FC_Idle_Ign_BeforeTrig(void) ;//word rev156,byte tps_odr,byte tps_mode,sword tes,word rev3d9,byte rev156ave_16

//***************************************************************
//���ܣ����ݵ��Ǽ�����ʱ�̣���������v_tl_ignition
//����ֵ����Ӧ������ʱ���ģ������ڿ�ʼ���֮���ʱ�䳤��
//�βΣ� t_cas0 --v_t_cas0
//       prd_cash--v_prd_cash
//       prd_cas--v_prd_cas
extern word F_Cal_T_Ign(void);

extern void FI_Ign_afterReset(void);//2009-8-7 add 

extern void FC_IGN_Altitude(void);//byte pair_odr,byte pair_mode,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode

//----------------------------------------------------------------------------
//ign
extern word v_tl_ignition;  //���ݽǱ����ڼ���õ��ĵ��Ƕ�Ӧ��ʱ�䳤�ȣ����٣�4US      
extern word v_t_cas;//volatile v_t_casSTR _v_t_cas;             //������ʱ�̵�ʱ���׼���������ѡ��ǰ�ػ����	

extern sbyte v_CA_ignition;  //�����ǰ�ǣ����٣�0.7CA     								
extern sword ign_lst8run_sum;

extern word rev15d6_target;//ת��Ŀ��ֵ��2010-7-6
extern word v_rev15d6tg_nocorr;

extern sbyte co_IdleIgn_mod;   //2010-1-13

//2013-1-14
extern sbyte ign_lst8run_ave; //ign_lst8run_ave=0��ign_lst8run_sum=0��
extern byte dRev_forAirLess;               //�����ٱȱ�׼���ٵ��ڸ�ֵʱ����Ϊ������
extern sbyte ignMax_forAirLess;

extern sbyte v_igs;
extern byte v_M;
extern byte v_O;

extern byte v_n0_high;
extern byte v_n0_low;
extern byte v_J;
extern byte v_K;
extern sbyte inc_rev_backidle;


