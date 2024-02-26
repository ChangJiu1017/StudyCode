

//对应MAPS采集的时刻
#define NO_MAPS_STOP   0
#define NO_MAPS_INLET   1
#define NO_MAPS_INLET_HY  2
#define NO_MAPS_OUTLET   3
#define NO_MAPS_OUTLET_HY  4


extern void FG_Maps(byte t_MAPS,byte prd_cash,byte tps_odr,word maps_ad1,word maps_ad2);
extern void FI_Maps_AfterReset(void);  //2008-12-15

extern byte v_MAPS_ad[5];
    //v_MAPS_ad[0]:
    //v_MAPS_ad[1]:对应于v_MAPS_ad[0]的后沿进气歧管压力 ；
extern byte v_MAPS;    //代表发动机负荷的参数，对应进气门关闭时刻的进气歧管压力 ；
extern byte v_MAPS_mode; //0-31
extern byte v_MAPS_odr;  //0-16

/*
//-----------flags------------------------------------------------
//ADD_RAM_V_STATUS_MAPS         //2008-4-8增加MAPS的状态
typedef union {                             //V_STATUS_MAPS                           
  byte Byte;
  struct {
    byte b_1_MAPS    :1;                                       
    byte b_2_MAPS    :1;                                       
    byte b_3_MAPS    :1;                                       
    byte b_4_MAPS    :1;                                       
    byte b_5_MAPS    :1;                                       
    byte b_6_MAPS    :1;                                       
    byte b_7_MAPS    :1;                                       
    byte b_8_MAPS    :1;                                       
  } Bits;
} V_STATUS_MAPSSTR;
extern volatile V_STATUS_MAPSSTR _V_STATUS_MAPS @ADD_RAM_V_STATUS_MAPS;
#define V_STATUS_MAPS                       _V_STATUS_MAPS.Byte
#define VF_MAX_MAPS                           _V_STATUS_MAPS.Bits.b_1_MAPS
#define VF_ER_MAPS                           _V_STATUS_MAPS.Bits.b_2_MAPS
#define VF_3_MAPS                           _V_STATUS_MAPS.Bits.b_3_MAPS
#define VF_4_MAPS                           _V_STATUS_MAPS.Bits.b_4_MAPS
#define VF_5_MAPS                           _V_STATUS_MAPS.Bits.b_5_MAPS
#define VF_6_MAPS                           _V_STATUS_MAPS.Bits.b_6_MAPS
#define VF_7_MAPS                           _V_STATUS_MAPS.Bits.b_7_MAPS
#define VF_8_MAPS                           _V_STATUS_MAPS.Bits.b_8_MAPS   */
