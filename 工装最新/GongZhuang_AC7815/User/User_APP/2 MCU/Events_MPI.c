/*HEAD
 ***********************************************************************
 * Filename	:events_mpi.c
 * Function	:calculation for events		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		1.ɾ��M0_PRO_3��VF_IS_CDI_INTIGN������.
 *		2.5000rpm���µ����ʱ������Ϊ600(2.4ms),5000rpm��������Ϊ400(1.6ms)
 *		3.ɾ��VF_RESERVE1_PRO2��ǰ���ؼ�������ã�����M0_MODE_CA��������
 *
 * V9.1.0	2017.12.4	yangsili 
 *		Base function.
 *
 * V9.2.0   2017.12.28	yangsili 
 *		1.����33814�Ǳ괦���ԭ�Ǳ괦��Ԥ��������
 *    2.��������ͨ��������T3ɨ��ʹ��
 *    3.����33814��λ�źŲ���ʱ�Ĵ�����
 *    4.CAN�����ж�����33814�Ĵ����ⲿ���߶�д��־��λ�ͻ�������
 ***********************************************************************
HEAD END*/

/** ###################################################################
**     Filename  : Events.c
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
/* MODULE Events */


#include "FAM.H"
#include "Service_CAN.h"

#include "Events.h"
#include "hard_trig.h"
#include "events.h"
#include "status_st.h"
#include "ign.h"

#include "add.h"

#include "public.h"

#include "sci.h"
#include "lock.h"

#include "wait_Moving.h"//2014-4-4

#include "DTC.H"

#include "vbat.h"//2014-8-12

#include "inj_ctl.h"

#include "ee.h"

#include "time_ccl.h"

#include "frame.h"//2015-10-25
#include "app_rw33814.h"//ysl,add
#include "act.h"
#include "tps.h"
#include "afr_cfci.h"
#include "ld.h"
#include "inj_phase.h"

byte nb_qy_betweenhy_now;
byte nb_tooth; 
byte nb_reset; 
byte nb_reset_lst;
byte nb_hy;
byte bln;
byte nb_hy_betweenqy_now;
byte zzq;

byte nb_10ms;

byte nb_qy_all;
byte nb_qy_valid;


byte nb_hyint; //�����жϵ�����
byte nb_hyint_all; //�����жϵ�����

byte nb_clk_er;//2014-5-9
byte nb_PLL_er;//2014-5-9


byte v_vbat_atInjOff;//2014-8-12


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//2014-9-25 move here from INT_SERVE.C
word v_t_cashy0;
word v_t_IgnCA_act;
sbyte v_IgnCA_act;
sbyte v_IgnCA_act_lst;    
word v_prd_cas_min;
word vlst_prd_cas;
word v_TW3;                             
word v_TW3_in;     //ֱ�������T3                        
word v_TW4;        //2008-12-1 TW4                     
word v_TW4_2nd;        //2008-12-1 TW4��ĵڶ�����                     
word v_tw1_synwithTW3;//��TW3ͬ����T1

sword v_FMF_SCI_idle;   //2008-8-2 to relace v_tw3_sci;
sword v_FMF_SCI_Nidle;  //2008-8-2�ǵ��ʱ�

byte nb_hy_betweenqy;//֮ǰ����ǰ��֮�䣬���صĸ�����//2008-8-1 add
byte nb_qy_betweenhy;//֮ǰ��������֮�䣬ǰ�صĸ�����//2008-8-1 add

byte nb_tw4_in;

byte v_nb_injection_ccl; //2012-6-27

byte nb_injection_fort35;//2009-12-25
word t_1st_t4rising;//2009-7-2

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
//2014-9-25 move here from INT_SERVE.C
byte nb_qy; //2009-8-30 add
byte v_nb_binj;
//�ۼ�������Ǳ������ĳ�����ֱ��255
//��ǰ����ǰ��֮�䣬���صĸ�����//2008-8-1 add
//��ǰ��������֮�䣬ǰ�صĸ�����//2008-8-1 add



word v_prd_cas_hy;  //�Ǳ���ص�����;

byte nb_RTI;
byte nb_CapI;

word v_t_t3ed;

byte v_nb_T3_miss;//T3��ʧ�Ĵ����� 2015-7-4
byte v_CA_reset;

//��ݷ��ֲ���
unsigned char v_ToothPosition; //��ǰ�ĳݺ�
unsigned char v_ToothPosition2; //��ǰ�ĳݺ�

byte v_Toothnum;

//#define NB_TOOTH  36//����
//#define UB_NO_TOOTH  34//���ĳݺ�


word v_tw_Tooth[NB_TOOTH];
word v_t_Tooth_lst;//��һ�ݵ�ʱ��
word v_t_Tooth_Now;

word v_tick_1stTooth;
word v_tick_1stTooth_lst;

byte v_nb_tdcerr;

byte VF_NEED_DWELL;
sbyte v_NO_ignTooth_trig;//����
sbyte v_NO_dwellTooth_trig;//���� 
byte VF_DWELL_SET;
byte TachOff_Flag;
byte VF_CHARGE_OK;		//20210618 chenwei add CDI_CHARGE��true��CDI_ON��false

byte VF_IGN_OVER;

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//2014-9-25 MOVE FROM HERE FROM INT_SERVE.C 
#define TXNUMBER        20                 //���͵��ֽ���  //2008-5-29 change from 20 to 30
#define PCBTDCZ         80
#define PCBTDCF         50
#define PCTBACKZ        126
#define PCTBACKF        94
#define PCTINJHZ        11
#define PCTINJHF        3
#define FMF_SCI_U       0xb00
#define FMF_SCI_L       0x300




#pragma CODE_SEG DEFAULT

//20210707 chenwei add
byte F_ChargeEn;	//0:�������˳��;1:�������˳���жϣ����ǻ�û�г��
byte F_Trig_IgnEn;	//0:�����˳�ţ���û����Ҫ����쳣 1�������ĳ�ź�Ҫ�������
byte F_Trig_IgnEn2;//20210420 chenwei add
word Trig_IgnTmeptime;

//20210930 chenwei add
byte v_nb_cycle;	//����Ȧ����Ŀǰ�ӵڵ�2Ȧ��ʼ������<10��
//20210423 chenwei add
word T1;
word T1_lst;
byte switch_1;
word frequency_1;
word t_interval_4us;
word T1_OFF;
byte switch_2;//????
word flow_1time_ul;//????

//20210606 chenwei add
byte SWITCH_IGN;
byte SWITCH_INJ;
byte v_IGN_control;
byte v_IGN_ON_OVER;
word Period_d5ms;
word Dwell_t_d5ms;

byte v_INJ_control;
byte v_IGN1_control;
byte Period_d5ms_INJ;
byte TW1_t_d5ms;

void Init_Int_afterReset(void)//2014-3-30
{
	sbyte sb;//20210301 chenwei add
    v_prd_cas=0xffff;
    v_TW3=1;//2008-3-27 Ϊ����֤����ͻ������ʱ��T3�쳣
    v_TW3_in=1;
    nb_hy=0;
    INJ_OFF;
  //  ISCV_OFF;
 //   FAN_OFF;
    //---------------------------------------------
    if(VF_VPOR_HARD)  //2008-8-10 ADD
    {   
        v_t_cas0=  0x0000;           												       													
        v_FMF_SCI_idle= 0;
        v_FMF_SCI_Nidle=0; //2008-8-10 MOVE TO INTSERVE.H
        nb_hy_betweenqy=1;
        nb_qy_betweenhy=1;
        V_CAS_STATUS=false;//2009-12-18 move here
        
        nb_qy_all=0;
        nb_qy_valid=0;
        
        nb_hyint=0;
        nb_hyint_all=0;
        
		VF_PREINJ_2CA=false;
        //nb_hyint_all=M1_NC_62D5[0];
        VF_CHARGE_OK=false;
					switch_1=0;//20210426 CHENWEI ADD
		frequency_1=0;//20210426 CHENWEI ADD

    }

	//20210708 chenwei add
		
        F_ChargeEn = 0;
		F_Trig_IgnEn = 1;
		Trig_IgnTmeptime = 0;
		F_Trig_IgnEn2=0;		//20210930 �˴�Ӧ�ó�ʼ��F_Trig_IgnEn2��ԭ�ȳ�ʼ������F_Trig_IgnEn=0
		
	
	VF_INJ_SET=false;
	VF_ADDFUEL_SET=false;

		T1=0;
		T1_lst=0;


    V_INTIGN_STATUS=V_INTINJ_STATUS=V_INTSCI_STATUS=V_TMR_STATUS=false;//2008-12-15
    
		VF_OLDCA_CASbak=0;
		VF_OLDCAHY_CASbak=0;
		VF_PCAOV_CASbak=0;
		VF_TOOSPEED_CASbak=0;
		VF_MAINTMROV_CASbak=0;
		VF_CATCH_CASbak=0;//		_V_CASH_STATUS.Bits.b5
		VF_CATCHHY_CASbak=0;//		_V_CASH_STATUS.Bits.b6
		VF_NEWCYCLE_ATMAIN_CASbak=0;//		_V_CASH_STATUS.Bits.b7
    
	/*2017-2-15 �ð汾���õ�е��ʽ��VF_IS_CDI_INTIGN=false
    //2016-9-18
        if(M0_PRO_3&0x80)
        {   //���ECU,ע�⣬Ӧ����V_INTIGN_STATUS֮��
            VF_IS_CDI_INTIGN=false;
        } 
        else
        {
            VF_IS_CDI_INTIGN=true;
        }
		*/
        VF_IS_CDI_INTIGN=false;//2016-9-20 ��ʱ
    
    v_nb_notsyninj_add=0;//2012-10-21 inition
    nb_qy_betweenhy_now=nb_hy_betweenqy_now=0;  //2009-12-11 add //2015-2-27 move out
   
    v_nb_T3_miss=0;
	
	VF_NEED_DWELL=false;
	
	v_ToothPosition=1;
	v_Toothnum=1;
    
    v_tick_1stTooth=v_tick_1stTooth_lst=0;
    
    v_tw_Tooth[1]=v_tw_Tooth[2]=v_tw_Tooth[3]=0;
	sb=BYPTR_M0_TOOTH_TDC;//20210301 chenwei add
	/*v_NO_dwellTooth_trig=BYPTR_M0_TOOTH_TDC/10-3;
	v_NO_ignTooth_trig=BYPTR_M0_TOOTH_TDC/10;*///20210301 chenwei ��ʱ���Σ����ִ�����������ˢ��֮��ü�϶�������쳣����Ϊ��������ok
	v_NO_dwellTooth_trig=sb/10-3;
	v_NO_ignTooth_trig=sb/10;
	VF_DWELL_SET=false;
	//V_CHARGE_OFF
	
		
	VF_IGN_OVER=false;
   
}
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
void API_10MS_OnInterrupt(void)
{
    nb_10ms++;
    
    /*
    if((nb_10ms&0x10)==0){
      MIL_ON;
    }
    else{
      MIL_OFF;
    } */
     
}

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
void RTI_D5MS_OnInterrupt(void)
{
 
  
}

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
void TR_SCI0_OnRxChar(void)
{
  /* Write your code here ... */
//////////////////////////////////////////////////////////////////////////////////
    byte rcdata;
    byte temph_int;
    byte templ_int;
    

        //(void)TR_SCI0_RecvChar(&rcdata);                                      //��ȡ���ջ�����
        rcdata = UART_ReceiveData(UART2);
        /*-----2014-3-31 ADD---------------*/
            //�˿����ݶ��뻺����
            if(v_num_rx_tmp<BYTE_MAX-1)
            {
                //֡����
                if(v_num_rx_tmp==0){
                    if(rcdata==FRAME_B0){
                        cmdRX[v_index_rx_tmp][v_num_rx_tmp]=rcdata;        
                        v_num_rx_tmp++;

                    } 
                } else if(v_num_rx_tmp==1){
                    if(rcdata==FRAME_B1){
                        cmdRX[v_index_rx_tmp][v_num_rx_tmp]=rcdata;        
                        v_num_rx_tmp++;
                    } else v_num_rx_tmp=0;
                     
                } else {
                    cmdRX[v_index_rx_tmp][v_num_rx_tmp]=rcdata;        
                    v_num_rx_tmp++;
                }
            }

            t_rx_lst_4us=T_MAINTMR;
        /*-----2014-3-31 ADD---------------*/

    
        temph_int=(rcdata>>4)&0x0F;           //rcdata�ĸ�4λ
        templ_int=rcdata&0x0F;                //rcdata�ĵ�4λ

        //if(temph_int==templ_int)              //�ж�rcdata�ĸ�4λ���4λֵ�Ƿ����
        //2014-3-31 CHANGE TO THE FOLLOWING
        if(temph_int==templ_int&&(VF_MODE_INTSCI==COMMODE_DIAG||(templ_int==0x07&&v_num_rx_tmp==0)))              //�ж�rcdata�ĸ�4λ���4λֵ�Ƿ����
        { //����Ϊ�Լ�������λ��У��,��Щ����λ�����Ǹ��õ�
          //��ͣ��ʱ�����ܻ����ڶ�EEPROM�Ĳ���
            switch(templ_int) 
            { 
                case 0x0:break;		
                case 0x1:
                   v_PCBTDC_SCI++;
                   cmdAdjIGN++;
                    
                    break;           	
                case 0x9:
                    v_PCBTDC_SCI--;          
                    cmdAdjIGN--;
                 
                    break; 			
                case 0x2:
                    VF_ADJUSTO_WHEN_AQ=true;
                    //2013-2-17
                    if(KESEI>=20&&!VF_MMOVE_ENG)v_PCSET_SCIH++;//��������FLASH/EEPROM�ĵ�ַ	
                    else
                    {
                        if(VF_E0_TPS)
                        {
                            v_FMF_SCI_idle+=2;//v_FMF_SCI_Nidle=v_FMF_SCI_idle; 
                        } 
                        else
                        {
                            v_FMF_SCI_Nidle+=2;
                        }
                    } 
                    break;
                case 0xa:
                    VF_ADJUSTO_WHEN_AQ=true;
                    //2013-2-17
                    if(KESEI>=20&&!VF_MMOVE_ENG)v_PCSET_SCIH--;//��������FLASH/EEPROM�ĵ�ַ	
                    else
                    {
                      
                        if(VF_E0_TPS)
                        {
                            v_FMF_SCI_idle-=2;//v_FMF_SCI_Nidle=v_FMF_SCI_idle; 
                        } 
                        else
                        {
                            v_FMF_SCI_Nidle-=2;
                        }
                    }
                    break;         	
                case 0x3:
                    v_PCTBACK_SCI++; 
                    cmdAdjT1++;
                   // Get_Kesei_for_CAB(0,1);//�궨���ݵ�WORD��	
                     
                    break;		
                case 0xB:
                    v_PCTBACK_SCI--;         
                    //Get_Tback_for_CAB(-1);//2012-10-22        
                    cmdAdjT1--;
                    
                   // Get_Kesei_for_CAB(0,-1);//�궨���ݵ�WORD��	
                    	
                    break;		
                case 0x4:
                  //  if(v_PCKESE_SCI>=0)v_PCSET_SCIL++;//��������FLASH/EEPROM�ĵ�ַ	
                  //  else v_PCSET_SCIH++;
                    if(KESEI==19)v_PCSET_SCIH++;//��������FLASH/EEPROM�ĵ�ַ	
                    else v_PCSET_SCIL++;  
                    break;//��������FLASH/EEPROM�ĵ�ַ	
                case 0xC:
                   // if(v_PCKESE_SCI>=0)v_PCSET_SCIL--;//��������FLASH/EEPROM�ĵ�ַ	
                    //else v_PCSET_SCIH--;
                   if(KESEI==19)v_PCSET_SCIH--;//��������FLASH/EEPROM�ĵ�ַ	
                    else v_PCSET_SCIL--;     
                    break;//��������FLASH/EEPROM�ĵ�ַ	
                case 0x5:
                   PCCQJSJD++;//Get_Kesei_for_CAB(1,0);             //break;//��������FLASH/EEPROM������		
                    cmdAdjCOS++;  
                    break;
                    
                case 0xD:
                   PCCQJSJD--;//Get_Kesei_for_CAB(-1,0);            //break;		
                    cmdAdjCOS--; 
                    break;
                    
                case 0x6:break;//VF_BTDCF_SCI=1; break;	
                case 0x7:
                    //��λ����Ҫ�������
                    VF_LINKPC_INTSCI=1;     //�Ѿ���PC������
                    VF_FORPC_INTSCI=1;//��PC���������
                    VF_PCWANT_INTSCI=1;        //��Ҫ��������
                    break; 		
                case 0x8:
                    //if(!VF_MMOVE_ENG)
                    v_PCKESE_SCI++;                     //���ʴ洢����ģʽλ

                    break;		
                case 0xF:
                    //if(!VF_MMOVE_ENG)
                    v_PCKESE_SCI--;          
                    
                    break;		
                case 0xE: 
                    //����������������Ԥ��
                    /*
                    if(REMARK==1){
                        VF_MILELIMIT_DUE_SYS=false;
                        VF_MILELIMIT_WARN_SYS=false;
                        //v_mile_limit=MILE_FOR_LOCK;
                    } else if(REMARK==2){
                        VF_MILELIMIT_DUE_SYS=false;
                        VF_MILELIMIT_WARN_SYS=false;
                        //v_mile_limit=M0_MILE_NOLIMIT;
                    } */
                    break;//
                   // VF_PCDAT_SCI=1; break;
                default: break;
            }
        }
    
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////  
}

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
void TR_SCI0_OnTxChar(void)
{
}

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
void TR_SCI0_OnFreeTxBuf(void)
{
  /* Write your code here ... */
  ;
}

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
void TR_SCI0_OnTxComplete(void)
{
  /* Write your code here ... */
  zzq=1;
  VF_TRANSING_INTSCI=0;
}


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
void TC_IGNIO_OnInterrupt(void)
{
  /* Write your code here ... */
    byte w;//
    word w1;
	word w2;
    word t_ign;
	word time_w;
    v_nb_IgnInt++;
    
    if(VF_PREDRV_INTIGN)   //�����ֲ������жϣ�����
    {   //call MOV_STSPEED_TO_W;//��õ�������ת�������ֵ������ 0xD1
        //TEST_COME
		
		if(VF_STARTS_ENG)w=0xd1;else{w=0xd1;}//M0_START_SPEED;else{w=M0_IDLE_STOP_SP;}
        if(!VF_BDRV_INTIGN)       //��ʾ��Ҫ��ʼ���
        { 
        	//20210618 chenwei add �����������ɹ�����ַ�����ͣ�����һȦ����ͣ��֮�󽫳�ʱ������һ�γ�ŵ���ҳ��ʱ��ܳ�>100ms����������(VF_CHARGE_OK&&(time_w>7000))���Ƴ��ʱ��
			time_w=T_MAINTMR;
			time_w=v_t_IgnCharge_r-time_w;
			if((((vlst_prd_cas>>8)&0xff)<w) || VF_PREINJ_2CA||(VF_CHARGE_OK&&(time_w>3750)))	//20210929 ��ע������С��������Ϊ15ms����
            { 
                //TEST_COME
				if(!VF_CASHY_BECHECK1_CAS||VF_PREINJ_2CA||(VF_CHARGE_OK&&(time_w>3750)))
				{
					CDI_ON;//2015-3-3 add condiction,���ڽ�ֹ���
					VF_CHARGE_OK=false;
				}
                v_nb_ignOn_r++;
                  
               // MIL_ON;
                v_t_IgnSetOn_r=T_MAINTMR;
                v_t_bIgn_toTrig_r=T_MAINTMR-v_t_trigIgn; 
                v_t_bIgn_toCas_r=T_MAINTMR-v_t_Cas_trigIgn;
                v_dt_bIgn_toCas=v_t_bIgn_toCas_r-v_t_bIgn_toCas;
                
                //v_t_IgnCA_act=T_MAINTMR-v_t_cas0;
                v_t_IgnCA_act=T_MAINTMR-v_t_cas; //2015-3-17
                
                v_t_IgnOn_r=T_MAINTMR-v_t_IgnCharge_r;//2014-4-5
                
                v_dnb_Ign_YSTDC=v_nb_ignOn_r-v_nb_YSTDC;
                
                F_Clr_PostIgnOnTime();//2014-10-8
                
            }   //��ת�ٸ��������ֵʱ�����


            VF_BDRV_INTIGN=true;       //�Ѿ���ʼ���
            VF_BIGN_INTIGN=false;
			
			if(VF_NEED_CHARGE_IGN)
			{
				VF_NEED_DWELL=true;
				v_NO_dwellTooth_trig=v_NO_dwellTooth;
				v_NO_ignTooth_trig=v_NO_ignTooth;
              //2020-12-18
              v_t_TrigTooth_To_Ign_trig=v_t_TrigTooth_To_Ign;     //�����ݵ����ʱ��
              v_t_TrigTooth_To_Dwell_trig=v_t_TrigTooth_To_Dwell;   //�����ݵ����ʱ��
			  VF_IGN_OVER=true;
			}
			
            
        }
        else
        { 
           // TC_INJO_DisableEvent();//�ر�ͨ��4�ıȽ�����ж�
           //******************* here *********************//
            //TIE_C0I=false;
						PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
            VF_BIGN_INTIGN=true;              //������
            VF_PREDRV_INTIGN=false;//2008-7-14 zhangping add
            VF_NEED_DWELL=false;
			VF_DWELL_SET=false;

			F_Trig_IgnEn2=0;
            if((IS_KEY_ON)||VF_KEYON_HARD)    //����Կ��
            { 
                CDI_CHARGE;
				VF_CHARGE_OK=true;
					F_ChargeEn = 0;
					if(F_Trig_IgnEn == 0)
					{
							//w = v_t_TrigTooth_To_Dwell_trig+T_MAINTMR-Trig_IgnTmeptime;//20210420 ��Ϊv_t_TrigTooth_To_Dwell_trig
							w1=T_MAINTMR;
							//w2=v_t_TrigTooth_To_Ign_trig-(w1-Trig_IgnTmeptime)+T_MAINTMR;		//20210930 16.48�����β���
							w2=T_MAINTMR+v_t_IgnOn;		//20210930 ceshi
							if(w2 < 4) w2 = 4;		
							//CCPS_ONOFF_status=!CCPS_ONOFF_status;
							
							F_Trig_Ign(w2);
							//M_MilLed_TOGGLE;//20210419 chenwei add
							F_Trig_IgnEn = 1;

							F_Trig_IgnEn2=1;
							
					}
				//TEST_COME
                v_nb_ignOff_r++;
                v_t_IgnCharge_r=T_MAINTMR;
                v_tl_IgnPreCharge_r=v_t_IgnCharge_r-v_t_IgnSetOn_r;
            }//��ת�ٸ��������ֵʱ�����
			
			//������Ҫ��Ϊ�˱�������û�е������ĳ��ʱ��̫��
                    if(F_Trig_IgnEn2 == 0)
					{
			t_ign=T_MAINTMR+7500;				   
            TC0=t_ign;//2015-10-8
            //TIE_C0I=true;
						PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
					}
            VF_BDRV_INTIGN=false;
                            
            //2016-7-25��������Ҫ������ĳ��ʱ�䣬30ms
            VF_PREDRV_INTIGN=true;//2016-7-25
        }
    }
    
}

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
void TC_INJO_OnInterrupt(void)
{
    word t_temp;
	//TEST_CR

    if(v_t1_xh==0)
    {
//        TPM2SC=0x00;
        //TIE_C1I=false;
				
				//FUN_TIMER11_DisAbleInt();
				PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
       //*******����ط�ԭ�ȹر��˶�ʱ�� 2013-12-13 ���ڴ˴�û��                 //�رն�ʱ��2���ر��ж�ʹ��
        if(VF_PTRIG_INTINJ)                 //���ECUû�д���׼�����͵Ķ������򷵻�
        { 
            if(!VF_PINJB_INTINJ)
            { 
                VF_NTESTPTSBINJ_TRANS=true;
                
                //if(VF_TRIGTPS_TRANS)
                //v_nb_mayadd_notsys v_nb_notsyninj_add
                if(VF_MMOVE_ENG&&((VF_TRIGTPS_TRANS&&!VF_ADDFUEL_SET)||(VF_ADDFUEL_SET&&VF_E0_TPS)))
					//2020-7-3 ���紥���󣬷������Żص����ˣ�ֹͣ����
                {//���ECUҪ�ж����͵Ķ���,�����ͽ�����ʶ���򷵻�
                    VF_INJSTOP_HARD=true;
					VF_ADDFUEL_SET=false;
                } 
                else
                {  // *************************2013-12-11
                    //--------------------------------
                    if(v_NB_INJDRV>0||VF_MMOVE_ENG||VF_PREINJ_2CA)                 //2008-12-16 �ƶ����ˣ���Ϊ���������������ʹ���
                    {
                        //if(!VF_TRIGTPS_TRANS)	
                        F_after_Inj_d3ms();               //׼��0.3ms������
                        //TEST_COME
						 if(v_NB_INJDRV>0&&(switch_1==0))v_NB_INJDRV--;//20210427 chenwei add "&&(switch_1==0)"
						//if(VF_PREINJ_2CA)VF_PREINJ_2CA=false;
                        //if(v_nb_notsyninj_add>0&&VF_TRIGTPS_TRANS)v_nb_notsyninj_add--; //2012-10-21add

                        if(VF_MMOVE_ENG)VF_T35TEST_HARD=false;//2009-4-18
						
						#ifndef SYS_TRADITIONAL
                        if(VF_T35TESTNEED_NOTYSTDC){//��ʱ�Ƿ���ҪT35��� 
                            VF_T35TEST_HARD=true;
                            VF_T35TESTNEED_NOTYSTDC=false;// 2008-6-29 add
                        }
                        #endif  
                    } 
                    else
                    {
                        VF_PTRIG_INTINJ=false;          //2008-12-16 
                        INJ_OFF;
                        VF_INJSTOP_HARD=true;   
                    }
                }
            }
            else if(!VF_PINJE_INTINJ)
            { 
                VF_PINJE_INTINJ=true;
                VF_NTESTPTSBINJ_TRANS=false;      //2008-8-18 ���ӣ�
                
                if(VF_MMOVE_ENG)               //2009-8-24 DELETE
                { 
                    //����������Ƿ�·�����õ�code_number,binjector������
                    if(!IS_INJDRV_HIGH)    //2009-12-18
                    {
                        VF_OPEN_INJCTL=true;
                    }
                } //2009-12-11 DELETE
                INJ_ON;  
                //MIL_ON;           //����������
                FP_End_Inj_WhenTMROverFlow();  //������������

                v_nb_injOn_r++;
                v_t_injOn_r=T_MAINTMR;
                v_t_bInj_toTrig_r=v_t_injOn_r-v_t_trigT1;
                v_t_bInj_toCas_r=v_t_injOn_r-v_t_Cas_trigT1;
                v_dt_bInj_toCas=v_t_bInj_toCas_r-v_t_bInj_toCas;
                
                v_nb_binj++;

                v_dnb_Inj=v_nb_injOn_r-v_nb_binj;
                
                
                if(VF_T35TEST_HARD)nb_injection_fort35++;//
                //v_TW4=0;            //2009-11-29 add  //2009-11-29�����ڴ���t4����0�ᵼ��t4��ʹ��ʱ����Ϊ0
            }
            else 
            { 
                //v_vbat_atInjOff=GetVbat_Tmp(); //2015-3-11 ��ʱɾ������Ϊ�����ͨ��AD���Ӱ��
               
               // v_TW4=0;//2010-1-14
                INJ_OFF;                    //�ر�������
				if(VF_PREINJ_2CA)VF_PREINJ_2CA=false;
				VF_INJ_SET=false;
				VF_ADDFUEL_SET=false;
                //MIL_OFF;
                //TIE_C1I=false;
                //FUN_TIMER11_DisAbleInt();
								PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
							
                F_Clr_PostInjOnTime();
                
                // *************************2013-12-11 �˴�û������жϣ�ֻ�п�����ر�����Ƚ��жϣ��б�Ҫ
                //---------------------------------------------------
                VF_INJSTOP_HARD=true;      //��λ���ͽ�����ʶ                
                v_t_t3ed=T_MAINTMR;           //����ʱ��1�ļ���ֵ����v_t_t3ed�У���ΪTV3���½�ʱ��
                                            //С��ԭ��������д����	
                
                //(void)Vsamp_Enable();
                  //2015-10-8
                  /* TFLG1: C7F=1,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=0,C0F=0 */
                    //TFLG1 = 0x80U;//2017-12-28,ysl,delete                    /* Reset interrupt request flag */
                    //TIE_C7I = 1U;//2017-12-28,ysl,delete                      /* Enable interrupt */
                  /* TCTL3: EDG7A=1 */
                    //TCTL3 |= 0x40U;//2017-12-28,ysl,delete                    /* Enable capture function */ //Capture on rising edges only


                v_t_T1_r=T_MAINTMR-v_t_injOn_r;
                v_dt_T1=v_t_T1_r-v_TW1_forTrig;

                v_t_injOff_r=T_MAINTMR;
                
                nb_tw4_in=0;

                v_nb_injOff_r++;
                
               // v_dnb_T1=v_nb_injOff_r-v_nb_injOn_r;   //2019-2-27 delete���ռ䲻����
               // v_dnb_Inj_YSTDC=v_nb_injOff_r-v_nb_YSTDC;
                
                if(v_nb_T3_miss<255)v_nb_T3_miss++;//2015-7-4
                
                //-----2008-7-27 add---------------------------------
                if(!VF_MMOVE_ENG)
                { 
                    if(VF_PREINJEND_SYSINI&&!VF_PREPUMPEND_SYSINI) //��Ҫ��T3��Ԥ��������������
                    {  //T35��T3��Ԥ���������������Ҫ��ƽ���
                       // tinjh,tinjl+= ti_shift_set+tv3_temp+tv3_idlel-v_tw3_h,v_tw3_l;//???
                       // if(tinjh>8) 
                       // { tinjh=8;}    TOBECHECK
                    }
                    FP_Inj_WhenTMROver(Interval_For_T1_Scaning(),v_TW1_forTrig);
                }
                else if(v_nb_mayadd_notsys>0 &&v_nb_notsyninj_add>0)
                // else if(v_nb_notsyninj_add>0)   //2013-1-7
                {
                    F_after_Inj_xms(1250);
                    //FP_Inj_WhenTMROver(1250,v_TW1_forTrig);
                    //2012-10-20 ��ʱɾ��
                    v_nb_mayadd_notsys--;
                    if(v_nb_notsyninj_add>0)v_nb_notsyninj_add--; //2012-10-21add
                  
                } 
                // 
            }
        }
  } 
  else {
      v_t1_xh--;
      //t_temp=T_MAINTMR;
      //t_temp=t_temp-1;
      //TC1=t_temp;
			//FUN_TIMER3_DisAbleInt();
			//FUN_TIMER3_TrigeInt(t_binj);
			//TIMER_ClrIntFlag(TIMER3); 		AC7811�Ķ�ʱ���������
      //TPM2MOD=0xffff;  //2008-3-7 ��ƽ��ӣ���Ҫ��С�Ӷ�ʱ��2���׹�����ʲôģʽ��
			
  }
 
  
}  
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

void TDC_HY_OnCapture(void)
{
	
}

void CA_capture_OnCapture(void) 
{
    unsigned char i;
	
	word w;

    word w_tmp;
	word w_tmp2;
    unsigned long int ul;
	
//	M_BumpO2_C_TOGGLE
    
#if (M_UPorDown == 0)
    if(!PORTE_CAI2)return;//33814ǰ����Ӧ����������
#endif
#if (M_UPorDown == 1)
    if(PORTE_CAI2)return;//33814ǰ����Ӧ����������
#endif
    for(i=0;i<10;i++)
    {
        __NOP;
    }
#if (M_UPorDown == 0)
    if(!PORTE_CAI2)return;//33814ǰ����Ӧ����������
#endif
#if (M_UPorDown == 1)
    if(PORTE_CAI2)return;//33814ǰ����Ӧ����������
#endif

		
    //v_timeout_10ms_noCA=0;//�Ǳ굽���󣬸�ֵ����
    //if(v_nb_tooth_cap<255)

    v_t_Tooth_lst=v_t_Tooth_Now;//2020-9-24 ��ĩβŲ������ط�����������return�����޷���ֵ
	
	v_t_Tooth_Now=(word)TC3;
	
	if((word)(v_t_Tooth_Now-v_t_Tooth_lst)<(word)40)return;
	
	v_t_CA_ncome=0;

    v_ToothPosition++;
	v_ToothPosition2++;
	
	if(!VF_CATCH_CAS&&(v_Toothnum<255))v_Toothnum++;
	
	
    if(v_ToothPosition>UB_NO_TOOTH)v_ToothPosition=1;
   
    //����֮ǰ�ĳ�ʱ��
    for(i=NB_TOOTH-1;i>=1;i--)
    {
        v_tw_Tooth[i]=v_tw_Tooth[i-1];    //���ݵ������������ݵ�ʱ��
    }

    //���㵱ǰ�ĳ�ʱ��
    v_tw_Tooth[0]=(word)(v_t_Tooth_Now-v_t_Tooth_lst);
	
	


    //�ж�ȱ��
        w_tmp=v_tw_Tooth[2]/4;
		#ifdef SY_24
			w_tmp2=w_tmp*2;
		#endif
#ifdef SY_24       
    if( 
            //v_tw_Tooth[0]>v_tw_Tooth[1]+v_tw_Tooth[2]
            (word)v_tw_Tooth[1]>(word)(v_tw_Tooth[0]+w_tmp2)
		&& ((word)v_tw_Tooth[1]>(word)(v_tw_Tooth[2]+w_tmp2))
     && ((word)v_tw_Tooth[2]>(word)(v_tw_Tooth[3]-w_tmp))
        && ((word)v_tw_Tooth[2]<(word)(v_tw_Tooth[3]+w_tmp))   
        && ((word)v_Toothnum>(word)9)
		//&& (!(VF_CATCH_CAS&&(v_ToothPosition<=10)&&(v_ToothPosition>2)))	 //2021-3-2 ��������ᵼ�������һ��ȱ���жϴ��󣬾���Ҳ����ȥ�����	
       && ((!(VF_CATCH_CAS&&(v_ToothPosition<=10)&&(v_ToothPosition>2)))||(v_ToothPosition2>UB_NO_TOOTH+3))
    )//24-1
#endif

#ifdef SY_36
	if( 
        (word)v_tw_Tooth[0]>(word)(v_tw_Tooth[1]+v_tw_Tooth[2])
        && ((word)v_tw_Tooth[1]>(word)(v_tw_Tooth[2]-w_tmp))
        && ((word)v_tw_Tooth[1]<(word)(v_tw_Tooth[2]+w_tmp))
		&& ((word)v_Toothnum>(word)9)
		&& (!(VF_CATCH_CAS&&(v_ToothPosition<=10)&&(v_ToothPosition>2)))
    )//36-2
#endif

#ifdef SY_12
	if( 
        (word)v_tw_Tooth[0]>(word)(v_tw_Tooth[1]+v_tw_Tooth[2]+v_tw_Tooth[3])
        && ((word)v_tw_Tooth[1]>(word)(v_tw_Tooth[2]-w_tmp))
        && ((word)v_tw_Tooth[1]<(word)(v_tw_Tooth[2]+w_tmp))
		&& ((word)v_Toothnum>(word)5)
		&& (!(VF_CATCH_CAS&&(v_ToothPosition<=6)&&(v_ToothPosition>2)))
    )//12-3
#endif	
    {
//    M_AddAirV_C_TOGGLE    
	#ifdef SY_24
		if(v_ToothPosition!=2)//24-1
	#else
		if(v_ToothPosition!=1)//36-2 12-3
	#endif	
		{//���ͬ��������ݲ���1�ų�
            if(v_nb_tdcerr<255)v_nb_tdcerr++;//Acc_nb_Act(NA_TDCER);
        }
              
        //�õ�ȱ��λ�ã�
		
		#ifdef SY_24
			v_ToothPosition=2;//24-1���ڵڶ�������Ϊͬ���ж�
			v_ToothPosition2=2;
		#else
			v_ToothPosition=1;//36-2���ڵ�һ������Ϊͬ���ж�
			v_ToothPosition2=1;
		#endif

        //v_nb_tooth_cap++;
              
        v_tick_1stTooth_lst=v_tick_1stTooth;              
        v_tick_1stTooth=v_t_Tooth_Now;
		
		v_t_cas0=v_t_Tooth_Now;
		
        //���������е��ж�
        //  �õ�һ��ȱ�ݣ��ʹ�������������              
        if(VF_CATCH_CAS){
        //����Ѿ���ù�һ��CA��
			#ifdef SY_24
				ul=NB_TOOTH*(unsigned int)v_tw_Tooth[2];
			#else
				ul=NB_TOOTH*(unsigned int)v_tw_Tooth[1];
			#endif
                    
			//���Լ���ת����
			v_prd_cas=(word)(v_tick_1stTooth-v_tick_1stTooth_lst);
			v_CA_reset=false;//�����־λ����ԭ���Ǳ�û����ȷ���ɼ�ʱ���Ǳ����ڲ��ᱻ����
			
			if(ul>0xafffu){
				if(v_prd_cas<0x7fffu) {
					//˵��v_prd_cas�����
					v_prd_cas=0xffffu;
				}
			}
			if(v_nb_cycle<10)v_nb_cycle++;		//20210930 chenwei add
                    
			/*if(v_prd_cas<0xffffu) 
			{
				VF_MMOVE_ENG=true;

			}*/
        } else{
            //��һ�λ��CA

			if(VF_PCAOV_CAS) v_prd_cas=0xEFFF;
			
			VF_PREINJ_2CA=true;
			v_nb_cycle=2;		//20210930 chenwei add

        }

        /*if(VF_NEED_CHARGE_IGN &&VF_MMOVE_ENG) //2016-7-25 ADD VF_BFIRE_ENG
        {
            VF_BIGN_INTIGN=true;//2015-12-25
            CDI_CHARGE;
                         
            TC0=T_MAINTMR+7500;				   
            TIE_C0I=true;
            VF_BDRV_INTIGN=false;
                            
            VF_PREDRV_INTIGN=true;//2016-7-25
            if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־ 
            //2016-8-4 ���ֿ�����Ҫ��������ϣ����򣬳�������жϺ����Ͻ���һ���жϣ�����ʼ��������ͻ���ҵ��ʱ��

        }*/

        FCLEAR_CAOV
                
        VF_PCAOV_CAS=false;
                
                
        VF_OLDCA_CAS=false;
		VF_OLDCAHY_CAS=true;
        VF_CATCH_CAS=true;//2008-10-21 �ָ�
				
		VF_YSTDC_INJPHASE=!VF_YSTDC_INJPHASE; 
		//TEST_CR
		
		VF_NEED_CHARGE_IGN=false; 
        if((
            !VF_YSTDCGET_INJPHASE                          
            ||(VF_YSTDCGET_INJPHASE&&VF_YSTDC_INJPHASE)//VF_YSTDC_INJPHASEΪtrue�������´�ȱ��Ϊ��ֹ��ǰ��ȱ�ݣ���Ҫ���
            ||(REMARK<MC_OPENLOOP)
            ||(VF_NOTDELAY_IGN&&v_TPS_odr>4) 
			)
            &&(
                !VF_IGNONLY_CFCI
                 ||
                 (VF_GT10_IDLETG_REV&&(v_revmax_625_cf<112||(v_ccl_cf<30)))//80 for 5000rpm    112��for 7000
            )
        )
        {
            VF_NEED_CHARGE_IGN=true;
        }
		
		
		/*if(VF_NEED_DWELL&&VF_MMOVE_ENG&&!VF_BFIRE_ENG)
		{

			VF_BIGN_INTIGN=true;
			CDI_CHARGE
			VF_NEED_DWELL=false;
			VF_DWELL_SET=false;
			
			TC0=T_MAINTMR+7500;				   
            TIE_C0I=true;
            VF_BDRV_INTIGN=false;
                            
            VF_PREDRV_INTIGN=true;
            if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־ 
			
		}*/
                
        //Acc_nb_Act(NA_CCL);
		//V_CHARGE_ON

    }
	
//----------------------�趨����--------------------------	
	#ifdef SY_12	
		if(v_ToothPosition==3)
		{
			v_t_cashy0=v_t_Tooth_Now;//����ʱ�䣬�����ж���λ
			VF_OLDCAHY_CAS=false;
	
		}
	#else	
		if(v_ToothPosition==6)
		{
			v_t_cashy0=v_t_Tooth_Now;//����ʱ�䣬�����ж���λ
			VF_OLDCAHY_CAS=false;
	
		}
	#endif
	
	#ifdef SY_36
	if(v_ToothPosition==12)
	{
		TachOff_Flag=true;	
	}
	#endif
	
//----------------------���ʹ���--------------------------	

	//----------����ʱ��ڶ�Ȧ����һ��ȱ�ݺ󣩵����ʹ���
	#ifdef SY_12
	if(VF_PREINJ_2CA&&(v_ToothPosition==5))
	#else	
	if(VF_PREINJ_2CA&&(v_ToothPosition==9))
	#endif
	{
			
		//test_20061701=v_tw1_inj;
		VF_TURNONINJ_ENG=true;
		VF_TRIGTPS_TRANS=false;
		VF_FORINJ_INJPHASE=true;
		
		//if(v_tw1_inj>800)w=v_tw1_inj;
		//else w=800;
		

		FP_Inj_WhenTMROver(100,v_preinj_ca);//�ӳ�100us
	}
	
	
	//�뿪����ʱ�ļ���
#ifdef SY_24
	if(VF_YSTDC_INJPHASE&&VF_NEEDADDFUEL_CR&&!VF_INJ_SET&&(v_ToothPosition<16)&&(v_ToothPosition>7))//24-1
#endif
#ifdef SY_36
	if(VF_YSTDC_INJPHASE&&VF_NEEDADDFUEL_CR&&!VF_INJ_SET&&(v_ToothPosition<25)&&(v_ToothPosition>10))//36-2
#endif
#ifdef SY_12
	if(VF_YSTDC_INJPHASE&&VF_NEEDADDFUEL_CR&&!VF_INJ_SET&&(v_ToothPosition<9)&&(v_ToothPosition>3))//12-3
#endif		
	{
		w=v_tw1_inj;
		
	#ifdef SY_24
		if((v_ToothPosition<12)&&(v_te<100))
	#endif
	#ifdef SY_36
		if((v_ToothPosition<18)&&(v_te<100))
	#endif
	#ifdef SY_12
		if((v_ToothPosition<6)&&(v_te<100))
	#endif
		{
			if(w>1500)w=1500;//6ms 8ms
		}
		else
		{
			if(w>1250)w=1250;//�����������5ms
		}

		FP_Inj_WhenTMROver(100,w);
		VF_INJ_SET=true;
		VF_NEEDADDFUEL_CR=false;
		VF_ADDFUEL_SET=true;
		
		//TEST_COME
	}



	//---------����һȦ�Ժ�����ʹ���-------------
	//���ŵ�����-�������һȦ�����һ���ݴ���	
#ifdef SY_24
	if(VF_YSTDC_INJPHASE&&(v_ToothPosition==22)&&!VF_IGNONLY_CFCI)//24-1
#endif
#ifdef SY_36
	if(VF_YSTDC_INJPHASE&&(v_ToothPosition==30)&&!VF_IGNONLY_CFCI)//36-2
#endif
#ifdef SY_12
	if(VF_YSTDC_INJPHASE&&(v_ToothPosition==8)&&!VF_IGNONLY_CFCI)//12-3
#endif	
	{
		v_t_bInj_toCas=v_t_bInj_toCas_tmp;
        //v_t_T1=v_tw1_inj;
        v_t_Cas_trigT1=v_t_cas0;
        v_t_trigT1=T_MAINTMR;

		//FP_Inj_WhenTMROver(4,v_tw1);//+Trig Injection//  //��������
		//FP_Inj_WhenTMROver(4,v_tw1_injphase);
		
		
		VF_TURNONINJ_ENG=true;
		VF_TRIGTPS_TRANS=false;
		VF_FORINJ_INJPHASE=true;
		
		FP_Inj_WhenTMROver(v_ts_binj,v_tw1_inj);
		
		VF_INJ_SET=true;//2020-7-1 �Ѵ�������
		//TEST_COME

	}

//----------------------��𴥷�--------------------------


	//-------������ʱ��ڶ�Ȧ����һ��ȱ�ݺ󣩵ĵ�𴥷�-------
#ifdef SY_24
	#ifdef	SY_HUANSONG		//20210930 chenwei add ����
	if(VF_PREINJ_2CA&&(v_ToothPosition==8)&&(v_tw_Tooth[2]<2080))//ת�ٵ���300rpm�����300rpm��24�ݣ�һ���ݼ��8ms����//24-1
	#else
	if(VF_PREINJ_2CA&&(v_ToothPosition==7)&&(v_tw_Tooth[2]<2080))//ת�ٵ���300rpm�����300rpm��24�ݣ�һ���ݼ��8ms����//24-1
	#endif
#endif
#ifdef SY_36
	if(VF_PREINJ_2CA&&(v_ToothPosition==6)&&(v_tw_Tooth[2]<1400))//ת�ٵ���300rpm�����300rpm��36�ݣ�һ���ݼ��5.5ms����//36-2
#endif
#ifdef SY_12
	if(VF_PREINJ_2CA&&(v_ToothPosition==4)&&(v_tw_Tooth[2]<4160))//ת�ٵ���300rpm�����300rpm��12�ݣ�һ���ݼ��16ms����//12-3
#endif	
	{

		#ifdef SY_12
		w=v_t_Tooth_Now+600;
		#else
		//w=v_t_Tooth_Now+300;
		w=v_t_Tooth_Now+100;	//20211008 chenwei 300 to 100
		#endif	
		F_Trig_DWELL(w);
		//TEST_COME
	}
#ifdef SY_12
	if(VF_PREINJ_2CA&&(v_ToothPosition==5))
#else	
	if(VF_PREINJ_2CA&&(v_ToothPosition==9))
#endif
	{

		//w=v_t_Tooth_Now+300;
		w=v_t_Tooth_Now+100;		//20211008 chenwei 300 to 100
        F_Trig_Ign(w);
		//TEST_COME		
	}
	
#ifdef SY_12
	if(VF_NEED_DWELL&&VF_MMOVE_ENG&&!VF_BFIRE_ENG&&(v_ToothPosition==4))
#else	
	//û�Ż�ʱ���ڵ�6�ݳ��
	if(VF_NEED_DWELL&&VF_MMOVE_ENG&&!VF_BFIRE_ENG&&(v_ToothPosition==6))
#endif
	{
		/*
		VF_BIGN_INTIGN=true;
		CDI_CHARGE
		VF_NEED_DWELL=false;
		VF_DWELL_SET=false;
			
		TC0=T_MAINTMR+7500;				   
        //TIE_C0I=true;
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
        VF_BDRV_INTIGN=false;
                            
        VF_PREDRV_INTIGN=true;
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־ 
		PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		*/		//20210929 chenwei ���Σ���ŸĶ̴˴����ܻ���ɳ�Ź���
	}
	
	
	
	//------------����һȦ�Ժ�ĵ�𴥷�---------------	
	if(VF_MMOVE_ENG) //2020-12-22 ɾ��&&!VF_IGNONLY_CFCI���������ʱ���г�ʱ�������  2015-3-12 ADD 
    {          
            //�������
            
      
            
           // if((v_ToothPosition==v_NO_dwellTooth_trig)&&VF_NEED_DWELL)
           
           //2020-12-18 ����ڷ���������ת��С�ڵ���500rpmʱ�������
    
           		
            if((v_ToothPosition==v_NO_dwellTooth_trig)&&VF_NEED_DWELL&&(VF_STARTS_ENG||(v_rev_625rpm>8)))
            {
                VF_DWELL_SET=true;              		        
               // w=v_t_TrigTooth_To_Dwell+v_t_Tooth_Now;                        
                w=v_t_TrigTooth_To_Dwell_trig+v_t_Tooth_Now;//2020-12-18                        
	
          			F_Trig_DWELL(w);
					F_ChargeEn = 1;
            }           

            //�������
            if(v_ToothPosition==v_NO_ignTooth_trig)
            {
                //w=v_t_TrigTooth_To_Ign+v_t_Tooth_Now;
                w=v_t_TrigTooth_To_Ign_trig+v_t_Tooth_Now; //2020-12-18

			if(VF_DWELL_SET)//�������˳�ţ����ǻ�û�䣬��Ҫ����ˣ��Ǿ��ڵ�𴥷�ʱ���ϳ��
				{/*
					VF_BIGN_INTIGN=true;
					CDI_CHARGE
					VF_NEED_DWELL=false;
					VF_DWELL_SET=false;
					VF_CHARGE_OK=true;
					
					v_t_IgnCharge_r=T_MAINTMR;*/	//20210927 16.52 �˴����ƺ����ǻ���룬�������ԣ����ִ����ȷ
			
				/*TC0=T_MAINTMR+7500;				   
				TIE_C0I=true;
				VF_BDRV_INTIGN=false;
                            
				VF_PREDRV_INTIGN=true;
				if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־*/
          				
          				//�ؼ���2��2020-12-15 ��ƽ��ע������Ӧ��Ҫ�������жϱ�־������F_Trig_Ign���Ѿ�������仰�����Բ���Ҫ�ˡ�
          				//��ֹ������CA�жϽ���ʱ���ʱ��IGNҲ�����ж�
          				//DWELL��Ϊ�Ƚ�ģʽ��CA�жϴ����ڼ䣬IGN�ж�ͬʱ���֣����ʱ��˿ڻ��Զ����л�ΪCDI_CHARGE��ƽ��Ҳ��Ӱ��
			}
				if(F_ChargeEn == 0)
				{
         		 	F_Trig_IgnEn = 1;
					
            F_Trig_Ign(w); 
			
				}
			
				else
				{
						
						F_Trig_IgnEn = 0;
						Trig_IgnTmeptime = T_MAINTMR;
				}
        }
    }


/*	�Ǳ�Ĵ���Ų������������ȥ	  
//----------------------�Ǳ�����--------------------------    
    

	//33814�ĽǱ괦��
	if(!VF_STARTS_ENG&&VF_MMOVE_ENG)
    {
    	if(!VF_33814CLRINITVRS_TST)
    	{
    		mc33814_VRSCrankingClear();
    		VF_33814CLRINITVRS_TST = true;
    	}
		
		if((v_rev_625rpm>REV62D5_1250))
		{
			mc33814_WriteVRSRuningParam(0xF3U);
		}
		else
		{
			mc33814_WriteVRSRuningParam();
		}
		
    } 
    else 
    {
    //CA_TH_L;
    }
*/	
//���ݳ�����ʱ��
   // v_t_Tooth_lst=v_t_Tooth_Now;
  


}



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
void SCL_AD_OnError(void)
{
  /* Write your code here ... */
}

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
void SCL_AD_OnRxChar(void)
{
  /* Write your code here ... */
}

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
void SCL_AD_OnTxChar(void)
{
  /* Write your code here ... */
}

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
void SCL_AD_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

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
void SCL_AD_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

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
void T4_OnCapture(void)
{
  /* Write your code here ... */
  #if 0
    word te;
    byte i;
    if(P_TV4_VALID)   //��ȡ�˿�,���˿ڵ�ƽ����ȷʱ������
    { 
        //T4_GetCaptureValue(&te);
        //2015-10-6
        te=TC6;
        
        for(i=0;i<=100;i++)
        {
        } //2008-5-7 REMOVE AND ADD VBAT AD
        if(P_TV4_VALID)
        {
            nb_tw4_in++;
            //2009-7-2 add 
            if(nb_tw4_in==1)//
            {//                                                `
                t_1st_t4rising=T_MAINTMR;
            }
            
            if(nb_tw4_in==2)
            {   /*
                te=T_MAINTMR;
                te=te-t_1st_t4rising;
                if(te>250)   //2009-7-2 add condiction
                {
                    v_TW4=TPM1C3V-v_t_t3ed;         //
                    v_TW4=v_TW4+v_tw1_synwithTW3;   //2008-12-2
                    TPM1C3SC;
                    TPM1C3SC_CH3F=0;      //����жϱ�־
                    TPM1C3SC_CH3IE=0;     //�ر��ж�ʹ��
                    VF_T4GET_INTINJ=true;
                } 
                else
                {
                    nb_tw4_in--;//2009-7-2 add     
                } */
                //2009-8-3 change back
                v_TW4=TC6-v_t_t3ed;         //
                v_TW4=v_TW4+v_tw1_synwithTW3;   //2008-12-2
                v_TW4_2nd=0;
                VF_T4GET_INTINJ=true;

            }
            else if(nb_tw4_in==3)
            {
                v_TW4_2nd=TC6-v_t_t3ed;         //
                v_TW4_2nd=v_TW4_2nd+v_tw1_synwithTW3;   //2008-12-2
                //----------------------------------------------------------------------------------------
                 //(void)T4_Disable();
                 //2015-10-8
                     TIE_C6I = 0U;                      /* Disable interrupt */
                    /* TCTL3: EDG6B=0,EDG6A=0 */
                      TCTL3 &= (uint8_t)~(uint8_t)0x30U; /* Disable capture function */
                //----------------------------------------------------------------------------------------                        
            }
        }
    }
		#endif
}

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
void Vsamp_OnCapture(void)
{
  /* Write your code here ... */
	#if 0
    word te;
    byte i;
    if(P_TV3_VALID)   //��ȡ�˿�,���˿ڵ�ƽ����ȷʱ������
    { 
        //Vsamp_GetCaptureValue(&te);
        //2015-10-8
        te=TC7;
        
        for(i=0;i<=100;i++)
        {
        } //2008-5-7 REMOVE AND ADD VBAT AD
        //F_SelCh_ADC(CH_AD_VBAT,0); //2008-5-7 add���ڲɼ�T3����ʱ�̵ĵ�ѹ��
        if(P_TV3_VALID)
        {
            //v_TW3_in=TC4-v_t_t3ed;
            v_TW3_in=TC7-v_t_t3ed;
            if(v_TW3_in>10){ //2013-5-10 add condiction
              
             //(void)Vsamp_Disable();
              //2015-10-8 change to the following
                  TIE_C7I = 0U;                      /* Disable interrupt */
                  /* TCTL3: EDG7B=0,EDG7A=0 */
                    TCTL3 &= (uint8_t)~(uint8_t)0xC0U; /* Disable capture function */

                //---------------------------------------------------------------------------------------               
               
                VF_T3GET_INTINJ=true;	//��λT3�Ѿ�����ı�־
                v_tw1_synwithTW3=v_TW1_forTrig;     //2008-6-25
                if(!VF_T35TEST_HARD)VF_AT3GETAFTERSTART_INTINJ=true;//2008-7-26 add 
                VF_T3BECHECK_INTINJ=false;
                
                v_nb_T3_miss=0;//2015-7-4
                //2014-4-7 ADD CONDICTION
                if(!VF_MMOVE_ENG||(VF_MMOVE_ENG&&!VF_FORINJ_INJPHASE)) //2008-12-1 �Ȼ��T3 ���ٲ���T4��//2010-3-6DELETE
                {
                    v_TW4=0;
                  //---------------------------------------------------------------------------------------                     
                   //(void)T4_Enable();
                   //2015-10-8
                       TFLG1 = 0x40U;                     /* Reset interrupt request flag */
                        TIE_C6I = 1U;                      /* Enable interrupt */
                      /* TCTL3: EDG6A=1 */
                        TCTL3 |= 0x10U;                    /* Enable capture function */

                   
                   // T4_EnableEvent();
                    VF_T4GET_INTINJ=false;//2010-3-20 add Ϊ���ڶ�ʧT4����£�������һ��T1���źţ�
                    //nb_tw4_in=0;    //2010-1-12
                } /*
                else
                {
                    TPM1C3SC;
                    TPM1C3SC_CH3IE=0;
                }   */
            }
        }
    }  
		#endif
}

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
void CRTS2_OnCapture(void)
{
  /* Write your code here ... */
    word w,w1;   
    
    /*
    w=FAD_1Channel(CH_AD_CURRENT2,false,1);//F_SampImmediate_ADC(CH_AD_CURRENT2);
    w1=FAD_1Channel(CH_AD_CURRENT2,false,1);
    */
    // Save_I_Load_34(w,w1);
    VF_CAP1_LDSENSOR=1;        
    nb_CapI++;
}

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
void CRTS1_OnCapture(void)
{
  /* Write your code here ... */
    word w,w1;   
    /*
    w=FAD_1Channel(CH_AD_CURRENT1,false,1);
    w1=FAD_1Channel(CH_AD_CURRENT1,false,1);//�ɼ����εĵ�ѹֵ��������ƽ�����Ĵ���
    */
    //Save_I_Load_12(w,w1);     //�������εĵ���ֵ(����ط���������)
    //VF_TRIG_SAMPI_CAS=false;
    VF_CAP0_LDSENSOR=1 ;           //��ʾ�ɼ����ˣ�
    nb_CapI++;
}


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
void CA_capture_OnOverflow(void)
{
  /* ���ĳ������������ʱ������� */
  F_INTTMR1_32m();
  
}


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
void Cpu_OnPllLockStatusChanged(void)
{
    
   // nb_PLL_er=Load_Data_EE(EE_PLL_ER);nb_PLL_er++;Save_Data_EE(EE_PLL_ER,nb_PLL_er); //2014-7-15

  /* Write your code here ... */
}

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
void SCI_GPS_OnError(void)
{
  /* Write your code here ... */
}

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
void SCI_GPS_OnRxChar(void)
{
  /* Write your code here ... */
}

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
void SCI_GPS_OnTxChar(void)
{
  /* Write your code here ... */
}

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
void SCI_GPS_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

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
void SCI_GPS_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.05 [04.46]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/

void CAN_OnFreeTxBuffer(word BufferMask)
{
  /* Write your code here ... */
}

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
void CAN_OnFullRxBuffer(void)
{
  /* Write your code here ... */
 
  dword MessageID;
  byte FrameType, FrameFormat, Length, Data[8];
  byte i;  //���Ʊ���
  byte j;
    byte b1,b2,b3,b4;
  if(ERR_OK == CAN_ReadFrame( &MessageID, &FrameType, &FrameFormat, &Length, &Data[0]) ) //��������
  {        
            //2015-5-29 ���յ������ⲿ������ 
            if(MessageID==FRAME_CMD_TEST) 
            {
                for(i=0;i<8;i++)_STRU_CMDIN.Byte[i]=Data[i];

                //2015-7-13 �ָ�
                _STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME=true;
                _STRU_CMDIN.CMD_IN.TimeOut_200ms=0;
                /* 
                2015-6-12 ר������λ����DCU������
                _STRU_CMDIN.CMD_IN.ID_L=(byte)(MessageID&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_H=(byte)((MessageID>>8)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_HL=(byte)((MessageID>>16)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_HH=(byte)((MessageID>>24)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME=true;
                */    
            } 
           // else if(MessageID==ID_15765_ASK)
            else if(MessageID==ID_15765_ASK_ST_ELM||MessageID==ID_15765_ASK_ST) //2016-2-27
            {   //����ǰ���15765Э�鷢��������
                _FRAME_15765.COMMON.IS_NEW=true;
                //_FRAME_15765.COMMON.IS_SENDING=false;//2016-2-23 delete ����SCRͬ��
                
                for(i=0;i<8;i++)_FRAME_15765.Byte[i]=Data[i];
    
            }
							//20210422 chenwei add��ά����������
			/***��ͨ����Ƶ�������͵�������***/
			else if(MessageID==0x18DE0004)
			{				
				switch_1=Data[0];
				b1=Data[1];
				b2=Data[2];		//���� ms
				frequency_1=b1*250;
				flow_1time_ul=b2*250;

				b1=Data[3];
				b2=Data[4];
				//b1=1000/b1;		//  ms/��
				Period_d5ms=Data[3]*250;
				Dwell_t_d5ms=Data[4]*250;
			}
      else if(MessageID == RW33814ID) //2018-4-9,ysl,add,CAN�����ж�����33814�Ĵ����ⲿ���߶�д��־��λ�ͻ�������
			{
					App_RW33814RxData_int(&Data[0]);
					VF_33814RW_GEN = true;
			}
  }

}

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
void CAN_OnBusOff(void)
{
  /* Write your code here ... */
}

//20210606 chenwei add ��ʤ�����ã�ͨ������Ƶ�ʺ�����ʵ�ֵ��������ڸ�ѹ���;�

word nb_1ms;
word t_1ms;
word nb_2ms;
word t_2ms;
word t_5ms;
word nb_5ms;
word nb_6ms;
word SM_Flag;
extern sword v_StepAct;
void IGN_CONTROL_BYCAN(void)
{
 	//nb_d5ms++;
	//nb_d5ms_INJ++;
nb_5ms=T_MAINTMR-t_5ms;
		if((nb_5ms>=1250))
		{
			FC_SM(false);
			nb_5ms=0;
			t_5ms=T_MAINTMR;
		}
		if((v_StepAct==0))
		{
			SM_Flag++;
			//SM_Reverse=0;
			if(SM_Flag%2)
			{
				v_StepAct=150;
			}
			else
			{
				v_StepAct=-150;
			}
		}
 	if(switch_1==1)
	{
		//���
		P_PUMPC_ON
		/*nb_5ms=T_MAINTMR-t_5ms;
		if((nb_5ms>=1250))
		{
			FC_SM(false);
			nb_5ms=0;
			t_5ms=T_MAINTMR;
		}
		if((v_StepAct==0))
		{
			SM_Flag++;
			//SM_Reverse=0;
			if(SM_Flag%2)
			{
				v_StepAct=150;
			}
			else
			{
				v_StepAct=-150;
			}
		}*/
		//mc33814_ReadStatus_def(_0800_IGN1_FAULT);
		nb_2ms=T_MAINTMR-t_2ms;
		if((!v_IGN1_control)&&(nb_2ms>=(Period_d5ms-Dwell_t_d5ms)))
		{
			//INJ_ON;
			//CDI_ON;
			
			CDI_CHARGE
			nb_2ms=0;
			v_IGN1_control=true;
			t_2ms=T_MAINTMR;
		}
					
	 	if(v_IGN1_control&&(nb_2ms>=Dwell_t_d5ms))
		{
			//M_Inj_TOGGLE
			//INJ_OFF;
			 
			CDI_ON
			//CDI_CHARGE;
			nb_2ms=0;
			v_IGN1_control=false;
			t_2ms=T_MAINTMR;
		}
 	
		nb_1ms=T_MAINTMR-t_1ms;
		if((!v_INJ_control)&&(nb_1ms>=(frequency_1-flow_1time_ul)))
		{
			INJ_ON;
			nb_1ms=0;
			v_INJ_control=true;
			t_1ms=T_MAINTMR;
		}
					
	 	if(v_INJ_control&&(nb_1ms>=flow_1time_ul))
		{
			
			INJ_OFF;
			nb_1ms=0;
			v_INJ_control=false;
			t_1ms=T_MAINTMR;
		}
 	}
		

}
