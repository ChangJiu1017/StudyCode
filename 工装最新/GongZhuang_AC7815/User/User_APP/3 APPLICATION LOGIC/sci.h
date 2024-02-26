/*===========================================================*/
/*====SCI模块=====================*/
/*===========================================================*/

//功能：SCI模块的初始化
//#pragma CODE_SEG MAX_ROM
    extern void TOPC(void) ;
//#pragma CODE_SEG DEFAULT

extern void Get_Tback_for_CAB(sbyte cmd);//2012-10-22
extern void F_CLR_WDG(void);
extern byte TX_DATA[28]; //2008-5-29 change from 20 to 30 
extern word t_Cal_1cycle;//mcu计算一圈需要的时间；//2008-8-10
extern void Get_Kesei_for_CAB(sbyte cmd,sbyte cmd_w);
extern void FI_SCI_AfterReset(void);
extern void FI_AfterInjAction_SCI_AfterReset(void);
extern byte v_NB_TX; 
extern byte v_NB_TX_max;
//extern word t_1cycle_remain_4us;  //2009-12-24 一圈所需要的时间
//extern word t_1cycle_remain_min;  //after leave idle

extern byte v_num_rx_tmp;    //当前接收缓冲区 2014-3-31

extern const byte   MS_PC_SET_SELB       ;//   =       4;//pc_set_sel的初始值//2014-9-25 move from	data_sys_strategy.h		

extern byte PCCQJSJD;
extern sbyte v_PCTBACK_SCI;																		
extern sbyte v_PCBTDC_SCI;																							
extern sbyte v_PCKESE_SCI;
extern sbyte cmdAdjT1; //2012-10-22
extern sbyte cmdAdjIGN; //2012-10-22
extern sbyte cmdAdjCOS; //2012-10-22
extern sbyte cmdAdjO; //2014-8-25;


extern void Adj_para(byte idx,sbyte step);
extern word v_PCSET_SCI_TOCOM;
extern sbyte v_PCKESE_SCI_TOCOM;
extern sbyte v_PCBTDC_SCI_TOCOM;
extern sbyte v_PCTBACK_SCI_TOCOM;


//para to be adjusted for sys_pi
    #define	ADJ_ldo	1   //v_Adj[ADJ_mpa]
    #define	ADJ_mpa	2
    #define	ADJ_T	3
    #define	ADJ_ss	4
    #define	ADJ_us	5
    #define	ADJ_f0	6
    #define	ADJ_E	7
    #define	ADJ_F	8
    #define	ADJ_G	9
    #define	ADJ_g	10
    #define	ADJ_X	11
    #define	ADJ_Y	12
    #define	ADJ_Z	13
    #define	ADJ_R	14
    #define	ADJ_UP	15
    #define	ADJ_UA	16
    #define	ADJ_UR	17
    #define	ADJ_B	18
    #define	ADJ_C	19
    #define	ADJ_D	20
    #define	ADJ_P_ACC	21
    #define	ADJ_P_DEC	22
    #define	ADJ_S	23
    #define	ADJ_L_ACC	24
    #define	ADJ_L_DEC	25
    
    #define	ADJ_Igb	26
    #define	ADJ_dIgnO	27
    #define	ADJ_igs	28
    
    #define	ADJ_M	29
    #define	ADJ_O	30
    
    #define	ADJ_n0_high	31
    #define	ADJ_n0_low	32
    
    #define	ADJ_k	33
    #define	ADJ_j	34
    #define	ADJ_nfull	35
    #define	ADJ_nstand	36
    #define	ADJ_kp	37
    #define	ADJ_ki	38
    #define	ADJ_kd	39
    
    #define	ADJ_TPDtg	40//2015-12-16 add for tpd control
	#define ADJ_KI_IACV	41
	#define ADJ_KP_IACV	42	
	#define	ADJ_T1	43
	#define	ADJ_STEPBASE	44

    #define ADJ_NB 45
//2015-10-26 define for SYS_PI
//extern sbyte v_Adj[40];
extern sbyte v_Adj[ADJ_NB];

extern byte Adj_byte(byte ori,byte index); 
extern sbyte Adj_sbyte(sbyte ori,byte index); 
extern word Adj_word(word ori,byte index); 
extern sword Adj_sword(sword ori,byte index); 



#define REMARK (byte)(v_PCSET_SCI_TOCOM&0xff)
#define REMARKH (byte)((v_PCSET_SCI_TOCOM>>8)&0xff)    //2009-3-3 ADD 

#define REMARKW v_PCSET_SCI    //2012-10-31
#define KESEI v_PCKESE_SCI_TOCOM
#define PCBTDC v_PCBTDC_SCI_TOCOM
#define PCTBACK v_PCTBACK_SCI_TOCOM


#define BYTE_CMD    3
#define BYTE_LEN    2

#define BYTE_B1     1
#define BYTE_B0     0

#define BYTE_MAX    12

#define BYTE_MIN    5

//20210923 chenwei add
extern word test_20210923;
extern word v_pcode;
extern word v_t_MILGliter_d1s;
extern byte TEST_b1,TEST_b2,TEST_b3,TEST_b4;
extern byte test_2021092301;
extern byte test_2021092302;
extern 	byte VF_MIL_DTC4;
extern 	byte VF_MIL_DTC3;
extern 	byte VF_MIL_DTC2;
extern 	byte VF_MIL_DTC1;
extern word v_pcode_lst;
extern word v_pcode_lookup;
extern byte v_t_fromkeyon_1s;
extern byte v_n_SPN_ng;
extern word test_20210925;

extern word v_t_IgnOn_LookUp;
extern word v_t_IgnOn;
extern word test_20210926;
extern word test_20210602;
extern byte VF_OPENLOOP_IACV;
extern sword v_step_sm;
extern word test_2021093001;
extern sword test_2021093002;
extern byte v_t_afterstarts;

//20211004 chenwei add for test
extern word test_2021100401;
extern word test_2021100402;
extern word test_2021100403;
extern word test_2021100404;
extern byte test_2021100405;