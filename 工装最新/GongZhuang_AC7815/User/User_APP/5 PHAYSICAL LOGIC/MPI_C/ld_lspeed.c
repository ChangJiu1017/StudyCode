#include "public.h"

#include "ee.h"
#include "status_st.h"
#include "ld.h"
#include "add.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
byte vc_FMF_lspd;//低速增油系数
byte vc_lspd;    //2009-8-20 add 
byte vc_lspd_lst;//2009-8-20 add 
byte vc_lspd_lst2;//2009-8-20 add 
byte vc_lspd_lst3;//2009-8-20 add 

#define M0_REV1500_15D6RPM 96
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//**********REC_Q_WHEN_LOW_SPEED***********************
//功能：获得低速时喷油量修正，增油修正；
//形参：rpm156－－v_rev_15d6rpm;
//slow_rpm_map_get
//2008-9-18 查看刘老师程序，弯梁低速map取 （96-rpm156）/2
void F_FMF_LowSpeed(word rpm156)
{
/*
    if( MS_ENABLE_ADJUST_FMF_LSPD)
    {
        
        if(kesei_to_beta>50)kesei_to_beta=50;
        else if(kesei_to_beta<0)kesei_to_beta=0;
    }
 */
    vc_lspd_lst3=vc_lspd_lst2;
    vc_lspd_lst2=vc_lspd_lst;
    vc_lspd_lst=vc_lspd;
        
    if(rpm156<=M0_REV1500_15D6RPM)//96*15.6=1497.6 rpm
    {
    
        vc_lspd=M0_REV1500_15D6RPM-rpm156;
        if(vc_lspd>50)vc_lspd=50;//     2010-4-22 
        /*if( MS_ENABLE_ADJUST_FMF_LSPD)      
        {
            vc_lspd=(byte)(((word)vc_lspd*kesei_to_beta)>>4);//(byte)(vc_FMF_lspd>>2); //2008-10-9
        }else*///2009-12-28
        {
            //vc_lspd=(byte)(((word)vc_lspd*M0_CO_LSPEED_TO16)>>4);//(byte)(vc_FMF_lspd>>2); //2008-10-9
            vc_lspd=(byte)(((word)vc_lspd*M0_CO_LSPEED_TO16)>>4);//(byte)(vc_FMF_lspd>>2); //2008-10-9
            //vc_FMF_lspd=vc_FMF_lspd;//(byte)(vc_FMF_lspd>>2);//2008-9-23 低速增油增加一点 2008-9-26 for 比如azil
        }
        //if(vc_FMF_lspd>
       /* if(rpm156<=64)//15.6*64=998.4rpm
        {
             vc_FMF_lspd=vc_FMF_lspd+64-rpm156;    //TOBECHCEK,
             //需要在油量修正中保证这一点；
        }              //2008-3-28 zhangping 纠正; *///2008-5-29 DELETE
        //2008-5-29 zhangping   remark
        //增油：4.5dtfi/1000rpm;
        
        //2013-5-19 add
        if(rpm156<60)//
        {
            vc_lspd+=30-rpm156/2;    
        }
        
        //2014-3-23 D12 75 3-23 FOR LSPD FMF
        #if M0_MFUD1==INJ309
        
        //#if MV_EXHAUST>PL150
            vc_lspd/=2;//2013-5-30
        #else
            #if M0_MFUD1==INJ109
                vc_lspd/=2;//2019-5-28 杨老师和刘老师都认为11的低速修正作用太弱   /4
            #else
            
            #endif
            
        #endif
        
        //2008-7-28 div 2
        //vc_FMF_lspd=vc_FMF_lspd/2;
        //2008-7-31 add back;
    }
    else
    {
        vc_lspd=0;     //2008-4-18 add
    }
    
    if(!VF_BFIRE_ENG)
    {
        vc_lspd_lst3=vc_lspd_lst2=vc_lspd_lst=vc_lspd;
        
    }
    //vc_FMF_lspd=(byte)(( vc_lspd +(word)vc_lspd_lst+vc_lspd_lst3+vc_lspd_lst2)/4);
    //2009-8-24 RESUME
    vc_FMF_lspd=(byte)(( vc_lspd +(word)vc_lspd_lst)/2);
    
}
/*
2008-5-29  
关于流量的低速修正事宜
一、2007年，周瑾的理论预测结?
    发动机转速从1000-1500变化时，每上升100RPM，气量减小5%（相对于RPM1300时?

二、根据刘老师目前的程序
    FMF=Q*11/1000*4；
    FMF的量纲与DTFI一致；
    Q的量纲为ul/cycle；
    
    假定怠速时FMF=100,则刘老师原来的算法基本可以，
    只是1000rpm以下就有待验证了。
*/
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
