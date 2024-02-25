/**********CCPS.h**************/
//***碳罐电磁阀***//



extern byte CCPS_Pct_DutyCycleMax_C;    //允许活性碳罐工作的最大碳罐电磁阀占空比
extern byte CCPS_Pct_DutyCycleMin_C;    //允许活性碳罐工作的最小碳罐电磁阀占空比
extern byte CCPS_Pct_DutyCycleIdleMax_C;    //允许活性碳罐工作的怠速时的碳罐电磁阀占空比


extern void FI_CCPS_afterReset(void);
extern void F_CCPSOpen_Condition(void);
extern void F_CCPS_ACTION(void);




