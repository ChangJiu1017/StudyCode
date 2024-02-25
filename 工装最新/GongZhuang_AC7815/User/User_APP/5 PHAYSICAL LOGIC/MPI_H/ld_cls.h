/*===========================================================*/
/*====OS±Õ»··´À¡¿ØÖÆÄ£¿é=====================*/
/*===========================================================*/

//***********INIT_O2_AFTERRESET***********************
//¹¦ÄÜ£ºCLS²ßÂÔ²ÎÊýµÄ³õÊ¼»¯
extern void FI_CLS_AfterReset(void);

//¹¦ÄÜ£ºÑõ´«¸ÐÆ÷·´À¡¿ØÖÆ²ßÂÔ£¬²¢»ñµÃ·´À¡½á¹û£º v_dmLambda
extern void F_CLS_OS(byte cls2_idlemissf,sword tw1_idlesc,word FMF_frommap) ;
       //2008-8-13 change byte fmf_trans1  to sbyte fmf_trans1

extern void Save_1st_Tes(byte tes_1st);

//------declaration of public variables of cls --------------
extern byte vc_IdleMissfSum_CLS2; // µ¡ËÙ·´À¡¶Ôµ¡ËÙÊ§»ð¼ì²â×ÜÁ¿µÄÐÞÕý£»(×ÜÎªÕý)»
extern byte vc_IdleSc_CLS2;       // µ¡ËÙ·´À¡¶Ôµ¡ËÙ×Ô¶¯ËÑË÷Á¿µÄÐÞÕý£»(×ÜÎªÕý)   
//extern byte m_o2_mx	;//	4ah					;o2·´À¡µÄÄ¿±êÖµ	//2008-5-29 set as public for communicate
//extern byte v_Nb_osChange;     //2008-11-17  add 
extern byte numb_fk_m;//NUMB_FK_M;

extern void F_SL_idleIni(sbyte sl,byte blnOK);
extern void FR_idleClsAve(sbyte cls);

extern sbyte vc_CLS_2;            //same	as	leansdti //µ¡ËÙ·´À¡ÐÞÕýÁ¿
extern sbyte vc_CLS_1;            //O2·´À¡µÄÐÞÕýÁ¿£»AT	v_O2_ad	FEEDBACKCONTROL,MODI TINJ	BY + vc_CLS_1		
extern sbyte leansdtav	;//	1e2h				
