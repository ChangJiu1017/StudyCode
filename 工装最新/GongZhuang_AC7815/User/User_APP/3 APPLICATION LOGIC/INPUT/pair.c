/*HEAD
 ***********************************************************************
 * Filename	:pair.c
 * Function	:CAN comunication		
 * User		:xielian
 * Date		:2017.3.28
 * Version	:V8.2.0
 *
 *** History ***
 * V8.0.0   2017.1.17	zhangping 
 *		Base module.
 * V8.1.0   2017.1.17	zhangping 
 *		VF_WITHPAIRS_PRO2通过BYPTR_M0_EN_PAIRS进行配置.
 * 
 * V8.2.0   2017.1.17	xielian
 *		删除V_PRO2_F5_STATUS和V_PRO1_F6_STATUS用软件版本赋值
 *
 * V8.2.1   2017.3.28	xielian
 *		bugfix,增加VF_WITHPAIRS_PRO2赋值和V_PRO2_F5_STATU,V_PRO1_F6_STATUS初始化
 ***********************************************************************
HEAD END*/

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Public.h"
#include "Status_st.h"
#include "pair.H"

#include "ee.h"
#include "add.h"

#include "dtc.h"

#include "time_ccl.h"

#include "sys_init.h"  //2014-11-27 加入，之前有误；

//#include "wait_moving.h"
//#include "sci.h"

//定义所使用的压力传感器类型
#define PAIRS_KP215 0
#define PAIRS_CAB   1

#define M0_PAIRS    PAIRS_KP215


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_PAIR_STATUS;
//#pragma DATA_SEG DEFAULT
                           ///////////////////////////////////////////////////////////////////
byte v_pair_ad;            //pair 的AD   //经过判断的认为无故障的PAIRS信号//2008-11-5 //    //
byte v_pair_odr;           //根据何一杰测试结果，针对US1119PAIRS，                    //    //
                           //海拔每升高1000m，采样值降低15.2，近似取16?               //    //
byte v_pair_co;            //修正后的环境压力值，使用保守值                           //    //
byte v_pair_mode;          //                                                         //    //
byte v_pair_ee;            //最近一次读取的存储在EE中的PAIRS；                        //    //
                           //                                                         //    //
//2009-12-18 add           //                                                         //    //
byte v_pair_max_afterkeyon;//                                                         //    //
byte v_pair_min_afterkeyon;//                                                         //    //
byte v_pair_aveinlast10min;//                                                         //    //
byte nb_forpair;           /////////////////////////////////////////////////////////////    //

byte v_Pair_ad_tmp;//PAIRS 直接AD采样,满足条件时 赋给 v_pair_ad_input

/*-----------The following are local ...----------------------------------------------------*/
#define M0_PAIR_CAB_L   170//0 第一个标定点，海拔对应的大气压力传感器标准值；暂定0海平面?
#define M0_PAIR_CAB_U   100//第二个标定点对应的pair的标准值?
//#define MAX_PAIR        190 //PAIR信号的最大值；
//#define MAX_PAIR        200 //PAIR信号的最大值；
#define MAX_PAIR        240 //PAIR信号的最大值； //2010-5-10 change    //2010-8-23 FROM 245 TO 240
#define MIN_PAIR        70  //PAIR信号的最小值；

//2013-4-
#define ORI_MAX_PAIR    240//70//PAIRS原始采样值
#define ORI_MIN_PAIR    70//240//

#define COR_MAX_PAIR    200//75//PAIRS经修正后的值     注意在使用时的等于，等于时就表示有故障的；
#define COR_MIN_PAIR    75//200//

#define MAP_FOR_H7000   112//海拔7000对应的pair的差值
#define MAP_FOR_H6000   96//42//96//海拔6000对应的pair的差值

#define MAX_VBAT        250
#define MIN_VBAT        0

//2010-7-7 chang from 100 to 120 for mpxm2102
#define DELTA_K_PAIRS   120
#define DELTA_B_PAIRS   120

//#define DELTA_PAIR_INSHORTTIME  5//在短期内   //2009-change from 4 to 6
                        //2010-8-20 from 3 to 4
//#define DELTA_PAIR_INSHORTTIME  10//2011-9-8 changge from 5 to 10
#define DELTA_PAIR_INSHORTTIME  16//单组内的采样波动

#define DELTA_PAIR_FORAVE       10//前后两组压力平均的差异
                        
#define DELTA_PAIR_AFTERKEYON   48
#define VF_PERIOD_PAIRS         ((VF_MMOVE_ENG&&VF_16S_TMCCL)||((!VF_MMOVE_ENG)&&VF_4S_TMCCL))//2008-11-6          

#define NB_PAIRER_LIMIT 5//2010-10-10 CHANGE FROM 2 TO 3 from 3 to5

/*
#define NB_FORPAIR_LIMIT    256
#define NB_FORPAIR_SAMPULIMIT    240    //2010-7-19 ADD
#define NB_FORPAIR_LLIMIT    5
*/

//2010-10-10 change to 
#define NB_FORPAIR_LIMIT    10//2020-6-19 for CR 64
#define NB_FORPAIR_SAMPULIMIT    56    //2010-7-19 ADD
#define NB_FORPAIR_LLIMIT    6
//2010-10-10,停车时，0.4ms采集一次，因此一个周期大约2.5s;
//2015-6-29 ,20ms采集一次，因此需要减少次数，以在短时间内能采集到数据



#define EE_DATA_WITHPAIRS       0xaa
#define EE_DATA_WITHOUTPAIRS    0x55
#define EE_DATA_PAIRS_BYMANNUAL 0x44

/*使用KP215的K/P设置*/			
#define	KKP215_M256		195

#ifdef	SY_HUANSONG		//20210924 chenwei add 环松选项 for test
#define	DKKP215_M256		-107
#define	DBKP215		26

#else	
#define	DKKP215_M256		-61
#define	DBKP215		10

#endif

/*------------------------------------------------------------------------------------*/
sbyte   kdPairs;        //pairs 标定的斜率；
sbyte   bdPairs;        //pairs 标定的截距差异；


byte v_pair_ee;                 //储存在EE中的PAIRS
byte t_pair_er;             //pair 传感器连续出错的次数
//在REMARK!=4,v_pair_ad直接等于v_pair_ad_input，且不进行诊断，也不保存；

byte v_pair_ad_input;   //PAIRS A/D采集的直接输入值，需要经过诊断之后才可用；
byte v_pair_ad_input_min;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最小值
byte v_pair_ad_input_max;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最大值
byte v_vbat_maxwhenpair;//       v_vbat_maxwhenpair= MAX_VBAT
byte v_vbat_minwhenpair;

byte v_pair_ad_64t;//64采样的平均,   v_pair_ad=((word)v_pair_ad*4-v_pair_ad_64t)/4

byte v_pair_ad_64t_lst;//203-4-24
//2010-1-4 ADD
sbyte v_KPT; //pairs的温度修正系数；   v_KPT=(Xs1-Xs2)/(Ta1-Ta2)
byte v_Tmp_KPT;//   k,b标定时，对应的标准温度；
sbyte v_KPT_L; //pairs的低温温度修正系数；   v_KPT=(Xs1-Xs2)/(Ta1-Ta2)

byte v_Pair_deg;

sword v_p_kpa;

#define TMP_KPT_ST  75//EQU 25 SHESHIDU
//byte Load_PAIRS_DK(byte add);
void Check_PairinEE_Valid(void);//2008-11-5 add 
byte Get_Pairs_Odr_and_Altitude(byte pairs,byte blnEE);     //2009-12-30 add para ,如果是ee中的数据，不得报错
void Rec_Pairs_ad(void);
void Rec_Pairs_co(void);
/*------------------------------------------------------------------------------------------*/
void FI_P_air_AfterReset(void)
{
    if(VF_VPOR_HARD)
    {
               
        V_PAIR_STATUS=false;//2008-12-15

        V_MAPS_STATUS=false;     //2013-9-9 add condiciton   
        //VF_HARDCFG_ERR_PAIRS//VF_SOFTCFG_ERR_PAIRS
		
		
        // 
        Check_PairinEE_Valid();         //2008-11-5 add  需要放置在
        t_pair_er=0;//2009-11-24 add 
        v_vbat_maxwhenpair= MIN_VBAT;
        v_vbat_minwhenpair= MAX_VBAT;
        v_pair_min_afterkeyon=v_pair_ad_input_min=ORI_MAX_PAIR;
        v_pair_max_afterkeyon=v_pair_ad_input_max=ORI_MIN_PAIR;

        nb_forpair=0;
        v_t_pairs_1s=0; //发动机不运行时不累加,但也不清零

		//2017-2-27 取消通过软件版本号来选择功能，更改为通过使能标志位控制
		//V_PRO2_F5_STATUS=M0_PRO_2;
        //V_PRO1_F6_STATUS=M0_PRO_3;
		
		V_PRO2_F5_STATUS=104;//2017-3-30 根据常用配置进行默认初始化
		V_PRO1_F6_STATUS=128;//2017-3-30 根据常用配置进行默认初始化
        
        
        if(BYPTR_M0_EN_PAIRS==EN_PAIRS_NOTEN)
		{
			VF_WITHPAIRS_PRO2=false;//2017-1-21
        }
		else
		{
			VF_WITHPAIRS_PRO2=true;	///2017-3-28 VF_WITHPAIRS_PRO2没有定义，实际发现其为随机值
		}

    }
    
}

//2008-11-5
//增加对海拔级别的保存功能
void FM_P_air_WhenKeyOff(void)
{
    //保存PAIRS的必要条件：
    //  1 当PAIRS没有故障；
    //  2 当PAIRS在REMARK==4时获得了有效的数据
   // if((!VF_ER_PAIR)&&(VF_1PAIRSGOT_PAIR)&&(VF_WITHPAIRS_PRO2)&&VF_ECUHARD_WITHPAIR_PAIR&&(VF_PAIR_VALID_FOREE_PAIR))
    if((!VF_ER_PAIR)&&(VF_1PAIRSGOT_PAIR)&&(VF_WITHPAIRS_PRO2)&&VF_ECUHARD_WITHPAIR_PAIR)//2016-6-5
    {   //2013-4-23 add VF_ECUHARD_WITHPAIR_PAIR
        if(v_pair_co<COR_MAX_PAIR&&v_pair_co>COR_MIN_PAIR)//2009-12-15 add condiction 
        {
            Save_Data_EE(EE_PAIRS,v_pair_co); //保存到EE中
            Save_Data_EE(EE_PAIRS_B,v_pair_co+1); //保存到EE中 ,注意一定是经修正后的值
            //2010-1-4 data(xb)=data(x)+1;   
        }
    }
    
    
}

/*
//功能：
//  环境压力采样值的滤波，诊断
    2014-10-8 把REMARK作为形参
*/
void FG_P_air(byte remark)
{
    sbyte sb;
    sword sw;
    
    //诊断
    
    v_pair_ad_input=v_Pair_ad_tmp;

    //deOS++;
    //获取采样值；

    if(nb_forpair<=NB_FORPAIR_LIMIT)nb_forpair++;

    if(VF_MORE1MIN_TMCCL&&t_pair_er==0)VF_PAIR_VALID_FOREE_PAIR=true;
    
        if(nb_forpair>=NB_FORPAIR_LIMIT)
        {

            //deTPS++;
            /*
            if(v_vbat_maxwhenpair-v_vbat_minwhenpair<=2&&v_vbat_maxwhenpair>=v_vbat_minwhenpair
                &&v_vbat_minwhenpair>=180//2010 add for mpxm2102 //2010-8-20 chang from 3 to 2
            )
              */
            
            //2016-6-5 放松电压条件
             /*
            if( 
                v_vbat_maxwhenpair-v_vbat_minwhenpair<=20
                &&v_vbat_maxwhenpair>=v_vbat_minwhenpair
                &&v_vbat_minwhenpair>=150//2010 add for mpxm2102 //2010-8-20 chang from 3 to 2
            ) 
               */
               
            
            if(v_vbat_minwhenpair>=150)//2016-6-6
            {   //2009-12-18 add 只有当电压波动不大时才进行判断
              //  deVBAT++;
                //获得一段时间内最大最小值的差异
                //if(v_pair_ad_input_max>v_pair_ad_input_min)sb=v_pair_ad_input_max-v_pair_ad_input_min;
                //else sb=v_pair_ad_input_min-v_pair_ad_input_max;
                //2010-7-9 chang to the following
                if(v_pair_ad_input_max>v_pair_ad_input_min)sb=v_pair_ad_input_max-v_pair_ad_input_min;
                else sb=0;
                
                v_pair_ad_64t_lst=v_pair_ad_64t;
                v_pair_ad_64t=(byte)(((word)v_pair_ad_input_min+v_pair_ad_input_max)/2);

                
                if( 
                      sb>DELTA_PAIR_INSHORTTIME
                      ||(v_pair_ad_64t<ORI_MIN_PAIR)
                      ||(v_pair_ad_64t>ORI_MAX_PAIR))//VF_ER_PAIR=true;
                {
                    //本组数据波动过大,或者超出最值范围
                    t_pair_er++;    
                } 
                
                //2016-2-28
                    if(v_pair_ad_input<ORI_MIN_PAIR)Accu_AD_Mv(MV_PAIR,MV_L_LIMIT);
                    else if(v_pair_ad_input>ORI_MAX_PAIR)Accu_AD_Mv(MV_PAIR,MV_U_LIMIT); //2016-2-29 临时	//20211005 chenwei fangkai ；//
                    else Accu_AD_Mv(MV_PAIR,MV_NORMAL);

                
                if(VF_1PAIRSGOT_PAIR){
                    //如果之前已经采集过一次了 ,则比较前后两次采集的差异
                    sw=(sword)v_pair_ad_64t_lst-v_pair_ad_64t;
                    if(sw<0)sw=-sw;
                    if(sw>DELTA_PAIR_FORAVE)t_pair_er++;
                }
                
                if(v_pair_min_afterkeyon>v_pair_ad_64t)v_pair_min_afterkeyon=v_pair_ad_64t;
                if(v_pair_max_afterkeyon<v_pair_ad_64t)v_pair_max_afterkeyon=v_pair_ad_64t;
                if(v_pair_max_afterkeyon-v_pair_min_afterkeyon>DELTA_PAIR_AFTERKEYON)
                {
                    t_pair_er++;   
                }
                
                if(VF_1PAIRSGOT_PAIR)  //如果是已经获得一次采样了
                {
                    if(v_t_pairs_1s==0){
                        v_pair_aveinlast10min=v_pair_ad_64t;
                        v_t_pairs_1s=1;
                    }
                                        
                    if(v_t_pairs_1s>=5)
                    {
                        if(v_t_pairs_1s<RUNS_FORPAIRS_LIMIT_U){ //注意<
                        
                            sw=((sword)v_pair_ad_64t-v_pair_aveinlast10min);
                            if(sw>20||sw<-20)
                            {
                                t_pair_er++;        
                            }
                        }
                        if(v_t_pairs_1s>RUNS_FORPAIRS_LIMIT_L){
                          
                            v_t_pairs_1s=1;
                            v_pair_aveinlast10min=v_pair_ad_64t;
                        }
                    }
                }

                if(t_pair_er>=NB_PAIRER_LIMIT)
                {
                    VF_ER_PAIR=true;    
                }
                
                if(!VF_ER_PAIR){//不考虑 t_pair_er 大于0的情况
                    if(!VF_1PAIRSGOT_PAIR&&t_pair_er==0){   //2013-4-26
                        v_pair_ad=v_pair_ad_64t;
                        
                    } else{
                      
                        sw=(sword)v_pair_ad*7+v_pair_ad_64t;
                        if(sw<0)sw=0;
                        v_pair_ad=sw/8;
                        
                        if(v_pair_ad>v_pair_ad_64t)v_pair_ad--;
                        if(v_pair_ad<v_pair_ad_64t)v_pair_ad++;
                    }
                    VF_1PAIRSGOT_PAIR=true;
                }
                
                
                    //VF_1PAIRSGOT_PAIR=true;  //2008-11-8 遗漏，增加  //2009-12-18 第一次算数
                
                
            } 
            else
            {
                deOS++;
            }
            //deVBAT= v_vbat_maxwhenpair;
            //deDTC=v_vbat_maxwhenpair-v_vbat_minwhenpair;//2016-6-5 张建说这个值在12-13之间变化？
            //2009-8-16　雷宏涛在菏泽发现海拔级别不对的问题：
            //  有时开钥匙海拔级别显示5级；
            //  张平验证发现：有时开钥匙EE中存储的V_PAIR_CO值为70；
            //  经查可能是这里的原因；
            //  因为：  if(!VF_ER_PAIRS&&!VF_1ST_AD_PAIRS)时才赋值 v_pair_ad；
            //  当第一次到此时，VF_1ST_AD_PAIRS＝true;
            //  此时v_pair_ad的值就不会赋值；
            //      则使用的就是上一次存储在EE中被读取并经过诊断之后的数据。
            //开钥匙4-8秒之间，再关钥匙，下一次开钥匙的0-4S（若是启动则是16S以内）可能出现压力值有问题。

            v_pair_ad_input_min=ORI_MAX_PAIR;
            v_pair_ad_input_max=ORI_MIN_PAIR;
            v_vbat_maxwhenpair= MIN_VBAT;
            v_vbat_minwhenpair= MAX_VBAT;
            nb_forpair=0;
            if(VF_1PAIRSGOT_PAIR)Rec_Pairs_ad(); //2013-8-13 add condiction 解决指示压力传感器故障的问题
        } 
        else
        {
            if(v_vbat_maxwhenpair<v_Vbat_inuse)v_vbat_maxwhenpair=v_Vbat_inuse;
            if(v_vbat_minwhenpair>v_Vbat_inuse)v_vbat_minwhenpair=v_Vbat_inuse;
            if(nb_forpair>=NB_FORPAIR_LLIMIT&&nb_forpair<=NB_FORPAIR_SAMPULIMIT) //2010-7-19 ADD NB_FORPAIR_SAMPULIMIT
            {
                //选择倒数第三个
                if(v_pair_ad_input_min>v_pair_ad_input)v_pair_ad_input_min=v_pair_ad_input;               
                if(v_pair_ad_input_max<v_pair_ad_input)v_pair_ad_input_max=v_pair_ad_input; 
            }
        }
    
    /*--------------对于v_pair_ad值的设置------------------------*/
    //当remark==REMARK_FOR_PAIRS时，最后一次诊断无故障的值；
    //当remark!=REMARK_FOR_PAIRS时，为当时实际采样的值，主要是考虑到标定的因素；
    /*--------------对于v_pair_co值的设置------------------------*/
    //最后一次诊断无故障的值；不管当remark是否等于REMARK_FOR_PAIRS；
    //如果本次上电后一直没有有效值，则使用v_pair_ee作为默认值；
    /*--------------对于v_pair_ee值的设置------------------------*/
    //当上电是从EEPROM中读取时，如果有故障，则使用海拔高度为0的采样值作为默认值；
    //      对不同的ECU由于，标定后系数不一样，默认的v_pair_ee有微小差异，但应该在170附近；
    //每次断电时，如果没有PAIRS故障，并且REMARK没有离开过REMARK_FOR_PAIRS时，将v_pair_co保存，作为下一次的v_pair_ee；
    //也就是说：
    //      ECU认为REMARK==REMARK_FOR_PAIRS时，采样值是没有人工干预的；此时若判断没有故障，则可以使用，并保存；
    //              REMARK！=REMARK_FOR_PAIRS时，采样值可能有人工干预，此时若判断没有故障，则可以使用，但不保存；
    //备注：其中REMARK_FOR_PAIRS=4；
    
    
    if(
        remark==MC_PAIR 
        &&v_vbat_minwhenpair>=180 //2013-4-8 add condiction  for pairs 
    )
    {
        //v_pair_ad=v_pair_ad_input;//当REMARK离开REMARK_FOR_PAIRS之后，直接显示采样值
                            //这是考虑自己内部标定的情况
                            //2009-6-2 备注，此时仍然是要判断PAIRS是否有故障；
        //Rec_Pairs_ad();   //2009-6-2 这句话是之前删除的,不知道什么原因删除;
        Rec_Pairs_ad();//2009-6-2 add back 
    }
    
    /*-------------------------------------------------------------*/
    //if(REMARK!=MC_NORMAL)
    //    VF_PAIR_VALID_FOREE_PAIR=true;//每次REMARK离开REMARK_FOR_PAIRS之后，被置位，则本次上电过程中的采样信息将不被记录；
    //2013-4-9 纠正回来

    //获取海拔级别；
    //2008-9-4 delete this condiction temp
    if(!VF_WITHPAIRS_PRO2||VF_ER_PAIR||VF_PAIR_BYMANNUAL_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR)
    {   
        /*无压力传感器的情况
            1、使用EEPROM中的值；
            2、不往EEPROM中写回值；
        */ 
        /*
        //2013-4-2 仍然先暂时使用这个
        VF_ER_PAIR=false; 
        v_pair_co=v_pair_ee=v_pair_ad=M0_PAIR_CAB_L;  //2010-9-13                     
        */
        //2013-3-11 CHANGE TO THE FOLLOWING如果EEPROM中的值
        
        //2014-1-16 ADD CONDICTION  &&VF_ECUHARD_WITHOUTPAIR_PAIR
        //标识只有压力标识位置有效的情况下,用EEPROM的值，否则用默认值        
        if(VF_ECUHARD_WITHPAIR_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR||VF_PAIR_BYMANNUAL_PAIR){
            v_pair_ad=v_pair_co=v_pair_ee;//使用EEPROM中的值；
        } else v_pair_ad=v_pair_co=M0_PAIR_CAB_L;
                        
    } 
    
    
    
    //2013-4-2 MOVE HERE ,原来无压力传感器的ECU的REMARK!=4时，工作不正常
    /*-------------------------------------------------------------*/
    //注意：
    //每次使用的v_pair_co都是最新的可靠的数据，
    //如果，没有采集到，则用EE中的
    //如果EE中的还有问题，则以0海拔为参数，设置为默认值；
    //if(!VF_ER_PAIRS)  //2009-6-2 delete this condiction 
    {   
        //如果没有故障，则计算级别：   
        if(Get_Pairs_Odr_and_Altitude(v_pair_co,false))t_pair_er++;   
    }
    
    //2013-8-13
    if(!VF_WITHPAIRS_PRO2) VF_ER_PAIR=false;
    
    
   // VF_ER_PAIR=false;//2013-8-1 暂时屏蔽闪9下
            
}

/*----------------------------------------------------------------------------*/
//**************load_d_dk*********************************************
//读取EE和EE+1中喷油相关参数，并进行校验；当校验不通过时，赋予0；
//TOBECHECK:返回的信息比原来刘老师少了，原来刘老师还有0标志；
/*
byte Load_PAIRS_DK(byte add)
{
     byte b;
     
     b=Load_Data_EE(add);
     if((b+1)==Load_Data_EE(add+1))   //2010-1-4 add data(xb)=data(x)+1;
     {
        return b;
     }
     else
     {
        return 0;
     }
}
*/
//功能：
//      获取上次断电时存储在EE中的海拔高度数据；
//      用于校验本次的压力情况
void Check_PairinEE_Valid(void)
{
        byte B_NOTCAB;
        byte b;
        
        
        #if M0_PAIRS==PAIRS_CAB 
            B_NOTCAB=false;//2010-1-4
            kdPairs=(sbyte)Load_D_DK(EE_PAIRS_K,false);    //2010-1-4
            if(VF_DOUBLEER_EE)B_NOTCAB=true;
            bdPairs=(sbyte)Load_D_DK(EE_PAIRS_OFFSET,false);
            if(VF_DOUBLEER_EE)B_NOTCAB=true;
            //2010-1-4
            v_Tmp_KPT=Load_D_DK(EE_P_K_TMP,false);
            if(VF_DOUBLEER_EE||v_Tmp_KPT<30||v_Tmp_KPT>140)
            {
               B_NOTCAB=true;              // 2010-4-27
               v_KPT=0;
               v_KPT_L=0;//2010-1-15
            } 
            v_KPT=(sbyte)Load_D_DK(EE_PAIR_KPT,false);
            
            if(1)  //2010-1-22 
            {
                if(VF_DOUBLEER_EE)
                {
                    v_KPT=0;    
                    B_NOTCAB=true;    //2010-1-15 delete tmp
                }
                v_KPT_L=(sbyte)Load_D_DK(EE_PAIR_KPTL,false);
                //v_KPT=0;
                if(VF_DOUBLEER_EE)
                {
                    B_NOTCAB=true;    //2010-1-15 delete tmp
                    v_KPT_L=v_KPT;    //2010-1-15 linshi 
                }
            } else 
            {
                v_KPT=v_KPT_L=0;
            }
        
        #else
            v_KPT=v_KPT_L=0;
            kdPairs=DKKP215_M256;
            bdPairs=DBKP215;
            B_NOTCAB=false;
            v_Tmp_KPT=TMP_KPT_ST;
        #endif
        
        VF_PAIR_BYMANNUAL_PAIR=VF_ECUHARD_WITHPAIR_PAIR=VF_ECUHARD_WITHOUTPAIR_PAIR=false;
        b=Load_Key(EE_WITHPAIRS);
        
        if(EE_DATA_WITHPAIRS==b)VF_ECUHARD_WITHPAIR_PAIR=true;
        else if(EE_DATA_WITHOUTPAIRS==b)VF_ECUHARD_WITHOUTPAIR_PAIR=true;
        else if(EE_DATA_PAIRS_BYMANNUAL==b)VF_PAIR_BYMANNUAL_PAIR=true;
        else  {
            B_NOTCAB=true;    //2014-11-27    
        }
        //B_NOTCAB=VF_ECUHARD_WITHPAIR_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR;//如果没有置是否有PAIRS的硬件标志,表示ECU没有标定
        
        //判断pairs是否标定过
        if(kdPairs>DELTA_K_PAIRS
            ||kdPairs<-DELTA_K_PAIRS
            ||bdPairs>DELTA_B_PAIRS
            ||bdPairs<-DELTA_B_PAIRS
            ||B_NOTCAB)
        {                   //2010-1-4 add VF_DOUBLEER_EE
            VF_NOTCAB_PAIR=true;//
            kdPairs=0;//?
            bdPairs=0;//
            v_KPT=0;  
            v_KPT_L=0;//2010-1-15      
            v_Tmp_KPT=TMP_KPT_ST;//v_Tmp_bymcu-v_Tmp_KPT
        } 
        else VF_NOTCAB_PAIR=false;

        //deTPS=b;

        VF_VALUEERINEE_PAIR=false;

        v_pair_ee=Load_D_DK(EE_PAIRS,false);          //获得环境压力采样值在EE中的备份   0x19  0x1A
        
        
        if(Get_Pairs_Odr_and_Altitude(v_pair_ee,true))VF_VALUEERINEE_PAIR=true;
        else v_pair_co=v_pair_ee;                   //将EE中存储的结果作为v_pair_co的初始值?
        
        
        //当EE中存储的PAIRS有问题时，默认为0海拔；
        if(VF_VALUEERINEE_PAIR)
        {
            v_pair_ee=v_pair_co=M0_PAIR_CAB_L;
        } else //if//(VF_ECUHARD_WITHOUTPAIR_PAIR)
        {
            v_pair_co= v_pair_ee;//在EE数据没有问题,且硬件标志为没有压力传感器时,使用EE中存储的压力 信号
                                  //这里不需要管是否ECU已经标定过,因此,即使ECU没有标定也不用指示压力故障
        }
        
        Rec_Pairs_co();//
        Get_Pairs_Odr_and_Altitude(v_pair_co,true);// 
        
        //如果是带压力传感器的ECU，允许压力值没有设定 2013-9-9
        if(VF_ECUHARD_WITHPAIR_PAIR)VF_VALUEERINEE_PAIR=false;//2013-9-9
        if(VF_ECUHARD_WITHOUTPAIR_PAIR)VF_VALUEERINEE_PAIR=false;//2013-9-9  临时，需要生产线上配合

        if(VF_ECUHARD_WITHOUTPAIR_PAIR&&VF_WITHPAIRS_PRO2)VF_SOFTCFG_ERR_PAIRS=true;//硬件无压力信号，软件有使用压力
        
        //deOS= V_PAIR_STATUS;

}

//根据环境压力采样值，获得级别和海拔高度（老模式）
//结果：
//     v_pair_odr ，v_pair_mode
//     v_pair_Altitude
//  并校验压力采样值是否合理，压力级别与海拔高度是否一致 
byte Get_Pairs_Odr_and_Altitude(byte pairs,byte blnEE)
{

    sword sw;
    
    //每一级海拔对应的PAIR大约为？？ 
    //计算海拔级别
    sw=M0_PAIR_CAB_L-(sword)pairs;       
    if(sw<0)
    {
        //if(sw<-16) 
        if(sw<-24) 
        {
            if(blnEE)VF_VALUEERINEE_PAIR=true;
            else 
            {
                VF_ER_PAIR=true;

            }
        }
        else
        {
            v_pair_odr=0;
            v_pair_mode=0;   
        }
    } 
    else if(sw>=MAP_FOR_H6000)//海拔大于=7000m
    {
        if(blnEE)VF_VALUEERINEE_PAIR=true ;
        else 
        {
            VF_ER_PAIR=true;

        }
        v_pair_odr=7;
        v_pair_mode=0; 

    } 
    else
    {   //
        v_pair_odr=(sw>>4)&0x0f;
        v_pair_mode=sw&0x0f;            
    }

    v_Pair_deg=((v_pair_odr<<4)&0xf0)+v_pair_mode;
    if(v_Pair_deg>64)v_Pair_deg=64;

    
    v_p_kpa=101-(sword)v_Pair_deg/8*5;
        
    return ((pairs>=COR_MAX_PAIR)||(pairs<=COR_MIN_PAIR));
}

//用PAIRS标定结果对PAIRS值进行修正；
void Rec_Pairs_ad(void)
{
    //sword sw; //2009-12-18 又在这里摔了一跤
    long int sw;
    
    //sw=(((((long int)v_pair_ad*(256+(sword)kdPairs))>>8)&0xff)+bdPairs);   //2009-12- 21 
    sw=(((((long int)v_pair_ad*(256+(sword)kdPairs))>>8)&0xffff)+bdPairs);   //2009-12- 21 
    //计算标定后的结果
    
    if(sw<COR_MIN_PAIR)v_pair_co=COR_MIN_PAIR;
    else if(sw>COR_MAX_PAIR)v_pair_co=COR_MAX_PAIR;
    else v_pair_co=(byte)sw;  
    
    //2010-1-4 add
    sw=(sword)v_Tmp_bymcu-v_Tmp_KPT;
    if(sw>0)sw=(sw*v_KPT)/128;
    else  sw=(sw*v_KPT_L)/128;  //2010-1-15 add 
    
    v_pair_co=v_pair_co+(sbyte)sw;
}

//用PAIRS标定结果对PAIRS值进行修正；
void Rec_Pairs_co(void)
{
    //sword sw; //2009-8-16 删除，并更改为long int 型；
    long int sw;//2009-8-16
    
    //sw=((((v_pair_ad*(256+(sword)kdPairs))>>8)&0xff)+bdPairs); 
    //2009-6-2 改的 Rec_Pairs_co有问题，这里sw的范围不够，因此会出负数；
    //2010-1-5 add------
    sw=(sword)v_Tmp_bymcu-v_Tmp_KPT;
    if(sw>0)sw=(sw*v_KPT)/128;
    else sw=(sw*v_KPT_L)/128; 
    //------------------
    sw=v_pair_co-sw;//2010-1-5 add -sw;
    sw=((sw-bdPairs)*256)/(256+(sword)kdPairs);
    //计算标定后的结果
    if(sw<ORI_MIN_PAIR)v_pair_ad=ORI_MIN_PAIR;
    else if(sw>ORI_MAX_PAIR)v_pair_ad=ORI_MAX_PAIR;
    else v_pair_ad=(byte)sw;  
    
}  

/*
        判断PAIRS是否有故障的方法：
        1、采样值是否满足正常范围；
        2、每段时间内，采样值的最大、最小值的差异是否超过正常限制值；
        3、将每次关钥匙时的认为正确PAIRS值保存起来，当检测出PAIRS可能故障时，使用最后一次认为正确的值；
        4、对于采样值的使用：
                只有认为合格时才使用，当ECU未编号时判断条件松一些；
                
*/