#include "Public.h"

#include "status_st.h"
#include "ee.h"
#include "sci.h"

#include "events.h"

#include "DTC.h"

#include "add.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_INJPHASE_STATUS;
//#pragma DATA_SEG DEFAULT
word v_ts_binj;//≈Á”ÕÃ·«∞ ±º‰£



/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

void FI_Phase_Inj_AfterReset(void)
{
    V_INJPHASE_STATUS=false;
    VF_MODE1_INJPHASE=true;
	v_ts_binj=125;//0.5ms
    
}
//π¶ƒ‹£∫º∆À„≈Á”Õœ‡Œª£¨µ√µΩœ÷‘⁄µΩ≈Á”ÕµƒÃ·«∞ ±º‰£ª v_ts_binj
//      calculate injection timing and prepare to trig injection event.
//      To make sure to oil injected reach inlet valve after it is closed.
//		expect: condiction 1
//∑µªÿ÷µ£∫  F_Phase_Inj---=0£¨≤ª≈Á…‰£ª
//                  µ»”⁄1£¨≈Á…‰
//                  µ»”⁄2£¨Ω«±Í“Ï≥£‰
//–Œ≤Œ£∫ tps_odr
//       prd_cash
//       prd_cas
//       vbat
//       rpm625
//       t_cas0
//       ystdc_search_t:≥Â≥ÃºÏ≤‚∂‘œ‡Œªµƒ–ﬁ’˝œµ ˝£ª
byte F_Phase_Inj(byte tps_odr,word prd_cas,byte vbat,byte rpm625,word t_cas0,byte ystdc_search_t,word t_cas)
{
    word w;
    byte bln;
    word w1;
    word w2;
    word wn;
	word d1s;
    
    bln=false;
	/*
	if(BYPTR_M0_T_INJPHASE==255)
	{
		d1s=0;
	}
	else
	{
		d1s=(word)BYPTR_M0_T_INJPHASE*25;
	}*/
	d1s=0;
	

    //if(!VF_ADDINJNEED_NOTYSTDC&&VF_BFIRE_ENG &&!(VF_JLEAVEIDLETRANS_ENG&&rpm625<REV62D5_2500)&&!VF_LIDLE_ADVANCE_INJPHASE) //2013-1-5
    if(!VF_ADDINJNEED_NOTYSTDC&&VF_BFIRE_ENG &&!(VF_JLEAVEIDLETRANS_ENG&&rpm625<REV62D5_2500)) //2019-2-1 DELETE VF_LIDLE_ADVANCE_INJPHASE,PETER
    {                          //2010-3-6 add condictiong VF_SCANT4_ATIDLE_NOTYSTDC;
        if(VF_E0_TPS&&rpm625<REV62D5_2000) //2009-12-30
        {
            /*if(prd_cas>INJ_REACH_BMS)w=prd_cas-INJ_REACH_BMS;
            else w=1;
            */
             w=prd_cas/2-750+d1s;  //f5,b3
        }
        //else if(!VF_FORINJ_INJPHASE&&(prd_cash>=NOPUMP_SPEED))    //VF_T35TESTNEED_NOTYSTDC
       /* else if(!VF_FORINJ_INJPHASE&&(rpm625<REV62D5_ADD1INJ||VF_T35TESTNEED_NOTYSTDC))    //VF_T35TESTNEED_NOTYSTDC
        {                             // 2009-12-26 add condiciton VF_T35TESTNEED_NOTYSTDC
            if(!VF_T35TESTNEED_NOTYSTDC&&(rpm625<REV62D5_2500))w=prd_cas-INJ_REACH_BMS; // 2013-1-6 Õ∆≥Ÿ
            else w=prd_cas/2; //2012-11-23 PM2
        }*/
        else
        {
            //2008-10-22 ∑¢œ÷£¨◊™ÀŸ∏ﬂ”⁄NOPUMP_SPEED ±£¨≤‚T35£¨≥Ã–Úª·‘À◊™µΩ’‚¿Ô£ª
            //normal_injt
            if(VF_TURNONINJ_ENG)
			{            
                
				
				w=prd_cas/2;//750=3ms
                    
                if(w>750)
				{
					w=w-750;
					
					if(rpm625>REV62D5_8000)
					{
						w=1;
					}
				}
                else 
				{
					w=1;
				}
        	}
        	else
			{
      		    goto NO_Ptig_i;
			}

        }
    }
    else
    {
        /*
        if(VF_3INJ_ENABLE_INJPHASE)w=prd_cas/2;//2013-2-28◊™ÀŸµÕµƒ ±∫Ú£¨  µ±øø∫Û“ªµ„£¨±„”⁄≤π≈Á£ª
        else  w=prd_cas/4; 
        
        //if(!VF_BFIRE_ENG||(v_nb_notsyninj_add)||VF_LIDLE_ADVANCE_INJPHASE)w=prd_cas/2;  //2013-1-10
        if(!VF_BFIRE_ENG||(v_nb_notsyninj_add))w=prd_cas/2;  //2019-2-1
        
        */
        
        //F5, B6
        if(!VF_BFIRE_ENG)
        {
            w=prd_cas/4; //–Ë“™±‹ø™∆Ù∂Ø ±µÁ—π±ª¿≠µÕƒ«“ª»¶
			 //2020-6-17 ‘≠¿¥Œ™/2£¨∆Ù∂ØΩ◊∂Œ◊™ÀŸ±‰ªØÃ´øÏ£¨”–“ª»¶≈Á”Õ—”≥ŸΩœ∂‡¥•∑¢∫Û µº √ª”–≈Á…‰
        } 
        else
        {
            w=1;
        }
        
        
        
        //2019-2-1 peter
        //if(VF_LIDLE_ADVANCE_INJPHASE){ //‘ˆº”BFIREµƒÃıº˛£¨
        /*  2019-2-21 delete for B3,F5
        if(VF_LIDLE_ADVANCE_INJPHASE&&VF_BFIRE_ENG&&rpm625<REV62D5_2000)
        {
            w=1;  //¿Îø™µ°ÀŸ£¨«“ ±º‰ªπ‘Áµƒª∞£¨¡¢º¥≈Á…‰£¨»√≈Á…‰ø…“‘Ω¯»ÎµΩµ±«∞’˝‘⁄Ω¯∆¯’‚“ª»¶
            deDTC++;
        } */
    }

    
    if(w<1)w=1;//2008-9-6 add
    w2=w;
    v_t_bInj_toCas_tmp=w;

    CLR_WDG;
    wn=T_MAINTMR;
    /*while(!VF_INJSTOP_HARD){

        CLR_WDG;
        if(!VF_OLDCA_CAS)
        {    
            break;//2010-1-18 ¡Ÿ ±
        }
        if(VF_PCAOV_CAS) {
            VF_JSTOP_ENG=true;
            return 3;//2008-6-5 ”µ”–∑¿÷π£¨≥ˆœ÷≥Ã–Ú∏’‘À◊™÷¡¥À ±£¨±ªÕ£≥µ
        }
    };*/  
        //waiting for the last injection end
    if(!VF_TURNONINJ_ENG)VF_TRIGTPS_TRANS=true;//
    else{VF_TRIGTPS_TRANS=false;}
    //”Î¡ı¿œ ¶‘≠ÀºœÎ≤ªÕ¨

    /*w =t_cas+w;
	
    w1=T_MAINTMR;
	
	//“‘œ¬’‚∂Œ”¶∏√ «√ª ≤√¥”√µƒ£¨µ´ «œ»±£¡Ù£¨–Ë“™◊–œ∏¿Ì“ªœ¬
    if(((w<(unsigned long int)w1+2&&w>t_cas)&& w1>t_cas)//¥”≤∂◊ΩΩ«±Íø™ º÷¡ΩÒ£¨÷˜∂® ±∆˜√ª”–“Á≥ˆ
        ||
        (w>t_cas&&w1<t_cas)//2015-3-5 add DY150∑¢≥Â
        || ((t_cas>w1)&& (w>t_cas||w<(unsigned long int)w1+2)))                  //¥”≤∂◊ΩΩ«±Íø™ º÷¡ΩÒ£¨÷˜∂® ±∆˜”–“Á≥ˆ
    {   //’‚¡Ω÷÷«Èøˆœ¬£¨”¶∏√¡¢º¥µ„ª£ª //’‚¿Ôµƒ«∞Ã· «£¨ºŸ…Ë¥”Ω«±Í≤∂ªÒø™ º÷¡µ±«∞µƒ ±º‰≤ªª·≥¨π˝÷˜∂® ±∆˜µƒ“ª∏ˆ÷‹∆⁄
        w=10;//2008-3-17 zhangping
    }
    else
    {
        w=w-w1;
		
    }*/

    
    if(w<=2)w=4;

    //2014-3-8
    /*if(VF_3INJ_ENABLE_INJPHASE){
        w1=T_MAINTMR-v_t_t3ed; //µ±«∞¿ÎT1Ω· ¯ ±µƒ ±º‰£ª
        if(w1<1250)w1=1250-w1;
        else w1=1;
        
        if(w<w1)w=w1;;//∑¿÷π¡Ω∏ˆT1∞§µƒÃ´Ω¸ ,÷¡…Ÿ±£≥÷5MS   
    }*/
    
    v_ts_binj=w;



    /*-move to main form inj phase-*/
    bln=true;
    
  NO_Ptig_i:
    if(VF_MODE1TO2_INJPHASE){VF_FORINJ_INJPHASE=false;VF_MODE1_INJPHASE=false;}
    
    return bln;
}

