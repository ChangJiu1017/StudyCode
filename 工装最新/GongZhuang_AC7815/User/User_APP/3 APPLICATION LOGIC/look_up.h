/*===========================================================*/
/*====发动机负荷计算模块=====================*/
/*===========================================================*/

// @Function      byte GET_BASIC_MAP(byte get_wh_map2) 
//功能：在已知步长，步距和两点值的前提下，计算线性插值结果；
//返回值：线性插值结果；
//形参：y1--第一点的Y值；
//      y2--第二点的Y值；
//      dx_mode--当前X值与第一点的步距；
//      dx_mode_all---第二点与第一点的X值之间的总步长；
//      注意：Y1和Y2值不能颠倒；
//extern sword F_LinearInter(sword y1,sword y2,byte dx_mode,byte dx_mode_all);
//2012-10-31
extern sword F_LinearInter(sword y1,sword y2,word dx_mode,word dx_mode_all,byte blnL);


//功能：获取基础map:以TPS和REV为坐标的两维map；
//返回值：查表并插值后的结果；
//形参： get_wh_map2－－map类型，TINJ油量map, TIGN点火map;
//       rev_odr--v_rev_odr
//      rev_mode--v_rev_mode
//      tps_odr--v_TPS_odr
//      tps_mode--v_TPS_mode
//extern word FG_Map_basic(byte get_wh_map2,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode,byte maps_odr,byte maps_mode);
//extern word FG_Ld(byte get_wh_map2,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode);

//extern sword F_2DLookUp(byte x,unsigned char p[],byte ub,byte xM4,byte blnM4) ; //ub 代表数组的最大下标  z
extern sword F_2DLookUp(byte x,const unsigned char p[][2],byte ub,byte xM4,byte blnM4) ; //ub 代表数组的最大下标  2014-10-4 correction [2]

//extern sword F_2DLookUpSbyte(sbyte x, char p[],byte ub);
extern sword F_2DLookUpSbyte(sbyte x,char p[][2],byte ub);              //2014-10-4 correction

extern word F_EXPM100_Lookup(word xM64);//计算EXP

extern sword F_2DLookUpSword(sword x,const sword p[][2],byte ub);


extern word FG_3DMap(byte  * pMap,
          byte x_odr,byte x_mode,
          byte y_odr,byte y_mode,
          byte x_ub,byte y_ub,
          //byte blnXUlimit,byte blnYUlimit,
          byte nb_xmode,byte nb_ymode,
          byte blnM2,byte blnSbyte);
          

extern sword FG_3DMap_xyz(byte * Z_3D,
          sword x,sword y,
          byte X_A[],byte Y_A[],
          byte ub_x,byte ub_y,
          byte blnM2  //2017-1-4
          );
          
                    
extern byte CMP_Sword(long int sw1,long int sw2,long int standard);//2014-10-4

//------MAP数据的分类标示
#define TINJ       1               //需要查找的map为：以TPS,RPM坐标的油量map        
#define TIGN       2               //需要查找的map为：以TPS,RPM坐标的点火map   
#define CO_LAMBDA       3          //需要查找的map为：以TPS,RPM坐标的LAMBDA稀浓控制map   
#define TINJ_BY_MAPRPM  4          //需要查找的map为：以MAPS,RPM坐标的油量map   
#define TIGN_BY_MAPRPM  5          //需要查找的map为：以MAPS,RPM坐标的油量map   
#define CO_LAMBDA_BYMAPRPM       6 //需要查找的map为：以MAPS,RPM坐标的LAMBDA稀浓控制map   
#define F_TPSTOMAP  7              //需要查找的map为：以TPS,RPM坐标的MAPS与TPS的权重map  
#define ALTITUDE_PARTLOAD   8      //部分负荷的海拔修正；

#define MAP_SL 9  //2013-2-25 对自学习map的查询 

#define MPA     10

extern byte fmf_map_half;  //喷油map查找值的一半；2009-10-11　add


