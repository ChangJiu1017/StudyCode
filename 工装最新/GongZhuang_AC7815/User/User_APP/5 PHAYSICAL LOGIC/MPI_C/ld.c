/*HEAD
 ***********************************************************************
 * Filename	:ld.c
 * Function	:calculation for load		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		增加冷机高转速加速时对UP和负荷的限制.
 *
 * V8.2.1   2018.9.26	xielian
 *		将车型改为可匹配量后须将预编译部分代码做调整.
 * V8.3.0   2019.3.14	xielian
 *		冷机温度较低时小油门UP最小限制.
 * V8.4.0   2019.7.18	xielian
 *		转速超9000rpm的ldb查表修正.
 * V8.5.0   2019.12.20	xielian
 *		冷机高速减油，取消不作用时的圆滑过渡.
 ***********************************************************************
HEAD END*/

/* Including needed modules to compile this module/procedure */
#include "Public.h"
#include "status_st.h"
#include "Events.h"

#include "MCU.H"

#include "hard_trig.h"
/* Include shared modules, which are used for whole project */
#include "sci.h"
#include "ee.h"
//#include "wait_moving.h"

#include "ad.h"

#include "ee.h"

//
#include "dtc.h"//　
#include "ign.h"//
#include "inj_phase.h"
#include "time_ccl.h"

#include "look_up.h"

#include "inj_notystdc.h"
#include "inj_ctl.h"

#include "status_eng.h"
#include "sci.h"

#include "sys_init.h"

#include "ld.h"

#include "stepmotor.h"

#include "data_inj_source.h"


/*--------负荷预测用变量------------------------*/
word v_t_YSTDC_next;//预测的下一个压缩上止点的时间；
word v_t_TPSb;
byte v_TPSb;
word v_t_TPSc;
byte v_TPSc;
word v_ldb_b;

sword v_dld_dtps;//dtps变化引起的负荷的变化

word v_ld_min;
word v_ld_max;

byte t_uplimt_d1s;

sword tmp_fmf_lst;
sword tmp_dfmf;

sword tmp_idlefmf_lst;
sbyte v_lambda_sltg;

sword v_tw1_inj;
byte t_idleini;


void FI_Ld_AfterReset(void){
    
   // v_ld_min=(byte)(BYPTR_M2_LDB_TPSRPM[0][0]+BYPTR_M2_LDB_TPSRPM[1][0]);   //
   // v_ld_max=(byte)(BYPTR_M2_LDB_TPSRPM[15][0]+BYPTR_M2_LDB_TPSRPM[16][0]);   //
    
    //v_ld_min=((word)Get_Map_byPtr(BYPTR_M2_LDB_TPSRPM,16,0,0)+Get_Map_byPtr(BYPTR_M2_LDB_TPSRPM,16,1,0));   //
    v_ld_min=((word)Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,0,15)+Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,1,15));
	//2015-5-23
    v_ld_max=((word)Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,15,0)+Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,16,0));   //
    
	/*			//20210926 因为BYPTR_M0_LDBMUL作用调整到计算T1之前了，此处不用放大
	if((BYPTR_M0_LDBMUL<=250)&&(BYPTR_M0_LDBMUL>32))
	{
		v_ld_min=(word)((unsigned long int)v_ld_min*BYPTR_M0_LDBMUL/32);
		v_ld_max=(word)((unsigned long int)v_ld_max*BYPTR_M0_LDBMUL/32);
	}*/	
	
	t_uplimt_d1s=0;
	VF_FUELLIMT_TRANS=false;
	VF_IDLEFUELCO_ST=false;
	VF_RPDACC_ST=false;
	t_idleini=0;
}

/*
    2014-4-1将
    FC_Ld拆分为三部分
    void FC_Ld_basic(void);
    void FC_Ld_just(void);
    void FC_mi_trig(void);
*/

void FC_Ld_basic(void)
{
    sword sw;
	
	//以下过程的执行位置与原来的相同即可，由标志位VF_CAL1NEED_REV_STOP控制
      /*------开环、怠速时，基于转速的LAMBDA控制策略-------------*/
      #if ECU_TYPE==ECU_MOT	  	  
	  
          if(VF_1ST_INI_SYSINI&&VF_AQAFTER_PNINI_SYSINI&&VF_MMOVE_ENG&&VF_RUN_MORE5S_TMCCL)
		  {
			   sw=v_n0_low-v_rev_15d6rpm;
			   if(sw<17)//250rpm
			   {
				   if((t_idleini<15)&&VF_1S_TMCCL)t_idleini++;
			   }
			   else
			   {
				   if((t_idleini>0)&&VF_1S_TMCCL)t_idleini--;
			   }
			   
		  }		  		  
		  
		  if((t_idleini>=15)&&!VF_IDLEAIRLESS_IGN)
		  {
			 // F_SearchIdleSpeed(v_rev_15d6rpm,v_teng_inuse,0,v_rev_156ave,vc_IdleSc_CLS2,v_CA_ignition);//+怠速搜索偏移量 //与vc_FMF_starts的位置关系，TOBECHECK
                       //2012-8-2 vc_FMF_starts在此暂时作为0处理
			vc_tw3offset=FS_AQOK_FLAG(0,0,vc_tw3offset,v_FMF_SCI_idle,true);
			
		  }
		  
      #endif

      /*------闭环自学习策略-------------*/

      F_SL_ByCLS(); //2014-4-13 GEN也需要

            
      /*------闭环控制策略-------------*/
      F_CLS_OS(vc_cls2_idlemissf,vc_tw1_IDLESC,v_ldb);//O2闭环反馈控制
      
      /*------氧传感器工作状态判断-------------*/
      Detect_OS_Flu(v_TPS_odr,v_rev_odr,v_t_eng_t_s,v_prd_cas) ;       

      FC_U(v_TPS_odr,v_rev_15d6rpm,V_Ta,V_Tw,v_ldb);
      
      //自学习MAP
      if(VF_RD_NEW_CLSSL){
          
          v_SL_cls=//FG_Map_basic(MAP_SL,v_Rev_sl_rd,v_Revmod_sl_rd,v_TPd_sl_rd,v_TPmod_sl_rd);//获取基础LOAD
                  FG_3DMap((byte *)v_SL_map           ,0,v_Revmod_sl_rd,0,v_TPmod_sl_rd,SL_X_UB,SL_Y_UB,SL_XMODE_NUM,SL_YMODE_NUM,false,true);
          
          v_SL_cls=CheckSlLimit(v_ldb,v_SL_cls,255);//2013-5-9 add 
          
      }
      
      //2013-10-23 对自学习的处理，刚离开怠速时的学习
      if(VF_JLEAVEIDLELST_ENG||VF_JLEAVEIDLE_ENG){ 
          v_SL_cls=0;    
      }
      
      
      if(!VF_FORINJ_INJPHASE){
          v_t_YSTDC_next=v_t_cas0+v_prd_cas+v_prd_cas;
      }
      
}


/*
    计算当前状态下的负荷和油量需求
*/
void FC_Ld_just(void) 
{
      byte tpd;
	  sword sw;
	  byte b;
	  sword sw_ldb_9000;
	  sword sw_ldb_8500;
      
      if(VF_UB_TPS)tpd=16;else tpd=v_TPS_odr;//2015-12-13 for FG_3DMap

      /*------获取map油量-------------*/
      //v_FMF=FG_Ld(TINJ,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,v_MAPS_odr,v_MAPS_mode);//获取基础LOAD
      v_FMF=//FG_Map_basic(TINJ,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode);//获取基础LOAD
          //FG_3DMap(BYPTR_M2_LDB_TPSRPM,v_rev_odr,v_rev_mode,tpd,v_TPS_mode,LDB_X_UB,LDB_Y_UB,LDB_XMODE_NUM,LDB_YMODE_NUM,true,false);
		      FG_3DMap_xyz((byte *)BYPTR_M2_LDB_TPSRPM,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB,(byte *)BYPTR_TPD_M2LDB,15,16,0x01);//2016-9-13

	//20210922 chenwei 备注 此处先屏蔽，改为再载入T1之前对计算完之后的V_FMF放大，现测试看看	  
	 /* if((BYPTR_M0_LDBMUL<=250)&&(BYPTR_M0_LDBMUL>32))
	  {
		  v_FMF=(word)((unsigned long int)v_FMF*BYPTR_M0_LDBMUL/32);
	  }*/
	  
	  
	  if(v_rev_625rpm>REV62D5_9000)
	  {
		  sw_ldb_9000=(sword)Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,tpd,15)*2;		  		  
		  sw_ldb_8500=(sword)Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,tpd,14)*2;
		  
		/*  if((BYPTR_M0_LDBMUL<=250)&&(BYPTR_M0_LDBMUL>32))
		  {
			  sw_ldb_9000=(sword)((unsigned long int)sw_ldb_9000*BYPTR_M0_LDBMUL/32);
			  sw_ldb_8500=(sword)((unsigned long int)sw_ldb_8500*BYPTR_M0_LDBMUL/32);
		  }*/	//20210926 放大系数改到计算t1之前
		  
		  b=v_rev_625rpm-REV62D5_8500;
		  
		  sw=F_LinearInter(sw_ldb_8500,sw_ldb_9000,b,8,false);
		  
		  v_FMF=v_FMF+sw-sw_ldb_9000;		  
		  
	  }

	  
	  
	  //TEST_COME

      /*------获取气量的低速修正系数-------------*/
      //2009-12-28 ADD CONDICTION FOR THIS
      if(v_prd_cash>PCA6500)
      {   
          F_FMF_LowSpeed(v_rev_15d6rpm);            //获取低速修正系数
      }
      
      //-------------------------------------------------------------------/
      //对油量的修正
      /*----------------LAMBDA PLOTUP-------------*/
        v_LambdaTg=//(sbyte)FG_Map_basic((byte)(CO_LAMBDA),v_rev_odr, v_rev_mode, v_TPS_odr, v_TPS_mode); //2008-7-7 rectify
          //(sbyte)FG_3DMap(BYPTR_M2_UP        ,v_rev_odr,v_rev_mode,tpd,v_TPS_mode,   UP_X_UB,UP_Y_UB,UP_XMODE_NUM,UP_YMODE_NUM,false,false);
		      //(sbyte)FG_3DMap_xyz(BYPTR_M2_UP,v_rev_625rpm,tpd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,false);
		      //(sbyte)FG_3DMap_xyz(BYPTR_M2_UP,v_rev_625rpm,v_TPd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,false);
		      (sbyte)FG_3DMap_xyz((byte *)BYPTR_M2_UP,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB,(byte *)BYPTR_TPD_M2LDB,15,16,0x80);//2016-9-13	  

	v_LambdaTg_lookup=v_LambdaTg;	   //20210130 chenwei add存放up查表值
	  F_OVERTMP_ACTION();//20210206 chenwei add
		
      v_LambdaTg=Adj_sbyte(v_LambdaTg,ADJ_UP);//2015-10-27  
	  
	  v_lambda_sltg=v_LambdaTg;  

     //2017-1-10 in ld.c FC_Ld_just 
        if(Is_RunPointMatch(0))
        {
            if(v_LambdaTg>50)v_LambdaTg=50;//    
        }
		  

     /* if(VF_E0_TPS&&(v_LambdaTg>80))
	  {
		  v_LambdaTg=80;
	  }*/		//20210922 杨海反应之前德尔福怠速需要开环，此处暂时先屏蔽，最终方案需要评审
	  
	  if(!VF_E0_TPS&&(v_te<10)&&(v_TPS_odr<=12))
	  {
		  if(v_LambdaTg<70)v_LambdaTg=70;
	  }
	  
	  if(VF_1ST_INI_SYSINI&&VF_AQAFTER_PNINI_SYSINI&&VF_E0_TPS)
	  {
		  if(v_LambdaTg>32)v_LambdaTg=32;
	  }
	  
	  
	  if(v_LambdaTg>80)
	  {
          v_UP_M128=v_LambdaTg-80;
      }
      else 
	  {
		  v_UP_M128=0;
	  }
	  
	  
      /*------1500rpm以下的低速油量的修正-------------*/
      //v_FMF=FC_FMF_LowSpeed(v_FMF,vc_FMF_lspd); //低速修正  
      v_FMF+=vc_FMF_lspd; //低速修正  
            
      /*2012-10-22 用于计算LOAD增量*/
      v_ldb=v_FMF;
	  
	  
	  /*---------2017-3-25 冷机加速对UP和基础负荷的限制------*/
	  //2017-3-24 冷机加速时高转速工况空燃比偏浓，对UP和ldb进行限制，限制作用时间1s	 
      if(BYPTR_M0_FMF_CO!=UP_FMF_NOTEN)
	 {
		 if((v_te<40)&&(v_rev_625rpm>REV62D5_5750)&&(t_uplimt_d1s<10)&&(v_TPS_odr>2))
		 {
			if(VF_D1S_TMCCL)t_uplimt_d1s++;
			if((!VF_TRANSLT10_TRANS)||VF_TRANSGT15_TRANS)
			{
				if(v_LambdaTg>80)
				{
					v_LambdaTg=80;
				}
				if((BYPTR_M0_FMF_CO>1)&&(BYPTR_M0_FMF_CO<150))
				{
					v_FMF=(word)(((unsigned long int)v_FMF*BYPTR_M0_FMF_CO)>>7);
					tmp_dfmf=(sword)(v_ldb-v_FMF);
					tmp_dfmf+=10;
					VF_FUELLIMT_TRANS=true;
				}
			}
			else
			{
				//2017-3-25 限制不作用时，恢复油量应该圆滑处理
				sw=(sword)(v_ldb-tmp_fmf_lst);
				if((sw>0)&&(sw<tmp_dfmf)&&VF_FUELLIMT_TRANS)
				{
					if(VF_RUN2_TMCCL)
					{
						v_FMF=tmp_fmf_lst+1;
					}
					else
					{
						v_FMF=tmp_fmf_lst;
					}
				}
				else
				{
					VF_FUELLIMT_TRANS=false;
				}
			}
		 }
		 else
		 {
			 if(VF_E0_TPS)
			 {
				 t_uplimt_d1s=0;
			 }
			/* 
			 //2017-3-25 限制不作用时，恢复油量应该圆滑处理
			 sw=(sword)(v_ldb-tmp_fmf_lst);
			 if((sw>0)&&(sw<tmp_dfmf)&&VF_FUELLIMT_TRANS)
			 {
				 if(VF_RUN2_TMCCL)
				 {
					 v_FMF=tmp_fmf_lst+1;
				 }
				 else
				 {
					 v_FMF=tmp_fmf_lst;
				 }
			 }
			 else*/
			 {
				 VF_FUELLIMT_TRANS=false;
			 }			 
		 }
		 
		 tmp_fmf_lst=v_FMF;
		 
	 }
	 
	  /*---------回油门后怠速油量修正------------*/
	  if((v_TPd_lst>128)&&(v_TPd<v_TPd_lst))//记录TPD在8级以上的加速状态
	  {
		  VF_RPDACC_ST=true;
	  }
	  
	   if((BYPTR_M0_IDLEFMF_CO>=128)&&(BYPTR_M0_IDLEFMF_CO<255))
	 {
	 	#ifdef TE_10K	//20210728 chenwei add 10K缸温
		 if(VF_E0_TPS&&((v_ccl_cf_bak>6)||(v_te<0))&&(v_te<180)&&(V_Ta<40)&&VF_RPDACC_ST&&((v_t_afterbackidle_d1s>20)&&(v_t_afterbackidle_d1s<100)))//(v_rev_15d6rpm<v_n0_low)&&
		 #endif

		 #ifdef TE_2K	//20210728 chenwei add 2K缸温
		 if(VF_E0_TPS&&((v_ccl_cf_bak>6)||(v_te<0))&&(v_te<100)&&(V_Ta<40)&&VF_RPDACC_ST&&((v_t_afterbackidle_d1s>20)&&(v_t_afterbackidle_d1s<100)))//(v_rev_15d6rpm<v_n0_low)&&
		 #endif
		 
		 {

			 v_FMF=(word)(((unsigned long int)v_FMF*BYPTR_M0_IDLEFMF_CO)>>7);
			 VF_IDLEFUELCO_ST=true;
		 }			
		 else
		 {
			 
			 //不作用时，恢复油量圆滑处理
			 sw=(sword)(tmp_idlefmf_lst-v_ldb);
			 if((sw>0)&&VF_IDLEFUELCO_ST)
			 {
				 if(VF_RUN8_TMCCL&&VF_NEWCYCLE_ATMAIN_CAS)
				 {
					 v_FMF=tmp_idlefmf_lst-1;
				 }
				 else
				 {
					 v_FMF=tmp_idlefmf_lst;
				 }
			 }
			 else
			 {
				 VF_IDLEFUELCO_ST=false;
			 }
			
			if(VF_E0_TPS&&(v_t_afterbackidle_d1s>20))
			{
				VF_RPDACC_ST=false;//回怠速后油量修正时间之前需要保证标志位不被清除;
			}	
			 			 
		 }
		 
		 tmp_idlefmf_lst=v_FMF;
		 
	 }
	  
	  
      
      /*------O值对油量的修正-------------*/
      v_FMF=FC_IdleSC_FMF(v_FMF,vc_tw1_IDLESC_inuse,vc_tw1_IDLESC,v_FMF_SCI_idle,v_FMF_SCI_Nidle,vc_tw3offset);    //怠速搜索值对油量的修正 //2008-7-8 move to here

      /*------闭环控制对油量的修正-------------*/
      //v_FMF=FC_CLS(v_FMF,v_dmLambda);         //+O2反馈控制及获得对油量的修正系数
      v_FMF=v_FMF+v_dmLambda;         //+O2反馈控制及获得对油量的修正系数
      
      if(REMARK==4&&v_FMF<=0)v_FMF=1;//2013-8-15防止FMF出现负值，导致油量异常；at main
      
      /*------REMARK!=6 并且已经初始化后的 时，自学习值对油量的修正-------------*/
      /*------温度对气量的修正-------------*/
      //v_FMF=FC_FMF_TMP(v_FMF,vc_FMF_tair,vc_FMF_teng);//气量的温度修正系数
      
      FC_mti();
      
      v_FMF=(word)(((unsigned long int)v_FMF*v_mti)>>7);//2013-3-15


      
      v_ld_NotAltitude=v_FMF;

      /*------海拔修正-----------------*/ //2012-10-22　move here
      v_mpa=//FG_Map_basic(MPA,v_TPS_odr,v_TPS_mode,v_pair_odr,v_pair_mode);
              //FG_3DMap(BYPTR_M2_MPA_M128  ,v_TPS_odr,v_TPS_mode,v_pair_odr,v_pair_mode, MPA_X_UB,MPA_Y_UB,MPA_XMODE_NUM,MPA_YMODE_NUM,false,false);
		          (byte)FG_3DMap_xyz((byte *)BYPTR_M2_MPA_M128,v_TPd,v_Pair_deg,(byte *)BYPTR_TPD_M2LDB,(byte *)BYPTR_PAIR_DEG,15,7,0x00);//2016-9-13

      v_mpa=Adj_byte(v_mpa,ADJ_mpa);

      v_FMF=(word)(((unsigned long int)v_FMF*v_mpa)>>7);//2013-3-15
      /*-----油门过渡，对负荷的修正-2012-12-4 ADD ---*/


      v_dms=v_dms/8;  //2013-6-22 add 
      v_FMF+=v_dms;

      /*-------------------------------------------*/
      v_ld=v_FMF;//2012-10-22
      
      Co_FMF_Altitude=(sword)v_FMF-v_ld_NotAltitude;
      
      /*2012-10-22 用于计算LOAD增量*/
      v_dld=(sword)v_ld-v_ldb;


}

//计算与油量相关的瞬态值
void FC_mi_just(void) 
{
      word w;
      word sw;

      //2015-4-15 需求对U的基础油量做高原修正，U里面还有UP的，否则太浓
      //Co_FMF_Altitude是负数
      
      //2015-4-15 add 
      sw=(sword)v_ldb+Co_FMF_Altitude;
      if(sw<10)sw=10;


      FC_msMU((word)sw);

      {//!VF_FORINJ_INJPHASE||低速时后计算，高速时先计算
          v_FMF=v_ld+v_msMu;
          if(v_FMF>M0_FMF_FOR1CYCLE)v_FMF=M0_FMF_FOR1CYCLE;//2012-10-9
      }
         
  /*------过渡、恢复供油修正-------------*/

      //2014-12-9

      //FC_Film(v_ld_NotAltitude,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 更改油量基准
      
      /*
      if(!VF_E0_TPS&&(VF_JCI_CFCI||VF_JCI_LST_CFCI)){
          w=v_FMF;
      } else{
          w=v_ldb;
      } //2015-4-9 发现这条语句不合适，会导致后面油门加大时，油膜反而变小
      FC_Film(w,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 更改油量基准
      */
      
      //2015-4-9 油膜总量计算不合理，更改为
      //w=M0_FMF_IDLE+(word)v_TPd;//2015-4-9需要保证油门越大，油膜越大，否则，会出现油门维持，转速上升后油膜减小
      
      if (BYPTR_MV_VEHICLE== WILD)
	  {
		 w=M0_FMF_IDLE+(word)v_TPd;//2015-4-9需要保证油门越大，油膜越大，否则，会出现油门维持，转速上升后油膜减小 
	  }		                 
      else
	  {
		  w=M0_FMF_IDLE+(word)v_TPd*2;//2015-4-9需要保证油门越大，油膜越大，否则，会出现油门维持，转速上升后油膜减小
	  }
          
      

      
      //2015-4-15 油膜的基础油量也需要海拔修正的，add
      w=(word)(((unsigned long int)w*v_mpa)>>7);//2013-3-15
      
      
      FC_Film(w,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 更改油量基准
      
      
      //2012-12-8 add VF_O2CHANGED_OSDIAG
      if(!VF_O2CHANGED_OSDIAG)v_FMF=v_FMF+v_dmidle; //油量的失火修正

}

void FC_mi_trig(void)
{

        v_FMF=F_AttuFilm(v_FMF);
        if(v_FMF>M0_FMF_FOR1CYCLE)v_FMF=M0_FMF_FOR1CYCLE;//2012-10-9

    
        if(VF_FORINJ_INJPHASE){
            
            v_mi=v_FMF;
            v_dmi=(sword)v_mi-v_ld;
            #ifndef SYS_TRADITIONAL
            v_FMF=F_remain_FMF(v_FMF);
			#endif


            //v_FMF_all_lst=v_FMF;
            //注意：v_FMF_remain是加喷圈用的，千万被冲掉了，导致加喷圈油量不对；
        } else 
        {
            v_mi=v_FMF;
            //v_dmi=(sword)v_mi-v_ld; //2016-5-5 删除在FC_mi_trig中的显示 

            #ifndef SYS_TRADITIONAL
			v_FMF=F_remain_FMF(v_FMF);
            #endif
            
        }
        
		#ifndef SYS_TRADITIONAL
        //2013-4-19
        if(v_dt1_for_T35>0){
            v_FMF+=v_dt1_for_T35;
            v_dt1_for_T35=0;
        }
		#endif
 
        //如果是正常喷射的油量，或者是加喷的油量(需要加喷时)
        v_FMF_lst=v_FMF;//
        
        //最后：v_FMF是需要计算T1用的油量

        #ifdef SYS_TRADITIONAL
			//20210922 chenwei add for test
			 if((BYPTR_M0_LDBMUL<=250)&&(BYPTR_M0_LDBMUL>32))
			  {
				  v_FMF=(word)((unsigned long int)v_FMF*BYPTR_M0_LDBMUL/32);
			  }
			
			v_tw1=Ctl_T1_TRAD(v_FMF,v_Vbat_inuse);//+v_Adj[ADJ_T1]*10;
			v_tw1_inj=v_tw1;
		#else		
			v_tw1=Ctl_DT3(v_FMF,v_Vbat_inuse,0,v_TW3,v_TPS_odr,v_TPS_mode,v_rev_156ave,v_teng_inuse,v_tw1_synwithTW3,v_prd_cas);
		#endif
}
/*
    计算负荷
    VF_CAL1NEED_REV_STOP:
          代表是在FG_Rev_AfterIgn后计算的；
          与之相关的过程的执行的位置，与原来的位置相同，
          因此，VF_FORINJ_INJPHASE不需要变更；
          
    VF_CAL2NEED_PHASE_STOP:代表与VF_FORINJ_INJPHASE相关的，取代原来的VF_FORINJ_INJPHASE；
          即，代表所调用的FC_Ld是不是计算正常喷射圈的油量
          代表需要计算正常喷射的油量，而非加喷圈的油量，
*/

//保存基础的TPS和时间
void Save_TPS_basic(byte tps)
{
    //v_t_TPSb=tps;
    v_t_TPSb=T_MAINTMR;
    v_ldb_b=v_ldb;
    v_TPSb=tps;    
}

//预测负荷
void FPredict_Ld(byte tps)       
{
    word dt_1;
    word dt_2;
    
    word wm;
    
    sword dld_1;
    //sword dld_2;
    
    sword dTps;
    
    //sword dTps_pre;
    
    
    
    v_TPSc=tps;
    v_t_TPSc=T_MAINTMR;
    
    dTps=(sword)v_TPSc-v_TPSb; //2015-3-7
    
    //if(dTps>=3||dTps<=-3)
    if(dTps>=5||dTps<=-5)
    {
        dt_1=v_t_TPSc-v_t_TPSb;
        dt_2=v_t_YSTDC_next-v_t_TPSc;
        
        wm=dt_2/dt_1;
        
        //时间太远的话，预测误差太大，因此限定在4倍
        if(wm>4){
            dt_2=dt_1*4;    
        }
        
        dld_1=(sword)v_ldb-v_ldb_b;
        
        v_dld_dtps=((long int)dt_2*dld_1)/dt_1;
        
        //dTps_pre=((long int)dt_2*dTps)/dt_1;
        
        //dTps/
        
        if(dTps<0)v_dld_dtps/=4;//2015-3-7
        
        //v_ldb+=v_dld_dtps; //2015-3-7 delete
        v_ld+=v_dld_dtps;
        
        //2015-5-23，发现这句话中的v_ld_min有问题，导致油门抖动时，v_ldb被限制的比较大
        //if(v_ldb>v_ld_max)v_ldb=v_ld_max;
        //if(v_ldb<v_ld_min)v_ldb=v_ld_min;
        
        if(v_ld>v_ld_max)v_ld=v_ld_max;
        if(v_ld<5)v_ld=5;//2015-5-23
        
        
        
    }
    
}


