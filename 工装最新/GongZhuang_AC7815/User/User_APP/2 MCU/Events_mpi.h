/*HEAD
 ***********************************************************************
 * Filename	:events_mpi.h
 * Function	:事件函数头文件	
 * User		:yangsili
 * Date		:2017.12.28
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.28	yangsili
 *		1.添加mc33814_ResetProcess()函数声明
 *    2.添加CAN函数声明
 *
 ***********************************************************************
HEAD END*/

/** ###################################################################
**     Filename  : Events.h
**     Project   : GENERATOR
**     Processor : MC9S12G128CLH
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2013-11-22, 9:35
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "MCU.H"

#include "public.h"

#define true 1
#define false 0


//extern byte nb_reset;
extern byte bln;
extern byte zzq;
extern byte nb_qy_all;
extern byte nb_qy_valid;

extern byte nb_clk_er;//2014-5-9
extern byte nb_PLL_er;//2014-5-9

#define v_t_cas0                      _v_t_cas0.Word

extern word v_t_cashy0;
extern word v_TW3;

extern word v_TW4;        //2008-12-1 TW4   
extern word v_TW4_2nd;  //2010-3-4
extern word v_tw1_synwithTW3;//与TW3同步的T1 //2008-6-25

extern byte nb_hy_betweenqy;//当前两个前沿之间，后沿的个数； 
extern byte nb_qy_betweenhy;//当前两个后沿之间，前沿的个数；
extern sword v_FMF_SCI_idle;   //2008-8-2 to relace v_tw3_sci;
extern sword v_FMF_SCI_Nidle;  //2008-8-2非怠耸辟
extern word v_prd_cas_hy;  //角标后沿的周期;

extern byte nb_hyint; //后沿中断的数量
extern byte nb_hyint_all;


extern byte nb_injection_fort35;//2009-12-25

extern byte v_nb_injection_ccl; //2012-6-27

extern word v_t_IgnCA_act         ;
extern byte nb_CapI;


extern byte v_nb_binj; ///2012-11-24



extern word v_t_cashy0 ;
extern byte nb_qy;
extern word v_TW3;
//extern word v_TW3_in; 

extern word v_TW4;        //2008-12-1 TW4   
extern word v_TW4_2nd;  //2010-3-4
extern word v_tw1_synwithTW3;//与TW3同步的T1 //2008-6-25

extern byte nb_hy_betweenqy;//当前两个前沿之间，后沿的个数； 
extern byte nb_qy_betweenhy;//当前两个后沿之间，前沿的个数；
extern sword v_FMF_SCI_Nidle;  //2008-8-2非怠耸辟
extern word v_prd_cas_hy;  //角标后沿的周期;

extern byte nb_hyint; //后沿中断的数量
extern word t_1st_t4rising;
extern byte nb_injection_fort35;//2009-12-25

extern byte v_nb_injection_ccl; //2012-6-27

extern word v_t_IgnCA_act;
extern byte nb_CapI;

extern byte nb_RTI;

extern byte v_nb_binj; ///2012-11-24
extern word v_prd_cas_min;
extern word vlst_prd_cas;
extern word v_t_t3ed;
extern byte nb_tw4_in;
extern sbyte v_nb_DCPrd;   

//extern byte v_nb_notsyninj_add;
extern word v_t_t3ed;

extern byte v_nb_tdcerr;

extern unsigned char v_ToothPosition; //当前的齿号 2020-12-18


//for scr
    extern word v_nb_ForPump;//需要泵工作的次数
    extern word v_nb_inj_exe;//实际执行的喷射的次数；
    extern byte v_t_512us_delayForVbat;//VBAT采集必须在喷油结束后的512us后
    extern byte v_nb_forT30;


    extern byte nb_1s;
    extern byte nb_30ms;
    extern byte nb_200ms;
    extern byte nb_1s_ceshi;
    extern byte flag_ceshi;
    extern byte nb_t0_cnt;  //预喷射计数
    extern byte nb_t1_cnt;  //上电喷射计数
    extern byte nb_pre_inj;
    extern byte nb_Pre_T35;
    extern word v_f_kesei;
    extern word t_remark;
    extern word v_TW1_exe;
    extern byte nb_kesei;
    extern byte INJ_th_set;
    extern byte m_remark,m_kesei;
    extern byte nb_1s_ceshi1;


    extern byte v_nb_T3_miss;//T3丢失的次数； 2015-7-4
	
	extern byte v_CA_reset;
	
	extern byte VF_IGN_OVER;
	extern byte TachOff_Flag;
	
	extern sbyte v_NO_ignTooth_trig;//点火齿
	extern sbyte v_NO_dwellTooth_trig;
	extern word tw1_wm;

	
//20210423 chenwei add
	extern word T1;
	extern word T1_lst;
	extern byte switch_1;
	extern word frequency_1;
	extern word t_interval_4us;
	extern word T1_OFF;
	extern byte switch_2;
	extern word flow_1time_ul;//????

	extern byte nb_d5ms;
extern void FC_SM(byte blnIni);
void Init_Int_afterReset(void);//2014-3-30

//#pragma CODE_SEG DEFAULT


void API_10MS_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  API_10MS_OnInterrupt (module Events)
**
**     Component   :  API_10MS [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RTI_D5MS_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  RTI_D5MS_OnInterrupt (module Events)
**
**     Component   :  RTI_D5MS [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TR_SCI0_OnRxChar(void);
/*
** ===================================================================
**     Event       :  TR_SCI0_OnRxChar (module Events)
**
**     Component   :  TR_SCI0 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TR_SCI0_OnTxChar(void);
/*
** ===================================================================
**     Event       :  TR_SCI0_OnTxChar (module Events)
**
**     Component   :  TR_SCI0 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TR_SCI0_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  TR_SCI0_OnFreeTxBuf (module Events)
**
**     Component   :  TR_SCI0 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TR_SCI0_OnTxComplete(void);
/*
** ===================================================================
**     Event       :  TR_SCI0_OnTxComplete (module Events)
**
**     Component   :  TR_SCI0 [AsynchroSerial]
**     Description :
**         This event indicates that the transmitter is finished
**         transmitting all data, preamble, and break characters and is
**         idle. It can be used to determine when it is safe to switch
**         a line driver (e.g. in RS-485 applications).
**         The event is available only when both <Interrupt
**         service/event> and <Transmitter> properties are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/



void TC_IGNIO_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TC_IGNIO_OnInterrupt (module Events)
**
**     Component   :  TC_IGNIO [FreeCntr]
**     Description :
**         This event is called when a compare matches the counter
**         value (if compare or reload is selected as a interrupt
**         source) or a counter overflows (for free-running devices).
**         It is valid only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent">. The event is
**         available only if <Interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TC_INJO_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TC_INJO_OnInterrupt (module Events)
**
**     Component   :  TC_INJO [FreeCntr]
**     Description :
**         This event is called when a compare matches the counter
**         value (if compare or reload is selected as a interrupt
**         source) or a counter overflows (for free-running devices).
**         It is valid only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent">. The event is
**         available only if <Interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CA_capture_OnCapture(void);
/*
** ===================================================================
**     Event       :  CA_capture_OnCapture (module Events)
**
**     Component   :  CA_capture [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCL_AD_OnError(void);
/*
** ===================================================================
**     Event       :  SCL_AD_OnError (module Events)
**
**     Component   :  SCL_AD [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCL_AD_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SCL_AD_OnRxChar (module Events)
**
**     Component   :  SCL_AD [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCL_AD_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SCL_AD_OnTxChar (module Events)
**
**     Component   :  SCL_AD [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCL_AD_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  SCL_AD_OnFullRxBuf (module Events)
**
**     Component   :  SCL_AD [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCL_AD_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  SCL_AD_OnFreeTxBuf (module Events)
**
**     Component   :  SCL_AD [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void T4_OnCapture(void);
/*
** ===================================================================
**     Event       :  T4_OnCapture (module Events)
**
**     Component   :  T4 [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Vsamp_OnCapture(void);
/*
** ===================================================================
**     Event       :  Vsamp_OnCapture (module Events)
**
**     Component   :  Vsamp [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TDC_HY_OnCapture(void);
/*
** ===================================================================
**     Event       :  TDC_HY_OnCapture (module Events)
**
**     Component   :  TDC_HY [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CRTS2_OnCapture(void);
/*
** ===================================================================
**     Event       :  CRTS2_OnCapture (module Events)
**
**     Component   :  CRTS2 [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CRTS1_OnCapture(void);
/*
** ===================================================================
**     Event       :  CRTS1_OnCapture (module Events)
**
**     Component   :  CRTS1 [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

//#pragma CODE_SEG __NEAR_SEG INT_NEAR
void Cpu_OnClockMonitorFail(void);
//#pragma CODE_SEG DEFAULT
/*
** ===================================================================
**     Event       :  Cpu_OnClockMonitorFail (module Events)
**
**     Component   :  Cpu [MC9S12G128_48]
**     Description :
**         This event is called immediately after fetching of the
**         Clock monitor reset vector, i.e. Processor Expert
**         initialization and startup initialization have not been
**         done before (see generated code).
**         Propery 'Clock monitor' in the 'Clock settings' must be
**         set to 'Force reset' value in order to generate reset by
**         the clock monitor.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CA_capture_OnOverflow(void);
/*
** ===================================================================
**     Event       :  CA_capture_OnOverflow (module Events)
**
**     Component   :  CA_capture [Capture]
**     Description :
**         This event is called if counter overflows (only when the
**         component is enabled - <Enable> and the events are enabled -
**         <EnableEvent>.This event is available only if a <interrupt
**         service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnPllLockStatusChanged(void);
/*
** ===================================================================
**     Event       :  Cpu_OnPllLockStatusChanged (module Events)
**
**     Component   :  Cpu [MC9S12G128_64]
**     Description :
**         This event is triggered by any change of the LOCK status
**         bit. Enabling this event also allocate PLL interrupt
**         vector (see CPU interrupts on Properties tab of the Cpu
**         component). PLL must be enabled to enable this event.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_GPS_OnError(void);
/*
** ===================================================================
**     Event       :  SCI_GPS_OnError (module Events)
**
**     Component   :  SCI_GPS [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_GPS_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SCI_GPS_OnRxChar (module Events)
**
**     Component   :  SCI_GPS [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_GPS_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SCI_GPS_OnTxChar (module Events)
**
**     Component   :  SCI_GPS [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_GPS_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  SCI_GPS_OnFullRxBuf (module Events)
**
**     Component   :  SCI_GPS [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SCI_GPS_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  SCI_GPS_OnFreeTxBuf (module Events)
**
**     Component   :  SCI_GPS [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CAN_OnFreeTxBuffer(word BufferMask);  //2017-12-28,ysl,add

/*
** ===================================================================
**     Event       :  CAN_OnFullRxBuffer (module Events)
**
**     Component   :  CAN [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full
**         after a successful reception of a message. The event is
**         available only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN_OnFullRxBuffer(void); //2017-12-28,ysl,add

/*
** ===================================================================
**     Event       :  CAN_OnBusOff (module Events)
**
**     Component   :  CAN [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN_OnBusOff(void); //2017-12-28,ysl,add



/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.05 [04.46]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
