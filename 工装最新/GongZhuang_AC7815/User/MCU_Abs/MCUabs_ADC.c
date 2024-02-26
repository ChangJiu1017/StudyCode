
#include "MCUabs_ADC.h"


byte ADC_debug;
word RT_ch7;
word RT_ch8;
word RT_ch9;
word RT_ch10;
word RT_ch11;
word RT_ch12;
word id_time=0;


/**
* @prototype FUN_ADC_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize ADC module.
*			 ��ʼ��UARTģ��.
*/
void FUN_ADC_InitHardwr(void)
{
    ADC_InitType tempAdcConfig = {0, 0, 0, 0, 0, 0, 0};  //ADC����ģʽ�ṹ��
    ADC_TrigSourceType tempAdcTrigSource = {0, 0};       //ADC����Դ���ڲ����ⲿ���ṹ�� ��Ĭ���ڲ�����
    ADC_InitType* adcConfig;
    ADC_TrigSourceType* adcTrigSource;
    adcConfig = &tempAdcConfig;
    adcTrigSource = &tempAdcTrigSource;
    //ADC ADC_CTRL1[15:11]����Ϊ11000��mode5:�������ͨ������ɨ�裬��ע�봥����
    //���򴥷�������������ͨ������ת����ע�봥��������ע����ͨ������ת������Ч�Ĺ����ע����ͨ����RSQL��ISQL������
    //��ͬģʽ�Ĺ��ܼ����ã��ɲο�AC781X�ο��ֲ��AC781X ADCģ��Ӧ�ñʼ��ĵ���
    adcConfig->scanMode = 0;//ADC_Regular_Single_One_Mode;//0;//1;//ɨ��ģʽ
    adcConfig->continousMode = 0;//1;//����ģʽ
    adcConfig->disContinousModeOnRegularGroup = 0;//1���򿪹�������ת��ģʽ
    adcConfig->disContinousModeOnInjectGroup = 0;//1����ע������ת��ģʽ
    adcConfig->injectAutoMode = 0;//1���Զ�ע��ģʽ
    //����PINMUX
		
//PA10  ADC3   MAP2_AD_MCU   ѹ����� 2
//PA11  ADC4   MAP1_AD_MCU   ѹ����� 1

//PA12  ADC5   AD_PZT1       �¶ȼ��T1
//PA13  ADC5   AD_PZT2       �¶ȼ��T2

//PA14  ADC7   AD_VBAT1      ��ѹ���

//PA15  ADC8   linj_AD_MCU   �������ŵ�ѹ  (ת��Ϊ����ֵ) //��������Ϊ10mR
//PB0   ADC9   linj_AD_MCU T2   �������ŵ�ѹ  (ת��Ϊ����ֵ) //��������Ϊ10mR

    //GPIO_SetFunc(GPIO_PA7, GPIO_FUNC_1);                            ///<ADC_IN10 Analog function enable
    //GPIO_SetFunc(GPIO_PA7, GPIO_FUNC_1);                            ///<ADC_IN0 Analog function enable
   // GPIO_SetFunc(GPIO_PA8, GPIO_FUNC_1);                            ///<ADC_IN1 Analog function enable
   //GPIO_SetFunc(GPIO_PA9, GPIO_FUNC_1);                            ///<ADC_IN2 Analog function enable
    GPIO_SetFunc(GPIO_PA10, GPIO_FUNC_1);                           ///<ADC_IN3 Analog function enable
    GPIO_SetFunc(GPIO_PA11, GPIO_FUNC_1);                           ///<ADC_IN4 Analog function enable
    GPIO_SetFunc(GPIO_PA12, GPIO_FUNC_1);                           ///<ADC_IN5 Analog function enable
    GPIO_SetFunc(GPIO_PA13, GPIO_FUNC_1);                           ///<ADC_IN6 Analog function enable
    GPIO_SetFunc(GPIO_PA14, GPIO_FUNC_1);                           ///<ADC_IN7 Analog function enable
    GPIO_SetFunc(GPIO_PA15, GPIO_FUNC_1);                           ///<ADC_IN8 Analog function enable
    GPIO_SetFunc(GPIO_PB0, GPIO_FUNC_1);                           	///<ADC_IN9 Analog function enable
    //PIO_SetFunc(GPIO_PB1, GPIO_FUNC_1);                           	///<ADC_IN11 Analog function enable
    ADC_Init(ADC, adcConfig);                      ///<ADC works Mode Config
    
    ADC_SetClockPrescaler(ADC, 4);                 ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_TrigSourceInit(ADC, adcTrigSource);
		
    //ADC_SetRegularGroupLength(ADC, 1);
		//ADC_SetRegularGroupLength(ADC, 6);             //setting 6 ADC channel sample
    //ADC_SetRegularGroupSequence(ADC, 1, ADC_CHANNEL_AD0); //set ADC_CHANNEL0 for AD Sample
    //ADC_SetRegularGroupSequence(ADC, 2, ADC_CHANNEL_AD1); //set ADC_CHANNEL1 for AD Sample
    //ADC_SetRegularGroupSequence(ADC, 3, ADC_CHANNEL_AD2); //set ADC_CHANNEL2 for AD Sample
    //ADC_SetRegularGroupSequence(ADC, 4, ADC_CHANNEL_AD3); //set ADC_CHANNEL3 for AD Sample
    //ADC_SetRegularGroupSequence(ADC, 5, ADC_CHANNEL_AD4); //set ADC_CHANNEL4 for AD Sample
    //ADC_SetRegularGroupSequence(ADC, 6, ADC_CHANNEL_AD5); //set ADC_CHANNEL5 for AD Sample
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD0, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD1, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD2, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD3, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD4, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD5, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_Cmd(ADC, ENABLE);
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD0, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD1, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD2, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD3, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD4, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD5, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD0, ADC_SampleTime_14Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD6, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD7, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD8, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD9, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
    //ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD10, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
		//ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD11, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)
//    ADC_ChannelSampleTimeSel(ADC, ADC_CHANNEL_AD12, ADC_SampleTime_6Cycle); ///<Set ADC Sample Rate 369K = 96M/2/(4+1)/(14+12)

}


/**
* @prototype ADC_InitVar(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize ADC Var module.
*			 ��ʼ��ADCģ�����.
*/

void ADC_InitVar(void)
{
	ADC_debug = 8;
	RT_ch7 = 0;
	RT_ch8 = 0;
	RT_ch9 = 0;
	RT_ch10 = 0;
	RT_ch11 = 0;
	RT_ch12 = 0;
}

/**
 * FUN_ADSample
 *
 * @param[in] chn:������
 * @return RT_V��0xFFFF������ ����������
 *
 * @brief ͨ���������������
 */

word FAUN_ADS2S128(byte Channel,byte bln8bit,byte times)
{
		word temp = 0;
    word w = 0;
		for(byte i=0;i<times;i++){
				temp = FUN_ADSample(Channel);
				if(temp>=0x400)
				{
						return 0;
				}
				else
				{
						w += temp;
				}
		}
		w/=times;
		if(bln8bit){w >>= 2;}
		return w;
}


word FUN_ADSample(uint32_t chn)
{
		word RT_V = 0;
	  word count = 0;
		ADC_ClearFlag(ADC, ADC_FLAG_EOC);
		ADC_SetRegularGroupSequence(ADC, 1, chn);
		ADC_SoftwareStartRegularConvCmd(ADC, ADC_ENABLE);
		while (!ADC_GetIntFlag(ADC, ADC_FLAG_EOC))
		{
				if((count ++) > 60000)
				{
						return (0xFFFF);
				}
		}
		RT_V = ADC_GetRegularConversionValue(ADC);
		return (RT_V >> 2);
}


/**
* @prototype FUN_ADC_APP_Scheduler
*
* @param[in] ...
* @return	 ...
*
* @brief  	 ADC֮APP���ܵ���
*			 
*/

void FUN_ADC_APP_Scheduler(void)
{
	
		if(ADC_debug == 7)
		{
			RT_ch7 = FUN_ADSample(0);
		}
		if(ADC_debug == 8)
		{
			RT_ch8 = FUN_ADSample(1);
		}
		if(ADC_debug == 9)
		{
			RT_ch9 = FUN_ADSample(2);
		}
		if(ADC_debug == 10)
		{
			RT_ch10 = FUN_ADSample(3);
		}
		if(ADC_debug == 11)
		{
			RT_ch11 = FUN_ADSample(4);
		}
		if(ADC_debug == 12)
		{
			RT_ch12 = FUN_ADSample(5);
		}
}


