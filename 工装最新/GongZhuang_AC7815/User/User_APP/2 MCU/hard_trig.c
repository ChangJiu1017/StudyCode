/*HEAD
 ***********************************************************************
 * Filename	:hard_trig.c
 * Function	:calculation for trig		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		喷油点火时OS不采样，更改为：5750rpm以上，点火充电时OS仍然采样.
 *
 * V9.1.0	2017.12.5	yangsili 
 *		Base function.
 *
 * V9.2.0   2017.12.5	yangsili 
 *		1.更改点火、喷油定时器通道
 *    2.2018-4-9,增加怠速提升阀步进电机线圈检测采集变量
 ***********************************************************************
HEAD END*/

#include "FAM.h"
#include "Service_CAN.h"
#include "MCU.H"

#include "Events.h"

#include "hard_trig.h"
#include "status_st.h"

#include "DTC.H"


#include "ad.h"

#include "add.h"

#include "inj_ctl.h"

#include "ad_scr.h"     //2014-11-4

#include "energy.h"     //2015-1-25
#include "time_ccl.h"
#include "ld.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
byte v_t1_xh;//喷油驱动定时器的高高位
word v_TW1_forTrig;                //为了不直接使用v_TW1_forTrig的全局变量，特别添加该变量；
byte ch_select;                        //当发动机!MMOVE时，该参数值由 Trig_Inj_whenStop传入
                        //当发动机MMOVE时，该参数值由‘Pre_Inj_when_TMROver’传入?
                        //当检测到急加速，马上喷油时，该值由INJ_AFTERD3MS_withTW1传入；?
                        //该处理与刘老师原思想有所不同；

byte v_nb_on;

byte v_nb_off;//2009-8-17 add for debug
byte v_nb_stop;//2009-8-17

byte v_nb_mayadd_notsys;//可以加喷的非同步的角标的次数；

 

word t_lstSampPairs; //停车时，上次采集PAIRS的时刻，2010-10-10


/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

#define CONST_T1            false                //2009-11-25 add 
#define REMARK_FOR_CONSTT1  13                  //v_PCSET_SCI_TOCOM
#define TD3MS 	125                     //定时器2对应于0.3ms的模?//2008-7-8 更改为?


byte v_NB_noRX;           //为没有与PC通讯的时间计数
byte nb_mainov;
word t_1stDetectKeyoff;     //第一次检测到关钥匙时的时间  t_1stDetectKeyoff=T_MAINTMR;

//****************************************************************************
//****************************************************************************
//这块的变量定义是和AD模块相关的。
word v_TPS_ad10bit,v_TPS_ad10bit_lst,v_TPS_ad10bit_lst2;
word v_vbat_ad10bit,v_vbat_ad10bit_lst,v_vbat_ad10bit_lst2;
word v_O2_ad10bit ,v_O2_ad10bit_lst ,v_O2_ad10bit_lst2 ;
word v_pair_ad10bit,v_pair_ad10bit_lst,v_pair_ad10bit_lst2;
word v_teng_ad10bit,v_teng_ad10bit_lst ,v_teng_ad10bit_lst2 ;
word v_tair_ad10bit ,v_tair_ad10bit_lst,v_tair_ad10bit_lst2;
word v_I1_ad10bit ,v_I1_ad10bit_lst,v_I1_ad10bit_lst2;
word v_I2_ad10bit ,v_I2_ad10bit_lst ,v_I2_ad10bit_lst2;

word v_MAP_ad10bit,v_MAP_ad10bit_lst,v_MAP_ad10bit_lst2;

byte v_TPCB;//2016-1-9

word v_TPS_ad10bit_bInj,v_TPS_ad10bit_bInj_lst,v_TPS_ad10bit_bInj_lst2;
word v_vbat_ad10bit_bInj,v_vbat_ad10bit_bInjend;

//2018-4-9,ysl,增加怠速提升阀步进电机线圈检测采集
word v_Coi1_ad10bit;//,v_Coi1_ad10bit_lst,v_Coi1_ad10bit_lst2;
word v_Coi2_ad10bit;//,v_Coi2_ad10bit_lst,v_Coi2_ad10bit_lst2;


/* Dosing Command */
dword v_DosingRate_AsACU_fromCAN;            // CMD_4 bytes Dosing rate 请求的喷射率
dword v_DosingRate_AsDCU_fromACU;            // 作为DCU时，即XMASTER模式时，从ACU返回的状态；

byte  Motor_state;		      // CMD_2 bits Motor override state 电机状态(FAI not used)
byte  Pump_state;		      // CMD_4 bits Requested pump state 泵状态 

byte  Pump_state_lst;		      // 上一次的Pump_state 

byte  Pump_state_nxt;		      // 增加对需要切换的状态的定义；2014-9-10

byte  v_PumpState_AsACU_fromCAN;             //Pump_state来自CAN
byte  v_PumpState_AsDCU_fromACU;         //作为DCU时，接收到的，来自ACU的命令
byte  v_PumpState_fromSCI;                //接收来自SCI的泵的状态的命令

/* UDS Dosing and OBD status */
byte  Actual_Pump_state;	  // RST_4 bits Actual Pump State 
byte  DM_Pump_Solenoid;       // RST_2 bits Pump Solenoid-Diagnostic status 电磁阀诊断状态 
byte  DM_Pump_Interal_heater; // RST_2 bits Pump Interal heater-Diagnostic status内部加热器诊断状态 
byte  DM_Pump_Urea;			  // RST_Pump Urea-Diagnostic status 尿素诊断状态 
byte  DM_Pump_Drive_unit;     // RST_Pump Drive unit-Diagnostic status 驱动单元诊断状态 
byte  DM_Pump_EEPROM; 		  // RST_Pump EEPROM-Diagnostic status EEPROM诊断状态 
byte  Pump_Heating_status;    // RST_Reserved Pump Heating status  预留泵加热状态 
byte  Purge_status; 		  // RST_Pump incomplete purge status  清洗状态

byte v_mode_DCU;

byte urea_temp;	       //尿素溶液温度
byte urea_level;	   //尿素溶液液位

/* pdata为SCR Software Identification数据区,qdata为SCR Address Claim数据区 */
byte pdata[8]={0x00,0x01,0x00,0x00,0x00,0x00,0x00,'*'}, qdata[8]={0x00,0x00,0x00,0x45,0x00,0x00,0x00,0x00};
//byte pdata[8]={0xff,0x01,0x01,0x01,0x01,0x01,0x01,0xf9}, qdata[8]={0x08,0x07,0x05,0x05,0x04,0x03,0x02,0xff};  //测试用
byte rxdata[8]={0,0,0,0,0,0,0,0};

byte RTI_flag, API_flag;        //中断时间标志
byte RTI_cnt, API_cnt;          //中断时间计数标志
byte Connect_DCU;	            //与DCU连接成功标志
byte AD_value;		            //12位采样值
byte priming_in;                //预喷-装填进行中
byte purging_in;                //清洗进行中
word t_tick_now;                //当前时间

byte v_tick_200ms_LostFromSCI;          //连续没有接受到来自SCI的数据的时间

word t_RxFrame_lst;             //最后一次获取数据帧的时间
word dt_RxFrameToNow;           //连续未接收到数据帧的时间
word t_TxFrame_lst;             //最后一次发出数据帧的时间


byte v_nb_timer;
word v_nb_timer2;

dword v_t_ModeStay_10ms;        //一直维持在最后一种模式的时间

//2014-12-24 ADD 

    word t_forStartI;
    word t_forUb;//=t_forUb-200us
    
    byte v_nb_IIndex;//采集的下标

    word v_t_I[UB_I+1][2];      // 0:当前，1：上一次
    word v_I_I[UB_I+1][2];      // 2015-1-19 from byte to word
    byte v_V_I[UB_I+1][2];      //

    word v_T1_pre[UB_I+1][2];   //预测T1

    word v_wn_tmp[UB_I+1][2];   //当前的Wn

    byte v_CH_AD_beforeI;//采集电流之前的AD通道

    byte v_nb_IAD;

    word t_T1_PredictbyW;//2015-1-28
    

//2015-5-24 ADD 
byte v_nb_OS_AD_CD1;
byte v_nb_OS_AD_CD2;
byte v_nb_OS_AD_CD3;

word v_qd_ad;
word v_qd_ad_lst;
byte v_qd;
byte nb_qd;
extern byte nb_cc;		//20210819 chenwei add 侧撑熄火IO拉低的次数/时间（0.1s累计一次）


//****************************************************************************
//****************************************************************************
//功能：设置喷油器工作用T1
void Set_TW1_for_Trig(word t1)
{
    v_TW1_forTrig=t1;    
}

//****************************************************************************
// @Function      void TERMINATE_INJECTION(void) 
//功能：终止喷油动作；
void F_Terminate_Inj(void)      //关闭定时器2和喷油器
{ 
    //TPM2SC;
    //TPM2SC=0x00; 
    //2017-10-30,ysl,更改定时器通道 
    //TFLG1 = 0x02U;   //关闭定时器2，关闭溢出中断，清中断标志位
    //TIE_C1I=false;
    //TFLG1 = 0x40U;   //关闭定时器2，关闭溢出中断，清中断标志位
    //TIE_C6I=false;
   // CLS_TM2_EN;  //2013-6-18
    PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
    PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
    INJ_OFF;//                      PORTF_INJO=1;                     //关喷油器端口
    VF_INJSTOP_HARD=true;             //设置喷油动作结束标?
    
    return;
}

#if SYS==SYS_PI
    //功能：
    //  当发动机未运转时，定期初始化角标信息
    void FI_CAS_WhenCANotCaught(void)
    {
        	VF_CATCH_CAS=false;                                 //未获得任何角标
        	VF_MAINTMROV_CAS=false;		                    	//T1未溢出
        	VF_PCAOV_CAS=false;			                        //角标未溢出
        	VF_OLDCA_CAS=true;		                        	//当前是老角标
          
          VF_2NDCA_STOP=false;                            //未得到第二个角标
          VF_3RDCA_STOP=false;                            //未得到第三个角标
          
          VF_NEWCYCLE_ATMAIN_CAS=false;
          
          CA_TH_L;//2014-4-18
    }

    //****************************************************************************
    // @Function      void F_Terminate_Ign(void)
    //功能：终止点火动作； 
    void F_Terminate_Ign(void) 
    {
        //TFLG1 = 0x01U; 
        //TIE_C0I=false;//zhangping add //TOBECHECK

    PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
    PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
        CDI_ON;//2014-4-7
        VF_ISCHARGE_IGN=false;//2014-4-28
    }

    //****************************************************************************
    // @Function      void trigign(word) 
    //功能：触发点火
    //形参：tl_ign－－从初始时刻开始至点火之间的时间
    void F_Trig_Ign(word tl_ign) 
    { 
        word t_ign;
       //ZZQ TPM1C4SC_CH4IE=0;
       //ZZQ TPM1SC_CLKSA=0;  //2008-7-9 stop timer1
                         //这是必须的，否则可能出现 TPM1C4V=tl_ign+T_MAINTMR 赋值需要256*4us的情况，导致复位；?
    //    TPM1C4V=0x0320+T_MAINTMR;                 //设定触发点火的时间 //2008-4-14 这句不是废话
        
        v_t_bIgn_toTrig=tl_ign;
        
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志 //2008-4-14 将清除中断标记挪到给TPM1C4V赋值之前
        PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		 //(void)TC_IGNIO_GetCounterValue(&t_ign);
        //t_ign=T_MAINTMR;//2015-10-8 
        //t_ign=tl_ign+t_ign;;                 //设定触发点火的时间,2008-2-23 张平添加，需要与小钟确认
        //TOBECHECK因为，AW60只有两个定时器，因此点火不能有独立的定时器
        //TC_IGNIO_SetCompare(t_ign);
        TC0=tl_ign;//2015-10-8
        //PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL4, tl_ign);
        VF_PREDRV_INTIGN=true;
        VF_BDRV_INTIGN=false;
        VF_BIGN_INTIGN=false;
        //TIE_C0I=true;
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
    //    if(TPM1C4SC_CH4F){ TPM1C4SC_CH4F=0;}    //清除中断标志 //2008-4-14 从该位置挪走
        //ZZQ TPM1C4SC_CH4IE=1;                       //使能通道4的比较输出中断
       //ZZQ TPM1SC_CLKSA=1;  //2008-7-9 start timer1
        return;
    }

	void F_Trig_DWELL(word tl_ign) 
    { 
        word t_ign;
        
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //清除中断标志 //2008-4-14 将清除中断标记挪到给TPM1C4V赋值之前
		PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		//t_ign=T_MAINTMR;//2015-10-8 
        //t_ign=tl_ign+t_ign;;
		//TC0=t_ign;
		
		TC0=tl_ign;//2015-10-8            
        VF_PREDRV_INTIGN=true;
		VF_BDRV_INTIGN=true;
		VF_BIGN_INTIGN=false;
          
        //if(VF_NEED_CHARGE_IGN)//2015-3-3在未着火的情况下，从QY开始充电，这里不充，QY充电的时间来得及
        //{
            //TIE_C0I=true;    
			PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
        //} else{
            //TIE_C0I=false;//如果不要触发CDI_CHARGE，则关闭中断使能
        //}

        return;
    }  

#endif

//****************************************************************************
// @Interrupt handler   void F_INTTMR1_32m(void) 
//功能：主定时器溢出后的操作；
 void F_INTTMR1_32m(void)
{ 
    byte com_to_pc;

    nb_mainov++;
    //2009-8-30ADD 

    com_to_pc=v_NB_noRX*2;   
    if(VF_FORPC_INTSCI)//用于记录，每次主定时器溢出时，与PC机通讯的状态//v_NB_noRX<<1;//带进位左移
    { 
        com_to_pc=v_NB_noRX|0x01;//置位移入位
    }      
    else
    {
        com_to_pc=v_NB_noRX&0xFE;//清除移入位
    }      
    VF_FORPC_INTSCI= false;
    
    if((com_to_pc&0x0F)==0){ VF_LINKPC_INTSCI=false;}        //确认与PC是否断开连接
    else{ VF_LINKPC_INTSCI=true;}
    
    //指示灯的控制

    //-CLR_WDG;                //SRS = 0xFF;//清看门狗 TOBECHECK
    VF_MAINTMROV_CAS=true;

    return;   
}

//****************************************************************************
// @Function      void F_after_Inj_d3ms(void) 
//功能：设置定时器2在0.3ms后溢出，溢出后在中断中开始喷油动作
//word TempInjtime[8];
void F_after_Inj_d3ms(void)  //
{   
    word t_binj;
    
    //TC_INJO_GetCounterValue(&t_binj); //2015-10-8
    t_binj=T_MAINTMR;
    
    t_binj=t_binj+TD3MS;
    //TC_INJO_SetCompare(t_binj);
    //2017-10-30,ysl,更改定时器通道 
    //TC1=t_binj;
    TC6=t_binj;
    //TempInjtime[0] = TD3MS;
    INJ_OFF;           //关闭喷油器
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    //2017-10-30,ysl,更改定时器通道 
    //TFLG1 = 0x02U; 
    //TIE_C1I=true;
    //TFLG1 = 0x40U; 
    //TIE_C6I=true;
	PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
	PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
   // VF_T3GET_INTINJ=false;   //2010-3-24 deleyte
   // VF_T4GET_INTINJ=false;//2008-12-1
    return;
}
//2012-10-21 add 
//****************************************************************************
// @Function      void F_after_Inj_xms(void) 
//功能：设置定时器2在0.3ms后溢出，溢出后在中断中开始喷油动作
void F_after_Inj_xms(word xms)  //
{   
    word t_binj;
   // v_t1_xh=(byte)xms>>16;
    
    INJ_OFF;           //关闭喷油器
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    //TC_INJO_GetCounterValue(&t_binj);
    
    t_binj=T_MAINTMR;
    
    t_binj=t_binj+xms;
    //TempInjtime[1] = xms;
    //TC_INJO_SetCompare(t_binj);
    //2017-10-30,ysl,更改定时器通道 
    //TC1=t_binj;
    TC6=t_binj;
    
    //TFLG1 = 0x02U; 
    //TIE_C1I=true;
    //TFLG1 = 0x40U; 
    //TIE_C6I=true;

	PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
	PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
    
   // VF_T3GET_INTINJ=false;   //2010-3-24 deleyte
   // VF_T4GET_INTINJ=false;//2008-12-1
    return;
}
//****************************************************************************
// @Function      void F_after_Inj_d3ms(void) 
//功能：设置定时器2在0.3ms后溢出，溢出后在中断中开始喷油动作，并且喷油脉宽设置为T1
//形参：t1－－需要驱动的喷油脉冲宽度
void F_after_Inj_d3msWithTW1(word t1)  //
{
    word t_binj;
    v_TW1_forTrig=t1;
   
    INJ_OFF;           //关闭喷油器
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    VF_T3GET_INTINJ=false;
    //TC_INJO_GetCounterValue(&t_binj);
    
    t_binj=T_MAINTMR;

    t_binj=t_binj+TD3MS;
    //TempInjtime[2] = TD3MS;
    //TC_INJO_SetCompare(t_binj);         //设定0.3ms的溢出周期
    //2017-10-30,ysl,更改定时器通道 
    //TC1=t_binj;
    TC6=t_binj;
    
    //TFLG1 = 0x02U; 
    //TIE_C1I=true;
    //TFLG1 = 0x40U; 
    //TIE_C6I=true;
	PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
	PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
    return;
}
//****************************************************************************
// @Function      void FP_Inj_WhenTMROver(word t1) 
//功能：当TMR溢出后，开始准备触发‘INJ_AFTERD3MS’，TMR的值是预先设置好的，设置值就是所需要的喷油相位；
//      是在发动机运行时，主循环中调用的?
//形参：t_binj－－喷油提前时间，即从调用函数开始t_binj (ms)后开始喷油
//      t1－－需要进行喷油的喷油脉冲宽度?
void FP_Inj_WhenTMROver(word t_binj,word t1)
{
    word t_binj1;
    
    v_nb_inj++;
    
    v_t_bInj_toTrig=t_binj;//
    
    //v_t_Cas_trigT1=t1;
    
    v_TW1_forTrig=t1;
    v_t1_xh=0;
   // if(t_binj<4)t_binj=4;//2008-12-16 最短时间；
   
    if(t_binj<25)t_binj=25;//2019-6-17 发现可能被其他中断打断，造成喷油缺失
    VF_INJSTOP_HARD=false;
    INJ_OFF;             //关喷油器；
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=false;
    VF_T3GET_INTINJ=false;
    //TC_INJO_GetCounterValue(&t_binj1);
    t_binj1=T_MAINTMR;

    t_binj1=t_binj1+t_binj;
		
    //TempInjtime[3] = v_TW1_forTrig;
    //TC_INJO_SetCompare(t_binj1);
    //2017-10-30,ysl,更改定时器通道 
    //TC1=t_binj1;
    TC6=t_binj1;
    
    //TFLG1 = 0x02U; 
    //TIE_C1I=true;
    //TFLG1 = 0x40U; 
    //TIE_C6I=true;
	PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
		//20210426 chenwei add
		if(switch_1==1)
		{t_binj=t_interval_4us;}
	PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
    //VF_T4GET_INTINJ=false;//2008-12-1
    //goto enable_t3;
}

//****************************************************************************
// @Function      void FP_End_Inj_WhenTMROverFlow(void) 
//功能：将喷油脉宽载入定时器2,准备在定时器2溢出中断时，结束喷油
void FP_End_Inj_WhenTMROverFlow(void)       //
{ 
    //word tw1;
     word t_binj;
     word w_t;
     
     
    if((VF_T1SCANING_INJCTL||VF_PUMP_INJCTL)&&!VF_MMOVE_ENG) //2008-6-25 add
    {       //2013-3-23 add                   //2013-4-22 ADD CONDICTION  !VF_MMOVE_ENG
        v_TW1_forTrig=Get_TW1_for_Scaning();//2008-12-1  //2014-4-5 这个被赵自庆删了，调试花了好久
        //v_TW1_forTrig=tw1;
    }
    else
    {
      //  TPM2MOD=v_TW1_forTrig; 
		if(VF_MMOVE_ENG&&!VF_ADDFUEL_SET&&!VF_PREINJ_2CA)
		{
			v_TW1_forTrig=v_tw1_inj;
			v_t_T1=v_tw1_inj;
		}
		
    }
     
     
    //TC_INJO_GetCounterValue(&t_binj);
    t_binj=T_MAINTMR;
    
    
    w_t=v_TW1_forTrig;
		
    //TempInjtime[4] = v_TW1_forTrig;
    //if(w_t>100)w_t-=16;
    if(w_t>100)w_t-=8;
    else w_t=80;
    
    //t_binj=t_binj+v_TW1_forTrig;
    t_binj=t_binj+w_t;//2014-12-24
    
		
//		{
//				byte Data[8];
//				Data[0] = w_t;
//				Data[1] = w_t>>8;
//				CAN_SendFrameExt(0x18ffFEEFul|CAN_EXTENDED_FRAME_ID,0,8,Data);
//		}
		
    //TC_INJO_SetCompare(t_binj);
    //2017-10-30,ysl,更改定时器通道 
    //TC1=t_binj;
    TC6=t_binj;
    
    //TIE_C1I=true;
    //TIE_C6I=true;
	PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
	PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
       // ENABLE_TMR2(v_TW1_forTrig);
       
    VF_T3GET_INTINJ=false;//2010-3-24 add ;
    
    return;
}

/*=============================================================*/
//采集
/*=============================================================*/
#if 0
word FAD_1Channel(byte Channel,byte bln8bit,byte times)
{
    word t_bsamp;
    word w;
    byte ch_select_pr;
    byte i;
    
    ch_select_pr=ch_select;//保存之前的AD通道
    (void)ATDDR0;
  
    ATDCTL2 = 0x40U; //关闭AD中断  ATDCTL2_ASCIE=0;

    
    w=0;
    for(i=0;i<times;i++){
      
        ATDCTL5 = Channel;
        
        t_bsamp=T_MAINTMR;
        
        while(!ATDSTAT2)
        {
            if((word)(T_MAINTMR-t_bsamp)>(word)100)
            {
                //2015-4-9 发现有本程序中的A 语句时，会导致进入此处，引起TPS采样为0；
                return 0;
            }
        }
        w+=DAT_ADC_10BITS;
    }
    
    //ATDCTL2 = 0x42U;//开启AD中断不开启AD中断
    w/=times;
    
    if(bln8bit)w/=4;
    
   // (void)F_SelCh_ADC(ch_select_pr,0); // A 语句
    //if(KESEI==1) 
    {
      
      //  ATDCTL5=ch_select_pr;// 2015-4-9 ADD HERE  ,2015-4-9-2 删除这句之后，TPS采样往大的突增的现象，原因不明
    } 
    //else 
    {
        ch_select=ch_select_pr;
    }
    
    return w;
    
}
#endif


/*
    电流采集
    一次启动后，开启中断，中断到了存储AD值，且继续下一次
    直到  T1-（当前时间-启动时间）<200US

    //考虑到担心第一点采样时影响其他通道的采样，因此，第一点电流采样直接取0，并且不切换通道
    
*/

/*
void FAD_I_TC5(byte bStatus) 
{
    word t_now;
    word t_bsamp;

    t_now=T_MAINTMR;
    
    v_nb_IAD++;
    
    
    if(bStatus==0){
        //开始测试
        VF_FORENERGY_AD=true;
        t_forStartI=T_MAINTMR;
        v_nb_IIndex=0;
        v_CH_AD_beforeI=ch_select;
        
        t_forUb=v_TW1_forTrig-DT_LSTAD; 
        
        //纪录当前时刻
        v_t_I[0][0]=0;
        v_I_I[0][0]=0;
        //ATDCTL5=CH_TU;  //不切换通道 
    
        v_V_I[0][0]=FAD_1Channel(CH_DCDY,true,1);
    
        //设置触发时间
        t_now+=DT_IAD;
        TC5=t_now;        
        
        TIE_C5I=true;
    
    
    } else{

        if(bStatus==2){
            //结束测试

            if(v_nb_IIndex<UB_I)v_nb_IIndex++;

            v_V_I[v_nb_IIndex][0]=FAD_1Channel(CH_DCDY,true,1);
            v_I_I[v_nb_IIndex][0]=FAD_1Channel(CH_TU,true,1);; //电流记录的是上个时间的值，不是此刻的
            v_t_I[v_nb_IIndex][0]=t_now-t_forStartI;


            VF_FORENERGY_AD=false;
            
            ch_select=v_CH_AD_beforeI;
            
            TIE_C5I=false;
            
            VF_FORENERGY_AD=false;
            VF_IAD_BEBACKUP_AD=false;
            
        }else{
            
            if(v_nb_IIndex>0)v_I_I[v_nb_IIndex][0]=DAT_ADC_10BITS/4; //电流记录的是上个时间的值，不是此刻的
            
            ATDCTL5=CH_TU;   
            
            t_bsamp=t_now-t_forStartI;
            
            if(t_bsamp<=t_forUb){
            
                if(v_nb_IIndex<UB_I)v_nb_IIndex++;
                v_t_I[v_nb_IIndex][0]=t_bsamp;
                
                //如果T1快要结束
                t_now+=DT_IAD;
                TC5=t_now;        
                
                TIE_C5I=true;
            } else{
                TIE_C5I=false;    
            }
        }
    }
    
}
*///以上采样值不太对，原因不明，暂时调整一下


#if SYS==SYS_PI

    //*******************************:start of a sub or function**********************************
    void Samp_TPS_And_Cal()
    {
        
        v_TPS_ad10bit_lst2=v_TPS_ad10bit_lst;
        v_TPS_ad10bit_lst=v_TPS_ad10bit;
        v_TPS_ad10bit=FAD_1Channel(CH_AD_TPS,false,4);

		/*			//20210925 杨赠长反应，环松有两种节气门体 初始化的电压不一致，未避免两版程序，还是沿用之前的
		#ifdef	SY_HUANSONG		//20210924 chenwei add 环松测试 未定
		v_TPS_ad10bit-=59;

		#endif*/
        
        SaveADForM(v_TPS_ad10bit);
        
        FG_TPS(v_rev_15d6rpm);
         
        return;
    }

    //*******************************:start of a sub or function**********************************
    void Samp_Save_Vbat()
    {
        
        v_vbat_ad10bit_lst2=v_vbat_ad10bit_lst;
        v_vbat_ad10bit_lst=v_vbat_ad10bit;
        v_vbat_ad10bit=FAD_1Channel(CH_AD_VBAT,false,2);
        FG_U_bat(v_vbat_ad10bit);          //保存DCDY采集结果
    }

    //*******************************:start of a sub or function**********************************
    void Samp_Save_TMP()
    {
        
        v_teng_ad10bit_lst2=v_teng_ad10bit_lst;
        v_teng_ad10bit_lst=v_teng_ad10bit;
        v_teng_ad10bit=FAD_1Channel(CH_AD_TENG,false,2);

        v_tair_ad10bit_lst2=v_tair_ad10bit_lst;
        v_tair_ad10bit_lst=v_tair_ad10bit;
        
        
        v_tair_ad10bit=FAD_1Channel(CH_AD_TAIR,false,2);
        
        //2018-4-9,ysl,delete 2016-1-9 ADD v_TPCB 板端的温度传感器信号采样
        //v_TPCB=255-FAD_1Channel(CH_AD_TPCB,true,2);

        //2012-9-24 10 BITS
        FG_TMP(v_tair_ad10bit,v_teng_ad10bit,v_rev_15d6rpm,v_ldb,v_TPS_odr);
        
    }

    //*******************************:start of a sub or function**********************************
    void Samp_Save_OS()
    {

        v_O2_ad10bit_lst2=v_O2_ad10bit_lst;
        v_O2_ad10bit_lst=v_O2_ad10bit;
        v_O2_ad10bit=FAD_1Channel(CH_AD_OS,false,2);
        
        v_O2_ad10bit_lst2=v_O2_ad10bit_lst;
        v_O2_ad10bit_lst=v_O2_ad10bit;
        v_O2_ad10bit=FAD_1Channel(CH_AD_OS,false,2);
        
        FG_OS(v_O2_ad10bit,v_O2_ad10bit_lst);      //记录O2采集结果 
    }

    void Samp_Save_MAPS(void)
    {
        v_MAP_ad10bit=FAD_1Channel(CH_AD_MAP,false,2);//2016-1-11 from true to false
        
        //FG_Maps(v_MAP_ad10bit/4,v_prd_cash,v_TPS_odr,0,0); 
        FG_Maps(0,v_prd_cash,v_TPS_odr,v_MAP_ad10bit,v_MAP_ad10bit); 
        
        
    }

    void Samp_Save_PAIR(byte remark)
    {
            //2008-7-16 add pair
        VF_PAIR_BESAMP_WHENMOVE_REV=true;//2010-5-11
        v_pair_ad10bit_lst2=v_pair_ad10bit_lst;
        v_pair_ad10bit_lst=v_pair_ad10bit;
        v_pair_ad10bit=FAD_1Channel(CH_AD_PAIR,false,2);
        v_Pair_ad_tmp=(byte)(v_pair_ad10bit>>2); //2013-8-14 add for pair samp when stop
        FG_P_air(remark);  //2013-7-16 2014-10-8 ADD REMARK AS PARA

    }
#ifdef SY_QD	
	void Samp_Save_QD(void)
    {
        v_qd_ad=FAD_1Channel(CH_AD_TDC_OP,false,2);
		v_qd_ad=(byte)(v_qd_ad>>2);
		
		v_qd=(byte)(((word)v_qd_ad+v_qd_ad_lst)/2);				
		
		v_qd_ad_lst=v_qd_ad;

    }
#endif

    /*=============================================================*/
    //AD采样
    /*=============================================================*/
    void FAD_ALL(byte remark){
        byte b;
        word w;
        word w1;

        //如果电压没有采集
        b=0;
        if(!VF_VBAT_ED_AD){
            if(VF_INJENDWHENSTOP_STOP|| VFF_INJ_DTC||IS_INJDRV_HIGH) 
            {
                b=1;
                Samp_Save_Vbat();
                
                VF_VBAT_ED_AD=true;
                

            }
        }
        //判断钥匙状态
        //b=FAD_1Channel(CH_AD_TDC_OP,true,2);
	#ifdef SY_QD
		if(!VF_MMOVE_ENG||VF_NEWCYCLE_ATMAIN_CAS)
		{
			Samp_Save_QD();
			if(v_qd<100)//2v
			
			{
				if(VF_1S_TMCCL&&(nb_qd<255))nb_qd++;
			}
			else
			{
				nb_qd=0;
			}
			
			if(nb_qd>=5)
			{
				VF_QD_ST=true;
			}
		}
	#endif       

        //发动机未运行的状态下：
        if(!VF_MMOVE_ENG)
        {
            Save_AD_cas(b);
        }
        
        if(!VF_TPS_ED_AD){
            VF_TPS_ED_AD=true;
            Samp_TPS_And_Cal();

        }
        
        if(!VF_OS_ED_AD){
            //低转速时，需要消除点火和喷油对氧传感器采样的影响，因为氧传感器地通过车架
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))//2014-10-8 change to the following
            
            //由于高速时有干扰，更改条件
            //if(IS_INJDRV_HIGH&&!VF_ISCHARGE_IGN)
            if(IS_INJDRV_HIGH)v_nb_OS_AD_CD3++;
            
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))//2014-10-8 change to the following
            if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||(v_rev_625rpm>REV62D5_5750)))//2016-11-9  IN FAD_ALL,hard_trig.c
            //if(IS_INJDRV_HIGH)//2015-5-24 
            {
                v_nb_OS_AD_CD1++;
                //2015-3-8 发现：应该是T_MAINTMR-，而不应该是v_t_injOff_r-T_MAINTMR
                if((word)(T_MAINTMR-v_t_injOff_r)>(word)25)//2015-3-8 FROM 10 TO 25
                {
                    if((word)(T_MAINTMR-v_t_IgnSetOn_r)>(word)25)
                    {
                        v_nb_OS_AD_CD2++;
                        VF_OS_ED_AD=true;
                        Samp_Save_OS();
                    }
                }
            }
        }

        
        if(!VF_TES_ED_AD) 
        {
            Samp_Save_TMP();
            VF_TES_ED_AD=true;
            VF_TAS_ED_AD=true;
            
            
            Samp_Save_MAPS();//2016-1-11 add here !VF_TES_ED_AD
        }

        if(1)Samp_Save_Vbat();

        //2015-3-8更改压力采样的条件
        //if(!VF_PAIRS_ED_AD)
        
        //2015-3-8 时间不对，待查
        {
        

         // deDTC++;
            if(!VF_MMOVE_ENG&&!VF_CATCH_CAS&&(v_t_CA_ncome!=0)&&((!(IS_INJDRV_LOW))||VFF_INJ_DTC||VF_NOFUSE_VBAT)
                    &&VF_INJENDWHENSTOP_STOP//2010-10-10
                ) 

            {
            
                //2015-3-8
                //喷油结束2ms后，点火结束2ms后
                w1=T_MAINTMR;
                
                w=w1-t_lstSampPairs;
                if(w>5000)//20ms==5000)
                {
                           // if(deDTFI>w)deDTFI=w;
                           // if(remark==3)deDTFI=0xffff;
                    //2015-3-8 add this two condiciton
                    if((word)(T_MAINTMR-v_t_injOff_r)>(word)500)
                    {
                        if((word)(T_MAINTMR-v_t_IgnSetOn_r)>(word)500)
                        {

                            
                             Samp_Save_PAIR((remark));
							 //TEST_CR//P5
							 //TEST_COME//P6
                             VF_PAIRS_ED_AD=true;
                            
                     //       deOS++;
                            
                            t_lstSampPairs=w1;
                            //TEST_COME
                        }
                    }
                }
            } 
        }

        //#if ECU_TYPE==ECU_GEN
        
        #ifdef IS_GEN
            /*
            //2014-4-27 与发动机相关的采样，可以考虑增加发动机未运行的条件
            F_SelCh_ADC(CH_AD_CURRENT1,1);	           //20121017 wwf add  current sample
            //temp=F_WaitSample_ADC_result();        //WAITING FOR THE END OF VBAT SAMP
            F_SelCh_ADC(CH_AD_CURRENT1,1);	
            //temp2=F_WaitSample_ADC_result();        //WAITING FOR THE END OF VBAT SAMP
            Save_I_Load_12(temp,temp2);
            
            F_SelCh_ADC(CH_AD_CURRENT2,1);	
            //temp=F_WaitSample_ADC_result();        //WAITING FOR THE END OF VBAT SAMP
            F_SelCh_ADC(CH_AD_CURRENT2,1);	
            //temp2=F_WaitSample_ADC_result();   
            Save_I_Load_34(temp,temp2);
            */
            //FAD_1Channel
            /*
            F_CAL_TPS_TG_BY_I(v_rev_3d9,v_TPS);
            */
            // 2015-10-23 暂时不考虑电流的作用
            
            FD_LUB();//2012-11-2
        
        #endif    
          
    }

#endif

//2014-3-30 MOVE HERE
void FI_Hard_AfterReset(void)
{

    if(VF_JKEYON_HARD||VF_POR_HARD)
    {
        VF_VPOR_HARD=true;//2008-7-6 temperory
    } 
    else
    {
        VF_VPOR_HARD=false;    
    }

    // 因为前面有对VF_T35TEST_INJ的复位过程，因此这段程序得不能放在前面
    if(VF_VPOR_HARD)
    {
        VF_T35TEST_HARD=true;            //如果是上电复位，设置进行T35测试的标识,2014-10-16 DELETE
		VF_TES_ED_AD=false;
		v_qd=255;
		v_qd_ad=255;
		v_qd_ad_lst=255;
		nb_qd=0;
		VF_QD_ST=false;

		VF_CC_CONNECT=false;
		VF_CC_ST=false;		//20210819 chenwei add
		nb_cc=0;

    }


    if(VF_POR_HARD){
        v_nb_on=0;
        v_nb_off=0;
    }
    else v_nb_on++;
    
    v_nb_mayadd_notsys=0;
    
    VF_FORENERGY_AD=false;//2014-12-14
    VF_IAD_BEBACKUP_AD=false;//2014-12-14
    

}
