
#include "MCUabs_GPIO.h"


void FUN_GPIO_InitHardwr(void)
{
	  GPIO_SetFunc(GPIO_PC1, 0); ///Set PWM2_CH2
    GPIO_SetFunc(GPIO_PC2, 0); ///Set PWM2_CH3
    GPIO_SetFunc(GPIO_PC3, 0); ///Set PWM2_CH0
    GPIO_SetFunc(GPIO_PC4, 0); ///Set PWM2_CH1
///////////////////////NONE/////////////////////////////
//		GPIO_SetDir(M_PB15nonePIN, GPIO_INPUT);
//		GPIO_SetPullupEnable(M_PB15nonePIN, ENABLE);
//	
//		GPIO_SetDir(M_PC0nonePIN, GPIO_INPUT);
//		GPIO_SetPullupEnable(M_PC0nonePIN, ENABLE);
//	
//		GPIO_SetDir(M_PC2nonePIN, GPIO_INPUT);
//		GPIO_SetPullupEnable(M_PC2nonePIN, ENABLE);
//	
//		GPIO_SetDir(M_PC8nonePIN, GPIO_INPUT);
//		GPIO_SetPullupEnable(M_PC8nonePIN, ENABLE);
///////////////////////OUT/////////////////////////////
		GPIO_SetDir(M_AddAirV_CPIN, GPIO_OUTPUT);
		M_AddAirV_CDriCurr(M_GPIO_4mA);
		M_AddAirV_C_OFF;
	
		GPIO_SetDir(M_BumpO2_CPIN, GPIO_OUTPUT);
		M_BumpO2_CDriCurr(M_GPIO_4mA);
		M_BumpO2_C_OFF;
	
		GPIO_SetDir(M_FeulBump_CPIN, GPIO_OUTPUT);
		M_FeulBump_CDriCurr(M_GPIO_4mA);
		M_FeulBump_C_OFF;
		
		GPIO_SetDir(M_CTank_CPIN, GPIO_OUTPUT);
		M_CTank_CDriCurr(M_GPIO_4mA);
		M_CTank_C_OFF;
	
		
		GPIO_SetDir(M_O2Heat_CPIN, GPIO_OUTPUT);
		M_O2Heat_CDriCurr(M_GPIO_4mA);
		M_O2Heat_C_OFF;
		
		GPIO_SetDir(M_MotorEN1PIN, GPIO_OUTPUT);
		M_MotorEN1_DriCurr(M_GPIO_16mA);
		M_MotorEN1_OFF;
		
		GPIO_SetDir(M_MotorEN2PIN, GPIO_OUTPUT);
		M_MotorEN2_DriCurr(M_GPIO_16mA);
		M_MotorEN2_OFF;
		
		GPIO_SetDir(M_MotorPh1PIN, GPIO_OUTPUT);
		M_MotorPh1_DriCurr(M_GPIO_16mA);
		M_MotorPh1_OFF;
		
		GPIO_SetDir(M_MotorPh2PIN, GPIO_OUTPUT);
		M_MotorPh2_DriCurr(M_GPIO_16mA);
		M_MotorPh2_OFF;
/////////////////////IN/////////////////////////////
		GPIO_SetDir(M_AddAirV_TPIN, GPIO_INPUT);
		GPIO_SetDir(M_MoTorLink_TPIN, GPIO_INPUT);
		GPIO_SetDir(M_CC_TPIN, GPIO_INPUT);
		GPIO_SetDir(M_KeySTPIN, GPIO_INPUT);
}





