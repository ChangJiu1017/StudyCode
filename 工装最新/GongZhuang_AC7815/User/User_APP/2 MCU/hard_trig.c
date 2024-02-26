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
 *		���͵��ʱOS������������Ϊ��5750rpm���ϣ������ʱOS��Ȼ����.
 *
 * V9.1.0	2017.12.5	yangsili 
 *		Base function.
 *
 * V9.2.0   2017.12.5	yangsili 
 *		1.���ĵ�����Ͷ�ʱ��ͨ��
 *    2.2018-4-9,���ӵ������������������Ȧ���ɼ�����
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
byte v_t1_xh;//����������ʱ���ĸ߸�λ
word v_TW1_forTrig;                //Ϊ�˲�ֱ��ʹ��v_TW1_forTrig��ȫ�ֱ������ر���Ӹñ�����
byte ch_select;                        //��������!MMOVEʱ���ò���ֵ�� Trig_Inj_whenStop����
                        //��������MMOVEʱ���ò���ֵ�ɡ�Pre_Inj_when_TMROver������?
                        //����⵽�����٣���������ʱ����ֵ��INJ_AFTERD3MS_withTW1���룻?
                        //�ô���������ʦԭ˼��������ͬ��

byte v_nb_on;

byte v_nb_off;//2009-8-17 add for debug
byte v_nb_stop;//2009-8-17

byte v_nb_mayadd_notsys;//���Լ���ķ�ͬ���ĽǱ�Ĵ�����

 

word t_lstSampPairs; //ͣ��ʱ���ϴβɼ�PAIRS��ʱ�̣�2010-10-10


/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

#define CONST_T1            false                //2009-11-25 add 
#define REMARK_FOR_CONSTT1  13                  //v_PCSET_SCI_TOCOM
#define TD3MS 	125                     //��ʱ��2��Ӧ��0.3ms��ģ?//2008-7-8 ����Ϊ?


byte v_NB_noRX;           //Ϊû����PCͨѶ��ʱ�����
byte nb_mainov;
word t_1stDetectKeyoff;     //��һ�μ�⵽��Կ��ʱ��ʱ��  t_1stDetectKeyoff=T_MAINTMR;

//****************************************************************************
//****************************************************************************
//���ı��������Ǻ�ADģ����صġ�
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

//2018-4-9,ysl,���ӵ������������������Ȧ���ɼ�
word v_Coi1_ad10bit;//,v_Coi1_ad10bit_lst,v_Coi1_ad10bit_lst2;
word v_Coi2_ad10bit;//,v_Coi2_ad10bit_lst,v_Coi2_ad10bit_lst2;


/* Dosing Command */
dword v_DosingRate_AsACU_fromCAN;            // CMD_4 bytes Dosing rate �����������
dword v_DosingRate_AsDCU_fromACU;            // ��ΪDCUʱ����XMASTERģʽʱ����ACU���ص�״̬��

byte  Motor_state;		      // CMD_2 bits Motor override state ���״̬(FAI not used)
byte  Pump_state;		      // CMD_4 bits Requested pump state ��״̬ 

byte  Pump_state_lst;		      // ��һ�ε�Pump_state 

byte  Pump_state_nxt;		      // ���Ӷ���Ҫ�л���״̬�Ķ��壻2014-9-10

byte  v_PumpState_AsACU_fromCAN;             //Pump_state����CAN
byte  v_PumpState_AsDCU_fromACU;         //��ΪDCUʱ�����յ��ģ�����ACU������
byte  v_PumpState_fromSCI;                //��������SCI�ıõ�״̬������

/* UDS Dosing and OBD status */
byte  Actual_Pump_state;	  // RST_4 bits Actual Pump State 
byte  DM_Pump_Solenoid;       // RST_2 bits Pump Solenoid-Diagnostic status ��ŷ����״̬ 
byte  DM_Pump_Interal_heater; // RST_2 bits Pump Interal heater-Diagnostic status�ڲ����������״̬ 
byte  DM_Pump_Urea;			  // RST_Pump Urea-Diagnostic status �������״̬ 
byte  DM_Pump_Drive_unit;     // RST_Pump Drive unit-Diagnostic status ������Ԫ���״̬ 
byte  DM_Pump_EEPROM; 		  // RST_Pump EEPROM-Diagnostic status EEPROM���״̬ 
byte  Pump_Heating_status;    // RST_Reserved Pump Heating status  Ԥ���ü���״̬ 
byte  Purge_status; 		  // RST_Pump incomplete purge status  ��ϴ״̬

byte v_mode_DCU;

byte urea_temp;	       //������Һ�¶�
byte urea_level;	   //������ҺҺλ

/* pdataΪSCR Software Identification������,qdataΪSCR Address Claim������ */
byte pdata[8]={0x00,0x01,0x00,0x00,0x00,0x00,0x00,'*'}, qdata[8]={0x00,0x00,0x00,0x45,0x00,0x00,0x00,0x00};
//byte pdata[8]={0xff,0x01,0x01,0x01,0x01,0x01,0x01,0xf9}, qdata[8]={0x08,0x07,0x05,0x05,0x04,0x03,0x02,0xff};  //������
byte rxdata[8]={0,0,0,0,0,0,0,0};

byte RTI_flag, API_flag;        //�ж�ʱ���־
byte RTI_cnt, API_cnt;          //�ж�ʱ�������־
byte Connect_DCU;	            //��DCU���ӳɹ���־
byte AD_value;		            //12λ����ֵ
byte priming_in;                //Ԥ��-װ�������
byte purging_in;                //��ϴ������
word t_tick_now;                //��ǰʱ��

byte v_tick_200ms_LostFromSCI;          //����û�н��ܵ�����SCI�����ݵ�ʱ��

word t_RxFrame_lst;             //���һ�λ�ȡ����֡��ʱ��
word dt_RxFrameToNow;           //����δ���յ�����֡��ʱ��
word t_TxFrame_lst;             //���һ�η�������֡��ʱ��


byte v_nb_timer;
word v_nb_timer2;

dword v_t_ModeStay_10ms;        //һֱά�������һ��ģʽ��ʱ��

//2014-12-24 ADD 

    word t_forStartI;
    word t_forUb;//=t_forUb-200us
    
    byte v_nb_IIndex;//�ɼ����±�

    word v_t_I[UB_I+1][2];      // 0:��ǰ��1����һ��
    word v_I_I[UB_I+1][2];      // 2015-1-19 from byte to word
    byte v_V_I[UB_I+1][2];      //

    word v_T1_pre[UB_I+1][2];   //Ԥ��T1

    word v_wn_tmp[UB_I+1][2];   //��ǰ��Wn

    byte v_CH_AD_beforeI;//�ɼ�����֮ǰ��ADͨ��

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
extern byte nb_cc;		//20210819 chenwei add ���Ϩ��IO���͵Ĵ���/ʱ�䣨0.1s�ۼ�һ�Σ�


//****************************************************************************
//****************************************************************************
//���ܣ�����������������T1
void Set_TW1_for_Trig(word t1)
{
    v_TW1_forTrig=t1;    
}

//****************************************************************************
// @Function      void TERMINATE_INJECTION(void) 
//���ܣ���ֹ���Ͷ�����
void F_Terminate_Inj(void)      //�رն�ʱ��2��������
{ 
    //TPM2SC;
    //TPM2SC=0x00; 
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
    //TFLG1 = 0x02U;   //�رն�ʱ��2���ر�����жϣ����жϱ�־λ
    //TIE_C1I=false;
    //TFLG1 = 0x40U;   //�رն�ʱ��2���ر�����жϣ����жϱ�־λ
    //TIE_C6I=false;
   // CLS_TM2_EN;  //2013-6-18
    PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
    PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
    INJ_OFF;//                      PORTF_INJO=1;                     //���������˿�
    VF_INJSTOP_HARD=true;             //�������Ͷ���������?
    
    return;
}

#if SYS==SYS_PI
    //���ܣ�
    //  ��������δ��תʱ�����ڳ�ʼ���Ǳ���Ϣ
    void FI_CAS_WhenCANotCaught(void)
    {
        	VF_CATCH_CAS=false;                                 //δ����κνǱ�
        	VF_MAINTMROV_CAS=false;		                    	//T1δ���
        	VF_PCAOV_CAS=false;			                        //�Ǳ�δ���
        	VF_OLDCA_CAS=true;		                        	//��ǰ���ϽǱ�
          
          VF_2NDCA_STOP=false;                            //δ�õ��ڶ����Ǳ�
          VF_3RDCA_STOP=false;                            //δ�õ��������Ǳ�
          
          VF_NEWCYCLE_ATMAIN_CAS=false;
          
          CA_TH_L;//2014-4-18
    }

    //****************************************************************************
    // @Function      void F_Terminate_Ign(void)
    //���ܣ���ֹ������� 
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
    //���ܣ��������
    //�βΣ�tl_ign�����ӳ�ʼʱ�̿�ʼ�����֮���ʱ��
    void F_Trig_Ign(word tl_ign) 
    { 
        word t_ign;
       //ZZQ TPM1C4SC_CH4IE=0;
       //ZZQ TPM1SC_CLKSA=0;  //2008-7-9 stop timer1
                         //���Ǳ���ģ�������ܳ��� TPM1C4V=tl_ign+T_MAINTMR ��ֵ��Ҫ256*4us����������¸�λ��?
    //    TPM1C4V=0x0320+T_MAINTMR;                 //�趨��������ʱ�� //2008-4-14 ��䲻�Ƿϻ�
        
        v_t_bIgn_toTrig=tl_ign;
        
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־ //2008-4-14 ������жϱ��Ų����TPM1C4V��ֵ֮ǰ
        PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		 //(void)TC_IGNIO_GetCounterValue(&t_ign);
        //t_ign=T_MAINTMR;//2015-10-8 
        //t_ign=tl_ign+t_ign;;                 //�趨��������ʱ��,2008-2-23 ��ƽ��ӣ���Ҫ��С��ȷ��
        //TOBECHECK��Ϊ��AW60ֻ��������ʱ������˵�����ж����Ķ�ʱ��
        //TC_IGNIO_SetCompare(t_ign);
        TC0=tl_ign;//2015-10-8
        //PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL4, tl_ign);
        VF_PREDRV_INTIGN=true;
        VF_BDRV_INTIGN=false;
        VF_BIGN_INTIGN=false;
        //TIE_C0I=true;
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
    //    if(TPM1C4SC_CH4F){ TPM1C4SC_CH4F=0;}    //����жϱ�־ //2008-4-14 �Ӹ�λ��Ų��
        //ZZQ TPM1C4SC_CH4IE=1;                       //ʹ��ͨ��4�ıȽ�����ж�
       //ZZQ TPM1SC_CLKSA=1;  //2008-7-9 start timer1
        return;
    }

	void F_Trig_DWELL(word tl_ign) 
    { 
        word t_ign;
        
        //if(TFLG1_C0F){ TFLG1=TFLG1_C0F_MASK;}    //����жϱ�־ //2008-4-14 ������жϱ��Ų����TPM1C4V��ֵ֮ǰ
		PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
		//t_ign=T_MAINTMR;//2015-10-8 
        //t_ign=tl_ign+t_ign;;
		//TC0=t_ign;
		
		TC0=tl_ign;//2015-10-8            
        VF_PREDRV_INTIGN=true;
		VF_BDRV_INTIGN=true;
		VF_BIGN_INTIGN=false;
          
        //if(VF_NEED_CHARGE_IGN)//2015-3-3��δ�Ż������£���QY��ʼ��磬���ﲻ�䣬QY����ʱ�����ü�
        //{
            //TIE_C0I=true;    
			PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
        //} else{
            //TIE_C0I=false;//�����Ҫ����CDI_CHARGE����ر��ж�ʹ��
        //}

        return;
    }  

#endif

//****************************************************************************
// @Interrupt handler   void F_INTTMR1_32m(void) 
//���ܣ�����ʱ�������Ĳ�����
 void F_INTTMR1_32m(void)
{ 
    byte com_to_pc;

    nb_mainov++;
    //2009-8-30ADD 

    com_to_pc=v_NB_noRX*2;   
    if(VF_FORPC_INTSCI)//���ڼ�¼��ÿ������ʱ�����ʱ����PC��ͨѶ��״̬//v_NB_noRX<<1;//����λ����
    { 
        com_to_pc=v_NB_noRX|0x01;//��λ����λ
    }      
    else
    {
        com_to_pc=v_NB_noRX&0xFE;//�������λ
    }      
    VF_FORPC_INTSCI= false;
    
    if((com_to_pc&0x0F)==0){ VF_LINKPC_INTSCI=false;}        //ȷ����PC�Ƿ�Ͽ�����
    else{ VF_LINKPC_INTSCI=true;}
    
    //ָʾ�ƵĿ���

    //-CLR_WDG;                //SRS = 0xFF;//�忴�Ź� TOBECHECK
    VF_MAINTMROV_CAS=true;

    return;   
}

//****************************************************************************
// @Function      void F_after_Inj_d3ms(void) 
//���ܣ����ö�ʱ��2��0.3ms���������������ж��п�ʼ���Ͷ���
//word TempInjtime[8];
void F_after_Inj_d3ms(void)  //
{   
    word t_binj;
    
    //TC_INJO_GetCounterValue(&t_binj); //2015-10-8
    t_binj=T_MAINTMR;
    
    t_binj=t_binj+TD3MS;
    //TC_INJO_SetCompare(t_binj);
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
    //TC1=t_binj;
    TC6=t_binj;
    //TempInjtime[0] = TD3MS;
    INJ_OFF;           //�ر�������
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
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
//���ܣ����ö�ʱ��2��0.3ms���������������ж��п�ʼ���Ͷ���
void F_after_Inj_xms(word xms)  //
{   
    word t_binj;
   // v_t1_xh=(byte)xms>>16;
    
    INJ_OFF;           //�ر�������
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    //TC_INJO_GetCounterValue(&t_binj);
    
    t_binj=T_MAINTMR;
    
    t_binj=t_binj+xms;
    //TempInjtime[1] = xms;
    //TC_INJO_SetCompare(t_binj);
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
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
//���ܣ����ö�ʱ��2��0.3ms���������������ж��п�ʼ���Ͷ���������������������ΪT1
//�βΣ�t1������Ҫ����������������
void F_after_Inj_d3msWithTW1(word t1)  //
{
    word t_binj;
    v_TW1_forTrig=t1;
   
    INJ_OFF;           //�ر�������
    VF_PINJE_INTINJ=false;
    VF_INJSTOP_HARD=false;
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=true;
    VF_T3GET_INTINJ=false;
    //TC_INJO_GetCounterValue(&t_binj);
    
    t_binj=T_MAINTMR;

    t_binj=t_binj+TD3MS;
    //TempInjtime[2] = TD3MS;
    //TC_INJO_SetCompare(t_binj);         //�趨0.3ms���������
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
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
//���ܣ���TMR����󣬿�ʼ׼��������INJ_AFTERD3MS����TMR��ֵ��Ԥ�����úõģ�����ֵ��������Ҫ��������λ��
//      ���ڷ���������ʱ����ѭ���е��õ�?
//�βΣ�t_binj����������ǰʱ�䣬���ӵ��ú�����ʼt_binj (ms)��ʼ����
//      t1������Ҫ�������͵�����������?
void FP_Inj_WhenTMROver(word t_binj,word t1)
{
    word t_binj1;
    
    v_nb_inj++;
    
    v_t_bInj_toTrig=t_binj;//
    
    //v_t_Cas_trigT1=t1;
    
    v_TW1_forTrig=t1;
    v_t1_xh=0;
   // if(t_binj<4)t_binj=4;//2008-12-16 ���ʱ�䣻
   
    if(t_binj<25)t_binj=25;//2019-6-17 ���ֿ��ܱ������жϴ�ϣ��������ȱʧ
    VF_INJSTOP_HARD=false;
    INJ_OFF;             //����������
    VF_PTRIG_INTINJ=true;
    VF_PINJB_INTINJ=false;
    VF_T3GET_INTINJ=false;
    //TC_INJO_GetCounterValue(&t_binj1);
    t_binj1=T_MAINTMR;

    t_binj1=t_binj1+t_binj;
		
    //TempInjtime[3] = v_TW1_forTrig;
    //TC_INJO_SetCompare(t_binj1);
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
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
//���ܣ��������������붨ʱ��2,׼���ڶ�ʱ��2����ж�ʱ����������
void FP_End_Inj_WhenTMROverFlow(void)       //
{ 
    //word tw1;
     word t_binj;
     word w_t;
     
     
    if((VF_T1SCANING_INJCTL||VF_PUMP_INJCTL)&&!VF_MMOVE_ENG) //2008-6-25 add
    {       //2013-3-23 add                   //2013-4-22 ADD CONDICTION  !VF_MMOVE_ENG
        v_TW1_forTrig=Get_TW1_for_Scaning();//2008-12-1  //2014-4-5 �����������ɾ�ˣ����Ի��˺þ�
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
    //2017-10-30,ysl,���Ķ�ʱ��ͨ�� 
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
//�ɼ�
/*=============================================================*/
#if 0
word FAD_1Channel(byte Channel,byte bln8bit,byte times)
{
    word t_bsamp;
    word w;
    byte ch_select_pr;
    byte i;
    
    ch_select_pr=ch_select;//����֮ǰ��ADͨ��
    (void)ATDDR0;
  
    ATDCTL2 = 0x40U; //�ر�AD�ж�  ATDCTL2_ASCIE=0;

    
    w=0;
    for(i=0;i<times;i++){
      
        ATDCTL5 = Channel;
        
        t_bsamp=T_MAINTMR;
        
        while(!ATDSTAT2)
        {
            if((word)(T_MAINTMR-t_bsamp)>(word)100)
            {
                //2015-4-9 �����б������е�A ���ʱ���ᵼ�½���˴�������TPS����Ϊ0��
                return 0;
            }
        }
        w+=DAT_ADC_10BITS;
    }
    
    //ATDCTL2 = 0x42U;//����AD�жϲ�����AD�ж�
    w/=times;
    
    if(bln8bit)w/=4;
    
   // (void)F_SelCh_ADC(ch_select_pr,0); // A ���
    //if(KESEI==1) 
    {
      
      //  ATDCTL5=ch_select_pr;// 2015-4-9 ADD HERE  ,2015-4-9-2 ɾ�����֮��TPS���������ͻ��������ԭ����
    } 
    //else 
    {
        ch_select=ch_select_pr;
    }
    
    return w;
    
}
#endif


/*
    �����ɼ�
    һ�������󣬿����жϣ��жϵ��˴洢ADֵ���Ҽ�����һ��
    ֱ��  T1-����ǰʱ��-����ʱ�䣩<200US

    //���ǵ����ĵ�һ�����ʱӰ������ͨ���Ĳ�������ˣ���һ���������ֱ��ȡ0�����Ҳ��л�ͨ��
    
*/

/*
void FAD_I_TC5(byte bStatus) 
{
    word t_now;
    word t_bsamp;

    t_now=T_MAINTMR;
    
    v_nb_IAD++;
    
    
    if(bStatus==0){
        //��ʼ����
        VF_FORENERGY_AD=true;
        t_forStartI=T_MAINTMR;
        v_nb_IIndex=0;
        v_CH_AD_beforeI=ch_select;
        
        t_forUb=v_TW1_forTrig-DT_LSTAD; 
        
        //��¼��ǰʱ��
        v_t_I[0][0]=0;
        v_I_I[0][0]=0;
        //ATDCTL5=CH_TU;  //���л�ͨ�� 
    
        v_V_I[0][0]=FAD_1Channel(CH_DCDY,true,1);
    
        //���ô���ʱ��
        t_now+=DT_IAD;
        TC5=t_now;        
        
        TIE_C5I=true;
    
    
    } else{

        if(bStatus==2){
            //��������

            if(v_nb_IIndex<UB_I)v_nb_IIndex++;

            v_V_I[v_nb_IIndex][0]=FAD_1Channel(CH_DCDY,true,1);
            v_I_I[v_nb_IIndex][0]=FAD_1Channel(CH_TU,true,1);; //������¼�����ϸ�ʱ���ֵ�����Ǵ˿̵�
            v_t_I[v_nb_IIndex][0]=t_now-t_forStartI;


            VF_FORENERGY_AD=false;
            
            ch_select=v_CH_AD_beforeI;
            
            TIE_C5I=false;
            
            VF_FORENERGY_AD=false;
            VF_IAD_BEBACKUP_AD=false;
            
        }else{
            
            if(v_nb_IIndex>0)v_I_I[v_nb_IIndex][0]=DAT_ADC_10BITS/4; //������¼�����ϸ�ʱ���ֵ�����Ǵ˿̵�
            
            ATDCTL5=CH_TU;   
            
            t_bsamp=t_now-t_forStartI;
            
            if(t_bsamp<=t_forUb){
            
                if(v_nb_IIndex<UB_I)v_nb_IIndex++;
                v_t_I[v_nb_IIndex][0]=t_bsamp;
                
                //���T1��Ҫ����
                t_now+=DT_IAD;
                TC5=t_now;        
                
                TIE_C5I=true;
            } else{
                TIE_C5I=false;    
            }
        }
    }
    
}
*///���ϲ���ֵ��̫�ԣ�ԭ��������ʱ����һ��


#if SYS==SYS_PI

    //*******************************:start of a sub or function**********************************
    void Samp_TPS_And_Cal()
    {
        
        v_TPS_ad10bit_lst2=v_TPS_ad10bit_lst;
        v_TPS_ad10bit_lst=v_TPS_ad10bit;
        v_TPS_ad10bit=FAD_1Channel(CH_AD_TPS,false,4);

		/*			//20210925 ��������Ӧ�����������ֽ������� ��ʼ���ĵ�ѹ��һ�£�δ����������򣬻�������֮ǰ��
		#ifdef	SY_HUANSONG		//20210924 chenwei add ���ɲ��� δ��
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
        FG_U_bat(v_vbat_ad10bit);          //����DCDY�ɼ����
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
        
        //2018-4-9,ysl,delete 2016-1-9 ADD v_TPCB ��˵��¶ȴ������źŲ���
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
        
        FG_OS(v_O2_ad10bit,v_O2_ad10bit_lst);      //��¼O2�ɼ���� 
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
    //AD����
    /*=============================================================*/
    void FAD_ALL(byte remark){
        byte b;
        word w;
        word w1;

        //�����ѹû�вɼ�
        b=0;
        if(!VF_VBAT_ED_AD){
            if(VF_INJENDWHENSTOP_STOP|| VFF_INJ_DTC||IS_INJDRV_HIGH) 
            {
                b=1;
                Samp_Save_Vbat();
                
                VF_VBAT_ED_AD=true;
                

            }
        }
        //�ж�Կ��״̬
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

        //������δ���е�״̬�£�
        if(!VF_MMOVE_ENG)
        {
            Save_AD_cas(b);
        }
        
        if(!VF_TPS_ED_AD){
            VF_TPS_ED_AD=true;
            Samp_TPS_And_Cal();

        }
        
        if(!VF_OS_ED_AD){
            //��ת��ʱ����Ҫ�����������Ͷ���������������Ӱ�죬��Ϊ����������ͨ������
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))//2014-10-8 change to the following
            
            //���ڸ���ʱ�и��ţ���������
            //if(IS_INJDRV_HIGH&&!VF_ISCHARGE_IGN)
            if(IS_INJDRV_HIGH)v_nb_OS_AD_CD3++;
            
            //if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||VF_GT30_IDLETG_REV))//2014-10-8 change to the following
            if(IS_INJDRV_HIGH&&(!VF_ISCHARGE_IGN||(v_rev_625rpm>REV62D5_5750)))//2016-11-9  IN FAD_ALL,hard_trig.c
            //if(IS_INJDRV_HIGH)//2015-5-24 
            {
                v_nb_OS_AD_CD1++;
                //2015-3-8 ���֣�Ӧ����T_MAINTMR-������Ӧ����v_t_injOff_r-T_MAINTMR
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

        //2015-3-8����ѹ������������
        //if(!VF_PAIRS_ED_AD)
        
        //2015-3-8 ʱ�䲻�ԣ�����
        {
        

         // deDTC++;
            if(!VF_MMOVE_ENG&&!VF_CATCH_CAS&&(v_t_CA_ncome!=0)&&((!(IS_INJDRV_LOW))||VFF_INJ_DTC||VF_NOFUSE_VBAT)
                    &&VF_INJENDWHENSTOP_STOP//2010-10-10
                ) 

            {
            
                //2015-3-8
                //���ͽ���2ms�󣬵�����2ms��
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
            //2014-4-27 �뷢������صĲ��������Կ������ӷ�����δ���е�����
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
            // 2015-10-23 ��ʱ�����ǵ���������
            
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

    // ��Ϊǰ���ж�VF_T35TEST_INJ�ĸ�λ���̣������γ���ò��ܷ���ǰ��
    if(VF_VPOR_HARD)
    {
        VF_T35TEST_HARD=true;            //������ϵ縴λ�����ý���T35���Եı�ʶ,2014-10-16 DELETE
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
