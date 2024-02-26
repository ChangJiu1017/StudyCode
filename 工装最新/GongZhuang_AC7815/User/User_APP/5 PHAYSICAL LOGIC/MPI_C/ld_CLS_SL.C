/*HEAD
 ***********************************************************************
 * Filename	:ld_cls_sl.c
 * Function	:Self-learning control for load		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		怠速点自学习偏浓，调整学习目标.
 *
 * V8.3.0   2019.1.24	xielian
 *		增加怠速自学习条件.
 * V8.3.1   2019.2.19	xielian
 *		老模式自学习最大限值恢复.
 * V8.3.2   2019.6.15	xielian
 *		怠速调试阶段学习目标增大.
 * V8.3.3   2019.6.24	xielian
 *		怠速自学习增加os波动幅度的条件.
 ***********************************************************************
HEAD END*/

#include "public.h"
#include "status_st.h"

#include "ee.h"
#include "add.h"

#include "look_up.h"

#include "inj_ctl.h"

#include "ad.h"

#include "sys_init.h"

#include "ld.h"

#include "sci.h"

#include "time_ccl.h"

#include "ign.h"
#include "Events.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_CLSSL_STATUS;
volatile byte VF_DTCOK_CLSSLbak;   //没有故障，满足学习的条件
volatile byte VF_EECHKED_CLSSLbak;  //EEPROM 中的自学习数据已经校验过
volatile byte VF_LIMITLEARN_CLSSLbak;
volatile byte VF_MAYLEARN_CLSSLbak;
volatile byte VF_RD_NEW_CLSSLbak;   //用于同步两次读EE的操作
volatile byte VF_STIO_CLSSLbak;
volatile byte VF_TOBELEARN_CLSSLbak;
volatile byte VF_FKSTABLE_CLSSLbak;     //反馈值稳定　2013-3-7

//volatile V_STATUS_STR _V_CLSSLH_STATUS;
volatile byte VF_TMPOK_CLSSLbak;      //温度满足要求
volatile byte VF_LEARNPOINT_CHG_CLSSLbak;   //学习点变更
volatile byte VF_LEARNECONDICTION_CLSSLbak;  //满足学习条件
volatile byte VF_CONDICTIONSTEADY_CLSSLbak;  //满足学习条件，且稳定一定时间
volatile byte VF_CONDICTIONSTEADY_LST_CLSSLbak;  //上一次，满足学习条件，且稳定一定时间
volatile byte VF_EE_ER_CLSSLbak;  //EEPROM 中，自学习值有误
volatile byte VF_MATCH_INI_CLSSLbak; //满足自学习的基本条件
//#pragma DATA_SEG DEFAULT

sbyte v_SL_cls;         //自学习值
sbyte v_SL_cls_lst;         //上一次的自学习值，用于光顺
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/
#define NB_LIMIT_STIO_CLS_NIDLE   36//当量比时允许连续偏浓或稀的圈数 
                                //30圈时很容易达到的；  //2010-4-26 from 16 to 30
#define NB_LIMIT_STIO_CLS_IDLE   150//当量比时允许连续偏浓或稀的圈数 
                                //30圈时很容易达到的；  //2010-4-26 from 16 to 30
#define DELTA_CLS_LIMIT     40//一定期间内，认为进入当量比时CLS反馈值的最大最小差异
                                //2010-4-26 chang from 10 to 20
#define DELTA_TPS_LIMIT     64 //一定期间内，认为稳定工况的TPS的变动范围允许的小级别；    //2009-12-13 change from 8 to 12, 0.3ji

#define DELTA_REV_LIMIT     30 //一定期间内，认为稳定工况的REV的变动范围允许值，单位31.2RPM；

#ifdef	TE_10K	//20210728 chenwei add 10K缸温
//#define MAX_SL_TENG             140//  140 for 采样值 212
#define MAX_SL_TENG             125//  125 for 采样值 200

#define LESS_SL_TENG             111//  111 for 采样值 185
#endif

#ifdef	TE_2K	//20210728 chenwei add 2K缸温
#define MAX_SL_TENG             80//  125 for 采样值 200

#define LESS_SL_TENG             75//  111 for 采样值 185

#endif



#define MIN_SL_TENG             40 //2013-1-16 from 80 to 60

/*----add for diag. OS--------------*/
#define DELTA_O2_IDLE_LIMIT     16//4
//#define DELTA_O2_NIDLE_LIMIT    16  //要求OS波动后才能自学习;
//#define DELTA_O2_NIDLE_LIMIT    10  //要求OS波动后才能自学习;
#define DELTA_O2_NIDLE_LIMIT    16  //要求OS波动后才能自学习;2014-1-9

#define DELTA_O2_WOT_LIMIT    25  //要求OS波动后才能自学习;2014-1-9

#define DELTA_TPS_IDLE_LIMIT    4
#define DELTA_TPS_WOT_LIMIT     4
#define DELTA_REV_IDLE_LIMIT    10
#define DELTA_REV_IDLE_M2LIMIT    20
#define DELTA_REV_WOT_LIMIT     10
#define O2_MAX_FORLITTLE_LIMIT  25

#define O2_U_MIN_LIMIT          30//在一个期间内，OS最大值的下限      //2008-11-14
                                    //当低于该值之后，不学习了；
#define DELTA_DTFI_LIMIT_BETWEEN 10//两次学习值之间差异不应该太大  //2008-11-14 本来想加，后来想一想觉得必要性不大

/*----------THE FOLLOWING ARE public variables-----------------------------------------------*/ 
//2013-2-25 自学习map的思路
sbyte v_SL_map[2][2];     //正在使用的自学习map的值；
byte v_SL_map_AD[2][2];     //正在使用的自学习map的值；

//sbyte v_SL_map_rd[4][4];  //读取的自学习map的值，包括备份；校验之后放入v_SL_map
sbyte v_SL_map_wr[4];  //准备写于到EE中的、学习到的值；
byte v_SL_add_WR[4];      //需要写入的自学习的四个点的地址
byte v_flag_eewr;         //EE需要写入的标志，刚准备写入时，v_flag_eewr=8

byte v_flag_chkwr;         //EE需要写入的标志，刚准备写入时，v_flag_chkwr

byte v_addr_for1st_eeRD;  //EE读取时，对应的最小位置的地址；

byte v_p_mod_sl;  //学点点

sbyte v_fmf_sl_idle; //怠速点自学习值；

/*
#关于学习点数
    16*16，除了16级之外，其它与基础MAP对应；
    
#存储位置
    EEPROM的第一页和第二页
    第二页为第一页的对应位置的备份，即值加一
    矩阵的横向为转速，纵向为油门，都是从小到大；

#清空自学习值方法：
    发动机静止时，
    REMARK=7,KESEI=10,
    if(BTDC==12&&BACK==12)
    
    全部清零
    即EEPROM的第1页全部为0和第2页全部为1
    

#关于怠速
    使用和学习都是TPD=0和TPD=1级的点，等量；    


#自学习MAP的读取
    运行时每圈读取一次，
    如果读取时间来不及时，置为0；
    
#自学习MAP的写入
　　运行时写入；
    学习完之后写入，
    每圈写一个字节；一共8个字节，边界点除外；

#自学习map的读取和写入的协调
    读取时，兼顾剩余的待写入的内容；
    即，把v_SL_map_wr中未写完的内容，合并到v_SL_map中；
    注意，在主循环中，先执行CheckWriting，再读取，之后再执行写入；
    因为外部写入操作结束后，内部还需要5MS的执行时间；
    

*/

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 

sbyte c_cls_max;    //在一定期间内，闭环修正量的最大值 
sbyte c_cls_min;    //在一定期间内，闭环修正量的最小值；    

byte v_nb_flu_forSL;//在一定期间内，氧传感器波动的次数

/*----2008-11-12-- for OS DIA----------------*/
byte o2_ns_max;          //在n秒内,OS的最大值;
byte o2_ns_min;          //在n秒内,OS的最大值;

byte o2_flu_th;         //学习时，氧传感器波动范围

byte v_add_sl;          //本循环自学习的点；

byte v_add_sl_steady;

byte v_ccl_sl;          //当前自学习点学习的圈数；

byte v_TPd_sl_rd;
byte v_TPmod_sl_rd;
byte v_Rev_sl_rd;
byte v_Revmod_sl_rd;
byte v_TPd_rd;
byte v_REV_rd;

sbyte v_cls_sl_cur;     //当前学习值；
sbyte v_cls_sl_learn;     //当前学习值；
sword v_cls_sl_sum;//2013-4-7

sword v_cls_uing_sum;// 2013-5-7  
sbyte v_cls_uing;// 2013-5-7 闭环值，包括自学习和当前反馈值之和

//对持续偏稀，偏偏弄得学习的处理
byte nb_ccl_r;  //浓 的次数
byte nb_ccl_l;  //稀 的次数
byte v_u_sl;    //v_UP_M128
byte v_t_aftersl_s; //持续稀或者浓之后的时间
byte v_os_flu_trans;//持续稀或者浓

byte v_relate_sl;
word v_relate_sl_sum;//2013-5-9

sbyte v_LambdaTg_SL;//2013-5-16

sbyte v_map_SL_basic;    //自学习MAP的基础值  

byte v_STIOwin_1s;        //当量比判断的窗口
byte v_OS_flu_SLpoint;    //怠速当量比判断的时间窗口内，OS波动的次数

byte v_nb_SL  ;    // 自学习的次数

byte v_nb_wr_fail;  //写入失败的次数

word v_ldb_SL;//2013-5-7 学习点对应的LDB

#define NS_FORWOTSL_LIMIT   20//2010-4-26 chang from 3 to 15， 即12s to 15s  
                    //2012-12-17 FROM 10 TO 20          
//#define NS_FORWOTSL_LIMIT   150//2010-4-26 chang from 3 to 15， 即12s to 15s  //2012-4-24  FROM 15 TO 150S          
#define NS_FORWOTSL_ULIMIT  40//2010-5-3 ADD ; 超过该时间就不学了  //2012-12-17 FROM 23 TO 40         

//2012-12-17 备注：注意NS_FORWOTSL_ULIMIT和NS_FORWOTSL_LIMIT是相关联的NS_FORWOTSL_ULIMIT=NS_FORWOTSL_LIMIT+20会好一些 

//2013-1-12
#define M0_LEARN_FACTOR 128// 2012-12-14, CHANGE BACK TO 64

#define M0_SLCLS_MIN_FORLOWTPS -6

#define T_SL_D1S_MIN    20



byte v_DTC_SL;//2015-12-17 未满足自学习条件的原因
        #define DTC_SL_O2       1  //O2 工作未满足条件
        #define DTC_SL_RUN_CON  2  //处于不适合学习的工况
        #define DTC_SL_ADJ      4  //处于用户在调节油量，或者怠速开环调节的时刻
        #define DTC_SL_TMP      8  //温度不合适
        #define DTC_SL_DTC      16 //有故障
        #define DTC_SL_T3FK     32 //T3反馈异常
        

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Init_STIO_CHK_Concidtion(void);

void Init_SL_map(void);

byte F_readSL_EE(byte iadd);

void FC_SL(void);


sbyte v_sl_gen[17];//2016-1-4 发电机系统的自学习参数

void Read_sl_WOT(void);
void Save_sl_WOT(void);
sbyte v_sl_WOT[8];//选择8个点

#define ST_ADD_WOT 244
#define ED_ADD_WOT 251
#define ADD_FOR_WOT 240//15TPD,AND 16 TPD

byte v_status_IDLESL;
extern sbyte v_lambda_sltg;

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：复位后的初始化
void FI_SLCLS_AfterReset(void)
{
        //重新初始化重来：
    //V_CLSSL_STATUS=false;//V_SL_CLS=false;
		VF_DTCOK_CLSSLbak=0;   //没有故障，满足学习的条件
		VF_EECHKED_CLSSLbak=0;  //EEPROM 中的自学习数据已经校验过
		VF_LIMITLEARN_CLSSLbak=0;
		VF_MAYLEARN_CLSSLbak=0;
		VF_RD_NEW_CLSSLbak=0;   //用于同步两次读EE的操作
		VF_STIO_CLSSLbak=0;
		VF_TOBELEARN_CLSSLbak=0;
		VF_FKSTABLE_CLSSLbak=0;     //反馈值稳定　2013-3-7
    //V_CLSSLH_STATUS=false;//V_SL_CLS=false;
		VF_DTCOK_CLSSLbak=0;   //没有故障，满足学习的条件
		VF_EECHKED_CLSSLbak=0;  //EEPROM 中的自学习数据已经校验过
		VF_LIMITLEARN_CLSSLbak=0;
		VF_MAYLEARN_CLSSLbak=0;
		VF_RD_NEW_CLSSLbak=0;   //用于同步两次读EE的操作
		VF_STIO_CLSSLbak=0;
		VF_TOBELEARN_CLSSLbak=0;
		VF_FKSTABLE_CLSSLbak=0;     //反馈值稳定　2013-3-7

    Init_STIO_CHK_Concidtion();
    if(VF_VPOR_HARD)
    {
        v_fmf_sl_idle=F_readSL_EE(0);
        v_nb_wr_fail=0;
    }
    
    VF_TOBELEARN_CLSSL=false;
    v_flag_eewr=0;//注意不能放到VF_VPOR_HARD外面
    
    //2013-3-12
   // FL_SL_EE_wr();//针对运行时，还没写完的情况；
    
    v_cls_sl_cur=0;
    v_cls_sl_sum=0;
    
    v_cls_uing=0;
    v_cls_uing_sum=0;
    
    F_Clr_SL1s();//t_SL_d1s=0;
    Init_SL_map();
    v_nb_SL=0;
    v_SL_cls=0;

    v_TPd_sl_rd=0;
    v_Rev_sl_rd=0;
    v_TPmod_sl_rd=0;
    v_Rev_sl_rd=0;
    
    VF_RD_NEW_CLSSL=true;
}

//功能：
//  关闭钥匙时，对自学习参数的处理
//    可以考虑对自学习的map进行校验
void F_ActionSL_AfterKeyoff(void)
{
    return;
    //2013-4-13 觉得必要性不大，直接删除了
    if(!VF_EECHKED_CLSSL){
        VF_EE_ER_CLSSL=false;
        F_readSL_EE(v_addr_for1st_eeRD);
        
        if(VF_EE_ER_CLSSL){
            wdrive_iic_ext((word)256+v_addr_for1st_eeRD,0);
            wdrive_iic_ext((word)512+v_addr_for1st_eeRD,1);
        }
        
        if(v_addr_for1st_eeRD<255){
            v_addr_for1st_eeRD++;
                
        }
        else{
            VF_EECHKED_CLSSL=true;
        }
    }
}


/*
    查表得到自学习值：
    v_addr_for1st_eeRD
*/
void FL_SL_EE_rd(void){
    byte j;
    byte iadd;
    byte b,b1;
    byte i;
    
    sword sw,sw1;
    
    if(VF_ISWRITING_EE)return;//确定

    sw=(sword)v_TPd_rd-v_TPd;
    if(sw<0)sw=-sw;
    
    sw1=(sword)v_REV_rd-v_rev_deg;
    if(sw1<0)sw1=-sw1;

    v_add_sl=v_rev_odr+((v_TPS_odr<<4)&0xf0);

    iadd=0;
    
    //油门零级时统一为一个点，即原来怠速的O值，与转速无关
    if(!VF_E0_TPS) {
        if(v_rev_mode>15){
            if(v_rev_odr<0x0f){
                v_add_sl++;
                iadd++;
            } 
        } 
        
        //怠速点按照0油门级来算
        if(v_TPS_mode>7){
            if(v_TPS_odr<0x0f){
                v_add_sl+=16;
                iadd+=2;
            }
        }
    } else v_add_sl=0;        


    #ifndef IS_GEN
    
        //判断学习MAP位置是否改变
        if(//2013-4-9
        //(v_addr_for1st_eeRD==v_add_sl) //这个有问题，这是判断学习点的
        //工况区间没有变化
        //((v_TPd_sl_rd==v_TPS_odr)&&(v_Rev_sl_rd==v_rev_odr)) //2013-5-9 这个有问题
        ((v_TPd_sl_rd==v_TPS_odr)&&(v_Rev_sl_rd==v_rev_odr)&&v_addr_for1st_eeRD==v_add_sl) //2013-5-9 这个有问题

        ||
        (     //在区间交界点是
            sw<=2&&sw1<=4
            &&(!((VF_E0_TPS||VF_E0LAST_TPS)&&(VF_E0_TPS!=VF_E0LAST_TPS)))  //不是怠速和非怠速的切换
            &&(v_addr_for1st_eeRD!=0)  //2013-3-12 add
            &&(v_add_sl!=0)
        )
		||(VF_AQAFTER_PNINI_SYSINI&&VF_E0_TPS)
		){
            VF_LEARNPOINT_CHG_CLSSL=false;    
        } else VF_LEARNPOINT_CHG_CLSSL=true;

    #else
        //2016-1-1 对于发电机
        //判断学习MAP位置是否改变
        if(//2013-4-9
        //(v_addr_for1st_eeRD==v_add_sl) //这个有问题，这是判断学习点的
        //工况区间没有变化
        //((v_TPd_sl_rd==v_TPS_odr)&&(v_Rev_sl_rd==v_rev_odr)) //2013-5-9 这个有问题
        ((v_TPd_sl_rd==v_TPS_odr)&&(v_Rev_sl_rd==v_rev_odr)&&v_addr_for1st_eeRD==v_add_sl) //2013-5-9 这个有问题

        ||
        (     //在区间交界点是
            //sw<=2&&sw1<=4
            sw<=20&&sw1<=4    //2016-1-1
            &&(!((VF_E0_TPS||VF_E0LAST_TPS)&&(VF_E0_TPS!=VF_E0LAST_TPS)))  //不是怠速和非怠速的切换
            &&(v_addr_for1st_eeRD!=0)  //2013-3-12 add
            &&(v_add_sl!=0)
        )){
            VF_LEARNPOINT_CHG_CLSSL=false;    
        } else VF_LEARNPOINT_CHG_CLSSL=true;
    #endif
    

    //if(VF_LEARNPOINT_CHG_CLSSL||!VF_RD_NEW_CLSSL){//2013-10-22 发现怠速OK学习不对，原因为第一次启动时VF_RD_NEW_CLSSL=true，没有读学习点了
    if(VF_LEARNPOINT_CHG_CLSSL||!VF_RD_NEW_CLSSL||(VF_STARTS_ENG&&VF_BFIRE_ENG)){//2013-10-22 发现怠速OK学习不对，原因为第一次启动时VF_RD_NEW_CLSSL=true，没有读学习点了
        //2013-3-15 add condiction 
        //如果工况点有变化，或者两次完全读好，则开始读EEPROM      
        //Init_SL_map();
        //如果一圈剩余的时间不够读取EE，则设置为0

        if(VF_RD_NEW_CLSSL){
            v_SL_map[0][0]=0;
            v_SL_map[0][1]=0;
            v_TPd_sl_rd=v_TPS_odr;
            v_Rev_sl_rd=v_rev_odr;
            v_TPd_rd=v_TPd;
            v_REV_rd=v_rev_deg;
            
            
            //怠速时，归结为一个点
            if(VF_E0_TPS){
                v_Revmod_sl_rd=0;
                v_TPmod_sl_rd=0;
                v_Rev_sl_rd=0;
                    
            }
            
            v_addr_for1st_eeRD=v_add_sl;
            
            v_p_mod_sl=iadd;
            
            i=0;
        } else{
            v_SL_map[1][0]=0;
            v_SL_map[1][1]=0;
            i=1;
        }


        //for(i=0;i<=1;i++)
        //初始化调试时，不使用自学习之
        if(!(VF_1ST_INI_SYSINI)){
            for(j=0;j<=1;j++){
                
               /*
                if(!VF_OLDCA_CAS){
                    //i=10;
                    break;
                } 
                 *///原来的8bitECU 有这个，可能是怕影响点火


                {
                    //当前工况点
                    //v_rev_odr;
                    //v_TPS_odr;
                    
                    //计算需要读取的地址；
                    //b=v_rev_odr+j&0x03;
                    b=v_Rev_sl_rd+j;
                    if(b>0x0f)b=0x0f;
                    
                    b1=v_TPd_sl_rd+i;
                    if(b1>0x0f)b1=0x0f;
                    
                    iadd=(b1)*16+b;
                    if(v_TPd_sl_rd==0){
                        iadd=0;//油门怠速的情况，直接用怠速，即怠速的学习不分级
                    }   
                }
                /*
                for(k=0;k<=3;k++){
                    if(v_SL_add_WR[k]==iadd)break;   
                }
                if(VF_TOBELEARN_CLSSL&&k<=3){
                    //说明，需要读取的点，在需要写入的自学习的点里面
                    *( char *)(v_SL_map[0]+j)=v_SL_map_wr[k];    
                    deVBAT++;
                } 
                */
                //2013-3-6
                *( char *)(v_SL_map_AD[i]+j)=iadd;
                if(VF_TOBELEARN_CLSSL&&(v_SL_add_WR[0]==iadd)){
                    //说明，需要读取的点，在需要写入的自学习的点里面
                    *( char *)(v_SL_map[i]+j)=v_SL_map_wr[0];    

                } 
                else{

                    b=F_readSL_EE(iadd);
                    *( char *)(v_SL_map[i]+j)=b;
                    
                    if(iadd==0)v_fmf_sl_idle=b;
                }

                //2016-9-9
                if(iadd>=ADD_FOR_WOT){
                  
                    if( 
                        iadd>=ST_ADD_WOT
                            &&iadd<=ED_ADD_WOT
                      )
                    {
                        *( char *)(v_SL_map[i]+j)=v_sl_WOT[iadd-ST_ADD_WOT];      
                    } 
                    else if(iadd<ST_ADD_WOT)
                    {
                        *( char *)(v_SL_map[i]+j)=v_sl_WOT[0];      
                    } 
                    else
                    {
                        *( char *)(v_SL_map[i]+j)=v_sl_WOT[ED_ADD_WOT-ST_ADD_WOT]; //2016-9-9 纠正     
                    }
                }
     
                
            }
        }
        
        VF_RD_NEW_CLSSL=!VF_RD_NEW_CLSSL;
    }

    if(VF_RD_NEW_CLSSL){
        v_Revmod_sl_rd=v_rev_mode;
        v_TPmod_sl_rd=v_TPS_mode;

        if(v_TPS_odr<v_TPd_sl_rd){
            v_TPmod_sl_rd=0;    
        } else if(v_TPS_odr>v_TPd_sl_rd){
            v_TPmod_sl_rd=16;    
        }
        
        if(v_rev_odr<v_Rev_sl_rd){
            v_Revmod_sl_rd=0;    
        } else if(v_rev_odr>v_Rev_sl_rd){
            v_Revmod_sl_rd=32;    
        }

        if(VF_E0_TPS){
              v_Revmod_sl_rd=0;
              v_TPmod_sl_rd=0;
        }
    }
}

/*
    将自学习结果写入到EE中
    写入结束后，置标志位；
    
*/
void FL_SL_EE_wr(void){
    byte i;
    byte j;

    if(VF_ISWRITING_EE){
        return;//确定
    }
    if(VF_TOBELEARN_CLSSL){
        //deTPS=v_flag_eewr+v_flag_chkwr*4;

        //deOS++;
        if(v_flag_eewr>0){
            i=v_flag_eewr&0x01;
            //write_eeprom(29,v_SL_add_WR[j]+256+256*(word)i,2,3,v_SL_map_wr[j]+i);
            wdrive_iic_ext(v_SL_add_WR[0]+256+256*(word)i,v_SL_map_wr[0]+i);
            v_flag_eewr--;
            if(v_flag_eewr==0){
                v_flag_chkwr=2;
                //VF_TOBELEARN_CLSSL=false;
            }
        } else if( v_flag_chkwr>0){
            i=v_flag_chkwr&0x01;
            
            j=rdrive_iic_ext(v_SL_add_WR[0]+256+256*(word)i);
            
            if(j!=(byte)(v_SL_map_wr[0]+i)){
                v_flag_eewr=2;
                v_nb_wr_fail++;
            }
            v_flag_chkwr--;
            if(v_flag_chkwr==0&&v_flag_eewr==0)VF_TOBELEARN_CLSSL=false;
        }
        //deVBAT=v_flag_eewr+v_flag_chkwr*4;
    }
}


/*
    怠速初始化之后，清除怠速四周点的学习值；
*/
void FC_SL_EE_afterIni(void)
{
    
    wdrive_iic_ext((word)256,0);
    wdrive_iic_ext((word)512,1);
  //仅仅清除 怠速点的 学习值  2013-3-19
/*
    wdrive_iic((word)256+1,0);
    wdrive_iic((word)512+1,1);

    wdrive_iic((word)256+16,0);
    wdrive_iic((word)512+16,1);

    wdrive_iic((word)256+17,0);
    wdrive_iic((word)512+17,1);
  */  
}

//功能：
//  校核自学习条件
void F_SL_ByCLS(void)
{
    sword sw;

    byte b;
    byte b1;
    byte b2;

    
    //--------------------------------------------------------------------------------------
    /*----------判断自学习条件，获得自学习map---------------------------------------------*/

    if(VF_E0_TPS){
        v_STIOwin_1s=0x1f;    //for 3.1s
        
    } else{
        //v_STIOwin_1s=0x07;    //for 0.7s
        v_STIOwin_1s=0x1f;    //for 3.1s
    }
    
    //VF_ENAQ_SYSINI
    
    
    
    //温度条件
        if(((MAX_SL_TENG>v_te)&&VFF_NOZZLEINI_TESHIGH_DTC)||(VF_E0_TPS&&VF_AQAFTER_PNINI_SYSINI))VF_TMPOK_CLSSL=true;//te<125 并且 tw>60 ？ 冷机和热机阶段不允许自学习？
        else VF_TMPOK_CLSSL=false;
    
    if(
        (!VF_O2WORK_OSDIAG)||(!VF_O2CHANGED_OSDIAG)//氧传感器未工作
        ||VF_OSER_OSDIAG    //氧传感器故障                              //2013-1-12

        ||VF_STARTS_ENG     //启动阶段
        ||VF_CF_CFCI        //如果是断油
        ||VF_JLEAVEIDLE_ENG //离开怠速 //2013-4-11 DELETE
        ||VF_JBACKIDLE_ENG  //回怠速   //2013-4-11 DELETE
        ||(VF_GT30_IDLETG_REV&&VF_E0_TPS&&!VF_1ST_INI_SYSINI)//2014-1-19 ADD FOR NOT LEARNING
        ||v_rev_625rpm<REV62D5_1000 //转速低于1000RPM时
        ||(v_rev_625rpm>REV62D5_9250) //2013-7-7 add for lx200 sl

        ||(v_FMF_SCI_Nidle!=0&&!VF_E0_TPS)//2014-1-10 ADD CONDICTION FOR NOT LEARNING
        ||(VF_ENAQ_SYSINI&&VF_E0_TPS)//2013-10-31 ADD CONDICTION FOR CLS_SL ,2015-12-17 ADDVF_E0_TPS 
        
        ||!VF_TMPOK_CLSSL   //温度条件不满足
        
        ||!VF_DTCOK_CLSSL   //有故障

        ||VF_T3FKLIMIT_INJCTL||VF_T3ER_INJCTL  //T3反馈异常
        ||no_t3_fk_run>2//2013-7-14 ADD FOR CLS_SL    //2013-7-31

        //||VF_CICORECING_CFCI        //有再给油修正时   //2013-4-10 DELETE 移到学习内部
        //||(!VF_TRANSLT10_TRANS&&VF_RICH_OSDIAG)        //工况稳定的判断  //2013-3-8　delete &&VF_STEADY_CLSSL条件
                                    // U修正仍然存在时不宜学习，因为此时的LAMBDA目标可能不是1，而是0.85等等；如果这里修正到1，那么后面热车就会过稀
                                    //2013-3-13 ADD VF_RICH_OSDIAG, 过渡时，如果偏稀，也学习
                                    //2013-4-10 DELETE 移到学习内部
        
        ||(!VF_E0_TPS&&((v_UP_M128>0)||(v_TPS_odr<=7)||(v_rev_625rpm<=REV62D5_4500)))//2018-12-29 避开怠速点v_UP_M128>0//2016-5-23 ADD ,临时关闭开环点的自学习,IN F_SL_ByCLS
        )
    {
        
        v_DTC_SL=0;
        if(
            (!VF_O2WORK_OSDIAG)||(!VF_O2CHANGED_OSDIAG)//氧传感器未工作
            ||VF_OSER_OSDIAG    //氧传感器故障                              //2013-1-12
        )
        {
            v_DTC_SL+=DTC_SL_O2;    
        }
        
        //特殊工况
        if(VF_STARTS_ENG     //启动阶段
        ||VF_CF_CFCI        //如果是断油
        ||VF_JLEAVEIDLE_ENG //离开怠速 //2013-4-11 DELETE
        ||VF_JBACKIDLE_ENG  //回怠速   //2013-4-11 DELETE
        ||(VF_GT30_IDLETG_REV&&VF_E0_TPS)//2014-1-19 ADD FOR NOT LEARNING
        ||v_rev_625rpm<REV62D5_1000 //转速低于1000RPM时
        ||(v_rev_625rpm>REV62D5_9250) //2013-7-7 add for lx200 sl
       // ||v_UP_M128>0//2016-5-23 ADD ,临时关闭开环点的自学习,F_SL_ByCLS
        )
        {
            v_DTC_SL+=DTC_SL_RUN_CON;    
        }

        if((v_FMF_SCI_Nidle!=0&&!VF_E0_TPS)//2014-1-10 ADD CONDICTION FOR NOT LEARNING
        ||(VF_ENAQ_SYSINI&&VF_E0_TPS)//2013-10-31 ADD CONDICTION FOR CLS_SL ,2015-12-17 ADDVF_E0_TPS 
        )
        {
            v_DTC_SL+=DTC_SL_ADJ;    
        }
        
        if(!VF_TMPOK_CLSSL)   //温度条件不满足
        {
            v_DTC_SL+=DTC_SL_TMP;    
        }
        
        if(!VF_DTCOK_CLSSL) //2016-1-2 发现少了一个！
        {
            v_DTC_SL+=DTC_SL_DTC;    
        }


        if(VF_T3FKLIMIT_INJCTL||VF_T3ER_INJCTL  //T3反馈异常
            ||no_t3_fk_run>2//2013-7-14 ADD FOR CLS_SL    //2013-7-31
        )
        {
            v_DTC_SL+=DTC_SL_T3FK;    
        }
        
        //===============================================
        VF_STIO_CLSSL=false;
        VF_FKSTABLE_CLSSL=false;

        VF_LEARNECONDICTION_CLSSL=false;
        VF_CONDICTIONSTEADY_LST_CLSSL=VF_CONDICTIONSTEADY_CLSSL=false;

        F_Clr_SL1s();//t_SL_d1s=0;
        v_OS_flu_SLpoint=0;
        
        
        VF_MATCH_INI_CLSSL=false;  //不满足初始化时，怠速调试的条件
        
        VF_RICH_MORE_CLS=false;    //2013-3-13
        VF_LEAN_MORE_CLS=false;
        v_t_aftersl_s=0;
        v_ccl_sl=0;
        v_LambdaTg_SL=0;
        
        v_status_IDLESL=255;
        
        
    } 
    else
    {   
        v_DTC_SL=0;
            if(VF_1S_TMCCL){
                if(v_t_aftersl_s>0)v_t_aftersl_s--;
            }
        //怠速初始化的条件
        //    if(v_nb_flu_lstIdle>2&&v_nb_OS_flu>15)VF_MATCH_INI_CLSSL=true;

        //2013-10-31
                if(!VF_LIMITFK_CLS&&v_nb_OS_flu>15)VF_MATCH_INI_CLSSL=true; //2013-10-31 CHANGE TO 
        
        //怠速时，需要避开   2013-4-12
            if(VF_E0_TPS&&(v_rev_625rpm>REV62D5_2000)&&!VF_AQAFTER_PNINI_SYSINI) {
                v_ccl_sl=0;    
            }

        //在非怠速时，才考虑自学习；
        //计算学习点
            
            if(!VF_LEARNPOINT_CHG_CLSSL){
                if(v_rev_mode>15){
                    if(v_rev_odr<0x0f){
                        b1=v_rev_mode/2-7;
                    } else{
                        b1=8;
                    }
                } else{
                    b1=8-v_rev_mode/2;
                }
                
                //怠速点按照0油门级来算
                if(!VF_E0_TPS){
                    if(v_TPS_mode>7){
                        if(v_TPS_odr<0x0f){
                            b=v_TPS_mode-7;
                        } else{
                            b=8;    
                        }
                    } else{
                        b=(8-v_TPS_mode);
                    }
                }else
                {
                    b=8;
                }
                
                b1*=b;//最大值为64
            
                //if(VF_E0_TPS)b1=64;
                
                if(v_ccl_sl<255)v_ccl_sl++;  
                
                if(v_ccl_sl==1){
                    v_cls_sl_cur=v_dmLambda;
                    v_cls_sl_sum=(sword)v_cls_sl_cur*64; //2013-4-7
                    
                    //2013-5-7
                    v_cls_uing=v_SL_cls;
                    v_cls_uing_sum=(sword)v_cls_uing*32;//2013-5-7
                    
                    v_relate_sl=b1;
                    v_relate_sl_sum=(word)v_relate_sl*16;//2013-5-9
                    
                    v_u_sl=v_UP_M128;
                    
                    v_LambdaTg_SL=v_lambda_sltg;//v_LambdaTg;//2013-5-16
                    //2013-3-14需要 保证二者的统一
                    //v_add_sl_steady=v_add_sl;
                    //v_map_SL_basic=v_SL_map[sb1][sb2];
                   
                    nb_ccl_r=0;
                    nb_ccl_l=0;  
                    v_map_SL_basic=*( char *)(v_SL_map[0]+v_p_mod_sl);//v_SL_map[sb1][sb2];    
                    v_add_sl_steady=v_addr_for1st_eeRD;
                    v_ldb_SL=v_ldb;//2013-5-7
                    
                }else{
                    v_cls_sl_sum=v_cls_sl_sum-v_cls_sl_cur+v_dmLambda;
                    v_cls_sl_cur=v_cls_sl_sum/64;//2013-4-7

                    //2013-5-7
                    v_cls_uing_sum=v_cls_uing_sum-v_cls_uing+v_SL_cls;
                    v_cls_uing=v_cls_uing_sum/32;//2013-4-7
                    
                    v_relate_sl_sum=v_relate_sl_sum-v_relate_sl+(word)b1;  //2013-5-9
                    v_relate_sl=v_relate_sl_sum/16;//计算当前点与被学习点的相关系数
                    
                    v_u_sl=(sbyte)(((sword)v_u_sl*15+v_UP_M128)/16);
                    //计算在这个工况点内持续稀的次数和持续浓的次数，
                    //if(!VF_E0_TPS) //2013-4-10 DELETE
                    {
                        if(VF_RICH_OSDIAG){
                            if(nb_ccl_r<120)nb_ccl_r++;
                        } else if(nb_ccl_l<120)nb_ccl_l++;
                    }
                    
                    
                    
                }
                
                /*
                    工况边界有两种情况
                      1\在两个工况点的正中间
                          会导致要学习的点不同；
                      2、在一个工况点的两边
                          会导致使用的学习MAP不同；
                */
                
            } 
            
        /*----判断OS是否处于当量比---------------------------*/
            //在一定的时间窗口内判断
            if((t_SL_d1s&v_STIOwin_1s)==0){
                Init_STIO_CHK_Concidtion();
            }

            {
                  
                  
                  //判断反馈量
                  if(c_cls_max<v_dmLambda)c_cls_max=v_dmLambda; 
                  if(c_cls_min>v_dmLambda)c_cls_min=v_dmLambda; 
                  //判断OS值
                  if(o2_ns_max<v_O2_ad)o2_ns_max=v_O2_ad;
                  if(o2_ns_min>v_O2_ad)o2_ns_min=v_O2_ad;
                  //if(!((VF_CICORECING_CFCI)&&(!VF_CFLST_CFCI)))
                  if(!((!VF_CICORECING_CFCI)&&(!VF_CFLST_CFCI)))    //2008-11-16 发现之前VF_CICOJEND_INJ的置位都有问题;
                  {    //有断油或正在恢复供油时不判断
                      o2_ns_max=56;
                      o2_ns_min=0;    
                  }

              /*----判断是否满足条件：自学习------------------------*/
              /*---------------------判断条件-------------------*/

              //OS处于当量比工作的判断
                  
                  if(VF_E0_TPS){
                      //b=NB_LIMIT_STIO_CLS_IDLE;
                      o2_flu_th=DELTA_O2_IDLE_LIMIT;
                  } else {
                      //b=NB_LIMIT_STIO_CLS_NIDLE+v_rev_odr*10; //2010-4-26 chang from /8 to /2;  //2013-1-16 add *8
                      o2_flu_th=DELTA_O2_NIDLE_LIMIT;
                      
                      if(VF_WOT_TPS){
                          //2014-1-9
                          o2_flu_th=DELTA_O2_WOT_LIMIT;
                      }
                  }

              //反馈值稳定    
                  sw=c_cls_max-c_cls_min;
                  b2=DELTA_CLS_LIMIT+v_rev_odr;///2; //2010-4-26 from /4 to /2;
              
                  
                  //注意这段语句的位置
                  if(!VF_E0_TPS){
                      if(sw>b2){
                          VF_FKSTABLE_CLSSL=false;
                          F_Clr_SL1s();//t_SL_d1s=0;
                      }
                      //if((nb_Rich_max>b)||(nb_Lean_max>b))
                      /*
                      if(v_nb_flu_forSL
                      {
                          VF_STIO_CLSSL=false;
                          t_SL_d1s=0;
                      } */
                  }
              //在时间窗口内作出判断
                  if(!VF_E0_TPS){
                      //非怠速时，在时间窗口内判断氧传感器是否处于当量比
                      if((t_SL_d1s&v_STIOwin_1s)==v_STIOwin_1s){
                            /*
                                其它条件都正常情况下，反馈值可以计入学习的条件
                                    处于当量比
                                    或者反馈到头
                                    或者怠速情况下，也需要氧传感器反馈起来
                            */
                            VF_STIO_CLSSL=false;
                            VF_FKSTABLE_CLSSL=false;

                            if(//(nb_Lean_max<=b)&&(nb_Rich_max<=b)
                                //(v_nb_flu_forSL>=2)
                                (v_nb_flu_forSL>=4) //2014-1-17  避免因失火导致的误学习
                                &&(o2_ns_max>=O2_U_MIN_LIMIT)
                                &&((o2_ns_max-o2_ns_min)>=o2_flu_th)){
                                VF_STIO_CLSSL=true;
                            }

                            if(sw<=b2)VF_FKSTABLE_CLSSL=true;
                            
                            if(!VF_STIO_CLSSL||!VF_FKSTABLE_CLSSL){
                                F_Clr_SL1s();//t_SL_d1s=0;
                            }
                      }
                      
                      
                  } else{
                      //怠速时不使用时间窗口，只要OS波动达到一定次数就行
                      if(v_OS_flu_SLpoint>3)
					  {
                          if((o2_ns_max>=O2_U_MIN_LIMIT)&&((o2_ns_max-o2_ns_min)>=o2_flu_th))
						  {
							 VF_STIO_CLSSL=true; 
							 VF_FKSTABLE_CLSSL=true;
						  }						  						  
                              
                      }
                      //2013-3-19发现 这里 漏了对VF_STIO_CLSSL和VF_FKSTABLE_CLSSL值 FALSE 
                      else{
                          VF_STIO_CLSSL=false;
                          VF_FKSTABLE_CLSSL=false;    
                      }
                  }
                  
                  //2013-4-10 add 
                  if(
                      (VF_CICORECING_CFCI||(!VF_TRANSLT10_TRANS)
                      &&VF_RICH_OSDIAG)
                  ) {
                      //如果有给油修正，或者过渡修正时，且偏浓，
                      //则当作作　不满足学习条件
                      VF_STIO_CLSSL=false;
                  }

            }
        
        //根据已知的条件，判断是否开始学习
            //是否满足学习条件
            if(
                (VF_STIO_CLSSL||(VF_LIMITFK_CLS))    //2010-4-27 add condiction ||(VF_MAX_TPS&&VF_LEARNINGATWOT_CLSSL)
                &&VF_FKSTABLE_CLSSL
            )
            {
                VF_LEARNECONDICTION_CLSSL=true;    
            } 
            else 
            {
                VF_LEARNECONDICTION_CLSSL=false;
            }
        
            //满足学习条件并且稳定一段时间
            VF_CONDICTIONSTEADY_LST_CLSSL=VF_CONDICTIONSTEADY_CLSSL;
            if(VF_LEARNECONDICTION_CLSSL&&t_SL_d1s>T_SL_D1S_MIN){
                //学习条件已经稳定
                VF_CONDICTIONSTEADY_CLSSL=true;
                
            } else{
                VF_CONDICTIONSTEADY_CLSSL=false;
            }
        
        //持续偏浓或持续偏稀的情况的处理，重点针对过渡点，即稳定工况遇不到的点    
            if(!VF_LEAN_MORE_CLS&&!VF_RICH_MORE_CLS
                &&(!VF_CONDICTIONSTEADY_CLSSL||(v_OS_flu_SLpoint<=1))//2013-4-10 add v_OS_flu_SLpoint<=1
                &&!VF_1ST_INI_SYSINI)
                {//保持第一次的值   
                //2013-4-10 ADD CONDICTION !VF_E0_TPS       VF_1ST_INI_SYSINI
                if(
                    (((VF_LEARNPOINT_CHG_CLSSL&&v_ccl_sl>32)||v_ccl_sl>64)&&!VF_E0_TPS)//2013-4-10 add
                    ||v_ccl_sl>=255
                ){
                    //工况有变化，并在该工况持续一定圈数
                    
                    //if(nb_ccl_r<=5||nb_ccl_l<=5||v_os_flu_chg<=3)//浓的次数太少，即一直稀
                    //if(nb_ccl_r<=5||nb_ccl_l<=5)//浓的次数太少，即一直稀
                    {
                        //v_cls_sl_cur+=v_LambdaTg_SL/4;//2013-4-7 //2013-11-21 DELETE 影响正常v_cls_sl_cur的计算，因为老是会进到这里

                        //if(v_u_sl>=0) //2013-4-9 DELETE
                        {
                            //不偏稀设置
                            if(v_cls_sl_cur>5&&nb_ccl_r<5&&!VF_E0_TPS){ //一直稀且需要增油； 2013-4-10 FROM 0 TO 5
                                //反馈结果要增
                                //则表示需要学习，往 浓 的学
                                //计算学习值；
                                FC_SL();//保持第一次的值                            
                                VF_LEAN_MORE_CLS=true;
                                VF_TOBELEARN_CLSSL=false;
                                v_os_flu_trans=0;
                                v_t_aftersl_s=15;
                                v_nb_SL-=4;
                            }
                        }
                    /*
                    } else if(nb_ccl_l<=5)//稀的次数太少，即一直农
                    { */
                        //if(v_u_sl<=0) //2013-4-9 DELETE
                        /*
                        {
                            //不偏浓设置
                            //&&!VF_E0_TPS
                            if(v_cls_sl_cur<-5&&nb_ccl_l<5){ //一直浓且需要减油
                                //反馈结果要增
                                //则表示需要学习，往 稀 的学
                                //计算学习值；
                                if(VF_E0_TPS){//2013-4-10 ADD 
                                    if(v_cls_sl_cur<-10)v_cls_sl_cur=-10;
                                } 
                                  
                                FC_SL();                         
                                VF_TOBELEARN_CLSSL=false;
                                
                                
                                VF_RICH_MORE_CLS=true;
                                
                                v_t_aftersl_s=15;
                                v_os_flu_trans=0;
                                v_nb_SL-=4;
                            }
                        }
                        */
                    }
                }
            }
            
        //2019-1-20 PETER                                            // 显示在NORMAL模式的DTC显示这个条件
              v_status_IDLESL=255;                                   // 当该值为0时，满足怠速UP>=60的学习条件，每个BIT为0的条件
              /*if((v_Vbat_inuse>=205)&&(v_t_eng_t_s>60))v_status_IDLESL-=1;  
			  //1：电压采样值>=205;且发动机运行的时间超过60s
              if(v_teng_inuse>=80&&v_teng_inuse<=180)v_status_IDLESL-=2;                
			  //2：发动机温度te在50和140之间
              if(((v_fmf_O<=15)&&(v_cls_sl_cur>0))||((v_fmf_O>=-10)&&(v_cls_sl_cur<0)))v_status_IDLESL-=4;            
			  //4：o值大于30且学习值为正，也不学，0值小于-10且学习值为负，则不学
              if(v_t_OS_Lt20_to_now<15) v_status_IDLESL-=8;//  30 改为15，满足一次条件只学1次                 
			  //8：改为上一次氧传感器波动距离现在的时间//氧传感器最近一次小于20，到现在的时间小于20s
              if(VF_CONDICTIONSTEADY_CLSSL&&(v_ccl_sl>=255))v_status_IDLESL-=16; 	
			  //16：发动机工况稳定，且工作圈数超255//(VF_CONDICTIONSTEADY_CLSSL) &&
              if(v_LambdaTg_SL>=65)v_status_IDLESL-=32;                 
			  //32：UP>=65
              if(VF_O2CHANGED_OSDIAG&&(v_nb_OS_flu>20))v_status_IDLESL-=64;               
			  //64:氧传感器波动
              if(VF_E0_TPS&&(v_t_E0_1s>20)&&(v_rev_15d6rpm>(v_n0_low-12))&&(v_rev_15d6rpm<(v_n0_low+12)))v_status_IDLESL-=128;                        
			  //128：油门处于怠速 且怠速超过20s，且转速高于目标低怠速-200rpm,且低于目标低怠速+200rpm
			  
			  if(VF_1ST_INI_SYSINI)v_status_IDLESL=255;*/ 
                                                                        //学习如果增，最多学5，如果减，最多-5 
                    
            
        //计入学习    
            if(VF_RICH_MORE_CLS||VF_LEAN_MORE_CLS){
                //如果需要过浓或过稀的情况学习
                //2013-4-9 add VF_LEAN_MORE_CLS
                if(v_t_aftersl_s>0){//2013-5-13 delete  VF_LEAN_MORE_CLS
                      //if(v_os_flu_trans>0){ //2013-4-8 FROM >4 TO >0  //2013-5-13 delete  VF_LEAN_MORE_CLS
                      if(v_os_flu_trans>1){ //2013-5-18 from 0 to 1
                          VF_TOBELEARN_CLSSL=true;

                          VF_RICH_MORE_CLS=VF_LEAN_MORE_CLS=false;
                          v_nb_SL+=16;
                      }

                }else{
                    VF_RICH_MORE_CLS=VF_LEAN_MORE_CLS=false;//2013-3-14    
                }
                /*
                //2013-4-9
                VF_TOBELEARN_CLSSL=true;
                VF_MAYLEARN_CLSSL=true;
                VF_RICH_MORE_CLS=VF_LEAN_MORE_CLS=false;
                v_nb_SL+=16;
                */
                F_Clr_SL1s();//t_SL_d1s=0;//2013-3-14
                VF_CONDICTIONSTEADY_CLSSL=false;
            } else if
            
            (
                    (
                        v_OS_flu_SLpoint>=2&&//2013-410 add 
                        (
                        //学习条件稳定，且学习点虽然变化，但上一次满足最小点数要求
                        (VF_LEARNPOINT_CHG_CLSSL&&VF_CONDICTIONSTEADY_CLSSL&&v_ccl_sl>32) 
                        //学习条件稳定，且学习点虽然没有变化，但是是已经满足最大点数上限；
                        ||((VF_CONDICTIONSTEADY_CLSSL) &&v_ccl_sl>=255)
                        //学习条件刚不满足，但是学习点已经满足最小点数要求
                        ||(!VF_CONDICTIONSTEADY_CLSSL&&VF_CONDICTIONSTEADY_LST_CLSSL&&v_ccl_sl>64)
                        ) 
                    )
                    ||
                      (
                          !v_status_IDLESL   //2019-1-20
                    
                    )
            )
            {
                //if(!VF_TOBELEARN_CLSSL) 2013-3-10 DELETE
                {
                    if(REMARK!=MC_LOADB)//2012-12-15    2013-1-12
                    {
                        
                        //if(VF_E0_TPS)
                        #if M0_MFUD1==INJ109
					
                            if(!VF_1ST_INI_SYSINI)
							{
								if(v_LambdaTg_SL>32)
								{
									v_cls_sl_cur+=v_LambdaTg_SL/16;//2014-1-9
								}
							}
							else
							{
								if(VF_E0_TPS&&VF_AQAFTER_PNINI_SYSINI)
								{
									v_cls_sl_cur+=v_LambdaTg_SL/8;
								}
								else
								{
									v_cls_sl_cur+=v_LambdaTg_SL/16;
								}
							}
														
                            /*if(VF_E0_TPS)
                            {
                                v_cls_sl_cur+=v_LambdaTg_SL/4;//2014-1-9
                            }*/
                        #else
                            //v_cls_sl_cur+=v_LambdaTg_SL/4;//2013-4-7 //2016-6-2 delete
                            //2016-6-9
							if(!VF_1ST_INI_SYSINI)
							{
								if(v_LambdaTg_SL>32)
								{
									//v_cls_sl_cur+=v_LambdaTg_SL/4;
									if(VF_E0_TPS&&(v_teng_inuse>180))// 150 if(VF_E0_TPS&&(v_te>80))
									{
										v_cls_sl_cur+=v_LambdaTg_SL/12;// /6 v_LambdaTg_SL/4
									}
									else
									{
										v_cls_sl_cur+=v_LambdaTg_SL/16;//2017-3-12 ADD IN F_SL_ByCLS
									}                                
								}
							}
							else
							{
								if(VF_E0_TPS&&VF_AQAFTER_PNINI_SYSINI)
								{
									v_cls_sl_cur+=v_LambdaTg_SL/8;
								}
								else
								{
									v_cls_sl_cur+=v_LambdaTg_SL/16;
								}
							}
                            
                        #endif

                        //限制学习范围
                        if(!v_status_IDLESL)
                        {
                            if(v_cls_sl_cur>3)v_cls_sl_cur=3;// 10 5
                            if(v_cls_sl_cur<-3)v_cls_sl_cur=-3;  
                        }
                         
                        if(VF_1ST_INI_SYSINI){
                            if(VF_E0_TPS){
                                //初始化状态下的学习结果的处理
                                if(VF_AQAFTER_PNINI_SYSINI
                                    &&VF_MATCH_INI_CLSSL
                                    &&!VF_AQOK_SYSINI
                                    &&(v_ccl_sl>=255)//初始化时，时间要足够长
                                    //&&(v_rev_156ave<(v_n0_low+15))
                                ){
                                    //初始化时，不使用学习值，因此，与学习基数无关，也不需要马上写入
                                    
                                    F_SL_idleIni(v_cls_sl_cur,true);
                                    
                                } 
								else 
								{
									F_SL_idleIni(v_cls_sl_cur,false); 
								}
                                //等待补充
                                //怠速初始化后的学习还需要更改；因为怠速初始化后不往EE中写入学习值的。
                            }
                        }
                        else{
                            //计算学习值；
                            FC_SL();  
                                                      
                            VF_TOBELEARN_CLSSL=true;
                        }

                        if( v_ccl_sl>=255)v_ccl_sl=0;//2013-3-10
                        v_nb_SL+=8;
                        
                    }
                }
            }

       //只要不满足学习的基本条件：t_SL_d1s就清零，否则一直累加
       //重新初始化学习条件    
    }
    
    
    /*条件改变后的初始化*/

    if(VF_LEARNPOINT_CHG_CLSSL){
        v_ccl_sl=0;
    //2013-4-10 更正的位置
        nb_ccl_r=0;
        nb_ccl_l=0;
        
        v_OS_flu_SLpoint=0; 
    }
    

   //在工况变动之后使用学习值
    //if(VF_TOBELEARN_CLSSL&&VF_MAYLEARN_CLSSL)
    if(VF_TOBELEARN_CLSSL)
    {
        if(REMARK!=MC_LOADB) {
            if(v_flag_eewr==0&&v_flag_chkwr==0)v_flag_eewr=2;
              //2013-4-9 增加v_flag_chkwr，否则写入一办之后v_flag_eewr油杯只为
              
        }
    }
    
    if((t_SL_d1s&v_STIOwin_1s)==0){
         Init_STIO_CHK_Concidtion();
    }
    

}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

void FC_SL(void){
    sword sw;
    sbyte sb1;
    sword sw_limit;
    byte badd;
    
    if(!VF_TOBELEARN_CLSSL){
      
        v_nb_SL+=2;//2013-4-7
        
        
        v_cls_sl_learn=v_cls_sl_cur;//2013-3-9
        
        //当有U修正，并且要减油时，需要特殊处理
        if(v_msMu>0&&v_cls_sl_learn<0){
            sw=(sword)v_cls_sl_learn+v_msMu;  //2013-5-7  扣去U那一部分
            if(sw>0)v_cls_sl_learn=0;
            else {
                if(sw<-10)sw=-10;
                v_cls_sl_learn=sw;
            }
            //v_cls_sl_learn+=v_msMu/2;
            
            //if(v_cls_sl_learn>0)v_cls_sl_learn=0;        
        }
        
        
        sw=((sword)v_ccl_sl*v_cls_sl_learn)/256; //v_ccl_sl_learn最大为255
        //sw=((v_ccl_sl*(long int)v_cls_sl_cur)/256); //v_ccl_sl_learn最大为255
        //sw=sw*v_relate_sl/128;           // v_relate_sl最大为64
        //sw=sw*v_relate_sl/64;           // v_relate_sl最大为64，2013-4-8 FROM /128 TO /64
        sw=sw*v_relate_sl/128;           // v_relate_sl最大为64，2013-4-8 FROM /128 TO /64  2013-5-16

        if(sw==0){
            if(v_cls_sl_learn>=3)sw=1;
            if(v_cls_sl_learn<=-5)sw=-1;   //2013-4-10 FROM 0 TO -5
        }
        
        if(v_te>=LESS_SL_TENG){
            sw/=4;//2013-5-14    对学习值进行限制
            if(sw<0)sw/=2;//2013-5-14
        }
        
        v_cls_sl_learn=sw;
        
        sw+=v_map_SL_basic;

        //sb1=v_TPd/8+30; //2012-12-14  2013-1-12
        //2013-5-7 change to 
        
        //2013-5-9 CheckSlLimit
        
        sw=CheckSlLimit(v_ldb_SL,sw,v_add_sl_steady);//2013-5-9  //2013-5-13 add v_add_sl_steady
        
      /////////////////////////////////////////////////////////////
        sw_limit=(sword)v_cls_uing+v_cls_sl_cur;
        //deVBAT=sw_limit;//2013-5-7
        
        
        //2013-5-7增加限制,学习点对被学习点的限制
        if(v_cls_sl_learn>0&&v_map_SL_basic>0){
            //如果学习结果要加
            if(v_map_SL_basic>sw_limit)sw=v_map_SL_basic;
        } else if(v_cls_sl_learn<0&&v_map_SL_basic<0){
            if(v_map_SL_basic<sw_limit)sw=v_map_SL_basic;
        }
      /////////////////////////////////////////////////////////////
        
        v_cls_sl_learn=sw-v_map_SL_basic;//2013-4-17
        
        v_SL_map_wr[0]=sw;
        v_SL_add_WR[0]=v_add_sl_steady;

           /*
            2016-9-7 v_sl_WOT
            v_add_sl=v_rev_odr+((v_TPS_odr<<4)&0xf0);
        */
 
            badd=(v_add_sl&0x0f);
            
            if(badd>=1&&badd<4)badd=4;
            if(badd>=11)badd=11;
            
            if(badd>=4&&badd<=11)
            {
                badd-=4;
                
                sw=(sword)v_sl_WOT[badd]*3+v_SL_map_wr[0];
                sw/=4;
                
                if(sw==0)
                {
                    if(v_SL_map_wr[0]<0)sw--;
                    if(v_SL_map_wr[0]>0)sw++;
                }
                
                v_sl_WOT[badd]=(sbyte)sw;
                
                //deTPS++;
            }

        
        for(sb1=0;sb1<=3;sb1++){
            if((*( char *)(v_SL_map_AD[0]+sb1))==v_add_sl_steady){
                (*( char *)(v_SL_map[0]+sb1))=v_SL_map_wr[0];
                //break;    
            }
        }
        //怠速点的处理
        if(v_add_sl_steady==0){
            v_fmf_sl_idle =v_SL_map_wr[0];
            FR_idleClsAve((sbyte)((sword)v_fmf_sl_idle-v_map_SL_basic)); //2013-4-11 add 

            //2013-4-17 
        		vc_CLS_2-=v_cls_sl_learn;
        		if(vc_CLS_2>60)vc_CLS_2=60;
        		else if(vc_CLS_2<-60)vc_CLS_2=-60;

        }  
        v_flag_eewr=0;
    }
    //2013-4-7 纠正  v_ccl_sl=0的位置 
    v_ccl_sl=0;//2013-3-15 每次学习之后，重新开始计数 
    //每次学习之后，波动重新累计
    v_OS_flu_SLpoint=0;

}

//2013-5-9 check sl limit
sbyte CheckSlLimit(word fmf,sword swl,byte sladd)
{
    sbyte sb1;
    
    sb1=fmf/12+30;
    
    //if(sladd==0)sb1=20;//2014-1-19 limit idle sl for add
    if(sladd==0)sb1=30;//30//2015-5-20 limit idle sl for add
    
    if(swl>sb1)   swl=sb1;
    
    if(sladd>15 && sladd<=50)sb1/=2;//2013-5-13
    
    if(sladd==0)sb1=40;//2014-1-19 limit idle sl for dec

    if(swl<-sb1)  swl=-sb1;
    
    return swl;
}

//功能：
//  初始化当量比判断的条件
void Init_STIO_CHK_Concidtion(void)
{
    

    c_cls_max=-120;
    c_cls_min=120;
    
    o2_ns_max=0;
    o2_ns_min=80;
    
    v_nb_flu_forSL=0;//b波动 的次数
    //2010-4-26
    //nb_os_con=0;

}


/*
    初始化自学习map
*/
void Init_SL_map(void){
    v_SL_map[0][0]=0;
    v_SL_map[1][0]=0;
    v_SL_map[0][1]=0;
    v_SL_map[1][1]=0;

}


byte F_readSL_EE(byte iadd){
    byte b;
    byte b1;
                    
    b=rdrive_iic_ext(iadd+(word)256);
    b1=rdrive_iic_ext(iadd+(word)512);
    
    if((byte)(b+1)!=b1){
        b=0;
        VF_EE_ER_CLSSL=true;        
    }

    return b;
}


//read sl for WOT   3500rpm to 8000rpm,2016-9-7
void Read_sl_WOT(void)
{
    byte i;
    
    for(i=ST_ADD_WOT;i<=ED_ADD_WOT;i++){
        CLR_WDG;
        v_sl_WOT[i-ST_ADD_WOT]=F_readSL_EE(i);
    }
}

void Save_sl_WOT(void)
{
    byte i;

    for(i=ST_ADD_WOT;i<=ED_ADD_WOT;i++){
        CLR_WDG;
        wdrive_iic(i+(word)256,v_sl_WOT[i-ST_ADD_WOT]);
        wdrive_iic(i+(word)512,v_sl_WOT[i-ST_ADD_WOT]+1);
    }
    
}


/*
  关于自学习的限制条件
      1　无故障，已PN初始化
      2  氧传感器正常工作
      3　怠速点需要有怠速波动 v_nb_flu_lstIdle
      4　无修正：VF_CICORECING_CFCI，过渡，U等等；
      5   温度限制
      6   氧传感器已经反馈工作

    if(!(rev156ave>=M0_MAX_RPM_CONIDLE||rev156ave<M0_MIN_RPM_CONIDLE 
            ||!VF_IDLE_ENG||VF_CICORECING_CFCI//co_ci!=0
            ||VFF_INJ_DTC||VFF_TPS_DTC    //2010-2-7 ADD CONDICTION 
            ||v_nb_flu_lstIdle<4    //2010-3-17     //2010-5-10 12 to 2   //2010-5-10 delete
                                     //2011-11-30 恢复 v_nb_flu_lstIdle<2条件，并稍微加严
            ||(((v_nb_OS_flu<6)||!VF_NOINIT_SYSINI||v_nb_flu_lstIdle<3)&&(v_nb_OS_flu<MAX_O2FEDBTIME))   //from 20 to 10  , add v_nb_flu_lstIdle<2
            ||VF_CFLST_CFCI||(teng<M0_MIN_RZT_TI_SH)||(teng>M0_MAX_RZT_TI_SH))) //2008-11-12 delete VF_OSCHANGELITTLE_OSDIAG

  特殊情况下，自学习加快：
     1　初始化中
          	if(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG
          	          &&!VF_ENAQ_SYSINI&&VF_IDLE_ENG&&t_s_idleOS_change>=15
          	          &&VF_IDLECONDICTION_MATCH_SYSINI
          	          &&(v_nb_flu_lstIdle>2||t_s_idleOS_change>120)
          	          &&VFF_NOZZLEINI_TESHIGH_DTC//2013-3-3
     2、PN初始化了，但未怠速初始化
     
     3、某些标定模式下
          BASIC LOAD
          或者海拔标定
      
  关于怠速自学习
      1、自学习值放在RAM中更好，不马上写入，否则会造成突变
      2、自学习后需要置一些标志位和复位某些变量；
          

    		VF_GETIDLEFK_CLS=false;
    		vc_CLS_IDLE_av=0;
    		vc_CLS_2=0;
    		vc_tw1_IDLESC=0;//
    		v_FMF_SCI_idle=0;
    		
    		//2010-7-15 add 
    		second_ffk_idle=0;
    		v_nb_flu_lstIdle=0;
        leansdtav=0;  //		;2007-3-30,banked	
        leansdtsum=0;//OFF_SET_SUM*256;//2008-12-3 DELETEOFF_SET_SUM

*/

/*
void s_ti_sh_get(byte rev156ave,byte teng,byte tps_odr,byte rev_odr)
{
    sword temp_x;
    byte blnOk; 
    sbyte sb_cls2;
    
   //2009-8-25 仍是需要加严条件;    // VF_CICFGT5_CFCI  VF_CICORECING_CFCI
    if(!(rev156ave>=M0_MAX_RPM_CONIDLE||rev156ave<M0_MIN_RPM_CONIDLE 
            ||!VF_IDLE_ENG||VF_CICORECING_CFCI//co_ci!=0
            ||VFF_INJ_DTC||VFF_TPS_DTC    //2010-2-7 ADD CONDICTION 
            ||v_nb_flu_lstIdle<4    //2010-3-17     //2010-5-10 12 to 2   //2010-5-10 delete
                                     //2011-11-30 恢复 v_nb_flu_lstIdle<2条件，并稍微加严
            ||(((v_nb_OS_flu<6)||!VF_NOINIT_SYSINI||v_nb_flu_lstIdle<3)&&(v_nb_OS_flu<MAX_O2FEDBTIME))   //from 20 to 10  , add v_nb_flu_lstIdle<2
            ||VF_CFLST_CFCI||(teng<M0_MIN_RZT_TI_SH)||(teng>M0_MAX_RZT_TI_SH))) //2008-11-12 delete VF_OSCHANGELITTLE_OSDIAG
//         条件 M0_MAX_RPM_CONIDLE 由106更改为116
//          条件(v_nb_OS_flu<6)更改为(v_nb_OS_flu<20)  
    {                   
    		if(VF_1S_TMCCL&&(second_ffk_idle<255))second_ffk_idle++;
    }
    else{
        second_ffk_idle=0;
    }

    if(VF_O2CHANGED_OSDIAG&&VF_IDLE_ENG)
    {
        if(VF_1S_TMCCL&&(t_s_idleOS_change<255))t_s_idleOS_change++;
    } else t_s_idleOS_change=0;
    //2010-3-19
	//if(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG&&VF_IDLE_ENG&&t_s_idleOS_change>=15&&VF_IDLECONDICTION_MATCH_SYSINI&&(v_nb_flu_lstIdle>5||t_s_idleOS_change>60)) //2010-3-16
	//2010-7-14 CHANG TO THE FOLLOWING
	///if(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG&&VF_IDLE_ENG&&t_s_idleOS_change>=15&&VF_IDLECONDICTION_MATCH_SYSINI&&(v_nb_flu_lstIdle>2||t_s_idleOS_change>120)) //2010-3-16
	//2010-7-15 add condiction VF_ENAQ_SYSINI
	if(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG
	          &&!VF_ENAQ_SYSINI&&VF_IDLE_ENG&&t_s_idleOS_change>=15
	          &&VF_IDLECONDICTION_MATCH_SYSINI
	          &&(v_nb_flu_lstIdle>2||t_s_idleOS_change>120)
	          &&VFF_NOZZLEINI_TESHIGH_DTC//2013-3-3
	          ) //2010-3-16
	{    
       // &&(v_nb_flu_lstIdle>4||t_s_idleOS_change>30)
       blnOk=false;//
        if(v_nb_flu_lstIdle>2) //2010-7-7 add condiction      
        {
            blnOk=true;
        }
        else if(t_s_idleOS_change>120)//2010-7-7 add          //2010-7-10 from 30 to 60
        {
             blnOk=false;
        }
        sb_cls2= leansdtav/2+vc_CLS_2/2;
        sb_cls2+=8;
        vc_tw3offset=FS_AQOK_FLAG(vc_tw1_IDLESC,sb_cls2,vc_tw3offset,v_FMF_SCI_idle,blnOk);                    
    		
    		VF_GETIDLEFK_CLS=false;
    		vc_CLS_IDLE_av=0;
    		vc_CLS_2=0;
    		vc_tw1_IDLESC=0;//
    		v_FMF_SCI_idle=0;
    		
    		//2010-7-15 add 
    		second_ffk_idle=0;
    		v_nb_flu_lstIdle=0;
        leansdtav=0;  //		;2007-3-30,banked	
        leansdtsum=0;//OFF_SET_SUM*256;//2008-12-3 DELETEOFF_SET_SUM
		
		//2010-7-8
        if(!blnOk)
        {
            VF_O2WORK_OSDIAG=false;    
        }
	}
	else if(vc_tw1_IDLESC>=-1&&vc_tw1_IDLESC<=1) //2010-3-26 add this condiction
	{
    //2008-10-31 连续VF_MATCHINI_CLS=true，持续M0_T_CON_FOR_IDLELEARN_S时间之后，即获得初始化的数据；?
        //if((second_ffk_idle>=M0_T_CON_FOR_IDLELEARN_S)&&VF_4S_TMCCL){
        if((second_ffk_idle>=M0_T_CON_FOR_IDLELEARN_S)&&VF_2S_TMCCL&&(v_FMF_SCI_idle==0)){ //2010-2-4
        		
        		//temp_x=(96-(sword)rev156ave)/2+leansdtav;
        		//2010-10-13 DELETE CONDICTION  rev156ave
        		temp_x=leansdtav;       		
        		
        		//2008-9-4 change to the following
        		if(temp_x>55)temp_x=55;
        		if(temp_x<-40)temp_x=-40;
        		
        		if(!VF_GETIDLEFK_CLS){ //第一次得到怠速反馈值；

        			VF_GETIDLEFK_CLS=true;
        			vc_CLS_IDLE_av=(sbyte)temp_x;
        		}
        		vc_CLS_IDLE_av=(sbyte)(((sword)temp_x+vc_CLS_IDLE_av)/2);//需要考虑符号
    		            //0XB0,0XB1
        }
	}
	
	//2011-1130 ADD 
	if(VF_OSER_OSDIAG) vc_CLS_IDLE_av=0;
	
    return;
}
*/
