/*HEAD
 ***********************************************************************
 * Filename	:dtc.h
 * Function	:�������ģ��ͷ�ļ�	
 * User		:yangsili
 * Date		:2017.12.30
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.30	yangsili
 *		1.������Ϳ�·��ϡ��ͱá�O2H��Ϻ�������
 *    2.������ǰ׼�����������������
 *
 ***********************************************************************
HEAD END*/

/*===========================================================*/
/*====�������ģ��=====================*/
/*===========================================================*/

#include "FAM.h"
#include "sys_definition.h"

//���ܣ�DTC�����ĳ�ʼ��
extern void FI_DTC_AfterReset(void);

//���ܣ����/���ù����벢ָʾ��
extern void F_DiagDisp(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy); //2008-8-1 add �β�

//���ܣ���������·���·��ϣ����õ�code_number,binjector������
extern void F_Diag_INJ(byte b_state);

void DTC_DiagINJOpen_def(void);//2017-12-29,ysl,add ���ܣ�������״̬ʵʱ��ȡ����Ҫ���ڿ�·����ˢ��

extern void F_Diag_IGN(byte b_state);//��ϵ���·

extern void DTC_DiagPump_def(byte b_state);//�ͱ���ϣ�2017-12-12,ysl,add

extern void DTC_DiagO2H_def(byte b_state);//��������ϣ�2017-12-12,ysl,add?

extern void DTC_Diag33814Reset_def(void);//33814��λ���,2017-12-28,ysl,add

extern void DTC_Diag_def(word prd_cas);  //�����������

extern void F_Diag_AirV(void);//2014-10-28
extern void F_Diag_CoolV(void);//2014-11-27

//���ܣ���Կ��ʱ�ۼӹ����뵽��ʷ���ϴ����У�
//      ��led_status_con>=8ʱ���ۼӹ��ϴ�������ͨ������sum_er_inc����EE����Ӧ�Ĺ�����ֵ�ۼӲ����棻
//      sum_er_inc�����������W,Ҳ����Wֵ�ǵ�ַ��
//      �ڼ�⵽Ҫ��Կ��ʱ�������ã�
extern void FM_SumError_WhenKeyOff(void);

//���ܣ�������Ͻ�����ù�����--CODE_NUMBER
extern void F_SumFailure(void);

//���ܣ�������Ͻ������ָʾ
extern void F_ShowMilFunction(void);

extern void F_Diag_CA(byte nb_hy_inqy,byte nb_qy_inhy,word t_cas,word t_cas_hy,word prd_cas,word prd_cas_hy);//2008-8-1 ��ϽǱ�
//�Ƿ���ʾ����ҳ
extern byte F_DispOtherPageOrNot(void);

extern byte FG_DispPage_ForIgn(void);

extern byte FG_DispCode_ForCos(void);

extern byte FG_DispCode_ForDtfiL(void);

extern byte FG_DispCode_ForTBack(void);

//extern void F_Diag_ISCV(void);//2008-8-5 ���

extern byte FG_DispCode_ForPN0Cos(void);//2008-11-14 

extern void Clr_DTC_History(void);

extern void F_Diag_INJ_atOFF(void); //2014-11-12
extern void F_Diag_INJ_atON(void);

//-----------public variables--------------------------------
//extern byte CODE_NUMBER;//�����룺������ʦ����ļ���
extern word v_flag_odd_qh_asc;
extern word v_flag_odd_qh_dec;
extern word v_flag_even_qh_asc;
extern word v_flag_even_qh_dec;
extern byte nb_odd_ystdc;
extern byte nb_even_ystdc;

extern byte nb_DTC;
extern byte nb_QYTUBIAN;//QIANYAN TUBIAN DE CISHU

//2009-12-11 add for  CA diag
extern byte nb_qhy_err;//ǰ����ʱ����������ۼƵĴ�����  VF_QHYCA_INTERR_IGN nb_qhy_err t_qhymax_p

extern byte nb_ign_open_er;         //
extern byte nb_ign_short_er;         //
extern byte nb_ign_detect;         //

extern byte v_rev62d5_hyUL;
extern byte nb_hy_lack;        //ȱ�ٺ��صĴ���

//extern void FS_CA_st(void); 2014-10-7
extern void FS_CA_st(sbyte ignCa);

//��ʱ���׼����ϲ���
extern unsigned long int t_acc_for10ms;
extern unsigned long int t_acc_forCAOV;
extern unsigned long int t_acc_StopAD;
extern unsigned long int t_acc_StopCom;
extern unsigned long int t_acc_MIL;

#define FCLEAR_CAOV       t_acc_forCAOV=0;
#define FCLEAR_ADATSTOP   t_acc_StopAD=0;
#define FCLEAR_COMATSTOP  t_acc_StopCom=0;
#define FCLEAR_MIL        t_acc_MIL=0;

extern void FI_TimeOutFlag(void);
extern void Shift_Status(void);
extern void Check_KeyStatus(void);



/*======DTC_ACT============================================================================================*/
//    typedef int8_t          sbyte ;
//    typedef int16_t         sword ;
//    typedef unsigned char   bool;
    


//��������Ĳ���
extern word v_t_bInj_toTrig;       //�Ĵ���ʱ�̵�����ʱ�̵�ʱ��
extern word v_t_bInj_toCas;        //�Ĵ����õĽǱ��ʱ�̵�����ʱ�̵�ʱ��
extern word v_t_bInj_toCas_tmp;        //�Ĵ����õĽǱ��ʱ�̵�����ʱ�̵�ʱ��

extern word v_t_T1;

extern word v_t_bInj_toTrig_r;    //ʵ�ʵĴ���ʱ�̵�����ʱ�̵�ʱ��
extern word v_t_bInj_toCas_r;     //ʵ�ʵĴ����õĽǱ��ʱ�̵�����ʱ�̵�ʱ��
extern word v_t_T1_r;

extern word v_t_Cas_trigT1;       //����T1�ĽǱ��ʱ��
extern word v_t_trigT1;           //����T1�Ƚ������ʱ��

//���ڵ��Ĳ���
extern word v_t_bIgn_toTrig;       //�Ĵ���ʱ�̵����ʱ�̵�ʱ��
extern word v_t_bIgn_toCas;        //�Ĵ����õĽǱ��ʱ�̵����ʱ�̵�ʱ��
extern word v_t_IgnOn;
extern word v_t_IgnOn_LookUp;		//20210602 cw add ���õ��ĳ��ʱ��


extern word v_t_bIgn_toTrig_r;    //ʱ��Ĵ���ʱ�̵����ʱ�̵�ʱ��
extern word v_t_bIgn_toCas_r;     //ʵ�ʵĴ����õĽǱ��ʱ�̵����ʱ�̵�ʱ��
extern word v_t_IgnOn_r;

extern word v_t_Cas_trigIgn;      //�������ĽǱ��ʱ��
extern word v_t_trigIgn;          //�������Ƚ������ʱ��
extern word v_t_IgnCharge_r;       // ʵ�ʵ�����ʱ��

extern word v_tl_IgnPreCharge;      //׼��������ʱ��
extern word v_tl_IgnPreCharge_r;      //׼��������ʵ��ʱ��

//����ִ�еĴ���
extern word v_nb_ign;             //�������Ĵ���
extern word v_nb_inj;             //�������͵Ĵ���
extern word v_nb_ignOn_r;           //��ʵ�ĵ��Ĵ���
extern word v_nb_injOn_r;           //��ʵ�����͵Ĵ���
extern word v_nb_ignOff_r;           //��ʵ�ĵ��Ĵ���
extern word v_nb_injOff_r;           //��ʵ�����͵Ĵ���

extern word v_nb_ccl_r;              //����������Ȧ��

extern word v_t_injOn_r;              //INJ_ON��ʱ��
extern word v_t_injOff_r;              //INJ_Off��ʱ��
extern word v_t_IgnSetOn_r;              //IGN_ON��ʱ��

extern sword v_dt_bInj_toCas;
extern sword v_dt_T1;
extern sword v_dt_bIgn_toCas;

extern sword v_dnb_Ign;
extern sword v_dnb_Inj;
extern sword v_dnb_T1;

extern sword v_dnb_Ign_YSTDC;
extern sword v_dnb_Inj_YSTDC;

extern word v_nb_YSTDC;
extern word v_nb_IgnInt;   //����жϵĴ���

extern void FI_ActDTC_afterReset(void);

extern byte nb_airv_open_er;         //2014-11-12
extern byte nb_airv_short_er;        

extern byte CheckIsSPNNG(byte id,byte DTC_TYPE);
extern void SetSpnStatus(byte blnSet,byte id,byte DTC_TYPE);
extern void ClearSpnStatus(byte DTC_TYPE);
extern dword rd_byAddGrp(byte nbbytes,word add_1st);

extern void Save_AD_cas(byte ad);//2016-2-28
extern byte v_AD_cas;//2016-2-28 cas����ֵ��8bit 2016-2-28

extern byte v_CA_break;


#define MV_PAIR 0
#define MV_TES 1
#define MV_TAS 2
#define MV_TPS 3


#define MV_L_LIMIT 0
#define MV_U_LIMIT 1
#define MV_NORMAL  2


extern void Accu_AD_Mv(byte mv_type,byte limit_type);//�ۼӸ�������ֵ����ֵ��2016-2-28

extern void DTC_DiagMIL_def(void);  //20210308 chenwei add MIL�����
//20210923 chenwei add
extern void MIL_ACTION_FORHUANSONG(word pcode);
extern void SAVE_PCODE_FOROBD4(word pcode);
extern void DTC_DiagNoCA_def(void);		//20211009 chenwei add

extern byte v_n_SPN_ng;
extern byte v_t_fromkeyon_1s;

