

extern void Save_MissfireNotIdle(void);  
extern void Detect_MissfireNotIdle(sbyte fmf_trans1,byte remark,word rev_15d6,sbyte MissfireNotidle_IdleSC);
extern void Init_MissfireNotIdle_afterReset(void);
extern word CO_FMF_MissfireNotIdle(sword fmf,byte tps_odr);

extern sbyte v_MissfireNotIdle;        //�൱������ʦԭ�ȵ�modi_fachong, ֻ������ֵ��Ϊ0��
extern sbyte v_FMF_MissfireNotIdle;    //������������ϵ����