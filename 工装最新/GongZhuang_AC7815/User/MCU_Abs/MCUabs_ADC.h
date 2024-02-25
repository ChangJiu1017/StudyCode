
#include "FAM.h"
#ifndef __MCUABS_ADC_H__
#define __MCUABS_ADC_H__


//PA6 	ADC10					FUNC1			电机1电流
//PA7 	ADC0 					FUNC1			电池电压
//PA8 	ADC1 					FUNC1			电机2电流
//PA9 	ADC2 					FUNC1			线氧或油位
//PA10 	ADC3 					FUNC1			档位
//PA11 	ADC4 					FUNC1			倾倒
//PA12 	ADC5 					FUNC1			主继电器电压
//PA13 	ADC6 					FUNC1			节气门位置
//PA14 	ADC7 					FUNC1			压力
//PA15 	ADC8 					FUNC1			缸头温度
//PB0 	ADC9					FUNC1			氧传感器1
//PB1 	ADC11					FUNC1			进气温度

#define Vbat_ch									ADC_CHANNEL_AD0//电池电压
#define Motor2Curr_ch						ADC_CHANNEL_AD1//电机2电流
#define FuelPosition_ch					ADC_CHANNEL_AD2//线氧或油位
#define GearPosition_ch					ADC_CHANNEL_AD3//档位
#define FallRate_ch							ADC_CHANNEL_AD4//倾倒
#define MainRalayV_ch						ADC_CHANNEL_AD5//主继电器电压
#define TPS_ch									ADC_CHANNEL_AD6//节气门位置
#define Alt_ch									ADC_CHANNEL_AD7//海拔
#define Teng_ch									ADC_CHANNEL_AD8//缸头温度
#define O2Sensor_ch							ADC_CHANNEL_AD9//氧传感器1
#define Motor1Curr_ch						ADC_CHANNEL_AD10//电机1电流
#define Amb_ch									ADC_CHANNEL_AD11//进气温度
	


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





























