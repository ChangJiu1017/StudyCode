/*===========================================================*/
/*====��������ģ��=====================*/
/*===========================================================*/
#include "map_selection.h"

//#define M0_FMF_FOR1CYCLE  1800//��Ȧ���������������2012-10-9
//#define M0_FMF_FOR1CYCLE  3000//��Ȧ���������������2014-12-9
#define M0_FMF_FOR1CYCLE  10000


/*----------THE FOLLOWING ARE public variables----------------------------------------------*/

extern sword v_FMF_remain;  //��һȦ����������ĳ����ֵʱ��Ҫ��������v_FMF_remainΪ��ֺ���������
extern sword v_FMF;         //��Ȧ��Ҫִ�е���������
extern sword v_FMF_lst; //��һ��ʵ��������
extern sword v_FMF_all_lst; //��һ����������

extern sword v_tw1;         //��Ȧ��Ҫִ�е�T1

//2008-5-29 add

extern sword v_fmf_O;//2009-8-3 ���¶����Oֵ
extern byte   M0_FMF_IDLE    ;//2009-8-20 add 
extern byte v_fmf_grind;//2010-2-3

extern sword vc_FMF_dtfi_trans; //2010-5-14 ����Ĺ��ɲ��ֵ�dtfi 

extern word v_dms;//2013-6-22

/*----------the following are declaration of public sub---------------*/
//���ܣ���ʼ��FMFģ��
extern void FI_FMF_AfterReset(void); //����ģ��ĳ�ʼ��

//���ܣ���������������
//+��Ҫ��֤����������if(v_FMF_H,v_FMF_L>=256)v_FMF_H,v_FMF_L=256;
//     FMF_lspd--������������ϵ��
extern sword FC_FMF_LowSpeed(sword fmf,byte FMF_lspd);

extern sword v_FMF_all;//һ��ѭ������Ҫ�����������  2014-4-14

//-----------rec_by_astk_tmp:--------------------------------------------------------------
//���ܣ��¶ȡ��������Ͷ������Ĺ�ͬ������
//  +map_ti*(128+vc_FMF_STARTS/2)/128+vc_FMF_STARTS+map_ti*coefficient_rzt/128=map_tinj*(coefficien+vc_FMF_STARTS/2)/128+vc_FMF_STARTS/2
//  �¶�����
//  ���RZT��TAIR������������ϵ����
//  �����壺
//		v_Tair_ad:ֱ�Ӱ���������״̬�������㣻
//		v_Teng_inuse:���ǶԽ����ļ���ЧӦ��
//����ֵ�������������
extern sword FC_FMF_TMP(sword fmf,byte fmf_tair,sbyte fmf_teng) ; //2008-11-11 add ,byte fmf_restart

//���ܣ��ջ�����������
//����ֵ�������������
extern sword FC_CLS(sword fmf,sbyte fmf_cls);

//���ܣ����ɺͶ��Ϳ���������
//����ֵ�������������
//extern sword FC_FMF_TransCI(sword fmf,byte tair,byte teng,sbyte fmf_cfci,sbyte fmf_trans1,byte fmf_trans2,byte fmf_starts);

//���ܣ�����ʧ���������������
//����ֵ�������������
extern sword FC_MissF_FMF_Idle(sword fmf,sbyte tw1_idlemissf);

//���ܣ�����ʧ��Լ���������TW1��������
//�βΣ�IDLEMISSF_tmp����vc_TW1_IDLEMISSF_tmp
extern sword FC_MissF_TW1_Idle(sword fmf, sbyte IDLEMISSF_tmp);

//���ܣ�����ʧ���DTFI��������
//�βΣ�IDLEMISSF_tmp����vc_TW1_IDLEMISSF_tmp
extern sword FC_MissFireForDtfi_Idle(sword dtfi,sbyte IDLEMISSF_tmp);

//���ܣ���ȦΪ������Ȧ������׼����������Ȧ���ͣ�ʱ�����������Ĳ��
//      TOBECHECK,��Ҫ�жϱ�Ȧ�Ƿ�����Ȧ
//����ֵ����ֺ��������
extern sword F_remain_FMF(sword fmf) ;

//���ܣ�����������ʱ����FMF������
//����ֵ�������������
extern sword FC_IdleSC_FMF(sword fmf,sbyte tw1_IDLESC_inuse,sword tw1_idle_sc,sword fmf_sci_idle,sword fmf_sci_nidle,sbyte tw3offset);

//���ܣ�T3������T1��������
//����ֵ���������TW1
//       tw1_t3--ƫ����2
//extern sword CO_TW1_T3FK(sword tw1,sword tw1_t3_fk_pid,sbyte tw1_t3,sbyte vbat_dt,sword tw1_t3FK_I);

//���ܣ���ص�ѹ�䶯��TW1������
//����ֵ���������TW1
extern sword FC_UbatChange_TW1(sword tw1,sbyte tw1_vbatchg,sword tw1_vbatdownidle) ;


extern byte v_nb_rinj;     //����֮ǰ�����������������Ĵ�����
extern byte v_nb_rinj_y;   //����֮ǰ�����������������Ĵ�����
