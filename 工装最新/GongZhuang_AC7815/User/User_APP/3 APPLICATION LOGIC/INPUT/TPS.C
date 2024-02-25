/*HEAD
 ***********************************************************************
 * Filename	:tps.c
 * Function	:节气门相关计算	
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.0.1
 *
 *** History ***
 * V8.0.0   2017.2.27	xielian 
 *		Base module.
 * 
 * V8.0.1   2019.1.30	xielian
 *		VF_E0LAST_TPS计算地方增加newcycle条件
 * V8.1.0   2019.2.3	xielian
 *		增加VF_E0LAST_TPS_RD用于油门相关计算，增加v_TPd_lst_in1cycle和v_TPd_lst2_in1cycle
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"

#include "ad.h"
#include "add.h"

#include "dtc.h"
#include "time_ccl.h"

//#include "wait_moving.h"


#define BOC_ENABLE false
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_TPS_STATUS;
volatile V_STATUS_STR _V_TPSH_STATUS; //2009-9-7
//#pragma DATA_SEG DEFAULT
byte v_TPS;       //上一次JQMWZ采样值的高8位		

byte v_TPS_bTrans;       //上一次JQMWZ采样值的高8位		

sbyte v_TPS_ddt;        //本次TPS-上次TPS		          yte v_TPS_odr;           //TPS大级别			
byte v_TPS_mode;          //TPS小级别

byte v_TPS_odr;         //TPS级别  	


byte t_1s_NIncTps;//2010-5-14

byte v_TPd;//油门级别　2012-8-20
byte v_TPd_lst;//油门级别　2012-11-22
byte v_TPd_lst2;//油门级别　2012-11-22

byte v_TPd_lst_in1cycle;
byte v_TPd_lst2_in1cycle;

byte nb_BH;                           
byte nb_BH1;  

byte v_TPS_lst; //2012-11-14
byte nb_leavidle;


//2015-4-9 for test tps max or min
word v_TPS_forTLast;
unsigned long int v_TPS_forTSum;
word v_TPS_forTNow;

byte VF_E0LAST_TPS_RD;
extern byte VF_STEP_TRANS;;
                         
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

#define MIN_JQMWZ0AD10	80//EQU	80 ;2004.11.20    ;FOR CTS TP	TPS LIMITED TO 15-28 AS IDLE
#define JQMWZ0AD10_BOS  112

//#if ECU_TYPE==ECU_GEN
#ifdef IS_GEN

    //2014-4-27 对发电机最大TPD的限制
    #define MAX_JQMWZ0AD10	92//2013-7-8 add 
#else 

	#ifdef	SY_HUANSONG		//20210929 chenwei add 环松 两种节气门体初始电压0.8-0.8和0.8-1V，担心限制，增加此选项
	#define MAX_JQMWZ0AD10	205
	#else
    #define MAX_JQMWZ0AD10	192//2020-7-1 CR 0.45-0.95V 140//EQU	140 ;2006-3-23 THE MAX IDLE_TPS=140/4=35 FOR 8BIT A/D
	#endif
	
#endif

#define JQMWZ0D5 	4//EQU 4 		;THE 9TH BITS OF JQMWZ IS IN THE 4TH BIT OF JQMWZ_LOW2

#define JQMWZ0		0           //EQU	0	;0-5.3	    :0-23 4/PER1.0
#define JQMWZJ8		24*4        //EQU	24	;6.0-10.3	:23-63,8/PER1.0
#define JQMWZJ16	64*4        //EQU	64	;11.0-13.3	:64-112 16/PER1.0
#define JQMWZJ32	112*4       //EQU	112	;14.0-14.3	:104:135 32/PER1.0
#define JQMWZ15		144*4       //EQU	144	;15.0-15.3	:136-167 32/PER1.0
#define JQMWZ_MAX	176*4       //EQU	176	;>=176,JQM REACH TO MAX

//#define JQM_MAX	252//EQU	252	;WHEN	JQMWZ>JQM_MAX	OR	JQMWZ<JQM_MIN,	SET	BJQM_ER		
#define JQM_MAX	245//2016-3-2 FROM 252 TO 245
//#define JQM_MIN	16//12	;6		    //2010-5-15 change from 20 to 16
#define JQM_MIN	18//12	;6		    //2010-5-15 change from 20 to 16

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte v_TPS0_AD10	;//	5AH					;节气门位置的0点；MEMORY	FOR	SAVE	JQMWZ0	

word v_TPS_AD10;

byte nb_tps_ad;

byte t_tpse0_s; //2009-12-26
byte nb_Break_ON;//2010-5-11
byte nb_Hold_ON;//2010-12-30

word v_TPS0_256sum;
byte v_TPS0_min;
word v_TPS_16sum;//2013-4-9

byte v_TPS0_ub;//upper limit of TPS0;
byte nb_tps_er;   
//byte nb_tps_back;  //2013-4-8
//byte v_TPS0_min_back;

byte nb_tps0;//2013-4-14

byte v_bTPS_AD10_lst;//2013-4-14

byte v_T_percent;//2016-3-2 增加负荷百分比
     
//#define NB_TPSER_LIMIT  5
#define NB_TPSER_LIMIT  5

#ifdef	SY_TPS_40mv		//20210923 chenwei add
const sword M2_DTPS_IDLE[10][2]=					//		
{	6	,	0 	,	//	
    7	,	1 	,	//	
    8	,	2 	,	//	
    9	,	3 	,	//
	10	,	4 	,
	11	,	6 	,
	12	,	8 	,
	13	,	10 	,
	14	,	12 	,
	15	,	15 	,		
};
#endif

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Get_TPS_odr(word tps);
void jqm_er_order(word rev156);
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：复位后TPS参数的初始化
void FI_TPS_AfterReset()
{
    if(VF_VPOR_HARD||v_TPS0_AD10< MIN_JQMWZ0AD10)//2008-3-12 为防止其它原因引起的复位而增加的条件
    {
        v_TPS0_min=v_TPS0_AD10=MAX_JQMWZ0AD10;
        v_TPS0_ub=v_TPS0_AD10+2;
        VF_TPS0_MIN_GOTTEN1ST_TPS=VF_TPS_AFTER_GOTTEN1ST_TPS=false;
        nb_tps0=0;
        
    }
    
    //2013-4-9
    if(VF_VPOR_HARD){
        v_TPS_16sum=0;
    }
    
    v_TPS0_256sum=(word)(((word)v_TPS0_AD10<<8)&0xff00);// v_TPS0_AD10*256

    V_TPS_STATUS=false;//2008-12-15
    V_TPSH_STATUS=false;//2009-9-7
    //if(VF_VPOR_HARD)
    nb_tps_er=0; //2010-2-7
    t_tpse0_s=0;

    t_1s_NIncTps=0;
    

    #if BOC_ENABLE    
        nb_Break_ON=0;
        nb_Hold_ON=0;
    #endif
    
    v_TPS_forTLast=v_TPS_forTSum=v_TPS_forTNow=0;
    
    v_T_percent=0;
	VF_STEP_TRANS=false;
	
	VF_NEEDADDFUEL_CR=false;
	nb_leavidle=0;
        
}

//保存TPS的信息，作为上一次的信息，以备下一次TPS来临时，作为比较之用
//2014-4-6 
void FS_TPSlst(void){
    v_TPS_lst=v_TPS;
    v_TPd_lst2=v_TPd_lst;
    v_TPd_lst=v_TPd;//2012-11-22
    
}
//功能：根据AD采集结果计算TPS其它参数及变化量；
//      寻找怠速零点位置对应的TPS采样值；
//      诊断TPS;
//      当TPS有误时，采用转速值预测TPS，进入跛行程序
//形参：rev156--v_rev_15d6rpm;
void FG_TPS(word rev156)
{
    sword sw;

    byte bTPS_AD10;

    //if(nb_tps_ad==4)
    {
          v_TPS_AD10=v_TPS_ad10bit;////4次10BIT AD结果的平均；

        
          if(v_TPS_AD10>MAX_JQMWZ0AD10)bTPS_AD10=MAX_JQMWZ0AD10;
          else bTPS_AD10=v_TPS_AD10;
          
          if(v_TPS_16sum==0){
              v_TPS_16sum=((word)bTPS_AD10<<4)&0xfff0;
          } else {
              v_TPS_16sum=v_TPS_16sum+bTPS_AD10-(byte)(v_TPS_16sum>>4);
          }
          
          //if(VF_MMOVE_ENG||VF_SAMPATIME_STOP)
          {

              if(VF_MMOVE_ENG||VF_SAMPATIME_STOP){ //往上走
                  if(!VF_MMOVE_ENG) bTPS_AD10=(byte)(v_TPS_16sum>>4);//如果发动机未运行，则取平均值
                  if(bTPS_AD10<MIN_JQMWZ0AD10)bTPS_AD10=MIN_JQMWZ0AD10;
                  
                  if(VF_TPS0_MIN_GOTTEN1ST_TPS&&VF_TPS_AFTER_GOTTEN1ST_TPS){
                      
                      if(bTPS_AD10+12<v_TPS0_AD10){
                          nb_tps0++;
                          if(nb_tps0>16){
                              VF_TPS0_MIN_GOTTEN1ST_TPS=VF_TPS_AFTER_GOTTEN1ST_TPS=false;
                              nb_tps0=0;    
                          }
                      } else nb_tps0=0;
                  }

                  if(!VF_TPS0_MIN_GOTTEN1ST_TPS){
                      if(v_TPS0_min<=bTPS_AD10)nb_tps0++;
                      if(nb_tps0>=3){
                          VF_TPS0_MIN_GOTTEN1ST_TPS=true;
                          nb_tps0=0;
                          
                      }
                  }
                  
                  if(VF_TPS0_MIN_GOTTEN1ST_TPS){
                      if(!VF_TPS_AFTER_GOTTEN1ST_TPS){
                          if(bTPS_AD10<=v_bTPS_AD10_lst){
                              nb_tps0++;
                              if(nb_tps0>=3){
                                  VF_TPS_AFTER_GOTTEN1ST_TPS=true;        
                                  nb_tps0=0;
                              }
                          } else{
                              nb_tps0=0;
                          }
                      }
                  }

                  if(bTPS_AD10<v_TPS0_AD10){
                      //如果当前TPS比TPS0小
                      //if(sw+16<v_TPS0_AD10) {
                          //如果当前TPS比TPS0小很多
                          //v_TPS0_AD10=v_TPS0_AD10/2+v_TPS_AD10/2;    
                          
                          sw=((sword)bTPS_AD10-v_TPS0_AD10);
                          if(!VF_TPS0_MIN_GOTTEN1ST_TPS)sw=sw*196;
                          else sw*=16;
                          v_TPS0_256sum=v_TPS0_256sum+sw;    

                      //nb_tps_back=3;  2013-4-14
                  
                  } else{

                      if( (bTPS_AD10<v_TPS0_AD10+16)
                          &&(bTPS_AD10<MAX_JQMWZ0AD10))
                      {
                            //VF_TPS_AFTER_GOTTEN1ST_TPS
                            //      if(VF_MMOVE_ENG)
                            sw=((sword)(bTPS_AD10)-v_TPS0_AD10);    
                            if(!VF_TPS_AFTER_GOTTEN1ST_TPS){
                                sw=sw*128;    
                            } else if(!VF_MMOVE_ENG){
                                sw*=8;
                            }
                            v_TPS0_256sum+=sw;
                      }
                      
                      
                  }
              }
              
              v_TPS0_AD10=(byte)(v_TPS0_256sum>>8);
              v_bTPS_AD10_lst=bTPS_AD10;

              bTPS_AD10=0;
              if(v_TPS0_AD10>v_TPS0_min+20){
                  v_TPS0_AD10=v_TPS0_min+20;
                  bTPS_AD10=1;
              }
          	  
          	  if(VF_TPS_AFTER_GOTTEN1ST_TPS){
          	      if(v_TPS0_AD10>v_TPS0_ub){
          	          v_TPS0_AD10=v_TPS0_ub;
          	          bTPS_AD10=1;
          	      }
          	  }
          	  if(v_TPS0_AD10>=MAX_JQMWZ0AD10){
          	      v_TPS0_AD10=MAX_JQMWZ0AD10;
          	      bTPS_AD10=1;
          	  }

          	  //2014-12-9 考虑在这里增加迟滞
              	  if(VF_E0_TPS &&!VF_E0LAST_TPS_RD){
              	      v_TPS0_AD10+=6;
              	      bTPS_AD10=1;
              	              
              	  }
          	  
          	      if(!VF_E0_TPS &&VF_E0LAST_TPS_RD){
              	      v_TPS0_AD10-=6;
              	      bTPS_AD10=1;
          	      }


              if(bTPS_AD10)v_TPS0_256sum=(word)(((word)v_TPS0_AD10<<8)&0xff00);// v_TPS0_AD10*256

          } 
          
              if(v_TPS0_ub>v_TPS0_AD10+2)v_TPS0_ub=v_TPS0_AD10+2;
              
              if(VF_E0_TPS &&!VF_E0LAST_TPS_RD){
                  v_TPS0_ub=v_TPS0_AD10+2;    
              }
              
              //最小TPS0；
              if(v_TPS0_min>v_TPS0_AD10)v_TPS0_min=v_TPS0_AD10;
        
          //计算与TPS相关的参数
              VF_INCTHAN6_TPS=false;//2009-9-7

              /*
              v_TPS_lst=v_TPS;
              *///2014-4-6 MOVE TO FS
                
        	    v_TPS=(v_TPS_AD10>>2)&0xff;//相当于8BITAD；
      	
      	     // v_TPS_bTrans=v_TPS_lst;//2013-5-16      //2013-5-17 DELETE
      	      
          	  sw=(sword)v_TPS-(sword)v_TPS_lst;
          	  if(sw>6)VF_INCTHAN6_TPS=true;
      	
          
              
              {
              	//获取TPS级别
              	Get_TPS_odr(v_TPS_AD10);
              }

  	}
	
	
  	if(VF_MMOVE_ENG||VF_SAMPATIME_STOP){
  	    //2013-5-22 ADD CONDICTION TO TPS DIAG
    	//
        //2008-3-17 从VF_MMOVE_ENG条件中挪出
        //VF_ER_TPS=false; //2009-12-11 delete 一旦出现
        if(v_TPS<JQM_MIN||v_TPS>=JQM_MAX)
        {
            if(nb_tps_er<255)nb_tps_er++;//VF_ER_TPS=true
        } else 
        {
            VF_ER_TPS=false;//2010-2-7
            nb_tps_er=0;//2010-2-7
        }

        //2016-2-28
            if(v_TPS<JQM_MIN)Accu_AD_Mv(MV_TPS,MV_L_LIMIT);
            else if(v_TPS>JQM_MAX)Accu_AD_Mv(MV_TPS,MV_U_LIMIT);
            else Accu_AD_Mv(MV_TPS,MV_NORMAL);
            
        
        if(nb_tps_er>NB_TPSER_LIMIT)VF_ER_TPS=true;     //2009-12-11 add 
        
        //---jqm_er_order-----------
        jqm_er_order(rev156);
	  }
    



}

   
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//由v_TPS0_AD10，jqmwz_low8_last，jqmwz_h2_last计算得到ORDER_JQMH4，和ORDER_JQML4,并计算TPS增量；
void Get_TPS_odr(word tps)              //计算ORDER_JQMH4和ORDER_JQML4
{ 
//    byte j0;//=76;
    sword dtps;
    sbyte sb;
    word w;
	sword sw;
	byte b;

    VF_MAXLST_TPS=VF_MAX_TPS;   //2009-12-16 
    
    VF_MAX_TPS=false;
    VF_UB_TPS=false;
 	    
    dtps=tps-v_TPS0_AD10;//?
	
	#ifdef	SY_TPS_40mv		//20210923 chenwei add TPS-40mv
	
		dtps=dtps+6;//2020-11-26 针对tps 斜率40mv的处理
	
    //可能有疑问?
    if(dtps>=0)
    {
       if(dtps<16)//2020-11-26 针对tps 斜率40mv的处理
		{
			v_TPS_odr=0;
			
			v_TPS_mode=F_2DLookUpSword(dtps,M2_DTPS_IDLE,9);
			if(v_TPS_mode>15)v_TPS_mode=15;			
		}		
		else if(dtps<JQMWZJ8) 
        {   
            //v_TPS_odr=(byte)(dtps/16);
            v_TPS_odr=(byte)((dtps>>4)&0x0f);
            v_TPS_mode=dtps&0x0F;
        } 
        else if(dtps<JQMWZJ16) 
        { 
            //v_TPS_odr=(dtps-JQMWZJ8)/32+6;
            //v_TPS_mode=((dtps-JQMWZJ8)/2) & 0x0F;
            v_TPS_odr=(((dtps-JQMWZJ8)>>5)&0x07)+6;
            v_TPS_mode=((dtps-JQMWZJ8)>>1) & 0x0F;
        } 
        else if(dtps<JQMWZJ32)
        { 
            //v_TPS_odr=(dtps-JQMWZJ16)/64+11;
            //v_TPS_mode=((dtps-JQMWZJ16)/4) & 0x0F;
            v_TPS_odr=(((dtps-JQMWZJ16)>>6)&0x03)+11;  //2009-3-7 修正JQMWZJ8为 JQMWZJ16
            v_TPS_mode=((dtps-JQMWZJ16)>>2) & 0x0F;    //2009-3-7 修正JQMWZJ8为 JQMWZJ16
        } 
        else if(dtps<JQMWZ_MAX)
        {
            //v_TPS_odr=(dtps-JQMWZJ32)/128+14;
            //v_TPS_mode=((dtps-JQMWZJ32)/8)&0x0F;
            v_TPS_odr=(((dtps-JQMWZJ32)>>7)&0x01)+14;  //2009-3-7 修正JQMWZJ8为 JQMWZJ32
            v_TPS_mode=((dtps-JQMWZJ32)>>3) & 0x0F;    //2009-3-7 修正JQMWZJ8为 JQMWZJ32
			
				if((v_TPS_odr==0x0f)&&(v_TPS_mode>=12))//2020-11-26 针对tps斜率40mv的特殊处理
				{				
					v_TPS_odr = 0x0f;
					v_TPS_mode= 0x0f;
					VF_MAX_TPS= true;
					VF_UB_TPS = true;
				}
        }
        else
        {
            v_TPS_odr = 0x0f;
            v_TPS_mode= 0x0f;
            VF_MAX_TPS= true;
            VF_UB_TPS = true;
        }
    }
	
	#else		//20210923 chenwei add TPS-50mv
	
	//可能有疑问?
    if(dtps>=0)
    {
        if(dtps<JQMWZJ8) 
        {   
            //v_TPS_odr=(byte)(dtps/16);
            v_TPS_odr=(byte)((dtps>>4)&0x0f);
            v_TPS_mode=dtps&0x0F;
        } 
        else if(dtps<JQMWZJ16) 
        { 
            //v_TPS_odr=(dtps-JQMWZJ8)/32+6;
            //v_TPS_mode=((dtps-JQMWZJ8)/2) & 0x0F;
            v_TPS_odr=(((dtps-JQMWZJ8)>>5)&0x07)+6;
            v_TPS_mode=((dtps-JQMWZJ8)>>1) & 0x0F;
        } 
        else if(dtps<JQMWZJ32)
        { 
            //v_TPS_odr=(dtps-JQMWZJ16)/64+11;
            //v_TPS_mode=((dtps-JQMWZJ16)/4) & 0x0F;
            v_TPS_odr=(((dtps-JQMWZJ16)>>6)&0x03)+11;  //2009-3-7 修正JQMWZJ8为 JQMWZJ16
            v_TPS_mode=((dtps-JQMWZJ16)>>2) & 0x0F;    //2009-3-7 修正JQMWZJ8为 JQMWZJ16
        } 
        else if(dtps<JQMWZ_MAX)
        {
            //v_TPS_odr=(dtps-JQMWZJ32)/128+14;
            //v_TPS_mode=((dtps-JQMWZJ32)/8)&0x0F;
            v_TPS_odr=(((dtps-JQMWZJ32)>>7)&0x01)+14;  //2009-3-7 修正JQMWZJ8为 JQMWZJ32
            v_TPS_mode=((dtps-JQMWZJ32)>>3) & 0x0F;    //2009-3-7 修正JQMWZJ8为 JQMWZJ32
        }
        else
        {
            v_TPS_odr = 0x0f;
            v_TPS_mode= 0x0f;
            VF_MAX_TPS= true;
            VF_UB_TPS = true;
        }
    }
	
	#endif
	
    else
    {
        v_TPS_odr=0;
        v_TPS_mode=0;
    }
    
//    #if ECU_TYPE==ECU_GEN
    #ifdef IS_GEN
    
        //2014-4-27 发电机油门级别不低于1.1级
        if(v_TPS_odr<1){
            v_TPS_odr=1;
            v_TPS_mode=1;
        }
    #endif

    
    VF_WOT_TPS=false;
    if(v_TPS_odr==0x0f)VF_WOT_TPS=true;
    
    //2019-1-30 增加newcycle条件，否则VF_E0LAST_TPS相关计算标志位有概率不置位
	if(VF_NEWCYCLE_ATMAIN_CAS)
	{
		VF_E0LAST_TPS=VF_E0_TPS;
	}
	
	VF_E0LAST_TPS_RD=VF_E0_TPS;//2019-1-31 新增，用于计算油门TPD
	
    if(v_TPS_odr==0)
    {
        //2009-12-26 ad dVF_E0MORETHAN4S_TPS;
        if(VF_1S_TMCCL)t_tpse0_s++;
        if(t_tpse0_s>=4)  //2010-3-20
        {
            t_tpse0_s=4;
            VF_E0MORETHAN4S_TPS=true;
        }
        VF_E0_TPS=true;
        if(v_TPS_mode<M0_TPS_IDLE_L) v_TPS_mode=M0_TPS_IDLE_L;
        //希望通过增加标志位将VF_MAXRPMGETAFTERST_IDLE移出本函数
        //VF_STILLLE2_TPS=true;
    } else 
    {
        VF_E0_TPS=false;//2009-1-13 add 
        t_tpse0_s=0;
        VF_E0MORETHAN4S_TPS=false;
    }
	if(VF_MMOVE_ENG&&VF_E0_TPS&&(!VF_E0LAST_TPS))
		{
			v_t_afterbackidle_d1s=0;
		}
		
	if(VF_MMOVE_ENG&&((VF_E0_TPS&&!VF_E0LAST_TPS_RD)||(!VF_E0_TPS&&VF_E0LAST_TPS_RD)))
	{
		VF_STEP_TRANS=true;
	}
	
	//2020-7-4 检测到离开怠速时的加喷状态
	if(VF_MMOVE_ENG&&!VF_E0_TPS&&VF_E0LAST_TPS)
	{
		#ifdef	TE_10K	//20210728 chenwei add 10K缸温
		if(v_te<100)
		#endif
		
		#ifdef	TE_2K	//20210728 chenwei add 2K水温
		if(v_te<70)
		#endif
		{
			b=20;
		}
		else
		{
			b=30;
		}
		if((nb_leavidle>b)||(VF_IGNONLY_CFCI&&VF_YSTDC_INJPHASE))VF_NEEDADDFUEL_CR=true;//2020-11-26 取消加喷转速限制
		nb_leavidle=0;

	}
	
	sw=(sword)v_TPS-(sword)v_TPS_lst;
	if(VF_NEEDADDFUEL_CR&&(VF_E0_TPS||(sw<-6)))
	//离开怠速，很快又回到怠速，取消加喷
	//离开怠速后，检测到油门下降，取消加喷
	{
		VF_NEEDADDFUEL_CR=false;
	}
		
	//if(VF_NEWCYCLE_ATMAIN_CAS)
	//{
		//FG_EngStatus_Just();
	//}

    //------Get_rpm156_idle_odf_fromTPS_to_w;-----------------    
    //当TPS处于怠速位置时，order_jqml4范围是0-0.13,对应order_jqml4为0-15；
    //rpm156_idle_odf是为了得到当前order_jqml4与dsymwzsdad10的差异，即怠速油门的大小；

    
    if(v_TPS_odr>=M0_JQM_DRY_IGTOR)VF_DRYIGNI_TPS=true;else VF_DRYIGNI_TPS=false;//2008-12-16 
    
    /*
    v_TPd_lst2=v_TPd_lst;
    v_TPd_lst=v_TPd;//2012-11-22
    
    *///2014-4-6 MOVE TO FS
	
	v_TPd_lst2_in1cycle=v_TPd_lst_in1cycle;
	v_TPd_lst_in1cycle=v_TPd;
    
    v_TPd=v_TPS_odr*16+v_TPS_mode;//2012-8-20

    w=v_TPd;
    if(w>250)w=250;
    
    //w=(w*2)/5;
    v_T_percent=(byte)w;

    //2013-4-7 计算油门突增量
        dtps=((sword)v_TPd-v_TPd_lst)/2; //2013-4-7
        if(!VF_LEAVEIDLE_FRE_ENG)
        {
            //2010-1-11 add 
            VF_TPSINCLE0P1_TPS=false;
            //if((dtps>0)&&(dtps<=16))//2010-6-28 chang from 4 to 16，相当于是离开怠速增2级以上，需要加喷
            //2011-5-7 to 6, 0.3级
            //if((dtps>0)&&(dtps<=6))//2010-6-28 chang from 4 to 16，相当于是离开怠速增2级以上，需要加喷
            if((dtps>0)&&(dtps<=3))//2014-1-21  VF_TPSINCLE0P1_TPS
            {   
                VF_TPSINCLE0P1_TPS=true;
            }
            //2009-12-22
            if(v_teng_inuse>32)
            {
                sb=6;
            } else
            {
                sb=v_teng_inuse/8;
            }
            sb+=v_TPS_odr;
            if(sb<=0)sb=1;
            if(((dtps>8||(dtps<-8))&&(VF_GE2200_REV||v_teng_inuse<27))
                )dtps=dtps*24/(sb); //2010-4-3  因冒烟调整小；
            //2010-12-1  add
        } 
        
        if(dtps>120)dtps=120;
        else if(dtps<-120)dtps=-120;
        v_TPS_ddt=(sbyte)dtps;

    return;
}

//********** jqm_er_order****limp program**********************
//remark by zhangping 
//当TPS故障时，运行跛行程序；
//利用转数预测TPS级别
void jqm_er_order(word rev156)
{
    if(VF_ER_TPS){
    	//	VF_GETIDLEFK_CLS=false;//o2_status,ti_shift_t_get=false;
    	    //	when rpm >3993, let v_TPS_odr=3
    		//if((rev156<256)&&(v_TPS_odr<3))
    		if(rev156<256)   //2013-4-7
    		{
    		    //rpm<1248
    		    v_TPS_odr=0;
    		    if(rev156>88)v_TPS_odr=((rev156-88)>>1)&0x7f;//oder_jqm=(rev156-88)/2+6
    		    v_TPS_odr+=6;
                if(v_TPS_odr>0x30)v_TPS_odr=0x30;//if(oder_jqm >3.0)oder_jqm =3.0;
                
                v_TPS_mode=v_TPS_odr&0x0f;//(low 4 bits of oder_jqm)+7
                v_TPS_odr=(v_TPS_odr>>4)&0x0f;//(high 3 bits of oder_jqm)+7
    		}
    		else
    		{
    			v_TPS_odr=3;v_TPS_mode=0;
    		}
    }
}

//2011-121 检查BOS端口
void Check_BOS(void)
{
    #if BOC_ENABLE    
        if(VF_E0_TPS&&(IS_BREAK_ON)) t_1s_NIncTps=4;
        if(VF_1S_TMCCL&&(t_1s_NIncTps>0))t_1s_NIncTps--;
        
        if(IS_BREAK_ON&&(t_1s_NIncTps==0)) //2010-5-14 add condiction 
        {
            if(nb_Break_ON<50)
            {
                nb_Break_ON++;    
            } else
            {
                VF_BOC_TPS=true;        
            }
        } 
        else 
        {
            nb_Break_ON=0;
            VF_BOC_TPS=false;
        }
        
        if(IS_HOLD_ON)
        {
            if(nb_Hold_ON<50)nb_Hold_ON++;
            else VF_HOLD_TPS=true;
        }
        else 
        {
            
            VF_HOLD_TPS=false;
        }
        
        if(VF_BOC_TPS)
        {
            v_TPS0_AD10=JQMWZ0AD10_BOS;    
        }
        
        if(IS_BREAK_ON )nb_BH1++;
        if(IS_HOLD_ON)nb_BH++;
        
    #endif
    
        
}

void SaveADForM(word tps) 
{
    
    if(v_TPS_forTLast<tps&&v_TPS_forTNow<tps){
        v_TPS_forTSum=tps*64;
        v_TPS_forTNow=tps;    
    } else{
        v_TPS_forTSum=v_TPS_forTSum-v_TPS_forTNow+tps;
        
        v_TPS_forTNow=v_TPS_forTSum/64;
    }
    
  //  deDTC=v_TPS_forTNow/4;
    
    v_TPS_forTLast=tps;
        
}

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;rpm156_idle_odf=dsymwzsdad10-v_TPS_mode;
*/
