/*===========================================================*/
/*====过渡油量模块=====================*/
/*===========================================================*/
//功能：STARTS前，过渡工况参数的初始化；
extern void FI_Trans_BeforeStarts(void);

//功能：等待CA时，检测油门有否有变化，以决定加喷或加油
//喷射之前获取TPS的增量
//返回值：  0--无加油门动作；
//          1--有加油门动作，有预设的喷油动作，需要对油量进行修正；
//          2--有加油门动作，无预设的喷油动作（取消T35动作），加喷；
//形参：tps_ad_10bit--10bit ad 采集结果
//      teng--发动机温度
//      tps_odr--tps级别
//      lst_tps_adh8--上次采样值的高8位；
//      lst_prd_cas--上次角标周期；
//extern byte F_Accel_BeforeCACome(word tps_ad_10bit,byte teng,byte tps_odr,byte lst_tps_adh8,byte lst_prd_cas);//返回值含义：0不加油；1加油

//功能：获得JQM对油量的修正系数ACCELDT,ACCELDT_FIRST,vc_TPS_TRANS
//      注：因为要用到VF_JCI_INJ标记，所以需要在CI_CO后面
//      注：因为用到发动机状态的标记，所以需要在Get_Rev计算，Get_TPS计算，和 Get_Status_Eng之后
//形参：tps_ddt--v_tps_ddt
//      ka_b1--过渡油量的修正系数，与油膜有关
//      prd_cash
//      teng
extern void F_Trans_FMF(sbyte tps_ddt);

extern void FI_Trans_AfterReset(void);   //2009-1-13

//------------public variables------------------------------------------


extern sword v_TPS_addt;             //快要喷油时的TPS增量;对快要触发喷油前的T1要进行修正，由Accel_BeforeCACome得到 

extern byte v_nb_binj; ///2012-11-24


//注释：
//    当加油门时油量修正包括：
//        vc_FMF_trans1，vc_FMF_trans2，vc_TW1_trans2
//        以及，如果是离开怠速：vc_FMFremain_leaveidle
//        以及如果在CA未到来/即将喷油之前如果检测到油门增量： v_TPS_addt
//    当减油门时油量修正包括：
//        vc_FMF_trans1，vc_TW1_trans2
