/*HEAD
 ***********************************************************************
 * Filename	:sys_definition.h
 * Function	:系统配置头文件	
 * User		:yangsili
 * Date		:2017.12.28
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.28	yangsili
 *		1.添加 SYS_MOTOR_33814CAS_S宏定义
 *    2.添加是否采用氧加热宏定义
 ***********************************************************************
HEAD END*/

//#include "map_selection.h"

//#ifndef SYS
    
    #define SYS          SYS_PI

    #define SYS_PI      1
    #define SYS_SCR     2
    #define SYS_UAV     3

    #define ECU810  0      
    #define ECU820  1

    #define EAEA_GEN_26T  11
    
    #define ECU_MOT 1
    #define ECU_GEN 2
    
    #define MOTOR_SM  0
    #define MOTOR_DC  1
    #define MOTOR_TYPE MOTOR_DC 

    #define ECU_TYPE ECU_MOT
    
    //#define IS_GEN
     
    #define MO_ECUHARD ECU820

    //#define M0_RELEASE 0
    #define M0_RELEASE 0
            //0:for calibration,map in EE ; 1 for release, map in flash;
            
    //本模块用于对硬件的操作或对硬件动作的触发
    //作为物理层
    #define M0_WITHT4   1 //2008-12-1 EAEA000850有T4电路；占用了原ISCV的位置；ISCV移到PTE6




    //2015-1-20 增加SCR 34PIN DCU的定义
    
    
    #define SCR_DCU_V4_18PIN  1
    #define SCR_DCU_V5_34PIN  2

    #define SCR_DCU_TYPE  SCR_DCU_V5_34PIN
    
    //2017-12-16,ysl,增加角标处理方式宏定义
    //#define SYS_MOTOR_33814CAS_S        //33814角标处理 move to map_selection
    //2018-1-17,ysl,氧加热型系统
    //#define SYS_O2HEATER_S              //氧加热系统     move to map_selection

//#endif
