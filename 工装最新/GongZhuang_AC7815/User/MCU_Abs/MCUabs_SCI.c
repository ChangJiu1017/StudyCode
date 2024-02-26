
#include "MCUabs_SCI.h"

byte F_SCITxEvent;
word Tx_timeOk;
byte g_recvedDataRdy;
byte g_recvDataTtlLen;
byte g_sendDataBuf[UART_RxDATA_LEN_SEL] = {0x11, 0x22, 0x33, 0x44, 0x12, 0x22, 0x32, 0x42};
byte g_recvedDataBuf[UART_TxDATA_LEN_SEL];


/**
* @prototype FUN_UART_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize UART module.
*			 初始化UART模块.
*/
void FUN_UART_InitHardwr(void)
{
	UART_SettingType uartSetting;
	
	GPIO_SetFunc(UART2_RX_PIN, GPIO_FUNC_1);
	GPIO_SetFunc(UART2_TX_PIN, GPIO_FUNC_1);
	
	uartSetting.baudrate = UART_BAND_RATE_SEL;//设置波特率
	uartSetting.dataBits = 8;
	uartSetting.stopBits = 1;
	uartSetting.parity	 = 0;
	uartSetting.fifoByte = ENABLE;//使能缓冲区
	uartSetting.dmaEn 	 = DISABLE;
	UART_Init(UART2, &uartSetting);//初始化串口2
	
	UART_SetEventCallback(UART2, UART_IRQHandler);
	UART_SetRxIntEn(UART2, ENABLE);//使能接收中断
	//UART_SetTxIntEn(UART2, DISABLE/*DISABLE*/);//使能接收中断
	
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
  NVIC_SetPriority(UART2_IRQn, 1);
	
	
	
	GPIO_SetFunc(UART6_RX_PIN, GPIO_FUNC_2);
	GPIO_SetFunc(UART6_TX_PIN, GPIO_FUNC_2);
	
	uartSetting.baudrate = UART_BAND_RATE_SEL;//设置波特率
	uartSetting.dataBits = 8;
	uartSetting.stopBits = 1;
	uartSetting.parity	 = 0;
	uartSetting.fifoByte = ENABLE;//使能缓冲区
	uartSetting.dmaEn 	 = DISABLE;
	UART_Init(UART6, &uartSetting);//初始化串口6
	
	UART_SetEventCallback(UART6, UARTLine_IRQHandler);
	//UART_SetRxIntEn(UART6, ENABLE);//使能接收中断
	
	//NVIC_ClearPendingIRQ(UART6_IRQn);
	//NVIC_EnableIRQ(UART6_IRQn);
  //NVIC_SetPriority(UART6_IRQn, 1);
}

/**
* @prototype UART_InitVar(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize UART Var module.
*			 初始化UART模块变量.
*/
void SCI_InitVar(void)
{
	F_SCITxEvent = 0;
	Tx_timeOk = 0;
	g_recvedDataRdy = 0;
	g_recvDataTtlLen = 0;
}

/**
* @prototype FUN_UART_SendString(uint8_t *pDataBuf, uint8_t dataLen, word *Snd)
*
* @param[in] ...
* @return	 0:ERR  1:OK
*
* @brief  	 UART 发送API
*			 
*/

byte FUN_UART_SendStringWait(const uint8_t *pDataBuf, word dataLen, word *Snd)
{
		//M_CALevel_H//M_CALevel_TOGGLE
		for (word ii = 0; ii < dataLen; ii++)
		{
				if(UART_SendData(UART2, pDataBuf[ii]) == (-1))
				{
						*Snd = ii;
						return 0;
				}
				while(!UART2->UARTn_LSR0.TC);
		}
		*Snd = dataLen;
		//M_CALevel_L//M_CALevel_TOGGLE
		return 1;
}

/**
* @prototype FUN_SCI_APP_Scheduler
*
* @param[in] ...
* @return	 ...
*
* @brief  	 SCI之APP层总调度
*			 
*/
void FUN_SCI_APP_Scheduler(void)
{
		word w;
		if(F_SCITxEvent == 1)
		{
				FUN_UART_SendString(g_sendDataBuf, UART_TxDATA_LEN_SEL, &w);
				//Tx_timeOk = 0;
				F_SCITxEvent = 0;
		}
}
/**
* @prototype UART_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1)
*
* @param[in] ...
* @return	 ...
*
* @brief  	 UART module interrupt handler.
*			 UART 中断处理函数.
*/
int32_t UART_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1)
{
	  if ((UART2->UARTn_IER.ERXNE == 1) && (LSR0 & LSR0_DR))
    {
        //if ((!g_recvedDataRdy) && (g_recvDataTtlLen < UART_RxDATA_LEN_SEL))//接收数据
				//{
				//	g_recvedDataBuf[g_recvDataTtlLen++] = UART_ReceiveData(UART2);
				//	
				//	if (g_recvDataTtlLen == UART_RxDATA_LEN_SEL)//数据接收完毕
				//	{
				//		g_recvedDataRdy = TRUE;
				//		g_recvDataTtlLen = 0;
				//	}
				//}
				//TR_SCI0_OnRxChar();
    }

    if ((UART2->UARTn_IER.ETXE == 1) && (LSR0 & LSR0_THRE))//发送数据
    {
				#if 1
				if(TxSub > 0)
				{
						TxSub--;
						UART2->UARTn_RBR_THR = TempTxBuffer[TxSub_Int];//(byte)*(OutPtrR++);
					
						TxSub_Int++;	
						if(TxSub_Int >= (SCIOutLen))
						{
								TxSub_Int = 0;
								TxSub = 0;
								UART2->UARTn_IER.ETXE = 0U;
								//VF_TRANSING_INTSCI=0;
						}
				}
				else
				{
						//TxSub = 0;	
						TxSub_Int = 0;
						UART2->UARTn_IER.ETXE = 0U;
				}
				#endif
				
				#if 0
				if(SCIOutLen > 0)
				{
						SCIOutLen--;
						UART2->UARTn_RBR_THR = TempTxBuffer[TxSub_Int];//(byte)*(OutPtrR++);
						TxSub_Int++;
						if(TxSub_Int >= (M_TxSize))
						{
								TxSub_Int = 0;
						}
				}
				else
				{	
						UART2->UARTn_IER.ETXE = 0U;
				}
				#endif
    }

    return 0;
}

/**
* @prototype UART_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1)
*
* @param[in] ...
* @return	 ...
*
* @brief  	 UART module interrupt handler.
*			 UART 中断处理函数.
*/
int32_t UARTLine_IRQHandler(uint8_t port, uint32_t LSR0, uint32_t LSR1)
{
		return 0;
}











