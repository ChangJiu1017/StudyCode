/*HEAD			
 ***********************************************************************			
 * Filename	:map_PNX_Motor.c		
 * Function	:parameter relative to vehicle type.		
 * User		:xielian	
 * Date		:2017.1.20	
 * Version	:V8.1.1		
 *			
 *** History ***			
 * V8.1.0	2017.1.20	xielian 	
 *		Base. 
 * V8.2.0   2018.9.26	xielian
 *		�������Ԥ�����������ĵ���
 ***********************************************************************			
HEAD END*/

#include "public.h"
#include "add.h"

#include "map_selection.h"//2013-4-16

const byte MO_STEP_FMF_IDLE_RICHTOLEAN= 2;//4 ;        //���ٷ�������Ũ��ϡ����Ҫ���ӵ���������λ��ͬFMF 
const byte MO_STEP_FMF_IDLE_LEANTORICH= 2;//4 ;     //���ٷ�������ϡ��Ũ����Ҫ��������������λ��ͬFMF



/*--��ģ����ص�map----------------------------*/

     const word   M0_MS_RPM_DOWN  =           0;//���ټ���ʱ��REMARK������������ʱ��λ����ת�ٵ����Ʒ�ΧΪ1350-1550;
                                        //�����ڲ�ͬ���ͣ���Ҫ�����ĵ��ٷ�Χ��ͬ������ͨ��ʵ��ֵ����ֵ�����֣� 
    // #ifndef SYS_TRADITIONAL
	// const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     //#else
	 const word   INJ_REACH_BMS =     0x03e8;//  ����ϵͳ������Чʱ��Լ1mm��ȼ�ͷ���ʱ��Լ30mm/ms
	 //#endif
	 const byte   PRE_OIL_INC  =           50;//Ԥ���������������4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	30;//�뿪����ʱ�м��ͺͼ�����ԣ���ֹ���������뿪���ٵ�ʱ������̣���������֮�����С���ΪWAIT_RUN_STARTB������Ȧ			
                                              //2010-1-11 from 40 to 10
                                              //2012-12-7 FROM 10 TO 20���������Ź�Ũ
    //����������for idle missfire������������������������������������������
     const byte   MAX_TI_INC_DRPM	=		40;//25;//ʧ��������������������4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		12;//���ۼ�ʧ���������ڸ�ֵʱ��ֻ�Խ�����ʱʧ������//equ		12		;2007-7-17����ʱ���ۼ�ʧ�����48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//��Ȧ��ת���½�>=93 (6*15.625),��O2dec,��Ϊϡ,��4US�����٣�15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//��Ȧ��ת���½�>=109 (7*15.625),��Ϊʧ�� �����٣�15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//����ѭ�����½�>=156��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//������ѭ�����½�>=187��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   STEP_TI_INC_AMISF=	3;//2009-12-14 	1;//2008-5-30 3//һ��ʧ���������,������4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=	2;//2009-12-14	1;//2008-5-30 2//һ��ȼ�ղ��õ����� ������4US;
     const byte M0_IGN_MAX_L6000RPM=50;							//35		��ת��ʱ,�Ե��ٵ��ǵ�����							
     const byte M0_IGN_MAX_GT6000RPM=57;							//40		57.14285714							
     //const byte M0_CO_LSPEED_TO16=12;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;
     //const byte M0_CO_LSPEED_TO16=16;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
     //const byte M0_CO_LSPEED_TO16=2;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
     const byte M0_CO_LSPEED_TO16=8;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi
                                   //2013-1-27 BACKTO 8
    

    const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;��������Ũ,���͵�����Ȧ��,����??6Ȧ,����һ��

/*
//2010-3-17 ADD 
#if(MV_VEHICLE== WILD)
    const word   M0_MS_RPM_DOWN  =           0;//���ټ���ʱ��REMARK������������ʱ��λ����ת�ٵ����Ʒ�ΧΪ1350-1550;
                                        //�����ڲ�ͬ���ͣ���Ҫ�����ĵ��ٷ�Χ��ͬ������ͨ��ʵ��ֵ����ֵ�����֣� 
    //const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
    #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	const byte   PRE_OIL_INC  =           50;//Ԥ���������������4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
    const byte WAIT_RUN_STARTB  =      40;//�뿪����ʱ�м��ͺͼ�����ԣ���ֹ���������뿪���ٵ�ʱ������̣���������֮�����С���ΪWAIT_RUN_STARTB������Ȧ			
                                              //2010-1-11 from 40 to 10
                                              //2010-6-4 chang from 10 to 40
    //����������for idle missfire������������������������������������������
    const byte MAX_TI_INC_DRPM	=		40;//25;//ʧ��������������������4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
    const byte MAX_MISS_FIRE_DTI=		12;//���ۼ�ʧ���������ڸ�ֵʱ��ֻ�Խ�����ʱʧ������//equ		12		;2007-7-17����ʱ���ۼ�ʧ�����48US
    const byte RPM_DEC_FOR_LEANS=	10;//	9;//2008-5-30 change from 5 to 8//��Ȧ��ת���½�>=93 (6*15.625),��O2dec,��Ϊϡ,��4US�����٣�15.6rpm
                                       
    const byte RPM_DEC_FOR_MISFIRE	=12;//	11;//2008-5-30 7//��Ȧ��ת���½�>=109 (7*15.625),��Ϊʧ�� �����٣�15.6rpm
    const byte RPM_DEC_2C_MISSFIRE=	15;//	14;//2008-5-30 11//����ѭ�����½�>=156��Ϊȼ�ղ��ã����٣�15.6rpm
    const byte RPM_DEC_3C_MISSFIRE	=17;//	16;//2008-5-30 13//������ѭ�����½�>=187��Ϊȼ�ղ��ã����٣�15.6rpm
    const byte STEP_TI_INC_AMISF=	3;//2009-12-14 	1;//2008-5-30 3//һ��ʧ���������,������4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
    const byte STEP_A_SMALL_FIRE=	2;//2009-12-14	1;//2008-5-30 2//һ��ȼ�ղ��õ����� ������4US;
    const byte M0_IGN_MAX_L6000RPM=50;							//35		��ת��ʱ,�Ե��ٵ��ǵ�����							
    const byte M0_IGN_MAX_GT6000RPM=57;							//40		57.14285714							
     //const byte M0_CO_LSPEED_TO16=12;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;
    const byte M0_CO_LSPEED_TO16=8;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;    //2009-12-20 modi

    
    const byte M0_CCL_DERICH_CNTRICH_IDLE  =     24;//;��������Ũ,���͵�����Ȧ��,����??6Ȧ,����һ��
                                                    //2010-7-10 from 24 to 48����2010-7-30 from 48 to 24

#endif

#if(MV_VEHICLE== CUB)
     const word   M0_MS_RPM_DOWN  =           0;//���ټ���ʱ��REMARK������������ʱ��λ����ת�ٵ����Ʒ�ΧΪ1350-1550;
                                        //�����ڲ�ͬ���ͣ���Ҫ�����ĵ��ٷ�Χ��ͬ������ͨ��ʵ��ֵ����ֵ�����֣� 
     //const word   INJ_REACH_BMS =     0x0753;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	 const byte   PRE_OIL_INC  =           50;//Ԥ���������������4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE=             72;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	40;//�뿪����ʱ�м��ͺͼ�����ԣ���ֹ���������뿪���ٵ�ʱ������̣���������֮�����С���ΪWAIT_RUN_STARTB������Ȧ			

    //����������for idle missfire������������������������������������������
     const byte   MAX_TI_INC_DRPM	=		15;//ʧ��������������������4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		8;//���ۼ�ʧ���������ڸ�ֵʱ��ֻ�Խ�����ʱʧ������//equ		12		;2007-7-17����ʱ���ۼ�ʧ�����48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//��Ȧ��ת���½�>=93 (6*15.625),��O2dec,��Ϊϡ,��4US�����٣�15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//��Ȧ��ת���½�>=109 (7*15.625),��Ϊʧ�� �����٣�15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//����ѭ�����½�>=156��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//������ѭ�����½�>=187��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   STEP_TI_INC_AMISF=		1;//2008-5-30 3//һ��ʧ���������,������4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=		1;//2008-5-30 2//һ��ȼ�ղ��õ����� ������4US;
    const byte M0_IGN_MAX_L6000RPM=43;								//30			��ת��ʱ,�Ե��ٵ��ǵ�����							
    const byte M0_IGN_MAX_GT6000RPM=50;								//35										
    const byte M0_CO_LSPEED_TO16=8;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;

     const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;��������Ũ,���͵�����Ȧ��,����??6Ȧ,����һ��

#endif

#if(MV_VEHICLE== SCOOTER)
     const word   M0_MS_RPM_DOWN  =           0x200;//���ټ���ʱ��REMARK������������ʱ��λ����ת�ٵ����Ʒ�ΧΪ1350-1550;
                                        //�����ڲ�ͬ���ͣ���Ҫ�����ĵ��ٷ�Χ��ͬ������ͨ��ʵ��ֵ����ֵ�����֣� 
     //const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     #ifndef SYS_TRADITIONAL
	 const word   INJ_REACH_BMS =     0x08ca;//����������ʼ��T1��ȼ�͵��������֮���ʱ�� 
     #else
	 const word   INJ_REACH_BMS =     0x03e8;
	 #endif
	 const byte   PRE_OIL_INC  =           50;//Ԥ���������������4US       EQU     50 ;20
     //const byte   M0_FMF_IDLE    =             72;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     //const byte   M0_FMF_IDLE    =             90;//��������������4US           EQU     94      ;the ti at ilde with jqm_oder=0.2/1500rpm is 86*0.004+4.344=4.
     const byte   WAIT_RUN_STARTB  =      	30;//�뿪����ʱ�м��ͺͼ�����ԣ���ֹ���������뿪���ٵ�ʱ������̣���������֮�����С���ΪWAIT_RUN_STARTB������Ȧ			
    
    //����������for idle missfire������������������������������������������
     const byte   MAX_TI_INC_DRPM	=		25;//ʧ��������������������4US;equ		25		;the max ti inc for drpm is 0.1ms, misfire
     const byte   MAX_MISS_FIRE_DTI=		12;//���ۼ�ʧ���������ڸ�ֵʱ��ֻ�Խ�����ʱʧ������//equ		12		;2007-7-17����ʱ���ۼ�ʧ�����48US
     const byte   RPM_DEC_FOR_LEANS=		8;//2008-5-30 change from 5 to 8//��Ȧ��ת���½�>=93 (6*15.625),��O2dec,��Ϊϡ,��4US�����٣�15.6rpm
     const byte   RPM_DEC_FOR_MISFIRE	=	10;//2008-5-30 7//��Ȧ��ת���½�>=109 (7*15.625),��Ϊʧ�� �����٣�15.6rpm
     const byte   RPM_DEC_2C_MISSFIRE=		13;//2008-5-30 11//����ѭ�����½�>=156��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   RPM_DEC_3C_MISSFIRE	=	15;//2008-5-30 13//������ѭ�����½�>=187��Ϊȼ�ղ��ã����٣�15.6rpm
     const byte   STEP_TI_INC_AMISF=		1;//2008-5-30 3//һ��ʧ���������,������4US; vc_TW1_IDLEMISSF inc by step_ti_inc_amisf each misfire, 
     const byte   STEP_A_SMALL_FIRE=		1;//2008-5-30 2//һ��ȼ�ղ��õ����� ������4US;
    const byte M0_IGN_MAX_L6000RPM=43;								//30			��ת��ʱ,�Ե��ٵ��ǵ�����							
    const byte M0_IGN_MAX_GT6000RPM=50;								//35										
    const byte M0_CO_LSPEED_TO16=8;// ������������=((-ת��+1500)/15.6)*M0_CO_LSPEED_TO16/16;

     const byte M0_CCL_DERICH_CNTRICH_IDLE  =     48;//;��������Ũ,���͵�����Ȧ��,����??6Ȧ,����һ��

#endif
*/
//--for cf������������������������
const byte M0_MIN_STOPOIL_RUN=7;//���ۼƶ���Ȧ��������ֵʱ������ҪCI����?	
const byte M0_INJ_AG_INC =             75;//CI�͸��뿪����ʱ������Ȧ�������Ļ���ֵ������4US;equ	75	;the first inj after stpoil inc oil by75*4us
//const byte M0_ACCELDT_MAX=	           150;//������������ʱ��v_Ka_b1���˵�����ϵ���ܺ͵����ֵ��	
const byte M0_ACCELDT_MAX=	           250;//������������ʱ��v_Ka_b1���˵�����ϵ���ܺ͵����ֵ��	
const byte M0_MAX_RPM_AQ=  118;//�����Զ��������ܵ�ת�����ޣ�����15.6RPM��ת��Ϊ1843
const word M0_TIME_TO_CDI_H = 0x0032 ;      //CDI�ŵ�֮�󣬵��´γ��֮���ʱ��;;0.8MS����λ:4US
//const word M0_TIME_TO_CDI_H = 500 ;      //CDI�ŵ�֮�󣬵��´γ��֮���ʱ��;;0.8MS����λ:4US
const byte  M0_MAX_RPM_CONIDLE= 116;         //���ٿ��Ƶ����ת�٣���λ15.6RPM// equ	106	;when the average of rpm156 over 16 runs>=106,clear conti_idle_time
const byte  M0_MIN_RPM_CONIDLE=	77;          //���ٿ��Ƶ����ת�٣���λ15.6RPM//equ	77	;1203 rpm 2004.10.28;82 ;03.9.1 ;77	;03.8.22;71	;when the v_rev_156ave<which,1109rpm, clear conti_idle_time 
const byte  M0_NBTDCO_USED_IGN=	50;          //�����ŵ���Ƕ�С�ڸ�ֵ��ת�ٸ���PCAH2929_IGNʱ��һ��⵽�Ǳ�͵�𣬵�λ0.7��CA?

const byte  M0_RZT_HOT_NOOIL=		72;  //	equ		72		;��RZT>=72,��Ϊ�ȳ�,���ٶ���ת�ٲ��ټ�С�����٣����¶Ȳ���ֵһ��
const byte  M0_RPM_DEC_REOIL	=	20 ; //	equ		20		;�����ٹ���ת�ٵļ�С��Ϊ(20-(72-v_Teng_inuse)/4),�ȳ���Ϊ<2250�����٣����¶Ȳ���ֵһ��
//const byte  M0_RPM_NOOIL_DEC_TOILDE= 18; //	equ		18		;TOIDLEʱ�Ķ���ת�ټ�С��(18-(72-v_Teng_inuse)/4), �ȳ������Ϊ>=2375�����٣����¶Ȳ���ֵһ��
const byte  M0_RPM_NOOIL_DEC_TOILDE= 8; //	equ		18		;TOIDLEʱ�Ķ���ת�ټ�С��(18-(72-v_Teng_inuse)/4), �ȳ������Ϊ>=2375�����٣����¶Ȳ���ֵһ��
                  //2013-1-12 FROM 18 TO 8
//const byte  M0_IDLE_STOP_SP= 0x75  ;     //��STARTS�׶ε���͹���ת��	EQU	075H	//;	when	v_prd_cas_h>=75h:rpm<500	not	inj	when	if	motor	started
//const byte  M0_IDLE_STOP_SP= 0xFE  ;     //��STARTS�׶ε���͹���ת��	EQU	075H	//;	when	v_prd_cas_h>=75h:rpm<500	not	inj	when	if	motor	started
                               // 2012-5-29

//2015-12-6 ��M0_IDLE_STOP_SP�Ļ�0x75 ����Ϊԭ���ᵼ��ͨ��ͣ���˻�
const byte  M0_IDLE_STOP_SP= 0x75  ;


//const byte  M0_START_SPEED  =  0xD1;     //STARTS�׶ε���͹���ת��
const byte  M0_START_SPEED  =  0xFE;     //STARTS�׶ε���͹���ת��
const byte  M0_MAX_START_RPM = 0x06;     //�ж������Ǳ�ʱ�����ܴﵽ���������ת��?
const byte  M0_JQM_DRY_IGTOR= 15 ;       //���������Զ�Ӧ�����TPS���𣬼�������NOT MMOVEʱ���ж����ż�����ڵ��ڸ�ֵʱ��������EQU	15	;WHEN	JQM>=15,	FOR	DRY	INGITOR

#ifdef TE_10K						//20210720 chenwei add 

const byte  M0_TES_MAX=	247     ;  //2019-9-26 ��������Ҫ��� 235      //����ͷ�¶ȴ���������ֵ���ڸ�ֵ����Ϊ�������й��ϣ�����ͣ��//EQU	235	;WHEN	V_TENG_INUSE>M0_TES_MAX,	STOP	OIL	NOT	CONSIDER	THE	MOTOR	STATUS				
#endif

#ifdef TE_2K
const byte  M0_TES_MAX=	253  	;	//20210720 chenwei add 2Kˮ�´�������ֵ��Ϊ253

#endif


const byte  M0_TES_FOR_IDLENOTTOOLONG=	210;         //����ͷ�¶ȴ���������ֵ���ڸ�ֵ����������ʱ�䳬��һ��ֵ֮��ͣ��//EQU	200	;2004.6.22,V_TENG_INUSE>=200	AND	RUNNING	IN	IDLE	FOR	TWO	MINITE	STOP	INJ			
                                        //2008-11-7 CHANGE FROM 200 TO 210
//2008-11-2 move here
const byte  M0_MIN_RZT_TI_SH=	50;//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND=0,	NO	TI_SHIFT	GET	
const byte  M0_MAX_RZT_TI_SH=	192;//EQU	50	;WHEN	v_Teng_inuse<M0_MIN_RZT_TI_SH,	LET	TEMP_SECOND=0,	NO	TI_SHIFT	GET	
//����for tmp
const byte  M0_TAS_MAX=	            90;//�����¶ȴ���������ֵ�������ֵ�����ڸ�ֵ�ж�Ϊ�й��ϣ�EQU	90	;WHEN	AIR	TEMPERATURE	MEASURED	>63DEGREE,	SNSOR	ERRO		
const byte  M0_TAS_MIN=	            2;//�����¶ȴ���������ֵ�������ֵ��С�ڸ�ֵ�ж�Ϊ�й��ϣ�//EQU	2		
const byte  M0_TES_MIN=	            1;//ů��״̬�£���ͷ�¶ȴ���������С��ֵ��С�ڸ�ֵ�ж�Ϊ�й��ϣ�/EQU	1	;-30DC,IF	V_TENG_INUSE<M0_TES_MIN	OR	V_TENG_INUSE>RZT_MAX	FOR	15	TIMES,		
const byte  M0_TAS_MAX_INUSE	=       60;//�����¶ȴ���������ֵ�������ֵ��ʹ��ʱ�����ֵ�������ڸ�ֵ������ֵʹ�ã�EQU	60	;2005.6.1	THE	MAX	OF	V_TAIR_INUSE	IS	45DEGREE	FOR	V_TAIR_AD	SAMPLING		
const byte  M0_TAS_MAX_PREDICTED	=   50;//�����¶ȴ�����Ԥ��ֵֵ�������ֵ��EQU	50	;2004.9.20,37	DEGREE	;45	;2004.8.29	34	DEGREE	;60	47	DEGREE		
const byte  MO_TES_LLIMITFORPUMP;  //130   //��TES���ڸ��¶�ֵ���ҷ���������ʱ��̣ܶ�����������û������ͣ��ʱ������Ҫ���� ��     

const byte M0_PCAH_DOWNRPM   =   29;  //;2007-4-9 when rpm<2020,down the rpm to pc by some value  ,����ͬPCAH
const byte  M0_TENG_LT_GUOSAN       =    25;//��������ʱ���¶��������

/*----------��������������-----------------------------------------*/
//������DTFIһ��,��Q*250/11;


const word M0_FMF_PREINJ=128;   //����Ԥ������ :						5.632	


const word M0_FMF_1ST_BASIC=128;//������һȦ�Ļ�������	
const byte M0_TES_FOR_1STRUN_FMF_CORRECTION=60;////27,������ʱ,�¶ȵ��ڸ�ֵʱ,��Ҫ��������,����������ΪM0_TES_FOR_1STRUN_FMF_CORRECTION-teng	
                                              //2009-12-10 change from 27 to 60
/*��������ֹʱ��һЩ����*/
const word  M0_T_DELAYED_AFTER_KEYOFF_S=500  ;  //ECU��Կ�׹رպ��M0_T_DELAYED_AFTER_KEYOFF_S��ʱ,��ȫ�رյ�Դ
                                      //201282 CHANGE FROM 3600 TO 500
const word  M0_T_INTERVAL_COM_WHENSTOP_4US= 5000 ; //��λ:4us,������δ����ʱ,ͨѶ���ٶ�;��������֮���ʱ����;
const word  M0_T_KEYOFF_SHORTLY_S= 1800   ;       //��λ:s,Կ�׹ر�ʱ��С�ڸ�ֵʱ,�� VF_OFFSHORT_KEYλ

/*--os�ջ����Ʋ���-----*/
 const byte MO_STEP_FMF_IDLELEAN= 1    ;     //���ٷ���������ϡʱ�����ڵ�����������λ��ͬFMF	
 //const byte MO_STEP_FMF_IDLE_RICHTOLEAN= 4 ;        //���ٷ�������Ũ��ϡ����Ҫ���ӵ���������λ��ͬFMF 
 //2011-3-17 4 to 6
 
 
 const byte MO_FMF_RICHLIMIT_WOT_NOTFK=	4 ;      //����ȫ������������ʱ����������ļ��������෴���������Ҫ���ǡ�=0
 //const byte MO_FMF_CLS_INIT=5   ;       //�����󣬵��ٷ��������ĳ�ʼֵ
 //const byte MO_FMF_CLS_INIT=0   ;       //�����󣬵��ٷ��������ĳ�ʼֵ
 //const byte MO_FMF_CLS_INIT=5   ;       //�����󣬵��ٷ��������ĳ�ʼֵ
 const byte MO_FMF_CLS_INIT=0   ;       //�����󣬵��ٷ��������ĳ�ʼֵ //2015-12-15 FROM 5 TO 0
      //2013-3-1 FROM 0 TO 5
      //2013-9-16 from 5 to 0
      //2014-1-11 FROM 0 TO 5
 const byte MO_FMFENRICH_BACKTOIDLE_CLS    = 2   ;    //2008-11-13 ADD 
 const byte MO_DFMF_LEANTOOLONG_WHENIDLE= 25;//����1s�ڣ����ٷ���������֮ǰ��ƽ��ֵ�߳���ֵ������OSһ��ָʾƫϡ������ΪOS�����⣻ 
 const byte MO_DFMF_LIMIT_LEANTOOLONG_WHENIDLE= 4  ; //2008-11-14 ��������������ʱ���Ե���������ֵ������ֵ
 const byte MO_SUBDFMF_LIMIT_LEANTOOLONG_WHENIDLE= 10 ;  //2008-11-14 ��������������ʱ���Ե��ټ�����ֵ������ֵ

 const byte M0_CCL_ENRICH_CNTLEAN_IDLE =      16;//;��������ϡ,���͵�����Ȧ��,����?6Ȧ,��һ����
 const byte M0_CCL_ENRICH_CNTLEAN_IDLE_HSP=        24;//;����ת�ٸ�ʱ����ϡ,�����͵�Ȧ��
 const byte M0_CCL_ENRICH_CNTLEAN_IDLE_CI=         16;//;����ʱ,�ٹ��ͽ���ǰ,����ϡ,��Ҫ���͵�Ȧ��,
 const byte MO_STEP_FMF_NOTIDLE    = 2 ;          //�Ǽ����ڼ�,�ǵ��ٷ���ʱ��ϡŨ�仯ʱ������������ֵ��
                                                //2009-9-8 change from 4 to 3;
 const byte MO_STEP_FMF_ACCEL  =  2;//5    ;       //�����ڼ䣬ϡŨ�仯ʱ������������ֵ��
 const byte MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST =  6 ;  //������������������Ϊϡ�Ĵ���ʱ����Ҫ���������ٲ������������ָ�����
                                                //��nb_o2change_afteridletoofast��>MO_NB_O2CHANGELIMIT_AFTERIDLETOOFAST֮��ָ�����
 const byte MO_DFMF_LIMIT_WHENWOT_CHANGELITTLE  = 20 ;  //VF_OSCHANGELITTLE_CLS=TRUE������ȫ��ʱ���������ֵ;//2008-11-17
 const sbyte MO_DFMF_LLIMIT_WHEN_ACCEL=-15; //����ʱ���Լ������������ƣ�
/*--������Ȧ�Ĳ���------*/
const byte M0_TES_ADDINJ_ENRICH=50; //����Ȧ�������¶������������¶ȵ��ڸ�ֵʱ���������ͣ�������Ϊ�� M0_TES_ADDINJ_ENRICH-TES����λ��ͬFMF
const byte M0_INJPHASE_FORIDLE=8;         //2008-12-3   //����������λ
const byte M0_NB_ADDINJ_JCI=1;       //�ջָ�����ʱ������Ĵ�����

/*------ISCV����-------*/
const byte  M0_TENG_L_ISCV =27 ;    //2008-11-4   ISCV��ʱ��ů�������������¶�
const byte  M0_D1S_ISCVON_BASE= 50 ;//2008-11-4   ISCV����ʱ��Ļ���ֵ
const byte  MO_TENG_U_ISCV= 33  ;   //2008-11-4   ����ISCV��ʱ�������¶�
const byte M0_TES_U_FOR_CI_ISCVON=110;//���¶ȵ��ڸ�ֵʱ��������ʱ��Ҫ�򿪵�����߷�

/*-----���ٲ���-------*/
const byte M0_TPS_IDLE_L=5;// 7	//���ٵ�������ż��𣬶�Ӧ���ż���Ϊ��"0.NM",���У�N=ȡ����M0_TPS_IDLE_L/4����M=M0_TPS_IDLE_L MOD 4

/*--����ϵͳ��ʼ������-----------*/
const byte M0_T_KEYON_FOR_INI_S=1;     //    3 //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��ÿ��Կ�׿�������ʱ�����ƣ�
//const byte M0_T_KEYOFF_FOR_INI_S=1;    //  3  //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��ÿ��Կ�׹رպ���ʱ�����ƣ� 
const byte M0_T_KEYOFF_FOR_INI_S=2;    //  3  //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��ÿ��Կ�׹رպ���ʱ�����ƣ�   //2016-7-30

const byte M0_NB_KEYONOFF_FOR_INI=8;   //ϵͳ��Ҫ�����ʼ��״̬����Ӧ��Կ�׷������صĴ��� 
const byte M0_TES_ULIMIT_FOR_INI=180;//��ʼ��ʱ����ͷ�¶�������ޣ�
const byte M0_REV15D6_LLIMIT_FOR_FULLAQ=94;//��ɨʱ�������ת�ٸ��ڸ�ֵ1480rpm�����ж�����

