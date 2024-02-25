#include "public.h"
//#include "ad.h"
//#include "status_st.h" 

#include "look_up.h"
#include "public.h"
//#include "sci.h"
//#include "ld.h"

//#include "add.h"
#include "add_flash.h"

/*	
X_M64最大值为511	
/*DATA FOR MODEL TW*/	
#define M0_EXP_UB 98	
const word M1_EXP_M128_X[99]={	
//2012-10-31 X_M64m32	
0	,
32	,
64	,
96	,
128	,
160	,
192	,
224	,
256	,
288	,
320	,
352	,
384	,
416	,
448	,
480	,
512	,
544	,
576	,
608	,
640	,
672	,
704	,
736	,
768	,
832	,
864	,
896	,
960	,
992	,
1056	,
1120	,
1152	,
1184	,
1216	,
1248	,
1280	,
1312	,
1344	,
1376	,
1408	,
1440	,
1472	,
1504	,
1568	,
1600	,
1632	,
1728	,
1760	,
1792	,
1888	,
1920	,
2016	,
2048	,
2080	,
2144	,
2176	,
2208	,
2304	,
2336	,
2368	,
2432	,
2496	,
2528	,
2624	,
2656	,
2720	,
2784	,
2816	,
2912	,
2944	,
3040	,
3104	,
3296	,
3392	,
3520	,
3616	,
3712	,
3808	,
3904	,
4032	,
4128	,
4320	,
4416	,
4544	,
4736	,
4832	,
5056	,
5248	,
5440	,
5664	,
5952	,
6272	,
6688	,
7104	,
7712	,
8512	,
9920	,
16352	,
};
	
const byte M1_EXP_M128_EXP[99]={	
//	exp_M128
128	,//
126	,//
124	,//
122	,//
120	,//
118	,//
116	,//
115	,//
113	,//
111	,//
109	,//
107	,//
106	,//
104	,//
103	,//
101	,//
100	,//
98	,//
97	,//
95	,//
94	,//
92	,//
91	,//
89	,//
87	,//
85	,//
84	,//
82	,//
81	,//
78	,//
76	,//
74	,//
73	,//
72	,//
70	,//
69	,//
68	,//
67	,//
66	,//
65	,//
64	,//
63	,//
62	,//
61	,//
60	,//
59	,//
57	,//
55	,//
54	,//
53	,//
51	,//
50	,//
48	,//
47	,//
46	,//
45	,//
44	,//
43	,//
42	,//
41	,//
40	,//
39	,//
38	,//
37	,//
36	,//
35	,//
34	,//
33	,//
32	,//
31	,//
30	,//
29	,//
28	,//
26	,//
24	,//
23	,//
22	,//
21	,//
20	,//
19	,//
18	,//
17	,//
16	,//
15	,//
14	,//
13	,//
12	,//
11	,//
10	,//
9	,//
8	,//
7	,//
6	,//
5	,//
4	,//
3	,//
2	,//
1	,//
0	,//
};	
			
		
byte fmf_map_half;  //喷油map查找值的一半；2009-10-11　add
//byte dec_step_wotlearn;//2009-9-28 add 
//功能：在已知步长，步距和两点值的前提下，计算线性插值结果；
//返回值：线性插值结果；
//形参：y1--第一点的Y值；
//      y2--第二点的Y值；
//      dx_mode--当前X值与第一点的步距；
//      dx_mode_all---第二点与第一点的X值之间的总步长；
//      注意：Y1和Y2值不能颠倒；
//sword F_LinearInter(sword y1,sword y2,byte dx_mode,byte dx_mode_all)
sword F_LinearInter(sword y1,sword y2,word dx_mode,word dx_mode_all,byte blnL)  //2012-10-31
{
   sword y;

    //y=(sword)(y1+(((long int)(y2-y1)*dx_mode)/dx_mode_all));
    if(blnL)y=(sword)(y1+((((long int)y2-y1)*dx_mode)/dx_mode_all));
    else y=(sword)(y1+(((sword)(y2-y1)*(sword)dx_mode)/(sword)dx_mode_all)); //2012-11-6 add 
    
    return y;
}

//查二维表
//2012-9-24
/*
    X_M4:X乘以4倍；
  
*/
sword F_2DLookUp(byte x,const unsigned char p[][2],byte ub,byte xM4,byte blnM4)
{
    byte b;
    sword deg;
    sword x0;
    sword x1;
    
    
        
    if(x<*(unsigned char *)(p[0]))deg=*(unsigned char *)(p[0]+1);
    else if(x>=*(unsigned char *)(p[ub]))deg=*(unsigned char *)(p[ub]+1);
    else
    {
        for(b=0;b<=ub;b++)
        {
            //if(x<=*(unsigned char *)(p[b]))
            if(x<*(unsigned char *)(p[b])) //2012-9-24
            {
                if(blnM4){//X放大四倍
                    x0=xM4-(sword)(*(unsigned char *)(p[b-1]))*4;
                    if(x0<0)x0=0;
                    x1=(sword)(*(unsigned char *)(p[b])-*(unsigned char *)(p[b-1]))*4;
                    if(x0>x1)x0=x1;
                    deg=F_LinearInter(*(unsigned char *)(p[b-1]+1),*(unsigned char *)(p[b]+1)
                        ,x0,x1,false) ;
                } 
                else{   //X不做处理
                    deg=F_LinearInter(*(unsigned char *)(p[b-1]+1),*(unsigned char *)(p[b]+1)
                        ,x-*(unsigned char *)(p[b-1]),*(unsigned char *)(p[b])-*(unsigned char *)(p[b-1]),false) ;
                }
                break;    
            }
        }
    }
    return deg; 
}

sword F_2DLookUpSbyte(sbyte x,char p[][2],byte ub)
{
    byte b;
    sword deg;
    
    
    if(x<(sbyte)*( char *)(p[0])){
        deg=(sbyte)*( char *)(p[0]+1);

    }
    else if(x>=(sbyte)*( char *)(p[ub])){
        deg=(sbyte)*( char *)(p[ub]+1);

    }
    else
    {
        for(b=0;b<=ub;b++)
        {

            //if(x<=(sbyte)*( char *)(p[b]))
            if(x<(sbyte)*( char *)(p[b])) //2012-9-24
            {
                deg=F_LinearInter((sbyte)*( char *)(p[b-1]+1),(sbyte)*( char *)(p[b]+1)
                    ,x-(sbyte)*( char *)(p[b-1]),(sbyte)*( char *)(p[b])-(sbyte)*( char *)(p[b-1]),false) ;
                
                
                break;    
            }
        }
    }

    return deg; 
}

//功能：获取基础map:以TPS和REV为坐标的两维map；
//返回值：查表并插值后的结果；
//形参： get_wh_map2－－map类型，TINJ油量map, TIGN点火map;
//       rev_odr--v_rev_odr
//      rev_mode--v_rev_mode
//      tps_odr--v_TPS_odr
//      tps_mode--v_TPS_mode
//word FG_Map_basic(byte get_wh_map2,byte rev_odr,byte rev_mode,byte tps_odr,byte tps_mode)
/*
    x for col
    y for row
*/

/*
    pMap：map指针
    
    blnM2:注意是针对喷油map需要*2的情况；

    x for col
    y for row

    x_ub:注意需要确保下标都是0开始，则x的个数=x_ub+1

    #define LDB_X_UB        15
    #define LDB_Y_UB        16
    #define LDB_XMODE_NUM   32
    #define LDB_YMODE_NUM   16
    
    #define IGN_X_UB        15
    #define IGN_Y_UB        16
    #define IGN_XMODE_NUM   32
    #define IGN_YMODE_NUM   16
    
    #define UP_X_UB        15
    #define UP_Y_UB        16
    #define UP_XMODE_NUM   32
    #define UP_YMODE_NUM   16
    
    #define MPA_X_UB        15
    #define MPA_Y_UB        7
    #define MPA_XMODE_NUM   16
    #define MPA_YMODE_NUM   16
    
    #define SL_X_UB        1
    #define SL_Y_UB        1
    #define SL_XMODE_NUM   32
    #define SL_YMODE_NUM   16
    


    TINJ/TIGN/CO_LAMBDA: x--rev,y--tps, xulimit--vf_ub_rev,yulimit--VF_MAX_TPS
    
    FG_3DMap(BYPTR_M2_LDB_TPSRPM,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,LDB_X_UB,LDB_Y_UB,LDB_XMODE_NUM,LDB_YMODE_NUM,true);

    FG_3DMap(BYPTR_M2_IGN_TPSRPM,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,IGN_X_UB,IGN_Y_UB,IGN_XMODE_NUM,IGN_YMODE_NUM,false);

    FG_3DMap(BYPTR_M2_UP        ,v_rev_odr,v_rev_mode,v_TPS_odr,v_TPS_mode,   UP_X_UB,UP_Y_UB,UP_XMODE_NUM,UP_YMODE_NUM,false);

    FG_3DMap(BYPTR_M2_MPA_M128  ,v_TPS_odr,v_TPS_mode,v_pair_odr,v_pair_mode, MPA_X_UB,MPA_Y_UB,MPA_XMODE_NUM,MPA_YMODE_NUM,false);

    FG_3DMap(v_SL_map           ,0,v_Revmod_sl_rd,0,v_TPmod_sl_rd,UP_X_UB,UP_Y_UB,UP_XMODE_NUM,UP_YMODE_NUM,false);
                                //v_Rev_sl_rd,v_Revmod_sl_rd,v_TPd_sl_rd,v_TPmod_sl_rd
    
    MPA:
*/
word FG_3DMap(byte  * pMap,
          byte x_odr,byte x_mode,
          byte y_odr,byte y_mode,
          byte x_ub,byte y_ub,
          //byte blnXUlimit,byte blnYUlimit,
          byte nb_xmode,byte nb_ymode,
          byte blnM2,byte blnSbyte)
{
    
    byte b12;
    byte b34;
    word b1234;
    
    byte x_odr_nxt;
    byte y_odr_nxt;
    
    word y_idx;
    word y_idx_nxt;


    p_byte_array=pMap;

    x_odr_nxt=x_odr+1;
    if(x_odr_nxt>x_ub)x_odr_nxt=x_ub;
    
    y_odr_nxt=y_odr+1;
    if(y_odr_nxt>y_ub)y_odr_nxt=y_ub;
    
    y_idx=(word)y_odr*(x_ub+1);
    y_idx_nxt=(word)y_odr_nxt*(x_ub+1);
    
    
    if(blnSbyte){
        b12=(byte)F_LinearInter((sbyte)(*(byte *)(p_byte_array+y_idx+x_odr))
                    ,(sbyte)(*(byte *)(p_byte_array+y_idx+x_odr_nxt)),   x_mode,nb_xmode,false);
        b34=(byte)F_LinearInter((sbyte)(*(byte *)(p_byte_array+y_idx_nxt+x_odr))
                    ,(sbyte)(*(byte *)(p_byte_array+y_idx_nxt+x_odr_nxt)),x_mode,nb_xmode,false);

        b1234=(byte)F_LinearInter((sbyte)b12,(sbyte)b34,y_mode,nb_ymode,false);
    } else{
      
        b12=(byte)F_LinearInter(*(byte *)(p_byte_array+y_idx+x_odr)
                    ,*(byte *)(p_byte_array+y_idx+x_odr_nxt),   x_mode,nb_xmode,false);
        b34=(byte)F_LinearInter(*(byte *)(p_byte_array+y_idx_nxt+x_odr)
                    ,*(byte *)(p_byte_array+y_idx_nxt+x_odr_nxt),x_mode,nb_xmode,false);

        b1234=(byte)F_LinearInter((byte)b12,(byte)b34,y_mode,nb_ymode,false);
    }
    if(blnM2)b1234+=b1234;
    
    return b1234;
    
}

//两个sword 比较

byte CMP_Sword(long int sw1,long int sw2,long int standard)
//byte CMP_Sword(sword sw1,sword sw2,sword standard)
{
    long int l;
    
    l=sw1-sw2;
    //if(l>=standard)return CMP_RESULT_GE;
    //else if(l<=standard)return CMP_RESULT_LE;
    //else 
    if(l>standard)return CMP_RESULT_GT;
    else if(l==0)return CMP_RESULT_EQ;
    else return CMP_RESULT_LT;
}

//查表EXP，放大一百倍，//指数放大64倍
/*
byte F_EXPM100_Lookup(word xM64)
{
    byte exp_M100;
    byte b;
    
    exp_M100=M1_EXP_M128_EXP[M0_EXP_UB];
    
    if(xM64<=M1_EXP_M128_X[M0_EXP_UB])
    {
        // X_M24最大值为240，即N_add_M12的最大值为　4800*240/5=230400,应该为长整型
        for(b=0;b<=M0_EXP_UB;b++)
        {
            if(xM64<=M1_EXP_M128_X[b])
            {
                exp_M100=M1_EXP_M128_EXP[b]; 
                break;    
            }
        }
    }
    
    return exp_M100;
    
}
*/

word F_EXPM100_Lookup(word x) 
{
    word exp_M100;  
    sword x0,x1;
    byte b;
    
    
    if(x<M1_EXP_M128_X[0])exp_M100=4096; // M1_EXP_M128_EXP[0]128*32=
    else if(x>=M1_EXP_M128_X[M0_EXP_UB])exp_M100=0;
    else
    {
        for(b=1;b<=M0_EXP_UB;b++)
        {
            if(x<M1_EXP_M128_X[b]) //
            {
                //X放大32倍
                x0=(sword)x-M1_EXP_M128_X[b-1];
                if(x0<0)x0=0;
                x1=(sword)M1_EXP_M128_X[b]-M1_EXP_M128_X[b-1];
                if(x0>x1)x0=x1;
                
                exp_M100=F_LinearInter((word)M1_EXP_M128_EXP[b-1]*32,(word)M1_EXP_M128_EXP[b]*32
                    ,x0,x1,true) ;
                
                
                    
                break;    
            }
        }
    }
    
    return exp_M100;
}

/*
  sword 的查表，要求按升序来，2013-1-21
*/
sword F_2DLookUpSword(sword x, const sword p[][2],byte ub)
{
    byte b;
    sword deg;
    
    
    if(x<(sword)*( sword *)(p[0])){
        deg=(sword)*( sword *)(p[0]+1);

    }
    else if(x>=(sword)*( sword *)(p[ub])){
        deg=(sword)*( sword *)(p[ub]+1);

    }
    else
    {
        for(b=0;b<=ub;b++)
        {
            //if(x<=(sbyte)*( char *)(p[b]))
            if(x<(sword)*( sword *)(p[b])) //2012-9-24
            {
                deg=F_LinearInter((sword)*( sword *)(p[b-1]+1),(sword)*( sword *)(p[b]+1)
                    ,x-(sword)*( sword *)(p[b-1]),(sword)*( sword *)(p[b])-(sword)*( sword *)(p[b-1]),true) ;
                
                break;    
            }
        }
    }

    return deg; 
}


/*
    FG_3DMap_xyz
    3d MAP查表，非ODR,MODE模式，
    直接使用X,Y的真实值
    
    输入：
        x,y,X_A[],Y_A[],*Z_3D
        ub_x,X数组的最大下标，并不是数据中元素的个数
        
    2016-9-13
        针对UP，sbyte类型负数查表后变为正数的问题
        blnM2重新定义：
            bit7：1，sbyte类型
            bit7: 0， BYTE类型，
    在FG_3DMap_xyz中进行区别处理
    
    对应的：
    		  (sbyte)FG_3DMap_xyz(BYPTR_M2_UP,v_rev_625rpm,v_TPd,BYPTR_N_62D5_M2LDB,BYPTR_TPD_M2LDB,15,16,0x80);//2016-9-13
    2017-1-4
        
*/

sword FG_3DMap_xyz(byte  * Z_3D,
          sword x,sword y,
          byte X_A[],byte Y_A[], //2014-11-22 from sword to byte
          byte ub_x,byte ub_y,
          byte blnM2
          )
{
    
    byte i;
    byte index_p;
    byte index_q;

    sword p_a[18][2];//需要确保ub_x和ub_y小于17
    sword q_a[18][2];//需要确保ub_x和ub_y小于17
    
    sword z_q;
    sword z_p;

	word b1234;
    
    //找到X所处的位置
        if(x<=X_A[0])
        {
            index_p=index_q=0;
        } 
        else if(x>=X_A[ub_x]) 
        {
            index_p=index_q=ub_x;
        } 
        else{

            for(i=0;i<=ub_x-1;i++){
                if(x<=X_A[i+1]){
                    index_p=i;
                    index_q=i+1;
                    break;    
                }
            }
        }

    //把X所处的区域对应的两列拷入 p_a和q_a两个数组
        for(i=0;i<=ub_y;i++)
        {
            p_a[i][0]=Y_A[i];
            q_a[i][0]=Y_A[i];
            
            if(blnM2&0x80)  //2016-9-13
            {   //sbyte 类型
                p_a[i][1]=(sword)((sbyte)(*(byte *)(Z_3D+i*(ub_x+1)+index_p)));//Z_3D[i][index_p];
                q_a[i][1]=(sword)((sbyte)(*(byte *)(Z_3D+i*(ub_x+1)+index_q)));//Z_3D[i][index_q];
            }
            else
            {
                p_a[i][1]=(*(byte *)(Z_3D+i*(ub_x+1)+index_p));//Z_3D[i][index_p];
                q_a[i][1]=(*(byte *)(Z_3D+i*(ub_x+1)+index_q));//Z_3D[i][index_q];
            }
            
        }

    //查表,算出x所处的位置的相邻的两列，对应的y的值
        
        z_p=F_2DLookUpSword(y,p_a,ub_y);
        z_q=F_2DLookUpSword(y,q_a,ub_y);
    
    
    //查表得到最终值
        for(i=0;i<=ub_x;i++)
        {
            p_a[i][0]=X_A[i];
        }
        p_a[index_p][1]=z_p;
        p_a[index_q][1]=z_q;
        
	b1234 = (sword)F_2DLookUpSword(x,p_a,ub_x);
	if(blnM2&0x01)b1234+=b1234;//2016-9-13 增加为blnM2&0x01

    return b1234; //2015-9-14 因为这个导致NSR计算不连续，IN FG_3DMap_xyz

}
