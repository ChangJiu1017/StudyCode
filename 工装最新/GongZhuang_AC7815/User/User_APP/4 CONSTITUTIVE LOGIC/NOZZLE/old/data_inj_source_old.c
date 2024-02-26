#include "data_sys_standard.h"
#include "map_selection.H"

//#include "\map for injector\m7\map_PN4_Injector.c"
//#include "\map for injector\m8-080426\map_PN4_Injector.c"
//#include "\map for injector\m8-080527\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-080606\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-080620\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-080606-temptest\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-080918\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-081023\map_PN4_Injector.c"//负斜率喷油器
//#include "\map for injector\m8-081119\map_PN4_Injector.c"//负斜率喷油器  //2008-11-19 derived from m8-080606
//#include "\map for injector\m8-090707\map_PN4_Injector.c"//负斜率喷油器  //2008-11-19 derived from m8-080606
//#include "\map for injector\m8-091207\map_PN4_Injector.c"//2009-12-7 乐起降的新标准
//#include "\map for injector\m8-100101\map_PN4_Injector.c"//该标准的喷油器有问题
//#include "\map for injector\m8-100113\map_PN4_Injector.c"//2009-12-7 乐起降的新标准

#define INJ_REF_3D5 1
#define INJ_REF_2D5 0

#define INJ2109 1
#define INJ2108 0

#define INJ21XX  INJ2108     //2013-6-11 ADD 

#define INJ3108 INJ_REF_3D5

//#if M0_MFUD1==INJ109_3
#if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义
    #include "\map for injector\1109-140916 NO55\map_PN4_Injector.c"//2013-6-19
#endif

#if M0_MFUD1==INJ109_4
    #include "\map for injector\1109-140827 NO58 0.3\map_PN4_Injector.c"//2013-6-19
#endif


#if M0_MFUD1==INJ209_3 //2014-8-27
    #include "\map for injector\2109-140827 NO56\map_PN4_Injector.c"//2010-3-3
#endif

// #if M0_MFUD1==INJ309_3
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

        #include "\map for injector\310902-140916 NO57\map_PN4_Injector.c"//2010-3-11    2013-1-10　DELETE


		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
		const byte M2_NC_V[7][2]=										
		{	146	,	137 	,						
			157	,	148 	,						
			173	,	152 	,						
			189	,	155 	,						
			205	,	159 	,						
			221	,	160 	,						
			237	,	160 	,						
		};										
												
										//RPM		
		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
		{	146	,	188 	,	//	1504		187		5.984
			157	,	188 	,	//	1504		187		5.984
			173	,	191 	,	//	1528		184		5.888
			189	,	194 	,	//	1552		181		5.792
			205	,	197 	,	//	1576		178		5.696
			221	,	200 	,	//	1600		175		5.6
			237	,	203 	,	//	1624		172		5.504
		};										
							//			一个循环给T1留下的时间，单位4US		


#endif


//m8v9p4-100116
//#include "\map for injector\m8v9p4-100116\map_PN4_Injector.c"//2010-1-16
// #if M0_MFUD1==INJ309_2
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义
        //#include "\map for injector\m8-3108-130110\map_PN4_Injector.c"//2010-3-11    2013-1-10　DELETE
        #include "\map for injector\m8-3108-130506\map_PN4_Injector.c"//2010-3-11    2013-1-10　DELETE


		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
		const byte M2_NC_V[7][2]=										
		{	146	,	137 	,						
			157	,	148 	,						
			173	,	152 	,						
			189	,	155 	,						
			205	,	159 	,						
			221	,	160 	,						
			237	,	160 	,						
		};										
												
										//RPM		
		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
		{	146	,	188 	,	//	1504		187		5.984
			157	,	188 	,	//	1504		187		5.984
			173	,	191 	,	//	1528		184		5.888
			189	,	194 	,	//	1552		181		5.792
			205	,	197 	,	//	1576		178		5.696
			221	,	200 	,	//	1600		175		5.6
			237	,	203 	,	//	1624		172		5.504
		};										
							//			一个循环给T1留下的时间，单位4US		


#endif
 
//#if M0_MFUD1==INJ109_2
  #if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义
    #include "\map for injector\1109 NOZZLE INT-130728\map_PN4_Injector.c"//2013-6-19
#endif
 
//#if M0_MFUD1==INJ209_2
  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M8// 2014-12-5 新定义
    #include "\map for injector\2109-130611\map_PN4_Injector.c"//2010-3-3
#endif

//#if M0_MFUD1==INJ109
//  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_2 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义
  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M9// 2015-4-14 变更为210904，2015-5-22又恢复回来
    		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		
    #include "\map for injector\1109-140205\map_PN4_Injector.c"//2013-6-19
#endif

//2015-4-14 降噪音版本
#if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_4 && M0_MFUD4==MFUD4_M9// 
    		//2015-5-22 否定
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		
    #include "\map for injector\210902-20150414 jiangzao\map_PN4_Injector.c"//2013-6-19
#endif
 
 

//#if M0_MFUD1==INJ209
  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9 && M0_MFUD7==14// 2014-12-5 新定义
    		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		

        //#include "\map for injector\2109-140425\map_PN4_Injector.c"//2014-4-25
        #include "\map for injector\2109-140608\map_PN4_Injector.c"//2014-6-8 219 UPDATEv     9.14
       //#include "\map for injector\210901-v9.24\map_PN4_Injector.c"//2017-2-11  219 UPDATEv     9.24
#endif

  #if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9 && M0_MFUD7==24
    		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		

       #include "\map for injector\210901-v9.24\map_PN4_Injector.c"//2017-2-11  219 UPDATEv     9.24
#endif

//#if M0_MFUD1==INJ309
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD1_1 && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

    		//2013-7-10 根据3108喷嘴汇总的特性，仅限与3108喷嘴										
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		

        #include "\map for injector\3109-140525\map_PN4_Injector.c"//2014-5-25
#endif


//#if M0_MFUD1==INJ_SCR1
//2014-12-5 
  #if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M9// 2014-12-5 新定义

    //#include "\map for injector\m9 v24 scr 20140825\map_PN4_Injector.c"
    #include "\map for injector\m9 v24 scr 20141105\map_PN4_Injector.c"
#endif


#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M10// 2015-2-13 NEW DEFINITION

    #include "\map for injector\m10 v24 scr 2015-3-24\map_PN4_Injector.c"  //2015-3-24 change to 
    
#endif
/*
#if M0_MFUD1==MFUD1_1 && M0_MFUD6==MFUD6_SCR && M0_MFUD4==MFUD4_M10// 2015-4-1 NEW DEFINITION
    #include "\map for injector\m10 v24 scr 2015-4-1\map_PN4_Injector.c"  //2015-4-1,标准流量点24UL

#endif
*/


//2015-6-7
//#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10// 
#if M0_MFUD1==MFUD1_3 && M0_MFUD6==51 && M0_MFUD4==MFUD4_M10// "2015-11-20   M10(V10.09)-0.6x2喷孔-t3b-0.2-1"
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		
    #include "\map for injector\M10 11-21 2HOLE\map_PN4_Injector.c"//2013-6-19
    
#endif


//2015-12-14
#if M0_MFUD1==MFUD1_3 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		
    #include "\map for injector\M10 12-14 2HOLE\map_PN4_Injector.c"//2015-12-14 10.09版本，加大斜率
#endif

//2015-12-16 2*1002 "M10(V10.10)-0.5喷孔-t3b-0.1(增加斜率） "
#if M0_MFUD1==MFUD1_2 && M0_MFUD6==MFUD6_2 && M0_MFUD4==MFUD4_M10
    		const byte M2_NC_V[7][2]=										
    		{	146	,	137 	,						
    			157	,	148 	,						
    			173	,	152 	,						
    			189	,	155 	,						
    			205	,	159 	,						
    			221	,	160 	,						
    			237	,	160 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//按回位时间			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			一个循环给T1留下的时间，单位4US		
    #include "\map for injector\M10 12-16 1HOLE\map_PN4_Injector.c"

#endif
/**/