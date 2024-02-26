
#include "FAM.h"
#ifndef __MCUABS_ADC_H__
#define __MCUABS_ADC_H__


//PA6 	ADC10					FUNC1			���1����
//PA7 	ADC0 					FUNC1			��ص�ѹ
//PA8 	ADC1 					FUNC1			���2����
//PA9 	ADC2 					FUNC1			��������λ
//PA10 	ADC3 					FUNC1			��λ
//PA11 	ADC4 					FUNC1			�㵹
//PA12 	ADC5 					FUNC1			���̵�����ѹ
//PA13 	ADC6 					FUNC1			������λ��
//PA14 	ADC7 					FUNC1			ѹ��
//PA15 	ADC8 					FUNC1			��ͷ�¶�
//PB0 	ADC9					FUNC1			��������1
//PB1 	ADC11					FUNC1			�����¶�

#define Vbat_ch									ADC_CHANNEL_AD0//��ص�ѹ
#define Motor2Curr_ch						ADC_CHANNEL_AD1//���2����
#define FuelPosition_ch					ADC_CHANNEL_AD2//��������λ
#define GearPosition_ch					ADC_CHANNEL_AD3//��λ
#define FallRate_ch							ADC_CHANNEL_AD4//�㵹
#define MainRalayV_ch						ADC_CHANNEL_AD5//���̵�����ѹ
#define TPS_ch									ADC_CHANNEL_AD6//������λ��
#define Alt_ch									ADC_CHANNEL_AD7//����
#define Teng_ch									ADC_CHANNEL_AD8//��ͷ�¶�
#define O2Sensor_ch							ADC_CHANNEL_AD9//��������1
#define Motor1Curr_ch						ADC_CHANNEL_AD10//���1����
#define Amb_ch									ADC_CHANNEL_AD11//�����¶�
	


extern byte ADC_debug;
extern word RT_ch0;
extern word RT_ch1;
extern word RT_ch2;
extern word RT_ch3;
extern word RT_ch4;
extern word RT_ch5;
extern word RT_ch6;
extern word RT_ch7;
extern word RT_ch8;
extern word RT_ch9;
extern word RT_ch10;
extern word RT_ch11;

extern void FUN_ADC_InitHardwr(void);
extern void ADC_InitVar(void);
extern word FAUN_ADS2S128(byte Channel,byte bln8bit,byte times);
extern word FUN_ADSample(uint32_t chn);
extern void FUN_ADC_APP_Scheduler(void);

#endif





























