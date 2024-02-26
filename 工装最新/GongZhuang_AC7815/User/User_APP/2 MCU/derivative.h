/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */
 
/* Include the derivative-specific header file */
//#include <MC9S08AW60.h>
//typedef signed char sbyte;
//typedef signed int sword;

#include "PE_Types.h"

//#include "ram_cab.h"
#include "hard_trig.h" //2012-11-1 因为hard_trig中包含着对硬件的定义，涉及到各模块，因此包含入derivetive.h
#define CAL 								__attribute__((section ("Map_FLASH")))
#define ATC7811MapAddr			(0x0803C000)

//#define TEST_ON  	PTT_PTT0=1;
//#define TEST_OFF  PTT_PTT0=0;

//#define T1_ON  		PTT_PTT1=1;
//#define T1_OFF  	PTT_PTT1=0;
