//¹¦ÄÜ£ºµ¡ËÙÊ§»ð¼ì²âÔöÓÍ²ßÂÔ
//ÐÎ²Î£º
//    rpm156---×ªËÙ£¬Á¿¸Ù£º15.6RPM
//    tps_odr--ÓÍÃÅ¼¶±ð£¬
//    c_IdleMissfSum_CLS2---´øËÙ·´À¡Á¿¶Ôµ¡ËÙÊ§»ðÐÞÕýÁ¿µÄÐÞÕý
extern void F_Missf_Idle(word rpm156,byte tps_odr,byte c_IdleMissfSum_CLS2);

extern void FI_IdleMissfire_BeforeStarts(void);

//********************************************************************************************
//¹¦ÄÜ£ºÊÇµ¡ËÙÆ«ÒÆÁ¿ËÑË÷¹ý³Ì£ºÒÀ¾ÝÊÇ¡®RPM--ÓÍÁ¿¡¯µÄ¹ØÏµ
//  Ê¹ÄÜ'µ¡ËÙÆ«ÒÆÁ¿ËÑË÷¹ý³Ì'µÄÌõ¼þÊÇ£º
//				Ñõ´«¸ÐÆ÷¹¤×÷;ÇÒ run_need_for_aq=1;ÇÒs_need_for_aq=1;
//  ÓëËÑË÷Ïà¹ØµÄÆäËüÊÂ¼þ£º
//  1 ËÑË÷¿ªÊ¼ºó£¬Í¨¹ý½«wait_run_drpm_t=4£ºÀ´Í£Ö¹Ê§»ð¼ì²â£»
//  2 ËÑË÷¹ý³ÌÖÐ£¬·¢ÏÖdcdy_down_too_m£¨¼´µçÑ¹ÏÂ½µ¹ý¶àÊ±£©£¬ÖØÐÂÀ´¹ý£»
//  ËÑË÷½á¹û¶ÔÓÍÁ¿µÄÓ°Ïì£º£¨µ÷ÓÃadd_templ_to_pcti£©
//  ÐèÒª½«Ê§»ðÔöÁ¿¼õ°ë£¬²¢ÇÒ
//  1 µ±ÅÐ¶Ï³öÐèÒªÔöÓÍÊ±£ºÍ¬Ê±Ôö¼ÓT3·´À¡Öµ£¬dti_rpm_scedh£¬dti_rpm_scedlºÍ·¢³åÖµ£»
//  2 µ±ÅÐ¶ÏÐèÒª¼õÓÍÊ±£¬ÇÒÐèÒª¼õÓÍÁ½Óë¼õ°ëºóµÄÊ§»ðÔöÓÍÏà¼Ó²»´óÓÚ0£¬¼õÓÍ£¨¼õÈ¥¶þÕßÖ®ºó£©£»
//  ¹ØÓÚ£ºrun_need_for_aq(ÒÑÊÇLOCAL)£º³õÖµÊÇÔÚrun_need_initÖÐÉèÖÃÎªwait_run_aq=20£»Ã¿´Îµ÷ÓÃav_rpm_64r£¬ÇÒa_search_compÊ±µÝ¼õ£»
//														µ±Ã¿´ÎÀë¿ªµ¡ËÙ£¬ÇÒÓëÉÏ´ÎÀë¿ªµ¡ËÙÏà¾àÒ»¶¨Ê±¼äºó£¬RUN_NEED_FOR_AQ»áÔöÒ»£»
//  ¹ØÓÚ£ºs_need_for_aq(ÒÑÊÇLOCAL)£º
//										Ã¿1sµÝ¼õÒ»´Î£»
//										³õÖµ£º¸´Î»ºó³õÊ¼»¯Ê±ÎªAQ_PERIOD_FIRST£»
//													ÔÚnext_scÖÐ£¬µ±number_sced==2Ê±£¬±»³õÊ¼»¯Îªaq_period£»
//  ¹ØÓÚ£ºnumber_sced(ÊÇLOCAL):
//  ¹ØÓÚ£ºnumber_need_sum(ÊÇLOCAL):Ã¿²½Ì½µ×Ê±µÄÈ¦Êý38,ÆäÖÐÇ°6È¦ÎÞÐ§£»
//  ¹ØÓÚ£ºbrpm_l_for_aq(ÊÇLOCAL)£ºËÑË÷¹ý³ÌÖÐ£¬Æ½¾ù×ªËÙAV_RPMµÍÓÚmin_av_n_dec_aqÊ±ÖÃÎ»£¬·ñÔòÇå³ý£»
//  ¹ØÓÚ£ºdti_rpm_sc£ºOÖµ×Ô¶¯ËÑË÷¹ý³ÌÖÐµÄÁÙÊ±ÔöÁ¿£¬ÒÔÇ°ÊÇÖ±½Ó¼ÓÔÚPCTINJÉÏµÄ£»
//  ¹ØÓÚ£ºb_1st_search£ºÒ»´ÎËÑË÷Öµ´Ó¿ªÊ¼µ½½áÊøÓÉnumber_scedÖµ¾ö¶¨£»ÔÚ´Ë¹ý³ÌÖÐ£¬Èô²»Âú×ã½áÊøÌõ¼þ£¬ÓÐ¿ÉÄÜ»áÖØÀ´Ò»±é£¬
//  b_1st_search£º¾ÍÊÇÈ·¶¨µ±Ç°ÊÇ²»ÊÇÖØÀ´¹ýµÄ£»
//  ¹ØÓÚµ÷ÓÃQIDLE_SH_INIT:±íÊ¾Í£Ö¹ËÑË÷£»
//ÔÚÐÂµÄÑ­»·¿ªÊ¼£¬¼ì²âÊÇ·ñÀë¿ªµ¡ËÙ£¬¼ÆËãÍê×ªËÙÖ®ºó
//ÐÎ²Î£º×ªËÙ£¬¸×Í·ÎÂ¶È£¬Æô¶¯ÓÍÁ¿ÐÞÕýÏµÊý£¬×ªËÙÆ½¾ùÖµ£¬µ¡ËÙ·´À¡Á¿¶Ôµ¡ËÙËÑË÷ÖµµÄÐÞÕý£»

//missf
extern sbyte vc_cls2_idlemissf; //Ê§»ð¼ì²â¶Ôµ¡ËÙµ¡ËÙÑõ´«¸ÐÆ÷·´À¡½á¹ûµÄÐÞÕý£»//ÕÅÆ½Ìí¼Ó»
extern sword sum_ti_inc_drpm;//L;//	equ	0x1cc	;µ¡ËÙÊ§»ð¼ì²âÀÛ¼ÓÖµL£»ÒÉÎÊ											
extern byte vc_idlemiss_cur;//     µ±Ç°Ê§»ðÔöÓÍµÄÔöÁ¿							
										

