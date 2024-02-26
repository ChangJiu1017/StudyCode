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
#define ADD_V_AD_STATUS              (REG_BASE1 + 0x00000346UL)//2014-4-28 增加的AD采集的状态

#define ADD_V_FLASH_STATUS           (REG_BASE1 + 0x00000347UL)//2014-5-17 增加的FLASH的状态

#define ADD_V_SCR_STATUS             (REG_BASE1 + 0x00000348UL)
#define ADD_V_SCRPUMP_STATUS         (REG_BASE1 + 0x00000349UL)
#define ADD_V_SCRCOM_STATUS          (REG_BASE1 + 0x0000034aUL)
#define ADD_V_CAN_STATUS             (REG_BASE1 + 0x0000034bUL)//2014-8-12 增加的CAN模式转换的状态

#define ADD_V_AIRV_STATUS            (REG_BASE1 + 0x0000034cUL)//2014-10-28 空气电磁阀状态
#define ADD_V_NOX_STATUS             (REG_BASE1 + 0x0000034dUL)//2014-11-1 nox传感器状态
#define ADD_V_COOLV_STATUS           (REG_BASE1 + 0x0000034eUL)//2014-11-1 冷却水阀状态
#define ADD_V_ADB_STATUS             (REG_BASE1 + 0x0000034fUL)//2014-11-1 液位传感器状态
#define ADD_V_TU_STATUS              (REG_BASE1 + 0x00000350UL)//2014-11-1 尿素液传感器状态
#define ADD_V_TE_INLET_STATUS        (REG_BASE1 + 0x00000351UL)//2014-11-1 进口排气温度
#define ADD_V_TE_OUTLET_STATUS       (REG_BASE1 + 0x00000352UL)//2014-11-1 出口排气温度

#define ADD_V_SCRDTC_STATUS           (REG_BASE1 + 0x00000353UL)//2014-11-3 SCR ACU诊断
#define ADD_V_SCRDTC2_STATUS          (REG_BASE1 + 0x00000354UL)//2014-11-3 SCR ACU诊断
#define ADD_V_SCRDTC3_STATUS          (REG_BASE1 + 0x00000355UL)//2014-11-3 SCR ACU诊断
#define ADD_V_SCRDTC4_STATUS          (REG_BASE1 + 0x00000356UL)//2014-11-10 SCR 通讯诊断
#define ADD_V_SCRDTC5_STATUS          (REG_BASE1 + 0x00000357UL)//2014-12-7 SCR 通讯诊断

//#define ADD_ENERGY_STATUS             (REG_BASE1 + 0x00000360UL)//2014-2-2能量模型中的标志
#define ADD_V_SPN0_STATUS             (REG_BASE1 + 0x00000358UL)//2015-6-4 SPN定义
#define ADD_V_SPN1_STATUS             (REG_BASE1 + 0x00000359UL)//2015-6-4 SPN定义
#define ADD_V_SPN2_STATUS             (REG_BASE1 + 0x0000035aUL)//2015-6-4 SPN定义
#define ADD_V_SPN3_STATUS             (REG_BASE1 + 0x0000035bUL)//2015-6-4 SPN定义
#define ADD_V_SPN4_STATUS             (REG_BASE1 + 0x0000035cUL)//2015-6-4 SPN定义
#define ADD_V_SPN5_STATUS             (REG_BASE1 + 0x0000035dUL)//2015-6-4 SPN定义

#define ADD_V_DTCCTL_STATUS           (REG_BASE1 + 0x0000035eUL)//2015-6-10 DTC的传输控制

#define ADD_V_PUMPCTL_STATUS          (REG_BASE1 + 0x0000035fUL)//2015-6-11 泵控制磨死

//系统状态使能开关
#define ADD_V_SYSENABLE0_STATUS       (REG_BASE1 + 0x00000360UL)//2015-6-28 
#define ADD_V_MASTER_STATUS           (REG_BASE1 + 0x00000361UL)//2015-7-2 
#define ADD_CONDICTION_STATUS         (REG_BASE1 + 0x00000362UL)//2015-7-6 一些动作的条件 
#define ADD_PUMP2_STATUS              (REG_BASE1 + 0x00000363UL)//2015-7-7 供给泵 
#define ADD_CAL_STATUS                (REG_BASE1 + 0x00000364UL)//2015-7-7 标定状态
#define ADD_DATAVIEW_STATUS           (REG_BASE1 + 0x00000365UL)//2015-7-12 DATAVIEW
#define ADD_PN_STATUS                 (REG_BASE1 + 0x00000366UL)//2015-7-16 动力喷嘴状态，两路
                                       //注意这个是数组，需要占两个BYTE
#define ADD_PN2_STATUS                 (REG_BASE1 + 0x00000368UL)//2015-7-17 动力喷嘴状态，两路
                                       //注意这个是数组，需要占两个BYTE
#define ADD_PN3_STATUS                 (REG_BASE1 + 0x0000036AUL)//2015-7-17 动力喷嘴状态，两路
                                       //注意这个是数组，需要占两个BYTE

#define ADD_V_SPN6_STATUS             (REG_BASE1 + 0x0000036cUL)//2015-10-15 SPN定义
#define ADD_V_SPN7_STATUS             (REG_BASE1 + 0x0000036dUL)//2015-10-15 SPN定义

/*---2017-12-9 ysl add FOR PI-EAEA0122------------*/
#define ADD_V_33814_STATUS            (REG_BASE1 + 0x0000036eUL)
#define ADD_V_33814INJ1CTL_STATUS     (REG_BASE1 + 0x0000036fUL)
#define ADD_V_33814O2HCTL_STATUS      (REG_BASE1 + 0x00000370UL)
#define ADD_V_33814PUMPCTL_STATUS     (REG_BASE1 + 0x00000371UL)

#define ADD_V_IACV_STATUS             (REG_BASE1 + 0x00000372UL)//2017-3-13 for IACV
#define ADD_V_IACV_STATUS2             (REG_BASE1 + 0x00000373UL)//2017-3-13 for IACV
#define ADD_V_FUELCOMP_STATUS             (REG_BASE1 + 0x00000374UL)

//20201109 chenwei add 欧五氧传感器诊断
#define ADD_V_OSEUROVDIAG_STATUS             (REG_BASE1 + 0x00000376UL)

#define ADD_V_ENGPRTECTION_STATUS             (REG_BASE1 + 0x00000377UL)    //20210112 chenwei add 发动机保护相关

#define ADD_V_O2H_STATUS             (REG_BASE1 + 0x00000378UL)    //20211014 chenwei add 环松氧加热功能



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
  //2015-7-16 用于PN，两路
  struct {
    byte Q_DT3  :1;   //Q-DT3已标定                                    
    byte FRE  :1;     //频率已标定                                  
    byte T3_ST  :1;   //已获得T3标定值                                    
    byte T1M  :1;     //T1M已标定                                  
    byte VB_OK  :1;      //标定电压确认OK                                 
    byte T1M_CUR  :1; //当次的T1M已获得                                       
    byte b6  :1;                                       
    byte ALL  :1;                                       
  } CALED;
  
  struct {
    byte T3_FORAVE_GOT  :1;  //获得用于平均的T3                                     
    byte NOLIQUIDCHKING   :1;//判断是否无液体                                       
    byte ISNOLIQUID   :1;                                       
    byte ISJAM   :1;                                       
    byte DEICING   :1;                                       
    byte DEICE_FAIL   :1;//
    byte T3_FORT1SCAN  :1;                                       
    byte T3_FORNOLIQUIDT1  :1; //T3是对应NOLIQUID T1的                                      
  } HARD;

  struct {
    byte  IS_T1_FOR_T30 :1;  //获得用于平均的T3                                     
    byte  IS_T1_FOR_PUMP  :1;//用于空泵的T1                                       
    byte  IS_T1_ON  :1;      //驱动已打开                                 
    byte  NOLIQUIDCHK_CON  :1;                                       
    byte  DEICING_CON:1;                                       
    byte  JAM_CON:1;//
    byte  IS_DEICED :1; //2015-7-27已融冰                                      
    byte   :1; //T3是对应NOLIQUID T1的                                      
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
//这块是和SCI通信相关的定义
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

extern volatile byte VF_PUMPFROZEN_SCRbak;//_V_SCRPUMP_STATUS.Bits.b0      //泵被冰冻
extern volatile byte VF_MIXERPURGED_SCRbak;//_V_SCRPUMP_STATUS.Bits.b2      //混合腔清扫
extern volatile byte VF_JMODECHANGE_PUMPbak;//_V_SCRPUMP_STATUS.Bits.b3 //2014-8-26 //2014-11-4纠正
extern volatile byte VF_T4_MAYBEEXIST_PUMPbak;//    _V_SCRPUMP_STATUS.Bits.b6      //是否有T4，与PN的工作状态相关，这个标志位表示这个情况下，应该有T4
extern volatile byte VF_T_STANDARD_GOT_SCRbak;//    _V_SCRPUMP_STATUS.Bits.b7      //2015-5-20 T的标准值是否得到
extern volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b0//已经获得TW数据 ,保存在EEPROM中，2015-3-26
extern volatile byte VF_IS_NORMAL_PNTW_EE_SCRbak;//_V_SCR_STATUS.Bits.b1//已经获得TW数据
extern volatile byte VF_IS_DOSED_BUTNOTPURGE_SCRbak;//_V_SCR_STATUS.Bits.b2//已经有过喷射
extern volatile byte VF_ISINJECTING_WHENDOSING_SCRbak;//  _V_SCR_STATUS.Bits.b3      //正在DOSING且有流量
extern volatile byte VF_INJ_SHIFT_SCRbak;//_V_SCR_STATUS.Bits.b4	  //喷射转换标志位
extern volatile byte VF_T1_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b5    //2015-2-5 正在给T1赋值，此时不能触发执行T1的动作，否则得到的T1可能不对；
extern volatile byte VF_PRD_SETTING_SCRbak;//_V_SCR_STATUS.Bits.b6    //2015-2-5 正在给周期赋值，此时不能触发执行周期的动作，否则得到的周期可能不对；
extern volatile byte VF_STOPDOSING_BYDTCbak;//_SCR_V_SCR_STATUS.Bits.b7    //2015-5-16,因有故障，停止喷射
extern volatile byte VF_AMODEADJUST_SCIbak;//_V_SCRCOM_STATUS.Bits.b0
extern volatile byte VF_CAB_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b1    //2015-3-21 带气标定
extern volatile byte VF_CAB_QDT3_WITOUTHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b2    //2015-3-21 不带气标定
extern volatile byte VF_AIRASSIST_MODbak;//_V_SCRCOM_STATUS.Bits.b3    // 2015-3-21增加：是否是气助模式的标识
extern volatile byte VF_CAB_CONSTV_CABbak;//_V_SCRCOM_STATUS.Bits.b4    //2015-6-11 定容模式的标定
extern volatile byte VF_CHK_QDT3_WITHAIR_CABbak;//_V_SCRCOM_STATUS.Bits.b7    //2015-6-11 带气标定后的验证
extern volatile byte VF_TOBEON_AIRVbak;//_V_AIRV_STATUS.Bits.b0             //表明空气电磁阀要工作
extern volatile byte VF_ISOPEN_AIRVbak;//_V_AIRV_STATUS.Bits.b1             //
extern volatile byte VF_ISSHORT_AIRVbak;//_V_AIRV_STATUS.Bits.b2             //
extern volatile byte VF_VALVEOFF_FOR_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b3   //2015-3-26 喷嘴OBD时，关阀的动作
extern volatile byte VF_VALVEONTOOFF_PNOBD_AIRVbak;//_V_AIRV_STATUS.Bits.b4   //2015-4-15 需要纪录过程数据，是阀关到阀开的过程，还是阀开到阀关的过程
extern volatile byte VF_OPEN2_INJCTLbak;//      _V_AIRV_STATUS.Bits.b5
extern volatile byte VF_TOBEON_NOXbak;//_V_NOX_STATUS.Bits.b0             //表示NOX需要工作
extern volatile byte VF_VOLTINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b1             //表示NOX需要工作:NOX加热打开
extern volatile byte VF_TMPINRANGE_NOXbak;//_V_NOX_STATUS.Bits.b2             //表示NOX需要工作：工作电压在范围内
extern volatile byte VF_NOXVALID_NOXbak;//_V_NOX_STATUS.Bits.b3             //表示NOX需要工作
extern volatile byte VF_O2VALID_NOXbak;//_V_NOX_STATUS.Bits.b4             //表示NOX需要工作
extern volatile byte VF_HEATEROK_NOXbak;//_V_NOX_STATUS.Bits.b5             //表示NOX需要工作
extern volatile byte VF_NOXOK_NOXbak;//_V_NOX_STATUS.Bits.b6             //表示NOX需要工作
extern volatile byte VF_O2_NOXbak;//_V_NOX_STATUS.Bits.b7             //表示NOX需要工作
extern volatile byte VF_TOBEON_COOLVbak;//_V_COOLV_STATUS.Bits.b0             //表明冷却水电磁阀要工作
extern volatile byte VF_ISOPEN_COOLVbak;//_V_COOLV_STATUS.Bits.b1             //
extern volatile byte VF_ISSHORT_COOLVbak;//_V_COOLV_STATUS.Bits.b2             //
extern volatile byte VF_NOXON_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b3             // 2015-6-13  SALVE模式下发NOX ON命令
extern volatile byte VF_NOXOFF_ATSLAVE_NOXbak;//_V_COOLV_STATUS.Bits.b4             // 2015-6-13  SLAVE模式下发NOX OFF命令
extern volatile byte VF_LOW_TSCRbak;//_V_COOLV_STATUS.Bits.b5             // 2015-8-27 TSCR偏低
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
extern volatile byte VF_IS_TSCR_SET_TEINbak;//_V_TEIN_STATUS.Bits.b3             //是否设置了TSCR,2015-8-11
extern volatile byte VF_UB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b4             //
extern volatile byte VF_LB_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b5             //
extern volatile byte VF_ERR_TAMB_OUTERbak;//      _V_TEIN_STATUS.Bits.b6             //

extern volatile byte VF_UB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b0             //
extern volatile byte VF_LB_TEOUTbak;//_V_TEOUT_STATUS.Bits.b1             //
extern volatile byte VF_ERR_TEOUTbak;//_V_TEOUT_STATUS.Bits.b2             //
extern volatile byte VF_UB_TAMB_INNERbak;//      _V_TEOUT_STATUS.Bits.b3             //
extern volatile byte VF_LB_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b4             //
extern volatile byte VF_ERR_TAMB_INNERbak;//_V_TEOUT_STATUS.Bits.b5             //
extern volatile byte VF_NOTAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b6             //2015-8-27没有来自CAN的温度信号
extern volatile byte VF_NOPAMB_FROMCANbak;//_V_TEOUT_STATUS.Bits.b7             //2015-8-27没有来自CAN的环境压力信号

extern volatile byte VF_VOLT_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b0             //泵
extern volatile byte VF_INI_ACU_SCRDTCbak;//    _V_SCRDTC_STATUS.Bits.b1             //泵 //泵未标定
extern volatile byte VF_INJECTOR_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b2             //泵
extern volatile byte VF_PUMPCHOKE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b3             //泵
extern volatile byte VF_ACU_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b4             //泵
extern volatile byte VF_FROZEN_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b5             //泵
extern volatile byte VF_AIRVALVE_SCRDTCbak;//_V_SCRDTC_STATUS.Bits.b6             //空气电磁阀
extern volatile byte VF_MIXPRESSURE_SCRDTCbak;//         _V_SCRDTC_STATUS.Bits.b7             //泵管路异常

extern volatile byte VF_LB_ADB_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b0             //泵
extern volatile byte VF_ERR_ADB_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b1             //泵
extern volatile byte VF_FROZEN_TU_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b2             //泵
extern volatile byte VF_ERR_TU_SCRDTCbak;//   _V_SCRDTC2_STATUS.Bits.b3             //泵
extern volatile byte VF_ERR_COOLV_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b4             //泵
extern volatile byte VF_ERR_TEIN_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b5             //泵
extern volatile byte VF_ERR_TEOUT_SCRDTCbak;//     _V_SCRDTC2_STATUS.Bits.b6             //泵
extern volatile byte VF_NOXON_BUTNOVALUE_SCRDTCbak;// 	  _V_SCRDTC2_STATUS.Bits.b7             //泵

extern volatile byte VF_EXH_U_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b0             //泵
extern volatile byte VF_EXH_UU_SCRDTCbak;//     _V_SCRDTC3_STATUS.Bits.b1             //泵
extern volatile byte VF_CATALYST_SCRDTCbak;//       _V_SCRDTC3_STATUS.Bits.b2             //泵
extern volatile byte VF_HIGHALTITUDE_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b3             //2015-8-27 ADD HIGH ALTITUDE
extern volatile byte VF_TORQUELIMIT_SCRDTCbak;//   _V_SCRDTC3_STATUS.Bits.b4             //2015-8-28 限扭标志

extern volatile byte VF_NOLINK_FROMECU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b7             //没有来自ECU的连接
extern volatile byte VF_NOLINK_FROMDCU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b6             //泵
extern volatile byte VF_NOLINK_FROMACU_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b5             //泵
extern volatile byte VF_NOLINK_FROMNOX_SCRDTCbak;//        _V_SCRDTC4_STATUS.Bits.b4             //泵
extern volatile byte VF_NOLINK_FROMDCU_MGPERS_SCRDTCbak;// 	            _V_SCRDTC4_STATUS.Bits.b3             //泵 ,2015-1-27 增加mg/S的输入

extern volatile byte VF_NOLINK_FROMSCR1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b7             
extern volatile byte VF_NOLINK_FROMINCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b6             
extern volatile byte VF_NOLINK_FROMAMCON_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b5             
extern volatile byte VF_NOLINK_FROMEEC2_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b4             
extern volatile byte VF_NOLINK_FROMEEC1_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b3             
extern volatile byte VF_NOLINK_FROMPT_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b2//2015-4-4             
extern volatile byte VF_NOLINK_N_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b1//2015-7-21  没有转速信号
extern volatile byte VF_NOLINK_T_SCRDTCbak;//      _V_SCRDTC5_STATUS.Bits.b0//2015-7-21  没有扭矩信号

extern volatile byte VF_CALPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b0       //有来自于CAN线上的，对标定数据的请求      
extern volatile byte VF_DTC_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b1             //使能故障码的发出
extern volatile byte VF_PNPARA_REQUEST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b2             //使能喷嘴标定数据的发出，2015-7-21
extern volatile byte VF_DIAG_RQST_CANCTLbak;//_V_CANCTL_STATUS.Bits.b3             //诊断仪发出请求，2015-8-29
extern volatile byte VF_TPCM_BAM_SENT_CANCTLbak;//_V_CANCTL_STATUS.Bits.b4             //TPCM_BAM包头已经发出了
extern volatile byte VF_FS_BS_CONTINUE_CANCTLbak;//_V_CANCTL_STATUS.Bits.b5             //2015-9-20 15765帧流控制方式

extern volatile byte VF_ISCONSTV_NOTDT3_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b0       //代表是否是定容控制
extern volatile byte VF_ISFREREC_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b1       //代表是否频率修正
extern volatile byte VF_ISBACKPRE_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b2       //代表是否背压修正
extern volatile byte VF_NOLIQUID_CHKING_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b3       //2015-6-15:泵空判断:用于作为当时的T1-T3是否可以作为判断泵空的条件
extern volatile byte VF_INJ2_SHIFT_SCRbak;//_V_PUMPCTL_STATUS.Bits.b5       //2015-6-17 add for ch2 injection control
extern volatile byte VF_SYSINJ_CH2_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b6       //2015-6-29 CH2的纯液体喷射是否采用同步模式
extern volatile byte VF_IS_PRIME_PUMPbak;//_V_PUMPCTL_STATUS.Bits.b7       //2015-7-4 纯液体泵是否已填充的标识；

extern volatile byte VF_SLAVEOBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b0       //
extern volatile byte VF_MASTEROBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b1       //
extern volatile byte VF_T3OBD_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b2       //
extern volatile byte VF_GELANFU_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b3       //
extern volatile byte VF_CALPARA_AUTOOUT_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b4       //2015-7-30 自动使能标定数据的发出
extern volatile byte VF_NO_TRANS_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b5       //2015-9-29
extern volatile byte VF_NO_NH3_EN_SYSbak;//_V_SYSENABLE0_STATUS.Bits.b6       //

extern volatile byte VF_1NEWINJ_MASTERbak;//_V_MASTER_STATUS.Bits.b0          // 用于MASTER模式，又一个新的INJ到来
extern volatile byte VF_1DC_CON_MASTERbak;//_V_MASTER_STATUS.Bits.b1          // 用于MASTER模式，满足一个DC的条件
extern volatile byte VF_1WUC_MASTERbak;//_V_MASTER_STATUS.Bits.b2          // 用于MASTER模式，满足一个WUC的条件
extern volatile byte VF_STH3_CON_MASTERbak;//  _V_MASTER_STATUS.Bits.b3          // 用于MASTER模式，满足NH3ST目标
extern volatile byte VF_JSTARTS_MASTERbak;//_V_MASTER_STATUS.Bits.b4          // 用于MASTER模式，发动机刚启动，

extern volatile byte VF_PIPEOBD_CONbak;//      _V_CONDICTION_STATUS.Bits.b0  //满足PIPEOBD的条件        
extern volatile byte VF_PRIMING_CONbak;//      _V_CONDICTION_STATUS.Bits.b1  //满足 的条件        
extern volatile byte VF_DOSING_CONbak;//    _V_CONDICTION_STATUS.Bits.b2  //满足喷射的条件        
extern volatile byte VF_T1SCAN_CONbak;//    _V_CONDICTION_STATUS.Bits.b3  //满足喷射的条件        
extern volatile byte VF_PURGING_CONbak;//      _V_CONDICTION_STATUS.Bits.b3  //满足喷射的条件        

extern volatile byte VF_ISSETPARA_PUMP2bak;//        _V_PUMP2_STATUS.Bits.b0          //2015-7-19在设置参数：频率和T1中 
extern volatile byte VF_ISENABLE_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b1//是否使能PUMP2
extern volatile byte VF_TOBESCAN_PUMP2bak;//      _V_PUMP2_STATUS.Bits.b2//需要扫描
extern volatile byte F_L_DOSERNOLIQUID_PUMP2bak;//  	  _V_PUMP2_STATUS.Bits.b3//纯液泵的计量泵，在多次尝试后，仍无液体

extern volatile byte VF_MODE_OBDWORK_CALbak;//        _V_CAL_STATUS.Bits.b0  //OBD可工作的MASTER，2015-7-18
extern volatile byte VF_NH3MODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b1  //2015-10-12
extern volatile byte VF_TRANSMODEL_OFF_CALbak;//        _V_CAL_STATUS.Bits.b2  //2015-10-12

extern volatile byte VF_FLASHLINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b0          // 
extern volatile byte VF_LINKTOEE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b1          //1: TO EE, 0 : TO FLASH 
extern volatile byte VF_BY_BYTE_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b2          //1: TO BY BYTE, 0 : BY WORD 
extern volatile byte VF_BY_1UNIT_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b3          //1: TO 1UNIT, 0 : 2 UNITS 
extern volatile byte VF_LINK_DATAVIEWbak;//_V_DATAVIEW_STATUS.Bits.b4          //1: 与DATAVIEW 保持了连接，





/*-------------------STATUS FOR SCR CONTROL-------------------------------*/
//extern volatile V_STATUS_STR   _V_SCRPUMP_STATUS;//@ADD_V_SCRPUMP_STATUS;    //2014-8-15 SCR状态配置
//#define V_SCRPUMP_STATUS              V_SCRPUMP_STATUSbak//_V_SCRPUMP_STATUS.Byte
#define	VF_PUMPFROZEN_SCR	            VF_PUMPFROZEN_SCRbak//_V_SCRPUMP_STATUS.Bits.b0      //泵被冰冻
#define	VF_MIXERPURGED_SCR	          VF_MIXERPURGED_SCRbak//_V_SCRPUMP_STATUS.Bits.b2      //混合腔清扫
#define	VF_JMODECHANGE_PUMP	          VF_JMODECHANGE_PUMPbak//_V_SCRPUMP_STATUS.Bits.b3 //2014-8-26 //2014-11-4纠正
#define	VF_T4_MAYBEEXIST_PUMP	        VF_T4_MAYBEEXIST_PUMPbak//    _V_SCRPUMP_STATUS.Bits.b6      //是否有T4，与PN的工作状态相关，这个标志位表示这个情况下，应该有T4
#define	VF_T_STANDARD_GOT_SCR	        VF_T_STANDARD_GOT_SCRbak//    _V_SCRPUMP_STATUS.Bits.b7      //2015-5-20 T的标准值是否得到

//extern volatile V_STATUS_STR   _V_SCR_STATUS;//@ADD_V_SCR_STATUS;    //2014-8-15 SCR状态配置
//#define V_SCR_STATUS           _V_SCR_STATUS.Byte

#define VF_IS_NORMAL_PNTW_EE_SCR 			VF_IS_NORMAL_PNTW_EE_SCRbak//_V_SCR_STATUS.Bits.b0//已经获得TW数据 ,保存在EEPROM中，2015-3-26
#define VF_IS_NORMAL_PNTW_GOT_SCR 		VF_IS_NORMAL_PNTW_EE_SCRbak//_V_SCR_STATUS.Bits.b1//已经获得TW数据
#define VF_IS_DOSED_BUTNOTPURGE_SCR 	VF_IS_DOSED_BUTNOTPURGE_SCRbak//_V_SCR_STATUS.Bits.b2//已经有过喷射
#define VF_ISINJECTING_WHENDOSING_SCR VF_ISINJECTING_WHENDOSING_SCRbak//  _V_SCR_STATUS.Bits.b3      //正在DOSING且有流量
#define	VF_INJ_SHIFT_SCR	      			VF_INJ_SHIFT_SCRbak//_V_SCR_STATUS.Bits.b4	  //喷射转换标志位
#define VF_T1_SETTING_SCR       			VF_T1_SETTING_SCRbak//_V_SCR_STATUS.Bits.b5    //2015-2-5 正在给T1赋值，此时不能触发执行T1的动作，否则得到的T1可能不对；
#define VF_PRD_SETTING_SCR       			VF_PRD_SETTING_SCRbak//_V_SCR_STATUS.Bits.b6    //2015-2-5 正在给周期赋值，此时不能触发执行周期的动作，否则得到的周期可能不对；
#define VF_STOPDOSING_BYDTC_SCR       VF_STOPDOSING_BYDTCbak//_SCR_V_SCR_STATUS.Bits.b7    //2015-5-16,因有故障，停止喷射


//extern volatile V_STATUS_STR   _V_SCRCOM_STATUS;//@ADD_V_SCRCOM_STATUS;    //2014-8-15 SCR状态配置
//#define V_SCRCOM_STATUS           	_V_SCRCOM_STATUS.Byte
#define	VF_AMODEADJUST_SCI						VF_AMODEADJUST_SCIbak//_V_SCRCOM_STATUS.Bits.b0
#define	VF_CAB_QDT3_WITHAIR_CAB	      VF_CAB_QDT3_WITHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b1    //2015-3-21 带气标定
#define	VF_CAB_QDT3_WITOUTHAIR_CAB	  VF_CAB_QDT3_WITOUTHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b2    //2015-3-21 不带气标定
#define	VF_AIRASSIST_MOD	          	VF_AIRASSIST_MODbak//_V_SCRCOM_STATUS.Bits.b3    // 2015-3-21增加：是否是气助模式的标识
#define	VF_CAB_CONSTV_CAB	          	VF_CAB_CONSTV_CABbak//_V_SCRCOM_STATUS.Bits.b4    //2015-6-11 定容模式的标定
#define	VF_CHK_QDT3_WITHAIR_CAB	      VF_CHK_QDT3_WITHAIR_CABbak//_V_SCRCOM_STATUS.Bits.b7    //2015-6-11 带气标定后的验证


//extern volatile V_STATUS_STR    _V_AIRV_STATUS;//@ADD_V_AIRV_STATUS;    //2014-10-28 空气电磁阀状态
//#define V_AIRV_STATUS           _V_AIRV_STATUS.Byte
#define	VF_TOBEON_AIRV    	    			VF_TOBEON_AIRVbak//_V_AIRV_STATUS.Bits.b0             //表明空气电磁阀要工作
#define	VF_ISOPEN_AIRV    	    			VF_ISOPEN_AIRVbak//_V_AIRV_STATUS.Bits.b1             //
#define	VF_ISSHORT_AIRV    	    			VF_ISSHORT_AIRVbak//_V_AIRV_STATUS.Bits.b2             //
#define	VF_VALVEOFF_FOR_PNOBD_AIRV    VF_VALVEOFF_FOR_PNOBD_AIRVbak//_V_AIRV_STATUS.Bits.b3   //2015-3-26 喷嘴OBD时，关阀的动作
#define	VF_VALVEONTOOFF_PNOBD_AIRV    VF_VALVEONTOOFF_PNOBD_AIRVbak//_V_AIRV_STATUS.Bits.b4   //2015-4-15 需要纪录过程数据，是阀关到阀开的过程，还是阀开到阀关的过程
#define	VF_OPEN2_INJCTL	              VF_OPEN2_INJCTLbak//      _V_AIRV_STATUS.Bits.b5
#define	VF_SHORT2_INJCTL	            VF_OPEN2_INJCTLbak//      _V_AIRV_STATUS.Bits.b6


//extern volatile V_STATUS_STR    _V_NOX_STATUS;//@ADD_V_NOX_STATUS;    //2014-10-28 空气电磁阀状态
//#define V_NOX_STATUS           _V_NOX_STATUS.Byte
#define	VF_TOBEON_NOX    	     		VF_TOBEON_NOXbak//_V_NOX_STATUS.Bits.b0             //表示NOX需要工作
#define	VF_VOLTINRANGE_NOX    	  VF_VOLTINRANGE_NOXbak//_V_NOX_STATUS.Bits.b1             //表示NOX需要工作:NOX加热打开
#define	VF_TMPINRANGE_NOX    	    VF_TMPINRANGE_NOXbak//_V_NOX_STATUS.Bits.b2             //表示NOX需要工作：工作电压在范围内
#define	VF_NOXVALID_NOX    	      VF_NOXVALID_NOXbak//_V_NOX_STATUS.Bits.b3             //表示NOX需要工作
#define	VF_O2VALID_NOX    	      VF_O2VALID_NOXbak//_V_NOX_STATUS.Bits.b4             //表示NOX需要工作
#define	VF_HEATEROK_NOX    	      VF_HEATEROK_NOXbak//_V_NOX_STATUS.Bits.b5             //表示NOX需要工作
#define	VF_NOXOK_NOX    	        VF_NOXOK_NOXbak//_V_NOX_STATUS.Bits.b6             //表示NOX需要工作
#define	VF_O2_NOX    	            VF_O2_NOXbak//_V_NOX_STATUS.Bits.b7             //表示NOX需要工作


//extern volatile V_STATUS_STR      _V_COOLV_STATUS;//@ADD_V_COOLV_STATUS;    //2014-10-28 冷却水阀状态
//#define V_COOLV_STATUS            _V_COOLV_STATUS.Byte
#define	VF_TOBEON_COOLV    	      VF_TOBEON_COOLVbak//_V_COOLV_STATUS.Bits.b0             //表明冷却水电磁阀要工作
#define	VF_ISOPEN_COOLV    	      VF_ISOPEN_COOLVbak//_V_COOLV_STATUS.Bits.b1             //
#define	VF_ISSHORT_COOLV    	    VF_ISSHORT_COOLVbak//_V_COOLV_STATUS.Bits.b2             //
#define VF_NOXON_ATSLAVE_NOX      VF_NOXON_ATSLAVE_NOXbak//_V_COOLV_STATUS.Bits.b3             // 2015-6-13  SALVE模式下发NOX ON命令
#define VF_NOXOFF_ATSLAVE_NOX     VF_NOXOFF_ATSLAVE_NOXbak//_V_COOLV_STATUS.Bits.b4             // 2015-6-13  SLAVE模式下发NOX OFF命令
#define VF_LOW_TSCR               VF_LOW_TSCRbak//_V_COOLV_STATUS.Bits.b5             // 2015-8-27 TSCR偏低

//extern volatile V_STATUS_STR    _V_TU_STATUS;//@ADD_V_TU_STATUS;    //2014-10-28 尿素温度状态
//#define V_TU_STATUS           _V_TU_STATUS.Byte
#define	VF_UB_TU    	            VF_UB_TUbak//_V_TU_STATUS.Bits.b0             //
#define	VF_LB_TU    	            VF_LB_TUbak//_V_TU_STATUS.Bits.b1             //
#define	VF_DECING_TU    	        VF_DECING_TUbak//_V_TU_STATUS.Bits.b2             //

#define	VF_UB_VOLT    	          VF_UB_VOLTbak//_V_TU_STATUS.Bits.b3             // 2014-11-15
#define	VF_LB_VOLT    	          VF_LB_VOLTbak//_V_TU_STATUS.Bits.b4             // 2014-11-15

#define	VF_UERR_TU    	          VF_UERR_TUbak//_V_TU_STATUS.Bits.b5             //
#define	VF_LERR_TU    	          VF_LERR_TUbak//_V_TU_STATUS.Bits.b6             //

//extern volatile V_STATUS_STR    _V_ADB_STATUS;//@ADD_V_ADB_STATUS;    //2014-10-28 液位状态
//#define V_ADB_STATUS              _V_ADB_STATUS.Byte
#define	VF_LB_ADB    	            VF_LB_ADBbak//_V_ADB_STATUS.Bits.b1             //
#define	VF_LERR_ADB    	          VF_LERR_ADBbak//_V_ADB_STATUS.Bits.b2             //
#define	VF_UERR_ADB    	          VF_UERR_ADBbak//_V_ADB_STATUS.Bits.b3             //

#define	VF_LLB_ADB    	          VF_LLB_ADBbak//_V_ADB_STATUS.Bits.b4             //
#define	VF_LLLB_ADB    	          VF_LLLB_ADBbak//_V_ADB_STATUS.Bits.b5             //
#define	VF_UL_ADB    	            VF_UL_ADB_Vbak//_ADB_STATUS.Bits.b6             //


//extern volatile V_STATUS_STR    _V_TEIN_STATUS;//@ADD_V_TE_INLET_STATUS;    //2014-10-28 进口排气状态
//#define V_TEIN_STATUS                 _V_TEIN_STATUS.Byte
#define	VF_UB_TEIN    	              VF_UB_TEINbak//_V_TEIN_STATUS.Bits.b0             //
#define	VF_LB_TEIN    	              VF_ERR_TEINbak//_V_TEIN_STATUS.Bits.b1             //
#define	VF_ERR_TEIN    	              VF_ERR_TEINbak//_V_TEIN_STATUS.Bits.b2             //
#define	VF_IS_TSCR_SET_TEIN    	      VF_IS_TSCR_SET_TEINbak//_V_TEIN_STATUS.Bits.b3             //是否设置了TSCR,2015-8-11
#define	VF_UB_TAMB_OUTER    	        VF_UB_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b4             //
#define	VF_LB_TAMB_OUTER    	        VF_LB_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b5             //
#define	VF_ERR_TAMB_OUTER    	        VF_ERR_TAMB_OUTERbak//      _V_TEIN_STATUS.Bits.b6             //


//extern volatile V_STATUS_STR    _V_TEOUT_STATUS;//@ADD_V_TE_OUTLET_STATUS;    //2014-10-28 出口排气状态
//#define V_TEOUT_STATUS                _V_TEOUT_STATUS.Byte
#define	VF_UB_TEOUT    	              VF_UB_TEOUTbak//_V_TEOUT_STATUS.Bits.b0             //
#define	VF_LB_TEOUT    	              VF_LB_TEOUTbak//_V_TEOUT_STATUS.Bits.b1             //
#define	VF_ERR_TEOUT    	            VF_ERR_TEOUTbak//_V_TEOUT_STATUS.Bits.b2             //
#define	VF_UB_TAMB_INNER    	        VF_UB_TAMB_INNERbak//      _V_TEOUT_STATUS.Bits.b3             //
#define	VF_LB_TAMB_INNER    	        VF_LB_TAMB_INNERbak//_V_TEOUT_STATUS.Bits.b4             //
#define	VF_ERR_TAMB_INNER    	        VF_ERR_TAMB_INNERbak//_V_TEOUT_STATUS.Bits.b5             //
#define	VF_NOTAMB_FROMCAN    	  			VF_NOTAMB_FROMCANbak//_V_TEOUT_STATUS.Bits.b6             //2015-8-27没有来自CAN的温度信号
#define	VF_NOPAMB_FROMCAN    	  			VF_NOPAMB_FROMCANbak//_V_TEOUT_STATUS.Bits.b7             //2015-8-27没有来自CAN的环境压力信号



//extern volatile V_STATUS_STR    _V_SCRDTC_STATUS;//@ADD_V_SCRDTC_STATUS;    //2014-10-28 SLAVE模式时的状态，
//#define V_SCRDTC_STATUS                _V_SCRDTC_STATUS.Byte
#define	VF_VOLT_SCRDTC    	         VF_VOLT_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b0             //泵
#define	VF_INI_ACU_SCRDTC    	       VF_INI_ACU_SCRDTCbak//    _V_SCRDTC_STATUS.Bits.b1             //泵 //泵未标定
#define	VF_INJECTOR_SCRDTC    	     VF_INJECTOR_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b2             //泵
#define	VF_PUMPCHOKE_SCRDTC    	     VF_PUMPCHOKE_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b3             //泵
#define	VF_ACU_SCRDTC    	           VF_ACU_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b4             //泵
#define	VF_FROZEN_SCRDTC    	       VF_FROZEN_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b5             //泵
#define	VF_AIRVALVE_SCRDTC    	     VF_AIRVALVE_SCRDTCbak//_V_SCRDTC_STATUS.Bits.b6             //空气电磁阀
#define	VF_MIXPRESSURE_SCRDTC    	   VF_MIXPRESSURE_SCRDTCbak//         _V_SCRDTC_STATUS.Bits.b7             //泵管路异常


//extern volatile V_STATUS_STR    _V_SCRDTC2_STATUS;//@ADD_V_SCRDTC2_STATUS;    //2014-10-28 MASTER
//#define V_SCRDTC2_STATUS                _V_SCRDTC2_STATUS.Byte
#define	VF_LB_ADB_SCRDTC    	       VF_LB_ADB_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b0             //泵
#define	VF_ERR_ADB_SCRDTC    	       VF_ERR_ADB_SCRDTCbak//   _V_SCRDTC2_STATUS.Bits.b1             //泵
#define	VF_FROZEN_TU_SCRDTC    	     VF_FROZEN_TU_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b2             //泵
#define	VF_ERR_TU_SCRDTC    	       VF_ERR_TU_SCRDTCbak//   _V_SCRDTC2_STATUS.Bits.b3             //泵
#define	VF_ERR_COOLV_SCRDTC    	     VF_ERR_COOLV_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b4             //泵
#define	VF_ERR_TEIN_SCRDTC    	     VF_ERR_TEIN_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b5             //泵
#define	VF_ERR_TEOUT_SCRDTC    	     VF_ERR_TEOUT_SCRDTCbak//     _V_SCRDTC2_STATUS.Bits.b6             //泵
#define	VF_NOXON_BUTNOVALUE_SCRDTC   VF_NOXON_BUTNOVALUE_SCRDTCbak// 	  _V_SCRDTC2_STATUS.Bits.b7             //泵

//extern volatile V_STATUS_STR    _V_SCRDTC3_STATUS;//@ADD_V_SCRDTC3_STATUS;    //2014-10-28 出口排气状态
//#define V_SCRDTC3_STATUS                _V_SCRDTC3_STATUS.Byte
#define	VF_EXH_U_SCRDTC    	         VF_EXH_U_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b0             //泵
#define	VF_EXH_UU_SCRDTC    	       VF_EXH_UU_SCRDTCbak//     _V_SCRDTC3_STATUS.Bits.b1             //泵
#define	VF_CATALYST_SCRDTC    	     VF_CATALYST_SCRDTCbak//       _V_SCRDTC3_STATUS.Bits.b2             //泵

#define	VF_HIGHALTITUDE_SCRDTC    	 VF_HIGHALTITUDE_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b3             //2015-8-27 ADD HIGH ALTITUDE
#define	VF_TORQUELIMIT_SCRDTC    	   VF_TORQUELIMIT_SCRDTCbak//   _V_SCRDTC3_STATUS.Bits.b4             //2015-8-28 限扭标志



//extern volatile V_STATUS_STR    _V_SCRDTC4_STATUS;//@ADD_V_SCRDTC4_STATUS;    //2014-10-28 出口排气状态
//#define V_SCRDTC4_STATUS                _V_SCRDTC4_STATUS.Byte
#define	VF_NOLINK_FROMECU_SCRDTC    	    VF_NOLINK_FROMECU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b7             //没有来自ECU的连接
#define	VF_NOLINK_FROMDCU_SCRDTC    	    VF_NOLINK_FROMDCU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b6             //泵
#define	VF_NOLINK_FROMACU_SCRDTC    	    VF_NOLINK_FROMACU_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b5             //泵
#define	VF_NOLINK_FROMNOX_SCRDTC    	    VF_NOLINK_FROMNOX_SCRDTCbak//        _V_SCRDTC4_STATUS.Bits.b4             //泵
#define	VF_NOLINK_FROMDCU_MGPERS_SCRDTC   VF_NOLINK_FROMDCU_MGPERS_SCRDTCbak// 	            _V_SCRDTC4_STATUS.Bits.b3             //泵 ,2015-1-27 增加mg/S的输入


//extern volatile V_STATUS_STR    _V_SCRDTC5_STATUS;//@ADD_V_SCRDTC5_STATUS;    //2014-12-7 CAN通讯诊断 
//#define V_SCRDTC5_STATUS                _V_SCRDTC5_STATUS.Byte
#define	VF_NOLINK_FROMSCR1_SCRDTC    	    VF_NOLINK_FROMSCR1_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b7             
#define	VF_NOLINK_FROMINCON_SCRDTC    	  VF_NOLINK_FROMINCON_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b6             
#define	VF_NOLINK_FROMAMCON_SCRDTC    	  VF_NOLINK_FROMAMCON_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b5             
#define	VF_NOLINK_FROMEEC2_SCRDTC    	    VF_NOLINK_FROMEEC2_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b4             
#define	VF_NOLINK_FROMEEC1_SCRDTC    	    VF_NOLINK_FROMEEC1_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b3             
#define	VF_NOLINK_FROMPT_SCRDTC    	      VF_NOLINK_FROMPT_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b2//2015-4-4             
#define	VF_NOLINK_N_SCRDTC    	          VF_NOLINK_N_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b1//2015-7-21  没有转速信号
#define	VF_NOLINK_T_SCRDTC    	          VF_NOLINK_T_SCRDTCbak//      _V_SCRDTC5_STATUS.Bits.b0//2015-7-21  没有扭矩信号

          
//extern volatile V_STATUS_STR    _V_CANCTL_STATUS;//@ADD_V_DTCCTL_STATUS;    //2014-12-7 CAN通讯诊断 
//#define V_CANCTL_STATUS                _V_CANCTL_STATUS.Byte
#define	VF_CALPARA_REQUEST_CANCTL    	 		VF_CALPARA_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b0       //有来自于CAN线上的，对标定数据的请求      
#define	VF_DTC_REQUEST_CANCTL    	 				VF_DTC_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b1             //使能故障码的发出
#define	VF_PNPARA_REQUEST_CANCTL    	 		VF_PNPARA_REQUEST_CANCTLbak//_V_CANCTL_STATUS.Bits.b2             //使能喷嘴标定数据的发出，2015-7-21
#define	VF_DIAG_RQST_CANCTL    	 					VF_DIAG_RQST_CANCTLbak//_V_CANCTL_STATUS.Bits.b3             //诊断仪发出请求，2015-8-29
#define	VF_TPCM_BAM_SENT_CANCTL    		 		VF_TPCM_BAM_SENT_CANCTLbak//_V_CANCTL_STATUS.Bits.b4             //TPCM_BAM包头已经发出了
#define	VF_FS_BS_CONTINUE_CANCTL    			VF_FS_BS_CONTINUE_CANCTLbak//_V_CANCTL_STATUS.Bits.b5             //2015-9-20 15765帧流控制方式




//extern volatile V_STATUS_STR    _V_PUMPCTL_STATUS;//@ADD_V_PUMPCTL_STATUS;    //2015-6-11 泵控制模式
//#define V_PUMPCTL_STATUS                V_PUMPCTL_STATUS.Byte
#define	VF_ISCONSTV_NOTDT3_PUMP    	    	VF_ISCONSTV_NOTDT3_PUMPbak//_V_PUMPCTL_STATUS.Bits.b0       //代表是否是定容控制
#define	VF_ISFREREC_PUMP    	          	VF_ISFREREC_PUMPbak//_V_PUMPCTL_STATUS.Bits.b1       //代表是否频率修正
#define	VF_ISBACKPRE_PUMP    	          	VF_ISBACKPRE_PUMPbak//_V_PUMPCTL_STATUS.Bits.b2       //代表是否背压修正
#define	VF_NOLIQUID_CHKING_PUMP    	    	VF_NOLIQUID_CHKING_PUMPbak//_V_PUMPCTL_STATUS.Bits.b3       //2015-6-15:泵空判断:用于作为当时的T1-T3是否可以作为判断泵空的条件
#define	VF_INJ2_SHIFT_SCR    	          	VF_INJ2_SHIFT_SCRbak//_V_PUMPCTL_STATUS.Bits.b5       //2015-6-17 add for ch2 injection control
#define	VF_SYSINJ_CH2_PUMP    	        	VF_SYSINJ_CH2_PUMPbak//_V_PUMPCTL_STATUS.Bits.b6       //2015-6-29 CH2的纯液体喷射是否采用同步模式
#define VF_IS_PRIME_PUMP                	VF_IS_PRIME_PUMPbak//_V_PUMPCTL_STATUS.Bits.b7       //2015-7-4 纯液体泵是否已填充的标识；


//extern volatile V_STATUS_STR    _V_SYSENABLE0_STATUS;//@ADD_V_SYSENABLE0_STATUS;    //2015-6-11 泵控制模式
//#define V_SYSENABLE0_STATUS                _V_SYSENABLE0_STATUS.Byte
#define	VF_SLAVEOBD_SYS    	    					VF_SLAVEOBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b0       //
#define	VF_MASTEROBD_SYS    	          	VF_MASTEROBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b1       //
#define	VF_T3OBD_SYS    	          			VF_T3OBD_SYSbak//_V_SYSENABLE0_STATUS.Bits.b2       //
#define	VF_GELANFU_SYS    	    					VF_GELANFU_SYSbak//_V_SYSENABLE0_STATUS.Bits.b3       //
#define	VF_CALPARA_AUTOOUT_SYS    	    	VF_CALPARA_AUTOOUT_SYSbak//_V_SYSENABLE0_STATUS.Bits.b4       //2015-7-30 自动使能标定数据的发出
#define	VF_NO_TRANS_EN_SYS    	    			VF_NO_TRANS_EN_SYSbak//_V_SYSENABLE0_STATUS.Bits.b5       //2015-9-29
#define	VF_NO_NH3_EN_SYS    	    				VF_NO_NH3_EN_SYSbak//_V_SYSENABLE0_STATUS.Bits.b6       //



//extern volatile V_STATUS_STR    _V_MASTER_STATUS;//@ADD_V_MASTER_STATUS;    //2015-7-2 MASTER控制
//#define V_MASTER_STATUS         _V_MASTER_STATUS.Byte
#define	VF_1NEWINJ_MASTER    	    VF_1NEWINJ_MASTERbak//_V_MASTER_STATUS.Bits.b0          // 用于MASTER模式，又一个新的INJ到来
#define	VF_1DC_CON_MASTER    	    VF_1DC_CON_MASTERbak//_V_MASTER_STATUS.Bits.b1          // 用于MASTER模式，满足一个DC的条件
#define	VF_1WUC_MASTER    	    	VF_1WUC_MASTERbak//_V_MASTER_STATUS.Bits.b2          // 用于MASTER模式，满足一个WUC的条件
#define	VF_STH3_CON_MASTER    	  VF_STH3_CON_MASTERbak//  _V_MASTER_STATUS.Bits.b3          // 用于MASTER模式，满足NH3ST目标
#define	VF_JSTARTS_MASTER    	    VF_JSTARTS_MASTERbak//_V_MASTER_STATUS.Bits.b4          // 用于MASTER模式，发动机刚启动，




//extern volatile V_STATUS_STR    _V_CONDICTION_STATUS;//@ADD_CONDICTION_STATUS;    //
//#define V_CONDICTION_STATUS         _V_CONDICTION_STATUS.Byte
#define	VF_PIPEOBD_CON    	      VF_PIPEOBD_CONbak//      _V_CONDICTION_STATUS.Bits.b0  //满足PIPEOBD的条件        
#define	VF_PRIMING_CON    	      VF_PRIMING_CONbak//      _V_CONDICTION_STATUS.Bits.b1  //满足 的条件        
#define	VF_DOSING_CON    	        VF_DOSING_CONbak//    _V_CONDICTION_STATUS.Bits.b2  //满足喷射的条件        
#define	VF_T1SCAN_CON    	        VF_T1SCAN_CONbak//    _V_CONDICTION_STATUS.Bits.b3  //满足喷射的条件        
#define	VF_PURGING_CON    	      VF_PURGING_CONbak//      _V_CONDICTION_STATUS.Bits.b3  //满足喷射的条件        

//extern volatile V_STATUS_STR    _V_PUMP2_STATUS;//@ADD_PUMP2_STATUS;    
//#define V_PUMP2_STATUS         _V_PUMP2_STATUS.Byte
#define	VF_ISSETPARA_PUMP2    	  VF_ISSETPARA_PUMP2bak//        _V_PUMP2_STATUS.Bits.b0          //2015-7-19在设置参数：频率和T1中 
#define VF_ISENABLE_PUMP2    	    VF_ISENABLE_PUMP2bak//      _V_PUMP2_STATUS.Bits.b1//是否使能PUMP2
#define VF_TOBESCAN_PUMP2    	    VF_TOBESCAN_PUMP2bak//      _V_PUMP2_STATUS.Bits.b2//需要扫描
#define VF_L_DOSERNOLIQUID_PUMP2  VF_L_DOSERNOLIQUID_PUMP2bak//  	  _V_PUMP2_STATUS.Bits.b3//纯液泵的计量泵，在多次尝试后，仍无液体


//extern volatile V_STATUS_STR    _V_CAL_STATUS;//@ADD_CAL_STATUS;  //关于标定  
//#define V_CAL_STATUS         _V_CAL_STATUS.Byte
#define	VF_MODE_OBDWORK_CAL    	  VF_MODE_OBDWORK_CALbak//        _V_CAL_STATUS.Bits.b0  //OBD可工作的MASTER，2015-7-18
//设置几个标定用的开关
#define	VF_NH3MODEL_OFF_CAL    	  VF_NH3MODEL_OFF_CALbak//        _V_CAL_STATUS.Bits.b1  //2015-10-12
#define	VF_TRANSMODEL_OFF_CAL    	VF_TRANSMODEL_OFF_CALbak//        _V_CAL_STATUS.Bits.b2  //2015-10-12


//extern volatile V_STATUS_STR    _V_DATAVIEW_STATUS;//@ADD_DATAVIEW_STATUS;  //DATAVIEW 2015-7-12
//#define V_DATAVIEW_STATUS         _V_DATAVIEW_STATUS.Byte
#define	VF_FLASHLINK_DATAVIEW    	VF_FLASHLINK_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b0          // 
#define	VF_LINKTOEE_DATAVIEW    	VF_LINKTOEE_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b1          //1: TO EE, 0 : TO FLASH 
#define	VF_BY_BYTE_DATAVIEW    	  VF_BY_BYTE_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b2          //1: TO BY BYTE, 0 : BY WORD 
#define	VF_BY_1UNIT_DATAVIEW    	VF_BY_1UNIT_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b3          //1: TO 1UNIT, 0 : 2 UNITS 
#define	VF_LINK_DATAVIEW    	    VF_LINK_DATAVIEWbak//_V_DATAVIEW_STATUS.Bits.b4          //1: 与DATAVIEW 保持了连接，


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
#define	VF_LEANCON_CLS				VF_LEANCON_CLSbak    //2012-12-7　偏稀控制
#define	VF_LIMITFK_CLS				VF_LIMITFK_CLSbak	//氧传感器修正量达到限制
#define	VF_ENRICHLIMITFK_CLS	VF_ENRICHLIMITFK_CLSbak    //增油反馈被限制住了
#define	VF_T35_CLS						VF_T35_CLSbak    //2013-3-19
#define	VF_1LEAN_NOTIDLE_CLS	VF_1LEAN_NOTIDLE_CLSbak    //2012-6-28　离开怠速后是否出现一次稀
#define	VF_RICH_MORE_CLS			VF_RICH_MORE_CLSbak    //用于自学习，表示浓的太多；
#define	VF_LEAN_MORE_CLS			VF_LEAN_MORE_CLSbak    //用于自学习，表示稀的太多；


/******************************************************************/
//extern volatile V_STATUS_STR _V_CLSSL_STATUS;//    @ADD_V_CLSSL_STATUS;
//extern volatile V_STATUS_STR _V_CLSSLH_STATUS;//   @ADD_V_CLSSLH_STATUS;
//#define V_CLSSL_STATUS      _V_CLSSL_STATUS.Byte
//#define V_CLSSLH_STATUS      _V_CLSSLH_STATUS.Byte

extern volatile byte VF_DTCOK_CLSSLbak;   //没有故障，满足学习的条件
extern volatile byte VF_EECHKED_CLSSLbak;  //EEPROM 中的自学习数据已经校验过
extern volatile byte VF_LIMITLEARN_CLSSLbak;
extern volatile byte VF_MAYLEARN_CLSSLbak;
extern volatile byte VF_RD_NEW_CLSSLbak;   //用于同步两次读EE的操作
extern volatile byte VF_STIO_CLSSLbak;
extern volatile byte VF_TOBELEARN_CLSSLbak;
extern volatile byte VF_FKSTABLE_CLSSLbak;     //反馈值稳定　2013-3-7
#define	VF_DTCOK_CLSSL			VF_DTCOK_CLSSLbak   //没有故障，满足学习的条件
#define	VF_EECHKED_CLSSL		VF_EECHKED_CLSSLbak  //EEPROM 中的自学习数据已经校验过
#define	VF_LIMITLEARN_CLSSL	VF_LIMITLEARN_CLSSLbak
#define	VF_MAYLEARN_CLSSL		VF_MAYLEARN_CLSSLbak
#define	VF_RD_NEW_CLSSL			VF_RD_NEW_CLSSLbak   //用于同步两次读EE的操作
#define	VF_STIO_CLSSL				VF_STIO_CLSSLbak
#define	VF_TOBELEARN_CLSSL	VF_TOBELEARN_CLSSLbak
#define	VF_FKSTABLE_CLSSL		VF_FKSTABLE_CLSSLbak     //反馈值稳定　2013-3-7

extern volatile byte VF_TMPOK_CLSSLbak;      //温度满足要求
extern volatile byte VF_LEARNPOINT_CHG_CLSSLbak;   //学习点变更
extern volatile byte VF_LEARNECONDICTION_CLSSLbak;  //满足学习条件
extern volatile byte VF_CONDICTIONSTEADY_CLSSLbak;  //满足学习条件，且稳定一定时间
extern volatile byte VF_CONDICTIONSTEADY_LST_CLSSLbak;  //上一次，满足学习条件，且稳定一定时间
extern volatile byte VF_EE_ER_CLSSLbak;  //EEPROM 中，自学习值有误
extern volatile byte VF_MATCH_INI_CLSSLbak; //满足自学习的基本条件
#define	VF_TMPOK_CLSSL								VF_TMPOK_CLSSLbak      //温度满足要求
#define	VF_LEARNPOINT_CHG_CLSSL				VF_LEARNPOINT_CHG_CLSSLbak   //学习点变更
#define	VF_LEARNECONDICTION_CLSSL			VF_LEARNECONDICTION_CLSSLbak  //满足学习条件
#define	VF_CONDICTIONSTEADY_CLSSL			VF_CONDICTIONSTEADY_CLSSLbak  //满足学习条件，且稳定一定时间
#define	VF_CONDICTIONSTEADY_LST_CLSSL	VF_CONDICTIONSTEADY_LST_CLSSLbak  //上一次，满足学习条件，且稳定一定时间
#define	VF_EE_ER_CLSSL								VF_EE_ER_CLSSLbak  //EEPROM 中，自学习值有误
#define	VF_MATCH_INI_CLSSL						VF_MATCH_INI_CLSSLbak  //满足自学习的基本条件


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
#define	VFF_NOZZLEINI_TESHIGH_DTC               VFF_NOZZLEINI_TESHIGH_DTCbak           //41  满足初始化时，对温度的要求
#define	VFF_NOZZLEINI_T1DGOT_DTC                VFF_NOZZLEINI_T1DGOT_DTCbak        //：获得了T1D
#define	VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTC VFF_NOZZLEINI_FAILURE_BYNOINFLEXION_DTCbak        //：没有找到T4拐点而导致扫描T1D失败；
#define	VFF_NOZZLEINI_ABCGOT_DTC                VFF_NOZZLEINI_ABCGOT_DTCbak    //：获得了计算T3B所需的ABC系数；
#define	VFF_NOZZLEINI_ABCSAVED_DTC              VFF_NOZZLEINI_ABCSAVED_DTCbak    //：成功保存ABC测试结果；
#define	VFF_NOZZLEINI_NOTDOM_DTC                VFF_NOZZLEINI_NOTDOM_DTCbak    //：没有DOM
#define	VFF_NOZZLEINI_DOMFAILURE_DTC            VFF_NOZZLEINI_DOMFAILURE_DTCbak//:DOM数据有问题；
#define	VFF_NOZZLEINI_NOTINI_DTC                VFF_NOZZLEINI_NOTINI_DTCbak//:喷嘴没有初始化；

//extern volatile V_STATUS_STR _V_DTCHHH_STATUS;// @ADD_V_DTCHHH_STATUS;    //2009-3-5 add for power nozzle ini
extern volatile byte VFF_NOZZLEINI_VBATNOTDOM_DTCbak;//:ECU电压没有dom
extern volatile byte VFF_NOZZLEINI_VBATVALID_DTCbak;//:电压过低导致无法初始化；
extern volatile byte VFF_NOZZLEINI_NOINJ_DTCbak;//:喷油器故障导致无法初始化；
extern volatile byte VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak;//扫描结果的T4<T1d+500，认为结果有问题
extern volatile byte VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak;//T1D扫描之后，计算得到的系数非法；2009-3-5
extern volatile byte VFF_NOZZLEINI_NOT4_DTCbak;//2009-11-29 无t4,导致无法初始化；
#define V_DTCHHH_STATUS   (VFF_NOZZLEINI_VBATNOTDOM_DTCbak<<0|VFF_NOZZLEINI_VBATVALID_DTCbak<<1|\
													 VFF_NOZZLEINI_NOINJ_DTCbak<<2|VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak<<3|\
													 VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak<<4|VFF_NOZZLEINI_NOT4_DTCbak<<5)
#define	VFF_NOZZLEINI_VBATNOTDOM_DTC            VFF_NOZZLEINI_VBATNOTDOM_DTCbak//:ECU电压没有dom
#define	VFF_NOZZLEINI_VBATVALID_DTC             VFF_NOZZLEINI_VBATVALID_DTCbak//:电压过低导致无法初始化；
#define	VFF_NOZZLEINI_NOINJ_DTC                 VFF_NOZZLEINI_NOINJ_DTCbak//:喷油器故障导致无法初始化；
#define	VFF_NOZZLEINI_T1DFAIL_T4LET1_DTC        VFF_NOZZLEINI_T1DFAIL_T4LET1_DTCbak//扫描结果的T4<T1d+500，认为结果有问题
#define	VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTC VFF_NOZZLEINI_ABCSCAN_RESULTINVALID_DTCbak//T1D扫描之后，计算得到的系数非法；2009-3-5
#define	VFF_NOZZLEINI_NOT4_DTC                  VFF_NOZZLEINI_NOT4_DTCbak//2009-11-29 无t4,导致无法初始化；

extern volatile V_STATUS_STR _V_EE_STATUS;//@ADD_V_EE_STATUS;
#define V_EE_STATUS  _V_EE_STATUS.Byte
#define	VF_JINI_EE	_V_EE_STATUS.Bits.b0
#define	VF_JPROECU_EE	_V_EE_STATUS.Bits.b1
#define	VF_JBEWR_EE	_V_EE_STATUS.Bits.b2
#define	VF_OPEREE_1CYCLE_EE	_V_EE_STATUS.Bits.b3
#define VF_DOUBLEER_EE  	_V_EE_STATUS.Bits.b4//2010-1-4 data(x)+1 !=data(xb)

#define VF_IICER_EE  	_V_EE_STATUS.Bits.b5//2013-2-18
#define VF_NOACK_EE  	_V_EE_STATUS.Bits.b6//2013-2-18
#define VF_ISWRITING_EE  	_V_EE_STATUS.Bits.b7//2013-2-18   EE是不是正在写入中


extern volatile V_STATUS_STR _V_IDLEAQ_STATUS;//@ADD_V_IDLEAQ_STATUS;
#define V_IDLEAQ_STATUS    _V_IDLEAQ_STATUS.Byte
#define	VF_1STAQ_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b0
#define	VF_NOTAQING_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b1
#define	VF_LIMITAQ_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b2
#define	VF_LOWRPM_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b3
#define VF_AQSEVERALTIMES_IDLEAQ     _V_IDLEAQ_STATUS.Bits.b4  //2009-4-21
#define	VF_AQINI_FAIL_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b5//2009-9-14
#define	VF_AQMORETHAN1TIME_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b6//2010-2-7 AQ1次以上
#define	VF_AQINI_ENRICHTOOSHORT_IDLEAQ	_V_IDLEAQ_STATUS.Bits.b7      //当怠速初始化时，如果第一次往浓扫描时，步数过少，则置位；

extern volatile V_STATUS_STR _V_CFCI_STATUS;//@ADD_V_CFCI_STATUS;
#define V_CFCI_STATUS  _V_CFCI_STATUS.Byte
#define	VF_CF_CFCI	_V_CFCI_STATUS.Bits.b0	//断油标志位                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
#define	VF_CFLST_CFCI	_V_CFCI_STATUS.Bits.b1
#define	VF_CFLST2_CFCI	_V_CFCI_STATUS.Bits.b2
#define	VF_JCI_LST_CFCI	_V_CFCI_STATUS.Bits.b3//2013-1-14

#define	VF_CICORECING_CFCI	_V_CFCI_STATUS.Bits.b4	//再给油修正状态
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
#define	VF_C2OPEN_IACV	_V_IACV_STATUS.Bits.b6       //2018-06-28 开路好判断是哪个线圈开路，所以可以分开
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
#define	VF_FUELLIMT_TRANS	_V_TRANSH_STATUS.Bits.b2	//2017-3-27 加速过程LD限制作用标志位，开始作用时置位，油量恢复并圆滑过渡后复位
#define	VF_TRANSLT10_TRANS	_V_TRANSH_STATUS.Bits.b3//2009-8-7 add 正在油门变动中，加速值大于3
#define	VF_TRANSGT15_TRANS	_V_TRANSH_STATUS.Bits.b4//2009-8-7 add 正在加速中，加速值大于3
#define	VF_ACCMID_TRANS	_V_TRANSH_STATUS.Bits.b5//2010-1-11 油门中等加速
#define	VF_ACCMIDLST_TRANS	_V_TRANSH_STATUS.Bits.b6//2010-1-11 油门中等加速

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
#define	VF_IDLEAIRLESS_IGN	    _V_IGN_STATUS.Bits.b2//2009-4-17 ADD 怠速气量偏少
#define	VF_IDLEIGNFK_NORMAL_IGN	    _V_IGN_STATUS.Bits.b3//
#define VF_QHYCA_INTERR_IGN     _V_IGN_STATUS.Bits.b4//2009-8-7 角标前后沿时间间隔有误
#define	VF_IDLEIGNFK_IGN    	_V_IGN_STATUS.Bits.b5 //2009-12-18 怠速点火反馈
#define	VF_REVTARGET_INC_IGN    	_V_IGN_STATUS.Bits.b6 //处于基于点火角的怠速提高阶段

#define	VF_AIRLESS_WHENIDLEAQ_IGN    	_V_IGN_STATUS.Bits.b7 //怠速搜索时,点火角偏大;//2010-1-10


extern volatile V_STATUS_STR _V_INJCTL_STATUS;//@ADD_V_INJCTL_STATUS;
#define V_INJCTL_STATUS    _V_INJCTL_STATUS.Byte
extern volatile V_STATUS_STR _V_INJCTLH_STATUS;//@ADD_V_INJCTLH_STATUS;
#define V_INJCTLH_STATUS    _V_INJCTLH_STATUS.Byte
extern volatile V_STATUS_STR _V_INJCTLHL_STATUS;//@ADD_V_INJCTLHL_STATUS; //2010-2-27
#define V_INJCTLHL_STATUS    _V_INJCTLHL_STATUS.Byte

#define	VF_FMF_REACHMIN_INJCTL	            _V_INJCTL_STATUS.Bits.b0  //2015-12-26 模型的油量减到了最小值

#define	VF_PUMP1ST_INJCTL	            _V_INJCTL_STATUS.Bits.b1  //2015-10-29  第一次泵油参数需要大一些,2015-12-17

#define	VF_MISST4_AFTERMATCHHIGH_INJCTL	    _V_INJCTL_STATUS.Bits.b2    //2010-1-11 add 
#define	VF_T3NOTEXIST_INJCTL	            _V_INJCTL_STATUS.Bits.b3    //T3不存在
#define	VF_OPEN_INJCTL	                    _V_INJCTL_STATUS.Bits.b4
#define	VF_SHORT_INJCTL	                    _V_INJCTL_STATUS.Bits.b5
#define	VF_T3ENABLE_INJCTL	                _V_INJCTL_STATUS.Bits.b6
#define	VF_T3ER_INJCTL	                    _V_INJCTL_STATUS.Bits.b7     //t3超范围

#define	VF_T3FKLST_INJCTL	        _V_INJCTLH_STATUS.Bits.b0
#define	VF_T3FKLIMIT_INJCTL	        _V_INJCTLH_STATUS.Bits.b1
#define	VF_T4SCATTER_INJCTL	    _V_INJCTLH_STATUS.Bits.b2    //t4散差大；
#define	VF_T1SCANING_INJCTL	        _V_INJCTLH_STATUS.Bits.b3
#define	VF_T1SCANINGBREAK_INJCTL	_V_INJCTLH_STATUS.Bits.b4//2009-2-19 是否需要中途终止T1扫描；
#define	VF_VBATFLU_WHENT4_INJCTL	        _V_INJCTLH_STATUS.Bits.b5//T4扫描时，电压有波动；
#define	VF_T4SCANOK1TIME_INJCTL	    _V_INJCTLH_STATUS.Bits.b6//T4扫描成功一次；
#define	VF_NEEDT35AGAIN_INJCTL	            _V_INJCTLH_STATUS.Bits.b7//无t3   2009-12-18
//2010-2-26
#define	VF_SEARCHT1I_INJCTL	        _V_INJCTLHL_STATUS.Bits.b0
#define	VF_SEARCHT4UP_INJCTL	    _V_INJCTLHL_STATUS.Bits.b1
#define	VF_SEARCHT4DOWN_INJCTL	    _V_INJCTLHL_STATUS.Bits.b2
#define	VF_T4UPGOT_INJCTL	        _V_INJCTLHL_STATUS.Bits.b3
#define	VF_T4DOWNGOT_INJCTL	        _V_INJCTLHL_STATUS.Bits.b4
#define	VF_T4LST_RELIABLE_INJCTL	        _V_INJCTLHL_STATUS.Bits.b5  //上一点T4平均值可靠；
#define	VF_SEARCHAGAIN_INJCTL	        _V_INJCTLHL_STATUS.Bits.b6
#define	VF_PUMP_INJCTL	        _V_INJCTLHL_STATUS.Bits.b7  //2013-3-23 ADD 

extern volatile V_STATUS_STR _V_NOTYSTDC_STATUS;//@ADD_V_NOTYSTDC_STATUS;
#define V_NOTYSTDC_STATUS  _V_NOTYSTDC_STATUS.Byte
#define	VF_ADDINJ_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b0
#define	VF_ADDINJLST_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b1
#define	VF_ADDINJNEED_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b2	//加喷标志位
#define	VF_NEEDDIVFMF_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b3  //2009-12-18 MOVE HERE 喷射次数切换标志
#define	VF_T35TESTNEED_NOTYSTDC	_V_NOTYSTDC_STATUS.Bits.b4	//是否需要T35检测
#define	VF_2INJATINJPHASE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b5   //2014-4-4喷射圈已经喷了两次了
#define	VF_LIDLE_ADVANCE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b6   //2013-5-16
#define	VF_4INJ_ENABLE_INJPHASE	_V_NOTYSTDC_STATUS.Bits.b7   //2012-9-14 

extern volatile V_STATUS_STR _V_INJPHASE_STATUS;//@ADD_V_INJPHASE_STATUS;
#define V_INJPHASE_STATUS  _V_INJPHASE_STATUS.Byte
#define	VF_FORINJ_INJPHASE	_V_INJPHASE_STATUS.Bits.b0
#define	VF_MODE1_INJPHASE	_V_INJPHASE_STATUS.Bits.b1	//非压缩上止点喷射阶段且转速小于5250rpm
#define	VF_MODE1TO2_INJPHASE	_V_INJPHASE_STATUS.Bits.b2
#define	VF_SWITCHPHASE_INJPHASE	_V_INJPHASE_STATUS.Bits.b3
#define	VF_YSTDC_INJPHASE	_V_INJPHASE_STATUS.Bits.b4      //2012-10-21　启动时设置为FALSE
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
extern volatile byte VF_OLDCA_CASbak;//		_V_CASH_STATUS.Bits.b0//角标到来时置FALSE,处理完转速后置TRUE;
extern volatile byte VF_OLDCAHY_CASbak;//		_V_CASH_STATUS.Bits.b1
extern volatile byte VF_PCAOV_CASbak;//		_V_CASH_STATUS.Bits.b2
extern volatile byte VF_TOOSPEED_CASbak;//		_V_CASH_STATUS.Bits.b3
extern volatile byte VF_MAINTMROV_CASbak;//		_V_CASH_STATUS.Bits.b4
extern volatile byte VF_CATCH_CASbak;//		_V_CASH_STATUS.Bits.b5
extern volatile byte VF_CATCHHY_CASbak;//		_V_CASH_STATUS.Bits.b6
extern volatile byte VF_NEWCYCLE_ATMAIN_CASbak;//		_V_CASH_STATUS.Bits.b7

#define	VF_OLDCA_CAS							VF_OLDCA_CASbak//		_V_CASH_STATUS.Bits.b0//角标到来时置FALSE,处理完转速后置TRUE;
#define	VF_OLDCAHY_CAS						VF_OLDCAHY_CASbak//		_V_CASH_STATUS.Bits.b1
#define	VF_PCAOV_CAS							VF_PCAOV_CASbak//		_V_CASH_STATUS.Bits.b2
#define	VF_TOOSPEED_CAS						VF_TOOSPEED_CASbak//		_V_CASH_STATUS.Bits.b3
#define	VF_MAINTMROV_CAS					VF_MAINTMROV_CASbak//		_V_CASH_STATUS.Bits.b4
#define	VF_CATCH_CAS							VF_CATCH_CASbak//		_V_CASH_STATUS.Bits.b5
#define	VF_CATCHHY_CAS						VF_CATCHHY_CASbak//		_V_CASH_STATUS.Bits.b6
#define	VF_NEWCYCLE_ATMAIN_CAS		VF_NEWCYCLE_ATMAIN_CASbak//		_V_CASH_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_CASHL_STATUS;//@ADD_V_CASHL_STATUS; //2014-4-15 ADD 
#define V_CASHL_STATUS                _V_CASHL_STATUS.Byte
#define	VF_CA_BEDIAGED_CAS          	_V_CASHL_STATUS.Bits.b0//一圈内已经诊断过角标了
#define	VF_CADIAG_TIMEIN_CAS	        _V_CASHL_STATUS.Bits.b1//一圈内到达需要诊断CA的时间了
#define	VF_CAHYPREDICT_TIMEIN_CAS   	_V_CASHL_STATUS.Bits.b2//预测后沿应该到了的时间
#define	VF_CAST_BESET_CAS   	        _V_CASHL_STATUS.Bits.b3//基准CA作为一个循环的开始，基准CA设置后，这个置位
#define	VF_CASHY_BECHECK1_CAS   	        _V_CASHL_STATUS.Bits.b4//已经校核过CAS的后沿
#define	VF_CASHY_BECHECK2_CAS   	        _V_CASHL_STATUS.Bits.b5//已经校核过CAS的后沿
#define	VF_CASHY_BECHECK3_CAS   	        _V_CASHL_STATUS.Bits.b6//已经校核过CAS的后沿VF_VRSLIMT_H
#define	VF_VRSLIMT_H   	        _V_CASHL_STATUS.Bits.b7//角标门限的切换


extern volatile V_STATUS_STR _V_INTINJ_STATUS;//@ADD_V_INTINJ_STATUS;
#define V_INTINJ_STATUS    _V_INTINJ_STATUS.Byte
#define	VF_AT3GETAFTERSTART_INTINJ	_V_INTINJ_STATUS.Bits.b0
#define	VF_PINJB_INTINJ	_V_INTINJ_STATUS.Bits.b1
#define	VF_PINJE_INTINJ	_V_INTINJ_STATUS.Bits.b2
#define	VF_PTRIG_INTINJ	_V_INTINJ_STATUS.Bits.b3
#define	VF_T3GET_INTINJ	_V_INTINJ_STATUS.Bits.b4
#define	VF_T4GET_INTINJ	_V_INTINJ_STATUS.Bits.b5
#define	VF_T3BECHECK_INTINJ	_V_INTINJ_STATUS.Bits.b6//T3被检验并修正，2010-3-3

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

#define	VF_AFRAME_NEED_INTSCI	_V_INTSCI_STATUS.Bits.b6//需要发送一帧数据

#define	VF_TOBE_DECODE_INTSCI	_V_INTSCI_STATUS.Bits.b7 //2013-9-3  缓冲区有命令待解析


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
#define	VF_IDLELEAN1_OS	_V_OS_STATUS.Bits.b3 //2010-7-11   怠速时已经稀过一次


extern volatile V_STATUS_STR _V_OSDIAG_STATUS;//@ADD_V_OSDIAG_STATUS;
#define V_OSDIAG_STATUS    _V_OSDIAG_STATUS.Byte
extern volatile V_STATUS_STR _V_OSDIAGH_STATUS;//@ADD_V_OSDIAGH_STATUS;
#define V_OSDIAGH_STATUS    _V_OSDIAGH_STATUS.Byte
extern volatile V_STATUS_STR _V_OSDIAGHL_STATUS;//@ADD_V_OSDIAGHL_STATUS;
#define V_OSDIAGHL_STATUS    _V_OSDIAGHL_STATUS.Byte
#define	VF_O2NOTWORK_CF	_V_OSDIAG_STATUS.Bits.b0	//2018-7-6 断油期间引起的氧传感器不工作状态
#define	VF_O2CHANGED_OSDIAG	_V_OSDIAG_STATUS.Bits.b1	//氧传感器OS波动超过一定次数（6次）
#define	VF_ENRICH_FORTESTLEAN_OSDIAG	_V_OSDIAG_STATUS.Bits.b2           //加浓以测试是否真的偏稀
#define	VF_OSLIMITMUST_ENLARGEATNOTIDLE_OSDIAG	_V_OSDIAG_STATUS.Bits.b3    //2009-12-14 非怠速的os必须放开
#define	VF_OSLIMITMUST_ENLARGEATIDLE_OSDIAG	_V_OSDIAG_STATUS.Bits.b4    //2009-12-14 怠速的os必须放开
#define	VF_LEANLIMIT_OSDIAG	_V_OSDIAG_STATUS.Bits.b5     //2009-12-13 防止向稀反馈
#define	VF_TESTFOROSLEAN_OSDIAG	_V_OSDIAG_STATUS.Bits.b6
#define	VF_O2WORK_OSDIAG	_V_OSDIAG_STATUS.Bits.b7

#define	VF_O2FKFAST_OS	_V_OSDIAGH_STATUS.Bits.b0       //2009-12-24
//#define	VF_OSMAYBECOOL_OSDIAG	_V_OSDIAGH_STATUS.Bits.b1
#define	VF_RCHO2MAX_OSDIAG	_V_OSDIAGH_STATUS.Bits.b2
#define	VF_RCHO2MIN_OSDIAG	_V_OSDIAGH_STATUS.Bits.b3
#define	VF_RICHLST_OSDIAG	_V_OSDIAGH_STATUS.Bits.b4
#define	VF_RICH_OSDIAG	_V_OSDIAGH_STATUS.Bits.b5	//氧传感器反馈偏浓
#define	VF_SCHO2MAX_OSDIAG	_V_OSDIAGH_STATUS.Bits.b6
#define	VF_SCHO2MIN_OSDIAG	_V_OSDIAGH_STATUS.Bits.b7

//2009-12-12 OS故障统一放置于此处；开钥匙时才清除；
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
#define	VF_PAIR_BYMANNUAL_PAIR	_V_PAIR_STATUS.Bits.b2  //手工设定
#define	VF_PAIR_VALID_FOREE_PAIR	_V_PAIR_STATUS.Bits.b3
#define	VF_NOTCAB_PAIR	_V_PAIR_STATUS.Bits.b4
#define	VF_VALUEERINEE_PAIR	_V_PAIR_STATUS.Bits.b5
#define	VF_ECUHARD_WITHPAIR_PAIR	_V_PAIR_STATUS.Bits.b6 //ECU硬件带压力传感器
#define	VF_ECUHARD_WITHOUTPAIR_PAIR	_V_PAIR_STATUS.Bits.b7 //ECU硬件不带压力传感器

extern volatile V_STATUS_STR _V_REV_STATUS;//@ADD_V_REV_STATUS;
#define V_REV_STATUS   _V_REV_STATUS.Byte
#define	VF_CALCUED_REV	_V_REV_STATUS.Bits.b0
#define	VF_DEC_REV	_V_REV_STATUS.Bits.b1
#define	VF_UB_REV	_V_REV_STATUS.Bits.b2
#define	VF_GE2200_REV	_V_REV_STATUS.Bits.b3//2009-12-19
#define VF_PAIR_BESAMP_WHENMOVE_REV _V_REV_STATUS.Bits.b4//2010-5-11
#define VF_LE_IDLETG_ONCE_REV _V_REV_STATUS.Bits.b5//2013-4-19

#define	VF_GT10_IDLETG_REV	_V_REV_STATUS.Bits.b6//2013-4-7
#define	VF_GT30_IDLETG_REV	_V_REV_STATUS.Bits.b7//2013-4-7 高怠速？设定的怠速目标转速+30*15.6


extern volatile V_STATUS_STR _V_STARTS_STATUS;//@ADD_V_STARTS_STATUS;
#define V_STARTS_STATUS    _V_STARTS_STATUS.Byte
#define	VF_1STSTART_STARTS	_V_STARTS_STATUS.Bits.b0//2012-11-24第一次启动
#define	VF_LEAVEIDLE_FRE_ENG	_V_STARTS_STATUS.Bits.b2  //2010-6-4 发动机频繁离开怠速 
#define	VF_BE_PREINJ_ENG	_V_STARTS_STATUS.Bits.b3  //2012-8-6 

#define VF_REMAINIDLE_STARTS _V_STARTS_STATUS.Bits.b4  //2013-1-26启动后一直保持油门为怠速

extern volatile V_STATUS_STR _V_ENG_STATUS;//@ADD_V_ENG_STATUS;
#define V_ENG_STATUS   _V_ENG_STATUS.Byte

#define	VF_BFIRE_ENG	_V_ENG_STATUS.Bits.b0	//着火标志位,true代表没着火
#define	VF_IDLE_ENG	_V_ENG_STATUS.Bits.b1	//油门在怠速标志位VF_E0_TPS
#define	VF_INJRUN_ENG	_V_ENG_STATUS.Bits.b2
#define	VF_JBACKIDLE_ENG	_V_ENG_STATUS.Bits.b3	//油门刚回怠速标志位，油门从非怠速到怠速时置位，新一圈来临时复位
#define	VF_JMMOVE_ENG	_V_ENG_STATUS.Bits.b4
#define	VF_JBACKIDLE_LST_ENG	_V_ENG_STATUS.Bits.b5
#define	VF_JLEAVEIDLELST_ENG	_V_ENG_STATUS.Bits.b6     //肯定不是ECE   //2013-10-24 VF_SHOULDNOTBE_ECE_ENG 被VF_JLEAVEIDLELST取代_ENG
#define	VF_JLEAVEIDLE_ENG	_V_ENG_STATUS.Bits.b7

//extern volatile V_STATUS_STR _V_ENGH_STATUS;//@ADD_V_ENGH_STATUS; //45
extern volatile byte VF_JLEAVEIDLETRANS_ENGbak;	//刚离开怠速时
extern volatile byte VF_JSTOP_ENGbak;    //2014-3-30 刚停机
extern volatile byte VF_MMOVE_ENGbak;
extern volatile byte VF_NOINJ_ENGbak;     //不需要喷油，包括超速断油的情况
extern volatile byte VF_RUNTOOS_ENGbak;   //软复位时，需要保留该标记
extern volatile byte VF_STARTS_ENGbak;
extern volatile byte VF_STOPBYTOOHOT_ENGbak;
extern volatile byte VF_TOIDLE_ENGbak;
//#define V_ENGH_STATUS   _V_ENGH_STATUS.Byte
#define	VF_JLEAVEIDLETRANS_ENG	VF_JLEAVEIDLETRANS_ENGbak	//刚离开怠速时
#define	VF_JSTOP_ENG						VF_JSTOP_ENGbak    //2014-3-30 刚停机
#define	VF_MMOVE_ENG						VF_MMOVE_ENGbak
#define	VF_NOINJ_ENG						VF_NOINJ_ENGbak     //不需要喷油，包括超速断油的情况
#define	VF_RUNTOOS_ENG					VF_RUNTOOS_ENGbak   //软复位时，需要保留该标记
#define	VF_STARTS_ENG						VF_STARTS_ENGbak
#define	VF_STOPBYTOOHOT_ENG			VF_STOPBYTOOHOT_ENGbak
#define	VF_TOIDLE_ENG						VF_TOIDLE_ENGbak

extern volatile V_STATUS_STR _V_ENGHL_STATUS;//@ADD_V_ENGHL_STATUS;
#define V_ENGHL_STATUS   _V_ENGHL_STATUS.Byte
#define	VF_TURNONINJ_ENG	_V_ENGHL_STATUS.Bits.b0
#define	VF_WORM_ENG	_V_ENGHL_STATUS.Bits.b1
#define	VF_WARMUP_ENG	_V_ENGHL_STATUS.Bits.b2    //2009-11-19  //启动增油消失，且缸头温度大于40时，
#define VF_CONIDLE_MORETHAN3S_ENG _V_ENGHL_STATUS.Bits.b3//2009-12-13      连续怠速超过3s;
#define	VF_JLEAVEIDLETRANS_LST_ENG	_V_ENGHL_STATUS.Bits.b4  //2012-11-20 ADD
#define	VF_WORM_LST_ENG	_V_ENGHL_STATUS.Bits.b5
#define	VF_HOTSTART_ENG	_V_ENGHL_STATUS.Bits.b6//2009-12-31 取消高怠速
#define	VF_MAYBE_ECE_ENG	_V_ENGHL_STATUS.Bits.b7//2010-1-29 

extern volatile V_STATUS_STR _V_SYS_STATUS;//@ADD_V_SYS_STATUS;
#define V_SYS_STATUS   _V_SYS_STATUS.Byte
#define	VF_DOM_SYS	_V_SYS_STATUS.Bits.b0
#define	VF_MILELIMIT_WARN_SYS	_V_SYS_STATUS.Bits.b1  //2012-12-6 ,里程到期的警告
#define	VF_MILELIMIT_DUE_SYS	_V_SYS_STATUS.Bits.b2            //2012-12-6，里程到期
//#define VF_BEFOREINJ_INJPHASE _V_SYS_STATUS.Bits.b3 //2012-12-11
#define VF_BEINJ_1CYC_INJPHASE _V_SYS_STATUS.Bits.b3 //2012-12-11

//2013-4-26 临时移动至此  2013-6-18
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

#define	VF_IDLECONDICTION_MATCH_SYSINI	_V_SYSINI_STATUS.Bits.b0 //初始化状态下，满足怠速搜索的条件，2010-4-10
#define	VF_RSTTISCI_SYSINI          	_V_SYSINI_STATUS.Bits.b1
#define	VF_IDLEAIRTEST_SYSINI           _V_SYSINI_STATUS.Bits.b3  //在怠速粗调后,精调前,测试怠速气量是否过低
#define VF_ENAQ_SYSINI                  _V_SYSINI_STATUS.Bits.b4  //2009-9-11 增加，初始化状态下，开始开环搜索的标志
#define	VF_JFINISHINI_WHENMOVING_SYSINI _V_SYSINI_STATUS.Bits.b5  //2009-11-25 //如果怠速刚初始化完，且发动机还继续在运行，则置该位，目的在于此时不显示故障
#define	VF_AQAFTER_PNINI_SYSINI         _V_SYSINI_STATUS.Bits.b7  //2009-11-25 //如果怠速刚初始化完，且发动机还继续在运行，则置该位，目的在于此时不显示故障

#define	VF_TIFKOK_SYSINI	        _V_SYSINIH_STATUS.Bits.b0
#define	VF_TIOK_SYSINI              _V_SYSINIH_STATUS.Bits.b1
#define	VF_TISETOK_SYSINI	        _V_SYSINIH_STATUS.Bits.b2
#define	VF_PREINJEND_SYSINI     	_V_SYSINIH_STATUS.Bits.b3      //2008-12-16  MOVE HERE	预喷结束标志
#define	VF_PREPUMPEND_SYSINI	    _V_SYSINIH_STATUS.Bits.b4  //2008-12-16  MOVE HERE	泵油结束标志，停机后或泵油停止置位；开钥匙或软件复位时复位
#define	VF_NOTNEED_DROPSCAN_SYSINI	_V_SYSINIH_STATUS.Bits.b5      //2008-12-16  MOVE HERE	开钥匙置位，自检复位
#define	VF_PUMPING_SYSINI     	_V_SYSINIH_STATUS.Bits.b6      //2013-4-3 add
#define	VF_PREINJ_2CA     	_V_SYSINIH_STATUS.Bits.b7 

//注意：
//V_SYSINIHL_STATUS与V_SYSINIHH_STATUS在软复位后仍保持；
#define VF_NOINIT_SYSINI        _V_SYSINIHL_STATUS.Bits.b0  //未调试
#define VF_TIER_SYSINI          _V_SYSINIHL_STATUS.Bits.b1  
#define VF_AQOK_SYSINI          _V_SYSINIHL_STATUS.Bits.b2  // b7
#define	VF_PNNOINI_SYSINI	    _V_SYSINIHL_STATUS.Bits.b3     //2009-7-31  动力喷嘴未初始化标志
#define	VF_ECUNOSET_SYSINI	    _V_SYSINIHL_STATUS.Bits.b4      //ECU没有编号
#define	VF_1STINI_FAIL_SYSINI	_V_SYSINIHL_STATUS.Bits.b6      //强制自检失败，非电压原因

#define VF_RUNENOUGH_FORINI_SYSINI  _V_SYSINIHH_STATUS.Bits.b0  
#define VF_CLR_SL_SYSINI        _V_SYSINIHH_STATUS.Bits.b1  //2013-4-2
#define VF_1ST_INI_SYSINI           _V_SYSINIHH_STATUS.Bits.b4      //2010-3-5 更改为需要怠速时测T4的标志，也需要保留
#define VF_SEARCHTMP_SYSINI         _V_SYSINIHH_STATUS.Bits.b6  //需要保留//2010-3-6
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
#define	VF_RUN_MORE5S_TMCCL	_V_TMCCLH_STATUS.Bits.b5          //2012-8-22 运行超过5S
#define	VF_D1S_TMCCL	_V_TMCCLH_STATUS.Bits.b6	//2017-3-25 增加0.1s标志位
#define	VF_MORE1MIN_TMCCL	_V_TMCCLH_STATUS.Bits.b7

extern volatile V_STATUS_STR _V_TMP_STATUS;//@ADD_V_TMP_STATUS;
#define V_TMP_STATUS   _V_TMP_STATUS.Byte
#define	VF_1STGET_TMP	_V_TMP_STATUS.Bits.b0
#define	VF_TASER_TMP	_V_TMP_STATUS.Bits.b1
#define	VF_TESER_TMP	_V_TMP_STATUS.Bits.b2
#define	VF_TCHANGED_TMP	_V_TMP_STATUS.Bits.b3
#define	VF_TESSTABLE_TMP	_V_TMP_STATUS.Bits.b4 //2012-8-1 TES到达稳定
#define	VF_TESLTTASYC_TMP	_V_TMP_STATUS.Bits.b5 //2012-8-1 TES比预测的TAS小
#define	VF_TESMAYERR_TMP	_V_TMP_STATUS.Bits.b6   //2012-8-2
#define	VF_TENG_FORECE_TMP	_V_TMP_STATUS.Bits.b7   //2008-12-16 ADD 

extern volatile V_STATUS_STR _V_TPS_STATUS;//@ADD_V_TPS_STATUS;
extern volatile V_STATUS_STR _V_TPSH_STATUS;//@ADD_V_TPSH_STATUS;
#define V_TPS_STATUS   _V_TPS_STATUS.Byte
#define	VF_E0LAST_TPS	_V_TPS_STATUS.Bits.b0
#define	VF_ER_TPS	_V_TPS_STATUS.Bits.b1
#define	VF_MAX_TPS	_V_TPS_STATUS.Bits.b2	//节气门全开状态
#define	VF_UB_TPS	_V_TPS_STATUS.Bits.b3
#define	VF_WOT_TPS	_V_TPS_STATUS.Bits.b4
#define	VF_MAXLST_TPS	_V_TPS_STATUS.Bits.b5
#define	VF_DRYIGNI_TPS	_V_TPS_STATUS.Bits.b6  //2008-12-16
#define	VF_E0_TPS	_V_TPS_STATUS.Bits.b7  //2009-1-13     TPS=0
//2009-9-7 ADD
#define V_TPSH_STATUS   _V_TPSH_STATUS.Byte
#define	VF_INCTHAN6_TPS	    _V_TPSH_STATUS.Bits.b0

#define	VF_E0MORETHAN4S_TPS	    _V_TPSH_STATUS.Bits.b1  //油门在怠速的时间超过4s
#define	VF_NOTIDLE_LE3_TPS	    _V_TPSH_STATUS.Bits.b2  //油门离开怠速后,
#define	VF_TPSINCLE0P1_TPS	    _V_TPSH_STATUS.Bits.b3  //油门增量小于0.1级//2010-1-11
#define	VF_VALID_TPS	    _V_TPSH_STATUS.Bits.b4  //2013-4-8

#define	VF_TPS0_MIN_GOTTEN1ST_TPS	    _V_TPSH_STATUS.Bits.b5  //2013-4-14第一次获得TPS最小值     
#define	VF_TPS_AFTER_GOTTEN1ST_TPS	    _V_TPSH_STATUS.Bits.b6  //2013-4-14 atfer 第一次获得TPS最小值
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
#define	VF_FAILURE_ETC	_V_ETC_STATUS.Bits.b1  //ETC 不能工作，2012-11-2
#define	VF_LUBEMPTY_ETC	_V_ETC_STATUS.Bits.b2  //无机油，2012-11-2

//#define VF_TOBE_RUN	_V_ETC_STATUS.Bits.b3 
#define VF_BYTPD_ETC	_V_ETC_STATUS.Bits.b3  //按目标油门控制，2015-12-16
#define VF_INC_ETC	_V_ETC_STATUS.Bits.b4//2012-11-14 
#define VF_ACC_ETC	_V_ETC_STATUS.Bits.b5//2012-11-14//电机突然启动加速 
#define VF_INC_LST_ETC	_V_ETC_STATUS.Bits.b6//2012-11-14//电机上一次是增
#define VF_DEC_ETC	_V_ETC_STATUS.Bits.b7//2012-11-14//电机突然启动逆向加速 
 
extern volatile V_STATUS_STR _V_LDSENSOR_STATUS;//@ADD_V_LOADSENSOR_STATUS;    //ae-70=62
#define V_LDSENSOR_STATUS  _V_LDSENSOR_STATUS.Byte
#define	VF_CAP0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b0
#define	VF_CAP1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b1

#define	VF_OPEN0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b2
#define	VF_OPEN1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b3

#define	VF_MAYACC0_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b4//负荷可能有突变
#define	VF_MAYACC1_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b5

#define	VF_INC_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b6
#define	VF_DEC_LDSENSOR	_V_LDSENSOR_STATUS.Bits.b7//负荷突减

extern volatile V_STATUS_STR _V_LOCK_STATUS;//@ADD_V_LOCK_STATUS;    //2013-6-13 关于锁的设置
#define V_LOCK_STATUS         _V_LOCK_STATUS.Byte
#define	VF_SETLOCK_LOCK	      _V_LOCK_STATUS.Bits.b0      //设置锁，即上锁，  1 for yes
#define	VF_KEYON_LOCK	        _V_LOCK_STATUS.Bits.b1      //锁是不是被打开，  1 for yes
#define	VF_SETLIMIT_LOCK	    _V_LOCK_STATUS.Bits.b2      //是否设置授权使用，1 for yes
#define	VF_BKEYERR_LOCK	      _V_LOCK_STATUS.Bits.b3      //密码是否出错，    1 for yes
#define	VF_LIMITDUE_LOCK	    _V_LOCK_STATUS.Bits.b4      //授权使用是否到期，1 for yes
#define	VF_BPERMITERR_LOCK	  _V_LOCK_STATUS.Bits.b5      //授权密码出错，    1 for yes

#define	VF_PWNOINI_LOCK	      _V_LOCK_STATUS.Bits.b6      //没有设置过初始密码 ,//是否使用密码 2014-6-24 更改含义

#define	VF_MODE_INTSCI	      _V_LOCK_STATUS.Bits.b7      //2013-9-3  0：诊断模式，1：是防盗模式
#define COMMODE_DIAG  0
#define COMMODE_LOCK  1

extern volatile V_STATUS_STR _V_LOCK2_STATUS;//@ADD_V_LOCK2_STATUS;    //2013-6-13 关于锁的设置
#define V_LOCK2_STATUS         _V_LOCK2_STATUS.Byte


#define	VF_EEPWS_VALID_LOCK	              _V_LOCK2_STATUS.Bits.b0        //ECU中存储的用户密码有效
#define	VF_EEPWL_VALID_LOCK	              _V_LOCK2_STATUS.Bits.b1         //ECU中存储的经销商密码有效
#define	VF_EESTTIME_VALID_LOCK	          _V_LOCK2_STATUS.Bits.b2       //ECU中存储的起始时间有效
#define	VF_EEEDTIME_VALID_LOCK	          _V_LOCK2_STATUS.Bits.b3       //ECU中存储的截至时间有效

#define	VF_PWIN_LOCK	                    _V_LOCK2_STATUS.Bits.b4       //2014-5-31 已经输入密码了 
#define	VF_NOTACTIVE_LOCK	                _V_LOCK2_STATUS.Bits.b5       //2014-5-8 ADD FOR,ECU没有被激活 
#define	VF_KEYON_FOR1STLOCK_LOCK	        _V_LOCK2_STATUS.Bits.b6       //第一次设置密码时，需要钥匙特殊操作：连续开关两次以上钥匙
#define	VF_KEYOFF_SHORT_LOCK	            _V_LOCK2_STATUS.Bits.b7       //钥匙关闭足够长时间，需要重新解锁


extern volatile V_STATUS_STR _V_ETC3_STATUS_RPMto_H;//@ADD_V_ETC3_STATUS;  //B2
//#define V_ETC3_STATUS  _V_ETC3_STATUS.Byte
#define V_ETC3_STATUS_RPMto_H  _V_ETC3_STATUS_RPMto_H.Byte

#define	VF_RPM_TO_H0	_V_ETC3_STATUS_RPMto_H.Bits.b0     // LCW2013-1-19 RPM上升达到第一个高转速点，当低于某值时清除             
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
#define VF_RPM_TO_L0  _V_ETC4_STATUS_RPMto_L.Bits.b0 // LCW2013-1-19 RPM下降达到第一个低转速点，当高于某值后清除
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
#define	VF_IAD_BEBACKUP_AD	_V_IGN2_STATUS.Bits.b4  //2014-12-24 储存电流采样值
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
#define	VF_MAY_UPDATE_FLASH	      _V_FLASH_STATUS.Bits.b1 //2014-8-13 也许接收到升级信号了
#define	VF_ISBOOTED_FLASH	      _V_FLASH_STATUS.Bits.b2 //2014-12-4 刚被升级过的

extern volatile V_STATUS_STR _V_FUELCOMP_STATUS;//@ADD_V_FUELCOMP_STATUS;
#define V_FUELCOMP_STATUS   _V_FUELCOMP_STATUS.Byte
#define VF_RPDACC_ST	_V_FUELCOMP_STATUS.Bits.b0//2018-6-27 急加速状态
#define VF_IDLEFUELCO_ST	_V_FUELCOMP_STATUS.Bits.b1//2018-6-27 回油门后怠速油量修正期间置位，油量恢复并圆滑过渡后复位
#define VF_U_CO	_V_FUELCOMP_STATUS.Bits.b2
#define VF_ADDFUEL_SET	_V_FUELCOMP_STATUS.Bits.b3
#define VF_QD_ST	_V_FUELCOMP_STATUS.Bits.b4
#define VF_CC_ST	_V_FUELCOMP_STATUS.Bits.b5		//20210819 chenwei add 侧撑熄火开关，端口接通且满足一定时间
#define VF_CC_CONNECT	_V_FUELCOMP_STATUS.Bits.b6		//20210819 chenwei add 侧撑熄火开关端口

/*---------------------2015-6-4 定义SPN---------------------------------*/
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
    //如果这个变化了，则同步的NUM_DTC_SCR，M1_SPN，M1_FMI，SCRDTC_ST需要一起变
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
 
    //PI系统，2016-2-23
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
	  #define VF_IACS_ERR  _V_SPN5_STATUS.Bits.b5    //20210308 chenwei add 怠速空气控制系统故障P0505
	  #define VF_MIL_OCOROPEN  _V_SPN5_STATUS.Bits.b6    //20210308 chenwei add MIL灯亮连接故障
	  #define VF_O2H_OPEN  _V_SPN5_STATUS.Bits.b7    //20210308 chenwei add O3H短接到负或断路故障
	  #define VF_O2H_OC  _V_SPN6_STATUS.Bits.b0    //20210308 chenwei add O3H短接到正级故障
	  //#define VF_INI_SYSINI	_V_SPN6_STATUS.Bits.b1	//20211004 chenwei add 环松初始化 
	  #define VF_CA_NOSIGNAL	_V_SPN6_STATUS.Bits.b1	//20211009 chenwei add 无角标信号故障

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


//20201109 chenwei add 欧五诊断相关标志位
extern volatile V_STATUS_STR _V_OSEUROVDIAG_STATUS;//@ADD_V_OSEUROVDIAG_STATUS;
#define V_OSEUROVDIAG_STATUS         _V_OSEUROVDIAG_STATUS.Byte
#define VF_SCHMAX_SUM                      _V_OSEUROVDIAG_STATUS.Bits.b0  //20201109 chenwei add有效VF_SCHMAX_SUM穿越下限的上升标志(300mv)
#define VF_SCHMIN_SUM                      _V_OSEUROVDIAG_STATUS.Bits.b1  //20201109 chenwei add有效VF_SCHMIN_SUM穿越上限的下降标志(600mv)
#define VF_EurVDiag_nbRL					_V_OSEUROVDIAG_STATUS.Bits.b2     //20201125 chenwei add 欧五专用的停机标志位，用于关钥匙后写入数据进入ee前
#define VF_EurVDiag_nbLR					_V_OSEUROVDIAG_STATUS.Bits.b3     //20201125 chenwei add 欧五专用的停机标志位，用于关钥匙后写入数据进入ee前
#define VF_CCPS_status					_V_OSEUROVDIAG_STATUS.Bits.b4     //20201126 chenwei add 欧五碳罐电磁阀启用条件
#define VF_CCPS_close					_V_OSEUROVDIAG_STATUS.Bits.b5         //20201127 chenwei add CCPS关闭瞬间的标志
#define VF_SAIC_status					_V_OSEUROVDIAG_STATUS.Bits.b6     //20201207 chenwei add 取代VF_SAIC_st_EnCond SAIC开启与否的标志，true为SAIC_ON，false为SAIC_OFF
#define VF_SAIC_Pin_Err					_V_OSEUROVDIAG_STATUS.Bits.b7     //20201130 chenwei add SAIC引脚开路



//ADD_V_ENGPRTECTION_STATUS
extern volatile V_STATUS_STR  _V_ENGPRTECTION_STATUS;//@ADD_V_ENGPRTECTION_STATUS;
#define V_ENGPRTECTION_STATUS         _V_ENGPRTECTION_STATUS.Byte
#define	VF_OVERTEMPRATRUEPROTECTION_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b0    //20210115 chenwei remark 满足过温保护条件的标志，1
#define	VF_OVERTEMANDHIGHLOAD_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b1    //20210115 chenwei add满足超过过温保护限值及中高负荷时置1，用于v_t_overTMP_1s累计
#define	VF_STATUS_SPEED_ENG 	          _V_ENGPRTECTION_STATUS.Bits.b2     //20210123 chenwei add 转速波动在某一范围以内
#define	VF_STATUS_TPD_ENG 	           _V_ENGPRTECTION_STATUS.Bits.b3     //20210123 chenwei add TPD波动在某一范围以内
#define VF_MIL_NEMDTC	               _V_ENGPRTECTION_STATUS.Bits.b4		//20210923 chenwei add 环松MIL信号仪表，新故障开始的标志
#define VF_MIL_DTC_LSTOVER	               _V_ENGPRTECTION_STATUS.Bits.b5		//20210924 chenwei add 代表上一个D故障码信号执行结束
#define VF_MIL_DTC_3D2S	               _V_ENGPRTECTION_STATUS.Bits.b6		//20210923 chenwei add 环松MIL信号仪表，执行闪烁第1位
#define VF_MIL_NEMDTC2	               _V_ENGPRTECTION_STATUS.Bits.b7


//20211014 chenwei add 环松氧加热相关
extern volatile V_STATUS_STR  _V_O2H_STATUS;//@ADD_V_O2H_STATUS;
#define V_O2H_STATUS         _V_O2H_STATUS.Byte
#define	VF_OS_HIGHVOLT 	          _V_O2H_STATUS.Bits.b0    //20211014 chenwei add 氧加热开路


#endif

