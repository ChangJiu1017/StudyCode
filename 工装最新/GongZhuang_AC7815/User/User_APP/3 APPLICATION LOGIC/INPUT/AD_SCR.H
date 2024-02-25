


extern void F_SCRAD(void);

extern byte v_EGTU_ad;
extern byte v_EGTD_ad;
extern byte v_TU_ad;
extern byte v_ADB_ad;

extern    sword v_EGTU_deg;
extern    sword v_EGTD_deg;
    
extern    sbyte v_TU_deg;
extern    byte v_ADB_percent;

extern byte v_VBAT;         //2014-10-15 add 

extern sword v_EGT_deg;// 排气温度使用值 2015-1-23 add 


extern byte nb_VbatValid;//电压超范围的次数

extern void InitAdAfterReset(void);
extern byte CheckVoltValid(void);
extern void CheckSCRAD(void);


