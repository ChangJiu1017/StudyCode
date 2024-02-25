/*本头文件是对系统公用宏的声明*/

//以下为与车型无关的系统参数
//---------for motor------------------------------------------------

/*===========================================================================================*/
/*============================周期的定义=====================================================*/
/*===========================================================================================*/
#define PCAPER_RUN_19D2K 0x1a       //当转速大于该值之后，每两圈通讯一2250rpm
#define PCAH2929_IGN    0x14//用于立即点火
#define PCA1650         0x23
#define PCA_MAX_IDLE    0x20//2008-8-13   EQU     0X20            ;当RPM>1831,作为非正常怠速,反馈对称
#define PCA1831	        0x20//EQU	20H				
#define PCA2092     	  0x1c//EQU	1CH	
#define PCA2343	        0x19//EQU 0x19//	19H											
#define PCA2400         0x18//  
#define PCA2400_2092    0x4	

#define PCA2092_1650    0x7	
			
#define PCA3255	        0x12//EQU	12H		
#define PCA3662	        0x10//EQU	10H		
#define PCA900	        0x41//equ	41h
#define PCA2929	        0x14//equ	14h
#define PCA1500	        0x27//equ	27h	;rpm=1500	与后沿相关		
#define PCA3907H        0x0e//	EQU	0EH	
#define PCA3900         0x0f	
#define PCA2000	        0x1d//;when	rpm<2000,	add	an	injecting	if	rpm	falling	more	than	125	rpm	during	two	run
#define PCA4508	        0x0d	
#define PCA6500         0x09    //6500rpm，0x900	
                                //6600rpm，0x8e0
#define PCA700	        0x53//equ	53h
#define PCA_TOIDLE_NOOIL 0x19//	equ	0x19	;when	to	idle	stop	oil	when	rpm>2253	

#define PCA5800	        0x0A	

/*===========================================================================================*/
/*============================转速的定义=====================================================*/
/*===========================================================================================*/
#define REV62D5_9250  148
#define REV62D5_9000  144

#define REV62D5_8500  136
#define REV62D5_8000  128
#define REV62D5_7000  112
#define REV62D5_6000  96
#define REV62D5_5750  92
#define REV62D5_5500  88
#define REV62D5_5250  84
#define REV62D5_5000  80
#define REV62D5_4750  76
#define REV62D5_4500  72
#define REV62D5_4250  68
#define REV62D5_4000  64 
#define REV62D5_3500  56
#define REV62D5_3000  48  
#define REV62D5_2500  40
#define REV62D5_2250  36
#define REV62D5_2000  32
#define REV62D5_1815  29
#define REV62D5_1750  28
#define REV62D5_1250  20
#define REV62D5_1000  16

#define REV62D5_CALLD_LOW  REV62D5_2000 
#define REV62D5_CALLD_HIGH REV62D5_2500

#define REV62D5_ADDT35     REV62D5_3000

#define REV62D5_ADD1INJ    REV62D5_5000//2020-5-8 共轨系统两次喷射是否还要保留，或者转速是否需要降低？
#define REV62D5_ADD2INJ    REV62D5_1000//REV62D5_2500//REV62D5_4000//2014-12-9 from 2250 to 4000  2017-3-11
#define REV62D5_ADD3INJ    REV62D5_1000//2020-5-8 共轨仍然保持较多，因为共轨不需要这么多次喷射

//#define REV62D5_MODE1  REV62D5_5250
//#define REV62D5_MODE2  REV62D5_5750

#define REV62D5_MODE1  REV62D5_2000//REV62D5_5250
#define REV62D5_MODE2  REV62D5_2500//REV62D5_5750


/*===========================================================================================*/
/*============================排量的定义=====================================================*/
/*===========================================================================================*/
#define PL50    50
#define PL125   125
#define PL110   110
#define PL150   150
#define PL200   200
#define PL175   175
#define PL100   100
#define PL133   133


#define NB_HYERR      3

//－－－复位原因定义－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
#define RST_BY_KEYOFF         95
#define RST_BY_STOP_CA        97
#define RST_TOMAINLOOP        99
#define RST_BY_STOP_LEAVEMAIN 93
#define RST_BY_STOP_LEAVEMAIN_OV 91
#define RST_BY_STOP_WAITCA   89

#define RST_BY_STOP_IGN     11
#define RST_BY_TOOSPEED     13
#define RST_BY_STOP_CA_HY   19
#define RST_BY_ERR_CA 23
#define RST_BY_STOP_CA_2ND  29
#define RST_BY_STOP_CA_2ND_HY   39
#define RST_BY_INJ_EXPECTION    25
#define RST_BY_INJ_EXPECTION2    27

//程序运行位置-------------------------------------------------------------------
#define POS_STOP_IDLEINI    99
#define POS_STOP_GETCLS     101
#define POS_STOP_NOTGET     100

#define POS_KEYOFF_IDLEINI  199
#define POS_KEYOFF_GETCLS   201
#define POS_KEYOFF_NOTGET   200

#define POS_KEYOFF_INMAIN   190
#define POS_PCAOV_INMAIN    191
#define POS_PCAOV_OUTMAIN   189

#define POS_KEYON_BEFORE    170
#define POS_KEYON_AFTER     171
#define POS_KEYOFF_WAIT1STCA    160
#define POS_KEYOFF_WAIT2NDCA    159
#define POS_KEYON_NOTGT2    158

#define POS_INI_SYS             80
#define POS_INI_INJCTL          70
#define POS_INI_BEFORESCAN      71

#define POS_INI_PN              60
#define POS_INI_PN_PUMP         61
#define POS_INI_PN_DETECTOPEN   69
#define  NB_DC_PRD 7
//----------------------------------------------------------------------------------
