

extern void FC_Film(word fmf,sword tw,sbyte ta,byte tps_odr,byte rev_odr);
extern word Init_FilmForPreInj(sword tw,sbyte ta);
extern void Init_FilmBeforeStarts(void);
extern void Init_FilmAfterReset(void);
extern word F_AttuFilm(sword fmf);

extern sword FC_Film_UA(byte tps0,byte tps1,word fmf0);//2014-4-16

extern void FS_Film(void);//

extern sword v_dmf_binj;//����֮ǰ����Ĥ�仯��
extern word v_mf_residue;    //2012-10-18 add 

//extern void FATTU_Film(void);

extern byte vc_wf;  //��ʱ���ӵ��¶ȶ���Ĥ��������
extern word v_mf_area_M256;

extern byte v_L_DEC_M1024;

extern sword vc_FMF_dtfi_trans; //2010-5-14 ����Ĺ��ɲ��ֵ�dtfi 

extern byte v_co_dmf_M64;

extern byte v_ss;
