/*HEAD
************************************************************************************************************************
* Filename     : pump.h
* Function     : pumpÍ·ÎÄ¼þ
* User         : yangsili
* Date         : 2018.4.2
* Version      : V9.1.0
*
*** History ***
*
************************************************************************************************************************
HEAD END*/


#ifndef _PUMP_H_
#define _PUMP_H_

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "PE_Types.h"


void pump_PumpTimeAcc_100ms(void);
void pump_PumpControl_maincycle(void);
bool pump_PumpSelftest_def(byte times);
void pump_PumpInit_init(void);
byte pump_pumpstatus_def(void);

extern byte v_Toothnum;

#endif