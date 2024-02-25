#include "public.h"
#include "status_st.h"
#include "ad.h"

#include "ld.h"

#include "add.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
/*
byte vc_FMF_tair; //2008-6-5 changgeto BYTE
sbyte vc_FMF_teng; //ÓÍÁ¿µÄ¸×Í·ÎÂ¶ÈÐÞÕýÏµÊý

#define	M0_K_FMF_AIR_M128		-36
#define M0_B_FMF_AIR		135

#define M0_25CM128 38144
*/

byte v_mti;

#define M0_273PLUS25_M64 (word)19072//298/2

#define M0_273PLUS25_M64_M128   (unsigned long int)2441216

#define M0_273PLUS25_M64_M128_MM0 (unsigned long int)244121600//  M0_m0

#define M0_273M64 ((word)17472)


#define M0_m0 100

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
/*
//¹¦ÄÜ£º»ñµÃ¿ÕÆøÎÂ¶È¶ÔÓÍÁ¿µÄÐÞÕýÏµÊý£vc_FMF_tair»
void FC_FMF_Tair(sbyte tair)
{
    sword sw;
    
    
    
    sw=tair;
    sw+=273;
    if(tair<0)sw+=tair/2;
    
    //sw=(sw*M0_K_FMF_AIR_M128)/128+M0_B_FMF_AIR;
    //vc_FMF_tair=(byte)sw;
    //2012-11-30 CORRECTED
    vc_FMF_tair=M0_25CM128/sw;
    
    //w=tair;
    //if(w>=MO_TAIR_INDEXFORFMF_MAX)w=MO_TAIR_INDEXFORFMF_MAX;
    //vc_FMF_tair=M1_FMF_TAIR[w];
    
}

//·¢¶¯»úÎÂ¶È²ÉÑù£¬×¢ÒâÕâÀïÊÇ·¢¶¯»úÎÂ¶È²ÉÑù£¬²»ÊÇTW
void FC_FMF_Teng(sword teng,sbyte tair)
{
    sword sw;
    
    sw=teng-tair;
    
    if(sw<0)sw=0;
    
    //sw=(sw*M0_K_FMF_AIR_M128)/256;
    sw=(sw*M0_K_FMF_AIR_M128)/256;//2012-9-26
    
    vc_FMF_teng=(sbyte)sw;
}
*/

/*
  ¼ÆËãmti:
    298*v_FMF/(273+V_Ta+v_T_M64*(v_te-25)/64);
  =298*v_FMF/[273+v_ta+v_T_m64*(v_te-25)/64]
  =298*64*v_FMF/[(273*64+v_ta*64+v_T_m64*(v_te-25))]
*/
void FC_mti(void){
    word w;
    word w2;
    
    /*
    lm=M0_273PLUS25_M64*(long int)v_FMF;
    
    w=(M0_273M64+V_Ta*64+v_T_M64*(v_te-V_Ta));  //2013-3-17 from 25 to TA
    w=lm/w;
    
    v_mti=(sbyte)((sword)w-v_FMF);
    */
    w2=v_FMF;
    if(w2<M0_m0)w2=M0_m0;
    w2=v_T_M64*M0_m0/w2;//2013-4-16 adjust

    w=(M0_273M64+V_Ta*64+w2*(v_te-V_Ta));  //2013-3-17 from 25 to TA

    v_mti=M0_273PLUS25_M64_M128/w;
    /*
    w2=v_FMF;
    if(w2<M0_m0)w2=M0_m0;
    
    v_mti=M0_273PLUS25_M64_M128_MM0/w/w2;    
    */
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

