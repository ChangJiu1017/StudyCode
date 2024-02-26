/*===========================================================*/
/*====���䵥Ԫ����ģ��=====================*/
/*===========================================================*/

extern void FI_IDLE_T3FK(void);//2012-12-25 

//���ܣ���λ�����Ͳ����ĳ�ʼ��
extern void Init_InjPara_afterReset(void);//������Ҫ���� Check_TW1SCI_afterRDorWR��

//���ܣ�STARTSǰ���Ͳ����ĳ�ʼ��
extern void Init_InjPara_beforeStarts(void);

//*******SET_T1_AS_T35GET***************************
//���ܣ�������T35���Ե�T1����TINJH,L
//����ֵ��T35����ʱʹ�õ�T1;
extern word Set_TW1_forT35Get(void);

//���ܣ���������ֹʱ������T35���ԵĴ�����
//����ֵ��T35���ԵĴ���
extern byte Set_NB_forT35Get(void);

//***tnooil_to_ti*****SET_T1_FOR_PUMP******************
//���ܣ��������T1;
//  ������T1����TINJH,L:no oil T1=inj_shift+t3fk0+modi_dcdy
//�βΣ� vbat_odr������ص�ѹ����vbat_mode����ص�ѹ������?
//����ֵ������T1
//  vbat_odr--��ѹ����
//  vbat_mode--��ѹ������
extern word Set_TW1_forPump(byte vbat) ;

//*******SET_TIMES_FOR_PUMP*****************************
//���ܣ����ñ��͵Ĵ�����
//����ֵ�����͵Ĵ���
extern byte Set_NB_forPump(void);

//**********cal_tv3_temp***************************************
//���ܣ�����T35���Խ������õ�T35��TW3������ϵ��
//  calcute T35, and get the rec_co for FLUX correction;
//  cal_tv3_temp:
extern void Cal_T35(sbyte tps_ddt,word tw3,byte vbat,word tw1_synwith_tw3,word tw4,word tw4_2nd,word nb_inj_forDrv);
                                                                                                        //2008-6-30 add tw1
//���ܣ���ʼ��T1T3���Ե���ʼ��
extern void Init_T1Scaning(byte vbat,byte blnIni);//2008-6-25

//���ܣ����T1-T3���Ե��ܴ�����
extern word Get_NB_T1SCANING(void);

//�Ƿ���Ҫ�ڿ�Կ�׺󣬽���T1ɨ�裻//2008-12-1
extern byte B_T1_Scaning_Needed(void) ;

//���ܣ��趨T1ɨ���Ƶ�ʣ���λ4us//Ƶ���趨
extern word Interval_For_T1_Scaning(void);

//���ܣ�
//      ��õ�ǰɨ���õ�TW1����λ4us
extern word Get_TW1_for_Scaning(void);//

extern word Set_TW1_byDT3mode(sword fmf,byte vbat);       //2008-7-25 add

//extern sbyte Cal_dtTw1_bydVbat(byte vbatCal,byte vbatbTrig);

extern word Ctl_DT3(word fmf,byte vbat,
            sbyte vbat_dt,word tw3,byte tps_odr,byte tps_mode,
            byte rev_156ave,byte teng,word tw1p,word pad_cas);
                  //2009-1-17 add 
extern word check_t3ers(word tw3,byte vbat);               //2009-2-19 ����ֵΪ�������T3

extern void SaveGama_AfterKeyoff(void);

extern byte Check_GamaValid(byte status);

extern sword Limit_TW1(word prd_cas,sword tw1,byte vbat);  //2012-10-19


extern void Init_PN_pp(void);

extern void SaveDtw3Obj(void);//2014-10-16

//-----declearation of public variable--------------------------
extern sword vc_tw1_t3FK_P;         //T3��������ֵ����������Ȧ��T3�ͱ�ȦT3Ŀ��ֵ������õ���T1������ϵ����  ע����vc_tw1_t3fk������
extern sbyte v_vbat_dt_TW3FK;       //TW3���������VBAT_DT������ϵ����
extern sword vc_tw1_t3FK_I;
extern sword v_dt1;

extern word v_dtw3_obj;//dt3 ��Ŀ��ֵ����T3��ȥ���ܺ�ʣ�µ�ֵ��
extern word v_dtw3;            //����õ���DTW3��

extern word v_dtw3_obj_lst;//2008-9-24 add

extern sword av_dti_t3fk;//	equ	0x1e5	;��ת�е���ƫ����2��ƽ��ֵ,�ص���ʱ����Ҫ��T3����ֵ�����ֵ��	

extern sword v_T3b;             //2008-11-27 add for disp
extern word v_T1d;             //2009-1-16 

//2009-4-18 
//��ʱ����
extern byte nb_T35T;
extern sword vc_tw1_t3fk_now;          //save the ti_offset at	idle set by	user��ƫ����2��ǰֵ

//extern byte status_T4Scan;  //T4ɨ���״̬,����ǰ��λ��;
extern byte nb_VbatFlu;        //t4ɨ�������,2009-11-28 ������ѹ�����Ĵ���;

extern sword v_T3d;   //2010-1-11
extern byte v_Vbat_for_gama;//2010-1-11
extern sword v_gama_new;//2010-1-11
extern word v_T4_ave;//2010-1-14
extern sword v_gama0;
extern sword v_gama_tmp;
extern sword v_gama_inuse;
extern sbyte v_gama_delta_div32;//���α���Ϊ�ɿ���v_gama_new-��һ��v_gama_inuse��

extern sbyte v_dDt3;//2010-1-29

//extern byte nb_inj_lst;//byte nb_inj_lst=v_NB_INJDRV  //2010-1-13 add 
extern sbyte v_DT1ToDT3_Cur;
extern word v_T3_scan_lst;
extern byte v_nb_T1Scan;   //����T1����
extern byte v_nb_T1di;//ɨ����T1Di�Ĵ�����
extern byte v_nb_AveScanT3Fail;
extern word v_T1i;//��ʼ������ʼt1

extern byte v_Point_T4Dec;
extern byte v_Point_T4Asc;
extern byte bInjFlag;
extern word v_Tw4_ScanArray[60]; //����T1-T3��ϵʱ���洢16�����ƽ��T4ֵ
extern byte v_nb_T1Scan;   // v_Tw4_ScanArray [v_nb_T1Scan];
extern sbyte v_nb_T1for1Point;
extern word v_Tw4_CurTw1[8]; //ÿ��T1����16���ε����ݴ��
extern byte v_nb_T4Exist;//T4���ڵĴ���
extern word v_T4_inInjctl;//2010-3-6 tmp
extern byte v_Status_T4Dec; //bitx: 1:�½��� 0��δ�½���
extern byte v_Status_NotDecMore1;//�½�������1����λ   //v_Status_NotDecMore1  v_Status_T4Dec

extern byte v_ChaPoint_pre;    //T4��������֮ǰ�����һ�������㣻
extern word v_T3d_ini;//t3d��Ӧ��ԭʼ��t3�źţ�
extern byte v_nb_ScanT4T1fail; //ɨ��T1T4��������Ĵ���

extern byte nb_Piston_bubble;    //ͬһ��T1����ʱ����ѹǻ�����ݵĴ�����
extern sword v_gama_delta_T1d;//2010-1-11
extern word v_T3b0_T1d0;
extern word v_T1d0;
extern byte nb_T3Bubble;//2010-5-22

//2013-3-23
extern byte v_fmf_pump;
extern byte  nb_T3Fk;
extern byte  nb_T3Bubble;

extern byte  v_Vbat_whenT4_min;
extern sword vc_tw1_t3FK_I; 
extern word v_dtw3_obj_lst;


extern byte no_t3_fk_run;//	equ	0x0e	;��T3�����Ĵ�������ĳЩ�����£���Ҫ��ֹT3������no_t3_fk_run��ֵ������ֹ�Ĵ�����	

extern byte v_sta_T3_LT;//T3������״̬��T3С��Ŀ��ʱ��λ1

extern byte vc_tw3_t35;

#define nb_pump  nb_T3Fk
#define nb_pump_constdt3  nb_T3Bubble

#define vbat_lst  v_Vbat_whenT4_min
//#define v_dt1     vc_tw1_t3FK_I  2017-3-28 ɾ��
#define v_dtw3_lst v_dtw3_obj_lst

extern byte v_DTC_T4ScanFail;//2015-12-17 t4ɨ�����Ĺ���ָʾ


extern byte v_nb_T30;//2015-8-28 add 
/*  
    2.0     0.18
    1.3     0.14
    1.8     0.135
    2.0     0.135
*/
extern byte v_nb_pump_limit;// 2016-4-28

//extern byte v_fmftot1a;
extern word Ctl_T1_TRAD(word fmf,byte vbat);

