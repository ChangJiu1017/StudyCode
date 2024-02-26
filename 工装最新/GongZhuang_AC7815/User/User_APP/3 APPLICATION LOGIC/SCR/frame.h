
#include "sys_definition.h"

                             
      //_STRU_NOX_OUT.NOX_OUT.NOX_ppm_H
      //_STRU_NOX_OUT.NOX_OUT.O2_Valid==NOX_O2_INRANGE
      
//2014-10-30 add 
#if SYS==SYS_SCR
          //2015-1-27 将帧结构更改为数组格式
          #define FRAME_NUM 12
          
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
          #define FRAME_NO_ACU_IN_MGPERS   11


      #define ADD_FRAME_START               (REG_BASE1 + 0x00000370UL)
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
      /*=======================定义NOX传感器诊断的信息===========================================*/
          //定义一个联合：来自CAN，或者需要发给CAN的8各字节的联合
          ///   其中包括了NOX的结构，ACU结构和DCU结构
          //结构体位域的定义
      #ifndef FRAME_H
          #define FRAME_H     
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

                  byte             :4; //2014-11-5 补充
                  
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
                  byte         :8;//0
                  byte         :8; ///1
                  byte mg_per_s_L         :8;  //2
                  byte mg_per_s_H   :8; //3
                  
                  byte        :8;  //4
                  
                  byte            :4;  //5
                  byte            :4;

                  byte            :8;    
                  byte            :8;    
                  
              
                  byte TimeOut_Limit    :8;
                  byte TimeOut_200ms    :8;
              
                  byte VF_RECNEW_FRAME  :1;      //byte 10
                  byte                  :7;
              } ACU_IN_MGPERS;
              
          } STRU_CAN8;
          
          extern volatile STRU_CAN8 STRU_CAN8_FRAME[FRAME_NUM]   @ADD_FRAME_START;
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

          #define ACU_STA_ISFROZEN    1
          #define ACU_STA_NOTFROZEN   2
          #define ACU_STA_FROZENN1   0
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
       
       
       //--------2014-12-7 ADD FOR 重汽---------------
          #define ID_SCR1           0x01 //待定
          #define ID_INCON          0x18FEF600
          #define ID_AMCOM          0x18FEF500
          #define ID_EEC2           0x0CF00300
          #define ID_EEC1           0x0CF00400
          
          #define ID_DCU_MGPERS     0x18FFEF00     //重汽的泵格式
          
       
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

          extern dword v_t_4us_PURGING;
          extern dword v_t_4us_PRIMING;
          extern dword v_t_4us_PURGING_whenkeyon;//2015-1-11钥匙开启时已经清扫的时间；

          extern dword v_t_ACUOut_timeout_4us;
          extern dword v_t_ACUIn_timeout_4us;
          extern dword v_t_Pumpoff_afterPURGING;//清洗后，泵停止的时间；

          
    
#endif 

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
