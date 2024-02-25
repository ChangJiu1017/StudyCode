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
 *		�͵�ѹ������ϱܿ������׶�.
 * V8.1.0   2019.3.1	xielian
 *		�������׶ε�ѹ������Ż�.
  *v8.3.2    2020.01.08   ghz
 *      1.���ӱ��õ�Դ�����������
 *      
 ***********************************************************************
HEAD END*/

#include "Public.h"
#include "Status_st.h"
#include "time_ccl.h"
#include "ee.h"
#include "add.h"


    
/*----------THE FOLLOWING ARE PUBLIC variables----------------------------------------------*/
sbyte vc_TW1_vbatchg;         // ��ص�ѹ�䶯ʱ���Լ���������T1������ϵ����
sword vc_TW1_vbatdownidle;    // ��ص�ѹ�½��ҵ���ʱ���Լ���������T1������ϵ���� 
sword vc_TW1_vbatUpidle;      // ��ص�ѹ�½��ҵ���ʱ���Լ���������T1������ϵ���� //2008-9-20 add
                             
byte v_Vbat_ad;               //��ص�ѹʹ��ֵ
                              //���ݹ�����ͬ��v_Vbat_adֵ�ĸ�ֵ����Ҳ��ͬ��

byte v_Vbat_inuse;            //��ص�ѹ��ʹ��ֵ 
byte CycleNum;

byte v_vbat_bIdleTrig;        //���ٴ�������ǰ�ĵ�ѹ����ֵ��

byte t_er_vbat;               //vbat��ʱ���ϵĴ���      //2009-11-25
byte t_er_vbat_toolow;

byte v_vbat_whenkeyoff;

sword v_Vbat_ad_lst;           //��һ�εĲ���ֵ��

//byte VF_ER_REGU;  �ŵ�status_st.h��ȥ����
byte v_t_regu;

byte v_vbat_av_mov;
byte v_vbat_av_nmov;

#define VBAT_MOV_REGU 190	//����ʱƽ����ѹ����
#define VBAT_AV_DELTA_REGU 10	//ƽ����ѹ��

byte v_vbatsp_st;


/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 

//��ѹ����״̬�ı�׼��PIECU��
    #define DCDY_LOW_WARM	80//EQU	80	;WHEN	AV_D5	V_VBAT_AD	LOWER	THAN	THIS	AT	WRAM_STATUS,	SET	BATTERY	ER				

    #define DCDY_OFFSET 5//2020-6-22 for CR  13	EQU	13	;VB=0.9+3.2*V_VBAT_AD*5.0/256,	ECU	IN	WHICH	THE	V_VBAT_AD	NOT	SAMP	FROM	KEYIGNI,	V_VBAT_AD	SHOULD	BE	0.55	LESS

    #define DCDY_NO_FUSE    10//����˿�ն�ʱ����ѹ���������ֵ�������ڸ�ֵ����Ϊ����˿����	

    #define DCDY_15D5 233  //��ѹ���ߵ���ֵ
    #define DCDY_11V  161  //��ѹ���ƹ��͵���ֵ
    #define DCDY_12V  177  //����������ʱ����ѹ���͵���ֵ

/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/
byte dcdy_adresult;//	equ	0X1AF			;DCDY��AD�ɼ������
byte av_d5dcdy	;//equ	0X193		//2008-6-10 replace by vbat_av			;		
word sum_dcdy	;//equ	0X193					;256 time average of v_Vbat_AD/2	sum_dcdyl
byte vbat_av;    //��ص�ѹƽ��ֵ //2008-6-10 add
byte index_av;//add by zhangping 
word v_vbat_ad_sum_8t;//2008-10-14 add   dcdy_adresult��8�ε��ۼ�ֵ
byte vbat_up_status;//ÿһλ0����������1������������ĩλ��������Σ� //2008-10-19 add
                    //Ϊ�˽���������ڵ�ѹ����ʱ��T3PIDƫС�������		 
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
		v_t_regu=0;//20210304 chenwei ���ָñ���δ��ʼ��
    }
    v_max_whenstop=0;

	CycleNum=0;
	
	if(VF_POR_HARD)
	{
		v_vbatsp_st=0;
	}
}

//���ܣ�����VBAT�Ĳ������������VBAT��ʹ��ֵ��ƽ��ֵ�������Լ�������������ϵ����
//�βΣ�adres_vbat_8bit������ص�ѹ��8λ��AD�ɼ����
void FG_U_bat(word adres_vbat_10bit)
{
    sword sw;
    byte adres_vbat_8bit;
	sbyte Temp_Vbat_inuse;
    
    adres_vbat_8bit=(byte)(adres_vbat_10bit/4);
    
    //��ADC���-DCDYƫ������Ϊ��ص�ѹ�ɼ����///SAVE_ADCRES_TO_DCDYADCRESULT:?
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
        //��һ�βɼ���ѹ�Ĵ���
        dcdy_adresult=0;
        if(adres_vbat_8bit>=DCDY_OFFSET)dcdy_adresult=adres_vbat_8bit-DCDY_OFFSET;
        v_Vbat_ad=dcdy_adresult;

        VF_SAMPED_VBAT=true;

        sum_dcdy=(word)(((dcdy_adresult)<<8)&0xff00);//��WAITING_MOVEʱ����		sum_dcdyh�ĳ�ʼ�� //2008-6-10
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
            
            //��ѹ8�ε��ƶ�ƽ��
            v_vbat_ad_sum_8t=v_vbat_ad_sum_8t+dcdy_adresult-v_vbat_ad_sum_8t/16;;//2008-10-14 add
            
            //����ǵ�һ�βɼ���ѹ���򸳳�ʼֵ
            if(!VF_SAMPED_VBAT) //2008-6-10 add
            {
                sum_dcdy=(word)(((dcdy_adresult)<<8)&0xff00);//��WAITING_MOVEʱ����		sum_dcdyh�ĳ�ʼ�� //2008-6-10
                vbat_av=(byte)((sum_dcdy)>>8); //2008-6-10
                VF_SAMPED_VBAT=true;
                v_vbat_ad_sum_8t=(((word)vbat_av<<4)&0xff0);//2008-10-14 add
                vbat_up_status=0;//2008-10-19
            }

            sw=(sword)v_Vbat_ad_lst-dcdy_adresult;//��һ�ε�ѹ�뱾�ε�ѹ�Ĳ���
            if(sw>1||sw<-1)VF_VDOWNGT3_VBAT=true;//2008-9-18 add   ����VF_VDOWNGT3_VBAT��������

            //���� v_Vbat_ad��
            if(VF_MMOVE_ENG)
            {
                vc_TW1_vbatchg=(sbyte)(v_Vbat_ad_lst-dcdy_adresult);//2008-9-24
                if(!VF_IDLE_ENG&&!VF_STARTS_ENG)    //2008-10-14 change to 
                {
                    if((sw>7||sw<-7))
                    {
                        //�����һ�ε�ѹ�ͱ��β����
                        v_Vbat_ad=(byte)(((word)v_Vbat_ad_lst+dcdy_adresult)/2);
                    }
                    else
                    {
                        v_Vbat_ad=(byte)(v_vbat_ad_sum_8t/16); 
                    }
                } 
                //2010-5-17��������ٸ��ͣ��ұ��ε�ѹ����һ�ε�ʱ
               /* else if(VF_CICORECING_CFCI&&dcdy_adresult<v_Vbat_ad_lst)
                {
                    v_Vbat_ad=dcdy_adresult;                
                }*/
                else
                {
                    //2008-10-14 move here
                    //v_Vbat_ad=(byte)(((word)v_Vbat_ad_lst+dcdy_adresult)/2);
					v_Vbat_ad=(byte)(v_vbat_ad_sum_8t/16); //2019-2-27 ���ٻ�����״̬�µ�ѹ�����˲����ֵ
                    //2008-10-14 ���ٻ�����״̬��        

				   /*2019-2-27 ɾ����������֤���ٵ�ѹ������Ӱ��СһЩ
				   if(vc_TW1_vbatchg>0&&VF_IDLE_ENG)
                    {   //����ʱ����ѹ���ͣ���
                        v_Vbat_ad=dcdy_adresult;//2008-9-23 ��ѹ�½�ʱ��ʹ��ƽ��ֵ
                    }*/
                    
                    /*if((VF_IDLE_ENG||VF_STARTS_ENG))
                    {
                        //�����ε�ѹ�½��϶��Ҵ��ڵ���ʱ  2019-4-1 ������޵�5
                    		if(vc_TW1_vbatchg>=5){
                    				//2019-2-27 ɾ��v_Vbat_ad=dcdy_adresult;	//2008-9-20 ֮ǰ������仰								;
                    				//v_Vbat_ad=dcdy_adresult;//2019-4-1 �ָ���ֻ�ǽ��½�������ߵ�5
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


    //����ƽ����ѹ�����ж�ƽ����ѹ�Ƿ��й���
    if(VF_RUN4_TMCCL||(index_av<=32 && !VF_MMOVE_ENG)){//ÿ4Ȧ����һ��ƽ��ֵ��

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
    //������δ����,˲̬��ѹ����9.9v,��Ϊ��ѹ����;
    //if(VF_STARTS_ENG||(VF_CATCH_CAS&&!VF_BFIRE_ENG))  //��֤��һ���Ǳ�ʱҲ����˴�
    //{
       /* //�����������е�ѹ�Ĵ���
        if(!VF_BFIRE_ENG)
        {                
            sw=v_max_whenstop;    
            sw=sw-dcdy_adresult;
            if(sw<0)sw=0;
            sw=sw/2;
            if(sw>24)sw=24;
            //���磺205����Ϊ133��
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
        {   //����ʱ
            sw=(sword)dcdy_adresult-v_Vbat_inuse;			
            sw=sw/(4);
            //if(VF_STARTS_ENG)sw=sw/(4);
            if(sw<1)sw=1;
            if(sw>5)sw=5;
			if(VF_STARTS_ENG)
			{
				if(VF_RUN2_TMCCL)//2019-4-25 ÿȦ��Ϊÿ��Ȧ VF_NEWCYCLE_ATMAIN_CAS
				{
					v_Vbat_inuse+=sw;//2019-2-28 ������һȦҪ����2~3��
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

    //2009-11-25 add �Ե�ѹ���ϵĴ������£�
        if(VF_1S_TMCCL&&(VF_NEWCYCLE_ATMAIN_CAS||!VF_MMOVE_ENG))//{;//ÿ1���ж�һ��?
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
        if((t_er_vbat_toolow>=30)||(VF_MMOVE_ENG&&(t_er_vbat>90)))VF_ER_VBAT=true;//2019-1-7 ���ֵ�ѹ�����������ö�Σ����Լ�����ÿ�ο����ۼ�2��������˽�����ʱ��Ŵ�һЩ
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
    
    //��ADC���-DCDYƫ������Ϊ��ص�ѹ�ɼ����///SAVE_ADCRES_TO_DCDYADCRESULT:?
    vc_TW1_vbatdownidle=0;
    vc_TW1_vbatchg=0;
    vc_TW1_vbatUpidle=0;
    
    //2008-3-31 ����Ϊ�˱ܿ�����������ʱ��
    //�������������Ҵ�������Ȧʱ
    //����AD�ɼ��������DCDY,���ҵ�����������ʱ����Ҫ����������������
    //�����ڵ��ٲ��ҵ�ѹ�½�����ʱ���ۼ�dcdy_dt����������������������
    //���ó��ϣ�1 ������ѭ��ʱ���ȴ�������ʱ�̣�2 WAITING_MOVEʱ��
    temp2l=v_Vbat_ad;
    bV=0;
    if(adres_vbat_8bit>=DCDY_OFFSET)bV=adres_vbat_8bit-DCDY_OFFSET;

    //2009-8-24 add
    v_vbat_bIdleTrig=bV;

    if(VF_MMOVE_ENG&&VF_IDLE_ENG)
    { //2008-10-14 add condiction VF_IDLE_ENG
        vc_TW1_vbatchg=((sword)v_Vbat_ad-bV);
    		if(vc_TW1_vbatchg>=3&&VF_IDLE_ENG){;//�����ε�ѹ�½��Ҵ��ڵ���ʱ
    			vc_TW1_vbatdownidle=(sword)(vc_TW1_vbatchg)*6;
    		}
    		//2008-9-24 add,�Ե�ѹ�ٽ�ʱ��Ҳ�����ͣ�
    		else if(vc_TW1_vbatchg>1&&vc_TW1_vbatchg<3)
    		{
    		    vc_TW1_vbatdownidle=(sword)(vc_TW1_vbatchg)*4;    
    		}
		
    }
}

//2013-10-12 ��ȡ��ʱ��ѹ����ֵ
byte GetVbat_Tmp(void){
    
    return FAD_1Channel(CH_AD_VBAT,false,2)/4;//2014-4-5
}

void VbatSpDiagWhenKeyon(void)
{
    if(v_vbatsp_st!=100)
    {
        
    	VFF_VBATSP_ERR=false;		
    	v_vbatsp_st = Load_Data_EE(0x3f);
    	
    	//���õ�Դ���		
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

