/*HEAD
 ***********************************************************************
 * Filename	:pair.c
 * Function	:CAN comunication		
 * User		:xielian
 * Date		:2017.3.28
 * Version	:V8.2.0
 *
 *** History ***
 * V8.0.0   2017.1.17	zhangping 
 *		Base module.
 * V8.1.0   2017.1.17	zhangping 
 *		VF_WITHPAIRS_PRO2ͨ��BYPTR_M0_EN_PAIRS��������.
 * 
 * V8.2.0   2017.1.17	xielian
 *		ɾ��V_PRO2_F5_STATUS��V_PRO1_F6_STATUS������汾��ֵ
 *
 * V8.2.1   2017.3.28	xielian
 *		bugfix,����VF_WITHPAIRS_PRO2��ֵ��V_PRO2_F5_STATU,V_PRO1_F6_STATUS��ʼ��
 ***********************************************************************
HEAD END*/

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Public.h"
#include "Status_st.h"
#include "pair.H"

#include "ee.h"
#include "add.h"

#include "dtc.h"

#include "time_ccl.h"

#include "sys_init.h"  //2014-11-27 ���룬֮ǰ����

//#include "wait_moving.h"
//#include "sci.h"

//������ʹ�õ�ѹ������������
#define PAIRS_KP215 0
#define PAIRS_CAB   1

#define M0_PAIRS    PAIRS_KP215


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_PAIR_STATUS;
//#pragma DATA_SEG DEFAULT
                           ///////////////////////////////////////////////////////////////////
byte v_pair_ad;            //pair ��AD   //�����жϵ���Ϊ�޹��ϵ�PAIRS�ź�//2008-11-5 //    //
byte v_pair_odr;           //���ݺ�һ�ܲ��Խ�������US1119PAIRS��                    //    //
                           //����ÿ����1000m������ֵ����15.2������ȡ16?               //    //
byte v_pair_co;            //������Ļ���ѹ��ֵ��ʹ�ñ���ֵ                           //    //
byte v_pair_mode;          //                                                         //    //
byte v_pair_ee;            //���һ�ζ�ȡ�Ĵ洢��EE�е�PAIRS��                        //    //
                           //                                                         //    //
//2009-12-18 add           //                                                         //    //
byte v_pair_max_afterkeyon;//                                                         //    //
byte v_pair_min_afterkeyon;//                                                         //    //
byte v_pair_aveinlast10min;//                                                         //    //
byte nb_forpair;           /////////////////////////////////////////////////////////////    //

byte v_Pair_ad_tmp;//PAIRS ֱ��AD����,��������ʱ ���� v_pair_ad_input

/*-----------The following are local ...----------------------------------------------------*/
#define M0_PAIR_CAB_L   170//0 ��һ���궨�㣬���ζ�Ӧ�Ĵ���ѹ����������׼ֵ���ݶ�0��ƽ��?
#define M0_PAIR_CAB_U   100//�ڶ����궨���Ӧ��pair�ı�׼ֵ?
//#define MAX_PAIR        190 //PAIR�źŵ����ֵ��
//#define MAX_PAIR        200 //PAIR�źŵ����ֵ��
#define MAX_PAIR        240 //PAIR�źŵ����ֵ�� //2010-5-10 change    //2010-8-23 FROM 245 TO 240
#define MIN_PAIR        70  //PAIR�źŵ���Сֵ��

//2013-4-
#define ORI_MAX_PAIR    240//70//PAIRSԭʼ����ֵ
#define ORI_MIN_PAIR    70//240//

#define COR_MAX_PAIR    200//75//PAIRS���������ֵ     ע����ʹ��ʱ�ĵ��ڣ�����ʱ�ͱ�ʾ�й��ϵģ�
#define COR_MIN_PAIR    75//200//

#define MAP_FOR_H7000   112//����7000��Ӧ��pair�Ĳ�ֵ
#define MAP_FOR_H6000   96//42//96//����6000��Ӧ��pair�Ĳ�ֵ

#define MAX_VBAT        250
#define MIN_VBAT        0

//2010-7-7 chang from 100 to 120 for mpxm2102
#define DELTA_K_PAIRS   120
#define DELTA_B_PAIRS   120

//#define DELTA_PAIR_INSHORTTIME  5//�ڶ�����   //2009-change from 4 to 6
                        //2010-8-20 from 3 to 4
//#define DELTA_PAIR_INSHORTTIME  10//2011-9-8 changge from 5 to 10
#define DELTA_PAIR_INSHORTTIME  16//�����ڵĲ�������

#define DELTA_PAIR_FORAVE       10//ǰ������ѹ��ƽ���Ĳ���
                        
#define DELTA_PAIR_AFTERKEYON   48
#define VF_PERIOD_PAIRS         ((VF_MMOVE_ENG&&VF_16S_TMCCL)||((!VF_MMOVE_ENG)&&VF_4S_TMCCL))//2008-11-6          

#define NB_PAIRER_LIMIT 5//2010-10-10 CHANGE FROM 2 TO 3 from 3 to5

/*
#define NB_FORPAIR_LIMIT    256
#define NB_FORPAIR_SAMPULIMIT    240    //2010-7-19 ADD
#define NB_FORPAIR_LLIMIT    5
*/

//2010-10-10 change to 
#define NB_FORPAIR_LIMIT    10//2020-6-19 for CR 64
#define NB_FORPAIR_SAMPULIMIT    56    //2010-7-19 ADD
#define NB_FORPAIR_LLIMIT    6
//2010-10-10,ͣ��ʱ��0.4ms�ɼ�һ�Σ����һ�����ڴ�Լ2.5s;
//2015-6-29 ,20ms�ɼ�һ�Σ������Ҫ���ٴ��������ڶ�ʱ�����ܲɼ�������



#define EE_DATA_WITHPAIRS       0xaa
#define EE_DATA_WITHOUTPAIRS    0x55
#define EE_DATA_PAIRS_BYMANNUAL 0x44

/*ʹ��KP215��K/P����*/			
#define	KKP215_M256		195

#ifdef	SY_HUANSONG		//20210924 chenwei add ����ѡ�� for test
#define	DKKP215_M256		-107
#define	DBKP215		26

#else	
#define	DKKP215_M256		-61
#define	DBKP215		10

#endif

/*------------------------------------------------------------------------------------*/
sbyte   kdPairs;        //pairs �궨��б�ʣ�
sbyte   bdPairs;        //pairs �궨�Ľؾ���죻


byte v_pair_ee;                 //������EE�е�PAIRS
byte t_pair_er;             //pair ��������������Ĵ���
//��REMARK!=4,v_pair_adֱ�ӵ���v_pair_ad_input���Ҳ�������ϣ�Ҳ�����棻

byte v_pair_ad_input;   //PAIRS A/D�ɼ���ֱ������ֵ����Ҫ�������֮��ſ��ã�
byte v_pair_ad_input_min;//��VF_PERIOD_PAIRS�ڼ��ڲɼ�����PAIRS����Сֵ
byte v_pair_ad_input_max;//��VF_PERIOD_PAIRS�ڼ��ڲɼ�����PAIRS�����ֵ
byte v_vbat_maxwhenpair;//       v_vbat_maxwhenpair= MAX_VBAT
byte v_vbat_minwhenpair;

byte v_pair_ad_64t;//64������ƽ��,   v_pair_ad=((word)v_pair_ad*4-v_pair_ad_64t)/4

byte v_pair_ad_64t_lst;//203-4-24
//2010-1-4 ADD
sbyte v_KPT; //pairs���¶�����ϵ����   v_KPT=(Xs1-Xs2)/(Ta1-Ta2)
byte v_Tmp_KPT;//   k,b�궨ʱ����Ӧ�ı�׼�¶ȣ�
sbyte v_KPT_L; //pairs�ĵ����¶�����ϵ����   v_KPT=(Xs1-Xs2)/(Ta1-Ta2)

byte v_Pair_deg;

sword v_p_kpa;

#define TMP_KPT_ST  75//EQU 25 SHESHIDU
//byte Load_PAIRS_DK(byte add);
void Check_PairinEE_Valid(void);//2008-11-5 add 
byte Get_Pairs_Odr_and_Altitude(byte pairs,byte blnEE);     //2009-12-30 add para ,�����ee�е����ݣ����ñ���
void Rec_Pairs_ad(void);
void Rec_Pairs_co(void);
/*------------------------------------------------------------------------------------------*/
void FI_P_air_AfterReset(void)
{
    if(VF_VPOR_HARD)
    {
               
        V_PAIR_STATUS=false;//2008-12-15

        V_MAPS_STATUS=false;     //2013-9-9 add condiciton   
        //VF_HARDCFG_ERR_PAIRS//VF_SOFTCFG_ERR_PAIRS
		
		
        // 
        Check_PairinEE_Valid();         //2008-11-5 add  ��Ҫ������
        t_pair_er=0;//2009-11-24 add 
        v_vbat_maxwhenpair= MIN_VBAT;
        v_vbat_minwhenpair= MAX_VBAT;
        v_pair_min_afterkeyon=v_pair_ad_input_min=ORI_MAX_PAIR;
        v_pair_max_afterkeyon=v_pair_ad_input_max=ORI_MIN_PAIR;

        nb_forpair=0;
        v_t_pairs_1s=0; //������������ʱ���ۼ�,��Ҳ������

		//2017-2-27 ȡ��ͨ������汾����ѡ���ܣ�����Ϊͨ��ʹ�ܱ�־λ����
		//V_PRO2_F5_STATUS=M0_PRO_2;
        //V_PRO1_F6_STATUS=M0_PRO_3;
		
		V_PRO2_F5_STATUS=104;//2017-3-30 ���ݳ������ý���Ĭ�ϳ�ʼ��
		V_PRO1_F6_STATUS=128;//2017-3-30 ���ݳ������ý���Ĭ�ϳ�ʼ��
        
        
        if(BYPTR_M0_EN_PAIRS==EN_PAIRS_NOTEN)
		{
			VF_WITHPAIRS_PRO2=false;//2017-1-21
        }
		else
		{
			VF_WITHPAIRS_PRO2=true;	///2017-3-28 VF_WITHPAIRS_PRO2û�ж��壬ʵ�ʷ�����Ϊ���ֵ
		}

    }
    
}

//2008-11-5
//���ӶԺ��μ���ı��湦��
void FM_P_air_WhenKeyOff(void)
{
    //����PAIRS�ı�Ҫ������
    //  1 ��PAIRSû�й��ϣ�
    //  2 ��PAIRS��REMARK==4ʱ�������Ч������
   // if((!VF_ER_PAIR)&&(VF_1PAIRSGOT_PAIR)&&(VF_WITHPAIRS_PRO2)&&VF_ECUHARD_WITHPAIR_PAIR&&(VF_PAIR_VALID_FOREE_PAIR))
    if((!VF_ER_PAIR)&&(VF_1PAIRSGOT_PAIR)&&(VF_WITHPAIRS_PRO2)&&VF_ECUHARD_WITHPAIR_PAIR)//2016-6-5
    {   //2013-4-23 add VF_ECUHARD_WITHPAIR_PAIR
        if(v_pair_co<COR_MAX_PAIR&&v_pair_co>COR_MIN_PAIR)//2009-12-15 add condiction 
        {
            Save_Data_EE(EE_PAIRS,v_pair_co); //���浽EE��
            Save_Data_EE(EE_PAIRS_B,v_pair_co+1); //���浽EE�� ,ע��һ���Ǿ��������ֵ
            //2010-1-4 data(xb)=data(x)+1;   
        }
    }
    
    
}

/*
//���ܣ�
//  ����ѹ������ֵ���˲������
    2014-10-8 ��REMARK��Ϊ�β�
*/
void FG_P_air(byte remark)
{
    sbyte sb;
    sword sw;
    
    //���
    
    v_pair_ad_input=v_Pair_ad_tmp;

    //deOS++;
    //��ȡ����ֵ��

    if(nb_forpair<=NB_FORPAIR_LIMIT)nb_forpair++;

    if(VF_MORE1MIN_TMCCL&&t_pair_er==0)VF_PAIR_VALID_FOREE_PAIR=true;
    
        if(nb_forpair>=NB_FORPAIR_LIMIT)
        {

            //deTPS++;
            /*
            if(v_vbat_maxwhenpair-v_vbat_minwhenpair<=2&&v_vbat_maxwhenpair>=v_vbat_minwhenpair
                &&v_vbat_minwhenpair>=180//2010 add for mpxm2102 //2010-8-20 chang from 3 to 2
            )
              */
            
            //2016-6-5 ���ɵ�ѹ����
             /*
            if( 
                v_vbat_maxwhenpair-v_vbat_minwhenpair<=20
                &&v_vbat_maxwhenpair>=v_vbat_minwhenpair
                &&v_vbat_minwhenpair>=150//2010 add for mpxm2102 //2010-8-20 chang from 3 to 2
            ) 
               */
               
            
            if(v_vbat_minwhenpair>=150)//2016-6-6
            {   //2009-12-18 add ֻ�е���ѹ��������ʱ�Ž����ж�
              //  deVBAT++;
                //���һ��ʱ���������Сֵ�Ĳ���
                //if(v_pair_ad_input_max>v_pair_ad_input_min)sb=v_pair_ad_input_max-v_pair_ad_input_min;
                //else sb=v_pair_ad_input_min-v_pair_ad_input_max;
                //2010-7-9 chang to the following
                if(v_pair_ad_input_max>v_pair_ad_input_min)sb=v_pair_ad_input_max-v_pair_ad_input_min;
                else sb=0;
                
                v_pair_ad_64t_lst=v_pair_ad_64t;
                v_pair_ad_64t=(byte)(((word)v_pair_ad_input_min+v_pair_ad_input_max)/2);

                
                if( 
                      sb>DELTA_PAIR_INSHORTTIME
                      ||(v_pair_ad_64t<ORI_MIN_PAIR)
                      ||(v_pair_ad_64t>ORI_MAX_PAIR))//VF_ER_PAIR=true;
                {
                    //�������ݲ�������,���߳�����ֵ��Χ
                    t_pair_er++;    
                } 
                
                //2016-2-28
                    if(v_pair_ad_input<ORI_MIN_PAIR)Accu_AD_Mv(MV_PAIR,MV_L_LIMIT);
                    else if(v_pair_ad_input>ORI_MAX_PAIR)Accu_AD_Mv(MV_PAIR,MV_U_LIMIT); //2016-2-29 ��ʱ	//20211005 chenwei fangkai ��//
                    else Accu_AD_Mv(MV_PAIR,MV_NORMAL);

                
                if(VF_1PAIRSGOT_PAIR){
                    //���֮ǰ�Ѿ��ɼ���һ���� ,��Ƚ�ǰ�����βɼ��Ĳ���
                    sw=(sword)v_pair_ad_64t_lst-v_pair_ad_64t;
                    if(sw<0)sw=-sw;
                    if(sw>DELTA_PAIR_FORAVE)t_pair_er++;
                }
                
                if(v_pair_min_afterkeyon>v_pair_ad_64t)v_pair_min_afterkeyon=v_pair_ad_64t;
                if(v_pair_max_afterkeyon<v_pair_ad_64t)v_pair_max_afterkeyon=v_pair_ad_64t;
                if(v_pair_max_afterkeyon-v_pair_min_afterkeyon>DELTA_PAIR_AFTERKEYON)
                {
                    t_pair_er++;   
                }
                
                if(VF_1PAIRSGOT_PAIR)  //������Ѿ����һ�β�����
                {
                    if(v_t_pairs_1s==0){
                        v_pair_aveinlast10min=v_pair_ad_64t;
                        v_t_pairs_1s=1;
                    }
                                        
                    if(v_t_pairs_1s>=5)
                    {
                        if(v_t_pairs_1s<RUNS_FORPAIRS_LIMIT_U){ //ע��<
                        
                            sw=((sword)v_pair_ad_64t-v_pair_aveinlast10min);
                            if(sw>20||sw<-20)
                            {
                                t_pair_er++;        
                            }
                        }
                        if(v_t_pairs_1s>RUNS_FORPAIRS_LIMIT_L){
                          
                            v_t_pairs_1s=1;
                            v_pair_aveinlast10min=v_pair_ad_64t;
                        }
                    }
                }

                if(t_pair_er>=NB_PAIRER_LIMIT)
                {
                    VF_ER_PAIR=true;    
                }
                
                if(!VF_ER_PAIR){//������ t_pair_er ����0�����
                    if(!VF_1PAIRSGOT_PAIR&&t_pair_er==0){   //2013-4-26
                        v_pair_ad=v_pair_ad_64t;
                        
                    } else{
                      
                        sw=(sword)v_pair_ad*7+v_pair_ad_64t;
                        if(sw<0)sw=0;
                        v_pair_ad=sw/8;
                        
                        if(v_pair_ad>v_pair_ad_64t)v_pair_ad--;
                        if(v_pair_ad<v_pair_ad_64t)v_pair_ad++;
                    }
                    VF_1PAIRSGOT_PAIR=true;
                }
                
                
                    //VF_1PAIRSGOT_PAIR=true;  //2008-11-8 ��©������  //2009-12-18 ��һ������
                
                
            } 
            else
            {
                deOS++;
            }
            //deVBAT= v_vbat_maxwhenpair;
            //deDTC=v_vbat_maxwhenpair-v_vbat_minwhenpair;//2016-6-5 �Ž�˵���ֵ��12-13֮��仯��
            //2009-8-16���׺����ں����ֺ��μ��𲻶Ե����⣺
            //  ��ʱ��Կ�׺��μ�����ʾ5����
            //  ��ƽ��֤���֣���ʱ��Կ��EE�д洢��V_PAIR_COֵΪ70��
            //  ��������������ԭ��
            //  ��Ϊ��  if(!VF_ER_PAIRS&&!VF_1ST_AD_PAIRS)ʱ�Ÿ�ֵ v_pair_ad��
            //  ����һ�ε���ʱ��VF_1ST_AD_PAIRS��true;
            //  ��ʱv_pair_ad��ֵ�Ͳ��ḳֵ��
            //      ��ʹ�õľ�����һ�δ洢��EE�б���ȡ���������֮������ݡ�
            //��Կ��4-8��֮�䣬�ٹ�Կ�ף���һ�ο�Կ�׵�0-4S��������������16S���ڣ����ܳ���ѹ��ֵ�����⡣

            v_pair_ad_input_min=ORI_MAX_PAIR;
            v_pair_ad_input_max=ORI_MIN_PAIR;
            v_vbat_maxwhenpair= MIN_VBAT;
            v_vbat_minwhenpair= MAX_VBAT;
            nb_forpair=0;
            if(VF_1PAIRSGOT_PAIR)Rec_Pairs_ad(); //2013-8-13 add condiction ���ָʾѹ�����������ϵ�����
        } 
        else
        {
            if(v_vbat_maxwhenpair<v_Vbat_inuse)v_vbat_maxwhenpair=v_Vbat_inuse;
            if(v_vbat_minwhenpair>v_Vbat_inuse)v_vbat_minwhenpair=v_Vbat_inuse;
            if(nb_forpair>=NB_FORPAIR_LLIMIT&&nb_forpair<=NB_FORPAIR_SAMPULIMIT) //2010-7-19 ADD NB_FORPAIR_SAMPULIMIT
            {
                //ѡ����������
                if(v_pair_ad_input_min>v_pair_ad_input)v_pair_ad_input_min=v_pair_ad_input;               
                if(v_pair_ad_input_max<v_pair_ad_input)v_pair_ad_input_max=v_pair_ad_input; 
            }
        }
    
    /*--------------����v_pair_adֵ������------------------------*/
    //��remark==REMARK_FOR_PAIRSʱ�����һ������޹��ϵ�ֵ��
    //��remark!=REMARK_FOR_PAIRSʱ��Ϊ��ʱʵ�ʲ�����ֵ����Ҫ�ǿ��ǵ��궨�����أ�
    /*--------------����v_pair_coֵ������------------------------*/
    //���һ������޹��ϵ�ֵ�����ܵ�remark�Ƿ����REMARK_FOR_PAIRS��
    //��������ϵ��һֱû����Чֵ����ʹ��v_pair_ee��ΪĬ��ֵ��
    /*--------------����v_pair_eeֵ������------------------------*/
    //���ϵ��Ǵ�EEPROM�ж�ȡʱ������й��ϣ���ʹ�ú��θ߶�Ϊ0�Ĳ���ֵ��ΪĬ��ֵ��
    //      �Բ�ͬ��ECU���ڣ��궨��ϵ����һ����Ĭ�ϵ�v_pair_ee��΢С���죬��Ӧ����170������
    //ÿ�ζϵ�ʱ�����û��PAIRS���ϣ�����REMARKû���뿪��REMARK_FOR_PAIRSʱ����v_pair_co���棬��Ϊ��һ�ε�v_pair_ee��
    //Ҳ����˵��
    //      ECU��ΪREMARK==REMARK_FOR_PAIRSʱ������ֵ��û���˹���Ԥ�ģ���ʱ���ж�û�й��ϣ������ʹ�ã������棻
    //              REMARK��=REMARK_FOR_PAIRSʱ������ֵ�������˹���Ԥ����ʱ���ж�û�й��ϣ������ʹ�ã��������棻
    //��ע������REMARK_FOR_PAIRS=4��
    
    
    if(
        remark==MC_PAIR 
        &&v_vbat_minwhenpair>=180 //2013-4-8 add condiction  for pairs 
    )
    {
        //v_pair_ad=v_pair_ad_input;//��REMARK�뿪REMARK_FOR_PAIRS֮��ֱ����ʾ����ֵ
                            //���ǿ����Լ��ڲ��궨�����
                            //2009-6-2 ��ע����ʱ��Ȼ��Ҫ�ж�PAIRS�Ƿ��й��ϣ�
        //Rec_Pairs_ad();   //2009-6-2 ��仰��֮ǰɾ����,��֪��ʲôԭ��ɾ��;
        Rec_Pairs_ad();//2009-6-2 add back 
    }
    
    /*-------------------------------------------------------------*/
    //if(REMARK!=MC_NORMAL)
    //    VF_PAIR_VALID_FOREE_PAIR=true;//ÿ��REMARK�뿪REMARK_FOR_PAIRS֮�󣬱���λ���򱾴��ϵ�����еĲ�����Ϣ��������¼��
    //2013-4-9 ��������

    //��ȡ���μ���
    //2008-9-4 delete this condiction temp
    if(!VF_WITHPAIRS_PRO2||VF_ER_PAIR||VF_PAIR_BYMANNUAL_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR)
    {   
        /*��ѹ�������������
            1��ʹ��EEPROM�е�ֵ��
            2������EEPROM��д��ֵ��
        */ 
        /*
        //2013-4-2 ��Ȼ����ʱʹ�����
        VF_ER_PAIR=false; 
        v_pair_co=v_pair_ee=v_pair_ad=M0_PAIR_CAB_L;  //2010-9-13                     
        */
        //2013-3-11 CHANGE TO THE FOLLOWING���EEPROM�е�ֵ
        
        //2014-1-16 ADD CONDICTION  &&VF_ECUHARD_WITHOUTPAIR_PAIR
        //��ʶֻ��ѹ����ʶλ����Ч�������,��EEPROM��ֵ��������Ĭ��ֵ        
        if(VF_ECUHARD_WITHPAIR_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR||VF_PAIR_BYMANNUAL_PAIR){
            v_pair_ad=v_pair_co=v_pair_ee;//ʹ��EEPROM�е�ֵ��
        } else v_pair_ad=v_pair_co=M0_PAIR_CAB_L;
                        
    } 
    
    
    
    //2013-4-2 MOVE HERE ,ԭ����ѹ����������ECU��REMARK!=4ʱ������������
    /*-------------------------------------------------------------*/
    //ע�⣺
    //ÿ��ʹ�õ�v_pair_co�������µĿɿ������ݣ�
    //�����û�вɼ���������EE�е�
    //���EE�еĻ������⣬����0����Ϊ����������ΪĬ��ֵ��
    //if(!VF_ER_PAIRS)  //2009-6-2 delete this condiction 
    {   
        //���û�й��ϣ�����㼶��   
        if(Get_Pairs_Odr_and_Altitude(v_pair_co,false))t_pair_er++;   
    }
    
    //2013-8-13
    if(!VF_WITHPAIRS_PRO2) VF_ER_PAIR=false;
    
    
   // VF_ER_PAIR=false;//2013-8-1 ��ʱ������9��
            
}

/*----------------------------------------------------------------------------*/
//**************load_d_dk*********************************************
//��ȡEE��EE+1��������ز�����������У�飻��У�鲻ͨ��ʱ������0��
//TOBECHECK:���ص���Ϣ��ԭ������ʦ���ˣ�ԭ������ʦ����0��־��
/*
byte Load_PAIRS_DK(byte add)
{
     byte b;
     
     b=Load_Data_EE(add);
     if((b+1)==Load_Data_EE(add+1))   //2010-1-4 add data(xb)=data(x)+1;
     {
        return b;
     }
     else
     {
        return 0;
     }
}
*/
//���ܣ�
//      ��ȡ�ϴζϵ�ʱ�洢��EE�еĺ��θ߶����ݣ�
//      ����У�鱾�ε�ѹ�����
void Check_PairinEE_Valid(void)
{
        byte B_NOTCAB;
        byte b;
        
        
        #if M0_PAIRS==PAIRS_CAB 
            B_NOTCAB=false;//2010-1-4
            kdPairs=(sbyte)Load_D_DK(EE_PAIRS_K,false);    //2010-1-4
            if(VF_DOUBLEER_EE)B_NOTCAB=true;
            bdPairs=(sbyte)Load_D_DK(EE_PAIRS_OFFSET,false);
            if(VF_DOUBLEER_EE)B_NOTCAB=true;
            //2010-1-4
            v_Tmp_KPT=Load_D_DK(EE_P_K_TMP,false);
            if(VF_DOUBLEER_EE||v_Tmp_KPT<30||v_Tmp_KPT>140)
            {
               B_NOTCAB=true;              // 2010-4-27
               v_KPT=0;
               v_KPT_L=0;//2010-1-15
            } 
            v_KPT=(sbyte)Load_D_DK(EE_PAIR_KPT,false);
            
            if(1)  //2010-1-22 
            {
                if(VF_DOUBLEER_EE)
                {
                    v_KPT=0;    
                    B_NOTCAB=true;    //2010-1-15 delete tmp
                }
                v_KPT_L=(sbyte)Load_D_DK(EE_PAIR_KPTL,false);
                //v_KPT=0;
                if(VF_DOUBLEER_EE)
                {
                    B_NOTCAB=true;    //2010-1-15 delete tmp
                    v_KPT_L=v_KPT;    //2010-1-15 linshi 
                }
            } else 
            {
                v_KPT=v_KPT_L=0;
            }
        
        #else
            v_KPT=v_KPT_L=0;
            kdPairs=DKKP215_M256;
            bdPairs=DBKP215;
            B_NOTCAB=false;
            v_Tmp_KPT=TMP_KPT_ST;
        #endif
        
        VF_PAIR_BYMANNUAL_PAIR=VF_ECUHARD_WITHPAIR_PAIR=VF_ECUHARD_WITHOUTPAIR_PAIR=false;
        b=Load_Key(EE_WITHPAIRS);
        
        if(EE_DATA_WITHPAIRS==b)VF_ECUHARD_WITHPAIR_PAIR=true;
        else if(EE_DATA_WITHOUTPAIRS==b)VF_ECUHARD_WITHOUTPAIR_PAIR=true;
        else if(EE_DATA_PAIRS_BYMANNUAL==b)VF_PAIR_BYMANNUAL_PAIR=true;
        else  {
            B_NOTCAB=true;    //2014-11-27    
        }
        //B_NOTCAB=VF_ECUHARD_WITHPAIR_PAIR||VF_ECUHARD_WITHOUTPAIR_PAIR;//���û�����Ƿ���PAIRS��Ӳ����־,��ʾECUû�б궨
        
        //�ж�pairs�Ƿ�궨��
        if(kdPairs>DELTA_K_PAIRS
            ||kdPairs<-DELTA_K_PAIRS
            ||bdPairs>DELTA_B_PAIRS
            ||bdPairs<-DELTA_B_PAIRS
            ||B_NOTCAB)
        {                   //2010-1-4 add VF_DOUBLEER_EE
            VF_NOTCAB_PAIR=true;//
            kdPairs=0;//?
            bdPairs=0;//
            v_KPT=0;  
            v_KPT_L=0;//2010-1-15      
            v_Tmp_KPT=TMP_KPT_ST;//v_Tmp_bymcu-v_Tmp_KPT
        } 
        else VF_NOTCAB_PAIR=false;

        //deTPS=b;

        VF_VALUEERINEE_PAIR=false;

        v_pair_ee=Load_D_DK(EE_PAIRS,false);          //��û���ѹ������ֵ��EE�еı���   0x19  0x1A
        
        
        if(Get_Pairs_Odr_and_Altitude(v_pair_ee,true))VF_VALUEERINEE_PAIR=true;
        else v_pair_co=v_pair_ee;                   //��EE�д洢�Ľ����Ϊv_pair_co�ĳ�ʼֵ?
        
        
        //��EE�д洢��PAIRS������ʱ��Ĭ��Ϊ0���Σ�
        if(VF_VALUEERINEE_PAIR)
        {
            v_pair_ee=v_pair_co=M0_PAIR_CAB_L;
        } else //if//(VF_ECUHARD_WITHOUTPAIR_PAIR)
        {
            v_pair_co= v_pair_ee;//��EE����û������,��Ӳ����־Ϊû��ѹ��������ʱ,ʹ��EE�д洢��ѹ�� �ź�
                                  //���ﲻ��Ҫ���Ƿ�ECU�Ѿ��궨��,���,��ʹECUû�б궨Ҳ����ָʾѹ������
        }
        
        Rec_Pairs_co();//
        Get_Pairs_Odr_and_Altitude(v_pair_co,true);// 
        
        //����Ǵ�ѹ����������ECU������ѹ��ֵû���趨 2013-9-9
        if(VF_ECUHARD_WITHPAIR_PAIR)VF_VALUEERINEE_PAIR=false;//2013-9-9
        if(VF_ECUHARD_WITHOUTPAIR_PAIR)VF_VALUEERINEE_PAIR=false;//2013-9-9  ��ʱ����Ҫ�����������

        if(VF_ECUHARD_WITHOUTPAIR_PAIR&&VF_WITHPAIRS_PRO2)VF_SOFTCFG_ERR_PAIRS=true;//Ӳ����ѹ���źţ������ʹ��ѹ��
        
        //deOS= V_PAIR_STATUS;

}

//���ݻ���ѹ������ֵ����ü���ͺ��θ߶ȣ���ģʽ��
//�����
//     v_pair_odr ��v_pair_mode
//     v_pair_Altitude
//  ��У��ѹ������ֵ�Ƿ����ѹ�������뺣�θ߶��Ƿ�һ�� 
byte Get_Pairs_Odr_and_Altitude(byte pairs,byte blnEE)
{

    sword sw;
    
    //ÿһ�����ζ�Ӧ��PAIR��ԼΪ���� 
    //���㺣�μ���
    sw=M0_PAIR_CAB_L-(sword)pairs;       
    if(sw<0)
    {
        //if(sw<-16) 
        if(sw<-24) 
        {
            if(blnEE)VF_VALUEERINEE_PAIR=true;
            else 
            {
                VF_ER_PAIR=true;

            }
        }
        else
        {
            v_pair_odr=0;
            v_pair_mode=0;   
        }
    } 
    else if(sw>=MAP_FOR_H6000)//���δ���=7000m
    {
        if(blnEE)VF_VALUEERINEE_PAIR=true ;
        else 
        {
            VF_ER_PAIR=true;

        }
        v_pair_odr=7;
        v_pair_mode=0; 

    } 
    else
    {   //
        v_pair_odr=(sw>>4)&0x0f;
        v_pair_mode=sw&0x0f;            
    }

    v_Pair_deg=((v_pair_odr<<4)&0xf0)+v_pair_mode;
    if(v_Pair_deg>64)v_Pair_deg=64;

    
    v_p_kpa=101-(sword)v_Pair_deg/8*5;
        
    return ((pairs>=COR_MAX_PAIR)||(pairs<=COR_MIN_PAIR));
}

//��PAIRS�궨�����PAIRSֵ����������
void Rec_Pairs_ad(void)
{
    //sword sw; //2009-12-18 ��������ˤ��һ��
    long int sw;
    
    //sw=(((((long int)v_pair_ad*(256+(sword)kdPairs))>>8)&0xff)+bdPairs);   //2009-12- 21 
    sw=(((((long int)v_pair_ad*(256+(sword)kdPairs))>>8)&0xffff)+bdPairs);   //2009-12- 21 
    //����궨��Ľ��
    
    if(sw<COR_MIN_PAIR)v_pair_co=COR_MIN_PAIR;
    else if(sw>COR_MAX_PAIR)v_pair_co=COR_MAX_PAIR;
    else v_pair_co=(byte)sw;  
    
    //2010-1-4 add
    sw=(sword)v_Tmp_bymcu-v_Tmp_KPT;
    if(sw>0)sw=(sw*v_KPT)/128;
    else  sw=(sw*v_KPT_L)/128;  //2010-1-15 add 
    
    v_pair_co=v_pair_co+(sbyte)sw;
}

//��PAIRS�궨�����PAIRSֵ����������
void Rec_Pairs_co(void)
{
    //sword sw; //2009-8-16 ɾ����������Ϊlong int �ͣ�
    long int sw;//2009-8-16
    
    //sw=((((v_pair_ad*(256+(sword)kdPairs))>>8)&0xff)+bdPairs); 
    //2009-6-2 �ĵ� Rec_Pairs_co�����⣬����sw�ķ�Χ��������˻��������
    //2010-1-5 add------
    sw=(sword)v_Tmp_bymcu-v_Tmp_KPT;
    if(sw>0)sw=(sw*v_KPT)/128;
    else sw=(sw*v_KPT_L)/128; 
    //------------------
    sw=v_pair_co-sw;//2010-1-5 add -sw;
    sw=((sw-bdPairs)*256)/(256+(sword)kdPairs);
    //����궨��Ľ��
    if(sw<ORI_MIN_PAIR)v_pair_ad=ORI_MIN_PAIR;
    else if(sw>ORI_MAX_PAIR)v_pair_ad=ORI_MAX_PAIR;
    else v_pair_ad=(byte)sw;  
    
}  

/*
        �ж�PAIRS�Ƿ��й��ϵķ�����
        1������ֵ�Ƿ�����������Χ��
        2��ÿ��ʱ���ڣ�����ֵ�������Сֵ�Ĳ����Ƿ񳬹���������ֵ��
        3����ÿ�ι�Կ��ʱ����Ϊ��ȷPAIRSֵ����������������PAIRS���ܹ���ʱ��ʹ�����һ����Ϊ��ȷ��ֵ��
        4�����ڲ���ֵ��ʹ�ã�
                ֻ����Ϊ�ϸ�ʱ��ʹ�ã���ECUδ���ʱ�ж�������һЩ��
                
*/