
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("AutoChips Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
 * the prior written permission of AutoChips inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of AutoChips Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * AutoChips Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("AUTOCHIPS SOFTWARE")
 * RECEIVED FROM AUTOCHIPS AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. AUTOCHIPS EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES AUTOCHIPS PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE AUTOCHIPS SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN AUTOCHIPS
 * SOFTWARE. AUTOCHIPS SHALL ALSO NOT BE RESPONSIBLE FOR ANY AUTOCHIPS SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AUTOCHIPS'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE AUTOCHIPS SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT AUTOCHIPS'S OPTION, TO REVISE OR REPLACE THE
 * AUTOCHIPS SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO AUTOCHIPS FOR SUCH AUTOCHIPS SOFTWARE AT ISSUE.
 */
 
/*************<start>******************/

/**************************************************************************************************
	每个子例程对应着一个宏定义,使用GPIO_SAMPLE_SEL选择子例程.

	1. GPIO_BASIC_SAMPLE		基本输入输出
	2. GPIO_MUX_SAMPLE			引脚复用
	3. GPIO_EXTI_SAMPLE			外部中断
	4. GPIO_EXTI_WKUP_SAMPLE	外部中断唤醒MCU
	5. GPIO_NMI_WKUP_SAMPLE		非屏蔽中断唤醒MCU
	
	每个子例程都有详细的描述与配置宏,详细描述与配置宏见gpio.h.
	
 **************************************************************************************************/
 
/*************<include>****************/
//#include "gpio.h"
#include "FAM.h"

/*************<macro>******************/


/*************<enum>*******************/


/*************<union>******************/


/*************<struct>*****************/


/*************<variable>***************/



/*************<prototype>**************/


/**
* @prototype main(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 main entry.
*			 main函数.
*/
extern word t_lstWREE;
void ECU_main(void);
void TIMER_Interrupt_Control_Timer0(void);
void TIMER_Interrupt_Control_Timer1(void);
//void Can_Busoff_Reset(void);

int main(void)
{
	//CAN数据发送标志位   标志位的目的是使得CAN只发送一次就停止发送
		//word w;
	  unsigned int x=0;
	  unsigned int y=0;
	  unsigned int z=0;
	  unsigned int h=0;
	  unsigned int p=0;
	  unsigned int t=0;
	  unsigned int r=0;
	  unsigned int f=0;
	  unsigned int g=0;
	
	  Flag_end=0;     //启动喷射标志位
	  Flag_first=0;   //定时器计数启动标志位
	 
	
		InitDelay();       //初始化延时函数 
		//InitDebug();
		
		FUN_Hardwr_Init(); //硬件初始化
		FUN_Var_Init();    //变量初始化
	
#if(M_Debug == 0)
		ECU_main();
#else  
		for(;;)
		{	
			
		//Can_Busoff_Reset();			//CAN复位后泵的喷射会出现异常 暂时不适用此函数
		FUN_APP_Scheduler();   //APP程序调度
			 
		if((g_recvCANMsgInfo[0].ID==0X40)&&(g_recvCANMsgInfo[0].Data[0]==0X40)) //CAN 数据接收标志位
     {
				 g_sendCANMsgInfo.ID = 0X10;
			   g_sendCANMsgInfo.DLC = 8;
				 g_sendCANMsgInfo.IDE = 0;
				 g_sendCANMsgInfo.RTR = 0;			  
				 
        switch(g_recvCANMsgInfo[0].Data[1])  //根据上位机发送的信息来执行程序的调用
        {
          case 0x11 :  //启动开始命令
              { 																		 
							if(x==0)
							{
							 x=1;								
					     g_sendCANMsgInfo.Data[0]=0x10;
					     g_sendCANMsgInfo.Data[1]=0x44;
					     g_sendCANMsgInfo.Data[2]=0x00;
					     g_sendCANMsgInfo.Data[3]=0x00;
					     g_sendCANMsgInfo.Data[4]=0x00;
					     g_sendCANMsgInfo.Data[5]=0x00;
					     g_sendCANMsgInfo.Data[6]=0x00;
					     g_sendCANMsgInfo.Data[7]=0x00;
					     CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);						 
							}											 
              }
							break;
					case 0x12 :     //对泵的参数通过CAN信息进行赋值
              { 						
								T1_time=g_recvCANMsgInfo[0].Data[2];       //回位时间
								T2_time=g_recvCANMsgInfo[0].Data[3];       //做功时间
								T_time=g_recvCANMsgInfo[0].Data[4];        //喷射的频率
								Frequ_time=g_recvCANMsgInfo[0].Data[5];    //循环周期
								Spray_time=g_recvCANMsgInfo[0].Data[6];    //喷射次数  后两位(次数较多设置两个数据)
								Spray3_time=g_recvCANMsgInfo[0].Data[7];   //喷射次数  前两位
							
							if(t==0)
							{						                                //对泵的参数赋值后的变量利用CAN盒发出  方便后续调试使用
							 t=1;
					     g_sendCANMsgInfo.Data[0]=0x10;
					     g_sendCANMsgInfo.Data[1]=0x60;            //向上位机发送变量初始化命令
					     g_sendCANMsgInfo.Data[2]=T1_time; 
					     g_sendCANMsgInfo.Data[3]=T2_time;
					     g_sendCANMsgInfo.Data[4]=T_time;
					     g_sendCANMsgInfo.Data[5]=(Spray3_time*(0XFF+1)+Spray_time);
					     g_sendCANMsgInfo.Data[6]=Spray_time;
					     g_sendCANMsgInfo.Data[7]=Spray3_time;
					     CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
							}							              						 
              }
               break;   
							
          case 0x88 :     //启动喷的喷射 引脚反转放在中断 
              {																
							if(Flag_end==0)  //引脚反转标志位
							{	
								Flag_first=1;						  						 								  	
//								if(ADC_collect==1)    //电压采样放在处断中进行  保证时间的精度
//									{				              					              									              				          							              
//						        CAN_Send_adc();
//										mdelay(5);//延时
//					        	ADC_collect=0;	
//				          }		  						 
			        }
							else if((Flag_end==1)&&(y==0))    //到达设定的喷射次数 停止喷射
							{
							   y=1;							
							   z=0;						 
							   g_sendCANMsgInfo.Data[0]=0x10;   //上位机数据接收标志位
					       g_sendCANMsgInfo.Data[1]=0x62;   //向上位机发送停止指令  判断周期是否执行完 程序结束发送 0x99 否则发送0x88
					       g_sendCANMsgInfo.Data[2]=0x00;
					       g_sendCANMsgInfo.Data[3]=0x00;
					       g_sendCANMsgInfo.Data[4]=0x00;
					       g_sendCANMsgInfo.Data[5]=0x00;
					       g_sendCANMsgInfo.Data[6]=0x00;
					       g_sendCANMsgInfo.Data[7]=0x00;
					       CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY); 										
							}								
              }         
						break; 
									
					case 0x99 :  //喷射周期结束标志 发送停止指令
              { 
								
							 if(h==0)
								{
							 	 h=1;
                 z=0;
									
							   g_sendCANMsgInfo.Data[0]=0x10;
					       g_sendCANMsgInfo.Data[1]=0x64;    //向上位机发送停止指令
					       g_sendCANMsgInfo.Data[2]=0x00;
					       g_sendCANMsgInfo.Data[3]=0x00;
					       g_sendCANMsgInfo.Data[4]=0x00;
					       g_sendCANMsgInfo.Data[5]=0x00;
					       g_sendCANMsgInfo.Data[6]=0x00;
					       g_sendCANMsgInfo.Data[7]=0x00;
					       CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY); 							 							 
								
								}	             							
              }
							break;								
					case 0x40 :   //初始化程序变量 准备一个周期的喷射
					    {
                y=0;
                t=0;
                f=0;
                g=0;								
                ADC_collect=0; //电流采样标志位
                ID_count=0;		 //计数													
								Flag_first=0;  //开始喷射
								Flag_end=0;	   
								
								if(z==0)
								{
							 	 z=1;				
							   g_sendCANMsgInfo.Data[0]=0x10;
					       g_sendCANMsgInfo.Data[1]=0x65;    //初始化结束向上位机发送启动请求命令
					       g_sendCANMsgInfo.Data[2]=0x00;
					       g_sendCANMsgInfo.Data[3]=0x00;
					       g_sendCANMsgInfo.Data[4]=0x00;
					       g_sendCANMsgInfo.Data[5]=0x00;
					       g_sendCANMsgInfo.Data[6]=0x00;
					       g_sendCANMsgInfo.Data[7]=0x00;
					       CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY); 							 							 
								}			            																					
		          }	
            break;	
					 case 0x22 : 
              {
                if(g==0)
								{
                 g=1;									
							   g_sendCANMsgInfo.Data[0]=0x10;
					       g_sendCANMsgInfo.Data[1]=0x63;  
					       g_sendCANMsgInfo.Data[2]=0x00;
					       g_sendCANMsgInfo.Data[3]=0x00;
					       g_sendCANMsgInfo.Data[4]=0x00;
					       g_sendCANMsgInfo.Data[5]=0x00;
					       g_sendCANMsgInfo.Data[6]=0x00;
					       g_sendCANMsgInfo.Data[7]=0x00;
					       CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY); 	
								}									
              }
							break;
							
					  case 0x33 : 
              { 							
							   T1_time=g_recvCANMsgInfo[0].Data[2];       //回位时间
								 T2_time=g_recvCANMsgInfo[0].Data[3];       //做功时间
								 T_time=g_recvCANMsgInfo[0].Data[4];        //喷射的频率
								 Frequ_time=g_recvCANMsgInfo[0].Data[5];    //循环周期
								 Spray_time=g_recvCANMsgInfo[0].Data[6];    //喷射次数  后两位(次数较多设置两个数据)
								 Spray3_time=g_recvCANMsgInfo[0].Data[7];   //喷射次数  前两位
                if(f==0)
								{
								 f=1;
               g_sendCANMsgInfo.Data[0]=0x10;
					     g_sendCANMsgInfo.Data[1]=0x66;            //向上位机发送变量初始化命令
					     g_sendCANMsgInfo.Data[2]=T1_time; 
					     g_sendCANMsgInfo.Data[3]=T2_time;
					     g_sendCANMsgInfo.Data[4]=T_time;
					     g_sendCANMsgInfo.Data[5]=(Spray3_time*(0XFF+1)+Spray_time);
					     g_sendCANMsgInfo.Data[6]=Spray_time;
					     g_sendCANMsgInfo.Data[7]=Spray3_time;
					     CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);			
								}									
              }
							break;
							
							
					 case 0x55 :    //初始化程序结构主函数的执行 退出循环 初始化程序变量
					 {
						 
						 x=0;
						 y=0;	
             z=0;
             h=0;
             p=0;
             t=0;
						 r=0;
             g=0;
             f=0;						 
						 Flag_end=1;
						 ADC_collect=0;
						 
						 return 0;					 //退出			 
					 }				 				 
						 break;
					}  //switch
		} //if 							  			
		} //for
#endif
} //main函数

void CAN_Send_adc(void) //向上位机发送采样数据
{
	  g_sendCANMsgInfo.Data[0]=0x10;
	  g_sendCANMsgInfo.Data[1]=0x61;         //上位机接收数据标志位
	  g_sendCANMsgInfo.Data[2]=Electric>>8;  //电流
		g_sendCANMsgInfo.Data[3]=Electric; 
		g_sendCANMsgInfo.Data[4]=Volt>>8;      //电压
		g_sendCANMsgInfo.Data[5]=Volt;   
		g_sendCANMsgInfo.Data[6]=Electric_T2>>8; //电流T2
		g_sendCANMsgInfo.Data[7]=Electric_T2;    //
		CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
  
	  g_sendCANMsgInfo.Data[0]=0x10;
	  g_sendCANMsgInfo.Data[1]=0x51;          //上位机接收数据标志位
	  g_sendCANMsgInfo.Data[2]=Temp>>8;       //温度
		g_sendCANMsgInfo.Data[3]=Temp;          
		g_sendCANMsgInfo.Data[4]=Pressure>>8;   //压力
		g_sendCANMsgInfo.Data[5]=Pressure;      
		g_sendCANMsgInfo.Data[6]=ID_count;      //记录CAN帧数
		g_sendCANMsgInfo.Data[7]=ID_count;     
		CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
    	
}


//CAN高低短接时 CAN会复位操作 
void Can_Busoff_Reset(void)
{
	if((CAN2->BIT.RESET)&&(CAN2->BIT.BUSOFF==0))
	{
		CAN2->BIT.RESET=0;
		CAN2->REG.CFG_STAT=TPSS;
		CAN2->REG.CFG_STAT=TPSS;
	}
}

void FUN_APP_Scheduler(void)      //APP总调度
{
	    FUN_ADC_APP_Scheduler();
			FUN_SCI_APP_Scheduler();					CLR_WDG2
			FUN_InjIng_APP_Scheduler();
			FUN_CAN_APP_Scheduler();					CLR_WDG2
			FUN_IIC_APP_Scheduler();
			FUN_SPI_APP_Scheduler();
			CLR_WDG2	
}

void FUN_Hardwr_Init(void) //硬件初始化
{
	  
		#if(M_DebugApp == 0)
		//DisableInterrupts
		//SCB->VTOR = M_AppStartAddr;
		//EnableInterrupts		
		#endif 
		RTC_Timeoutxus(500);
		FUN_GPIO_InitHardwr();
		FUN_ADC_InitHardwr();
		FUN_UART_InitHardwr();
		FUN_InjIng_InitHardwr();
		FUN_CAN_InitHardwr();
		FUN_IIC_InitHardwr();
		FUN_SPI_InitHardwr();
	  #if(M_DebugApp == 0)
	  WDG_InitHardwr();
	  #endif
	  TIMER_Interrupt_Control_Timer0();  //定时器中断
    TIMER_Interrupt_Control_Timer1(); 
    	
}

void FUN_Var_Init(void)  //变量初始化
{
		SCI_InitVar();
		ADC_InitVar();
		InjIng_InitVar();
		CAN_InitVar();
		IIC_InitVar();
}

/*************<end>********************/
