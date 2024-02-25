//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Public.h"
#include "Status_st.h"
#include "MAPS.H"


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_MAPS_STATUS;//2014-1-28 peter, delete
//#pragma DATA_SEG DEFAULT

byte v_MAPS_ad[5];
    //v_MAPS_ad[0]:
    //v_MAPS_ad[1]:对应于v_MAPS_ad[0]的后沿进气歧管压力 ；
byte v_MAPS;    //代表发动机负荷的参数，对应进气门关闭时刻的进气歧管压力 ；
byte v_MAPS_mode; //0-31
byte v_MAPS_odr;  //0-16
byte v_LAMBDA_byMAPS;//大气压力
 
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
//进气压力分级数据.2008-2-29,order_pre.dat;
//<48,0级  1级	2级	3级	4级	5级	6级	7级别	8级	9级	10级	11级	12级	13级	14级	    15级别	>232,16级;
//48	56	    64	72	80	88	96	104	    112	120	128	136	144	152	168	    200	    232	;
#define MAPS_ODR_0 192//48*4
#define MAPS_ODR_13 608//152*4
#define MAPS_ODR_14 672//168*4
#define MAPS_ODR_16 928//232*4

#define MAX_MAPS    250*4
#define MIN_MAPS    10*4
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void FI_Maps_AfterReset(void)
{
}

void FG_Maps(byte t_MAPS,byte prd_cash,byte tps_odr,word maps_ad1,word maps_ad2)
{
    word maps;
    
    VF_ER_MAPS=false;
    
    /*
    if(MO_WITHMAPS==WITH_MAPS)
    {
        if(maps_ad1>MAX_MAPS||maps_ad2>MAX_MAPS||maps_ad1<MIN_MAPS||maps_ad2<MIN_MAPS)
        {
            VF_ER_MAPS=true;
        }
    } 
    else
    {
        
    }
    */
    
    
    v_MAPS_ad[t_MAPS]=(byte)((maps_ad1+maps_ad2)>>3);

    v_LAMBDA_byMAPS=v_MAPS_ad[t_MAPS]; //2008-10-24 ADD HERE FOR LINEAR OS DISPLAY
   
   return;
   
    switch(t_MAPS)
    {
        case NO_MAPS_STOP:
            v_LAMBDA_byMAPS=v_MAPS_ad[t_MAPS];
            
            if(v_LAMBDA_byMAPS<20)VF_ER_MAPS=true;
            if(v_LAMBDA_byMAPS>200)VF_ER_MAPS=true;
        break;
        case NO_MAPS_INLET:
            VF_MAX_MAPS=false;
            maps=((maps_ad1+maps_ad2)>>1)&0x7fff;
            v_MAPS=(byte)(maps>>2);
            if(v_MAPS_ad[NO_MAPS_INLET]<20)VF_ER_MAPS=true;
            if(maps<MAPS_ODR_0)
            {
                v_MAPS_mode=0;
                v_MAPS_odr=0;    
            }
            else if(maps<MAPS_ODR_13)
            {
                v_MAPS_odr=((maps-MAPS_ODR_0)>>5)&0x0f;
                v_MAPS_mode=((maps-MAPS_ODR_0)&0x1f);
            }
            else if(maps<MAPS_ODR_14)
            {
                v_MAPS_odr=13+(((maps-MAPS_ODR_13)>>6)&0x0f);
                v_MAPS_mode=(((maps-MAPS_ODR_13)>>1)&0x1f);
            }
            else if(maps<MAPS_ODR_16)
            {
                v_MAPS_odr=14+(((maps-MAPS_ODR_16)>>7)&0x0f);
                v_MAPS_mode=(((maps-MAPS_ODR_16)>>2)&0x1f);
            }
            else
            {
                v_MAPS_mode=0x0f;
                v_MAPS_odr=0x1f;    
                VF_MAX_MAPS =true;
            }
        break;
        case NO_MAPS_INLET_HY:
        
        break;
        case NO_MAPS_OUTLET:
        break;
        case NO_MAPS_OUTLET_HY:
             if(prd_cash>PCA2000);
        
        break;
    }         
}