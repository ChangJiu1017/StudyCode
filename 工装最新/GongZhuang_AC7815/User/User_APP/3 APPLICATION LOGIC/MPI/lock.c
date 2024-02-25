//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Public.h"

#include "sci.h"//2014-10-3
#include "status_st.h"
#include "ee.h"
#include "add_ee_mpi.h"
#include "add_flash_mpi.h"
#include "add_ram_MPI.h"

#include "lock.h"
#include "time_ccl.h"
#include "sys_init.h"     

//#include "data_inj_source.h"

//-------------2013-9-3 add --------------------------------
/*
byte u_code;
//byte u_code_lst;
byte cmdRX[2][BYTE_MAX];
byte v_index_rx_tmp;
///v_index_rx_tmp: 0 ��ʾcmdRX[0][30]���Խ�������;1 ��ʾcmdRX[1][30]���Խ�������;��Ӧ��һ���������ڱ���������;
byte v_num_rx_tmp;    //��ǰ���ջ�����
byte v_num_rx_decode;  //��Ҫ�����������ڵ��ֽڵĸ���

word t_rx_lst_4us;    //��һ�ν��յ��������ݵ�ʱ��
byte v_sta_cmd;       //����ͨѶģʽ�µ�״̬

byte v_nb_rx;//���յ�������

byte v_bchk;//2013-9-10
byte v_pw[6];//����
*/


byte v_t_run_s;//һ���ϵ���ۼƵ�Կ�׿�����ʱ��

void CheckPassword(word stADDin,word stADDstore,word flagAdd);//

word GetDate(word stAdd);
void Save_Lock(word EEADR_HEAD,byte n,byte m);
void Load_Lock(word EEADR_HEAD,byte n,byte m);

void F_Decode(void);//2013-9-4 decode
void F_dePW(void);

//#define T_FOR_RECHK_LOCK  (word)300
#define T_FOR_RECHK_LOCK  (word)120//2014-5-21 30S


#define ENBALE_PW true

//==================================================================================================
//==================================================================================================
//==================================================================================================


//Կ�׿���ʱ�ĳ�ʼ������Ҫ��ȡ״̬
void InitLockWhenKeyON(void)
{
    byte bdat;
    
    #if ENBALE_PW

        if(VF_POR_HARD){
            V_LOCK_STATUS=false;//2014-5-13 add 
            
            V_LOCK2_STATUS=false;
            v_t_run_s=0;
        }
        
        if(VF_VPOR_HARD) //2014-5-7 ��ʱ
        {
            
        }
        //ע�⣺VF_KEYOFF_SHORT_LOCK��ʼ��ǰ���˳�����ڰ���Կ�׵�ʱ���жϣ�
        //Ϊ��ֹ��һ�ο�Կ��ʱ������ʼ��ΪTRUE
        
        //2013-9-11
        if(nb_keyon_forIni>=2)VF_KEYON_FOR1STLOCK_LOCK=true;
        else VF_KEYON_FOR1STLOCK_LOCK=false;
        
        
        //������ϵ磬���߹�Կ��ʱ�乻���Ļ������¶�ȡ����״̬
        if(VF_VPOR_HARD)  //2013-9-11
        {
            if(!VF_KEYOFF_SHORT_LOCK) {
              
                VF_KEYOFF_SHORT_LOCK=true;// 2014-4-3
                V_LOCK_STATUS=false;
            } else{
                if(VF_KEYON_LOCK)V_LOCK_STATUS=0x02;//����Ѿ������ˣ��Ǳ��ֿ���״̬��
                else V_LOCK_STATUS=false;
            }

            VF_MODE_INTSCI=COMMODE_DIAG;   //2013-9-5
            
            /*
            if(DAT_PWS_INI!=Load_D_DK(EE_PWS_NOINI)){
                //�ж��Ƿ����û�����ù�����
                VF_PWNOINI_LOCK=true ;     
            }
            *///2014-6-24 DELETE
           
            //VF_KEYON_FOR1STLOCK_LOCK=true;//2014-4-1 ��ʱ
            //���û�п���������Ҫ��ȡ������״̬
            //if(!VF_KEYON_LOCK) //2014-5-23 DELETE THIS CONDICTION
            {
                bdat=Load_Key(EE_PWS_LOCK);//��ȡ������״̬
                
                if(bdat==DAT_LOCK){
                    VF_SETLOCK_LOCK=true;  
                    
                } else VF_KEYON_LOCK=true;
            }
            
            //��Ȩʹ�� �������� ��������  
            bdat=Load_Key(EE_PWL_LOCK);//��ȡ������״̬
            
            if(bdat==DAT_LOCK){
                //������Ϊ��Ȩʹ��
                if(VF_SETLOCK_LOCK){
                    //�Ѿ�����
                    VF_SETLIMIT_LOCK=true;    
                    
                    if(GetDate(CUR_YY)>=GetDate(ED_YY)){
                        //��Ȩ���ڽ���
                        VF_LIMITDUE_LOCK=true;
                    }
               
                } else{
                    //δ��������Ҫȡ����Ȩʹ�ñ�־
                    Save_Key(EE_PWL_LOCK,DAT_OTHER);
                }
            }
            /*
            */
            v_nb_rx=0;
            v_num_rx_tmp=0;
            
            VF_PWIN_LOCK=false;
            
        }

        //2014-4-7 ÿ�ο�Կ��ʱ�����������ٶ�һ��
        if(VF_VPOR_HARD){
            Load_Lock(EE_PWS_6,6,16);
            
            if(
                (Load_Key(EE_ACTIVE)==EE_ACTIVE_DAT)
                ||
                (BYPTR_M0_MODE_ACTIVE==MODE_ACTIVE_NOTACT)//2017-1-21
                )
                {
                VF_NOTACTIVE_LOCK=true;    
            } else VF_NOTACTIVE_LOCK=false;//2014-6-24 add 
        }
        
        
        //VF_NOTACTIVE_LOCK=false;//2016-8-31   2017-1-21 DELETE
        
        
        
        
        /*
        //����Կ�ף��������������״̬
        if(VF_VPOR_HARD){
            VF_BKEYERR_LOCK=VF_BPERMITERR_LOCK=false;    
        }
        
        */
        
        VF_PWNOINI_LOCK=true;
    #else
    
        if(VF_VPOR_HARD) //2014-5-7 ��ʱ
        {
            V_LOCK_STATUS=false;//2014-5-13 add 
            
            V_LOCK2_STATUS=false;
            v_t_run_s=0;
        }

        VF_KEYON_LOCK=true;
        v_nb_rx=0;
        v_num_rx_tmp=0;

        if(nb_keyon_forIni>=2)VF_KEYON_FOR1STLOCK_LOCK=true;
        else VF_KEYON_FOR1STLOCK_LOCK=false;
    
        VF_PWNOINI_LOCK=false;
    #endif
}

void ActionAtKeyoff(void){
    //����ʱ��
//    byte i;
    /*
    for(i=0;i<=2;i++){
        Save_Data_EE(LST_YY+i,Load_Data_EE(CUR_YY+i));
    }
    */
}

//Կ�׹ر�֮��Ķ���
void ActionWhenKeyoff(void){
    
    if(v_t_eng_t_s>T_FOR_RECHK_LOCK)VF_KEYOFF_SHORT_LOCK=false;  
    //else VF_KEYOFF_SHORT_LOCK=true;
}

//2014-5-8 add 
void ClrActive(void){
    Save_Key(EE_ACTIVE,EE_ACTIVE_DAT+10);
}

//У���������룺�������������ECU�ڲ��洢������Ա�
//ͬʱ��������Ȩʹ�ú�
void CheckPassword(word stADDin,word stADDstore,word flagAdd){
    
//    byte baddin0,baddin;
    
    
    //if(VF_PW_ISLOCK_LOCK)
   /*     
    datflag=Load_Data_EE(flagAdd) ;
        
    if(datflag==DAT_IN){
        //��������
        VF_KEYIN_NORMAL_LOCK=true;    
    } else if(datflag==DAT_UNLOCK){
        //��������ȡ����
        VF_UNLOCK_NORMAL_LOCK=true;
    }
    else if(datflag==DAT_LOCK){
        //����
        VF_LOCK_NORMAL_LOCK=true;
    }
    
    if(VF_KEYIN_NORMAL_LOCK||VF_UNLOCK_NORMAL_LOCK)
    {   //��������ͽ�������Ҫȷ�������Ƿ���ȷ
        VF_PWIN_NORMAL_LOCK=true;//����������Ķ�����
        for(i=0;i<=7;i++){
            
            baddin=Load_Data_EE((word)stADDin+i);
            baddin0=Load_Data_EE((word)stADDstore+i);
            
            if(baddin!=baddin0){
                //if(VF_KEYIN_NORMAL_LOCK)
                //if(VF_UNLOCK_NORMAL_LOCK)
                VF_KEYERR_NORMAL_LOCK=true;
                
                VF_KEYIN_NORMAL_LOCK=VF_UNLOCK_NORMAL_LOCK=false;  
            
            }
        
        }
    } else if(VF_LOCK_NORMAL_LOCK){
        //����
        VF_PWIN_NORMAL_LOCK=true;
        
    }
   */ 
}


//������ECU������صĹ���
void OperLock(void){


    //return;
    //V_LOCK2_STATUS=false;

    if(T_MAINTMR-T_FRAMEBREAK_4US>t_rx_lst_4us||v_num_rx_tmp>=BYTE_MAX-1){
        //���ν���֮�䳬��һ��ʱ�䣬����֡����
        
        //��С���ֽ���
        if(v_num_rx_tmp>=BYTE_MIN){
              
            v_index_rx_tmp=1-v_index_rx_tmp;
            v_num_rx_decode=v_num_rx_tmp;
            
            v_num_rx_tmp=0;
            VF_TOBE_DECODE_INTSCI=true;
        }
    }
    
    
    F_Decode();

    //2014-5-24 add 
    if(VF_KEYON_LOCK){
        v_t_run_s=0;
    }

   
}

//��ȡ����
word GetDate(word stAdd)
{
    word w;
    
    w=Load_Key(stAdd);   //yy
    
    w=w*16;
    
    w+=Load_Key(stAdd+5); //MM
    
    w*=32;
    
    w+=Load_Key(stAdd+10); //DD 
    
    return w;//2014-6-4 add ,֮ǰ���˷������ֵ
}


//
void FI_rx(void){
    v_num_rx_tmp=0;
    v_num_rx_decode=0;
    v_index_rx_tmp=0;
}


/*��APP ������������н���*/
void F_Decode(void)
{
    byte i;
    byte b_chk;
    byte blen;
    byte j;
    byte blnLogicErr;
    byte cmd;
    
    
    //if(VF_MODE_INTSCI==COMMODE_LOCK)
    //�������Ҫ���������
    if(VF_TOBE_DECODE_INTSCI){

        if(v_num_rx_decode>=5){
            v_nb_rx++;

            //���������ֽ����������3����
            if(cmdRX[1-v_index_rx_tmp][BYTE_LEN]<=v_num_rx_decode-BYTE_CMD){
                b_chk=1;
                
                blen=cmdRX[1-v_index_rx_tmp][BYTE_LEN];
                
                for(i=BYTE_LEN;i<=BYTE_CMD+blen-1;i++){ 
                    //����У���
                    b_chk+=cmdRX[1-v_index_rx_tmp][i];
                    
                }
                v_bchk=b_chk;
                
                blnLogicErr=false;
                if(b_chk==cmdRX[1-v_index_rx_tmp][BYTE_CMD+blen]){

                    //F_dePW();

                    //�ж�У���
                    cmd=cmdRX[1-v_index_rx_tmp][BYTE_CMD];//2014-5-31 �ݴ�
                    switch(cmd){
                          
                        case CMD_LOCK_SET:
                            F_dePW();//2014-4-1 ADD
                            VF_PWIN_LOCK=true;//2014-5-31
                            if(!VF_SETLOCK_LOCK)Save_Lock(EE_PWS_6,6,16);//2014-6-3
                             
                        case CMD_SET_PW_S:  //2014-3-31 add 
                            
                            //VF_KEYON_FOR1STLOCK_LOCK
                            //VF_PWNOINI_LOCK

                            //������ǰ�����Ѿ�����������ȡ����
                            /*
                            if(VF_KEYON_LOCK
                                &&
                                (!VF_PWNOINI_LOCK||VF_KEYON_FOR1STLOCK_LOCK)
                            ) */
                            
                            if(VF_KEYON_LOCK&&VF_PWIN_LOCK)//2014-5-31 add VF_PWIN_LOCK
                            {
                               // Save_Lock(EE_PWS_6,6,16);  //2014-6-3 �ѱ������������ŵ����������ǿ�
                                //
                                Save_Key(EE_PWS_LOCK,DAT_LOCK); 
                                
                                Save_Key(EE_PWS_NOINI,DAT_PWS_INI);               
                                
                                               
                                VF_SETLOCK_LOCK=true;
                            } else{
                                blnLogicErr=true;
                            }
                        break;

                        case CMD_INPUT_PW_S:
                            F_dePW();//2014-4-1 ADD 
                            VF_PWIN_LOCK=true;
                            if(!VF_SETLOCK_LOCK){
                                Save_Lock(EE_PWS_6,6,16);  //2014-6-3 �ѱ������������ŵ����������ǿ�    
                            }
                        break;
                            
                        case CMD_INPUT_PW_L:
                            F_dePW();//2014-4-1 ADD 
                            VF_PWIN_LOCK=true;
                            
                            if(VF_SETLOCK_LOCK&&!VF_SETLIMIT_LOCK) 
                            //��������ˣ���δ������Ȩ��
                            {
                              
                                Save_Lock(EE_PWL_6,6,16);  
                            }

                        break;

                        /*
                        case CMD_UNLOCK_SET:
                            F_dePW();//2014-4-1 ADD 
                            //���� 
                            VF_BKEYERR_LOCK=false;
                            j=0;
                            for(i=0;i<=5;i++){
                                if(v_pw[i]>=ASC_0&&v_pw[i]<=ASC_9){ //2014-4-7 ���ӶԷǷ�����Ĺ���
                                    if(v_pw[i]!=Load_Key(EE_PWS_6+j))
                                    {
                                        VF_BKEYERR_LOCK=true;
                                        break;
                                    }
                                }
                                j+=16;
                            }
                            
                            if(!VF_BKEYERR_LOCK)VF_KEYON_LOCK=true;
                            
                        break;
                        */
                        case CMD_UNLOCK_SET:         //�û��ճ��Ŀ�������
                        case CMD_CANCELLOCK_SET:     //2014-5-30 ����CMD_UNLOCK_SET����CMD_CANCELLOCK_SET���û���ȡ�����붯��
                        case CMD_CANCELLIMIT:        //�����̵�ȡ����Ȩ����
                            //ȡ����Ȩʹ������
                            //2014-5-24
                            F_dePW();//2014-4-1 ADD 
                            
                            //���� 
                            VF_BKEYERR_LOCK=false;
                            VF_BPERMITERR_LOCK=false;//2014-6-6
                            j=0;
                            
                            if(cmd==CMD_CANCELLIMIT)j=EE_DADR_L_TO_S;//2014-6-5
                            
                            for(i=0;i<=5;i++){
                                //if(v_pw[i]>=ASC_0&&v_pw[i]<=ASC_9) 2014-6-5 DELETE THIS CONDICTION
                                { //201//��ӶԷǷ�����Ĺ��?
                                    if(v_pw[i]!=Load_Key(EE_PWS_6+j))
                                    {
                                        VF_BKEYERR_LOCK=true;
                                        if(CMD_CANCELLIMIT==cmd)VF_BPERMITERR_LOCK=true;
                                        
                                        break;
                                    }
                                } //else{
                                  //  VF_BKEYERR_LOCK=true;
                                //}
                                j+=16;
                            }

                            if(!VF_BKEYERR_LOCK){
                            //�������û�д���
                                if(cmd==CMD_UNLOCK_SET)VF_KEYON_LOCK=true;
                                else if(cmd==CMD_CANCELLOCK_SET) {
                                    //ȡ������
                                    if(!VF_SETLIMIT_LOCK){
                                        Save_Key(EE_PWS_LOCK,DAT_OTHER);                
                                        VF_SETLOCK_LOCK=false;
                                        //VF_KEYON_LOCK=true;
                                        //VF_SETLIMIT_LOCK
                                    } else blnLogicErr=true;//2014-4-1
                                } else{
                                    VF_SETLIMIT_LOCK=false;
                                    Save_Key(EE_PWL_LOCK,DAT_OTHER);
                                    VF_LIMITDUE_LOCK=false;//2014-6-9 PERMIT 08 DUELIMIT 01                
                                }
                            }
                              
                        break;

                        case CMD_DISP_PWS_SET:
                            //��ʱ��
                            Load_Lock(EE_PWS_6,6,16);   //2014-5-24 DELETE
                            
                        break;
                            
                        case CMD_DELOCK_SET: //ǿ��ȡ������  2013-9-11
                            //  2014-5-24 DELETE
                            //��ʱ��
                            /*    2014-6-5 DELETE
                            VF_SETLOCK_LOCK=false;
                            VF_KEYON_LOCK=true;
                            Save_Key(EE_PWS_LOCK,DAT_OTHER);                
                             */  
                        break;
                        
                        case  CMD_INPUT_TIME:  //���뵱ǰʱ��
                            F_dePW();//2014-4-1 ADD 
                            Save_Lock(CUR_YY,3,5);
                            
                        break;
                        
                        case  CMD_SET_TIME:  //������Ȩʱ��
                            F_dePW();//2014-4-1 ADD 
                            Save_Lock(ED_YY,3,5); //2014-5-29 CHANGE FROM ST_YY TO ED_YY
                        break;
                        
                        case  CMD_QUERY_TIME://��ѯ��Ȩʱ��
                            //F_dePW();//2014-4-1 // DELETE AT 2014-5-31 
                            Load_Lock(ED_YY,3,5); //2014-5-29 CHANGE FROM ST_YY TO ED_YY
                        break;
                        
                        case  CMD_QUERY_CUR_TIME://2014-5-31 ADD 
                            Load_Lock(CUR_YY,3,5); 
                        break;
                        
                        case CMD_LIMIT_SET:
                            F_dePW();//2014-4-1 ADD
                            Save_Lock(EE_PWL_6,6,16);//2014-6-3 
                            VF_PWIN_LOCK=true;//2014-5-31
                            
                        case CMD_SET_PW_L: //2014-3-31 add 
                            //������Ȩʹ�õ�ǰ��ʱ���Ѿ�����
                            if(VF_SETLOCK_LOCK&&VF_PWIN_LOCK&&!VF_SETLIMIT_LOCK)
                            { //2014-5-31 add VF_PWIN_LOCK
                                VF_SETLIMIT_LOCK=true;
                                Save_Key(EE_PWL_LOCK,DAT_LOCK);//2014-6-3 ��������                
                                //Save_Lock(EE_PWL_6,6,16);
                                VF_PWIN_LOCK=false;

                            } else blnLogicErr=true;
                            
                        break;
                        
                        case CMD_DISP_PWL_SET://��ʾ����������

                            Load_Lock(EE_PWL_6,6,16);  
                        break;
                        
                        case CMD_DE_PWL_SET://ǿ��ȡ������������
                            //VF_SETLIMIT_LOCK=false;     //2014-6-19
                           // Save_Key(EE_PWL_LOCK,DAT_OTHER);                
                      
                        break;
                        
                        //�������ģʽ
                        case CMD_LOCKMODE_IN:
                            VF_MODE_INTSCI=COMMODE_LOCK;
                            u_code=T_MAINTMR;
                            REMARKW=MC_LOCK;//2014-3-31 ADD
                            v_PCKESE_SCI=0; 
                        break;
                        
                        //�˳�����ģʽ
                        case CMD_LOCKMODE_OUT:
                            VF_MODE_INTSCI=COMMODE_DIAG;
                            REMARKW=MC_NORMAL;
                            v_PCKESE_SCI=0;
                            
                        break;
           
                        //2014-4-7 ADD 
                        case CMD_REMARK_ADJ:  //0xa0
                            REMARKW=cmdRX[1-v_index_rx_tmp][BYTE_CMD+1];
                            
                        break;
                        case CMD_KESEI_ADJ:   //0xa1
                            v_PCKESE_SCI=cmdRX[1-v_index_rx_tmp][BYTE_CMD+1];
                        break;
                        case CMD_IGN_ADJ:     //0xa2
                            v_PCBTDC_SCI=cmdRX[1-v_index_rx_tmp][BYTE_CMD+1];
                        break;
                        case CMD_T1_ADJ:      //0xa3
                            v_PCTBACK_SCI=cmdRX[1-v_index_rx_tmp][BYTE_CMD+1];
                        break;
                        case CMD_DTC_ADJ:     //0xa4
                            PCCQJSJD=cmdRX[1-v_index_rx_tmp][BYTE_CMD+1];
                        break;
                        default:
                        break; 
                    }
                    
                    //2014-4-1 ���Ӷ��߼�������ж�
                    if(blnLogicErr){
                        if(v_sta_cmd==CMD_LOG_ERR)v_sta_cmd=CMD_LOG_ERR2;
                        else v_sta_cmd=CMD_LOG_ERR;
                    } else {
                        v_sta_cmd=cmd;
                        
                    }
                } else {
                  if(v_sta_cmd==CMD_SUM_ERR)v_sta_cmd=CMD_SUM_ERR2;
                  else v_sta_cmd=CMD_SUM_ERR;
                }
            } else {
                  if(v_sta_cmd==CMD_LEN_ERR)v_sta_cmd=CMD_LEN_ERR2;
                  else v_sta_cmd=CMD_LEN_ERR;
            }
        } else{
                  if(v_sta_cmd==CMD_SUM_ERR)v_sta_cmd=CMD_SUM_ERR2;
                  else v_sta_cmd=CMD_SUM_ERR;
        }
        VF_TOBE_DECODE_INTSCI=false;
    }    
}

//��������
void F_dePW(void){
     byte i;
     
     //VF_PWIN_LOCK=false;
     //��������
     for(i=0;i<=5;i++){
        //cmdRX[1-v_index_rx_tmp][BYTE_CMD+i];
        
        v_pw[i]=cmdRX[1-v_index_rx_tmp][BYTE_CMD+i+1]^((byte)(u_code+i));//2013-9-11 add +1
        //cmdRX[1-v_index_rx_tmp][BYTE_CMD+2*i+1];
           
     }
}

/*����ʱ��
    ʱ��Ĵ洢��ʽ��
            Y,YB,M,MB,D,DB
            �׵�ַ֮��������6����ַ
    ����Ĵ洢��ʽ��
            ��λ��λ��ÿһλ��һ�����ݵģ�����12����ַ
    
    n:�����ַ�����m������ÿ���ַ��׵�ַ����һ�ַ��׵�ַ������
*/
void Save_Lock(word EEADR_HEAD,byte n,byte m){
    byte i;
    byte j;
    
    /*
    for(i=0;i<=n;i++){
        j=i*2;
        Save_Data_EE(EEADR_HEAD+j,v_pw[i]);    
        Save_Data_EE(EEADR_HEAD+j+1,v_pw[i]+1);    
    }
    */
    j=0;
    //for(i=0;i<=n;i++)
    for(i=0;i<n;i++)
    {
        Save_Key(EEADR_HEAD+j,v_pw[i]);
        j+=m;
    }
}

void Load_Lock(word EEADR_HEAD,byte n,byte m){
    byte i;
    byte j;
    
    j=0;
    //for(i=0;i<=n;i++)
    //2014-5-7 ��������
    for(i=0;i<n;i++)
    {
        v_pw[i]=Load_Key(EEADR_HEAD+j);
        j+=m;
    }
    /*    
    for(i=0;i<=n;i++){
        j=i*2;
        v_pw[i]=Load_D_DK(EEADR_HEAD+j);    
    } */
    
}

void SetPwDefault(word ADD_PW)
{
    byte i;
    
    for(i=0;i<=5;i++) {
        v_pw[i]=ASC_0;
    }
    
    Save_Lock(ADD_PW,6,16);
}
