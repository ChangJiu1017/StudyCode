

#include "ld_cls.h"
#include "ld_CLS_SL.h"
#include "ld_isc.h"
#include "ld_lspeed.h"
#include "ld_tair.h"
#include "ld_altitude.h"

#include "afr_idle_aq.h"
#include "afr_idle_missfire.h"
#include "afr_MissFire_NotIdle.h"
#include "afr_STARTS.h"
#include "afr_TRANS.h"
#include "afr_CFCI.h"

#include "mi_Film.h"
#include "mi_FMF.h"

#include "Engine_Protection.h"  //20210125 chenwei add



//负荷相关
extern void FC_Ld_basic(void);
extern void FC_Ld_just(void);
extern void FC_mi_trig(void);

extern void FC_mi_just(void); 

extern void FI_Ld_AfterReset(void);

extern void FPredict_Ld(byte tps);

extern void Save_TPS_basic(byte tps);  //2014-10-4 add 


extern byte v_TPSb;
extern byte v_TPSc;

extern sword v_dld_dtps;//dtps变化引起的负荷的变化

extern sword v_tw1_inj;
extern sbyte v_LambdaTg_lookup;   //20210130 chenwei add up查表值

