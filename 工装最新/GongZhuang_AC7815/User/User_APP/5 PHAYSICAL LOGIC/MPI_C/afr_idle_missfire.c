#include "status_st.h"
#include "public.h"
#include "ign.h"//2010-1-13
#include "sci.h"

#include "add.h"

//#pragma DATA_SEG MY_ZEROPAGE
volatile V_STATUS_STR _V_IDLEMF_STATUS;
//#pragma DATA_SEG DEFAULT
//missf
byte v_dmidle;    //Ê§»ğ¼ì²âĞŞÕıÏµÊı	
byte vc_idlemiss_cur;//     µ±Ç°Ê§»ğÔöÓÍµÄÔöÁ¿							
byte vc_TW1_IDLEMISSF_av; //Ê§»ğ¼ì²âÆ½¾ùÖµ								

byte vc_TW1_IDLEMISSF_tmp;  //
byte vc_TW1_IDLEMISSF_sum;  //´ú±íOS¹¤×÷ºó£¬ÒòÊ§»ğ£¬²úÉúµÄ¶Ôµ¡ËÙ·´À¡Á¿µÄĞŞÕıµÄ²¿·Ö£¬µ±

sbyte vc_cls2_idlemissf; //Ê§»ğ¼ì²â¶Ôµ¡ËÙÑõ´«¸ĞÆ÷·´À¡½á¹ûµÄĞŞÕı£»//ÕÅÆ½Ìí¼Ó»

//for idle miss fire control
//LOCAL for "q_missfire.c" ONLY
byte rpm156_last;//	equ	0x1f2				;
byte rpm156_last2;//	equ	0x1f3	
byte rpm156_last3;//	equ	0x1f4	
byte rpm156_last4;//	equ	0x1f5	
byte rpm156_limit;//	equ	0x1f6	
byte rpm156_last5;//	equ	0x1f7	
byte rpm156_last6;//	equ	0x1fb	
byte rpm_status;//	equ	0x104			;µ¡ËÙÊ§»ğ¼ì²âÊÇ£¬ÏàÁÚÁ½È¦Ö®¼äRPM±ä»¯µÄ×´Ì¬£»;2007-7-16,	rcord	the	rpm	dec	status	by	15.625		

//LOCAL FOR "Q_MISSFIRE.C"AND "Q_IDLE.C"
sword sum_ti_inc_drpm;//L;//	equ	0x1cc	;µ¡ËÙÊ§»ğ¼ì²âÀÛ¼ÓÖµL£»ÒÉÎÊ											
//sum_ti_inc_drpmh;//	equ	0x1cd	;µ¡ËÙÊ§»ğ¼ì²âÀÛ¼ÓÖµH£»		
//missf
#define MAX_MODI_FACH 20	//EQU	20						
//#define MIN_CLR_TI_DRPM	60//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
//2009-12-22
#define MIN_CLR_TI_DRPM	40//EQU	60	;WHEN	RPM<MIN_CLR_TI_DRPM,	CLRF	VC_TW1_IDLEMISSF		
#define MIN_RPM_NO_TI_I	65//EQU	65	;2006-10-21	WHEN	THE	RPM<65*15.6=1014,	NO	TI	INC	FOR	DRPM	
#define OFFSET_FACH 10    //EQU	10		

byte wait_run_drpm_t;//	equ	0x1f8		;wait_run_drpm_t>0, will disable v_dmidle test		

/*----------THE FOLLOWING DECLARATION OF LOCAL SUB------------------------------------------*/
void miss_fire_doing(void);//Ê§»ğ´¦Àí£»
void small_fire_doing(void);
void leans_inc_miss_f(void);
void miss_f_inc_ti(sbyte w);
void miss_fire_inc_l_do(sbyte w);

//¹¦ÄÜ£ºSTARTSÇ°£¬µ¡ËÙ²ßÂÔ²ÎÊıµÄ³õÊ¼»¯
void FI_IdleMissfire_BeforeStarts(void)
{
    sum_ti_inc_drpm=0;    
    //Init_MissFire_AfterReset:://Ô­À´ÊÇÔÚ¸´Î»ºó£¬³õÊ¼»¯µÄ£¬ÎÒ¸øÅ²µ½ÕâÀïBEFORE STARTS,Ó¦¸Ã¿ÉÒÔµÄ£»
    vc_TW1_IDLEMISSF_av=0;//								;
    v_dmidle=0;//								;
    vc_TW1_IDLEMISSF_sum=0;//							;
    rpm_status=0xff;//								;½«µ¡ËÙ×ªËÙ±ä»¯±êÊ¶ÉèÖÃÎªÈ«²¿ÉÏÉı£»
    //---2008-7-29-----------------------------
    V_IDLEMF_STATUS=false;
    //Init_MF_BeforeStartRun
    wait_run_drpm_t=6;//			;2007-7-17
}

//********************************************************************************************
//×¢Òârpm_ch_statusÓëRPM_STATUSµÄÇø±ğ£»¶şÕßÇø±ğÔÚÓÚ£¬RPM_STATUSÖ»ÓÃÓÚµ¡ËÙ£¬¶ørpm_ch_statusÓÃÓÚËùÓĞ×ªËÙ£»
//¶¨Òå£º
//vc_TW1_IDLEMISSF_tmp:ÁÙÊ±Ê§»ğÔöÓÍ£¬µ±È¦T1´¥·¢Ç°¼ì²âµ½Ö®ºóÂíÉÏÊ¹ÓÃ£¬ÔÚµÚ¶şÈ¦ÂíÉÏÇåÁã£»
//v_dmidle:Ê§»ğÔöÓÍÁ¿£¬Ôö¼ÓºóË¥¼õ¶¼ÓĞ¹ı³Ì£»
//						µ±O2WORKÊ±£¬²»Ê¹ÓÃ£»
//						ÇåÁãÌõ¼ş£º
//										1 temp_rp1<=min_clr_ti_drpm;;//if rpm <min_rpm_no_ti_i, clrf ti_drpm and end miss_fire_test
//										2 START_INITIONÖĞ
//										3 ÔÚav_ti_drpm_calÖĞ,when vc_TW1_IDLEMISSF_av>max_ti_inc_drpm, which not happened normally,clrf
//										4 ANNUATION_TI_INC_DRPM:Ë¥¼õ£¬Ö±µ½0£»
//						¸³Öµ¼°Ê¹ÓÃ£º
//										1 Ö÷³ÌĞòÖĞµ÷ÓÃSMISS_FIREÊ§»ğ¼ì²âºó£¬ÔÚµ÷ÓÃAV_TI_DRPM_CAL£»
//										2 ËÑË÷µ¡ËÙÆ«ÒÆÁ¿Ê±£¬ĞèÒª¶Ô½«TI_INC_DRPM²¿·ÖÅ²µ½dti_rpm_scedlÖĞÈ¥£»ADD_TEMPL_TO_PCTI
//										3 SMISS_FIREÖĞ¼ì²âµ½Ê§»ğºó£¬ĞèÒªÔö£»
//										4 ANNUATION_TI_INC_DRPM¶ÔÆäË¥¼õ£»
//										5 Ê§»ğÔöÓÍ¶ÔÓÍÁ¿ĞŞÕı£»REC_TI_INC_DRPM
//vc_TW1_IDLEMISSF_av:
//										1 Ö÷³ÌĞòÖĞµ÷ÓÃSMISS_FIREÊ§»ğ¼ì²âºó£¬ÔÚµ÷ÓÃAV_TI_DRPM_CAL£»
//										2 START_INITIONÖĞÇåÁã
//										3 ËÑË÷µ¡ËÙÆ«ÒÆÁ¿Ê±£¬ĞèÒª¶Ô½«TI_INC_DRPM²¿·ÖÅ²µ½dti_rpm_scedlÖĞÈ¥£»ADD_TEMPL_TO_PCTI
//										4 AV_TI_DRPM_CALÖĞ,½«128´ÎÆ½¾ùÖµ¸³¸øËü£»
//					ÊÇ²»ÊÇ¿ÉÒÔËµ£¬¸´Î»ºó£¬¸ÃÖµÔÚ²»³ö´íµÄÇé¿öÏÂ¶¼²»Îª0;
//wait_run_drpm_t:µ±wait_run_drpm_t=0Ê±£¬set bdrpm_test£¬Ê¹ÄÜÊ§»ğ¼ì²â£»
//													;dec wait_run_drpm_t by 1, when to 0, set bdrpm_test
//sum_ti_inc_drpmh,l:
//										Ã¿È¦·¢ÏÖÓÍÃÅ²»ÔÚµ¡ËÙÊ±£¬sum_ti_inc_drpm-=6;Ö±µ½Îª0;
//										Ã¿´Î´Óµ¡ËÙÆğ²½Ê±£¬sum_ti_inc_drpm-=128£¬Ö±µ½Îª0£»
//										START_INITIONÖĞÇåÁã
//										ADD_TEMPL_TO_PCTIÖĞ£¬Ä³ÖÖÇé¿öÏÂ£¬¼õ°ë£»
//										AV_TI_DRPM_CALÖĞ£¬½«ti_inc_drpm-av_ti_drpmÖµÀÛ¼Óµ½ÆäÉÏ£»
//2007-7-15·´À¡³ÌĞòµÄÊ§»ğ¼ì²â

//¹¦ÄÜ£ºµ¡ËÙÊ§»ğ¼ì²âÔöÓÍ²ßÂÔ
void F_Missf_Idle(word rpm156,byte tps_odr,byte c_IdleMissfSum_CLS2)
{
    byte temp_rp1;//	equ	0x1ff	;ÁÙÊ±±äÁ¿_¾Ö²¿±äÁ¿	
    byte rev_inc_miss;  //Ê§»ğÔöÓÍÅĞ¶ÏµÄÔöÁ¿;ÓëÊ§»ğÔöÓÍÏà¹Ø,2010-1-10
    byte bDrev;
    
    vc_idlemiss_cur=0;
    vc_cls2_idlemissf=0;//±ØĞëÖÃÁã£¬²¢ÇÒ±ØĞë·ÅÔÚ³ÌĞòÖĞÊÊµ±µÄÎ»ÖÃ£»
    vc_TW1_IDLEMISSF_tmp=0; //Ã¿È¦¶¼ÇåÁã£»

    //----Í¨¹ıÔö¼Ó±äÁ¿vc_IdleMissfSum_CLS2£¬½«CLS¶ÔTW1_IDLEMISSF_sumµÄĞŞÕıÅ²³ö£»-------
  	if(vc_TW1_IDLEMISSF_sum>=c_IdleMissfSum_CLS2)
  	{
  	    vc_TW1_IDLEMISSF_sum-=c_IdleMissfSum_CLS2;    
  	}
  	else vc_TW1_IDLEMISSF_sum=0;
						
        
    //Ô­À´ÔÚDO_INJ_WHEN_LEAVE_IDLEÖĞ£¬´ÓÖĞÅ²¹ıÀ´
    //VF_JLEAVEIDLE_TRANSĞèÒªÔÚÖ®Ç°±»ÉèÖÃ¹ı
    if(VF_JLEAVEIDLETRANS_ENG)//DEC_TI_DPRM_WHEN_LEAVEIDLE:
    {   //Àë¿ªµ¡ËÙÊ±£¬¶ÔÊ§»ğÔöÁ¿µÄĞŞÕı£
	      if(sum_ti_inc_drpm>128)sum_ti_inc_drpm-=128;
        else sum_ti_inc_drpm=0;
    }
    //if(VF_JCLOSE_ISC)miss_f_inc_ti(12);//2008-10-12 add   //µ¡ËÙ·§¸Õ¹Ø±ÕÊ±¿ÌÔö¼ÓÒ»²¿·ÖÊ§»ğÔöÓÍ
    
    //£­£­£­£­smiss_fire£­£­£­£­£­£­£­£­£­£­£­£­£­£­£­£­£­£­£­:
    //Ô­smiss_fire¹ı³Ì×Ô´Ë¿ªÊ¼ 
    if(rpm156>255)temp_rp1=255;else{temp_rp1=(byte)rpm156;}

    //2009-12-15 Èç¹ûÊÇÔÚÆô¶¯½×¶Î×ªËÙ¹ıµÍ£¬Ò²¼ÆËãÊ§»ğÔöÓÍ£»
    if(temp_rp1<=MIN_CLR_TI_DRPM&&!VF_STARTS_ENG)
    {
        v_dmidle=0; //2008-5-30 add back, because missfire too at tm125-4-1#
        //2008-5-30 µ±×ªËÙ¹ıµÍ£¬Çå³ıÊ§»ğÔöÁ¿µÄÍ¬Ê±£¬Çå³ı sum,»¹ĞèÒª
    		vc_cls2_idlemissf=vc_cls2_idlemissf-vc_TW1_IDLEMISSF_sum; //2008-5-30 add
    		vc_TW1_IDLEMISSF_sum=0;                  //2008-5-30 add
    }
    //2010-1-10 add 
    rev_inc_miss=v_dmidle/4;  //20101-1-10
    
    rpm156_last6=rpm156_last5;
    rpm156_last5=rpm156_last4;
    rpm156_last4=rpm156_last3;
    rpm156_last3=rpm156_last2;
    rpm156_last2=rpm156_last;
    rpm156_last=rpm156_limit;
    
    /*-----------------------------------------------------------*/
    //µ±Âú×ãÒÔÏÂÌõ¼şºóµÄ¿ªÊ¼Ê§»ğ¼ì²â
    if((!(VF_AQAFTER_PNINI_SYSINI&&VF_O2CHANGED_OSDIAG))&&VF_E0_TPS&&(temp_rp1<rev15d6_target+10)&&temp_rp1<122)   //2009-11-26 add condiction VF_ENAQ_SYSINI
    {               //2010-1-13 form 128 to 106//1650rpm  to rev_target_idle+10
                //2010-7-10ÓÉÓÚµã»ğ²ßÂÔ¸ü¸Äºó£¬  rev15d6_targetÔÚ»ØÓÍÃÅÊ±»áºÜ´ó£¬Õâ¾Íµ¼ÖÂÔÚ¶ÏÓÍÊ±ÒÔÎªÊ§»ğ£¬Òò´Ë£¬Ôö¼Ó temp_rp1<122
        //---------Annuation_ti_inc_drpm://Ê§»ğÔöÁ¿Ë¥¼õ¹ı³Ì----------------Ì
        if(VF_4S_TMCCL){
            VF_WITHMS_IDLEMF=false;//2008-7-29 add
            if(v_dmidle>1)
            {
                v_dmidle--;
                VF_WITHMS_IDLEMF=true;//2008-7-29 add
                //2008-7-29 ADD MISSFRE FLAG
            }
        }
        //2009-12-15 Ö»ÓĞÊ§»ğÔöÓÍÓĞĞ§Ê±£¬²Å½øĞĞÊ§»ğÔöÓÍË¥¼õ£»
        rpm156_limit=temp_rp1;
        VF_ENMF_IDLEMF=true;
        if(rpm156_limit>=(rpm156_last+1))rpm_status=rpm_status*2+1;//ÔÚ»ã±àÖĞ£¬ÓÃÊÇ·ñÊÇ½øÎ»£¬È»ºóÓÃ×óÒÆÊµÏÖ
        else{rpm_status=rpm_status*2;};//µ±rpm_statusµÄµÚNÎ»ÎªfalseÊ±£¬±íÊ¾ÉÏNÈ¦µÄ×ªËÙÏà¶ÔÓëµÚN£«1È¦ÊÇÏÂ½µµÄ£»

        if(((rpm_status&0x03)==0) &&( rpm156_last2>=rpm156_limit))
        {
            //;µ±×î½üÁ½È¦×ªËÙÊÇÁ¬ĞøÏÂ½µ£¬ÇÒµ±È¦±ÈÉÏÉÏÈ¦Âı;
        		 //2014-1-15
        		 bDrev=(rpm156_last2-rpm156_limit);
        		 #if M0_MFUD1==INJ209
            	  	bDrev=bDrev/2;
        		 #else
                  bDrev=bDrev/4;
        		 #endif
        		 
          		bDrev++;
        		  vc_TW1_IDLEMISSF_tmp=bDrev;
        		 
          		if(RPM_DEC_FOR_MISFIRE+rev_inc_miss>=rpm156_last2-rpm156_limit){
          		    //Ã»³¬¹ı,²»×÷ÎªÊ§»ğ,½øĞĞÌ«Ï¡ÅĞ¶Ï
          			  //Ò»°ãÊ§»ğ
            			if(RPM_DEC_FOR_LEANS+rev_inc_miss<(rpm156_last2-rpm156_limit)){
            			    //2È¦ÏÂ½µĞ¡Óê8*15.6Ê±²»½øÈë
              				if(VF_O2WORK_OSDIAG&&VF_RICH_OSDIAG&&VF_O2DEC_OS){
              					leans_inc_miss_f();//		;O2ÏÂ½µ,+4US
              				} 
              				else //2009-12-14 add else    //2009-12-14 Ô­ÏÈÔÚÕâ¸öÌõ¼şÏÂÎŞÊ§»ğÔöÓÍ£»
              				{
              					leans_inc_miss_f();//		;O2ÏÂ½µ,+4US    				    
              				}
            			}
                      //Á½È«ÏÂ½µ²»¶àÊ±£¬ÔÙ¿´¿´ÏÂÒ»È¦
                      //two_cycle_miss_t
              		//´ËÊ±ÒÑÁ¬ĞøÁ½È¦ÏÂ½µ,ÊÇ·ñÁ¬Ğø4È¦ÏÂ½µ?	
              		else if(((rpm_status&0x0f)==0) &&(rpm156_limit<=rpm156_last4)){;//ÒÑÁ¬ĞøËÄÈ¦ÏÂ½µ   //2009-12-14 add else to here
                			if(RPM_DEC_2C_MISSFIRE+rev_inc_miss<(rpm156_last4-rpm156_limit)){;//½øÈësmall_fire_doing£º
                					small_fire_doing();//
                			}
                			else{;//three_cycle_miss_t:;Ã»Á¬ĞøËÄÈ¦³¬¹ıÏŞÖµ,×öÈıÑ­»·(6È¦)×Å»ğ²»³ä·Ö¼ì²â
                				if(((rpm_status&0x3f)==0)&&(rpm156_last6>=rpm156_limit)){
                					if(RPM_DEC_3C_MISSFIRE+rev_inc_miss<(rpm156_last6-rpm156_limit)){
                						small_fire_doing()		;//Á¬ĞøÈı¸öÑ­»·(6È¦)ÏÂ½µ,Ê§»ğ´¦Àí	}
                					}
                				}
                				//else ÕâÖÖÇé¿öÏÂ£¬±íÊ¾ÎŞÊ§»ğ£»
                			}
              		}//else ÕâÖÖÇé¿öÏÂ£¬±íÊ¾ÎŞÊ§»ğ£»
          		}
          		else{		;//³¬¹ı,×öÊ§»ğ´¦Àí:Èç¹ûµ±È¦×ªËÙ²»ÊÇ¹ıµÍ,Ôò×÷ÎªÊ§»ğ,²¢×ö¹ıÏ¡ÅĞ¶Ï,2007-7-17 ²»×ö¹ıÏ¡¼ì²â	
          		    //Ê§»ğÑÏÖØ
          			miss_fire_doing();//
          		}
        } 
    }
    else
    {

        VF_ENMF_IDLEMF	=false;    
      	rpm_status=0xff;
      	rpm156_limit=0;
      	//Àë¿ªµ¡ËÙºó£¬Ê§»ğÔöÁ¿Ë¥¼õ¹ı³Ì
      	if(tps_odr) //µ±ÓÍÃÅ²»ÔÚµ¡ËÙÊ±
      	{
              if(sum_ti_inc_drpm>6)sum_ti_inc_drpm-=6;
              else sum_ti_inc_drpm=0;
      	}
    }


    //¸Ğ¾õÒÔÏÂÄÚÈİ¶¼¿ÉÒÔ·ÅÈëµ½ÉÏÃæµÄ´óÀ¨ºÅÖ®ÄÚ
    sum_ti_inc_drpm+=(sbyte)v_dmidle-vc_TW1_IDLEMISSF_av;
    if(sum_ti_inc_drpm<0)sum_ti_inc_drpm=0;                 //2010-1-10 
    vc_TW1_IDLEMISSF_av=(byte)((sum_ti_inc_drpm)/128);

    //----------av_ti_drpm_cal-----------------------------------------
    //Ô­av_ti_drpm_cal¹ı³Ì×Ô´Ë¿ªÊ¼
    //¼ÆËã128È¦v_dmidleµÄÊ§»ğÔöÁ¿Æ½¾ù
    if(!VF_ENMF_IDLEMF){
		    v_dmidle=vc_TW1_IDLEMISSF_av;
    }

    if(vc_TW1_IDLEMISSF_av>MAX_TI_INC_DRPM||VF_ENAQ_SYSINI){;//Ê§»ğÔöÁ¿Æ½¾ùÖµ¹ı´ó£¬ÈÏÎªÓĞÎÊÌâ£»
        //2010-5-3 add condiction VF_ENAQ_SYSINI
    		sum_ti_inc_drpm=0;//Ô­ÁõÀÏÊ¦ÊÇ£ºsum_ti_inc_drpmh=0
    		vc_TW1_IDLEMISSF_av=0;
    		v_dmidle=0;
    }

    if(vc_idlemiss_cur> MAX_TI_INC_DRPM)vc_idlemiss_cur=MAX_TI_INC_DRPM;
    v_dmidle+=vc_idlemiss_cur;
    

	  if(v_dmidle> MAX_TI_INC_DRPM)v_dmidle= MAX_TI_INC_DRPM ;

    
    return;
}

/*----------THE FOLLOWING ARE LOCAL SUB PROGRAMS--------------------------------------------*/
//ÒÔÏÂÊÇÓĞ¹Ø²»Í¬Àà±ğµÄÊ§»ğ´¦Àímiss_fire_doing(´óÊ§»ğ),small_fire_doing£¨Ğ¡Ê§»ğ£©ºÍleans_inc_miss_f£¨Ï¡£©£»
//Ê§»ğ´¦ÀíµÄÍ¬Ê±ĞèÒª¿¼ÂÇO2ÊÇ·ñ¹¤×÷£»
//the subroutine related to  smiss_fire;
void miss_fire_doing(void)//Ê§»ğ´¦Àí£»
{
    if(MIN_RPM_NO_TI_I<rpm156_limit)
    {
        //µ±È¦×ªËÙ¹ıµÍ²»×÷ÎªÊ§»ğ,when rpm too low no ti_inc inc
        if(VF_O2WORK_OSDIAG){;//miss_fire_o2work
    	    if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)	miss_fire_inc_l_do(STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_fire_inc_l_do;
        }
        else{
    		miss_f_inc_ti( STEP_TI_INC_AMISF);//w=STEP_TI_INC_AMISF;call miss_f_inc_ti;
        }
    }
    return;
}


void small_fire_doing(void)
{
    //Ğ¡Ê§»ğ´¦Àí
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        //µ±È¦×ªËÙ¹ıµÍ²»×÷ÎªÊ§»ğ,w
        if(VF_O2WORK_OSDIAG){;//small_fire_o2work;
    		if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_fire_inc_l_do;
    		//2010-12-6¾ÀÕıÆäµ÷ÓÃÎ»ÖÃ£»
        }
        else{
    		miss_f_inc_ti(STEP_A_SMALL_FIRE);//w=STEP_A_SMALL_FIRE;call miss_f_inc_ti;
    		//2010-12-6¾ÀÕıÆäµ÷ÓÃÎ»ÖÃ£»
        }
    }
    return;
}

void leans_inc_miss_f(void)
{
    //Ï¡;//½øÈëµÄÌõ¼şÊÇif(CONSTSP,O2WORK&motor_status,rich&pdata_status,bo2dec)
    if(rpm156_limit>MIN_RPM_NO_TI_I)
    {
        if(VF_O2WORK_OSDIAG){;//leans_fire_o2work:
        	if(!VF_O2CHANGED_OSDIAG&&!VF_ENRICHLIMITFK_CLS)miss_fire_inc_l_do(1);//	w=1;call miss_fire_inc_l_do;
        }
        else{
        	miss_f_inc_ti(1);//	w=1;call miss_f_inc_ti;
        } 
    }
    return;
}

//ÒÔÏÂÊÇ×Ó¹ı³ÌµÄ×Ó¹ı³Ì£»¼´¶ÔÊ§»ğÔöÓÍÁ¿µÄ´¦Àí
void miss_f_inc_ti(sbyte w)
{
    vc_TW1_IDLEMISSF_tmp+=w;
    vc_idlemiss_cur=w;   //replace by vc_idlemiss_cur

    return;
}

void miss_fire_inc_l_do(sbyte w)
{
    vc_cls2_idlemissf=0;
    if(vc_TW1_IDLEMISSF_sum<MAX_MISS_FIRE_DTI){;//ÀÛ¼ÆÊ§»ğÔöÁ¿¹ı´ó,Ö»ÊÇ¶ÔÁÙÊ±Ê§»ğÔöÁ¿Ôö¼Ó,²»½øÈë
    	//	if(VF_VBATUPCONTINUE_VBAT)w=w+1;//2008-10-23 +2 //2008-10-24 changeto +1
    		vc_TW1_IDLEMISSF_tmp+=w;
    		vc_cls2_idlemissf=w; //ADD_W_TO_leansdt_WHEN_MISSFIRE
    		vc_TW1_IDLEMISSF_sum+=w;
    }
    vc_TW1_IDLEMISSF_tmp+=w;
    return;
}
