#include "status_st.h"
#include "public.h"

#include "ee.h"
#include "add.h"

/*
     关于发冲的说明：
        当在非怠速时检测到转速的变化趋势，急降--急升--再急降，认为是偏稀引起的失火；
           
*/
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_NOTIDLEMF_STATUS ;
//#pragma DATA_SEG DEFAULT

sbyte v_MissfireNotIdle;        //相当于刘老师原先的modi_fachong, 只是中心值变为0；
sbyte v_FMF_MissfireNotIdle;    //发冲流量修正系数；
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define M0_OFFSET_MISSFIRENOTIDLE       0//相当于刘老师原来的offset_fach，只是中心值变为0；
#define MO_RUNS_FOR_WAITFACH    30
#define M0_REV2000_15D6 128             //
#define M0_DREV_LIMIT   64              //两次转速差异的极限值，超出极限值时，认为不合理；
#define M0_SUM_DRPM_ULIMIT      127     //
#define M0_SUM_DRPM_LLIMIT      -127    //
#define M0_DOWNMUCH_EXPECTION       -51     //sum_rpmchange156小于该限值时，bu认为转速突将
#define M0_DOWNMUCH_THREHOLD    -11     //sum_rpmchange156小于该限值时，认为转速突将
#define M0_STEP_FOR_1MISSFIRE   4       //
#define M0_CO_MISSFIRE_U        10
#define M0_C0_MISSFIRE_L        -10     //
#define M0_SUM_DRPM_UP_THREHOLD 20      //
#define M0_DRPM_RISE_TO_DOWN    5       //
#define M0_RISEMUCH_EXPECTION   72      //上升超过该值时，认为有异常
#define M0_RUNS_RISEMUCH_LIMIT  28      //转速突增的圈数的限值
#define M0_NB_S_FORRESET_WHEN_REMARKE6  5 //
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte wait_run_fach;     //   
byte drun_number;       //
sword sum_rpmchange156; //
byte max_sum_rchange;   //
word rev_15d6_lst;     //上一次的rev_15d6
byte nb_1ss_when_remarke6;//当REMARK==6时，经过的1S的次数；
    
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Annuation_MissfireINC(byte remark,sbyte MissfireNotidle_IdleSC);
void Rpm_Inc_Set(word rev_15d6);
void Rpm_Dec_Set(word rev_15d6);
void Clr_bToo_much(word rev_15d6);
void Rpm_Change_Init(word rev_15d6,sword dRev);
void Next_Rpm_C_t(word rev_15d6,sword dRev);
void Down_End(word rev_15d6,sword dRev);
void Rise_End(word rev_15d6,sword dRev);
void RPM_CH_WR_IALL(word rev_15d6);
void Check_MissfireNotidle_valid(void);
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
 
//复位后对发冲参数的初始化
void Init_MissfireNotIdle_afterReset(void)
{
    V_NOTIDLEMF_STATUS=false;//2008-12-15
    if( 0&&VF_VPOR_HARD)
    {   /*
        v_MissfireNotIdle=Load_Data_EE(EEMODI_FACHONG);
        Check_MissfireNotidle_valid();     */
    }
}

void Detect_MissfireNotIdle(sbyte fmf_trans1,byte remark,word rev_15d6,sbyte MissfireNotidle_IdleSC)
{

    if( 0 )
    { /*
        Annuation_MissfireINC(remark,MissfireNotidle_IdleSC);
        //增加标志位：VF_DECNOTIDLE_RPM， VF_INCNOTIDLE_RPM， VF_BDOWN_TOOMUCH_RPM，VF_BRISE_TOOMUCH_RPM
        drun_number++;
        if(wait_run_fach>0)wait_run_fach--;
        if(!VF_WORM_ENG)
        {
                if((fmf_trans1>4)||(fmf_trans1<-4))wait_run_fach=MO_RUNS_FOR_WAITFACH;
                if(wait_run_fach>0)VF_REVDOWNMUCH_NOTIDLEMF=false;
                if(VF_YSTDC_INJPHASE)
                {
                        if(VF_IDLE_ENG)wait_run_fach=MO_RUNS_FOR_WAITFACH;
                        if(rev_15d6>M0_REV2000_15D6)
                        {
                                dRev=(sword)rev_15d6-rev_15d6_lst;
                                                
                                if((dRev>M0_DREV_LIMIT)||(dRev<-M0_DREV_LIMIT))dRev=0;
                                //对转速的增减情况进行判断
                                if(dRev==0)
                                {
                                        VF_REVDECNOTIDLE_NOTIDLEMF=false;
                                        VF_REVINCNOTIDLE_NOTIDLEMF=false;                
                                }
                                else if(dRev>0)
                                {       //对本圈转速上升的情况进行处理
                                        if(VF_REVDECNOTIDLE_NOTIDLEMF)
                                        {
                                                Rpm_Inc_Set(rev_15d6);
                                                //goto down_end 
                                                Down_End( rev_15d6, dRev);               
                                        }
                                        else
                                        {       //rpm_r_r_test
                                                if(VF_REVINCNOTIDLE_NOTIDLEMF)
                                                {
                                                        //rpm_r_r
                                                        sum_rpmchange156+=dRev;
                                                        if(sum_rpmchange156<M0_SUM_DRPM_ULIMIT)
                                                        {
                                                                Rpm_Inc_Set(rev_15d6);        
                                                        }
                                                        else
                                                        {       //goto next rpm_c_t
                                                                //当转速一直上升，累计增量超过一定值之后，从头开始
                                                                Next_Rpm_C_t(rev_15d6, dRev);
                                                        }
                                                } 
                                                else
                                                {       //上一圈的转速不增也不减，而本圈上升
                                                        //rpm_nc_r
                                                        Rpm_Inc_Set(rev_15d6);
                                                        if(sum_rpmchange156>=0)
                                                        {
                                                                //rpm_r_r
                                                                sum_rpmchange156+=dRev;
                                                                if(sum_rpmchange156<M0_SUM_DRPM_ULIMIT)
                                                                {
                                                                        Rpm_Inc_Set(rev_15d6);        
                                                                }
                                                                else
                                                                {       //goto next rpm_c_t
                                                                        //当转速一直上升，累计增量超过一定值之后，从头开始
                                                                        Next_Rpm_C_t(rev_15d6, dRev);
                                                                }
                                                        }
                                                        else
                                                        {       //此时sum_rpmchange156小于0
                                                                //说明之前累计在减，而本圈又增；
                                                                //goto down_end
                                                                Down_End(rev_15d6, dRev);
                                                        }
                                                }
                                                
                                        }
                                } 
                                else
                                {
                                        //对本圈转速下降的情况进行处理
                                        //rpm_dec_calcul
                                        if(VF_REVINCNOTIDLE_NOTIDLEMF)
                                        {       //之前转速上升，本圈转速下降
                                                Rpm_Dec_Set(rev_15d6);
                                                Rise_End(rev_15d6, dRev);  //转速上升到头了      
                                        } 
                                        else
                                        {
                                                //rpm_d_d_test
                                                if(VF_REVDECNOTIDLE_NOTIDLEMF)
                                                {       //rpm_d_d
                                                        sum_rpmchange156+=dRev;
                                                        if(sum_rpmchange156<M0_SUM_DRPM_LLIMIT)
                                                        {
                                                                sum_rpmchange156=M0_SUM_DRPM_LLIMIT;                
                                                        }
                                                        Rpm_Dec_Set(rev_15d6);
                                                }
                                                else
                                                {       //rpm_nc_d
                                                        Rpm_Dec_Set(rev_15d6);
                                                        if(sum_rpmchange156<=0)
                                                        {
                                                                //之前累计不上升，本圈下降       
                                                                sum_rpmchange156+=dRev;
                                                                if(sum_rpmchange156<M0_SUM_DRPM_LLIMIT)
                                                                {
                                                                        sum_rpmchange156=M0_SUM_DRPM_LLIMIT;                
                                                                }
                                                                Rpm_Dec_Set(rev_15d6);
                                                        } 
                                                        else
                                                        {
                                                                //之前累计上升，本圈下降
                                                                Rise_End(rev_15d6, dRev);
                                                        }
                                                        
                                                }
                                        }
                                }
                        }
                        else
                        {      //当转速低于某个限值时
                               //RPM_CH_WR_IALL
                               RPM_CH_WR_IALL(rev_15d6);       
                        }
                }//ELSE RETURN //非压缩循环
        } 
        else
        {       //处于暖车过程
                //RPM_CH_WR_IALL
                RPM_CH_WR_IALL(rev_15d6);
        }
    */}
}

//获得发冲流量修正系数
word CO_FMF_MissfireNotIdle(sword fmf,byte tps_odr)
{
    sbyte sL;

    if( 0)
    {
     /*   //if((tps_odr>0)&&(!VF_IDLE_ENG)&&((!VF_O2WORK_OSDIAG)||VF_O2ER_OSDIAG))
        if((tps_odr)&&(!VF_IDLE_ENG)&&((!VF_O2WORK_OSDIAG)||VF_OSER_OSDIAG))
        {
                v_FMF_MissfireNotIdle=v_MissfireNotIdle-M0_OFFSET_MISSFIRENOTIDLE;
                //sL=tps_odr*2+1;
                sL=(tps_odr<<1)&0xfe+1 ;
                if(v_FMF_MissfireNotIdle<-sL)v_FMF_MissfireNotIdle=-sL;
                //sL=tps_odr*2+4;
                //sL=(tps_odr<<1)&0xfe+4 ;
                sL+=3;  //这样可以省16bytes
                if(v_FMF_MissfireNotIdle>sL)v_FMF_MissfireNotIdle=sL; 
                if(VF_STARTS_ENG)
                {
                        if(v_FMF_MissfireNotIdle<0)v_FMF_MissfireNotIdle=0;
                }
                fmf=fmf+v_FMF_MissfireNotIdle;
                if(fmf<10)fmf=10;        
        } else
        {
                v_FMF_MissfireNotIdle=0;
        } */
    }
    
    return fmf;
}

void Save_MissfireNotIdle(void)
{
    if( 0 )
    {
        /*
        //当发动机运行1分钟以后，递减一次
        if(VF_MORE1MIN_TMCCL)
        {
                if(VF_O2WORK_OSDIAG)
                {       
                        v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;        
                } else
                {
                        v_MissfireNotIdle--;
                        Check_MissfireNotidle_valid();
                }
                Save_Data_EE(EEMODI_FACHONG,(byte)v_MissfireNotIdle);
        } */
    }
}
/*---------local functions--------------------------------------------------------*/
//发冲系数的递减
void Annuation_MissfireINC(byte remark,sbyte MissfireNotidle_IdleSC)
{
    if( 0)
    {
       /* 
        //当REMARK==6时，让发冲指数复位
        if(MissfireNotidle_IdleSC!=0)
        {
                v_MissfireNotIdle-=MissfireNotidle_IdleSC;
                if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_CO_MISSFIRE_U;                
                if(v_MissfireNotIdle<M0_C0_MISSFIRE_L)v_MissfireNotIdle=M0_C0_MISSFIRE_L;                
        }
        //处于标定状态下，让发冲系数为0
        if((remark==6)||(remark==8))
        {
                if(VF_1S_TMCCL&&(nb_1ss_when_remarke6<=M0_NB_S_FORRESET_WHEN_REMARKE6))nb_1ss_when_remarke6++;
                if(nb_1ss_when_remarke6>=M0_NB_S_FORRESET_WHEN_REMARKE6)
                {
                        v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;                
                }
        } 
        else
        {
                nb_1ss_when_remarke6=0;        
        }   */
    }
}

//转速增加时的初始化，设置转速上升标志
void Rpm_Inc_Set(word rev_15d6)
{
        VF_REVDECNOTIDLE_NOTIDLEMF=false;
        VF_REVINCNOTIDLE_NOTIDLEMF=true;
        rev_15d6_lst=rev_15d6;                
}

//设置转速下降标志
void Rpm_Dec_Set(word rev_15d6)
{
        VF_REVDECNOTIDLE_NOTIDLEMF=true;
        VF_REVINCNOTIDLE_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6;                
}

//功能 ：清除已经得到的可能发冲的标记
void Clr_bToo_much(word rev_15d6)
{
        VF_REVDOWNMUCH_NOTIDLEMF=false;
        VF_REVRISEMUCH_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6;                
}

void Rpm_Change_Init(word rev_15d6,sword dRev)
{
        sum_rpmchange156=dRev;
        drun_number=0;
        rev_15d6_lst=rev_15d6;        
}

//功能：从头开始进行发冲检测
//euqal to rise_small, down_small
void Next_Rpm_C_t(word rev_15d6,sword dRev)
{
        Clr_bToo_much(rev_15d6);
        Rpm_Change_Init(rev_15d6, dRev);        
}

//功能：
//      当（之前转速累计增量小于0，或上一圈转速为递减；）
//      而本圈转速为上升时，调用 Down_End，
//      判断是否是转速突降，
//      若是突降，增判断是否是发冲，若发冲，进行发冲处理
void Down_End(word rev_15d6,sword dRev)
{       //此时sum_rpmchange156应该是负数？？
    if( 0)
    {
    /*    
        if((sum_rpmchange156<M0_DOWNMUCH_EXPECTION)||         //有异常
                (sum_rpmchange156>M0_DOWNMUCH_THREHOLD))  //转速下降不够多
        { //DOWN SMALL
                Next_Rpm_C_t(rev_15d6, dRev);
        }
        else
        {
                max_sum_rchange=M0_DRPM_RISE_TO_DOWN-sum_rpmchange156;
                if(VF_REVRISEMUCH_NOTIDLEMF&&VF_REVDOWNMUCH_NOTIDLEMF)
                {       //在此检测到发冲， 进行处理
                        //leans_too_much
                        v_MissfireNotIdle+=M0_STEP_FOR_1MISSFIRE;
                        if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_CO_MISSFIRE_U;
                } 
                else
                {
                        //down_too_1stly
                        //检测到要发冲的第一个下降
                        VF_REVRISEMUCH_NOTIDLEMF=false;
                        VF_REVDOWNMUCH_NOTIDLEMF=true;
                        Rpm_Change_Init(rev_15d6, dRev);
                }
        }  */
    }
}

//功能：
//     当转速持续上升到，出现转速下降时，判断之前的转速是否是突增 
//     即是否是VF_BDOWN_TOOMUCH_RPM 
void Rise_End(word rev_15d6,sword dRev)
{
    if( 0)
    {
 /*       
        if(sum_rpmchange156<=M0_SUM_DRPM_UP_THREHOLD)
        {
                //上升的不够多
                //RISE_SMALL
                Next_Rpm_C_t(rev_15d6,dRev);
        } 
        else
        {
                //trise_too_much
                if(VF_REVDOWNMUCH_NOTIDLEMF)
                {       //上次转速图降；
                        //本次累计上升大于
                        if((sum_rpmchange156>=max_sum_rchange)//本次上升值 比上次下降值大M0_DRPM_RISE_TO_DOWN
                                &&(sum_rpmchange156<M0_RISEMUCH_EXPECTION)
                                &&(drun_number<M0_RUNS_RISEMUCH_LIMIT) )
                        {
                                VF_REVRISEMUCH_NOTIDLEMF=true;
                                Rpm_Change_Init(rev_15d6,dRev);                
                        }else
                        {       //RISE_SMALL
                                Next_Rpm_C_t(rev_15d6,dRev);
                        }
                } 
                else
                {
                        Next_Rpm_C_t(rev_15d6,dRev);        
                }
        
        } */
    }
}

void RPM_CH_WR_IALL(word rev_15d6)
{
    if( 0 )
    {
        /*
        wait_run_fach=MO_RUNS_FOR_WAITFACH;
        sum_rpmchange156=0;
        max_sum_rchange=0;
        
        VF_REVDECNOTIDLE_NOTIDLEMF=false;
        VF_REVINCNOTIDLE_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6; 
        Clr_bToo_much(rev_15d6); */              
    }
}

void Check_MissfireNotidle_valid(void)
{
    if( 0)
    {
        /*
        if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;                
        if(v_MissfireNotIdle<M0_C0_MISSFIRE_L)v_MissfireNotIdle=M0_C0_MISSFIRE_L;*/                
    }
}