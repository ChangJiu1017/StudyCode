/*===========================================================*/
/*====ÅçÉäÏàÎ»ÅĞ±ğÄ£¿é=====================*/
/*===========================================================*/

//¹¦ÄÜ£º¼ÆËãÅçÓÍÏàÎ»£¬µÃµ½ÏÖÔÚµ½ÅçÓÍµÄÌáÇ°Ê±¼ä£» v_ts_binj
//      calculate injection timing and prepare to trig injection event.
//      To make sure to oil injected reach inlet valve after it is closed.
//		expect: condiction 1
//·µ»ØÖµ£º  F_Phase_Inj---=0£¬²»ÅçÉä£»
//          ²»µÈÓÚ0£¬ÅçÉä
//ĞÎ²Î£º tps_odr
//       prd_cash
//       prd_cas
//       vbat
//       rpm625
//       t_cas0
//       ystdc_search_t:³å³Ì¼ì²â¶ÔÏàÎ»µÄĞŞÕıÏµÊı£»
extern byte F_Phase_Inj(byte tps_odr,word prd_cas,byte vbat,byte rpm625,word t_cas0,byte ystdc_search_t,word t_cas);//»

extern void FI_Phase_Inj_AfterReset(void);

//-----public variables------------------------------------
extern word v_ts_binj;//ÅçÓÍÌáÇ°½Ç¶È£»

