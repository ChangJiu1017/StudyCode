

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



#define M_AddAirV_CPIN					(GPIO_PC1)//����������
#define M_AddAirV_C							(GPIOC_OUT(1))//����������
#define M_AddAirV_C_ON					M_AddAirV_C = 1;
#define M_AddAirV_C_OFF					M_AddAirV_C = 0;
#define M_AddAirV_C_TOGGLE			M_AddAirV_C = !M_AddAirV_C;
#define M_AddAirV_CDriCurr(x)		GPIO_SetE4E2(M_AddAirV_CPIN, x)

#define M_AddAirV_TPIN					(GPIO_PB4)//���������
#define M_AddAirV_T							(GPIOB_IN(4))//���������
#define M_AddAirV_TDriCurr(x)		GPIO_SetE4E2(M_KIGNI_TPIN, x)

#define M_MoTorLink_TPIN				(GPIO_PB5)//������Ӽ��
#define M_MoTorLink_T						(GPIOB_IN(5))//������Ӽ��
#define M_MoTorLink_TDriCurr(x)	GPIO_SetE4E2(M_Inj1_OPPIN, x)

#define M_BumpO2_CPIN						(GPIO_PC3)//��������
#define M_BumpO2_C							(GPIOC_OUT(3))//��������
#define M_BumpO2_C_ON						M_BumpO2_C = 1;
#define M_BumpO2_C_OFF					M_BumpO2_C = 0;
#define M_BumpO2_C_TOGGLE				M_BumpO2_C = !M_BumpO2_C;
#define M_BumpO2_CDriCurr(x)		GPIO_SetE4E2(M_BumpO2_CPIN, x)

#define M_CC_TPIN								(GPIO_PB8)//��ſ��ؼ��
#define M_CC_T									(GPIOB_IN(8))//��ſ��ؼ��
#define M_CC_TDriCurr(x)				GPIO_SetE4E2(M_CC_TPIN, x)

#define M_FeulBump_CPIN					(GPIO_PC4)//�ͱÿ���
#define M_FeulBump_C						(GPIOC_OUT(4))//�ͱÿ���
//#define M_FeulBump_C_ST					(GPIOC_IN(4))//�ͱÿ���״̬
#define M_FeulBump_C_ON					M_FeulBump_C = 1;
#define M_FeulBump_C_OFF				M_FeulBump_C = 0;
#define M_FeulBump_C_TOGGLE			M_FeulBump_C = !M_FeulBump_C;
#define M_FeulBump_CDriCurr(x)	GPIO_SetE4E2(M_FeulBump_CPIN, x)

#define M_CTank_CPIN						(GPIO_PC2)//̼�޿���
#define M_CTank_C								(GPIOC_OUT(2))//̼�޿���
#define M_CTank_C_ON						M_CTank_C = 1;
#define M_CTank_C_OFF						M_CTank_C = 0;
#define M_CTank_C_TOGGLE				M_CTank_C = !M_CTank_C;
#define M_CTank_CDriCurr(x)			GPIO_SetE4E2(M_CTank_CPIN, x)

#define M_O2Heat_CPIN						(GPIO_PB15)//�����������ȿ���
#define M_O2Heat_C							(GPIOB_OUT(15))//�����������ȿ���
#define M_O2Heat_C_ST						(GPIOB_IN(15))//������������״̬
#define M_O2Heat_C_ON						M_O2Heat_C = 1;
#define M_O2Heat_C_OFF					M_O2Heat_C = 0;
#define M_O2Heat_C_TOGGLE				M_O2Heat_C = !M_O2Heat_C;
#define M_O2Heat_CDriCurr(x)		GPIO_SetE4E2(M_O2Heat_CPIN, x)

#define M_KeySTPIN							(GPIO_PB12)//Կ�׼��
#define M_KeyST									(GPIOB_IN(12))//Կ�׼��
#define M_KeySTDriCurr(x)				GPIO_SetE4E2(M_KeySTPIN, x)

#define M_MotorEN1PIN						(GPIO_PC12)//�������ѡ��1
#define M_MotorEN1							(GPIOC_OUT(12))//�������ѡ��1
#define M_MotorEN1_ON						M_MotorEN1 = 1;
#define M_MotorEN1_OFF					M_MotorEN1 = 0;
#define M_MotorEN1_TOGGLE				M_MotorEN1 = !M_MotorEN1;
#define M_MotorEN1_DriCurr(x)		GPIO_SetE4E2(M_MotorEN1PIN, x)

#define M_MotorEN2PIN						(GPIO_PC13)//�������ѡ��2
#define M_MotorEN2							(GPIOC_OUT(13))//�������ѡ��2
#define M_MotorEN2_ON						M_MotorEN2 = 1;
#define M_MotorEN2_OFF					M_MotorEN2 = 0;
#define M_MotorEN2_TOGGLE				M_MotorEN2 = !M_MotorEN2;
#define M_MotorEN2_DriCurr(x)		GPIO_SetE4E2(M_MotorEN2PIN, x)

#define M_MotorPh1PIN						(GPIO_PC14)//�������1
#define M_MotorPh1							(GPIOC_OUT(14))//�������1
#define M_MotorPh1_ON						M_MotorPh1 = 1;
#define M_MotorPh1_OFF					M_MotorPh1 = 0;
#define M_MotorPh1_TOGGLE				M_MotorPh1 = !M_MotorPh1;
#define M_MotorPh1_DriCurr(x)		GPIO_SetE4E2(M_MotorPh1PIN, x)

#define M_MotorPh2PIN						(GPIO_PC15)//�������2
#define M_MotorPh2							(GPIOC_OUT(15))//�������2
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



//PA0 	out						FUNC0			33813��SPIƬѡ�ź�				iicģ��
//PA1 	SPI1_SCK			FUNC1			33813��SPIʱ��						iicģ��
//PA2 	SPI1_MISO			FUNC1			33813��SPI������					iicģ��
//PA3 	SPI1_MOSI			FUNC1			33813��SPI������					iicģ��
//PA4 	CANRX					FUNC2			CAN����							ok		canģ��
//PA5 	CANTX					FUNC2			CAN����							ok		canģ��	
//PA6 	ADC10					FUNC1			���1����						ok		adcģ��
//PA7 	ADC0 					FUNC1			��ص�ѹ						ok		adcģ��
//PA8 	ADC1 					FUNC1			���2����						ok		adcģ��
//PA9 	ADC2 					FUNC1			��������λ					ok		adcģ��
//PA10 	ADC3 					FUNC1			��λ								ok		adcģ��
//PA11 	ADC4 					FUNC1			�㵹								ok		adcģ��
//PA12 	ADC5 					FUNC1			���̵�����ѹ				ok		adcģ��
//PA13 	ADC6 					FUNC1			������λ��					ok		adcģ��
//PA14 	ADC7 					FUNC1			ѹ��								ok		adcģ��
//PA15 	ADC8 					FUNC1			��ͷ�¶�						ok		adcģ��
//PB0 	ADC9					FUNC1			��������1						ok		adcģ��
//PB1 	ADC11					FUNC1			�����¶�						ok		adcģ��

//PB2 	out						FUNC0			CANʹ��							ok		canģ��
//PB3 	out						FUNC0			����������					ok		ioģ��
//PB4 	in						FUNC0			���������					ok		ioģ��
//PB5 	in						FUNC0			������Ӽ��1				ok		ioģ��
//PB6 	out						FUNC0			��������						ok		ioģ��
//PB8 	in						FUNC0			��ſ��ؼ��				ok		ioģ��
//PB9 	out						FUNC0			�ͱÿ���						ok		ioģ��
//PB10 	out						FUNC0			̼�޿���						ok		ioģ��
//PB11 	out						FUNC0			�����������ȿ���		ok		ioģ��
//PB12 	in						FUNC0			Կ�׼��						ok		ioģ��

//PB13 	out						FUNC0			�������						ok		ingģ��
//PB14 	out						FUNC0			��������						ok		ingģ��
//PB15 	none	��������															ok		ioģ��

//PC0 	none	��������															ok		ioģ��
//PC1 	cap						FUNC1			�Ǳ겶��PWM2_2			ok		ingģ��
//PC2 	none	��������															ok		ioģ��
//PC3 	cap						FUNC3			33813�ĸ�λ�ź�PWM2_0		ok	ingģ��
//PC4 	cap						FUNC3			�����źŲ���PWM2_1	ok		ingģ��
//PC5 	out						FUNC0			IIC_SCL							ok		iicģ��
//PC6 	in/out				FUNC0			IIC_SDA							ok		iicģ��
//PC7 	SWCLK	
//PC8 	none	��������															ok		ioģ��
//PC9 	SWDIO	
//PC10 	UART6_Tx			FUNC2			33813��תLINE				ok		sciģ��		
//PC11 	UART6_Rx			FUNC2			33813��תLINE				ok		sciģ��		
//PC12 	out						FUNC0			�������ѡ��1				ok		ioģ��
//PC13 	out						FUNC0			�������ѡ��2				ok		ioģ��	
//PC14 	out						FUNC0			�������1						ok		ioģ��	
//PC15 	out						FUNC0			�������2						ok		ioģ��

//PD1 	UART2_Tx			FUNC1			����2								ok		sciģ��	
//PD2 	UART2_Rx			FUNC1			����2								ok		sciģ��	




void FUN_GPIO_InitHardwr(void);
#endif






