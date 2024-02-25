/*HEAD
 ***********************************************************************
 * Filename	:afr_starts.c
 * Function	:calculation for afr		
 * User		:xielian
 * Date		:2018.9.26
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2018.9.26	xielian
 *		将车型改为可匹配量后须将预编译部分代码做调整.
 * V8.3.0   2019.2.26	xielian
 *		us,ur功能做调整.
 * V8.3.1   2019.3.1	peter
 *		强化ur作用.
 * V8.4.0   2019.3.14	xielian
 *		冷机U衰减放慢的处理.
 * V8.4.1   2019.5.8	xielian
 *		一体式喷射单元热机预喷量减半.
 * V8.4.2   2019.5.28	xielian
 *		U衰减速度调整.
 * V8.5.0   2019.12.20	xielian
 *		非怠速us不作用，增加油门大于7的条件；非怠速小油门对ur增加的最大值进行限制
 ***********************************************************************
HEAD END*/

#include "public.h"

#include "status_st.h"
#include "events.h"//2009-12-10 

#include "ld.h"

#include "time_ccl.h"
#include "look_up.h"

#include "ad.h"

#include "sci.h"

#include "add.h"

#include "rev.h"//2019-2-27 peter ，为了用v_rev_625rpm

byte v_R_d4;//2019-2-27 发现它没被定义


#define M0_TC_MAX    ((word)600)// 431  
//2012-12-20
// M1_E_M16=(EPSILON*4)^0.4*16						
const byte M2_EP_M64[9][2]={				
	24	,	109 	,
	28	,	114 	,
	32	,	119 	,
	36	,	123 	,
	40	,	127 	,
	44	,	131 	,
	48	,	134 	,
	52	,	138 	,
	56	,	141 	,
};				
					
/*
//2013-4-20 ADD 
const sbyte BYPTR_M1_US_M32[8][2]={																
	-20	,	18	,												
	-10	,	16	,												
	0	,	  14	,												
	10	,	10	,	//2013-4-22 from 12 to 10											
	20	,	6	,	//2013-4-22 from 10 to 7											
	30	,	5	,												
	40	,	4	,												
	50	,	3	,												
};																
*///2013-6-21

/*
//2013-4-20 ADD 
const sbyte BYPTR_M1_G0_M128[8][2]={																
	-20	,	68	,												
	-10	,	68	,												
	0	,	64	,												
	10	,	35	,//2013-4-22 FROM 45 TO 35												
	20	,	25	,//2013-4-22 FROM 30 TO 25												
	30	,	20	,//2013-4-21 FROM 30 TO 20												
	40	,	15	,												
	50	,	12	,												
};																
*/

//2013-6-28
//2013-4-20 ADD 

/*
const sbyte BYPTR_M1_G0_M128[8][2]={																
	-20	,	68	,												
	-10	,	68	,												
	0	,	64	,												
	10	,	30	,//2013-4-22 FROM 45 TO 35,TO 30												
	20	,	20	,//2013-4-22 FROM 30 TO 25,TO 20												
	30	,	12	,//2013-4-21 FROM 30 TO 20,TO 12												
	40	,	12	,												
	50	,	12	,												
};																
*///2013-7-15 合并后删除

/*
//2013-4-20 ADD 
const sbyte BYPTR_M1_UR_M16[8][2]={																
	-20	,	18	,												
	-10	,	16	,												
	0	,	  14	,												
	10	,	12	,												
	20	,	10	,												
	30	,	8	,												
	40	,	6	,												
	50	,	6	,												
};																
*/

/*
//2013-5-15 ADD 
const sbyte BYPTR_M1_UA_M32[8][2]={																
	-20	,	22	,												
	0	,	18	,												
	20	,	  14	,												
	40	,	10	,											
	60	,	8	,											
	80	,	6	,												
	100	,	5	,												
	120	,	4	,												
};																
*/
/*				
//2013-5-15 ADD 
const sbyte BYPTR_M1_UA_M32[8][2]={																
	-20	,	40	,												
	0	,	30	,												
	20	,	24	,												
	40	,	16	,											
	60	,	10	,											
	80	,	6	,												
	100	,	3	,												
	120	,	2	,												
};																
*///2013-6-20

//#define   BYPTR_M1_US_M32 BYPTR_M1_US_M32

#define   v_XZ v_G_M128

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_STARTS_STATUS;   
//#pragma DATA_SEG DEFAULT

#define MO_FMFSTARTS_MAX 85
#define NB_PRESUM_LIMIT 200

byte v_M0_E_M8;
byte v_M0_F_M256;

byte	v_T_M64;
byte	v_UR_M16;
byte	v_UA_M128;

//2015-10-27 new
byte v_f0;
byte v_g;

byte	v_Y_M256;
sbyte  v_UP_M128;//2012-10-23 

word v_u_M128_Cold;//2012-10-24


sword v_du_M128_M5;//2012-11-9


byte v_fmf_idle_st;//怠速油量 2013-3-1

#define M0_T0_273  273

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/
					
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/

sword v_u_M128;


sword u_cold_lst;
word v_msMu;//冷机浪费的燃油；

byte v_UR_M16_ex;  //对U的修正；加速，全负荷等等；


word v_Film_preinj;     //预喷的油膜；

word v_t_exp_M128_M32;    //时间系数：
                        //停机时，与停机时间相关；运行时，与发动机转速相关；
byte v_ep_M64;//  e=eplison^0.4 *16


byte v_us_M16;  //2013-4-14
byte Tci0_D2_stop;

word sigma_N_M16;

byte v_UR_2;// 

byte v_X;
byte v_Z;

//20211014 chenwei add test
sword v_te_keyon;	//开钥匙时的te
sbyte V_Ta_keyon;	//开钥匙ta

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void FI_Starts_AfterReset(void)
{
    if(VF_POR_HARD)  //2009-12-19
    {

        V_STARTS_STATUS=0;
        VF_1STSTART_STARTS=true;
        VF_BE_PREINJ_ENG=false;
    }
    if(VF_VPOR_HARD){
      

        v_UP_M128=0;
        
        v_du_M128_M5=0;
		
		//VF_U_CO=false;
        

    }
	VF_U_CO=false;
    
    VF_HOTSTART_ENG=false;
    
    v_UR_M16_ex=0;//2012-10-19
    u_cold_lst=0;//2012-11-1 add ;
    
    //2012-12-19 add for TC
    sigma_N_M16=0;
    
    v_fmf_idle_st=M0_FMF_IDLE;//2013-2-3
    
    v_UR_2=0;
    
}
//功能：初始化STARTS,//在发动机运转时刻调用
//形参：teng--发动机温度
void FI_Starts_BeforeStarts(sword tw,sbyte tair)
{
    VF_REMAINIDLE_STARTS=true;
}


//功能：计算F
void FC_F(void){
    sword sw;


    //2015-10-27
        v_T_M64=Adj_byte(BYPTR_M0_T_M64,ADJ_T);
    
    //2012-9-29
    //F_lst=v_M0_F_M256=F_2DLookUpSbyte((sbyte)tw,M1_F_M128,7);
    //if((REMARK==MC_U)||!VF_BFIRE_ENG){ //2012-11-9 ADD CONDICTION 
    if((REMARK==MC_U)||!VF_MMOVE_ENG||!VF_BFIRE_ENG||VF_E0_TPS){ //2012-11-22 
        //2014-2-21 ADD CONDICTION VF_E0_TPS;
        //v_M0_F_M256=v_M0_dF_M128+(byte)F_2DLookUpSbyte((sbyte)V_Ta,BYPTR_M1_F_M256,7);   //2012-9-10
        //2015-10-27
        v_M0_F_M256=Adj_byte((byte)F_2DLookUpSbyte((sbyte)V_Ta,(char_P)BYPTR_M1_F_M256,7),ADJ_F);   
        
        //2013-12-5
        //2013-11-28
        sw=V_Tw-v_te;
        if(sw>120)sw=120;//2014-2-21 FROM 30 TO 120
        if(sw<0)sw=0;
        
        v_g=Adj_byte(BYPTR_M0_g_M128,ADJ_g);
        
        v_X=Adj_byte(BYPTR_M0_X,ADJ_X);

        v_Y_M256=Adj_byte(BYPTR_M0_Y_M256,ADJ_Y);
        
        v_Z=Adj_byte(BYPTR_M0_Z,ADJ_Z);
        //v_us_M16=(byte)F_2DLookUpSbyte((sbyte)sw,BYPTR_M1_US_M32,7)+v_dUs;
        /*
        v_us_M16=(byte)F_2DLookUpSbyte((sbyte)sw,p_sbyte_array,7)+v_dUs;//2014-5-16    //2014-10-4 发现没有给p_sbyte_array赋值
        */
        
        //更改如下：
        //v_us_M16=(byte)F_2DLookUpSbyte((sbyte)sw,BYPTR_M1_US_M32,7)+v_dUs;
        v_us_M16=Adj_byte((byte)F_2DLookUpSbyte((sbyte)sw,(char_P)BYPTR_M1_US_M32,7),ADJ_us);//2015-10-27

        //v_XZ=(byte)F_2DLookUpSbyte((sbyte)V_Ta,BYPTR_M1_G0_M128,7)+v_dG;
        v_XZ=Adj_byte((byte)F_2DLookUpSbyte((sbyte)V_Ta,(char_P)BYPTR_M1_G0_M128,7),ADJ_G);//+v_dG;
        
        v_R_d4=Adj_byte(BYPTR_M0_R_D4, ADJ_R );//2015-10-27
        
        //v_M0_E_M8=v_M0_dE_M8+(byte)F_2DLookUpSbyte((sbyte)V_Ta,BYPTR_M1_E_M4,7);//2013-8-26
        //2015-10-27
        v_M0_E_M8=Adj_byte((byte)F_2DLookUpSbyte((sbyte)V_Ta,(char_P)BYPTR_M1_E_M4,7),ADJ_E);
        
    }

    if(VF_IGNONLY_CFCI||(REMARK==MC_U)||!VF_MMOVE_ENG||!VF_BFIRE_ENG||VF_E0_TPS)
      //2014-12-12 纠正
    {
        /*
        sw=V_Tw;
        if(sw>120)sw=120;
        */
        //2014-3-14 for UR
        //sw=V_Tw+V_Ta/2;
        sw=V_Tw+V_Ta; //2014-3-17
        if(sw>120)sw=120;
        if(sw<-20)sw=-20;
        //2014-2-21
        //v_UR_M16=(byte)F_2DLookUpSbyte((sbyte)sw,BYPTR_M1_UR_M16,7)+v_dUr;
        v_UR_M16=Adj_byte((byte)F_2DLookUpSbyte((sbyte)sw,(char_P)BYPTR_M1_UR_M16,7),ADJ_UR);//2015-10-27
    }
    
}

/*2014-4-1 
    将F_Starts_FMF分解为FC_U和FC_msMU两个部分，
    FC_U由一个循环调用一次
    FC_msMU为随时可调用
*/
void FC_U(byte tps_odr,word rev,sbyte ta,sword tw,word fmf_frommap)
{
    byte w;
    sword sw;
    sword sw_ub;//2013-5-19 add 
    sbyte sb;
    sword sw_u;//2012-10-19
    long int lu;

    byte b;
	sbyte sb_t;
	byte b_u;
	sword sw_u_us;
	sword sw_u_ur;
 
    //2014-2-21 v_UR_M16_ex
    if(!VF_E0_TPS){
        if(!VF_IGNONLY_CFCI&&!VF_CICORECING_CFCI){//2014-2-26 add VF_CICORECING_CFCI
            if(VF_RUN64_TMCCL){
                //b=v_UR_M16/
                //w=(word)
				w=v_UR_M16;
				//if(v_TPS_odr<=7)b=b/2;//2019-12-19 add
				if((BYPTR_M0_URNIDLE_CO>0)&&(BYPTR_M0_URNIDLE_CO<255))
				{
					if(v_TPS_odr<=8)
					{
						w=(word)(((unsigned long int)w*v_TPS_odr*BYPTR_M0_URNIDLE_CO)>>9);
					}
				}
				
                if(v_UR_M16_ex<w)v_UR_M16_ex++;//v_UR_M16
				if((v_TPS_odr<=8)&&(v_UR_M16_ex>w))v_UR_M16_ex--;
            }
        }
    }
    
    if(VF_JCI_CFCI){
        if((v_ccl_cf_bak>8)||(v_te<0)){ //2014-3-12, 断油循环如果短的话，为0
            //2013-4-22
            sb=80;    //FROM 64 TO 80    
            if(v_ccl_cf_bak<80)sb=v_ccl_cf_bak;
            sb-=8;//2014-3-12
            
            w=(word)v_UR_M16*sb/80; // 5000RPM CF---60CYCLES 9000RPM CF--110 CYCLES
            if(w>250)w=250;
            if(v_UR_M16_ex<w)v_UR_M16_ex=w;
            
            //v_UR_M16_ex=
            VF_CICORECING_CFCI=true;
            v_t_d1s_afterCI=0;

            //v_UR_2=16;//2014-1-20
            v_UR_2=v_UR_M16_ex;//2014-2-25 FROM 16 TO
        }
    }

    if(VF_CICORECING_CFCI)
    {
        if(VF_1S_TMCCL||!VF_E0_TPS)//2014-2-27 TO VF_RUN16_TMCCL
        {
            if(v_t_d1s_afterCI<40){ //2014-2-26
                //v_UR_M16_ex=(sword)v_UR_M16_ex*BYPTR_M0_L_ACC_M1024/56;//2014-2-27
                v_UR_M16_ex=(sword)v_UR_M16_ex*48/56;//2014-2-28
				
				if(!VF_E0_TPS)v_UR_M16_ex=v_UR_M16_ex/8;
				if(!VF_E0_TPS&&VF_E0LAST_TPS)v_UR_M16_ex=v_UR_M16_ex/8;
            }
        }
    }

    //if(!VF_FORINJ_INJPHASE)return;   //2012-12-3 ADD 

   /*－－－计算U－－－－－－－－－－－*/
      if(tw<80){
          
          sw=tw;
          if(sw<0){
              sw-=2;
              sw/=3;//2013-3-28 对TW在0度以下打折处理
          }
          sw=sw-M0_T_UC;
          if(sw<0)sw=-sw;
          
          //v_M0_E_M8 v_M0_F_M256
          //lu=((((long int)(v_M0_E_M8*(sw)))<<8)&0xffffff00)/M0_T0_273;
            lu=((((long int)(v_M0_E_M8*(sw)))<<9)&0xfffffe00)/M0_T0_273;//2013-12-12      E当前为M4，而不是M8
          if(lu>20000)lu=20000;
          
          sw=F_EXPM100_Lookup((word)lu);
          
          if(tw>M0_T_UC){
              //TW大于M0_T_UC的情况
              sw_u=(word)((v_M0_F_M256*(long int)sw)>>13);               //  /256/32=8192　　13
          } else {
              sw_u=(((long int)v_M0_F_M256<<11)&0xfffff800)/sw;                                         //2048  11
          }


          if(sw_u>2047)sw_u=2047;//2013-4-14
          if(v_us_M16>32)v_us_M16=32;//2013-4-21
          

      } else sw_u=0;
      
      
      v_f0=Adj_byte(BYPTR_M0_f_M128,ADJ_f0);//2015-10-27
      
      //2013-4-22 move here
      if(VF_STARTS_ENG||(v_t_eng_t_s<=8)){     //v_u_M128_Cold
          if(sw_u<v_f0)sw_u=v_f0;//2014-2-18 对U的最小值进行限值
          //#if MV_VEHICLE==HIDER || MV_VEHICLE==WILD  //  2013-10-20
              //if(sw_u<10)sw_u=10; //2015-5-23 DELETE 张建反映热机浓
          //#else
              //if(sw_u<4)sw_u=4; //2013-10-20 add for
          //#endif
      }

      
      //2013-4-22 move here
      //if(VF_STARTS_ENG||VF_CICORECING_CFCI){
	  if(VF_STARTS_ENG||(v_t_eng_t_s<=15)||VF_CICORECING_CFCI){  
          sb=0;
          sw_ub=sw_u;
		  
		  sw_u_us=sw_ub;
		  sw_u_ur=sw_ub;
          
          //if(VF_STARTS_ENG){
		  if(VF_STARTS_ENG||(v_t_eng_t_s<=15)){
              
             
              sb=(word)v_Z*v_t_eng_d1s/64;//2014-2-21
              
              sb=(sbyte)v_us_M16-sb;
              if(sb<2)sb=2;//2013-4-21
              
              //if(!VF_BFIRE_ENG)sb=sb-sb/4;//2013-4-21     //2013-5-14 delete //2013-6-21 ADD FOR US
              //if(!VF_BFIRE_ENG)sb=sb/2;//2014-2-21
              
              //2013-12-5 ADD FOR STARTS LIMIT FOR !E0_TPS
			  //if(!VF_E0_TPS)sb=0; //FOR LIMIT STARTS 2013-11-28
              if(!VF_E0_TPS&&(v_TPS_odr>7))sb=0; 
			  
 
              if(sb>0){
                  sw_u_us=sw_u_us+(sb*(word)sw_ub)/16;//
              }
			  
			  
			  if((BYPTR_M0_U_CO_1ST>32)&&(BYPTR_M0_U_CO_1ST<255))
			  {
				  b_u=BYPTR_M0_U_CO_1ST;
				  //sb_t=(sbyte)(v_te-V_Ta);
				  sb_t=(sbyte)(v_te_keyon-V_Ta_keyon);		//20211014 chenwei 改为开钥匙
				 // if(b_u>64)b_u=64;
				 if(b_u>48)b_u=48;	//20211014 chenwei 64 to 48
				  //if(VF_1STSTART_STARTS&&((sbyte)sb_t<=2))//&&(V_Ta<=30)
				  if(VF_1STSTART_STARTS&&((sbyte)sb_t<=6)&&(V_Ta<=5))	//20211014 chenwei add ta条件
				  {
				    //sw_u=sw_u+sw_u/2;//2					
					  VF_U_CO=true;					  
				  }
				  
				  if(VF_U_CO)
				  {
					  sw_u_us=(sword)((long int)sw_u_us*BYPTR_M0_U_CO_1ST/32);
				  }
			  }
			  
         } 
		 if(VF_CICORECING_CFCI)
		 {

              sw=40-(sword)V_Ta*2;   //2014-3-17    FOR UR
              if(sw<0)sw=0;
              sw+=12;    //2013-4-22 back to 22

              if(!VF_E0_TPS){
                  sw/=4;
              }
              
              if(v_t_d1s_afterCI>sw){
                  VF_CICORECING_CFCI=false;
              } else {

                  //sw_u+=(sword)v_UR_M16_ex*3;//2014-2-27 晚上 10点
                  //2015-6-3 觉得UR对非怠速再给油的贡献太大，更改为
                  sw_u_ur+=(sword)v_UR_M16_ex;
                  
                  if(VF_E0_TPS){
                      sw_u_ur+=(sword)v_UR_M16_ex*2;
                      sw_u_ur+=(sword)v_UR_M16_ex;//2019-2-28 ur作用加强
                  }
                                    
              }
			  //v_u_ur=sw_u_ur;
              
          }
		  
		  if(sw_u_us<sw_u_ur)sw_u_us=sw_u_ur;
          sw_u=sw_u_us;
      }
      
      //2013-6-26 再给油UR的再作用
      
      //2015-6-3 觉得UR对非怠速再给油的贡献太大，更改为,增加油门怠速的条件
      if(VF_E0_TPS){
          sw_u+=v_UR_2;
          sw_u+=v_UR_2;
          sw_u+=v_UR_2;//2019-2-28 ur作用加强
      }
        
        if(v_UR_2>0){

            //2014-2-25
            b=v_UR_2/4;
            
            b+=2;
            if(b>v_UR_2)b=v_UR_2;
            v_UR_2=v_UR_2-b;//2013-6-22
        }

          
      sw_u+=v_du_M128_M5*5;//2012-11-9   //2012-11-21
      if(sw_u<0)sw_u=0;
      

      //sw_u+=v_UR_M16_ex;
      if(u_cold_lst>sw_u)
	  { //2012-11-1 add sw_u>0
          /*
		  sw=(u_cold_lst-sw_u)/4;
          if(sw<1)sw=1;
          u_cold_lst-=sw;*/
		  
		  if(VF_RUN8_TMCCL||(!VF_E0_TPS&&(v_TPS_odr>7))||(VF_RUN8_TMCCL&&VF_CICORECING_CFCI))//2019-3-10 B7
		  //2019-5-20 ur的衰减要加快
    	  {
    		sw=(u_cold_lst-sw_u)/2;
			if(!VF_E0_TPS&&(V_Ta<0))sw=sw/2;//2019-3-14 低温衰减慢一点
    		if(VF_E0_TPS&&(!VF_CICORECING_CFCI))sw/=2;//2019-5-22 4改为2 2019-3-10-b8
    		
			//2020-9-18 CR 放宽衰减速度
			if(V_Ta<0)//2019-5-22 低温us作用较大
			{
				if(VF_E0_TPS&&(!VF_CICORECING_CFCI)&&(sw>15))sw=15;
			}
			else
			{
				if(VF_E0_TPS&&(!VF_CICORECING_CFCI)&&(sw>10))sw=10;
			}
    		
    		if(sw<1)sw=1;
    		u_cold_lst-=sw;
    	  }    
      } else u_cold_lst=sw_u;
      
      v_u_M128_Cold=u_cold_lst;
      

    v_u_M128=v_u_M128_Cold;//+v_UP_M128;//2012-10-23 add v_UP_M128
    if(v_u_M128_Cold<0)v_u_M128=0;


    //2012-11-21
    if(REMARK!=MC_LOADB){
    
        //2016-5-5 考虑U和UP不重复叠加
       
        //v_u_M128+=v_UP_M128;
        //更改如下：
        
        if(v_u_M128<v_UP_M128)
        {
            v_u_M128=v_UP_M128;    
        }
        
    } 
    
    if(v_u_M128<0)v_u_M128=0;//2012-12-22
    if(v_u_M128>=1279)v_u_M128=1279;
    
}

//2014-4-1 计算U的增油量
word FC_msMU(word fmf_frommap)
{
    sword sw;
    sword sw_u;
    
    sword sw_dmap;
    
    

    /*
    #if MV_EXHAUST<PL125

       sw=(word)M0_FMF_IDLE+M0_FMF_IDLE/2;//2014-2-14 因为MINISTAR 110冷车加速老是不够
    #else
        //sw=(word)M0_FMF_IDLE+M0_FMF_IDLE/2; //2013-5-17 add //2014-2-28 DELETE
        sw=fmf_frommap; //2014-2-28   
    #endif
    
    //2013-5-15 ADD FOR NO IDLE
    if(VF_E0_TPS||sw<fmf_frommap){
        sw=fmf_frommap;
    } 
    */
    
    //2014-12-9 ADD g
    //sw=fmf_frommap+(word)v_TPd*v_g/64; //2014-12-11 for starts

    //2016-5-11更改如下：
    
    sw_dmap=((sword)fmf_frommap-M0_FMF_IDLE);
    if(sw_dmap<0)sw_dmap=0;
    
    //sw=fmf_frommap+(word)sw_dmap*v_g/64; //2016-5-11
    sw=fmf_frommap+(word)sw_dmap*v_g/32; //2016-8-12 把小g的作用再加强



   // deDTFI=sw;

    sw_u=(sword)(((unsigned long int)sw*(v_u_M128+128))>>7); //2013-5-15
    
    v_msMu=sw_u-sw;

    //2013-3-1
    if(VF_E0_TPS){
        sw=(word)(((unsigned long int)M0_FMF_IDLE*(v_u_M128+128))>>7);//2013-2-3
        if(sw>255)sw=255;
        v_fmf_idle_st=(byte)sw;
    }

    return fmf_frommap+v_msMu;  //2013-5-15   
}



/*=======关于预喷油量=========================*/
//功能：
//      获得单次预喷油量，
word FMF_PreInj(void)
{   
    return M0_FMF_PREINJ;
}

//功能：  2008-11-24add 
//      总的预喷次数
word NB_Preinj(sword tw,sbyte ta)
{

    byte wnb;
	sbyte sb_t;


    if(!VF_BE_PREINJ_ENG)
    {
        v_Film_preinj=Init_FilmForPreInj(tw,ta);
        
        //2015-5-20 预喷值的处理
        v_Film_preinj=(unsigned long int)v_Film_preinj*(BYPTR_M0_S_ACC_M128+4)/32;
        
        VF_BE_PREINJ_ENG=true;//2015-5-20 移动到下一行来，注意与上一句的顺序
        wnb=v_Film_preinj/M0_FMF_PREINJ+1;
    } else 
    {
        wnb=0;
        v_Film_preinj=0;
    }
    //wnb=0;//2012-10-12临时取消预喷；  //2012-11-18
    //wnb/=2;//2012-11-20
    //if(ta<0)ta=0;
	
	sb_t=(sbyte)(v_te-V_Ta);
	if(VF_1STSTART_STARTS&&((sbyte)sb_t<=2))
	{
		wnb=wnb;
	}
	else
	{
		wnb/=2;
	}
	
	#if (M0_MFUD6==MFUD1_1)
	
		#ifdef	TE_10K	//20210728 chenwei add 10K缸温
		if(v_te>60)//2019-5-8 热机之后减半
		#endif
		#ifdef	TE_2K	//20210728 chenwei add 2K水温
		if(v_te>50)//2019-5-8 热机之后减半
		#endif
		{
			wnb/=2;
		}
	#endif
    
    //2015-10-12 HAOJIN，afr_starts.c 因电池不好，会出现多次预喷的情况，导致不好启动，因此  
    //if(ta>0)wnb=0;
	//sb_t=(sbyte)(v_te-V_Ta);
	//if((!(VF_1STSTART_STARTS&&((sbyte)sb_t<=2)&&(V_Ta<=30)))&&(ta>0))wnb=0;
    
    
    return wnb;
}
/*=======关于启动时第一次喷油量===============*/
//功能： 获得启动第一圈的油量
word FMF_1st_RUN(byte teng_inuse,byte vbat)
{
    sword sw;
               
    sw=0;
    if(teng_inuse<M0_TES_FOR_1STRUN_FMF_CORRECTION)sw=M0_TES_FOR_1STRUN_FMF_CORRECTION-teng_inuse;
    sw=sw+v_FMF_SCI_idle;    //2009-12-10 add 
    //return (word)(M0_FMF_1ST_BASIC+PRE_OIL_INC+sw);  
    return (word)(M0_FMF_IDLE+PRE_OIL_INC+sw);  //2011-5-7
    
                      
}
