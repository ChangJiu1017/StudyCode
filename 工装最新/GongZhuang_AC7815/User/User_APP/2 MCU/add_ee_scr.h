/*===========================================================*/
/*====¶¨ÒåEEµÄµØÖ·=====================*/
/*===========================================================*/

/*---2008-11-7 Allocate for pairs backup---------------------*/
#define EE_PAIRS        0x19
#define EE_PAIRS_B      0x1a
//2010-1-4
#define EE_P_K_TMP      0x2c
#define EE_P_K_TMPB      0x2d

#define EE_PAIR_KPT     0x2e         //EE_P_K_TMP   EE_PAIR_KPT
#define EE_PAIR_KPTB     0x2f

#define EE_PAIR_KPTL     0x3A         //EE_P_K_TMP   EE_PAIR_KPT   µÍÎÂÎÂ¶ÈÏµÊý 2010-1-15 
#define EE_PAIR_KPTLB     0x3B
  

#define	TI_SET_OK	0x0e//0eh	;=21	ok						

#define EE_NBOREC 0x15

#define	E_DTI_T3FK0_ADR	0XA1												
#define	E_DTI_T3FK0_AB	0XA2	

#define	EE_TAMP_SAVE	0xb5//	;sace	the	air	temperature		
#define	EE_TAMP_B_SAVE	0xb6//	;save	the	back	up	of	v_Tair_inuse

//;defining		group						
#define	SUM_INJ_SET_ER	0xb7												
#define	SUM_BATTER_ER	0xb8//	;if	motor	runs	more	than	1	min	inc	which	when	batter_er	happened
#define	SUM_NOOIL_ER	0xb9//	;same	as	code_l6.	inc	by	1	when	no	oil	detected		
#define	SUM_INJECTOR_ER	0xba//	;same	as	code_l5									
#define	SUM_INIT_ER	0xbb	//;same	as	code_nub_l4									
#define	SUM_RZT_ER	0xbc	//;same	as	code_nub_l3									
#define	SUM_O2_ER	0xbd	//;same	as	code_nub_l2									
#define	SUM_JQM_ER	0xbe	//;same	as	code_nub_l									

#define	RUN_TIME_OK	0xc0//0c0h	;25h	;=31	ok													
#define	RUN_TIME_OK_D	32																
#define	EE_RUN_TIMEHL	0xc1//	0c1h	;26H	;EEPORM	FOR	SAVE	RUN	TIME	",USING"		AS	"UNIT,3"	BYTES	UP	TO	4660	HOURS
#define	EE_RUN_TIMEH	0xc2//	0c2h	;27H														
#define	EE_RUN_TIME	0xc3//0c3h	;28H															
#define	RUN_TIME_OK1	0xc4//	0c4h	;29h	;=32	ok												
#define	RUN_TIME_OK1_D	33																
#define	EEADR_ALTITUDE	0xc5//	;º£°Î¸ß¶È´æ´¢															
//;run		group																
#define	CYCLE_OK	0xc6//0c6h	;7h	;=11	ok													
#define	CYCLE_OK_D	11//	;a	constant														
#define	EE_CYCLE_HH	0xc7//0c7h	;8h															
#define	EE_CYCLE_HL	0xc8//0c8h	;9h							
#define	EE_CYCLE_H	0xc9//0c9h	;0ah							
#define	CYCLE_OK1	0xca//0cah	;0bh	;=12	ok					
#define	CYCLE_OK1_D	12	
							
//;defining		injector	parametor							
#define	INJ_SHIFT_OK	0xd0//	0d0h	;=inj_shifth_map	ok					
#define	INJ_SLOP_OK	0xd3//0d3h	;=inj_shift_map	ok	
#define	SET_YEAR_OK	0xd6//0d6h	;=inj_sloph_map	ok	
#define	SET_MONTH_OK	0xd9//	0d9h	;=inj_slopl_map	ok
#define	SET_DAY_OK	0xdc//0dch	;=inj_shifth_ref	ok	
#define	SET_OK2	0xdf//0dfh	;=inj_shiftl_ref	ok	

#define	ADR_ECU_NUM_HH	0xe0//	0e0h		
#define	ADR_ECU_NUM_HL	0xe1//	0e1h		
#define	ADR_ECU_NUM_H	0xe2//	0e2h		
#define	ADR_ECU_NUM_L	0xe3//	0e3h		
#define	ADR_SET_YEAR	0xe4//	0e4h		
#define	ADR_SET_MONTH	0xe5//	0e5h		
#define	ADR_SET_DAY	0xe6//	0e6h		
#define	ADR_SET_HOUR	0xe7//	0e7h		
#define	ADR_SET_MINUTE	0xe8//	0e8h
          
/*-----------------------------------------------------------*/
//;defining		and	motor	information	0e9-0ehh
#define	ADR_ENG_PSFS    0xe9//	0e9h		
#define	ADR_ENG_PLH	    0xea//0eah			
#define	ADR_ENG_PLL	    0xeb//0ebh			
#define	ADR_ENG_LX	    0xec//0ech			
#define	ADR_MOT_ZZS	    0xed//0edh			
#define	ADR_MOT_CJ	    0xee//0eeh							
#define	ADR_INJ_LX	    0xef//0efh							
//;define	THE	PROGRAM	information	0f0h-0ffh					
#define	EEINJ_SHIFTH_MAP	0xf0//	0f0h	;=the	high	btye	of	map_injector	shift
#define	EEINJ_SHIFTL_MAP	0xf1//	0f1h	;=the	low	btye	of	map_injector	shift
#define	EEINJ_SLOPH_MAP	0xf2//	0f2h	;;=the	high	byte	of	map_injector	slope
#define	EEINJ_SLOPL_MAP	0xf3//	0f3h	;;=the	low	byte	of	map_injector	slope
#define	EEPROG_NUM_HH	0xf4//	0f4h	;					
#define	EEPROG_NUM_HL	0xf5//	0f5h	;					
#define	EEPROG_NUM_H	0xf6//	0f6h	;					
#define	EEPROG_NUM_L	0xf7//	0f7h	;					
#define	EEDTI_MAP_NUM_H	0xf8//	0f8h	;					
#define	EEDTI_MAP_NUM_L	0xf9//	0f9h	;					
#define	EEDIG_MAP_NUM	0xfa//	0fah	;					
#define	EENBTDCO_USED	0xfb//	0fbh	;					
#define	EETINJ0H	0xfc//0fch	;;=the	high	byte	of	ref_injector	shift	
#define	EETINJ0L	0xfd//0fdh	;;=the	low	byte	of	ref_injector	shift
#define	EESLOPH_REF	0xfe//0feh	;2					
#define	EESLOPL_REF	0xff//0ffh	;0				

//2008-8-12 add for pair cab
/*
    #define	EE_PAIRS_OFFSET	0x7b//	;123	;DOMµÄÁãµã	
    #define	EE_PAIRS_OFFSETB	0x7c//	;124		
    #define	EE_PAIRS_K	0x7d//	//;125	;DOMµÄÐ±ÂÊ	
    #define	EE_PAIRS_KB	0x7e//	//;126		

    ´Ó½ñÈÕ¿ªÊ¼£¬ÅäºÏFAI-LINESETTER½«PARIS DOMµÄµØÖ·¸ü¸ÄÎª¬
    PAIRS_OFFSET_A = &H28 'DOM Æ«ÒÆ
    PAIRS_OFFSET_B = &H29
    PAIRS_K_A = &H2A      'DOMÐ±ÂÊÆ«²î
    PAIRS_K_B = &H2B
	
*/
#define	EE_PAIRS_OFFSET	0x28//	;123	;DOMµÄÁãµã	
#define	EE_PAIRS_OFFSETB	0x29//	;124		
#define	EE_PAIRS_K	0x2A//	//;125	;DOMµÄÐ±ÂÊ	
#define	EE_PAIRS_KB	0x2B//	//;126		


//2008-11-13 add DTC HISTORY
#define EE_DTC_9     0x42
#define EE_DTC_10    0x43
#define EE_DTC_11    0x44
#define EE_DTC_12    0x45
#define EE_DTC_13    0x46
#define EE_DTC_14    0x47
#define EE_DTC_15    0x48
#define EE_DTC_16    0x49
#define EE_DTC_17    0x4a
#define EE_DTC_18    0x4b
#define EE_DTC_19    0x4c
#define EE_DTC_20    0x4d
#define EE_DTC_21    0x4e
#define EE_DTC_22    0x4f
#define EE_DTC_23    0x50
#define EE_DTC_24    0x51
#define EE_DTC_25    0x52
#define EE_DTC_26    0x53
#define EE_DTC_27    0x54
#define EE_DTC_28    0x55
#define EE_DTC_29    0x56
#define EE_DTC_30    0x64
#define EE_DTC_31    0x65
#define EE_DTC_32    0x66
#define EE_DTC_33    0x67
#define EE_DTC_34    0x68
#define EE_DTC_35    0x69
#define EE_DTC_36    0x6a
#define EE_DTC_37    0x6b
#define EE_DTC_38    0x6c
#define EE_DTC_39    0x6d
#define EE_DTC_40    0x6e

//2008-12-2
//ÓÃÓÚ´¢´æT4²ÎÊý
#define EE_DOMAH_1    0x7b  
#define EE_DOMAH_1B   0x7c  
#define EE_DOMAL_2    0x7d 
#define EE_DOMAL_2B   0x7e

#define EE_DOMBH_3    0x79 
#define EE_DOMBH_3B   0x7a
#define EE_DOMBL_4    0x8c
#define EE_DOMBL_4B   0x8e

#define EE_DOMCH_5    0x8f
#define EE_DOMCH_5B    0x90
#define EE_DOMCL_6    0x91
#define EE_DOMCL_6B    0x92

#define EE_VRATE_7    0x93
#define EE_VRATE_7B    0x94

#define EE_DT3AH_8    0x80
#define EE_DT3AH_8B    0x81
#define EE_DT3AL_9    0x82
#define EE_DT3AL_9B    0x83

#define EE_DT3BH_10    0x84
#define EE_DT3BH_10B    0x85
#define EE_DT3BL_11    0x86
#define EE_DT3BL_11B    0x87

#define EE_DT3CH_12    0x88
#define EE_DT3CH_12B    0x89
#define EE_DT3CL_13    0x8a
#define EE_DT3CL_13B    0x8b

//2010-3-3
#define EE_DELTAGAMA    0x99//±¾´Î£­ÉÏÒ»´Î
#define EE_DELTAGAMAB   0x9a 

//2010-3-3
#define EE_O_BAK_FLAG1  0xcb
#define EE_O_BAK    0xcc
#define EE_O_BAK_FLAG2  0xcd
#define EE_O_BAKB    0xce
#define EE_O_BAK_FLAG3  0xcf
#define EE_VAL_O_BAK_FLG1   24
#define EE_VAL_O_BAK_FLG2   25
#define EE_VAL_O_BAK_FLG3   26

#define EE_MILES_NEEDT4   0x96

//2010-3-13 Åç×ì³õÊ¼»¯Ö®ºóµÄÀï³ÌµÄ±¸·Ý
#define	EE_CYCLE_HH_INI	0xaa//0c7h	;8h															
#define	EE_CYCLE_HL_INI	0xab//0c8h	;9h							

/*
//2012-12-6 ADD FOR ¹²ÓÃ
#define EE_MILE_SET   0x95
#define EE_MILE_SETB   0x96
*/

//2013-4-15
#define ECUHARD1  0x00
#define ECUHARD2  0x01
#define ECUHARD3  0x02
#define ECUHARD4  0x03
#define ECUHARD5  0x04
#define ECUHARD6  0x05
#define ECUAB1  0x06
#define ECUAB2  0x07

//2013-5-10
#define NB_WRFAIL	    0x95
#define NB_WRFAILB	  0x96
#define NB_SLFAIL	    0x9b
#define NB_SLFAILB	  0x9c
#define NB_NOACK	    0x9d
#define NB_NOACKB     0x9e
#define NB_SCLER      0x9f
#define NB_SCLERB     0xa0

//2013-6-13 ¹ØÓÚ¼ÓÃÜ²¿·Ö
#define 	EE_PWS_S_HHHH	0x340
#define 	EE_PWS_S_HHHL	0x341
#define 	EE_PWS_S_HHH	0x342
#define 	EE_PWS_S_HHL	0x343
#define 	EE_PWS_S_HH	0x344
#define 	EE_PWS_S_HL	0x345
#define 	EE_PWS_S_H	0x346
#define 	EE_PWS_S_L	0x347
		
#define 	EE_PWL_S_HHHH	0x360
#define 	EE_PWL_S_HHHL	0x361
#define 	EE_PWL_S_HHH	0x362
#define 	EE_PWL_S_HHL	0x363
#define 	EE_PWL_S_HH	0x364
#define 	EE_PWL_S_HL	0x365
#define 	EE_PWL_S_H	0x366
#define 	EE_PWL_S_L	0x367
		
#define 	DAT_LOCK	0x55
#define 	DAT_UNLOCK	0xAA
#define 	DAT_IN	0x88
#define 	DAT_POSTPW	0x11
#define 	DAT_OTHER	0x00

#define   DAT_VALID 0x55   //Êý¾ÝÓÐÐ§
//2013-4-23

//#define EE_WITHPAIRS 0x0a
#define EE_WITHPAIRS 0x06 //2014-11-26 CHANGE TO 0x06

//----------------2013-9-7-------------------------
//2014-3-27 µ÷Õû
#define DAT_PWS_INI  0x55

#define EE_PWS_6 0x300
#define EE_PWL_6 0x360

#define EE_DADR_L_TO_S 0x60//2014-6-5 

#define EE_PWS_NOINI 0x305

#define EE_PWS_LOCK	0x315
#define EE_PWL_LOCK	0x325

#define EE_STTIME_SET 0x335  
#define EE_EDTIME_SET 0x345

#define ST_YY       0x3C0//ÔÂÈÕ½ôËæÆäºó£¬YY,MM,DD¸÷Õ¼5bytes
#define ED_YY       0x3D0
#define CUR_YY      0x3E0
#define LST_YY      0x3F0

#define EE_MAPINEE  0x3f5//2014-4-11 °Ñmap´æÔÚEEPROMµÄ±êÖ¾

//2014-5-8
#define EE_ACTIVE     0x30
#define EE_ACTIVE_DAT 30

/*AE_EE FOR SCR*/

#define	EE_PNCAB	0x28//	;123	;DOMµÄÁãµã	
#define	EE_PNCAB_1	0x29//	;124

//2014-11-14
#define EE_NB_INJ_HHH 0xd1
#define EE_NB_INJ_HH 0xd2
#define EE_NB_INJ_H 0xd4
#define EE_NB_INJ_L 0xd5
		
