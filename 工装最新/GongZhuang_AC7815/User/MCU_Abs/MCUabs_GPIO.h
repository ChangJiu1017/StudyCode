

#ifndef __MCUABS_GPIO_H__
#define __MCUABS_GPIO_H__
#include "FAM.h"

/*************<enum>*******************/
typedef enum
{
	GPIO_INPUT	= 0,
	GPIO_OUTPUT	= 1
}GPIO_Dir_TypeDef;

typedef enum
{
	GPIO_FUNC_0	= 0,
	GPIO_FUNC_1	= 1,
	GPIO_FUNC_2	= 2,
	GPIO_FUNC_3	= 3
}GPIO_Func_TypeDef;

typedef enum
{
	KEY_UNDOWN = 0,
	KEY_DOWN = 1,
}KEY_Action_TypeDef;



	
#define M_GPIO_4mA						0
#define M_GPIO_8mA						1
#define M_GPIO_12mA						2
#define M_GPIO_16mA						3



#define M_AddAirV_CPIN					(GPIO_PC1)//补气阀控制
#define M_AddAirV_C							(GPIOC_OUT(1))//补气阀控制
#define M_AddAirV_C_ON					M_AddAirV_C = 1;
#define M_AddAirV_C_OFF					M_AddAirV_C = 0;
#define M_AddAirV_C_TOGGLE			M_AddAirV_C = !M_AddAirV_C;
#define M_AddAirV_CDriCurr(x)		GPIO_SetE4E2(M_AddAirV_CPIN, x)

#define M_AddAirV_TPIN					(GPIO_PB4)//补气阀检测
#define M_AddAirV_T							(GPIOB_IN(4))//补气阀检测
#define M_AddAirV_TDriCurr(x)		GPIO_SetE4E2(M_KIGNI_TPIN, x)

#define M_MoTorLink_TPIN				(GPIO_PB5)//电机连接检测
#define M_MoTorLink_T						(GPIOB_IN(5))//电机连接检测
#define M_MoTorLink_TDriCurr(x)	GPIO_SetE4E2(M_Inj1_OPPIN, x)

#define M_BumpO2_CPIN						(GPIO_PC3)//泵氧控制
#define M_BumpO2_C							(GPIOC_OUT(3))//泵氧控制
#define M_BumpO2_C_ON						M_BumpO2_C = 1;
#define M_BumpO2_C_OFF					M_BumpO2_C = 0;
#define M_BumpO2_C_TOGGLE				M_BumpO2_C = !M_BumpO2_C;
#define M_BumpO2_CDriCurr(x)		GPIO_SetE4E2(M_BumpO2_CPIN, x)

#define M_CC_TPIN								(GPIO_PB8)//侧撑开关检测
#define M_CC_T									(GPIOB_IN(8))//侧撑开关检测
#define M_CC_TDriCurr(x)				GPIO_SetE4E2(M_CC_TPIN, x)

#define M_FeulBump_CPIN					(GPIO_PC4)//油泵控制
#define M_FeulBump_C						(GPIOC_OUT(4))//油泵控制
//#define M_FeulBump_C_ST					(GPIOC_IN(4))//油泵控制状态
#define M_FeulBump_C_ON					M_FeulBump_C = 1;
#define M_FeulBump_C_OFF				M_FeulBump_C = 0;
#define M_FeulBump_C_TOGGLE			M_FeulBump_C = !M_FeulBump_C;
#define M_FeulBump_CDriCurr(x)	GPIO_SetE4E2(M_FeulBump_CPIN, x)

#define M_CTank_CPIN						(GPIO_PC2)//碳罐控制
#define M_CTank_C								(GPIOC_OUT(2))//碳罐控制
#define M_CTank_C_ON						M_CTank_C = 1;
#define M_CTank_C_OFF						M_CTank_C = 0;
#define M_CTank_C_TOGGLE				M_CTank_C = !M_CTank_C;
#define M_CTank_CDriCurr(x)			GPIO_SetE4E2(M_CTank_CPIN, x)

#define M_O2Heat_CPIN						(GPIO_PB15)//氧传感器加热控制
#define M_O2Heat_C							(GPIOB_OUT(15))//氧传感器加热控制
#define M_O2Heat_C_ST						(GPIOB_IN(15))//氧传感器加热状态
#define M_O2Heat_C_ON						M_O2Heat_C = 1;
#define M_O2Heat_C_OFF					M_O2Heat_C = 0;
#define M_O2Heat_C_TOGGLE				M_O2Heat_C = !M_O2Heat_C;
#define M_O2Heat_CDriCurr(x)		GPIO_SetE4E2(M_O2Heat_CPIN, x)

#define M_KeySTPIN							(GPIO_PB12)//钥匙检测
#define M_KeyST									(GPIOB_IN(12))//钥匙检测
#define M_KeySTDriCurr(x)				GPIO_SetE4E2(M_KeySTPIN, x)

#define M_MotorEN1PIN						(GPIO_PC12)//电机电流选择1
#define M_MotorEN1							(GPIOC_OUT(12))//电机电流选择1
#define M_MotorEN1_ON						M_MotorEN1 = 1;
#define M_MotorEN1_OFF					M_MotorEN1 = 0;
#define M_MotorEN1_TOGGLE				M_MotorEN1 = !M_MotorEN1;
#define M_MotorEN1_DriCurr(x)		GPIO_SetE4E2(M_MotorEN1PIN, x)

#define M_MotorEN2PIN						(GPIO_PC13)//电机电流选择2
#define M_MotorEN2							(GPIOC_OUT(13))//电机电流选择2
#define M_MotorEN2_ON						M_MotorEN2 = 1;
#define M_MotorEN2_OFF					M_MotorEN2 = 0;
#define M_MotorEN2_TOGGLE				M_MotorEN2 = !M_MotorEN2;
#define M_MotorEN2_DriCurr(x)		GPIO_SetE4E2(M_MotorEN2PIN, x)

#define M_MotorPh1PIN						(GPIO_PC14)//电机方向1
#define M_MotorPh1							(GPIOC_OUT(14))//电机方向1
#define M_MotorPh1_ON						M_MotorPh1 = 1;
#define M_MotorPh1_OFF					M_MotorPh1 = 0;
#define M_MotorPh1_TOGGLE				M_MotorPh1 = !M_MotorPh1;
#define M_MotorPh1_DriCurr(x)		GPIO_SetE4E2(M_MotorPh1PIN, x)

#define M_MotorPh2PIN						(GPIO_PC15)//电机方向2
#define M_MotorPh2							(GPIOC_OUT(15))//电机方向2
#define M_MotorPh2_ON						M_MotorPh2 = 1;
#define M_MotorPh2_OFF					M_MotorPh2 = 0;
#define M_MotorPh2_TOGGLE				M_MotorPh2 = !M_MotorPh2;
#define M_MotorPh2_DriCurr(x)		GPIO_SetE4E2(M_MotorPh2PIN, x)

///////////////////////////////////////////////////
#define M_PB15nonePIN						(GPIO_PB15)//none
#define M_PB15none							(GPIOB_IN(15))//none
#define M_PB15noneDriCurr(x)		GPIO_SetE4E2(M_PB15nonePIN, x)

#define M_PC0nonePIN						(GPIO_PC0)//none
#define M_PC0none								(GPIOC_OUT(0))//none
#define M_PC0noneDriCurr(x)			GPIO_SetE4E2(M_PC0nonePIN, x)

//#define M_PC2nonePIN						(GPIO_PC2)//none
//#define M_PC2none								(GPIOC_OUT(2))//none
//#define M_PC2noneDriCurr(x)			GPIO_SetE4E2(M_PC1nonePIN, x)

#define M_PC8nonePIN						(GPIO_PC8)//none
#define M_PC8none								(GPIOC_IN(8))//none
#define M_PC8noneDriCurr(x)			GPIO_SetE4E2(M_PC8nonePIN, x)



//PA0 	out						FUNC0			33813的SPI片选信号				iic模块
//PA1 	SPI1_SCK			FUNC1			33813的SPI时钟						iic模块
//PA2 	SPI1_MISO			FUNC1			33813的SPI主机入					iic模块
//PA3 	SPI1_MOSI			FUNC1			33813的SPI主机出					iic模块
//PA4 	CANRX					FUNC2			CAN接收							ok		can模块
//PA5 	CANTX					FUNC2			CAN发送							ok		can模块	
//PA6 	ADC10					FUNC1			电机1电流						ok		adc模块
//PA7 	ADC0 					FUNC1			电池电压						ok		adc模块
//PA8 	ADC1 					FUNC1			电机2电流						ok		adc模块
//PA9 	ADC2 					FUNC1			线氧或油位					ok		adc模块
//PA10 	ADC3 					FUNC1			档位								ok		adc模块
//PA11 	ADC4 					FUNC1			倾倒								ok		adc模块
//PA12 	ADC5 					FUNC1			主继电器电压				ok		adc模块
//PA13 	ADC6 					FUNC1			节气门位置					ok		adc模块
//PA14 	ADC7 					FUNC1			压力								ok		adc模块
//PA15 	ADC8 					FUNC1			缸头温度						ok		adc模块
//PB0 	ADC9					FUNC1			氧传感器1						ok		adc模块
//PB1 	ADC11					FUNC1			进气温度						ok		adc模块

//PB2 	out						FUNC0			CAN使能							ok		can模块
//PB3 	out						FUNC0			补气阀控制					ok		io模块
//PB4 	in						FUNC0			补气阀检测					ok		io模块
//PB5 	in						FUNC0			电机连接检测1				ok		io模块
//PB6 	out						FUNC0			泵氧控制						ok		io模块
//PB8 	in						FUNC0			侧撑开关检测				ok		io模块
//PB9 	out						FUNC0			油泵控制						ok		io模块
//PB10 	out						FUNC0			碳罐控制						ok		io模块
//PB11 	out						FUNC0			氧传感器加热控制		ok		io模块
//PB12 	in						FUNC0			钥匙检测						ok		io模块

//PB13 	out						FUNC0			点火驱动						ok		ing模块
//PB14 	out						FUNC0			喷油驱动						ok		ing模块
//PB15 	none	输入下拉															ok		io模块

//PC0 	none	输入下拉															ok		io模块
//PC1 	cap						FUNC1			角标捕获PWM2_2			ok		ing模块
//PC2 	none	输入下拉															ok		io模块
//PC3 	cap						FUNC3			33813的复位信号PWM2_0		ok	ing模块
//PC4 	cap						FUNC3			轮速信号捕获PWM2_1	ok		ing模块
//PC5 	out						FUNC0			IIC_SCL							ok		iic模块
//PC6 	in/out				FUNC0			IIC_SDA							ok		iic模块
//PC7 	SWCLK	
//PC8 	none	输入下拉															ok		io模块
//PC9 	SWDIO	
//PC10 	UART6_Tx			FUNC2			33813串转LINE				ok		sci模块		
//PC11 	UART6_Rx			FUNC2			33813串转LINE				ok		sci模块		
//PC12 	out						FUNC0			电机电流选择1				ok		io模块
//PC13 	out						FUNC0			电机电流选择2				ok		io模块	
//PC14 	out						FUNC0			电机方向1						ok		io模块	
//PC15 	out						FUNC0			电机方向2						ok		io模块

//PD1 	UART2_Tx			FUNC1			串口2								ok		sci模块	
//PD2 	UART2_Rx			FUNC1			串口2								ok		sci模块	




void FUN_GPIO_InitHardwr(void);
#endif






