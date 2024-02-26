/*HEAD
 ***********************************************************************
 * Filename	:pump.c
 * Function	:�ͱÿ���
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


#define PUMP_CLOSE_TIME_TURNON    30            //�ϵ���ͱùر�ʱ��5��
#define PUMP_CLOSE_TIME_FLAMEOUT  20            //����Ϩ����ͱùر�ʱ��2��
#define ENG_MOVE_TIME             30            //2020-7-14 CR 2s//������ת��ʱ��5�룬������ʱ����Կ���Ϩ���ͱ���ʱ�ر�//������ת��ʱ��5�룬������ʱ����Կ���Ϩ���ͱ���ʱ�ر�
#define PUMP_WORK_COND            VF_MMOVE_ENG   //��������


byte w_t_acc_PumpClose;   //�ͱùر��ۼƼ�ʱ
byte w_t_acc_EngMove;     //������ת���ۼƼ�ʱ
bool b_EngMove;           //������ת����־
byte t_pump_on_ca;




/***********************************************************************************************************************
* ������   : pump_PumpTimeAcc_100ms
* ����     : �ͱ�ʱ����㣬�ϵ�״̬�·�����δ����ʱ����ʱx����λ�ر��ͱã�
*            ����������ʱ��5�����ϣ�����Ϩ����Դ���Ϩ��ʽ��ʱ�ر��ͱ�
* ����     : 2018/4/2
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/ 
void pump_PumpTimeAcc_100ms(void)
{
   byte wt_end = 0;
   //δ��⵽������ת�����Աùر�ʱ���ۼ�x�룬��λVF_33814PUMPCLOSE��־λ
   if(!PUMP_WORK_COND)
   {   
      //�����������������ת����5���ӣ��Դ���Ϩ����ʱ�ر��ͱã��������ϵ�ر�ʱ��ر��ͱ�
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
   
   //��⵽������ת�����Է�����ת��ʱ���ۼ�x������VF_33814PUMPCLOSE��־λ
   else if(PUMP_WORK_COND && (w_t_acc_EngMove <= ENG_MOVE_TIME))
   {
      w_t_acc_EngMove++;
      if(w_t_acc_EngMove >= ENG_MOVE_TIME)
      {
         b_EngMove = true;
         VF_33814PUMPCLOSE = false;     //���VF_33814PUMPCLOSE��־λ
         w_t_acc_PumpClose = 0;         //�����ͱùر��ۼƼ�ʱ
      }
   }
}



/***********************************************************************************************************************
* ������   : pump_PumpControl_maincycle
* ����     : �ͱÿ��ƺ��������������������ͱã�����ر��ͱ�
* ����     : 2018/4/2
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void pump_PumpControl_maincycle(void)
{
   //VF_33814PUMPCLOSE������������ʱ�ر��ͱõ�����
   if(PUMP_WORK_COND || VF_CATCH_CAS || ((v_Toothnum>3)&&!VF_MMOVE_ENG) ||!VF_33814PUMPCLOSE)
   {
      //if(VF_33814PUMPSELFCHECK)//2020-7-7 �ͱ�����󣬹ض��������ܵ����Լ첻ͨ��
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
		  if((t_pump_on_ca>5)&&VF_PCAOV_CAS)//�����⵽�г��źų���10s���Ǳ��������������λ(��������ź��ͱ�һֱ����)
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
   if(VF_CC_ST)//2v	//20210819 chenwei add��� ||VF_CC_ST
   {
	   //P_PUMPC_OFF
   }
 #endif
}

/***********************************************************************************************************************
* ������   : pump_pumpstatus_def
* ����     : �ͱ��Լ�ʧ�ܣ���ȡ����״̬
* ����     : 2020/7/6
* ������� : ��
* ����ֵ   : 
* �޸ļ�¼ :  
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
* ������   : pump_PumpSelftest_def
* ����     : �ͱ��Լ죬�����һ���Լ���Ϊ׼
* ����     : 2018/7/13
* ������� : times:�Լ�����������һ���Լ���Ϊ׼
* ����ֵ   : true:�Լ�ͨ��
*            false:�Լ첻ͨ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
bool pump_PumpSelftest_def(byte times)
{
    word b;
    byte i;
    byte result;
    
    for(i = 0; i < times; i++)
    {
        //����0״̬�Ĵ���
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
    
    //���ͽ��
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
* ������   : pump_PumpInit_init
* ����     : �ͱÿ��Ƴ�ʼ��,�������ͱ�
* ����     : 2018/4/3
* ������� : ��
* ����ֵ   : ��
* �޸ļ�¼ :  
***********************************************************************************************************************/
void pump_PumpInit_init(void)
{
   w_t_acc_PumpClose = 0;   //�ͱùر��ۼƼ�ʱ
   w_t_acc_EngMove = 0;     //�����������ۼƼ�ʱ
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