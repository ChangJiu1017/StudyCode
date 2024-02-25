
#include "PE_Types.h"
#include "derivative.h"	

extern void Reset_SCI_afterSettipc(void);
extern word Addr_Mul_two(word addr);
extern byte EE_PROM_1word(word addr,word data);
extern byte EE_ERASE_1word(word addr);
extern byte EE_PROM_ALL(word *ptr,byte nn);
extern byte EE_ERAZ_ALL(byte nn);
extern word zzq_ee[30];
/*===========================================================*/
/*====����EE�ĵ�ַ=====================*/
/*===========================================================*/
#define EE_BASE_ADR     ATC7811MapAddr//0x00000400

//extern const word ee_w[3000] @ EE_BASE_ADR;

//2014-3-25 add 
//���ܣ���EE�ж�ȡ���� ;�����ڵ�Ƭ�������EE�����ĳ���                 
extern byte Load_Data_EE(word adr);//���ؽ���Ƕ�ӦEE��ַ�е�����

//���ܣ������ݱ��浽EE�� ;�����ڵ�Ƭ�������EE�����ĳ���           
extern byte Save_Data_EE(word adr,byte data);//���ؽ���Ƿ񱣴�ɹ�

//���ܣ��жϿ���λ��������������ʱ����EE��FLASHд������  ;��������λ�������ĳ���
extern void write_eeprom(byte Kesei,word Remark, byte BTDC,byte BACK,byte Data);

//���ܣ��жϿ���λ��������������ʱ����EE��FLASH��ȡ����  ;��������λ�������ĳ���
extern byte show_eeprom(byte Kesei,word Remark, byte BTDC,byte BACK,byte data);

extern byte Load_D_DK(word add,byte bEqual);//2014-10-6

extern void Init_EE_afterReset(void);

extern byte rdrive_iic(word adr);//2014-3-30 add 
extern void wdrive_iic(word adr,byte dat);

extern byte rdrive_iic_ext(word adr);//2014-3-30 add 
extern void wdrive_iic_ext(word adr,byte dat);

extern void CheckWriting(void);
extern void CheckWriting_ext(void);

extern void Back_EE_fromExt(void);

