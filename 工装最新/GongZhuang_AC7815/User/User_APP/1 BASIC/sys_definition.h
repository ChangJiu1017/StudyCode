/*HEAD
 ***********************************************************************
 * Filename	:sys_definition.h
 * Function	:ϵͳ����ͷ�ļ�	
 * User		:yangsili
 * Date		:2017.12.28
 * Version	:V9.1.0
 *
 *** History ***
 * V9.1.0   2017.12.28	yangsili 
 *		Base module.
 * 
 * V9.2.0   2017.12.28	yangsili
 *		1.��� SYS_MOTOR_33814CAS_S�궨��
 *    2.����Ƿ���������Ⱥ궨��
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
            
    //��ģ�����ڶ�Ӳ���Ĳ������Ӳ�������Ĵ���
    //��Ϊ�����
    #define M0_WITHT4   1 //2008-12-1 EAEA000850��T4��·��ռ����ԭISCV��λ�ã�ISCV�Ƶ�PTE6




    //2015-1-20 ����SCR 34PIN DCU�Ķ���
    
    
    #define SCR_DCU_V4_18PIN  1
    #define SCR_DCU_V5_34PIN  2

    #define SCR_DCU_TYPE  SCR_DCU_V5_34PIN
    
    //2017-12-16,ysl,���ӽǱ괦��ʽ�궨��
    //#define SYS_MOTOR_33814CAS_S        //33814�Ǳ괦�� move to map_selection
    //2018-1-17,ysl,��������ϵͳ
    //#define SYS_O2HEATER_S              //������ϵͳ     move to map_selection

//#endif
