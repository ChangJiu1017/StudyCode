#include "Public.h"
#include "Status_st.h"

#include "add.h"

#include "time_ccl.h"
#include "sys_init.h"
#include "ee.h"

//#include "look_up.h"
//#include "sci.h"
//#include "fmf_odr.h"
//#include "DTC.H"
//#include "inj_ctl.h"
//#include "data_inj_source.H"

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_SYS_STATUS;
volatile V_STATUS_STR _V_SYSINI_STATUS;
volatile V_STATUS_STR _V_SYSINIH_STATUS;
volatile V_STATUS_STR _V_SYSINIHL_STATUS;
volatile V_STATUS_STR _V_SYSINIHH_STATUS;
//#pragma DATA_SEG DEFAULT


volatile V_STATUS_STR _V_PRO1_F6_STATUS ;
volatile V_STATUS_STR _V_PRO2_F5_STATUS ;

byte nb_keyon_forIni;       // //2008-10-30 ADD
byte nb_keyon;              //2009-8-28
sbyte vc_tw1_t3fk;          //save the ti_offset at	idle set by	user，偏移量2
sbyte vc_tw3offset;         //T3零点save the value of	ti shift set by	pc，T3的零点-调试得到//即O值

/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define NB_INC_FORINI       5   //2009-4-6

#define M0_INJ_SHIFTL_REF 0x3e  //2014-10-7

#define TW3OFFSET_FOR_NOINI 0//0 15   //2010-7-10 
#define NB_MILE_FAIL_LIMIT  10 
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte v_nb_injparaOK;        //在读完喷油器参数后,正常情况下应该=5
sbyte vc_CLS_IDLE_lst;      //读取的上次反馈值TOBECHECK,他是怎么被使用的   //TOBECHECK
                            //2008-3-4 已经CHECK，vc_CLS_IDLE_lst在被读出之后被按比例转移到O值上了
byte nb_injconnected;   //2009-7-7
byte bTemp;//2009-3-29 for test


byte blnIdleIni;//2016-7-31 增加，怠速初始化进行中
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Get_Ti_shift(void);    //调用该函数之前需要已经设置了VF_POR_MCU

void Check_Ini_Status(void);   //local //是原来的test_para_inj
void Save_IdleDebugResult(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte cls);//2010-1-4

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
void FG_SysInit_AfterReset() 
{
    //本语句需要在状态量初始化之前;
    //if(!VF_PNOPEN_LST_SYSINI) nb_keyon_forIni=0;      //2009-8-28 add 
        INJ_OFF;

        VF_DOM_SYS=false;                     //

        nb_injconnected=0;//2009-7-7
        
        V_SYSINI_STATUS=false;//
        V_SYSINIH_STATUS=false;//2008-12-15
            
        if(VF_VPOR_HARD)//2009-3-28 add condiction
        {
            V_SYSINIHL_STATUS=false;//2008-12-15
            V_SYSINIHH_STATUS=false;//2009-8-6 add 
          //  V_SYSINI_STATUS=false;//2008-12-15

		  #ifdef SY_HUANSONG		//20211013 chenwei add 环松
			 
			        VF_ENAQ_SYSINI=false;
			        VF_AQOK_SYSINI=false ;
			        VF_NOINIT_SYSINI=false;//2009-11-19 add 
			        //2009-11-19
			        VF_AQAFTER_PNINI_SYSINI=false;//2009-11-19
			        VF_JFINISHINI_WHENMOVING_SYSINI=true;//2009-11-25
			        blnIdleIni=0;//2016-7-31

					VFF_SYSNOTINIT_DTC=false;		//20211013 add
				
		    #endif
          
        }
        else
        {
            //V_SYSINIHL_STATUS与V_SYSINIHH_STATUS在软复位后仍保持；
            /*
                需要保留的sysini位
            	VF_NOINIT_SYSINI	_V_SYSINI_STATUS.Bits.b2      //未调试
                VF_RUNENOUGH_FORINI_SYSINI	_V_SYSINI_STATUS.Bits.b7
                VF_DIRECT_INI_SYSINI b6
                VF_MILES_NEEDT4_SYSINI	_V_SYSINIHL_STATUS.Bits.b2    //2010-3-8   //达到需要自检的里程；
                VF_MILES_NEEDT4AGAIN_SYSINI	_V_SYSINIHL_STATUS.Bits.b3
                VF_1ST_INI_SYSINI     b4 //2010-3-5 更改为需要怠速时测T4的标志，也需要保留
                VF_REMOVE_INI_SYSINI      b3
                VF_SEARCHTMP_SYSINI b6，需要保留//2010-3-6
                VF_AQOK_SYSINI     b7
                VF_NEEDSAVEGAMA_SYSINI
                VF_TIER_SYSINI
                  

            */
        }

    //强制自检的检测：
        //nb_keyon++;
        if(VF_POR_HARD)
        {
            nb_keyon_forIni=0;
            
            blnIdleIni=0;//2016-7-31 
        }
        
        
        if(VF_VPOR_HARD)
        {    
            VF_CLR_SL_SYSINI=false;
            nb_keyon_forIni++; 
			nb_keyon++;//2019-3-14 挪到这个地方，代表开钥匙的次数
			VF_DIAG_FAI=false;
			
			if(nb_keyon_forIni>=3)
			{
				VF_DIAG_FAI=true;
			}
            
            //2009-8-6 add
            //if(nb_keyon_forIni==5||nb_keyon_forIni==8)//2010-1-19 change to from 3 to 5
            if(nb_keyon_forIni>=5 && nb_keyon_forIni<=8)//2016-7-30 >=5 <=8
            {
                //VF_DIRECT_INI_SYSINI=true;
                //2010-1-19 add 
                VF_1ST_INI_SYSINI=true;
                if(nb_keyon_forIni==8)VF_CLR_SL_SYSINI=true;
                //nb_keyon_forIni=0;  //2013-4-2 delete
                
                blnIdleIni=0;//2016-7-31
				
				
				//2020-5-19 退出扫描，直接进入快闪
				VFF_NOZZLEINI_T1DGOT_DTC=true;
				//VF_NEEDSAVEGAMA_SYSINI=true;
				
				VF_T1SCANINGBREAK_INJCTL=true;//需要推出扫描      //2010-3-13将终止的动作放在这里，而不是校验GAMA之后
				VF_NOTNEED_DROPSCAN_SYSINI=true;//2009-11-25
				//VF_RUNENOUGH_FORINI_SYSINI=false;//2010-3-15
				
				VF_PNNOINI_SYSINI=false;//2009-7-31 add     //2009-8-3 纠正
				FC_ti_set();
				
            } else 
            {
                //VF_DIRECT_INI_SYSINI=false;
                //VF_1ST_INI_SYSINI=false;
            }
        }
		//VF_1ST_INI_SYSINI=false;
           

    //2009-8-28 MOVE HERE:
    INJ_OFF;
    
    //在开钥匙时，读取EEPROM中的数据，并置是否初始化、是否调试的标志位；
    if(VF_VPOR_HARD) //2009-4-15 add condiction //2009-8-4 delete this condiction //2009-12-12
    {
       //2020-5-20 共轨系统不再check Check_Ini_Status(); //判断是否有编号，并且
        Get_Ti_shift();//-call	ti_shift_get从判断VF_POR_MCU之前挪到之后，因为用到VF_POR_MCU了;//获取偏移量（o值）：temp_ti_set;包括上次的反馈量
        //if(VF_TOBEINI_SYSINI)vc_tw3offset=1;//偏浓考虑； 2009-3-28 move here 
        //vc_tw1_t3fk=(sbyte)Load_D_DK(E_DTI_T3FK0_ADR,true);//w;  
		vc_tw1_t3fk=0;
        //2010-3-8　move here ，由于涉及到VF_PNNOINI_SYSINI标志位，因此，挪到这里，并且位置应该在Check_Ini_Status之后；

    }//2009-4-15 add condiction 
    

//===========================================
    //2016-7-31

    if(VF_VPOR_HARD)
    {
        if(blnIdleIni>0)
        {   //这个应该要放在读取O值之后
            if(!VF_PNNOINI_SYSINI)
            {
                VF_NOINIT_SYSINI=true;
                VF_AQAFTER_PNINI_SYSINI=true;

                //VF_NEEDSAVEGAMA_SYSINI=true;
                VF_1ST_INI_SYSINI=true;
            }
            
        }
    }


    

    if(!VF_NEEDSAVEGAMA_SYSINI)//因为一旦 VF_NEEDSAVEGAMA_SYSINI置位后，若软RESET时，就不能变VF_SEARCHTMP_SYSINI，否则影响存储
    {
        //, VF_SEARCHTMP_SYSINI 
        VF_SEARCHTMP_SYSINI=true;
        if(VF_1ST_INI_SYSINI)
        {
            VF_SEARCHTMP_SYSINI=false;
        }
    }

    
    //设置泵油、预喷及T1扫描的标志位；
    //if(VF_DIRECT_INI_SYSINI||VF_MILES_NEEDT4_SYSINI||VF_MILES_NEEDT4AGAIN_SYSINI) //VF_NEEDSCANT4_SYSINI
    //2010-3-13 change to the following condiction:
    /*if((VF_1ST_INI_SYSINI)&&!VF_NEEDSAVEGAMA_SYSINI) //VF_NEEDSCANT4_SYSINI如果需要自检，且未获得需要存储的自检结果时，需要置T1扫描的标志位；
    {   //如果是要自检
        VF_PREINJEND_SYSINI=false;
        //VF_NOTNEED_DROPSCAN_SYSINI=false;
		VF_NOTNEED_DROPSCAN_SYSINI=true;//2020-5-20 需要退出扫描阶段		
        VF_PREPUMPEND_SYSINI=false;//2009-8-26 add 
    }
    else */
    {   //如果是不要自检
        VF_NOTNEED_DROPSCAN_SYSINI=true; 
        if(VF_VPOR_HARD)
        {   //如果是开钥匙复位；
            VF_PREINJEND_SYSINI=false;			    //清除预喷结束标识					
            VF_PREPUMPEND_SYSINI=false;		    //清除泵油结束标识				
        }
        else
        {
            if(VF_RUNTOOS_ENG)VF_PREPUMPEND_SYSINI=false;
            else 
            {
                VF_PREINJEND_SYSINI=true;
                VF_PREPUMPEND_SYSINI=true;//2009-8-26 add 
            }
        }
    }
    if(!VF_VPOR_HARD)VF_PREINJEND_SYSINI=true;
    
    //关于默认O值；
    if((VF_1ST_INI_SYSINI&&!VF_AQOK_SYSINI)||VF_NOINIT_SYSINI)          //或未调试，或无gama//自检过程中还没有获得gama，即T4还未成功
    {
            //2010-3-16 因为需要运行发动机，因此，使用默认O值；
            vc_tw3offset=TW3OFFSET_FOR_NOINI;
            //vc_tw1_t3fk=25;//20107-3 add //2013-4-23 delete
    }
    
    
    //2014-5-22增加恢复EEPROM中的map
        
    if(VF_CLR_SL_SYSINI){
        if(VF_MAP_INEE_FLASH){
            Set_Flag_EEinFlash(true);
            Copy_Map_fromFlash2EE(false);//2015-2-2
        }
    }
    
    
    
    
}

void FC_KeyStatus(void)
{
    {    //2016-7-31 这个可能是有问题的，这个时间可能是关钥匙时没有清0；或者关钥匙清0的时刻在这之后的；
        if(v_t_eng_t_s>=M0_T_KEYOFF_FOR_INI_S)
        {
            nb_keyon_forIni=0;    //2009-11-21 CHANGE FROM 0 TO 1
            
            //if(!VF_KEYON_HARD)blnIdleIni=0;//2016-7-31  //2016-7-31

        } 

        if(v_t_eng_t_s>=10)
        {
            if(!VF_KEYON_HARD)blnIdleIni=0;//2016-7-31  //2016-7-31

        }
		
		if(VF_MMOVE_ENG)
		{
			nb_keyon=0;
		} 
    }

}

void FI_SysInit_BeforeStarts(void)
{
    nb_keyon_forIni=0;

    //2010-1-4  add 
    VF_AQAFTER_PNINI_SYSINI=
                    VF_NOINIT_SYSINI //该标志为软标志，即与硬件不一定对应，在(VF_REMOVE_INI_SYSINI||VF_DIRECT_INI_SYSINI)后置位，或确实未调试时开钥匙置位；在调试后就清位
                    &&(!VF_PNNOINI_SYSINI)    //该标志在开钥匙时从EE中获取，在可靠自检后即清位；
                    &&VF_NOTNEED_DROPSCAN_SYSINI      //该标志在开钥匙时置位，只有在需要自检时才清位；
                    &&(VF_1ST_INI_SYSINI);    //2010-3-17 delete VF_1ST_INI_SYSINI   //2010-3-21 ADD BACK;
    
    VF_JFINISHINI_WHENMOVING_SYSINI=false;//2009-11-25 add 
    
    //VF_RUNENOUGH_FORINI_SYSINI=false;//2010-3-21 add ;//2015-12-9 DELETE AT STARTS,防止在发电机上出现停机时出现角标干扰，又有转速的情况
    
    
    //VF_MILES_NEEDT4_SYSINI=VF_MILES_NEEDT4AGAIN_SYSINI=false;
}

//2009-7-31 add 用于清除O值
//功能：
//   用于在动力喷嘴初始化之后，清除已调试标志；
void FC_ti_set(void)
{

    VF_NOINIT_SYSINI=true;  

    vc_tw3offset=TW3OFFSET_FOR_NOINI;//2009-8-25 add 
    VF_AQAFTER_PNINI_SYSINI=true;//2009-11-18 add
    
    blnIdleIni=1;
    
    //2015-12-10 如果是发电机系统，则直接置位
    #ifdef IS_GEN
         FS_AQOK_FLAG(0,0,0,0,true);
         blnIdleIni=0;
    #endif
}


//2009-7-31 add 动力喷嘴已经初始化的标志；
//功能：
//   用于在动力喷嘴初始化之后，设置动力喷嘴已经初始化的标志
//2010-3-13 更改为在关钥匙时，完成该动作；
void FS_PNINIT_FLAG(void)
{
    Save_Data_EE(SET_OK2,(byte)(M0_INJ_SHIFTL_REF+NB_INC_FORINI));  //2009-4-8 add 
}

//怠速调试结束后，设置标志位  2010-3-15
sbyte FS_AQOK_FLAG(sword idle_sc,sbyte fmf_cls,sbyte t3_offset,sbyte v_o,byte blnOk)
{
    sword sw;
    
    if(VF_AQAFTER_PNINI_SYSINI&&blnOk)
    {
        VF_ENAQ_SYSINI=false;
        VF_AQOK_SYSINI=true ;
        VF_NOINIT_SYSINI=false;//2009-11-19 add 
        //2009-11-19
        VF_AQAFTER_PNINI_SYSINI=false;//2009-11-19
        VF_JFINISHINI_WHENMOVING_SYSINI=true;//2009-11-25
        blnIdleIni=0;//2016-7-31
    }
    sw=t3_offset+idle_sc+fmf_cls+v_o;//2010-3-23 add v_o
    if(sw>100)sw=100;
    if(sw<-100)sw=-100;
    return (sbyte)sw;
}

//********setti_pc************************************
//功能：设置偏移量：计算所说的诊断仪的计算功能
//  注意：偏移量２:DTI_T3FK0是在计算过程中，REMARK=6时，上位机对DTINJ_TV3_FK+vc_tw1_t3fk+DCDY_DT计算平均值而得到；
//				然后上位机，将DTI_T3FK0按写正常的EE的方式写入到地址E_Dti_t3fk0_ADR和E_DTI_T3FK0_AB中；
//				因此，写偏移量２的动作是一个普通的写EE的动作，而与写偏移量1(O值)的动作有所区别。
void FS_ti_pc(byte pckese,byte pcset,byte pcbtdc,byte pctback,
                sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset)
{	

    // if(VF_PNNOINI_SYSINI)return; //2009-7-31 add
    if(VF_PNNOINI_SYSINI||VF_ECUNOSET_SYSINI)return; //2009-7-31 add   //2009-8-7 add condiction VF_ECUNOSET_SYSINI
    
    VF_RSTTISCI_SYSINI=false;
    if(pckese==17&&pcset==5){
    		if(pcbtdc==4&&pctback==6){
    			  Save_IdleDebugResult(tw1_idlesc,tw3_sci,tw3_offset,0);
    		}
	    VF_RSTTISCI_SYSINI=true;	
    }
    return;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

/*
    功能：保存初始化结果

    2014-10-7 增加形参
*/
void FM_PNInit_Result(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte CLS_IDLE_av,sword t1_offset)
{
    sword sw;

    if(VF_ECUNOSET_SYSINI)return; //2009-7-31 add   //2009-8-7 add condiction VF_ECUNOSET_SYSINI
                            //2010-3-21 考虑到没有初始化就用的情况,去处条件:VF_PNNOINI_SYSINI
    Save_IdleDebugResult(tw1_idlesc,tw3_sci,tw3_offset,CLS_IDLE_av);

    if(VF_1ST_INI_SYSINI)
    {
        sw=t1_offset;       //vc_tw1_t3fk_now=vc_tw1_t3fk+av_dti_t3fk;
        if(sw>120)sw=120;
        if(sw<-120)sw=-120;
        FS_tw1_t3fk((byte)sw);   //2010-1-4
    }

}

/*存储T1的T3反馈结果*/
void FS_tw1_t3fk(byte b)
{
    Save_Data_EE(E_DTI_T3FK0_ADR,b);//    ?
    Save_Data_EE(E_DTI_T3FK0_AB,b);////2010-1-6 考虑到诊断仪的延续性，r值备份值不加一  
}

//功能:每次结束停车时喷油动作时,都需要调用该程序,对相应的标志位进行置位
void F_FinishSysInitAction_whenStop(byte whatAction)
{
    switch(whatAction)
    {
        case ACTION_WHENSTOP_PREINJ:
            VF_PREINJEND_SYSINI=true;
            break;
        case ACTION_WHENSTOP_PUMP:
            VF_PREPUMPEND_SYSINI=true;
            //VF_NOTNEED_DROPSCAN_SYSINI=true;//2009-3-2     //2009-8-26 delete
            break;
        case ACTION_WHENSTOP_T1T3:VF_NOTNEED_DROPSCAN_SYSINI=true;break;
        case ACTION_WHENSTOP_ALL:
            //VF_NOTNEED_DROPSCAN_SYSINI=true;     //2010-3-14 delete
            VF_PREPUMPEND_SYSINI=true;
            VF_PREINJEND_SYSINI=true;
            break;
    }
}

//*****************ti_shift_get****************************************
//获取偏移量（o值）：temp_ti_set
//若校验O值有误，采用默认值；并置未调试标志；tiN_init_er=true；
//读取上次反馈值：ti_sh_t，若读取无误，并且数据在有效范围内，置tiN_sh_ready
//若上次反馈有效，将反馈值按比列加到偏移量上;并保存；
//local, 
//调用场合：
//1 复位后； 2 设置O值之后；
void Get_Ti_shift(void)   //调用该函数之前需要已经设置了VF_POR_MCU
{
    
    VF_TISETOK_SYSINI=false; //O值有效
    VF_TIER_SYSINI=false;    //O值需要修复

    vc_tw3offset=Load_Key(TI_SET_OK);
#ifdef	SY_HUANSONG		//20211014 chenwei 	环松不做出时候，新ECU出厂，不应该报故障
	VF_TIER_SYSINI=false;
#endif
    VF_TISETOK_SYSINI=!VF_TIER_SYSINI;
    //如果是读取O值，则
    VF_NOINIT_SYSINI=!VF_TISETOK_SYSINI;

    return;
}

/*
    从EEPROM中读取关键参数
    关键参数采用连续5个地址校验的方式，首个地址存储原值
*/
byte Load_Key(word adr){
      byte j;
      byte i;
      byte bd[5];
      byte bmay;
      byte nb;

      bmay=0;
    //读取o值A区的地址的数据
        for(i=0;i<=4;i++){
            bd[i]=Load_Data_EE(adr+i);    //TI_SET_OK    
        }
    
    //校验A区中地址的数据是否有效
        nb=0;
        for(i=0;i<=3;i++){
            if(((byte)(bd[i]-1))==(bd[i+1])){
                 //如果相邻的两个数满足+1的规律, 则可以假定这个可能为O值
                 bmay=(byte)(bd[i]+i);
                 for(j=0;j<=4;j++){
                     if(((byte)(bmay-j))==bd[j])nb++;
                     if(nb>=3)break;   
                 }
            }
            
            if(nb>=3) {
              break;   
            }
        }

        for(i=0;i<=3;i++){
                //如果A区不是5个都满足要求
            if(((byte)(bd[i]-1))!=(bd[i+1]))VF_TIER_SYSINI=true;
        }
    

    
    //如果A区的O值无效,如果读取的是O值
    
    //if(!VF_TISETOK_SYSINI)
    if(nb<3)
    {
          if(adr==TI_SET_OK){
        	    bmay=Load_Data_EE(EE_O_BAK_FLAG1+1);
        	    if(((byte)(bmay+1))==Load_Data_EE(EE_O_BAK_FLAG1+3))nb=3;//VF_TISETOK_SYSINI=true;
          }
    }
    
    
    //if(VF_TIER_SYSINI&&VF_TISETOK_SYSINI)
    if(nb>=3&& VF_TIER_SYSINI)//如果密码合法，且密码中有异常
    {
        //如果O值需要修复,且O值有效
        Save_Data_EE(EE_NBOREC,(Load_Data_EE(EE_NBOREC)+1));//存储O值修复的次数
        
      	Save_Data_EE(adr,bmay);
      	Save_Data_EE(adr+1,(byte)(bmay-1));
      	Save_Data_EE(adr+2,(byte)(bmay-2));
      	Save_Data_EE(adr+3,(byte)(bmay-3));
      	Save_Data_EE(adr+4,(byte)(bmay-4));
    }
    
    if(nb>=3)VF_TIER_SYSINI=false;//密码合法
    else bmay=15;
             
    return bmay;
}

//保存KEY值，2014-3-27
void Save_Key(word adr,byte dat)
{
  	
  	Save_Data_EE(adr,dat);
  	Save_Data_EE(adr+1,(byte)(dat-1));
  	Save_Data_EE(adr+2,(byte)(dat-2));
  	Save_Data_EE(adr+3,(byte)(dat-3));
  	Save_Data_EE(adr+4,(byte)(dat-4));
    
}


//2010-2-6
//add ,sbyte cls;
void Save_IdleDebugResult(sword tw1_idlesc,sword tw3_sci,sbyte tw3_offset,sbyte cls)
{
    sword sw;
    byte temp_ti_set;
    
  	sw=tw1_idlesc+tw3_sci+tw3_offset;

  	//2014-1-19 LIMIT  LDO
  	if(sw>=40)  
  	{
  	    sw=40;
  	}
  	else if(sw<-60)
  	{
  	    sw=-60;
  	}
  	
  	temp_ti_set=(byte)sw;

    Save_Key(TI_SET_OK,temp_ti_set);//2014-3-26

  	Save_Data_EE(EE_O_BAK_FLAG1,EE_VAL_O_BAK_FLG1);
  	Save_Data_EE(EE_O_BAK_FLAG1+1,temp_ti_set);
  	Save_Data_EE(EE_O_BAK_FLAG1+2,EE_VAL_O_BAK_FLG1+1);
  	Save_Data_EE(EE_O_BAK_FLAG1+3,(byte)(temp_ti_set+1));
  	Save_Data_EE(EE_O_BAK_FLAG1+4,EE_VAL_O_BAK_FLG1+2);
  	
  	Save_Data_EE(EE_NBOREC,0);//清除O值出错的历史；
  	
  	Get_Ti_shift();//ti_shift_get();
  	
  	//blnIdleIni=0;//2016-7-31

}



/*
    INITION FOR EEPROM
    两个条件之一 
        1、按规定的命令字节
        2、钥匙开关8下进入初始化
*/
void Init_EE_SL(void)
{
    word j;
    
    for(j=0;j<=255;j++){
        write_eeprom(29,j+256,2,3,0);//+call	write_eeprom//判断条件合适时，将参数写入EE
        write_eeprom(29,j+512,2,3,1);//+call	write_eeprom//判断条件合适时，将参数写入EE
        //PCCQJSJD=show_eeprom(v_PCKESE_SCI_TOCOM,v_PCSET_SCI_TOCOM,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,PCCQJSJD);//2008-4-26 有待验证
    }

}

#ifdef SYS
    #if SYS==SYS_PI
        //*********Check_Ini_Status***************************************
        //从EEPROM中读取以下地址的数据：
        //inj_shift_ok;	inj_slop_ok;	set_year_ok;	set_month_ok;	set_day_ok;			set_ok2;
        //并与下列数据对比，若不等则binj_p_set_er＝true；
        //DT30_MAPH;		DT30_MAPL;		KT3_MAPH;			KT3_MAPL;			inj_shifth_ref;	inj_shiftl_ref
        //使用场合：被LOAD_INJn_PARA调用
        void Check_Ini_Status(void)   //local
        {
            byte SETFLG;
            
            VF_ECUNOSET_SYSINI=true;//binj_p_set_er=true;
        //    VF_ECUNOSET_SYSINI=false;//2009-4-22 临时不管ECU是否编号
            VF_PNNOINI_SYSINI=true;//2009-7-31
            v_nb_injparaOK=0;
            bTemp=Load_Data_EE(INJ_SHIFT_OK);
            CLR_WDG;
            /*
            if((byte)(M0_DT30_MAP>>8)==Load_Data_EE(INJ_SHIFT_OK)){
            		v_nb_injparaOK++;
            		if((byte)M0_DT30_MAP==Load_Data_EE(INJ_SLOP_OK)){
            				v_nb_injparaOK++;
            				if((byte)(M0_KT3_MAP>>8)==Load_Data_EE(SET_YEAR_OK)){
            						v_nb_injparaOK++;
            						if((byte)M0_KT3_MAP==Load_Data_EE(SET_MONTH_OK)){
            								v_nb_injparaOK++;
            								if(M0_INJ_SHIFTH_REF==Load_Data_EE(SET_DAY_OK)){    //该位是在设置ECU编号时，由上位机写入的：4
            										v_nb_injparaOK++;                        //VF_INJRDE5_EE表示已设置编号；
            										//VF_ECUNOSET_SYSINI=false;//2008-11-2 //重新定义VF_NOSET_ECU的含义，不包含喷油器没设定
            										VF_ECUNOSET_SYSINI=false;//2009-7-31
            										//2009-4-6 change to the following:
            									    //if(!VF_TOBEINI_SYSINI)VF_ECUNOSET_SYSINI=false;//2009-7-31 delete	
            										//if(M0_INJ_SHIFTL_REF+==Load_Data_EE(SET_OK2)){ //该位是在设置喷油器参数时，由上位机写入的：0x3e
            										//2009-4-7 change to the following:
            								}
            						}
            				}
            		}
            }
            */
            VF_ECUNOSET_SYSINI=false;//2014-1-19 add in Check_Ini_Status 
            
            
            //2013-10-29 更改如此
        		SETFLG=Load_Data_EE(SET_OK2);
        		if(((byte)(M0_INJ_SHIFTL_REF+NB_INC_FORINI)==SETFLG)
        		   ||((byte)(M0_INJ_SHIFTL_REF)==SETFLG) 
        		){ //该位是在设置喷油器参数时，由上位机写入的：0x3e
        		    //VF_ECUNOSET_SYSINI=false;
        		    VF_PNNOINI_SYSINI=false;//2009-7-31
        		} else{
        		    //2013-10-29 add
        		    //喷嘴初始化标记异常
        		    //判断O值是否正常，如果正常，用该标记修复初始化标记
        		    //if(!VF_TIER_SYSINI&&!VF_NOINIT_SYSINI){
        		    if(VF_TISETOK_SYSINI&&!VF_NOINIT_SYSINI){ //2014-3-27 ADD 
        		    
        		        VF_PNNOINI_SYSINI=false;
                    Save_Data_EE(EE_NBOREC,(Load_Data_EE(EE_NBOREC)+1));//存储O值修复的次数
        		        Save_Data_EE(SET_OK2,M0_INJ_SHIFTL_REF+NB_INC_FORINI);    
        		    }
        		    
        		}
         
            //2013-10-29 将 VF_PNNOINI_SYSINI从VF_ECUNOSET_SYSINI中独立出来；
            //VF_ECUNOSET_SYSINI代表编号标识；
            //以前：编号，且做了喷嘴扫描，即DAT(SET_OK2)=67，则VF_PNNOINI_SYSINI=true
            //更改为：VF_PNNOINI_SYSINI只与DAT(SET_OK2)=67相关
            //且可被O值修复；
            
         
            return;
        }

    #else
        /*校验SCR ECU是否编号*/
        void Check_Ini_Status(void)   //local
        {
            byte SETFLG;
            
            VF_ECUNOSET_SYSINI=true;//binj_p_set_er=true;

            CLR_WDG;

            if(Load_Data_EE(0xF0)==Load_Data_EE(INJ_SHIFT_OK)){

            		if(Load_Data_EE(0xF1)==Load_Data_EE(INJ_SLOP_OK)){

            				if(Load_Data_EE(0xF2)==Load_Data_EE(SET_YEAR_OK)){

            						if(Load_Data_EE(0xF3)==Load_Data_EE(SET_MONTH_OK)){

            								if(4==Load_Data_EE(SET_DAY_OK)){    //该位是在设置ECU编号时，由上位机写入的：4

            										VF_ECUNOSET_SYSINI=false;//2009-7-31

            								}
            						}
            				}
            		}
            }

            
       
            VF_PNNOINI_SYSINI=false;
         
            return;
        }
    
        void Save_DT1_whenKeyoff(sword t1_offset)
        {
            sword sw;

            {
                sw=t1_offset;       //vc_tw1_t3fk_now=vc_tw1_t3fk+av_dti_t3fk;
                if(sw>120)sw=120;
                if(sw<-120)sw=-120;
                FS_tw1_t3fk((byte)sw);   //2010-1-4
            }

        }
        
    
    #endif

#endif    