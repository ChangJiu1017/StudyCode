

extern void FI_IdleAQ_BeforeStarts(void);//2008-12-8
extern void F_SearchIdleSpeed(word rev156,byte teng,byte fmf_starts,
                byte rev_156ave,byte c_IdleSc_CLS2,sbyte ignca) ;
extern void FI_IdleAQ_AfterReset(void);//2009-8-25 add 


extern sword vc_tw1_IDLESC_inuse;//��������ʱ�����������Ʋ�������ʱ������
extern sword vc_tw1_IDLESC;      //����������õ���ƫ����ֵ��

extern sbyte vc_MissfireNotidle_IdleSC;//��������ֵ�Է���ָ����������//2008-11-5

//extern byte for_blnaq ;
//extern byte av_rpm7D8_64r;
extern byte number_sced;
extern byte for_temp;
extern byte rpm_testMode;
extern byte Mode1_Time;
extern byte Mode4_time;

//2009-11-25
//extern sbyte n_aq[3];//0:���ϴΣ�1���ϴΣ�2������
//extern byte n_aq_ave[2];//0:shangci,1:benci ;
//extern byte n_aq_ave_max[2];//1:max, 0:2nd max;
extern sbyte o_aq_ave_max[2];//1:max n, 0:2nd max n;
extern sbyte o_aq_for_maxn[3];//���δ�ɨ�Ľ���� 0:���ϴΣ�1���ϴΣ�2������
//extern sbyte n_aq_min;
//extern sbyte n_aq_max;

//2009-12-24 chang to 
extern sword n_aq[3];//0:���ϴΣ�1���ϴΣ�2������
extern word n_aq_ave[2];//0:shangci,1:benci ;
extern word n_aq_ave_max[2];//1:max, 0:2nd max;
extern sword n_aq_min;
extern sword n_aq_max;
extern sbyte ign_aq_ave[3];//0:shangci,1:benci ;
extern sbyte ign_aq[3];//2010-1-26
extern sword drpm_64_32;//	equ	0x1dc									
extern sword drpm_32_0;//	equ	0x1dd	
extern sbyte v_dO_enlean;  //2010-3-24 add    ��ϡ����ʱ��С��oֵ
