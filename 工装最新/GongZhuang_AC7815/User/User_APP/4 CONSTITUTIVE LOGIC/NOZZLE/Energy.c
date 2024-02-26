

#include "Public.h"
#include "Status_st.h"
#include "energy.h"

#include "look_up.h"

#include "add.h"

#include "ee.h"

#include "sys_init.h"

//2010-3-13 add 
#include "data_sys_standard.h"
#include "sys_definition.h"   //2014-11-25

#include "hard_trig.h"
                 

//���ݵ�������   2014-12-24
word vbatv_M200[3];
word i_M51[3];

unsigned long int l_tmp;

#define M200_M51_M125	    1275000UL
#define M51_M51_M50	      130050UL


#define 	T1S_12V	375U
#define 	T1S_24V	250U

#define 	IS0_12V	128
#define 	IS0_24V	200


unsigned long int v_I_T2000Sum_M51;//T1=2000ʱ�ĵ���//2015-1-18  
word v_I_T2000Ave_M51;//T1=2000ʱ�ĵ���//2015-1-18  

byte v_no_lst_IAD_com;//��һ�ε� ��������ֵ���������

    
#define R_ST_M100 95//��׼���裬�Ŵ�100��
#define L_ST_M10000 35//��׼��У��Ŵ�10000��
#define 	KF_ST_M1000	50	//Ħ������ϵ�����Ŵ�1000��
#define 	ISAT_M51	255U

#define 	D_FOR_ET_M5000	510000U
#define 	D_FOR_SQI_M5000	130050U

#define 	D_FOR_ET_M5000_D256	1992U
#define 	D_FOR_SQI_M5000_D256	508U


#define 	D_FOR_ER_M5000	100
#define 	D_FOR_EIN_M5000	5202
//#define 	D_FOR_EF_M5000	5000000UL
#define 	D_FOR_EF_1_M5000	100
#define 	D_FOR_EF_2_M5000	50000


#define 	MKQ	56320U

word v_t_CalI[5][2];//������������ʱ��

//---------------------------------------------------------------
byte v_L_M10000;//���ֵ

word v_Im_M51;//T1��ֹʱ����
word v_Is_M51;//��׼��λ�ö�Ӧ�ĵ���

dword v_Et_M5000;//INT(Id*Ud)
dword v_int_sqI_M5000;//�������ֵ�
word v_Er_M5000;//int(Id^2*r)
word v_Ein_M5000;//L/2*Id^2
word v_Ef_M5000;//���������������ĵĻ���

word v_Ets_M5000;//��׼������

word v_En_M5000; //���ε�����
word v_Ens_M5000;//v_Ens_M5000=v_En_M5000*v_ks_M256/256;��׼״̬�µ�����
word v_En0_M5000;//��׼״̬�£����͵��������궨�õ���

word v_W_M5000;//�������Ч��
word v_ks_M256;

byte v_L_M10000_tmp;//���ֵ

word v_Im_M51_tmp;//T1��ֹʱ����
word v_Is_M51_tmp;//��׼��λ�ö�Ӧ�ĵ���

dword v_Et_M5000_tmp;//INT(Id*Ud)
dword v_int_sqI_M5000_tmp;//�������ֵ�
word v_Er_M5000_tmp;//int(Id^2*r)
word v_Ein_M5000_tmp;//L/2*Id^2
word v_Ef_M5000_tmp;//���������������ĵĻ���

word v_Ets_M5000_tmp;//��׼������

word v_En_M5000_tmp; //���ε�����
word v_Ens_M5000_tmp;//v_Ens_M5000=v_En_M5000*v_ks_M256/256;��׼״̬�µ�����
word v_En0_M5000_tmp;//��׼״̬�£����͵��������궨�õ���

word v_W_M5000_tmp;//�������Ч��
word v_ks_M256_tmp;

word v_W_M5000_tmp_lst;//��һ��ִ��T1�����У���һ���v_W_M5000_tmp

//�ܺͣ����ڼ���ƽ��ֵ֮��
dword v_Et_ave;
dword v_int_sqI_ave;
word v_Er_ave;
word v_Ein_ave;
word v_Ef_ave;

word v_En_ave;

word v_Ens_ave;
word v_W_ave;
word v_Is_ave;
word v_Im_ave;

word v_Kq_MKQ; //E-Qб�ʣ��ɱ궨�õ�

word v_Wn_tg;//��Ч����Ŀ��
dword dWn_tg;//Ŀ�깦������

byte v_ModelType;//����ģ�͵�����
//--- local--------------------------------------------
dword v_Et_sum;
dword v_int_sqI_sum;
dword v_Er_sum;
dword v_Ein_sum;
dword v_Ef_sum;

dword v_En_sum;
word v_nb_Isum;//

dword v_Ens_sum;
dword v_W_sum;
dword v_Is_sum;
dword v_Im_sum;


//��׼�㸽�����������ֺ͵���ֵ����
sword ARRAY_E_t[2][2];
sword ARRAY_I_t[2][2];
dword Et_tmp;
dword Et_tmp_lst;
word Et_tmp_H;
word Et_tmp_H_lst;

dword int_sqI_tmp;
dword int_sqI_tmp_lst;
word  int_sqI_tmp_H;
word  int_sqI_tmp_H_lst;

//dword Et_M5000_tmp;
//dword int_sqI_M5000_tmp;
dword Et_M5000_tmp_lst;
//dword int_sqI_M5000_tmp_lst;

byte v_nb_IIndex_lst;//��һ�ε�AD�ɼ����±�  

word v_Wn_tg_lst;
word v_Wn_r;

//dT1��PID�Ĺ���
sword dT1_p;
sword dT1_i;
sword dT1_d;
sword v_dT1_wn;//����ģ�͵�dT1
word v_T1_lookup;//���õ���T1ֵ��

#define M0_KP_M32   8
#define M0_KI_M32   2
#define M0_KD_M32   1

//===============================Local SubRoutine============================================

void Ctl_Wn(void);

     
/*==========================================================================================*/

/*

    ��ʼ��
*/
void Init_EnergyReset(void) 
{
    v_En0_M5000=0;
    
    v_Wn_tg=WN_TG_INI;
    dWn_tg=0;
    
    v_ModelType=MODEL_TYPE_T3;
    
    dT1_p=dT1_d=dT1_i=0;
    
    v_dT1_wn=0;
    
}


/*
    ��ȡ��һ��Ҫ��ʾ�ĵ�
*/
byte Get_NxtDispPoint(void)
{
      
    if(v_no_lst_IAD_com<v_nb_IIndex_lst)v_no_lst_IAD_com++;//��һ�ε�AD�ɼ����±�  
    else v_no_lst_IAD_com=0;      //2015-1-28 DELETE
    
    return v_no_lst_IAD_com;
}

/*
    
*/
word Disp_t_I(void)
{
    return v_t_I[v_no_lst_IAD_com][1];    
}

byte Disp_IM12D5_I(void)
{
  
    return v_I_I[v_no_lst_IAD_com][1]/4;
}

byte Disp_VBAT_I(void)
{
    return v_V_I[v_no_lst_IAD_com][1];
}

word Disp_wn(void)
{
    return v_wn_tmp[v_no_lst_IAD_com][1];    
}

word Disp_dTW1_forWnTg(void)
{
    return v_T1_pre[v_no_lst_IAD_com][1];    
}

/*
    ����Wn
*/

void Save_Wn_tg(void)
{
    v_Wn_tg_lst=v_Wn_tg;
}

/*
    �����������Ŀ��    
*/
void Adj_Wn_tg(sbyte blnInc)
{
    sword sw;
    
    //if(blnInc)
    {
        dWn_tg+=(sbyte)blnInc*10;  
    }
    // else{
    //    dWn_tg-=10;
    //}

    sw=dWn_tg+WN_TG_INI;
    
    if(sw<0){
        dWn_tg=-(sword)WN_TG_INI;    
    }
    
    v_Wn_tg=dWn_tg+WN_TG_INI;
    
}

/*
    
        
*/
void F_StartQtest_ForEnergy(byte sta) 
{
    switch(sta)
    {
        case QTEST_ENERGY_ST:
            v_Et_sum=0;
            v_int_sqI_sum=0;
            v_Er_sum=0;
            v_Ein_sum=0;
            v_Ef_sum=0;
            
            v_En_sum=0;
            
            v_nb_Isum=0;
            v_Im_sum=0;
            
            v_Ens_sum=0;
            v_W_sum=0;
            v_Is_sum=0;
            
        break;
        
        case QTEST_ENERGY_ACCU:
            
            v_nb_Isum++;
            v_Et_sum+=v_Et_M5000;
            v_int_sqI_sum+=v_int_sqI_M5000;
            v_Er_sum+=v_Er_M5000;
            v_Ein_sum+=v_Ein_M5000;
            v_Ef_sum+=v_Ef_M5000;
            
            v_En_sum+=v_En_M5000;

            v_Ens_sum+=v_Ens_M5000;
            v_W_sum+=v_W_M5000;
            v_Is_sum+=v_Is_M51;
            
            v_Im_sum+=v_Im_M51;
            
        break;
        
        case QTEST_ENERGY_ED:
            if(v_nb_Isum>=1)
            {
              
                v_Et_ave=v_Et_sum/v_nb_Isum;
                v_int_sqI_ave=v_int_sqI_sum/v_nb_Isum;
                v_Er_ave=v_Er_sum/v_nb_Isum;
                v_Ein_ave=v_Ein_sum/v_nb_Isum;
                v_Ef_ave=v_Ef_sum/v_nb_Isum;
                
                v_En_ave=v_En_sum/v_nb_Isum;

                v_Ens_ave=v_Ens_sum/v_nb_Isum;
                v_W_ave=v_W_sum/v_nb_Isum;
                v_Is_ave=v_Is_sum/v_nb_Isum;
                
                v_Im_ave=v_Im_sum/v_nb_Isum;
            }
        
        break;
      
    }
}

/*
    ��������
    
    curpoint: ��ǰ�����
    blnLst:  0--��ǰ�㣬1--��һ�εĵ�
    blnCalEnergy: 0--�������֣�1--��������
    
*/
void Fint_Energy_1pint(byte curPoint,byte blnLst,byte blnCalEnergy)
{
    //for(i=1;i<=v_nb_IIndex_lst;i++)
    word dt;
    dword dw_it;
    
    
    if(curPoint>0)
    {
        //���ֹ���
            Et_tmp_lst=Et_tmp;
            
            int_sqI_tmp_lst=int_sqI_tmp;
            
            Et_M5000_tmp_lst=v_Et_M5000_tmp;
            
            v_W_M5000_tmp_lst=v_W_M5000_tmp;//2015-1-26
            
            vbatv_M200[2]=v_V_I[curPoint][blnLst];
            vbatv_M200[1]=v_V_I[curPoint-1][blnLst];
            vbatv_M200[0]=(vbatv_M200[2]+vbatv_M200[1])/2;
            vbatv_M200[0]=(word)(vbatv_M200[0]+7)*27;
            
            i_M51[2]=v_I_I[curPoint][blnLst];
            i_M51[1]=v_I_I[curPoint-1][blnLst];
            
            /*
            i_M51[0]=(i_M51[2]+i_M51[1])/2;
            i_M51[0]=i_M51[0]*5/3;//3��ŷ��������
            */
            //i_M51[0]=(i_M51[2]+i_M51[1])*5/6; //3��ŷ��������

            i_M51[0]=(i_M51[2]+i_M51[1])/2;
            
            dw_it=((dword)(v_t_I[curPoint][blnLst]-v_t_I[curPoint-1][blnLst]))*i_M51[0];
            
            
            l_tmp=(unsigned long int)vbatv_M200[0]*dw_it ;
            Et_tmp=Et_tmp+l_tmp;
            
            if(Et_tmp<Et_tmp_lst)Et_tmp_H++;
            
            l_tmp=(unsigned long int)i_M51[0]*dw_it;   
            int_sqI_tmp+=l_tmp;

            if(int_sqI_tmp<int_sqI_tmp_lst)int_sqI_tmp_H++;
        
        //���㵱ǰ����
              
                v_Et_M5000_tmp= 
                      ((((dword)Et_tmp_H)<<24)&0xff000000UL)
                      +((Et_tmp>>8)&0x00ffffffUL);//    �൱��/256

                v_Et_M5000_tmp/=D_FOR_ET_M5000_D256;

                v_int_sqI_M5000_tmp= 
                      ((((dword)int_sqI_tmp_H)<<24)&0xff000000UL)
                      +((int_sqI_tmp>>8)&0x00ffffffUL);//    �൱��/256

                v_int_sqI_M5000_tmp/=D_FOR_SQI_M5000_D256;

            //�ҵ���׼���T1��λ��
                if(v_t_I[curPoint][blnLst]>=T1S_24V&&v_t_I[curPoint-1][blnLst]<=T1S_24V)
                {
                    ARRAY_E_t[0][0]=v_t_I[curPoint-1][blnLst];
                    ARRAY_E_t[1][0]=v_t_I[curPoint][blnLst];
                    ARRAY_E_t[0][1]=Et_M5000_tmp_lst; 
                    ARRAY_E_t[1][1]=v_Et_M5000_tmp;
                    
                    ARRAY_I_t[0][0]=v_t_I[curPoint-1][blnLst];
                    ARRAY_I_t[1][0]=v_t_I[curPoint][blnLst];
                    ARRAY_I_t[0][1]=v_I_I[curPoint-1][blnLst];
                    ARRAY_I_t[1][1]=v_I_I[curPoint][blnLst];
                    
                }


            //�������������
            if(blnCalEnergy)
            {
                v_Im_M51_tmp=v_I_I[curPoint][0];
                
                v_Er_M5000_tmp=(dword)v_int_sqI_M5000_tmp*R_ST_M100/D_FOR_ER_M5000;
                
                if(v_Im_M51_tmp<ISAT_M51){
                    v_L_M10000_tmp=L_ST_M10000;
                } else{
                    v_L_M10000_tmp=(word)L_ST_M10000*ISAT_M51/v_Im_M51_tmp;
                }
                
                v_Ein_M5000_tmp=(((dword)v_Im_M51_tmp*v_Im_M51_tmp)*v_L_M10000_tmp)/D_FOR_EIN_M5000/2;
                
                //v_Ef_M5000=((dword)v_Et_M5000*v_Et_M5000/D_FOR_EF_M5000)*KF_ST_M1000;
                v_Ef_M5000_tmp=(((dword)v_Et_M5000_tmp*v_Et_M5000_tmp/D_FOR_EF_1_M5000)*KF_ST_M1000)/D_FOR_EF_2_M5000;
                
                l_tmp=v_Er_M5000_tmp+(dword)v_Ein_M5000_tmp+v_Ef_M5000_tmp;
            
            
            //����En
                if(l_tmp<v_Et_M5000_tmp)
                {
                    l_tmp=v_Et_M5000_tmp-l_tmp;
                    if(l_tmp>0xffff)l_tmp=0xffff;
                    v_En_M5000_tmp=(word)l_tmp;       
                } else{
                    v_En_M5000_tmp=0;
                }
            
            
            //����õ���׼��������͵���
            
            
                v_Is_M51_tmp=F_2DLookUpSword(T1S_24V,ARRAY_I_t,1);
                v_Ets_M5000_tmp=F_2DLookUpSword(T1S_24V,ARRAY_E_t,1);
                
                
                v_ks_M256_tmp=(dword)IS0_24V*256/v_Is_M51_tmp;
                
            
                v_Ens_M5000_tmp=(unsigned long int)v_En_M5000_tmp*v_ks_M256_tmp/256;
                
                
                v_W_M5000_tmp=v_Ens_M5000_tmp-v_En0_M5000_tmp;
        
                v_wn_tmp[curPoint][0]=v_W_M5000_tmp;
                
                
                FS_1inj();
            }
            
            

    } else{
        Et_tmp=0;
        Et_tmp_lst=0;
        int_sqI_tmp=0;
        
        Et_tmp_H=0;
        Et_tmp_H_lst=0;
        int_sqI_tmp_H=0;
        
    }
}


/*
    ����Ŀ��������
    �Լ���ǰ���������ֵ��
    ����T1
    
    Dt1/DW*(Wtg-wi)+ T1i
    
    �ó��������T1ִ�й����У�������v_W_M5000_tmp֮��

    ����ֵ����Ҫִ�е�T1
*/
word FC_dTW1_byWnTg(byte index)
{
    word w1;
    word w2;
    word w3;
    
    //v_t_I[curPoint][0]-v_t_I[curPoint-1][0];    
    
    //if(t1n>t1lst)w1=t1n-t1lst;
    //else w1=0;
    if(index>6){
      
      if(v_Wn_tg>v_W_M5000_tmp) {
        
          w1=v_t_I[index][0]-v_t_I[index-4][0];
          
          
          if(v_wn_tmp[index][0]>v_wn_tmp[index-4][0])w2=v_wn_tmp[index][0]-v_wn_tmp[index-4][0];
          else w2=1;
          
          w3=v_Wn_tg-v_wn_tmp[index][0];
          
          w1=(unsigned long int)w1*w3/w2;
          
          v_T1_pre[index][0]=w1;//+v_t_I[index][0];
          v_T1_pre[index+1][0]=0;
      } else w1=0;
    } else w1=2000;
      
    return w1;
    
}


/*
    �洢�������μ���ֵ
*/
void FS_1inj(void)
{
  //�������������
      v_Im_M51=v_Im_M51_tmp;//v_I_I[v_nb_IIndex_lst][1];
      
      v_Er_M5000=v_Er_M5000_tmp;//(dword)v_int_sqI_M5000*R_ST_M100/D_FOR_ER_M5000;
      
      v_L_M10000=v_L_M10000_tmp;
      
      v_Ein_M5000=v_Ein_M5000_tmp;//(((dword)v_Im_M51*v_Im_M51)*v_L_M10000)/D_FOR_EIN_M5000/2;
      
      v_Ef_M5000=v_Ef_M5000_tmp;//(((dword)v_Et_M5000*v_Et_M5000/D_FOR_EF_1_M5000)*KF_ST_M1000)/D_FOR_EF_2_M5000;
      
      v_En_M5000=v_En_M5000_tmp;       
  
      v_Is_M51=v_Is_M51_tmp;//=F_2DLookUpSword(T1S_24V,ARRAY_I_t,1);
      v_Ets_M5000=v_Ets_M5000_tmp;//F_2DLookUpSword(T1S_24V,ARRAY_E_t,1);
      
      v_ks_M256=v_ks_M256_tmp;//(dword)IS0_24V*256/v_Is_M51;
      
      v_Ens_M5000=v_Ens_M5000_tmp;//(unsigned long int)v_En_M5000*v_ks_M256/256;
      
      v_W_M5000=v_W_M5000_tmp;//v_Ens_M5000-v_En0_M5000; 
      
      v_int_sqI_M5000=v_int_sqI_M5000_tmp;
      v_Et_M5000=v_Et_M5000_tmp;
      
      v_Wn_r=v_W_M5000; //2015-2-2
      
      Ctl_Wn();         //2015-2-2,���һ������֮�󣬿�ʼ����T1������   
}

/*
    һ����������󣬻�ȡ�������������
*/

void FC_Energy_1Inj(word index,word nb_point)
{
    long int l_tmp;
    long int l_tmp_lst;
    
    byte blnCalE;
    
    
    byte i;
    
    if(!VF_IAD_BEBACKUP_AD){


          //if(v_no_lst_IAD_com>=v_nb_IIndex_lst) 
          {
              v_nb_IIndex_lst=index;//v_nb_IIndex;
              
              for(i=0;i<=v_nb_IIndex_lst;i++){
                  v_t_I[i][1]=v_t_I[i][0];    
                  //v_I_I[i][1]=v_I_I[i][0];    
                  //v_I_I[i][1]=v_I_I[i][0]*3/2; //2015-1-20 ����������5��ŷ������10/3   
                  v_I_I[i][1]=v_I_I[i][0]; //2015-1-23 ����������5��ŷ������3   
                  v_V_I[i][1]=v_V_I[i][0];
                  
                  v_T1_pre[i][1]=v_T1_pre[i][0];
                  
                  v_wn_tmp[i][1]=v_wn_tmp[i][0];
                  
                  blnCalE=false;
                  if(i==v_nb_IIndex_lst)blnCalE=true;
                  
                  Fint_Energy_1pint(i,1,blnCalE);
                     
              }

              //v_no_lst_IAD_com=0;
          }

          F_StartQtest_ForEnergy(QTEST_ENERGY_ACCU);
          VF_IAD_BEBACKUP_AD=true;
          
          if(nb_point==1)F_StartQtest_ForEnergy(QTEST_ENERGY_ED);
    }
  
    return;

}

/*
    ���ܣ���������ģ�ͣ����T1
*/
word F_T1Nxt_Wn(word mi,byte vbat)
{
    word t1;

    v_T1_lookup=1000;
    /*
        ��Ҫ����T1�Ĳ����
    
    */
    
    
                
    t1=v_T1_lookup+v_dT1_wn;
    
    //v_Wn_tg=1000+mi;
    
    return t1;   
}


/*
    ���ܣ�����������������õ�T1��ƫ��
    
    wn_r:ʵ��õ���wn
    vbat:��ѹ����
    
    �ڼ�����һ������֮�󣬿�ʼ����
*/
void Ctl_Wn(void)
{
    sword dWn;
    sword kp;
    sword ki;
    sword kd;
    
    dWn=(sword)v_Wn_tg_lst-v_Wn_r;
    
    ki=(dWn*M0_KI_M32)/32;
    if(dWn>1){
        ki++;
    } else if(dWn<-1){
        ki--;
    }
    
    kp=(dWn*M0_KP_M32)/32;
    if(dWn>4){
        kp++;
    } else if(dWn<-4){
        kp--;
    }
    
    kd=0;

    dT1_i+=ki;
    dT1_p=kp;
    dT1_d=kd;
    
    
    v_dT1_wn=dT1_p+dT1_i+ dT1_d;
        
}

//=================================================================================================

/*
    ���ܣ����Ŀ��Wn
    ���������������Wn��Ŀ��ֵ
*/
word FG_Wn_tg(word mi)
{
    
}

