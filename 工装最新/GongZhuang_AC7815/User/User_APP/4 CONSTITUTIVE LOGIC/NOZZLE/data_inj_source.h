/*-------���ļ��ǶԶ�������ģ���map������----*/
/*��Ӧ��map������data_inj_source.c��*/
#include "add.h"
#include "public.h"


//����������������ĸmap��������������������
extern const word M0_INJ_SLOP_MAP;// @          ADD_M0_INJ_SLOP_MAP ;
extern const word M0_INJ_SHIFT_MAP;//  @         ADD_M0_INJ_SHIFT_MAP ;
extern const word M0_TW1_CO_T35;//  @            ADD_M0_TW1_CO_T35    ;
extern const word M0_TV3_T1EQ3D5_0 ;// @         ADD_M0_TV3_T1EQ3D5_0  ;
extern const word M0_TV3_T13D5MS_SET;//  @       ADD_M0_TV3_T13D5MS_SET ;
extern const word M0_TV3_T13D5MS_INT;//2017-2-23 ����T30��ʼֵ
extern const word M0_SUM_TV3_TEMP_SET ;// @      ADD_M0_SUM_TV3_TEMP_SET ;
extern const word M0_KT3_MAP   ;//  @            ADD_M0_KT3_MAP       ;
extern const word M0_DT30_MAP  ;//  @            ADD_M0_DT30_MAP     ;
extern const byte M0_VBAT_ST  ;//       @        ADD_M0_VBAT_ST;              //2008-8-5 ��������׼��ص�ѹ

//extern const word M1_BE_TW3[97]	@		ADD_M1_BE_TW3; 		//2009-12-8 delete
extern const word M1_DT3_Q[29] ;// @ ADD_M1_DT3_Q;
extern const byte M0_PROG_NUM_HL_INJ;               //#define M0_PROG_NUM_HL_INJ 0xA0//	;2008-3-5 0x90	;THE BIT4`BIT7 OF PRO_NUM_HL define the injector type,2008-3-5M8��׼����
extern const byte M0_NUMBER_T1;//#define M0_NUMBER_T1 	0x58//	;2008-3-21��ʼ ר�����ڴ�����������ϸС�����޸�
extern const byte M0_TYPE_INJCTOR;//#define M0_TYPE_INJCTOR 8	//;2008-2-22 ��������ΪM8:M7��T3 ,t30=2.15,kt3=1.144,t10=4.529,kt1=1.276
extern const word M0_MAX_TINJ;//#define M0_MAX_TINJ 0xe00//
extern const word M0_MIN_TINJ;//#define M0_MIN_TINJ	0x300//EQU	03H	;IF	V_FMF_H<MIN_TINJ,LET	MAP_TINJ=0300H		
extern const byte M0_INJ_BIG_PCAH;// ��������ת�ٸ���3900ʱ�����T1�����Ƶ�ϵ����#define M0_INJ_BIG_PCAH	2//EQU	2	;WHEN	RPM>3907,	INJ	PULSE	CAN	BE	MORE	THAN	PCA	BY	2.048MS		
extern const byte M0_INJ_SHIFTH_REF;//#define M0_INJ_SHIFTH_REF	4//EQU	4	;INJ_SHIFT_REF=0X43E=4.344MS												
extern const byte M0_INJ_SHIFTL_REF;//#define M0_INJ_SHIFTL_REF	0x3e//EQU	3EH													


//2013-7-10 ADD 
extern const byte M2_NC_V[7][2];//2013-7-10
extern const byte M2_T1MAXD8_V[7][2]; //2013-7-10
/*--The following are deleted macro--------------*/
//delete//#define MTI_DY_JG 4//      EQU     4       ;4*2*4us=32us as min_order of ti 
//#define INJ_SLOPH_REF	2//EQU	2	;THE	SLOP	OF	REF_INJ	IS	11							
//#define INJ_SLOPL_REF	0//EQU	0				

/*====2014-12-5�¶���===========================================================================*/


 #if M0_MFUD1==0 && M0_MFUD3==0 && M0_MFUD4==0 && M0_MFUD6==0 && M0_MFUD7==0
            

      #define M0_QMAX	20000	//	2020-5-19 ��ʱ����֣�QMAX�趨һ���ϴ����ֵ

//----------------------------------2020-5-19 �������ã�����������-----------------------------

	  #define 	aa10_8	-25 
#define 	ba10_8	-1396 
#define 	ca10_8	4413 
#define 	ab_10_6	-560 
#define 	bb_10_6	13091 
#define 	cb_10_6	-34234 
#define 	ac_10_0	693 
#define 	bc_10_0	-10576 
#define 	cc_10_0	34171 
		
#define 	KD13M128_CO_2_M1024	13
#define 	KD13M128_CO_1_M100	-649
#define 	KD13M128_CO_0_M1	961//936//961
		
#define 	DM250_CO_1_M10	-55
#define 	DM250_CO_0	2130//2180
		
#define 	DT3c_M100	78
		
#define 	CO_G1_M8192	19
#define 	CO_G2_M250	-437





       //3.5V ��С����������ͬ
        //��׼DOP������ //2010-3-8ע�⣺�ò����Ķ�ʱ����Ҫ��sys_ini�еĵ�һ��Ķ�
        #define m_DOMA  472    //a*-1
        #define m_DOMB  8527   //b
        #define m_DOMC  455    //c*-1
        #define m_DOMCM2  910    //c*-1
        //	T1D0=0.03143*(VBAT*0.0625+0.9)**2-1.20457*(VBAT*0.0625+0.9)+15.07923
        //	T1D0_4US/250=0.03143*0.0625*0.0625*VBAT*VBAT+0.03143*2*0.0625*VBAT*0.9+0.03143*0.9*0.9-1.20457*0.0625*VBAT-1.20457*0.9+15.07923
        //	T1D0_4US=M0_T1D0_2*VBAT**2+M0_T1D0_1*VBAT+M0_T1D0_0
        //	
        //	
        #define	M0_T1D0_2_M256M128	1005	//	0.03143		VBAT			
        #define	M0_T1D0_1_M256	-4592	//	-1.20457		202	T1D0_4US	1133.231079	
        #define	M0_T1D0_0_M256	897316	//	15.07923		13.525	T1D	4.536773144	1134.193286

        #define M0_CHA_TO_DEC_20US  50//2010-7-8 FROM 15 TO 50
        
        #define M0_BE_CHA2  0
        
        #define T3_DOM_ULIMIT  800
	  
	  #define	M0_T1D0_2_M256M128	1005	//	0.03143		VBAT			
        #define	M0_T1D0_1_M256	-4592	//	-1.20457		202	T1D0_4US	1133.231079	
        #define	M0_T1D0_0_M256	897316	//	15.07923		13.525	T1D	4.536773144	1134.193286

        #define M0_CHA_TO_DEC_20US  50//2010-7-8 FROM 15 TO 50
        
        #define M0_BE_CHA2  0
        
        #define T3_DOM_ULIMIT  800
    //#endif

//for low voltage
    //2012-10-9��Ե͵�ѹ�Ĵ�ʩ			//2012-10-9 ��ʱ		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1���ֵ
    #define M0_T1MAX_4US	2100	//us	8000		//T1���ֵ 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	�͵�ѹʱÿ������ֵ��Ӧ��T1���ֵ������
    		//8Vʱ���T1	2512		
    //#define M0_QMAX	310	//	18		�������ֵ
    //#define M0_QMAX	400	//2014-6-17 35UL/PULSE
     
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	�͵�ѹʱÿ������ֵ��Ӧ���������ֵ�ļ���
    		//8Vʱ,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8Vʱ��DT3��Ҫ��	128	0.512	
    #define M0_DT1_4US	15	//ÿ��1V��1000	1000		
    		//8V,T1��Ҫ����	480	1.92	
    #define M0_V_LOWTH	145	//10V	��ѹ�����ޣ��������ֵ�����͵�ѹ�ƣ�		
    #define M0_VMIN	123	//8V	��ѹ��Сֵ�����ڸõ�ѹʱ�����õ�ѹ����		
    #define M0_T1_T35 750        //��λΪ4us //3MS

    //2015-5-20
    //  #define M0_FMF_PUMP     50
    //  #define M0_NB_PUMP_FAIL 80
    
    
    //2015-10-12 AT DATA_INJ_SOURCE.C 310901
      #define M0_FMF_PUMP     5
      #define M0_NB_PUMP_FAIL 10
      
      
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1ɨ���õĳ�ʼֵ
	  
	  

#endif


