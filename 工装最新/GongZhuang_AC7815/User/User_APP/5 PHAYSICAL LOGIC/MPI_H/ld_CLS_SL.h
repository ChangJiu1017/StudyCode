
extern void FI_SLCLS_AfterReset(void);
extern void F_ActionSL_AfterKeyoff(void);
extern void F_SL_ByCLS(void) ;

extern void FL_SL_EE_rd(void);
extern void FL_SL_EE_wr(void);

extern void FC_SL_EE_afterIni(void);


extern sbyte v_SL_map[2][2];     //正在使用的自学习map的值；
extern byte v_SL_map_AD[2][2];     //正在使用的自学习map的值；

extern sbyte v_SL_map_wr[4];  //准备写于到EE中的、学习到的值；
extern byte v_SL_add_WR[4];      //需要写入的自学习的四个点的地址

extern byte v_flag_eewr;

extern byte v_add_sl;          //本循环自学习的点；
extern sbyte v_SL_cls_lst;         //上一次的自学习值，用于光顺

extern sbyte v_map_SL_basic;    //自学习MAP的基础值  

extern byte v_ccl_sl;          //当前自学习点学习的圈数；
extern byte v_nb_SL  ;    // 自学习的次数

extern byte v_addr_for1st_eeRD;

extern sbyte v_cls_sl_learn;     //当前学习值；

extern byte v_OS_flu_SLpoint;    //怠速当量比判断的时间窗口内，OS波动的次数

extern sbyte v_fmf_sl_idle; //怠速点自学习值；

extern byte v_TPd_sl_rd;
extern byte v_TPmod_sl_rd;
extern byte v_Rev_sl_rd;
extern byte v_Revmod_sl_rd;

extern byte v_TPd_rd;
extern byte v_REV_rd;

extern byte nb_ccl_r;  //浓 的次数
extern byte nb_ccl_l;  //稀 的次数
extern byte v_u_sl;    //v_UP_M128
extern byte v_t_aftersl_s; //持续稀或者浓之后的时间
extern byte v_os_flu_trans;//持续稀或者浓

extern word v_relate_sl_sum;//2013-5-9


extern byte v_add_sl_steady;      //上一循环自学习的点；

extern byte v_nb_wr_fail;  //写入失败的次数

extern byte v_nb_flu_forSL;//在一定期间内，氧传感器波动的次数
extern byte v_flag_chkwr;         //EE需要写入的标志，刚准备写入时，v_flag_chkwr

extern sbyte CheckSlLimit(word fmf,sword swl,byte sladd);

extern byte v_nb_wr_fail;  //写入失败的次数

//extern sbyte v_SLtg_sum;      //2013-5-16  学习目标累计
extern sbyte v_LambdaTg_SL;//2013-5-16

extern byte v_DTC_SL;//2015-12-17 未满足自学习条件的原因

extern byte v_status_IDLESL;  //2019-1-20


/*
需要显示的信息：
    V_CLSSLH_STATUS
    V_CLSSL_STATUS
*/

//extern void FR_SL_test(void);
