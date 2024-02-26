

#ifndef __SERVICE_SCI_H__
#define __SERVICE_SCI_H__
#include "FAM.h"


byte FUN_UART_SendString(const uint8_t *pDataBuf, word dataLen, word *Snd);

#define TR_SCI0_SendBlock						FUN_UART_SendString


#define SCI0SR1_TDRE								UART_TxIsEmpty(UART2)
#define SCI0CR2_RIE(x)									UART_SetRxIntEn(UART2, x);




#endif




