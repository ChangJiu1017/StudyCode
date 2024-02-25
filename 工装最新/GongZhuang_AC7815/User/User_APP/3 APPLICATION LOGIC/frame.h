
#include "sys_definition.h"
#include "status_st.h"
                             
      //_STRU_NOX_OUT.NOX_OUT.NOX_ppm_H
      //_STRU_NOX_OUT.NOX_OUT.O2_Valid==NOX_O2_INRANGE
      		  //20210422 chenwei add ????????????????,???
		  #define ID_15765_PUMPCONTROL_11  0x18DE0001
		  #define ID_15765_PUMPCONTROL_21  0x18DE0002
		  #define ID_15765_PUMPCONTROL_31  0x18DE0003
		  #define ID_15765_PUMPCONTROL_41  0x18DE0004
//2014-10-30 add 
//#if SYS==SYS_SCR
          //2015-1-27 将帧结构更改为数组格式
          //#define FRAME_NUM 12
          //#define FRAME_NUM 13   //2015-4-4
          //#define FRAME_NUM 17   //2015-4-13
         // #define FRAME_NUM 19   //2015-5-30帧个数
          //#define FRAME_NUM 19   //2015-7-13帧个数
          #define FRAME_NUM 21   //2015-7-25帧个数
          
          #define FRAME_NO_NOX_OUT  0
          #define FRAME_NO_NOX_IN   1
          #define FRAME_NO_ACU_OUT   2
          #define FRAME_NO_ACU_IN     3
          #define FRAME_NO_CAN_DEBUG   4
          #define FRAME_NO_ECU_T_N     5
          #define FRAME_NO_ECU_SCR1    6
          #define FRAME_NO_ECU_INCON   7
          #define FRAME_NO_ECU_AMCON   8
          #define FRAME_NO_ECU_EEC2    9
          #define FRAME_NO_ECU_EEC1    10
          #define FRAME_NO_ACU_IN_MGPERS    11
          #define FRAME_NO_PT               12//ID_ECU_PT  2015-4-4 ADD 来自温度采集

          #define FRAME_NO_AMB               13
          #define FRAME_NO_DM1               14
          #define FRAME_NO_ET1               15
          #define FRAME_NO_DEF               16
          #define FRAME_NO_CMDIN             17
          #define FRAME_NO_PARAOUT           18
          
          #define FRAME_NO_ECM_IN        19  //2015-7-13
          #define FRAME_NO_ECM1_IN       20  //2015-7-13
          

      //#define ADD_FRAME_START               (REG_BASE1 + 0x00000370UL)
      #define ADD_FRAME_FROZEN              (REG_BASE1 + 0x00000380UL) //2015-8-24 ADD ,这个地址之前，都给STATUS_ST使用
      #define ADD_FRAME_15765               (REG_BASE1 + 0x000003B0UL) //2015-9-16 ADD 3b0 as 
      #define ADD_FRAME_START               (REG_BASE1 + 0x000003D0UL) //2015-9-16 change from 3b0 to 3d0 
      /*
      #define ADD_NOX_IN                    (REG_BASE1 + 0x00000370UL)
      #define ADD_ACU_OUT                   (REG_BASE1 + 0x00000380UL)
      #define ADD_ACU_IN                    (REG_BASE1 + 0x00000390UL)
      #define ADD_NOX_OUT                   (REG_BASE1 + 0x000003a0UL)
      #define ADD_CAN_DEBUG                 (REG_BASE1 + 0x000003c0UL)
      #define ADD_ECU_T_N                   (REG_BASE1 + 0x000003d0UL)

      #define ADD_ECU_SCR1                  (REG_BASE1 + 0x000003e0UL)
      #define ADD_ECU_INCON                 (REG_BASE1 + 0x000003f0UL)
      #define ADD_ECU_AMCON                 (REG_BASE1 + 0x00000400UL)
      #define ADD_ECU_EEC2                  (REG_BASE1 + 0x00000410UL)
      #define ADD_ECU_EEC1                  (REG_BASE1 + 0x00000420UL)
      #define ADD_ACU_IN_MGPERS             (REG_BASE1 + 0x00000430UL)
      */
        
        
              /*
                  注意更改 NUM_FRAME_PARAOUT时，
                  需要同步的更改
                      INT10MS_CANPARAOUT
                      ID_CANPARAOUT
                      ComposeParaFrame
                      
                      
                  另外，要注意保证他们在以上数组中的顺序    
                  
              */
         /*   		 																			
          //#define NUM_FRAME_PARAOUT  19
        		 #define	FRAME_CMD_TEST	0x180001FA																			
        		 #define	FRAME_CMD_CAL	0x180002FA																			
        		 #define	FRAME_CMD_DCUMODE	0x180003FA
        		 
        		 #define	FRAME_PARA_CAL2	  0x180009FB
             #define	FRAME_PARA_CAL	  0x180010FB																			
        		 #define	FRAME_PARA_PUMP	  0x180011FB																			
        		 #define	FRAME_WORK0_DOSER	0x180008FB																			
        		 #define	FRAME_WORK1_DOSER	0x180012FB																			
        		 #define	FRAME_WORK2_DOSER	0x180013FB																			
        		 #define	FRAME_WORK3_DOSER	0x180014FB																			
        		 #define	FRAME_SENSOR1	    0x180015FB																			
        		 #define	FRAME_SENSOR2	    0x180016FB																			
        		 #define	FRAME_SENSOR3	    0x180017FB																			
        		 #define	FRAME_ENGINE	    0x180020FB																			
        		 #define	FRAME_LINK_TIMEOUT	0x180030FB																			
        		 #define	FRAME_PARA1_MASTER	0x180050FB																			
        		 #define	FRAME_PARA2_MASTER	0x180051FB																			
        		 #define	FRAME_PARA3_MASTER	0x180052FB																			
        		 #define	FRAME_FAI_DTC	      0x180101FB																			
          */
      		 #define	FRAME_CMD_TEST	0x18EF3DFA
           
    #if SYS==SYS_SCR       
           //#define NUM_FRAME_PARAOUT  19//2015-6-12 
           //#define NUM_FRAME_PARAOUT  20//2015-7-12 这个数量是包括DM1的
           //#define NUM_FRAME_PARAOUT  35//2015-7-21 这个数量是包括DM1的
           //#define NUM_FRAME_PARAOUT  39//2015-8-24 增加 RUNTIME_X
           //#define NUM_FRAME_PARAOUT  36//2015-8-29,将DM1等几个剥离出来，因为控制使能的要求不一样
           #define NUM_FRAME_PARAOUT  40//2015-10-16 ADD RUNTIME +4
            
           /*
      		 #define	FRAME_PARA_CAL	  0x18FF403D
      		 #define	FRAME_PARA_CAL2	  0x18FF413D
      		 #define	FRAME_PARA_PUMP	  0x18FF423D
      		 #define	FRAME_WORK1_DOSER	0x18FF4A3D
      		 #define	FRAME_WORK2_DOSER	0x18FF4B3D
      		 #define	FRAME_WORK3_DOSER	0x18FF4C3D
      		 #define	FRAME_WORK0_DOSER	0x18FF4D3D
      		 #define	FRAME_SENSOR1	    0x18FF4E3D
      		 #define	FRAME_SENSOR2	    0x18FF4F3D
      		 #define	FRAME_SENSOR3	    0x18FF503D
      		 #define	FRAME_ENGINE	    0x18FF513D
      		 #define	FRAME_LINK_TIMEOUT	0x18FF523D
      		 #define	FRAME_PARA1_MASTER	0x18FF533D
      		 #define	FRAME_PARA2_MASTER	0x18FF543D
      		 #define	FRAME_PARA3_MASTER	0x18FF553D
      		 #define	FRAME_FAI_DTC	      0x18FF563D
           #define	FRAME_FAI_FLASH	    0x18FFA03D //2015-7-12
           */
           //  #define	FRAME_CMD_TEST	0x18EF3DFA //注意这个不能算在数量里面
             #define	FRAME_PARA_CAL	0x18FF403D
             #define	FRAME_PARA_CAL2	0x18FF413D
             #define	FRAME_PARA_PUMP	0x18FF423D
             #define	FRAME_PARA_CAL3	0x18FF433D
             #define	FRAME_PARA_CAL4	0x18FF443D
             #define	FRAME_PARA_CAL5	0x18FF453D
             #define	FRAME_PARA_CAL6	0x18FF463D
             #define	FRAME_PARA_CAL7	0x18FF473D
             #define	FRAME_PARA_CAL8	0x18FF483D
             #define	FRAME_PARA_CAL9	0x18FF493D
             #define	FRAME_WORK1_DOSER	0x18FF4A3D
             #define	FRAME_WORK2_DOSER	0x18FF4B3D
             #define	FRAME_WORK3_DOSER	0x18FF4C3D
             #define	FRAME_WORK0_DOSER	0x18FF4D3D
             #define	FRAME_SENSOR1	0x18FF4E3D
             #define	FRAME_SENSOR2	0x18FF4F3D
             #define	FRAME_SENSOR3	0x18FF503D
             #define	FRAME_SENSOR4	0x18FF623D
             #define	FRAME_SENSOR5	0x18FF633D
             #define	FRAME_SENSOR6	0x18FF643D
             #define	FRAME_ENGINE	0x18FF513D
             #define	FRAME_LINK_TIMEOUT	0x18FF523D
             #define	FRAME_PARA1_MASTER	0x18FF533D
             #define	FRAME_PARA2_MASTER	0x18FF543D
             #define	FRAME_PARA3_MASTER	0x18FF553D
             #define	FRAME_PARA4_MASTER	0x18FF573D
             #define	FRAME_PARA5_MASTER	0x18FF583D
             #define	FRAME_PARA6_MASTER	0x18FF593D
             #define	FRAME_PARA7_MASTER	0x18FF603D
             #define	FRAME_PARA8_MASTER	0x18FF613D
             #define	FRAME_FAI_DTC	0x18FF563D
             #define	FRAME_FAI_FLASH	0x18FFA03D


             #define	RUNTIME_1	0x18FF653D  //2015-8-24 add 
             #define	RUNTIME_2	0x18FF663D
             #define	RUNTIME_3	0x18FF673D
             #define	RUNTIME_4	0x18FF683D

             #define	RUNTIME_5	0x18FF693D  //2015-10-16 ADD 
             #define	RUNTIME_6	0x18FF6A3D
             #define	RUNTIME_7	0x18FF6B3D
             #define	RUNTIME_8	0x18FF6C3D
      #endif
      
      #if SYS==SYS_PI
             #define NUM_FRAME_PARAOUT  65	//2021-02-02 zhouxinrui 59+6=64
             
             #define	FRAME_STATUS	0x18FF4000
             #define	FRAME_LD_LDB	0x18FF4200
             #define	FRAME_LD_LDL	0x18FF4300
             #define	FRAME_LD_MPA	0x18FF4400
             #define	FRAME_LD_MTI	0x18FF4500
             #define	FRAME_LAMBDA_U	0x18FF4700
             #define	FRAME_LAMBDA_UC	0x18FF4800
             #define	FRAME_LAMBDA_UC2	0x18FF4900
             #define	FRAME_LAMBDA_UP	0x18FF4A00
             #define	FRAME_LAMBDA_UA	0x18FF4B00
             #define	FRAME_LAMBDA_UR	0x18FF4C00
             #define	FRAME_FILM_1	0x18FF4D00
             #define	FRAME_FILM_2	0x18FF4E00
             #define	FRAME_FILM_3	0x18FF4F00
             #define	FRAME_MI	0x18FF5000
             #define	FRAME_IGN_1	0x18FF5400
             #define	FRAME_IGN_2	0x18FF5500
             #define	FRAME_IGN_3	0x18FF5600
             #define	FRAME_INJPHASE	0x18FF5800
             #define	FRAME_SPEED_1	0x18FF5A00
             #define	FRAME_SPEED_2	0x18FF5B00
             #define	FRAME_SPEED_3	0x18FF5C00
             #define	FRAME_INITION	0x18FF5D00
             #define	FRAME_NOZZLE1	0x18FF5E00
             #define	FRAME_NOZZLE2	0x18FF5F00
             #define	FRAME_TPS1	0x18FF6100
             #define	FRAME_TPS2	0x18FF6200
             #define	FRAME_N	0x18FF6300
             #define	FRAME_OS1	0x18FF6400
             #define	FRAME_OS2	0x18FF6500
             #define	FRAME_TMP1	0x18FF6600
             #define	FRAME_TMP2	0x18FF6700
             #define	FRAME_VBAT	0x18FF6800
             #define	FRAME_CAS1	0x18FF6900
             #define	FRAME_CAS2	0x18FF6A00
             #define	FRAME_PAIRS1	0x18FF6B00
             #define	FRAME_PAIRS2	0x18FF6C00
             #define	FRAME_ETC1	0x18FF6D00
             #define	FRAME_ETC2	0x18FF6E00
             #define	FRAME_ACTUATOR	0x18FF6F00
             #define	FRAME_FAI_DTC	0x18FF7200
             #define	OVERTPM_PRO	0x18FF8000        //20210121 chenwei alter RUNTIME_1 to OVERTPM_PRO
             #define	RUNTIME_2	0x18FF8100
             #define	RUNTIME_3	0x18FF8200
             #define	RUNTIME_4	0x18FF8300
             #define	RUNTIME_5	0x18FF8400
             #define	RUNTIME_6	0x18FF8500
             #define	MISF_1	0x18FF8600
             #define	MISF_2	0x18FF8700
			 #define	MISF_3	0x18FF8800
			 #define	MISF_4	0x18FF8900
			 #define	MISF_5	0x18FF8A00
			 #define	MISF_6	0x18FF8B00
			 #define	MISF_7	0x18FF8C00
			 #define    EUROPVOS_1 0x18FF8D00          //20201118 chenwei add 欧五os故障诊断
			 #define    EUROPVOS_2 0x18FF8E00          //20201118 chenwei add 欧五os故障诊断
			 #define    CCPS_1     0x18FF8F00          //20201127 chenwei add CCPS碳罐电磁阀
			 #define    SAIC_1     0x18FF9000          //20201202 chenwei add SAIC二次补气阀
			 #define	ITSD_1		0x18FF9100			//20210201 zhouxinrui add ITSD进气温度合理性诊断
			 #define	ITSD_2		0x18FF9200			//20210201 zhouxinrui add ITSD进气温度合理性诊断
			 #define	ITSD_3		0x18FF9300			//20210201 zhouxinrui add ITSD进气温度合理性诊断
			 #define	CLTD_1		0x18FF9400			//20210201 zhouxinrui add CLTD进气温度合理性诊断
			 #define	CLTD_2		0x18FF9500			//20210201 zhouxinrui add CLTD进气温度合理性诊断
			 #define	CLTD_3		0x18FF9600			//20210201 zhouxinrui add CLTD进气温度合理性诊断
             #define	FRAME_FAI_FLASH	0x18FFA03D
             
      
      #endif        		 
        		 
        	//以下为不在SCI TO CAN列表中的	 
        	
        	//2015-8-29 需要将以下从 NUM_FRAME_PARAOUT中剥离出来，因为他们的发送与否，与是否标定模式无关，使能方式也不一样
        		 #define  FRAME_DM1_00            0x18FECA3D//2015-6-30 0x18FECA00
        		 #define FRAME_TPCM_BAM 					0x18ECFF00		
             #define FRAME_TPCM_BAM_PACKECT   0x18EBFF00

      /*=======================定义NOX传感器诊断的信息===========================================*/
          //定义一个联合：来自CAN，或者需要发给CAN的8各字节的联合
          ///   其中包括了NOX的结构，ACU结构和DCU结构
          //结构体位域的定义
      #ifndef FRAME_H
          #define FRAME_H     
          
          //#define BYTES_FOR_1DTC 26
          //#define BYTES_FOR_1DTC 27 //2015-11-28 增加冻结时FAI_ID
          #define BYTES_FOR_1DTC 28 //2015-11-29 增加冻结时故障状态
          //#define BYTES_PER_FRAME_FROZEN 32//在EEPROM中，每个冻结帧所占的位置数，注意BYTES_PER_FRAME_FROZEN需要大于等于BYTES_FOR_1DTC
          #define BYTES_PER_FRAME_FROZEN 16//2015-11-28 FROM 32 TO 16
          #define POS_OF_FAI_ID 0
          
          //#define INVALID_FAI_ID  255//故障号无效的ID
          #define INVALID_FAI_ID  254//2015-12-1
          
          typedef union 
          {
              byte Byte[BYTES_FOR_1DTC]; //帧的数据是从0-7，8表示有效的失去联系的时间，9表示失去联系的时间，单位200ms
                            //即 byte[8]>=byte[9]时，代表数据有效；
              
              struct{//2015-1-27 ADD 
                  byte FAI_ID   :8;//故障号对应的FAI的ID,
                  //dword NB_SPN  :24;
                  dword NB_DC   :24;
                  word NB_WUC   :16;
                  word NB_OBDC  :16;
                  dword ENG_RT  :32;//发动机运行时间
                  dword KEYON_T :32;//上电时间
                  dword NB_PUMP :32;
                  byte T_2D5    :8;
                  word N        :16;
                  sbyte ET      :8;
                  sword TSCR    :16;
                  byte FAI_ID_FROZEN :8;//帧冻结时的ID，2015-11-28
                  byte DTC_STA:       8;//故障状态，2015-11-29
                  //byte type_dtc :8;//故障类型
              } COMMON;


              /*
               _FROZEN_FRAME.FAI_ID;
               _FROZEN_FRAME.NB_SPN;
               _FROZEN_FRAME.NB_DC;
               _FROZEN_FRAME.NB_WUC;
               _FROZEN_FRAME.NB_OBDC;
               _FROZEN_FRAME.ENG_RT;//发动机运行时间
               _FROZEN_FRAME.KEYON_T;//上电时间
               _FROZEN_FRAME.NB_PUMP;
               _FROZEN_FRAME.T_2D5;
               _FROZEN_FRAME.N;
               _FROZEN_FRAME.ET;
               _FROZEN_FRAME.TSCR;
               _FROZEN_FRAME.type_dtc;//故障类型
              */

          }STRU_FROZEN;//冻结帧结构

          //2015-11-30 添加
          typedef union 
          {
              //byte Byte[6]; 
              byte Byte[8]; 
              
              struct{//2015-1-27 ADD 
                  byte FAI_ID   :8;//故障号对应的FAI的ID,
                  dword NB_DC   :24;
                  dword NB_OBDC   :16;
                  byte DTC_STA    :8;//故障状态，2015-11-29
              } COMMON;

          }STRU_FROZEN_SHORT;//冻结帧短结构


          #define N_PCITYPE_SF  0
          #define N_PCITYPE_FF  1
          #define N_PCITYPE_CF  2
          #define N_PCITYPE_FC  3

           
           #define  ASK_01  0x01
           #define  ASK_02  0x02
           #define  ASK_03  0x03
           #define  ASK_04  0x04
           #define  ASK_07  0x07
           #define  ASK_09  0x09
           #define  ASK_0A  0x0A
           
           #define  RQST_01 0x41
           #define  RQST_02 0x42
           #define  RQST_03 0x43
           #define  RQST_04 0x44
           #define  RQST_07 0x47
           #define  RQST_09 0x49
           
          
          //2015-9-16
          typedef union 
          {
              byte Byte[9]; 
              
              struct{
                  byte SF_DL     :4  ;
                  byte N_PCITYPE :4  ;
                  byte BYTE1     :8  ; 
                  byte BYTE2     :8  ; 
                  byte BYTE3     :8  ; 
                  byte BYTE4     :8  ; 
                  byte BYTE5     :8  ; 
                  byte BYTE6     :8  ; 
                  byte BYTE7     :8  ; 
                  byte BYTE8     :8  ;
                     
              } SF;

              struct{
                  byte FF_DL_H     :4  ;
                  byte N_PCITYPE :4  ;
                  byte FF_DL_L     :8  ; 
                  byte BYTE2     :8  ; 
                  byte BYTE3     :8  ; 
                  byte BYTE4     :8  ; 
                  byte BYTE5     :8  ; 
                  byte BYTE6     :8  ; 
                  byte BYTE7     :8  ; 
                  byte BYTE8     :8  ;
              } FF;
              
              struct{
                  byte SN     :4  ;
                  byte N_PCITYPE :4  ;
                  byte BYTE1     :8  ; 
                  byte BYTE2     :8  ; 
                  byte BYTE3     :8  ; 
                  byte BYTE4     :8  ; 
                  byte BYTE5     :8  ; 
                  byte BYTE6     :8  ; 
                  byte BYTE7     :8  ; 
                  byte BYTE8     :8  ;
              } CF;

              struct{
                  byte FS        :4  ;
                  byte N_PCITYPE :4  ;
                  byte BS        :8  ; 
                  byte STMIN     :8  ; 
                  byte BYTE3     :8  ; 
                  byte BYTE4     :8  ; 
                  byte BYTE5     :8  ; 
                  byte BYTE6     :8  ; 
                  byte BYTE7     :8  ; 
                  byte BYTE8     :8  ;
              } FC;

              struct{
                  byte           :4  ;
                  byte N_PCITYPE :4  ;
                  byte BYTE1     :8  ;
                  byte BYTE2     :8  ; 
                  byte BYTE3     :8  ; 
                  byte BYTE4     :8  ; 
                  byte BYTE5     :8  ; 
                  byte BYTE6     :8  ; 
                  byte BYTE7     :8  ; 
                  byte IS_NEW          :1  ;
                  byte IS_SENDING      :1  ;
                  byte BYTE8     :6  ;
              } COMMON;


          }FRAME_15765;


          typedef union {
              byte Byte[13]; //帧的数据是从0-7，8表示有效的失去联系的时间，9表示失去联系的时间，单位200ms
                            //即 byte[8]>=byte[9]时，代表数据有效；

              struct{//2015-1-27 ADD 
                  byte         :8;//0
                  byte         :8; ///1
                  byte         :8;  //2
                  byte         :8; //3
                  
                  byte         :8;  //4
                  byte         :8; //5
                  byte         :8; //6   
                  byte         :8; //7   
                  
              
                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } COMMON;

              struct {
                  byte NOX_ppm_L        :8;//byte0
                  byte NOX_ppm_H        :8;//byte0
                  
                  byte O2_percent_L     :8; 
                  byte O2_percent_H     :8; 
                  
                  byte VoltRangeValid   :2; 
                  byte NOXTmp_Valid     :2; 
                  byte NOX_Valid        :2; 
                  byte O2_Valid         :2;
                  
                  byte NOX_HeaterValid  :5;
                  byte NOX_HeatMode     :2;
                  byte                  :1;                                       /* PLL Frequency Modulation Enable Bit 0 */
                  
                  byte NOX_err          :5;                                       /* PLL Frequency Modulation Enable Bit 1 */
                  byte                  :3; 
                  
                  byte O2_err           :5; 
                  byte                  :3;
              
                  byte TimeOut_Limit    :8;      //BYTE 8
                  byte TimeOut_200ms    :8;      //BYTE 9
                  
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
                    
              } NOX_OUT;
              
              struct {
                  byte            :8;//byte0
                  byte            :8;//byte0
                  byte            :8;//byte0
                  byte            :8;//byte0
                  byte            :8;//byte0
                  byte            :8;//byte0
                  byte            :8;//byte0
                  
                  byte StartCode  :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;

              } NOX_IN;

              struct{
                  byte Q_L       :8;//0
                  byte Q_H1       :8; ///1
                  byte Q_H2       :8;  //2
                  byte Q_H3        :8; //3
                  
                  byte Valve_Sta  :2;  //4
                  byte            :6;
                  
                  byte Pump_Sta   :4;  //5
                  byte            :4;

                  byte            :8;    //2014-11-5 补充
                  byte            :8;    //2014-11-5 补充
                  
              
                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ACU_IN;
              
              struct{
                  byte Q_L       :8;      //0
                  byte Q_H1       :8;     //1
                  byte Q_H2       :8;     //2
                  byte Q_H3        :8;    //3
                  
                  byte Pump_Sta   :4;     //4
                  byte Valve_Sta  :2;     //2014-11-15 FAI CUSTOMIZED
                  byte Volt_Sta   :2;     //2014-11-15 FAI CUSTOMIZED//电压状态
                  
                  byte Acu_Ini    :2;      //2014-11-15 FAI CUSTOMIZED//5      //2014-11-14 FOR 6 TO 3X2
                  byte PN_Sta     :2;      //2014-11-15 FAI CUSTOMIZED//5
                  byte            :2;      //5
                  byte Sta_PumpHeater :2;  //
                  
                  byte Sta_MixChamber :2;  //6
                  byte Sta_ACU     :2;
                  byte Sta_EE      :2;
                  byte             :2;
                  
                  byte Sta_Frozen  :2;
                  byte Sta_Purging :2;

                  byte ACU_OUT8_H4  :4; //2014-11-5 补充    //2015-4-8 ADD ACU_OUT8_H4
                  
                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ACU_OUT;

              struct{
                  byte N_PRD_L      :8;
                  byte N_PRD_H      :8;
                  byte TP_TPS       :8;
                  byte TP_TA        :8;
                  
                  byte T1_L         :8;
                  byte T1_H         :8;

                  byte TB_T1_L      :8; //两个T1之间的时间间隔
                  byte TB_T1_H      :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_OUT;

              //2014-12-7
              struct{
                  byte TORQUE_MODE  :4;
                  byte              :4;
                  
                  byte T_PERCENT_ORDER      :8;
                  
                  byte T_PERCENT_ACT       :8;
                  
                  byte N_ENG_L        :8;
                  byte N_ENG_H         :8;
                  
                  byte SOURCE         :8;

                  byte STA_STARTER      :8; 

                  byte T_PERCENT_DEMAND      :8; 

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_EEC1;
              
              //2014-12-7
              struct{
                  byte PEDAL_IDLE_SW  :2;
                  byte ETC_SW         :2;
                  byte N_LIMIT        :2;
                  byte                :2;
                  
                  byte PEDAL_PERCENT  :8;
                  
                  byte T_PERCENT_AT_N :8;
                  
                  byte REMOTE_PEDAL   :8;
                  byte                :8;
                  byte                :8;
                  byte                :8; 
                  byte                :8; 

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_EEC2;
              
              //2014-12-7
              struct{
                  byte PA_KPA  :8;
                  byte TA_INNER_L      :8;
                  byte TA_INNER_H      :8;
                  
                  byte TA_L  :8;
                  byte TA_H  :8;
                  
                  byte TA_INLET :8;
                  
                  byte T_GROUND_L   :8;
                  byte T_GROUND_H   :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_AMCON;
              
              //2014-12-7
              struct{
                  byte      :8;
                  byte P_TURBO_RELATIVE      :8;
                  byte T_INLET      :8;
                  
                  byte P_TURBO_ABSOLUTE  :8;
                  byte    :8;
                  
                  byte    :8;
                  
                  byte     :8;
                  byte     :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_INCON;
              
              //2014-12-7
              struct{
                  byte T_SCR_U_L     :8;
                  byte T_SCR_U_H     :8;
                  
                  byte T_SCR_D_L      :8;
                  byte T_SCR_D_H      :8;
                  
                  byte P_ADBLUE  :8;
                  byte Q_EXHAUST_L   :8;
                  
                  byte Q_EXHAUST_H   :8;
                  
                  byte Q_EXHAUST_HH    :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ECU_SCR1;

              //2015-1-27 add forzhongqi ,mg/s
              struct{
                  byte mg_per_s_L         :8;  //2 //2015-3-2更正
                  byte mg_per_s_H   :8; //3
                  byte         :8;//0
                  byte         :8; ///1
                  
                  /*
                  byte        :8;  //4
                  
                  byte            :4;  //5
                  byte            :4;
                  */
                  
                  //2015-4-1 纠正
                      byte Valve_Sta  :2;  //4
                      byte            :6;
                      
                      byte Pump_Sta   :4;  //5
                      byte            :4;

                  byte            :8;    
                  byte            :8;    
                  
              
                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ACU_IN_MGPERS;

              //2015-4-4 ADD PT
              struct{
                  byte PTUS     :8;  //采样值
                  byte PTDS     :8;
                  
                  byte PTUD      :8; //摄氏度
                  byte PTDD      :8;
                  
                  byte     :8;
                  byte     :8;
                  
                  byte     :8;
                  
                  byte     :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } PT;

              //2015-4-13 ADD AMB
              struct{
                  byte BARO_PRESSURE     :8;  //环境压力
                  byte CAB_TMP_L     :8;
                  
                  byte CAB_TMP_H      :8; //驾驶舱温度
                  byte AMB_TMP_L      :8;
                  
                  byte AMB_TMP_H    :8;
                  byte ENG_INTAKE_TMP    :8;
                  
                  byte RAOD_SURFACE_TMP_L    :8;
                  
                  byte RAOD_SURFACE_TMP_H    :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } AMB;
              
              //2015-4-13 ADD DM1
              /*
              struct{
                  byte PROTECT_STATUS     :2;  //
                  byte AMBER_WARNING_LAMP     :2;  //
                  byte RED_STOP_LAMP     :2;  //
                  byte MALFUNCTION     :2;  //

                  byte FLASH_STATUS     :8;
                  
                  byte       :8; //驾驶舱温度
                  byte       :8;
                  
                  byte     :8;
                  byte     :8;
                  
                  byte     :8;
                  
                  byte     :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } DM1;
              *///2015-6-4 被后面的定义所取代
              
              
              //2015-4-13 ADD ET1
              struct{
                  byte ENG_COOL_TMP     :8;  //

                  byte ENG_FULE_TMP     :8;
                  
                  byte ENG_OIL_TMP_L      :8; //
                  byte ENG_OIL_TMP_H      :8;
                  
                  byte TURBO_OIL_TMP_L    :8;
                  byte TURBO_OIL_TMP_H    :8;
                  
                  byte INTERCOOLER_TMP    :8;
                  
                  byte INTERCOOLER_STAT_OPENING    :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ET1;
              
              //2015-4-13 ADD DEF
              struct{
                  byte TANK_LEVEL     :8;  //

                  byte DEF_TMP     :8;
                  
                  byte       :8; //
                  byte       :8;
                  
                  byte     :8;
                  byte     :8;
                  
                  byte     :8;
                  
                  byte     :8;

                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } DEF;
              
              /*--2015-7-13-----------增加标准帧，东风+ECOCAT, CAN2.0----------------*/
                  struct{
                      byte       :8;  //

                      byte       :8;
                      
                      byte       :8; //
                      byte N_D5_L      :8;//3
                      
                      byte N_D5_H    :8; //4
                      byte     :8;
                      
                      byte ET    :8;  //2015-8-22 ADD ENG COOLANT
                      
                      byte     :8;

                      byte TimeOut_Limit    :8;
                      byte TimeOut_200ms    :8;
                  
                      byte VF_RECNEW_FRAME  :1;      //byte 10
                      byte                  :7;
                  } ECM;
                  
                  
                  struct{
                      byte FUEL_RATE_LPERH_D01_L     :8;  //

                      byte FUEL_RATE_LPERH_D01_H     :8;
                      
                      byte       :8; //
                      byte       :8;
                      
                      byte     :8;
                      byte     :8;
                      
                      byte     :8;
                      
                      byte     :8;

                      byte TimeOut_Limit    :8;
                      byte TimeOut_200ms    :8;
                  
                      byte VF_RECNEW_FRAME  :1;      //byte 10
                      byte                  :7;
                  } ECM1;
                  
                  struct{
                      byte      :8;  //

                      byte      :8;
                      
                      byte     ACT_T_L :8; //
                      byte     ACT_T_H :8;
                      
                      byte     MAX_T_L:8;
                      byte     MAX_T_H:8;
                      
                      byte     :8;
                      
                      byte     :8;

                      byte TimeOut_Limit    :8;
                      byte TimeOut_200ms    :8;
                  
                      byte VF_RECNEW_FRAME  :1;      //byte 10
                      byte                  :7;
                  } ECM6;
                  
              
              /*============2015-5-14 配合CAN版本的DATAVIEW==========*/
                  //2015-5-14 输入的CAN命令
                      struct
                      {
                          byte CMD     :8;  //

                          byte TEST_TIMES_L     :8;
                          
                          byte TEST_TIMES_H      :8; //
                          byte TEST_TIMES_HL     :8;
                          
                          byte TEST_TIMES_HH    :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }CMD_TEST;
              
                      struct
                      {
                          byte CMD     :8;  //

                          byte RESULT1     :8;
                          
                          byte RESULT21      :8; //
                          byte RESULT22      :8;
                          
                          byte RESULT23    :8;
                          byte RESULT24    :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }CMD_CAL;
              
                      struct
                      {
                          byte CMD     :8;  //

                          byte      :8;
                          
                          byte       :8; //
                          byte       :8;
                          
                          byte     :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }CMD_DCU_MODE;
              
 
               
                      struct
                      {
                          byte CMD       :8;  //

                          byte BYTE1     :8;
                          
                          byte BYTE2     :8; //
                          byte BYTE3     :8;
                          
                          byte BYTE4    :8;
                          byte BYTE5    :8;
                          
                          byte BYTE6     :8;
                          
                          byte BYTE7     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }CMD_IN;
              
                      //2015-7-12
                      struct
                      {
                          byte CMD       :8;  //

                          byte CMD2     :8;
                          
                          byte ADD_L     :8; //
                          byte ADD_H     :8;
                          
                          byte DATA1_L    :8;
                          byte DATA1_H    :8;
                          
                          byte DATA2_L     :8;
                          byte DATA2_H     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }CMD_FLASH;

              
                // ------------DCU 发给ECU的参数-----------------
                      struct
                      {
                          byte FMF_STL     :8;  //标定点标准值

                          byte FMF_STH     :8;
                          
                          byte DFMF_TO_ST  :8; //标定结果与标定值的差异
                          byte DT1_TO_STL      :8;
                          
                          byte DT1_TO_STH    :8;
                          byte T3_AT_FMFST_L    :8;
                          
                          byte T3_AT_FMFST_H    :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_CAL;
                      
                      
                      
                      /**/
                      struct
                      {
                          byte FMF_CONSTV_STL     :8;  //标定点标准值

                          byte FMF_CONSTV_STH     :8;
                          
                          byte VBAT_AT_T1MCAL  :8; //T1M标定时的电压
                          byte T1M_CAL_L      :8;
                          
                          byte T1M_CAL_H    :8;
                          byte T30_CAL    :8;
                          
                          byte T30_CUR    :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_CAL2;
                      
                      
                      struct
                      {
                          byte      :8;  

                          byte      :8;
                          
                          byte      :8; 
                          byte      :8;
                          
                          byte     :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_PUMP_MODEL;
                      
                      struct
                      {
                          byte BYTE0     :8;  

                          byte BYTE1     :8;
                          
                          byte BYTE2     :8; 
                          byte BYTE3     :8;
                          
                          byte BYTE4    :8;
                          byte BYTE5    :8;
                          
                          byte BYTE6    :8;
                          
                          byte BYTE7    :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_OUT_ST;  //2015-7-21 ADD 
                      
                      struct
                      {
                          byte T1M_NOW_L     :8;  

                          byte T1M_NOW_H     :8;
                          
                          byte DT1M_BY_VBAT_L     :8; 
                          byte DT1M_BY_VBAT_H     :8;
                          
                          byte DT1M_BY_T30    :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_WORK0_DOSER;
              
              
                      struct
                      {
                          byte  DCU_MODE    :8;  

                          byte  PUMP_STATE    :8;
                          
                          byte  FRE    :8; 
                          byte  PRDL    :8;
                          
                          byte  PRDH   :8;
                          byte  PRDHL   :8;
                          
                          byte  Q_L   :8;
                          
                          byte  Q_H   :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_WORK1_DOSER;
                      
              
                      struct
                      {
                          byte  T1L    :8;  

                          byte  T1H    :8;
                          
                          byte  T3L    :8; 
                          byte  T3H    :8;
                          
                          byte  T4L   :8;
                          byte  T4H   :8;
                          
                          byte  T30L   :8;
                          
                          byte  T30H   :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_WORK2_DOSER;

                      struct
                      {
                          byte  DT3_TG_L    :8;  

                          byte  DT3_TG_H    :8;
                          
                          byte  DT3_L    :8; 
                          byte  DT3_H    :8;
                          
                          byte  DT1_L   :8;
                          byte  DT1_H   :8;
                          
                          byte  DT1_TOST_L   :8;
                          
                          byte  DT1_TOST_H   :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_WORK3_DOSER;
                    
                      struct
                      {
                          byte  VBAT_AD    :8;  

                          byte  VBAT_V    :8;
                          
                          byte  TU_DEG    :8; 
                          byte  TU_AD     :8;
                          
                          byte  ADB_PERCENT   :8;
                          byte  ADB_AD   :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_SENSOR1;
                      
                      struct
                      {
                          byte TSCRB_L     :8;  //
                               
                          byte TSCRB_H     :8;
                          
                          byte TSCR_L      :8; //
                          byte TSCR_H      :8;
                          
                          byte EGTU_DEGL    :8;
                          byte EGTU_DEGH    :8;
                          
                          byte EGTU_AD    :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_SENSOR2;

                      struct
                      {
                          byte EGTD_DEGL     :8;  

                          byte EGTD_DEGH     :8;
                          
                          byte EGTD_AD     :8; 
                          byte      :8;
                          
                          byte     :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA_SENSOR3;
              
              
                      struct
                      {
                          byte N_L     :8;  

                          byte N_H     :8;
                          
                          byte T_PERCENT     :8; 
                          byte      :8;
                          
                          byte     :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }ENG;
              
                      struct
                      {
                          byte TO_SCI     :8;  

                          byte TO_DCU     :8;
                          
                          byte TO_ECU     :8; 
                          byte TO_MGS     :8;
                          
                          byte     :8;
                          byte     :8;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }LINK_TIMEOUT;
              
                      struct
                      {
                          byte DEFB_MLPERH_L     :8;  

                          byte DEFB_MLPERH_H     :8;
                          
                          byte DEFB_PERCENT_M2D5     :8; 
                          byte QINJ_D1UL_L     :8;
                          
                          byte QINJ_D1UL_H    :8;
                          byte KNH3_M64    :8;
                          
                          byte KAIR_M64    :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA1_MASTER;
              

                      struct
                      {
                          byte MEXH_MLPERH_L     :8;  

                          byte MEXH_MLPERH_H     :8;
                          
                          byte SV_L     :8; 
                          byte SV_H     :8;
                          
                          byte SV_HL    :8;
                          byte SV_HH    :8;
                          
                          byte MAX_NSR_M128    :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA2_MASTER;
              
              
                      struct
                      {
                          byte MDEFMAX_MLPERH_L     :8;  

                          byte MDEFMAX_MLPERH_H     :8;
                          
                          byte EXH_ML_L     :8; 
                          byte EXH_ML_H     :8;
                          
                          byte      :8;
                          byte      :8;
                          
                          byte      :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }PARA3_MASTER;
              
                      struct
                      {
                          byte DTC0     :8;  

                          byte DTC1     :8;
                          
                          byte DTC2     :8; 
                          byte DTC3     :8;
                          
                          byte DTC4     :8;
                          byte DTC5     :8;
                          
                          byte DTC6     :8;
                          
                          byte DTC7    :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      }FAI_DTC;



                      struct{
                          byte PROTECT_LAMP_STATUS     :2;  //
                          byte AMBER_WARNING_LAMP_STATUS     :2;  //
                          byte RED_STOP_LAMP_STATUS     :2;  //
                          byte MIL_LAMP_STATUS     :2;  //

                          
                          byte FLASHLAMPSTATUS     :8;
                          
                          byte SPNL      :8; //
                          byte SPNH      :8;
                          
                          byte FMI    :5;
                          byte SPNHL    :3;
                          
                          
                          byte OC    :7;
                          byte CM    :1;
                          
                          byte     :8;
                          
                          byte     :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      } DM1;
                      
                      
                      //2015-6-4
                      //DM1 与 TPCM_BAM 是两种格式的故障码传输方式，倾向与TPCM_BAM的格式
                      struct{
                          byte CTL_20     :8;  //控制字节，为20

                          byte BYTES_OF_2_PULS_4M_DTCNUM_L     :8; //报文有效字节数
                          
                          byte BYTES_OF_2_PULS_4M_DTCNUM_H      :8; //
                          byte PACKECT_NUM      :8; //报文总数据包数
                          
                          byte FF    :8;      //=0xff
                          
                          byte PGN_E3FE00_L    :8;
                          
                          byte PGN_E3FE00_H    :8;
                          
                          byte PGN_E3FE00_HL    :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      } TPCM_BAM;
                      
                      

                      
                      //2015-6-4
                      struct{
                          byte P01     :8;  //Package Identification, set to 0x01

                            byte PROTECT_LAMP_STATUS     :2;  //
                            byte AMBER_WARNING_LAMP_STATUS     :2;  //
                            byte RED_STOP_LAMP_STATUS     :2;  //
                            byte MIL_LAMP_STATUS     :2;  //
                          
                          byte FLASHLAMPSTATUS      :8; //
                          byte PGNL       :8; //报文总数据包数
                          
                          byte PGNH    :8;      //=0xff
                          
                          byte PGNHL    :8;
                          
                          byte PGNHH    :8;
                          
                          byte PGN_NXT    :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      } TPCM_BAM_PACKECT_1ST;
                      
                      
                      
                      struct{
                          byte P0X     :8;  //Package Identification, set to 0x01

                          byte PGNX1     :8;  //
                          
                          byte PGNX2      :8; //
                          byte PGNX3       :8; //报文总数据包数
                          
                          byte PGNX4    :8;      //=0xff
                          
                          byte PGNX5    :8;
                          
                          byte PGNX6    :8;
                          
                          byte PGNX7    :8;

                          byte TimeOut_Limit    :8;
                          byte TimeOut_200ms    :8;
                      
                          byte VF_RECNEW_FRAME  :1;      //byte 10
                          byte                  :7;
                      } TPCM_BAM_PACKECT_X;
                      
                      
              
          } STRU_CAN8;
          
          extern volatile STRU_CAN8 STRU_CAN8_FRAME[FRAME_NUM];//   @ADD_FRAME_START;
          
          extern volatile STRU_FROZEN _FROZEN_FRAME;// @ADD_FRAME_FROZEN;//2015-8-24
          
          extern volatile FRAME_15765 _FRAME_15765;//@ADD_FRAME_15765;//2015-9-16
      #endif
          /*
          extern volatile STRU_CAN8 _STRU_NOX_OUT   @ADD_NOX_OUT;
          extern volatile STRU_CAN8 _STRU_NOX_IN    @ADD_NOX_IN;

          extern volatile STRU_CAN8 _STRU_ACU_OUT   @ADD_ACU_OUT;
          extern volatile STRU_CAN8 _STRU_ACU_IN    @ADD_ACU_IN;

          extern volatile STRU_CAN8 _STRU_CAN_DEBUG @ADD_CAN_DEBUG;

          extern volatile STRU_CAN8 _STRU_ECU_T_N   @ADD_ECU_T_N;//2014-11-8

          //2014-12-7
          extern volatile STRU_CAN8 _STRU_ECU_SCR1   @ADD_ECU_SCR1;//2014-11-8
          extern volatile STRU_CAN8 _STRU_ECU_INCON   @ADD_ECU_INCON;//2014-11-8
          extern volatile STRU_CAN8 _STRU_ECU_AMCON   @ADD_ECU_AMCON;//2014-11-8
          extern volatile STRU_CAN8 _STRU_ECU_EEC2   @ADD_ECU_EEC2;//2014-11-8
          extern volatile STRU_CAN8 _STRU_ECU_EEC1   @ADD_ECU_EEC1;//2014-11-8
          
          extern volatile STRU_CAN8 _STRU_ACU_IN_MGPERS    @ADD_ACU_IN_MGPERS;
          */
          
          
           
              #define _STRU_NOX_OUT     STRU_CAN8_FRAME[FRAME_NO_NOX_OUT] //@ADD_NOX_IN;
              #define _STRU_NOX_IN      STRU_CAN8_FRAME[FRAME_NO_NOX_IN] //@ADD_NOX_IN;

              #define _STRU_ACU_OUT     STRU_CAN8_FRAME[FRAME_NO_ACU_OUT] //@ADD_ACU_OUT;
              #define _STRU_ACU_IN      STRU_CAN8_FRAME[FRAME_NO_ACU_IN] //@ADD_ACU_IN;

              #define _STRU_CAN_DEBUG   STRU_CAN8_FRAME[FRAME_NO_CAN_DEBUG]//@ADD_CAN_DEBUG;

              #define _STRU_ECU_T_N     STRU_CAN8_FRAME[FRAME_NO_ECU_T_N]//@ADD_ECU_T_N;//2014-11-8
              #define _STRU_ECU_SCR1    STRU_CAN8_FRAME[FRAME_NO_ECU_SCR1] //@ADD_ECU_SCR1;//2014-11-8
              #define _STRU_ECU_INCON   STRU_CAN8_FRAME[FRAME_NO_ECU_INCON]//@ADD_ECU_INCON;//2014-11-8
              #define _STRU_ECU_AMCON   STRU_CAN8_FRAME[FRAME_NO_ECU_AMCON] //@ADD_ECU_AMCON;//2014-11-8
              #define _STRU_ECU_EEC2    STRU_CAN8_FRAME[FRAME_NO_ECU_EEC2]//@ADD_ECU_EEC2;//2014-11-8
              #define _STRU_ECU_EEC1    STRU_CAN8_FRAME[FRAME_NO_ECU_EEC1]//@ADD_ECU_EEC1;//2014-11-8
              #define _STRU_ACU_IN_MGPERS   STRU_CAN8_FRAME[FRAME_NO_ACU_IN_MGPERS]// @ADD_ACU_IN_MGPERS;
              #define _STRU_PT          STRU_CAN8_FRAME[FRAME_NO_PT]// @ADD_ACU_IN_MGPERS;


              #define _STRU_AMB          STRU_CAN8_FRAME[FRAME_NO_AMB]// @ADD_ACU_IN_MGPERS;
              #define _STRU_DM1          STRU_CAN8_FRAME[FRAME_NO_DM1]// @ADD_ACU_IN_MGPERS;
              #define _STRU_ET1          STRU_CAN8_FRAME[FRAME_NO_ET1]// @ADD_ACU_IN_MGPERS;
              #define _STRU_DEF          STRU_CAN8_FRAME[FRAME_NO_DEF]// @ADD_ACU_IN_MGPERS;

              #define _STRU_CMDIN        STRU_CAN8_FRAME[FRAME_NO_CMDIN]// @;
              #define _STRU_PARAOUT      STRU_CAN8_FRAME[FRAME_NO_PARAOUT]// @;

              #define _STRU_ECM      STRU_CAN8_FRAME[FRAME_NO_ECM_IN]// @; 标准帧的输入
              #define _STRU_ECM1      STRU_CAN8_FRAME[FRAME_NO_ECM1_IN]// @;其他扩展帧的输入
          
              
          //MACRO

          #define _STRU_DCU_FROM_ACU _STRU_ACU_OUT
          //byte v_st_NOX_VBAT;
          //byte v_st_NOX_TMP;

          //电压范围检测
          #define NOX_VBAT_OUTOFRANGE   0
          #define NOX_VBAT_INRANGE      1
          #define NOX_VBAT_NOTINUSE     2
          #define NOX_VBAT_NOTEXIST     3

          //NOX传感器加热温度
          #define NOX_TMP_OUTOFRANGE   0
          #define NOX_TMP_INRANGE      1
          #define NOX_TMP_NOTINUSE     2
          #define NOX_TMP_NOTEXIST     3

          //NOX信号
          #define NOX_NOX_OUTOFRANGE   0
          #define NOX_NOX_INRANGE      1
          #define NOX_NOX_NOTINUSE     2
          #define NOX_NOX_NOTEXIST     3

          //O2信号
          #define NOX_O2_OUTOFRANGE   0
          #define NOX_O2_INRANGE      1
          #define NOX_O2_NOTINUSE     2
          #define NOX_O2_NOTEXIST     3

          //加热器
          #define NOX_HEATER_FMINOTEXIST   31
          #define NOX_HEATER_OPEN      5
          #define NOX_HEATER_SHORT     3

          //传感器加热模式
          #define NOX_HEATMODE_AUTO       0
          #define NOX_HEATMODE_LEAN34     1
          #define NOX_HEATMODE_LEAN12     2
          #define NOX_HEATMODE_PREHEAT    3

          //NOX ERROR
          #define NOX_NOXER_FMINOTEXIST   31
          #define NOX_NOXER_OPEN      5
          #define NOX_NOXER_SHORT     3

          //O2 ERROR
          #define NOX_O2ER_FMINOTEXIST   31
          #define NOX_O2ER_OPEN      5
          #define NOX_O2ER_SHORT     3

          //START CODE
          #define NOX_STARTCODE_START_OUTLET  5
          #define NOX_STARTCODE_START_INLET  1
          #define NOX_STARTCODE_STOP  0  

          //ACU接受的电磁阀的命令
          #define ACU_VALVE_CLOSE     0
          #define ACU_VALVE_OPEN       1
          #define ACU_VALVE_N1        2    //未使用
          #define ACU_VALVE_N2        3    //未使用

          //ACU接收的泵的命令
          #define ACU_PUMP_ON         0
          #define ACU_PUMP_OFF        1
          #define ACU_PUMP_NULL1      2
          #define ACU_PUMP_NULL2      3
          

          //ACU状态,发给DCU的
          #define ACU_STA_VALVE_OK    0
          #define ACU_STA_VALVE_SHORTH  1
          #define ACU_STA_VALVE_SHORTL  2
          #define ACU_STA_VALVE_OPEN  3
                                       
          #define ACU_STA_PUMPHEATER  0

          #define ACU_STA_MIXCHAMBER_OK     0
          #define ACU_STA_MIXCHAMBER_CHOKE  1
          #define ACU_STA_MIXCHAMBER_N1     2
          #define ACU_STA_MIXCHAMBER_N2     3

          #define ACU_STA_ACU_OK    0
          #define ACU_STA_ACU_NG    1
          #define ACU_STA_ACU_N1    2
          #define ACU_STA_ACU_N2    3

          #define ACU_STA_EE_OK     0
          #define ACU_STA_EE_NG     1
          #define ACU_STA_EE_N1     2
          #define ACU_STA_EE_N2     3

          //#define ACU_STA_ISFROZEN    1
          //#define ACU_STA_NOTFROZEN   2
          //#define ACU_STA_FROZENN1   0
          #define ACU_STA_DICING_FAIL   0
          #define ACU_STA_DEICING    1
          #define ACU_STA_NOTFROZEN   2
          #define ACU_STA_FROZENN2   3


          #define ACU_STA_PURGED    0
          #define ACU_STA_NOTPURGED 1
          #define ACU_STA_PURGEDN1  2
          #define ACU_STA_PURGEDN2  3
      
      //ACU OUT
          //2014-11-15 FAI CUSTOMIZED
          #define VOLT_OK   0
          #define VOLT_UB 1
          #define VOLT_LB 2
          
          #define ACU_OK        0
          #define ACU_NOT_CAB   1
          #define ACU_OPEN      2
          #define ACU_T3FK_OUT  2
          #define ACU_SHORT     3
          
          #define PN_STA_OK  0
          #define PN_STA_CHOKE  1
          #define PN_STA_T3FKOUT  2
       
      //2015-6-3
          #define DM1_LAMP_OFF  0 
          #define DM1_LAMP_ON   1
       
       
          
      
      //调试数据从CAN发出
          #define CAN_DEBUG(i) _STRU_CAN_DEBUG.Byte[i]

 	        //(void) CAN_SendFrameExt(ID_CAN_DEBUG,0,8,&_STRU_CAN_DEBUG.Byte);//CAN调试用  

      //************************定义CAN地址************************************************//
          #define ID_CAN_DEBUG      0x18FFFFFF  //2014-11-5 PETER FOR DEBUG
          /*
          #define ID_ACU            0x18FFDE3D
          *///2014-10-9 新给的地址
          //#define ID_DCU            0x18FFC400      //浙大做实验用的
          
          //#define ID_ACU            0x18FFC33D     //浙大做实验用的格兰富的泵
          #define ID_ACU            0x18FFDE3D     //凯龙提供的格兰富的泵
          #define ID_DCU            0x18FFDF00      //2014-11-17 凯龙

          #define PGN_ACU_ANSWER    0x18FEDA3D     // ACU对PGN的回复，凯龙18 FF DF 3D
          #define PGN_ACU_REQUEST   0x18EAFF00     //请求PGN,  凯龙：18FFDF00

                                                    //ACU回复的，DCU对SCR地址的请求：18 FF DF 3D
          #define ID_NOXTX_OUTLET   0x18F00F52
          #define ID_NOXTX_INLET    0x18F00E51 //F51 

          #define ID_NOXRX_1        0x18FEDF00 //0F00 ERROR
          #define ID_NOXRX_2        0x18FEDF3D

          #define ID_ECU_T_N_OUT    0x0CF00401 //模拟ECU的输出  //2014-12-23 at zhongqi,冲突更改
       
          #define ID_ECU_PT         0x18FFFF3D  				

       //--------2014-12-7 ADD FOR 重汽---------------
          #define ID_SCR1           0x01 //待定
          #define ID_INCON          0x18FEF600
          //#define ID_AMCOM          0x18FEF500  //2015-8-22 发现 与ID_AMB重复了
          #define ID_EEC2           0x0CF00300
          #define ID_EEC1           0x0CF00400
          
          #define ID_DCU_MGPERS     0x18FFEF00     //重汽的泵格式
       
       
       //2015-4-13 kinglong obd
          #define ID_AMB            0x18FEF500  //环境
          #define ID_DM1            0x18FECA3D//18FECA00  //故障
          #define ID_ET1            0x18FEEE00  //发动机冷却液温度
          #define ID_DEF            0x18FE563D  //DEF箱参数   

       //2015-7-13 ECOCAT+DONGFENG
          #define ID_ST_ECM         0x23F
          #define ID_ST_ECM1        0x232
          #define ID_ST_ECM6        0x266
          
          #define ID_DEF_METER_ECM  0x260//FAI给东风自定义的
          #define ID_DEF_TORQUELIMIT  0x202//FAI给东风自定义的

       //2015-9-16 ADD 
          #define	ID_15765_ASK	    0x18DB33F1
          #define	ID_15765_RQST	    0x18DAF13D

       //2016-2-23
          #define ID_15765_ASK_ST    0x7e0     //2015-12-14 东风标准帧
          #define ID_15765_ASK_ST_ELM    0x7DF     //2015-12-14 东风标准帧
          
          #define ID_15765_RQST_ST   0x7e8     //2015-12-14 东风标准帧

       
       //************************声明************************************************//
          extern void FI_Frame_afterReset(void);

          extern void DisCompFrame(void);



    //2014-11-17 AT KAILONG
          				//			4us
          #define 	T_20MS_4US	5000U		//	20	ms	5000
          #define 	T_50MS_4US	12500U		//	50	ms	12500
          #define 	T_100MS_4US	25000U		//	100	ms	25000
          #define 	T_200MS_4US	50000U		//	200	ms	50000
          							
          							
          #define 	T_1S_4US	250000UL		//	1	s	250000
          #define 	T_2S_4US	500000UL		//	2	s	500000
          #define 	T_5S_4US	1250000UL		//	5	s	1250000
          #define 	T_10S_4US	2500000UL		//	10	s	2500000
          #define 	T_15S_4US	3750000UL		//	15	s	3750000
          #define 	T_20S_4US	5000000UL		//	20	s	5000000
          #define 	T_25S_4US	6250000UL		//	25	s	6250000
          #define 	T_30S_4US	7500000UL		//	30	s	7500000
          #define 	T_35S_4US	8750000UL		//	35	s	8750000
          #define 	T_40S_4US	10000000UL		//	40	s	10000000
          #define 	T_45S_4US	11250000UL		//	45	s	11250000
          #define 	T_50S_4US	12500000UL		//	50	s	12500000
          #define 	T_55S_4US	13750000UL		//	55	s	13750000
          #define 	T_60S_4US	15000000UL		//	60	s	15000000


          #define T_CAN200MS_4US     T_200MS_4US
          #define T_PURGING_4US      T_30S_4US
          #define T_PRIMING_4US      T_30S_4US
          #define T_ACUOUT_TIMEOUT_4US   T_1S_4US
          #define T_ACUIN_TIMEOUT_4US    T_1S_4US
          #define T_PUMPOFF_AFTER_PURGING_LIMIT T_2S_4US

          extern word v_t_CAN_200ms_4us;
          
          extern dword v_t_1s_4us;//2015-8-22

          extern dword v_t_4us_PURGING;
          extern dword v_t_4us_PRIMING;
          
          extern dword v_t_4us_PURGING_valid;//有效的清扫时间2015-7-27
          extern dword v_t_4us_PRIMING_valid;//有效的填充时间

          extern dword v_t_4us_PURGING_whenkeyon;//2015-1-11钥匙开启时已经清扫的时间；

          extern dword v_t_ACUOut_timeout_4us;
          extern dword v_t_ACUIn_timeout_4us;
          extern dword v_t_Pumpoff_afterPURGING;//清洗后，泵停止的时间；

          extern void ResetCanTime(void);      //2015-6-3
          extern byte ComposeParaFrame(dword id); //2015-6-3
          extern void Accu_CanParaTime_Per10ms(void);

      //2015-4-4 DTC 的故障数量
          //#define NUM_DTC_SCR 40
          //#define NUM_DTC_SCR 43//2015-8-11 ADD 
          
          #if SYS==SYS_SCR
              #define NUM_DTC_SCR 56//2015-10-15 ADD 
          #else
              //#define NUM_DTC_SCR 51//38  20201117 chenwei 由45增加至51，增加P014C和P014D以及欧五的失火欧五os故障
              //#define NUM_DTC_SCR 59//38  2021-01-14 zhouxinrui 由51增加至59，增加欧五的两个温度传感器故障
              #define NUM_DTC_SCR 50//38  20211004 chenwei add 初始化故障
          #endif
          
          #define M0_MAXID_FORSLAVE 15 //2015-7-6 MOVE FROM DTC TO FRAME.H
          
          extern byte Get_DTCNO_byID(byte id);
          extern byte GetMIL(void);
          extern byte GetSCRLamp(void);
          extern byte GetIsPumpoffByDTC(void);//2015-6-16


          extern byte v_id_cur;
          extern sbyte CurPgnInx;//当前显示


          extern word v_n_SPN_pack;//spn故障的PACK数量
          extern word v_n_SPN_pack_disp;  //当前显示的位置
          extern byte v_NO_SpnPack_CanOut;//从CAN发出的SPN的PACK的号；

          #define BAM_FOR_NONE  0
          #define BAM_FOR_DM1   1
          extern byte v_BAM_for;//多帧包发送的内容

      
      //冻结帧
          extern void Frame_action_whenKeyoff(void);

          
          extern const byte M1_DTCNO[];
          
          extern byte v_nb_DTC_history;//历史故障的数量；
          extern byte v_nb_DTC_history_milon;//历史中需要亮灯的故障的数量；

          extern void ClearDTCHistory(byte blnReset);        
          
       //2015-11-29 根据故障状态类型，分为如下：   
          #define DTC_TYPE_PENDING_NOW      3  //DTC_TYPE_PENDING_NOW更名为DTC_TYPE_PENDING_NOW，用于对应诊断仪的07服务的PENDING
          #define DTC_TYPE_CONFIRM  0  
          #define DTC_TYPE_PENDING  1  
          #define DTC_TYPE_KEYON    2  


          #define DTC_STA_PENDING 0x55
          #define DTC_STA_CONFIRM 0xAA
          #define DTC_STA_CLR     0x11


          extern byte v_ub_tmp_dtc;//DTC 数组的已有的数据的个数；

          extern void Load_FrozenFrame(byte blnLoad,word frame_pos,byte blnIDonly);

          extern byte v_st_mil;//2015-11-29 纠正，from 2 to 3
          extern byte v_st_scr;
          extern byte v_pumpoff;

          extern byte deFrame[8];//2016-3-3
		  extern void Clear_EURV_DrivingCycle_count(void);//20210218 chenwei add清除历史故障吗是将EEPROM中存储驾驶循环的数写0
		  extern void Clear_SPN7_PCODE(void);//20210224 chenwei 清除SPN7涉及的故障码
		  extern word v_pcode;
		  extern void Clear_PCODE_Specific(byte bposDTC,byte blnReset,word adr);

                  

//#endif 

/*
ID号码
0.0-2.4
n/a
如果计量喷射系统序列号是知道的，最少21个bit，如果不知道就使用0
生产代码
2.5-3.7
n/a
由SAE分配生产编码，目前设为0
ECU实例
4.0-4.2
n/a
典型值为0
功能实例
4.3-4.7
n/a
典型值为0
功能
5
n/a
值为69，代表发动机排放后处理系统。
反向
6.0
n/a
典型值为0
车辆系统
6.1-6.7
n/a
典型值为0

车辆系统实例
7.0-7.3
n/a
典型值为0
Industri Group
7.4-7.6
n/a
典型值为0
任意地址能力
7.7
n/a
该值始终设为0，因为计量喷射系统本身无能力改变自己的地址。

*/
