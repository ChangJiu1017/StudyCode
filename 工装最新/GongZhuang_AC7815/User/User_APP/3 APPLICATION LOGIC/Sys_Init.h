#define ACTION_WHENSTOP_PREINJ  0
#define ACTION_WHENSTOP_PUMP    1
#define ACTION_WHENSTOP_T1T3    2
#define ACTION_WHENSTOP_ALL     3

//2009-3-2��sys_init.h�еĶԳ�ʼ����Ķ��壻
#define NOZZLEINI_CODE_ING_STOP 9               //���ڶԶ���������г�ʼ��������
#define NOZZLEINI_CODE_TESHIGH_STOP 10          //���������ʼ�����¶����������㣻
//#define NOZZLEINI_CODE_//#define	VFF_NOZZLEINI_TESHIGH_DTC               _V_DTCHHL_STATUS.Bits.b0           //41  //2009-3-2   //�����ʼ��ʱ,��ͷ�¶ȹ���
#define NOZZLEINI_CODE_T1DGOT                  11 //#define	VFF_NOZZLEINI_T1DGOT_DTC                _V_DTCHHL_STATUS.Bits.b1//        //�������T1D
#define NOZZLEINI_CODE_FAILURE_BYNOINFLEXION   12 //#define	VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTC _V_DTCHHL_STATUS.Bits.b2//        //��û���ҵ�T4�յ������ɨ��T1Dʧ�ܣ�
#define NOZZLEINI_CODE_ABCGOT                  13 //#define	VFF_NOZZLEINI_ABCGOT_DTC                _V_DTCHHL_STATUS.Bits.b3//    //������˼���T3B�����ABCϵ����
#define NOZZLEINI_CODE_ABCSAVED                14 //#define VFF_NOZZLEINI_ABCSAVED_DTC              _V_DTCHHL_STATUS.Bits.b4//    //���ɹ�����ABC���Խ����
#define NOZZLEINI_CODE_NOTDOM                  15 //#define	VFF_NOZZLEINI_NOTDOM_DTC                _V_DTCHHL_STATUS.Bits.b5//    //��û��DOM
#define NOZZLEINI_CODE_DOMFAILURE              16 //#define	VFF_NOZZLEINI_DOMFAILURE_DTC            _V_DTCHHL_STATUS.Bits.b6//:DOM���������⣻
#define NOZZLEINI_CODE_NOTINI                  17 //#define	VFF_NOZZLEINI_NOTINI_DTC                _V_DTCHHL_STATUS.Bits.b7//:����û�г�ʼ����
#define NOZZLEINI_CODE_VBATNOTDOM              18 //#define	VFF_NOZZLEINI_VBATNOTDOM_DTC            _V_DTCHHH_STATUS.Bits.b0//:ECU��ѹû��dom
#define NOZZLEINI_CODE_VBATVALID               19 //#define	VFF_NOZZLEINI_VBATVALID_DTC             _V_DTCHHH_STATUS.Bits.b1//:��ѹ���͵����޷���ʼ����
#define NOZZLEINI_CODE_NOINJ                   20 //#define	VFF_NOZZLEINI_NOINJ_DTC                 _V_DTCHHH_STATUS.Bits.b2//:���������ϵ����޷���ʼ����
#define NOZZLEINI_CODE_T1DFAIL_T4LET1          21 //#define	VFF_NOZZLEINI_T1DFAIL_T4LET1_DTC        _V_DTCHHH_STATUS.Bits.b3//ɨ������T4<T1d+500����Ϊ���������
#define NOZZLEINI_CODE_ABCSCAN_RESULTINVALID   22 //#define	VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTC _V_DTCHHH_STATUS.Bits.b4//T1Dɨ��֮�󣬼���õ���ϵ���Ƿ���2009-3-5

#define IDLEINI_CODE_PNINI  8                //���������Ƿ��ʼ��(�൱����ǰ���������趨)//2009-4-7
#define IDLEINI_CODE_TES    7                //2009-4-16 TES�����а�����ů�����̣�
#define IDLEINI_CODE_TPS    4
#define IDLEINI_CODE_N      4
#define IDLEINI_CODE_I      5                //T3����
#define IDLEINI_CODE_VBAT   6
#define IDLEINI_CODE_OS             1
#define IDLEINI_CODE_OS_LEARNED     1
#define IDLEINI_AIRRICH     2
#define IDLEINI_AIRLESS     3

//2010-3-9
//У��GAMA�Ƿ�����Ч�ģ�
#define CHECK_GAMA_STATUS_NEWTO0    1 //У����GAMA��GAMA0(����ʼ��ʱ���Ե�GAMA)�Ƿ�����Ҫ��
#define CHECK_GAMA_STATUS_NEW       2 //У����GAMA�Ƿ�����Ҫ��
#define CHECK_GAMA_STATUS_NEWTOTMP  3 //У���Լ�ʱ����GAMA�Ĳ����Ƿ�����Ҫ��
#define CHECK_GAMA_STATUS_GAMA0     4


//NEET4���β�   2010-3-11 
#define NEEDT4_CLR_MILE     0 //�������Լ�ı�־
#define NEEDT4_NEED_MILE    1 //��Ҫ����Լ�ı�־
#define NEEDT4_MILE_AGAIN   2 //��Ҫ�ظ�����Լ�ı�־
#define NEEDT4_CLR_REMOVE   3//��������Լ�ı�־��
#define NEEDT4_NEED_REMOVE   4//��������Լ�ı�־��
#define NEEDT4_REMOVE_AGAIN   5//��������Լ�ı�־��



//2008-11-2 new
extern void FG_SysInit_AfterReset(void);//2013-4-2 delete t 
//extern void FR_SysInit_AfterKeyoff(unsigned long int t_eng_s_whenidle);
extern void FI_SysInit_BeforeStarts(void);//��

extern void F_FinishSysInitAction_whenStop(byte whatAction);          //2008-12-16 add

//���ܣ�����ƫ������������˵������ǵļ��㹦��
//  ע�⣺ƫ������:DTI_T3FK0���ڼ�������У�REMARK=6ʱ����λ����DTINJ_TV3_FK+vc_tw1_t3fk+DCDY_DT����ƽ��ֵ���õ���
//				Ȼ����λ������DTI_T3FK0��д������EE�ķ�ʽд�뵽��ַE_Dti_t3fk0_ADR��E_DTI_T3FK0_AB�У�
//				��ˣ�дƫ�������Ķ�����һ����ͨ��дEE�Ķ���������дƫ����1(Oֵ)�Ķ�����������
//  pckese, pcset, pcbtdc, pctback���ֱ�Ϊ����λ
//  tw1_idlesc����������ֵ��T1������ϵ��
//  tw3_sci����λ����T3�ĵ�������
//  tw3_offset��T3��ƫ��������ƫ����1��Oֵ��
extern void FS_ti_pc(byte pckese,byte pcset,byte pcbtdc,byte pctback,
                sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset);


//���ܣ������ʼ�����
//2008-10-30
//extern void FM_PNInit_Result(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte CLS_IDLE_av);
extern void FM_PNInit_Result(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte CLS_IDLE_av,sword t1_offset);

extern void Init_EE_SL(void);  //2014-10-4

extern void Save_DT1_whenKeyoff(sword t1_offset);//SCRר�ã�

extern sbyte vc_tw1_t3fk;           //ƫ����2����T1��ƫ����
extern sbyte vc_tw3offset;          //T3���ssave the value of	ti shift set by	pc��T3�����-���Եõ�//��Oֵ
extern byte nb_keyon_forIni;       // //2008-10-30 ADD
extern byte nb_keyon;              //2009-8-28
//extern void FC_SysInit_whenStop(unsigned long int t_eng_s_whenidle);
extern void FC_ti_set(void);//2009-7-31
extern void FS_PNINIT_FLAG(void);//2009-7-31
//2009-8-6 add
//�洢T1��T3�������
extern void FS_tw1_t3fk(byte b)  ;
//extern void Set_Miles_NeedT4Flag(byte NeedT4);
extern sbyte FS_AQOK_FLAG(sword idle_sc,sbyte fmf_cls,sbyte t3_offset,sbyte v_o,byte blnOk); //2010-7-8 add condiciton ok

extern void FC_KeyStatus(void);

//2014-3-29 add in sys_init.h
extern byte Load_Key(word adr); //2014-11-26 ������frombyte to word
extern void Save_Key(word adr,byte dat);
extern byte blnIdleIni;//2016-7-31 ���ӣ����ٳ�ʼ��������
