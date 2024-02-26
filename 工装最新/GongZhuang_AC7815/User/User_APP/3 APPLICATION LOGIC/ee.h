
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
/*====定义EE的地址=====================*/
/*===========================================================*/
#define EE_BASE_ADR     ATC7811MapAddr//0x00000400

//extern const word ee_w[3000] @ EE_BASE_ADR;

//2014-3-25 add 
//功能：从EE中读取数据 ;适用于单片机自身对EE操作的场合                 
extern byte Load_Data_EE(word adr);//返回结果是对应EE地址中的数据

//功能：将数据保存到EE中 ;适用于单片机自身对EE操作的场合           
extern byte Save_Data_EE(word adr,byte data);//返回结果是否保存成功

//功能：判断控制位，并在条件合适时，向EE或FLASH写入数据  ;适用于上位机操作的场合
extern void write_eeprom(byte Kesei,word Remark, byte BTDC,byte BACK,byte Data);

//功能：判断控制位，并在条件合适时，从EE或FLASH读取数据  ;适用于上位机操作的场合
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

