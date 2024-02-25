/*HEAD
 ***********************************************************************
 * Filename	:status_eng.c
 * Function	:calculation for engine status		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.3.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		增加小负荷点火和UP限制配置（排放）.
 *
 * V8.3.0   2017.3.28	xielian
 *		1.MV_EXHAUST相关预编译字段更改为if
 *		2.恢复通过VF_NOTACTIVE_LOCK配置ECU激活
 * V8.3.1   2018.9.26	xielian
 *		将车型改为可匹配量后须将预编译部分代码做调整.
 * V8.3.2   2019.2.22	xielian
 *		判断着火转速提高到900rpm.
 * V8.3.3   2019.3.19	xielian
 *		检测到启动成功后排油标志需要清除.
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "time_ccl.h"

#include "ad.h"

#include "inj_notystdc.h"

//2010-4-8 linshi
#include "time_ccl.h"
#include "ld.h"

#include "data_sys_standard.h"

//2013-7-10
#include "data_inj_source.h"

#include "look_up.h"

#include "sci.h"

//#pragma DATA_SEG MY_ZEROPAGE
//#pragma DATA_SEG DEFAULT

byte v_rev62d5rpm_cf;
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/	
		 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/
byte strpmtime	;//	4ch				;用于协助确定SUM_O2_AT_ST;same	as	v_NB_INJDRV		
byte wait_run_start	;//equ	134h	;用于防止连续两次离开怠速的时间间距过短；when	convert	from	idle	to	notidle,	let	which=wait_run_startb	and	dec	which	per	runs.	Continously	injecting	if	convert	from	idle	to	noidle	when	wait_run_start=0
byte t_1s_idletoohot;   //  怠速过热的时间，单位：S 
byte t_1s_conidle;  //2009-12-13 持续怠速的时间;

word v_t1max_v;     //2013-7-10
byte v_ccl_for3rd;  //2014-3-17 第三次喷射允许的循环数

char *bp;         //2014-9-26 MOVE HERE

#define M1_NR_62D5  BYPTR_M1_NR0_62D5RPM


byte VF_EXH_MATCH;

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Get_CF_rpm(void);   //调用位置，计算TPS之后，触发喷油之前;
void STOP_IGNI_WHEN_TOOHOT(void);
void F_CC_Diag(void);	//20210819 chenwei add 侧撑熄火诊断

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/


//获取发动机基础状态，一圈一次
void FG_EngStatus_Basic(void)
{
    byte b;
    sbyte stemp;
    
        VF_JLEAVEIDLETRANS_LST_ENG=VF_JLEAVEIDLETRANS_ENG;//2012-11-20 add 
        VF_JBACKIDLE_LST_ENG=VF_JBACKIDLE_ENG;
        VF_JLEAVEIDLELST_ENG=VF_JLEAVEIDLE_ENG;//2013-10-24 ADD 
        VF_WORM_LST_ENG=VF_WORM_ENG;

        VF_MMOVE_ENG=true;
		
		
        if(v_prd_cash>=M0_START_SPEED)VF_MMOVE_ENG=false;

        if(VF_FORINJ_INJPHASE)VF_INJRUN_ENG=true;else VF_INJRUN_ENG=false;

    ///////////////////////////
    //对VF_NOINJ_ENG的设置
        Get_CF_rpm();
        STOP_IGNI_WHEN_TOOHOT();
        if(VF_IGNONLY_CFCI)VF_NOINJ_ENG=true;else{VF_NOINJ_ENG=false;}
        if(VF_STARTS_ENG||(!VF_BFIRE_ENG))b=M0_START_SPEED;else{b=M0_IDLE_STOP_SP;}
        if(v_prd_cash>=b) {
            VF_NOINJ_ENG=true;;//转速高于断油转速，设置标记  //2009-12-21 change to this line
            VF_IGNONLY_CFCI=true;//2016-7-27 add  in status_eng   //2016-8-3 delete for test HJ150CG
        }
        
        if(REMARK==MC_STOP)VF_NOINJ_ENG=true;//2008-6-26


        //2010-3-14 
		//2020-5-20 共轨不需要停机
       /* if((VF_RUNENOUGH_FORINI_SYSINI
            &&(VF_1ST_INI_SYSINI)
            &&(!VF_NOTNEED_DROPSCAN_SYSINI||!VF_NEEDSAVEGAMA_SYSINI))
            &&!VF_STARTS_ENG)VF_NOINJ_ENG=true;*/

        //这一条与驾驶技巧有关
        if(VF_STARTS_ENG&&v_teng_inuse<80&&v_prd_cash>=PCA900&&!VF_BFIRE_ENG&&VF_DRYIGNI_TPS)VF_NOINJ_ENG=true;


    //---------START_OR_WORM_TEST-------------------------------------------------------:
    //启动和暖车标志判断（部分）：STARTS,WORM_STATUS;(由starts_t 被张平分解得到)
    if(VF_STARTS_ENG){     //2013-4-22
        stemp=(30-v_te/2);            //2012-12-22 ZS COLD START
        if(stemp<0)stemp=0;
        stemp+=22;    //2013-4-22 back to 22

        if(stemp>150)stemp=150;//2013-11-23

        if(v_t_eng_d1s>=stemp){
            VF_STARTS_ENG=false;
            VF_1STSTART_STARTS=false;//2013-1-26
        };//当时间到了之后，退出启动状态；
		//v_t_afterstarts=0;		//20210930 chenwei 落到初始化中，发现步进电机衰减前期（1十几）秒并未起作用，该值前5s左右为0
    }
    
    //2013-4-22
    //if((v_u_M128_Cold==0&&!VF_STARTS_ENG)||VF_MORE1MIN_TMCCL){ //2013-4-12
    if((v_u_M128_Cold==0&&!VF_STARTS_ENG)||(v_t_eng_t_s>=4)){ //20210929 chenwei 改 杨海环松沙滩车项目，启动要求高怠速不超过3s
      	//VF_STARTS_ENG=false;
      	VF_WORM_ENG=false;
    }

    VF_WARMUP_ENG=!VF_WORM_ENG;//2012-9-18
	
	/*if(!VF_STARTS_ENG&&VF_PUMPFILL_INJCTL)
	{
		VF_PUMPFILL_INJCTL=false;
		//Save_Data_EE(0x7f,false);//2019-3-19 启动成功之后不需要排油操作了，清掉标志位
	} */


    //------------check_stfire_flag--------------------------------------------------------:
    //判断着火标志：SUM_O2_AT_ST
    if(!VF_BFIRE_ENG){
    		v_t_eng_d1s=0;	
    		if(v_prd_cash<PCA900){//700
    				if(strpmtime==255){
    				    VF_BFIRE_ENG=true;
    				}
    				else{strpmtime++;}
    		}
    }
    
    if(VF_IDLE_ENG)
    {
        if(VF_1S_TMCCL)
        {
            t_1s_conidle++;
            if(t_1s_conidle>=3)VF_CONIDLE_MORETHAN3S_ENG=true;    
        }
    }else t_1s_conidle=0;
        
    
    //2017-1-15--------
    if(VF_STARTS_ENG)
    {
        if(v_te>=20&&v_te<=30
            &&
            V_Ta>=20&&V_Ta<=30)
            {
                VF_EXH_MATCH=true;    
            } else VF_EXH_MATCH=false;
    }
    
    
    
        
}

//2017-1-15
/*
    0:up
    1:ign
*/
byte Is_RunPointMatch(byte map)
{
    byte blnR;
    byte bitSET;
    
    if(map==0)
	{
		//bitSET=Get_Map_byPtr(BYPTR_M2_UP,16,0,15);
        bitSET=BYPTR_M0_UL_REV_62D5RPM_UPFK;
	}
    else
    {
        //bitSET=Get_Map_byPtr(M2_IGN_TPSRPM,16,0,15);
        bitSET=BYPTR_M0_UL_REV_62D5RPM_IGNFK;
          
    }
    
    blnR=false;
    
    if(
        ((BYPTR_M0_EN_IGN_FK_PARTLOAD==EN_IGN_FK_PARTLOAD_MINLIM) &&map==1)
        ||
        ((BYPTR_M0_EN_UP_FK_PARTLOAD==EN_UP_FK_PARTLOAD_EN) &&(map==0))
     )
    {
        
        if(VF_EXH_MATCH) //负荷工况
        {
            if(bitSET>1&&bitSET<150)
            {
                if(v_rev_625rpm>=REV62D5_2000
                  && v_rev_625rpm<=bitSET 
                  &&v_TPS_odr>=1
                  &&v_TPS_odr<7) 
                {
                    blnR=true;    
                }
                
            }
            
        }
    }
    return blnR;
}

/*
    //获得当前瞬间的状态，
    主要与油门机器动作相关    
*/
void FG_EngStatus_Just(void)
{
        if(VF_NEWCYCLE_ATMAIN_CAS){
          
            VF_JLEAVEIDLETRANS_ENG=false;   
            VF_JBACKIDLE_ENG=false;
            VF_JLEAVEIDLE_ENG=false;
            if(wait_run_start!=0){
                wait_run_start--;
            }
            VF_TOIDLE_ENG=false;//2013-6-26
        }
    
    //------------------DO_INJ_WHEN_LEAVE_IDLE-------------------------------------------
    //起步（从怠速到非怠速）策略;两次起步之间必须有一定的间隔。
        if(wait_run_start){
            //2010-6-3 add
            //如果连续轰油门 
            if(!VF_E0_TPS&&VF_E0LAST_TPS)
            {
                VF_LEAVEIDLE_FRE_ENG=true;        
            }
        }
        else{
            VF_LEAVEIDLE_FRE_ENG=false;
            if(!VF_E0_TPS&&VF_E0LAST_TPS){
                if(!VF_TPSINCLE0P1_TPS)VF_JLEAVEIDLETRANS_ENG=true;	  //2010-1-11 add condiciton
                wait_run_start=WAIT_RUN_STARTB;
            }
        }
        //if(v_TPS_odr>8)wait_run_start=0; //2010-1-11 add 
        if(v_TPS_odr>8||VF_IGNONLY_CFCI)wait_run_start=0; //2014-3-14 清零wait_run_start
        
    //----------------怠速与非怠速转化处理//JQM_TOIDLE_TESTS-----------------------------
    //在TPS计算结束，新的循环计算开始，转速计算结束，节气门是否离开或回怠速检测完成后，判断怠速标志
        if(VF_E0_TPS&&!VF_E0LAST_TPS){;//对油门回怠速的测试，及初始化
            VF_JBACKIDLE_ENG=true;
        		//if(tps_ddt<-16||trans1<-7)//;当JQM_CHANGE<-16,下降两级以上, 设定toidel
        		if(v_TPS_ddt<-16||VF_DECLST2_TRANS)//;当JQM_CHANGE<-16,下降两级以上, 设定toidel
        		{
        				VF_TOIDLE_ENG=true;
        			//	VF_JBACKIDLEQUICK_ENG=true;
        		}
        } 
        
        if(!VF_E0_TPS&&VF_E0LAST_TPS){
            VF_JLEAVEIDLE_ENG=true;
            v_t_forlstrun_d1s=0;//2013-3-26
            //v_ccl_cf_bak=0;    //2013-4-22 zheli bu heli
            VF_LE_IDLETG_ONCE_REV=false;//2013-4-19
        }
            
        if(!VF_STARTS_ENG&&VF_E0_TPS){ 
        	  VF_IDLE_ENG=true;
        }
        else
        {
            VF_IDLE_ENG=false;//2008-3-13 zhangping add after debug;
        }
      
}



//功能：发动机开始运转时，对发动机状态参数的初始化
//void FI_status_ENG_BeforeStarts(byte v_teng_inuse)
void FI_status_ENG_BeforeStarts(void) //2014-10-3
{
    wait_run_start=0;						;		
    t_1s_idletoohot=0;          //记录怠速时发动机过热的时间,单位s;
        	
		VF_PREDRV_INTIGN=true; //DRIVE_STATUS=	0FFh			; MEANS ALL DRIVE PUSLES  END
		VF_BDRV_INTIGN=true;
		VF_PINJB_INTINJ=false; //2008-3-31 纠正
		VF_PINJE_INTINJ=true;
		VF_PTRIG_INTINJ=false;
		VF_INJSTOP_HARD=true;//	bsf		drive_status,binj_end		;//设置喷油结束标识?
		VF_TRIGTPS_TRANS=true;

    //2016-8-20 防止停机时烧保险，发动机遇到FI_status_ENG_BeforeStarts
    INJ_OFF;
    

		VF_BIGN_INTIGN=false;//BCF		DRIVE_STATUS, BIGN	;CLEAR IGNI OVER FLAG
		
		VF_YSTDC_INJPHASE=true;// bcf		motor_status,ystdc					;//make the ystdc the air in ystdc mostly		
	              //2012-10-21 启动时设置为false了，　应该改为true;	
		VF_YSTDCGET_INJPHASE=false;
		
		
    V_ENG_STATUS=false;     
    //V_ENGH_STATUS=false; 
	VF_JLEAVEIDLETRANS_ENGbak = 0;	//刚离开怠速时
	VF_JSTOP_ENGbak = 0;    //2014-3-30 刚停机
	VF_MMOVE_ENGbak = 0;
	VF_NOINJ_ENGbak = 0;     //不需要喷油，包括超速断油的情况
	VF_RUNTOOS_ENGbak = 0;   //软复位时，需要保留该标记
	VF_STARTS_ENGbak = 0;
	VF_STOPBYTOOHOT_ENGbak = 0;
	VF_TOIDLE_ENGbak = 0;    
    V_ENGHL_STATUS=false;     
		VF_MMOVE_ENG=true;//BSF		MOTOR_STATUS, MMOVE
	
		
		VF_STARTS_ENG=true;//BSF		MOTOR_STATUS,STARTS
		VF_WORM_ENG=true;//BSF		MOTOR_STATUS,WORM_STATUS				
        //VF_HOTSTART_ENG=false;
    t_1s_conidle=0;//
		if(VF_VPOR_HARD)VF_STOPBYTOOHOT_ENG=false;
	  
	  if(v_teng_inuse>=80)VF_HOTSTART_ENG=true; //2009-12-31
	
    strpmtime=252;
    
    VF_JLEAVEIDLETRANS_LST_ENG=VF_JLEAVEIDLETRANS_ENG=false;//2012-11-20
    
    VF_T35TEST_HARD=false;//2014-3-30 add
    v_t_afterstarts=0;		//20210930 chenwei add
   
}

//2008-10-30 add
//硬件复位后进行的初始化
void FI_status_AfterReset()
{
    V_ENG_STATUS=false;     
    V_ENGHL_STATUS=false;     
    
    if(VF_VPOR_HARD) //2009-3-29 如果是开钥匙，则所有标记都复位
    {
	VF_JLEAVEIDLETRANS_ENGbak = 0;	//刚离开怠速时
	VF_JSTOP_ENGbak = 0;    //2014-3-30 刚停机
	VF_MMOVE_ENGbak = 0;
	VF_NOINJ_ENGbak = 0;     //不需要喷油，包括超速断油的情况
	VF_RUNTOOS_ENGbak = 0;   //软复位时，需要保留该标记
	VF_STARTS_ENGbak = 0;
	VF_STOPBYTOOHOT_ENGbak = 0;
	VF_TOIDLE_ENGbak = 0;      
    }         
    else //2009-3-29add
    {   //如果是停车等方式的复位，则
        //#define	VF_RUNTOOS_ENG	_V_ENGH_STATUS.Bits.b4
        //#define	VF_STARTS_ENG	_V_ENGH_STATUS.Bits.b5
        //#define	VF_STOPBYTOOHOT_ENG	_V_ENGH_STATUS.Bits.b6
        //V_ENGH_STATUS&=0x50;//即保留VF_RUNTOOS_ENG和VF_STOPBYTOOHOT_ENG为原状态   
			
	VF_JLEAVEIDLETRANS_ENGbak = 0;	//刚离开怠速时
	VF_JSTOP_ENGbak = 0;    //2014-3-30 刚停机
	VF_MMOVE_ENGbak = 0;
	VF_NOINJ_ENGbak = 0;     //不需要喷油，包括超速断油的情况
	//VF_RUNTOOS_ENGbak = 0;   //软复位时，需要保留该标记
	VF_STARTS_ENGbak = 0;
	//VF_STOPBYTOOHOT_ENGbak = 0;
	VF_TOIDLE_ENGbak = 0; 
    }
    
    v_t1max_v=2000;//2013-7-10
    v_t_afterstarts=0;	//20210930 chenwei add
    
}
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
/************GET_CF_RPM:***************************************************/
/************获得停油转速；*************************************************/
//返回值：停油转速,单位62.5RPM
//－－－－与CICF相关－－－－－－－－－－－－－－－－－－－－－－－－－－		
//断油转速先由查表得到,如果是回到怠速(TOIDLE),则减小(rpm_nooil_dec_toilde-(rzt_hot_nooil-v_Teng_inuse)/4)
//再供油转速减小(rpm_dec_reoil-(rzt_hot_nooil-v_Teng_inuse)/4)
void Get_CF_rpm(void)   //调用位置，计算TPS之后，触发喷油之前
{
    byte b1;
    byte b2;
    sbyte sbe;
    
    //long int ln;
    
    VF_IGNONLY_CFCI=false;//
    
    //F_LinearInter(sword y1,sword y2,word dx_mode,word dx_mode_all
    //BYPTR_M1_NC_62D5
    
    b1=16;
    b2=v_TPS_mode;
    if(VF_E0_TPS){
        b1=8;
        if(v_TPS_mode<8)b2=0;
        else b2=v_TPS_mode-8;
    }
    
    
    if(VF_CFLST_CFCI){
        bp=(char *)M1_NR_62D5;
        if(v_teng_inuse>30)sbe=0;
        else if(v_teng_inuse<0)sbe=30;
        else{
            sbe=30-v_teng_inuse;
        }
        
        sbe=(sword)sbe*BYPTR_M0_NRT_M64/64;
    } else{
        bp=(char *)BYPTR_M1_NC_62D5;
        sbe=0;
        
    }
    
    if(v_TPS_odr>=15)v_rev62d5rpm_cf=*(bp+v_TPS_odr);
    else{
        v_rev62d5rpm_cf=F_LinearInter(*(bp+v_TPS_odr),*(bp+v_TPS_odr+1),b2,b1,false);//2013-7-10 发现少了一个参数false，未包含look_up.h
        
        //2013-6-28 非怠速时，限制断油转速不得比断油map小250rpm
        if(!VF_E0_TPS){
            b1=BYPTR_M1_NC_62D5[v_TPS_odr]-4;
            if(v_rev62d5rpm_cf<b1)v_rev62d5rpm_cf=b1;    //250rpm
        }
    }
    
    
    
    
    //2013-7-10
    //#if MV_EXHAUST>PL150  //2014-12-15 DELETE
        b1=F_2DLookUp(v_Vbat_inuse,M2_NC_V,6,0,false);
        
        /*
        if(VF_CFLST_CFCI){
            //b1-=16;//如果处于断油状态，则-1000rpm
            b1-=32;//如果处于断油状态，则-2000rpm
           // b1-=8;//如果处于断油状态，则-500rpm
        }
        *///2015-2-10 DELETE
        if(v_rev62d5rpm_cf>b1)v_rev62d5rpm_cf=b1;
        
        /*
        v_t1max_v=F_2DLookUp(v_Vbat_inuse,M2_T1MAXD8_V,6,0,false);
        
        v_t1max_v*=8;//2013-7-10
        
        if(v_rev_625rpm>REV62D5_5000){
          
            v_t1max_v=v_prd_cas+v_prd_cas-v_t1max_v;
        } else {
            v_t1max_v=2000;
        }
         *///2014-12-15 MOVE TO INJ_TCL.C
        
    //#endif
    
    if(VF_E0_TPS)v_rev62d5rpm_cf+=sbe;
    
    
    
    if(VF_MILELIMIT_DUE_SYS){
    }
    
    
    if ((BYPTR_MV_VEHICLE==SCOOTER) && (BYPTR_MV_EXHAUST<=PL50))
    	{
        /*
        if(VF_WOT_TPS){
            if(v_t_WOT_1s<=6 && v_t_NOTE0TPS_1s<=4){
                v_rev62d5rpm_cf+=40;//增加2500rpm        
            }
        } */
        if(v_t_NOTE0TPS_1s<=6&&!VF_E0_TPS){
            v_rev62d5rpm_cf+=50;//增加2500rpm        
        }
    	}
        
    

    
    if(v_rev62d5rpm_cf<40)v_rev62d5rpm_cf=40;//2013-3-5
                                            //2013-3-28D18 FROM 39 TO 40

    if ((BYPTR_MV_VEHICLE==SCOOTER) && (BYPTR_MV_EXHAUST<=PL50))
    	{
        if(VF_E0_TPS){
            if(v_rev_625rpm>=v_rev62d5rpm_cf){
        		    VF_IGNONLY_CFCI=true;
            }
        }
    	}
    else
    	{
        if(v_rev_625rpm>=v_rev62d5rpm_cf){
    		    VF_IGNONLY_CFCI=true;
        }
    	}
	#ifdef SY_QD
	    if(VF_QD_ST)//2v
		{
		   VF_IGNONLY_CFCI=true;
		}  
	#endif  

	#ifdef SY_CC
	    if(VF_CC_ST)//2v	//20210819 chenwei add 侧撑 ||VF_CC_ST
		{
			if((v_rev_625rpm>BYPTR_M0_CC_NC_SPEED)&&(BYPTR_M0_CC_NC_SPEED!=255))	//20211008 chenwei add 转速条件
			{
				VF_IGNONLY_CFCI=true;
			}

			if((v_rev_625rpm<BYPTR_M0_CC_NR_SPEED)&&(BYPTR_M0_CC_NR_SPEED!=255))	//20211008 chenwei add 转速条件
			{
				VF_IGNONLY_CFCI=false;
			}
		}   

	#endif
	
    //2016-6-5
    //2017-3-1 恢复，VF_NOTACTIVE_LOCK通过使能位可配置
    if(VF_NOTACTIVE_LOCK){
        if(REMARK==MS_PC_SET_SELB&& KESEI==0)VF_IGNONLY_CFCI=true;//2014-5-9
    } //2016-8-31 临时屏蔽
    

    if(VF_IGNONLY_CFCI)v_ccl_for3rd=3;//2014-3-17 VF_IGNONLY_CFCI
    
    //2013-7-15
    //授权使用到期
    /*  2013-9-2 DELETE
    if(VF_LIMITDUE_LOCK)v_rev62d5rpm_cf=40;//2013-6-14
    
    if(!VF_KEYON_LOCK){
        VF_IGNONLY_CFCI=true;
    }
      */
    return;
}

//*****************************************************/
/***    过热停油；***/
void STOP_IGNI_WHEN_TOOHOT(void)
{
    byte b;
    
    //2015-6-23 add ,限制最低限温
    
    b=BYPTR_M0_WIDTH_CDION;
    if(b<200) 
    {
        b=200;    
    }
    
    if(v_teng_inuse>=M0_TES_MAX)
    {
        VF_IGNONLY_CFCI=true;
        VF_STOPBYTOOHOT_ENG=true;
    }
    //if(MS_ENABLE_CI_TOOHOT_WHENIDLE)
    {
        if(!VF_IDLE_ENG)t_1s_idletoohot=0;
        
        //if(v_teng_inuse>M0_TES_FOR_IDLENOTTOOLONG){//将怠速过温的温度载入W;
        if(v_teng_inuse>b){//2014-3-5 ADD M0_WIDTH_CDION
        		if(VF_1S_TMCCL)t_1s_idletoohot++;
        		if(t_1s_idletoohot>=120)VF_IGNONLY_CFCI=true;
        		VF_STOPBYTOOHOT_ENG=true;
        	//	if(t_1s_idletoohot>=1)VF_IGNONLY_CFCI=true;      //for debug
        }else{t_1s_idletoohot=0;}
    }
    return;
}

/****20210819 chenwei 侧撑熄火*******
*****方式：检测J16 IO口拉低，置位****
************************************/
byte nb_cc;		//20210819 chenwei add 侧撑熄火IO拉低的次数/时间（0.1s累计一次）
void F_CC_Diag(void)
{
	if(!PORTJ_CC)	
	{
		VF_CC_CONNECT=true;

		if(((nb_cc<=5)&&VF_STARTS_ENG)||(nb_cc>5))	//检测到侧撑接通持续,每0.1s加1，1s或者处于启动阶段1s内（即开钥匙1s启动），则侧撑标志置位
		{
			VF_CC_ST=true;
		}
	}
	else 
	{
		VF_CC_CONNECT=false;
		VF_CC_ST=false;
		nb_cc=0;
	}
}

/*                      
    
*/
