/*HEAD
 ***********************************************************************
 * Filename	:pump.c
 * Function	:油泵控制
 * User		:yangsili
 * Date		:2018.4.2
 * Version	:V9.1.0
 *
 *** History ***
 *
 ***********************************************************************
HEAD END*/

#include "hard_trig.h"
#include "Status_st.h"
#include "add_ram_MPI.h"
#include "data_sys_macro.h"
#include "pump.h"
#include "public.h"


#define PUMP_CLOSE_TIME_TURNON    30            //上电后油泵关闭时间5秒
#define PUMP_CLOSE_TIME_FLAMEOUT  20            //带电熄火后油泵关闭时间2秒
#define ENG_MOVE_TIME             30            //2020-7-14 CR 2s//发动机转动时间5秒，超过该时间可以开启熄火油泵延时关闭//发动机转动时间5秒，超过该时间可以开启熄火油泵延时关闭
#define PUMP_WORK_COND            VF_MMOVE_ENG   //供油条件


byte w_t_acc_PumpClose;   //油泵关闭累计计时
byte w_t_acc_EngMove;     //发动机转动累计计时
bool b_EngMove;           //发动机转动标志
byte t_pump_on_ca;




/***********************************************************************************************************************
* 函数名   : pump_PumpTimeAcc_100ms
* 功能     : 油泵时间计算，上电状态下发动机未运行时，计时x秒置位关闭油泵；
*            发动机启动时间5秒以上，带电熄火后，以带电熄火方式计时关闭油泵
* 日期     : 2018/4/2
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/ 
void pump_PumpTimeAcc_100ms(void)
{
   byte wt_end = 0;
   //未检测到发动机转动，对泵关闭时间累计x秒，置位VF_33814PUMPCLOSE标志位
   if(!PUMP_WORK_COND)
   {   
      //如果发动机曾经持续转动了5秒钟，以带电熄火延时关闭油泵，否则以上电关闭时间关闭油泵
      if(b_EngMove)
      {
          wt_end = PUMP_CLOSE_TIME_FLAMEOUT;
      }
      else
      {
          wt_end = PUMP_CLOSE_TIME_TURNON; 
      }
      
      if(w_t_acc_PumpClose <= wt_end)
      { 
         w_t_acc_PumpClose++;
      }
      
      if(w_t_acc_PumpClose >= wt_end)
      {
         VF_33814PUMPCLOSE = true; 
         b_EngMove = false;
      }

      w_t_acc_EngMove = 0;
	  

   } 
   
   //检测到发动机转动，对发动机转动时间累计x秒后，清除VF_33814PUMPCLOSE标志位
   else if(PUMP_WORK_COND && (w_t_acc_EngMove <= ENG_MOVE_TIME))
   {
      w_t_acc_EngMove++;
      if(w_t_acc_EngMove >= ENG_MOVE_TIME)
      {
         b_EngMove = true;
         VF_33814PUMPCLOSE = false;     //清除VF_33814PUMPCLOSE标志位
         w_t_acc_PumpClose = 0;         //清零油泵关闭累计计时
      }
   }
}



/***********************************************************************************************************************
* 函数名   : pump_PumpControl_maincycle
* 功能     : 油泵控制函数，符合条件，开启油泵，否则关闭油泵
* 日期     : 2018/4/2
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void pump_PumpControl_maincycle(void)
{
   //VF_33814PUMPCLOSE条件可以起到延时关闭油泵的作用
   if(PUMP_WORK_COND || VF_CATCH_CAS || ((v_Toothnum>3)&&!VF_MMOVE_ENG) ||!VF_33814PUMPCLOSE)
   {
      //if(VF_33814PUMPSELFCHECK)//2020-7-7 油泵内阻大，关断慢，可能导致自检不通过
      {
          if(!P_PUMPC_ONOFF_TST)
          {  
              P_PUMPC_ON
			  //TEST_COME
          }
      }
	  
	  if((v_Toothnum>3)&&!VF_MMOVE_ENG)
	  {
		  if(VF_1S_TMCCL&&(t_pump_on_ca<255))t_pump_on_ca++;
		  if((t_pump_on_ca>5)&&VF_PCAOV_CAS)//如果检测到有齿信号超过10s，角标溢出，计数器复位(避免干扰信号油泵一直开启)
		  {
			  t_pump_on_ca=0;
			  v_Toothnum=1;
		  }
	  }
   }
   else
   {
      if(P_PUMPC_ONOFF_TST)
      {
         P_PUMPC_OFF
		 //TEST_CR
      }
   }
 #ifdef SY_QD  
   if(VF_QD_ST)//2v
   {
	   P_PUMPC_OFF
   }
   #endif

 #ifdef SY_CC  
   if(VF_CC_ST)//2v	//20210819 chenwei add侧撑 ||VF_CC_ST
   {
	   //P_PUMPC_OFF
   }
 #endif
}

/***********************************************************************************************************************
* 函数名   : pump_pumpstatus_def
* 功能     : 油泵自检失败，读取错误状态
* 日期     : 2020/7/6
* 输入参数 : 无
* 返回值   : 
* 修改记录 :  
***********************************************************************************************************************/
byte pump_pumpstatus_def(void)
{
    byte state;
    mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
    state = P_33814REL1_ST;
    //mc33814_SPIRwWord_def(_6000_WRITE_CONTROL, _0100_OTHER_OFFON_CONT, 1U);
    return state;
}
/***********************************************************************************************************************
* 函数名   : pump_PumpSelftest_def
* 功能     : 油泵自检，以最后一次自检结果为准
* 日期     : 2018/7/13
* 输入参数 : times:自检次数，以最后一次自检结果为准
* 返回值   : true:自检通过
*            false:自检不通过
* 修改记录 :  
***********************************************************************************************************************/
bool pump_PumpSelftest_def(byte times)
{
    word b;
    byte i;
    byte result;
    
    for(i = 0; i < times; i++)
    {
        //先清0状态寄存器
        mc33814_SPIRwWord_def(_4000_WRITE_STATUS, _0200_RELAY1_FAULT, 0U);
        result = 0;
        if(P_PUMPC_ONOFF_TST)
        { 
            for(b=0;b<=500;b++);//
            P_PUMPC_OFF
            for(b=0;b<=2000;b++);
            P_PUMPC_ON
            mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
            result = P_33814REL1_ST;
        }
        else
        {
            for(b=0;b<=2000;b++);
            P_PUMPC_ON 
            for(b=0;b<=500;b++);
            P_PUMPC_OFF
            mc33814_ReadStatus_def(_0200_RELAY1_FAULT);
            result = P_33814REL1_ST;
        }
    }
    
    //推送结果
    if(result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/***********************************************************************************************************************
* 函数名   : pump_PumpInit_init
* 功能     : 油泵控制初始化,并开启油泵
* 日期     : 2018/4/3
* 输入参数 : 无
* 返回值   : 无
* 修改记录 :  
***********************************************************************************************************************/
void pump_PumpInit_init(void)
{
   w_t_acc_PumpClose = 0;   //油泵关闭累计计时
   w_t_acc_EngMove = 0;     //发动机运行累计计时
   b_EngMove = false; 
   VF_33814PUMPCLOSE = false;

   if(pump_PumpSelftest_def(2))
   {
      VF_33814PUMPSELFCHECK = true;
      P_PUMPC_ON
   }
   else
   {
      VF_33814PUMPSELFCHECK = false;
      P_PUMPC_OFF
   }
   
   t_pump_on_ca=0;
}