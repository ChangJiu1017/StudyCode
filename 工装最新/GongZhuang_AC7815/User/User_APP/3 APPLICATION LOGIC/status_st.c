/*HEAD
 ***********************************************************************
 * Filename	:Status_st.c
 * Function	:Status_st״̬��������
 * User		:yangsili
 * Date		:2017.12.9
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.9	yangsili
 *		Base module.
 * 
 * V9.2.0   2018.1.6	yangsili
 *		1.����33814�����͡������ȡ��ͱñ�������λ����
 *
 ***********************************************************************
HEAD END*/

#include "PE_Types.h"
//#include "IO_Map.h"
#include "status_st.h"
////#include "events.h"
volatile byte VF_OLDCA_CASbak;//		_V_CASH_STATUS.Bits.b0//�Ǳ굽��ʱ��FALSE,������ת�ٺ���TRUE;
volatile byte VF_OLDCAHY_CASbak;//		_V_CASH_STATUS.Bits.b1
volatile byte VF_PCAOV_CASbak;//		_V_CASH_STATUS.Bits.b2
volatile byte VF_TOOSPEED_CASbak;//		_V_CASH_STATUS.Bits.b3
volatile byte VF_MAINTMROV_CASbak;//		_V_CASH_STATUS.Bits.b4
volatile byte VF_CATCH_CASbak;//		_V_CASH_STATUS.Bits.b5
volatile byte VF_CATCHHY_CASbak;//		_V_CASH_STATUS.Bits.b6
volatile byte VF_NEWCYCLE_ATMAIN_CASbak;//		_V_CASH_STATUS.Bits.b7
volatile byte VF_INJSTOP_HARDbak;//_V_HARD_STATUS.Bits.b0
volatile byte VF_JKEYON_HARDbak;//_V_HARD_STATUS.Bits.b1   //2013-4-1
volatile byte VF_KEYON_HARDbak;//_V_HARD_STATUS.Bits.b2
volatile byte VF_POR_HARDbak;//_V_HARD_STATUS.Bits.b3
volatile byte VF_JKEYOFF_HARDbak;//_V_HARD_STATUS.Bits.b4 //2010-8-20
volatile byte VF_TURNONKEY_HARDbak;//_V_HARD_STATUS.Bits.b5
volatile byte VF_VPOR_HARDbak;//_V_HARD_STATUS.Bits.b6
volatile byte VF_T35TEST_HARDbak;//_V_HARD_STATUS.Bits.b7
/////////////////////////////////////////////////////
//volatile V_STATUS_STR _V_HARD_STATUS;
//volatile V_STATUS_STR _V_CASH_STATUS;
volatile V_STATUS_STR _V_VBAT_STATUS;
//volatile V_STATUS_STR _V_DTCH_STATUS;
volatile byte VFF_FUEL_DTCbak;          //9
volatile byte VFF_INJCTL_DTCbak;         //10
volatile byte VFF_ECUINIT_DTCbak;          //11
volatile byte VFF_CAECU_DTCbak;           //12
volatile byte VFF_ECUPWRON_DTCbak;      //13
volatile byte VFF_TAS_DTCbak;             //14
volatile byte VFF_TENGHIGH_DTCbak;     //15
volatile byte VFF_ISCV_DTCbak;         //16

//volatile V_STATUS_STR _V_DTCHL_STATUS;
volatile byte VFF_OSFK_DTCbak;         //17
volatile byte VFF_SAV_DTCbak;             //18
volatile byte VFF_EGR_DTCbak;              //19
volatile byte VFF_TWC_DTCbak;             //20
volatile byte VFF_MAPS_DTCbak;         //21
volatile byte VFF_AMF_DTCbak;               //22
volatile byte VFF_IGN_COIL_SPNbak;              //22
volatile byte VFF_VBATSP_ERRbak;

//volatile V_STATUS_STR _V_DTCHH_STATUS;
volatile byte VFF_CYL1INJ_DTCbak;           //33
volatile byte VFF_CYL2INJ_DTCbak;           //34
volatile byte VFF_CYL1OS_DTCbak;           //35
volatile byte VFF_CYL2OS_DTCbak;           //36
volatile byte VFF_PAIR_DTCbak;         //37
volatile byte VFF_IDLESCLIMIT_DTCbak;      //38
volatile byte VFF_7OFDIAG5_DTCbak;      //39
volatile byte VFF_8OFDIAG5_DTCbak;      //40

//volatile V_STATUS_STR _V_DTCHHL_STATUS;
volatile byte VFF_NOZZLEINI_TESHIGH_DTCbak;
volatile byte VFF_NOZZLEINI_T1DGOT_DTCbak;
volatile byte	VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak;
volatile byte	VFF_NOZZLEINI_ABCGOT_DTCbak;
volatile byte	VFF_NOZZLEINI_ABCSAVED_DTCbak;
volatile byte	VFF_NOZZLEINI_NOTDOM_DTCbak;
volatile byte	VFF_NOZZLEINI_DOMFAILURE_DTCbak;
volatile byte	VFF_NOZZLEINI_NOTINI_DTCbak;

//volatile V_STATUS_STR _V_DTCHHH_STATUS;
volatile byte VFF_NOZZLEINI_VBATNOTDOM_DTCbak;//:ECU��ѹû��dom
volatile byte VFF_NOZZLEINI_VBATVALID_DTCbak;//:��ѹ���͵����޷���ʼ����
volatile byte VFF_NOZZLEINI_NOINJ_DTCbak;//:���������ϵ����޷���ʼ����
volatile byte VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak;//ɨ������T4<T1d+500����Ϊ���������
volatile byte VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak;//T1Dɨ��֮�󣬼���
volatile byte VFF_NOZZLEINI_NOT4_DTCbak;//2009-11-29 ��t4,�����޷���ʼ����

volatile V_STATUS_STR _V_LDSENSOR_STATUS;
volatile V_STATUS_STR _V_MAPS_STATUS;
volatile V_STATUS_STR _V_CAS_STATUS;
volatile V_STATUS_STR _V_ETC_STATUS;
volatile V_STATUS_STR _V_LOCK_STATUS;
volatile V_STATUS_STR _V_LOCK2_STATUS;
volatile V_STATUS_STR _V_TMR_STATUS;
volatile V_STATUS_STR _V_TMCCL_STATUS;
volatile V_STATUS_STR _V_TMCCLH_STATUS;
volatile V_STATUS_STR _V_STOP_STATUS;
volatile V_STATUS_STR _V_ETC3_STATUS_RPMto_H;
volatile V_STATUS_STR _V_ETC4_STATUS_RPMto_L;
volatile V_STATUS_STR _V_I_CAP_STATUS;
volatile V_STATUS_STR _V_TRANS_STATUS;
volatile V_STATUS_STR _V_TRANSH_STATUS;
volatile V_STATUS_STR _V_INTINJ_STATUS;
volatile V_STATUS_STR _V_INTIGN_STATUS;
volatile V_STATUS_STR _V_INTSCI_STATUS;
//volatile V_STATUS_STR _V_DTC_STATUS;
volatile byte VFF_VBAT_DTCbak;         //1
volatile byte VFF_T3ER_DTCbak;         //2
volatile byte VFF_ECUNOTINIT_DTCbak;     //5  //2009-3-27 b4 to b2
volatile byte VFF_INJ_DTCbak;              //4
volatile byte VFF_SYSNOTINIT_DTCbak;     //3  //2009-3-27 b2 to b4
volatile byte VFF_TES_DTCbak;             //6
volatile byte VFF_OS_DTCbak;             //7
volatile byte VFF_TPS_DTCbak; 

volatile V_STATUS_STR _V_EE_STATUS;
volatile V_STATUS_STR _V_REV_STATUS;
volatile V_STATUS_STR _V_NOTIDLEMF_STATUS;

volatile V_STATUS_STR _V_IGN_STATUS;   //2014-8-5 MOVE HERE

volatile V_STATUS_STR _V_IGN2_STATUS; //2014-4-16 add

volatile V_STATUS_STR _V_CASHL_STATUS;//2014-4-16 


volatile V_STATUS_STR _V_AD_STATUS;//2014-4-28

volatile V_STATUS_STR _V_FLASH_STATUS;//2014-5-17

volatile V_STATUS_STR _V_NOTYSTDC_STATUS;//2014-8-5

volatile V_STATUS_STR _V_INJPHASE_STATUS;//2014-8-5

volatile V_STATUS_STR _V_ENG_STATUS;
//volatile V_STATUS_STR _V_ENGH_STATUS;
volatile byte VF_JLEAVEIDLETRANS_ENGbak;	//���뿪����ʱ
volatile byte VF_JSTOP_ENGbak;    //2014-3-30 ��ͣ��
volatile byte VF_MMOVE_ENGbak;
volatile byte VF_NOINJ_ENGbak;     //����Ҫ���ͣ��������ٶ��͵����
volatile byte VF_RUNTOOS_ENGbak;   //��λʱ����Ҫ�����ñ��
volatile byte VF_STARTS_ENGbak;
volatile byte VF_STOPBYTOOHOT_ENGbak;
volatile byte VF_TOIDLE_ENGbak;
volatile V_STATUS_STR _V_ENGHL_STATUS;

volatile V_STATUS_STR _V_FUELCOMP_STATUS;

//////////////////////////////////////////////
volatile v_prd_casSTR _v_prd_cas;
volatile v_t_cas0STR _v_t_cas0;
volatile v_PCSET_SCISTR _v_PCSET_SCI;

/*---2014-10-9 FOR SCR CONTROL------------*/
//volatile V_STATUS_STR _V_CAN_STATUS;//2014-8-12 ���ӵ�CANģʽת����״̬
//volatile V_STATUS_STR   _V_SCRPUMP_STATUS;    //2014-8-15 SCR״̬����
//volatile V_STATUS_STR   _V_SCR_STATUS;  //2014-8-15
//volatile V_STATUS_STR   _V_SCRCOM_STATUS;    //2014-8-15 SCR״̬����
//volatile V_STATUS_STR    _V_AIRV_STATUS;      //2014-10-29  ///������ŷ���״̬



//volatile V_STATUS_STR    _V_SCRDTC_STATUS;   //2014-11-10
//volatile V_STATUS_STR    _V_SCRDTC2_STATUS;   //2014-11-10
//volatile V_STATUS_STR    _V_SCRDTC3_STATUS;   //2014-11-10
//volatile V_STATUS_STR    _V_SCRDTC4_STATUS;   //2014-11-10
//volatile V_STATUS_STR    _V_SCRDTC5_STATUS;    //2014-12-7 CANͨѶ��� 

volatile V_STATUS_STR   _V_SPN0_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN1_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN2_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN3_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN4_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN5_STATUS;//2015-6-4
volatile V_STATUS_STR   _V_SPN6_STATUS;//2020-12-15   -ghz
volatile V_STATUS_STR   _V_SPN7_STATUS;//2020-12-15   -ghz

//volatile V_STATUS_STR    _V_CANCTL_STATUS; //2015-6-10
//volatile V_STATUS_STR    _V_PUMPCTL_STATUS;//2015-6-11

//volatile V_STATUS_STR    _V_SYSENABLE0_STATUS;  //2015-6-28
//volatile V_STATUS_STR    _V_MASTER_STATUS;      //2015-7-2

//volatile V_STATUS_STR    _V_CONDICTION_STATUS;  //2015-7-6
//volatile V_STATUS_STR    _V_PUMP2_STATUS;

//volatile V_STATUS_STR    _V_CAL_STATUS;//2015-7-7
//volatile V_STATUS_STR    _V_DATAVIEW_STATUS;//2015-7-12

//volatile V_STATUS_STR    _V_PN_STATUS[2];//2015-7-16
//volatile V_STATUS_STR    _V_PN2_STATUS[2];//2015-7-17
//volatile V_STATUS_STR    _V_PN3_STATUS[2];//2015-7-20

//volatile STRU_FROZEN _FROZEN_FRAME;//2015-8-24

/*---2017-12-9 ysl add FOR PI-EAEA0122------------*/
volatile V_STATUS_STR   _V_33814_STATUS;//2018-1-6
volatile V_STATUS_STR   _V_33814INJ1CTL_STATUS;//2017-12-9
volatile V_STATUS_STR   _V_33814O2HCTL_STATUS;//2017-12-9
volatile V_STATUS_STR   _V_33814PUMPCTL_STATUS;//2017-12-9

volatile V_STATUS_STR	_V_OSEUROVDIAG_STATUS;//2021-01-13 zhouxinrui add


volatile V_STATUS_STR   _V_IACV_STATUS;
volatile V_STATUS_STR   _V_IACV_STATUS2;    

volatile V_STATUS_STR   _V_ENGPRTECTION_STATUS;   //20210112 chenwei add ??????????

volatile V_STATUS_STR   _V_O2H_STATUS;   //20211014 chenwei add ?????????1??????

volatile byte VF_PUMPFROZEN_SCRbak;//_V_SCRPUMP_STATUS.Bits.b0      //�ñ�����
volatile byte VF_MIXERPURGED_SCRbak;//_V_SCRPUMP_STATUS.Bits.b2      //���ǻ��ɨ
volatile byte VF_JMODECHANGE_PUMPbak;//_V_SCRPUMP_STATUS.Bits.b3 //2014-8-26 //2014-11-4����
volatile byte VF_T4_MAYBEEXIST_PUMPbak;//    _V_SCRPUMP_STATUS.Bits.b6      //�Ƿ���T4����PN�Ĺ���״̬��أ������־λ��ʾ�������£�Ӧ����T4
volatile byte VF_T_STANDARD_GOT_SCRbak;//    _V_SCRPUMP_STATUS.Bits.b7      //2015-5-20 T�ı�׼ֵ�Ƿ�õ�
volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b0//�Ѿ����TW���� ,������EEPROM�У�2015-3-26
volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b1//�Ѿ����TW����
volatile byte VF_IS_DOSED_BUTNOTPURGE_SCRbak;//_V_SCR_STATUS.Bits.b2//�Ѿ��й�����
volatile byte VF_ISINJECTING_WHENDOSING_SCRbak;//  _V_SCR_STATUS.Bits.b3      //����DOSING��������
volatile byte VF_INJ_SHIFT_SCRbak;//_V_SCR_STATUS.Bits.b4	  //����ת����־λ
volatile byte VF_T1_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b5    //2015-2-5 ���ڸ�T1��ֵ����ʱ���ܴ���ִ��T1�Ķ���������õ���T1���ܲ��ԣ�
volatile byte VF_PRD_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b6    //2015-2-5 ���ڸ����ڸ�ֵ����ʱ���ܴ���ִ�����ڵĶ���������õ������ڿ��ܲ��ԣ�
volatile byte VF_STOPDOSING_BYDTCbak;//_SCR_V_SCR_STATUS.Bits.b7    //2015-5-16,���й��ϣ�ֹͣ����
volatile byte VF_AMODEADJUST_SCIbak;//_V_SCRCOM_STATUS.Bits.b0
volatile byte VF_CAB_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b1    //2015-3-21 �����궨
volatile byte VF_CAB_QDT3_WITOUTHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b2    //2015-3-21 �������궨
volatile byte VF_AIRASSIST_MODbak;//_V_SCRCOM_STATUS.Bits.b3    // 2015-3-21���ӣ��Ƿ�������ģʽ�ı�ʶ
volatile byte VF_CAB_CONSTV_CABbak;//_V_SCRCOM_STATUS.Bits.b4    //2015-6-11 ����ģʽ�ı궨
volatile byte VF_CHK_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b7    //2015-6-11 �����궨�����֤
volatile byte VF_TOBEON_AIRVbak;//_V_AIRV_STATUS.Bits.b0             //����������ŷ�Ҫ����
volatile byte VF_ISOPEN_AIRVbak;//_V_AIRV_STATUS.Bits.b1             //
volatile byte VF_ISSHORT_AIRVbak;//_V_AIRV_STATUS.Bits.b2             //
volatile byte VF_VALVEOFF_FOR_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b3   //2015-3-26 ����OBDʱ���ط��Ķ���
volatile byte VF_VALVEONTOOFF_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b4   //2015-4-15 ��Ҫ��¼�������ݣ��Ƿ��ص������Ĺ��̣����Ƿ��������صĹ���
volatile byte VF_OPEN2_INJCTLbak;//      _V_AIRV_STATUS.Bits.b5
volatile byte VF_TOBEON_NOXbak;//_V_NOX_STATUS.Bits.b0             //��ʾNOX��Ҫ����
volatile byte VF_VOLTINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b1             //��ʾNOX��Ҫ����:NOX���ȴ�
volatile byte VF_TMPINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b2             //��ʾNOX��Ҫ������������ѹ�ڷ�Χ��
volatile byte VF_NOXVALID_NOXbak;//_V_NOX_STATUS.Bits.b3             //��ʾNOX��Ҫ����
volatile byte VF_O2VALID_NOXbak;//_V_NOX_STATUS.Bits.b4             //��ʾNOX��Ҫ����
volatile byte VF_HEATEROK_NOXbak;//_V_NOX_STATUS.Bits.b5             //��ʾNOX��Ҫ����
volatile byte VF_NOXOK_NOXbak;//_V_NOX_STATUS.Bits.b6             //��ʾNOX��Ҫ����
volatile byte VF_O2_NOXbak;//_V_NOX_STATUS.Bits.b7             //��ʾNOX��Ҫ����
volatile byte VF_TOBEON_COOLVbak;//_V_COOLV_STATUS.Bits.b0             //������ȴˮ��ŷ�Ҫ����
volatile byte VF_ISOPEN_COOLVbak;//_V_COOLV_STATUS.Bits.b1             //
volatile byte VF_ISSHORT_COOLVbak;//_V_COOLV_STATUS.Bits.b2             //
volatile byte VF_NOXON_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b3             // 2015-6-13  SALVEģʽ�·�NOX ON����
volatile byte VF_NOXOFF_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b4             // 2015-6-13  SLAVEģʽ�·�NOX OFF����
volatile byte VF_LOW_TSCRbak;//_V_COOLV_STATUS.Bits.b5             // 2015-8-27 TSCRƫ��
volatile byte VF_UB_TUbak;//_V_TU_STATUS.Bits.b0             //
volatile byte VF_LB_TUbak;//_V_TU_STATUS.Bits.b1             //
volatile byte VF_DECING_TUbak;//_V_TU_STATUS.Bits.b2             //
volatile byte VF_UB_VOLTbak;//_V_TU_STATUS.Bits.b3             // 2014-11-15
volatile byte VF_LB_VOLTbak;//_V_TU_STATUS.Bits.b4             // 2014-11-15
volatile byte VF_UERR_TUbak;//_V_TU_STATUS.Bits.b5             //
volatile byte VF_LERR_TUbak;//_V_TU_STATUS.Bits.b6             //
volatile byte VF_LB_ADBbak;//_V_ADB_STATUS.Bits.b1             //
volatile byte VF_LERR_ADBbak;//_V_ADB_STATUS.Bits.b2             //
volatile byte VF_UERR_ADBbak;//_V_ADB_STATUS.Bits.b3             //
volatile byte VF_LLB_ADBbak;//_V_ADB_STATUS.Bits.b4             //
volatile byte VF_LLLB_ADBbak;//_V_ADB_STATUS.Bits.b5             //
volatile byte VF_UL_ADB_Vbak;//_ADB_STATUS.Bits.b6             //
volatile byte VF_UB_TEINbak;//_V_TEIN_STATUS.Bits.b0             //
volatile byte VF_ERR_TEINbak;//_V_TEIN_STATUS.Bits.b1             //
volatile byte VF_ERR_TEINbak;//_V_TEIN_STATUS.Bits.b2             //
volatile byte VF_IS_TSCR_SET_TEINbak;//_V_TEIN_STATUS.Bits.b3             //�Ƿ�������TSCR,2015-8-11
volatile byte VF_UB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b4             //
volatile byte VF_LB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b5             //
volatile byte VF_ERR_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b6             //
volatile byte VF_UB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b0             //
volatile byte VF_LB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b1             //
volatile byte VF_ERR_TEOUTbak;//_V_TEOUT_STATUS.Bits.b2             //
volatile byte VF_UB_TAMB_INNERbak;//      _V_TEOUT_STATUS.Bits.b3             //
volatile byte VF_LB_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b4             //
volatile byte VF_ERR_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b5             //
volatile byte VF_NOTAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b6             //2015-8-27û������CAN���¶��ź�
volatile byte VF_NOPAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b7             //2015-8-27û������CAN�Ļ���ѹ���ź�

volatile byte VF_VOLT_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b0             //��
volatile byte VF_INI_ACU_SCRDTCbak;//    _V_SCRDTC_STATUS.Bits.b1             //�� //��δ�궨
volatile byte VF_INJECTOR_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b2             //��
volatile byte VF_PUMPCHOKE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b3             //��
volatile byte VF_ACU_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b4             //��
volatile byte VF_FROZEN_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b5             //��
volatile byte VF_AIRVALVE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b6             //������ŷ�
volatile byte VF_MIXPRESSURE_SCRDTCbak;//         _V_SCRDTC_STATUS.Bits.b7             //�ù�·�쳣

volatile byte VF_LB_ADB_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b0             //��
volatile byte VF_ERR_ADB_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b1             //��
volatile byte VF_FROZEN_TU_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b2             //��
volatile byte VF_ERR_TU_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b3             //��
volatile byte VF_ERR_COOLV_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b4             //��
volatile byte VF_ERR_TEIN_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b5             //��
volatile byte VF_ERR_TEOUT_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b6             //��
volatile byte VF_NOXON_BUTNOVALUE_SCRDTCbak;// 	  _V_SCRDTC2_STATUS.Bits.b7             //��

volatile byte VF_EXH_U_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b0             //��
volatile byte VF_EXH_UU_SCRDTCbak;//     _V_SCRDTC3_STATUS.Bits.b1             //��
volatile byte VF_CATALYST_SCRDTCbak;//       _V_SCRDTC3_STATUS.Bits.b2             //��
volatile byte VF_HIGHALTITUDE_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b3             //2015-8-27 ADD HIGH ALTITUDE
volatile byte VF_TORQUELIMIT_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b4             //2015-8-28 ��Ť��־

volatile byte VF_NOLINK_FROMECU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b7             //û������ECU������
volatile byte VF_NOLINK_FROMDCU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b6             //��
volatile byte VF_NOLINK_FROMACU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b5             //��
volatile byte VF_NOLINK_FROMNOX_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b4             //��
volatile byte VF_NOLINK_FROMDCU_MGPERS_SCRDTCbak;// 	            _V_SCRDTC4_STATUS.Bits.b3             //�� ,2015-1-27 ����mg/S������

volatile byte VF_NOLINK_FROMSCR1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b7             
volatile byte VF_NOLINK_FROMINCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b6             
volatile byte VF_NOLINK_FROMAMCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b5             
volatile byte VF_NOLINK_FROMEEC2_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b4             
volatile byte VF_NOLINK_FROMEEC1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b3             
volatile byte VF_NOLINK_FROMPT_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b2//2015-4-4             
volatile byte VF_NOLINK_N_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b1//2015-7-21  û��ת���ź�
volatile byte VF_NOLINK_T_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b0//2015-7-21  û��Ť���ź�

volatile byte VF_CALPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b0       //��������CAN���ϵģ��Ա궨���ݵ�����      
volatile byte VF_DTC_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b1             //ʹ�ܹ�����ķ���
volatile byte VF_PNPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b2             //ʹ������궨���ݵķ�����2015-7-21
volatile byte VF_DIAG_RQST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b3             //����Ƿ�������2015-8-29
volatile byte VF_TPCM_BAM_SENT_CANCTLbak;//_V_CANCTL_STATUS.Bits.b4             //TPCM_BAM��ͷ�Ѿ�������
volatile byte VF_FS_BS_CONTINUE_CANCTLbak;//_V_CANCTL_STATUS.Bits.b5             //2015-9-20 15765֡�����Ʒ�ʽ

volatile byte VF_ISCONSTV_NOTDT3_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b0       //�����Ƿ��Ƕ��ݿ���
volatile byte VF_ISFREREC_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b1       //�����Ƿ�Ƶ������
volatile byte VF_ISBACKPRE_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b2       //�����Ƿ�ѹ����
volatile byte VF_NOLIQUID_CHKING_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b3       //2015-6-15:�ÿ��ж�:������Ϊ��ʱ��T1-T3�Ƿ������Ϊ�жϱÿյ�����
volatile byte VF_INJ2_SHIFT_SCRbak;//_V_PUMPCTL_STATUS.Bits.b5       //2015-6-17 add for ch2 injection control
volatile byte VF_SYSINJ_CH2_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b6       //2015-6-29 CH2�Ĵ�Һ�������Ƿ����ͬ��ģʽ
volatile byte VF_IS_PRIME_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b7       //2015-7-4 ��Һ����Ƿ������ı�ʶ��

volatile byte VF_SLAVEOBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b0       //
volatile byte VF_MASTEROBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b1       //
volatile byte VF_T3OBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b2       //
volatile byte VF_GELANFU_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b3       //
volatile byte VF_CALPARA_AUTOOUT_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b4       //2015-7-30 �Զ�ʹ�ܱ궨���ݵķ���
volatile byte VF_NO_TRANS_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b5       //2015-9-29
volatile byte VF_NO_NH3_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b6       //

volatile byte VF_1NEWINJ_MASTERbak;//_V_MASTER_STATUS.Bits.b0          // ����MASTERģʽ����һ���µ�INJ����
volatile byte VF_1DC_CON_MASTERbak;//_V_MASTER_STATUS.Bits.b1          // ����MASTERģʽ������һ��DC������
volatile byte VF_1WUC_MASTERbak;//_V_MASTER_STATUS.Bits.b2          // ����MASTERģʽ������һ��WUC������
volatile byte VF_STH3_CON_MASTERbak;//  _V_MASTER_STATUS.Bits.b3          // ����MASTERģʽ������NH3STĿ��
volatile byte VF_JSTARTS_MASTERbak;//_V_MASTER_STATUS.Bits.b4          // ����MASTERģʽ����������������

volatile byte VF_PIPEOBD_CONbak;//      _V_CONDICTION_STATUS.Bits.b0  //����PIPEOBD������        
volatile byte VF_PRIMING_CONbak;//      _V_CONDICTION_STATUS.Bits.b1  //���� ������        
volatile byte VF_DOSING_CONbak;//    _V_CONDICTION_STATUS.Bits.b2  //�������������        
volatile byte VF_T1SCAN_CONbak;//    _V_CONDICTION_STATUS.Bits.b3  //�������������        
volatile byte VF_PURGING_CONbak;//      _V_CONDICTION_STATUS.Bits.b3  //�������������        

volatile byte VF_ISSETPARA_PUMP2bak;//        _V_PUMP2_STATUS.Bits.b0          //2015-7-19�����ò�����Ƶ�ʺ�T1�� 
volatile byte VF_ISENABLE_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b1//�Ƿ�ʹ��PUMP2
volatile byte VF_TOBESCAN_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b2//��Ҫɨ��
volatile byte F_L_DOSERNOLIQUID_PUMP2bak;//  	  _V_PUMP2_STATUS.Bits.b3//��Һ�õļ����ã��ڶ�γ��Ժ�����Һ��

volatile byte VF_MODE_OBDWORK_CALbak;//        _V_CAL_STATUS.Bits.b0  //OBD�ɹ�����MASTER��2015-7-18
volatile byte VF_NH3MODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b1  //2015-10-12
volatile byte VF_TRANSMODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b2  //2015-10-12

volatile byte VF_FLASHLINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b0          // 
volatile byte VF_LINKTOEE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b1          //1: TO EE, 0 : TO FLASH 
volatile byte VF_BY_BYTE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b2          //1: TO BY BYTE, 0 : BY WORD 
volatile byte VF_BY_1UNIT_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b3          //1: TO 1UNIT, 0 : 2 UNITS 
volatile byte VF_LINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b4          //1: ��DATAVIEW ���������ӣ�



//volatile STRU_FROZEN _FROZEN_FRAME;//2015-8-24
