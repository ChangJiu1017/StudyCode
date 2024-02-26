/*HEAD
 ***********************************************************************
 * Filename	:vbat.c
 * Function	:battery calculation		
 * User		:xielian
 * Date		:2019.1.7
 * Version	:V8.0.0
 *
 *** History ***
 * V8.0.0   2019.1.7	xielian 
 *		Base module.
 * 
 * V8.0.1   2019.1.7	xielian
 *		低电压故障诊断避开启动阶段.
 * V8.1.0   2019.3.1	xielian
 *		对启动阶段电压处理的优化.
  *v8.3.2    2020.01.08   ghz
 *      1.增加备用电源、整流器诊断
 *      
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "time_ccl.h"
#include "ee.h"
#include "add.h"


    
/*----------THE FOLLOWING ARE PUBLIC variables----------------------------------------------*/
sbyte vc_TW1_vbatchg;         // 电池电压变动时，对即将触发的T1的修正系数；
sword vc_TW1_vbatdownidle;    // 电池电压下降且怠速时，对即将触发的T1的修正系数； 
sword vc_TW1_vbatUpidle;      // 电池电压下降且怠速时，对即将触发的T1的修正系数； //2008-9-20 add
                             
byte v_Vbat_ad;               //电池电压使用值
                              //根据工况不同，v_Vbat_ad值的赋值方法也不同，

byte v_Vbat_inuse;            //电池电压的使用值 
byte CycleNum;

byte v_vbat_bIdleTrig;        //怠速触发喷油前的电压采样值；

byte t_er_vbat;               //vbat临时故障的次数      //2009-11-25
byte t_er_vbat_toolow;

byte v_vbat_whenkeyoff;

sword v_Vbat_ad_lst;           //上一次的采样值；

//byte VF_ER_REGU;  放到status_st.h中去定义
byte v_t_regu;

byte v_vbat_av_mov;
byte v_vbat_av_nmov;

#define VBAT_MOV_REGU 190	//运行时平均电压门限
#define VBAT_AV_DELTA_REGU 10	//平均电压差

byte v_vbatsp_st;


/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

//电压故障状态的标准，PIECU：
    #define DCDY_LOW_WARM	80//EQU	80	;WHEN	AV_D5	V_VBAT_AD	LOWER	THAN	THIS	AT	WRAM_STATUS,	SET	BATTERY	ER				

    #define DCDY_OFFSET 5//2020-6-22 for CR  13	EQU	13	;VB=0.9+3.2*V_VBAT_AD*5.0/256,	ECU	IN	WHICH	THE	V_VBAT_AD	NOT	SAMP	FROM	KEYIGNI,	V_VBAT_AD	SHOULD	BE	0.55	LESS

    #define DCDY_NO_FUSE    10//保险丝烧断时，电压采样的最大值；即低于该值，认为保险丝断了	

    #define DCDY_15D5 233  //电压过高的限值
    #define DCDY_11V  161  //电压疑似过低的限值
    #define DCDY_12V  177  //发动机运行时，电压过低的限值

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/
byte dcdy_adresult;//	equ	0X1AF			;DCDY的AD采集结果；
byte av_d5dcdy	;//equ	0X193		//2008-6-10 replace by vbat_av			;		
word sum_dcdy	;//equ	0X193					;256 time average of v_Vbat_AD/2	sum_dcdyl
byte vbat_av;    //电池电压平均值 //2008-6-10 add
byte index_av;//add by zhangping 
word v_vbat_ad_sum_8t;//2008-10-14 add   dcdy_adresult的8次的累加值
byte vbat_up_status;//每一位0代表不上升，1代表上升；最末位代表最近次； //2008-10-19 add
                    //为了解决弯梁车在电压上升时，T3PID偏小的情况；		 
byte dcdy_adresult_lst;//2008-10-19
byte v_max_whenstop;//  2009-12-9

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/

/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/
void FI_U_bat_AfterReset(void)
{
    V_VBAT_STATUS=false;  //VF_SAMPED_VBAT=false; 2008-12-16 move from waitmoving to here
    if(VF_VPOR_HARD){
        t_er_vbat=0;//2009-11-25
        t_er_vbat_toolow=0;
		v_t_regu=0;//20210304 chenwei 发现该变量未初始化
    }
    v_max_whenstop=0;

	CycleNum=0;
	
	if(VF_POR_HARD)
	{
		v_vbatsp_st=0;
	}
}

//功能：根据VBAT的采样结果，计算VBAT的使用值、平均值、级别，以及对油量的修正系数；
//形参：adres_vbat_8bit－－电池电压的8位的AD采集结果
void FG_U_bat(word adres_vbat_10bit)
{
    sword sw;
    byte adres_vbat_8bit;
	sbyte Temp_Vbat_inuse;
    
    adres_vbat_8bit=(byte)(adres_vbat_10bit/4);
    
    //将ADC结果-DCDY偏移量作为电池电压采集结果///SAVE_ADCRES_TO_DCDYADCRESULT:?
    vc_TW1_vbatdownidle=0;
    vc_TW1_vbatchg=0;
    vc_TW1_vbatUpidle=0;
    
    VF_VDOWNGT3_VBAT=false;//2008-9-18 add
    
    v_vbat_bIdleTrig=0;//2009-8-24
    
    dcdy_adresult_lst=dcdy_adresult;//2008-10-19
    
    if(!VF_MMOVE_ENG)           //2009-12-9
    {   
        if(v_max_whenstop<dcdy_adresult)v_max_whenstop=dcdy_adresult;
    }


    if(!VF_SAMPED_VBAT && !VF_MMOVE_ENG)
    {   
        //第一次采集电压的处理
        dcdy_adresult=0;
        if(adres_vbat_8bit>=DCDY_OFFSET)dcdy_adresult=adres_vbat_8bit-DCDY_OFFSET;
        v_Vbat_ad=dcdy_adresult;

        VF_SAMPED_VBAT=true;

        sum_dcdy=(word)(((dcdy_adresult)<<8)&0xff00);//在WAITING_MOVE时，对		sum_dcdyh的初始化 //2008-6-10
        vbat_av=(byte)((sum_dcdy)>>8); //2008-6-10
        v_vbat_ad_sum_8t=(((word)vbat_av<<4)&0xff0);//2008-10-14 add
        vbat_up_status=0;//2008-10-19
    }
    else
    {
        if(VF_STARTS_ENG||VF_FORINJ_INJPHASE||!VF_MMOVE_ENG)
        {   
            dcdy_adresult=0;
            if(adres_vbat_8bit>=DCDY_OFFSET)dcdy_adresult=adres_vbat_8bit-DCDY_OFFSET;
            
            vbat_up_status=((vbat_up_status<<1)&0xfe);//2008-10-19
            if((dcdy_adresult>dcdy_adresult_lst))//||(dcdy_adresult>v_Vbat_ad))
            {
                vbat_up_status=vbat_up_status+1;        
            }
            
            //电压8次的移动平均
            v_vbat_ad_sum_8t=v_vbat_ad_sum_8t+dcdy_adresult-v_vbat_ad_sum_8t/16;;//2008-10-14 add
            
            //如果是第一次采集电压，则赋初始值
            if(!VF_SAMPED_VBAT) //2008-6-10 add
            {
                sum_dcdy=(word)(((dcdy_adresult)<<8)&0xff00);//在WAITING_MOVE时，对		sum_dcdyh的初始化 //2008-6-10
                vbat_av=(byte)((sum_dcdy)>>8); //2008-6-10
                VF_SAMPED_VBAT=true;
                v_vbat_ad_sum_8t=(((word)vbat_av<<4)&0xff0);//2008-10-14 add
                vbat_up_status=0;//2008-10-19
            }

            sw=(sword)v_Vbat_ad_lst-dcdy_adresult;//上一次电压与本次电压的差异
            if(sw>1||sw<-1)VF_VDOWNGT3_VBAT=true;//2008-9-18 add   加严VF_VDOWNGT3_VBAT的条件；

            //计算 v_Vbat_ad；
            if(VF_MMOVE_ENG)
            {
                vc_TW1_vbatchg=(sbyte)(v_Vbat_ad_lst-dcdy_adresult);//2008-9-24
                if(!VF_IDLE_ENG&&!VF_STARTS_ENG)    //2008-10-14 change to 
                {
                    if((sw>7||sw<-7))
                    {
                        //如果上一次电压和本次差异大
                        v_Vbat_ad=(byte)(((word)v_Vbat_ad_lst+dcdy_adresult)/2);
                    }
                    else
                    {
                        v_Vbat_ad=(byte)(v_vbat_ad_sum_8t/16); 
                    }
                } 
                //2010-5-17如果是在再给油，且本次电压比上一次低时
               /* else if(VF_CICORECING_CFCI&&dcdy_adresult<v_Vbat_ad_lst)
                {
                    v_Vbat_ad=dcdy_adresult;                
                }*/
                else
                {
                    //2008-10-14 move here
                    //v_Vbat_ad=(byte)(((word)v_Vbat_ad_lst+dcdy_adresult)/2);
					v_Vbat_ad=(byte)(v_vbat_ad_sum_8t/16); //2019-2-27 怠速或启动状态下电压采用滤波后的值
                    //2008-10-14 怠速或启动状态下        

				   /*2019-2-27 删掉，尽量保证怠速电压对流量影响小一些
				   if(vc_TW1_vbatchg>0&&VF_IDLE_ENG)
                    {   //怠速时，电压降低，则
                        v_Vbat_ad=dcdy_adresult;//2008-9-23 电压下降时，使用平均值
                    }*/
                    
                    /*if((VF_IDLE_ENG||VF_STARTS_ENG))
                    {
                        //当本次电压下降较多且处于怠速时  2019-4-1 提高门限到5
                    		if(vc_TW1_vbatchg>=5){
                    				//2019-2-27 删掉v_Vbat_ad=dcdy_adresult;	//2008-9-20 之前就有这句话								;
                    				//v_Vbat_ad=dcdy_adresult;//2019-4-1 恢复，只是将下降门限提高到5
									vc_TW1_vbatdownidle=(sword)(vc_TW1_vbatchg)*6;
                    		}
                    }*/
                }
            } 
            else
            {
                v_Vbat_ad=(byte)(((word)v_Vbat_ad_lst+dcdy_adresult)/2);
            }
            
        }
    }

    if(!VF_IDLE_ENG&&!VF_STARTS_ENG)vbat_up_status=0;//2008-10-19
    if(!VF_MMOVE_ENG)
    {
       index_av+=32; 
    }


    //计算平均电压，并判断平均电压是否有故障
    if(VF_RUN4_TMCCL||(index_av<=32 && !VF_MMOVE_ENG)){//每4圈计算一次平均值；

    		sum_dcdy=sum_dcdy+v_Vbat_ad-vbat_av; //2008-6-10
    		vbat_av=(byte)((sum_dcdy)>>8); //2008-6-10

        VF_CURRENT_ER_VBAT=false;//2009-11-25 add 
    		if((vbat_av<DCDY_12V)&&(vbat_av>DCDY_NO_FUSE)&&(!VF_STARTS_ENG)&&VF_15S_AFTERSTART_TMCCL)VF_CURRENT_ER_VBAT=true;//else{VF_CURRENT_ER_VBAT=false;}//2008-6-10 
		    if(vbat_av>=DCDY_15D5)VF_CURRENT_ER_VBAT=true;//2013-4-20
			
			if(VF_MMOVE_ENG&&VF_15S_AFTERSTART_TMCCL)
			{
				v_vbat_av_mov=vbat_av;
			}
			else if(!VF_MMOVE_ENG)
			{
				v_vbat_av_nmov=vbat_av;
			}


    }

    //2009-8-3change to the following
    //发动机未运行,瞬态电压低于9.9v,认为电压过低;
    //if(VF_STARTS_ENG||(VF_CATCH_CAS&&!VF_BFIRE_ENG))  //保证第一个角标时也进入此处
    //{
       /* //对启动过程中电压的处理
        if(!VF_BFIRE_ENG)
        {                
            sw=v_max_whenstop;    
            sw=sw-dcdy_adresult;
            if(sw<0)sw=0;
            sw=sw/2;
            if(sw>24)sw=24;
            //例如：205，变为133，
            v_Vbat_inuse=dcdy_adresult-sw;
        } else
        {
            if(VF_FORINJ_INJPHASE)
            {
                v_Vbat_inuse=dcdy_adresult;    
            }
        }*/
		//v_Vbat_inuse=v_Vbat_ad;
		
		
		
    
    //}
    //else 
	if(VF_STARTS_ENG||(VF_E0_TPS&&VF_MMOVE_ENG&&VF_FORINJ_INJPHASE))    //2009-12-10 ADD CONTICION
    {
        
        if(dcdy_adresult>=dcdy_adresult_lst)
        {   //上升时
            sw=(sword)dcdy_adresult-v_Vbat_inuse;			
            sw=sw/(4);
            //if(VF_STARTS_ENG)sw=sw/(4);
            if(sw<1)sw=1;
            if(sw>5)sw=5;
			if(VF_STARTS_ENG)
			{
				if(VF_RUN2_TMCCL)//2019-4-25 每圈改为每两圈 VF_NEWCYCLE_ATMAIN_CAS
				{
					v_Vbat_inuse+=sw;//2019-2-28 本函数一圈要调用2~3次
				}
			}
			else
			{
				if(VF_RUN4_TMCCL)v_Vbat_inuse+=sw;
			}
            
            if(v_Vbat_inuse>v_Vbat_ad)v_Vbat_inuse=v_Vbat_ad;    
        } else
        {
            v_Vbat_inuse=v_Vbat_ad;
        }
    } 
    else
    {
        v_Vbat_inuse=v_Vbat_ad;    
    }
   

    //2013-4-20
    if(VF_MMOVE_ENG&&!VF_BFIRE_ENG){
        t_er_vbat=0;
        t_er_vbat_toolow=0;
    }

    //2009-11-25 add 对电压故障的处理如下：
        if(VF_1S_TMCCL&&(VF_NEWCYCLE_ATMAIN_CAS||!VF_MMOVE_ENG))//{;//每1秒判断一次?
        {
            if((((!VF_STARTS_ENG)&&VF_15S_AFTERSTART_TMCCL)||(!VF_MMOVE_ENG))&&(vbat_av<DCDY_11V)&&(v_Vbat_ad>DCDY_NO_FUSE)) //2009-8-3
            {
                if(t_er_vbat_toolow<255)t_er_vbat_toolow++; 
            } 
			else 
			{
				t_er_vbat_toolow=0; 
			}

            if(VF_CURRENT_ER_VBAT)
            {
                if(t_er_vbat<255)t_er_vbat++;
            }
			else 
			{
				t_er_vbat=0;
			}
			
			
			if(VF_1ST_INI_SYSINI&&VF_MMOVE_ENG&&VF_15S_AFTERSTART_TMCCL)
			{
				sw=(sword)v_vbat_av_mov-(sword)v_vbat_av_nmov;
				if((sw<VBAT_AV_DELTA_REGU) && (v_vbat_av_mov<VBAT_MOV_REGU))
				{
					if(v_t_regu<255)v_t_regu++;
				}
				else
				{
					v_t_regu=0;
				}
			}
        }


        //2013-4-20
        if((t_er_vbat_toolow>=30)||(VF_MMOVE_ENG&&(t_er_vbat>90)))VF_ER_VBAT=true;//2019-1-7 发现电压采样函数调用多次，所以计数器每次可能累计2个数，因此将计数时间放大一些
        else VF_ER_VBAT=false;     
        
        //2009-12-31 for no fuse
        if(dcdy_adresult<DCDY_NO_FUSE)
        {
            VF_NOFUSE_VBAT=true;  
            VF_ER_VBAT=true;
        }
		
		if(v_t_regu>10)
		{
			VF_ER_REGU=true;
		}
    
    
    v_Vbat_ad_lst=v_Vbat_ad;//2014-10-8 add
}


//2008-9-20 ADD
void FG_U_bat_BeforeTrig(byte adres_vbat_8bit)
{
    //byte temp2l;
    sword temp2l; //2008-9-20 change to sword
    byte bV;
    
    //将ADC结果-DCDY偏移量作为电池电压采集结果///SAVE_ADCRES_TO_DCDYADCRESULT:?
    vc_TW1_vbatdownidle=0;
    vc_TW1_vbatchg=0;
    vc_TW1_vbatUpidle=0;
    
    //2008-3-31 这是为了避开喷油器驱动时刻
    //当发动机运行且处于喷油圈时
    //利用AD采集结果计算DCDY,并且当发动机运行时，需要对油量进行修正；
    //当处于怠速并且电压下降过多时，累加dcdy_dt，并对油量进行再修正；
    //调用场合：1 主运行循环时，等待点火结束时刻；2 WAITING_MOVE时；
    temp2l=v_Vbat_ad;
    bV=0;
    if(adres_vbat_8bit>=DCDY_OFFSET)bV=adres_vbat_8bit-DCDY_OFFSET;

    //2009-8-24 add
    v_vbat_bIdleTrig=bV;

    if(VF_MMOVE_ENG&&VF_IDLE_ENG)
    { //2008-10-14 add condiction VF_IDLE_ENG
        vc_TW1_vbatchg=((sword)v_Vbat_ad-bV);
    		if(vc_TW1_vbatchg>=3&&VF_IDLE_ENG){;//当本次电压下降且处于怠速时
    			vc_TW1_vbatdownidle=(sword)(vc_TW1_vbatchg)*6;
    		}
    		//2008-9-24 add,对电压少降时，也多增油；
    		else if(vc_TW1_vbatchg>1&&vc_TW1_vbatchg<3)
    		{
    		    vc_TW1_vbatdownidle=(sword)(vc_TW1_vbatchg)*4;    
    		}
		
    }
}

//2013-10-12 获取临时电压采样值
byte GetVbat_Tmp(void){
    
    return FAD_1Channel(CH_AD_VBAT,false,2)/4;//2014-4-5
}

void VbatSpDiagWhenKeyon(void)
{
    if(v_vbatsp_st!=100)
    {
        
    	VFF_VBATSP_ERR=false;		
    	v_vbatsp_st = Load_Data_EE(0x3f);
    	
    	//备用电源诊断		
    	if((v_vbatsp_st==100) || (v_vbatsp_st==111))
    	{
    		VFF_VBATSP_ERR=true;
    	}
    	v_vbatsp_st=100;	
    	Save_Data_EE(0x3f,100);
    }
	
}

void VbatSpDiagWhenKeyoff(void)
{
	if((v_vbatsp_st==101)  && (v_t_eng_t_s>(M0_T_DELAYED_AFTER_KEYOFF_S+10)))
	{
		VFF_VBATSP_ERR=true;
			
		v_vbatsp_st=111;
		Save_Data_EE(0x3f,111);
	}

    if((v_vbatsp_st==100) && (v_t_eng_t_s>3))
	{
		VFF_VBATSP_ERR=false;
			
		v_vbatsp_st=101;
		Save_Data_EE(0x3f,101);
	}
}  

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/

