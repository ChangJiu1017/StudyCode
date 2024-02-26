#include "public.h"
#include "status_st.h"

#include "ad.h"

//2011-4-9 add
#include "ld.h"   //2010-5-17 add 
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
////volatile V_STATUS_STR _V_TRANS_STATUS; //2014-1-28 peter,delete
//volatile V_STATUS_STR _V_TRANSH_STATUS;  //2014-1-28 peter,delete

sword v_TPS_addt;            //快要喷油时的TPS增量;对快要触发喷油前的T1要进行修正

byte vc_fmf_Enrich;		      //加速加浓系数，标准状态为128
//byte v_nb_binj; ///2012-11-24 //2014-1-28 peter, delete
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define ACCEL_DEC_BY_JQM_CH_IDLE    -1//2008-8-9 add from liulaoshi 2007-12以后的程序：;对LX125车,怠速时油门减小,accedt多减1
#define ACCEL_INC_BY_JQM_CH_IDLE     1//2008-8-9 add from liulaoshi 2007-12以后的程序；;对LX125车,怠速时油门增加,acceldt多增1,

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：STARTS前，过渡工况参数的初始化；
void FI_Trans_BeforeStarts(void)   
{
    //-------Init_TRANS_BeforeStartRun: --------------------------------
    //发动机开始运转前，过渡工况参数的初始化
    v_TPS_addt=0;//	clrf	vc_TPS_acce1									;//清除

    V_TRANS_STATUS=false;
    V_TRANSH_STATUS=false;
    	
  	return;
    
}

void FI_Trans_AfterReset(void) //2009-1-13 add 
{
    VF_TRIGTPS_TRANS=false;
}


//功能：获得JQM对油量的修正系数ACCELDT,ACCELDT_FIRST,vc_TPS_TRANS
//      注：因为要用到VF_JCI_INJ标记，所以需要在CI_CO后面
//      注：因为用到发动机状态的标记，所以需要在Get_Rev计算，Get_TPS计算，和 Get_Status_Eng之后
//形参：tps_ddt--v_tps_ddt
//      ka_b1--过渡油量的修正系数，与油膜有关
//      prd_cash
//      teng
void F_Trans_FMF(sbyte tps_ddt)
{

    //------------------------------------------------------------------------------    
    VF_DECSPD_TRANS=false;
    VF_ACCSPD_TRANS=false;
    VF_ACCLST2_TRANS=false;//accel_status&=0xd0;//;get ready for bit1/2/3/4/6 clr by rolling accel_status left
    VF_DECLST2_TRANS=false;//accel_status<<1;//不带进位左移；
    VF_ACCLST_TRANS=VF_ACCGT1_TRANS;
    VF_DECLST_TRANS=VF_DECGT1_TRANS;
    VF_DECGT1_TRANS=false;
    VF_ACCGT1_TRANS=false;
    VF_ACCMIDLST_TRANS=VF_ACCMID_TRANS;
    VF_ACCMID_TRANS=false;
    

    //----------------GET_TPS_CHANGE_COS-----------------------------------:
    //在GET_JQMOD后执行
    //if(!VF_LEAVEIDLE_FRE_ENG) //2012-9-26 DELETE
    {
        if(tps_ddt<0){
            
        		if(tps_ddt<-1){
          			if(VF_DECLST_TRANS)VF_DECLST2_TRANS=true;
          			VF_DECGT1_TRANS=true;
          			if(tps_ddt<-16)VF_DECSPD_TRANS=true;
        		}
        }
        else //if(!VF_LEAVEIDLE_FRE_ENG)  //2010-6-7 add condiction VF_LEAVEIDLE_FRE_ENG
        {//jqm_chinc:		
        		if(tps_ddt>1){
          			if(VF_ACCLST_TRANS)VF_ACCLST2_TRANS=true;
          			VF_ACCGT1_TRANS=true;
          			if(tps_ddt>12)VF_ACCSPD_TRANS=true;
          			//if(tps_ddt>=8)VF_ACCMID_TRANS=true;
          			if(tps_ddt>=4)VF_ACCMID_TRANS=true;   //2012-12-14半个油门级别
        		}
        }
    }



    return;
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
