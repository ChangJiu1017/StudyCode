
#include "Service_SCI.h"


byte * OutPtrR;
byte * OutPtrW;
byte TempTxBuffer[M_TxSize];
byte TxSub;
byte TxSub_Int;
byte SCIOutLen;

//(void)TR_SCI0_SendBlock(TX_DATA, 28, &w);
//byte TR_SCI0_SendBlock(byte *Ptr, word Size, word *Snd)

void FUN_InitSCIService(void)
{
	TxSub = 0;
	TxSub_Int = 0;
	SCIOutLen = 0;
	OutPtrR = &TempTxBuffer[0];
	OutPtrW = &TempTxBuffer[0];
}


byte FUN_UART_SendString(const uint8_t *Ptr, word Size, word *Snd)
{
		word count = 0U;
#if 1
		if(TxSub == 0)
		{
				while((count < Size) && (count < M_TxSize))
				{
						TempTxBuffer[count] = Ptr[count];
						count++;
						TxSub++;
						if(count == Size)
						{
								SCIOutLen = Size;
								if(UART2->UARTn_IER.ETXE==0)
								{
										//UART2->UARTn_RBR_THR = TempTxBuffer[0];//(byte)*(OutPtrR++);
										UART2->UARTn_IER.ETXE = 1;
								}
						}
				}
		}
#endif
}











