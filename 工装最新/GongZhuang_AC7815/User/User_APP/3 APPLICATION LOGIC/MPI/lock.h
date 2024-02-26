
#include "sci.h"
//2013-6-14
extern void OperLock(void);
extern void ActionWhenKeyoff(void);
extern void InitLockWhenKeyON(void);

extern void ActionAtKeyoff(void);

extern void ClrActive(void);//2014-5-8 add

extern void SetPwDefault(word ADD_PW);//2014-5-31 add 

//
#define REMARK_CLRACT 251
#define KESEI_CLRACT  -6

//2013-9-3 与防盗相关的
#define FRAME_B0 0xa0
#define FRAME_B1 0xa2

//TPS位置，返回的状态位的含义
#define CMD_LOCK_SET      0xf1//设置密码  
#define CMD_CANCELLOCK_SET    0xf2//取消密码
#define CMD_UNLOCK_SET    0xf3//解锁  

#define CMD_INPUT_PW_S      0xf6//输入用户密码  
#define CMD_SET_PW_S      0xf8//写入已输入的用户密码  


#define CMD_DISP_PWS_SET    0xfe//显示用户密码
#define CMD_DELOCK_SET    0xff//强制取消密码

#define CMD_LIMIT_SET     0xe2  //设置授权使用
#define CMD_CANCELLIMIT   0xe3  //取消授权使用
#define CMD_INPUT_TIME    0xe4  //输入当前时间
#define CMD_SET_TIME          0xe5     //设置授权时间
#define CMD_QUERY_TIME        0xe7   //查询授权时间
//#define CMD_ED_TIME       0xe6  //设置授权截至时间

#define CMD_QUERY_CUR_TIME        0xe9   //查询当前时间，2014-5-31

#define CMD_INPUT_PW_L      0xE6//输入经销商密码  
#define CMD_SET_PW_L      0xE8//写入已输入的经销商密码  

#define CMD_DISP_PWL_SET  0xee//显示经销商密码
#define CMD_DE_PWL_SET    0xef//强制取消经销商密码


#define CMD_LOCKMODE_IN   0xb0  //进入防盗通讯模式
#define CMD_LOCKMODE_OUT  0xc0  //退出防盗通讯模式

#define CMD_LEN_ERR 0x11     //帧长有误
#define CMD_SUM_ERR 0x12     //校验有误

#define CMD_LEN_ERR2 0x13     //帧长有误
#define CMD_SUM_ERR2 0x14     //校验有误

#define CMD_LOG_ERR 0x15     //ECU发来命令有逻辑错误
#define CMD_LOG_ERR2 0x16     //ECU发来命令有逻辑错误

#define CMD_INJON_ERR 0x17     //喷油器在工作

#define CMD_REMARK_ADJ  0xa0
#define CMD_KESEI_ADJ   0xa1
#define CMD_IGN_ADJ     0xa2
#define CMD_T1_ADJ      0xa3
#define CMD_DTC_ADJ     0xa4

#define ASC_0 48
#define ASC_9 57


#define OS_LOCK     254
#define TES_LOCK    253
#define REMARK_LOCK 32

#define T_FRAMEBREAK_4US 2500//5ms   //2013-9-10 FROM 125 TO 1250


#define LOCK_ENBALE true    //2013-9-16

//-------------2013-9-3 add --------------------------------
extern byte u_code;
//extern byte u_code_lst;
extern byte cmdRX[2][BYTE_MAX];
extern byte v_index_rx_tmp;
///v_index_rx_tmp: 0 表示cmdRX[0][30]可以接收数据;1 表示cmdRX[1][30]可以接收数据;对应另一个就是用于被解析的了;
extern byte v_num_rx_tmp;    //当前接收缓冲区
extern byte v_num_rx_decode;  //需要解析的数组内的字节的个数

extern word t_rx_lst_4us;    //上一次接收到串口数据的时间
extern byte v_sta_cmd;       //防盗通讯模式下的状态

extern byte v_nb_rx;//接收到的数量
extern byte v_bchk;//2013-9-10

extern byte v_pw[6];//密码

extern byte v_t_run_s;//一次上电后累计的钥匙开启的时间,2014-4-27

//2014-3-27
//#define EE_ADR_PWS_HEAD 0x300// 用户密码首地址，6个字节按顺序首地址+16，每个字节连续存5个数，首个为密码ASC码值，后面的一次-1；


