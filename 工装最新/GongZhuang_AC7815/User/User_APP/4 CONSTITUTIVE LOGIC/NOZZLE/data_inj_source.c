#include "data_sys_standard.h"
#include "map_selection.H"

//#include "\map for injector\m7\map_PN4_Injector.c"
//#include "\map for injector\m8-080426\map_PN4_Injector.c"
//#include "\map for injector\m8-080527\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-080606\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-080620\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-080606-temptest\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-080918\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-081023\map_PN4_Injector.c"//��б��������
//#include "\map for injector\m8-081119\map_PN4_Injector.c"//��б��������  //2008-11-19 derived from m8-080606
//#include "\map for injector\m8-090707\map_PN4_Injector.c"//��б��������  //2008-11-19 derived from m8-080606
//#include "\map for injector\m8-091207\map_PN4_Injector.c"//2009-12-7 ���𽵵��±�׼
//#include "\map for injector\m8-100101\map_PN4_Injector.c"//�ñ�׼��������������
//#include "\map for injector\m8-100113\map_PN4_Injector.c"//2009-12-7 ���𽵵��±�׼

#define INJ_REF_3D5 1
#define INJ_REF_2D5 0

#define INJ2109 1
#define INJ2108 0

#define INJ21XX  INJ2108     //2013-6-11 ADD 

#define INJ3108 INJ_REF_3D5


//����
#if M0_MFUD1==0 && M0_MFUD3==0 && M0_MFUD4==0 && M0_MFUD6==0 && M0_MFUD7==0// 2020-5-19 ����										
    		const byte M2_NC_V[7][2]=										
    		{	146	,	192 	,						
    			157	,	192 	,						
    			173	,	192 	,						
    			189	,	192 	,						
    			205	,	192 	,						
    			221	,	192 	,						
    			237	,	192 	,						
    		};										
    												
    										//RPM		
    		const byte M2_T1MAXD8_V[7][2]=					//����λʱ��			10000		
    		{	146	,	188 	,	//	1504		187		5.984
    			157	,	188 	,	//	1504		187		5.984
    			173	,	191 	,	//	1528		184		5.888
    			189	,	194 	,	//	1552		181		5.792
    			205	,	197 	,	//	1576		178		5.696
    			221	,	200 	,	//	1600		175		5.6
    			237	,	203 	,	//	1624		172		5.504
    		};										
    							//			һ��ѭ����T1���µ�ʱ�䣬��λ4US		

        #include "110902934F_map_PN4_Injector.c"

#endif


