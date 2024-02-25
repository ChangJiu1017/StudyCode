/*HEAD
************************************************************************************************************************
* Filename     : app_rw33814.c
* Function     : MC33814Ð¾Æ¬¶ÁÐ´¼Ä´æÆ÷
* User         : yangsili
* Date         : 2017/11/26
* Version      : V9.1.0
************************************************************************************************************************
HEAD END*/


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "FAM.h"
#include "Service_CAN.h"

#include "app_rw33814.h"
//#include "CAN.h"
#include "Status_st.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define READ_33814            0x15
#define WRITE_33814           0x34
#define READ_LOGIC_DTC_33814  0x27
#define RETURN_SPI_33814      0xfe


byte PICanData[8];
   
void App_RW33814RxData_int(byte *data)
{
   byte i = 0;
   for(i=0;i<8;i++) PICanData[i]=data[i];
}

void App_RW33814_def(void)
{ 
   word cmdAdd;
   word regAdd;
   word sendData;
   word returnData;
   byte sendCANData[8] = {0};
   
   cmdAdd = *(PICanData + 1) >> 4;
   regAdd = *(PICanData + 1) & 0x0fU;
   sendData = *(PICanData + 2);
   returnData = 0;

   cmdAdd <<= 12;
   regAdd <<= 8;
   
   if(PICanData[0] == READ_33814)
   {
       returnData = mc33814_ReadData_def(cmdAdd, regAdd);
   }
   else if(PICanData[0] == WRITE_33814)
   {
       mc33814_SPIRwWord_def(cmdAdd, regAdd, sendData);
       returnData = mc33814_ReadData_def(_1000_READ_CONFIG, regAdd);
   }
   
   sendCANData[0] = RETURN_SPI_33814;
   sendCANData[1] = returnData >> 8;
   sendCANData[2] = returnData;
   
   if(PICanData[0] == READ_LOGIC_DTC_33814)
   {
       sendCANData[0] = READ_LOGIC_DTC_33814;
       sendCANData[1] = V_33814INJ1CTL_STATUS;
       sendCANData[2] = V_IGN2_STATUS;
       sendCANData[3] = V_33814PUMPCTL_STATUS;
       sendCANData[4] = V_33814O2HCTL_STATUS;
       sendCANData[5] = V_IACV_STATUS;
   } 
   
   CAN_SendFrameExt(RW33814ID | CAN_EXTENDED_FRAME_ID, 0, 8, sendCANData);
}
