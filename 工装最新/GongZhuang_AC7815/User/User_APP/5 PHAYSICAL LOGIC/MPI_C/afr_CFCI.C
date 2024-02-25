/*HEAD			
 ***********************************************************************			
 * Filename	:afr_cfci.c		
 * Function	:calculation for cfci.		
 * User		:xielian	
 * Date		:2017.4.1	
 * Version	:V8.1.1		
 *			
 *** History ***			
 * V8.1.0	2017.1.20	xielian 	
 *		Base. 
 * V8.1.1	2017.4.1	xielian 	
 *		add v_ccl_ci calculation.
 ***********************************************************************			
HEAD END*/

#include "status_st.h"
#include "public.h"


//2010-3-13 add 
#include "data_sys_standard.h"

#include "add.h"
#include "time_ccl.h"

//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_CFCI_STATUS;
//#pragma DATA_SEG DEFAULT

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//cfci
byte v_ccl_cf;            //连续断油的圈数
byte v_revmax_625_cf;     //2015-10-11 断油前的转速

byte v_ccl_ci;//2017-3-29 恢复供油的次数
      
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte v_ccl_cf_bak;        //back up	of v_ccl_cf,but	will be	inced	by everyrun//没有被使用
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：初始化CI参数；
void FI_CI(void)
{
    //复位后对 vc_cfci和vc_cfci2的初始化；
    //Init_CFCI_BeforeStartRun:
    v_ccl_ci=0;
  	
  	v_ccl_cf_bak=0;
    V_CFCI_STATUS=false;
#ifdef SY_QD
	if(VF_JSTOP_ENG)
	{
		if(VF_QD_ST)VF_IGNONLY_CFCI=true;
	}
	#endif

#ifdef SY_CC	

	if(VF_JSTOP_ENG)
	{
		//if(VF_CC_ST)VF_IGNONLY_CFCI=true;
	}
#endif
	return ;

}

//功能：获得恢复供油系数ASTK_INJ_AGAIN和恢复供油后的加喷次数ADDTIME；
//         //应该在本循环Get_Status_Eng之后； 触发喷油之前；
void FG_CICO(void)
{

    //+在发动机启动前初始化，TOBECHECK，VF_CIADDINJ_INJ=false;
    /*--------FG_CICO---------------------------------------------------------------------*/
    VF_JCI_LST_CFCI=VF_JCI_CFCI;//2013-1-14
    VF_JCI_CFCI=false;
    
    //刚恢复供油 
    if(VF_CFLST_CFCI&&!VF_IGNONLY_CFCI)
	{
        VF_TOIDLE_ENG=false;
		v_t_ci_d1s=0;
        //v_ccl_cf_bak=0;
        if(v_ccl_cf>=M0_MIN_STOPOIL_RUN){
            VF_JCI_CFCI=true;//code2_number,binj_again=true;
            //VF_CICORECING_CFCI=true;//2008-11-16 move here

            //v_ccl_cf_bak=v_ccl_cf;

        }
    }
    
    if(!VF_CFLST_CFCI&&VF_IGNONLY_CFCI){
        v_ccl_cf_bak=0; //2013-4-22
          //2015-10-11 ADD 
          v_revmax_625_cf=v_rev_625rpm;//2014-6-25 add 
    
    }
    
    //if(v_ccl_cf_bak>1)v_ccl_cf_bak--;
    if(VF_IGNONLY_CFCI){
        if(v_ccl_cf_bak<255)v_ccl_cf_bak++;
        v_ccl_ci=0;
    } 
    else
    {
        if(v_ccl_ci<255)v_ccl_ci++;
    }
  
    //--------------断油圈数的累计和断油标志的设置---------------------------------------
    if(!VF_CFLST_CFCI)v_ccl_cf=0;
    else if(v_ccl_cf<127){v_ccl_cf++;}//;v_ccl_cf=min(v_ccl_cf,127);}

    
    //sWFIRST_TEST:
    VF_CFLST2_CFCI=VF_CFLST_CFCI;
    VF_CFLST_CFCI =VF_IGNONLY_CFCI;
    if(VF_NOINJ_ENG&&!VF_IDLE_ENG)VF_CF_CFCI=true;else VF_CF_CFCI=false;

    //----------------Astk_Inj_agn_attenuation-------------------------------:
    //2012-11-29 DELETE
    //VF_CICORECING_CFCI=false;
    //VF_CICFGT5_CFCI=false;

    return;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

/*
;//减速断油后的恢复过程,怠速起步过程和加喷、T35测试过程;
;//该子程序是在点火结束后，准备判断是否断油、并触发喷油之前调用的；
;//---------------------------------------------------------------------------

*/
