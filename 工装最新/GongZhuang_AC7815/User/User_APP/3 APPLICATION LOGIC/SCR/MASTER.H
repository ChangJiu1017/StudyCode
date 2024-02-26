




extern void Init_MasterWhenReset(void);
extern void SaveForRpm(word prd_pulse);
extern void SaveForTorque(byte pedal,byte ta);

extern void SaveForQinj_D1ul(word t1, word t_between_t1);

extern void FCal_MEDF(void);

extern byte Get_PumpStateByMaster(void);//2014-12-23根据MASTER模式判断泵状态


extern byte Get_PumpStateAtMater(void);//在MASTER模式下，判断泵状态；

//======================================================
extern void SaveForTPercent_EEC1(byte T); 

extern void SaveForN_EEC1(word n_0d125); 

extern void SaveForEnviroment_AMCON(
            byte pa_0d5kpa
            ,word ta_inner_0d03125_add273
            ,byte ta_inlet
            ,word t_ground
            ); 

extern void SaveForExhaust_SCR1(
            word t_exh_u,
            word t_exh_d,
            byte p_adblue,
            unsigned long int  q_exh); 

extern word	v_DEF_mlPerH;     //尿素喷射量

extern unsigned long int v_prd_eng_4us;             //发动机运转周期，8us，因为最低转速比摩托车低

extern word	v_DEFb_mlPerH;    //基础尿素喷射量

extern word v_Qinj_D1ul;             //当前的燃油喷射量，0.1ul
extern byte v_Qinj_PercentM2D5;       //当前燃油喷射量的比例,一定注意是100%还是200%的区别
extern byte v_n_PercentM2D5;          //转速的百分比

extern byte v_Pedal_percentM2D5;       //油门踏板位置的百分比

extern byte v_Pedal_ad_TPS;            //油门踏板位置，可以从模拟ECU获得,从TPS端口获得的
extern byte v_Pedal_ad_TA;            //油门踏板位置，可以从模拟ECU获得，从TA端口获得的
extern word v_pulse_prd_4us;           //虚拟ECU采集的脉冲的周期，可转化为发动机转速
extern word v_n_rpm;                   //发动机转速
extern unsigned long int v_prd_eng_8us;             //发动机运转周期，8us，因为最低转速比摩托车低



extern byte v_DEFBASE_percentM2D5;     //DEFBASE基础值找 DEF MAX的百分比

extern byte 	v_KNH3_M64;       // 
extern byte	v_Kair_M64;       //
extern byte 	v_MAXNSR_M128;    //
extern word  v_Mexh_D1cycle;     //每圈的排气质量,单位0.1mg，由于比重与油接近，因此不考虑了
extern word 	v_Mexh_kgPerH;    //排气流量
extern word	v_MDEFmax_mlPerH; //尿素基础喷射量的最大限值；

extern word  v_Mexh_kgPerH_byO2;    //根据O2计算得到的排气流量

extern word v_SV;              //空速

extern word  v_NOX_tg;    //NOX的目标值

extern word  v_Mexh_kgPerH_fromECU; //2014-12-7 来自ECU的排气流量
extern word v_T_exh_SCR_U_fromECU; //来自ECU的SCR上游的温度
extern word v_T_exh_SCR_D_fromECU; //来自ECU的SCR下游的温度

#define M0_N_ST 230  //2014-12-23 DEFINITION

extern void FC_1s_Master(void);


extern word v_Exh_ml;//2015-1-23
extern word v_DefMax_PerC;//2015-1-23每圈的DEF

