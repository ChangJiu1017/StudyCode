/*===========================================================*/
/*====�¶ȴ���ģ��=====================*/
/*===========================================================*/

//���ܣ����ݿ����ͷ������¶Ȳ���ֵ������¶��ź��Ƿ����󣬲���ÿ����ͷ������¶ȵ�ʹ��ֵ��
//�βΣadres_tair_8bit���������¶�8λ�ɼ�����
//�βΣadres_teng_8bit�����������¶�8λ�ɼ����
extern void FG_TMP(word adres_tair_10bit,word adres_teng_10bit,word rev,word fmf,byte tpd);


//���ܣ��������¶�Ԥ��ֵtamp_yc���浽��ַee_Tamp_save��ee_Tamp_save+1��EE�У�
//      ��ͣ����ر�Կ��ʱ�����ã�
extern void FM_TMP_air_StopOrKeyoff(void);

extern void FI_TMP_AfterReset(void);//2008-10-6 add

extern void Attu_TMP(void);//�¶�Ԥ��ֵ��˥��

extern void FI_TMP_beforeStart(void);

extern void ResetTwParaWhenKeyonOroff(void);//2016-4-4   

//------public variables--------------------------------------------        				
//#pragma DATA_SEG CAB_RAM			
//#pragma DATA_SEG DEFAULT

extern   byte v_tair_inuse ;   //�����¶�ʹ��ֵ
//extern byte v_teng_odr;     //��ͷ�¶ȼ���     			
extern byte v_teng_inuse;   //��ͷ�¶�ʹ��ֵ     
extern byte v_teng_odrh5;   //��ͷ�¶ȵĸ�5bits

//extern byte RZT_1st_afterKeyOn;//2008-11-6 ��Կ�׺��õĵ�һ��RZT����ֵ�����ڸ�ͷ�¶�Ԥ�⣻
extern byte tamp_yc_last;	//equ	0x46	    //;�ϴδ���RZTԤ��Ŀ����¶ȣ���EE�ж�ȡ��		

extern sword dtw0;

extern long int v_tao_M16;
extern long int v_sigma_M2048;
extern sword v_sq;//2013-1-21

extern sword tw_stop;//2013-1-21
extern long int v_Tg_Sum256;

extern sword v_twb;//2013-3-26 add


extern byte v_R_d4;//2014-10-8 MOVE 

#define N0_62D5 25//2014-2-12 1500RPM


extern byte v_Gx_M128;//ʵ�ʵ�G  //2015-2-3
 

///////////////////////////////////////////////////////////
#define M0_N0_D16 1516//1500rpm
//#define BYPTR_M0_IBP			25
//#define M0_m0 100  2014-10-8 delete

#define M0_T_UC 50    //2013-3-3 move here
#define M0_TW_FORINI      (M0_T_UC+10)
#define M0_TW_FORENRICH   (M0_T_UC+30)

//20211014 chenwei add
extern sword v_te_keyon;
extern sbyte V_Ta_keyon;