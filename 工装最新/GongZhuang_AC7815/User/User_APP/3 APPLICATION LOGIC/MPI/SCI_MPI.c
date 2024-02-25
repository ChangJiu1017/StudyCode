//****************************************************************************
// @Project Includes
//****************************************************************************
//#include "TR_SCI0.h"
//#include "PE_Types.h"
#include "FAM.h"
#include "Service_SCI.h"
#include "Public.h"
#include "Status_st.h"

#include "wait_moving.h"
#include "Events_mpi.h"

#include "ad.h"

#include "sci.h"


#include "dtc.h"

#include "ign.h"
#include "inj_phase.h"

#include "time_ccl.h"

#include "look_up.h"


#include "inj_notystdc.h"
#include "inj_ctl.h"

#include "data_inj_source.h"


#include "ee.h"

#include "Sys_Init.h"

#include "status_eng.h"

#include "stepmotor.h"

#include "lock.h"


//#include "events.h"

#include "ld.h"

//#include "s12sci.h"//2014-8-5

#include "frame.h"


//----------------------------------------------------------------------------
word bDebug[7];
byte TX_DATA[28];//2008-5-29 change from 20 to 30	
//byte bKesei[30];  //2009-8-17 add
//----------------------------------------------------------------------------

const byte  MS_PC_SET_SELB = 4; //2014-9-25 move here from public 

word t_Cal_1cycle;//mcu计算一圈需要的时间；//2008-8-10
word	t_left_1cycle;    //2012-10-12一个循环等待角标的时间
word	t_left_1cycle_lst; //上个循环等待角标的时间

byte v_NB_TX; 
byte v_NB_TX_max;            
byte nb_o2_disp;//2008-11-17 for disp nb_o2_disp
word t_lstCom_4us;

//下面是和SCI的定义相关的
//2014-9-25 MOVE FROM INT_SERVE.C TO SCI.C
byte PCCQJSJD ;
sbyte v_PCTBACK_SCI;						 												
sbyte v_PCBTDC_SCI;																							
sbyte v_PCKESE_SCI;
sbyte cmdAdjT1; //2012-10-22
sbyte cmdAdjIGN; //2012-10-22
sbyte cmdAdjCOS; //2012-10-22


sbyte cmdAdjO; //2014-8-25

word v_PCSET_SCI_TOCOM;
sbyte v_PCKESE_SCI_TOCOM;
sbyte v_PCBTDC_SCI_TOCOM;
sbyte v_PCTBACK_SCI_TOCOM;

byte v_nb_test;
byte v_nb_test2;
byte v_nb_test3;



extern byte t_pair_er;

/*------------------------------------------------------------------------------------------*/
//通讯协议2      //2014-10-22 移入
 byte u_code;
byte cmdRX[2][BYTE_MAX];
byte v_index_rx_tmp;
byte v_num_rx_tmp;    //当前接收缓冲区
byte v_num_rx_decode;  //需要解析的数组内的字节的个数

word t_rx_lst_4us;    //上一次接收到串口数据的时间
byte v_sta_cmd;       //防盗通讯模式下的状态

byte v_nb_rx;//接收到的数量
byte v_bchk;//2013-9-10

byte v_pw[6];//密码

//2015-10-26 define FOR SYS_PI
    sbyte v_Adj[45];



//-----------------------------------------------------------------------------
#define SAMPEDDATS TX_DATA[3]	//	0X168				;28H;****USED	AS	A	CONSTANT	FOR	DATA	TRANSFER		
#define PRD_L TX_DATA[3]	//	0X168																
#define PRD_H TX_DATA[4]	//	169H									
#define TPS TX_DATA[5]	//	16AH									
#define TPD TX_DATA[6]	//	16BH									
#define OS TX_DATA[7]	//	16CH							;V_O2_AD	FORM							
#define TES TX_DATA[8]	//	16DH							;????								
#define VBAT TX_DATA[9]	//	16EH							;????????								
#define DTC TX_DATA[10]	//	16FH								;ERRO	CODE,	DATA	MODICO2??					
#define TX_REMARK TX_DATA[11]	//	0X170			;130H	;REMARK	??							
#define SUM_SAMPDAT TX_DATA[12]	//	171H				;????????9??????????????????										
#define WORKDATS TX_DATA[13]	//	172H					;***	USED	AS	A	CONSTANT	FOR	DATA	TRANSFER			
#define T3_L TX_DATA[13]	//	172H						;	MEMORY	FOR	DATA	TRANS	TO	PC	WPW??		
#define T3_H TX_DATA[14]	//	173H										
#define O_L TX_DATA[15]	//	174H						;CCT	??INC_WPW??								
#define O_H TX_DATA[16]	//	175H										
#define T1_D8 TX_DATA[17]	//	176H						;RPW??									
#define IGN TX_DATA[18]	//	177H													
#define DTFI_L TX_DATA[19]	//	178H													
#define DTFI_H TX_DATA[20]	    //	179H						;DTFI	FORM											
#define TX_KESEI TX_DATA[21]	    //	17AH						;KESAI??												
#define SUM_WORKDAT TX_DATA[22]	//	17BH				;????????9??????????????????		

/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 


/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
byte v_nb_keyfail;//2013-3-31
unsigned long int v_t_wdg_key_4us;//2013-
word v_tmr_wdg_lst;
//#define T_1S_4US (unsigned long int)250000 //2015-10-30 delete 
#define T_4S_4US (unsigned long int)1000000  
#define ECU_DEBUG 1

//*************************************看门狗程序定时清除没有加进去*******************************8
void F_CLR_WDG(void)
{
    
   // SRS = 0xFF;       2013-12-16 ZZQ 没有加看门狗的清除程序

    
}
//2012-10-22 ADD
/*功能：用于作为在T1位置的标定参数的调节*/
void Get_Tback_for_CAB(sbyte cmd){
    cmdAdjT1+=cmd;        
}
//功能：
//  显示、并处理标定数据
//cmd:
//      0:不增不减
//      1 :
//      -1:
void Get_Kesei_for_CAB(sbyte cmd,sbyte cmd_w)
{
    char add;

    //return; //2013-12-5
    if(VF_MMOVE_ENG)return;
    
    if((KESEI==18) ||(KESEI==(-1)))
    {
        add=REMARK*12;
        TX_DATA[7]=Load_Data_EE(add);    

        add++;
        TX_DATA[8]=Load_Data_EE(add);    

        add++;
        TX_DATA[9]=Load_Data_EE(add);    

        add++;
        TX_DATA[10]=Load_Data_EE(add);    

        add++;
        TX_DATA[13]=Load_Data_EE(add);    

        add++;
        TX_DATA[14]=Load_Data_EE(add);    

        add++;
        TX_DATA[15]=Load_Data_EE(add);    

        add++;
        TX_DATA[16]=Load_Data_EE(add);    

        add++;
        TX_DATA[17]=Load_Data_EE(add);    

        add++;
        TX_DATA[18]=Load_Data_EE(add);    

        add++;
        TX_DATA[19]=Load_Data_EE(add);    

    }
    
} 

void FI_SCI_AfterReset(void)
{
    byte i;

    //v_TW3_SCI=15;   2008-3-25 这句话不合理，需要删除
    //if((REMARK!=8&&REMARK!=7&&REMARK!=5&&REMARK!=9)||VF_VPOR_HARD||REMARK<4) //2008-10-23 add REMARK<4
        //2008-5-20 add this condiction                                             //2008-11-7 delete this condiction
    if(VF_VPOR_HARD)v_PCSET_SCIH=0; //2008-10-23 ADD
    //2008-6-25 move to FI_AfterInjAction_SCI_AfterReset;
    if(VF_VPOR_HARD)
    {
        v_PCSET_SCI=MC_NORMAL;//2012-6-21 MOVE HERE
    }
    
    //2008-8-11 addd to here
    if(REMARK==MC_NORMAL)v_PCKESE_SCI=0  ;//2012-12-20 ADD CONDICTION
    
    v_PCBTDC_SCI=0  ; //2009-12-19 temp
    v_PCTBACK_SCI=0 ;

    cmdAdjT1=0;
    cmdAdjIGN=0;
    cmdAdjCOS=0;
    
    //2015-12-16
    for(i=0;i<ADJ_NB;i++){
        v_Adj[i]=0;
    }

}

//2008-6-25
void FI_AfterInjAction_SCI_AfterReset(void)
{
    // v_PCSET_SCI=1; //2008-6-25 add
    v_PCSET_SCI=MC_NORMAL;//2009-4-18 change to 4

    //2014-4-19 add 
    v_PCKESE_SCI=   0;
    v_PCBTDC_SCI=   0;
    v_PCTBACK_SCI=  0;

}


void Reset_SCI_afterSettipc(void)
{
    //TOBECHECK,是否是指针传递
    //-移动到该程序被调用之后,从 Set_ti_pc移出
    v_PCKESE_SCI=   0;
    v_PCSET_SCI=MS_PC_SET_SELB;
    v_PCBTDC_SCI=   0;
    v_PCTBACK_SCI=  0;
    v_FMF_SCI_idle= 0;//v_TW3_SCI_H,v_TW3_SCI_L=0;
    v_FMF_SCI_Nidle=0;//2008-8-2 add
} 




void TOPC(void) 
{ 
    
    word w;
    sword sw;
    
    word RAM_T3;
    word RAM_DTFI;
    word RAM_O;
    byte b;
    

    if(VF_RSTTISCI_SYSINI)
    {
        Reset_SCI_afterSettipc();//2008-2-28 add by zhangping 从 Set_ti_pc中挪出
        VF_RSTTISCI_SYSINI=false;
    }
    
    
    w=T_MAINTMR-t_lstCom_4us;
    if(w>5000)VF_TRANSING_INTSCI=0;//2014-3-28

    //15ms--3750  //25ms--6250            //20MS=5000
    if(
        (SCI0SR1_TDRE&&!VF_TRANSING_INTSCI)
        &&
        ((w>=5000&&!VF_LINKPC_INTSCI )|| (VF_LINKPC_INTSCI&&VF_PCWANT_INTSCI))
        //((VF_LINKPC_INTSCI&&VF_PCWANT_INTSCI))
        )
    {
        VF_BYTPD_ETC=false;
    
        //2015-10-30
        //    CAN_SendFrameExt(0x010101|CAN_EXTENDED_FRAME_ID,0,8,&_STRU_PARAOUT.Byte);

    
    
        if(VF_MMOVE_ENG)
        {
            v_PCSET_SCI_TOCOM   =v_PCSET_SCI;
            v_PCKESE_SCI_TOCOM  =v_PCKESE_SCI;
            v_PCBTDC_SCI_TOCOM  =v_PCBTDC_SCI;
            v_PCTBACK_SCI_TOCOM =v_PCTBACK_SCI;
        }

        //2013-2-21 MOVE HERE
        PCCQJSJD=show_eeprom(v_PCKESE_SCI_TOCOM,v_PCSET_SCI_TOCOM,v_PCBTDC_SCI_TOCOM,v_PCTBACK_SCI_TOCOM,PCCQJSJD);//2008-4-26 有待验证

        VF_PCWANT_INTSCI=false;
        t_lstCom_4us=T_MAINTMR;//2008-12-4 ADD
        //用于与PC数据通讯的字节，它们的地址是连续的	
    /*通用参数的赋值*/
        /*发动机转速*/
        w=v_prd_cas;
        PRD_H=(byte)((w>>8)&0xff);
        PRD_L=(byte)(w&0xff);
        
        /*节气门位置采样值*/
        TPS=v_TPS;

        /*节气门级别*/
        if(VF_MAX_TPS)
        {
            TPD=0x40;
            
        }
        else
        {
            TPD=v_TPS_odr;
            TPD=(TPD<<2)&0x3c;
        }

        TPD+=(v_TPS_mode>>2)&0x03;
        
        
        /*氧传感器采样值*/
        OS=v_O2_ad;//VF_FORINJ_INJPHASE     


        
        /*电压采样值*/
        if(REMARK==MC_OPENLOOP)
        {
            VBAT=dcdy_adresult;
        } else if(REMARK<MC_OPENLOOP)
        {
            VBAT=v_Vbat_ad;
        }else VBAT=v_Vbat_inuse;
        
        /*发动机温度采样*/
        //TES=V_Tw;//v_teng_ad; //2008-10-6 swap 
        TES=v_teng_ad; //2012-10-24-3
        
        /*DTC*/
        DTC=FG_DispCode_ForPN0Cos();//2008-11-14
        
        /*KESEI*/
        TX_KESEI=KESEI; 

        /*T3位置的显示*/
        RAM_T3=v_t_T1;//v_TW3;

        
        /*点火角位置的显示*/
        if(!VF_MMOVE_ENG&&(KESEI>=20))IGN=PCBTDC;
        else if(!VF_MMOVE_ENG) IGN=BYPTR_M0_CAS_FRONTEDGE;  //2008-12-16 ADD 
        else 
        {
            if(VF_IGNONLY_CFCI)IGN=0;
            //else IGN=BYPTR_M0_CAS_FRONTEDGE-v_CA_ignition;//2013-7-8 暂时 
            else{
                IGN=v_CA_ignition;   //2010-5-4 
                //2013-9-13; 对IGN的限制显示
                //if(REMARK!=MC_DTC)
                if(REMARK==MC_NORMAL) //2014-3-12 临时
                {//2014-2-14纠正
                    //if(v_CA_ignition<7)IGN=7;
                }
            }
        }

        /*T1位置的显示*/
        if(!VF_MMOVE_ENG&&KESEI>=20)T1_D8=PCTBACK;
        else if(VF_IGNONLY_CFCI&&VF_MMOVE_ENG){
            T1_D8=0;    
        }
        else
        {
            w=v_t_T1;//w=v_TW1_forTrig;
            w=(w>>3)&0x1fff;
            if(w>255)w=255;
            T1_D8=(byte)w;
        }
        
        /*DTFI位置的显示*/
        if(!VF_MMOVE_ENG)
        {
            RAM_DTFI=v_TW4;        //2008-12-1 add for TW4;
        }
        else RAM_DTFI=(sword)(v_dld+v_dmi);

        
        /*O值位置的显示*/

        RAM_O=(sword)(v_fmf_O);
     
    /*标定参数的赋值*/
        if(KESEI<17 && !VF_DOM_SYS){   //2013-1-15 ADD =
            //2014-8-10 ADD !VF_DOM_SYS
           switch(REMARK){
              case MC_STOP:
                TES=nb_reset;       //2012-11-16 add
                VBAT=nb_reset_lst;

              case MC_OPENLOOP:
			  TPS=v_LAMBDA_byMAPS;
			  
              break;
              case MC_NORMAL:
			  
			  
              break;

              case MC_LDB:
                TES=(sbyte)v_te;
                TPS=v_LAMBDA_byMAPS; 
				
				#ifdef SYS_STEPMOTOR
					
					v_Adj[ADJ_STEPBASE]+=cmdAdjCOS;
					DTC=v_stepbase;
				#endif
                if(VF_MMOVE_ENG){
                  
                    RAM_DTFI=(sword)v_dmLambda;
                    RAM_T3=v_ldb;
                    
                    if(KESEI==1){
                        //mti的标定模式
                        
                        RAM_O=v_ld;  

                        VBAT=v_mpa;
                        TES=v_mti;
                        #ifdef SYS_STEPMOTOR
                        DTC=v_t_IACV_on; //2016-9-11
						#endif
                        //2015-5-5
                        //TPS=vc_tw3_t35;
                        //RAM_T3=v_TW3;
                        //RAM_O=v_dtw3_obj;
                    }
                    
                    
                    //VBAT=v_dld_dtps;//2015-5-19
                     
                }

                
              break;
              case MC_LDL:
                RAM_DTFI=(sword)v_dmLambda;
                RAM_O=(sword)v_SL_cls;

                TPS=v_LAMBDA_byMAPS;
                
                //TPS=v_status_IDLESL ;//2019-1-25 PETER
                DTC=v_status_IDLESL ;//2019-2-1 PETER   b3
                
                if(KESEI>0)DTC=v_DTC_SL;
                if(KESEI>3)DTC=v_nb_SL;
                
                
                if(KESEI==1){
                    TES=v_cls_sl_learn;

                    TPS=v_cls_sl_cur;//2015-5-20

                } else if(KESEI==2){
                    VBAT=v_TPd;
                    TES=v_rev_deg;
                    TPS=v_ccl_sl;//2014-4-19
                    //VBAT=v_TPd_rd;
                    //TES=v_REV_rd;
                } else if(KESEI==3){
                    
                    TPS=v_SL_map_wr[0];
                                     //刚学到的值，需要变工况后再学到
                    VBAT=v_SL_add_WR[0];              //2013-3-11
                    if(!VF_TOBELEARN_CLSSL)T1_D8=v_map_SL_basic;
                    if(VF_ISWRITING_EE){
                        VBAT=v_flag_eewr;
                        TPS=v_flag_chkwr;
                    }
                
                } else if(KESEI==4){
                
                    TES=numb_fk_m; //REMARK=7,KESEI=4时的TES值看一下
                    VBAT=v_nb_flu_lstIdle;
                    
                }
                else if(KESEI==5){
                    DTC=(VF_TMPOK_CLSSLbak<<0|VF_LEARNPOINT_CHG_CLSSLbak<<1|VF_LEARNECONDICTION_CLSSLbak<<2|  //满足学习条件
												 VF_CONDICTIONSTEADY_CLSSLbak<<3|VF_CONDICTIONSTEADY_LST_CLSSLbak<<4|  //上一次，满足学习条件，且稳定一定时间
												 VF_EE_ER_CLSSLbak<<5|VF_MATCH_INI_CLSSLbak<<6);//V_CLSSLH_STATUS;
                    TPS=(VF_DTCOK_CLSSLbak<<0|VF_EECHKED_CLSSLbak<<1|VF_LIMITLEARN_CLSSLbak<<2|VF_MAYLEARN_CLSSLbak<<3|\
												 VF_RD_NEW_CLSSLbak<<4|VF_STIO_CLSSLbak<<5|VF_TOBELEARN_CLSSLbak<<6|VF_FKSTABLE_CLSSLbak<<7);     //反馈值稳定　2013-3-7);
                    VBAT=v_OS_flu_SLpoint;
                    TES=(VF_CCLUP_CLSbak<<0|VF_LEANCON_CLSbak<<1|VF_LIMITFK_CLSbak<<2|VF_ENRICHLIMITFK_CLSbak<<3|\
												 VF_T35_CLSbak<<4|VF_1LEAN_NOTIDLE_CLSbak<<5|VF_RICH_MORE_CLSbak<<6|VF_LEAN_MORE_CLSbak<<7);//V_CLS_STATUS;
                } else if(KESEI==6){
                    DTC=v_nb_SL;
                    TPS=v_SL_map_wr[0];
                    TES=v_add_sl_steady;                 //刚学到的值，需要变工况后再学到
                    VBAT=v_SL_add_WR[0];              //2013-3-11
                }  
                if(v_nb_wr_fail>0&&VF_TOBELEARN_CLSSL){
                    T1_D8=v_nb_wr_fail;  
                }


                if(!VF_MMOVE_ENG){
                  
                    T1_D8=v_PCTBACK_SCI_TOCOM;
                    IGN=PCBTDC;
                }
                //if(VF_STIO_CLSSL)DTC=v_nb_flu_forSL; //2014-1-17 ADD 

              break;
              case MC_NOZZLE:
                    TPS=v_LAMBDA_byMAPS; 

                    RAM_DTFI=(sword)v_dmLambda;
                    if(!VF_MMOVE_ENG)RAM_DTFI=v_gama_inuse;

                    
                    
                    if(KESEI<=1){
                        RAM_T3=v_dtw3;
                        RAM_O=(v_dtw3_obj_lst);
                        
                        sw=(sword)vc_tw1_t3FK_I+vc_tw1_t3FK_P+v_vbat_dt_TW3FK+vc_tw1_t3fk;//2008-10-24 CHANGE TO v_vbat_dt_TW3FK
					//	sw=v_dt1_ave+v_vbat_dt_TW3FK+vc_tw1_t3fk;
                        sw/=2;
                            if(sw>127)sw=127;
                            if(sw<-127)sw=-127;
                        DTC=(byte)sw;
                    }
                    
                    //2015-4-6 ADD FOR NOZZLE PARA METER
                    //
                    if(!VF_MMOVE_ENG&&KESEI==1){
                        DTC=M1_DT3_Q[10];
                        TPS=M1_DT3_Q[6];
                        RAM_O=aa10_8;
                    }
                    
                    if(VF_MMOVE_ENG){
                        if(KESEI==1){
                            RAM_DTFI=v_mi;
                            
                        } else if(KESEI==2){
                            RAM_O=v_TW1_forTrig;
                            RAM_DTFI=v_TW3_in;
                            DTC=vc_tw3_t35;
                            
                            //2015-5-5 临时
                            //TPS= v_dmLambda;//
                            
                            //2015-5-5
                            if(v_nb_T3_miss>1)
                            {
                                TPS=v_nb_T3_miss;
                            }
                            
                            
                        } else if(KESEI==3){
                            VBAT=v_nb_inj;
                            DTC=v_nb_injection_ccl;
                            OS=v_nb_binj;

                            RAM_O=(vc_FMF_dtfi_trans);

                            RAM_DTFI=v_TPS_addt;

                        } else if(KESEI==4){
                            VBAT=v_nb_rinj;
                            DTC=v_nb_rinj_y;

                            RAM_O=(vc_FMF_dtfi_trans);

                            RAM_DTFI=v_TPS_addt;
                        } else if(KESEI==5){
                            DTC=nb_t30_added;//2015-5-22
                            VBAT=no_t3_fk_run;
                        } else if(KESEI==6){
                            RAM_O= 	aa10_8;//	177 
                            RAM_DTFI= 	ba10_8;//	-1984 
                            RAM_T3= 	ca10_8;//	4356 

                        }else if(KESEI==7){
                            RAM_T3=v_dtw3;
                            RAM_O=v_TW1_forTrig;
                            RAM_DTFI=v_TW3_in;
                            TPS=v_dtw3_obj_lst;
                        }
                        
                    }

              break;
              case MC_MPA:

                if(VF_MMOVE_ENG){
                  
                    TPS=v_LAMBDA_byMAPS; //2012-6-19
                    RAM_O=v_ld;
                    TES=v_te;
                    VBAT=v_pair_co;
                    
                    if(KESEI<=3)
                    {
                        v_mpa+=cmdAdjT1;
                        v_Adj[ADJ_mpa]+=cmdAdjT1;

                        T1_D8=v_mpa;
                        
                        if(KESEI==1){
                              RAM_O=v_ldb;
                              //VBAT=((v_pair_odr<<4)&0xf0)+v_pair_mode;
                             // VBAT=v_pair_deg;2013-6-6 DELETETMP
                        } else if(KESEI==2) {
                            TES=v_pair_co;
                            DTC=v_pair_ad;
                        }
                        else if(KESEI==3){
                            RAM_T3=(sword)Co_FMF_Altitude; 
                            TES=v_pair_ee;
                            DTC=v_pair_ad;
                        }
                    } else {
                        if(KESEI==4){
                            v_T_M64+=cmdAdjT1;
                            v_Adj[ADJ_T]+=cmdAdjT1;
                            T1_D8=v_T_M64;
                            RAM_O=v_ld;
                            
                            VBAT=v_mti;
                            
                        } else if(KESEI==5){
                            VBAT=v_tair_inuse;
                            TES=V_Ta;
                             
                        } else if(KESEI==6){
                            VBAT=v_teng_ad;
                            TES=v_te;
                        } else if(KESEI==7){
                            VBAT=v_teng_inuse;
                            TES=v_teng_ad;

                        } else if(KESEI==8){
                            VBAT=v_tair_inuse;
                            TES=v_tair_ad;

                        }
                        //DTC=V_Ta;
                        
                    }

                    RAM_DTFI=(sword)v_dmLambda;
                    
                    
                }


              break;

              case MC_PAIR:
                  //与上位机一致,不能随意更改
                    TPS=v_pair_ad;  //与上位机一致,不能随意更改
                    TES=v_LAMBDA_byMAPS;      //2013-8-20 CHANGE BACK
                    OS=v_Tmp_bymcu;   //    2010-1-29 change  //MCU采集的温度；
                                
                    if(KESEI==1)
                    {
                        TES=v_KPT;
                        VBAT=v_Tmp_KPT;
                        OS=v_KPT_L;//2010-1-12
						T1_D8=VF_WITHPAIRS_PRO2+(VF_1PAIRSGOT_PAIR<<1)+(VF_ECUHARD_WITHPAIR_PAIR<<2)+(VF_ECUHARD_WITHOUTPAIR_PAIR<<3)+(VF_PAIR_BYMANNUAL_PAIR<<4)+(VF_ER_PAIR<<5)+(VF_VALUEERINEE_PAIR<<6);

                        //DTC=v_dPairMax;   2013-6-6 DELETETMP
                    } 
                    else if(KESEI==2){
                        DTC=nb_forpair;
                        TES=v_pair_ad;
                        TPS=v_pair_ad_input;   //PAIRS A/D采集的直接输入值，需要经过诊断之后才可用；

                        OS=v_pair_ad_64t;//64采样的平均,   2013-6-6 DELETETMP

                        RAM_O=(v_t_pairs_1s);  //2013-6-6 DELETETMP

                    }
                    /* 2013-6-11 DELETETMP*/
                    else if(KESEI==3){
                        
                        TPS=v_pair_ad_input;   //PAIRS A/D采集的直接输入值，需要经过诊断之后才可用；
                        DTC=v_pair_ad_input_min;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最小值
                        OS=v_pair_ad;
                        TES=v_pair_ad_input_max;//在VF_PERIOD_PAIRS期间内采集到的PAIRS的最大值
                        VBAT=v_vbat_maxwhenpair;//       v_vbat_maxwhenpair= MAX_VBAT
                        
                        RAM_O=(v_pair_ee);
                   
                    }
                    else if(KESEI==4){
                        TPS=v_pair_ad_input;
                        DTC=v_pair_min_afterkeyon;//
                        //OS=v_pair_odr*16+v_pair_mode;
                        OS=v_Pair_deg;//2013-5-2     2013-6-6 DELETETMP
                        TES=v_pair_max_afterkeyon;//
                        VBAT=v_vbat_minwhenpair;

                        RAM_O=(v_pair_co);
                    
                    } else if(KESEI==5){
                        TPS=v_pair_ad_input;
                        DTC=v_pair_ad_64t;  //  2013-6-6 DELETETMP
                        OS=v_pair_ad;
                        TES=v_pair_aveinlast10min;//

                        RAM_O=(v_t_pairs_1s);
						T1_D8=VF_ER_PAIR+(VF_NOTCAB_PAIR<<1)+(VF_VALUEERINEE_PAIR<<2)+(VF_SOFTCFG_ERR_PAIRS<<3);
						RAM_T3=t_pair_er;
                    } 
                    else if(KESEI==6)
                    {   //REMARK=11,KESEI=6
                        DTC=0;
                        if(VF_ER_PAIR)DTC+=1;
                        if(VF_NOTCAB_PAIR)DTC+=2;
                        if(VF_VALUEERINEE_PAIR)DTC+=4;
                        if(VF_SOFTCFG_ERR_PAIRS)DTC+=8;
                    }
                    
              break;
              case MC_U:
                RAM_T3=v_ld;
                RAM_DTFI=v_msMu;



                if(KESEI<=4)VBAT=V_Ta;
                
                TES=V_Tw;
                
                TPS=v_LAMBDA_byMAPS; 
                
                
                if(KESEI==3)TPS=v_t_eng_1s;

                //DTC=Tc/2;//v_u_M128_Cold/5;
                DTC=v_twb;//计算TW的基础值2013-3-26   //not in plan
                VBAT=V_Ta;
                if(VF_CICORECING_CFCI){//2013-4-22
                    DTC=v_UR_M16_ex;
                    VBAT=v_ccl_cf_bak;
                }
                
                if(KESEI==0){
                  v_du_M128_M5+=cmdAdjT1;
                  T1_D8=v_u_M128_Cold/5;


                  RAM_O=(sword)(v_dmi);
				  
				  DTC=v_step_IACV_on;//test IACV
				  //RAM_O=rev15d6_target;
				  
                  
                } else if(KESEI==1){

                  T1_D8=v_M0_E_M8;
                    v_Adj[ADJ_E]+=cmdAdjT1;
                }
                else if(KESEI==2){

                    v_Adj[ADJ_F]+=cmdAdjT1;
                  T1_D8=v_M0_F_M256;
                
                } 
                //2012-12-20 增加
                else if(KESEI==3){
                    v_Adj[ADJ_G]+=cmdAdjT1;

                    T1_D8=v_G_M128;
                    
                    IGN=v_Gx_M128;//2015-2-2
                    
                    DTC=v_sq;//2015-2-2
                    
                    RAM_O=v_sigma_M2048/256;
                    
                    VBAT=deTPS;
                    //TPS=v_t_eng_1s;
                    //2016-4-3
                    RAM_T3=v_tao_M16;
                    RAM_DTFI=v_tao_M16>>16;
                    
                    if(!VF_MMOVE_ENG)OS=deOS; //2016-4-4
                    
                } else if(KESEI==4){

                    v_Adj[ADJ_E]+=cmdAdjT1;
                    T1_D8=v_M0_E_M8;
                    
                    v_Adj[ADJ_F]+=cmdAdjIGN;
                    IGN=v_M0_F_M256;
                    
                    v_Adj[ADJ_G]+=cmdAdjCOS;



                    T1_D8=v_M0_E_M8;
                    
                    IGN=v_M0_F_M256;
                    
                    DTC=v_G_M128;
                    
                } else if(KESEI==5){
                    T1_D8=v_us_M16;
                }
                else if(KESEI==6)
                {
                }
                else if(KESEI==7)
                {
                }
                else if(KESEI==8)
                {
                }
                else if(KESEI==9)
                {
                    v_R_d4+=cmdAdjT1;
                        v_Adj[ADJ_R]+=cmdAdjT1;
                    T1_D8=v_R_d4; //2013-4-11
                }
                else if(KESEI==10)
                {
                    v_Adj[ADJ_UP]+=cmdAdjT1;
                    T1_D8=v_LambdaTg; //v_dLambdaTg

                    RAM_O=(sword)v_dmLambda;
                }
                else if(KESEI==11)
                {
                    v_Adj[ADJ_UA]+=cmdAdjT1;
                    T1_D8=v_UA_M128;
                    //need to add ua*ld
                    RAM_O=vc_FMF_dtfi_trans+v_TPS_addt;
                }
                else if(KESEI==12)
                {
                    v_Adj[ADJ_UR]+=cmdAdjT1;
                    T1_D8=v_UR_M16;
                    RAM_O=(sword)v_dmLambda;
                }

              break;
              
              case MC_REMARK14:
                T1_D8=v_LambdaTg; //v_dLambdaTg
                TPS=v_LAMBDA_byMAPS; //2014-1-15 ADD 
                  
              case MC_MI:
                 
               /*  #if M0_MFUD1==INJ309
                        RAM_T3=v_mi+v_mi;
                 #else
                        RAM_T3=v_mi;
                 #endif */
                 
                 RAM_T3=v_mi;
				 
				 RAM_DTFI=(sword)v_ld+v_msMu;
                 TES=V_Tw;
                 DTC=v_u_M128_Cold/5;
                 RAM_O=(sword)v_dmLambda;
                 TPS=v_LAMBDA_byMAPS;//2014-1-15 ADD 
                 
                 
                 if(KESEI==4)
                 {
                    TPS=vc_CLS_1;
                    RAM_DTFI=vc_CLS_2;
                    TES=leansdtav;
                 }
                  
                 
              break;
              case MC_MF:
                RAM_T3=V_mf;
                
                
                VBAT=v_mi/4;//2012-11-12 add for cab

                RAM_DTFI=v_mf_evap;
                
                RAM_O=(v_mf_remain);

                    TES=V_Tw;
                
                if(KESEI==0){
                    if(v_mf_area_M256>255)DTC=255;
                    else DTC=v_mf_area_M256;
                    
                    VBAT=v_ld/4;
                    
                }
                else if(KESEI==1){
                  vc_B_mf+=cmdAdjT1;
                    v_Adj[ADJ_B]+=cmdAdjT1;
                  T1_D8=vc_B_mf; 
                  
                  VBAT=v_te;
                }
                else if(KESEI==2){
                  vc_C_mf+=cmdAdjT1;
                  T1_D8=vc_C_mf;
                    v_Adj[ADJ_C]+=cmdAdjT1;
                }
                else if(KESEI==3){
                  vc_D_mf+=cmdAdjT1;
                  T1_D8=vc_D_mf;
                    v_Adj[ADJ_D]+=cmdAdjT1;
                  
                  RAM_O=v_mi;
                  
                } 
                // for which  kesei>=5 is not planned
                else if(KESEI==5){   //2012-11-30 15:00PM ADD
                    RAM_DTFI=v_dmf_SumL;
                    RAM_O=(v_dmf_SumS);

                }else if(KESEI==7){
                    sw=v_dmfS+v_dmfL;
                    RAM_O=(sw);
                }
                
                
                TPS=v_LAMBDA_byMAPS; 

              	break;

              case MC_DMFC:
                  RAM_T3=v_dmfS+v_dmfL;
                  
                  if(KESEI>0)RAM_T3=v_mi;
                  
                  if(KESEI==1){
                    v_P_DEC_M64+=cmdAdjT1;
                    v_Adj[ADJ_P_DEC]+=cmdAdjT1;
                    T1_D8=v_P_DEC_M64;
                  }
                  else if(KESEI==2){
                    v_S_M128+=cmdAdjT1;
                    v_Adj[ADJ_S]+=cmdAdjT1;
                    T1_D8=v_S_M128;
                  }
                  else if(KESEI==3){
                    v_L_M1024+=cmdAdjT1;
                    v_Adj[ADJ_L_ACC]+=cmdAdjT1;
                    T1_D8=v_L_M1024;
                  }
                  else if(KESEI==4){
                    v_P_ACC_M64+=cmdAdjT1;
                    v_Adj[ADJ_P_ACC]+=cmdAdjT1;
                    T1_D8=v_P_ACC_M64;
                  } else if(KESEI==5){
                    v_L_DEC_M1024+=cmdAdjT1;
                    v_Adj[ADJ_L_DEC]+=cmdAdjT1;
                  
                    T1_D8=v_L_DEC_M1024;
                  }
                  
                  RAM_DTFI=v_dmf_SumL;

                  RAM_O=(v_dmf_SumS);
                  
                  VBAT=v_dmfL/4; //2012-11-19 ADD /4 FOR CAB
                  DTC=v_dmfS;
                  
                  TES=V_Tw;
                  
                  TPS=v_LAMBDA_byMAPS; 

                	break;

              break;
              case MC_IGNITION:
                  v_Adj[ADJ_Igb]+=cmdAdjIGN;
                  
                  //2013-7-24
                  if(KESEI==0){
                    
                      DTC=v_Adj[ADJ_Igb];
                  }
                  
                  if(KESEI==8){
                      VBAT=vc_Ign_Altitude;
                      DTC=v_Pair_deg;// 2013-6-6 DELETETMP
                  }
                break;

              case MC_SENSOR:
                  	if(KESEI==0){
                  	    TPS=v_TPS_AD10;
                  	    TES=v_TPS0_ub;//v_TPS_AD10; 
                  	    DTC=v_TPS0_AD10;
                  	    
                  	    if(VF_TPS0_MIN_GOTTEN1ST_TPS)VBAT=v_TPS0_min; 
                        
                        if(!VF_TPS_AFTER_GOTTEN1ST_TPS){
                            OS=v_bTPS_AD10_lst;    
                        }
                         
                  	}else if(KESEI==1){
                  	    TES= V_TPSH_STATUS;  //2013-4-8
                        VBAT=v_bTPS_AD10_lst;
                  	    DTC=v_TPS0_AD10;

                        RAM_DTFI=v_TPS0_256sum;
                        RAM_O=(v_TPS_16sum);
                  	}else if(KESEI==2){
                        DTC=v_O2_lambdaE1;
                  	
                  	    //2015-5-24 add for 
                  	        TPS=v_nb_OS_AD_CD;
                  	        TES=v_O2_ad10bit_lst/4;
                  	        VBAT=v_O2_ad10bit/4;
                  	        

                  	}else if(KESEI==3){
                        DTC=v_O2_lambdaE1;
                  	    VBAT=v_O2flu_max_av;
                  	    TES=v_O2flu_min_av;//2014-3-7 ADD 
                  	}else if(KESEI==4){ //2013-8-23 add 
                  	    VBAT=v_teng_inuse;
                  	    TES=v_te;
                  	    DTC=v_teng_ad;
                  	}else if(KESEI==7){ //2013-8-23 add
                  	    //@ remark 23,kesei 7 
                  	    VBAT=v_tair_inuse;
                  	    TES=V_Ta;
                  	    DTC=v_tair_ad;
                  	    
                  	    TPS=v_TPCB;//2016-1-10  @ remark 23,kesei 7
                  	}
                  	else if(KESEI==8){
                  	    VBAT=m_nbtdc0;//v_ccl_eng_byte;    //2015-3-3怠速的点火触发角度
                  	    TES=nb_qy_betweenhy ;

                  	    DTC=nb_hy_betweenqy  ;
                  	    TPS=nb_hy_lack;//2014-4-18ADD ,BUT NOT IN THE DATAVIEW PLAN
                  	        
                  	}else if(KESEI==9){
                  	    VBAT=v_qhCAint_ave;
                  	    TES=v_qhCAint_to_prd_m255 ;
                  	    TPS=t_qhymax_p    ;
                  	    DTC=t_qhymin_p  ;
                  	}else if(KESEI==10){
                  	
                  	    RAM_O=v_tl_ignition;  //2015-3-17
                  	    TPS=v_IgnCA_act_lst;
                  	    RAM_DTFI=deDTFI;
                  	    RAM_T3=deT3;
                  	    
                  	    TES=v_AD_cas;   //2016-2-28 remark=23,kesei=10
                  	}

              break;
              case MC_ACTUATOR:
              
                  /*   2016-9-30 delete 删除这个部分后，升级工具写入正常了，MC_ACTUATOR
                  if(KESEI==1){
                      RAM_DTFI=v_dt_bInj_toCas;
                      RAM_T3=v_dt_bIgn_toCas;
                      RAM_O=v_dt_T1;        
                  } else if(KESEI==2){
                      RAM_DTFI=v_nb_inj;
                      RAM_T3=v_dnb_Inj;
                      RAM_O=v_dnb_T1;
                      
                      TPS=v_nb_inj-v_nb_YSTDC;
                      OS=v_nb_injOff_r-v_nb_YSTDC;
                      TES=v_nb_injOff_r-v_nb_ccl_r;
                                                             
                  } else if(KESEI==3){
                      RAM_DTFI=v_dnb_Ign_YSTDC;
                      RAM_T3=v_dnb_Inj_YSTDC;
                      RAM_O=v_nb_YSTDC;        
                  }else if(KESEI==4){
                      RAM_DTFI=v_t_bInj_toTrig;
                      RAM_T3=v_t_bInj_toCas;
                      RAM_O=v_t_T1;        
                  }else if(KESEI==5){
                      RAM_DTFI=v_t_bIgn_toTrig;
                      RAM_T3=v_t_bIgn_toCas;
                      RAM_O=v_t_IgnOn;        
                  }else if(KESEI==6){
                      RAM_DTFI=v_t_bIgn_toTrig_r;
                      RAM_T3=v_t_bIgn_toCas_r;
                      RAM_O=v_t_IgnOn_r;        
                  }else if(KESEI==7){
                      RAM_DTFI=v_t_bInj_toTrig_r;
                      RAM_T3=v_t_bInj_toCas_r;
                      RAM_O=v_t_T1_r;        
                  } else if(KESEI==8){
                      
                      RAM_DTFI=v_tl_IgnPreCharge_r;
                      RAM_T3=v_tl_IgnPreCharge;
                      RAM_O=v_t_IgnOn_r;        
                  } else if (KESEI==9){
                      
                      RAM_DTFI=v_nb_ignOn_r;
                      RAM_T3=v_nb_ignOff_r;
                      RAM_O=v_nb_ccl_r;
                      TPS=v_nb_YSTDC-v_nb_ignOff_r;
                      OS=v_nb_YSTDC-v_nb_ignOn_r;
                              
                  } else if (KESEI==10){
                      
                      RAM_DTFI=v_nb_ignOn_r;
                      RAM_T3=v_nb_ignOff_r;
                      RAM_O=v_nb_IgnInt;
                      TPS=v_nb_ccl_r-v_nb_ignOff_r;
                      OS=v_nb_ccl_r-v_nb_ignOn_r;
                      TES=v_nb_IgnInt-v_nb_ignOn_r;
                              
                  }
                  */
                  
              break;
              case MC_SPEEDCONTROL:
                  
                  
                  VBAT= ign_lst8run_ave; //ign_lst8run_ave=0；ign_lst8run_sum=0；
                  DTC= dRev_forAirLess;               //当怠速比标准怠速低于该值时，认为气量低
                  TPS= ignMax_forAirLess;
                  /*
                  if(KESEI==0)DTC=v_rev15d6_target_idle;*/
				  
				  if(KESEI==1)
				  {
					  DTC=v_nb_on;
					  TPS=v_nb_off;
					  TES=nb_keyon_forIni;
				  }
              break;

              case MC_GENERATOR:
                RAM_DTFI=(sword)v_dld;
                
                //2014-4-13删除发电机的条件
                    //metc needed;
                    if(KESEI<=1){
                        TES=v_I_ldx[0];      //第一通道　电流传感器值
                        VBAT=v_I_max_lst[0];     //第一通道　电流传感器初始值

                        //v_nfull+=cmdAdjT1;   //目标转速/3.9/4  可以调节
                        
                        v_Adj[ADJ_nfull]+=cmdAdjT1;   //目标转速/3.9/4  可以调节

                        T1_D8=v_rev3d9_tg/4;
                    } else if(KESEI==2){
                        TES=v_I_ldx[1];//v_I2_ld;         //第二通道　电流传感器值
                        VBAT=v_I_max_lst[1];     //第二通道　电流传感器初始值
                        
                        v_Adj[ADJ_kp]+=cmdAdjT1;   //转速控制比例项可以调节
                        T1_D8=v_KP_M8;
                    } else if(KESEI==3){
                        TES=drev_tg_i;
                        
                        v_Adj[ADJ_ki]+=cmdAdjT1;   //转速控制积分项可以调节
                        T1_D8=v_KI_M8;
                    } else if(KESEI==4){

                        v_Adj[ADJ_kd]+=cmdAdjT1;   //转速控制微分项可以调节
                        T1_D8=v_KD_M8;
                    } else if(KESEI==5){
                        //2015-12-16 add 电机目标油门控制
                        //generator-->nstand4
                        VF_BYTPD_ETC=true;
                        
                        
                        v_Adj[ADJ_TPDtg]+=cmdAdjT1;//2015-12-16
                        
                        //v_nbStep_ETC+=cmdAdjT1*8;   //电机点动调节
                        //T1_D8=v_nbStep_ETC;
                        
                        //2016-9-11 IACV
 
                            T1_D8=v_TPS_tg;
							
							if(VF_MMOVE_ENG){
                          
                            RAM_DTFI=(sword)v_dmLambda;
                            RAM_T3=v_ldb;
                        }

                        #ifdef SYS_STEPMOTOR
                            T1_D8=v_step_IACV_on;

							DTC=v_step_sm;
							
							//VBAT=v_ntcoradj;
							//RAM_T3=v_dnave_sigma_i;
						#endif


                         
                        
                    } else if(KESEI==6){
                        TES=v_I_stable_lst;         //两路电流转化为代表负荷后的值；
                        
                    VBAT=v_TPS_tg;
                    DTC=v_TPS_I_lst;//V_ETC_STATUS;//VF_STA_LD;
					
					#ifdef SYS_STEPMOTOR
						v_Adj[ADJ_KI_IACV]+=cmdAdjT1;
						v_Adj[ADJ_KP_IACV]+=cmdAdjIGN;
						
						T1_D8=v_ki_iacv;
						IGN=v_kp_iacv;
					#endif

						
                        
                    } else if(KESEI==7){
                        DTC=V_ETC_STATUS;
                        TES=VF_STA_LD;//
                    } else if(KESEI==8){
                        TES=v_nb_DCPrd;
                        VBAT=v_nb_DC;
                        DTC=v_nbStep_DC;
                        //DTC=
                    }

              break;
              case MC_DTC:
                  
                      /*2019-3-6 dataview上故障仅显示3位有效数字
					  RAM_O=V_DTC_STATUS+(((word)V_DTCH_STATUS<<8)&0xff00);
                      RAM_DTFI=V_DTCHL_STATUS+(((word)V_DTCHH_STATUS<<8)&0xff00);
					  */
                      RAM_O=V_DTCH_STATUS;
                      RAM_DTFI=V_DTCHL_STATUS;
					  TES=V_DTCHH_STATUS;
                  

              break;
              
              case MC_REMARK5:
                  TPS=v_pair_co;//标定后的参数
              break;
              
              case MC_PRONO:
                  //2013-5-3
                  if((VFF_NOZZLEINI_DOMFAILURE_DTC||VFF_NOZZLEINI_VBATNOTDOM_DTC ||VF_NOTCAB_PAIR))TES=0;
                  else TES=1;
                  
                     if(KESEI==6&&PCTBACK==6)
                    {
                        if(PCBTDC==5){
                            Clr_DTC_History();
                            Init_EE_SL();
                          //  ClrIniResult();
                          //  ClrPermit();
                          //  Clr_TMCCL();
                          //  Clr_ExPair();
                            
                        } 
                        v_PCTBACK_SCI=0;
                        v_PCBTDC_SCI=0;
                        v_PCKESE_SCI=0;
                    }
                
              
              break;
              case MC_INI:
                /*
                if(!VF_1ST_INI_SYSINI){
                    //未在初始化状态时，进入初始化，只有关闭钥匙才能退出这个状态；
                    if(KESEI==16&&PCTBACK==16)
                    {
                        if(PCBTDC==8){
                            VF_1ST_INI_SYSINI=true; //进入初始化状态

                        } 
                        v_PCTBACK_SCI=0;
                        v_PCBTDC_SCI=0;
                        v_PCKESE_SCI=0;
                    }
                }
                
                if(VF_1ST_INI_SYSINI){
                    if(KESEI==15&&PCTBACK==16)
                    {
                        if(PCBTDC==7){
                            //进入T4扫描模式
                            VF_RUNENOUGH_FORINI_SYSINI=true;
                            VF_NOTNEED_DROPSCAN_SYSINI=false;
                        } else if(PCBTDC==6&&VF_E0_TPS){
                            //获取怠速调试结果
                            vc_tw3offset=FS_AQOK_FLAG(vc_tw1_IDLESC,v_dmLambda,vc_tw3offset,v_FMF_SCI_idle,true);
                            v_FMF_SCI_idle=0;                    
                            vc_tw1_IDLESC=0;
                        }
                        
                        v_PCTBACK_SCI=0;
                        v_PCBTDC_SCI=0;
                        v_PCKESE_SCI=0;
                    } 
                }
                 */
				 
				 if(KESEI==0)
				 {
				 	TES=VF_PUMP1ST_INJCTL;
				 }

                //2015-6-30
                /*
                if(KESEI==12)
                {
                  
                    VF_1ST_INI_SYSINI=true; //进入初始化状态
                    VF_RUNENOUGH_FORINI_SYSINI=true;
                    VF_NOTNEED_DROPSCAN_SYSINI=false;
                    v_PCKESE_SCI=0;
                    
                    //需要增加
                    VF_SEARCHTMP_SYSINI =false;
                    //否则只扫一次
                }
                */

                
                
                //进入泵油状态
                    if(KESEI==10&&PCTBACK==10){  //T1向上调节10下
                        v_PCKESE_SCI=0;
                        if(PCBTDC==5){//点火调节向上5下
                            //进入泵油模式
                            VF_PREPUMPEND_SYSINI=false;
                            v_PCKESE_SCI=3;            
                        }
                        v_PCTBACK_SCI=0;
                        v_PCBTDC_SCI=0;
                    }

                    if(KESEI==3){
                        OS=nb_pump;//泵油的次数
                        TPS=nb_pump_constdt3;//DT3接近目标值的次数
                        DTC=v_dt1;//对T1的修正

                        RAM_DTFI=v_dtw3;

                        RAM_O=(v_dtw3_obj);
                        
                            
                    } else if(KESEI==10){

                        v_fmf_pump+=cmdAdjT1;
                        T1_D8=v_fmf_pump;    //目标油量，单位11/250 UL,最小可以调整到0
                    }
                
                
                if(!VF_MMOVE_ENG){
                  
                    if(VF_INJENDWHENSTOP_STOP)T1_D8=v_PCTBACK_SCI_TOCOM;
                    IGN=PCBTDC;
                }
                
                TES=V_SYSINIHH_STATUS;  //初始化的状态
                	
              break;
              
              case MC_LOCK:
                  //密码模式
                  DTC=V_LOCK_STATUS;  //密码状态
                  
              
              break;
              
           }
        
        }

        if(VF_IDLEAIRLESS_IGN&&(VF_AQAFTER_PNINI_SYSINI||REMARK==MC_DTC)&&VF_MMOVE_ENG)
        {
            DTC=255;
        }
         
    /*以上----临时显示参数初始化用--------------------------*/   
        
        Get_Kesei_for_CAB(0,0);
        
        /*EEPROM caozuo xiangguan*/
            /*remark*/
            if(KESEI==19&&!VF_MMOVE_ENG)         //2008-4-16 ADD //2008-10-15 add condiction VF_MMOVE_ENG
            {
                 TX_REMARK=REMARKH;//v_PCSET_SCIH;    //2009-3-3
            }
            else
            {
                 TX_REMARK=(byte)REMARK;
            }

            /*DTC或者数据或者T3反馈值显示*/
            //--display data sometimes 
            //if(!VF_MMOVE_ENG&&(KESEI>=20)&&KESEI<23)DTC=PCCQJSJD;//dat_EE_RD;//2008-4-26 更改，需要验证
            //else if(!VF_MMOVE_ENG&&(KESEI>=23))DTC=PCCQJSJD;//dat_EE_WR;
            
            if(KESEI>=20){
                DTC=PCCQJSJD;//dat_EE_RD;//2008-4-26 更改，需要验证
                //REMARKH=0; 
                RAM_O=REMARKH;  //2013-5-4 jiuzheng
            }

        
        if(VF_PUMP_INJCTL&&!VF_1ST_INI_SYSINI)
        {
  
            RAM_DTFI=v_dtw3;
            
            TPS=v_dtw3_obj;    //目标DT3
            
            RAM_O=v_TW1_forTrig;
            
            OS=nb_pump_constdt3;  //趋于目标时的次数
            
            //RAM_T3=deO;
            
        }
        
        //2015-12-17 增加初始化时，参数的显示
        
        if(VF_1ST_INI_SYSINI){
            
            if(VF_INJENDWHENSTOP_STOP&&!VF_MMOVE_ENG&&!VFF_NOZZLEINI_T1DGOT_DTC) 
            {
                TES=v_DTC_T4ScanFail;
                RAM_DTFI=v_gama_new;
                
            }
            if(VFF_NOZZLEINI_T1DGOT_DTC&&!VF_MMOVE_ENG) 
            {
                RAM_DTFI=v_gama_new;
            }
        }
        /*
            OS=VF_STA_LD;
            TES=v_TPS_tg;
            
            RAM_DTFI=v_d_ETC_all;
        if(KESEI==1){
          
        //TPS=deTPS;
            RAM_O=v_d_ETC_p;    
            RAM_DTFI=v_d_ETC_i;    
            RAM_T3=v_d_ETC_d; 
            
            VBAT=deTPS;
            OS=v_d_ign_fb;
            DTC=v_d_AVrpm_to_tg;
            //RAM_DTFI=v_d_AVrpm_to_tg;   
            
        } */
        
        
        /*
        if(REMARK==4&&KESEI<5)
        {
            RAM_DTFI=v_d_ETC_i;
            RAM_O=v_d_AVrpm_to_tg;
            TES=v_TPS_tg;
            
            VBAT=v_I_ldx[0];
            DTC=v_I_ldx[1];
            
            if(KESEI==1)
            {
                VBAT=v_I_max_lst[0];
                DTC=v_I_max_lst[1];
                TES=v_I_min_lst[0];
                TPS=v_I_min_lst[1];
                
            }
        } */
        
       // RAM_DTFI=v_T3b;


       
       #ifdef IS_GEN
           if(REMARK==30)
           {
              TPS=v_nb_IncLd;
              VBAT=v_nb_DecLd; 
              
              if(KESEI==1)
              {
                  TPS=v_dI_ld[0];
                  VBAT=v_dI_ld[1];
                  TES=v_nb_ldAd_1cycle;
              } 
              else if(KESEI==2){
                  RAM_DTFI=v_d_ETC_p;
              }
              else if(KESEI==3){
                  RAM_DTFI=v_d_ETC_i;
              }
              else if(KESEI==4){
                  RAM_DTFI=v_d_ETC_d;
              }
           }
           
       #endif
       
    /*
       if(REMARK==4&&VF_MMOVE_ENG)
       {
          if(KESEI==1) { //发动机运行,KESEI=1时，
              //2016-5-26 fai
              RAM_DTFI=v_dmi;
              RAM_T3=deDTFI;  //与油膜减小的系数相关的
          }
          else if(KESEI==2)RAM_DTFI=v_dld;
       }
    */   

        if(
            VF_NOTACTIVE_LOCK&&REMARK==4&&KESEI==0
            )
        { //2014-5-29 ADD 
            //261 RPM
            PRD_H=0xE0;
            PRD_L=0x00;
        }
        /*
        TPS=deTPS;
        OS=deOS;
        DTC=deDTC;
        RAM_T3=v_t_IgnSetOn_r;
        RAM_DTFI=v_t_injOff_r;
        */
      /*  
        if(REMARK==4){
            TES=blnIdleIni;
        }
      */
        
        T3_H=(byte)((RAM_T3>>8)&0xff);  //当次的DT3
        T3_L=(byte)(RAM_T3&0xff);

        DTFI_H=(byte)((RAM_DTFI>>8)&0xff);  //当次的DT3
        DTFI_L=(byte)(RAM_DTFI&0xff);

        O_H=(byte)(((RAM_O)>>8)&0xff);
        O_L=(byte)((RAM_O)&0xff);
        
       
        Get_Kesei_for_CAB(0,0);


        if(VF_MODE_INTSCI==COMMODE_LOCK){
            REMARKW=MC_LOCK;
            
            //OS=OS_LOCK;
            //TES=TES_LOCK;
            //DTFI_H=(byte)((u_code>>8)&0xff);  //当次的DT3
            //DTFI_L=(byte)(u_code&0xff);
            IGN=u_code;
            
            TPS=v_sta_cmd;
            //T3_H=    v_nb_f0;
            //T3_L=v_nb_f1;
            //IGN=v_num_rx_tmp;
            
            T3_L=v_pw[0];
            T3_H=v_pw[1];

            O_L=v_pw[2];
            O_H=v_pw[3];

            DTFI_L=v_pw[4];   
            DTFI_H=v_pw[5];   

        }
        
        
        //if(VF_MMOVE_ENG)
        //2014-5-9
        //if(KESEI==-1)
        /*
        TPS=v_nb_rst;
        TES=v_nb_JMMOVE;
        VBAT=v_nb_JSTOP;
        if(KESEI==1)
        */
        /*
        {
            TPS=nb_clk_er;
            TES=nb_PLL_er;
            VBAT=v_nb_rst;//CPMUFLG;
        }
        */
        //    TPS=m_nbtdc0;
            //VBAT=deVBAT;//BYPTR_M0_IGNMAX_IDLE;//nb_qy_valid;//v_rev62d5_hyUL;
        //    TES=nb_hy_lack;
        
        //
        //VBAT=deTPS;
        /*
        if(KESEI==0&&REMARK==4){
            TPS=deTPS;//nb_hyint_all;//m_nbtdc0;
            VBAT=deVBAT;//BYPTR_M0_IGNMAX_IDLE;//nb_qy_valid;//v_rev62d5_hyUL;
            TES=v_CA_limit_byrpm;//nb_hy_lack;
            DTC=v_Ign_Map;
            OS=co_IdleIgn_byRPM;//
        }
        */
        
       /*        
        if(REMARK==4){
            if(KESEI==0){
              PWR_ON;
            }
            else{
              PWR_OFF;
            }
            
            TES=v_vbat_atInjOff;
            DTC=deDTC;
            TPS=deTPS;
            OS=VF_MAY_UPDATE_FLASH;
        }

        DTC=v_nb_cmd1;
        TES=v_nb_cmd2;
        OS=BOOT_AssureArr[2];
        TPS=deTPS;
       */ 
       
       /*
       if(REMARK==4)
          TES=v_nb_T30;
       */
        
       // DTC=VF_MAY_UPDATE_FLASH;
        
        //DTC=V_SYSINIHL_STATUS;
        
       // VBAT=10;

        //OS=nb_ign_open_er;
        //OS=nb_keyon_forIni;
        
       // VBAT=IS_IGNI_DRVHIGH;//2016-3-9
        
        /*---------clear cmdAdjT1 command-----------------------*/   

        //TES=v_co_dmf_M64;
        
        
        cmdAdjT1=0;
        cmdAdjIGN=0;
        cmdAdjCOS=0;
        
        //-----------------------------------------------

        TX_DATA[0]=70;   //F
        TX_DATA[1]=65;   //A
        TX_DATA[2]=73;   //I
        
        TX_DATA[12]=1;
        TX_DATA[22]=1;
        for(b=3;b<=11;b++){
            TX_DATA[12]+=TX_DATA[b];        
            TX_DATA[22]+=TX_DATA[10+b];        
        }
        
        //t_lstCom_4us
        
        /*
        TX_DATA[23]=(byte)(((t_lstCom_4us)>>8)&0xff);
        TX_DATA[24]=(byte)((t_lstCom_4us)&0xff);
        */
        
        //2016-6-6
        
        TX_DATA[23]=M0_PC_HH;
        TX_DATA[24]=M0_PC_HL;

        TX_DATA[25]=V_LOCK_STATUS;//2013-9-7 add 
        TX_DATA[26]=V_LOCK2_STATUS;//2013-9-7 add 预留
        TX_DATA[27]=TX_DATA[23]+TX_DATA[24]+TX_DATA[25]+TX_DATA[26]+1;//2013-9-7 add 预留
        //TES=V_SYSINIHH_STATUS;


        
         if(VF_LINKPC_INTSCI){
            v_NB_TX=3;         //当前已经发送的字节
            v_NB_TX_max=23;
            //(void)TR_SCI0_SendBlock(TX_DATA+3, 20, &b); 
            (void)TR_SCI0_SendBlock(TX_DATA+3, 20, &w); //2014-11-20 由于TR_SCI0_SendBlock的第三个形参是word类型的地址传递，用BYTE类型的地址，会导致b地址后一位的数被改变

        }
        else{
            v_NB_TX=0;
            //v_NB_TX_max=25;
            v_NB_TX_max=28;//2013-9-7 from 25 to 28
            //(void)TR_SCI0_SendBlock(TX_DATA, 28, &b); 
            (void)TR_SCI0_SendBlock(TX_DATA, 28, &w); //2014-11-20 由于TR_SCI0_SendBlock的第三个形参是word类型的地址传递，用BYTE类型的地址，会导致b地址后一位的数被改变

        }
        
        VF_TRANSING_INTSCI=true;


        //TR_SCI0_SendChar();
    }
    
    return;
}

/*
    使用增量
*/

byte Adj_byte(byte ori,byte index) 
{
    sword sw;
    
    sw=(sword)ori+v_Adj[index];
    
    if(sw<0)sw=0;
    if(sw>255)sw=255;  
    
    return (byte)sw;  
}

sbyte Adj_sbyte(sbyte ori,byte index) 
{
    sword sw;
    
    sw=(sword)ori+v_Adj[index];
    
    if(sw<-126)sw=-126;
    if(sw>126)sw=126;
    
    return (sbyte)sw;    
}

word Adj_word(word ori,byte index) 
{
    long int lint;
    
    lint=(long int)ori+v_Adj[index];
    
    if(lint<0)lint=0;
    if(lint>65535)lint=65535;
    
    return (word)lint;    
}

sword Adj_sword(sword ori,byte index) 
{
    long int lint;
    
    lint=(long int)ori+v_Adj[index];
    
    if(lint<-32760)lint=-32760;
    if(lint>32760)lint=32760;
    
    return (sword)lint;    
}




/*
    调整参数
    
    step:
        0--reset
        1--acc
        -1--dec
*/
void Adj_para(byte idx,sbyte step)
{
    byte b;
    
    if(step==0){
        v_Adj[idx]=0; 
    }
    else if(step==1)
    {
        if(v_Adj[idx]<120)v_Adj[idx]++;
    } 
    else if(step==-1) 
    {
        if(v_Adj[idx]>-120)v_Adj[idx]--;
    }
}
