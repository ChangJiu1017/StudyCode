//-----环境压力
extern void FG_P_air(byte remark);//
extern void FI_P_air_AfterReset(void);
extern void FM_P_air_WhenKeyOff(void);

extern byte nb_forpair;
extern byte v_Pair_ad_tmp;//2010-5-11 add 

extern byte v_pair_ad_input;   //PAIRS A/D采集的直接输入值，需要经过诊断之后才可用；
extern byte v_pair_ad_input_min;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最小值
extern byte v_pair_ad_input_max;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最大值
extern byte v_vbat_maxwhenpair;//       v_vbat_maxwhenpair= MAX_VBAT
extern byte v_vbat_minwhenpair;
extern byte v_pair_ad_64t;//64采样的平均,   v_pair_ad=((word)v_pair_ad*4-v_pair_ad_64t)/4

extern byte v_pair_max_afterkeyon;//                                                         //    //
extern byte v_pair_min_afterkeyon;//                                                         //    //
extern byte v_pair_aveinlast10min;//                                                         //    //

extern byte v_Pair_deg;
extern sword v_p_kpa;



