/*HEAD
 ***********************************************************************
 * Filename	:mi_fmf.c
 * Function	:fuel injection calculation		
 * User		:xielian
 * Date		:2017.3.23
 * Version	:V8.1.0
 *
 *** History ***
 * V8.1.0   2017.3.23	xielian 
 *		Base module.
 * 
 * V8.2.0   2017.3.23	xielian
 *		多次喷射拆分时存在偏浓问题，对多次喷射的油量拆分进行限制.
 *
 * V8.3.0   2017.3.28	xielian
 *		针对低电压启动喷射相位正好是低电压的处理
 * V8.3.1   2017.4.1	xielian
 *		油量拆分加入温度限制条件
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "status_st.h"

#include "time_ccl.h"
//#include "wait_moving.h"

#include "ld.h" //2010-5-12
#include "inj_notystdc.h"

#include "data_inj_source.h"//2012-10-9

#include "ad.h"

#include "look_up.h" //2013-5-15

#include "status_eng.h"

#include "sci.h"

#include"StepMotor.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/

sword v_FMF_remain;
sword v_FMF;

sword v_FMF_lst; //上一次实际喷射量
sword v_FMF_all_lst; //上一次总喷射量

sword v_FMF_Injed;//一个循环内已经喷射的油量 
sword v_FMF_all;//一个循环内需要喷射的总油量


sword v_tw1;

word v_ldb;//从map上查找到的fmf

sword v_dld;//delta of load
word v_ld;

word v_mi;

word v_dms;//2013-6-22
sword v_dmi;//2012-10-22
word v_ld_NotAltitude;//用于计算油膜和
//2008-5-29 add


sword v_fmf_O;//2009-8-3 重新定义的O值

byte M0_FMF_IDLE    ;//2009-8-20 add 

byte v_fmf_grind;//2010-2-3

byte v_nb_rinj;     //喷射之前，对油量进行修正的次数；
byte v_nb_rinj_y;   //喷射之前，对油量进行修正的次数；

byte vbat_lst_remain;
byte vbat_remain;

#define M2_FMF_TPSRPM BYPTR_M2_LDB_TPSRPM


/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
//sword v_FMF_QF8US;
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

//功能：初始化FMF模块
void FI_FMF_AfterReset(void)
{
    //v_FMF_remain=0; //  clrf	v_FMF_RAMAIN_H		;2007-3-30,banked  //delete
    

    //M0_FMF_IDLE=(byte)(M2_FMF_TPSRPM[0][0]+M2_FMF_TPSRPM[0][1]);
    M0_FMF_IDLE=(byte)(Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,0,0)+Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,1,0));   //2014-5-17 纠正
   
	/*		//20210926 因为BYPTR_M0_LDBMUL作用调整到计算T1之前了，此处不用放大
	if((BYPTR_M0_LDBMUL<=250)&&(BYPTR_M0_LDBMUL>32))
	{
		 M0_FMF_IDLE=(byte)((unsigned long int)M0_FMF_IDLE*BYPTR_M0_LDBMUL/32);
	}*/
	
	
    v_nb_rinj=v_nb_rinj_y=0;
    
    //v_dms=(word)M2_FMF_TPSRPM[16][0]+M2_FMF_TPSRPM[16][0]-M0_FMF_IDLE; //2013-6-22
    v_dms=0; //2014-2-27
    
    v_ccl_for3rd=0;//2014-3-29
}

void InitVbat_remain(void)
{
    vbat_lst_remain=vbat_remain=v_Vbat_ad;
}

//功能：低速增油修正；
//+需要保证低速修正后if(v_FMF_H,v_FMF_L>=256)v_FMF_H,v_FMF_L=256;
sword FC_FMF_LowSpeed(sword fmf,byte FMF_lspd)
{
    byte b;
    
    if((FMF_lspd)>0 )
    {
        //if(MO_VEHICLE==CUB)b=FMF_lspd;///2;
        //else b=FMF_lspd;
        b=FMF_lspd; 
        fmf+=b;//2008-3-12补充的条件
        //if(fmf>256)fmf=256; //2012-9-26 DELETE
    }
    return fmf;    
}

//-----------rec_by_astk_tmp:--------------------------------------------------------------
//功能：温度对气量的修正
//返回值：修正后的油量
/*
关于v_CO_FMF_StartsTMP
    v_CO_FMF_StartsTMP：启动及温度修正；
    v_CO_FMF_StartsTMP=FMF*(+vc_FMF_teng+vc_FMF_tair)/128

    FMF:修正时刻的油量
         
*/
//2008-11-11 add fmf_restart;
sword FC_FMF_TMP(sword fmf,byte fmf_tair,sbyte fmf_teng) 
{
    sword sw1;
    
    //sw1=(sword)((((sword)fmf_teng+fmf_tair)*(long int)fmf)/128);
    sw1=(sword)((((sword)fmf_teng+fmf_tair)*(long int)fmf)>>7);

    return (sw1);
}


//功能：闭环控制修正；
//返回值：修正后的油量
sword FC_CLS(sword fmf,sbyte fmf_cls)
{
    fmf=fmf+fmf_cls;
    if(fmf<0)fmf=0;
    return fmf;   
}


//功能：怠速失火对油量的修正；
//返回值：修正后的油量
sword FC_MissF_FMF_Idle(sword fmf,sbyte tw1_idlemissf)
{
    sbyte sb;
    
    sb=tw1_idlemissf;
    fmf= fmf+sb;        
    if(fmf<0)fmf=0;
    return fmf;   
}

//功能：怠速失火对即将触发的TW1的修正；
//形参：IDLEMISSF_tmp－－vc_TW1_IDLEMISSF_tmp
sword FC_MissF_TW1_Idle(sword tw1, sbyte IDLEMISSF_tmp)
{
    sbyte sb;
    
    sb=IDLEMISSF_tmp;
    if(!VF_T35TESTNEED_NOTYSTDC)
    {
        tw1+=sb;
        //if(tw1<=0)tw1=0;
        if(tw1<=100)tw1=100; //2008-9-6 add
    }
    return tw1;        
}

//功能：怠速失火对DTFI的修正；
//形参：IDLEMISSF_tmp－－vc_TW1_IDLEMISSF_tmp
sword FC_MissFireForDtfi_Idle(sword dtfi,sbyte IDLEMISSF_tmp)
{   
    sbyte sb;
    
    sb=IDLEMISSF_tmp;
    if(!VF_T35TESTNEED_NOTYSTDC)
    {
        dtfi+=sb;
        //if(dtfi<0)dtfi=0; //2008-5-29 这个话有误，需要删除
    }
    return dtfi;        
}


/*
    关于油量的拆分
    
    喷射圈：
        1）如果油量一次可以喷出：则喷一次；
        2）如果油量需要两次喷出：则喷射圈喷两次；
        3）如果油量需要三次喷出：
              则喷射圈喷两次，剩下一次留在非喷射圈处理；
              
    非喷射圈：
        重新计算需要喷射的油量；
        即本循环的总油量，
        减去已经喷出的部分
        如果有剩余的本圈再喷；
        
        1）如果高速或者如果剩余本圈能喷出：则本圈喷出
        2）如果低速，如果需要喷两次才能喷出，则可以喷两次            
        
    只要一个循环喷射多次，VF_NEEDDIVFMF_NOTYSTDC都为1

*/
sword F_remain_FMF(sword fmf)
{
    word w;
    
    word q_max_tmp;
    word q_min_tmp;
	byte blnLowV;
	
	vbat_lst_remain=vbat_remain;
    vbat_remain=v_Vbat_ad;
    
    blnLowV=false;
    
    
    if(VF_NEWCYCLE_ATMAIN_CAS)VF_LIDLE_ADVANCE_INJPHASE=false;
    
    if((vbat_remain<BYPTR_M0_VBATLIMT)||(vbat_lst_remain<BYPTR_M0_VBATLIMT)){
        if(!VF_BFIRE_ENG) blnLowV=true;   
    }
    
    /*  2017-3-11 调整思路,IN F_remain_FMF
        
        
        1)由于部分车型油量拆分后，喷射量明显大于需求量
        因此，增加拆分的最低流量的限值的标定；
        
        该值的量纲与LDB一致，
        
        存储位置在LDBmap的TPD=0，N=9000的位置，即，ldbmap的最右上角位置
        
    
        q_max_tmp=(word)(Get_Map_byPtr(BYPTR_M2_LDB_TPSRPM,16,0,15))*2; //2017-3-11
    
        不满足三次拆分条件，即2000rpm以上时
        if(v_fmf_max<q_max_tmp)v_fmf_max=q_max_tmp; //2017-3-11
        
        
        
        2)非喷射圈加喷值的控制
        存储位置在LDBmap的TPD=0，N=8500的位置，即，ldbmap的最右上角位置靠左的一个位置
            q_min_tmp=(word)(Get_Map_byPtr(BYPTR_M2_LDB_TPSRPM,16,0,14))*2; //2017-3-12
            非喷射圈油量需要大于这个数之后，才加喷
            如果发现连喷引起的偏浓，就将这个数加大，但是最好别超过怠速值
            
    
    */
    
    q_max_tmp=(word)(Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,0,15))*2; //2017-3-11
   
    q_min_tmp=(word)(Get_Map_byPtr((byte *)BYPTR_M2_LDB_TPSRPM,16,0,14))*2; //2017-3-12
    
    v_FMF_remain=0;   //2012-11-21 delete   //2012-11-29 ADD BACK
   
    //2016-3-12
    if(!VF_NEEDDIVFMF_NOTYSTDC){  //表示上一次拆分
        //则 V_FMF_MAX就需要减小   
        v_fmf_max+=40;
    }
   
    //if(!VF_3INJ_ENABLE_INJPHASE)
    
    if(v_rev_625rpm>REV62D5_CALLD_LOW)//2017-3-11
    {
        if(v_fmf_max<q_max_tmp)v_fmf_max=q_max_tmp; //2017-3-11
       // if(v_fmf_max<500)v_fmf_max=500; //2017-3-11
    }
    
    
    deT3=v_fmf_max;
    //VF_4INJ_ENABLE_INJPHASE
    
    if(VF_FORINJ_INJPHASE){
        //如果是喷射圈
        //if((fmf>=v_fmf_max)&&VF_2INJ_ENABLE_INJPHASE)//2009-12-21 只要喷油圈油量超过喷油能力就拆分
		#ifdef	TE_10K	//20210728 chenwei add 10K缸温
		if(
            ((fmf>=v_fmf_max)&&VF_2INJ_ENABLE_INJPHASE&&(v_te<50))//2017-4-1 增加温度条件
            ||
            (blnLowV)
            )//2017-3-28 针对森科低电压启动时，喷射相位正好是最低电压的情况
             //此时需要拆分，拆分后正好有一个循环是排气圈，电压会高一些
         #endif

		 #ifdef	TE_2K	//20210728 chenwei add 2K水温
		if(
            ((fmf>=v_fmf_max)&&VF_2INJ_ENABLE_INJPHASE&&(v_te<45))//2017-4-1 增加温度条件
            ||
            (blnLowV)
            )//2017-3-28 针对森科低电压启动时，喷射相位正好是最低电压的情况
             //此时需要拆分，拆分后正好有一个循环是排气圈，电压会高一些
         #endif
		{
            /*
            fmf-=fmf/16;//拆分时，打折 2014-3-9 //2014-3-11
            *///2015-12-15 delete
            w=fmf/2;
            
            if(w>v_fmf_max){
                //油量分两次都喷不完
                if(VF_3INJ_ENABLE_INJPHASE){
                    //低速：允许一循环喷3次的

                    w=fmf/3;
                    
                    if(w>v_fmf_max&&VF_4INJ_ENABLE_INJPHASE){
                        //三次喷射还喷不完，且一循环可以喷四次
                        fmf=v_fmf_max;//
                        v_nb_notsyninj_add=1;    
                        v_FMF_Injed=fmf+fmf;
                    } else{
                        //三次喷射就可以满足要求
                        fmf=v_fmf_max;//
                        v_nb_notsyninj_add=1;    
                        v_FMF_Injed=w+w;
                    }
                } else{
                    //高速：只允许一圈喷一次
                    //v_FMF_remain=w;
                    fmf=w;//先喷大半，剩余一半在非喷射圈再重新计算
                    v_FMF_Injed=w;
                    v_nb_notsyninj_add=0;    
                }

            } 
            else
            {
                //油量两次可以喷完的情况
                //if(VF_3INJ_ENABLE_INJPHASE){
				if(VF_3INJ_ENABLE_INJPHASE&&!blnLowV)//2017-3-28
				{
                    //低速：允许一循环喷3次的
                    //在第一圈内等分两次喷完
                    fmf=w;//
                    v_nb_notsyninj_add=1;    
                    v_FMF_Injed=w+w;
                                               
                } else{
                    //高速：只允许一圈喷一次
                    //v_FMF_remain=w;
                    fmf=v_fmf_max;//先喷一大半，剩余一小半在非喷射圈再重新计算
                    v_FMF_Injed=fmf;
                    v_nb_notsyninj_add=0;    
                }
            }

            VF_NEEDDIVFMF_NOTYSTDC=true; //标识油量需要拆分
        
        } else{
            VF_NEEDDIVFMF_NOTYSTDC=false;//2008-10-10 move to here
            v_FMF_Injed=fmf;
            v_nb_notsyninj_add=0;
        }
        
        if(VF_NEEDDIVFMF_NOTYSTDC)v_FMF_all+=v_FMF_all/16;
        v_FMF_all=v_FMF_Injed;
        
        
        VF_2INJATINJPHASE_INJPHASE=false;
        if(v_nb_notsyninj_add)VF_2INJATINJPHASE_INJPHASE=true;
    } else{
        //非喷射圈
        //扣除上一圈已经喷射出去的
        
        v_FMF_all=fmf;
        
        fmf-=v_FMF_Injed;
        if(fmf<1)fmf=1;
        /*
            v_FMF_Injed=v_FMF;
            v_FMF_all+=v_FMF_Injed;
        */    
        //if((((fmf>=M0_FMF_IDLE/8)&&(v_te<50))||(fmf>=M0_FMF_IDLE))&&VF_2INJ_ENABLE_INJPHASE)	
        #ifdef	TE_10K	//20210728 chenwei add 10K缸温
        if((((fmf>=M0_FMF_IDLE/8)&&(v_te<50))||(fmf>=M0_FMF_IDLE/4))&&VF_2INJ_ENABLE_INJPHASE)//2019-2-27 ADD 		
		//2017-4-2 增加温度条件
		#endif

		#ifdef	TE_2K	//20210728 chenwei add	2K水温
        if((((fmf>=M0_FMF_IDLE/8)&&(v_te<45))||(fmf>=M0_FMF_IDLE/4))&&VF_2INJ_ENABLE_INJPHASE)//2019-2-27 ADD 		
		//2017-4-2 增加温度条件
		#endif
		{
            if(
              ((!VF_2INJATINJPHASE_INJPHASE&&VF_3INJ_ENABLE_INJPHASE)
              ||
              VF_4INJ_ENABLE_INJPHASE
              )
              &&fmf>v_fmf_max
              )
              {
                  fmf/=2;
                  v_nb_notsyninj_add=1;
                  
                  //2019-2-1  peter
              }
            
            VF_NEEDDIVFMF_NOTYSTDC=true;
                  if(VF_JLEAVEIDLE_ENG)VF_LIDLE_ADVANCE_INJPHASE=true;
        
        } 
		//2017-3-28 增加
        else if(blnLowV)
        {
            fmf=v_FMF_all;
            v_nb_notsyninj_add=1;
            VF_NEEDDIVFMF_NOTYSTDC=true;    
        }
    		else
    		{
            VF_NEEDDIVFMF_NOTYSTDC=false;
            fmf=1;
            v_nb_notsyninj_add=0;
        }
        
    }
          

    return fmf;
}

/*
    #if MV_EXHAUST>PL150
        if((V_Tw<-7&&!VF_BFIRE_ENG)){
            if(VF_E0_TPS)w=fmf/4;//2013-4-13 add 
            v_FMF_remain=w;
            v_nb_notsyninj_add=2;
        }
        else if((w>v_fmf_max)&&(VF_E0_TPS||v_ccl_for3rd>0||VF_JLEAVEIDLETRANS_ENG||VF_JCI_CFCI||VF_JCI_LST_CFCI))//2014-3-15 ADD FOR 3RDINJ
        {                                    //2013-4-19
            w=fmf/3;//2013-4-13 add 
            v_FMF_remain=w;
            v_nb_notsyninj_add=1;
        }
        else {
            v_FMF_remain=w;
        }
    #else
         if(((w>v_fmf_max)&&(VF_E0_TPS||v_ccl_for3rd>0||VF_JLEAVEIDLETRANS_ENG||VF_JCI_CFCI||VF_JCI_LST_CFCI))||(V_Tw<-7&&!VF_BFIRE_ENG))//2013-4-15(V_Tw<-12&&VF_STARTS_ENG)
        {                                    //2013-4-19
            w=fmf/3;//2013-4-13 add 
            v_FMF_remain=w;
            v_nb_notsyninj_add=1;
        } else {
            v_FMF_remain=w;
        }
   
    #endif

*/

//功能：怠速搜索临时量对FMF的修正
/*
//返回值：修正后的油量

        //if(REMARK!=MC_LOADB &&VFF_ECUINIT_DTC)v_FMF+=v_SL_cls;  
       // if(REMARK!=MC_LOADB )v_FMF+=v_SL_cls;    //2013-1-15
      2013-3-9 学习值计入O值中
*/
sword FC_IdleSC_FMF(sword fmf,sbyte tw1_IDLESC_inuse,sword tw1_idle_sc,sword fmf_sci_idle,sword fmf_sci_nidle,sbyte tw3offset)
{   //2009-8-3 add tps_odr
    sword sw;
	sbyte sb;
    
    //2009-8-3 change to the following
    //v_fmf_O=tw3offset+tw1_idle_sc+tw1_IDLESC_inuse;    //2009-12-23
//    #if ECU_TYPE==ECU_MOT
    
	#ifndef IS_GEN
		#ifndef SYS_TRADITIONAL
			v_fmf_O=tw3offset+tw1_idle_sc+tw1_IDLESC_inuse+fmf_sci_idle;
		#else
			v_fmf_O=fmf_sci_idle+tw3offset;
		#endif
		
		#ifdef SYS_STEPMOTOR
		if(!(REMARK==10&&KESEI==5))
		{
			sb=v_t_IACV_on;
			if(v_TPS_odr>4)
			{
				sb=v_t_IACV_on/4;
			}
			v_fmf_O+=sb;
		}
		#endif
			
			v_fmf_grind=v_fmf_Grind_all;    

			if(VF_E0_TPS)
			{
            //v_fmf_O+=fmf_sci_idle;   //2009-12-23 
            //2013-3-9
				if(REMARK!=MC_LOADB 
             //  && REMARK!=MC_LAMBDA   //2013-4-27 ADD   2014-2-26 delete v_fmf_O+=v_fmf_sl_idle  for u
					&&!VF_1ST_INI_SYSINI)v_fmf_O+=v_fmf_sl_idle;//v_SL_cls;    //2013-1-15
			} 
			else
			{  
            //5:--80,
            //4:--64,
            //3:--48,
            //1:--16
            //if(v_fmf_O>0) //2010-2-2
				if(REMARK!=MC_LOADB &&!VF_1ST_INI_SYSINI){ //2013-3-19 add condiction
					v_fmf_O+=v_fmf_sl_idle;    //2013-1-15
				}
				sw=80-(sword)v_TPd;//2009-9-7 change 5 to 7
				if(sw<0)sw=0;
				if(sw>64)sw=64;
            
				v_fmf_grind=v_fmf_grind*sw/64;//2010-2-3
				#ifndef SYS_TRADITIONAL
				v_fmf_O=(v_fmf_O+vc_TW1_IDLEMISSF_av)*sw/64;
				#endif
				v_fmf_O+=fmf_sci_nidle;          
            
            if((REMARK!=MC_LOADB) 
               &&(REMARK!=MC_LAMBDA)     //2013-4-27 ADD 
			   &&(v_TPS_odr>7)&&(v_rev_625rpm>REV62D5_4500)
                
            ){
                v_fmf_O+=v_SL_cls;    //2013-3-10
            }
        }
    #else
        //v_fmf_O=fmf_sci_nidle+tw3offset+fmf_sci_idle;
        v_fmf_O=fmf_sci_nidle+tw3offset+fmf_sci_idle+v_SL_cls; //2016-1-2 更正
	#endif

    
    fmf+=v_fmf_O;
    fmf+=v_fmf_grind;//2010-2-3
    
    if(fmf<10)fmf=10;
    return fmf;
}


//功能：电池电压变动对TW1的修正
//返回值：修正后的TW1
sword FC_UbatChange_TW1(sword tw1,sbyte tw1_vbatchg,sword tw1_vbatdownidle)
{
    sword sw;
    //+vc_TW1_vbatchg +vc_TW1_vbatdownidle;//对即将触发TW1的修正；
	//sw=tw1+tw1_vbatchg+tw1_vbatdownidle;
	sw=tw1;
	if(VF_E0_TPS||VF_STARTS_ENG)
	{
		sw=sw+tw1_vbatdownidle;
	}
	else if(tw1_vbatchg>5)
	{
		sw=sw+tw1_vbatchg;
	}
	
    //if(tw1<=0)tw1=0;
    if(sw<=100)sw=100; //2008-9-6 add
    
    return sw;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
