/*===========================================================*/
/*====OS�ջ���������ģ��=====================*/
/*===========================================================*/

//***********INIT_O2_AFTERRESET***********************
//���ܣ�CLS���Բ����ĳ�ʼ��
extern void FI_CLS_AfterReset(void);

//���ܣ����������������Ʋ��ԣ�����÷�������� v_dmLambda
extern void F_CLS_OS(byte cls2_idlemissf,sword tw1_idlesc,word FMF_frommap) ;
       //2008-8-13 change byte fmf_trans1  to sbyte fmf_trans1

extern void Save_1st_Tes(byte tes_1st);

//------declaration of public variables of cls --------------
extern byte vc_IdleMissfSum_CLS2; // ���ٷ����Ե���ʧ����������������(��Ϊ��)�
extern byte vc_IdleSc_CLS2;       // ���ٷ����Ե����Զ���������������(��Ϊ��)   
//extern byte m_o2_mx	;//	4ah					;o2������Ŀ��ֵ	//2008-5-29 set as public for communicate
//extern byte v_Nb_osChange;     //2008-11-17  add 
extern byte numb_fk_m;//NUMB_FK_M;

extern void F_SL_idleIni(sbyte sl,byte blnOK);
extern void FR_idleClsAve(sbyte cls);

extern sbyte vc_CLS_2;            //same	as	leansdti //���ٷ���������
extern sbyte vc_CLS_1;            //O2��������������AT	v_O2_ad	FEEDBACKCONTROL,MODI TINJ	BY + vc_CLS_1		
extern sbyte leansdtav	;//	1e2h				
