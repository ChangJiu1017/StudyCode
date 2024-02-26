#ifndef _APP_RW33814_H_
#define _APP_RW33814_H_


#include "PE_Types.h"
#include "mc33814.h"


#define RW33814ID  0x10f54001


void App_RW33814RxData_int(byte *data);

void App_RW33814_def(void);


#endif