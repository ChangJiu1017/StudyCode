/*===========================================================*/
/*====断油及恢复供油模块=====================*/
/*===========================================================*/

//功能：初始化CI参数；
extern void FI_CI(void);

//功能：获得恢复供油系数ASTK_INJ_AGAIN和恢复供油后的加喷次数ADDTIME；
//         //应该在本循环Get_Status_Eng之后； 触发喷油之前；
extern void FG_CICO(void);

//---------public variables-------------------------------------------
//cfci
extern byte v_ccl_cf;       //连续断油的圈数

extern byte v_ccl_cf_bak;
extern byte v_revmax_625_cf;     //2015-10-11 断油前的转速
extern byte v_ccl_ci;//2017-3-29 恢复供油的次数

