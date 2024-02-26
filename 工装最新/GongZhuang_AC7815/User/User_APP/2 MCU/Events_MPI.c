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
 *		1.删除M0_PRO_3对VF_IS_CDI_INTIGN的配置.
 *		2.5000rpm以下点火充电时间缩短为600(2.4ms),5000rpm以上缩短为400(1.6ms)
 *		3.删除VF_RESERVE1_PRO2对前后沿计算的配置，采用M0_MODE_CA进行配置
 *
 * V9.1.0	2017.12.4	yangsili 
 *		Base function.
 *
 * V9.2.0   2017.12.28	yangsili 
 *		1.增加33814角标处理和原角标处理预编译条件
 *    2.更改喷油通道，屏蔽T3扫描使能
 *    3.增加33814复位信号产生时的处理函数
 *    4.CAN接收中断增加33814寄存器外部工具读写标志置位和缓存数据
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


byte nb_hyint; //后沿中断的数量
byte nb_hyint_all; //后沿中断的数量

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
word v_TW3_in;     //直接输入的T3                        
word v_TW4;        //2008-12-1 TW4                     
word v_TW4_2nd;        //2008-12-1 TW4后的第二个；                     
word v_tw1_synwithTW3;//与TW3同步的T1

sword v_FMF_SCI_idle;   //2008-8-2 to relace v_tw3_sci;
sword v_FMF_SCI_Nidle;  //2008-8-2非怠耸辟

byte nb_hy_betweenqy;//之前两个前沿之间，后沿的个数；//2008-8-1 add
byte nb_qy_betweenhy;//之前两个后沿之间，前沿的个数；//2008-8-1 add

byte nb_tw4_in;

byte v_nb_injection_ccl; //2012-6-27

byte nb_injection_fort35;//2009-12-25
word t_1st_t4rising;//2009-7-2

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
//2014-9-25 move here from INT_SERVE.C
byte nb_qy; //2009-8-30 add
byte v_nb_binj;
//累加启动或角标溢出后的齿数，直到255
//当前两个前沿之间，后沿的个数；//2008-8-1 add
//当前两个后沿之间，前沿的个数；//2008-8-1 add



word v_prd_cas_hy;  //角标后沿的周期;

byte nb_RTI;
byte nb_CapI;

word v_t_t3ed;

byte v_nb_T3_miss;//T3丢失的次数； 2015-7-4
byte v_CA_reset;

//多齿飞轮参数
unsigned char v_ToothPosition; //当前的齿号
unsigned char v_ToothPosition2; //当前的齿号

byte v_Toothnum;

//#define NB_TOOTH  36//齿数
//#define UB_NO_TOOTH  34//最大的齿号


word v_tw_Tooth[NB_TOOTH];
word v_t_Tooth_lst;//上一齿的时间
word v_t_Tooth_Now;

word v_tick_1stTooth;
word v_tick_1stTooth_lst;

byte v_nb_tdcerr;

byte VF_NEED_DWELL;
sbyte v_NO_ignTooth_trig;//点火齿
sbyte v_NO_dwellTooth_trig;//充电齿 
byte VF_DWELL_SET;
byte TachOff_Flag;
byte VF_CHARGE_OK;		//20210618 chenwei add CDI_CHARGE置true，CDI_ON置false

byte VF_IGN_OVER;

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//2014-9-25 MOVE FROM HERE FROM INT_SERVE.C 
#define TXNUMBER        20                 //发送的字节数  //2008-5-29 change from 20 to 30
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
byte F_ChargeEn;	//0:代表发生了充磁;1:代表触发了充磁中断，但是还没有充磁
byte F_Trig_IgnEn;	//0:触发了充磁，还没充又要点火，异常 1：正常的充磁后，要触发点火
byte F_Trig_IgnEn2;//20210420 chenwei add
word Trig_IgnTmeptime;

//20210930 chenwei add
byte v_nb_cycle;	//运行圈数（目前从第第2圈开始计数，<10）
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
    v_TW3=1;//2008-3-27 为了验证油门突增高速时的T3异常
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
		F_Trig_IgnEn2=0;		//20210930 此处应该初始化F_Trig_IgnEn2，原先初始化成了F_Trig_IgnEn=0
		
	
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
    
	/*2017-2-15 该版本适用电感点火方式，VF_IS_CDI_INTIGN=false
    //2016-9-18
        if(M0_PRO_3&0x80)
        {   //电感ECU,注意，应该在V_INTIGN_STATUS之后
            VF_IS_CDI_INTIGN=false;
        } 
        else
        {
            VF_IS_CDI_INTIGN=true;
        }
		*/
        VF_IS_CDI_INTIGN=false;//2016-9-20 临时
    
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
	v_NO_ignTooth_trig=BYPTR_M0_TOOTH_TDC/10;*///20210301 chenwei 临时屏蔽，出现串口升级工具刷完之后泵间隙工作的异常，换为下面两行ok
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
    

        //(void)TR_SCI0_RecvChar(&rcdata);                                      //读取接收缓冲区
        rcdata = UART_ReceiveData(UART2);
        /*-----2014-3-31 ADD---------------*/
            //端口数据读入缓冲区
            if(v_num_rx_tmp<BYTE_MAX-1)
            {
                //帧连续
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

    
        temph_int=(rcdata>>4)&0x0F;           //rcdata的高4位
        templ_int=rcdata&0x0F;                //rcdata的低4位

        //if(temph_int==templ_int)              //判断rcdata的高4位与低4位值是否相等
        //2014-3-31 CHANGE TO THE FOLLOWING
        if(temph_int==templ_int&&(VF_MODE_INTSCI==COMMODE_DIAG||(templ_int==0x07&&v_num_rx_tmp==0)))              //判断rcdata的高4位与低4位值是否相等
        { //以下为对几个控制位的校验,这些控制位功能是复用的
          //在停车时，可能会用于对EEPROM的操作
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
                    if(KESEI>=20&&!VF_MMOVE_ENG)v_PCSET_SCIH++;//调整访问FLASH/EEPROM的地址	
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
                    if(KESEI>=20&&!VF_MMOVE_ENG)v_PCSET_SCIH--;//调整访问FLASH/EEPROM的地址	
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
                   // Get_Kesei_for_CAB(0,1);//标定数据的WORD类	
                     
                    break;		
                case 0xB:
                    v_PCTBACK_SCI--;         
                    //Get_Tback_for_CAB(-1);//2012-10-22        
                    cmdAdjT1--;
                    
                   // Get_Kesei_for_CAB(0,-1);//标定数据的WORD类	
                    	
                    break;		
                case 0x4:
                  //  if(v_PCKESE_SCI>=0)v_PCSET_SCIL++;//调整访问FLASH/EEPROM的地址	
                  //  else v_PCSET_SCIH++;
                    if(KESEI==19)v_PCSET_SCIH++;//调整访问FLASH/EEPROM的地址	
                    else v_PCSET_SCIL++;  
                    break;//调整访问FLASH/EEPROM的地址	
                case 0xC:
                   // if(v_PCKESE_SCI>=0)v_PCSET_SCIL--;//调整访问FLASH/EEPROM的地址	
                    //else v_PCSET_SCIH--;
                   if(KESEI==19)v_PCSET_SCIH--;//调整访问FLASH/EEPROM的地址	
                    else v_PCSET_SCIL--;     
                    break;//调整访问FLASH/EEPROM的地址	
                case 0x5:
                   PCCQJSJD++;//Get_Kesei_for_CAB(1,0);             //break;//调整访问FLASH/EEPROM的数据		
                    cmdAdjCOS++;  
                    break;
                    
                case 0xD:
                   PCCQJSJD--;//Get_Kesei_for_CAB(-1,0);            //break;		
                    cmdAdjCOS--; 
                    break;
                    
                case 0x6:break;//VF_BTDCF_SCI=1; break;	
                case 0x7:
                    //上位机需要获得数据
                    VF_LINKPC_INTSCI=1;     //已经与PC机相连
                    VF_FORPC_INTSCI=1;//从PC机获得数据
                    VF_PCWANT_INTSCI=1;        //需要传输数据
                    break; 		
                case 0x8:
                    //if(!VF_MMOVE_ENG)
                    v_PCKESE_SCI++;                     //访问存储器的模式位

                    break;		
                case 0xF:
                    //if(!VF_MMOVE_ENG)
                    v_PCKESE_SCI--;          
                    
                    break;		
                case 0xE: 
                    //解除，或者增加里程预警
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
    
    if(VF_PREDRV_INTIGN)   //若出现不正常中断，返回
    {   //call MOV_STSPEED_TO_W;//获得点火所需的转速最低限值？？？ 0xD1
        //TEST_COME
		
		if(VF_STARTS_ENG)w=0xd1;else{w=0xd1;}//M0_START_SPEED;else{w=M0_IDLE_STOP_SP;}
        if(!VF_BDRV_INTIGN)       //表示需要开始点火
        { 
        	//20210618 chenwei add 发现启动不成功会出现发动机停机最后一圈或者停机之后将长时间仍有一次充磁电火，且充磁时间很长>100ms，增加条件(VF_CHARGE_OK&&(time_w>7000))限制充磁时间
			time_w=T_MAINTMR;
			time_w=v_t_IgnCharge_r-time_w;
			if((((vlst_prd_cas>>8)&0xff)<w) || VF_PREINJ_2CA||(VF_CHARGE_OK&&(time_w>3750)))	//20210929 备注，环松小包体限制为15ms？？
            { 
                //TEST_COME
				if(!VF_CASHY_BECHECK1_CAS||VF_PREINJ_2CA||(VF_CHARGE_OK&&(time_w>3750)))
				{
					CDI_ON;//2015-3-3 add condiction,用于禁止点火
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
                
            }   //当转速高于最低限值时，点火


            VF_BDRV_INTIGN=true;       //已经开始点火
            VF_BIGN_INTIGN=false;
			
			if(VF_NEED_CHARGE_IGN)
			{
				VF_NEED_DWELL=true;
				v_NO_dwellTooth_trig=v_NO_dwellTooth;
				v_NO_ignTooth_trig=v_NO_ignTooth;
              //2020-12-18
              v_t_TrigTooth_To_Ign_trig=v_t_TrigTooth_To_Ign;     //触发齿到点火时刻
              v_t_TrigTooth_To_Dwell_trig=v_t_TrigTooth_To_Dwell;   //触发齿到充电时刻
			  VF_IGN_OVER=true;
			}
			
            
        }
        else
        { 
           // TC_INJO_DisableEvent();//关闭通道4的比较输出中断
           //******************* here *********************//
            //TIE_C0I=false;
						PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
            VF_BIGN_INTIGN=true;              //点火结束
            VF_PREDRV_INTIGN=false;//2008-7-14 zhangping add
            VF_NEED_DWELL=false;
			VF_DWELL_SET=false;

			F_Trig_IgnEn2=0;
            if((IS_KEY_ON)||VF_KEYON_HARD)    //开启钥匙
            { 
                CDI_CHARGE;
				VF_CHARGE_OK=true;
					F_ChargeEn = 0;
					if(F_Trig_IgnEn == 0)
					{
							//w = v_t_TrigTooth_To_Dwell_trig+T_MAINTMR-Trig_IgnTmeptime;//20210420 换为v_t_TrigTooth_To_Dwell_trig
							w1=T_MAINTMR;
							//w2=v_t_TrigTooth_To_Ign_trig-(w1-Trig_IgnTmeptime)+T_MAINTMR;		//20210930 16.48现屏蔽测试
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
            }//当转速高于最低限值时，充电
			
			//以下主要是为了避免由于没有点火引起的充磁时间太长
                    if(F_Trig_IgnEn2 == 0)
					{
			t_ign=T_MAINTMR+7500;				   
            TC0=t_ign;//2015-10-8
            //TIE_C0I=true;
						PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
					}
            VF_BDRV_INTIGN=false;
                            
            //2016-7-25，这里需要设置最长的充电时间，30ms
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
       //*******这个地方原先关闭了定时器 2013-12-13 我在此处没有                 //关闭定时器2，关闭中断使能
        if(VF_PTRIG_INTINJ)                 //如果ECU没有触发准备喷油的动作，则返回
        { 
            if(!VF_PINJB_INTINJ)
            { 
                VF_NTESTPTSBINJ_TRANS=true;
                
                //if(VF_TRIGTPS_TRANS)
                //v_nb_mayadd_notsys v_nb_notsyninj_add
                if(VF_MMOVE_ENG&&((VF_TRIGTPS_TRANS&&!VF_ADDFUEL_SET)||(VF_ADDFUEL_SET&&VF_E0_TPS)))
					//2020-7-3 加喷触发后，发现油门回怠速了，停止加喷
                {//如果ECU要中断喷油的动作,置喷油结束标识，则返回
                    VF_INJSTOP_HARD=true;
					VF_ADDFUEL_SET=false;
                } 
                else
                {  // *************************2013-12-11
                    //--------------------------------
                    if(v_NB_INJDRV>0||VF_MMOVE_ENG||VF_PREINJ_2CA)                 //2008-12-16 移动至此，因为这里是真正的喷油触发
                    {
                        //if(!VF_TRIGTPS_TRANS)	
                        F_after_Inj_d3ms();               //准备0.3ms后喷油
                        //TEST_COME
						 if(v_NB_INJDRV>0&&(switch_1==0))v_NB_INJDRV--;//20210427 chenwei add "&&(switch_1==0)"
						//if(VF_PREINJ_2CA)VF_PREINJ_2CA=false;
                        //if(v_nb_notsyninj_add>0&&VF_TRIGTPS_TRANS)v_nb_notsyninj_add--; //2012-10-21add

                        if(VF_MMOVE_ENG)VF_T35TEST_HARD=false;//2009-4-18
						
						#ifndef SYS_TRADITIONAL
                        if(VF_T35TESTNEED_NOTYSTDC){//此时是否需要T35检测 
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
                VF_NTESTPTSBINJ_TRANS=false;      //2008-8-18 增加，
                
                if(VF_MMOVE_ENG)               //2009-8-24 DELETE
                { 
                    //检测喷油器是否开路；并得到code_number,binjector故障码
                    if(!IS_INJDRV_HIGH)    //2009-12-18
                    {
                        VF_OPEN_INJCTL=true;
                    }
                } //2009-12-11 DELETE
                INJ_ON;  
                //MIL_ON;           //开启喷油器
                FP_End_Inj_WhenTMROverFlow();  //载入喷油脉宽

                v_nb_injOn_r++;
                v_t_injOn_r=T_MAINTMR;
                v_t_bInj_toTrig_r=v_t_injOn_r-v_t_trigT1;
                v_t_bInj_toCas_r=v_t_injOn_r-v_t_Cas_trigT1;
                v_dt_bInj_toCas=v_t_bInj_toCas_r-v_t_bInj_toCas;
                
                v_nb_binj++;

                v_dnb_Inj=v_nb_injOn_r-v_nb_binj;
                
                
                if(VF_T35TEST_HARD)nb_injection_fort35++;//
                //v_TW4=0;            //2009-11-29 add  //2009-11-29发现在此另t4等于0会导致t4在使用时总是为0
            }
            else 
            { 
                //v_vbat_atInjOff=GetVbat_Tmp(); //2015-3-11 暂时删除，因为会对普通的AD造成影响
               
               // v_TW4=0;//2010-1-14
                INJ_OFF;                    //关闭喷油器
				if(VF_PREINJ_2CA)VF_PREINJ_2CA=false;
				VF_INJ_SET=false;
				VF_ADDFUEL_SET=false;
                //MIL_OFF;
                //TIE_C1I=false;
                //FUN_TIMER11_DisAbleInt();
								PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
							
                F_Clr_PostInjOnTime();
                
                // *************************2013-12-11 此处没有溢出中断，只有开启或关闭输出比较中断，有必要
                //---------------------------------------------------
                VF_INJSTOP_HARD=true;      //置位喷油结束标识                
                v_t_t3ed=T_MAINTMR;           //将定时器1的计数值传到v_t_t3ed中，作为TV3的下降时刻
                                            //小钟原来好像是写反了	
                
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
                
               // v_dnb_T1=v_nb_injOff_r-v_nb_injOn_r;   //2019-2-27 delete，空间不够用
               // v_dnb_Inj_YSTDC=v_nb_injOff_r-v_nb_YSTDC;
                
                if(v_nb_T3_miss<255)v_nb_T3_miss++;//2015-7-4
                
                //-----2008-7-27 add---------------------------------
                if(!VF_MMOVE_ENG)
                { 
                    if(VF_PREINJEND_SYSINI&&!VF_PREPUMPEND_SYSINI) //需要用T3对预喷油量进行修正
                    {  //T35和T3对预喷参数的修正，需要张平添加
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
                    //2012-10-20 临时删除
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
			//TIMER_ClrIntFlag(TIMER3); 		AC7811的定时器不会溢出
      //TPM2MOD=0xffff;  //2008-3-7 张平添加，需要向小钟定时器2到底工作在什么模式下
			
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
    if(!PORTE_CAI2)return;//33814前沿响应的是上升沿
#endif
#if (M_UPorDown == 1)
    if(PORTE_CAI2)return;//33814前沿响应的是上升沿
#endif
    for(i=0;i<10;i++)
    {
        __NOP;
    }
#if (M_UPorDown == 0)
    if(!PORTE_CAI2)return;//33814前沿响应的是上升沿
#endif
#if (M_UPorDown == 1)
    if(PORTE_CAI2)return;//33814前沿响应的是上升沿
#endif

		
    //v_timeout_10ms_noCA=0;//角标到来后，该值清零
    //if(v_nb_tooth_cap<255)

    v_t_Tooth_lst=v_t_Tooth_Now;//2020-9-24 从末尾挪到这个地方，以免后面的return导致无法赋值
	
	v_t_Tooth_Now=(word)TC3;
	
	if((word)(v_t_Tooth_Now-v_t_Tooth_lst)<(word)40)return;
	
	v_t_CA_ncome=0;

    v_ToothPosition++;
	v_ToothPosition2++;
	
	if(!VF_CATCH_CAS&&(v_Toothnum<255))v_Toothnum++;
	
	
    if(v_ToothPosition>UB_NO_TOOTH)v_ToothPosition=1;
   
    //备份之前的齿时间
    for(i=NB_TOOTH-1;i>=1;i--)
    {
        v_tw_Tooth[i]=v_tw_Tooth[i-1];    //备份的是相邻两个齿的时间
    }

    //计算当前的齿时间
    v_tw_Tooth[0]=(word)(v_t_Tooth_Now-v_t_Tooth_lst);
	
	


    //判断缺齿
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
		//&& (!(VF_CATCH_CAS&&(v_ToothPosition<=10)&&(v_ToothPosition>2)))	 //2021-3-2 这个条件会导致如果有一次缺齿判断错误，就再也进不去的情况	
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
		{//如果同步后，这个齿不是1号齿
            if(v_nb_tdcerr<255)v_nb_tdcerr++;//Acc_nb_Act(NA_TDCER);
        }
              
        //得到缺齿位置；
		
		#ifdef SY_24
			v_ToothPosition=2;//24-1的在第二个齿作为同步判断
			v_ToothPosition2=2;
		#else
			v_ToothPosition=1;//36-2的在第一个齿作为同步判断
			v_ToothPosition2=1;
		#endif

        //v_nb_tooth_cap++;
              
        v_tick_1stTooth_lst=v_tick_1stTooth;              
        v_tick_1stTooth=v_t_Tooth_Now;
		
		v_t_cas0=v_t_Tooth_Now;
		
        //发动机运行的判断
        //  得到一次缺齿，就代表发动机运行了              
        if(VF_CATCH_CAS){
        //如果已经获得过一次CA，
			#ifdef SY_24
				ul=NB_TOOTH*(unsigned int)v_tw_Tooth[2];
			#else
				ul=NB_TOOTH*(unsigned int)v_tw_Tooth[1];
			#endif
                    
			//可以计算转速了
			v_prd_cas=(word)(v_tick_1stTooth-v_tick_1stTooth_lst);
			v_CA_reset=false;//这个标志位代表原来角标没有正确被采集时，角标周期不会被更新
			
			if(ul>0xafffu){
				if(v_prd_cas<0x7fffu) {
					//说明v_prd_cas有溢出
					v_prd_cas=0xffffu;
				}
			}
			if(v_nb_cycle<10)v_nb_cycle++;		//20210930 chenwei add
                    
			/*if(v_prd_cas<0xffffu) 
			{
				VF_MMOVE_ENG=true;

			}*/
        } else{
            //第一次获得CA

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
            if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志 
            //2016-8-4 发现可能需要把这个加上，否则，出了这个中断后，马上进另一个中断；即开始点火，这样就会搞乱点火时序；

        }*/

        FCLEAR_CAOV
                
        VF_PCAOV_CAS=false;
                
                
        VF_OLDCA_CAS=false;
		VF_OLDCAHY_CAS=true;
        VF_CATCH_CAS=true;//2008-10-21 恢复
				
		VF_YSTDC_INJPHASE=!VF_YSTDC_INJPHASE; 
		//TEST_CR
		
		VF_NEED_CHARGE_IGN=false; 
        if((
            !VF_YSTDCGET_INJPHASE                          
            ||(VF_YSTDCGET_INJPHASE&&VF_YSTDC_INJPHASE)//VF_YSTDC_INJPHASE为true，代表下次缺齿为上止点前的缺齿，需要点火
            ||(REMARK<MC_OPENLOOP)
            ||(VF_NOTDELAY_IGN&&v_TPS_odr>4) 
			)
            &&(
                !VF_IGNONLY_CFCI
                 ||
                 (VF_GT10_IDLETG_REV&&(v_revmax_625_cf<112||(v_ccl_cf<30)))//80 for 5000rpm    112　for 7000
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
            if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志 
			
		}*/
                
        //Acc_nb_Act(NA_CCL);
		//V_CHARGE_ON

    }
	
//----------------------设定后沿--------------------------	
	#ifdef SY_12	
		if(v_ToothPosition==3)
		{
			v_t_cashy0=v_t_Tooth_Now;//后沿时间，用于判断相位
			VF_OLDCAHY_CAS=false;
	
		}
	#else	
		if(v_ToothPosition==6)
		{
			v_t_cashy0=v_t_Tooth_Now;//后沿时间，用于判断相位
			VF_OLDCAHY_CAS=false;
	
		}
	#endif
	
	#ifdef SY_36
	if(v_ToothPosition==12)
	{
		TachOff_Flag=true;	
	}
	#endif
	
//----------------------喷油触发--------------------------	

	//----------启动时候第二圈（第一个缺齿后）的喷油触发
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
		

		FP_Inj_WhenTMROver(100,v_preinj_ca);//延迟100us
	}
	
	
	//离开怠速时的加喷
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
			if(w>1250)w=1250;//加喷最大脉宽5ms
		}

		FP_Inj_WhenTMROver(100,w);
		VF_INJ_SET=true;
		VF_NEEDADDFUEL_CR=false;
		VF_ADDFUEL_SET=true;
		
		//TEST_COME
	}



	//---------启动一圈以后的喷油触发-------------
	//都放到排气-进气冲程一圈的最后一个齿触发	
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

		//FP_Inj_WhenTMROver(4,v_tw1);//+Trig Injection//  //触发喷油
		//FP_Inj_WhenTMROver(4,v_tw1_injphase);
		
		
		VF_TURNONINJ_ENG=true;
		VF_TRIGTPS_TRANS=false;
		VF_FORINJ_INJPHASE=true;
		
		FP_Inj_WhenTMROver(v_ts_binj,v_tw1_inj);
		
		VF_INJ_SET=true;//2020-7-1 已触发喷油
		//TEST_COME

	}

//----------------------点火触发--------------------------


	//-------刚启动时候第二圈（第一个缺齿后）的点火触发-------
#ifdef SY_24
	#ifdef	SY_HUANSONG		//20210930 chenwei add 环松
	if(VF_PREINJ_2CA&&(v_ToothPosition==8)&&(v_tw_Tooth[2]<2080))//转速低于300rpm不点火，300rpm，24齿，一个齿间隔8ms左右//24-1
	#else
	if(VF_PREINJ_2CA&&(v_ToothPosition==7)&&(v_tw_Tooth[2]<2080))//转速低于300rpm不点火，300rpm，24齿，一个齿间隔8ms左右//24-1
	#endif
#endif
#ifdef SY_36
	if(VF_PREINJ_2CA&&(v_ToothPosition==6)&&(v_tw_Tooth[2]<1400))//转速低于300rpm不点火，300rpm，36齿，一个齿间隔5.5ms左右//36-2
#endif
#ifdef SY_12
	if(VF_PREINJ_2CA&&(v_ToothPosition==4)&&(v_tw_Tooth[2]<4160))//转速低于300rpm不点火，300rpm，12齿，一个齿间隔16ms左右//12-3
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
	//没着火时候在第6齿充磁
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
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志 
		PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		*/		//20210929 chenwei 屏蔽，充磁改短此处可能会造成充磁过长
	}
	
	
	
	//------------启动一圈以后的点火触发---------------	
	if(VF_MMOVE_ENG) //2020-12-22 删掉&&!VF_IGNONLY_CFCI，否则断油时候有长时间充磁情况  2015-3-12 ADD 
    {          
            //触发充电
            
      
            
           // if((v_ToothPosition==v_NO_dwellTooth_trig)&&VF_NEED_DWELL)
           
           //2020-12-18 如果在非启动，且转速小于等于500rpm时，不充电
    
           		
            if((v_ToothPosition==v_NO_dwellTooth_trig)&&VF_NEED_DWELL&&(VF_STARTS_ENG||(v_rev_625rpm>8)))
            {
                VF_DWELL_SET=true;              		        
               // w=v_t_TrigTooth_To_Dwell+v_t_Tooth_Now;                        
                w=v_t_TrigTooth_To_Dwell_trig+v_t_Tooth_Now;//2020-12-18                        
	
          			F_Trig_DWELL(w);
					F_ChargeEn = 1;
            }           

            //触发点火
            if(v_ToothPosition==v_NO_ignTooth_trig)
            {
                //w=v_t_TrigTooth_To_Ign+v_t_Tooth_Now;
                w=v_t_TrigTooth_To_Ign_trig+v_t_Tooth_Now; //2020-12-18

			if(VF_DWELL_SET)//代表触发了充磁，但是还没充，又要点火了，那就在点火触发时马上充磁
				{/*
					VF_BIGN_INTIGN=true;
					CDI_CHARGE
					VF_NEED_DWELL=false;
					VF_DWELL_SET=false;
					VF_CHARGE_OK=true;
					
					v_t_IgnCharge_r=T_MAINTMR;*/	//20210927 16.52 此处在似乎还是会进入，屏蔽试试，充磁执行正确
			
				/*TC0=T_MAINTMR+7500;				   
				TIE_C0I=true;
				VF_BDRV_INTIGN=false;
                            
				VF_PREDRV_INTIGN=true;
				if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志*/
          				
          				//关键点2：2020-12-15 张平，注意这里应该要清除点火中断标志，不过F_Trig_Ign中已经有了这句话，所以不需要了。
          				//防止正好在CA中断进行时这个时候，IGN也出现中断
          				//DWELL改为比较模式后，CA中断处理期间，IGN中断同时出现，这个时候端口会自动被切换为CDI_CHARGE电平，也不影响
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


/*	角标的处理挪到主函数里面去	  
//----------------------角标门限--------------------------    
    

	//33814的角标处理
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
//备份齿数的时刻
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
    if(P_TV4_VALID)   //读取端口,当端口电平不正确时，返回
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
                    TPM1C3SC_CH3F=0;      //清除中断标志
                    TPM1C3SC_CH3IE=0;     //关闭中断使能
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
    if(P_TV3_VALID)   //读取端口,当端口电平不正确时，返回
    { 
        //Vsamp_GetCaptureValue(&te);
        //2015-10-8
        te=TC7;
        
        for(i=0;i<=100;i++)
        {
        } //2008-5-7 REMOVE AND ADD VBAT AD
        //F_SelCh_ADC(CH_AD_VBAT,0); //2008-5-7 add用于采集T3测量时刻的电压；
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
               
                VF_T3GET_INTINJ=true;	//置位T3已经捕获的标志
                v_tw1_synwithTW3=v_TW1_forTrig;     //2008-6-25
                if(!VF_T35TEST_HARD)VF_AT3GETAFTERSTART_INTINJ=true;//2008-7-26 add 
                VF_T3BECHECK_INTINJ=false;
                
                v_nb_T3_miss=0;//2015-7-4
                //2014-4-7 ADD CONDICTION
                if(!VF_MMOVE_ENG||(VF_MMOVE_ENG&&!VF_FORINJ_INJPHASE)) //2008-12-1 先获得T3 ，再捕获T4；//2010-3-6DELETE
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
                    VF_T4GET_INTINJ=false;//2010-3-20 add 为了在丢失T4情况下，不采上一次T1的信号；
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
    w1=FAD_1Channel(CH_AD_CURRENT1,false,1);//采集两次的电压值，并进行平均化的处理。
    */
    //Save_I_Load_12(w,w1);     //保存两次的电流值(这个地方被屏蔽了)
    //VF_TRIG_SAMPI_CAS=false;
    VF_CAP0_LDSENSOR=1 ;           //表示采集到了？
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
  /* 这块的程序处理的是主定时器的溢出 */
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
  byte i;  //控制变量
  byte j;
    byte b1,b2,b3,b4;
  if(ERR_OK == CAN_ReadFrame( &MessageID, &FrameType, &FrameFormat, &Length, &Data[0]) ) //接收数据
  {        
            //2015-5-29 接收的来自外部的命令 
            if(MessageID==FRAME_CMD_TEST) 
            {
                for(i=0;i<8;i++)_STRU_CMDIN.Byte[i]=Data[i];

                //2015-7-13 恢复
                _STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME=true;
                _STRU_CMDIN.CMD_IN.TimeOut_200ms=0;
                /* 
                2015-6-12 专属与上位机给DCU的命令
                _STRU_CMDIN.CMD_IN.ID_L=(byte)(MessageID&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_H=(byte)((MessageID>>8)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_HL=(byte)((MessageID>>16)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.ID_HH=(byte)((MessageID>>24)&0x0000ffu);
                _STRU_CMDIN.CMD_IN.VF_RECNEW_FRAME=true;
                */    
            } 
           // else if(MessageID==ID_15765_ASK)
            else if(MessageID==ID_15765_ASK_ST_ELM||MessageID==ID_15765_ASK_ST) //2016-2-27
            {   //诊断仪按照15765协议发来的请求
                _FRAME_15765.COMMON.IS_NEW=true;
                //_FRAME_15765.COMMON.IS_SENDING=false;//2016-2-23 delete ，与SCR同步
                
                for(i=0;i<8;i++)_FRAME_15765.Byte[i]=Data[i];
    
            }
							//20210422 chenwei add顾维东开发需求
			/***可通过调频控制喷油的喷射量***/
			else if(MessageID==0x18DE0004)
			{				
				switch_1=Data[0];
				b1=Data[1];
				b2=Data[2];		//脉宽 ms
				frequency_1=b1*250;
				flow_1time_ul=b2*250;

				b1=Data[3];
				b2=Data[4];
				//b1=1000/b1;		//  ms/次
				Period_d5ms=Data[3]*250;
				Dwell_t_d5ms=Data[4]*250;
			}
      else if(MessageID == RW33814ID) //2018-4-9,ysl,add,CAN接收中断增加33814寄存器外部工具读写标志置位和缓存数据
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

//20210606 chenwei add 李胜测试用，通过调节频率和脉宽实现点火控制用于高压包耐久

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
		//点火
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
