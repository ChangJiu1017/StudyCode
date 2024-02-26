//#include "events.h"
//#include "map_selection.h"


#ifndef _Public_H
#define _Public_H


#define false 0
#define true 1

#include "sys_definition.h"
//#include "derivative.h"
#include "map_selection.h"
#include "data_sys_macro.h"
#include "data_sys_standard.h"
//#include "add.h" //2014-10-6 delete
//#include "add.h" //2015-5-23 因为ldb未声明，所以把这句话加回来

//extern byte min_byte(byte b1,byte b2);
//形参：x--需要确保范围的参数；
//      max-X的最大值限值；
//      _min-X的最小值*（-1），为正数；
//#define jqminc_bdc     100   //to be check
//****************************************************************************
extern word v_nb_rst;
extern word v_nb_JSTOP;
extern word v_nb_JMMOVE;

extern word bDebug[7];

#define deT3 bDebug[0]
#define deDTFI bDebug[1]
#define deO bDebug[2]
#define deVBAT bDebug[3]
#define deOS bDebug[4]
#define deTPS bDebug[5]
#define deDTC bDebug[6]

//extern byte bKesei[30];  //2009-8-17 add
extern byte nb_reset;
extern byte nb_reset_lst;


//2015-8-22
 #define HHBYTE(x) ((byte)(x>>24))
 #define HLBYTE(x) ((byte)(x>>16))
 #define HBYTE(x) ((byte)(x>>8))
 #define LBYTE(x) ((byte)(x))


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 

#define CMP_RESULT_GE 2
#define CMP_RESULT_GT 1
#define CMP_RESULT_EQ 0
#define CMP_RESULT_LT 255    
#define CMP_RESULT_LE 254    

extern byte v_nb_test;
//extern byte v_nb_test2;
extern byte v_nb_test3;		//20210922 chenwei add

//#define TEST_COME {v_nb_test++;if(v_nb_test&0x01){V_TEST_ON}else {V_TEST_OFF}}
//#define TEST_CR {v_nb_test2++;if(v_nb_test2&0x01){TEST_CR_ON}else {TEST_CR_OFF}}

//20210922 chenwei add
#define TACH_CR {v_nb_test3++;if(v_nb_test3&0x01){TACH_ON}else {TACH_OFF}}

extern byte v_nb_notsyninj_add;

//extern unsigned char v_ToothPosition;

#endif

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//*******************************:start of a sub or function**********************************
//-------------------------------:start of a program para-------------------------------------
//===============================:start of a model============================================