/*===========================================================*/
/*====ת�ٴ���ģ��=====================*/
/*===========================================================*/

//-------------functions----------------------------------
//���ܣ����ת�ټ���仯���ɣ���λ�жϣ�����ʱת��ƽ����
//�βΣ�prd_cas����ǰ�ؽǱ�����
//extern void FG_Rev_BeforeIgn(word prd_cas);

//���ܣ�����Get_Rev_BeforeIgn��û�м���ת�٣�����㣬��̼��
//�βΣ�prd_cas����ǰ�ؽǱ�����
//extern void FG_Rev_AfterIgn(word prd_cas) ;

//���ܣ���λ��ת�ٺͽǱ�����ĳ�ʼ��
extern void FI_Rev_AfterReset(void);//

extern void FG_Rev(word prd_cas);//2014-3-31

extern void FC_DueMain(void);
//-------------variables----------------------------------
extern word vlst_prd_cas;   //���������Ǳ���ʱ���������Ǳ����� 	
extern word v_prd_cas_min;  //��һȦ�Ǳ����ڵ���С��ֵ����vlst_prd_cas����õ�
extern word v_rev_15d6rpm;  //ת��rpm[���� ��15.6RPM];
extern word v_rev_3d9;//2010-1-25

extern byte v_rev_deg;//ת�ټ���  2013-1-4
extern byte v_rev_625rpm;        //save	RPM	using	62.5 unit

extern word v_t_CAHY_timeout;//