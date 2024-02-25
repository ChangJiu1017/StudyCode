
#include "MCUabs_CAN.h"
#include "Service_CAN.h"

byte F_CANTxEvent;

CAN_MSG_INFO 			g_sendCANMsgInfo;
CAN_MSG_INFO 			g_recvCANMsgInfo[3];
byte CanRxFifo;
byte CanTxFifo;

/*CAN_FilterControl 		g_canFilterTab[CAN_MAX_FILTER_NUM] = 
{
		{ 0, 0, 0x00000000, 0x00000000},
    { 1, 0, 0x00000000, 0x00000000},
    { 2, 0, 0x00000000, 0x00000000},
    { 3, 0, 0x00000000, 0x00000000},
    { 4, 0, 0x00000000, 0x00000000},
    { 5, 0, 0x00000000, 0x00000000},
    { 6, 0, 0x00000000, 0x00000000},
    { 7, 0, 0x00000000, 0x00000000},
    { 8, 0, 0x00000000, 0x00000000},
    { 9, 0, 0x00000000, 0x00000000},
    {10, 0, 0x00000000, 0x00000000},
    {11, 0, 0x00000000, 0x00000000},
    {12, 0, 0x00000000, 0x00000000},
    {13, 0, 0x00000000, 0x00000000},
    {14, 0, 0x00000000, 0x00000000},
    {15, 0, 0x00000000, 0x00000000},
};*/

/**
* @prototype FUN_ADC_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize CAN module.
*			 初始化CAN模块.
*/
void FUN_CAN_InitHardwr(void)
{

		CAN_Config canConfig = {0};
		CAN_BaudrateConfig canBandrateConfig = {0};
	#if(M_CAN1En == 1)	

		GPIO_SetFunc(CAN1_TX, GPIO_FUNC_1);   //设置CAN1引脚复用功能
		GPIO_SetFunc(CAN1_RX, GPIO_FUNC_1);
//		GPIO_SetFunc(CAN1_STB, GPIO_FUNC_3);	//20210906  add  17.30	详见8D报告《QQ011601-21090701-100  7811平台初始化后ECU未断电CAN通讯问题8D报告》
//		GPIO_SetDir(CAN1_STB, GPIO_OUTPUT);   //设置CAN1收发器控制为IO控制
//		CAN1_TRASCVER_NML;//使能CAN收发器
/*
	设置波特率为500K,采样点为81.25%.
	tSeg1 = (S_SEG_1 + 2); tSeg2 = (S_SEG_2 + 1).
	BandRate 	= (48M / (S_PRESC + 1) / ((S_SEG_1 + 2) + (S_SEG_2 + 1)))
	SamplePoint = (tSeg1 / (tSeg1 + tSeg2)).
	在已经知道波特率与采样点的情况下,经过计算后:tSeg1 = 13,tSeg2 = 3.
*/
//		canBandrateConfig.S_PRESC 	= 5;//分频后时钟为8M.
//		canBandrateConfig.S_SEG_1 	= 11;
//		canBandrateConfig.S_SEG_2 	= 2;
//		canBandrateConfig.S_SJW 	= 2;//满足S_SJW <= tSeg2即可.	
		
		canBandrateConfig = *(GetCommonBaudrate(CAN_BAUDRATE_250K));
			
		canConfig.interruptEnable = TRUE;//使能中断
		//canConfig.TSSS = TRUE;//使用次发送缓冲区发送
    canConfig.TPSS = DISABLE;//ENABLE;
    canConfig.TSSS = DISABLE;//ENABLE;
		canConfig.canMode = CAN_MODE_NORMAL;
		canConfig.autoReset = TRUE;//CAN BUSOFF后,自动恢复
		canConfig.filterList = 0;//g_canFilterTab;//赋值过滤器设置list
			
		CAN_SetEventCallBack(CAN2, CAN1_IRQnCallBack);
		CAN_Initialize(CAN2, &canConfig, &canBandrateConfig);
		
	#endif
	
	#if(M_CAN2En == 1)
		
		
		
	#endif
}


/**
* @prototype CAN_InitVar(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize CAN Var module.
*			 初始化CAN模块变量.
*/
void CAN_InitVar(void)
{
		F_CANTxEvent = 0;
		CanRxFifo = 0;
		CanTxFifo = 0;
}


/**
* @prototype Sdk_CAN_SendMsg(uint32_t ID, CAN_IDE_TypeDef IDE, CAN_RTR_TypeDef RTR, uint8_t *pDataBuf, uint8_t dataLen)
*
* @param[in] ID: send data CAN ID. 
* @param[in] IDE: Choose between CAN_IDE_STD and CAN_IDE_EXT.
* @param[in] RTR: Choose between CAN_RTR_STD and CAN_RTR_RMT.
* @param[in] pDataBuf: The pointer of the data to be sended.
* @param[in] dataLen: Send data length.
* @return	 CAN_SendSts_TypeDef,send fail or success.
*
* @brief  	 Send CAN message.
*			 发送CAN数据.
*/
CAN_SendSts_TypeDef CAN_SendMsg(const CAN_MSG_INFO * g_sendCANMsgInfo)
{
	//g_sendCANMsgInfo.ID = ID;
	//g_sendCANMsgInfo.IDE = IDE;
	//g_sendCANMsgInfo.RTR = RTR;
	//g_sendCANMsgInfo.DLC = dataLen;
	//memcpy(g_sendCANMsgInfo.Data, pDataBuf, dataLen);
	
	if(!CAN_IsTransmitBusy(CAN2, TRANSMIT_SECONDARY))
	{
		if (!CAN_MessageSend(CAN2, g_sendCANMsgInfo, TRANSMIT_SECONDARY))
		{
			return CAN_SEND_MSG_SUCCS;
		}
		else
		{
			return CAN_SEND_MSG_FILED;
		}
	}
	
	return CAN_SEND_MSG_FILED;
}


/**
* @prototype FUN_CAN_APP_Scheduler
*
* @param[in] ...
* @return	 ...
*
* @brief  	 CAN之APP层总调度
*			 
*/

void FUN_CAN_APP_Scheduler(void)
{
//		CAN1_TRASCVER_NML;
		FUN_CANRxDataResolver();
		FUN_CANTxDataPro();
		FUN_LOOKUPDebug();
}
byte	denugLookUpSub = 25;
sword denugLookUpRT;
sword	denugLookUpInV;
sword	denugLookUpInV2;
byte  denugLookUpTable[26][2]={			//Y(FMF^(2/3))	 ,FMF/4
	10	,	11	,
	15	,	15	,
	18	,	17	,
	21	,	19	,
	28	,	23	,
	31	,	25	,
	36	,	27	,
	37	,	28	,
	40	,	29	,
	41	,	30	,
	46	,	32	,
	47	,	33	,
	57	,	37	,
	58	,	38	,
	63	,	40	,
	73	,	44	,
	83	,	48	,
	93	,	52	,
	97	,	53	,
	102	,	55	,
	105	,	56	,
	108	,	57	,
	113	,	59	,
	116	,	60	,
	120	,	61	,
	122	,	62	,
	};			
byte denugLookUpTable2[17][16]={			
	12,	17,	22,	28,	32,	36,	40,	44,	46,	49,	50,	53,	56,	58,	60,	60,	//	0	
	12,	17,	22,	28,	32,	36,	40,	44,	46,	49,	50,	53,	56,	58,	60,	60,	//	1	
	12,	17,	22,	28,	32,	36,	38,	42,	44,	48,	50,	53,	56,	57,	60,	60,	//	2	
	12,	17,	22,	28,	32,	36,	38,	40,	44,	47,	48,	53,	54,	57,	59,	59,	//	3	
	11,	19,	23,	28,	32,	36,	38,	40,	44,	46,	48,	52,	54,	57,	59,	59,	//	4	
	11,	19,	23,	28,	32,	36,	38,	39,	42,	45,	48,	52,	54,	56,	58,	58,	//	5	
	11,	19,	23,	28,	32,	34,	36,	39,	42,	45,	47,	52,	53,	56,	58,	58,	//	6	
	11,	23,	24,	29,	32,	34,	36,	39,	40,	44,	47,	52,	53,	56,	57,	57,	//	7	
	11,	23,	25,	29,	33,	34,	36,	38,	40,	44,	47,	52,	53,	56,	57,	57,	//	8	
	11,	23,	25,	28,	33,	34,	36,	38,	40,	43,	46,	52,	53,	55,	56,	56,	//	9	
	11,	23,	25,	28,	32,	34,	36,	38,	40,	43,	46,	50,	53,	55,	56,	56,	//	10	
	11,	22,	25,	28,	32,	33,	36,	38,	40,	43,	46,	50,	52,	55,	56,	56,	//	11	
	11,	21,	25,	27,	30,	33,	35,	38,	40,	43,	46,	50,	52,	54,	55,	55,	//	12	
	11,	20,	24,	27,	30,	33,	35,	38,	40,	43,	46,	50,	52,	53,	54,	54,	//	13	
	11,	19,	24,	36,	30,	33,	35,	38,	39,	42,	46,	50,	52,	53,	53,	53,	//	14	
	11,	18,	23,	36,	29,	32,	34,	37,	39,	42,	45,	49,	50,	52,	52,	52,	//	15	
	11,	17,	23,	26,	29,	32,	34,	37,	39,	42,	45,	49,	50,	51,	52,	52,	//	16	
};
byte denugLookUpTable2_x[16]={																			
24,	32,	40,	48,	56,	64,	72,	80,	88,	96,	104,	112,	120,	128,	136,	144,				
};																			
byte denugLookUpTable2_y[17]={																			
0,	16,	32,	48,	64,	80,	96,	112,	128,	144,	160,	176,	192,	208,	224,	240,	255,			
};	
void FUN_LOOKUPDebug(void)
{
		//denugLookUpRT = F_2DLookUp(denugLookUpInV,denugLookUpTable,denugLookUpSub,0,0);
		//denugLookUpRT = (byte)FG_3DMap_xyz((byte *)denugLookUpTable2,denugLookUpInV,denugLookUpInV2,(byte *)denugLookUpTable2_x,(byte *)denugLookUpTable2_y,15,16,0x00);//2017-1-21
}
/**
* @prototype FUN_CANRxDataResolver
*
* @param[in] ...
* @return	 ...
*
* @brief  	 CAN数据解析
*			 
*/
void FUN_CANRxDataResolver(void)
{
		
		if(CanRxFifo > 0)
		{
				switch(g_recvCANMsgInfo[0].ID)
				{
					case 0x222:
							Chargetime = (g_recvCANMsgInfo[0].Data[1]<<8) + g_recvCANMsgInfo[0].Data[0];
							InjT1 		 = (g_recvCANMsgInfo[0].Data[3]<<8) + g_recvCANMsgInfo[0].Data[2];
							if(Chargetime < 50)Chargetime=50;
							if(InjT1 < 50)InjT1=50;
					break;
					case 0x333:
							denugLookUpInV = g_recvCANMsgInfo[0].Data[0];
							denugLookUpInV2 = g_recvCANMsgInfo[0].Data[1];
					break;
					case 0x444:
					break;
					default:
						break;
				}
				for (byte i = 0; i < (CanRxFifo - 1); i++)
				{
					g_recvCANMsgInfo[i] = g_recvCANMsgInfo[i + 1];
				}
				CanRxFifo--;
		}
}
/**
* @prototype FUN_CANTxDataPro
*
* @param[in] ...
* @return	 ...
*
* @brief  	 CAN之APP层CAN数据发送
*			 
*/
void FUN_CANTxDataPro(void)
{
		byte Data[8];
		if(F_CANTxEvent == 1)
		{
				if(CanTxFifo == 0)
				{
						Data[0] = InterCHV[0];
						Data[1] = (InterCHV[0]>>8);
						Data[2] = InterCHV[1];
						Data[3] = (InterCHV[1]>>8);
						Data[4] = InterCHV[2];
						Data[5] = (InterCHV[2]>>8);
						Data[6] = InterCHV[3];
						Data[7] = (InterCHV[3]>>8);
						if(CAN_SendFrameExt(0x100,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo > 1)
				{
						F_CANTxEvent = 0;
						CanTxFifo = 0;
				}
				if(CanTxFifo == 1)
				{
						Data[0] = InterCHV[4];
						Data[1] = (InterCHV[4]>>8);
						Data[2] = InterCHV[5];
						Data[3] = (InterCHV[5]>>8);
						Data[4] = 1;
						Data[5] = 1;
						Data[6] = 1;
						Data[7] = 1;
						if(CAN_SendFrameExt(0x200,0,4,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 2)
				{
						Data[0] = (RT_ch8);
						Data[1] = (RT_ch8>>8);
						Data[2] = 22;
						Data[3] = 23;
						Data[4] = 24;
						Data[5] = 25;
						Data[6] = 26;
						Data[7] = 27;
						if(CAN_SendFrameExt(0x300,0,2,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 3)
				{
						Data[0] = denugLookUpRT;
						Data[1] = (denugLookUpRT>>8);
						Data[2] = 32;
						Data[3] = 33;
						Data[4] = 34;
						Data[5] = 35;
						Data[6] = 36;
						Data[7] = 37;
						if(CAN_SendFrameExt(0x400,0,2,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 4)
				{
						Data[0] = 40;
						Data[1] = 41;
						Data[2] = 42;
						Data[3] = 43;
						Data[4] = 44;
						Data[5] = 45;
						Data[6] = 46;
						Data[7] = 47;
						if(CAN_SendFrameExt(0x500,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 5)
				{
						Data[0] = 50;
						Data[1] = 51;
						Data[2] = 52;
						Data[3] = 53;
						Data[4] = 54;
						Data[5] = 55;
						Data[6] = 56;
						Data[7] = 57;
						if(CAN_SendFrameExt(0x600,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 6)
				{
						Data[0] = 60;
						Data[1] = 61;
						Data[2] = 62;
						Data[3] = 63;
						Data[4] = 64;
						Data[5] = 65;
						Data[6] = 66;
						Data[7] = 67;
						if(CAN_SendFrameExt(0x700,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 7)
				{
						Data[0] = 70;
						Data[1] = 71;
						Data[2] = 72;
						Data[3] = 73;
						Data[4] = 74;
						Data[5] = 75;
						Data[6] = 76;
						Data[7] = 77;
						if(CAN_SendFrameExt(0x800|CAN_EXTENDED_FRAME_ID,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 8)
				{
						Data[0] = 80;
						Data[1] = 81;
						Data[2] = 82;
						Data[3] = 83;
						Data[4] = 84;
						Data[5] = 85;
						Data[6] = 86;
						Data[7] = 87;
						if(CAN_SendFrameExt(0x900|CAN_EXTENDED_FRAME_ID,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 9)
				{
						Data[0] = 90;
						Data[1] = 91;
						Data[2] = 92;
						Data[3] = 93;
						Data[4] = 94;
						Data[5] = 95;
						Data[6] = 96;
						Data[7] = 97;
						if(CAN_SendFrameExt(0x17900|CAN_EXTENDED_FRAME_ID,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo == 10)
				{
						Data[0] = 100;
						Data[1] = 101;
						Data[2] = 102;
						Data[3] = 103;
						Data[4] = 104;
						Data[5] = 105;
						Data[6] = 106;
						Data[7] = 107;
						if(CAN_SendFrameExt(0x17A00|CAN_EXTENDED_FRAME_ID,0,8,Data) == CAN_SEND_MSG_SUCCS)
						{
								CanTxFifo++;
						}
				}
				if(CanTxFifo > 10)
				{
						F_CANTxEvent = 0;
						CanTxFifo = 0;
				}
		}
}


/**
* @prototype CAN2_IRQnCallBack(uint32_t event, uint32_t wparam, uint32_t lparam)
*
* @param[in] ...
* @return	 ...
*
* @brief  	 CAN1 module interrupt handler.
*			 CAN1中断处理函数.
*/
int32_t CAN1_IRQnCallBack(uint32_t event, uint32_t wparam, uint32_t lparam)
{
	#if(M_Debug==1)
	if(CanRxFifo > 2)
	{	
			return 0;
	}
	#endif
	if (event & CAN_EVENT_RECVMSG)
	{
			if (CAN_IsMsgInReceiveBuf((CAN_Type*)lparam))
      		{
        		#if(M_Debug==1)
					CAN_MessageRead((CAN_Type*)lparam, (CAN_MSG_INFO *)(&g_recvCANMsgInfo[CanRxFifo]));
					CanRxFifo++;
				#else
					CAN_MessageRead((CAN_Type*)lparam, (CAN_MSG_INFO *)(&g_recvCANMsgInfo[0]));
					CAN_OnFullRxBuffer();
				#endif
				
					if(g_recvCANMsgInfo[0].ID==0x18EF3EFD)
					{
							//M_AddAirV_C = (g_recvCANMsgInfo[0].Data[0] != 0);
							//M_BumpO2_C = (g_recvCANMsgInfo[0].Data[1] != 0);	//20211003 chenwei 屏蔽
					}	

					
					
					if((g_recvCANMsgInfo[0].ID==0X711)||(g_recvCANMsgInfo[0].ID==0X7DF))
					{
							g_sendCANMsgInfo.ID = 0X791;
							g_sendCANMsgInfo.DLC = 8;
							g_sendCANMsgInfo.IDE = 0;
							g_sendCANMsgInfo.RTR = 0;
							if(g_recvCANMsgInfo[0].Data[2]==0X03)
							{
								g_sendCANMsgInfo.Data[0]=0x06;
								g_sendCANMsgInfo.Data[1]=0x50;
								g_sendCANMsgInfo.Data[2]=0x03;
								g_sendCANMsgInfo.Data[3]=0x00;
								g_sendCANMsgInfo.Data[4]=0x32;
								g_sendCANMsgInfo.Data[5]=0x13;
								g_sendCANMsgInfo.Data[6]=0x88;
								g_sendCANMsgInfo.Data[7]=0x00;
								CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
							}
							else if((g_recvCANMsgInfo[0].Data[2]==0X02)&&(g_recvCANMsgInfo[0].ID==0X711))
							{
								g_sendCANMsgInfo.Data[0]=0x03;
								g_sendCANMsgInfo.Data[1]=0x7F;
								g_sendCANMsgInfo.Data[2]=0x10;
								g_sendCANMsgInfo.Data[3]=0x78;
								g_sendCANMsgInfo.Data[4]=0x00;
								g_sendCANMsgInfo.Data[5]=0x00;
								g_sendCANMsgInfo.Data[6]=0x00;
								g_sendCANMsgInfo.Data[7]=0x00;
								while(CAN_IsTransmitBusy(CAN2, TRANSMIT_SECONDARY)){;}
								while(CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY)){;}//TRANSMIT_PRIMARY
								
								
								FUN_SetBootVar();
								g_sendCANMsgInfo.ID = 0X112;
								g_sendCANMsgInfo.Data[0]=BootFlagApp;
								g_sendCANMsgInfo.Data[1]=BootFlagApp1;
								g_sendCANMsgInfo.Data[2]=BootFlagApp2;
								g_sendCANMsgInfo.Data[3]=BootFlagApp3;
								g_sendCANMsgInfo.Data[4]=BootFlagApp4;
								g_sendCANMsgInfo.Data[5]=BootFlagApp5;
								g_sendCANMsgInfo.Data[6]=BootFlagApp6;
								g_sendCANMsgInfo.Data[7]=BootFlagApp7;
								while(CAN_IsTransmitBusy(CAN2, TRANSMIT_SECONDARY)){;}
								while(CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY)){;}//TRANSMIT_PRIMARY
								g_sendCANMsgInfo.Data[0]=BootFlagApp8;
								g_sendCANMsgInfo.Data[1]=BootFlagApp9;
								g_sendCANMsgInfo.Data[2]=BootFlagAppA;
								g_sendCANMsgInfo.Data[3]=BootFlagAppB;
								g_sendCANMsgInfo.Data[4]=BootFlagAppC;
								g_sendCANMsgInfo.Data[5]=BootFlagAppD;
								g_sendCANMsgInfo.Data[6]=BootFlagAppE;
								g_sendCANMsgInfo.Data[7]=BootFlagAppF;
								while(CAN_IsTransmitBusy(CAN2, TRANSMIT_SECONDARY)){;}
								while(CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY)){;}//TRANSMIT_PRIMARY
								NVIC_SystemReset();
							}
					}
      		}
	}
	return 1;
}


void FUN_SetBootVar(void)
{
		BootFlagApp = 0x5A;//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
		BootFlagApp1= 0x5B;
		BootFlagApp2= 0x00;
		BootFlagApp3= 0x18;
		BootFlagApp4= 0x44;
		BootFlagApp5= 0x11;
		BootFlagApp6= 0x24;
		BootFlagApp7= 0x00;
		BootFlagApp8= 0x5A;//0x5A 0x5B 0x00 0x18 0x44 0x11 0x24 0x00
		BootFlagApp9= 0x5B;
		BootFlagAppA= 0x00;
		BootFlagAppB= 0x18;
		BootFlagAppC= 0x44;
		BootFlagAppD= 0x11;
		BootFlagAppE= 0x24;
		BootFlagAppF= 0x00;
}









