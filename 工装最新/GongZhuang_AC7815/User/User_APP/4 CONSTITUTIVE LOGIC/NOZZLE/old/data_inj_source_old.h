/*-------本文件是对动力喷嘴模块的map的声明----*/
/*相应的map定义在data_inj_source.c中*/
#include "add.h"
#include "public.h"



//2014-10-31备注：临时实验用，与量产无关；
 #if M0_MFUD1==INJ109_4 // NOZZLE 58，限流0.3

        //2013-7-28 硬管
        //#if INJ1109==INJ1109_INT
            #define 	aa10_8	743 
            #define 	ba10_8	-5156 
            #define 	ca10_8	8247 
            #define 	ab_10_6	-4972 
            #define 	bb_10_6	33128 
            #define 	cb_10_6	-51745 
            #define 	ac_10_0	3517 
            #define 	bc_10_0	-21836 
            #define 	cc_10_0	41290 
            		
            #define 	KD13M128_CO_2_M1024	7
            #define 	KD13M128_CO_1_M100	-423
            #define 	KD13M128_CO_0_M1	862
            		
            #define 	DM250_CO_1_M10	-39
            #define 	DM250_CO_0	1659
            		
            #define 	DT3c_M100	77
            		
            #define 	CO_G1_M8192	16
            #define 	CO_G2_M250	-419
        //#endif
            
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
            #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif


//2014-10-31备注：临时实验用，于量产无关
 #if M0_MFUD1==INJ209_3//2014-8-27
    
            //T1-DT3斜率
            #define 	aa10_8	177 
            #define 	ba10_8	-1984 
            #define 	ca10_8	4356 
            #define 	ab_10_6	-1518 
            #define 	bb_10_6	13928 
            #define 	cb_10_6	-28123 
            #define 	ac_10_0	976 
            #define 	bc_10_0	-7311 
            #define 	cc_10_0	21837 
            		
            #define 	KD13M128_CO_2_M1024	0
            #define 	KD13M128_CO_1_M100	-77
            #define 	KD13M128_CO_0_M1	388
            		
            #define 	DM250_CO_1_M10	-49
            #define 	DM250_CO_0	1902
            		
            #define 	DT3c_M100	64
            		
            #define 	CO_G1_M8192	15
            #define 	CO_G2_M250	-411


    //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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

    //for low voltage
        //2012-10-9针对低电压的措施					
        #define M0_T1MAX_4US	2000	//us	8000		//T1最大值
        #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
        		//8V时最大T1	2512		
        //#define M0_QMAX	409	//	18		流量最大值
        #define M0_QMAX	460	//	18		流量最大值    //2012-11-24 FROM 409 TO 460
        #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
        		//8V时,FMF_MAX	185	8.14	
        #define M0_DDT3_4US_PER1BITV	1	//	0.072		
        		//8V时，DT3需要加	128	0.512	
        #define M0_DT1_4US	15	//每降1V加1000	1000		
        		//8V,T1需要增加	480	1.92	
        #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
        #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif



//2014-10-31备注：老m9，模型优化前的，怠速的电压特性不太好的
//2014-3-25 将电压最小值限制在8.6V，即采样值123
// #if M0_MFUD1==INJ309_2
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义
    // M9, 老的m9，隆鑫三轮一开始销售时的，给金浪200也用的是这个；
    //#else
        //2013-5-6
        #define 	aa10_8	138 
        #define 	ba10_8	-2045 
        #define 	ca10_8	4894 
        #define 	ab_10_6	-1503 
        #define 	bb_10_6	16425 
        #define 	cb_10_6	-35746 
        #define 	ac_10_0	1148 
        #define 	bc_10_0	-10607 
        #define 	cc_10_0	30169 
        		
        #define 	KD13M128_CO_2_M1024	0
        #define 	KD13M128_CO_1_M100	-137
        #define 	KD13M128_CO_0_M1	523
        		
        #define 	DM250_CO_1_M10	-76
        #define 	DM250_CO_0	2809
        		
        #define 	DT3c_M100	91
        		
        #define 	CO_G1_M8192	17
        #define 	CO_G2_M250	-472
    
       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    #define M0_QMAX	310	//	18		流量最大值
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2014-10-31备注：唐子光先期生产的车辆所使用的 
// #if M0_MFUD1==INJ109_2
  #if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义

        //2013-7-28 硬管
        //#if INJ1109==INJ1109_INT
            #define 	aa10_8	100 
            #define 	ba10_8	-1743 
            #define 	ca10_8	4328 
            #define 	ab_10_6	-1000 
            #define 	bb_10_6	12296 
            #define 	cb_10_6	-27879 
            #define 	ac_10_0	726 
            #define 	bc_10_0	-7396 
            #define 	cc_10_0	24336 
            		
            #define 	KD13M128_CO_2_M1024	21
            #define 	KD13M128_CO_1_M100	-1049
            #define 	KD13M128_CO_0_M1	1610
            		
            #define 	DM250_CO_1_M10	-58
            #define 	DM250_CO_0	2261
            		
            #define 	DT3c_M100	92
            		
            #define 	CO_G1_M8192	20
            #define 	CO_G2_M250	-463
        //#endif
            
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
            #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2014-10-31备注：可能是以前一直量产的218的结构
// #if M0_MFUD1==INJ209_2
  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义
    
   // #if(INJ21XX==INJ2108)
            // 2010-1-16  v9.4 -------------------------
            //T1-DT3斜率
            #define KD13M128_CO_2_M1024 2
            #define KD13M128_CO_1_M100  -145
            #define KD13M128_CO_0_M1    467
            #define DM250_CO_1_M10   -54      
            #define DM250_CO_0       2192     //T1-DT3截距
            #define DT3c_M100  93 // t30的电压修正参数   //DT3c=T30-1.06+0.0553*(E30-E)=T30-1.06+ 87*(E30-E)/100
            //-----------------------------------
            //标准电压特性的修正关系式中的参数 2010-1-13  v9.3
            //2010-1-13 修改电压特性模型
            #define 	aa10_8	-27
            #define 	ba10_8	-408
            #define 	ca10_8	1459
            #define 	ab_10_6	-356
            #define 	bb_10_6	4642
            #define 	cb_10_6	-10723
            #define 	ac_10_0	48
            #define 	bc_10_0	467
            #define 	cc_10_0	6808
           // g= T3d+co_g1*E+co_g2
            #define     CO_G1_M8192   21  //co_g1*8192
            #define     CO_G2_M250    -468  //(co_g1*0.9+co_g2)*250

    //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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

    //for low voltage
        //2012-10-9针对低电压的措施					
        #define M0_T1MAX_4US	2000	//us	8000		//T1最大值
        #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
        		//8V时最大T1	2512		
        //#define M0_QMAX	409	//	18		流量最大值
        #define M0_QMAX	460	//	18		流量最大值    //2012-11-24 FROM 409 TO 460
        #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
        		//8V时,FMF_MAX	185	8.14	
        #define M0_DDT3_4US_PER1BITV	1	//	0.072		
        		//8V时，DT3需要加	128	0.512	
        #define M0_DT1_4US	15	//每降1V加1000	1000		
        		//8V,T1需要增加	480	1.92	
        #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
        #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif




//－－－喷油器参数母map的声明－－－－－－－
extern const word M0_INJ_SLOP_MAP;// @          ADD_M0_INJ_SLOP_MAP ;
extern const word M0_INJ_SHIFT_MAP;//  @         ADD_M0_INJ_SHIFT_MAP ;
extern const word M0_TW1_CO_T35;//  @            ADD_M0_TW1_CO_T35    ;
extern const word M0_TV3_T1EQ3D5_0 ;// @         ADD_M0_TV3_T1EQ3D5_0  ;
extern const word M0_TV3_T13D5MS_SET;//  @       ADD_M0_TV3_T13D5MS_SET ;
extern const word M0_TV3_T13D5MS_INT;//2017-2-23 新增T30初始值
extern const word M0_SUM_TV3_TEMP_SET ;// @      ADD_M0_SUM_TV3_TEMP_SET ;
extern const word M0_KT3_MAP   ;//  @            ADD_M0_KT3_MAP       ;
extern const word M0_DT30_MAP  ;//  @            ADD_M0_DT30_MAP     ;
extern const byte M0_VBAT_ST  ;//       @        ADD_M0_VBAT_ST;              //2008-8-5 喷油器标准电池电压

//extern const word M1_BE_TW3[97]	@		ADD_M1_BE_TW3; 		//2009-12-8 delete
extern const word M1_DT3_Q[29] ;// @ ADD_M1_DT3_Q;
extern const byte M0_PROG_NUM_HL_INJ;               //#define M0_PROG_NUM_HL_INJ 0xA0//	;2008-3-5 0x90	;THE BIT4`BIT7 OF PRO_NUM_HL define the injector type,2008-3-5M8标准参数
extern const byte M0_NUMBER_T1;//#define M0_NUMBER_T1 	0x58//	;2008-3-21开始 专门用于代表喷油器的细小参数修改
extern const byte M0_TYPE_INJCTOR;//#define M0_TYPE_INJCTOR 8	//;2008-2-22 喷油器定为M8:M7的T3 ,t30=2.15,kt3=1.144,t10=4.529,kt1=1.276
extern const word M0_MAX_TINJ;//#define M0_MAX_TINJ 0xe00//
extern const word M0_MIN_TINJ;//#define M0_MIN_TINJ	0x300//EQU	03H	;IF	V_FMF_H<MIN_TINJ,LET	MAP_TINJ=0300H		
extern const byte M0_INJ_BIG_PCAH;// 当发动机转速高于3900时，最大T1的限制的系数；#define M0_INJ_BIG_PCAH	2//EQU	2	;WHEN	RPM>3907,	INJ	PULSE	CAN	BE	MORE	THAN	PCA	BY	2.048MS		
extern const byte M0_INJ_SHIFTH_REF;//#define M0_INJ_SHIFTH_REF	4//EQU	4	;INJ_SHIFT_REF=0X43E=4.344MS												
extern const byte M0_INJ_SHIFTL_REF;//#define M0_INJ_SHIFTL_REF	0x3e//EQU	3EH													


//2013-7-10 ADD 
extern const byte M2_NC_V[7][2];//2013-7-10
extern const byte M2_T1MAXD8_V[7][2]; //2013-7-10
/*--The following are deleted macro--------------*/
//delete//#define MTI_DY_JG 4//      EQU     4       ;4*2*4us=32us as min_order of ti 
//#define INJ_SLOPH_REF	2//EQU	2	;THE	SLOP	OF	REF_INJ	IS	11							
//#define INJ_SLOPL_REF	0//EQU	0				

/*====2014-12-5新定义===========================================================================*/


//2014-10-31备注：确定的最终使用的119的结构
//  #if M0_MFUD1==INJ109_3 // NOZZLE 55 限流0.6
  #if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

            //2014-9-15  110902+0.6限流孔		
            //其中：修正温度的t30脉宽为3.0ms，对应t3值为1.06,参考电压为3.5V		
            		
            #define 	aa10_8	-345 
            #define 	ba10_8	1459 
            #define 	ca10_8	-1269 
            #define 	ab_10_6	1159 
            #define 	bb_10_6	-2685 
            #define 	cb_10_6	-2376 
            #define 	ac_10_0	-591 
            #define 	bc_10_0	1415 
            #define 	cc_10_0	9760 
            		
            #define 	KD13M128_CO_2_M1024	28
            #define 	KD13M128_CO_1_M100	-1388
            #define 	KD13M128_CO_0_M1	1982
            		
            #define 	DM250_CO_1_M10	-58
            #define 	DM250_CO_0	2254
            		
            #define 	DT3c_M100	91
            		
            #define 	CO_G1_M8192	17
            #define 	CO_G2_M250	-434
                    
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
            #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 40//2016-4-28 add 
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2014-10-31备注：219分体结构，唐子光一直用的，2014-4-28结构没变，而优化的模型的，
// #if M0_MFUD1==INJ109
//2014-4-28
//  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义
#if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M9// 2015-4-14 原210902变为210904
        //2015-5-22 ,降噪结构恢复这个状态
        #define 	aa10_8	324 
        #define 	ba10_8	-3889 
        #define 	ca10_8	8751 
        #define 	ab_10_6	-2688 
        #define 	bb_10_6	27843 
        #define 	cb_10_6	-59338 
        #define 	ac_10_0	2097 
        #define 	bc_10_0	-19762 
        #define 	cc_10_0	49098 
        		
        #define 	KD13M128_CO_2_M1024	33
        #define 	KD13M128_CO_1_M100	-1475
        #define 	KD13M128_CO_0_M1	1914
        		
        #define 	DM250_CO_1_M10	-63
        #define 	DM250_CO_0	2419
        		
        #define 	DT3c_M100	91
        		
        #define 	CO_G1_M8192	18
        #define 	CO_G2_M250	-441
        //#endif
            
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
            #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
     // #define M0_FMF_PUMP     80
      #define M0_FMF_PUMP     10  //2015-7-10
    //  #define M0_NB_PUMP_FAIL 80
      #define M0_NB_PUMP_FAIL 40   //2015-7-10
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
      
      
#endif

//2015-4-14 降噪音版本
#if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_4 && M0_MFUD4==MFUD4_M9// 2015-4-14 210902

          //      2015-4-12  低噪音结构，210902+0.5喷孔		
          //      其中：修正温度的t30脉宽为3.0ms，对应t3值为1.06,参考电压为3.5V		
           //2015-5-22 被否定了     		
                #define 	aa10_8	-27 
                #define 	ba10_8	-857 
                #define 	ca10_8	2814 
                #define 	ab_10_6	-402 
                #define 	bb_10_6	8730 
                #define 	cb_10_6	-22576 
                #define 	ac_10_0	357 
                #define 	bc_10_0	-5581 
                #define 	cc_10_0	22214 
                		
                #define 	KD13M128_CO_2_M1024	14
                #define 	KD13M128_CO_1_M100	-686
                #define 	KD13M128_CO_0_M1	1119
                		
                #define 	DM250_CO_1_M10	-71
                #define 	DM250_CO_0	2666
                		
                #define 	DT3c_M100	81
                		
                #define 	CO_G1_M8192	13
                #define 	CO_G2_M250	-411
                            
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
            #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
      #define M0_FMF_PUMP     80//2015-5-21 FROM 80 TO 120
      #define M0_NB_PUMP_FAIL 40
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2014-10-31备注：可能是量产的219结构
//#if M0_MFUD1==INJ209
  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9 && M0_MFUD7==14// 2014-12-5 新定义
            
        #define 	aa10_8	177   //v9.14
        #define 	ba10_8	-1984 
        #define 	ca10_8	4356 
        #define 	ab_10_6	-1518 
        #define 	bb_10_6	13928 
        #define 	cb_10_6	-28123 
        #define 	ac_10_0	976 
        #define 	bc_10_0	-7311 
        #define 	cc_10_0	21837 
        		
        #define 	KD13M128_CO_2_M1024	0
        #define 	KD13M128_CO_1_M100	-81
        #define 	KD13M128_CO_0_M1	428
        		
        #define 	DM250_CO_1_M10	-63
        #define 	DM250_CO_0	2405
        		
        #define 	DT3c_M100	89
        		
        #define 	CO_G1_M8192	17
        #define 	CO_G2_M250	-434  
        

    //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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

    //for low voltage
        //2012-10-9针对低电压的措施					
        #define M0_T1MAX_4US	2000	//us	8000		//T1最大值
        #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
        		//8V时最大T1	2512		
        //#define M0_QMAX	409	//	18		流量最大值
        //#define M0_QMAX	460	//	18		流量最大值    //2012-11-24 FROM 409 TO 460
        
        //2016-5-7
        #define M0_QMAX	400	//	18		流量最大值 FOR 310901   
        #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
        		//8V时,FMF_MAX	185	8.14	
        #define M0_DDT3_4US_PER1BITV	1	//	0.072		
        		//8V时，DT3需要加	128	0.512	
        #define M0_DT1_4US	15	//每降1V加1000	1000		
        		//8V,T1需要增加	480	1.92	
        #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
        #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 40 //2016-4-28
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9 && M0_MFUD7==24
            
      /*  #define 	aa10_8	177   //v9.14
        #define 	ba10_8	-1984 
        #define 	ca10_8	4356 
        #define 	ab_10_6	-1518 
        #define 	bb_10_6	13928 
        #define 	cb_10_6	-28123 
        #define 	ac_10_0	976 
        #define 	bc_10_0	-7311 
        #define 	cc_10_0	21837 
        		
        #define 	KD13M128_CO_2_M1024	0
        #define 	KD13M128_CO_1_M100	-81
        #define 	KD13M128_CO_0_M1	428
        		
        #define 	DM250_CO_1_M10	-63
        #define 	DM250_CO_0	2405
        		
        #define 	DT3c_M100	89
        		
        #define 	CO_G1_M8192	17
        #define 	CO_G2_M250	-434   */
        
        #define 	aa10_8	137       //v9.24
        #define 	ba10_8	-1482 
        #define 	ca10_8	3209 
        #define 	ab_10_6	-1187 
        #define 	bb_10_6	10586 
        #define 	cb_10_6	-21069 
        #define 	ac_10_0	809 
        #define 	bc_10_0	-5894 
        #define 	cc_10_0	19082 
        		
        #define 	KD13M128_CO_2_M1024	0
        #define 	KD13M128_CO_1_M100	-114
        #define 	KD13M128_CO_0_M1	580
        		
        #define 	DM250_CO_1_M10	-67
        #define 	DM250_CO_0	2451
        		
        #define 	DT3c_M100	69
        		
        #define 	CO_G1_M8192	14
        #define 	CO_G2_M250	-411



    //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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

    //for low voltage
        //2012-10-9针对低电压的措施					
        #define M0_T1MAX_4US	2000	//us	8000		//T1最大值
        #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
        		//8V时最大T1	2512		
        //#define M0_QMAX	409	//	18		流量最大值
        //#define M0_QMAX	460	//	18		流量最大值    //2012-11-24 FROM 409 TO 460
        
        //2016-5-7
        #define M0_QMAX	400	//	18		流量最大值 FOR 310901   
        #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
        		//8V时,FMF_MAX	185	8.14	
        #define M0_DDT3_4US_PER1BITV	1	//	0.072		
        		//8V时，DT3需要加	128	0.512	
        #define M0_DT1_4US	15	//每降1V加1000	1000		
        		//8V,T1需要增加	480	1.92	
        #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
        #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 40 //2016-4-28
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

// #if M0_MFUD1==INJ309    //2014-5-25
       //2014年，上半年 M9批量生产用，嘉陵187用的
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

#define 	aa10_8	482 
#define 	ba10_8	-4979 
#define 	ca10_8	10598 
#define 	ab_10_6	-3539 
#define 	bb_10_6	33419 
#define 	cb_10_6	-68406 
#define 	ac_10_0	2605 
#define 	bc_10_0	-22841 
#define 	cc_10_0	53764 
		
#define 	KD13M128_CO_2_M1024	31
#define 	KD13M128_CO_1_M100	-1377
#define 	KD13M128_CO_0_M1	1747
		
#define 	DM250_CO_1_M10	-60
#define 	DM250_CO_0	2329
		
#define 	DT3c_M100	85
		
#define 	CO_G1_M8192	13
#define 	CO_G2_M250	-429

       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    //#define M0_QMAX	310	//	18		流量最大值
    //#define M0_QMAX	400	//2014-6-17 35UL/PULSE
    #define M0_QMAX	310	//2015-5-10 UL/PULSE
     
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		
    #define M0_T1_T35 750        //单位为4us //3MS

    //2015-5-20
    //  #define M0_FMF_PUMP     50
    //  #define M0_NB_PUMP_FAIL 80
    
    
    //2015-10-12 AT DATA_INJ_SOURCE.C 310901
      #define M0_FMF_PUMP     5
      #define M0_NB_PUMP_FAIL 10
      
      
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2014-10-31备注：临时实验用，于量产无关
// #if M0_MFUD1==INJ309_3  
//2014-9-21 310902		
//其中：修正温度的t30脉宽为3.0ms，对应t3值为1.06,参考电压为3.5V		
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义
		
#define 	aa10_8	-153 
#define 	ba10_8	665 
#define 	ca10_8	-616 
#define 	ab_10_6	255 
#define 	bb_10_6	299 
#define 	cb_10_6	-3188 
#define 	ac_10_0	-172 
#define 	bc_10_0	1414 
#define 	cc_10_0	5988 
		
#define 	KD13M128_CO_2_M1024	7
#define 	KD13M128_CO_1_M100	-412
#define 	KD13M128_CO_0_M1	802
		
#define 	DM250_CO_1_M10	-70
#define 	DM250_CO_0	2634
		
#define 	DT3c_M100	87
		
#define 	CO_G1_M8192	13
#define 	CO_G2_M250	-431

       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    #define M0_QMAX	310	//	18		流量最大值
    
    
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 750        //单位为4us //3MS

    /*
    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 80
    */
    //2016-4-7  
      #define M0_FMF_PUMP     10
      //#define M0_NB_PUMP_FAIL 20
      #define M0_NB_PUMP_FAIL 2//2016-4-28
      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif


//2014-10-15
//#if M0_MFUD1==INJ_SCR1
    //2014-9-30 更新 喷嘴模型，使用IRF540N，之后需要考虑使用IGBT
//2014-12-5 
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

    #define 	aa10_8	1081 
    #define 	ba10_8	-6420 
    #define 	ca10_8	7791 
    #define 	ab_10_6	-7035 
    #define 	bb_10_6	37652 
    #define 	cb_10_6	-43677 
    #define 	ac_10_0	4381 
    #define 	bc_10_0	-21354 
    #define 	cc_10_0	32324 
    		
    #define 	KD13M128_CO_2_M1024	10
    #define 	KD13M128_CO_1_M100	-474
    #define 	KD13M128_CO_0_M1	752
    		
    #define 	DM250_CO_1_M10	-27
    #define 	DM250_CO_0	1125
    		
    #define 	DT3c_M100	55
    		
    #define 	CO_G1_M8192	15
    #define 	CO_G2_M250	-402



       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    #define M0_QMAX	310	//	18		流量最大值
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		
		
    //#define 	M0_VBAT_ST	171


    //#define	M0_ST_T30	190
    #define M0_T1_T35 425        //单位为4us //1.7MS

    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 80

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif

//2015-3

#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M10// 2015-2-13 NEW DEFINITION
    // base on 2015-2-10 电压特性(V10.2)
    //标准电压28V，T3B会是负数
      #define 	aa10_8	704 
      #define 	ba10_8	-2680 
      #define 	ca10_8	2522 
      #define 	ab_10_6	-4868 
      #define 	bb_10_6	17683 
      #define 	cb_10_6	-15994 
      #define 	ac_10_0	3092 
      #define 	bc_10_0	-8535 
      #define 	cc_10_0	12127 
      		
      #define 	KD13M128_CO_2_M1024	45
      #define 	KD13M128_CO_1_M100	-1686
      #define 	KD13M128_CO_0_M1	1858
      		
      #define 	DM250_CO_1_M10	-29
      #define 	DM250_CO_0	1193
      		
      #define 	DT3c_M100	80
      		
      #define 	CO_G1_M8192	17
      #define 	CO_G2_M250	-349


       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    #define M0_QMAX	310	//	18		流量最大值
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		
		
    //#define 	M0_VBAT_ST	171


    //#define	M0_ST_T30	190
    #define M0_T1_T35 425        //单位为4us //1.7MS
    //2015-5-20
      #define M0_FMF_PUMP     50
      #define M0_NB_PUMP_FAIL 80

      #define T1_INI_FOR_SCAN 800 //2015-5-20T1扫描用的初始值
#endif
/*
//2015-4-1
//流量偏小，临时使用的
#if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M10// 2015-4-1 NEW DEFINITION
    
    //2015-4-1 标准流量点为24UL
    
    //      线圈0.45，M10,0.7喷孔+6mm		
    //  其中：修正温度的t30脉宽为1.7ms，对应t3值为0.8,参考电压为3.5V		
      		
      #define 	aa10_8	2318 
      #define 	ba10_8	-13209 
      #define 	ca10_8	15757 
      #define 	ab_10_6	-14357 
      #define 	bb_10_6	79071 
      #define 	cb_10_6	-92928 
      #define 	ac_10_0	10106 
      #define 	bc_10_0	-53835 
      #define 	cc_10_0	68866 
      		
      #define 	KD13M128_CO_2_M1024	73
      #define 	KD13M128_CO_1_M100	-2791
      #define 	KD13M128_CO_0_M1	2962
      		
      #define 	DM250_CO_1_M10	-28
      #define 	DM250_CO_0	1186
      		
      #define 	DT3c_M100	82
      		
      #define 	CO_G1_M8192	15
      #define 	CO_G2_M250	-321

       //3.5V 与小流量喷嘴相同
        //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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
    //#endif

//for low voltage
    //2012-10-9针对低电压的措施			//2012-10-9 临时		
    //#define M0_T1MAX_4US	2000	//us	8000		//T1最大值
    #define M0_T1MAX_4US	2100	//us	8000		//T1最大值 2013-7-15
    #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
    		//8V时最大T1	2512		
    #define M0_QMAX	310	//	18		流量最大值
    #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
    		//8V时,FMF_MAX	185	8.14	
    #define M0_DDT3_4US_PER1BITV	1	//	0.072		
    		//8V时，DT3需要加	128	0.512	
    #define M0_DT1_4US	15	//每降1V加1000	1000		
    		//8V,T1需要增加	480	1.92	
    #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
    #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		
		
    //#define 	M0_VBAT_ST	171


    //#define	M0_ST_T30	190
    #define M0_T1_T35 425        //单位为4us //1.7MS

#endif

*/

//2015-11-21 M10双孔，未加大斜率，暂时只留给隆鑫通机使用
//#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10// 
#if M0_MFUD1==MFUD1_3 && M0_MFUD6==51 && M0_MFUD4==MFUD4_M10// "2015-11-20   M10(V10.09)-0.6x2喷孔-t3b-0.2-1"

                  #define 	aa10_8	-53 
                  #define 	ba10_8	-283 
                  #define 	ca10_8	633 
                  #define 	ab_10_6	-220 
                  #define 	bb_10_6	3374 
                  #define 	cb_10_6	-5100 
                  #define 	ac_10_0	262 
                  #define 	bc_10_0	-2138 
                  #define 	cc_10_0	5337 
                  //#define 	cc_10_0	1337 
                  		
                  #define 	KD13M128_CO_2_M1024	95
                  #define 	KD13M128_CO_1_M100	-4214
                  #define 	KD13M128_CO_0_M1	5167
                  		
                  #define 	DM250_CO_1_M10	5
                  #define 	DM250_CO_0	350
                  		
                  #define 	DT3c_M100	30
                  		
                  #define 	CO_G1_M8192	6
                  #define 	CO_G2_M250	-195

                                        
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
           // #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            #define M0_QMAX	560	//	18		流量最大值   2015-12-11
            //#define M0_QMAX	300	//2013-7-31
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 425        //单位为4us //1.7MS

    //2015-5-20
      //#define M0_FMF_PUMP     50
      #define M0_FMF_PUMP     40
      //#define M0_NB_PUMP_FAIL 80
      #define M0_NB_PUMP_FAIL 40
      //#define T1_INI_FOR_SCAN 500 //2015-5-20T1扫描用的初始值
      #define T1_INI_FOR_SCAN 300 //2015-12-9T1扫描用的初始值

#endif

//2015-12-14
#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10
            #define 	aa10_8	-290 
            #define 	ba10_8	-163 
            #define 	ca10_8	1114 
            #define 	ab_10_6	311 
            #define 	bb_10_6	5117 
            #define 	cb_10_6	-9600 
            #define 	ac_10_0	67 
            #define 	bc_10_0	-3747 
            #define 	cc_10_0	9453 
            		
            #define 	KD13M128_CO_2_M1024	34
            #define 	KD13M128_CO_1_M100	-1538
            #define 	KD13M128_CO_0_M1	1937
            		
            #define 	DM250_CO_1_M10	-15
            #define 	DM250_CO_0	977
            		
            #define 	DT3c_M100	30
            		
            #define 	CO_G1_M8192	6
            #define 	CO_G2_M250	-195

                                        
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
           // #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	560	//	18		流量最大值   2015-12-11
            #define M0_QMAX 440	//	2015-12-15
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 425        //单位为4us //1.7MS

    //2015-5-20
      //#define M0_FMF_PUMP     50
      #define M0_FMF_PUMP     40
      //#define M0_NB_PUMP_FAIL 80
      #define M0_NB_PUMP_FAIL 40
      //#define T1_INI_FOR_SCAN 500 //2015-5-20T1扫描用的初始值
      #define T1_INI_FOR_SCAN 300 //2015-12-9T1扫描用的初始值


#endif

//2015-12-16 2*1002 "M10(V10.10)-0.5喷孔-t3b-0.1(增加斜率） "
#if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10
              #define 	aa10_8	210 
              #define 	ba10_8	-1987 
              #define 	ca10_8	2772 
              #define 	ab_10_6	-2693 
              #define 	bb_10_6	15293 
              #define 	cb_10_6	-18214 
              #define 	ac_10_0	2109 
              #define 	bc_10_0	-9554 
              #define 	cc_10_0	13425 
              		
              #define 	KD13M128_CO_2_M1024	33
              #define 	KD13M128_CO_1_M100	-1483
              #define 	KD13M128_CO_0_M1	1824
              		
              #define 	DM250_CO_1_M10	-26
              #define 	DM250_CO_0	1225
              		
              #define 	DT3c_M100	36
              		
              #define 	CO_G1_M8192	9
              #define 	CO_G2_M250	-210

                                        
               //3.5V 与小流量喷嘴相同
                //标准DOP参数、 //2010-3-8注意：该参数改动时，需要与sys_ini中的的一起改动
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


        //for low voltage
            //2012-10-9针对低电压的措施			//2012-10-9 临时		
            #define M0_T1MAX_4US	1800	//us	8000		//T1最大值
            #define M0_DT1MAX_4US_PER1BITV	16	//1000 PER VOLT	1000us PER 16 AD	15.625	低电压时每个采样值对应的T1最大值的增量
            		//8V时最大T1	2512		
           // #define M0_QMAX	460	//	18		流量最大值   2014-5-25 from 320 to 460
            //#define M0_QMAX	560	//	18		流量最大值   2015-12-11
            #define M0_QMAX 440	//	2015-12-15
            
            #define M0_DQMAX_PER1BVOLT	7	// 3ul/v	4.261363636	fmf/ad	低电压时每个采样值对应的流量最大值的减量
            		//8V时,FMF_MAX	185	8.14	
            #define M0_DDT3_4US_PER1BITV	1	//	0.072		
            		//8V时，DT3需要加	128	0.512	
            #define M0_DT1_4US	15	//每降1V加1000	1000		
            		//8V,T1需要增加	480	1.92	
            #define M0_V_LOWTH	145	//10V	电压的门限，低于这个值，按低电压计；		
            #define M0_VMIN	123	//8V	电压最小值，低于该电压时，按该电压处理		

    #define M0_T1_T35 425        //单位为4us //1.7MS

    //2015-5-20
      //#define M0_FMF_PUMP     50
      #define M0_FMF_PUMP     40
      //#define M0_NB_PUMP_FAIL 80
      #define M0_NB_PUMP_FAIL 40
      //#define T1_INI_FOR_SCAN 500 //2015-5-20T1扫描用的初始值
      #define T1_INI_FOR_SCAN 300 //2015-12-9T1扫描用的初始值


#endif

