/*===========================================================*/
/*====OS闭环反馈控制模块=====================*/
/*===========================================================*/

//***********INIT_O2_AFTERRESET***********************
//功能：CLS策略参数的初始化
extern void FI_CLS_AfterReset(void);

//功能：氧传感器反馈控制策略，并获得反馈结果： v_dmLambda
extern void F_CLS_OS(byte cls2_idlemissf,sword tw1_idlesc,word FMF_frommap) ;
       //2008-8-13 change byte fmf_trans1  to sbyte fmf_trans1

extern void Save_1st_Tes(byte tes_1st);

//------declaration of public variables of cls --------------
extern byte vc_IdleMissfSum_CLS2; // 怠速反馈对怠速失火检测总量的修正；(总为正)�
extern byte vc_IdleSc_CLS2;       // 怠速反馈对怠速自动搜索量的修正；(总为正)   
//extern byte m_o2_mx	;//	4ah					;o2反馈的目标值	//2008-5-29 set as public for communicate
//extern byte v_Nb_osChange;     //2008-11-17  add 
extern byte numb_fk_m;//NUMB_FK_M;

extern void F_SL_idleIni(sbyte sl,byte blnOK);
extern void FR_idleClsAve(sbyte cls);

extern sbyte vc_CLS_2;            //same	as	leansdti //怠速反馈修正量
extern sbyte vc_CLS_1;            //O2反馈的修正量；AT	v_O2_ad	FEEDBACKCONTROL,MODI TINJ	BY + vc_CLS_1		
extern sbyte leansdtav	;//	1e2h				
