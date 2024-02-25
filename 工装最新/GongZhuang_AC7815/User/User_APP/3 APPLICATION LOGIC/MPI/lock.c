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
///v_index_rx_tmp: 0 表示cmdRX[0][30]可以接收数据;1 表示cmdRX[1][30]可以接收数据;对应另一个就是用于被解析的了;
byte v_num_rx_tmp;    //当前接收缓冲区
byte v_num_rx_decode;  //需要解析的数组内的字节的个数

word t_rx_lst_4us;    //上一次接收到串口数据的时间
byte v_sta_cmd;       //防盗通讯模式下的状态

byte v_nb_rx;//接收到的数量

byte v_bchk;//2013-9-10
byte v_pw[6];//密码
*/


byte v_t_run_s;//一次上电后累计的钥匙开启的时间

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


//钥匙开启时的初始化，主要读取状态
void InitLockWhenKeyON(void)
{
    byte bdat;
    
    #if ENBALE_PW

        if(VF_POR_HARD){
            V_LOCK_STATUS=false;//2014-5-13 add 
            
            V_LOCK2_STATUS=false;
            v_t_run_s=0;
        }
        
        if(VF_VPOR_HARD) //2014-5-7 临时
        {
            
        }
        //注意：VF_KEYOFF_SHORT_LOCK初始化前后的顺序，由于按关钥匙的时间判断，
        //为防止第一次开钥匙时，被初始化为TRUE
        
        //2013-9-11
        if(nb_keyon_forIni>=2)VF_KEYON_FOR1STLOCK_LOCK=true;
        else VF_KEYON_FOR1STLOCK_LOCK=false;
        
        
        //如果是上电，或者关钥匙时间够长的话，重新读取上锁状态
        if(VF_VPOR_HARD)  //2013-9-11
        {
            if(!VF_KEYOFF_SHORT_LOCK) {
              
                VF_KEYOFF_SHORT_LOCK=true;// 2014-4-3
                V_LOCK_STATUS=false;
            } else{
                if(VF_KEYON_LOCK)V_LOCK_STATUS=0x02;//如果已经开锁了，那保持开锁状态；
                else V_LOCK_STATUS=false;
            }

            VF_MODE_INTSCI=COMMODE_DIAG;   //2013-9-5
            
            /*
            if(DAT_PWS_INI!=Load_D_DK(EE_PWS_NOINI)){
                //判读是否从来没有设置过密码
                VF_PWNOINI_LOCK=true ;     
            }
            *///2014-6-24 DELETE
           
            //VF_KEYON_FOR1STLOCK_LOCK=true;//2014-4-1 临时
            //如果没有开锁，则需要读取上锁的状态
            //if(!VF_KEYON_LOCK) //2014-5-23 DELETE THIS CONDICTION
            {
                bdat=Load_Key(EE_PWS_LOCK);//读取上锁的状态
                
                if(bdat==DAT_LOCK){
                    VF_SETLOCK_LOCK=true;  
                    
                } else VF_KEYON_LOCK=true;
            }
            
            //授权使用 ，必须是 先上锁的  
            bdat=Load_Key(EE_PWL_LOCK);//读取上锁的状态
            
            if(bdat==DAT_LOCK){
                //被设置为授权使用
                if(VF_SETLOCK_LOCK){
                    //已经上锁
                    VF_SETLIMIT_LOCK=true;    
                    
                    if(GetDate(CUR_YY)>=GetDate(ED_YY)){
                        //授权日期结束
                        VF_LIMITDUE_LOCK=true;
                    }
               
                } else{
                    //未上锁，需要取消授权使用标志
                    Save_Key(EE_PWL_LOCK,DAT_OTHER);
                }
            }
            /*
            */
            v_nb_rx=0;
            v_num_rx_tmp=0;
            
            VF_PWIN_LOCK=false;
            
        }

        //2014-4-7 每次开钥匙时，都把密码再读一下
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
        //关了钥匙，就清除密码错误的状态
        if(VF_VPOR_HARD){
            VF_BKEYERR_LOCK=VF_BPERMITERR_LOCK=false;    
        }
        
        */
        
        VF_PWNOINI_LOCK=true;
    #else
    
        if(VF_VPOR_HARD) //2014-5-7 临时
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
    //保存时间
//    byte i;
    /*
    for(i=0;i<=2;i++){
        Save_Data_EE(LST_YY+i,Load_Data_EE(CUR_YY+i));
    }
    */
}

//钥匙关闭之后的动作
void ActionWhenKeyoff(void){
    
    if(v_t_eng_t_s>T_FOR_RECHK_LOCK)VF_KEYOFF_SHORT_LOCK=false;  
    //else VF_KEYOFF_SHORT_LOCK=true;
}

//2014-5-8 add 
void ClrActive(void){
    Save_Key(EE_ACTIVE,EE_ACTIVE_DAT+10);
}

//校验密码输入：将输入的密码与ECU内部存储的密码对比
//同时适用于授权使用和
void CheckPassword(word stADDin,word stADDstore,word flagAdd){
    
//    byte baddin0,baddin;
    
    
    //if(VF_PW_ISLOCK_LOCK)
   /*     
    datflag=Load_Data_EE(flagAdd) ;
        
    if(datflag==DAT_IN){
        //输入密码
        VF_KEYIN_NORMAL_LOCK=true;    
    } else if(datflag==DAT_UNLOCK){
        //解锁，即取消锁
        VF_UNLOCK_NORMAL_LOCK=true;
    }
    else if(datflag==DAT_LOCK){
        //上锁
        VF_LOCK_NORMAL_LOCK=true;
    }
    
    if(VF_KEYIN_NORMAL_LOCK||VF_UNLOCK_NORMAL_LOCK)
    {   //输入密码和解锁都需要确认密码是否正确
        VF_PWIN_NORMAL_LOCK=true;//有密码输入的动作，
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
        //上锁
        VF_PWIN_NORMAL_LOCK=true;
        
    }
   */ 
}


//进行与ECU上锁相关的工作
void OperLock(void){


    //return;
    //V_LOCK2_STATUS=false;

    if(T_MAINTMR-T_FRAMEBREAK_4US>t_rx_lst_4us||v_num_rx_tmp>=BYTE_MAX-1){
        //两次接收之间超过一定时间，代表帧结束
        
        //最小的字节数
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

//获取日期
word GetDate(word stAdd)
{
    word w;
    
    w=Load_Key(stAdd);   //yy
    
    w=w*16;
    
    w+=Load_Key(stAdd+5); //MM
    
    w*=32;
    
    w+=Load_Key(stAdd+10); //DD 
    
    return w;//2014-6-4 add ,之前忘了返回这个值
}


//
void FI_rx(void){
    v_num_rx_tmp=0;
    v_num_rx_decode=0;
    v_index_rx_tmp=0;
}


/*对APP 的下行命令进行解析*/
void F_Decode(void)
{
    byte i;
    byte b_chk;
    byte blen;
    byte j;
    byte blnLogicErr;
    byte cmd;
    
    
    //if(VF_MODE_INTSCI==COMMODE_LOCK)
    //如果有需要处理的命令
    if(VF_TOBE_DECODE_INTSCI){

        if(v_num_rx_decode>=5){
            v_nb_rx++;

            //待解析的字节数必须大于3个，
            if(cmdRX[1-v_index_rx_tmp][BYTE_LEN]<=v_num_rx_decode-BYTE_CMD){
                b_chk=1;
                
                blen=cmdRX[1-v_index_rx_tmp][BYTE_LEN];
                
                for(i=BYTE_LEN;i<=BYTE_CMD+blen-1;i++){ 
                    //计算校验和
                    b_chk+=cmdRX[1-v_index_rx_tmp][i];
                    
                }
                v_bchk=b_chk;
                
                blnLogicErr=false;
                if(b_chk==cmdRX[1-v_index_rx_tmp][BYTE_CMD+blen]){

                    //F_dePW();

                    //判断校验和
                    cmd=cmdRX[1-v_index_rx_tmp][BYTE_CMD];//2014-5-31 暂存
                    switch(cmd){
                          
                        case CMD_LOCK_SET:
                            F_dePW();//2014-4-1 ADD
                            VF_PWIN_LOCK=true;//2014-5-31
                            if(!VF_SETLOCK_LOCK)Save_Lock(EE_PWS_6,6,16);//2014-6-3
                             
                        case CMD_SET_PW_S:  //2014-3-31 add 
                            
                            //VF_KEYON_FOR1STLOCK_LOCK
                            //VF_PWNOINI_LOCK

                            //上锁，前提是已经开锁，或者取消锁
                            /*
                            if(VF_KEYON_LOCK
                                &&
                                (!VF_PWNOINI_LOCK||VF_KEYON_FOR1STLOCK_LOCK)
                            ) */
                            
                            if(VF_KEYON_LOCK&&VF_PWIN_LOCK)//2014-5-31 add VF_PWIN_LOCK
                            {
                               // Save_Lock(EE_PWS_6,6,16);  //2014-6-3 把保存密码的命令放到密码输入那块
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
                                Save_Lock(EE_PWS_6,6,16);  //2014-6-3 把保存密码的命令放到密码输入那块    
                            }
                        break;
                            
                        case CMD_INPUT_PW_L:
                            F_dePW();//2014-4-1 ADD 
                            VF_PWIN_LOCK=true;
                            
                            if(VF_SETLOCK_LOCK&&!VF_SETLIMIT_LOCK) 
                            //如果上锁了，且未设置授权，
                            {
                              
                                Save_Lock(EE_PWL_6,6,16);  
                            }

                        break;

                        /*
                        case CMD_UNLOCK_SET:
                            F_dePW();//2014-4-1 ADD 
                            //开锁 
                            VF_BKEYERR_LOCK=false;
                            j=0;
                            for(i=0;i<=5;i++){
                                if(v_pw[i]>=ASC_0&&v_pw[i]<=ASC_9){ //2014-4-7 增加对非法密码的过滤
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
                        case CMD_UNLOCK_SET:         //用户日常的开锁动作
                        case CMD_CANCELLOCK_SET:     //2014-5-30 ，将CMD_UNLOCK_SET并入CMD_CANCELLOCK_SET，用户的取消密码动作
                        case CMD_CANCELLIMIT:        //经销商的取消授权动作
                            //取消授权使用限制
                            //2014-5-24
                            F_dePW();//2014-4-1 ADD 
                            
                            //开锁 
                            VF_BKEYERR_LOCK=false;
                            VF_BPERMITERR_LOCK=false;//2014-6-6
                            j=0;
                            
                            if(cmd==CMD_CANCELLIMIT)j=EE_DADR_L_TO_S;//2014-6-5
                            
                            for(i=0;i<=5;i++){
                                //if(v_pw[i]>=ASC_0&&v_pw[i]<=ASC_9) 2014-6-5 DELETE THIS CONDICTION
                                { //201//黾佣苑欠苈氲墓?
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
                            //如果密码没有错误
                                if(cmd==CMD_UNLOCK_SET)VF_KEYON_LOCK=true;
                                else if(cmd==CMD_CANCELLOCK_SET) {
                                    //取消密码
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
                            //临时的
                            Load_Lock(EE_PWS_6,6,16);   //2014-5-24 DELETE
                            
                        break;
                            
                        case CMD_DELOCK_SET: //强制取消密码  2013-9-11
                            //  2014-5-24 DELETE
                            //临时的
                            /*    2014-6-5 DELETE
                            VF_SETLOCK_LOCK=false;
                            VF_KEYON_LOCK=true;
                            Save_Key(EE_PWS_LOCK,DAT_OTHER);                
                             */  
                        break;
                        
                        case  CMD_INPUT_TIME:  //输入当前时间
                            F_dePW();//2014-4-1 ADD 
                            Save_Lock(CUR_YY,3,5);
                            
                        break;
                        
                        case  CMD_SET_TIME:  //设置授权时间
                            F_dePW();//2014-4-1 ADD 
                            Save_Lock(ED_YY,3,5); //2014-5-29 CHANGE FROM ST_YY TO ED_YY
                        break;
                        
                        case  CMD_QUERY_TIME://查询授权时间
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
                            //设置授权使用的前提时，已经上锁
                            if(VF_SETLOCK_LOCK&&VF_PWIN_LOCK&&!VF_SETLIMIT_LOCK)
                            { //2014-5-31 add VF_PWIN_LOCK
                                VF_SETLIMIT_LOCK=true;
                                Save_Key(EE_PWL_LOCK,DAT_LOCK);//2014-6-3 设置密码                
                                //Save_Lock(EE_PWL_6,6,16);
                                VF_PWIN_LOCK=false;

                            } else blnLogicErr=true;
                            
                        break;
                        
                        case CMD_DISP_PWL_SET://显示经销商密码

                            Load_Lock(EE_PWL_6,6,16);  
                        break;
                        
                        case CMD_DE_PWL_SET://强制取消经销商密码
                            //VF_SETLIMIT_LOCK=false;     //2014-6-19
                           // Save_Key(EE_PWL_LOCK,DAT_OTHER);                
                      
                        break;
                        
                        //进入防盗模式
                        case CMD_LOCKMODE_IN:
                            VF_MODE_INTSCI=COMMODE_LOCK;
                            u_code=T_MAINTMR;
                            REMARKW=MC_LOCK;//2014-3-31 ADD
                            v_PCKESE_SCI=0; 
                        break;
                        
                        //退出防盗模式
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
                    
                    //2014-4-1 增加对逻辑错误的判断
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

//解析密码
void F_dePW(void){
     byte i;
     
     //VF_PWIN_LOCK=false;
     //解析密码
     for(i=0;i<=5;i++){
        //cmdRX[1-v_index_rx_tmp][BYTE_CMD+i];
        
        v_pw[i]=cmdRX[1-v_index_rx_tmp][BYTE_CMD+i+1]^((byte)(u_code+i));//2013-9-11 add +1
        //cmdRX[1-v_index_rx_tmp][BYTE_CMD+2*i+1];
           
     }
}

/*保存时间
    时间的存储方式：
            Y,YB,M,MB,D,DB
            首地址之后，连续的6个地址
    密码的存储方式：
            高位低位，每一位有一个备份的，连续12个地址
    
    n:代表字符数；m：代表每个字符首地址与上一字符首地址的增量
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
    //2014-5-7 更改条件
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
