

#ifndef __Inj_Ing_H__
#define __Inj_Ing_H__
#include "FAM.h"



#define M_IngPIN							(GPIO_PC12)//点火
#define M_Ing									(GPIOC_OUT(12))
//#define M_Ing_ST							(GPIOC_IN(2))
#define M_Ing_ON							M_Ing = 1;
#define M_Ing_OFF							M_Ing = 0;
#define M_Ing_TOGGLE					M_Ing = !M_Ing;
#define M_IngDriCurr(x)				GPIO_SetE4E2(M_IngPIN, x)

#define M_InjPIN							(GPIO_PC14)//喷油
#define M_Inj									(GPIOC_OUT(14))
//#define M_Inj_ST							(GPIOC_IN(4))
#define M_Inj_ON							M_Inj = 1;
#define M_Inj_OFF							M_Inj = 0;
#define M_Inj_TOGGLE					M_Inj = !M_Inj;
#define M_InjDriCurr(x)				GPIO_SetE4E2(M_InjPIN, x)

#define M_UPorDown						1//0:上升沿    1:下降沿

#define M_CAIN								(GPIOC_IN(1))//角标		
#define M_INJ2T3IN						(GPIOC_IN(2))//备用
#define M_33814RSIN						(GPIOC_IN(3))//33814复位信号
#define M_RollIN							(GPIOC_IN(4))//轮速信号

#define TC4										PWM2->CHANNELS[3].CHV
#define TC7										PWM2->CHANNELS[1].CHV
#define TC6										PWM2->CHANNELS[0].CHV
#define TC3										PWM2->CHANNELS[2].CHV

#define TC0										PWM2->CHANNELS[PWM_CHANNEL_CHANNEL4].CHV
#define TC6										PWM2->CHANNELS[PWM_CHANNEL_CHANNEL5].CHV

#define TFLG2_TOF 						PWM_GetOverFlowFlag(PWM2)



extern byte InjST;
extern byte IngST;
extern word InterCHV[6];
extern word InjT1;
extern word Chargetime;
extern byte F_CAok;
extern byte F_CAok2;
extern byte F_CAok3;
extern byte PWM2ch0ST;


void FUN_InjIng_InitHardwr(void);
void InjIng_InitVar(void);
void FUN_InjIng_APP_Scheduler(void);
void PWM_CHANNEL0_Callback(void);
void PWM_CHANNEL1_Callback(void);
void PWM_CHANNEL2_Callback(void);
void PWM_CHANNEL3_Callback(void);
void PWM_CHANNEL4_Callback(void);
void PWM_CHANNEL5_Callback(void);
void PWM2_IRQHandler_Callback(void);
void TIMER0_IRQHandler_Callback(uint8_t timerIndex);
void TIMER1_IRQHandler_Callback(uint8_t timerIndex);
void TIMER2_IRQHandler_Callback(uint8_t timerIndex);
void TIMER3_IRQHandler_Callback(uint8_t timerIndex);
void TIMER4_IRQHandler_Callback(uint8_t timerIndex);
void TIMER5_IRQHandler_Callback(uint8_t timerIndex);
void TIMER6_IRQHandler_Callback(uint8_t timerIndex);
void TIMER7_IRQHandler_Callback(uint8_t timerIndex);
void TIMER8_IRQHandler_Callback(void);
void TIMER9_IRQHandler_Callback(void);
void TIMER10_IRQHandler_Callback(void);
void TIMER11_IRQHandler_Callback(void);
void FUN_TIMER0_Init(void);
void FUN_TIMER1_Init(void);
void FUN_TIMER23_Init(void);
void FUN_TIMER45_Init(void);
void FUN_TIMER67_Init(void);
void FUN_TIMER0_TrigeInt(dword time);
void FUN_TIMER1_TrigeInt(dword time);
void FUN_TIMER3_TrigeInt(word time);
void FUN_TIMER5_TrigeInt(word time);
void FUN_TIMER7_TrigeInt(word time);
void FUN_TIMER8_TrigeInt(word time);
void FUN_TIMER9_TrigeInt(word time);
void FUN_TIMER10_TrigeInt(word time);
void FUN_TIMER11_TrigeInt(word time);
void FUN_TIMER0_DisAbleInt(void);
void FUN_TIMER1_DisAbleInt(void);
void FUN_TIMER3_DisAbleInt(void);
void FUN_TIMER5_DisAbleInt(void);
void FUN_TIMER7_DisAbleInt(void);
void FUN_TIMER8_DisAbleInt(void);
void FUN_TIMER9_DisAbleInt(void);
void FUN_TIMER10_DisAbleInt(void);
void FUN_TIMER11_DisAbleInt(void);


void CA_capture_OnOverflow(void);
void TDC_HY_OnCapture(void);
void Vsamp_OnCapture(void);
void T4_OnCapture(void);
void CA_capture_OnCapture(void);
void TC_INJO_OnInterrupt(void);
void TC_IGNIO_OnInterrupt(void);

extern	unsigned char Flag_end;
extern	unsigned char Flag_first;
extern  unsigned char ADC_collect;
extern  unsigned char ID_count;
extern  unsigned char Frequ_time;
extern  unsigned char Spray_time;
extern  unsigned char Spray3_time;
extern  unsigned char T1_time;
extern  unsigned char T2_time;
extern  unsigned char T_time;
extern  uint32_t g_timer1Count;


extern  unsigned int Electric;
extern  unsigned int Electric_T2;
extern  unsigned int Volt;
extern  unsigned int Temp;
extern  unsigned int Temp_head;
extern  unsigned int Pressure;



#endif






