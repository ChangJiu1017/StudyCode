
#include "act.h"
#include "public.h"
#include "mcu.h"
#include "hard_trig.h"

#include "status_st.h"
#include "ign.h"
#include "dtc.h"

#include "add_ram_MPI.h"
#include "look_up.h"	//20210601 cw add

#include "Events.h"
#include "add_flash_mpi.h"

#include "Service_CAN.h"


//以下为描述事件的参数
//word v_CA_DwellST_toTDC;
sword v_CA_DwellST_toTDC;		//20210927 chenwei word to sword


//时间为单位
word v_t_ign_toTDC;
word v_t_DwellST_toTDC;



//以下为执行参数，从执行齿到事件发生
sbyte v_NO_ignTooth;//点火齿
sbyte v_NO_dwellTooth;//充电齿 


word v_t_TrigTooth_To_Ign;     //触发齿到点火时刻
word v_t_TrigTooth_To_Dwell;   //触发齿到充电时刻

word v_t_TrigTooth_To_Ign_trig;     //触发齿到点火时刻
word v_t_TrigTooth_To_Dwell_trig;   //触发齿到充电时刻

sbyte Check_TrigTooth(sword ca,sword tooth_tdc);
word Get_t_fromTrigTooth_to_Exe(sbyte tooth,sword tooth_TDC,word t_fromExe_to_TDC,byte ign);

word v_NO_Tooth_TDC;//byte  提高精度


word test_20210602;		//20210602 chenwei add
//byte VF_NEED_DWELL;


//=============================================================================================================

void Init_Act(void)
{
  

    v_NO_Tooth_TDC=BYPTR_M0_TOOTH_TDC;//上止点齿号
	
	/*v_NO_ignTooth=BYPTR_M0_TOOTH_TDC/10;//点火齿  //20210224 chenwei ramark 此处写法（含下一行）存在升级工具刷写后油泵间歇动作
    v_NO_dwellTooth=BYPTR_M0_TOOTH_TDC/10-3;//充电齿*///20210224 chenwei 暂时屏蔽

	v_NO_ignTooth=(sbyte)(v_NO_Tooth_TDC/10);//点火齿  
    v_NO_dwellTooth=(sbyte)(v_NO_Tooth_TDC/10-3);//充电齿
		
    v_t_TrigTooth_To_Ign=500;     //触发齿到点火时刻
    v_t_TrigTooth_To_Dwell=500;   //触发齿到充电时刻
    
    v_t_TrigTooth_To_Ign_trig=500;     //触发齿到点火时刻
    v_t_TrigTooth_To_Dwell_trig=500;   //触发齿到充电时刻

	v_nb_cycle=0;	//20211002 chenwei add
    
}

/*
    计算执行参数
    
*/


void Cal_Exe(void)
{
	sbyte sb;

	byte tpd;
	sword sw;
	byte b;
	sword sw_ldb_9000;
	sword sw_ldb_8500;
          
    //计算齿号


    
    //增加对齿位置的判断，在这个区间内的话，就不计算，同时主程序中需要把caL_exe挪到当前条件的外面

   // if(v_ToothPosition<v_NO_Tooth_TDC/10+2)return;//2020-12-18
    //2020-12-18
   /**/ 
	/*if(
	            ((v_ToothPosition>v_NO_Tooth_TDC/10+3)||(VF_NEWCYCLE_ATMAIN_CAS))
				&&(v_prd_cas!=0xefff)//对于以后变齿形状的情况要特别注意12齿
	   )*/
      if(v_prd_cas!=0xefff) 
       {     
       		//P_O2H_ONOFF_TST=!P_O2H_ONOFF_TST;		//20210930 chenwei add for test
            
              /*  
                //计算时间参数
        		if(v_rev_625rpm<REV62D5_3000)
        		{
        			v_t_IgnOn=1200;
        		}
        		else if(v_rev_625rpm<REV62D5_5750)
        		{
        			v_t_IgnOn=800;
        		}
        		else
                {   
                    v_t_IgnOn=600;//2017-1-10
                }*/   //20210601 cw 屏蔽此处，（重庆环松项目需求）改用充磁时间可标定如下：
               if(VF_UB_TPS)tpd=16;else tpd=v_TPS_odr;
			   
				//v_t_IgnOn_LookUp=(word)FG_3DMap_xyz(BYPTR_M2_V_T_IGNON,v_rev_625rpm,v_TPd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,0x00);

				//20210926 chenwei add 改为spped-vbat 查表
				v_t_IgnOn_LookUp=(word)FG_3DMap_xyz((byte *)BYPTR_M2_V_T_IGNON,v_Vbat_inuse,v_rev_625rpm,(byte *)BYPTR_VBAT_M2IGNON,(byte *)BYPTR_N_62D5_M2IGNON,15,15,0x00);
			
				v_t_IgnOn=(v_t_IgnOn_LookUp*250)/25;		//20210926 cw 20210926 提高精度，为25倍
				

				if(v_t_IgnOn<12)	v_t_IgnOn=12;	//20211009 chenwei add 最小充磁限制
				
        		
        		/*if(!VF_BFIRE_ENG)
        		{
        			v_t_IgnOn=1500;
        			
        		}*/		//20210926 担心环松小包体过流，此处取消，采用查表的值

        		if(v_nb_cycle<10)
				{
					v_CA_ignition=0;//一定圈数以内都上止点点火
				}
        		sb=v_CA_ignition;
        		if(sb<0)sb=-sb;
				
        			
        		v_t_ign_toTDC=(word)(((long int)v_prd_cas*sb)/514);//512=360/0.7CA            
                if(v_CA_ignition>=0)
        		{
        			v_t_DwellST_toTDC=v_t_IgnOn+v_t_ign_toTDC;
        		}
        		else
        		{
        			if(v_t_IgnOn>=v_t_ign_toTDC)
        			{
        				v_t_DwellST_toTDC=v_t_IgnOn-v_t_ign_toTDC;
        			}
        			else
        			{
        				//v_t_DwellST_toTDC=0;
						v_t_DwellST_toTDC=v_t_ign_toTDC-v_t_IgnOn;
						//调整充磁时间后，必须严格执行，不允许放到上止点时候充磁，否则可能充磁时间较长损坏高压包
        			}
        			
        		}
        		
                    
                
                //计算角度参数
                sw=(sword)(v_t_DwellST_toTDC*(long int)514/v_prd_cas);
					if(v_CA_ignition>=0)
					{
						v_CA_DwellST_toTDC=sw;
					}

					else
					{
						if(v_t_IgnOn>=v_t_ign_toTDC)
						{
							v_CA_DwellST_toTDC=sw;
						}
						else
						{
							v_CA_DwellST_toTDC=-sw;
						}

					}
				//此处v_CA_DwellST_toTDC需要由word类型改为sword类型
                          
                //计算齿号
                v_NO_ignTooth=Check_TrigTooth((sword)v_CA_ignition,v_NO_Tooth_TDC);//
                v_NO_dwellTooth=Check_TrigTooth((sword)v_CA_DwellST_toTDC,v_NO_Tooth_TDC);

                    
                
                
                //由确定好的执行齿计算载入时间
                v_t_TrigTooth_To_Ign =Get_t_fromTrigTooth_to_Exe(v_NO_ignTooth,v_NO_Tooth_TDC,v_t_ign_toTDC,1);
                v_t_TrigTooth_To_Dwell=Get_t_fromTrigTooth_to_Exe(v_NO_dwellTooth,v_NO_Tooth_TDC,v_t_DwellST_toTDC,0);

				if(v_nb_cycle<10)	//20210930 chenwei add
				{
					
					v_NO_dwellTooth_trig=v_NO_dwellTooth;
					v_NO_ignTooth_trig=(sbyte)(v_NO_Tooth_TDC/10);
					
	                v_t_TrigTooth_To_Ign_trig=100;//v_t_TrigTooth_To_Ign;     //触发齿到点火时刻 300改100
	                v_t_TrigTooth_To_Dwell_trig=v_t_TrigTooth_To_Dwell;   //触发齿到充电时刻
				}	
            
	    }
		//Set_ForExe();
}


/*
    根据确定的触发齿，计算从触发齿到执行的时间
*/
word Get_t_fromTrigTooth_to_Exe(sbyte tooth,sword tooth_TDC,word t_fromExe_to_TDC,byte ign)//ign: 1为点火 0为充磁 
{
    long int lt;
    word dTooth;
	sword sw;
	
	sw=(sword)tooth*10;
    
    //if(tooth<tooth_TDC)tooth+=NB_TOOTH;
    if(sw>tooth_TDC)tooth_TDC+=NB_TOOTH_M10;

    //dTooth=tooth_TDC-tooth;
	dTooth=tooth_TDC-sw;
	
	

    if(ign==0)
	{
		if(v_CA_DwellST_toTDC>=0)
		{
			lt=dTooth*(long int)v_prd_cas/NB_TOOTH_M10-t_fromExe_to_TDC;//此处不会有负数，因为角度为正
		}
		else
		{
			lt=dTooth*(long int)v_prd_cas/NB_TOOTH_M10+t_fromExe_to_TDC;//触发齿的齿号不允许晚于上止点齿号
		}		
		
	}
	else
	{
		if(v_CA_ignition>=0)
		{
			lt=dTooth*(long int)v_prd_cas/NB_TOOTH_M10-t_fromExe_to_TDC;//此处不会有负数，因为角度为正
			 	 
		}
		else
		{
			lt=dTooth*(long int)v_prd_cas/NB_TOOTH_M10+t_fromExe_to_TDC;
				
		}
	}
	
    
    if(lt<25)lt=25;//0.1ms，注意如果60齿以上，2个齿的延迟，按照10000rpm，有可能会0.1ms，可能会有问题。60齿以下，基本不会
    
    return (word)lt;
    
}
/*
    校核触发齿的有效性
*/
sbyte Check_TrigTooth(sword ca,sword tooth_tdc) 
{
    sbyte tooth;
	sword sw;      

	sw=tooth_tdc/10;
	if(v_nb_cycle<10)
	{
		tooth=sw-1-ca*(long int)NB_TOOTH/514;
	}
	else
	{
		tooth=sw-2-ca*(long int)NB_TOOTH/514;
	}
		

	if(tooth>sw)tooth=sw;
       
   //齿号是从0到NB_TOOTH-1，不能小于0，如果小于0，加上NB_TOOTH 
    if(tooth <0)
    {                  
        tooth = tooth + NB_TOOTH;
    }
   //齿号也不能大于NO_MAXTOOTH，另外，也不希望是1号齿，
    if((tooth >UB_NO_TOOTH)||(tooth<2))
    {                  
        tooth= UB_NO_TOOTH;
    }
       
    return tooth;
    
}

