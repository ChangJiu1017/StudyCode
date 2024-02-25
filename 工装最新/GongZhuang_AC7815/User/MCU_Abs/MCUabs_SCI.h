

#ifndef __SCI_H__
#define __SCI_H__
#include "FAM.h"

#define M_TxSize								60

#define UART6_RX_PIN						(GPIO_PB14)      //串口2
#define UART6_TX_PIN						(GPIO_PB13)

#define UART2_RX_PIN						(GPIO_PA8)//(GPIO_PB6)  //串口1
#define UART2_TX_PIN						(GPIO_PA7)//(GPIO_PB5)
#define UART_BAND_RATE_SEL			(19200)//波特率选择
#define UART_RxDATA_LEN_SEL			8
#define UART_TxDATA_LEN_SEL			8

extern byte F_SCITxEvent;
extern word Tx_timeOk;
extern byte * OutPtrR;
extern byte * OutPtrW;
extern byte TempTxBuffer[M_TxSize];
extern byte TxSub;
extern byte TxSub_Int;
extern byte SCIOutLen;

void FUN_UART_InitHardwr(void);
void SCI_InitVar(void);
byte FUN_UART_SendString(const uint8_t *pDataBuf, word dataLen, word *Snd);
byte FUN_UART_SendStringWait(const uint8_t *pDataBuf, word dataLen, word *Snd);
void FUN_SCI_APP_Scheduler(void);
int32_t UART_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1);
int32_t UARTLine_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1);

void TR_SCI0_OnRxChar(void);


#endif





















