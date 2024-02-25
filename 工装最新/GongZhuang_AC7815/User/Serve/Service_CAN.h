

#ifndef __SERVICE_CAN_H__
#define __SERVICE_CAN_H__
#include "FAM.h"
#define CAN_EXTENDED_FRAME_ID           0x80000000UL       /* Extended message ID identifier */

byte CAN_SendFrameExt(dword MessageID,byte FrameType,byte Length,byte *Data);
byte CAN_ReadFrame(dword *MessageID,byte *FrameType,byte *FrameFormat,byte *Length,byte *Data);
byte Is_can_ok(void);
void CAN_Init(void);
void Close_Can(void);

#endif




