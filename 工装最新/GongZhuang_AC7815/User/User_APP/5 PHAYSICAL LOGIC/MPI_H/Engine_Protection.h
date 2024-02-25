/**********Engine_Protection.h**************/
//***发动机保护?**//

extern void FI_EngProtection_AfterReset(void);
extern void F_OverTMPProtection_Condiction(void);
extern sbyte F_UP_OverTMP(byte data);   //20210123 chenwei add
extern void F_status_ENG(void);  //20210125 chenwei add
extern void F_OVERTMP_ACTION(void);//20210206 chenwei add

extern word V_OverTMP_st_MskUnEna;
extern byte v_TPS_odr;
extern byte v_Vbat_inuse;
extern byte cur_code;
extern unsigned long int v_t_eng_t_s;
extern sword v_te;
extern word v_t_overTMP_1s;    //达到过温保护启用缸温限值及工况要求后持续运行时间
extern byte UP_OVERTEP_PROTECTION;
extern sbyte v_LambdaTg;
extern byte UP_OVERTMP_PROTECTION;
extern word t_UPOVERTMP_start_1s; 
extern word v_rev_15d6rpm;
extern byte v_TPS;

extern sbyte v_LambdaTg_break;
extern sbyte v_LambdaTg_lookup;