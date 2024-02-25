
#include "FAM.h"

dword PWM2CNT;
dword PWM2CHV[6];
byte PWM2DebugCATime[6];

word v_tooth_time_curr[6];
word v_tooth_time_last[6];
word v_tooth_time_curr_CNT[6];
word v_tooth_time_last_CNT[6];
word InterCNT[6];
word InterCHV[6];

byte InjST;
byte IngST;
word InjBacktime;
word InjT1;
word Chargetime;
byte InjIntTime;
byte IngIntTime;

byte F_CAok;
byte F_CAok2;
byte F_CAok3;
byte PWM2ch0ST;


unsigned int Electric=0;
unsigned int Electric_T2=0;
unsigned int Volt=0;
unsigned int Temp=0;
unsigned int Temp_head=0;
unsigned int Pressure=0;

unsigned int dianliu=0;
unsigned int dianya=0;

unsigned char Flag_end = 0;
unsigned char Flag_first = 0;
unsigned char ADC_collect=0;
unsigned char ID_count=0;
unsigned char Frequ_time= 0;
unsigned char Spray_time= 0;
unsigned char Spray3_time=0;
unsigned char T1_time= 0;
unsigned char T2_time= 0;
unsigned char T_time= 0;
/**
* @prototype FUN_InjIng_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize InjIng module.
*			 初始化InjIng模块.
*/
void FUN_InjIng_InitHardwr(void)
{
	
//    GPIO_SetFunc(GPIO_PC1, 1); ///Set PWM2_CH2
//    GPIO_SetFunc(GPIO_PC2, 1); ///Set PWM2_CH3
//    GPIO_SetFunc(GPIO_PC3, 3); ///Set PWM2_CH0
//    GPIO_SetFunc(GPIO_PC4, 3); ///Set PWM2_CH1
	/*
		GPIO_SetDir(M_IngPIN, GPIO_OUTPUT);//设置点火引脚为输出
		//GPIO_SetPullupEnable(M_IngPIN, ENABLE);
		GPIO_SetE4E2(M_IngPIN, M_GPIO_4mA);
		GPIO_SetDir(M_InjPIN, GPIO_OUTPUT);//设置喷油引脚为输出
		//GPIO_SetPullupEnable(M_InjPIN, ENABLE);
		GPIO_SetE4E2(M_InjPIN, M_GPIO_4mA);
		M_Ing_OFF;
		M_Inj_OFF;
	
		PWM_SetCallback(PWM2, PWM2_IRQHandler_Callback);
	
    NVIC_SetPriority(PWM2_IRQn, 0);
    NVIC_EnableIRQ(PWM2_IRQn);
	
    PWM_InputCaptureInit(PWM2, PWM_CHANNEL_CHANNEL0, PWM_INPUTCAPTURE_RISINGEDGE);
    PWM_InputCaptureInit(PWM2, PWM_CHANNEL_CHANNEL1, PWM_INPUTCAPTURE_FALLINGEDGE);
		#if (M_UPorDown == 0)
    PWM_InputCaptureInit(PWM2, PWM_CHANNEL_CHANNEL2, PWM_INPUTCAPTURE_RISINGEDGE);
		#endif		
		#if (M_UPorDown == 1)
    PWM_InputCaptureInit(PWM2, PWM_CHANNEL_CHANNEL2, PWM_INPUTCAPTURE_FALLINGEDGE);
		#endif
		//PWM_InputCaptureInit(PWM2, PWM_CHANNEL_CHANNEL3, PWM_INPUTCAPTURE_FALLINGEDGE);
    
		PWM_OutputCompareInit(PWM2, PWM_CHANNEL_CHANNEL4, 0);
		PWM_OutputCompareInit(PWM2, PWM_CHANNEL_CHANNEL5, 0);
		//PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL0, 20); 
		//PWM2ch0ST = 1;
		
		//PWM_OutputMaskSet(PWM2,PWM_CHANNEL_CHANNEL0);
		//PWM_OutputMaskSet(PWM2,PWM_CHANNEL_CHANNEL2);
		PWM_OutputMaskSet(PWM2,PWM_CHANNEL_CHANNEL4);
		PWM_OutputMaskSet(PWM2,PWM_CHANNEL_CHANNEL5);
		
		PWM_EnableOverflowInt(PWM2);
		
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL0);
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL1);
    PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL2);
    //PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL3);
		
    //PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL0);
    //PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL2);
    PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
    PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
		
    PWM_SetCapturePSCValue(PWM2, PWM_CHANNEL_CHANNEL0, 0);
    PWM_SetCapturePSCValue(PWM2, PWM_CHANNEL_CHANNEL1, 0);
    PWM_SetCapturePSCValue(PWM2, PWM_CHANNEL_CHANNEL2, 0);
    //PWM_SetCapturePSCValue(PWM2, PWM_CHANNEL_CHANNEL3, 0);
		PWM2->CAPFILTER |= 0x000FFFFF;
		PWM2->MCVR = 0xFFFF;
    PWM_ClockSet(PWM2, PWM_CLOCK_SYSTEMCLOCK, 191);
		
		*/

    TIMER_SetCallback(TIMER0, TIMER0_IRQHandler_Callback);
    TIMER_SetCallback(TIMER1, TIMER1_IRQHandler_Callback);
    TIMER_SetCallback(TIMER2, TIMER2_IRQHandler_Callback);
    TIMER_SetCallback(TIMER3, TIMER3_IRQHandler_Callback);
    TIMER_SetCallback(TIMER4, TIMER4_IRQHandler_Callback);
    TIMER_SetCallback(TIMER5, TIMER5_IRQHandler_Callback);
    TIMER_SetCallback(TIMER6, TIMER6_IRQHandler_Callback);
    TIMER_SetCallback(TIMER7, TIMER7_IRQHandler_Callback);
    NVIC_EnableIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_EnableIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER3_IRQn);
    NVIC_EnableIRQ(TIMER4_IRQn);
    NVIC_EnableIRQ(TIMER5_IRQn);
    NVIC_EnableIRQ(TIMER6_IRQn);
    NVIC_EnableIRQ(TIMER7_IRQn);
    NVIC_SetPriority(TIMER0_IRQn, 0);
    NVIC_SetPriority(TIMER1_IRQn, 0);
    NVIC_SetPriority(TIMER2_IRQn, 0);
    NVIC_SetPriority(TIMER3_IRQn, 0);
    NVIC_SetPriority(TIMER4_IRQn, 0);
    NVIC_SetPriority(TIMER5_IRQn, 0);
    NVIC_SetPriority(TIMER6_IRQn, 0);
    NVIC_SetPriority(TIMER7_IRQn, 0);
}

/**
* @prototype InjIng_InitVar(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize InjIng Var module.
*			 初始化InjIng模块变量.
*/
void InjIng_InitVar(void)
{
		PWM2CNT = 0;
		for(byte ii = 0; ii < 6; ii++)
		{
				PWM2CHV[ii] = 0;
				PWM2DebugCATime[ii] = 0;
				v_tooth_time_curr[ii] = 0;
				v_tooth_time_last[ii] = 0;
				v_tooth_time_curr_CNT[ii] = 0;
				v_tooth_time_last_CNT[ii] = 0;
				InterCNT[ii] = 0;
				InterCHV[ii] = 0;
		}
		
		InjST = 1;
		IngST = 1;
		InjBacktime = 500;
		InjT1 = 500;
		Chargetime = 500;
		InjIntTime = 0;
		IngIntTime = 0;
}




/**
* @prototype FUN_InjIng_APP_Scheduler
*
* @param[in] ...
* @return	 ...
*
* @brief  	 InjIng之APP层总调度
*			 
*/

void FUN_InjIng_APP_Scheduler(void)
{
		PWM2CNT = PWM2->CNT;
		PWM2CHV[0] = PWM2->CHANNELS[0].CHV;
		PWM2CHV[1] = PWM2->CHANNELS[1].CHV;
		PWM2CHV[2] = PWM2->CHANNELS[2].CHV;
		PWM2CHV[3] = PWM2->CHANNELS[3].CHV;
		PWM2CHV[4] = PWM2->CHANNELS[4].CHV;
		PWM2CHV[5] = PWM2->CHANNELS[5].CHV;
}


/**
* @brief This function handle PWM 2 ch0 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL0_Callback(void)//捕获33813复位
{		
		//TIMER8_IRQHandler_Callback();
}/**
* @brief This function handle PWM 2 ch1 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL1_Callback(void)//用作轮速信号捕获  pc4
{
#if(M_Debug ==1)
		PWM2DebugCATime[1]++;
		__NOP;__NOP;__NOP;__NOP;__NOP;
	
		v_tooth_time_curr_CNT[1] = PWM2->CNT;
		InterCNT[1] = v_tooth_time_curr_CNT[1] - v_tooth_time_last_CNT[1];
		v_tooth_time_last_CNT[1] = v_tooth_time_curr_CNT[1];
	
		v_tooth_time_curr[1] = PWM2->CHANNELS[1].CHV;
		InterCHV[1] = v_tooth_time_curr[1] - v_tooth_time_last[1];
		v_tooth_time_last[1] = v_tooth_time_curr[1];
#else
	
#endif

}/**
* @brief This function handle PWM 2 ch2 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL2_Callback(void)//角标前沿  pc1  硬件滤波最大滤32个值
{
#if(M_Debug ==1)
		PWM2DebugCATime[2]++;
		__NOP;__NOP;__NOP;__NOP;__NOP;
		//if(M_CAIN == 0)return;
	
		v_tooth_time_curr_CNT[2] = PWM2->CNT;
		InterCNT[2] = v_tooth_time_curr_CNT[2] - v_tooth_time_last_CNT[2];
		v_tooth_time_last_CNT[2] = v_tooth_time_curr_CNT[2];
	
		v_tooth_time_curr[2] = PWM2->CHANNELS[2].CHV;
		InterCHV[2] = v_tooth_time_curr[2] - v_tooth_time_last[2];
		v_tooth_time_last[2] = v_tooth_time_curr[2];
#else
		PWM2DebugCATime[2]++;
		v_tooth_time_curr[2] = PWM2->CHANNELS[2].CHV;
		InterCHV[2] = v_tooth_time_curr[2] - v_tooth_time_last[2];
		v_tooth_time_last[2] = v_tooth_time_curr[2];
		//F_CAok = 1;
		//F_CAok2 = 1;
		//F_CAok3 = 1;
		CA_capture_OnCapture();
#endif
}/**
* @brief This function handle PWM 2 ch3 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL3_Callback(void)//备用
{
#if(M_Debug ==1)
		PWM2DebugCATime[3]++;
		__NOP;__NOP;__NOP;__NOP;__NOP;
		//if(M_HYIN == 1)return;
	
		v_tooth_time_curr_CNT[3] = PWM2->CNT;
		InterCNT[3] = v_tooth_time_curr_CNT[3] - v_tooth_time_last_CNT[3];
		v_tooth_time_last_CNT[3] = v_tooth_time_curr_CNT[3];
	
		v_tooth_time_curr[3] = PWM2->CHANNELS[3].CHV;
		InterCHV[3] = v_tooth_time_curr[3] - v_tooth_time_last[3];
		v_tooth_time_last[3] = v_tooth_time_curr[3];
#else
	
#endif
	
}/**
* @brief This function handle PWM 2 ch2 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL4_Callback(void)
{
		TIMER10_IRQHandler_Callback();
}/**
* @brief This function handle PWM 2 ch2 interrupt.
* @param[in] none
* @return none
*/
void PWM_CHANNEL5_Callback(void)
{
		TIMER11_IRQHandler_Callback();
}

uint8_t PWM_GetChannelIntEnFlag(PWM_Type *PWMx, uint8_t pwmChannel)
{
    return (PWMx->CHANNELS[pwmChannel].CHSCR & PWM_CHSCR_CHIE_Msk);
}
/**
* @brief This function handle PWM 2 interrupt.
* @param[in] none
* @return none
*/
void PWM2_IRQHandler_Callback(void)
{
    if (PWM_GetOverFlowFlag(PWM2))
    {
        PWM_ClrOverFlowFlag(PWM2);
				//CA_capture_OnOverflow();
    }
    else
    {
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL0) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL0))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL0);
						PWM_CHANNEL0_Callback();
        }
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL1) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL1))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL1);
						PWM_CHANNEL1_Callback();
        }
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL2) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL2))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL2);
						PWM_CHANNEL2_Callback();
        }
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL3) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL3))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL3);
						PWM_CHANNEL3_Callback();
        }
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL4))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL4);
						PWM_CHANNEL4_Callback();
        }
        if (PWM_GetChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5) && PWM_GetChannelIntEnFlag(PWM2, PWM_CHANNEL_CHANNEL5))
        {
            PWM_ClrChannelIntFlag(PWM2, PWM_CHANNEL_CHANNEL5);
						PWM_CHANNEL5_Callback();
        }
    }
}


#define MAX_MIN_SECOND_COUNT        99
#define MAX_SECOND_MINUTE_COUNT     59
#define MAX_HOURS_COUNT             23


/* 以下四个全局变量为具体计数 */
uint32_t g_timer0Count = 0; ///<timer0 Count 每10ms计数加1
uint32_t g_timer1Count = 0; ///<timer1 Count 每1s计数加1
uint32_t spray_time0=0;      // 泵的喷射次数前两位
uint32_t spray_time1=0;      // 泵的喷射次数后两位
uint8_t g_timer4Count = 0; ///<timer4 Count 每1分钟计数加1
uint8_t g_timer7Count = 0; ///<timer7 Count 每1小时计数加1


/**
* @brief This function handle timer0 interrupt.
* @param[in] none
* @return none
*/
void TIMER0_IRQHandler_Callback(uint8_t timerIndex)
{
		if (TIMER_GetIntFlag(TIMER0))
    {
        TIMER_ClrIntFlag(TIMER0); //清中断标志
			
        
			 		
//      if(flag_end==0)				
//		 {
//			  g_timer0count++;
//        if (g_timer0count==1)				            
//				    m_inj_on;
//				if (g_timer0count==21)
//					  m_inj_off;
//				if (g_timer0count==101 )
//				{				  
//					  g_timer0count=0;
//				    spray_time0++;
//				}
//				
//				if (spray_time0==1000)
//				{
//					  spray_time0=0;
//					  flag_end=1;
//			  }
//			}
//		
//				else
//				{
//					m_inj_off;	
//          m_ing_off;					
//				}
    }
}

 
/**
* @brief This function handle timer1 interrupt.
* @param[in] none
* @return none
*/
void TIMER1_IRQHandler_Callback(uint8_t timerIndex)
{
	
		if (TIMER_GetIntFlag(TIMER1)) //定时器T1 0.1ms 中断进行一次清零操作
    {
        TIMER_ClrIntFlag(TIMER1); //清中断标志
			 
     Pressure = FAUN_ADS2S128(0x3,0,2);    //压力实时采样
		 
		 Temp = FAUN_ADS2S128(0x5,0,2);    //温度实时采样
		 //Temp_head=FAUN_ADS2S128(0X05,0,2);//温度实时采样前两位
			
		 
		  if(Flag_end==0)
			{
				
				if (Flag_first==1)
		    {
				   g_timer1Count++;   //0.1ms计数加1
				}
				
        if (g_timer1Count==1)
				{
				    M_AddAirV_C_OFF;     //做工引脚
						M_CTank_C_ON;      //回位引脚
									
					  M_BumpO2_C_ON;       //PC1
					  M_FeulBump_C_OFF;    //PC2
		    }
				
		    if(g_timer1Count==0x12)  //1.8ms前一时刻进行电流采样
				{
					
						Volt=FAUN_ADS2S128(0x7,0,2);       //供电电压采样
		        Electric=FAUN_ADS2S128(0x8,0,2);   //喷嘴电流/电压采样
					  Electric_T2=FAUN_ADS2S128(0x9,0,2);
					  
					  M_O2Heat_C_TOGGLE;  //反转PB15引脚					
	
				}
				
				if (g_timer1Count==0x13)
				{				  
					  ID_count++;
					  ADC_collect=1;			
				}
				if(ADC_collect==1)  
					{				              					              									              				          							              
						CAN_Send_adc();
						//mdelay(5);//延时
					  ADC_collect=0;	
				  }		
				if (g_timer1Count==T1_time)  //回位停止
				{
					  M_AddAirV_C_ON
					  M_CTank_C_OFF;
					  M_BumpO2_C_OFF;
					  M_FeulBump_C_ON;
				}
						
				if (g_timer1Count==T2_time) //做工停止
				{
					  M_AddAirV_C_OFF
					  M_CTank_C_OFF;				
					  M_BumpO2_C_OFF;
					  M_FeulBump_C_OFF;
				}
				if (g_timer1Count==T_time) //一次喷射停止 喷射次数加1
				{				  
					  g_timer1Count=0;
					  //ADC_collect=0;
					if(Flag_first==1)
					{
				    spray_time1++;
					}
				}
				if (spray_time1==((uint32_t)Spray3_time*(0XFF+1)+Spray_time))		//到达喷射次数停止 对标志位进行赋值	 
				{
					  Flag_first=0;
					  Flag_end=1;				  
            g_timer1Count=0;					
					  ID_count=0;
					  ADC_collect=0;
					  spray_time1=0;			  					  
			  }
			}
		else   //等待下一次上位机发送启动命令 否则程序引脚电平不进行反转
			{
				   //M_Inj_OFF;	
				   //M_Ing_OFF;
			}
		}
    
}
/**
* @brief This function handle timer2 interrupt.
* @param[in] none
* @return none
*/
void TIMER2_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER2);
}
/**
* @brief This function handle timer3 interrupt.
* @param[in] none
* @return none 2/3定时器链接作为喷油用
*/
void TIMER3_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER3);
	

}
/**
* @brief This function handle timer4 interrupt.
* @param[in] none
* @return none
*/
void TIMER4_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER4);
}
/**
* @brief This function handle timer5 interrupt.
* @param[in] none
* @return none 4/5定时器链接作为点火用
*/
void TIMER5_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER5);
}
/**
* @brief This function handle timer6 interrupt.
* @param[in] none
* @return none
*/
void TIMER6_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER6);

}
/**
* @brief This function handle timer7 interrupt.
* @param[in] none
* @return none 6/7定时器链接作为其他备用
*/
void TIMER7_IRQHandler_Callback(uint8_t timerIndex)
{
		TIMER_ClrIntFlag(TIMER7);
	
}

void TIMER8_IRQHandler_Callback(void)
{
//		switch(PWM2ch0ST)
//		{
//			case 1:
//				M_AirV_ON;
//				FUN_TIMER8_TrigeInt(500);
//				//M_AirV_TOGGLE;
//			  PWM2ch0ST = 2;
//				break;
//			case 2:
//				M_AirV_OFF;
//				FUN_TIMER8_DisAbleInt();
//				//M_AirV_TOGGLE;
//				break;
//			default:
//				break;
//		}
}

void TIMER9_IRQHandler_Callback(void)
{

}

void TIMER10_IRQHandler_Callback(void)//点火
{
//#if(M_Debug ==1)
//		IngIntTime++;
//		switch(IngST)
//		{
//			case 1://充磁
//				M_Ing_ON;
//				FUN_TIMER11_TrigeInt((word)(Chargetime-1));
//				IngST = 2;
//			break;
//			case 2://充磁
//				M_Ing_OFF;
//			  IngST = 1;
//				FUN_TIMER11_DisAbleInt();
//			break;
//		default:
//			break;
//		}
//#else
//		TC_IGNIO_OnInterrupt();
//#endif	
}

void TIMER11_IRQHandler_Callback(void)//喷油
{
//#if(M_Debug ==1)
//		InjIntTime++;
//		switch(InjST)
//		{
//			case 1://回位
//				M_Inj_OFF;
//				FUN_TIMER10_TrigeInt((word)(4));
//				InjST = 2;
//			break;
//			case 2://喷油
//				M_Inj_ON;
//				FUN_TIMER10_TrigeInt((word)(InjBacktime-1));
//				InjST = 3;
//			break;
//			case 3://结束
//				M_Inj_OFF;
//			  InjST = 1;
//				//M_Ing_OFF;
//				FUN_TIMER10_DisAbleInt();
//			break;
//		default:
//			break;
//		}
//#else
//		TC_INJO_OnInterrupt();
//#endif
}



void TIMER_Interrupt_Control_Timer0(void)
{
	FUN_TIMER0_Init();
}

void TIMER_Interrupt_Control_Timer1(void)
{
	FUN_TIMER1_Init();
}

void FUN_TIMER0_Init(void)
{
    TIMER_ConfigType timerConfig;
	
    timerConfig.timerEn = ENABLE;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkMode = DISABLE;
    timerConfig.loadValue = 4799;
    TIMER_Init(TIMER0, &timerConfig);

	  TIMER_SetCallback(TIMER0, TIMER0_IRQHandler_Callback);
	
}

void FUN_TIMER1_Init(void)
{
    TIMER_ConfigType timerConfig;
	
    timerConfig.timerEn = ENABLE;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkMode = DISABLE;
    timerConfig.loadValue = 4799;
    TIMER_Init(TIMER1, &timerConfig);

    TIMER_SetCallback(TIMER1, TIMER1_IRQHandler_Callback);
	
}

/**
* @prototype FUN_TIMER23_Init(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 级联初始化.
*			
*/
void FUN_TIMER23_Init(void)
{
    TIMER_ConfigType timerConfig;
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = DISABLE;
    timerConfig.linkMode = DISABLE;
    timerConfig.loadValue = 191;
    TIMER_Init(TIMER2, &timerConfig);
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkMode = ENABLE;
    timerConfig.loadValue = 500;
    TIMER_Init(TIMER3, &timerConfig);
}

/**
* @prototype FUN_TIMER45_Init(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 级联初始化.
*			
*/
void FUN_TIMER45_Init(void)
{
    TIMER_ConfigType timerConfig;
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = DISABLE;
    timerConfig.linkMode = DISABLE;
    timerConfig.loadValue = 191;
    TIMER_Init(TIMER4, &timerConfig);
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkMode = ENABLE;
    timerConfig.loadValue = 500;
    TIMER_Init(TIMER5, &timerConfig);
}

/**
* @prototype FUN_TIMER67_Init(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 级联初始化.
*			
*/
void FUN_TIMER67_Init(void)
{
    TIMER_ConfigType timerConfig;
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = DISABLE;
    timerConfig.linkMode = DISABLE;
    timerConfig.loadValue = 191;
    TIMER_Init(TIMER6, &timerConfig);
	
    timerConfig.timerEn = DISABLE;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkMode = ENABLE;
    timerConfig.loadValue = 500;
    TIMER_Init(TIMER7, &timerConfig);
}

/**
* @prototype FUN_TIMER0_TrigeInt(void)
*
* @param[in] time:48M
* @return	 void
*
* @brief  	 time时间后触发Timer0中断.
*			
*/
void FUN_TIMER0_TrigeInt(dword time)
{
		TIMER0->INITVAL = time;
		TIMER0->INIT |= TIMER_INIT_TIE_Msk;
		TIMER0->INIT |= TIMER_INIT_TIMEREN_Msk;
}

void FUN_TIMER1_TrigeInt(dword time)
{
		TIMER1->INITVAL = time;
		TIMER1->INIT |= TIMER_INIT_TIE_Msk;
		TIMER1->INIT |= TIMER_INIT_TIMEREN_Msk;
}

/**
* @prototype FUN_TIMER3_Trige(void)
*
* @param[in] time:4us
* @return	 void
*
* @brief  	 time时间后触发Timer3中断.
*			
*/
void FUN_TIMER3_TrigeInt(word time)
{
		TIMER2->INITVAL = 191;
		TIMER3->INITVAL = time;
		TIMER2->INIT |= TIMER_INIT_TIE_Msk;
		TIMER3->INIT |= TIMER_INIT_TIE_Msk;
		TIMER2->INIT |= TIMER_INIT_TIMEREN_Msk;
		TIMER3->INIT |= TIMER_INIT_TIMEREN_Msk;
}

/**
* @prototype FUN_TIMER5_Trige(void)
*
* @param[in] time:4us
* @return	 void
*
* @brief  	 time时间后触发Timer5中断.
*			
*/
void FUN_TIMER5_TrigeInt(word time)
{
		TIMER4->INITVAL = 191;
		TIMER5->INITVAL = time;
		TIMER4->INIT |= TIMER_INIT_TIE_Msk;
		TIMER5->INIT |= TIMER_INIT_TIE_Msk;
		TIMER4->INIT |= TIMER_INIT_TIMEREN_Msk;
		TIMER5->INIT |= TIMER_INIT_TIMEREN_Msk;
}

/**
* @prototype FUN_TIMER7_Trige(void)
*
* @param[in] time:4us
* @return	 void
*
* @brief  	 time时间后触发Timer7中断.
*			
*/
void FUN_TIMER7_TrigeInt(word time)
{
		TIMER6->INITVAL = 191;
		TIMER7->INITVAL = time;
		TIMER6->INIT |= TIMER_INIT_TIE_Msk;
		TIMER7->INIT |= TIMER_INIT_TIE_Msk;
		TIMER6->INIT |= TIMER_INIT_TIMEREN_Msk;
		TIMER7->INIT |= TIMER_INIT_TIMEREN_Msk;
}
/**
* @prototype FUN_TIMER8_Trige(void)
*
* @param[in] time:4us
* @return	 void
*
* @brief  	 time时间后触发Timer8中断.
*			
*/
void FUN_TIMER8_TrigeInt(word time)
{
		PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL0, T_MAINTMR+time);
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL0);
}
void FUN_TIMER9_TrigeInt(word time)
{
		PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL2, T_MAINTMR+time);
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL2);
}
void FUN_TIMER10_TrigeInt(word time)
{
		PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL4, T_MAINTMR+time);
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
}
void FUN_TIMER11_TrigeInt(word time)
{
		PWM_SetChannelValue(PWM2, PWM_CHANNEL_CHANNEL5, T_MAINTMR+time);
		PWM_EnableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
}
/**
* @prototype FUN_TIMER0_DisAbleInt(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 禁止Timer0中断
*			 
*/
void FUN_TIMER0_DisAbleInt(void)
{
		TIMER0->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER0->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
}
void FUN_TIMER1_DisAbleInt(void)
{
		TIMER1->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER1->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
}
/**
* @prototype FUN_TIMER3_DisAbleInt(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 禁止Timer3中断
*			 
*/
void FUN_TIMER3_DisAbleInt(void)
{
		TIMER2->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER3->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER2->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
		TIMER3->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
}
/**
* @prototype FUN_TIMER5_DisAbleInt(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 禁止Timer5中断
*			 
*/
void FUN_TIMER5_DisAbleInt(void)
{
		TIMER4->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER5->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER4->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
		TIMER5->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
}
/**
* @prototype FUN_TIMER7_DisAbleInt(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 禁止Timer7中断
*			 
*/
void FUN_TIMER7_DisAbleInt(void)
{
		TIMER6->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER7->INIT &= ~ TIMER_INIT_TIE_Msk;
		TIMER6->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
		TIMER7->INIT &= ~ TIMER_INIT_TIMEREN_Msk;
}
/**
* @prototype FUN_TIMER8_DisAbleInt(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 禁止Timer8中断
*			 
*/
void FUN_TIMER8_DisAbleInt(void)
{
		PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL0);
}
void FUN_TIMER9_DisAbleInt(void)
{
		PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL2);
}
void FUN_TIMER10_DisAbleInt(void)
{
		PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL4);
}
void FUN_TIMER11_DisAbleInt(void)
{
		PWM_DisableChannelInt(PWM2, PWM_CHANNEL_CHANNEL5);
}





