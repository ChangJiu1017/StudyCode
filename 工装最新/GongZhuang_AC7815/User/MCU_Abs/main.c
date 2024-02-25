
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
	ÿ�������̶�Ӧ��һ���궨��,ʹ��GPIO_SAMPLE_SELѡ��������.

	1. GPIO_BASIC_SAMPLE		�����������
	2. GPIO_MUX_SAMPLE			���Ÿ���
	3. GPIO_EXTI_SAMPLE			�ⲿ�ж�
	4. GPIO_EXTI_WKUP_SAMPLE	�ⲿ�жϻ���MCU
	5. GPIO_NMI_WKUP_SAMPLE		�������жϻ���MCU
	
	ÿ�������̶�����ϸ�����������ú�,��ϸ���������ú��gpio.h.
	
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
*			 main����.
*/
extern word t_lstWREE;
void ECU_main(void);
void TIMER_Interrupt_Control_Timer0(void);
void TIMER_Interrupt_Control_Timer1(void);
//void Can_Busoff_Reset(void);

int main(void)
{
	//CAN���ݷ��ͱ�־λ   ��־λ��Ŀ����ʹ��CANֻ����һ�ξ�ֹͣ����
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
	
	  Flag_end=0;     //���������־λ
	  Flag_first=0;   //��ʱ������������־λ
	 
	
		InitDelay();       //��ʼ����ʱ���� 
		//InitDebug();
		
		FUN_Hardwr_Init(); //Ӳ����ʼ��
		FUN_Var_Init();    //������ʼ��
	
#if(M_Debug == 0)
		ECU_main();
#else  
		for(;;)
		{	
			
		//Can_Busoff_Reset();			//CAN��λ��õ����������쳣 ��ʱ�����ô˺���
		FUN_APP_Scheduler();   //APP�������
			 
		if((g_recvCANMsgInfo[0].ID==0X40)&&(g_recvCANMsgInfo[0].Data[0]==0X40)) //CAN ���ݽ��ձ�־λ
     {
				 g_sendCANMsgInfo.ID = 0X10;
			   g_sendCANMsgInfo.DLC = 8;
				 g_sendCANMsgInfo.IDE = 0;
				 g_sendCANMsgInfo.RTR = 0;			  
				 
        switch(g_recvCANMsgInfo[0].Data[1])  //������λ�����͵���Ϣ��ִ�г���ĵ���
        {
          case 0x11 :  //������ʼ����
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
					case 0x12 :     //�ԱõĲ���ͨ��CAN��Ϣ���и�ֵ
              { 						
								T1_time=g_recvCANMsgInfo[0].Data[2];       //��λʱ��
								T2_time=g_recvCANMsgInfo[0].Data[3];       //����ʱ��
								T_time=g_recvCANMsgInfo[0].Data[4];        //�����Ƶ��
								Frequ_time=g_recvCANMsgInfo[0].Data[5];    //ѭ������
								Spray_time=g_recvCANMsgInfo[0].Data[6];    //�������  ����λ(�����϶�������������)
								Spray3_time=g_recvCANMsgInfo[0].Data[7];   //�������  ǰ��λ
							
							if(t==0)
							{						                                //�ԱõĲ�����ֵ��ı�������CAN�з���  �����������ʹ��
							 t=1;
					     g_sendCANMsgInfo.Data[0]=0x10;
					     g_sendCANMsgInfo.Data[1]=0x60;            //����λ�����ͱ�����ʼ������
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
							
          case 0x88 :     //����������� ���ŷ�ת�����ж� 
              {																
							if(Flag_end==0)  //���ŷ�ת��־λ
							{	
								Flag_first=1;						  						 								  	
//								if(ADC_collect==1)    //��ѹ�������ڴ����н���  ��֤ʱ��ľ���
//									{				              					              									              				          							              
//						        CAN_Send_adc();
//										mdelay(5);//��ʱ
//					        	ADC_collect=0;	
//				          }		  						 
			        }
							else if((Flag_end==1)&&(y==0))    //�����趨��������� ֹͣ����
							{
							   y=1;							
							   z=0;						 
							   g_sendCANMsgInfo.Data[0]=0x10;   //��λ�����ݽ��ձ�־λ
					       g_sendCANMsgInfo.Data[1]=0x62;   //����λ������ָֹͣ��  �ж������Ƿ�ִ���� ����������� 0x99 ������0x88
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
									
					case 0x99 :  //�������ڽ�����־ ����ָֹͣ��
              { 
								
							 if(h==0)
								{
							 	 h=1;
                 z=0;
									
							   g_sendCANMsgInfo.Data[0]=0x10;
					       g_sendCANMsgInfo.Data[1]=0x64;    //����λ������ָֹͣ��
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
					case 0x40 :   //��ʼ��������� ׼��һ�����ڵ�����
					    {
                y=0;
                t=0;
                f=0;
                g=0;								
                ADC_collect=0; //����������־λ
                ID_count=0;		 //����													
								Flag_first=0;  //��ʼ����
								Flag_end=0;	   
								
								if(z==0)
								{
							 	 z=1;				
							   g_sendCANMsgInfo.Data[0]=0x10;
					       g_sendCANMsgInfo.Data[1]=0x65;    //��ʼ����������λ������������������
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
							   T1_time=g_recvCANMsgInfo[0].Data[2];       //��λʱ��
								 T2_time=g_recvCANMsgInfo[0].Data[3];       //����ʱ��
								 T_time=g_recvCANMsgInfo[0].Data[4];        //�����Ƶ��
								 Frequ_time=g_recvCANMsgInfo[0].Data[5];    //ѭ������
								 Spray_time=g_recvCANMsgInfo[0].Data[6];    //�������  ����λ(�����϶�������������)
								 Spray3_time=g_recvCANMsgInfo[0].Data[7];   //�������  ǰ��λ
                if(f==0)
								{
								 f=1;
               g_sendCANMsgInfo.Data[0]=0x10;
					     g_sendCANMsgInfo.Data[1]=0x66;            //����λ�����ͱ�����ʼ������
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
							
							
					 case 0x55 :    //��ʼ������ṹ��������ִ�� �˳�ѭ�� ��ʼ���������
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
						 
						 return 0;					 //�˳�			 
					 }				 				 
						 break;
					}  //switch
		} //if 							  			
		} //for
#endif
} //main����

void CAN_Send_adc(void) //����λ�����Ͳ�������
{
	  g_sendCANMsgInfo.Data[0]=0x10;
	  g_sendCANMsgInfo.Data[1]=0x61;         //��λ���������ݱ�־λ
	  g_sendCANMsgInfo.Data[2]=Electric>>8;  //����
		g_sendCANMsgInfo.Data[3]=Electric; 
		g_sendCANMsgInfo.Data[4]=Volt>>8;      //��ѹ
		g_sendCANMsgInfo.Data[5]=Volt;   
		g_sendCANMsgInfo.Data[6]=Electric_T2>>8; //����T2
		g_sendCANMsgInfo.Data[7]=Electric_T2;    //
		CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
  
	  g_sendCANMsgInfo.Data[0]=0x10;
	  g_sendCANMsgInfo.Data[1]=0x51;          //��λ���������ݱ�־λ
	  g_sendCANMsgInfo.Data[2]=Temp>>8;       //�¶�
		g_sendCANMsgInfo.Data[3]=Temp;          
		g_sendCANMsgInfo.Data[4]=Pressure>>8;   //ѹ��
		g_sendCANMsgInfo.Data[5]=Pressure;      
		g_sendCANMsgInfo.Data[6]=ID_count;      //��¼CAN֡��
		g_sendCANMsgInfo.Data[7]=ID_count;     
		CAN_MessageSend(CAN2, &g_sendCANMsgInfo, TRANSMIT_SECONDARY);
    	
}


//CAN�ߵͶ̽�ʱ CAN�Ḵλ���� 
void Can_Busoff_Reset(void)
{
	if((CAN2->BIT.RESET)&&(CAN2->BIT.BUSOFF==0))
	{
		CAN2->BIT.RESET=0;
		CAN2->REG.CFG_STAT=TPSS;
		CAN2->REG.CFG_STAT=TPSS;
	}
}

void FUN_APP_Scheduler(void)      //APP�ܵ���
{
	    FUN_ADC_APP_Scheduler();
			FUN_SCI_APP_Scheduler();					CLR_WDG2
			FUN_InjIng_APP_Scheduler();
			FUN_CAN_APP_Scheduler();					CLR_WDG2
			FUN_IIC_APP_Scheduler();
			FUN_SPI_APP_Scheduler();
			CLR_WDG2	
}

void FUN_Hardwr_Init(void) //Ӳ����ʼ��
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
	  TIMER_Interrupt_Control_Timer0();  //��ʱ���ж�
    TIMER_Interrupt_Control_Timer1(); 
    	
}

void FUN_Var_Init(void)  //������ʼ��
{
		SCI_InitVar();
		ADC_InitVar();
		InjIng_InitVar();
		CAN_InitVar();
		IIC_InitVar();
}

/*************<end>********************/
