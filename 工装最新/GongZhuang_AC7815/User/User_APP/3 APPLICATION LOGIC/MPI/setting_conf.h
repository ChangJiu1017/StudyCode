/*HEAD			
 ***********************************************************************			
 * Filename	:setting_conf.h		
 * Function	:setting parameter configuration.		
 * User		:xielian	
 * Date		:2017.1.20	
 * Version	:V8.1.1		
 *			
 *** History ***			
 * V8.1.0	2017.1.20	xielian 	
 *		New. 
 * V8.1.1	2017.2.15	xielian 	
 *		增加注释文件.
 * V8.1.2	2017.3.28	xielian 	
 *		增加过渡UP和FMF限制使能配置.
 ***********************************************************************			
HEAD END*/			
			
//OBD version: 255：FAI;	1:OBD1-国三;	2：OBD2-国四
#define 	OBD_VERSION_DEF		255
#define 	OBD_VERSION_FAI		255
#define 	OBD_VERSION_1		1
#define 	OBD_VERSION_2		2
#define 	OBD_VERSION_3		3	//20210222 chenwei add 欧五亮灯方式
#define 	OBD_VERSION_4		4	//20210922 chenwei add 重庆环松项目-仪表

//是否使能TA故障检测	255：不使能;	1：使能;			
#define 	EN_DTC_TA_DEF		255
#define 	EN_DTC_TA_NOTEN		255
#define 	EN_DTC_TA_EN		1

//是否使能点火线圈检测	255：不使能；1：使能开路检测			
#define 	EN_DTC_IGN_DEF		255
#define 	EN_DTC_IGN_NOTEN		255
#define 	EN_DTC_IGN_EN		1

//是否使能低负荷点火反馈	255：不使能；1：使能反馈控制；2：使能最小值限制			
#define 	EN_IGN_FK_PARTLOAD_DEF		255
#define 	EN_IGN_FK_PARTLOAD_NOTEN		255
#define 	EN_IGN_FK_PARTLOAD_FBCTR		1
#define 	EN_IGN_FK_PARTLOAD_MINLIM		2

//是否使能低负荷UP控制	255：不使能；1：使能			
#define 	EN_UP_FK_PARTLOAD_DEF		255
#define 	EN_UP_FK_PARTLOAD_NOTEN		255
#define 	EN_UP_FK_PARTLOAD_EN		1

//超速限速模式	255：推迟点火；1：断油			
#define 	MODE_REV_LIMIT_IGNRETARD		255
#define 	MODE_REV_LIMIT_FUELCUT		1

//上电后MIL延迟的时间s	255：不延迟			
#define 	MIL_DELAY_S_NODELAY		255

//TA信号来源	255：外接;1：板端			
#define 	SOURCE_TA_DEF		255
#define 	SOURCE_TA_OUTER		255
#define 	SOURCE_TA_ONBOARD		1

//是否使能秘钥	255：不使能；1：使能			
#define 	EN_PW_DEF		255
#define 	EN_PW_NOTEN		255
#define 	EN_PW_EN		1

//是否采集车速	255：不采集			
#define 	EN_VSPEED_DEF		255
#define 	EN_VSPEED_NOTEN		255
#define 	EN_VSPEED_EN		1

//是否使能海拔传感器	0：不使能			
#define 	EN_PAIRS_DEF		255
#define 	EN_PAIRS_NOTEN		0
#define 	EN_PAIRS_EN		1

//怠速控制类型	255：无阀;1：ISCV；2：IACV			
#define 	MODE_IDLECONTROL_NOICV		255
#define 	MODE_IDLECONTROL_ISCV		1
#define 	MODE_IDLECONTROL_IACV		2

//角标类型	255：标准FE+RE；1：仅FE；2：自动识别FE			
#define 	MODE_CA_FE_RE		255
#define 	MODE_CA_FE		1
#define 	MODE_CA_FE_AUTO		2

//ECU激活	255：激活；0：未激活			
#define 	MODE_ACTIVE_DEF		255
#define 	MODE_ACTIVE_ACT		255
#define 	MODE_ACTIVE_NOTACT		0

//喷雾飞行时间	默认：90			
#define 	T_SPRAY_FLY_TO_INLET_D1MS_DEF		90

//连续两次离开怠速的圈数间隔	默认：20			
#define 	UL_CYCLE_DEF		20

//急加速高速对UP和FMF限制功能使能
#define		UP_FMF_NOTEN	255
