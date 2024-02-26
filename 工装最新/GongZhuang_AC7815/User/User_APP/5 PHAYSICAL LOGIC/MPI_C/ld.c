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
 *		���������ת�ټ���ʱ��UP�͸��ɵ�����.
 *
 * V8.2.1   2018.9.26	xielian
 *		�����͸�Ϊ��ƥ�������뽫Ԥ���벿�ִ���������.
 * V8.3.0   2019.3.14	xielian
 *		����¶Ƚϵ�ʱС����UP��С����.
 * V8.4.0   2019.7.18	xielian
 *		ת�ٳ�9000rpm��ldb�������.
 * V8.5.0   2019.12.20	xielian
 *		������ټ��ͣ�ȡ��������ʱ��Բ������.
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
#include "dtc.h"//��
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


/*--------����Ԥ���ñ���------------------------*/
word v_t_YSTDC_next;//Ԥ�����һ��ѹ����ֹ���ʱ�䣻
word v_t_TPSb;
byte v_TPSb;
word v_t_TPSc;
byte v_TPSc;
word v_ldb_b;

sword v_dld_dtps;//dtps�仯����ĸ��ɵı仯

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
    
	/*			//20210926 ��ΪBYPTR_M0_LDBMUL���õ���������T1֮ǰ�ˣ��˴����÷Ŵ�
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
    2014-4-1��
    FC_Ld���Ϊ������
    void FC_Ld_basic(void);
    void FC_Ld_just(void);
    void FC_mi_trig(void);
*/

void FC_Ld_basic(void)
{
    sword sw;
	
	//���¹��̵�ִ��λ����ԭ������ͬ���ɣ��ɱ�־λVF_CAL1NEED_REV_STOP����
      /*------����������ʱ������ת�ٵ�LAMBDA���Ʋ���-------------*/
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
			 // F_SearchIdleSpeed(v_rev_15d6rpm,v_teng_inuse,0,v_rev_156ave,vc_IdleSc_CLS2,v_CA_ignition);//+��������ƫ���� //��vc_FMF_starts��λ�ù�ϵ��TOBECHECK
                       //2012-8-2 vc_FMF_starts�ڴ���ʱ��Ϊ0����
			vc_tw3offset=FS_AQOK_FLAG(0,0,vc_tw3offset,v_FMF_SCI_idle,true);
			
		  }
		  
      #endif

      /*------�ջ���ѧϰ����-------------*/

      F_SL_ByCLS(); //2014-4-13 GENҲ��Ҫ

            
      /*------�ջ����Ʋ���-------------*/
      F_CLS_OS(vc_cls2_idlemissf,vc_tw1_IDLESC,v_ldb);//O2�ջ���������
      
      /*------������������״̬�ж�-------------*/
      Detect_OS_Flu(v_TPS_odr,v_rev_odr,v_t_eng_t_s,v_prd_cas) ;       

      FC_U(v_TPS_odr,v_rev_15d6rpm,V_Ta,V_Tw,v_ldb);
      
      //��ѧϰMAP
      if(VF_RD_NEW_CLSSL){
          
          v_SL_cls=//FG_Map_basic(MAP_SL,v_Rev_sl_rd,v_Revmod_sl_rd,v_TPd_sl_rd,v_TPmod_sl_rd);//��ȡ����LOAD
                  FG_3DMap((byte *)v_SL_map           ,0,v_Revmod_sl_rd,0,v_TPmod_sl_rd,SL_X_UB,SL_Y_UB,SL_XMODE_NUM,SL_YMODE_NUM,false,true);
          
          v_SL_cls=CheckSlLimit(v_ldb,v_SL_cls,255);//2013-5-9 add 
          
      }
      
      //2013-10-23 ����ѧϰ�Ĵ������뿪����ʱ��ѧϰ
      if(VF_JLEAVEIDLELST_ENG||VF_JLEAVEIDLE_ENG){ 
          v_SL_cls=0;    
      }
      
      
      if(!VF_FORINJ_INJPHASE){
          v_t_YSTDC_next=v_t_cas0+v_prd_cas+v_prd_cas;
      }
      
}


/*
    ���㵱ǰ״̬�µĸ��ɺ���������
*/
void FC_Ld_just(void) 
{
      byte tpd;
	  sword sw;
	  byte b;
	  sword sw_ldb_9000;
	  sword sw_ldb_8500;
      
      if(VF_UB_TPS)tpd=16;else tpd=v_TPS_odr;//2015-12-13 for FG_3DMap

      /*------��ȡmap����-------------*/
      //v_FMF=FG_Ld(TINJ,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,v_MAPS_odr,v_MAPS_mode);//��ȡ����LOAD
      v_FMF=//FG_Map_basic(TINJ,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode);//��ȡ����LOAD
          //FG_3DMap(BYPTR_M2_LDB_TPSRPM,v_rev_odr,v_rev_mode,tpd,v_TPS_mode,LDB_X_UB,LDB_Y_UB,LDB_XMODE_NUM,LDB_YMODE_NUM,true,false);
		      FG_3DMap_xyz((byte *)BYPTR_M2_LDB_TPSRPM,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB,(byte *)BYPTR_TPD_M2LDB,15,16,0x01);//2016-9-13

	//20210922 chenwei ��ע �˴������Σ���Ϊ������T1֮ǰ�Լ�����֮���V_FMF�Ŵ��ֲ��Կ���	  
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
		  }*/	//20210926 �Ŵ�ϵ���ĵ�����t1֮ǰ
		  
		  b=v_rev_625rpm-REV62D5_8500;
		  
		  sw=F_LinearInter(sw_ldb_8500,sw_ldb_9000,b,8,false);
		  
		  v_FMF=v_FMF+sw-sw_ldb_9000;		  
		  
	  }

	  
	  
	  //TEST_COME

      /*------��ȡ�����ĵ�������ϵ��-------------*/
      //2009-12-28 ADD CONDICTION FOR THIS
      if(v_prd_cash>PCA6500)
      {   
          F_FMF_LowSpeed(v_rev_15d6rpm);            //��ȡ��������ϵ��
      }
      
      //-------------------------------------------------------------------/
      //������������
      /*----------------LAMBDA PLOTUP-------------*/
        v_LambdaTg=//(sbyte)FG_Map_basic((byte)(CO_LAMBDA),v_rev_odr, v_rev_mode, v_TPS_odr, v_TPS_mode); //2008-7-7 rectify
          //(sbyte)FG_3DMap(BYPTR_M2_UP        ,v_rev_odr,v_rev_mode,tpd,v_TPS_mode,   UP_X_UB,UP_Y_UB,UP_XMODE_NUM,UP_YMODE_NUM,false,false);
		      //(sbyte)FG_3DMap_xyz(BYPTR_M2_UP,v_rev_625rpm,tpd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,false);
		      //(sbyte)FG_3DMap_xyz(BYPTR_M2_UP,v_rev_625rpm,v_TPd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,false);
		      (sbyte)FG_3DMap_xyz((byte *)BYPTR_M2_UP,v_rev_625rpm,v_TPd,(byte *)BYPTR_N_62D5_M2LDB,(byte *)BYPTR_TPD_M2LDB,15,16,0x80);//2016-9-13	  

	v_LambdaTg_lookup=v_LambdaTg;	   //20210130 chenwei add���up���ֵ
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
	  }*/		//20210922 ���Ӧ֮ǰ�¶���������Ҫ�������˴���ʱ�����Σ����շ�����Ҫ����
	  
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
	  
	  
      /*------1500rpm���µĵ�������������-------------*/
      //v_FMF=FC_FMF_LowSpeed(v_FMF,vc_FMF_lspd); //��������  
      v_FMF+=vc_FMF_lspd; //��������  
            
      /*2012-10-22 ���ڼ���LOAD����*/
      v_ldb=v_FMF;
	  
	  
	  /*---------2017-3-25 ������ٶ�UP�ͻ������ɵ�����------*/
	  //2017-3-24 �������ʱ��ת�ٹ�����ȼ��ƫŨ����UP��ldb�������ƣ���������ʱ��1s	 
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
				//2017-3-25 ���Ʋ�����ʱ���ָ�����Ӧ��Բ������
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
			 //2017-3-25 ���Ʋ�����ʱ���ָ�����Ӧ��Բ������
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
	 
	  /*---------�����ź�����������------------*/
	  if((v_TPd_lst>128)&&(v_TPd<v_TPd_lst))//��¼TPD��8�����ϵļ���״̬
	  {
		  VF_RPDACC_ST=true;
	  }
	  
	   if((BYPTR_M0_IDLEFMF_CO>=128)&&(BYPTR_M0_IDLEFMF_CO<255))
	 {
	 	#ifdef TE_10K	//20210728 chenwei add 10K����
		 if(VF_E0_TPS&&((v_ccl_cf_bak>6)||(v_te<0))&&(v_te<180)&&(V_Ta<40)&&VF_RPDACC_ST&&((v_t_afterbackidle_d1s>20)&&(v_t_afterbackidle_d1s<100)))//(v_rev_15d6rpm<v_n0_low)&&
		 #endif

		 #ifdef TE_2K	//20210728 chenwei add 2K����
		 if(VF_E0_TPS&&((v_ccl_cf_bak>6)||(v_te<0))&&(v_te<100)&&(V_Ta<40)&&VF_RPDACC_ST&&((v_t_afterbackidle_d1s>20)&&(v_t_afterbackidle_d1s<100)))//(v_rev_15d6rpm<v_n0_low)&&
		 #endif
		 
		 {

			 v_FMF=(word)(((unsigned long int)v_FMF*BYPTR_M0_IDLEFMF_CO)>>7);
			 VF_IDLEFUELCO_ST=true;
		 }			
		 else
		 {
			 
			 //������ʱ���ָ�����Բ������
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
				VF_RPDACC_ST=false;//�ص��ٺ���������ʱ��֮ǰ��Ҫ��֤��־λ�������;
			}	
			 			 
		 }
		 
		 tmp_idlefmf_lst=v_FMF;
		 
	 }
	  
	  
      
      /*------Oֵ������������-------------*/
      v_FMF=FC_IdleSC_FMF(v_FMF,vc_tw1_IDLESC_inuse,vc_tw1_IDLESC,v_FMF_SCI_idle,v_FMF_SCI_Nidle,vc_tw3offset);    //��������ֵ������������ //2008-7-8 move to here

      /*------�ջ����ƶ�����������-------------*/
      //v_FMF=FC_CLS(v_FMF,v_dmLambda);         //+O2�������Ƽ���ö�����������ϵ��
      v_FMF=v_FMF+v_dmLambda;         //+O2�������Ƽ���ö�����������ϵ��
      
      if(REMARK==4&&v_FMF<=0)v_FMF=1;//2013-8-15��ֹFMF���ָ�ֵ�����������쳣��at main
      
      /*------REMARK!=6 �����Ѿ���ʼ����� ʱ����ѧϰֵ������������-------------*/
      /*------�¶ȶ�����������-------------*/
      //v_FMF=FC_FMF_TMP(v_FMF,vc_FMF_tair,vc_FMF_teng);//�������¶�����ϵ��
      
      FC_mti();
      
      v_FMF=(word)(((unsigned long int)v_FMF*v_mti)>>7);//2013-3-15


      
      v_ld_NotAltitude=v_FMF;

      /*------��������-----------------*/ //2012-10-22��move here
      v_mpa=//FG_Map_basic(MPA,v_TPS_odr,v_TPS_mode,v_pair_odr,v_pair_mode);
              //FG_3DMap(BYPTR_M2_MPA_M128  ,v_TPS_odr,v_TPS_mode,v_pair_odr,v_pair_mode, MPA_X_UB,MPA_Y_UB,MPA_XMODE_NUM,MPA_YMODE_NUM,false,false);
		          (byte)FG_3DMap_xyz((byte *)BYPTR_M2_MPA_M128,v_TPd,v_Pair_deg,(byte *)BYPTR_TPD_M2LDB,(byte *)BYPTR_PAIR_DEG,15,7,0x00);//2016-9-13

      v_mpa=Adj_byte(v_mpa,ADJ_mpa);

      v_FMF=(word)(((unsigned long int)v_FMF*v_mpa)>>7);//2013-3-15
      /*-----���Ź��ɣ��Ը��ɵ�����-2012-12-4 ADD ---*/


      v_dms=v_dms/8;  //2013-6-22 add 
      v_FMF+=v_dms;

      /*-------------------------------------------*/
      v_ld=v_FMF;//2012-10-22
      
      Co_FMF_Altitude=(sword)v_FMF-v_ld_NotAltitude;
      
      /*2012-10-22 ���ڼ���LOAD����*/
      v_dld=(sword)v_ld-v_ldb;


}

//������������ص�˲ֵ̬
void FC_mi_just(void) 
{
      word w;
      word sw;

      //2015-4-15 �����U�Ļ�����������ԭ������U���滹��UP�ģ�����̫Ũ
      //Co_FMF_Altitude�Ǹ���
      
      //2015-4-15 add 
      sw=(sword)v_ldb+Co_FMF_Altitude;
      if(sw<10)sw=10;


      FC_msMU((word)sw);

      {//!VF_FORINJ_INJPHASE||����ʱ����㣬����ʱ�ȼ���
          v_FMF=v_ld+v_msMu;
          if(v_FMF>M0_FMF_FOR1CYCLE)v_FMF=M0_FMF_FOR1CYCLE;//2012-10-9
      }
         
  /*------���ɡ��ָ���������-------------*/

      //2014-12-9

      //FC_Film(v_ld_NotAltitude,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 ����������׼
      
      /*
      if(!VF_E0_TPS&&(VF_JCI_CFCI||VF_JCI_LST_CFCI)){
          w=v_FMF;
      } else{
          w=v_ldb;
      } //2015-4-9 ����������䲻���ʣ��ᵼ�º������żӴ�ʱ����Ĥ������С
      FC_Film(w,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 ����������׼
      */
      
      //2015-4-9 ��Ĥ�������㲻��������Ϊ
      //w=M0_FMF_IDLE+(word)v_TPd;//2015-4-9��Ҫ��֤����Խ����ĤԽ�󣬷��򣬻��������ά�֣�ת����������Ĥ��С
      
      if (BYPTR_MV_VEHICLE== WILD)
	  {
		 w=M0_FMF_IDLE+(word)v_TPd;//2015-4-9��Ҫ��֤����Խ����ĤԽ�󣬷��򣬻��������ά�֣�ת����������Ĥ��С 
	  }		                 
      else
	  {
		  w=M0_FMF_IDLE+(word)v_TPd*2;//2015-4-9��Ҫ��֤����Խ����ĤԽ�󣬷��򣬻��������ά�֣�ת����������Ĥ��С
	  }
          
      

      
      //2015-4-15 ��Ĥ�Ļ�������Ҳ��Ҫ���������ģ�add
      w=(word)(((unsigned long int)w*v_mpa)>>7);//2013-3-15
      
      
      FC_Film(w,V_Tw,V_Ta,v_TPS_odr,v_rev_odr);//2012-10-9 ����������׼
      
      
      //2012-12-8 add VF_O2CHANGED_OSDIAG
      if(!VF_O2CHANGED_OSDIAG)v_FMF=v_FMF+v_dmidle; //������ʧ������

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
            //ע�⣺v_FMF_remain�Ǽ���Ȧ�õģ�ǧ�򱻳���ˣ����¼���Ȧ�������ԣ�
        } else 
        {
            v_mi=v_FMF;
            //v_dmi=(sword)v_mi-v_ld; //2016-5-5 ɾ����FC_mi_trig�е���ʾ 

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
 
        //�������������������������Ǽ��������(��Ҫ����ʱ)
        v_FMF_lst=v_FMF;//
        
        //���v_FMF����Ҫ����T1�õ�����

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
    ���㸺��
    VF_CAL1NEED_REV_STOP:
          ��������FG_Rev_AfterIgn�����ģ�
          ��֮��صĹ��̵�ִ�е�λ�ã���ԭ����λ����ͬ��
          ��ˣ�VF_FORINJ_INJPHASE����Ҫ�����
          
    VF_CAL2NEED_PHASE_STOP:������VF_FORINJ_INJPHASE��صģ�ȡ��ԭ����VF_FORINJ_INJPHASE��
          �������������õ�FC_Ld�ǲ��Ǽ�����������Ȧ������
          ������Ҫ����������������������Ǽ���Ȧ��������
*/

//���������TPS��ʱ��
void Save_TPS_basic(byte tps)
{
    //v_t_TPSb=tps;
    v_t_TPSb=T_MAINTMR;
    v_ldb_b=v_ldb;
    v_TPSb=tps;    
}

//Ԥ�⸺��
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
        
        //ʱ��̫Զ�Ļ���Ԥ�����̫������޶���4��
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
        
        //2015-5-23��������仰�е�v_ld_min�����⣬�������Ŷ���ʱ��v_ldb�����ƵıȽϴ�
        //if(v_ldb>v_ld_max)v_ldb=v_ld_max;
        //if(v_ldb<v_ld_min)v_ldb=v_ld_min;
        
        if(v_ld>v_ld_max)v_ld=v_ld_max;
        if(v_ld<5)v_ld=5;//2015-5-23
        
        
        
    }
    
}


