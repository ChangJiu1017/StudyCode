/*===========================================================*/
/*====��������ģ��=====================*/
/*===========================================================*/
//���ܣ�STARTSǰ�����ɹ��������ĳ�ʼ����
extern void FI_Trans_BeforeStarts(void);

//���ܣ��ȴ�CAʱ����������з��б仯���Ծ�����������
//����֮ǰ��ȡTPS������
//����ֵ��  0--�޼����Ŷ�����
//          1--�м����Ŷ�������Ԥ������Ͷ�������Ҫ����������������
//          2--�м����Ŷ�������Ԥ������Ͷ�����ȡ��T35�����������磻
//�βΣ�tps_ad_10bit--10bit ad �ɼ����
//      teng--�������¶�
//      tps_odr--tps����
//      lst_tps_adh8--�ϴβ���ֵ�ĸ�8λ��
//      lst_prd_cas--�ϴνǱ����ڣ�
//extern byte F_Accel_BeforeCACome(word tps_ad_10bit,byte teng,byte tps_odr,byte lst_tps_adh8,byte lst_prd_cas);//����ֵ���壺0�����ͣ�1����

//���ܣ����JQM������������ϵ��ACCELDT,ACCELDT_FIRST,vc_TPS_TRANS
//      ע����ΪҪ�õ�VF_JCI_INJ��ǣ�������Ҫ��CI_CO����
//      ע����Ϊ�õ�������״̬�ı�ǣ�������Ҫ��Get_Rev���㣬Get_TPS���㣬�� Get_Status_Eng֮��
//�βΣ�tps_ddt--v_tps_ddt
//      ka_b1--��������������ϵ��������Ĥ�й�
//      prd_cash
//      teng
extern void F_Trans_FMF(sbyte tps_ddt);

extern void FI_Trans_AfterReset(void);   //2009-1-13

//------------public variables------------------------------------------


extern sword v_TPS_addt;             //��Ҫ����ʱ��TPS����;�Կ�Ҫ��������ǰ��T1Ҫ������������Accel_BeforeCACome�õ� 

extern byte v_nb_binj; ///2012-11-24


//ע�ͣ�
//    ��������ʱ��������������
//        vc_FMF_trans1��vc_FMF_trans2��vc_TW1_trans2
//        �Լ���������뿪���٣�vc_FMFremain_leaveidle
//        �Լ������CAδ����/��������֮ǰ�����⵽���������� v_TPS_addt
//    ��������ʱ��������������
//        vc_FMF_trans1��vc_TW1_trans2
