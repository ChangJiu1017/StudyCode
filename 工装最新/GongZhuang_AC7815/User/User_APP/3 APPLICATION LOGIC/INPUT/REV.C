/*HEAD
 ***********************************************************************
 * Filename	:REV.c
 * Function	:校准相关运算
 * User		:yangsili
 * Date		:2018.1.6
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2018.1.6	yangsili 
 *		Base module.
 * 
 * V9.2.0   2018.1.6	yangsili
 *		1.增加33814复位、普通驱动诊断标志位置位
 *
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"

#include "ad.h"

#include "time_ccl.h"
#include "ee.h"//2008-8-7 add not neccery
#include "sci.h"
#include "ign.h"

#include "add.h"
#include "dtc.h"
#include "events.h"
                               
/*----------THE FOLLOWING ARE public variables-----------------------------------------------*/ 
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_REV_STATUS;   //2014-1-28
//#pragma DATA_SEG DEFAULT
//volatile v_rev_15d6rpmSTR _v_rev_15d6rpm; //转速rpm[15.6RPM]
word v_rev_15d6rpm; //转速rpm[15.6RPM];
word v_rev_3d9;//2010-1-25
//word vlst_prd_cas;//volatile vlst_prd_casSTR _vlst_prd_cas;   //上一次两个角标间的时间间隔	
//word v_prd_cas_min;     // volatile v_prd_cas_minSTR _v_prd_cas_min; //用于作为下一圈转速上升后，周期变小的最小域值
byte v_rev_156ave;      //average	of rpm156	by 128 time
byte v_rev_625rpm;        //save	RPM	using	62.5 unit
byte v_rev_odr;           //RPM级别					
byte v_rev_mode;          //RPM级别的尾数
byte v_ystdc_search_t;    //

byte v_rev_deg;//转速级别     2013-1-4
//2008-9-16 add
byte v_rev_156ave_512;     //转速的1024次平均  //2008-11-21 变更为256平均   //2008-11-21 128次
unsigned long int   sum_rev_156_512;   //转速的 

word v_t_AfterCA_limit; //2014-4-2

word v_t_CAHY_timeout;

/*----------THE FOLLOWING ARE local macros------------------------------------------------*/ 
#define PCA_TO_MODE1 12     //	equ	12	;v_prd_cas_h>=12,rpm<4882rpm	change	to	mode1					
#define PCA_TO_MODE2 11     //	equ	11	;v_prd_cas_h<11	rpm>5326	;rpm>5326	change	to	mode2	
#define GET_YSTDC_AFTERS 3

/*----------THE FOLLOWING ARE local variables------------------------------------------------*/ 
byte rpm156l_l;
byte rpm156l_l2;
word sum_rpm156;
byte sum_drpm156; 
byte number_av_gtrpm;

byte RPM_CH_STATUS;


void Get_Rpm_Trend(word prd_cas);
void Cal_Rev(word prd_cas);
void GET_REV_ORDERS(void);
void Stroke_Detect(void);
void AV_RPM156_GET_RPMDROP_AT_LOWRPM(void);


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS------------------------------------------------*/ 
//功能：获得转速及其变化规律，相位判断，低速时转速平均；
//形参：prd_cas－－前沿角标周期
void FG_Rev_BeforeIgn(word prd_cas)
{
    Get_Rpm_Trend(prd_cas);

    Stroke_Detect();//判断是否是喷射相位圈
    
    if(((prd_cas>>8)&0xff)>=PCA2000)Cal_Rev(prd_cas);    //2008-4-17 add contiction
    return; 
}

//功能：若在Get_Rev_BeforeIgn中没有计算转速，则计算，冲程检测
//形参：prd_cas－－前沿角标周期
void FG_Rev_AfterIgn(word prd_cas)
{
     Cal_Rev(prd_cas);      //与刘老师原来思路统一?
     GET_REV_ORDERS();
     AV_RPM156_GET_RPMDROP_AT_LOWRPM();
}

//计算转速及相关参数，综合FG_Rev_BeforeIgn和FG_Rev_AfterIgn 
void FG_Rev(word prd_cas)
{
    Get_Rpm_Trend(prd_cas);
    Stroke_Detect();//判断是否是喷射相位圈
    Cal_Rev(prd_cas);    //2008-4-17 add contiction
    GET_REV_ORDERS();
    AV_RPM156_GET_RPMDROP_AT_LOWRPM();
    
}

//功能：复位后，转速和角标参数的初始化
void FI_Rev_AfterReset()	
{
    v_prd_cas_min=0x100;
    V_REV_STATUS=false;             //2008-12-15
    v_rev_15d6rpm=0;//2010-2-7
	v_rev_3d9=0;

}


//主函数操作的时间，超出这个时间就不等待了
void FC_DueMain(void)
{
    word w;
    word w2;
    
    w=T_MAINTMR-v_t_cas0;
    
    //过了这个时间主循环就不能再今天其他操作了，因为qy快要来了
    if(w>v_t_AfterCA_limit){
        VF_CAMAIN_TIMEOUT_CAS=true;    
    }
    
    //说明可以诊断CA了，诊断CA的要求是，本圈的CAQY和CAHY都需要同时过了
    if(w>v_prd_cas/2){
        VF_CADIAG_TIMEIN_CAS=true;
    }
    
    w2=BYPTR_M0_CAS_FRONTEDGE-BYPTR_M0_CAS_TRAILEDGE;
    
    w2=(word)(((unsigned long int)v_prd_cas*w2)>>7);

    /* 2015-3-2 DELETE 发现 VF_CAHYPREDICT_TIMEIN_CAS与VF_OLD_CAS为自锁关系，这里=TRUE之后，在另一个地方又被置TRUE
    //后沿应该快要到了的时间
    if(!VF_CAHYPREDICT_TIMEIN_CAS) {
        if(w>v_prd_cas/4){
            v_t_CAHY_timeout=T_MAINTMR;
              
            VF_CAHYPREDICT_TIMEIN_CAS=true;
            //VF_CAHYPREDICT_TIMEIN_CAS=false;
            deDTC++;

        }
        deTPS++;//2015-3-2 TEST   

    }
    */

    //2018-1-6,ysl,增加33814复位、普通驱动诊断标志位置位
    if(w>(v_prd_cas/3))     VF_33814DIAG_RESET = true;
    if(w>(v_prd_cas/3+125)) VF_33814DIAG_GEN = true;
}


/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS------------------------------------------------*/
//获得前8次转速变化规律；;//转速变化规律；//根据刚到来的角标与上一次角标进行比较
void Get_Rpm_Trend(word prd_cas)
{
    byte b;
    
    RPM_CH_STATUS=(RPM_CH_STATUS<<1)&0xfe;//带进位左移；//与原来有所区别，原来是右移
    if(prd_cas<vlst_prd_cas)RPM_CH_STATUS++;
    //VF_OLDCA_CAS=true;   //2014-4-18 MOVE OUT
	
	
    if((!v_CA_break)||(!v_CA_reset))
	{
		vlst_prd_cas=prd_cas;
		//nb_CA_lost++;
	}

    //v_prd_cas_min=vlst_prd_cas-((vlst_prd_cas>>3)&0x1fff);   //2008-3-27 因有丢角标现象，因此 放松范围
    v_prd_cas_min=vlst_prd_cas-((vlst_prd_cas>>2)&0x3fff);   //2015-3-5 因有丢角标现象，因此 放松范围
    
    VF_CALCUED_REV=false;
    
    v_t_AfterCA_limit=v_prd_cas_min;//2014-4-2
    VF_NEEDRECBYLOWRPM_IGN=false; //2008-5-10 move to here

    b=((prd_cas>>8)&0xff); //2009-12-18 add ,防止转速回来时震荡          

    if((VF_IDLEIGNFK_IGN&&b>=PCA3662)||(!VF_IDLEIGNFK_IGN&&b>=PCA3255))//即当转速小于2000，计算转速；//如果转速高时，在下一循环开始时再计算；
    {  
        VF_NEEDRECBYLOWRPM_IGN=true;
    }
    return;
}

void Cal_Rev(word prd_cas) 
{   
    //VF_NEEDRECBYLOWRPM_IGN=false;//2008-5-10 remove from here
    if(!VF_CALCUED_REV)
    {   
        VF_CALCUED_REV=true;
        rpm156l_l2=rpm156l_l;
        rpm156l_l=(byte)(v_rev_15d6rpm & 0xff); //保存未修改的RPM156
        

        {
            v_rev_3d9=(word)(3840000/(long int)prd_cas); //2010-1-25
        }
        v_rev_15d6rpm=v_rev_3d9/4;//2010-1-26
        v_rev_625rpm=(byte)(v_rev_15d6rpm/4);//得到RPM625;
        VF_DEC_REV=false;
        if(((prd_cas>>8)&0xff)>=PCA2000);//即当转速小于2000，计算转速；//如果转速高时，在下一循环开始时再计算；
        {  
            v_rev_625rpm=(byte)(v_rev_15d6rpm/4);
            if(rpm156l_l2>=rpm156l_l&&rpm156l_l>(byte)(v_rev_15d6rpm & 0xff))VF_DEC_REV=true;//即当转速大于2000时，直接清除该位；
        }
        
        //2013-4-17
        VF_GT10_IDLETG_REV=false;
        VF_GT30_IDLETG_REV=false;

        //rev15d6_target与油门关联，替换为BYPTR_M0_N0_LNOISCV_15D6RPM ，2014-10-8
        

        if(v_rev_15d6rpm>v_n0_low+10){
            VF_GT10_IDLETG_REV=true;
        }
        if(v_rev_15d6rpm>v_n0_low+30){
            VF_LE_IDLETG_ONCE_REV=false;
            VF_GT30_IDLETG_REV=true;//2013-4-17
        }
        if(v_rev_15d6rpm<=rev15d6_target)VF_LE_IDLETG_ONCE_REV=true;//2013-4-19

    }
    //2009-12-19
    VF_GE2200_REV=false;
    if(v_rev_15d6rpm>128)
    {
        VF_GE2200_REV=true;   
    }
    return;    
}

//获得ORDER_RPM及小级别MODE_RPM
void GET_REV_ORDERS(void) 
{
  /*
    if(v_rev_625rpm>=23){
    		v_rev_odr=(byte)((v_rev_625rpm-23)/8);
    		v_rev_mode=(byte)((v_rev_15d6rpm-92)&0x1f);//;the lowest 5 bits as mode of rpm  //92/4=23
    		
    		
    		if(v_rev_odr>0xf)
    		{
    		    v_rev_odr=0x0f;  //2008-11-10 ADJUST TO TH  
    		    v_rev_mode=0x1f;//2008-11-10 ZHANGPING ADD
    		}
    }
    else{
    		v_rev_odr=0;
    		v_rev_mode=0;
    }
 */
 //2015-10-7 change to   
    if(v_rev_625rpm>=24){
    		
    		#ifdef X_N_9000_TO_9500RPM
    	      if(v_rev_625rpm<136)//<8500rpm
    	      {
            		v_rev_odr=(byte)((v_rev_625rpm-24)/8);
            		v_rev_mode=(byte)((v_rev_15d6rpm-96)&0x1f);//;the lowest 5 bits as mode of rpm  //92/4=23
    	      } 
    	      else
    	      {
    	          // 8500-136-odr 14
    	          
            		v_rev_odr=14+(byte)((v_rev_625rpm-136)/16);
            		v_rev_mode=(byte)((v_rev_15d6rpm-96)&0x3f);//;the lowest 5 bits as mode of rpm  //92/4=23
    	          v_rev_mode/=2;
    	      }
    	      
        		if(v_rev_odr>0xf)
        		{
        		    v_rev_odr=0x0f;  //2008-11-10 ADJUST TO TH  
        		    v_rev_mode=0x1f;//2008-11-10 ZHANGPING ADD
        		}
    		
    		#else
        		v_rev_odr=(byte)((v_rev_625rpm-24)/8);
        		v_rev_mode=(byte)((v_rev_15d6rpm-96)&0x1f);//;the lowest 5 bits as mode of rpm  //92/4=23
        		
        		
        		if(v_rev_odr>0xf)
        		{
        		    v_rev_odr=0x0f;  //2008-11-10 ADJUST TO TH  
        		    v_rev_mode=0x1f;//2008-11-10 ZHANGPING ADD
        		}
    		#endif
    		
    }
    else{
    		v_rev_odr=0;
    		v_rev_mode=0;
    }
    
    
    
    
    v_rev_deg=((v_rev_odr<<4)&0xf0)+((v_rev_mode>>1)&0x0f);

    VF_UB_REV=false;
    if(v_rev_odr==0xf)VF_UB_REV=true;
    
    return;
}

//该过程需要在Get_Rpm_Trend之后，systdc_get之前，因为涉及到标志位VF_YSTDC_PHASE
//YSTDC_TINJ_GET：
void Stroke_Detect() 
{

    //需要确认。。。。
    //2008-8-7 今日才真正理解VF_YSTDC_PHASE的含义；
	if(VF_MODE1_INJPHASE&&(v_rev_625rpm>REV62D5_MODE2)&&VF_YSTDC_INJPHASE){VF_MODE1TO2_INJPHASE=true;}
	else{VF_MODE1TO2_INJPHASE=false;}
	
	if(!VF_MODE1_INJPHASE&&(v_rev_625rpm<REV62D5_MODE1)&&!VF_YSTDC_INJPHASE){VF_MODE1_INJPHASE=true;}
  
    //2008-8-27 add condiction VF_SWITCHPHASE_INJPHASE ,for insure there is no addtional or lest oil when shift phase;
	if((VF_MODE1_INJPHASE&&(VF_YSTDC_INJPHASE||VF_SWITCHPHASE_INJPHASE))||(!VF_MODE1_INJPHASE&&!VF_YSTDC_INJPHASE)){
	    VF_FORINJ_INJPHASE=true;
	    VF_SWITCHPHASE_INJPHASE=false;//2012-12-27
	}
	else{VF_FORINJ_INJPHASE=false;}
	
	//2019-2-21 F5, B7	
	VF_FORINJ_INJPHASE=!VF_YSTDC_INJPHASE;
	VF_MODE1TO2_INJPHASE=false;
}

//********************	;CACULATING THE AVERAGE OF RPM156 by 4 run average, AND RETURN THE CHAGNE OF RPM15.6
//计算转速RPM156H,L的16圈累加值和平均值
//当处于非启动状态，且转速低于一定值（接近怠速时），计算rpm差的累加值和平均值；               
void AV_RPM156_GET_RPMDROP_AT_LOWRPM(void)
{
    byte templ;
    sbyte sb2;
    word w;
    
    templ=(byte)v_rev_15d6rpm;
    //if((byte)(v_rev_15d6rpm & 0xff)>=128)templ=127;
    if(v_rev_15d6rpm>=128)templ=127;
    if(VF_STARTS_ENG){
    		v_rev_156ave=templ;
    		sum_rpm156=(v_rev_156ave<<4)&0x0ff0;
    		sum_drpm156=0x20;
    		//2008-9-25 add
    		v_rev_156ave_512=templ;
    		sum_rev_156_512=v_rev_156ave_512;
    		sum_rev_156_512=((sum_rev_156_512<<6)&0x1fe00);
    }
    else{
    		sum_rpm156=sum_rpm156-v_rev_156ave+templ;
    		//2008-9-25 add
    		sum_rev_156_512=sum_rev_156_512-v_rev_156ave_512+templ;
    		v_rev_156ave_512=(sum_rev_156_512>>6)&0xff;
    		
    		v_rev_156ave=(byte)(sum_rpm156/16);
    		if((v_rev_156ave< M0_MAX_RPM_CONIDLE) && (templ< 5+M0_MAX_RPM_CONIDLE))
    		{   //当平均转速<1718且瞬态转速<1796时进入
    			//计算rpm差的累加值和平均值；
    			if(v_rev_156ave<templ)number_av_gtrpm=0;
    			//number_av_gtrpm:代表，低速时，当前转速连续小于平均值的次数；疑问
    			//sum_drpm156:代表，低速时，当前转速连续小于平均值次数小于16次时，与当前转速与平均值差异的累加值
    			if(number_av_gtrpm<255)number_av_gtrpm++;
    			if(number_av_gtrpm<16)
    			{
              sb2=(sum_drpm156>>4)&0xf;
              sum_drpm156=sum_drpm156-sb2;//-(sum_drpm156/16) which is the average od drpm156
              sb2=(v_rev_156ave-templ);
              if(templ>v_rev_156ave)sb2=-sb2; //即取绝对值；
              w=sum_drpm156+sb2;
              //if(w>255)sum_drpm156=(byte)w;else w=255;//sum_drpm156=min_byte(sum_drpm156+sb2,255);
              if(w>255)sum_drpm156=(byte)w;else sum_drpm156=255;//sum_drpm156=min_byte(sum_drpm156+sb2,255);  //2008-7-8 rectify
    		    	
    			}
    		}
    }
    return;
}

