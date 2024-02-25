/*HEAD
 ***********************************************************************
 * Filename	:hard_trig.h
 * Function	:Ӳ������ͷ�ļ�	
 * User		:yangsili
 * Date		:2017.12.5
 * Version	:V9.1.0
 *
 *** History ***
 *
 * V9.1.0	2017.12.5	yangsili 
 *		Base function.
 * V9.2.0   2017.12.5	yangsili 
 *		1.����33814ͷ�ļ�
 *    2.���ĺ궨�����üĴ���
 * 
 ***********************************************************************
HEAD END*/

#include "PE_Types.h"

//#include "IO_Map.h"

#include "sys_definition.h"
#include "FAM.h"
//2017-10-24,ysl
#include "mc33814.h"

//----------------------------------------------------------------------------
//2014-12-24
    //#define DT_IAD 25//100USһ��
    //#define DT_IAD 70//100USһ��
    //#define DT_IAD 25//100USһ��
    #define DT_IAD 25//100USһ��
    #define UB_I    100
    //#define UB_I    120
    //#define DT_LSTAD  40
    #define DT_LSTAD  75

    //2014-12-24 ���ڵ����ɼ�
    extern byte v_nb_IIndex;//�ɼ����±�

    extern word v_t_I[UB_I+1][2];      // 0:��ǰ��1����һ��
    extern word v_I_I[UB_I+1][2];      // 2015-1-19 from byte to word 
    extern byte v_V_I[UB_I+1][2];      //

    extern byte v_nb_IAD;

    extern word v_T1_pre[UB_I+1][2];   //Ԥ��T1
    
    extern word v_wn_tmp[UB_I+1][2];   //��ǰ��Wn
    
    extern word t_T1_PredictbyW;//2015-1-28

//----------------define for public variables---------------------------------
extern byte v_t1_xh;////����������ʱ���ĸ߸�λ
extern word v_TW1_forTrig;//����Ҫ������T1
extern byte v_nb_off;//2009-8-17 add for debug
extern byte v_nb_stop;//2009-8-17
extern word t_1stDetectKeyoff;     //��һ�μ�⵽��Կ��ʱ��ʱ��  t_1stDetectKeyoff=T_MAINTMR;
extern byte v_nb_mayadd_notsys;//���Լ���ķ�ͬ���ĽǱ�Ĵ����� 2012-9-14
extern byte ch_select;

extern byte v_nb_on;
 
extern void F_INTTMR1_32m(void);
extern void F_after_Inj_d3ms(void);
extern void F_after_Inj_xms(word xms);
extern void F_after_Inj_d3msWithTW1(word t1);
extern void FP_Inj_WhenTMROver(word t_binj,word t1);
extern void FP_End_Inj_WhenTMROverFlow(void);  
extern void Set_TW1_for_Trig(word t1);
extern void F_Terminate_Inj(void);
extern void F_Terminate_Ign(void);

extern void F_Trig_Ign(word tl_ign);
extern void F_Trig_DWELL(word tl_ign);
extern void FI_CAS_WhenCANotCaught(void);
extern void Wait_till_1stCACome(void);
extern void Waiti_for_2ndCACome(void);
extern void Init_after1stCACome(void);
extern void FI_status_ENG_BeforeStarts(void);
extern void Waiti_for_AD_SAMPL(void);

extern word FAD_1Channel(byte Channel,byte bln8bit,byte times);

extern void FAD_ALL(byte remark);

extern void FI_Hard_AfterReset(void);//2014-3-30

extern void Samp_TPS_And_Cal(void);//�ɼ�4��TPS,������TPS������
extern void Samp_Save_Vbat(void);
extern void Samp_Save_TMP(void);
extern void Samp_Save_OS(void);
extern void Samp_Save_MAPS(void); //t_MAPS����ɼ���ʱ��
extern void Samp_Save_PAIR(byte remark);//2008-7-16 add

//----------------define or public macro---------------------------------------
//=============================================================================
//��������������ECUӲ�����
//����������MACRO FOR I/O�˿ڶ��壭����������������������������������������������


#if SYS==SYS_PI
    //����������    
    #define PORTE_CAI2      M_CAIN//PTT_PTT3      //�Ǳ�ǰ�ض˿�  
    
    //20210819 chenwei add ��Ŷ˿�(20211002 ��ע)
    #define PORTJ_CC		M_CC_T//PTJ_PTJ2		//20210819 chenwei add ��ż��˿�	����0�൱��Ϩ�𿪹ش�

	
    
    //2017-10-24,ysl,����������1#����������2#��ӦPT7,����T3�˿ڡ�T4�˿ڵ����ö˿�PJ5
    #define PORTE_INT_TV3   M_INJ2T3IN//PTJ_PTJ5       //T3�˿�
    #define PORTF_INJO2     M_Inj_ST//PTT_PTT7       //��������������˿�2#
    #define P_TV4_VALID     0//PTJ_PTJ5 //PTFD_PTFD1//2008-12-1
    #define PORTF_INJO      M_Inj_ST//PTT_PTT6       //��������������˿�1#
    
    #define PORTF_IGNIO     M_Ing//PTT_PTT0 //PTGD_PTGD0       //        //�����������˿�  //rex 2011-11-2 PTF5���𻵣���ΪPTG0
    //2017-10-24,ysl,���ӵ��2#
    #define PORTF_IGNIO2    M_Ing//PTT_PTT1  //�����������˿�2
    
    
    //2017-10-30,ysl,����δʹ�õ�ָʾ�ƣ���������·���˿�
    //#define PORTG_GZXSO     PT0AD_PT0AD4         //ָʾ�������˿�   //2008-7-15?
    //#define PORTG_INJ_OPENI  PTJ_PTJ7 //PT0AD_PT0AD7       //��������·����?  //2008-7-15?
    
    //2017-10-24,ysl,����33814SPIƬѡ�˿�
    #define  P_MC33814_CS        SPI1_CS_C//PTJ_PTJ6      //MC33814Ƭѡ
    
    //2017-10-24,ysl,����Կ�׿��ض˿�,ISVC�˿�
    #define PORTG_KIGNI     M_KeyST//PT0AD_PT0AD7 //PTJ_PTJ7          //Կ�׼��˿�  //2008-7-15
    #define PORTF_TDC_HYI   //M_CAIN//PTT_PTT4        //�Ǳ���ؼ��˿�
    #define P_ISCV          1//PTP_PTP6 //PT0AD_PT0AD3 //PTFD_PTFD1       //ISCV�����˿�

    //2017-10-24,ysl,����Կ�׿��ض˿�
    #define IS_KEY_ON       M_KeyST//PT0AD_PT0AD7  //PTJ_PTJ7 //PORTG_KIGNI      //Կ���Ƿ���
    #define IS_KEY_OFF      !M_KeyST//!PT0AD_PT0AD7 //!PTJ_PTJ7//!PORTG_KIGNI  Կ���Ƿ�ر�

    //2014-4-8,ȷ��AD��8���ڣ�16��ͨ����PORT0Ϊ8-15��PORT1Ϊ0-7��
    //2017-10-24,ysl,����MIL��������ʽ
    #define MIL_ON  {mc33814_LampSet_def(1);}//{PT0AD_PT0AD4=1;}  //PTCDD_PTCDD2=1; PORTG_GZXSO=1;      //ָʾ�ƴ�  //2008-7-15
    #define MIL_OFF {mc33814_LampSet_def(0);}//{PT0AD_PT0AD4=0;} //PTCDD_PTCDD2=1;PORTG_GZXSO=0;     //ָʾ�ƹر�  //2008-7-15

	//2020-9-8 ת�����
	#define TACH_ON  {mc33814_TachSet_def(0);}
    #define TACH_OFF {mc33814_TachSet_def(1);}
	#define	TACH_TOGGLE	TACH_CR		//20210922 chenwei add	�˴������Σ�20210930 ������Ҫת������ָ���
    
	
    //2017-10-24,ysl,����POWER-C
    #define PWR_ON  {mc33814_PrepareToShutDownSet_def(1);} //{PT0AD_PT0AD6=1;}
    #define PWR_OFF {mc33814_PrepareToShutDownSet_def(0);}//{PTP_PTP4=0;} //{PT0AD_PT0AD6=0;}

    //2017-10-24,ysl,�����������˿�,����������2
    #define INJ_OFF   M_Inj_OFF//{PTT_PTT6=0;}//{PTT_PTT1=1;}//PORTF_INJO=1               //�������ر�  //2012-2-7    
    #define INJ_ON    M_Inj_ON//{PTT_PTT6=1;}//{PTT_PTT1=0;}//PORTF_INJO=0              //����������  //2012-2-7
    #define INJ2_OFF  1//{PTT_PTT7=0;}  //������2�ر�  
    #define INJ2_ON   1//{PTT_PTT7=1;}  //������2����
    
    //2017-11-28,ysl,�����������������ͱ�����
    #define P_O2H_ON    				M_O2Heat_C_ON//{PT0AD_PT0AD6 = 1;}
    #define P_O2H_OFF   				M_O2Heat_C_OFF//{PT0AD_PT0AD6 = 0;}
    #define P_O2H_ONOFF_TST    	M_O2Heat_C_ST//PT0AD_PT0AD6
    #define P_PUMPC_ON  				M_FeulBump_C_ON//{PTP_PTP7 = 1;}
    #define P_PUMPC_OFF 				M_FeulBump_C_OFF//{PTP_PTP7 = 0;}
    #define P_PUMPC_ONOFF_TST  	M_FeulBump_C_ST//PTP_PTP7
    
    //2017-10-24,ysl,���ĽǱ��ֵѡ��˿�
    #define CA_TH_U   1//PTJ_PTJ2=1;//PTJ_PTJ1=0;    
    #define CA_TH_L   1//PTJ_PTJ2=0;//PTJ_PTJ1=1;
    
    //2018-4-9,ysl,������߷�������Ӽ��1��2�˿�
    #define P_ISCVCOIL1_OPEN  1//PT0AD_PT0AD4
    #define P_ISCVCOIL2_OPEN  1//PT0AD_PT0AD5
 
	//20201126 CCPS̼�޵�ŷ��ܽ�
	 #define CCPS_ON     M_CTank_C_ON// {PTP_PTP6=1;} 
    #define CCPS_OFF      M_CTank_C_OFF//{PTP_PTP6=0;}
	#define CCPS_ONOFF_status  M_CTank_C_TOGGLE//PTP_PTP6    //20201209 chenwei add
        
    #define FAN_ON        // {PTP_PTP5=1;}//{PTGDD_PTGDD2=1;PTGD_PTGD2=1;}// PTE4
    #define FAN_OFF       // {PTP_PTP5=0;}//{PTGDD_PTGDD2=1;PTGD_PTGD2=0;}// PTE4

    //2017-12-11,ysl,���ĵ��·�ж�
    #define IS_IGNI_DRVHIGH !P_33814IGN1_OPEN//PT0AD_PT0AD5//2014-4-16 

    #define CA_HY_COME !PORTF_TDC_HYI          //�Ǳ�����Ƿ���
    
    //2017-12-8,ysl,����ԭ������������·���壬�������ڵ�33814��·����  
    #define IS_INJDRV_HIGH !P_33814INJ1_OPEN//PORTG_INJ_OPENI     //�������Ƿ�����Ϊ�ߵ�ƽ״̬
    #define IS_INJDRV_LOW  P_33814INJ1_OPEN//!PORTG_INJ_OPENI     //�������Ƿ�����Ϊ�͵�ƽ״̬
    //#define P_INJ1_OPEN P_33814INJ1_OPEN 

    #define P_TV3_VALID PORTE_INT_TV3          //T3�˿��Ƿ���Ч
  
    #ifdef DEBUG
        #define CDI_ON 1           
        #define CDI_CHARGE 1
    #else
        #define CDI_ON        {M_Ing_OFF/*PORTF_IGNIO=0;*/VF_ISCHARGE_IGN=false;}          //�������
        #define CDI_CHARGE    {M_Ing_ON/*PORTF_IGNIO=1;*/VF_ISCHARGE_IGN=true;}           //CDI���
    #endif
    //������ADC�˿ڶ��壭������������������������������������������������������������������������
//    #define DAT_ADC_10BITS ATDDR0

    #define CH_AD_TPS  			TPS_ch//1//ADC1SC1=0x00;
    #define CH_AD_OS   			O2Sensor_ch//4//ADC1SC1=0x01;
    #define CH_AD_TENG 			Teng_ch//3//ADC1SC1=0x03;
    #define CH_AD_TAIR 			Amb_ch//5//ADC1SC1=0x0A;
    #define CH_AD_CURRENT1 	9//ADC1SC1=0x0A;
    #define CH_AD_MAP 			FuelPosition_ch//6//2016-1-11 pin6, oil for UEGO
    #define CH_AD_VBAT 			Vbat_ch//7//ADC1SC1=0x08;  //18t
    #define CH_AD_PAIR 			Alt_ch//2//ADC1SC1=0x09; //����ѹ��//2014-10-28����from 0 to 2  
    #define CH_AD_CURRENT2 	0//ADC1SC1=0x0B;//CURRENT2
    #define CH_AD_TDC_OP 		8
    #define CH_AD_ISCVCOI1 	Motor1Curr_ch//10
    #define CH_AD_ISCVCOI2 	Motor2Curr_ch//11
    
    //���ͣ�6�����Ϳ�·��15�����·��13��
		#define FAD_1Channel			FAUN_ADS2S128

    //*******************************************************************************************
    //����Ǻ�AD�Ķ�����صġ�      
    extern word v_TPS_ad10bit,v_TPS_ad10bit_lst,v_TPS_ad10bit_lst2;
    extern word v_vbat_ad10bit,v_vbat_ad10bit_lst,v_vbat_ad10bit_lst2;
    extern word v_O2_ad10bit ,v_O2_ad10bit_lst ,v_O2_ad10bit_lst2 ;
    extern word v_pair_ad10bit,v_pair_ad10bit_lst,v_pair_ad10bit_lst2;
    extern word v_teng_ad10bit,v_teng_ad10bit_lst ,v_teng_ad10bit_lst2 ;
    extern word v_tair_ad10bit ,v_tair_ad10bit_lst,v_tair_ad10bit_lst2;
    extern word v_I1_ad10bit ,v_I1_ad10bit_lst,v_I1_ad10bit_lst2;
    extern word v_I2_ad10bit ,v_I2_ad10bit_lst ,v_I2_ad10bit_lst2;
    extern word v_TPS_ad10bit_bInj,v_TPS_ad10bit_bInj_lst,v_TPS_ad10bit_bInj_lst2;
    extern word v_vbat_ad10bit_bInj,v_vbat_ad10bit_bInjend;

    //2018-4-9,ysl,���ӵ������������������Ȧ���ɼ�
    extern word v_Coi1_ad10bit;//,v_Coi1_ad10bit_lst,v_Coi1_ad10bit_lst2;
    extern word v_Coi2_ad10bit;//,v_Coi2_ad10bit_lst,v_Coi2_ad10bit_lst2;
    
    
    
    //2015-9-23 ���ӵ���������ƶ˿�
        #define P_MOTOREN1 M_MotorEN1
        #define P_MOTOREN2 M_MotorEN2
        #define P_MOTORPH1 M_MotorPh1
        #define P_MOTORPH2 M_MotorPh2
        
        #define DC_MOTOR_ON   {M_MotorEN1_OFF;M_MotorEN2_OFF;}
        #define DC_MOTOR_OFF  {M_MotorEN1_ON;M_MotorEN2_ON;}
        


//-----eeprom�����˿ڣ�������������������������������������������������������������������������
//      #define PORT_SCL_H         PTCDD_PTCDD0=0
//      #define PORT_SDA_H         PTCDD_PTCDD1=0
//      #define PORT_SCL_L         PTCDD_PTCDD0=1
//      #define PORT_SDA_L         PTCDD_PTCDD1=1

//      #define P_SDA PTM_PTM3   
//      #define P_SCL PTM_PTM2  
//      
//      #define P_SCL_OUT  {DDRM_DDRM2=1;} //2013-5-22
//      
//      //#define P_SCLH  {PTCDD_PTCDD0=0;}
//      #define P_SCLH  {P_SCL=1;} //2013-5-22
//      
//      #define P_SDAH  {DDRM_DDRM3=0;}

//      //#define P_SCLL  {PTCDD_PTCDD0=1;P_SCL=0;}
//      #define P_SCLL  {P_SCL=0;}  //2013-5-22
//      
//      #define P_SDAL  {DDRM_DDRM3=1;P_SDA=0;}
    
#endif

//#define  FLASH_PROTECT_ALL    FPROT=0x7F;//2014-11-26 
//#define  FLASH_PROTECT_NONE   FPROT=0xFF;//2014-11-26 

//#define FLASH_PROTECT_LAST4K_TO_F000 FPROT=0xCF;//

//2015-5-24 ADD 
extern byte v_nb_OS_AD_CD1;
extern byte v_nb_OS_AD_CD2;
extern byte v_nb_OS_AD_CD3;

extern byte v_TPCB;//2016-1-9



