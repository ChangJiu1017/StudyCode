#include "public.h"
/*===========================================================*/
/*====启动控制模块=====================*/
/*===========================================================*/

//在发动机启动的时刻，根据缸头温度获得启动油量修正系数的初值
//功能：初始化STARTS,//在发动机运转时刻调用
//形参：teng--发动机温度
extern void FI_Starts_BeforeStarts(sword tw,sbyte tair);

//启动油量修正系数vc_FMF_starts的衰减过程
//功能：STARTS参数vc_FMF_starts的衰减过程；
extern word F_Starts_FMF(byte tps_odr,word rev,sbyte ta,sword tw,word fmf_frommap);//ASTK的衰减过程://在获得基础MAP之后;//ASTK对油量修正之前


extern void FI_Starts_AfterReset(void);


extern void FC_F(void);

extern void FC_U(byte tps_odr,word rev,sbyte ta,sword tw,word fmf_frommap);
extern word FC_msMU(word fmf_frommap);
//----------------------------------------------------------------------------
//starts
//extern byte vc_FMF_starts;// 启动油量修正系数
//extern byte vc_FMF_restarts;                    //2008-11-11

extern word FMF_PreInj(void);                   //2008-11-24   ;//单次预喷油量
extern word NB_Preinj(sword tw,sbyte ta);         //2008-11-24    //总预喷次数；
extern word FMF_1st_RUN(byte teng_inuse,byte vbat);              //2008-12-3

extern sword v_du_M128_M5;//2012-11-9


extern word v_t_exp_M128_M32;    //时间系数：
                        //停机时，与停机时间相关；运行时，与发动机转速相关；


extern byte v_fmf_idle_st;//怠速油量 2013-3-1

extern byte v_UR_M16_ex;  //对U的修正；加速，全负荷等等；
extern byte v_us_M16;  //2013-4-14

extern sbyte v_dUR_M16;


extern byte v_X;
extern byte v_Z;
extern byte v_f0;
extern byte v_g;

#define   M0_M0_M_N0M16 (long int)151600  			
#define   M0_M0_M_N0 (word)9600  			

#define   M0_M0_M_N0_M4 (word)38400  
//#define M0_CP	1
#define	M0_QL	297
//#define	BYPTR_M0_FBP	227

//#define	M0_FBP	140//2013-3-2 FROM 80 TO 100
                  //2013-3-25 from 100 to 140

//#define	M0_FBP	110//2013-5-13 from 140 to 110

#define M0_SD0_D4	165

//#define MUL_M0_N0_CP 151600//M0_m0*M0_N0_M16*M0_CP
#define MUL_M0_N0_CP_DIV_SD0_QL 3//M0_m0*M0_N0_M16*M0_CP
