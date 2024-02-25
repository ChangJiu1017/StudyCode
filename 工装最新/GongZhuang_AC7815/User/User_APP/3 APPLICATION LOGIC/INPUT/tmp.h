/*===========================================================*/
/*====ÎÂ¶È´¦ÀíÄ£¿é=====================*/
/*===========================================================*/

//¹¦ÄÜ£º¸ù¾İ¿ÕÆøºÍ·¢¶¯»úÎÂ¶È²ÉÑùÖµ£¬Õï¶ÏÎÂ¶ÈĞÅºÅÊÇ·ñÓĞÎó£¬²¢»ñµÃ¿ÕÆøºÍ·¢¶¯»úÎÂ¶ÈµÄÊ¹ÓÃÖµ£»
//ĞÎ²Î£adres_tair_8bit£­£­¿ÕÆøÎÂ¶È8Î»²É¼¯½á¹ûº
//ĞÎ²Î£adres_teng_8bit£­£­·¢¶¯»úÎÂ¶È8Î»²É¼¯½á¹û
extern void FG_TMP(word adres_tair_10bit,word adres_teng_10bit,word rev,word fmf,byte tpd);


//¹¦ÄÜ£º½«¿ÕÆøÎÂ¶ÈÔ¤²âÖµtamp_yc±£´æµ½µØÖ·ee_Tamp_saveºÍee_Tamp_save+1µÄEEÖĞ£»
//      µ±Í£³µ»ò¹Ø±ÕÔ¿³×Ê±±»µ÷ÓÃ£»
extern void FM_TMP_air_StopOrKeyoff(void);

extern void FI_TMP_AfterReset(void);//2008-10-6 add

extern void Attu_TMP(void);//ÎÂ¶ÈÔ¤²âÖµµÄË¥¼õ

extern void FI_TMP_beforeStart(void);

extern void ResetTwParaWhenKeyonOroff(void);//2016-4-4   

//------public variables--------------------------------------------        				
//#pragma DATA_SEG CAB_RAM			
//#pragma DATA_SEG DEFAULT

extern   byte v_tair_inuse ;   //¿ÕÆøÎÂ¶ÈÊ¹ÓÃÖµ
//extern byte v_teng_odr;     //¸×Í·ÎÂ¶È¼¶±ğ     			
extern byte v_teng_inuse;   //¸×Í·ÎÂ¶ÈÊ¹ÓÃÖµ     
extern byte v_teng_odrh5;   //¸×Í·ÎÂ¶ÈµÄ¸ß5bits

//extern byte RZT_1st_afterKeyOn;//2008-11-6 ¿ªÔ¿³×ºó»ñµÃµÄµÚÒ»´ÎRZT²ÉÑùÖµ£¬ÓÃÓÚ¸×Í·ÎÂ¶ÈÔ¤²â£»
extern byte tamp_yc_last;	//equ	0x46	    //;ÉÏ´Î´ÎÓÉRZTÔ¤²âµÄ¿ÕÆøÎÂ¶È£¬´ÓEEÖĞ¶ÁÈ¡µÄ		

extern sword dtw0;

extern long int v_tao_M16;
extern long int v_sigma_M2048;
extern sword v_sq;//2013-1-21

extern sword tw_stop;//2013-1-21
extern long int v_Tg_Sum256;

extern sword v_twb;//2013-3-26 add


extern byte v_R_d4;//2014-10-8 MOVE 

#define N0_62D5 25//2014-2-12 1500RPM


extern byte v_Gx_M128;//Êµ¼ÊµÄG  //2015-2-3
 

///////////////////////////////////////////////////////////
#define M0_N0_D16 1516//1500rpm
//#define BYPTR_M0_IBP			25
//#define M0_m0 100  2014-10-8 delete

#define M0_T_UC 50    //2013-3-3 move here
#define M0_TW_FORINI      (M0_T_UC+10)
#define M0_TW_FORENRICH   (M0_T_UC+30)

//20211014 chenwei add
extern sword v_te_keyon;
extern sbyte V_Ta_keyon;