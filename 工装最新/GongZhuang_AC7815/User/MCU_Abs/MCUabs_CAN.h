

#ifndef __MCUABS_CAN_H__
#define __MCUABS_CAN_H__
#include "FAM.h"

#define M_CAN1En		1//1:can1使能
#define M_CAN2En		1//1:can2使能



#define CAN1_RECV_DATA_ID1			(0x220)
#define CAN1_RECV_DATA_ID2			(0x230)
#define CAN1_SEND_DATA_ID1			(0x430)

#define CAN1_RECV_DATA_ID11			(0x18DFF930)
#define CAN1_RECV_DATA_ID12			(0x18BFF930)
#define CAN1_SEND_DATA_ID11			(0x18DEE050)

#define CAN1_TX									(GPIO_PC10)
#define CAN1_RX									(GPIO_PC11)
//#define CAN1_STB								(GPIO_PB0)

//#define CAN1_TRASCVER_NML				do{GPIO_ResetPinBits(CAN1_STB);}while(0)
//#define CAN1_TRASCVER_SLP				do{GPIO_SetPinBits(CAN1_STB);}while(0)
#define CAN1CtrlPinST						(GPIOB_IN(2))

/*************<enum>*******************/
typedef enum
{
	CAN_IDE_STD = 0,
	CAN_IDE_EXT
}CAN_IDE_TypeDef;

typedef enum
{
	CAN_RTR_STD = 0,
	CAN_RTR_RMT
}CAN_RTR_TypeDef;

typedef enum
{
	CAN_SEND_MSG_FILED = 0,
	CAN_SEND_MSG_SUCCS
}CAN_SendSts_TypeDef;

typedef enum
{
	FILTER_IDE_STD_ONLY 	= 0,
	FILTER_IDE_EXT_ONLY 	= 1,
	FILTER_IDE_STD_EXT_BOTH	= 2
}CAN_FilterMask_TypeDef;

typedef enum
{
	CAN_FILTER_CODE_MODE	= 0,
	CAN_FILTER_MASK_MODE	= 1,
}CAN_FilterMode_TypeDef;



extern byte F_CANTxEvent;
extern CAN_MSG_INFO 			g_sendCANMsgInfo;
extern CAN_MSG_INFO 			g_recvCANMsgInfo[3];


void FUN_CAN_InitHardwr(void);
void CAN_InitVar(void);
void FUN_CAN_APP_Scheduler(void);
CAN_SendSts_TypeDef CAN_SendMsg(const CAN_MSG_INFO * g_sendCANMsgInfo);
int32_t CAN1_IRQnCallBack(uint32_t event, uint32_t wparam, uint32_t lparam);
void FUN_CANRxDataResolver(void);
void FUN_CANTxDataPro(void);
void FUN_SetBootVar(void);


void FUN_LOOKUPDebug(void);
void CAN_OnFullRxBuffer(void);
sword F_2DLookUp(byte x,const unsigned char p[][2],byte ub,byte xM4,byte blnM4);
sword FG_3DMap_xyz(byte * Z_3D,sword x,sword y,byte X_A[],byte Y_A[],byte ub_x,byte ub_y,byte blnM2);

#endif




