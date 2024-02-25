
#include "sci.h"
//2013-6-14
extern void OperLock(void);
extern void ActionWhenKeyoff(void);
extern void InitLockWhenKeyON(void);

extern void ActionAtKeyoff(void);

extern void ClrActive(void);//2014-5-8 add

extern void SetPwDefault(word ADD_PW);//2014-5-31 add 

//
#define REMARK_CLRACT 251
#define KESEI_CLRACT  -6

//2013-9-3 �������ص�
#define FRAME_B0 0xa0
#define FRAME_B1 0xa2

//TPSλ�ã����ص�״̬λ�ĺ���
#define CMD_LOCK_SET      0xf1//��������  
#define CMD_CANCELLOCK_SET    0xf2//ȡ������
#define CMD_UNLOCK_SET    0xf3//����  

#define CMD_INPUT_PW_S      0xf6//�����û�����  
#define CMD_SET_PW_S      0xf8//д����������û�����  


#define CMD_DISP_PWS_SET    0xfe//��ʾ�û�����
#define CMD_DELOCK_SET    0xff//ǿ��ȡ������

#define CMD_LIMIT_SET     0xe2  //������Ȩʹ��
#define CMD_CANCELLIMIT   0xe3  //ȡ����Ȩʹ��
#define CMD_INPUT_TIME    0xe4  //���뵱ǰʱ��
#define CMD_SET_TIME          0xe5     //������Ȩʱ��
#define CMD_QUERY_TIME        0xe7   //��ѯ��Ȩʱ��
//#define CMD_ED_TIME       0xe6  //������Ȩ����ʱ��

#define CMD_QUERY_CUR_TIME        0xe9   //��ѯ��ǰʱ�䣬2014-5-31

#define CMD_INPUT_PW_L      0xE6//���뾭��������  
#define CMD_SET_PW_L      0xE8//д��������ľ���������  

#define CMD_DISP_PWL_SET  0xee//��ʾ����������
#define CMD_DE_PWL_SET    0xef//ǿ��ȡ������������


#define CMD_LOCKMODE_IN   0xb0  //�������ͨѶģʽ
#define CMD_LOCKMODE_OUT  0xc0  //�˳�����ͨѶģʽ

#define CMD_LEN_ERR 0x11     //֡������
#define CMD_SUM_ERR 0x12     //У������

#define CMD_LEN_ERR2 0x13     //֡������
#define CMD_SUM_ERR2 0x14     //У������

#define CMD_LOG_ERR 0x15     //ECU�����������߼�����
#define CMD_LOG_ERR2 0x16     //ECU�����������߼�����

#define CMD_INJON_ERR 0x17     //�������ڹ���

#define CMD_REMARK_ADJ  0xa0
#define CMD_KESEI_ADJ   0xa1
#define CMD_IGN_ADJ     0xa2
#define CMD_T1_ADJ      0xa3
#define CMD_DTC_ADJ     0xa4

#define ASC_0 48
#define ASC_9 57


#define OS_LOCK     254
#define TES_LOCK    253
#define REMARK_LOCK 32

#define T_FRAMEBREAK_4US 2500//5ms   //2013-9-10 FROM 125 TO 1250


#define LOCK_ENBALE true    //2013-9-16

//-------------2013-9-3 add --------------------------------
extern byte u_code;
//extern byte u_code_lst;
extern byte cmdRX[2][BYTE_MAX];
extern byte v_index_rx_tmp;
///v_index_rx_tmp: 0 ��ʾcmdRX[0][30]���Խ�������;1 ��ʾcmdRX[1][30]���Խ�������;��Ӧ��һ���������ڱ���������;
extern byte v_num_rx_tmp;    //��ǰ���ջ�����
extern byte v_num_rx_decode;  //��Ҫ�����������ڵ��ֽڵĸ���

extern word t_rx_lst_4us;    //��һ�ν��յ��������ݵ�ʱ��
extern byte v_sta_cmd;       //����ͨѶģʽ�µ�״̬

extern byte v_nb_rx;//���յ�������
extern byte v_bchk;//2013-9-10

extern byte v_pw[6];//����

extern byte v_t_run_s;//һ���ϵ���ۼƵ�Կ�׿�����ʱ��,2014-4-27

//2014-3-27
//#define EE_ADR_PWS_HEAD 0x300// �û������׵�ַ��6���ֽڰ�˳���׵�ַ+16��ÿ���ֽ�������5�������׸�Ϊ����ASC��ֵ�������һ��-1��


