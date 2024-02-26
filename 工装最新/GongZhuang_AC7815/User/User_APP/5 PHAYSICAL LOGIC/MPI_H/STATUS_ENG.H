/*===========================================================*/
/*====发动机工况/状态判别模块=====================*/
/*===========================================================*/

//功能：获得发动机状态
//      在新角标到来之后，点火结束，获得TPS和REV参数之后调用该函数

extern void FG_EngStatus_Basic(void);


//功能：发动机开始运转时，对发动机状态参数的初始化
//extern void FI_status_ENG_BeforeStarts(byte teng); //2014-1-27 peter, delete

//2008-10-30 add
//硬件复位后进行的初始化
extern void FI_status_AfterReset(void);//2008-10-30 add

extern void FG_EngStatus_Just(void);    //2014-10-4
extern void F_CC_Diag(void);	//20210819 chenwei add 侧撑熄火诊断


extern byte wait_run_start	;//equ	134h	;用于防止连续两次离开怠速的时间间距过短；when	convert	from	idle	to	notidle,	let	which=wait_run_startb	and	dec	which	per	runs.	Continously	injecting	if	convert	from	idle	to	noidle	when	wait_run_start=0
extern byte v_rev62d5rpm_cf;  //2013-1-12

extern byte v_ccl_for3rd;//2014-3-29

extern word v_t1max_v;//根据电压计算得到的T1的最大值；

extern    byte VF_EXH_MATCH;//2017-1-17
extern byte Is_RunPointMatch(byte map);

