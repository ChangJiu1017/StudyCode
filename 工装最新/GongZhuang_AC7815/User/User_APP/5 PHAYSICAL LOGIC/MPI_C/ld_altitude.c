#include "Public.h"
#include "Status_st.h"

#include "ld.h"
#include "look_up.h"

#include "ee.h"

#include "ad.h"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
sword Co_FMF_Altitude;           //Œ»Ã¨∫£∞Œ–ﬁ’˝¡ø£ª

byte v_mpa;//2012-10-11


/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
                                                                          
void FI_Altitude_AfterReset(void)
{
    if(VF_POR_HARD)
    {
       /*
        v_kDt=128;//M2_CO_PARTLOAD_ALTITUDE[0][pair_odr];
        v_Altitude_nta=128;             //2008-9-16 add
        v_Altitude_J=M0_CO_PARTLOAD_J;//
       */
       v_mpa=128; 
    }
}
