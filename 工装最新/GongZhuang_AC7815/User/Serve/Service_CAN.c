
#include "FAM.h"
#include "Service_CAN.h"

byte CAN_ReadFrame(dword *MessageID,byte *FrameType,byte *FrameFormat,byte *Length,byte *Data)
{
		*MessageID 		= g_recvCANMsgInfo[0].ID;
		*FrameFormat 	= g_recvCANMsgInfo[0].IDE;
		*Length				= g_recvCANMsgInfo[0].DLC;
		for (byte i = 0; i < (*Length); i++)
		{
			Data[i] = g_recvCANMsgInfo[0].Data[i];
		}
		return 0;
}



byte CAN_SendFrameExt(dword MessageID,byte FrameType,byte Length,byte *Data)
{
		byte RT = 0;
		if (MessageID & CAN_EXTENDED_FRAME_ID) /* Is it the extended frame? */
		{
				g_sendCANMsgInfo.ID = (MessageID & (~CAN_EXTENDED_FRAME_ID));
				g_sendCANMsgInfo.IDE = CAN_IDE_EXT;
		}
		else
		{
				g_sendCANMsgInfo.ID = MessageID;
				g_sendCANMsgInfo.IDE = CAN_IDE_STD;
		}
	
		g_sendCANMsgInfo.RTR = CAN_RTR_STD;
		g_sendCANMsgInfo.DLC = Length;
		g_sendCANMsgInfo.Data[0] = Data[0];
		g_sendCANMsgInfo.Data[1] = Data[1];
		g_sendCANMsgInfo.Data[2] = Data[2];
		g_sendCANMsgInfo.Data[3] = Data[3];
		g_sendCANMsgInfo.Data[4] = Data[4];
		g_sendCANMsgInfo.Data[5] = Data[5];
		g_sendCANMsgInfo.Data[6] = Data[6];
		g_sendCANMsgInfo.Data[7] = Data[7];
		RT = CAN_SendMsg((const CAN_MSG_INFO *)&g_sendCANMsgInfo);
		return RT;
}



byte Is_can_ok(void)
{
		//return CAN1->BIT.BUSOFF;
	if(CAN1CtrlPinST == 1)//(CAN1CtrlPinST == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void CAN_Init(void)
{
		FUN_CAN_InitHardwr();
}


void Close_Can(void)
{
		//CAN_Uninitialize(CAN1);
//		CAN1_TRASCVER_SLP;
		CAN_Uninitialize(CAN1);
}


