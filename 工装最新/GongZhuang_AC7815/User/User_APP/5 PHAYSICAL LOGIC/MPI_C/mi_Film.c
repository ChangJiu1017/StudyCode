/*HEAD
 ***********************************************************************
 * Filename	:mi_Film.c
 * Function	:Fuel film calculation.		
 * User		:xielian
 * Date		:2017.3.28
 * Version	:V8.2.1
 *
 *** History ***
 * V8.1.0	2017.2.17	xielian 
 *		Base function for PI-Motorcycle.
 *
 * V8.2.0	2017.2.17	xielian 
 *		L_ACC和L_DEC更改为按照正常赋值定义.
 * V8.2.1	2017.3.28	xielian 
 *		重新定义VF_TRANSGT15_TRANS. 
 * V9.1.0	2017.12.25	xielian
 *		Base Moudule.
 * V9.2.0	2017.12.25	xielian/yangsili 
 *		屏蔽一条语句. 
 ***********************************************************************
HEAD END*/

#include "status_st.h"
#include "public.h"

#include "ld.h"
#include "look_up.h"
#include "sci.h"
#include "ad.h"
#include "add.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
word V_mf;
word V_mf_lst;

sword v_dmfL;    //当圈的油膜增量
sword v_dmfS;    //当圈的油膜增量

sword v_dmf_binj;//喷射之前的油膜变化量
sword v_dmf_SumL;//总的油膜增量；
sword v_dmf_SumS;//总的油膜增量；

long int v_mf_remain;
long int v_mf_evap;

word v_mf_area_M256;

long int v_mf_relative;
word v_mf_residue;    //2012-10-18 add 
          //表示非阀背部分的始终作为油膜的量；　 v_mf_residue=v_mf_remain/32* vc_A_mf
sword v_mf_rf;
byte v_co_dmf_M64;

sword vc_FMF_dtfi_trans; //2010-5-14 纯粹的过渡部分的dtfi 

word v_dmi_UA;//2019-2-27

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
byte v_P_DEC_M64;
byte v_P_ACC_M64; 
byte v_L_M1024;
byte v_S_M128;

byte v_L_DEC_M1024;//2014-3-10 ADD L_DEC

byte vc_B_mf;
byte vc_C_mf;
byte vc_D_mf;
byte vc_SD0_mf;
byte vc_wf;  //临时增加的温度对油膜的修正；

/*data for Y3*/					
#define M0_Y3_COL_X 0					
#define M0_Y3_COL_Y 1

/*					
#define M0_Y3_UB 13					
const byte M1_Y_M3[14][2]=					
{	//FMF/2,Y1/3_M3				
  	18	,	10	,	
  	25	,	11	,	
  	32	,	12	,	
  	40	,	13	,	
  	50	,	14	,	
  	62	,	15	,	
  	75	,	16	,	
  	91	,	17	,	
  	108	,	18	,	
  	127	,	19	,	
  	147	,	20	,	
  	172	,	21	,	
  	197	,	22	,	
  	225	,	23	,	
};//为了节省空间，将X坐标/2					
*/

#define M0_Y3_UB 25					
const byte M1_Y[26][2]={			//Y(FMF^(2/3))	 ,FMF/4
	10	,	11	,
	15	,	15	,
	18	,	17	,
	21	,	19	,
	28	,	23	,
	31	,	25	,
	36	,	27	,
	37	,	28	,
	40	,	29	,
	41	,	30	,
	46	,	32	,
	47	,	33	,
	57	,	37	,
	58	,	38	,
	63	,	40	,
	73	,	44	,
	83	,	48	,
	93	,	52	,
	97	,	53	,
	102	,	55	,
	105	,	56	,
	108	,	57	,
	113	,	59	,
	116	,	60	,
	120	,	61	,
	122	,	62	,
	};			



/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
word mf_exp_X_M64;
byte Y_M3;
sbyte v_H_M5;
word mf_precyc;

byte v_ss;


#define TIMES_FMFSUM 32
word v_fmf_16Sum_forFilm;


byte nb_film_attu;//2016-5-6 add 

//#define C_MAX 32

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void Init_FilmAfterReset(void)
{
    if(VF_POR_HARD) {
        V_mf=0;
        v_mf_rf=0;   //2012-12-30 add ,并且改变v_mf_rf含义为实际的油膜
    }

    if(VF_VPOR_HARD)
    {
        vc_B_mf=BYPTR_M0_B_D8;
        vc_C_mf=BYPTR_M0_C_M8;
        vc_D_mf=BYPTR_M0_D_M256;
        
        vc_SD0_mf=BYPTR_M0_SD0_D12;

        v_dmf_SumL=0;
        v_dmf_SumS=0;
        V_mf=0;

        v_P_DEC_M64=BYPTR_M0_P_DEC_M64;
        v_P_ACC_M64=BYPTR_M0_P_ACC_M64; 
        //v_L_M1024=BYPTR_M0_L_DEC_M1024;
		v_L_M1024=BYPTR_M0_L_ACC_M1024;	//2016-8-31 注意串口版DATAVIEW L_ACC和L_DEC是反着的
        v_S_M128=BYPTR_M0_S_DEC_M128;
        
        //v_L_DEC_M1024=BYPTR_M0_L_ACC_M1024;
		v_L_DEC_M1024=BYPTR_M0_L_DEC_M1024;	////2016-8-31 注意串口版DATAVIEW L_ACC和L_DEC是反着的

        v_ss=BYPTR_M0_S_ACC_M128;//2015-10-27

    }
    
    //V_mf=v_mf_rf/4;
    V_mf_lst=V_mf=0;
    v_mf_relative=0;//v_mf_relative_lst=0;
    v_mf_evap=0;
    
    v_dmf_SumL=v_dmf_SumS=0;
    v_dmfL=v_dmfS=0;
    
    nb_film_attu=0;
    
    v_dmi_UA=0;
}


/*--------启动前，通过预喷建立油膜----------------*/
//返回值：预喷油膜量
//
word Init_FilmForPreInj(sword tw,sbyte ta)
{
    FC_Film((word)M0_FMF_IDLE*2,tw,ta,2,2);
    
    return V_mf;   
}

void Init_FilmBeforeStarts(void)
{
    
    v_fmf_16Sum_forFilm=TIMES_FMFSUM* (word)M0_FMF_IDLE; //2012-9-26

}
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 


/*实际油膜在停车时的衰减*/
void FATTU_Film(void){
    if(VF_1S_TMCCL){
        if(v_mf_rf>0){
            V_mf=v_mf_rf=((long int)(v_mf_rf/4)*(1024-v_L_M1024))>>8;    //2012-11-13
        }
    }
}

/*
    功能：计算油膜
*/
void FC_Film(word fmf,sword tw,sbyte ta,byte tps_odr,byte rev_odr)
{
    
    word w;
    long int lmf;
    long int ldmf;

    sword sw;
    sword sdmf;
    sword sdmfs;
    word wFsum;
    word mf_exp_M128;


    v_ss=Adj_byte(BYPTR_M0_S_ACC_M128,ADJ_ss);
    
    vc_B_mf=Adj_byte(BYPTR_M0_B_D8,ADJ_B);
    vc_C_mf=Adj_byte(BYPTR_M0_C_M8,ADJ_C);
    vc_D_mf=Adj_byte(BYPTR_M0_D_M256,ADJ_D);


    v_P_DEC_M64=Adj_byte(BYPTR_M0_P_DEC_M64,ADJ_P_DEC);
    v_P_ACC_M64=Adj_byte(BYPTR_M0_P_ACC_M64,ADJ_P_ACC); 
    //v_L_M1024=Adj_byte(BYPTR_M0_L_DEC_M1024,ADJ_L_ACC);
	v_L_M1024=Adj_byte(BYPTR_M0_L_ACC_M1024,ADJ_L_ACC);	//2016-8-31 注意串口版DATAVIEW L_ACC和L_DEC是反着的
    v_S_M128=Adj_byte(BYPTR_M0_S_DEC_M128,ADJ_S);
    //v_L_DEC_M1024=Adj_byte(BYPTR_M0_L_ACC_M1024,ADJ_L_DEC);
	v_L_DEC_M1024=Adj_byte(BYPTR_M0_L_DEC_M1024,ADJ_L_DEC);	//2016-8-31 注意串口版DATAVIEW L_ACC和L_DEC是反着的
    
    //2013-1-27
    if(!VF_E0_TPS){
        VF_REMAINIDLE_STARTS=false;
    }    

    
    V_mf_lst=V_mf;//2014-4-3 可能需要增加条件
    
    /*断油与否的处理*/
    if(VF_IGNONLY_CFCI&&VF_MMOVE_ENG)
    {
       //2013-1-8 ADD  把断油前没有分配的油膜也记入
      
        if(VF_NEWCYCLE_ATMAIN_CAS){
             //2014-4-16  add condiction
             sw=V_mf;
             sw=sw-v_dmf_SumL-v_dmf_SumS;//2014-1-8 断油时，油膜的重新分配；以前容易收断油前油门状态的影响；
             if(sw<=0)sw=1;
             V_mf=sw;
             
             v_mf_rf=V_mf=V_mf-(word)(((long int)V_mf*(v_L_M1024))>>11); //2013-2-28           /4096仍然太稀，  /2048
            
            
            //2015-4-9 是否考虑将v_dmf_SumL，放入VF_NEWCYCLE_ATMAIN_CAS条件的里面？
            v_dmf_SumL=0;
            v_dmf_SumS=0;
        }
        
        v_fmf_16Sum_forFilm=fmf*TIMES_FMFSUM; 
    } else
    {
    
    
        //2012-9-26计算平均油量
        if((
              (!VF_BFIRE_ENG&&v_ccl_eng_byte<=2) //2014-2-27 add  v_ccl_eng_byte<=2
              ||VF_ACCSPD_TRANS||VF_ACCLST2_TRANS
                     //2014-2-28 DELETE ||(!VF_TRANSLT10_TRANS&&!VF_E0_TPS&&VF_BFIRE_ENG)//2013-1-6    //2014-2-27 FOR FILM  //2014-2-27 add VF_BFIRE_ENG
                      ||VF_DECSPD_TRANS||VF_DECLST2_TRANS
                      ||VF_ACCMIDLST_TRANS//2012-12-14
                      ||(!VF_MAXLST_TPS&&VF_MAX_TPS)//2012-12-14 注意，现在油门在15到16级之间的情况
                      ||VF_JLEAVEIDLE_ENG//2012-9-28 ADDCONDICTION
                      ||VF_JLEAVEIDLETRANS_LST_ENG      //2012-12-30 11 AM ADD
                      ||VF_JLEAVEIDLETRANS_ENG          //2012-12-30 11 AM ADD
                      ||VF_JBACKIDLE_ENG//2012-11-23 ADD
                      ||VF_JBACKIDLE_LST_ENG//2012-12-13 ADD 
                      ||VF_JCI_LST_CFCI||VF_JCI_CFCI//2013-1-15
                      ||!VF_MMOVE_ENG//2015-5-20 计算预喷的油膜时的处理
                      //||(VF_E0_TPS&&v_rev_625rpm>REV62D5_1750  ) // VF_GT30_IDLETG_REV
                      //||(VF_E0_TPS&&VF_GT30_IDLETG_REV) // 2013-8-2 change from v_rev_625rpm>REV62D5_1750 to VF_GT30_IDLETG_REV
                      //||(VF_E0_TPS&&v_rev_625rpm>REV62D5_2500) // 2014-4-27 DELETE
          )            
          //&&(VF_CAL2NEED_PHASE_STOP) //2012-11-27 ADD 
          ){
            //这些条件下，使用瞬态油量，
            //v_fmf_16Sum_forFilm=fmf*TIMES_FMFSUM;//2012-9-28修改如下：
            wFsum=fmf*TIMES_FMFSUM;
            if(VF_ACCSPD_TRANS||VF_ACCLST2_TRANS||VF_JLEAVEIDLE_ENG){
                if(v_fmf_16Sum_forFilm<wFsum)v_fmf_16Sum_forFilm=wFsum;    
            } else v_fmf_16Sum_forFilm=wFsum;

        } else{
           // if(VF_CAL2NEED_PHASE_STOP)
           if(VF_NEWCYCLE_ATMAIN_CAS)//2014-4-6 add condiction
                v_fmf_16Sum_forFilm=v_fmf_16Sum_forFilm-v_fmf_16Sum_forFilm/TIMES_FMFSUM+fmf;
        }
        
        //if(VF_CAL2NEED_PHASE_STOP)//2012-9-26 ADD CONDICTION
        {
            /*MF_REMAIN M16*/
                fmf=v_fmf_16Sum_forFilm/TIMES_FMFSUM;  //2012-9-26 //2012-11-27发现这个位置太靠后了，需要提前
                v_mf_remain= vc_C_mf*(long int)fmf; //MAY BE LONG//2012-10-11

                #ifndef SYS_TRADITIONAL	//2017-12-25 共轨太浓
                	v_mf_remain+=v_mf_remain;//C的作用加倍//2013-4-13
				#else
					v_mf_remain+=v_mf_remain;
				#endif
			/*2017-12-22 太浓	
				#ifdef SYS_TRADITIONAL
					v_mf_remain+=v_mf_remain;
				#endif
            */    
                if(BYPTR_MV_VEHICLE==WILD )   //2015-5-11 大排量C作用再加倍
				{
					v_mf_remain+=v_mf_remain;//C的作用加倍//2015-5-11
				}
                    

                
                //v_mf_residue=(word)((v_mf_remain*vc_A_mf)>>8);//  /16
                v_mf_residue=(word)((v_mf_remain)>>8);// 2013-5-13        相当于A FROM 2 TO 1
            /*v_mf_evap
                code for:mf_evap_M16=M0_B_D4*Y*M0_m0*M0_n0_M16*M0_Cp*(V_teng-BYPTR_M0_IBP)/(M0_Sd0_D4*M0_Ql*V_rev);
                    MUL_M0_N0_CP_DIV_SD0_QL=M0_m0*M0_n0_M16*M0_Cp/M0_Sd0_D4*M0_Ql
            */
                w=fmf/4;
                Y_M3=F_2DLookUp(w,M1_Y,M0_Y3_UB,0,false);
                
                //if(v_te<M0_IBP)w=0;
                //else w=v_te-M0_IBP;
                if(v_te<BYPTR_M0_IBP)w=0;
                else w=v_te-BYPTR_M0_IBP;
                
                sw=v_rev_15d6rpm;
                if(sw<96)sw=96;//2013-2-28
                
                v_mf_evap=((word)vc_B_mf*(word)Y_M3*MUL_M0_N0_CP_DIV_SD0_QL)*(long int)(w)/sw;
                                //没有两个word时，计算结果会出负的
                
				
                //2014-3-11 变更为
                if(V_Tw<0)w=0;
                else w=V_Tw;
                if(w>BYPTR_M0_FBP)w=BYPTR_M0_FBP;
                
                sw=BYPTR_M0_FBP;
                
                v_mf_area_M256=256-vc_D_mf*(w)/sw;//32*8=256
                
                lmf=v_mf_remain-v_mf_evap;
                if(lmf<0)lmf=0;
                
                //v_mf_relative=(lmf*v_mf_area_M256)>>12;     4096*256 =12+8=20
                // 用U作临时处理
                v_mf_relative=((lmf*v_mf_area_M256)>>12)&0x000fffff;//
                
                
                v_mf_remain=(word)(v_mf_remain>>4);
                v_mf_evap=(word)(v_mf_evap>>4);


            
            if(v_mf_relative<v_mf_residue)v_mf_relative=v_mf_residue;
            if(v_mf_relative>0)
            {
                V_mf=v_mf_relative;    
            }else
            {
                V_mf=0;
            }
        }
    }
    
    /*对油膜为0的情况的处理*/
    /*这里需要对回怠速或者离开怠速作处理*/
    /*这里需要作急加速和急减速的处理*/
        if(VF_NEWCYCLE_ATMAIN_CAS){ //2014-4-16 add condiction
            //2013-2-28
            if(VF_JCI_LST_CFCI||VF_JCI_CFCI)//2013-1-15
            {
                
                if (BYPTR_MV_VEHICLE==WILD)    //2015-5-11
				{
					//w=V_mf-V_mf/8;
                    //w=V_mf-V_mf/32;//2015-6-18
                    w=V_mf-V_mf/64;//2015-6-19
                    
                    if(V_mf_lst>w)V_mf_lst=w;
				}                   
                else
				{
					//2014-1-8 change to the following
                    //w=V_mf/2+V_mf/4;
                    //2015-6-23 CHANGE TO
                    w=V_mf-V_mf/16;
                    if(V_mf_lst>w)V_mf_lst=w;
				}                
            }

           
            
            //if(VF_JLEAVEIDLE_ENG)
            if(VF_JLEAVEIDLE_ENG&&(!VF_GT10_IDLETG_REV||v_rev_625rpm>REV62D5_3500)) //2014-3-13 ADD CONDICITON针对反复轰油门的情况，转速会下掉
            {
                if(v_dmf_SumL<0)v_dmf_SumL=0;
                if(v_dmf_SumS<0)v_dmf_SumS=0;
            }
            
           //2015-5-11 DELETE 
            
            
            //2015-6-18
            if(VF_JBACKIDLE_ENG || VF_JBACKIDLE_LST_ENG||VF_DECSPD_TRANS ||VF_DECLST2_TRANS) 
            {
                /*
                if(v_dmf_SumL>0)v_dmf_SumL=0;
                if(v_dmf_SumS>0)v_dmf_SumS=0;    
                */
                //2015-6-18
                //因为上一次的油膜其实没有补齐，所以需要重新分配一下
                V_mf_lst=V_mf_lst-v_dmf_SumL-v_dmf_SumS;
                v_dmf_SumL=0;
                v_dmf_SumS=0;

            }
            
        }


    /*油膜变化*/
        //if(VF_CAL2NEED_PHASE_STOP)//2012-9-26 ADD CONDICTION
        if(!VF_IGNONLY_CFCI)//2015-5-8 add condiction , 否则也会产生断油时也会产生油膜项，因为其他地方有对V_mf赋值的；
        {
  
            ldmf=(long int)V_mf-V_mf_lst;

            if(ldmf>10000)ldmf=10000;
            else if(ldmf<-10000)ldmf=-10000;
            
            
            //2015-10-11 , copy from 2014-12-15 减油膜的情况,mi_film.c
            if(v_rev_odr>=5){
                if(ldmf<0&&!VF_E0_TPS){
                    w=((word)v_rev_odr+1)*(v_rev_odr+1);
                    ldmf/=w;
                }
            }
            
            
            sdmf=ldmf;
            //if(!VF_BFIRE_ENG)sdmf/=2;//2013-1-25


            //2015-5-11 ADD
            //油膜的符号方向变了，则总量重新分配
             /*
                if(VF_JLEAVEIDLETRANS_ENG || VF_JLEAVEIDLETRANS_LST_ENG||VF_ACCSPD_TRANS||VF_ACCLST2_TRANS ){
                    sw=v_dmf_SumS+v_dmf_SumL;
                    
                    if(v_dmf_SumS<0||v_dmf_SumL<0){
                        sdmf+=sw;
                        v_dmf_SumS=0;
                        v_dmf_SumL=0;    
                    }
                    
                }
                    
                if(VF_JBACKIDLE_ENG || VF_JBACKIDLE_LST_ENG||VF_DECSPD_TRANS ||VF_DECLST2_TRANS){
                    sw=v_dmf_SumS+v_dmf_SumL;
                    
                    if(v_dmf_SumS>0||v_dmf_SumL>0){
                        sdmf+=sw;
                        v_dmf_SumS=0;
                        v_dmf_SumL=0;    
                    }
                    
                }
                    
            */
            

            //if(VF_E0_TPS&&VF_STARTS_ENG)
            if(VF_E0_TPS&&VF_STARTS_ENG||!VF_BE_PREINJ_ENG)//2015-5-20 ADD VF_BE_PREINJ_ENG
            {  //2014-2-26 晚
                
                
                v_co_dmf_M64=v_ss;//BYPTR_M0_S_ACC_M128;//2015-10-27
                
                
            } else{
              //BYPTR_M0_S_ACC_M128 VF_REMAINIDLE_STARTS
                if(sdmf<0)
                {
                    v_co_dmf_M64=v_P_DEC_M64;//减速工况长短期项分配系数为DDMF（film distribution factor for deceleration）

                } else{
                    v_co_dmf_M64=v_P_ACC_M64;//加速工况长短期项分配系数为ADMF（film distribution factor for acceleration）

                }
            }
            
            {
                sdmfs=(sword)(((long int)sdmf*v_co_dmf_M64)/64);
                //if(VF_REMAINIDLE_STARTS)sdmfs/=4;//2014-2-26
                if(VF_REMAINIDLE_STARTS||!VF_BE_PREINJ_ENG)sdmfs/=4;//2014-2-26
                sdmf=sdmf-sdmfs;
            }              
            //2014-2-14 启动时油膜短项不为0
            //if(!VF_BFIRE_ENG)sdmfs=0;//2014-2-14 MOVE UP
            
            v_dmf_SumS=v_dmf_SumS+sdmfs;
            //if(!VF_BFIRE_ENG)v_dmf_SumS=0; //2013-1-25 DELETE
           
            v_dmf_SumL=v_dmf_SumL+sdmf;
        }
}

/*
    功能：油膜分配
*/
word F_AttuFilm(sword fmf)
{
    sword wmaxfmf;
    byte bidled8;
    byte b;
    sword sw;
    

    //2015-4-7 发现STARTS阶段，怠速时v_dmfS一直为一个数，不为0，难道START不进入此处？

    v_dmfS=(((long int)v_dmf_SumS*v_S_M128)>>7);
    
    
    /*这里需要作急加速和急减速的处理*/
    /*这里需要对回怠速或者离开怠速作处理*/
    /*这里需要对再给油作处理*/
    

    //2014-2-28 ADD 
    if(v_dmf_SumL<0){
        //b=v_L_DEC_M1024;//v_L_DEC_M1024=BYPTR_M0_L_ACC_M1024;
    
        /*2015-6-8 决定L DEC分为冷机与热机，
            冷机时，取值与L ACC相同，
            通常情况下，L DEC 在热机时需要小一些，
            在金浪125T上测试时，热机L DEC大了，斗车就会因偏稀而熄火
        
        
            思路二
                可以考虑用D对 L DEC 进行修正
                v_mf_area_M256
        */
        //2015-6-8 change to the following:
        b=(word)v_mf_area_M256*v_L_DEC_M1024/256;
    
    }
    else {
        b=v_L_M1024;
    }
    
    
    v_dmfL=((long int)(v_dmf_SumL/4)*b)>>8;    //2012-11-13
    
    if(VF_REMAINIDLE_STARTS){    //BYPTR_M0_S_ACC_M128 VF_REMAINIDLE_STARTS 
        v_dmfL=v_dmfL/4;//2014-2-14
    }
    
    
    
    
    //2014-4-16 add condiction
    if((v_dmfS<2)&&(v_dmfS>-2)){
        if(v_dmf_SumS>=2)v_dmfS+=2;
        else if(v_dmf_SumS<=-2)v_dmfS+=-2;
    }
    
    if(v_dmfL==0){
        if(v_dmf_SumL>0)v_dmfL=1; 
        else if(v_dmf_SumL<0)v_dmfL=-1;   
    }
    
    bidled8=M0_FMF_IDLE/8;
    if(VF_IGNONLY_CFCI)
    {
        v_dmfS=0;
        v_dmfL=0;        
    } 
    else
    {
        if(v_dmfS>0)
        {
        } 
        else if(v_dmfS<=0) //2013-3-3 CHANGE FROM <0 TO <=0
        {
            wmaxfmf=bidled8-(sword)fmf;
            if(wmaxfmf>0)wmaxfmf=0;
            //if(v_dmfS<wmaxfmf)v_dmfS=wmaxfmf;//2014-3-9 晚上9点，取消这个限制，以防止短项衰减太少        
            if(v_dmfL<wmaxfmf)v_dmfL=wmaxfmf;        
            
        }
    }
    

/*
    //2015-10-7
    #ifdef  LIMIT_TRANS_HIGHSPEED
        if(v_rev_625rpm>REV62D5_5750) 
        {
           // fmf+=v_dmfL/16;
            fmf+=v_dmfL/4; //2016-5-6
            fmf+=v_dmfS/4;
        }
        else if(!VF_2INJ_ENABLE_INJPHASE) 
        {
            fmf+=v_dmfL/2;
            fmf+=v_dmfS/2;        
        } 
        else
        {
            fmf+=v_dmfL;
            fmf+=v_dmfS;
                    
        }
    
    #else
        fmf+=v_dmfL;
        
        fmf+=v_dmfS;
    #endif
*/

//2016-5-21

    //4000RPM--64
    //5000RPM--80

    if(64<BYPTR_M0_TPD_LT6)sw=0;
    else sw=(64-BYPTR_M0_TPD_LT6);

    if(v_rev_625rpm<REV62D5_5000)
    {
        sw=0;
    } 
    else 
    {
        sw=((sword)(v_rev_625rpm-REV62D5_5000))*sw;    
    }


    if(sw>4096)sw=4096;
    sw=sw/4;
    
    sw=1024-sw;
    
    
    deDTFI=sw;
	


    fmf=fmf+((long int)(v_dmfL+v_dmfS)*sw)/1024;


        
            
    
    VF_TRANSLT10_TRANS=true;
    if(v_dmfS>5||v_dmfS<-5)VF_TRANSLT10_TRANS=false; //2012-12-20
    
    //2015-5-26 ADD 
    VF_TRANSGT15_TRANS=false;
		/*
        sw=v_L_M1024;
        if(v_dmf_SumL>sw)VF_TRANSGT15_TRANS=true;
        
        sw=-sw;
        if(v_dmf_SumL<-sw)VF_TRANSGT15_TRANS=true;
		*/
	//2017-3-27	重新定义VF_TRANSGT15_TRANS
	if(v_dmfL>15||v_dmfL<-15)VF_TRANSGT15_TRANS=true;
    
    
    if(fmf<bidled8)fmf=bidled8;
    
    //还有油膜需要补偿
    if(v_dmf_SumS>0&&v_dmf_SumL>0){
        v_mf_rf=(sword)V_mf-v_dmf_SumS-v_dmf_SumL;
        if(v_mf_rf<0)v_mf_rf=0;    
    }
    
    return fmf;               
}




void FS_Film(void)
{
    sword sfm;
    sword sfml;

    if(nb_film_attu==0)
    {
      
        if(VF_E0_TPS)
        {
            nb_film_attu=0;    
        } 
        else
        {
            //nb_film_attu=1;  
            nb_film_attu=0;  
        }
        
        if(v_dmf_SumL<0||v_dmf_SumS<0)
        {
            nb_film_attu=0;    
        }
        
        
            v_dmf_SumS-=v_dmfS; 
        
        //2019-2-21 处理 B8
        if(v_dmf_SumS<0||!VF_BFIRE_ENG||VF_STARTS_ENG||VF_E0_TPS)
        {
            //需要减油膜的情况
            v_dmf_SumL-=v_dmfL; 
        } 
        else
        {
            if(v_UA_M128>48)sfm=0;
            else 
            {   
                sfm=48-v_UA_M128;
            }
            
            if(sfm<16)sfm=16;//16的时候太浓
            //if(sfm<24)sfm=24;
            
            if(v_dmfS>256)v_dmfS=256;
            if(v_dmfS<-256)v_dmfS=-256;
            
            sfml=(sfm*v_dmfL)/32;
            //sfm=(sfm*v_dmfS)/32;
            
            //v_dmf_SumS-=sfm; 
            v_dmf_SumL-=sfml; 
        }
        
    } 
    else
    {
        nb_film_attu--;
    }
    
}

sword FC_Film_UA(byte tps0,byte tps1,word fmf0){
    sword sw2;
    sword sw1;
    word wfmf0;
    byte b1;
    
    word wt;//2019-2-1 peter
    
    sword swdtps;

/*peter
  2019-2-2 对UA做了修改
  特别是怠速非喷射圈，如果当圈一开始时就动油门，那么当圈进气量会增加
  这个时候，检测到TPD变化时，就以最早的相位进行喷射
  与之有关的变化
      INJ_phase.c
      mi_fmf.c
 
  另外，
      对氧传感器反馈限制的方法进行调整，从Limit位置调整到PID_CONTROL位置
      去除电压在主程序中对T1修正
      

  注意，
      本次这样修改后，热机的UA需要的比之前要大一些了

*/

    wt=T_MAINTMR-v_t_cas0;  //2019-2-2 from v_t_cas0 to v_t_cas

    //2013-12-20 CHANGE TO THE FOLLOWING
        sw2=v_te-V_Ta;
        if(sw2<0)sw2=0;
        if(sw2>120)sw2=120;
        //v_UA_M128=(byte)F_2DLookUpSbyte((sbyte)sw2,BYPTR_M1_UA_M32,7)+v_dUA_M128;
        //2015-10-27
        v_UA_M128=Adj_byte((byte)F_2DLookUpSbyte((sbyte)sw2,(char_P)BYPTR_M1_UA_M32,7),ADJ_UA);

        //b1=20;//2013-3-26
        b1=40;//2019-1-31 FROM 20 TO 40,PETER
     
        
        //2014-3-9 加强CI
        if(!VF_E0_TPS&&(VF_JCI_CFCI||VF_JCI_LST_CFCI)){
            //b1/=4;  //3-15H
            b1/=8;  //2019-1-31 FORM /4 TO /8,PETER
        }else if(VF_LEAVEIDLE_FRE_ENG){
            b1+=40;//
        } else{
            ///2014-3-8 加大断油期间加油门的贡献
            if(VF_JLEAVEIDLE_ENG||(VF_JLEAVEIDLELST_ENG&&((tps1-tps0)>90))||(VF_E0_TPS&&VF_IGNONLY_CFCI )){
               // b1/=2;    
                b1/=8;    
               
               /*
                if((VF_FORINJ_INJPHASE&&(VF_JLEAVEIDLE_ENG||VF_JLEAVEIDLELST_ENG))
                    ||
                   // (!VF_FORINJ_INJPHASE&&VF_JLEAVEIDLE_ENG&&wt<5000)//如果检测到的时间比较早，在非喷射圈还能喷进气缸，就加喷,一圈40ms，半圈20ms，扣去喷射提前时间9ms，还有11ms
                    (!VF_FORINJ_INJPHASE&&VF_JLEAVEIDLE_ENG)  // 2019-2-27
                   )//2019-2-1 peter
                   
                  
                b1/=4;//2019-1-31 如果是喷射圈，刚离开怠速，作用加强；
                 */
            } 
            
        }
    
    
    
    
    
	#ifdef	TE_10K	//20210728 chenwei add 10K缸温 
    //2019-1-31 peter,温度上来后，非喷射圈
    //if(v_te>50)
    if(v_te>80&&wt>=2600)
    {
        if(!VF_FORINJ_INJPHASE)b1=(v_te-80)+b1;    
    }
	#endif

	#ifdef	TE_2K	//20210728 chenwei add 2K水温 
    if(v_te>55&&wt>=2600)
    {
        if(!VF_FORINJ_INJPHASE)b1=(v_te-55)+b1;    
    }
	#endif
		
    //-----------------------

    //sw1=(sword)tps1-tps0;
	sw1=((sword)v_TPd-v_TPd_lst_in1cycle);
  	sw1=sw1*4;
  	
  	sw2=((sword)v_TPd_lst_in1cycle-v_TPd_lst2_in1cycle);
  	sw2=sw2*4;
  	
  	swdtps=sw2;
  	
  	if(sw2>sw1)sw1=sw2;	
	
    if(sw1>128)sw1=128;
   
    if(sw1<=4) {
        sw2=0;    
    } else{
      
        sw2=sw1;//2014-3-15
        
        if(sw2<0)sw2=-sw2;//2013-5-17
            
        wfmf0=fmf0;
        if(wfmf0>v_fmf_max)wfmf0=v_fmf_max;
            
        wfmf0=(word)sw2*wfmf0/b1;

        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>13);//2013-6-30
        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>12);//2014-4-16
        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>11);//2014-4-16
        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>9);//2014-12-9
        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>11);//2014-12-12
        //wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>13);//2015-3-24
        wfmf0=(word)(((unsigned long int)wfmf0*((word)vc_C_mf*v_UA_M128))>>14);//2015-3-24
        
        sw2=wfmf0;
        
        /*
        if(tps1<tps0){
            sw2=-sw2/2;//2013-5-18 ADD /2 
            
            if(sw2<-200)sw2=-200;//2013-5-18
                   
        }
         else if(sw1>=3){   //2013-6-30 add condciton

            //2013-8-27 add condiction
            if ((BYPTR_MV_VEHICLE==HIDER) || (BYPTR_MV_VEHICLE==WILD))
      			{
      				  sw1=(byte)((sw1*M0_FMF_IDLE)>>7); //2013-6-26
                if(sw2<sw1)sw2=sw1;
      			}            
        }
        *///2019-1-31 DELTE,PETER,这个影响了UA小的时候的UA与LD*UA的线性关系
        
    }

    if(VF_E0_TPS)v_dmi_UA=0;
    
    
    if(VF_JLEAVEIDLE_ENG||VF_JLEAVEIDLELST_ENG) 
    {
    	#ifdef	TE_10K	//20210728 chenwei add 10K缸温
        if(v_te<60)v_dmi_UA+=sw2/4;//B12 增加条件  B13
        #endif
		
		#ifdef	TE_2K	//20210728 chenwei add 2K水温
        if(v_te<50)v_dmi_UA+=sw2/4;//B12 增加条件  B13
        #endif
		
        if(v_te<40)v_dmi_UA+=sw2/4;//B12 增加条件  B13
	
    }
    else
    {
        if(!VF_FORINJ_INJPHASE){
            //sw2+=v_dmi_UA;//
            if(swdtps>20)sw2+=v_dmi_UA/2;    
            v_dmi_UA=v_dmi_UA/8;  //B18
            //v_dmi_UA=v_dmi_UA/2;
        }
    }
    return sw2;
}