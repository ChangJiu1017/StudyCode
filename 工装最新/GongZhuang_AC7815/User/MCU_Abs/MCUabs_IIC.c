
#include "MCUabs_IIC.h"

word ib;

byte F_IICTxEvent;
byte F_IICRxEvent;
byte IIC_WriteDataBuf[8] = {0x55, 0xAA, 0x7E, 0x18, 0x55, 0xAA, 0x7E, 0x18};

/**
* @prototype FUN_IIC_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize IIC module.
*			 初始化IIC模块.
*/
void FUN_IIC_InitHardwr(void)
{
	#if 0
		I2C_ConfigType i2cConfig = {0};
		
		GPIO_SetFunc(I2C1_SDA_PIN, GPIO_FUNC_1);
		GPIO_SetFunc(I2C1_SCL_PIN, GPIO_FUNC_1);
		
		i2cConfig.setting.master = ENABLE;
		i2cConfig.setting.intEn  = DISABLE;
		i2cConfig.setting.i2cEn  = ENABLE;
		i2cConfig.setting.syncEn = ENABLE;
		
		i2cConfig.addr1	= I2C_SLAVE_ADDR;
		#if (I2C_STAND_MODE)//100K波特率
		i2cConfig.sampleCNT = 11;//speed = APBclk/((sampleCnt+1)*(stepCNT+1)*2)
		i2cConfig.stepCNT = 19;
		#else //400K波特率
		i2cConfig.sampleCNT = 11;//speed = APBclk/((sampleCnt+1)*(stepCNT+1)*2)
		i2cConfig.stepCNT = 4;
		#endif
		
		I2C_Initialize(I2C1, &i2cConfig);
	#endif
		GPIO_SetDir(P_SCL, GPIO_OUTPUT);
		GPIO_SetDir(P_SDA, GPIO_OUTPUT);
		M_IICCLK_ON;//释放总线
		M_IICDIO_ON;//释放总线
}




/**
* @prototype IIC_InitVar(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize IIC Var module.
*			 初始化IIC模块变量.
*/
void IIC_InitVar(void)
{
		F_IICTxEvent = 0;
		F_IICRxEvent = 0;
}





/**
* @prototype FUN_IIC_APP_Scheduler
*
* @param[in] ...
* @return	 ...
*
* @brief  	 IIC之APP层总调度
*			 
*/
//word debugxxx;
void FUN_IIC_APP_Scheduler(void)
{
		if(F_IICTxEvent == 1)
		{
				//I2C_MasterWrite(I2C1, I2C_SLAVE_ADDR, IIC_WriteDataBuf, 8);
				//bit_start();
			  //write_byte(0xA5);
				//bit_stop();
				F_IICTxEvent = 0;
		}
		if(F_IICRxEvent == 1)
		{
				F_IICRxEvent = 0;
		}
}



void FUN_IICWait(word u)
{
		word i=0,j=1;	
		j=1;
		for(i=0; i<u; i++)
		{
				while(j>0)
				{
					j--;
				}
		}
}




/*SPI功能*/


/**
* @prototype SPI_InitHardwr(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Initalize SPI module.
*			 初始化SPI模块.
*/
void FUN_SPI_InitHardwr(void)
{
	SPI_ConfigType spiConfig = {0};
	
	/*设置引脚功能*/
//	GPIO_SetFunc(SPI1_CS_PIN,   GPIO_FUNC_0);
//	GPIO_SetDir(SPI1_CS_CPIN, GPIO_OUTPUT);
//	SPI1_CS_CDriCurr(M_GPIO_4mA);
//	SPI1_CS_C_ON;
	GPIO_SetFunc(SPI1_CS_PIN,   GPIO_FUNC_1);
	GPIO_SetFunc(SPI1_SCK_PIN,  GPIO_FUNC_1);
	GPIO_SetFunc(SPI1_MISO_PIN, GPIO_FUNC_1);
	GPIO_SetFunc(SPI1_MOSI_PIN, GPIO_FUNC_1);
	
	spiConfig.setting.master = ENABLE;
	spiConfig.setting.intEn  = DISABLE;
	spiConfig.setting.spiEn  = ENABLE;
	spiConfig.setting.txMsbFirst = ENABLE;
	spiConfig.setting.rxMsbFirst = ENABLE;
	spiConfig.setting.csOutputEn = ENABLE;
	spiConfig.setting.continuousCSEn = ENABLE;
	spiConfig.setting.frmSize = 15;
	spiConfig.setting.cpha 	 = 0;
	spiConfig.setting.cpol	 = 0;
	spiConfig.sckHigh 		 = 5;
	spiConfig.sckLow		 = 5;

	SPI_Initialize(SPI1, &spiConfig);
}

/**
* @prototype SPI_CheckDataDealPrd(void)
*
* @param[in] void
* @return	 void
*
* @brief  	 Check I2C data recv or send should be do.
*			 查询是否需要处理I2C数据.
*/
byte dbgSPItxEN;
word dbgSPItxData;
word dbgSPIrxData;
void FUN_SPI_APP_Scheduler(void)
{
		if(dbgSPItxEN == 1)
		{
				spi_RwWord_def(dbgSPItxData, &dbgSPIrxData);
				dbgSPItxEN = 0;
		}
}

byte spi_RwWord_def(word w_TxData, word *w_RecData)
{

    word w_RecTick = 0;
		while (!SPI_IsTxEF(SPI1));
		SPI_WriteDataReg(SPI1, (word)w_TxData);
		while (!SPI_IsRxFF(SPI1))//;
		{
				w_RecTick++;
        if(w_RecTick > 1500)
        {
            *w_RecData = 0;
            return 1;   
        }
		}
		*w_RecData = SPI_ReadDataReg(SPI1);

    while ((SPI_IsBusy(SPI1)));
    SPI_CSRelease(SPI1);
    return 0; 
}








