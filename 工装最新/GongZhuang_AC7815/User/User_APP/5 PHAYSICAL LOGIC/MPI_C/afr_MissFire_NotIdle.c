#include "status_st.h"
#include "public.h"

#include "ee.h"
#include "add.h"

/*
     ���ڷ����˵����
        ���ڷǵ���ʱ��⵽ת�ٵı仯���ƣ�����--����--�ټ�������Ϊ��ƫϡ�����ʧ��
           
*/
/*----------THE FOLLOWING ARE public variables----------------------------------------------*/
//#pragma DATA_SEG MY_ZEROPAGE
//volatile V_STATUS_STR _V_NOTIDLEMF_STATUS ;
//#pragma DATA_SEG DEFAULT

sbyte v_MissfireNotIdle;        //�൱������ʦԭ�ȵ�modi_fachong, ֻ������ֵ��Ϊ0��
sbyte v_FMF_MissfireNotIdle;    //������������ϵ����
/*----------THE FOLLOWING ARE local macros--------------------------------------------------*/ 
#define M0_OFFSET_MISSFIRENOTIDLE       0//�൱������ʦԭ����offset_fach��ֻ������ֵ��Ϊ0��
#define MO_RUNS_FOR_WAITFACH    30
#define M0_REV2000_15D6 128             //
#define M0_DREV_LIMIT   64              //����ת�ٲ���ļ���ֵ����������ֵʱ����Ϊ������
#define M0_SUM_DRPM_ULIMIT      127     //
#define M0_SUM_DRPM_LLIMIT      -127    //
#define M0_DOWNMUCH_EXPECTION       -51     //sum_rpmchange156С�ڸ���ֵʱ��bu��Ϊת��ͻ��
#define M0_DOWNMUCH_THREHOLD    -11     //sum_rpmchange156С�ڸ���ֵʱ����Ϊת��ͻ��
#define M0_STEP_FOR_1MISSFIRE   4       //
#define M0_CO_MISSFIRE_U        10
#define M0_C0_MISSFIRE_L        -10     //
#define M0_SUM_DRPM_UP_THREHOLD 20      //
#define M0_DRPM_RISE_TO_DOWN    5       //
#define M0_RISEMUCH_EXPECTION   72      //����������ֵʱ����Ϊ���쳣
#define M0_RUNS_RISEMUCH_LIMIT  28      //ת��ͻ����Ȧ������ֵ
#define M0_NB_S_FORRESET_WHEN_REMARKE6  5 //
/*----------THE FOLLOWING ARE local variables-----------------------------------------------*/ 
byte wait_run_fach;     //   
byte drun_number;       //
sword sum_rpmchange156; //
byte max_sum_rchange;   //
word rev_15d6_lst;     //��һ�ε�rev_15d6
byte nb_1ss_when_remarke6;//��REMARK==6ʱ��������1S�Ĵ�����
    
/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void Annuation_MissfireINC(byte remark,sbyte MissfireNotidle_IdleSC);
void Rpm_Inc_Set(word rev_15d6);
void Rpm_Dec_Set(word rev_15d6);
void Clr_bToo_much(word rev_15d6);
void Rpm_Change_Init(word rev_15d6,sword dRev);
void Next_Rpm_C_t(word rev_15d6,sword dRev);
void Down_End(word rev_15d6,sword dRev);
void Rise_End(word rev_15d6,sword dRev);
void RPM_CH_WR_IALL(word rev_15d6);
void Check_MissfireNotidle_valid(void);
/*----------THE FOLLOWING ARE PUBLIC SUB PROGRAMS-------------------------------------------*/ 
 
//��λ��Է�������ĳ�ʼ��
void Init_MissfireNotIdle_afterReset(void)
{
    V_NOTIDLEMF_STATUS=false;//2008-12-15
    if( 0&&VF_VPOR_HARD)
    {   /*
        v_MissfireNotIdle=Load_Data_EE(EEMODI_FACHONG);
        Check_MissfireNotidle_valid();     */
    }
}

void Detect_MissfireNotIdle(sbyte fmf_trans1,byte remark,word rev_15d6,sbyte MissfireNotidle_IdleSC)
{

    if( 0 )
    { /*
        Annuation_MissfireINC(remark,MissfireNotidle_IdleSC);
        //���ӱ�־λ��VF_DECNOTIDLE_RPM�� VF_INCNOTIDLE_RPM�� VF_BDOWN_TOOMUCH_RPM��VF_BRISE_TOOMUCH_RPM
        drun_number++;
        if(wait_run_fach>0)wait_run_fach--;
        if(!VF_WORM_ENG)
        {
                if((fmf_trans1>4)||(fmf_trans1<-4))wait_run_fach=MO_RUNS_FOR_WAITFACH;
                if(wait_run_fach>0)VF_REVDOWNMUCH_NOTIDLEMF=false;
                if(VF_YSTDC_INJPHASE)
                {
                        if(VF_IDLE_ENG)wait_run_fach=MO_RUNS_FOR_WAITFACH;
                        if(rev_15d6>M0_REV2000_15D6)
                        {
                                dRev=(sword)rev_15d6-rev_15d6_lst;
                                                
                                if((dRev>M0_DREV_LIMIT)||(dRev<-M0_DREV_LIMIT))dRev=0;
                                //��ת�ٵ�������������ж�
                                if(dRev==0)
                                {
                                        VF_REVDECNOTIDLE_NOTIDLEMF=false;
                                        VF_REVINCNOTIDLE_NOTIDLEMF=false;                
                                }
                                else if(dRev>0)
                                {       //�Ա�Ȧת��������������д���
                                        if(VF_REVDECNOTIDLE_NOTIDLEMF)
                                        {
                                                Rpm_Inc_Set(rev_15d6);
                                                //goto down_end 
                                                Down_End( rev_15d6, dRev);               
                                        }
                                        else
                                        {       //rpm_r_r_test
                                                if(VF_REVINCNOTIDLE_NOTIDLEMF)
                                                {
                                                        //rpm_r_r
                                                        sum_rpmchange156+=dRev;
                                                        if(sum_rpmchange156<M0_SUM_DRPM_ULIMIT)
                                                        {
                                                                Rpm_Inc_Set(rev_15d6);        
                                                        }
                                                        else
                                                        {       //goto next rpm_c_t
                                                                //��ת��һֱ�������ۼ���������һ��ֵ֮�󣬴�ͷ��ʼ
                                                                Next_Rpm_C_t(rev_15d6, dRev);
                                                        }
                                                } 
                                                else
                                                {       //��һȦ��ת�ٲ���Ҳ����������Ȧ����
                                                        //rpm_nc_r
                                                        Rpm_Inc_Set(rev_15d6);
                                                        if(sum_rpmchange156>=0)
                                                        {
                                                                //rpm_r_r
                                                                sum_rpmchange156+=dRev;
                                                                if(sum_rpmchange156<M0_SUM_DRPM_ULIMIT)
                                                                {
                                                                        Rpm_Inc_Set(rev_15d6);        
                                                                }
                                                                else
                                                                {       //goto next rpm_c_t
                                                                        //��ת��һֱ�������ۼ���������һ��ֵ֮�󣬴�ͷ��ʼ
                                                                        Next_Rpm_C_t(rev_15d6, dRev);
                                                                }
                                                        }
                                                        else
                                                        {       //��ʱsum_rpmchange156С��0
                                                                //˵��֮ǰ�ۼ��ڼ�������Ȧ������
                                                                //goto down_end
                                                                Down_End(rev_15d6, dRev);
                                                        }
                                                }
                                                
                                        }
                                } 
                                else
                                {
                                        //�Ա�Ȧת���½���������д���
                                        //rpm_dec_calcul
                                        if(VF_REVINCNOTIDLE_NOTIDLEMF)
                                        {       //֮ǰת����������Ȧת���½�
                                                Rpm_Dec_Set(rev_15d6);
                                                Rise_End(rev_15d6, dRev);  //ת��������ͷ��      
                                        } 
                                        else
                                        {
                                                //rpm_d_d_test
                                                if(VF_REVDECNOTIDLE_NOTIDLEMF)
                                                {       //rpm_d_d
                                                        sum_rpmchange156+=dRev;
                                                        if(sum_rpmchange156<M0_SUM_DRPM_LLIMIT)
                                                        {
                                                                sum_rpmchange156=M0_SUM_DRPM_LLIMIT;                
                                                        }
                                                        Rpm_Dec_Set(rev_15d6);
                                                }
                                                else
                                                {       //rpm_nc_d
                                                        Rpm_Dec_Set(rev_15d6);
                                                        if(sum_rpmchange156<=0)
                                                        {
                                                                //֮ǰ�ۼƲ���������Ȧ�½�       
                                                                sum_rpmchange156+=dRev;
                                                                if(sum_rpmchange156<M0_SUM_DRPM_LLIMIT)
                                                                {
                                                                        sum_rpmchange156=M0_SUM_DRPM_LLIMIT;                
                                                                }
                                                                Rpm_Dec_Set(rev_15d6);
                                                        } 
                                                        else
                                                        {
                                                                //֮ǰ�ۼ���������Ȧ�½�
                                                                Rise_End(rev_15d6, dRev);
                                                        }
                                                        
                                                }
                                        }
                                }
                        }
                        else
                        {      //��ת�ٵ���ĳ����ֵʱ
                               //RPM_CH_WR_IALL
                               RPM_CH_WR_IALL(rev_15d6);       
                        }
                }//ELSE RETURN //��ѹ��ѭ��
        } 
        else
        {       //����ů������
                //RPM_CH_WR_IALL
                RPM_CH_WR_IALL(rev_15d6);
        }
    */}
}

//��÷�����������ϵ��
word CO_FMF_MissfireNotIdle(sword fmf,byte tps_odr)
{
    sbyte sL;

    if( 0)
    {
     /*   //if((tps_odr>0)&&(!VF_IDLE_ENG)&&((!VF_O2WORK_OSDIAG)||VF_O2ER_OSDIAG))
        if((tps_odr)&&(!VF_IDLE_ENG)&&((!VF_O2WORK_OSDIAG)||VF_OSER_OSDIAG))
        {
                v_FMF_MissfireNotIdle=v_MissfireNotIdle-M0_OFFSET_MISSFIRENOTIDLE;
                //sL=tps_odr*2+1;
                sL=(tps_odr<<1)&0xfe+1 ;
                if(v_FMF_MissfireNotIdle<-sL)v_FMF_MissfireNotIdle=-sL;
                //sL=tps_odr*2+4;
                //sL=(tps_odr<<1)&0xfe+4 ;
                sL+=3;  //��������ʡ16bytes
                if(v_FMF_MissfireNotIdle>sL)v_FMF_MissfireNotIdle=sL; 
                if(VF_STARTS_ENG)
                {
                        if(v_FMF_MissfireNotIdle<0)v_FMF_MissfireNotIdle=0;
                }
                fmf=fmf+v_FMF_MissfireNotIdle;
                if(fmf<10)fmf=10;        
        } else
        {
                v_FMF_MissfireNotIdle=0;
        } */
    }
    
    return fmf;
}

void Save_MissfireNotIdle(void)
{
    if( 0 )
    {
        /*
        //������������1�����Ժ󣬵ݼ�һ��
        if(VF_MORE1MIN_TMCCL)
        {
                if(VF_O2WORK_OSDIAG)
                {       
                        v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;        
                } else
                {
                        v_MissfireNotIdle--;
                        Check_MissfireNotidle_valid();
                }
                Save_Data_EE(EEMODI_FACHONG,(byte)v_MissfireNotIdle);
        } */
    }
}
/*---------local functions--------------------------------------------------------*/
//����ϵ���ĵݼ�
void Annuation_MissfireINC(byte remark,sbyte MissfireNotidle_IdleSC)
{
    if( 0)
    {
       /* 
        //��REMARK==6ʱ���÷���ָ����λ
        if(MissfireNotidle_IdleSC!=0)
        {
                v_MissfireNotIdle-=MissfireNotidle_IdleSC;
                if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_CO_MISSFIRE_U;                
                if(v_MissfireNotIdle<M0_C0_MISSFIRE_L)v_MissfireNotIdle=M0_C0_MISSFIRE_L;                
        }
        //���ڱ궨״̬�£��÷���ϵ��Ϊ0
        if((remark==6)||(remark==8))
        {
                if(VF_1S_TMCCL&&(nb_1ss_when_remarke6<=M0_NB_S_FORRESET_WHEN_REMARKE6))nb_1ss_when_remarke6++;
                if(nb_1ss_when_remarke6>=M0_NB_S_FORRESET_WHEN_REMARKE6)
                {
                        v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;                
                }
        } 
        else
        {
                nb_1ss_when_remarke6=0;        
        }   */
    }
}

//ת������ʱ�ĳ�ʼ��������ת��������־
void Rpm_Inc_Set(word rev_15d6)
{
        VF_REVDECNOTIDLE_NOTIDLEMF=false;
        VF_REVINCNOTIDLE_NOTIDLEMF=true;
        rev_15d6_lst=rev_15d6;                
}

//����ת���½���־
void Rpm_Dec_Set(word rev_15d6)
{
        VF_REVDECNOTIDLE_NOTIDLEMF=true;
        VF_REVINCNOTIDLE_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6;                
}

//���� ������Ѿ��õ��Ŀ��ܷ���ı��
void Clr_bToo_much(word rev_15d6)
{
        VF_REVDOWNMUCH_NOTIDLEMF=false;
        VF_REVRISEMUCH_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6;                
}

void Rpm_Change_Init(word rev_15d6,sword dRev)
{
        sum_rpmchange156=dRev;
        drun_number=0;
        rev_15d6_lst=rev_15d6;        
}

//���ܣ���ͷ��ʼ���з�����
//euqal to rise_small, down_small
void Next_Rpm_C_t(word rev_15d6,sword dRev)
{
        Clr_bToo_much(rev_15d6);
        Rpm_Change_Init(rev_15d6, dRev);        
}

//���ܣ�
//      ����֮ǰת���ۼ�����С��0������һȦת��Ϊ�ݼ�����
//      ����Ȧת��Ϊ����ʱ������ Down_End��
//      �ж��Ƿ���ת��ͻ����
//      ����ͻ�������ж��Ƿ��Ƿ��壬�����壬���з��崦��
void Down_End(word rev_15d6,sword dRev)
{       //��ʱsum_rpmchange156Ӧ���Ǹ�������
    if( 0)
    {
    /*    
        if((sum_rpmchange156<M0_DOWNMUCH_EXPECTION)||         //���쳣
                (sum_rpmchange156>M0_DOWNMUCH_THREHOLD))  //ת���½�������
        { //DOWN SMALL
                Next_Rpm_C_t(rev_15d6, dRev);
        }
        else
        {
                max_sum_rchange=M0_DRPM_RISE_TO_DOWN-sum_rpmchange156;
                if(VF_REVRISEMUCH_NOTIDLEMF&&VF_REVDOWNMUCH_NOTIDLEMF)
                {       //�ڴ˼�⵽���壬 ���д���
                        //leans_too_much
                        v_MissfireNotIdle+=M0_STEP_FOR_1MISSFIRE;
                        if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_CO_MISSFIRE_U;
                } 
                else
                {
                        //down_too_1stly
                        //��⵽Ҫ����ĵ�һ���½�
                        VF_REVRISEMUCH_NOTIDLEMF=false;
                        VF_REVDOWNMUCH_NOTIDLEMF=true;
                        Rpm_Change_Init(rev_15d6, dRev);
                }
        }  */
    }
}

//���ܣ�
//     ��ת�ٳ���������������ת���½�ʱ���ж�֮ǰ��ת���Ƿ���ͻ�� 
//     ���Ƿ���VF_BDOWN_TOOMUCH_RPM 
void Rise_End(word rev_15d6,sword dRev)
{
    if( 0)
    {
 /*       
        if(sum_rpmchange156<=M0_SUM_DRPM_UP_THREHOLD)
        {
                //�����Ĳ�����
                //RISE_SMALL
                Next_Rpm_C_t(rev_15d6,dRev);
        } 
        else
        {
                //trise_too_much
                if(VF_REVDOWNMUCH_NOTIDLEMF)
                {       //�ϴ�ת��ͼ����
                        //�����ۼ���������
                        if((sum_rpmchange156>=max_sum_rchange)//��������ֵ ���ϴ��½�ֵ��M0_DRPM_RISE_TO_DOWN
                                &&(sum_rpmchange156<M0_RISEMUCH_EXPECTION)
                                &&(drun_number<M0_RUNS_RISEMUCH_LIMIT) )
                        {
                                VF_REVRISEMUCH_NOTIDLEMF=true;
                                Rpm_Change_Init(rev_15d6,dRev);                
                        }else
                        {       //RISE_SMALL
                                Next_Rpm_C_t(rev_15d6,dRev);
                        }
                } 
                else
                {
                        Next_Rpm_C_t(rev_15d6,dRev);        
                }
        
        } */
    }
}

void RPM_CH_WR_IALL(word rev_15d6)
{
    if( 0 )
    {
        /*
        wait_run_fach=MO_RUNS_FOR_WAITFACH;
        sum_rpmchange156=0;
        max_sum_rchange=0;
        
        VF_REVDECNOTIDLE_NOTIDLEMF=false;
        VF_REVINCNOTIDLE_NOTIDLEMF=false;
        rev_15d6_lst=rev_15d6; 
        Clr_bToo_much(rev_15d6); */              
    }
}

void Check_MissfireNotidle_valid(void)
{
    if( 0)
    {
        /*
        if(v_MissfireNotIdle>M0_CO_MISSFIRE_U)v_MissfireNotIdle=M0_OFFSET_MISSFIRENOTIDLE;                
        if(v_MissfireNotIdle<M0_C0_MISSFIRE_L)v_MissfireNotIdle=M0_C0_MISSFIRE_L;*/                
    }
}