

#ifndef __MCUABS_IIC_H__
#define __MCUABS_IIC_H__
#include "FAM.h"

extern word ib;


#define SPI1_CS_PIN				(GPIO_PA0)
#define SPI1_SCK_PIN			(GPIO_PA1)
#define SPI1_MISO_PIN			(GPIO_PA2)
#define SPI1_MOSI_PIN			(GPIO_PA3)

#define SPI1_CS_CPIN					(GPIO_PA0)//33813的片选
#define SPI1_CS_C							(GPIOA_OUT(0))//33813的片选
#define SPI1_CS_C_ON					SPI1_CS_C = 1;
#define SPI1_CS_C_OFF					SPI1_CS_C = 0;
#define SPI1_CS_C_TOGGLE			SPI1_CS_C = !SPI1_CS_C;
#define SPI1_CS_CDriCurr(x)		GPIO_SetE4E2(SPI1_CS_CPIN, x)
	 
	 
#define P_SCL 				(GPIO_PC5)
#define P_SDA 				(GPIO_PC6) 
#define P_SCLST 			GPIOC_IN(5)
#define P_SDAST				GPIOC_IN(6)

#define M_SCL					(GPIOC_OUT(5))
#define M_SDA					(GPIOC_OUT(6))

#define P_SCL_OUT  		{GPIO_SetDir(P_SCL, GPIO_OUTPUT);}
#define P_SCLH  			M_SCL = 1;
#define P_SCLL  			M_SCL = 0;

#define P_SDAH  			{GPIO_SetDir(P_SDA, GPIO_INPUT);}
#define P_SDAL  			{GPIO_SetDir(P_SDA, GPIO_OUTPUT);M_SDA = 0;}
			

#define M_P						1		
#define M_1US					(5)
#define M_2US					(20)
#define M_4US					(40)
//#define DELAY1US 			{for(ib=0;ib<M_4US;ib++){__NOP;};}
//#define DELAY2US 			{for(ib=0;ib<M_4US;ib++){__NOP;};}
//#define DELAY4US 			{for(ib=0;ib<M_4US;ib++){__NOP;};}	
#define DELAY1US 			FUN_IICWait(M_1US)
#define DELAY2US 			FUN_IICWait(M_2US)
#define DELAY4US 			FUN_IICWait(M_4US)	



#define M_IICCLKPIN						(GPIO_PC5)//定义IIC-CLK引脚别名
#define M_IICDIOPIN						(GPIO_PC6)//定义IIC-DIO引脚别名
#define M_IICCLK							(GPIOC_OUT(5))//定义IIC-CLK引脚别名
#define M_IICDIO							(GPIOC_OUT(6))//定义IIC-DIO引脚别名
#define M_IICCLK_ON						do{M_IICCLK = 1;}while(0)
#define M_IICCLK_OFF					do{M_IICCLK = 0;}while(0)
#define M_IICCLK_TOGGLE				do{M_IICCLK = !M_IICCLK;}while(0)
#define M_IICCLKDriCurr(x)		GPIO_SetE4E2(M_IICCLKPIN, x)
#define M_IICDIO_ON						do{M_IICDIO = 1;}while(0)
#define M_IICDIO_OFF					do{M_IICDIO = 0;}while(0)
#define M_IICDIO_TOGGLE				do{M_IICDIO = !M_IICDIO;}while(0)
#define M_IICDIODriCurr(x)		GPIO_SetE4E2(M_IICDIOPIN, x)



//#define I2C1_SDA_PIN				(GPIO_PC13)
//#define I2C1_SCL_PIN				(GPIO_PC12)


#define I2C_SLAVE_ADDR			(0x51)//从机地址
#define I2C_STAND_MODE			(TRUE)//设置为TRUE时,标准模式,否则是快速模式.

#define I2C_DATA_TTL_LEN		(8)//数据长度为8.

extern byte F_IICTxEvent;
extern byte F_IICRxEvent;

void FUN_IIC_InitHardwr(void);
void FUN_SPI_InitHardwr(void);
void IIC_InitVar(void);
void FUN_IIC_APP_Scheduler(void);
void write_byte(unsigned char i);
void FUN_IICWait(word u);
void bit_start(void);
void bit_stop(void);
void FUN_SPI_APP_Scheduler(void);
byte spi_RwWord_def(word w_TxData, word *w_RecData);


#endif


