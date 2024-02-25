/*本模块作为INJ_CTL.C模块的子模块*/
#include "derivative.h"
#include "status_st.h"
#include "public.h"
#include "look_up.h"
#include "data_inj_source.H"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
byte v_FMF_odr;     //FMF级别
byte v_FMF_mode;    //FMF小级别
                    //FMF=Q*250/11
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define FMF_ODR_MAX_Q       448U  //28*16

#define FMF_ODR_MAX         28
#define FMF_MODE_MAX        15
#define FMF_ODR_MIN         0
#define FMF_MODE_MIN        0

#define FMF_MIN_FORODR0         16
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Get_FMF_ODR(word fmf);
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：
//      计算DT3的目标值；
//      返回值T3目标值
word FG_DT3_Target(sword fmf)           //2009-4-20 change to sword
{
    word dt3obj;
    
    if(fmf>0)   //2009-4-20 add this condiction 
    {
        //fmf=fmf*4/3;//2009-4-21 add temperory
        Get_FMF_ODR(fmf);
        
        if(v_FMF_odr>=FMF_ODR_MAX)
        {
            //dt3obj=M1_DT3_Q[v_FMF_odr];    
            //2015-1-11 CHANGE TO 
            dt3obj=(word)F_LinearInter(M1_DT3_Q[FMF_ODR_MAX-8],M1_DT3_Q[FMF_ODR_MAX],v_FMF_mode,128,false);    
            dt3obj+=M1_DT3_Q[FMF_ODR_MAX]-M1_DT3_Q[FMF_ODR_MAX-8];
        } 
        else
        {
            dt3obj=(word)F_LinearInter(M1_DT3_Q[v_FMF_odr],M1_DT3_Q[v_FMF_odr+1],v_FMF_mode,16,false);    
        }
        //dt3obj=dt3obj; //2009-4-20 add temperory
    }
    else
    {
        dt3obj=M1_DT3_Q[0]/2; 
        v_FMF_odr=0; //2009-8-6add 
        v_FMF_mode=0;//2009-8-6add   
    }
    return dt3obj;            
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//功能：获得流量级别
//      
/*
void Get_FMF_ODR(word fmf)
{
    word wd;
    
    if(fmf<FMF_MIN_FORODR0)
    {
        v_FMF_odr=0;
        v_FMF_mode=0;        
    } else 
    {
        wd=((fmf-FMF_MIN_FORODR0)>>4)&0x3fff;
        if(wd>FMF_ODR_MAX)
        {
            v_FMF_odr=FMF_ODR_MAX;
            v_FMF_mode=0;        
        } 
        else
        {
            v_FMF_odr=(byte)wd;
            v_FMF_mode=(byte)((fmf-FMF_MIN_FORODR0)&0x000f);            
        }
    }
}
*/

//2015-1-11 对超过最大值的点作特殊处理
void Get_FMF_ODR(word fmf)
{
    word wd;
    
    if(fmf<=0)
    {
        v_FMF_odr=0;
        v_FMF_mode=0;        
    } else 
    {
        wd=((fmf)>>4)&0x3fff;
        if(wd>=FMF_ODR_MAX)
        {
            v_FMF_odr=FMF_ODR_MAX;
            if(wd>=FMF_ODR_MAX+8){
                v_FMF_mode=0x3f;
            } else{
                //v_FMF_mode=(byte)((fmf)&0x007f); 
                v_FMF_mode=(byte)((fmf-FMF_ODR_MAX_Q)&0x007f); 
            }
                   
        } 
        else
        {
            v_FMF_odr=(byte)wd;
            v_FMF_mode=(byte)((fmf)&0x000f);            
        }
    }
    
    //2015-12-26 add 
    if(v_FMF_odr<1)
    {
        VF_FMF_REACHMIN_INJCTL=true;
    } else VF_FMF_REACHMIN_INJCTL=false;
}
