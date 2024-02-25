
extern void Samp_MCUTmp(void) ;

extern word v_NB_INJDRV;
//extern byte HARD_RST_FLG1;
//extern byte HARD_RST_FLG2;
extern word t_delayforkeyofftest_4us;//2009-12-20

extern void Action_whenKeyOff(void); //2014-3-30
extern void Init_afterReset(void);         //

extern void Action_WhenStop(void);//停机状态的动作
extern void F_act_AfterStop(void);//停机后马上要执行的动作

extern void Init_beforeWaitingFor1stCA(void);
extern void IGN_CONTROL_BYCAN(void);
extern void Init_beforeStarts(void);

extern byte F_act_AfterKeyoff(void);//2014-10-4  
extern byte v_rev62d5_hyUL;
extern byte t_WOT;
extern byte nb_vbat_times;

extern sword v_Vbat_ad_lst;
extern word v_preinj_ca;
//20210427 chenwei add
extern byte v_nb_T3d;
extern byte switch_1;