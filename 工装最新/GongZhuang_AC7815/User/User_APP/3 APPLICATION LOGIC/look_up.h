/*===========================================================*/
/*====���������ɼ���ģ��=====================*/
/*===========================================================*/

// @Function      byte GET_BASIC_MAP(byte get_wh_map2) 
//���ܣ�����֪���������������ֵ��ǰ���£��������Բ�ֵ�����
//����ֵ�����Բ�ֵ�����
//�βΣ�y1--��һ���Yֵ��
//      y2--�ڶ����Yֵ��
//      dx_mode--��ǰXֵ���һ��Ĳ��ࣻ
//      dx_mode_all---�ڶ������һ���Xֵ֮����ܲ�����
//      ע�⣺Y1��Y2ֵ���ܵߵ���
//extern sword F_LinearInter(sword y1,sword y2,byte dx_mode,byte dx_mode_all);
//2012-10-31
extern sword F_LinearInter(sword y1,sword y2,word dx_mode,word dx_mode_all,byte blnL);


//���ܣ���ȡ����map:��TPS��REVΪ�������άmap��
//����ֵ�������ֵ��Ľ����
//�βΣ� get_wh_map2����map���ͣ�TINJ����map, TIGN���map;
//       rev_odr--v_rev_odr
//      rev_mode--v_rev_mode
//      tps_odr--v_TPS_odr
//      tps_mode--v_TPS_mode
//extern word FG_Map_basic(byte get_wh_map2,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode,byte maps_odr,byte maps_mode);
//extern word FG_Ld(byte get_wh_map2,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode);

//extern sword F_2DLookUp(byte x,unsigned char p[],byte ub,byte xM4,byte blnM4) ; //ub �������������±�  z
extern sword F_2DLookUp(byte x,const unsigned char p[][2],byte ub,byte xM4,byte blnM4) ; //ub �������������±�  2014-10-4 correction [2]

//extern sword F_2DLookUpSbyte(sbyte x, char p[],byte ub);
extern sword F_2DLookUpSbyte(sbyte x,char p[][2],byte ub);              //2014-10-4 correction

extern word F_EXPM100_Lookup(word xM64);//����EXP

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

//------MAP���ݵķ����ʾ
#define TINJ       1               //��Ҫ���ҵ�mapΪ����TPS,RPM���������map        
#define TIGN       2               //��Ҫ���ҵ�mapΪ����TPS,RPM����ĵ��map   
#define CO_LAMBDA       3          //��Ҫ���ҵ�mapΪ����TPS,RPM�����LAMBDAϡŨ����map   
#define TINJ_BY_MAPRPM  4          //��Ҫ���ҵ�mapΪ����MAPS,RPM���������map   
#define TIGN_BY_MAPRPM  5          //��Ҫ���ҵ�mapΪ����MAPS,RPM���������map   
#define CO_LAMBDA_BYMAPRPM       6 //��Ҫ���ҵ�mapΪ����MAPS,RPM�����LAMBDAϡŨ����map   
#define F_TPSTOMAP  7              //��Ҫ���ҵ�mapΪ����TPS,RPM�����MAPS��TPS��Ȩ��map  
#define ALTITUDE_PARTLOAD   8      //���ָ��ɵĺ���������

#define MAP_SL 9  //2013-2-25 ����ѧϰmap�Ĳ�ѯ 

#define MPA     10

extern byte fmf_map_half;  //����map����ֵ��һ�룻2009-10-11��add


