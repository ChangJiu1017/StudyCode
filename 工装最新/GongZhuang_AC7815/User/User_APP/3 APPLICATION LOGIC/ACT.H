//#include "sys_definition.h"
//#include "MCU.H"

#include "public.h"


#define DT_DWELL 25   //点火齿来时，离DWELL开始的时间的距离的判断，短于这个距离，就认为是已经执行了
//#define TOOTH_TDC 93
//#define NB_TOOTH  24//齿数
//#define NB_TOOTH_M10 240//NB_TOOTH*10
//#define UB_NO_TOOTH  23//最大的齿号

#ifdef SY_24
	#define NB_TOOTH  24//齿数
	#define NB_TOOTH_M10 240//NB_TOOTH*10
	#define UB_NO_TOOTH  23//最大的齿号
#endif

#ifdef SY_36
	#define NB_TOOTH  36//齿数
	#define NB_TOOTH_M10 360//NB_TOOTH*10
	#define UB_NO_TOOTH  34//最大的齿号
#endif

#ifdef SY_12
	#define NB_TOOTH  12//齿数
	#define NB_TOOTH_M10 120//NB_TOOTH*10
	#define UB_NO_TOOTH  9//最大的齿号
#endif


//以下为描述事件的参数
//extern word v_CA_DwellST_toTDC;
extern sword v_CA_DwellST_toTDC;


//时间为单位
extern word v_t_ign_toTDC;
extern word v_t_DwellST_toTDC;


//以下为执行参数，从执行齿到事件发生
extern sbyte v_NO_ignTooth;//点火齿
extern sbyte v_NO_dwellTooth;//充电齿
extern word v_t_TrigTooth_To_Ign;     //触发齿到点火时刻
extern word v_t_TrigTooth_To_Dwell;   //触发齿到充电时刻

extern word v_t_TrigTooth_To_Ign_trig;     //触发齿到点火时刻   2020-12-18
extern word v_t_TrigTooth_To_Dwell_trig;   //触发齿到充电时刻

extern byte v_TPS_odr;		//TPS级别
extern byte v_TPd;

extern void Init_Act(void);
extern void Cal_Exe(void);

extern byte v_nb_cycle;		//20210930 chenwei add