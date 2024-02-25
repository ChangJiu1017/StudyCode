
#include "public.h"

#include "ad_scr.h"

#include "look_up.h"

#include "status_st.h"

#if SYS==SYS_SCR

    #define F_INC(b)  if(b<255)b++;

    //#define 0
    #define TU_FROZEN     -10
    #define TUAD_MIN      5
    #define TUAD_MAX      250
    
    #define EGTAD_MIN     5
    #define EGTAD_MAX     240 //2015-2-3 FROM 250 TO 240

   
    #define EGT_WARM      230
    
    #define ADBAD_MIN     5
    #define ADBAD_MAX     250
    
    #define NB_ERR_UB 200
    
    
    #define ADB_0P    
    #define ADB_100P    
    

    byte v_EGTU_ad;
    byte v_EGTD_ad;
    byte v_TU_ad;
    byte v_ADB_ad;

    //2015-1-23 add 
    byte v_EGTU_ad_lst;
    byte v_EGTD_ad_lst;
    byte v_TU_ad_lst;
    byte v_ADB_ad_lst;


    sword v_EGTU_deg;
    sword v_EGTD_deg;
    
    sword v_EGT_deg;// 排气温度使用值 2015-1-23 add 
    
    
    sbyte v_TU_deg;
    byte v_ADB_percent;

    byte v_VBAT;//2014-10-15 ADD 

		#define UB_SCRAD  8  
		
		#define VBAT_MAX  206
		#define VBAT_MIN  140
		#define VBAT_AVE  173
		
	  
	  	
		byte nb_VbatValid;//电压超范围的次数
		
		byte nb_TU_LB;
		byte nb_TU_UB;
		byte nb_TU_FROZEN;
		
		byte nb_ETIN_LB;
		byte nb_ETIN_UB;
		byte nb_ETOUT_LB;
		byte nb_ETOUT_UB;
		
		byte nb_ADB_LB;
		byte nb_ADB_UB;
		
		byte nb_ADB_LOW;//液位低
				
    const byte M1_ADB_S_PERCENT[9][2]=				
    {//采样值，温度				
    81	,	100	,//	
    119	,	87	,//	
    148	,	75	,//	
    173	,	62	,//	
    193	,	50	,//	
    201	,	43	,//	
    221	,	25	,//	
    233	,	12	,//	
    243	,	0	,//	
    };				

    //	3.98E+03		3.30E+03	
    const byte M1_TU_S_DEG[9][2]=				
    {//采样值，温度				
        230	,	0	,//	-40
        179	,	10	,//	-20
        144	,	20	,//	-10
        126	,	25	,//	-5
        109	,	30	,//	0
        78	,	40	,//	10
        55	,	50	,//	20
        26	,	70	,//	40
        6	,	110	,//	80
    };				

        				
				
    //2015-1-19				
    //2015-1-20				
    const byte M1_PTS_ADD40D6[9][2]=				
    {//采样值，(温度+40)/6				
        2	,	0	,//	-40
        13	,	6	,//	0
        39	,	23	,//	100
        65	,	40	,//	200
        91	,	56	,//	300
        115	,	73	,//	400
        139	,	90	,//	500
        205	,	140	,//	800
        245	,	173	,//	1000
    };				

    //*========================================================================*//
    
    //*========================================================================*//
    void InitAdAfterReset(void)
    {
        nb_VbatValid=0;
        v_VBAT=VBAT_AVE;    

    		nb_TU_LB=0;
    		nb_TU_UB=0;
    		nb_TU_FROZEN=0;
    		
    		nb_ETIN_LB=0;
    		nb_ETIN_UB=0;
    		nb_ETOUT_LB=0;
    		nb_ETOUT_UB=0;
    		
    		nb_ADB_LB=0;
    		nb_ADB_UB=0;

    		nb_TU_LB=0;
    		nb_TU_UB=0;
    		nb_TU_FROZEN=0;
    		
    		nb_ETIN_LB=0;
    		nb_ETIN_UB=0;
    		nb_ETOUT_LB=0;
    		nb_ETOUT_UB=0;
    		
    		nb_ADB_LB=0;
    		nb_ADB_UB=0;
    		
    		nb_ADB_LOW=0;
    		
    		v_EGT_deg=0;

        v_EGTU_ad_lst=0;
        v_EGTD_ad_lst=0;
        v_TU_ad_lst=0;
        v_ADB_ad_lst=0;
    		

    }

    
    void F_SCRAD(void)
    {
    
    
            v_EGTU_deg=F_2DLookUp(v_EGTU_ad,M1_PTS_ADD40D6,UB_SCRAD,0,false);
            v_EGTU_deg=v_EGTU_deg*6-40;
            
            v_EGTD_deg=F_2DLookUp(v_EGTD_ad,M1_PTS_ADD40D6,UB_SCRAD,0,false);
            v_EGTD_deg=v_EGTD_deg*6-40;
    
            v_ADB_percent=F_2DLookUp(v_ADB_ad,M1_ADB_S_PERCENT,UB_SCRAD,0,false);
            
            v_TU_deg=F_2DLookUp(v_TU_ad,M1_ADB_S_PERCENT,UB_SCRAD,0,false);
            v_TU_deg-=40;
            
            
            
    }
    
    //校核电压是否符合范围
    byte CheckVoltValid(void)
    {
            if(v_VBAT>VBAT_MAX||v_VBAT<VBAT_MIN){
                if(nb_VbatValid<255)nb_VbatValid++;
            }
            else nb_VbatValid=0;
            
            if(nb_VbatValid>20){
                if(v_VBAT>VBAT_MAX)VF_UB_VOLT=true;
                if(v_VBAT<VBAT_MIN)VF_LB_VOLT=true;
                return false;
            }
            
            VF_UB_VOLT=VF_LB_VOLT=false;
            return true;     
    
    }
    
    void CheckSCRAD(void){
    
        //--------------------------
        if(v_TU_ad<TU_FROZEN) 
        {
            F_INC(nb_TU_FROZEN);    
        } else {
            nb_TU_FROZEN=0;
        }
        
        if(nb_TU_FROZEN>NB_ERR_UB)VF_PUMPFROZEN_SCR=true;//VF_UB_TU;
        else VF_PUMPFROZEN_SCR=false;
        
        //--------------------------
        if(v_TU_ad<TUAD_MIN) 
        {
            F_INC(nb_TU_LB);    
        } else {
            nb_TU_LB=0;
        }
        if(nb_TU_LB>NB_ERR_UB)VF_LB_TU=true;
        else VF_LB_TU=false;
        
        
        //--------------------------
        if(v_TU_ad>TUAD_MAX) 
        {
            F_INC(nb_TU_UB);    
        } else {
            nb_TU_UB=0;
        }
        if(nb_TU_UB>NB_ERR_UB)VF_UB_TU=true;
        else VF_UB_TU=false;


        //--------------------------
        if(v_EGTU_ad<EGTAD_MIN) 
        {
            F_INC(nb_ETIN_LB);    
        } else {
            nb_ETIN_LB=0;
        }
        
        if(nb_TU_LB>NB_ERR_UB)VF_LB_TEIN=true;
        else VF_LB_TEIN=false;

        //--------------------------
        if(v_EGTU_ad>EGTAD_MAX) 
        {
            F_INC(nb_ETIN_UB);
            
            v_EGTU_deg=0;//2015-1-23    
        } else {
            nb_ETIN_UB=0;
        }
        
        if(nb_ETIN_UB>NB_ERR_UB)VF_UB_TEIN=true;
        else VF_UB_TEIN=false;
        
        //--------------------------
        if(v_EGTD_ad<EGTAD_MIN) 
        {
            F_INC(nb_ETOUT_LB);    
        } else {
            nb_ETOUT_LB=0;
        }

        if(nb_ETOUT_LB>NB_ERR_UB)VF_LB_TEOUT=true;
        else VF_LB_TEOUT=false;
        
        //--------------------------
        if(v_EGTD_ad>EGTAD_MAX) 
        {
            F_INC(nb_ETOUT_UB);    
            v_EGTD_deg=0;//2015-1-23    

        } else {
            nb_ETOUT_UB=0;
        }

        if(nb_ETOUT_UB>NB_ERR_UB)VF_UB_TEOUT=true;
        else VF_UB_TEOUT=false;
        
        //--------------------------
        if(v_ADB_ad<ADBAD_MIN) 
        {
            F_INC(nb_ADB_LB);    
        } else {
            nb_ADB_LB=0;
        }

        if(nb_ADB_LB>NB_ERR_UB)VF_LB_ADB=true;
        else VF_LB_ADB=false;
        
        //--------------------------
        if(v_ADB_ad>ADBAD_MAX) 
        {
            F_INC(nb_ADB_UB);    
        } else {
            nb_ADB_UB=0;
        }

        if(nb_ADB_UB>NB_ERR_UB)VF_UB_ADB=true;
        else VF_UB_ADB=false;
        

        if(v_ADB_percent<20){
            F_INC(nb_ADB_LOW);
        } else{
            nb_ADB_LOW=0;
        }
     
        if(nb_ADB_LOW>100)VF_LB_ADB_SCRDTC=true;
        else VF_LB_ADB_SCRDTC=false;
        
        
        //如果两只温度传感器都
        if(!VF_UB_TEOUT&&!VF_UB_TEIN){
            v_EGT_deg=(v_EGTU_deg+v_EGTD_deg)/2;
        } else if(!VF_UB_TEOUT){
            v_EGT_deg=v_EGTD_deg;
        }else if(!VF_UB_TEIN){
            v_EGT_deg=v_EGTU_deg;
        } else{
            v_EGT_deg=0;
        }
        
    }

#endif
