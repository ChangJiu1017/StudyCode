/*===========================================================*/
/*====TPS����ģ��=====================*/
/*===========================================================*/


//���ܣ���λ��TPS�����ĳ�ʼ��
extern void FI_TPS_AfterReset(void);

//���ܣ�����AD�ɼ��������TPS�����������仯����
//      Ѱ�ҵ������λ�ö�Ӧ��TPS����ֵ��
//      ���TPS;
//      ��TPS����ʱ������ת��ֵԤ��TPS���������г���
//�βΣ�rev156--v_rev_15d6rpm;
extern void FG_TPS(word rev156);

extern void Check_BOS(void) ;

extern void FS_TPSlst(void);//2014-4-6

extern void SaveADForM(word tps);//2015-4-9

//----------------------------------------------------------------------------
extern byte v_TPS;     //�ϴ�TPS����ֵƽ������ĸ�8BITS  		
extern sbyte v_TPS_ddt;        //TPS������	
extern byte v_TPS_odr;         //TPS����  	
extern byte v_TPS_mode;        //TPS������

//2010-5-11
extern byte v_TPS0_min;
extern byte v_TPS0_AD10	;//	5AH					;������λ�õ�0�㣻MEMORY	FOR	SAVE	JQMWZ0	
extern byte t_1s_NIncTps;
extern byte nb_BH;                           
extern byte nb_BH1;                           

extern byte v_TPd;//���ż���2012-8-20
extern byte v_TPd_lst;//���ż���2012-11-22
extern byte v_TPd_lst2;//���ż���2012-11-22

extern byte v_TPS_lst; //2012-11-14
extern word v_TPS_AD10;

extern word v_TPS0_256sum;

//extern byte nb_tps_back;  //2013-4-8
//extern byte v_TPS0_min_back;
extern byte v_bTPS_AD10_lst;

extern word v_TPS_16sum;//2013-4-9
extern byte v_TPS0_ub;

extern byte v_TPS_bTrans;       //��һ��JQMWZ����ֵ�ĸ�8λ		

#define M0_ETC_TPS_MIN 25
#define M0_ETC_TPS_MAX 200
extern byte v_T_percent;//2016-3-2 ���Ӹ��ɰٷֱ�

extern byte v_TPd_lst_in1cycle;
extern byte v_TPd_lst2_in1cycle;

extern byte nb_leavidle;
