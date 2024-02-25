//#include "IO_Map.h"
#include "hard_trig.h"
#define REG_BASE1 0x00003500


#ifndef ADD_V_CLS_STATUS

#define ADD_V_CLS_STATUS            (REG_BASE1 + 0x00000301UL)
#define ADD_V_CLSSL_STATUS          (REG_BASE1 + 0x00000300UL)
#define ADD_V_CLSSLH_STATUS         (REG_BASE1 + 0x00000302UL)
#define ADD_V_DTC_STATUS            (REG_BASE1 + 0x00000303UL)
#define ADD_V_DTCH_STATUS           (REG_BASE1 + 0x00000304UL)
#define ADD_V_DTCHL_STATUS          (REG_BASE1 + 0x00000305UL)
#define ADD_V_DTCHH_STATUS          (REG_BASE1 + 0x00000306UL)
#define ADD_V_EE_STATUS             (REG_BASE1 + 0x00000307UL)
#define ADD_V_IDLEAQ_STATUS         (REG_BASE1 + 0x00000308UL)
#define ADD_V_CFCI_STATUS           (REG_BASE1 + 0x00000309UL)  //9
#define ADD_V_IDLEMF_STATUS         (REG_BASE1 + 0x0000030aUL)
#define ADD_V_ISC_STATUS            (REG_BASE1 + 0x0000030bUL)
#define ADD_V_TRANS_STATUS          (REG_BASE1 + 0x0000030cUL)
#define ADD_V_TRANSH_STATUS         (REG_BASE1 + 0x0000030dUL)   //    13
#define ADD_V_HARD_STATUS           (REG_BASE1 + 0x0000030eUL)
#define ADD_V_IGN_STATUS            (REG_BASE1 + 0x0000030fUL)
#define ADD_V_INJCTL_STATUS         (REG_BASE1 + 0x00000310UL)
#define ADD_V_INJCTLH_STATUS        (REG_BASE1 + 0x00000311UL)    //81-80     17
#define ADD_V_NOTYSTDC_STATUS       (REG_BASE1 + 0x00000312UL)   //82-70=      DEC18
#define ADD_V_INJPHASE_STATUS       (REG_BASE1 + 0x00000313UL)   //19
#define ADD_V_CAS_STATUS            (REG_BASE1 + 0x00000314UL)
#define ADD_V_CASH_STATUS           (REG_BASE1 + 0x00000315UL)
#define ADD_V_INTINJ_STATUS         (REG_BASE1 + 0x00000316UL)
#define ADD_V_TMR_STATUS            (REG_BASE1 + 0x00000317UL)
#define ADD_V_INTIGN_STATUS         (REG_BASE1 + 0x00000318UL)
#define ADD_V_INTSCI_STATUS         (REG_BASE1 + 0x00000319UL)
#define ADD_V_MAPS_STATUS           (REG_BASE1 + 0x0000031aUL)
#define ADD_V_NOTIDLEMF_STATUS      (REG_BASE1 + 0x0000031bUL)
#define ADD_V_OS_STATUS             (REG_BASE1 + 0x0000031cUL)
#define ADD_V_OSDIAG_STATUS         (REG_BASE1 + 0x0000031dUL)  //141
#define ADD_V_OSDIAGH_STATUS        (REG_BASE1 + 0x0000031eUL)
#define ADD_V_OSDIAGHL_STATUS       (REG_BASE1 + 0x0000031fUL)
#define ADD_V_PAIR_STATUS           (REG_BASE1 + 0x00000320UL)
#define ADD_V_REV_STATUS            (REG_BASE1 + 0x00000321UL)
#define ADD_V_STARTS_STATUS         (REG_BASE1 + 0x00000322UL)
#define ADD_V_ENG_STATUS            (REG_BASE1 + 0x00000323UL)     //9C-70=  44
#define ADD_V_ENGH_STATUS           (REG_BASE1 + 0x00000324UL)     //45
#define ADD_V_ENGHL_STATUS          (REG_BASE1 + 0x00000325UL)  //9E-70=2E =46
#define ADD_V_SYS_STATUS            (REG_BASE1 + 0x00000326UL)
#define ADD_V_SYSINI_STATUS         (REG_BASE1 + 0x00000327UL)   // 160
#define ADD_V_SYSINIH_STATUS        (REG_BASE1 + 0x00000328UL)
#define ADD_V_SYSINIHL_STATUS       (REG_BASE1 + 0x00000329UL)
#define ADD_V_SYSINIHH_STATUS       (REG_BASE1 + 0x0000032aUL)
#define ADD_V_TMCCL_STATUS          (REG_BASE1 + 0x0000032bUL)
#define ADD_V_TMCCLH_STATUS         (REG_BASE1 + 0x0000032cUL)
#define ADD_V_TMP_STATUS            (REG_BASE1 + 0x0000032dUL)
#define ADD_V_TPS_STATUS            (REG_BASE1 + 0x0000032eUL)   //A7-70= DEC 55
#define ADD_V_VBAT_STATUS           (REG_BASE1 + 0x0000032fUL)
#define ADD_V_STOP_STATUS           (REG_BASE1 + 0x00000330UL)
#define ADD_V_DTCHHL_STATUS         (REG_BASE1 + 0x00000331UL)//2009-3-2 add 
#define ADD_V_DTCHHH_STATUS         (REG_BASE1 + 0x00000332UL)//2009-3-5 add 
#define ADD_V_TPSH_STATUS           (REG_BASE1 + 0x00000333UL)//2009-9-7
#define ADD_V_INJCTLHL_STATUS       (REG_BASE1 + 0x00000334UL)//2010-2-27
#define ADD_V_LOADSENSOR_STATUS     (REG_BASE1 + 0x00000335UL)//2012-12-27
#define ADD_V_ETC_STATUS            (REG_BASE1 + 0x00000336UL)//2012-12-28

#define ADD_V_LOCK_STATUS           (REG_BASE1 + 0x00000337UL)//2013-6-13
#define ADD_V_LOCK2_STATUS          (REG_BASE1 + 0x00000338UL)//2013-6-13

//2013-6-18
#define ADD_V_ETC3_STATUS (REG_BASE1 + 0x00000339UL)
#define ADD_V_ETC4_STATUS (REG_BASE1 + 0x0000033aUL)
#define ADD_V_I_CAP_STATUS (REG_BASE1 + 0x0000033bUL)

#define ADD_RAM_v_prd_cas  (REG_BASE1 + 0x0000033cUL)
#define ADD_RAM_v_t_cas0   (REG_BASE1 + 0x0000033eUL)
#define ADD_RAM_v_PCSET_SCI (REG_BASE1 + 0x00000340UL)

#define ADD_PRO2_F5_STATUS (REG_BASE1 + 0x00000342UL)
#define ADD_PRO1_F6_STATUS (REG_BASE1 + 0x00000343UL)

#define ADD_V_IGN2_STATUS            (REG_BASE1 + 0x00000344UL)
#define ADD_V_CASHL_STATUS           (REG_BASE1 + 0x00000345UL)
#define ADD_V_AD_STATUS              (REG_BASE1 + 0x00000346UL)//2014-4-28 ���ӵ�AD�ɼ���״̬

#define ADD_V_FLASH_STATUS           (REG_BASE1 + 0x00000347UL)//2014-5-17 ���ӵ�FLASH��״̬

#define ADD_V_SCR_STATUS             (REG_BASE1 + 0x00000348UL)
#define ADD_V_SCRPUMP_STATUS         (REG_BASE1 + 0x00000349UL)
#define ADD_V_SCRCOM_STATUS          (REG_BASE1 + 0x0000034aUL)
#define ADD_V_CAN_STATUS             (REG_BASE1 + 0x0000034bUL)//2014-8-12 ���ӵ�CANģʽת����״̬

#define ADD_V_AIRV_STATUS            (REG_BASE1 + 0x0000034cUL)//2014-10-28 ������ŷ�״̬
#define ADD_V_NOX_STATUS             (REG_BASE1 + 0x0000034dUL)//2014-11-1 nox������״̬
#define ADD_V_COOLV_STATUS           (REG_BASE1 + 0x0000034eUL)//2014-11-1 ��ȴˮ��״̬
#define ADD_V_ADB_STATUS             (REG_BASE1 + 0x0000034fUL)//2014-11-1 Һλ������״̬
#define ADD_V_TU_STATUS              (REG_BASE1 + 0x00000350UL)//2014-11-1 ����Һ������״̬
#define ADD_V_TE_INLET_STATUS        (REG_BASE1 + 0x00000351UL)//2014-11-1 ���������¶�
#define ADD_V_TE_OUTLET_STATUS       (REG_BASE1 + 0x00000352UL)//2014-11-1 ���������¶�

#define ADD_V_SCRDTC_STATUS           (REG_BASE1 + 0x00000353UL)//2014-11-3 SCR ACU���
#define ADD_V_SCRDTC2_STATUS          (REG_BASE1 + 0x00000354UL)//2014-11-3 SCR ACU���
#define ADD_V_SCRDTC3_STATUS          (REG_BASE1 + 0x00000355UL)//2014-11-3 SCR ACU���
#define ADD_V_SCRDTC4_STATUS          (REG_BASE1 + 0x00000356UL)//2014-11-10 SCR ͨѶ���
#define ADD_V_SCRDTC5_STATUS          (REG_BASE1 + 0x00000357UL)//2014-12-7 SCR ͨѶ���

//#define ADD_ENERGY_STATUS             (REG_BASE1 + 0x00000360UL)//2014-2-2����ģ���еı�־
#define ADD_V_SPN0_STATUS             (REG_BASE1 + 0x00000358UL)//2015-6-4 SPN����
#define ADD_V_SPN1_STATUS             (REG_BASE1 + 0x00000359UL)//2015-6-4 SPN����
#define ADD_V_SPN2_STATUS             (REG_BASE1 + 0x0000035aUL)//2015-6-4 SPN����
#define ADD_V_SPN3_STATUS             (REG_BASE1 + 0x0000035bUL)//2015-6-4 SPN����
#define ADD_V_SPN4_STATUS             (REG_BASE1 + 0x0000035cUL)//2015-6-4 SPN����
#define ADD_V_SPN5_STATUS             (REG_BASE1 + 0x0000035dUL)//2015-6-4 SPN����

#define ADD_V_DTCCTL_STATUS           (REG_BASE1 + 0x0000035eUL)//2015-6-10 DTC�Ĵ������

#define ADD_V_PUMPCTL_STATUS          (REG_BASE1 + 0x0000035fUL)//2015-6-11 �ÿ���ĥ��

//ϵͳ״̬ʹ�ܿ���
#define ADD_V_SYSENABLE0_STATUS       (REG_BASE1 + 0x00000360UL)//2015-6-28 
#define ADD_V_MASTER_STATUS           (REG_BASE1 + 0x00000361UL)//2015-7-2 
#define ADD_CONDICTION_STATUS         (REG_BASE1 + 0x00000362UL)//2015-7-6 һЩ���������� 
#define ADD_PUMP2_STATUS              (REG_BASE1 + 0x00000363UL)//2015-7-7 ������ 
#define ADD_CAL_STATUS                (REG_BASE1 + 0x00000364UL)//2015-7-7 �궨״̬
#define ADD_DATAVIEW_STATUS           (REG_BASE1 + 0x00000365UL)//2015-7-12 DATAVIEW
#define ADD_PN_STATUS                 (REG_BASE1 + 0x00000366UL)//2015-7-16 ��������״̬����·
                                       //ע����������飬��Ҫռ����BYTE
#define ADD_PN2_STATUS                 (REG_BASE1 + 0x00000368UL)//2015-7-17 ��������״̬����·
                                       //ע����������飬��Ҫռ����BYTE
#define ADD_PN3_STATUS                 (REG_BASE1 + 0x0000036AUL)//2015-7-17 ��������״̬����·
                                       //ע����������飬��Ҫռ����BYTE

#define ADD_V_SPN6_STATUS             (REG_BASE1 + 0x0000036cUL)//2015-10-15 SPN����
#define ADD_V_SPN7_STATUS             (REG_BASE1 + 0x0000036dUL)//2015-10-15 SPN����

/*---2017-12-9 ysl add FOR PI-EAEA0122------------*/
#define ADD_V_33814_STATUS            (REG_BASE1 + 0x0000036eUL)
#define ADD_V_33814INJ1CTL_STATUS     (REG_BASE1 + 0x0000036fUL)
#define ADD_V_33814O2HCTL_STATUS      (REG_BASE1 + 0x00000370UL)
#define ADD_V_33814PUMPCTL_STATUS     (REG_BASE1 + 0x00000371UL)

#define ADD_V_IACV_STATUS             (REG_BASE1 + 0x00000372UL)//2017-3-13 for IACV
#define ADD_V_IACV_STATUS2             (REG_BASE1 + 0x00000373UL)//2017-3-13 for IACV
#define ADD_V_FUELCOMP_STATUS             (REG_BASE1 + 0x00000374UL)

//20201109 chenwei add ŷ�������������
#define ADD_V_OSEUROVDIAG_STATUS             (REG_BASE1 + 0x00000376UL)

#define ADD_V_ENGPRTECTION_STATUS             (REG_BASE1 + 0x00000377UL)    //20210112 chenwei add �������������

#define ADD_V_O2H_STATUS             (REG_BASE1 + 0x00000378UL)    //20211014 chenwei add ���������ȹ���



#define false 0                                              
#define true 1
 /******************************************************************/
 typedef union {                             //byte                            
  byte Byte;
  struct {
    byte b0  :1;                                       
    byte b1  :1;                                       
    byte b2  :1;                                       
    byte b3  :1;                                       
    byte b4  :1;                                       
    byte b5  :1;                                       
    byte b6  :1;                                       
    byte b7  :1;                                       
  } Bits;
  //2015-7-16 ����PN����·
  struct {
    byte Q_DT3  :1;   //Q-DT3�ѱ궨                                    
    byte FRE  :1;     //Ƶ���ѱ궨                                  
    byte T3_ST  :1;   //�ѻ��T3�궨ֵ                                    
    byte T1M  :1;     //T1M�ѱ궨                                  
    byte VB_OK  :1;      //�궨��ѹȷ��OK                                 
    byte T1M_CUR  :1; //���ε�T1M�ѻ��                                       
    byte b6  :1;                                       
    byte ALL  :1;                                       
  } CALED;
  
  struct {
    byte T3_FORAVE_GOT  :1;  //�������ƽ����T3                                     
    byte NOLIQUIDCHKING   :1;//�ж��Ƿ���Һ��                                       
    byte ISNOLIQUID   :1;                                       
    byte ISJAM   :1;                                       
    byte DEICING   :1;                                       
    byte DEICE_FAIL   :1;//
    byte T3_FORT1SCAN  :1;                                       
    byte T3_FORNOLIQUIDT1  :1; //T3�Ƕ�ӦNOLIQUID T1��                                      
  } HARD;

  struct {
    byte  IS_T1_FOR_T30 :1;  //�������ƽ����T3                                     
    byte  IS_T1_FOR_PUMP  :1;//���ڿձõ�T1                                       
    byte  IS_T1_ON  :1;      //�����Ѵ�                                 
    byte  NOLIQUIDCHK_CON  :1;                                       
    byte  DEICING_CON:1;                                       
    byte  JAM_CON:1;//
    byte  IS_DEICED :1; //2015-7-27���ڱ�                                      
    byte   :1; //T3�Ƕ�ӦNOLIQUID T1��                                      
  } HARD1;

} V_STATUS_STR;
////////////////////////////////////////////////////////////////////////////////

typedef union {
  word Word;
   
  struct {
    
    union {
      byte Byte;
    } v_prd_caslSTR;
    #define v_prd_casl                _v_prd_cas.Overlap_STR.v_prd_caslSTR.Byte
    
    union {
      byte Byte;
    } v_prd_cashSTR;
    #define v_prd_cash                _v_prd_cas.Overlap_STR.v_prd_cashSTR.Byte
    
  } Overlap_STR;

} v_prd_casSTR;
extern volatile v_prd_casSTR _v_prd_cas;// @ADD_RAM_v_prd_cas;    

#define v_prd_cas                     _v_prd_cas.Word

typedef union {
  word Word;
   
  struct {
    
    union {
      byte Byte;
    } v_t_cas0lSTR;
    #define v_t_cas0l                 _v_t_cas0.Overlap_STR.v_t_cas0lSTR.Byte
    
    union {
      byte Byte;
    } v_t_cas0hSTR;
    #define v_t_cas0h                 _v_t_cas0.Overlap_STR.v_t_cas0hSTR.Byte
    
  } Overlap_STR;

} v_t_cas0STR;
extern volatile v_t_cas0STR _v_t_cas0;// @ADD_RAM_v_t_cas0;
#define v_t_cas0                      _v_t_cas0.Word
//////////////////////////////////////////////////////////////////
//����Ǻ�SCIͨ����صĶ���
typedef union {
  word Word;
   
  struct {
    
    union {
      byte Byte;
    } v_PCSET_SCILSTR;
    #define v_PCSET_SCIL               _v_PCSET_SCI.Overlap_STR.v_PCSET_SCILSTR.Byte
    
    union {
      byte Byte;
    } v_PCSET_SCIHSTR;
    #define v_PCSET_SCIH               _v_PCSET_SCI.Overlap_STR.v_PCSET_SCIHSTR.Byte
    
  } Overlap_STR;

} v_PCSET_SCISTR;
extern volatile v_PCSET_SCISTR _v_PCSET_SCI;// @ADD_RAM_v_PCSET_SCI;//(REG_BASE + 0x00000342UL);
#define v_PCSET_SCI                    _v_PCSET_SCI.Word 

extern volatile byte VF_PUMPFROZEN_SCRbak;//_V_SCRPUMP_STATUS.Bits.b0      //�ñ�����
extern volatile byte VF_MIXERPURGED_SCRbak;//_V_SCRPUMP_STATUS.Bits.b2      //���ǻ��ɨ
extern volatile byte VF_JMODECHANGE_PUMPbak;//_V_SCRPUMP_STATUS.Bits.b3 //2014-8-26 //2014-11-4����
extern volatile byte VF_T4_MAYBEEXIST_PUMPbak;//    _V_SCRPUMP_STATUS.Bits.b6      //�Ƿ���T4����PN�Ĺ���״̬��أ������־λ��ʾ�������£�Ӧ����T4
extern volatile byte VF_T_STANDARD_GOT_SCRbak;//    _V_SCRPUMP_STATUS.Bits.b7      //2015-5-20 T�ı�׼ֵ�Ƿ�õ�
extern volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b0//�Ѿ����TW���� ,������EEPROM�У�2015-3-26
extern volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b1//�Ѿ����TW����
extern volatile byte VF_IS_DOSED_BUTNOTPURGE_SCRbak;//_V_SCR_STATUS.Bits.b2//�Ѿ��й�����
extern volatile byte VF_ISINJECTING_WHENDOSING_SCRbak;//  _V_SCR_STATUS.Bits.b3      //����DOSING��������
extern volatile byte VF_INJ_SHIFT_SCRbak;//_V_SCR_STATUS.Bits.b4	  //����ת����־λ
extern volatile byte VF_T1_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b5    //2015-2-5 ���ڸ�T1��ֵ����ʱ���ܴ���ִ��T1�Ķ���������õ���T1���ܲ��ԣ�
extern volatile byte VF_PRD_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b6    //2015-2-5 ���ڸ����ڸ�ֵ����ʱ���ܴ���ִ�����ڵĶ���������õ������ڿ��ܲ��ԣ�
extern volatile byte VF_STOPDOSING_BYDTCbak;//_SCR_V_SCR_STATUS.Bits.b7    //2015-5-16,���й��ϣ�ֹͣ����
extern volatile byte VF_AMODEADJUST_SCIbak;//_V_SCRCOM_STATUS.Bits.b0
extern volatile byte VF_CAB_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b1    //2015-3-21 �����궨
extern volatile byte VF_CAB_QDT3_WITOUTHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b2    //2015-3-21 �������궨
extern volatile byte VF_AIRASSIST_MODbak;//_V_SCRCOM_STATUS.Bits.b3    // 2015-3-21���ӣ��Ƿ�������ģʽ�ı�ʶ
extern volatile byte VF_CAB_CONSTV_CABbak;//_V_SCRCOM_STATUS.Bits.b4    //2015-6-11 ����ģʽ�ı궨
extern volatile byte VF_CHK_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b7    //2015-6-11 �����궨�����֤
extern volatile byte VF_TOBEON_AIRVbak;//_V_AIRV_STATUS.Bits.b0             //����������ŷ�Ҫ����
extern volatile byte VF_ISOPEN_AIRVbak;//_V_AIRV_STATUS.Bits.b1             //
extern volatile byte VF_ISSHORT_AIRVbak;//_V_AIRV_STATUS.Bits.b2             //
extern volatile byte VF_VALVEOFF_FOR_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b3   //2015-3-26 ����OBDʱ���ط��Ķ���
extern volatile byte VF_VALVEONTOOFF_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b4   //2015-4-15 ��Ҫ��¼�������ݣ��Ƿ��ص������Ĺ��̣����Ƿ��������صĹ���
extern volatile byte VF_OPEN2_INJCTLbak;//      _V_AIRV_STATUS.Bits.b5
extern volatile byte VF_TOBEON_NOXbak;//_V_NOX_STATUS.Bits.b0             //��ʾNOX��Ҫ����
extern volatile byte VF_VOLTINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b1             //��ʾNOX��Ҫ����:NOX���ȴ�
extern volatile byte VF_TMPINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b2             //��ʾNOX��Ҫ������������ѹ�ڷ�Χ��
extern volatile byte VF_NOXVALID_NOXbak;//_V_NOX_STATUS.Bits.b3             //��ʾNOX��Ҫ����
extern volatile byte VF_O2VALID_NOXbak;//_V_NOX_STATUS.Bits.b4             //��ʾNOX��Ҫ����
extern volatile byte VF_HEATEROK_NOXbak;//_V_NOX_STATUS.Bits.b5             //��ʾNOX��Ҫ����
extern volatile byte VF_NOXOK_NOXbak;//_V_NOX_STATUS.Bits.b6             //��ʾNOX��Ҫ����
extern volatile byte VF_O2_NOXbak;//_V_NOX_STATUS.Bits.b7             //��ʾNOX��Ҫ����
extern volatile byte VF_TOBEON_COOLVbak;//_V_COOLV_STATUS.Bits.b0             //������ȴˮ��ŷ�Ҫ����
extern volatile byte VF_ISOPEN_COOLVbak;//_V_COOLV_STATUS.Bits.b1             //
extern volatile byte VF_ISSHORT_COOLVbak;//_V_COOLV_STATUS.Bits.b2             //
extern volatile byte VF_NOXON_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b3             // 2015-6-13  SALVEģʽ�·�NOX ON����
extern volatile byte VF_NOXOFF_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b4             // 2015-6-13  SLAVEģʽ�·�NOX OFF����
extern volatile byte VF_LOW_TSCRbak;//_V_COOLV_STATUS.Bits.b5             // 2015-8-27 TSCRƫ��
extern volatile byte VF_UB_TUbak;//_V_TU_STATUS.Bits.b0             //
extern volatile byte VF_LB_TUbak;//_V_TU_STATUS.Bits.b1             //
extern volatile byte VF_DECING_TUbak;//_V_TU_STATUS.Bits.b2             //
extern volatile byte VF_UB_VOLTbak;//_V_TU_STATUS.Bits.b3             // 2014-11-15
extern volatile byte VF_LB_VOLTbak;//_V_TU_STATUS.Bits.b4             // 2014-11-15
extern volatile byte VF_UERR_TUbak;//_V_TU_STATUS.Bits.b5             //
extern volatile byte VF_LERR_TUbak;//_V_TU_STATUS.Bits.b6             //

extern volatile byte VF_LB_ADBbak;//_V_ADB_STATUS.Bits.b1             //
extern volatile byte VF_LERR_ADBbak;//_V_ADB_STATUS.Bits.b2             //
extern volatile byte VF_UERR_ADBbak;//_V_ADB_STATUS.Bits.b3             //
extern volatile byte VF_LLB_ADBbak;//_V_ADB_STATUS.Bits.b4             //
extern volatile byte VF_LLLB_ADBbak;//_V_ADB_STATUS.Bits.b5             //
extern volatile byte VF_UL_ADB_Vbak;//_ADB_STATUS.Bits.b6             //

extern volatile byte VF_UB_TEINbak;//_V_TEIN_STATUS.Bits.b0             //
extern volatile byte VF_ERR_TEINbak;//_V_TEIN_STATUS.Bits.b1             //
extern volatile byte VF_ERR_TEINbak;//_V_TEIN_STATUS.Bits.b2             //
extern volatile byte VF_IS_TSCR_SET_TEINbak;//_V_TEIN_STATUS.Bits.b3             //�Ƿ�������TSCR,2015-8-11
extern volatile byte VF_UB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b4             //
extern volatile byte VF_LB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b5             //
extern volatile byte VF_ERR_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b6             //

extern volatile byte VF_UB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b0             //
extern volatile byte VF_LB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b1             //
extern volatile byte VF_ERR_TEOUTbak;//_V_TEOUT_STATUS.Bits.b2             //
extern volatile byte VF_UB_TAMB_INNERbak;//      _V_TEOUT_STATUS.Bits.b3             //
extern volatile byte VF_LB_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b4             //
extern volatile byte VF_ERR_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b5             //
extern volatile byte VF_NOTAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b6             //2015-8-27û������CAN���¶��ź�
extern volatile byte VF_NOPAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b7             //2015-8-27û������CAN�Ļ���ѹ���ź�

extern volatile byte VF_VOLT_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b0             //��
extern volatile byte VF_INI_ACU_SCRDTCbak;//    _V_SCRDTC_STATUS.Bits.b1             //�� //��δ�궨
extern volatile byte VF_INJECTOR_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b2             //��
extern volatile byte VF_PUMPCHOKE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b3             //��
extern volatile byte VF_ACU_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b4             //��
extern volatile byte VF_FROZEN_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b5             //��
extern volatile byte VF_AIRVALVE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b6             //������ŷ�
extern volatile byte VF_MIXPRESSURE_SCRDTCbak;//         _V_SCRDTC_STATUS.Bits.b7             //�ù�·�쳣

extern volatile byte VF_LB_ADB_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b0             //��
extern volatile byte VF_ERR_ADB_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b1             //��
extern volatile byte VF_FROZEN_TU_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b2             //��
extern volatile byte VF_ERR_TU_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b3             //��
extern volatile byte VF_ERR_COOLV_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b4             //��
extern volatile byte VF_ERR_TEIN_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b5             //��
extern volatile byte VF_ERR_TEOUT_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b6             //��
extern volatile byte VF_NOXON_BUTNOVALUE_SCRDTCbak;// 	  _V_SCRDTC2_STATUS.Bits.b7             //��

extern volatile byte VF_EXH_U_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b0             //��
extern volatile byte VF_EXH_UU_SCRDTCbak;//     _V_SCRDTC3_STATUS.Bits.b1             //��
extern volatile byte VF_CATALYST_SCRDTCbak;//       _V_SCRDTC3_STATUS.Bits.b2             //��
extern volatile byte VF_HIGHALTITUDE_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b3             //2015-8-27 ADD HIGH ALTITUDE
extern volatile byte VF_TORQUELIMIT_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b4             //2015-8-28 ��Ť��־

extern volatile byte VF_NOLINK_FROMECU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b7             //û������ECU������
extern volatile byte VF_NOLINK_FROMDCU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b6             //��
extern volatile byte VF_NOLINK_FROMACU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b5             //��
extern volatile byte VF_NOLINK_FROMNOX_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b4             //��
extern volatile byte VF_NOLINK_FROMDCU_MGPERS_SCRDTCbak;// 	            _V_SCRDTC4_STATUS.Bits.b3             //�� ,2015-1-27 ����mg/S������

extern volatile byte VF_NOLINK_FROMSCR1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b7             
extern volatile byte VF_NOLINK_FROMINCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b6             
extern volatile byte VF_NOLINK_FROMAMCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b5             
extern volatile byte VF_NOLINK_FROMEEC2_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b4             
extern volatile byte VF_NOLINK_FROMEEC1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b3             
extern volatile byte VF_NOLINK_FROMPT_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b2//2015-4-4             
extern volatile byte VF_NOLINK_N_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b1//2015-7-21  û��ת���ź�
extern volatile byte VF_NOLINK_T_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b0//2015-7-21  û��Ť���ź�

extern volatile byte VF_CALPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b0       //��������CAN���ϵģ��Ա궨���ݵ�����      
extern volatile byte VF_DTC_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b1             //ʹ�ܹ�����ķ���
extern volatile byte VF_PNPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b2             //ʹ������궨���ݵķ�����2015-7-21
extern volatile byte VF_DIAG_RQST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b3             //����Ƿ�������2015-8-29
extern volatile byte VF_TPCM_BAM_SENT_CANCTLbak;//_V_CANCTL_STATUS.Bits.b4             //TPCM_BAM��ͷ�Ѿ�������
extern volatile byte VF_FS_BS_CONTINUE_CANCTLbak;//_V_CANCTL_STATUS.Bits.b5             //2015-9-20 15765֡�����Ʒ�ʽ

extern volatile byte VF_ISCONSTV_NOTDT3_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b0       //�����Ƿ��Ƕ��ݿ���
extern volatile byte VF_ISFREREC_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b1       //�����Ƿ�Ƶ������
extern volatile byte VF_ISBACKPRE_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b2       //�����Ƿ�ѹ����
extern volatile byte VF_NOLIQUID_CHKING_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b3       //2015-6-15:�ÿ��ж�:������Ϊ��ʱ��T1-T3�Ƿ������Ϊ�жϱÿյ�����
extern volatile byte VF_INJ2_SHIFT_SCRbak;//_V_PUMPCTL_STATUS.Bits.b5       //2015-6-17 add for ch2 injection control
extern volatile byte VF_SYSINJ_CH2_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b6       //2015-6-29 CH2�Ĵ�Һ�������Ƿ����ͬ��ģʽ
extern volatile byte VF_IS_PRIME_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b7       //2015-7-4 ��Һ����Ƿ������ı�ʶ��

extern volatile byte VF_SLAVEOBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b0       //
extern volatile byte VF_MASTEROBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b1       //
extern volatile byte VF_T3OBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b2       //
extern volatile byte VF_GELANFU_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b3       //
extern volatile byte VF_CALPARA_AUTOOUT_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b4       //2015-7-30 �Զ�ʹ�ܱ궨���ݵķ���
extern volatile byte VF_NO_TRANS_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b5       //2015-9-29
extern volatile byte VF_NO_NH3_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b6       //

extern volatile byte VF_1NEWINJ_MASTERbak;//_V_MASTER_STATUS.Bits.b0          // ����MASTERģʽ����һ���µ�INJ����
extern volatile byte VF_1DC_CON_MASTERbak;//_V_MASTER_STATUS.Bits.b1          // ����MASTERģʽ������һ��DC������
extern volatile byte VF_1WUC_MASTERbak;//_V_MASTER_STATUS.Bits.b2          // ����MASTERģʽ������һ��WUC������
extern volatile byte VF_STH3_CON_MASTERbak;//  _V_MASTER_STATUS.Bits.b3          // ����MASTERģʽ������NH3STĿ��
extern volatile byte VF_JSTARTS_MASTERbak;//_V_MASTER_STATUS.Bits.b4          // ����MASTERģʽ����������������

extern volatile byte VF_PIPEOBD_CONbak;//      _V_CONDICTION_STATUS.Bits.b0  //����PIPEOBD������        
extern volatile byte VF_PRIMING_CONbak;//      _V_CONDICTION_STATUS.Bits.b1  //���� ������        
extern volatile byte VF_DOSING_CONbak;//    _V_CONDICTION_STATUS.Bits.b2  //�������������        
extern volatile byte VF_T1SCAN_CONbak;//    _V_CONDICTION_STATUS.Bits.b3  //�������������        
extern volatile byte VF_PURGING_CONbak;//      _V_CONDICTION_STATUS.Bits.b3  //�������������        

extern volatile byte VF_ISSETPARA_PUMP2bak;//        _V_PUMP2_STATUS.Bits.b0          //2015-7-19�����ò�����Ƶ�ʺ�T1�� 
extern volatile byte VF_ISENABLE_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b1//�Ƿ�ʹ��PUMP2
extern volatile byte VF_TOBESCAN_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b2//��Ҫɨ��
extern volatile byte F_L_DOSERNOLIQUID_PUMP2bak;//  	  _V_PUMP2_STATUS.Bits.b3//��Һ�õļ����ã��ڶ�γ��Ժ�����Һ��

extern volatile byte VF_MODE_OBDWORK_CALbak;//        _V_CAL_STATUS.Bits.b0  //OBD�ɹ�����MASTER��2015-7-18
extern volatile byte VF_NH3MODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b1  //2015-10-12
extern volatile byte VF_TRANSMODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b2  //2015-10-12

extern volatile byte VF_FLASHLINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b0          // 
extern volatile byte VF_LINKTOEE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b1          //1: TO EE, 0 : TO FLASH 
extern volatile byte VF_BY_BYTE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b2          //1: TO BY BYTE, 0 : BY WORD 
extern volatile byte VF_BY_1UNIT_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b3          //1: TO 1UNIT, 0 : 2 UNITS 
extern volatile byte VF_LINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b4          //1: ��DATAVIEW ���������ӣ�





/*-------------------STATUS FOR SCR CONTROL-------------------------------*/
//extern volatile V_STATUS_STR   _V_SCRPUMP_STATUS;//@ADD_V_SCRPUMP_STATUS;    //2014-8-15 SCR״̬����
//#define V_SCRPUMP_STATUS              V_SCRPUMP_STATUSbak//_V_SCRPUMP_STATUS.Byte
#define	VF_PUMPFROZEN_SCR	            VF_PUMPFROZEN_SCRbak//_V_SCRPUMP_STATUS.Bits.b0      //�ñ�����
#define	VF_MIXERPURGED_SCR	          VF_MIXERPURGED_SCRbak//_V_SCRPUMP_STATUS.Bits.b2      //���ǻ��ɨ
#define	VF_JMODECHANGE_PUMP	          VF_JMODECHANGE_PUMPbak//_V_SCRPUMP_STATUS.Bits.b3 //2014-8-26 //2014-11-4����
#define	VF_T4_MAYBEEXIST_PUMP	        VF_T4_MAYBEEXIST_PUMPbak//    _V_SCRPUMP_STATUS.Bits.b6      //�Ƿ���T4����PN�Ĺ���״̬��أ������־λ��ʾ�������£�Ӧ����T4
#define	VF_T_STANDARD_GOT_SCR	        VF_T_STANDARD_GOT_SCRbak//    _V_SCRPUMP_STATUS.Bits.b7      //2015-5-20 T�ı�׼ֵ�Ƿ�õ�

//extern volatile V_STATUS_STR   _V_SCR_STATUS;//@ADD_V_SCR_STATUS;    //2014-8-15 SCR״̬����
//#define V_SCR_STATUS           _V_SCR_STATUS.Byte

#define VF_IS_NORMAL_PNTW_EE_SCR 			VF_IS_NORMAL_PNTW_EE_SCRbak//_V_SCR_STATUS.Bits.b0//�Ѿ����TW���� ,������EEPROM�У�2015-3-26
#define VF_IS_NORMAL_PNTW_GOT_SCR 		VF_IS_NORMAL_PNTW_EE_SCRbak//_V_SCR_STATUS.Bits.b1//�Ѿ����TW����
#define VF_IS_DOSED_BUTNOTPURGE_SCR 	VF_IS_DOSED_BUTNOTPURGE_SCRbak//_V_SCR_STATUS.Bits.b2//�Ѿ��й�����
#define VF_ISINJECTING_WHENDOSING_SCR VF_ISINJECTING_WHENDOSING_SCRbak//  _V_SCR_STATUS.Bits.b3      //����DOSING��������
#define	VF_INJ_SHIFT_SCR	      			VF_INJ_SHIFT_SCRbak//_V_SCR_STATUS.Bits.b4	  //����ת����־λ
#define VF_T1_SETTING_SCR       			VF_T1_SETTING_SCRbak//_V_SCR_STATUS.Bits.b5    //2015-2-5 ���ڸ�T1��ֵ����ʱ���ܴ���ִ��T1�Ķ���������õ���T1���ܲ��ԣ�
#define VF_PRD_SETTING_SCR       			VF_PRD_SETTING_SCRbak//_V_SCR_STATUS.Bits.b6    //2015-2-5 ���ڸ����ڸ�ֵ����ʱ���ܴ���ִ�����ڵĶ���������õ������ڿ��ܲ��ԣ�
#define VF_STOPDOSING_BYDTC_SCR       VF_STOPDOSING_BYDTCbak//_SCR_V_SCR_STATUS.Bits.b7    //2015-5-16,���й��ϣ�ֹͣ����


//extern volatile V_STATUS_STR   _V_SCRCOM_STATUS;//@ADD_V_SCRCOM_STATUS;    //2014-8-15 SCR״̬����
//#define V_SCRCOM_STATUS           	_V_SCRCOM_STATUS.Byte
#define	VF_AMODEADJUST_SCI						VF_AMODEADJUST_SCIbak//_V_SCRCOM_STATUS.Bits.b0
#define	VF_CAB_QDT3_WITHAIR_CAB	      VF_CAB_QDT3_WITHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b1    //2015-3-21 �����궨
#define	VF_CAB_QDT3_WITOUTHAIR_CAB	  VF_CAB_QDT3_WITOUTHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b2    //2015-3-21 �������궨
#define	VF_AIRASSIST_MOD	          	VF_AIRASSIST_MODbak//_V_SCRCOM_STATUS.Bits.b3    // 2015-3-21���ӣ��Ƿ�������ģʽ�ı�ʶ
#define	VF_CAB_CONSTV_CAB	          	VF_CAB_CONSTV_CABbak//_V_SCRCOM_STATUS.Bits.b4    //2015-6-11 ����ģʽ�ı궨
#define	VF_CHK_QDT3_WITHAIR_CAB	      VF_CHK_QDT3_WITHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b7    //2015-6-11 �����궨�����֤


//extern volatile V_STATUS_STR    _V_AIRV_STATUS;//@ADD_V_AIRV_STATUS;    //2014-10-28 ������ŷ�״̬
//#define V_AIRV_STATUS           _V_AIRV_STATUS.Byte
#define	VF_TOBEON_AIRV    	    			VF_TOBEON_AIRVbak//_V_AIRV_STATUS.Bits.b0             //����������ŷ�Ҫ����
#define	VF_ISOPEN_AIRV    	    			VF_ISOPEN_AIRVbak//_V_AIRV_STATUS.Bits.b1             //
#define	VF_ISSHORT_AIRV    	    			VF_ISSHORT_AIRVbak//_V_AIRV_STATUS.Bits.b2             //
#define	VF_VALVEOFF_FOR_PNOBD_AIRV    VF_VALVEOFF_FOR_PNOBD_AIRVbak//_V_AIRV_STATUS.Bits.b3   //2015-3-26 ����OBDʱ���ط��Ķ���
#define	VF_VALVEONTOOFF_PNOBD_AIRV    VF_VALVEONTOOFF_PNOBD_AIRVbak//_V_AIRV_STATUS.Bits.b4   //2015-4-15 ��Ҫ��¼�������ݣ��Ƿ��ص������Ĺ��̣����Ƿ��������صĹ���
#define	VF_OPEN2_INJCTL	              VF_OPEN2_INJCTLbak//      _V_AIRV_STATUS.Bits.b5
#define	VF_SHORT2_INJCTL	            VF_OPEN2_INJCTLbak//      _V_AIRV_STATUS.Bits.b6


//extern volatile V_STATUS_STR    _V_NOX_STATUS;//@ADD_V_NOX_STATUS;    //2014-10-28 ������ŷ�״̬
//#define V_NOX_STATUS           _V_NOX_STATUS.Byte
#define	VF_TOBEON_NOX    	     		VF_TOBEON_NOXbak//_V_NOX_STATUS.Bits.b0             //��ʾNOX��Ҫ����
#define	VF_VOLTINRANGE_NOX    	  VF_VOLTINRANGE_NOXbak//_V_NOX_STATUS.Bits.b1             //��ʾNOX��Ҫ����:NOX���ȴ�
#define	VF_TMPINRANGE_NOX    	    VF_TMPINRANGE_NOXbak//_V_NOX_STATUS.Bits.b2             //��ʾNOX��Ҫ������������ѹ�ڷ�Χ��
#define	VF_NOXVALID_NOX    	      VF_NOXVALID_NOXbak//_V_NOX_STATUS.Bits.b3             //��ʾNOX��Ҫ����
#define	VF_O2VALID_NOX    	      VF_O2VALID_NOXbak//_V_NOX_STATUS.Bits.b4             //��ʾNOX��Ҫ����
#define	VF_HEATEROK_NOX    	      VF_HEATEROK_NOXbak//_V_NOX_STATUS.Bits.b5             //��ʾNOX��Ҫ����
#define	VF_NOXOK_NOX    	        VF_NOXOK_NOXbak//_V_NOX_STATUS.Bits.b6             //��ʾNOX��Ҫ����
#define	VF_O2_NOX    	            VF_O2_NOXbak//_V_NOX_STATUS.Bits.b7             //��ʾNOX��Ҫ����


//extern volatile V_STATUS_STR      _V_COOLV_STATUS;//@ADD_V_COOLV_STATUS;    //2014-10-28 ��ȴˮ��״̬
//#define V_COOLV_STATUS            _V_COOLV_STATUS.Byte
#define	VF_TOBEON_COOLV    	      VF_TOBEON_COOLVbak//_V_COOLV_STATUS.Bits.b0             //������ȴˮ��ŷ�Ҫ����
#define	VF_ISOPEN_COOLV    	      VF_ISOPEN_COOLVbak//_V_COOLV_STATUS.Bits.b1             //
#define	VF_ISSHORT_COOLV    	    VF_ISSHORT_COOLVbak//_V_COOLV_STATUS.Bits.b2             //
#define VF_NOXON_ATSLAVE_NOX      VF_NOXON_ATSLAVE_NOXbak//_V_COOLV_STATUS.Bits.b3             // 2015-6-13  SALVEģʽ�·�NOX ON����
#define VF_NOXOFF_ATSLAVE_NOX     VF_NOXOFF_ATSLAVE_NOXbak//_V_COOLV_STATUS.Bits.b4             // 2015-6-13  SLAVEģʽ�·�NOX OFF����
#define VF_LOW_TSCR               VF_LOW_TSCRbak//_V_COOLV_STATUS.Bits.b5             // 2015-8-27 TSCRƫ��

//extern volatile V_STATUS_STR    _V_TU_STATUS;//@ADD_V_TU_STATUS;    //2014-10-28 �����¶�״̬
//#define V_TU_STATUS           _V_TU_STATUS.Byte
#define	VF_UB_TU    	            VF_UB_TUbak//_V_TU_STATUS.Bits.b0             //
#define	VF_LB_TU    	            VF_LB_TUbak//_V_TU_STATUS.Bits.b1             //
#define	VF_DECING_TU    	        VF_DECING_TUbak//_V_TU_STATUS.Bits.b2             //

#define	VF_UB_VOLT    	          VF_UB_VOLTbak//_V_TU_STATUS.Bits.b3             // 2014-11-15
#define	VF_LB_VOLT    	          VF_LB_VOLTbak//_V_TU_STATUS.Bits.b4             // 2014-11-15

#define	VF_UERR_TU    	          VF_UERR_TUbak//_V_TU_STATUS.Bits.b5             //
#define	VF_LERR_TU    	          VF_LERR_TUbak//_V_TU_STATUS.Bits.b6             //

//extern volatile V_STATUS_STR    _V_ADB_STATUS;//@ADD_V_ADB_STATUS;    //2014-10-28 Һλ״̬
//#define V_ADB_STATUS              _V_ADB_STATUS.Byte
#define	VF_LB_ADB    	            VF_LB_ADBbak//_V_ADB_STATUS.Bits.b1             //
#define	VF_LERR_ADB    	          VF_LERR_ADBbak//_V_ADB_STATUS.Bits.b2             //
#define	VF_UERR_ADB    	          VF_UERR_ADBbak//_V_ADB_STATUS.Bits.b3             //

#define	VF_LLB_ADB    	          VF_LLB_ADBbak//_V_ADB_STATUS.Bits.b4             //
#define	VF_LLLB_ADB    	          VF_LLLB_ADBbak//_V_ADB_STATUS.Bits.b5             //
#define	VF_UL_ADB    	            VF_UL_ADB_Vbak//_ADB_STATUS.Bits.b6             //


//extern volatile V_STATUS_STR    _V_TEIN_STATUS;//@ADD_V_TE_INLET_STATUS;    //2014-10-28 ��������״̬
//#define V_TEIN_STATUS                 _V_TEIN_STATUS.Byte
#define	VF_UB_TEIN    	              VF_UB_TEINbak//_V_TEIN_STATUS.Bits.b0             //
#define	VF_LB_TEIN    	              VF_ERR_TEINbak//_V_TEIN_STATUS.Bits.b1             //
#define	VF_ERR_TEIN    	              VF_ERR_TEINbak//_V_TEIN_STATUS.Bits.b2             //
#define	VF_IS_TSCR_SET_TEIN    	      VF_IS_TSCR_SET_TEINbak//_V_TEIN_STATUS.Bits.b3             //�Ƿ�������TSCR,2015-8-11
#define	VF_UB_TAMB_OUTER    	        VF_UB_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b4             //
#define	VF_LB_TAMB_OUTER    	        VF_LB_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b5             //
#define	VF_ERR_TAMB_OUTER    	        VF_ERR_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b6             //


//extern volatile V_STATUS_STR    _V_TEOUT_STATUS;//@ADD_V_TE_OUTLET_STATUS;    //2014-10-28 ��������״̬
//#define V_TEOUT_STATUS                _V_TEOUT_STATUS.Byte
#define	VF_UB_TEOUT    	              VF_UB_TEOUTbak//_V_TEOUT_STATUS.Bits.b0             //
#define	VF_LB_TEOUT    	              VF_LB_TEOUTbak//_V_TEOUT_STATUS.Bits.b1             //
#define	VF_ERR_TEOUT    	            VF_ERR_TEOUTbak//_V_TEOUT_STATUS.Bits.b2             //
#define	VF_UB_TAMB_INNER    	        VF_UB_TAMB_INNERbak//      _V_TEOUT_STATUS.Bits.b3             //
#define	VF_LB_TAMB_INNER    	        VF_LB_TAMB_INNERbak//_V_TEOUT_STATUS.Bits.b4             //
#define	VF_ERR_TAMB_INNER    	        VF_ERR_TAMB_INNERbak//_V_TEOUT_STATUS.Bits.b5             //
#define	VF_NOTAMB_FROMCAN    	  			VF_NOTAMB_FROMCANbak//_V_TEOUT_STATUS.Bits.b6             //2015-8-27û������CAN���¶��ź�
#define	VF_NOPAMB_FROMCAN    	  			VF_NOPAMB_FROMCANbak//_V_TEOUT_STATUS.Bits.b7             //2015-8-27û������CAN�Ļ���ѹ���ź�



//extern volatile V_STATUS_STR    _V_SCRDTC_STATUS;//@ADD_V_SCRDTC_STATUS;    //2014-10-28 SLAVEģʽʱ��״̬��
//#define V_SCRDTC_STATUS                _V_SCRDTC_STATUS.Byte
#define	VF_VOLT_SCRDTC    	         VF_VOLT_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b0             //��
#define	VF_INI_ACU_SCRDTC    	       VF_INI_ACU_SCRDTCbak//    _V_SCRDTC_STATUS.Bits.b1             //�� //��δ�궨
#define	VF_INJECTOR_SCRDTC    	     VF_INJECTOR_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b2             //��
#define	VF_PUMPCHOKE_SCRDTC    	     VF_PUMPCHOKE_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b3             //��
#define	VF_ACU_SCRDTC    	           VF_ACU_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b4             //��
#define	VF_FROZEN_SCRDTC    	       VF_FROZEN_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b5             //��
#define	VF_AIRVALVE_SCRDTC    	     VF_AIRVALVE_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b6             //������ŷ�
#define	VF_MIXPRESSURE_SCRDTC    	   VF_MIXPRESSURE_SCRDTCbak//         _V_SCRDTC_STATUS.Bits.b7             //�ù�·�쳣


//extern volatile V_STATUS_STR    _V_SCRDTC2_STATUS;//@ADD_V_SCRDTC2_STATUS;    //2014-10-28 MASTER
//#define V_SCRDTC2_STATUS                _V_SCRDTC2_STATUS.Byte
#define	VF_LB_ADB_SCRDTC    	       VF_LB_ADB_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b0             //��
#define	VF_ERR_ADB_SCRDTC    	       VF_ERR_ADB_SCRDTCbak//   _V_SCRDTC2_STATUS.Bits.b1             //��
#define	VF_FROZEN_TU_SCRDTC    	     VF_FROZEN_TU_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b2             //��
#define	VF_ERR_TU_SCRDTC    	       VF_ERR_TU_SCRDTCbak//   _V_SCRDTC2_STATUS.Bits.b3             //��
#define	VF_ERR_COOLV_SCRDTC    	     VF_ERR_COOLV_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b4             //��
#define	VF_ERR_TEIN_SCRDTC    	     VF_ERR_TEIN_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b5             //��
#define	VF_ERR_TEOUT_SCRDTC    	     VF_ERR_TEOUT_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b6             //��
#define	VF_NOXON_BUTNOVALUE_SCRDTC   VF_NOXON_BUTNOVALUE_SCRDTCbak// 	  _V_SCRDTC2_STATUS.Bits.b7             //��

//extern volatile V_STATUS_STR    _V_SCRDTC3_STATUS;//@ADD_V_SCRDTC3_STATUS;    //2014-10-28 ��������״̬
//#define V_SCRDTC3_STATUS                _V_SCRDTC3_STATUS.Byte
#define	VF_EXH_U_SCRDTC    	         VF_EXH_U_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b0             //��
#define	VF_EXH_UU_SCRDTC    	       VF_EXH_UU_SCRDTCbak//     _V_SCRDTC3_STATUS.Bits.b1             //��
#define	VF_CATALYST_SCRDTC    	     VF_CATALYST_SCRDTCbak//       _V_SCRDTC3_STATUS.Bits.b2             //��

#define	VF_HIGHALTITUDE_SCRDTC    	 VF_HIGHALTITUDE_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b3             //2015-8-27 ADD HIGH ALTITUDE
#define	VF_TORQUELIMIT_SCRDTC    	   VF_TORQUELIMIT_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b4             //2015-8-28 ��Ť��־



//extern volatile V_STATUS_STR    _V_SCRDTC4_STATUS;//@ADD_V_SCRDTC4_STATUS;    //2014-10-28 ��������״̬
//#define V_SCRDTC4_STATUS                _V_SCRDTC4_STATUS.Byte
#define	VF_NOLINK_FROMECU_SCRDTC    	    VF_NOLINK_FROMECU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b7             //û������ECU������
#define	VF_NOLINK_FROMDCU_SCRDTC    	    VF_NOLINK_FROMDCU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b6             //��
#define	VF_NOLINK_FROMACU_SCRDTC    	    VF_NOLINK_FROMACU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b5             //��
#define	VF_NOLINK_FROMNOX_SCRDTC    	    VF_NOLINK_FROMNOX_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b4             //��
#define	VF_NOLINK_FROMDCU_MGPERS_SCRDTC   VF_NOLINK_FROMDCU_MGPERS_SCRDTCbak// 	            _V_SCRDTC4_STATUS.Bits.b3             //�� ,2015-1-27 ����mg/S������


//extern volatile V_STATUS_STR    _V_SCRDTC5_STATUS;//@ADD_V_SCRDTC5_STATUS;    //2014-12-7 CANͨѶ��� 
//#define V_SCRDTC5_STATUS                _V_SCRDTC5_STATUS.Byte
#define	VF_NOLINK_FROMSCR1_SCRDTC    	    VF_NOLINK_FROMSCR1_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b7             
#define	VF_NOLINK_FROMINCON_SCRDTC    	  VF_NOLINK_FROMINCON_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b6             
#define	VF_NOLINK_FROMAMCON_SCRDTC    	  VF_NOLINK_FROMAMCON_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b5             
#define	VF_NOLINK_FROMEEC2_SCRDTC    	    VF_NOLINK_FROMEEC2_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b4             
#define	VF_NOLINK_FROMEEC1_SCRDTC    	    VF_NOLINK_FROMEEC1_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b3             
#define	VF_NOLINK_FROMPT_SCRDTC    	      VF_NOLINK_FROMPT_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b2//2015-4-4             
#define	VF_NOLINK_N_SCRDTC    	          VF_NOLINK_N_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b1//2015-7-21  û��ת���ź�
#define	VF_NOLINK_T_SCRDTC    	          VF_NOLINK_T_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b0//2015-7-21  û��Ť���ź�

          
//extern volatile V_STATUS_STR    _V_CANCTL_STATUS;//@ADD_V_DTCCTL_STATUS;    //2014-12-7 CANͨѶ��� 
//#define V_CANCTL_STATUS                _V_CANCTL_STATUS.Byte
#define	VF_CALPARA_REQUEST_CANCTL    	 		VF_CALPARA_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b0       //��������CAN���ϵģ��Ա궨���ݵ�����      
#define	VF_DTC_REQUEST_CANCTL    	 				VF_DTC_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b1             //ʹ�ܹ�����ķ���
#define	VF_PNPARA_REQUEST_CANCTL    	 		VF_PNPARA_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b2             //ʹ������궨���ݵķ�����2015-7-21
#define	VF_DIAG_RQST_CANCTL    	 					VF_DIAG_RQST_CANCTLbak//_V_CANCTL_STATUS.Bits.b3             //����Ƿ�������2015-8-29
#define	VF_TPCM_BAM_SENT_CANCTL    		 		VF_TPCM_BAM_SENT_CANCTLbak//_V_CANCTL_STATUS.Bits.b4             //TPCM_BAM��ͷ�Ѿ�������
#define	VF_FS_BS_CONTINUE_CANCTL    			VF_FS_BS_CONTINUE_CANCTLbak//_V_CANCTL_STATUS.Bits.b5             //2015-9-20 15765֡�����Ʒ�ʽ




//extern volatile V_STATUS_STR    _V_PUMPCTL_STATUS;//@ADD_V_PUMPCTL_STATUS;    //2015-6-11 �ÿ���ģʽ
//#define V_PUMPCTL_STATUS                V_PUMPCTL_STATUS.Byte
#define	VF_ISCONSTV_NOTDT3_PUMP    	    	VF_ISCONSTV_NOTDT3_PUMPbak//_V_PUMPCTL_STATUS.Bits.b0       //�����Ƿ��Ƕ��ݿ���
#define	VF_ISFREREC_PUMP    	          	VF_ISFREREC_PUMPbak//_V_PUMPCTL_STATUS.Bits.b1       //�����Ƿ�Ƶ������
#define	VF_ISBACKPRE_PUMP    	          	VF_ISBACKPRE_PUMPbak//_V_PUMPCTL_STATUS.Bits.b2       //�����Ƿ�ѹ����
#define	VF_NOLIQUID_CHKING_PUMP    	    	VF_NOLIQUID_CHKING_PUMPbak//_V_PUMPCTL_STATUS.Bits.b3       //2015-6-15:�ÿ��ж�:������Ϊ��ʱ��T1-T3�Ƿ������Ϊ�жϱÿյ�����
#define	VF_INJ2_SHIFT_SCR    	          	VF_INJ2_SHIFT_SCRbak//_V_PUMPCTL_STATUS.Bits.b5       //2015-6-17 add for ch2 injection control
#define	VF_SYSINJ_CH2_PUMP    	        	VF_SYSINJ_CH2_PUMPbak//_V_PUMPCTL_STATUS.Bits.b6       //2015-6-29 CH2�Ĵ�Һ�������Ƿ����ͬ��ģʽ
#define VF_IS_PRIME_PUMP                	VF_IS_PRIME_PUMPbak//_V_PUMPCTL_STATUS.Bits.b7       //2015-7-4 ��Һ����Ƿ������ı�ʶ��


//extern volatile V_STATUS_STR    _V_SYSENABLE0_STATUS;//@ADD_V_SYSENABLE0_STATUS;    //2015-6-11 �ÿ���ģʽ
//#define V_SYSENABLE0_STATUS                _V_SYSENABLE0_STATUS.Byte
#define	VF_SLAVEOBD_SYS    	    					VF_SLAVEOBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b0       //
#define	VF_MASTEROBD_SYS    	          	VF_MASTEROBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b1       //
#define	VF_T3OBD_SYS    	          			VF_T3OBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b2       //
#define	VF_GELANFU_SYS    	    					VF_GELANFU_SYSbak//_V_SYSENABLE0_STATUS.Bits.b3       //
#define	VF_CALPARA_AUTOOUT_SYS    	    	VF_CALPARA_AUTOOUT_SYSbak//_V_SYSENABLE0_STATUS.Bits.b4       //2015-7-30 �Զ�ʹ�ܱ궨���ݵķ���
#define	VF_NO_TRANS_EN_SYS    	    			VF_NO_TRANS_EN_SYSbak//_V_SYSENABLE0_STATUS.Bits.b5       //2015-9-29
#define	VF_NO_NH3_EN_SYS    	    				VF_NO_NH3_EN_SYSbak//_V_SYSENABLE0_STATUS.Bits.b6       //



//extern volatile V_STATUS_STR    _V_MASTER_STATUS;//@ADD_V_MASTER_STATUS;    //2015-7-2 MASTER����
//#define V_MASTER_STATUS         _V_MASTER_STATUS.Byte
#define	VF_1NEWINJ_MASTER    	    VF_1NEWINJ_MASTERbak//_V_MASTER_STATUS.Bits.b0          // ����MASTERģʽ����һ���µ�INJ����
#define	VF_1DC_CON_MASTER    	    VF_1DC_CON_MASTERbak//_V_MASTER_STATUS.Bits.b1          // ����MASTERģʽ������һ��DC������
#define	VF_1WUC_MASTER    	    	VF_1WUC_MASTERbak//_V_MASTER_STATUS.Bits.b2          // ����MASTERģʽ������һ��WUC������
#define	VF_STH3_CON_MASTER    	  VF_STH3_CON_MASTERbak//  _V_MASTER_STATUS.Bits.b3          // ����MASTERģʽ������NH3STĿ��
#define	VF_JSTARTS_MASTER    	    VF_JSTARTS_MASTERbak//_V_MASTER_STATUS.Bits.b4          // ����MASTERģʽ����������������




//extern volatile V_STATUS_STR    _V_CONDICTION_STATUS;//@ADD_CONDICTION_STATUS;    //
//#define V_CONDICTION_STATUS         _V_CONDICTION_STATUS.Byte
#define	VF_PIPEOBD_CON    	      VF_PIPEOBD_CONbak//      _V_CONDICTION_STATUS.Bits.b0  //����PIPEOBD������        
#define	VF_PRIMING_CON    	      VF_PRIMING_CONbak//      _V_CONDICTION_STATUS.Bits.b1  //���� ������        
#define	VF_DOSING_CON    	        VF_DOSING_CONbak//    _V_CONDICTION_STATUS.Bits.b2  //�������������        
#define	VF_T1SCAN_CON    	        VF_T1SCAN_CONbak//    _V_CONDICTION_STATUS.Bits.b3  //�������������        
#define	VF_PURGING_CON    	      VF_PURGING_CONbak//      _V_CONDICTION_STATUS.Bits.b3  //�������������        

//extern volatile V_STATUS_STR    _V_PUMP2_STATUS;//@ADD_PUMP2_STATUS;    
//#define V_PUMP2_STATUS         _V_PUMP2_STATUS.Byte
#define	VF_ISSETPARA_PUMP2    	  VF_ISSETPARA_PUMP2bak//        _V_PUMP2_STATUS.Bits.b0          //2015-7-19�����ò�����Ƶ�ʺ�T1�� 
#define VF_ISENABLE_PUMP2    	    VF_ISENABLE_PUMP2bak//      _V_PUMP2_STATUS.Bits.b1//�Ƿ�ʹ��PUMP2
#define VF_TOBESCAN_PUMP2    	    VF_TOBESCAN_PUMP2bak//      _V_PUMP2_STATUS.Bits.b2//��Ҫɨ��
#define VF_L_DOSERNOLIQUID_PUMP2  VF_L_DOSERNOLIQUID_PUMP2bak//  	  _V_PUMP2_STATUS.Bits.b3//��Һ�õļ����ã��ڶ�γ��Ժ�����Һ��


//extern volatile V_STATUS_STR    _V_CAL_STATUS;//@ADD_CAL_STATUS;  //���ڱ궨  
//#define V_CAL_STATUS         _V_CAL_STATUS.Byte
#define	VF_MODE_OBDWORK_CAL    	  VF_MODE_OBDWORK_CALbak//        _V_CAL_STATUS.Bits.b0  //OBD�ɹ�����MASTER��2015-7-18
//���ü����궨�õĿ���
#define	VF_NH3MODEL_OFF_CAL    	  VF_NH3MODEL_OFF_CALbak//        _V_CAL_STATUS.Bits.b1  //2015-10-12
#define	VF_TRANSMODEL_OFF_CAL    	VF_TRANSMODEL_OFF_CALbak//        _V_CAL_STATUS.Bits.b2  //2015-10-12


//extern volatile V_STATUS_STR    _V_DATAVIEW_STATUS;//@ADD_DATAVIEW_STATUS;  //DATAVIEW 2015-7-12
//#define V_DATAVIEW_STATUS         _V_DATAVIEW_STATUS.Byte
#define	VF_FLASHLINK_DATAVIEW    	VF_FLASHLINK_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b0          // 
#define	VF_LINKTOEE_DATAVIEW    	VF_LINKTOEE_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b1          //1: TO EE, 0 : TO FLASH 
#define	VF_BY_BYTE_DATAVIEW    	  VF_BY_BYTE_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b2          //1: TO BY BYTE, 0 : BY WORD 
#define	VF_BY_1UNIT_DATAVIEW    	VF_BY_1UNIT_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b3          //1: TO 1UNIT, 0 : 2 UNITS 
#define	VF_LINK_DATAVIEW    	    VF_LINK_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b4          //1: ��DATAVIEW ���������ӣ�


//extern volatile V_STATUS_STR    _V_PN_STATUS[2];//@ADD_PN_STATUS;  //PN ,2015-7-16
//extern volatile V_STATUS_STR    _V_PN2_STATUS[2];//@ADD_PN2_STATUS;  //PN ,2015-7-16
//extern volatile V_STATUS_STR    _V_PN3_STATUS[2];//@ADD_PN3_STATUS;  //PN ,2015-7-20

/*----------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////

extern volatile byte VF_CCLUP_CLSbak;
extern volatile byte VF_LEANCON_CLSbak;
extern volatile byte VF_LIMITFK_CLSbak;
extern volatile byte VF_ENRICHLIMITFK_CLSbak;
extern volatile byte VF_T35_CLSbak;  
extern volatile byte VF_1LEAN_NOTIDLE_CLSbak;
extern volatile byte VF_RICH_MORE_CLSbak;
extern volatile byte VF_LEAN_MORE_CLSbak;

#define	VF_CCLUP_CLS					VF_CCLUP_CLSbak
#define	VF_LEANCON_CLS				VF_LEANCON_CLSbak    //2012-12-7��ƫϡ����
#define	VF_LIMITFK_CLS				VF_LIMITFK_CLSbak	//���������������ﵽ����
#define	VF_ENRICHLIMITFK_CLS	VF_ENRICHLIMITFK_CLSbak    //���ͷ���������ס��
#define	VF_T35_CLS						VF_T35_CLSbak    //2013-3-19
#define	VF_1LEAN_NOTIDLE_CLS	VF_1LEAN_NOTIDLE_CLSbak    //2012-6-28���뿪���ٺ��Ƿ����һ��ϡ
#define	VF_RICH_MORE_CLS			VF_RICH_MORE_CLSbak    //������ѧϰ����ʾŨ��̫�ࣻ
#define	VF_LEAN_MORE_CLS			VF_LEAN_MORE_CLSbak    //������ѧϰ����ʾϡ��̫�ࣻ


/******************************************************************/
//extern volatile V_STATUS_STR _V_CLSSL_STATUS;//    @ADD_V_CLSSL_STATUS;
//extern volatile V_STATUS_STR _V_CLSSLH_STATUS;//   @ADD_V_CLSSLH_STATUS;
//#define V_CLSSL_STATUS      _V_CLSSL_STATUS.Byte
//#define V_CLSSLH_STATUS      _V_CLSSLH_STATUS.Byte

extern volatile byte VF_DTCOK_CLSSLbak;   //û�й��ϣ�����ѧϰ������
extern volatile byte VF_EECHKED_CLSSLbak;  //EEPROM �е���ѧϰ�����Ѿ�У���
extern volatile byte VF_LIMITLEARN_CLSSLbak;
extern volatile byte VF_MAYLEARN_CLSSLbak;
extern volatile byte VF_RD_NEW_CLSSLbak;   //����ͬ�����ζ�EE�Ĳ���
extern volatile byte VF_STIO_CLSSLbak;
extern volatile byte VF_TOBELEARN_CLSSLbak;
extern volatile byte VF_FKSTABLE_CLSSLbak;     //����ֵ�ȶ���2013-3-7
#define	VF_DTCOK_CLSSL			VF_DTCOK_CLSSLbak   //û�й��ϣ�����ѧϰ������
#define	VF_EECHKED_CLSSL		VF_EECHKED_CLSSLbak  //EEPROM �е���ѧϰ�����Ѿ�У���
#define	VF_LIMITLEARN_CLSSL	VF_LIMITLEARN_CLSSLbak
#define	VF_MAYLEARN_CLSSL		VF_MAYLEARN_CLSSLbak
#define	VF_RD_NEW_CLSSL			VF_RD_NEW_CLSSLbak   //����ͬ�����ζ�EE�Ĳ���
#define	VF_STIO_CLSSL				VF_STIO_CLSSLbak
#define	VF_TOBELEARN_CLSSL	VF_TOBELEARN_CLSSLbak
#define	VF_FKSTABLE_CLSSL		VF_FKSTABLE_CLSSLbak     //����ֵ�ȶ���2013-3-7

extern volatile byte VF_TMPOK_CLSSLbak;      //�¶�����Ҫ��
extern volatile byte VF_LEARNPOINT_CHG_CLSSLbak;   //ѧϰ����
extern volatile byte VF_LEARNECONDICTION_CLSSLbak;  //����ѧϰ����
extern volatile byte VF_CONDICTIONSTEADY_CLSSLbak;  //����ѧϰ���������ȶ�һ��ʱ��
extern volatile byte VF_CONDICTIONSTEADY_LST_CLSSLbak;  //��һ�Σ�����ѧϰ���������ȶ�һ��ʱ��
extern volatile byte VF_EE_ER_CLSSLbak;  //EEPROM �У���ѧϰֵ����
extern volatile byte VF_MATCH_INI_CLSSLbak; //������ѧϰ�Ļ�������
#define	VF_TMPOK_CLSSL								VF_TMPOK_CLSSLbak      //�¶�����Ҫ��
#define	VF_LEARNPOINT_CHG_CLSSL				VF_LEARNPOINT_CHG_CLSSLbak   //ѧϰ����
#define	VF_LEARNECONDICTION_CLSSL			VF_LEARNECONDICTION_CLSSLbak  //����ѧϰ����
#define	VF_CONDICTIONSTEADY_CLSSL			VF_CONDICTIONSTEADY_CLSSLbak  //����ѧϰ���������ȶ�һ��ʱ��
#define	VF_CONDICTIONSTEADY_LST_CLSSL	VF_CONDICTIONSTEADY_LST_CLSSLbak  //��һ�Σ�����ѧϰ���������ȶ�һ��ʱ��
#define	VF_EE_ER_CLSSL								VF_EE_ER_CLSSLbak  //EEPROM �У���ѧϰֵ����
#define	VF_MATCH_INI_CLSSL						VF_MATCH_INI_CLSSLbak  //������ѧϰ�Ļ�������


//extern volatile V_STATUS_STR _V_DTC_STATUS;//  @ADD_V_DTC_STATUS;

extern volatile byte VFF_VBAT_DTCbak;         //1
extern volatile byte VFF_T3ER_DTCbak;         //2
extern volatile byte VFF_ECUNOTINIT_DTCbak;     //5  //2009-3-27 b4 to b2
extern volatile byte VFF_INJ_DTCbak;              //4
extern volatile byte VFF_SYSNOTINIT_DTCbak;     //3  //2009-3-27 b2 to b4
extern volatile byte VFF_TES_DTCbak;             //6
extern volatile byte VFF_OS_DTCbak;             //7
extern volatile byte VFF_TPS_DTCbak; 

#define V_DTC_STATUS   (VFF_VBAT_DTCbak<<0|VFF_T3ER_DTCbak<<1|VFF_ECUNOTINIT_DTCbak<<2|VFF_INJ_DTCbak<<3|\
												VFF_SYSNOTINIT_DTCbak<<4|VFF_TES_DTCbak<<5|VFF_OS_DTCbak<<6|VFF_TPS_DTCbak<<7)
#define	VFF_VBAT_DTC				VFF_VBAT_DTCbak         //1
#define	VFF_T3ER_DTC				VFF_T3ER_DTCbak         //2
#define	VFF_ECUNOTINIT_DTC	VFF_ECUNOTINIT_DTCbak     //5  //2009-3-27 b4 to b2
#define	VFF_INJ_DTC					VFF_INJ_DTCbak              //4
#define	VFF_SYSNOTINIT_DTC	VFF_SYSNOTINIT_DTCbak     //3  //2009-3-27 b2 to b4
#define	VFF_TES_DTC					VFF_TES_DTCbak             //6
#define	VFF_OS_DTC					VFF_OS_DTCbak             //7
#define	VFF_TPS_DTC					VFF_TPS_DTCbak 
//8
//extern volatile V_STATUS_STR _V_DTCH_STATUS;// @ADD_V_DTCH_STATUS;
extern volatile byte VFF_FUEL_DTCbak;          //9
extern volatile byte VFF_INJCTL_DTCbak;         //10
extern volatile byte VFF_ECUINIT_DTCbak;          //11
extern volatile byte VFF_CAECU_DTCbak;           //12
extern volatile byte VFF_ECUPWRON_DTCbak;      //13
extern volatile byte VFF_TAS_DTCbak;             //14
extern volatile byte VFF_TENGHIGH_DTCbak;     //15
extern volatile byte VFF_ISCV_DTCbak;         //16

#define V_DTCH_STATUS   (VFF_FUEL_DTCbak<<0|VFF_INJCTL_DTCbak<<1|VFF_ECUINIT_DTCbak<<2|VFF_CAECU_DTCbak<<3|\
												 VFF_ECUPWRON_DTCbak<<4|VFF_TAS_DTCbak<<5|VFF_TENGHIGH_DTCbak<<6|VFF_ISCV_DTCbak<<7)
#define	VFF_FUEL_DTC			VFF_FUEL_DTCbak          //9
#define	VFF_INJCTL_DTC		VFF_INJCTL_DTCbak         //10
#define	VFF_ECUINIT_DTC		VFF_ECUINIT_DTCbak          //11
#define	VFF_CAECU_DTC			VFF_CAECU_DTCbak          //12
#define	VFF_ECUPWRON_DTC	VFF_ECUPWRON_DTCbak      //13
#define	VFF_TAS_DTC				VFF_TAS_DTCbak             //14
#define	VFF_TENGHIGH_DTC	VFF_TENGHIGH_DTCbak    //15
#define	VFF_ISCV_DTC			VFF_ISCV_DTCbak        //16

extern volatile V_STATUS_STR _V_DTCHL_STATUS;// @ADD_V_DTCHL_STATUS;
extern volatile byte VFF_OSFK_DTCbak;         //17
extern volatile byte VFF_SAV_DTCbak;             //18
extern volatile byte VFF_EGR_DTCbak;              //19
extern volatile byte VFF_TWC_DTCbak;             //20
extern volatile byte VFF_MAPS_DTCbak;         //21
extern volatile byte VFF_AMF_DTCbak;               //22
extern volatile byte VFF_IGN_COIL_SPNbak;              //22
extern volatile byte VFF_VBATSP_ERRbak; 

#define V_DTCHL_STATUS   (VFF_OSFK_DTCbak<<0|VFF_SAV_DTCbak<<1|VFF_EGR_DTCbak<<2|VFF_TWC_DTCbak<<3|\
													VFF_MAPS_DTCbak<<4|VFF_AMF_DTCbak<<5|VFF_IGN_COIL_SPNbak<<6|VFF_VBATSP_ERRbak<<7)
#define	VFF_OSFK_DTC			VFF_OSFK_DTCbak         //17
#define	VFF_SAV_DTC				VFF_SAV_DTCbak             //18
#define	VFF_EGR_DTC				VFF_EGR_DTCbak              //19
#define	VFF_TWC_DTC				VFF_TWC_DTCbak             //20
#define	VFF_MAPS_DTC			VFF_MAPS_DTCbak         //21
#define	VFF_AMF_DTC				VFF_AMF_DTCbak               //22
#define	VFF_IGN_COIL_SPN	VFF_IGN_COIL_SPNbak               //22
#define	VFF_VBATSP_ERR		VFF_VBATSP_ERRbak 


extern volatile V_STATUS_STR _V_DTCHH_STATUS;// @ADD_V_DTCHH_STATUS;
extern volatile byte VFF_CYL1INJ_DTCbak;           //33
extern volatile byte VFF_CYL2INJ_DTCbak;           //34
extern volatile byte VFF_CYL1OS_DTCbak;           //35
extern volatile byte VFF_CYL2OS_DTCbak;           //36
extern volatile byte VFF_PAIR_DTCbak;         //37
extern volatile byte VFF_IDLESCLIMIT_DTCbak;      //38
extern volatile byte VFF_7OFDIAG5_DTCbak;      //39
extern volatile byte VFF_8OFDIAG5_DTCbak;      //40
#define V_DTCHH_STATUS   (VFF_CYL1INJ_DTCbak<<0|VFF_CYL2INJ_DTCbak<<1|VFF_CYL1OS_DTCbak<<2|VFF_CYL2OS_DTCbak<<3|\
													VFF_PAIR_DTCbak<<4|VFF_IDLESCLIMIT_DTCbak<<5|VFF_7OFDIAG5_DTCbak<<6|VFF_8OFDIAG5_DTCbak<<7)
#define	VFF_CYL1INJ_DTC			VFF_CYL1INJ_DTCbak           //33
#define	VFF_CYL2INJ_DTC			VFF_CYL2INJ_DTCbak           //34
#define	VFF_CYL1OS_DTC			VFF_CYL1OS_DTCbak           //35
#define	VFF_CYL2OS_DTC			VFF_CYL2OS_DTCbak           //36
#define	VFF_PAIR_DTC				VFF_PAIR_DTCbak         //37
#define	VFF_IDLESCLIMIT_DTC	VFF_IDLESCLIMIT_DTCbak      //38
#define	VFF_7OFDIAG5_DTC		VFF_7OFDIAG5_DTCbak      //39
#define	VFF_8OFDIAG5_DTC		VFF_8OFDIAG5_DTCbak      //40

extern volatile V_STATUS_STR _V_DTCHHL_STATUS;// @ADD_V_DTCHHL_STATUS;    //2009-3-4 add for power nozzle ini
extern volatile byte VFF_NOZZLEINI_TESHIGH_DTCbak;
extern volatile byte VFF_NOZZLEINI_T1DGOT_DTCbak;
extern volatile byte VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak;
extern volatile byte VFF_NOZZLEINI_ABCGOT_DTCbak;
extern volatile byte VFF_NOZZLEINI_ABCSAVED_DTCbak;
extern volatile byte VFF_NOZZLEINI_NOTDOM_DTCbak;
extern volatile byte VFF_NOZZLEINI_DOMFAILURE_DTCbak;
extern volatile byte VFF_NOZZLEINI_NOTINI_DTCbak;
#define V_DTCHHL_STATUS   (VFF_NOZZLEINI_TESHIGH_DTCbak<<0|VFF_NOZZLEINI_T1DGOT_DTCbak<<1|\
													 VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak<<2|VFF_NOZZLEINI_ABCGOT_DTCbak<<3|\
													 VFF_NOZZLEINI_ABCSAVED_DTCbak<<4|VFF_NOZZLEINI_NOTDOM_DTCbak<<5|\
													 VFF_NOZZLEINI_DOMFAILURE_DTCbak<<6|VFF_NOZZLEINI_NOTINI_DTCbak<<7)
#define	VFF_NOZZLEINI_TESHIGH_DTC               VFF_NOZZLEINI_TESHIGH_DTCbak           //41  �����ʼ��ʱ�����¶ȵ�Ҫ��
#define	VFF_NOZZLEINI_T1DGOT_DTC                VFF_NOZZLEINI_T1DGOT_DTCbak        //�������T1D
#define	VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTC VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak        //��û���ҵ�T4�յ������ɨ��T1Dʧ�ܣ�
#define	VFF_NOZZLEINI_ABCGOT_DTC                VFF_NOZZLEINI_ABCGOT_DTCbak    //������˼���T3B�����ABCϵ����
#define	VFF_NOZZLEINI_ABCSAVED_DTC              VFF_NOZZLEINI_ABCSAVED_DTCbak    //���ɹ�����ABC���Խ����
#define	VFF_NOZZLEINI_NOTDOM_DTC                VFF_NOZZLEINI_NOTDOM_DTCbak    //��û��DOM
#define	VFF_NOZZLEINI_DOMFAILURE_DTC            VFF_NOZZLEINI_DOMFAILURE_DTCbak//:DOM���������⣻
#define	VFF_NOZZLEINI_NOTINI_DTC                VFF_NOZZLEINI_NOTINI_DTCbak//:����û�г�ʼ����

//extern volatile V_STATUS_STR _V_DTCHHH_STATUS;// @ADD_V_DTCHHH_STATUS;    //2009-3-5 add for power nozzle ini
extern volatile byte VFF_NOZZLEINI_VBATNOTDOM_DTCbak;//:ECU��ѹû��dom
extern volatile byte VFF_NOZZLEINI_VBATVALID_DTCbak;//:��ѹ���͵����޷���ʼ����
extern volatile byte VFF_NOZZLEINI_NOINJ_DTCbak;//:���������ϵ����޷���ʼ����
extern volatile byte VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak;//ɨ������T4<T1d+500����Ϊ���������
extern volatile byte VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak;//T1Dɨ��֮�󣬼���õ���ϵ���Ƿ���2009-3-5
extern volatile byte VFF_NOZZLEINI_NOT4_DTCbak;//2009-11-29 ��t4,�����޷���ʼ����
#define V_DTCHHH_STATUS   (VFF_NOZZLEINI_VBATNOTDOM_DTCbak<<0|VFF_NOZZLEINI_VBATVALID_DTCbak<<1|\
													 VFF_NOZZLEINI_NOINJ_DTCbak<<2|VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak<<3|\
													 VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak<<4|VFF_NOZZLEINI_NOT4_DTCbak<<5)
#define	VFF_NOZZLEINI_VBATNOTDOM_DTC            VFF_NOZZLEINI_VBATNOTDOM_DTCbak//:ECU��ѹû��dom
#define	VFF_NOZZLEINI_VBATVALID_DTC             VFF_NOZZLEINI_VBATVALID_DTCbak//:��ѹ���͵����޷���ʼ����
#define	VFF_NOZZLEINI_NOINJ_DTC                 VFF_NOZZLEINI_NOINJ_DTCbak//:���������ϵ����޷���ʼ����
#define	VFF_NOZZLEINI_T1DFAIL_T4LET1_DTC        VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak//ɨ������T4<T1d+500����Ϊ���������
#define	VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTC VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak//T1Dɨ��֮�󣬼���õ���ϵ���Ƿ���2009-3-5
#define	VFF_NOZZLEINI_NOT4_DTC                  VFF_NOZZLEINI_NOT4_DTCbak//2009-11-29 ��t4,�����޷���ʼ����

extern volatile V_STATUS_STR _V_EE_STATUS;//@ADD_V_EE_STATUS;
#define V_EE_STATUS  _V_EE_STATUS.Byte
#define	VF_JINI_EE	_V_EE_STATUS.Bits.b0
#define	VF_JPROECU_EE	_V_EE_STATUS.Bits.b1
#define	VF_JBEWR_EE	_V_EE_STATUS.Bits.b2
#define	VF_OPEREE_1CYCLE_EE	_V_EE_STATUS.Bits.b3
#define VF_DOUBLEER_EE  	_V_EE_STATUS.Bits.b4//2010-1-4 data(x)+1 !=data(xb)

#define VF_IICER_EE  	_V_EE_STATUS.Bits.b5//2013-2-18
#define VF_NOACK_EE  	_V_EE_STATUS.Bits.b6//2013-2-18
#define VF_ISWRITING_EE  	_V_EE_STATUS.Bits.b7//2013-2-18   EE�ǲ�������д����


extern volatile V_STATUS_STR _V_IDLEAQ_STATUS;//@ADD_V_IDLEAQ_STATUS;
#define V_IDLEAQ_STATUS    _V_IDLEAQ_STATUS.Byte
#define	VF_1STAQ_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b0
#define	VF_NOTAQING_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b1
#define	VF_LIMITAQ_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b2
#define	VF_LOWRPM_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b3
#define VF_AQSEVERALTIMES_IDLEAQ     _V_IDLEAQ_STATUS.Bits.b4  //2009-4-21
#define	VF_AQINI_FAIL_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b5//2009-9-14
#define	VF_AQMORETHAN1TIME_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b6//2010-2-7 AQ1������
#define	VF_AQINI_ENRICHTOOSHORT_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b7      //�����ٳ�ʼ��ʱ�������һ����Ũɨ��ʱ���������٣�����λ��

extern volatile V_STATUS_STR _V_CFCI_STATUS;//@ADD_V_CFCI_STATUS;
#define V_CFCI_STATUS  _V_CFCI_STATUS.Byte
#define	VF_CF_CFCI	_V_CFCI_STATUS.Bits.b0	//���ͱ�־λ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
#define	VF_CFLST_CFCI	_V_CFCI_STATUS.Bits.b1
#define	VF_CFLST2_CFCI	_V_CFCI_STATUS.Bits.b2
#define	VF_JCI_LST_CFCI	_V_CFCI_STATUS.Bits.b3//2013-1-14

#define	VF_CICORECING_CFCI	_V_CFCI_STATUS.Bits.b4	//�ٸ�������״̬
#define	VF_IGNONLY_CFCI	_V_CFCI_STATUS.Bits.b5
#define	VF_JCI_CFCI	_V_CFCI_STATUS.Bits.b6
#define	VF_CICFGT5_CFCI	_V_CFCI_STATUS.Bits.b7  //2009-12-13  V_CICF>5


extern volatile V_STATUS_STR _V_IACV_STATUS;//@ADD_V_IACV_STATUS;
#define V_IACV_STATUS  _V_IACV_STATUS.Byte
#define	VF_CORACT	_V_IACV_STATUS.Bits.b0
#define	VF_CFST	_V_IACV_STATUS.Bits.b1
#define	VF_CIRECINJST	_V_IACV_STATUS.Bits.b2
#define	VF_CORADJ	_V_IACV_STATUS.Bits.b3
#define	VF_CORADJ_LST	_V_IACV_STATUS.Bits.b4
#define	VF_C1OPEN_IACV	_V_IACV_STATUS.Bits.b5       //2018-06-23
#define	VF_C2OPEN_IACV	_V_IACV_STATUS.Bits.b6       //2018-06-28 ��·���ж����ĸ���Ȧ��·�����Կ��Էֿ�
#define	VF_SHORT_IACV	_V_IACV_STATUS.Bits.b7         //2018-06-23

extern volatile V_STATUS_STR _V_IACV_STATUS2;//@ADD_V_IACV_STATUS2;
#define	VF_STEP_IGNCO	_V_IACV_STATUS2.Bits.b0
#define	VF_CLSMODE_IACV	_V_IACV_STATUS2.Bits.b1
#define	VF_LEARZRO_KEYOFF	_V_IACV_STATUS2.Bits.b2

extern volatile V_STATUS_STR _V_IDLEMF_STATUS;//@ADD_V_IDLEMF_STATUS;
#define V_IDLEMF_STATUS    _V_IDLEMF_STATUS.Byte
#define	VF_ENMF_IDLEMF	_V_IDLEMF_STATUS.Bits.b0
#define	VF_WITHMS_IDLEMF	_V_IDLEMF_STATUS.Bits.b1

extern volatile V_STATUS_STR _V_ISC_STATUS;//@ADD_V_ISC_STATUS;
#define V_ISC_STATUS   _V_ISC_STATUS.Byte
#define	VF_CONNECT_ISC	_V_ISC_STATUS.Bits.b0
#define	VF_ER_ISC	_V_ISC_STATUS.Bits.b1
#define	VF_IDLEODRTEST_ISC	_V_ISC_STATUS.Bits.b2
#define	VF_JCLOSE_ISC	_V_ISC_STATUS.Bits.b3
#define	VF_WORKSTART_ISC	_V_ISC_STATUS.Bits.b5
#define	VF_WORKBACKIDLE_ISC	_V_ISC_STATUS.Bits.b6
#define	VF_STARTSPEED_HIGHENOUGH_ISC	_V_ISC_STATUS.Bits.b7 //2010-4-16

extern volatile V_STATUS_STR _V_TRANS_STATUS;//@ADD_V_TRANS_STATUS;
#define V_TRANS_STATUS _V_TRANS_STATUS.Byte
extern volatile V_STATUS_STR _V_TRANSH_STATUS;//@ADD_V_TRANSH_STATUS;
#define V_TRANSH_STATUS _V_TRANSH_STATUS.Byte
#define	VF_ACCGT1_TRANS	_V_TRANS_STATUS.Bits.b0
#define	VF_ACCLST_TRANS	_V_TRANS_STATUS.Bits.b1
#define	VF_ACCLST2_TRANS	_V_TRANS_STATUS.Bits.b2
#define	VF_ACCSPD_TRANS	_V_TRANS_STATUS.Bits.b3
#define	VF_DECGT1_TRANS	_V_TRANS_STATUS.Bits.b4
#define	VF_DECLST_TRANS	_V_TRANS_STATUS.Bits.b5
#define	VF_DECLST2_TRANS	_V_TRANS_STATUS.Bits.b6
#define	VF_DECSPD_TRANS	_V_TRANS_STATUS.Bits.b7

#define	VF_NTESTPTSBINJ_TRANS	_V_TRANSH_STATUS.Bits.b0
#define	VF_TRIGTPS_TRANS	_V_TRANSH_STATUS.Bits.b1
#define	VF_FUELLIMT_TRANS	_V_TRANSH_STATUS.Bits.b2	//2017-3-27 ���ٹ���LD�������ñ�־λ����ʼ����ʱ��λ�������ָ���Բ�����ɺ�λ
#define	VF_TRANSLT10_TRANS	_V_TRANSH_STATUS.Bits.b3//2009-8-7 add �������ű䶯�У�����ֵ����3
#define	VF_TRANSGT15_TRANS	_V_TRANSH_STATUS.Bits.b4//2009-8-7 add ���ڼ����У�����ֵ����3
#define	VF_ACCMID_TRANS	_V_TRANSH_STATUS.Bits.b5//2010-1-11 �����еȼ���
#define	VF_ACCMIDLST_TRANS	_V_TRANSH_STATUS.Bits.b6//2010-1-11 �����еȼ���

//extern volatile V_STATUS_STR _V_HARD_STATUS;//@ADD_V_HARD_STATUS;
//#define V_HARD_STATUS  _V_HARD_STATUS.Byte

extern volatile byte VF_INJSTOP_HARDbak;//_V_HARD_STATUS.Bits.b0
extern volatile byte VF_JKEYON_HARDbak;//_V_HARD_STATUS.Bits.b1   //2013-4-1
extern volatile byte VF_KEYON_HARDbak;//_V_HARD_STATUS.Bits.b2
extern volatile byte VF_POR_HARDbak;//_V_HARD_STATUS.Bits.b3
extern volatile byte VF_JKEYOFF_HARDbak;//_V_HARD_STATUS.Bits.b4 //2010-8-20
extern volatile byte VF_TURNONKEY_HARDbak;//_V_HARD_STATUS.Bits.b5
extern volatile byte VF_VPOR_HARDbak;//_V_HARD_STATUS.Bits.b6
extern volatile byte VF_T35TEST_HARDbak;//_V_HARD_STATUS.Bits.b7

#define	VF_INJSTOP_HARD		VF_INJSTOP_HARDbak//_V_HARD_STATUS.Bits.b0
#define	VF_JKEYON_HARD		VF_JKEYON_HARDbak//_V_HARD_STATUS.Bits.b1   //2013-4-1
#define	VF_KEYON_HARD			VF_KEYON_HARDbak//_V_HARD_STATUS.Bits.b2
#define	VF_POR_HARD				VF_POR_HARDbak//_V_HARD_STATUS.Bits.b3
#define	VF_JKEYOFF_HARD		VF_JKEYOFF_HARDbak//_V_HARD_STATUS.Bits.b4 //2010-8-20
#define	VF_TURNONKEY_HARD	VF_TURNONKEY_HARDbak//_V_HARD_STATUS.Bits.b5
#define	VF_VPOR_HARD			VF_VPOR_HARDbak//_V_HARD_STATUS.Bits.b6
#define	VF_T35TEST_HARD		VF_T35TEST_HARDbak//_V_HARD_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_IGN_STATUS;//@ADD_V_IGN_STATUS;
#define V_IGN_STATUS   _V_IGN_STATUS.Byte
#define	VF_NOTDELAY_IGN	_V_IGN_STATUS.Bits.b0
#define	VF_NEEDRECBYLOWRPM_IGN	_V_IGN_STATUS.Bits.b1
#define	VF_IDLEAIRLESS_IGN	    _V_IGN_STATUS.Bits.b2//2009-4-17 ADD ��������ƫ��
#define	VF_IDLEIGNFK_NORMAL_IGN	    _V_IGN_STATUS.Bits.b3//
#define VF_QHYCA_INTERR_IGN     _V_IGN_STATUS.Bits.b4//2009-8-7 �Ǳ�ǰ����ʱ��������
#define	VF_IDLEIGNFK_IGN    	_V_IGN_STATUS.Bits.b5 //2009-12-18 ���ٵ����
#define	VF_REVTARGET_INC_IGN    	_V_IGN_STATUS.Bits.b6 //���ڻ��ڵ��ǵĵ�����߽׶�

#define	VF_AIRLESS_WHENIDLEAQ_IGN    	_V_IGN_STATUS.Bits.b7 //��������ʱ,����ƫ��;//2010-1-10


extern volatile V_STATUS_STR _V_INJCTL_STATUS;//@ADD_V_INJCTL_STATUS;
#define V_INJCTL_STATUS    _V_INJCTL_STATUS.Byte
extern volatile V_STATUS_STR _V_INJCTLH_STATUS;//@ADD_V_INJCTLH_STATUS;
#define V_INJCTLH_STATUS    _V_INJCTLH_STATUS.Byte
extern volatile V_STATUS_STR _V_INJCTLHL_STATUS;//@ADD_V_INJCTLHL_STATUS; //2010-2-27
#define V_INJCTLHL_STATUS    _V_INJCTLHL_STATUS.Byte

#define	VF_FMF_REACHMIN_INJCTL	            _V_INJCTL_STATUS.Bits.b0  //2015-12-26 ģ�͵�������������Сֵ

#define	VF_PUMP1ST_INJCTL	            _V_INJCTL_STATUS.Bits.b1  //2015-10-29  ��һ�α��Ͳ�����Ҫ��һЩ,2015-12-17

#define	VF_MISST4_AFTERMATCHHIGH_INJCTL	    _V_INJCTL_STATUS.Bits.b2    //2010-1-11 add 
#define	VF_T3NOTEXIST_INJCTL	            _V_INJCTL_STATUS.Bits.b3    //T3������
#define	VF_OPEN_INJCTL	                    _V_INJCTL_STATUS.Bits.b4
#define	VF_SHORT_INJCTL	                    _V_INJCTL_STATUS.Bits.b5
#define	VF_T3ENABLE_INJCTL	                _V_INJCTL_STATUS.Bits.b6
#define	VF_T3ER_INJCTL	                    _V_INJCTL_STATUS.Bits.b7     //t3����Χ

#define	VF_T3FKLST_INJCTL	        _V_INJCTLH_STATUS.Bits.b0
#define	VF_T3FKLIMIT_INJCTL	        _V_INJCTLH_STATUS.Bits.b1
#define	VF_T4SCATTER_INJCTL	    _V_INJCTLH_STATUS.Bits.b2    //t4ɢ���
#define	VF_T1SCANING_INJCTL	        _V_INJCTLH_STATUS.Bits.b3
#define	VF_T1SCANINGBREAK_INJCTL	_V_INJCTLH_STATUS.Bits.b4//2009-2-19 �Ƿ���Ҫ��;��ֹT1ɨ�裻
#define	VF_VBATFLU_WHENT4_INJCTL	        _V_INJCTLH_STATUS.Bits.b5//T4ɨ��ʱ����ѹ�в�����
#define	VF_T4SCANOK1TIME_INJCTL	    _V_INJCTLH_STATUS.Bits.b6//T4ɨ��ɹ�һ�Σ�
#define	VF_NEEDT35AGAIN_INJCTL	            _V_INJCTLH_STATUS.Bits.b7//��t3   2009-12-18
//2010-2-26
#define	VF_SEARCHT1I_INJCTL	        _V_INJCTLHL_STATUS.Bits.b0
#define	VF_SEARCHT4UP_INJCTL	    _V_INJCTLHL_STATUS.Bits.b1
#define	VF_SEARCHT4DOWN_INJCTL	    _V_INJCTLHL_STATUS.Bits.b2
#define	VF_T4UPGOT_INJCTL	        _V_INJCTLHL_STATUS.Bits.b3
#define	VF_T4DOWNGOT_INJCTL	        _V_INJCTLHL_STATUS.Bits.b4
#define	VF_T4LST_RELIABLE_INJCTL	        _V_INJCTLHL_STATUS.Bits.b5  //��һ��T4ƽ��ֵ�ɿ���
#define	VF_SEARCHAGAIN_INJCTL	        _V_INJCTLHL_STATUS.Bits.b6
#define	VF_PUMP_INJCTL	        _V_INJCTLHL_STATUS.Bits.b7  //2013-3-23 ADD 

extern volatile V_STATUS_STR _V_NOTYSTDC_STATUS;//@ADD_V_NOTYSTDC_STATUS;
#define V_NOTYSTDC_STATUS  _V_NOTYSTDC_STATUS.Byte
#define	VF_ADDINJ_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b0
#define	VF_ADDINJLST_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b1
#define	VF_ADDINJNEED_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b2	//�����־λ
#define	VF_NEEDDIVFMF_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b3  //2009-12-18 MOVE HERE ��������л���־
#define	VF_T35TESTNEED_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b4	//�Ƿ���ҪT35���
#define	VF_2INJATINJPHASE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b5   //2014-4-4����Ȧ�Ѿ�����������
#define	VF_LIDLE_ADVANCE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b6   //2013-5-16
#define	VF_4INJ_ENABLE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b7   //2012-9-14 

extern volatile V_STATUS_STR _V_INJPHASE_STATUS;//@ADD_V_INJPHASE_STATUS;
#define V_INJPHASE_STATUS  _V_INJPHASE_STATUS.Byte
#define	VF_FORINJ_INJPHASE	_V_INJPHASE_STATUS.Bits.b0
#define	VF_MODE1_INJPHASE	_V_INJPHASE_STATUS.Bits.b1	//��ѹ����ֹ������׶���ת��С��5250rpm
#define	VF_MODE1TO2_INJPHASE	_V_INJPHASE_STATUS.Bits.b2
#define	VF_SWITCHPHASE_INJPHASE	_V_INJPHASE_STATUS.Bits.b3
#define	VF_YSTDC_INJPHASE	_V_INJPHASE_STATUS.Bits.b4      //2012-10-21������ʱ����ΪFALSE
#define	VF_YSTDCGET_INJPHASE	_V_INJPHASE_STATUS.Bits.b5
#define	VF_2INJ_ENABLE_INJPHASE	_V_INJPHASE_STATUS.Bits.b6
#define	VF_3INJ_ENABLE_INJPHASE	_V_INJPHASE_STATUS.Bits.b7   //2012-8-7

extern volatile V_STATUS_STR _V_CAS_STATUS;//@ADD_V_CAS_STATUS;
#define V_CAS_STATUS   _V_CAS_STATUS.Byte

#define	VF_CAMAIN_TIMEOUT_CAS	_V_CAS_STATUS.Bits.b0 //2014-4-2
#define	VF_PHASEER_CAS	_V_CAS_STATUS.Bits.b1 //2009-12-27 ADD 
#define	VF_NOQY_CAS	_V_CAS_STATUS.Bits.b2
#define	VF_QYTUBIAN_CAS	_V_CAS_STATUS.Bits.b3
#define	VF_TUTAIER_CAS	_V_CAS_STATUS.Bits.b4   //2009-12-11 ADD 
#define	VF_CANUMER_CAS	_V_CAS_STATUS.Bits.b5
#define	VF_HAVEHY_CAS	_V_CAS_STATUS.Bits.b6
#define	VF_HYER_CAS	_V_CAS_STATUS.Bits.b7


//extern FLAG	FLAG1;
//#define	VF_OLDCA_CAS		FLAG1.b0	
//#define	VF_INJSTOP_HARD	FLAG1.b1

//extern volatile V_STATUS_STR _V_CASH_STATUS;//@ADD_V_CASH_STATUS;
//#define V_CASH_STATUS   _V_CASH_STATUS.Byte
extern volatile byte VF_OLDCA_CASbak;//		_V_CASH_STATUS.Bits.b0//�Ǳ굽��ʱ��FALSE,������ת�ٺ���TRUE;
extern volatile byte VF_OLDCAHY_CASbak;//		_V_CASH_STATUS.Bits.b1
extern volatile byte VF_PCAOV_CASbak;//		_V_CASH_STATUS.Bits.b2
extern volatile byte VF_TOOSPEED_CASbak;//		_V_CASH_STATUS.Bits.b3
extern volatile byte VF_MAINTMROV_CASbak;//		_V_CASH_STATUS.Bits.b4
extern volatile byte VF_CATCH_CASbak;//		_V_CASH_STATUS.Bits.b5
extern volatile byte VF_CATCHHY_CASbak;//		_V_CASH_STATUS.Bits.b6
extern volatile byte VF_NEWCYCLE_ATMAIN_CASbak;//		_V_CASH_STATUS.Bits.b7

#define	VF_OLDCA_CAS							VF_OLDCA_CASbak//		_V_CASH_STATUS.Bits.b0//�Ǳ굽��ʱ��FALSE,������ת�ٺ���TRUE;
#define	VF_OLDCAHY_CAS						VF_OLDCAHY_CASbak//		_V_CASH_STATUS.Bits.b1
#define	VF_PCAOV_CAS							VF_PCAOV_CASbak//		_V_CASH_STATUS.Bits.b2
#define	VF_TOOSPEED_CAS						VF_TOOSPEED_CASbak//		_V_CASH_STATUS.Bits.b3
#define	VF_MAINTMROV_CAS					VF_MAINTMROV_CASbak//		_V_CASH_STATUS.Bits.b4
#define	VF_CATCH_CAS							VF_CATCH_CASbak//		_V_CASH_STATUS.Bits.b5
#define	VF_CATCHHY_CAS						VF_CATCHHY_CASbak//		_V_CASH_STATUS.Bits.b6
#define	VF_NEWCYCLE_ATMAIN_CAS		VF_NEWCYCLE_ATMAIN_CASbak//		_V_CASH_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_CASHL_STATUS;//@ADD_V_CASHL_STATUS; //2014-4-15 ADD 
#define V_CASHL_STATUS                _V_CASHL_STATUS.Byte
#define	VF_CA_BEDIAGED_CAS          	_V_CASHL_STATUS.Bits.b0//һȦ���Ѿ���Ϲ��Ǳ���
#define	VF_CADIAG_TIMEIN_CAS	        _V_CASHL_STATUS.Bits.b1//һȦ�ڵ�����Ҫ���CA��ʱ����
#define	VF_CAHYPREDICT_TIMEIN_CAS   	_V_CASHL_STATUS.Bits.b2//Ԥ�����Ӧ�õ��˵�ʱ��
#define	VF_CAST_BESET_CAS   	        _V_CASHL_STATUS.Bits.b3//��׼CA��Ϊһ��ѭ���Ŀ�ʼ����׼CA���ú������λ
#define	VF_CASHY_BECHECK1_CAS   	        _V_CASHL_STATUS.Bits.b4//�Ѿ�У�˹�CAS�ĺ���
#define	VF_CASHY_BECHECK2_CAS   	        _V_CASHL_STATUS.Bits.b5//�Ѿ�У�˹�CAS�ĺ���
#define	VF_CASHY_BECHECK3_CAS   	        _V_CASHL_STATUS.Bits.b6//�Ѿ�У�˹�CAS�ĺ���VF_VRSLIMT_H
#define	VF_VRSLIMT_H   	        _V_CASHL_STATUS.Bits.b7//�Ǳ����޵��л�


extern volatile V_STATUS_STR _V_INTINJ_STATUS;//@ADD_V_INTINJ_STATUS;
#define V_INTINJ_STATUS    _V_INTINJ_STATUS.Byte
#define	VF_AT3GETAFTERSTART_INTINJ	_V_INTINJ_STATUS.Bits.b0
#define	VF_PINJB_INTINJ	_V_INTINJ_STATUS.Bits.b1
#define	VF_PINJE_INTINJ	_V_INTINJ_STATUS.Bits.b2
#define	VF_PTRIG_INTINJ	_V_INTINJ_STATUS.Bits.b3
#define	VF_T3GET_INTINJ	_V_INTINJ_STATUS.Bits.b4
#define	VF_T4GET_INTINJ	_V_INTINJ_STATUS.Bits.b5
#define	VF_T3BECHECK_INTINJ	_V_INTINJ_STATUS.Bits.b6//T3�����鲢������2010-3-3

extern volatile V_STATUS_STR _V_TMR_STATUS;//@ADD_V_TMR_STATUS;
#define V_TMR_STATUS   _V_TMR_STATUS.Byte
#define	VF_PRDMAINTMR_TMR	_V_TMR_STATUS.Bits.b0

extern volatile V_STATUS_STR _V_INTIGN_STATUS;//@ADD_V_INTIGN_STATUS;
#define V_INTIGN_STATUS    _V_INTIGN_STATUS.Byte
#define	VF_PREDRV_INTIGN	_V_INTIGN_STATUS.Bits.b0
#define	VF_BDRV_INTIGN	_V_INTIGN_STATUS.Bits.b1
#define	VF_BIGN_INTIGN	_V_INTIGN_STATUS.Bits.b2


#define	VF_IS_CDI_INTIGN	_V_INTIGN_STATUS.Bits.b3//2016-9-18 ADD 

//2014-1-28 peter,modified
extern volatile V_STATUS_STR _V_INTSCI_STATUS;//@ADD_V_INTSCI_STATUS;
#define V_INTSCI_STATUS    _V_INTSCI_STATUS.Byte
#define	VF_TRANSING_INTSCI	_V_INTSCI_STATUS.Bits.b0
#define	VF_FORPC_INTSCI	_V_INTSCI_STATUS.Bits.b1
#define	VF_LINKPC_INTSCI	_V_INTSCI_STATUS.Bits.b2
#define	VF_PCWANT_INTSCI	_V_INTSCI_STATUS.Bits.b3
#define VF_ADJUSTO_WHEN_AQ  _V_INTSCI_STATUS.Bits.b4//2010-3-23 ADD 
#define	VF_ADDCOM_INTSCI	_V_INTSCI_STATUS.Bits.b5 //2012-12-4

#define	VF_AFRAME_NEED_INTSCI	_V_INTSCI_STATUS.Bits.b6//��Ҫ����һ֡����

#define	VF_TOBE_DECODE_INTSCI	_V_INTSCI_STATUS.Bits.b7 //2013-9-3  �����������������


extern volatile V_STATUS_STR _V_MAPS_STATUS;//@ADD_V_MAPS_STATUS;
#define V_MAPS_STATUS  _V_MAPS_STATUS.Byte
#define	VF_ER_MAPS	_V_MAPS_STATUS.Bits.b0
#define	VF_MAX_MAPS	_V_MAPS_STATUS.Bits.b1
#define VF_HARDCFG_ERR_PAIRS 	_V_MAPS_STATUS.Bits.b2
#define VF_SOFTCFG_ERR_PAIRS 	_V_MAPS_STATUS.Bits.b3

extern volatile V_STATUS_STR _V_NOTIDLEMF_STATUS;//@ADD_V_NOTIDLEMF_STATUS;
#define V_NOTIDLEMF_STATUS _V_NOTIDLEMF_STATUS.Byte
#define	VF_REVDOWNMUCH_NOTIDLEMF	_V_NOTIDLEMF_STATUS.Bits.b0
#define	VF_REVRISEMUCH_NOTIDLEMF	_V_NOTIDLEMF_STATUS.Bits.b1
#define	VF_REVDECNOTIDLE_NOTIDLEMF	_V_NOTIDLEMF_STATUS.Bits.b2
#define	VF_REVINCNOTIDLE_NOTIDLEMF	_V_NOTIDLEMF_STATUS.Bits.b3

extern volatile V_STATUS_STR _V_OS_STATUS;//@ADD_V_OS_STATUS;
#define V_OS_STATUS    _V_OS_STATUS.Byte
#define	VF_NOTO2DEC_OS	_V_OS_STATUS.Bits.b0
#define	VF_O2DEC_OS	_V_OS_STATUS.Bits.b1
#define	VF_O2DEC2_OS	_V_OS_STATUS.Bits.b2 //2010-7-10
#define	VF_IDLELEAN1_OS	_V_OS_STATUS.Bits.b3 //2010-7-11   ����ʱ�Ѿ�ϡ��һ��


extern volatile V_STATUS_STR _V_OSDIAG_STATUS;//@ADD_V_OSDIAG_STATUS;
#define V_OSDIAG_STATUS    _V_OSDIAG_STATUS.Byte
extern volatile V_STATUS_STR _V_OSDIAGH_STATUS;//@ADD_V_OSDIAGH_STATUS;
#define V_OSDIAGH_STATUS    _V_OSDIAGH_STATUS.Byte
extern volatile V_STATUS_STR _V_OSDIAGHL_STATUS;//@ADD_V_OSDIAGHL_STATUS;
#define V_OSDIAGHL_STATUS    _V_OSDIAGHL_STATUS.Byte
#define	VF_O2NOTWORK_CF	_V_OSDIAG_STATUS.Bits.b0	//2018-7-6 �����ڼ��������������������״̬
#define	VF_O2CHANGED_OSDIAG	_V_OSDIAG_STATUS.Bits.b1	//��������OS��������һ��������6�Σ�
#define	VF_ENRICH_FORTESTLEAN_OSDIAG	_V_OSDIAG_STATUS.Bits.b2           //��Ũ�Բ����Ƿ����ƫϡ
#define	VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG	_V_OSDIAG_STATUS.Bits.b3    //2009-12-14 �ǵ��ٵ�os����ſ�
#define	VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG	_V_OSDIAG_STATUS.Bits.b4    //2009-12-14 ���ٵ�os����ſ�
#define	VF_LEANLIMIT_OSDIAG	_V_OSDIAG_STATUS.Bits.b5     //2009-12-13 ��ֹ��ϡ����
#define	VF_TESTFOROSLEAN_OSDIAG	_V_OSDIAG_STATUS.Bits.b6
#define	VF_O2WORK_OSDIAG	_V_OSDIAG_STATUS.Bits.b7

#define	VF_O2FKFAST_OS	_V_OSDIAGH_STATUS.Bits.b0       //2009-12-24
//#define	VF_OSMAYBECOOL_OSDIAG	_V_OSDIAGH_STATUS.Bits.b1
#define	VF_RCHO2MAX_OSDIAG	_V_OSDIAGH_STATUS.Bits.b2
#define	VF_RCHO2MIN_OSDIAG	_V_OSDIAGH_STATUS.Bits.b3
#define	VF_RICHLST_OSDIAG	_V_OSDIAGH_STATUS.Bits.b4
#define	VF_RICH_OSDIAG	_V_OSDIAGH_STATUS.Bits.b5	//������������ƫŨ
#define	VF_SCHO2MAX_OSDIAG	_V_OSDIAGH_STATUS.Bits.b6
#define	VF_SCHO2MIN_OSDIAG	_V_OSDIAGH_STATUS.Bits.b7

//2009-12-12 OS����ͳһ�����ڴ˴�����Կ��ʱ�������
//#define		_V_OSDIAGHL_STATUS.Bits.b0
#define	VF_OS_LEAN_WHENRICH_OSDIAG	_V_OSDIAGHL_STATUS.Bits.b1
#define	VF_FLUSMALL_OSDIAG	        _V_OSDIAGHL_STATUS.Bits.b2
#define	VF_CANNOTWORK_OSDIAG	    _V_OSDIAGHL_STATUS.Bits.b3
#define	VF_CANNOTFLU_OSDIAG	        _V_OSDIAGHL_STATUS.Bits.b4
#define	VF_OUPUT_WHENLEAN_OSDIAG	_V_OSDIAGHL_STATUS.Bits.b5
#define	VF_OSSHORTCUT_OSDIAG	    _V_OSDIAGHL_STATUS.Bits.b6
#define	VF_OSER_OSDIAG	            _V_OSDIAGHL_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_PAIR_STATUS;//@ADD_V_PAIR_STATUS;     //90-70=   32
#define V_PAIR_STATUS  _V_PAIR_STATUS.Byte
#define	VF_1PAIRSGOT_PAIR	_V_PAIR_STATUS.Bits.b0  //2013-4-24
#define	VF_ER_PAIR	_V_PAIR_STATUS.Bits.b1
#define	VF_PAIR_BYMANNUAL_PAIR	_V_PAIR_STATUS.Bits.b2  //�ֹ��趨
#define	VF_PAIR_VALID_FOREE_PAIR	_V_PAIR_STATUS.Bits.b3
#define	VF_NOTCAB_PAIR	_V_PAIR_STATUS.Bits.b4
#define	VF_VALUEERINEE_PAIR	_V_PAIR_STATUS.Bits.b5
#define	VF_ECUHARD_WITHPAIR_PAIR	_V_PAIR_STATUS.Bits.b6 //ECUӲ����ѹ��������
#define	VF_ECUHARD_WITHOUTPAIR_PAIR	_V_PAIR_STATUS.Bits.b7 //ECUӲ������ѹ��������

extern volatile V_STATUS_STR _V_REV_STATUS;//@ADD_V_REV_STATUS;
#define V_REV_STATUS   _V_REV_STATUS.Byte
#define	VF_CALCUED_REV	_V_REV_STATUS.Bits.b0
#define	VF_DEC_REV	_V_REV_STATUS.Bits.b1
#define	VF_UB_REV	_V_REV_STATUS.Bits.b2
#define	VF_GE2200_REV	_V_REV_STATUS.Bits.b3//2009-12-19
#define VF_PAIR_BESAMP_WHENMOVE_REV _V_REV_STATUS.Bits.b4//2010-5-11
#define VF_LE_IDLETG_ONCE_REV _V_REV_STATUS.Bits.b5//2013-4-19

#define	VF_GT10_IDLETG_REV	_V_REV_STATUS.Bits.b6//2013-4-7
#define	VF_GT30_IDLETG_REV	_V_REV_STATUS.Bits.b7//2013-4-7 �ߵ��٣��趨�ĵ���Ŀ��ת��+30*15.6


extern volatile V_STATUS_STR _V_STARTS_STATUS;//@ADD_V_STARTS_STATUS;
#define V_STARTS_STATUS    _V_STARTS_STATUS.Byte
#define	VF_1STSTART_STARTS	_V_STARTS_STATUS.Bits.b0//2012-11-24��һ������
#define	VF_LEAVEIDLE_FRE_ENG	_V_STARTS_STATUS.Bits.b2  //2010-6-4 ������Ƶ���뿪���� 
#define	VF_BE_PREINJ_ENG	_V_STARTS_STATUS.Bits.b3  //2012-8-6 

#define VF_REMAINIDLE_STARTS _V_STARTS_STATUS.Bits.b4  //2013-1-26������һֱ��������Ϊ����

extern volatile V_STATUS_STR _V_ENG_STATUS;//@ADD_V_ENG_STATUS;
#define V_ENG_STATUS   _V_ENG_STATUS.Byte

#define	VF_BFIRE_ENG	_V_ENG_STATUS.Bits.b0	//�Ż��־λ,true����û�Ż�
#define	VF_IDLE_ENG	_V_ENG_STATUS.Bits.b1	//�����ڵ��ٱ�־λVF_E0_TPS
#define	VF_INJRUN_ENG	_V_ENG_STATUS.Bits.b2
#define	VF_JBACKIDLE_ENG	_V_ENG_STATUS.Bits.b3	//���Ÿջص��ٱ�־λ�����Ŵӷǵ��ٵ�����ʱ��λ����һȦ����ʱ��λ
#define	VF_JMMOVE_ENG	_V_ENG_STATUS.Bits.b4
#define	VF_JBACKIDLE_LST_ENG	_V_ENG_STATUS.Bits.b5
#define	VF_JLEAVEIDLELST_ENG	_V_ENG_STATUS.Bits.b6     //�϶�����ECE   //2013-10-24 VF_SHOULDNOTBE_ECE_ENG ��VF_JLEAVEIDLELSTȡ��_ENG
#define	VF_JLEAVEIDLE_ENG	_V_ENG_STATUS.Bits.b7

//extern volatile V_STATUS_STR _V_ENGH_STATUS;//@ADD_V_ENGH_STATUS; //45
extern volatile byte VF_JLEAVEIDLETRANS_ENGbak;	//���뿪����ʱ
extern volatile byte VF_JSTOP_ENGbak;    //2014-3-30 ��ͣ��
extern volatile byte VF_MMOVE_ENGbak;
extern volatile byte VF_NOINJ_ENGbak;     //����Ҫ���ͣ��������ٶ��͵����
extern volatile byte VF_RUNTOOS_ENGbak;   //��λʱ����Ҫ�����ñ��
extern volatile byte VF_STARTS_ENGbak;
extern volatile byte VF_STOPBYTOOHOT_ENGbak;
extern volatile byte VF_TOIDLE_ENGbak;
//#define V_ENGH_STATUS   _V_ENGH_STATUS.Byte
#define	VF_JLEAVEIDLETRANS_ENG	VF_JLEAVEIDLETRANS_ENGbak	//���뿪����ʱ
#define	VF_JSTOP_ENG						VF_JSTOP_ENGbak    //2014-3-30 ��ͣ��
#define	VF_MMOVE_ENG						VF_MMOVE_ENGbak
#define	VF_NOINJ_ENG						VF_NOINJ_ENGbak     //����Ҫ���ͣ��������ٶ��͵����
#define	VF_RUNTOOS_ENG					VF_RUNTOOS_ENGbak   //��λʱ����Ҫ�����ñ��
#define	VF_STARTS_ENG						VF_STARTS_ENGbak
#define	VF_STOPBYTOOHOT_ENG			VF_STOPBYTOOHOT_ENGbak
#define	VF_TOIDLE_ENG						VF_TOIDLE_ENGbak

extern volatile V_STATUS_STR _V_ENGHL_STATUS;//@ADD_V_ENGHL_STATUS;
#define V_ENGHL_STATUS   _V_ENGHL_STATUS.Byte
#define	VF_TURNONINJ_ENG	_V_ENGHL_STATUS.Bits.b0
#define	VF_WORM_ENG	_V_ENGHL_STATUS.Bits.b1
#define	VF_WARMUP_ENG	_V_ENGHL_STATUS.Bits.b2    //2009-11-19  //����������ʧ���Ҹ�ͷ�¶ȴ���40ʱ��
#define VF_CONIDLE_MORETHAN3S_ENG _V_ENGHL_STATUS.Bits.b3//2009-12-13      �������ٳ���3s;
#define	VF_JLEAVEIDLETRANS_LST_ENG	_V_ENGHL_STATUS.Bits.b4  //2012-11-20 ADD
#define	VF_WORM_LST_ENG	_V_ENGHL_STATUS.Bits.b5
#define	VF_HOTSTART_ENG	_V_ENGHL_STATUS.Bits.b6//2009-12-31 ȡ���ߵ���
#define	VF_MAYBE_ECE_ENG	_V_ENGHL_STATUS.Bits.b7//2010-1-29 

extern volatile V_STATUS_STR _V_SYS_STATUS;//@ADD_V_SYS_STATUS;
#define V_SYS_STATUS   _V_SYS_STATUS.Byte
#define	VF_DOM_SYS	_V_SYS_STATUS.Bits.b0
#define	VF_MILELIMIT_WARN_SYS	_V_SYS_STATUS.Bits.b1  //2012-12-6 ,��̵��ڵľ���
#define	VF_MILELIMIT_DUE_SYS	_V_SYS_STATUS.Bits.b2            //2012-12-6����̵���
//#define VF_BEFOREINJ_INJPHASE _V_SYS_STATUS.Bits.b3 //2012-12-11
#define VF_BEINJ_1CYC_INJPHASE _V_SYS_STATUS.Bits.b3 //2012-12-11

//2013-4-26 ��ʱ�ƶ�����  2013-6-18
#define	VF_MAYDEC0_LDSENSOR	_V_SYS_STATUS.Bits.b4    //lcw2013-1-12
#define	VF_MAYDEC1_LDSENSOR	_V_SYS_STATUS.Bits.b5    //lcw2013-1-12
#define	VF_DIAG_FAI	_V_SYS_STATUS.Bits.b6
#define	VF_OBDVER_FAI	_V_SYS_STATUS.Bits.b7


//a0-70=48
extern volatile V_STATUS_STR _V_SYSINI_STATUS;//@ADD_V_SYSINI_STATUS;
#define V_SYSINI_STATUS    _V_SYSINI_STATUS.Byte
extern volatile V_STATUS_STR _V_SYSINIH_STATUS;//@ADD_V_SYSINIH_STATUS;
#define V_SYSINIH_STATUS    _V_SYSINIH_STATUS.Byte
extern volatile V_STATUS_STR _V_SYSINIHL_STATUS;//@ADD_V_SYSINIHL_STATUS; //A2-70=  //DEC50;
#define V_SYSINIHL_STATUS    _V_SYSINIHL_STATUS.Byte
extern volatile V_STATUS_STR _V_SYSINIHH_STATUS;//@ADD_V_SYSINIHH_STATUS;
#define V_SYSINIHH_STATUS    _V_SYSINIHH_STATUS.Byte

#define	VF_IDLECONDICTION_MATCH_SYSINI	_V_SYSINI_STATUS.Bits.b0 //��ʼ��״̬�£����㵡��������������2010-4-10
#define	VF_RSTTISCI_SYSINI          	_V_SYSINI_STATUS.Bits.b1
#define	VF_IDLEAIRTEST_SYSINI           _V_SYSINI_STATUS.Bits.b3  //�ڵ��ٴֵ���,����ǰ,���Ե��������Ƿ����
#define VF_ENAQ_SYSINI                  _V_SYSINI_STATUS.Bits.b4  //2009-9-11 ���ӣ���ʼ��״̬�£���ʼ���������ı�־
#define	VF_JFINISHINI_WHENMOVING_SYSINI _V_SYSINI_STATUS.Bits.b5  //2009-11-25 //������ٸճ�ʼ���꣬�ҷ����������������У����ø�λ��Ŀ�����ڴ�ʱ����ʾ����
#define	VF_AQAFTER_PNINI_SYSINI         _V_SYSINI_STATUS.Bits.b7  //2009-11-25 //������ٸճ�ʼ���꣬�ҷ����������������У����ø�λ��Ŀ�����ڴ�ʱ����ʾ����

#define	VF_TIFKOK_SYSINI	        _V_SYSINIH_STATUS.Bits.b0
#define	VF_TIOK_SYSINI              _V_SYSINIH_STATUS.Bits.b1
#define	VF_TISETOK_SYSINI	        _V_SYSINIH_STATUS.Bits.b2
#define	VF_PREINJEND_SYSINI     	_V_SYSINIH_STATUS.Bits.b3      //2008-12-16  MOVE HERE	Ԥ�������־
#define	VF_PREPUMPEND_SYSINI	    _V_SYSINIH_STATUS.Bits.b4  //2008-12-16  MOVE HERE	���ͽ�����־��ͣ��������ֹͣ��λ����Կ�׻������λʱ��λ
#define	VF_NOTNEED_DROPSCAN_SYSINI	_V_SYSINIH_STATUS.Bits.b5      //2008-12-16  MOVE HERE	��Կ����λ���Լ츴λ
#define	VF_PUMPING_SYSINI     	_V_SYSINIH_STATUS.Bits.b6      //2013-4-3 add
#define	VF_PREINJ_2CA     	_V_SYSINIH_STATUS.Bits.b7 

//ע�⣺
//V_SYSINIHL_STATUS��V_SYSINIHH_STATUS����λ���Ա��֣�
#define VF_NOINIT_SYSINI        _V_SYSINIHL_STATUS.Bits.b0  //δ����
#define VF_TIER_SYSINI          _V_SYSINIHL_STATUS.Bits.b1  
#define VF_AQOK_SYSINI          _V_SYSINIHL_STATUS.Bits.b2  // b7
#define	VF_PNNOINI_SYSINI	    _V_SYSINIHL_STATUS.Bits.b3     //2009-7-31  ��������δ��ʼ����־
#define	VF_ECUNOSET_SYSINI	    _V_SYSINIHL_STATUS.Bits.b4      //ECUû�б��
#define	VF_1STINI_FAIL_SYSINI	_V_SYSINIHL_STATUS.Bits.b6      //ǿ���Լ�ʧ�ܣ��ǵ�ѹԭ��

#define VF_RUNENOUGH_FORINI_SYSINI  _V_SYSINIHH_STATUS.Bits.b0  
#define VF_CLR_SL_SYSINI        _V_SYSINIHH_STATUS.Bits.b1  //2013-4-2
#define VF_1ST_INI_SYSINI           _V_SYSINIHH_STATUS.Bits.b4      //2010-3-5 ����Ϊ��Ҫ����ʱ��T4�ı�־��Ҳ��Ҫ����
#define VF_SEARCHTMP_SYSINI         _V_SYSINIHH_STATUS.Bits.b6  //��Ҫ����//2010-3-6
#define VF_NEEDSAVEGAMA_SYSINI      _V_SYSINIHH_STATUS.Bits.b7  


extern volatile V_STATUS_STR _V_TMCCL_STATUS;//@ADD_V_TMCCL_STATUS;
#define V_TMCCL_STATUS _V_TMCCL_STATUS.Byte
extern volatile V_STATUS_STR _V_TMCCLH_STATUS;//@ADD_V_TMCCLH_STATUS;
#define V_TMCCLH_STATUS _V_TMCCLH_STATUS.Byte
#define	VF_16S_TMCCL	_V_TMCCL_STATUS.Bits.b0
#define	VF_1S_TMCCL	_V_TMCCL_STATUS.Bits.b1
#define	VF_2S_TMCCL	_V_TMCCL_STATUS.Bits.b2
#define	VF_3OS_TMCCL	_V_TMCCL_STATUS.Bits.b3
#define	VF_4S_TMCCL	_V_TMCCL_STATUS.Bits.b4
#define	VF_64S_TMCCL	_V_TMCCL_STATUS.Bits.b5
#define	VF_8S_TMCCL	_V_TMCCL_STATUS.Bits.b6
#define	VF_15S_AFTERSTART_TMCCL	_V_TMCCL_STATUS.Bits.b7

#define	VF_RUN2_TMCCL	_V_TMCCLH_STATUS.Bits.b0
#define	VF_RUN4_TMCCL	_V_TMCCLH_STATUS.Bits.b1
#define	VF_RUN8_TMCCL	    _V_TMCCLH_STATUS.Bits.b2
#define	VF_RUN16_TMCCL	_V_TMCCLH_STATUS.Bits.b3
#define	VF_RUN64_TMCCL	_V_TMCCLH_STATUS.Bits.b4    //2010-1-30
#define	VF_RUN_MORE5S_TMCCL	_V_TMCCLH_STATUS.Bits.b5          //2012-8-22 ���г���5S
#define	VF_D1S_TMCCL	_V_TMCCLH_STATUS.Bits.b6	//2017-3-25 ����0.1s��־λ
#define	VF_MORE1MIN_TMCCL	_V_TMCCLH_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_TMP_STATUS;//@ADD_V_TMP_STATUS;
#define V_TMP_STATUS   _V_TMP_STATUS.Byte
#define	VF_1STGET_TMP	_V_TMP_STATUS.Bits.b0
#define	VF_TASER_TMP	_V_TMP_STATUS.Bits.b1
#define	VF_TESER_TMP	_V_TMP_STATUS.Bits.b2
#define	VF_TCHANGED_TMP	_V_TMP_STATUS.Bits.b3
#define	VF_TESSTABLE_TMP	_V_TMP_STATUS.Bits.b4 //2012-8-1 TES�����ȶ�
#define	VF_TESLTTASYC_TMP	_V_TMP_STATUS.Bits.b5 //2012-8-1 TES��Ԥ���TASС
#define	VF_TESMAYERR_TMP	_V_TMP_STATUS.Bits.b6   //2012-8-2
#define	VF_TENG_FORECE_TMP	_V_TMP_STATUS.Bits.b7   //2008-12-16 ADD 

extern volatile V_STATUS_STR _V_TPS_STATUS;//@ADD_V_TPS_STATUS;
extern volatile V_STATUS_STR _V_TPSH_STATUS;//@ADD_V_TPSH_STATUS;
#define V_TPS_STATUS   _V_TPS_STATUS.Byte
#define	VF_E0LAST_TPS	_V_TPS_STATUS.Bits.b0
#define	VF_ER_TPS	_V_TPS_STATUS.Bits.b1
#define	VF_MAX_TPS	_V_TPS_STATUS.Bits.b2	//������ȫ��״̬
#define	VF_UB_TPS	_V_TPS_STATUS.Bits.b3
#define	VF_WOT_TPS	_V_TPS_STATUS.Bits.b4
#define	VF_MAXLST_TPS	_V_TPS_STATUS.Bits.b5
#define	VF_DRYIGNI_TPS	_V_TPS_STATUS.Bits.b6  //2008-12-16
#define	VF_E0_TPS	_V_TPS_STATUS.Bits.b7  //2009-1-13     TPS=0
//2009-9-7 ADD
#define V_TPSH_STATUS   _V_TPSH_STATUS.Byte
#define	VF_INCTHAN6_TPS	    _V_TPSH_STATUS.Bits.b0

#define	VF_E0MORETHAN4S_TPS	    _V_TPSH_STATUS.Bits.b1  //�����ڵ��ٵ�ʱ�䳬��4s
#define	VF_NOTIDLE_LE3_TPS	    _V_TPSH_STATUS.Bits.b2  //�����뿪���ٺ�,
#define	VF_TPSINCLE0P1_TPS	    _V_TPSH_STATUS.Bits.b3  //��������С��0.1��//2010-1-11
#define	VF_VALID_TPS	    _V_TPSH_STATUS.Bits.b4  //2013-4-8

#define	VF_TPS0_MIN_GOTTEN1ST_TPS	    _V_TPSH_STATUS.Bits.b5  //2013-4-14��һ�λ��TPS��Сֵ     
#define	VF_TPS_AFTER_GOTTEN1ST_TPS	    _V_TPSH_STATUS.Bits.b6  //2013-4-14 atfer ��һ�λ��TPS��Сֵ
#define	VF_NEEDADDFUEL_CR	    _V_TPSH_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_VBAT_STATUS;//@ADD_V_VBAT_STATUS;
#define V_VBAT_STATUS  _V_VBAT_STATUS.Byte
#define	VF_ER_REGU	_V_VBAT_STATUS.Bits.b0

#define	VF_CURRENT_ER_VBAT	_V_VBAT_STATUS.Bits.b1
#define	VF_ER_VBAT	_V_VBAT_STATUS.Bits.b2
#define	VF_L5V_VBAT	_V_VBAT_STATUS.Bits.b3
#define	VF_SAMPED_VBAT	_V_VBAT_STATUS.Bits.b5
#define	VF_VDOWNGT3_VBAT	_V_VBAT_STATUS.Bits.b6
#define	VF_NOFUSE_VBAT	_V_VBAT_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_STOP_STATUS;//@ADD_V_STOP_STATUS;
#define V_STOP_STATUS  _V_STOP_STATUS.Byte

#define	VF_1COM_STOP	_V_STOP_STATUS.Bits.b1 //2014-4-8 ADD 

#define	VF_SAMPATIME_STOP	_V_STOP_STATUS.Bits.b2
#define	VF_2NDCA_STOP	_V_STOP_STATUS.Bits.b3

#define	VF_FMFADDNEED_PHASE_STOP	_V_STOP_STATUS.Bits.b4   //2012-12-4 ADD 

//#define	VF_NOTNEED_DROPSCAN_SYSINI	_V_STOP_STATUS.Bits.b4
#define	VF_INJENDWHENSTOP_STOP	_V_STOP_STATUS.Bits.b5
#define	VF_3RDCA_STOP	_V_STOP_STATUS.Bits.b6
#define	VF_STARTFROMHIGHSPEED_STOP	_V_STOP_STATUS.Bits.b7  //2009-12-25 

extern volatile V_STATUS_STR _V_ETC_STATUS;//@ADD_V_ETC_STATUS;
#define V_ETC_STATUS  _V_ETC_STATUS.Byte
#define	VF_RUN_ETC	_V_ETC_STATUS.Bits.b0  //ETC is running
#define	VF_FAILURE_ETC	_V_ETC_STATUS.Bits.b1  //ETC ���ܹ�����2012-11-2
#define	VF_LUBEMPTY_ETC	_V_ETC_STATUS.Bits.b2  //�޻��ͣ�2012-11-2

//#define VF_TOBE_RUN	_V_ETC_STATUS.Bits.b3 
#define VF_BYTPD_ETC	_V_ETC_STATUS.Bits.b3  //��Ŀ�����ſ��ƣ�2015-12-16
#define VF_INC_ETC	_V_ETC_STATUS.Bits.b4//2012-11-14 
#define VF_ACC_ETC	_V_ETC_STATUS.Bits.b5//2012-11-14//���ͻȻ�������� 
#define VF_INC_LST_ETC	_V_ETC_STATUS.Bits.b6//2012-11-14//�����һ������
#define VF_DEC_ETC	_V_ETC_STATUS.Bits.b7//2012-11-14//���ͻȻ����������� 
 
extern volatile V_STATUS_STR _V_LDSENSOR_STATUS;//@ADD_V_LOADSENSOR_STATUS;    //ae-70=62
#define V_LDSENSOR_STATUS  _V_LDSENSOR_STATUS.Byte
#define	VF_CAP0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b0
#define	VF_CAP1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b1

#define	VF_OPEN0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b2
#define	VF_OPEN1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b3

#define	VF_MAYACC0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b4//���ɿ�����ͻ��
#define	VF_MAYACC1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b5

#define	VF_INC_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b6
#define	VF_DEC_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b7//����ͻ��

extern volatile V_STATUS_STR _V_LOCK_STATUS;//@ADD_V_LOCK_STATUS;    //2013-6-13 ������������
#define V_LOCK_STATUS         _V_LOCK_STATUS.Byte
#define	VF_SETLOCK_LOCK	      _V_LOCK_STATUS.Bits.b0      //����������������  1 for yes
#define	VF_KEYON_LOCK	        _V_LOCK_STATUS.Bits.b1      //���ǲ��Ǳ��򿪣�  1 for yes
#define	VF_SETLIMIT_LOCK	    _V_LOCK_STATUS.Bits.b2      //�Ƿ�������Ȩʹ�ã�1 for yes
#define	VF_BKEYERR_LOCK	      _V_LOCK_STATUS.Bits.b3      //�����Ƿ����    1 for yes
#define	VF_LIMITDUE_LOCK	    _V_LOCK_STATUS.Bits.b4      //��Ȩʹ���Ƿ��ڣ�1 for yes
#define	VF_BPERMITERR_LOCK	  _V_LOCK_STATUS.Bits.b5      //��Ȩ�������    1 for yes

#define	VF_PWNOINI_LOCK	      _V_LOCK_STATUS.Bits.b6      //û�����ù���ʼ���� ,//�Ƿ�ʹ������ 2014-6-24 ���ĺ���

#define	VF_MODE_INTSCI	      _V_LOCK_STATUS.Bits.b7      //2013-9-3  0�����ģʽ��1���Ƿ���ģʽ
#define COMMODE_DIAG  0
#define COMMODE_LOCK  1

extern volatile V_STATUS_STR _V_LOCK2_STATUS;//@ADD_V_LOCK2_STATUS;    //2013-6-13 ������������
#define V_LOCK2_STATUS         _V_LOCK2_STATUS.Byte


#define	VF_EEPWS_VALID_LOCK	              _V_LOCK2_STATUS.Bits.b0        //ECU�д洢���û�������Ч
#define	VF_EEPWL_VALID_LOCK	              _V_LOCK2_STATUS.Bits.b1         //ECU�д洢�ľ�����������Ч
#define	VF_EESTTIME_VALID_LOCK	          _V_LOCK2_STATUS.Bits.b2       //ECU�д洢����ʼʱ����Ч
#define	VF_EEEDTIME_VALID_LOCK	          _V_LOCK2_STATUS.Bits.b3       //ECU�д洢�Ľ���ʱ����Ч

#define	VF_PWIN_LOCK	                    _V_LOCK2_STATUS.Bits.b4       //2014-5-31 �Ѿ����������� 
#define	VF_NOTACTIVE_LOCK	                _V_LOCK2_STATUS.Bits.b5       //2014-5-8 ADD FOR,ECUû�б����� 
#define	VF_KEYON_FOR1STLOCK_LOCK	        _V_LOCK2_STATUS.Bits.b6       //��һ����������ʱ����ҪԿ���������������������������Կ��
#define	VF_KEYOFF_SHORT_LOCK	            _V_LOCK2_STATUS.Bits.b7       //Կ�׹ر��㹻��ʱ�䣬��Ҫ���½���


extern volatile V_STATUS_STR _V_ETC3_STATUS_RPMto_H;//@ADD_V_ETC3_STATUS;  //B2
//#define V_ETC3_STATUS  _V_ETC3_STATUS.Byte
#define V_ETC3_STATUS_RPMto_H  _V_ETC3_STATUS_RPMto_H.Byte

#define	VF_RPM_TO_H0	_V_ETC3_STATUS_RPMto_H.Bits.b0     // LCW2013-1-19 RPM�����ﵽ��һ����ת�ٵ㣬������ĳֵʱ���             
#define	VF_RPM_TO_H1	_V_ETC3_STATUS_RPMto_H.Bits.b1                   //
#define	VF_RPM_TO_H2	_V_ETC3_STATUS_RPMto_H.Bits.b2                   //
#define	VF_RPM_TO_H3	_V_ETC3_STATUS_RPMto_H.Bits.b3                   //
#define	VF_RPM_TO_H4	_V_ETC3_STATUS_RPMto_H.Bits.b4                   //
#define	VF_RPM_TO_H5	_V_ETC3_STATUS_RPMto_H.Bits.b5                   //
#define	VF_RPM_TO_H6	_V_ETC3_STATUS_RPMto_H.Bits.b6                   //
#define	VF_RPM_TO_H7	_V_ETC3_STATUS_RPMto_H.Bits.b7                   //



extern volatile V_STATUS_STR _V_ETC4_STATUS_RPMto_L;//@ADD_V_ETC4_STATUS;  //B3
//#define V_ETC4_STATUS  _V_ETC4_STATUS.Byte
#define V_ETC4_STATUS_RPMto_L  _V_ETC4_STATUS_RPMto_L.Byte

#define	VF_ETC4_0_ETC	_V_ETC4_STATUS_RPMto_L.Bits.b0
#define VF_RPM_TO_L0  _V_ETC4_STATUS_RPMto_L.Bits.b0 // LCW2013-1-19 RPM�½��ﵽ��һ����ת�ٵ㣬������ĳֵ�����
#define VF_RPM_TO_L1  _V_ETC4_STATUS_RPMto_L.Bits.b1
#define VF_RPM_TO_L2  _V_ETC4_STATUS_RPMto_L.Bits.b2
#define VF_RPM_TO_L3  _V_ETC4_STATUS_RPMto_L.Bits.b3
#define VF_RPM_TO_L4  _V_ETC4_STATUS_RPMto_L.Bits.b4
#define VF_RPM_TO_L5  _V_ETC4_STATUS_RPMto_L.Bits.b5
#define VF_RPM_TO_L6  _V_ETC4_STATUS_RPMto_L.Bits.b6
#define VF_RPM_TO_L7  _V_ETC4_STATUS_RPMto_L.Bits.b7


extern volatile V_STATUS_STR _V_I_CAP_STATUS;//@ADD_V_I_CAP_STATUS;  //B4 2013-1-22
//#define V_ETC4_STATUS  _V_ETC4_STATUS.Byte
#define V_I_CAP_STATUS  _V_I_CAP_STATUS.Byte
#define	VF_I_CAP_lst	_V_I_CAP_STATUS.Bits.b0
#define	VF_I2_CAP_lst	_V_I_CAP_STATUS.Bits.b1
#define	VF_I_CAP_Twns_lst	_V_I_CAP_STATUS.Bits.b2
#define	VF_I_CAP_Twns	_V_I_CAP_STATUS.Bits.b3
#define	VF_I2_CAP_Twns_lst	_V_I_CAP_STATUS.Bits.b4
#define	VF_I2_CAP_Twns	_V_I_CAP_STATUS.Bits.b5
#define	VF_LUBEMPTY_ETC_lst	_V_I_CAP_STATUS.Bits.b6 

//2013-4-23 DEFINETION FOR ECU HARD SETTING OF SOFTWARE
extern volatile V_STATUS_STR _V_PRO2_F5_STATUS;//@ADD_PRO2_F5_STATUS;    //ae-70=62
#define V_PRO2_F5_STATUS       _V_PRO2_F5_STATUS.Byte
#define	VF_WITHMAP_PRO2	    _V_PRO2_F5_STATUS.Bits.b7
#define	VF_WITHPAIRS_PRO2	  _V_PRO2_F5_STATUS.Bits.b6
#define	VF_WITHPN_PRO2	    _V_PRO2_F5_STATUS.Bits.b5
#define	VF_WITHOIL_PRO2	    _V_PRO2_F5_STATUS.Bits.b4
#define	VF_WITHO2_PRO2	    _V_PRO2_F5_STATUS.Bits.b3
#define	VF_WITHISCV_PRO2	  _V_PRO2_F5_STATUS.Bits.b2
#define	VF_RESERVE1_PRO2	  _V_PRO2_F5_STATUS.Bits.b1
#define	VF_RESERVE0_PRO2	  _V_PRO2_F5_STATUS.Bits.b0


extern volatile V_STATUS_STR _V_PRO1_F6_STATUS;//@ADD_PRO1_F6_STATUS;    //ae-70=62
#define V_PRO1_F6_STATUS       _V_PRO1_F6_STATUS.Byte
#define	VF_INDUCTION_PRO1	    _V_PRO1_F6_STATUS.Bits.b7
#define	VF_DCMOTOR_PRO1	      _V_PRO1_F6_STATUS.Bits.b6
#define	VF_STARTER_PRO1	      _V_PRO1_F6_STATUS.Bits.b5
#define	VF_STEPMOTOR_PRO1	    _V_PRO1_F6_STATUS.Bits.b4
#define	VF_HBRIDGE_PRO1	      _V_PRO1_F6_STATUS.Bits.b3
#define	VF_BLUETEETH_PRO1	    _V_PRO1_F6_STATUS.Bits.b2
#define	VF_KWP_PRO1	          _V_PRO1_F6_STATUS.Bits.b1
#define	VF_RESERVE0_PRO1	    _V_PRO1_F6_STATUS.Bits.b0

//2014-4-13
extern volatile V_STATUS_STR _V_IGN2_STATUS;//@ADD_V_IGN2_STATUS;
#define V_IGN2_STATUS   _V_IGN2_STATUS.Byte
#define	VF_NEED_CHARGE_IGN	_V_IGN2_STATUS.Bits.b0
#define	VF_OPEN_IGN	_V_IGN2_STATUS.Bits.b1
#define	VF_SHORT_IGN	_V_IGN2_STATUS.Bits.b2
#define	VF_ISCHARGE_IGN	_V_IGN2_STATUS.Bits.b3  //2014-4-28
#define	VF_IAD_BEBACKUP_AD	_V_IGN2_STATUS.Bits.b4  //2014-12-24 �����������ֵ
//---2017-12-9 ysl add FOR PI-EAEA0122
#define	VF_OC_33814IGN	_V_IGN2_STATUS.Bits.b5

//2014-4-28
extern volatile V_STATUS_STR _V_AD_STATUS;//@ADD_V_AD_STATUS;
#define V_AD_STATUS         _V_AD_STATUS.Byte
#define	VF_TPS_ED_AD	      _V_AD_STATUS.Bits.b0
#define	VF_VBAT_ED_AD 	    _V_AD_STATUS.Bits.b1
#define	VF_OS_ED_AD	        _V_AD_STATUS.Bits.b2
#define	VF_PAIRS_ED_AD	    _V_AD_STATUS.Bits.b3
#define	VF_TES_ED_AD	      _V_AD_STATUS.Bits.b4
#define	VF_TAS_ED_AD	      _V_AD_STATUS.Bits.b5
#define	VF_FORENERGY_AD	      _V_AD_STATUS.Bits.b6 //2014-12-24 AD FOR ENERGY
#define	VF_INJ_SET	      _V_AD_STATUS.Bits.b7


extern volatile V_STATUS_STR _V_FLASH_STATUS;//@ADD_V_FLASH_STATUS;  //2014-5-17
#define V_FLASH_STATUS         _V_FLASH_STATUS.Byte
#define	VF_MAP_INEE_FLASH	      _V_FLASH_STATUS.Bits.b0
#define	VF_MAY_UPDATE_FLASH	      _V_FLASH_STATUS.Bits.b1 //2014-8-13 Ҳ����յ������ź���
#define	VF_ISBOOTED_FLASH	      _V_FLASH_STATUS.Bits.b2 //2014-12-4 �ձ���������

extern volatile V_STATUS_STR _V_FUELCOMP_STATUS;//@ADD_V_FUELCOMP_STATUS;
#define V_FUELCOMP_STATUS   _V_FUELCOMP_STATUS.Byte
#define VF_RPDACC_ST	_V_FUELCOMP_STATUS.Bits.b0//2018-6-27 ������״̬
#define VF_IDLEFUELCO_ST	_V_FUELCOMP_STATUS.Bits.b1//2018-6-27 �����ź������������ڼ���λ�������ָ���Բ�����ɺ�λ
#define VF_U_CO	_V_FUELCOMP_STATUS.Bits.b2
#define VF_ADDFUEL_SET	_V_FUELCOMP_STATUS.Bits.b3
#define VF_QD_ST	_V_FUELCOMP_STATUS.Bits.b4
#define VF_CC_ST	_V_FUELCOMP_STATUS.Bits.b5		//20210819 chenwei add ���Ϩ�𿪹أ��˿ڽ�ͨ������һ��ʱ��
#define VF_CC_CONNECT	_V_FUELCOMP_STATUS.Bits.b6		//20210819 chenwei add ���Ϩ�𿪹ض˿�

/*---------------------2015-6-4 ����SPN---------------------------------*/
    extern volatile V_STATUS_STR _V_SPN0_STATUS;//@ADD_V_SPN0_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN1_STATUS;//@ADD_V_SPN1_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN2_STATUS;//@ADD_V_SPN2_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN3_STATUS;//@ADD_V_SPN3_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN4_STATUS;//@ADD_V_SPN4_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN5_STATUS;//@ADD_V_SPN5_STATUS;  //2014-6-4
    extern volatile V_STATUS_STR _V_SPN6_STATUS;//@ADD_V_SPN6_STATUS;  //2014-10-15
    extern volatile V_STATUS_STR _V_SPN7_STATUS;//@ADD_V_SPN7_STATUS;  //2014-10-15

    #define V_SPN0_STATUS         _V_SPN0_STATUS.Byte
    #define V_SPN1_STATUS         _V_SPN1_STATUS.Byte
    #define V_SPN2_STATUS         _V_SPN2_STATUS.Byte
    #define V_SPN3_STATUS         _V_SPN3_STATUS.Byte
    #define V_SPN4_STATUS         _V_SPN4_STATUS.Byte
    #define V_SPN5_STATUS         _V_SPN5_STATUS.Byte
    #define V_SPN6_STATUS         _V_SPN6_STATUS.Byte
    #define V_SPN7_STATUS         _V_SPN7_STATUS.Byte

/*
    //�������仯�ˣ���ͬ����NUM_DTC_SCR��M1_SPN��M1_FMI��SCRDTC_ST��Ҫһ���
        #define VF_VOLT_U_SPN  _V_SPN0_STATUS.Bits.b0
        #define VF_VOLT_L_SPN  _V_SPN0_STATUS.Bits.b1
        #define VF_NOTBECAL_SPN  _V_SPN0_STATUS.Bits.b2
        #define VF_PUMP_E_SPN  _V_SPN0_STATUS.Bits.b3
        #define VF_PUMP_M_SPN  _V_SPN0_STATUS.Bits.b4
        #define VF_ACU_SPN  _V_SPN0_STATUS.Bits.b5
        #define VF_FROZEN_SPN  _V_SPN0_STATUS.Bits.b6
        #define VF_AIRV_POWER_SPN  _V_SPN0_STATUS.Bits.b7
        
        #define VF_AIRV_GND_SPN  _V_SPN1_STATUS.Bits.b0
        #define VF_AIRV_OPEN_SPN  _V_SPN1_STATUS.Bits.b1
        #define VF_PRESSURE_SPN  _V_SPN1_STATUS.Bits.b2
        #define VF_ACUEE_SPN  _V_SPN1_STATUS.Bits.b3
        #define VF_PUMP2_E_SPN  _V_SPN1_STATUS.Bits.b4
        #define VF_PUMP2_M_SPN  _V_SPN1_STATUS.Bits.b5
        #define VF_PUMP2_DEICING_SPN  _V_SPN1_STATUS.Bits.b6
        #define VF_NOLINK_DCU_SPN  _V_SPN1_STATUS.Bits.b7
        
        #define VF_ADB_LB_SPN  _V_SPN2_STATUS.Bits.b0
        #define VF_ADB_LLB_SPN  _V_SPN2_STATUS.Bits.b1
        #define VF_ADB_LLLB_SPN  _V_SPN2_STATUS.Bits.b2
        #define VF_ADB_ERR_SPN  _V_SPN2_STATUS.Bits.b3
        #define VF_ADB_UU_SPN  _V_SPN2_STATUS.Bits.b4
        #define VF_ADB_UL_SPN  _V_SPN2_STATUS.Bits.b5
        
        #define VF_TU_ERR_SPN  _V_SPN3_STATUS.Bits.b0
        #define VF_TU_UU_SPN  _V_SPN3_STATUS.Bits.b1
        #define VF_TU_UL_SPN  _V_SPN3_STATUS.Bits.b2
        #define VF_TU_DEICING_SPN  _V_SPN3_STATUS.Bits.b3
        #define VF_COOLV_ERR_SPN  _V_SPN3_STATUS.Bits.b4
        #define VF_TEIN_U_SPN  _V_SPN3_STATUS.Bits.b5
        #define VF_TEIN_L_SPN  _V_SPN3_STATUS.Bits.b6
        #define VF_TEOUT_U_SPN  _V_SPN3_STATUS.Bits.b7
        
        #define VF_TEOUT_L_SPN  _V_SPN4_STATUS.Bits.b0
        #define VF_NOX_HEATER_SPN  _V_SPN4_STATUS.Bits.b1
        #define VF_NOX_U_SPN  _V_SPN4_STATUS.Bits.b2
        #define VF_NOX_VALUE_SPN  _V_SPN4_STATUS.Bits.b3
        #define VF_NOX_LINK_SPN  _V_SPN4_STATUS.Bits.b4
        #define VF_EXH_U_SPN  _V_SPN4_STATUS.Bits.b5
        #define VF_EXH_UU_SPN  _V_SPN4_STATUS.Bits.b6
        #define VF_CATALYST_SPN  _V_SPN4_STATUS.Bits.b7
        
        #define VF_NOLINK_ECU_SPN  _V_SPN5_STATUS.Bits.b0
        #define VF_NOLINK_ACU_SPN  _V_SPN5_STATUS.Bits.b1

        #define VF_NOPAMB_SPN  _V_SPN5_STATUS.Bits.b2//2015-8-11 ADD 				
        #define VF_NOTAMB_SPN  _V_SPN5_STATUS.Bits.b3				
        #define VF_NOET1_SPN  _V_SPN5_STATUS.Bits.b4
        
        //2015-10-15				
        #define VF_EXH_U_EGROFF_SPN  _V_SPN5_STATUS.Bits.b5				
        #define VF_EXH_U_EGRFAIL_SPN  _V_SPN5_STATUS.Bits.b6				
        #define VF_EXH_U_DEF_COMSUMELITTLE_SPN  _V_SPN5_STATUS.Bits.b7//				
        #define VF_EXH_U_DEF_QUALITYLOW_SPN  _V_SPN6_STATUS.Bits.b0				
        #define VF_EXH_U_DEF_OFF_SPN  _V_SPN6_STATUS.Bits.b1				  //
        #define VF_EXH_U_DEF_LACK_SPN  _V_SPN6_STATUS.Bits.b2				  //
        #define VF_EXH_UU_EGROFF_SPN  _V_SPN6_STATUS.Bits.b3				
        #define VF_EXH_UU_EGRFAIL_SPN  _V_SPN6_STATUS.Bits.b4				
        #define VF_EXH_UU_DEF_COMSUMELITTLE_SPN  _V_SPN6_STATUS.Bits.b5//				
        #define VF_EXH_UU_DEF_QUALITYLOW_SPN  _V_SPN6_STATUS.Bits.b6				
        #define VF_EXH_UU_DEF_OFF_SPN  _V_SPN6_STATUS.Bits.b7				  //
        #define VF_EXH_UU_DEF_LACK_SPN  _V_SPN7_STATUS.Bits.b0				//
        #define VF_EXH_U_CATALYST_AGING_SPN  _V_SPN7_STATUS.Bits.b1				
 */
 
    //PIϵͳ��2016-2-23
       #define VF_PAMB_LB_SPN  _V_SPN0_STATUS.Bits.b0				
      #define VF_PAMB_UB_SPN  _V_SPN0_STATUS.Bits.b1				
      #define VF_TAMB_C_SPN  _V_SPN0_STATUS.Bits.b2				
      #define VF_TAMB_B_SPN  _V_SPN0_STATUS.Bits.b3				
      #define VF_TAMB_SHORT_SPN  _V_SPN0_STATUS.Bits.b4				
      #define VF_TAMB_OPEN_SPN  _V_SPN0_STATUS.Bits.b5				
      #define VF_TAMB_WIRE_SPN  _V_SPN0_STATUS.Bits.b6				
      #define VF_TES_WIRE_SPN  _V_SPN0_STATUS.Bits.b7				
      #define VF_TES_B_SPN  _V_SPN1_STATUS.Bits.b0				
      #define VF_TES_LB_SPN  _V_SPN1_STATUS.Bits.b1				
      #define VF_TES_UB_SPN  _V_SPN1_STATUS.Bits.b2				
      #define VF_TES_C_SPN  _V_SPN1_STATUS.Bits.b3				
      #define VF_TPS_LB_SPN  _V_SPN1_STATUS.Bits.b4				
      #define VF_TPS_UB_SPN  _V_SPN1_STATUS.Bits.b5				
      #define VF_TPS_OPEN_SPN  _V_SPN1_STATUS.Bits.b6				
      #define VF_TPS_B_SPN  _V_SPN1_STATUS.Bits.b7				
      #define VF_TPS_C_SPN  _V_SPN2_STATUS.Bits.b0				
      #define VF_OS_WIRE_SPN  _V_SPN2_STATUS.Bits.b1				
      #define VF_OS_SHORT_SPN  _V_SPN2_STATUS.Bits.b2				
      #define VF_OS_OPEN_SPN  _V_SPN2_STATUS.Bits.b3				
      #define VF_OS_SLOW_SPN  _V_SPN2_STATUS.Bits.b4				
      #define VF_OS_C_SPN  _V_SPN2_STATUS.Bits.b5				
      #define VF_OS_HEATER_SPN  _V_SPN2_STATUS.Bits.b6				
      #define VF_PN_WIRE_SPN  _V_SPN2_STATUS.Bits.b7	
	  //#define VF_SHORT_33814INJ1CTL  _V_SPN2_STATUS.Bits.b7
      #define VF_PN_T3FK_SPN  _V_SPN3_STATUS.Bits.b0				
      #define VF_MISSFIRE_SPN  _V_SPN3_STATUS.Bits.b1				
      #define VF_IGN_COIL_SPN  _V_SPN3_STATUS.Bits.b2				
      #define VF_CAS_NO_SPN  _V_SPN3_STATUS.Bits.b3				
      #define VF_CAS_EMI_SPN  _V_SPN3_STATUS.Bits.b4				
      #define VF_IGN_SPN  _V_SPN3_STATUS.Bits.b5				
      #define VF_IDLE_LOW_SPN  _V_SPN3_STATUS.Bits.b6				
      #define VF_IDLE_NO_SPN  _V_SPN3_STATUS.Bits.b7				
      #define VF_VBAT_LB_SPN  _V_SPN4_STATUS.Bits.b0				
      #define VF_VBAT_UB_SPN  _V_SPN4_STATUS.Bits.b1				
      #define VF_ECU_NONUM_SPN  _V_SPN4_STATUS.Bits.b2				
      #define VF_ECU_EEPROM_SPN  _V_SPN4_STATUS.Bits.b3				
      #define VF_ETC_SPN  _V_SPN4_STATUS.Bits.b4				
      #define VF_LOAD_SPN  _V_SPN4_STATUS.Bits.b5
	  #define VF_OPEN_33814PUMPCTL  _V_SPN4_STATUS.Bits.b6
	  #define VF_SHORT_33814PUMPCTL  _V_SPN4_STATUS.Bits.b7
	  #define VF_OC_33814PUMPCTL  _V_SPN5_STATUS.Bits.b0
	  #define VF_IACV_OPEN  _V_SPN5_STATUS.Bits.b1
	  #define VF_IACV_SHORT  _V_SPN5_STATUS.Bits.b2
	  #define VF_SHORT_33814INJ1CTL  _V_SPN5_STATUS.Bits.b3
	  #define VF_OC_33814INJ1CTL  _V_SPN5_STATUS.Bits.b4
	  #define VF_IACS_ERR  _V_SPN5_STATUS.Bits.b5    //20210308 chenwei add ���ٿ�������ϵͳ����P0505
	  #define VF_MIL_OCOROPEN  _V_SPN5_STATUS.Bits.b6    //20210308 chenwei add MIL�������ӹ���
	  #define VF_O2H_OPEN  _V_SPN5_STATUS.Bits.b7    //20210308 chenwei add O3H�̽ӵ������·����
	  #define VF_O2H_OC  _V_SPN6_STATUS.Bits.b0    //20210308 chenwei add O3H�̽ӵ���������
	  //#define VF_INI_SYSINI	_V_SPN6_STATUS.Bits.b1	//20211004 chenwei add ���ɳ�ʼ�� 
	  #define VF_CA_NOSIGNAL	_V_SPN6_STATUS.Bits.b1	//20211009 chenwei add �޽Ǳ��źŹ���

     /*---2017-12-9 ysl add FOR PI-EAEA0122------------*/
extern volatile V_STATUS_STR          _V_33814_STATUS;//@ADD_V_33814_STATUS;
#define V_33814_STATUS                _V_33814_STATUS.Byte
#define	VF_33814DIAG_RESET	          _V_33814_STATUS.Bits.b0
#define	VF_33814DIAG_GEN  	          _V_33814_STATUS.Bits.b1
#define	VF_33814DIAGT_GEN  	          _V_33814_STATUS.Bits.b2
#define	VF_33814DIAGCAS_GEN  	        _V_33814_STATUS.Bits.b3
#define	VF_33814DIAGPRD_GEN  	        _V_33814_STATUS.Bits.b4
#define	VF_33814RW_GEN  	            _V_33814_STATUS.Bits.b5

extern volatile V_STATUS_STR _V_33814INJ1CTL_STATUS;//@ADD_V_33814INJ1CTL_STATUS;
#define V_33814INJ1CTL_STATUS         _V_33814INJ1CTL_STATUS.Byte
#define	VF_OPEN_33814INJ1CTL	        _V_33814INJ1CTL_STATUS.Bits.b0
#define	VF_OT_33814INJ1CTL 	          _V_33814INJ1CTL_STATUS.Bits.b1
//#define	VF_OC_33814INJ1CTL	          _V_33814INJ1CTL_STATUS.Bits.b2
//#define	VF_OPEN_33814INJ1CTL	        _V_33814INJ1CTL_STATUS.Bits.b3

extern volatile V_STATUS_STR _V_33814O2HCTL_STATUS;//@ADD_V_33814O2HCTL_STATUS;
#define V_33814O2HCTL_STATUS          _V_33814O2HCTL_STATUS.Byte
#define	VF_OC_33814O2HCTL	            _V_33814O2HCTL_STATUS.Bits.b2
#define	VF_OPEN_33814O2HCTL	          _V_33814O2HCTL_STATUS.Bits.b3
 
extern volatile V_STATUS_STR _V_33814PUMPCTL_STATUS;//@ADD_V_33814PUMPCTL_STATUS;
#define V_33814PUMPCTL_STATUS         _V_33814PUMPCTL_STATUS.Byte
//#define	VF_SHORT_33814PUMPCTL	        _V_33814PUMPCTL_STATUS.Bits.b0
#define	VF_OT_33814PUMPCTL 	          _V_33814PUMPCTL_STATUS.Bits.b1
//#define	VF_OC_33814PUMPCTL	          _V_33814PUMPCTL_STATUS.Bits.b2
//#define	VF_OPEN_33814PUMPCTL	        _V_33814PUMPCTL_STATUS.Bits.b3
#define	VF_33814PUMPCLOSE	            _V_33814PUMPCTL_STATUS.Bits.b4
#define	VF_33814PUMPSELFCHECK 	      _V_33814PUMPCTL_STATUS.Bits.b5


//20201109 chenwei add ŷ�������ر�־λ
extern volatile V_STATUS_STR _V_OSEUROVDIAG_STATUS;//@ADD_V_OSEUROVDIAG_STATUS;
#define V_OSEUROVDIAG_STATUS         _V_OSEUROVDIAG_STATUS.Byte
#define VF_SCHMAX_SUM                      _V_OSEUROVDIAG_STATUS.Bits.b0  //20201109 chenwei add��ЧVF_SCHMAX_SUM��Խ���޵�������־(300mv)
#define VF_SCHMIN_SUM                      _V_OSEUROVDIAG_STATUS.Bits.b1  //20201109 chenwei add��ЧVF_SCHMIN_SUM��Խ���޵��½���־(600mv)
#define VF_EurVDiag_nbRL					_V_OSEUROVDIAG_STATUS.Bits.b2     //20201125 chenwei add ŷ��ר�õ�ͣ����־λ�����ڹ�Կ�׺�д�����ݽ���eeǰ
#define VF_EurVDiag_nbLR					_V_OSEUROVDIAG_STATUS.Bits.b3     //20201125 chenwei add ŷ��ר�õ�ͣ����־λ�����ڹ�Կ�׺�д�����ݽ���eeǰ
#define VF_CCPS_status					_V_OSEUROVDIAG_STATUS.Bits.b4     //20201126 chenwei add ŷ��̼�޵�ŷ���������
#define VF_CCPS_close					_V_OSEUROVDIAG_STATUS.Bits.b5         //20201127 chenwei add CCPS�ر�˲��ı�־
#define VF_SAIC_status					_V_OSEUROVDIAG_STATUS.Bits.b6     //20201207 chenwei add ȡ��VF_SAIC_st_EnCond SAIC�������ı�־��trueΪSAIC_ON��falseΪSAIC_OFF
#define VF_SAIC_Pin_Err					_V_OSEUROVDIAG_STATUS.Bits.b7     //20201130 chenwei add SAIC���ſ�·



//ADD_V_ENGPRTECTION_STATUS
extern volatile V_STATUS_STR  _V_ENGPRTECTION_STATUS;//@ADD_V_ENGPRTECTION_STATUS;
#define V_ENGPRTECTION_STATUS         _V_ENGPRTECTION_STATUS.Byte
#define	VF_OVERTEMPRATRUEPROTECTION_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b0    //20210115 chenwei remark ������±��������ı�־��1
#define	VF_OVERTEMANDHIGHLOAD_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b1    //20210115 chenwei add���㳬�����±�����ֵ���и߸���ʱ��1������v_t_overTMP_1s�ۼ�
#define	VF_STATUS_SPEED_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b2     //20210123 chenwei add ת�ٲ�����ĳһ��Χ����
#define	VF_STATUS_TPD_ENG 	           _V_ENGPRTECTION_STATUS.Bits.b3     //20210123 chenwei add TPD������ĳһ��Χ����
#define VF_MIL_NEMDTC	               _V_ENGPRTECTION_STATUS.Bits.b4		//20210923 chenwei add ����MIL�ź��Ǳ��¹��Ͽ�ʼ�ı�־
#define VF_MIL_DTC_LSTOVER	               _V_ENGPRTECTION_STATUS.Bits.b5		//20210924 chenwei add ������һ��D�������ź�ִ�н���
#define VF_MIL_DTC_3D2S	               _V_ENGPRTECTION_STATUS.Bits.b6		//20210923 chenwei add ����MIL�ź��Ǳ�ִ����˸��1λ
#define VF_MIL_NEMDTC2	               _V_ENGPRTECTION_STATUS.Bits.b7


//20211014 chenwei add �������������
extern volatile V_STATUS_STR  _V_O2H_STATUS;//@ADD_V_O2H_STATUS;
#define V_O2H_STATUS         _V_O2H_STATUS.Byte
#define	VF_OS_HIGHVOLT 	          _V_O2H_STATUS.Bits.b0    //20211014 chenwei add �����ȿ�·


#endif

