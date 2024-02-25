#include "public.h"
#include "status_st.h"
#include "ee.h"

#include "Service_CAN.h"
#include "vbat.h"
//#include "IFsh1.h"//2014-5-9
#include "map_selection.h"

#include "add.h"  //2015-3-11 add 

//#include "data_inj_source.h"
//#include "ld.h" //2014-10-6 delete
//#include "add.h"
//#include "add_flash_mpi.h"

// const word ee_w[3000];
word zzq_ee[30]={300,301,302,303,304,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29
};                           

#define VF_WRFAIL_EE VF_IICER_EE

#define VF_SCLER_EE  VF_JPROECU_EE  //sclδ�ﵽԤ���Ҫ��2013-5-11

#define ADR_IIC_MEM_WR  0xa2     //write
#define ADR_IIC_MEM_RD  0xa3     //read


sbyte NO_Page_beErease;    //�ϵ縴λ�󱻳�ʼ��Ϊ��Чֵ

#define INVALID_PAGE 10
//#define T_WREE  ((word)1250)//EEPROMд���ʱ��     5ms
//#define T_WREE  ((word)250)//EEPROMд���ʱ��     2014-4-17
#define T_WREE  ((word)1250)//EEPROMд���ʱ��     2015-6-15


word t_lstWREE;
dword add_abs;

word dat_EE_b[4];//EEPROM��4����ַ�ı��ݣ�д��֮ǰ�ȶ�ȡ

word addr_b;
byte addr_bb;
word dataw;
word dat_tmp;
word add_tmp;

void Write_EE_1wordBack(word addn,word data);
byte EE_PROM_1word(word addr,word data);
byte EE_ERASE_1word(word addr);

word vbat_tmp;


byte v_nb_NoAck;
byte v_nb_SCLER;


//2014-5-16
byte  * p_byte_array;//2014-5-16 add 
sbyte * p_sbyte_array;//2014-5-16 add


#define  EE_PROTECT_ALL     DFPROT=0x7F;//2014-11-26 
#define  EE_PROTECT_NONE    DFPROT=0x80;//2014-11-26 

//�������ڴ�ţ�EEPROM����flash���ݵı�־�ĵ�ַ�������е�ֵ
/*
const word FLAG_MAP_IN_EE[4][2]={
    0x5fe,0x11, //0x400+0x200
    0x7fe,0x22, //0x400+0x200
    0x9fe,0x33, //0x400+0x200
    0xbfe,0x44  //0x400+0x200
};
*/


//2015-3-11 ����Ϊʹ���ⲿEEPROM�����
void bit_start(void);
void bit_stop(void);
unsigned char read_byte(void);
void write_byte(unsigned char i);


//#define M_P						3
//#define M_1US					(1*M_P)
//#define M_2US					(2*M_P)
//#define M_4US					(6*M_P)
 
//byte ib;
//2015-3-10 
//ͣ��ʱ���ӳ�ʱ��
/*  FROM 8BIT MCU
#define DELAY1US if(VF_MMOVE_ENG){for(ib=0;ib<=0;ib++){};}else{for(ib=0;ib<=8;ib++){};}
#define DELAY2US if(VF_MMOVE_ENG){for(ib=0;ib<=1;ib++){};}else{for(ib=0;ib<=8;ib++){};}
#define DELAY4US if(VF_MMOVE_ENG){for(ib=0;ib<=3;ib++){};}else{for(ib=0;ib<=8;ib++){};}
*/

//READ FOR 290US
//#define DELAY1US if(VF_MMOVE_ENG){for(ib=0;ib<=M_1US;ib++){};}else{for(ib=0;ib<=M_1US;ib++){};}
//#define DELAY2US if(VF_MMOVE_ENG){for(ib=0;ib<=M_2US;ib++){};}else{for(ib=0;ib<=M_2US;ib++){};}
//#define DELAY4US if(VF_MMOVE_ENG){for(ib=0;ib<=M_4US;ib++){};}else{for(ib=0;ib<=M_4US;ib++){};}

//READ FOR 480US
/*
#define DELAY1US if(VF_MMOVE_ENG){for(ib=0;ib<=1;ib++){};}else{for(ib=0;ib<=8;ib++){};}
#define DELAY2US if(VF_MMOVE_ENG){for(ib=0;ib<=2;ib++){};}else{for(ib=0;ib<=10;ib++){};}
#define DELAY4US if(VF_MMOVE_ENG){for(ib=0;ib<=6;ib++){};}else{for(ib=0;ib<=12;ib++){};}
*/

byte Check_EEinFlash(void);
word BOOT_EEPROM_Program_xbytes(word address, word *ptr,byte len,byte blnWaitend);//2015-3-7 add 

#define START_ADD_FOR_EE_CONST 0xe9

/*===================================================================================================*/
void Init_EE_afterReset(void)
{

    Check_EE_afterReset();
    
    NO_Page_beErease=INVALID_PAGE;

    if(VF_POR_HARD){
    }
    
    
    //Copy_Map_fromFlash2EE(true);
    
    if(VF_VPOR_HARD)
    {
        Copy_Map_fromFlash2EE(false);

        VF_JPROECU_EE=false;
        VF_JINI_EE=false;
        VF_WRFAIL_EE=false;
    }

}

/*
    ����EEPROM��ֵ�������Ǹ���ǰ���Ǹ��ı��ݣ�
    
*/
byte Load_D_DK(word add,byte bEqual)
{

     byte b;
     byte b1;
     
     b=Load_Data_EE(add);
     
     if(bEqual)b1=b;  //2010-1-6 ��rֵ�������⴦��
     else b1=(byte)(b+1);
     
     if(b1==Load_Data_EE(add+1))  //2010-1-4 add data(xb)=data(x)+1;
     {
        VF_DOUBLEER_EE=false;//2010-1-4
        return b;
     }
     else
     {
        VF_DOUBLEER_EE=true;//2010-1-4
        return 0;
     }
/**/
}

//���ܣ���EE�ж�ȡ���� ;�����ڵ�Ƭ�������EE�����ĳ���                 
byte Load_Data_EE(word adr)//���ؽ���Ƕ�ӦEE��ַ�е�����
{

    byte b;
    byte rd;
    //byte bee;//2009-3-27
    
    //bee=adr;
    if(adr<START_ADD_FOR_EE_CONST/*e9*/||adr>0xff)//2013-1-26 add >0xff
    {
        
        if(adr>=DADD_ST_MAPINEE0/*400*/) //2015-3-11 add adr>=DADD_ST_MAPINEE0
        {
            rd=rdrive_iic(adr);//������
        } else rd=rdrive_iic_ext(adr);
            
        
        
        //bee=rd;//2009-3-27 temp
    }
    else
    {
        b=adr-START_ADD_FOR_EE_CONST;/*e9*/
        switch(b)
        {
            case 0:rd=MV_SYSINFO_0;break;
            case 1:rd=MV_SYSINFO_1;break;
            case 2:rd=MV_SYSINFO_2;break;
            case 3:rd=MV_SYSINFO_3;break;
            case 4:rd=MV_SYSINFO_4;break;
            case 5:rd=MV_SYSINFO_5;break;
            case 6:rd=MV_SYSINFO_6;break;
            case 7:rd=MV_SYSINFO_7;break;
            case 8:rd=MV_SYSINFO_8;break;//л�������ֵ346������rd����������ǲ���������Ҫ��
            case 9:rd=MV_SYSINFO_9;break;
            case 10:rd=MV_SYSINFO_10;break;
            case 11:rd=MV_SYSINFO_11;break;
            case 12:rd=MV_SYSINFO_12;break;
            case 13:rd=MV_SYSINFO_13;break;
            case 14:rd=MV_SYSINFO_14;break;
            case 15:rd=MV_SYSINFO_15;break;
            case 16:rd=MV_SYSINFO_16;break;
            case 17:rd=MV_SYSINFO_17;break;
            case 18:rd=MV_SYSINFO_18;break;
            case 19:rd=MV_SYSINFO_19;break;
            case 20:rd=MV_SYSINFO_20;break;
            case 21:rd=MV_SYSINFO_21;break;
            case 22:rd=MV_SYSINFO_22;break;
        }
    }
    CLR_WDG;          //2008-3-9 zhangping add ; 

    return rd;

/**/
}

/*
    ���ⲿEEPROM�����ݵ��뵽�ڲ�EEPROM
    2015-3-23
    ���ڲ�EEPROM�����ԭ�򱻲���������£�
    ���ⲿEERPOM�����ݵ��������
    ���ҲҪ����дEERPOM�ǣ�����EEPROMһ��д��

    ���ݵ�ԭ��
        ��У��EEPROM�Ƿ񱻲�������������0xff
        
*/

#if SYS==SYS_PI
    void Back_EE_fromExt(void)//�ⲿ���ݵ��ڲ���������
    {
        byte i;
        
        byte blnE;
        byte rd;
        
        //return;
        
        //��ȷ��EEPROM�Ǳ������ģ�������0xff
        blnE=true;
        for(i=0;i<START_ADD_FOR_EE_CONST;i++)
        {
            
            rd=rdrive_iic(i);//�ⲿ���ݵ��ڲ���������
            if(rd!=0xff){
                blnE=false;
            }
        }
        
        if(blnE){
            //�Ǳ�������
            //���ⲿ�ı��ݹ���
            
            for(i=0;i<START_ADD_FOR_EE_CONST;i++)
            {
                rd=rdrive_iic_ext(i);
                CLR_WDG;
                wdrive_iic(i,rd);
            }
        }
        
        
    }
#else
    void Back_EE_fromExt(void) 
    {
        byte i;
        
        byte blnE;
        byte rd;
        
        return;
        
        
    }

#endif

//���ܣ������ݱ��浽EE�� ;�����ڵ�Ƭ�������EE�����ĳ���           
/*
    2015-3-23�����ȷ��Save_Data_EE���ڷ�����ͣ��״̬�µ��õ�
*/
byte Save_Data_EE(word adr,byte data)//���ؽ���Ƿ񱣴�ɹ�
{

    CLR_WDG;

     if(adr<=0xff||adr>=DADD_ST_MAPINEE0)//2015-3-23 ��ʵ����û�б�ҪдDADD_ST_MAPINEE0
     {
        wdrive_iic(adr,data); 
        
        //2015-3-23 �ڴ�����ͬʱд���ⲿEEPROM
        if(adr<=0xff)wdrive_iic_ext(adr,data); 
        
     }else wdrive_iic_ext(adr,data);
        
    
    if(VF_NOACK_EE||VF_SCLER_EE)
    {
        return false;
    }
    else 
    {
        return true; 
    }
/**/
}

//���ܣ��жϿ���λ��������������ʱ����EE��FLASHд������  ;��������λ�������ĳ���
void write_eeprom(byte Kesei,word Remark, byte BTDC,byte BACK,byte Data) 
{

    word ADD_ROM;
    byte ADD_PAGE;
    byte VF_ROM_MCU;
    
    CLR_WDG;
    ADD_ROM=Remark&0x03ff;
    VF_ROM_MCU=((Remark)>>15)&0x01; //;���0--ee��1--flash
    ADD_PAGE=((Remark)>>10)&0x1f; //,����ADD��ʵ��FLASH��ַ��ӳ��
    
    
    VF_JBEWR_EE=false;

    if(Check_FlashAdd_Valid(ADD_ROM,ADD_PAGE,VF_ROM_MCU))
    {
        if(VF_ROM_MCU>=1)
        {   
        //2015-3-11
        //������֣����õ����ڲ���FLASH
            
            //FCDIV=49;  2014-4-5 DELETE
            VF_JINI_EE=true;//2008-11-22 add 
            if(Kesei==29)
            {
                if(ADD_PAGE==14)ADD_PAGE=5;
                //erease flash
                if(BTDC==8&&BACK==0x10)
                {
                        wdrive_iic((GetAdr_inEEbyPN(ADD_PAGE,0)),0xff); //2013-1-26
                }
                
                //write to flash
                if(BTDC==2&&BACK==3)
                {
                        addr_b=Remark;
                        wdrive_iic((GetAdr_inEEbyPN(ADD_PAGE,ADD_ROM)),Data); //2013-1-26
                        
                }
                VF_JBEWR_EE=true;
            }
        }
        else
        {   

            //write to ee
            if(Kesei==29)
            {
                 if(BTDC==2&&BACK==3)
                 {
                      
                      addr_b=Remark;
                      /*
                      #if SYS==SYS_PI
                          wdrive_iic_ext((word)(Remark&0x7fff),Data); //2013-1-26
                      #else
                          wdrive_iic((word)(Remark&0x7fff),Data); //2013-1-26
                      #endif
                      */
                      Save_Data_EE((word)(Remark&0x7fff),Data) ;
                 }
                VF_JBEWR_EE=true;
            }
        }
    }
/**/
}

//���ܣ��жϿ���λ��������������ʱ����EE��FLASH��ȡ����  ;��������λ�������ĳ���
byte show_eeprom(byte Kesei,word Remark, byte BTDC,byte BACK,byte Data) 
{

    word ADD_ROM;
    byte ADD_PAGE;
    byte VF_ROM_MCU;
    byte rd;

    ADD_ROM=Remark&0x03ff;
    VF_ROM_MCU=((Remark)>>15)&0x01; //;���0--ee��1--flash
    ADD_PAGE=((Remark)>>10)&0x1f; //,����ADD��ʵ��FLASH��ַ��ӳ��
    CLR_WDG;

    rd=Data; //2008-4-26 tobecheck
    if(1/*Check_FlashAdd_Valid(ADD_ROM,ADD_PAGE,VF_ROM_MCU)*/) 
    {
        if(Kesei>=20 && Kesei<23)
        {
            if(VF_ROM_MCU>=1)
            {   //read from flash
								add_abs=ADD_FLASH_START+(Remark&0x7fff);//(unsigned long int)GetPtrbyPNandAd(ADD_PAGE)+ADD_ROM;//
								rd=*((unsigned char *)add_abs);
//							{
//								byte DT[8];
//								DT[0]=add_abs>>24;
//								DT[1]=add_abs>>16;
//								DT[2]=add_abs>>8;
//								DT[3]=add_abs>>0;
//								DT[4]=rd;
//								CAN_SendFrameExt(0x18ff0001ul|CAN_EXTENDED_FRAME_ID,0,5,DT);
//							}
//                if(VF_MAP_INEE_FLASH){
//                    rd=rdrive_iic(GetAdr_inEEbyPN(ADD_PAGE,ADD_ROM));//������
//                    
//                }else{
//                    //map ��ԭʼ��flash��λ��
//                    if(ADD_PAGE==14)ADD_PAGE=5;
//                    add_abs=(unsigned long int)GetPtrbyPNandAd(ADD_PAGE)+ADD_ROM;//(ADD_FLASH_BLOCK(ADD_PAGE)+ADD_ROM);
//                    //���ӵ�ַƫ��
//										rd=*((unsigned char *)add_abs);//
//                    
//                }
            }
            else//read from eeprom
            {
                
                    
																									 
                if(Kesei==22&&VF_1ST_INI_SYSINI)rd=rdrive_iic(Remark);//������
                else rd=Load_Data_EE(Remark);
                    
                
            }
        } 
        if(Kesei>=23 && Kesei<29)
        {
            rd=Data;
        }
    
    } else{
        rd=4;
    }
    
    return rd;
/**/
}

 
//**********************************************************************************
//****************************************************************************
word Addr_Mul_two(word addr)
{
   addr=addr<<1;
   return addr;
}

/*
    д��һ���ֽڣ��������ݵ�
*/

void Write_EE_1wordBack(word addn,word data)
{
    word ptr[2];
    
    EE_ERASE_1word(addn);
    
    //*((char *)((word)EE_BASE_ADR+adr));
    //���������ĸ���ַ������
    addr_b=EE_BASE_ADR+(addn&0xfffc);
    
    //��Ҫд��������ȷ����ݴ��λ��
    addr_bb=addn&0x0003;
    dat_EE_b[addr_bb]=(byte)data;
    
    ptr[0]=((word)(dat_EE_b[0]<<8)&0xff00)+(byte)(dat_EE_b[1]&0x00ff) ;
    ptr[1]=((word)(dat_EE_b[2]<<8)&0xff00)+(byte)(dat_EE_b[3]&0x00ff);
    
    /*
    dataw=((word)(dat_EE_b[0]<<8)&0xff00)+(byte)(dat_EE_b[1]&0x00ff);
    EE_PROM_1word(addr_b,dataw);
    
    dataw=((word)(dat_EE_b[2]<<8)&0xff00)+(byte)(dat_EE_b[3]&0x00ff);
    EE_PROM_1word(addr_b+2,dataw);
    */
    //2015-3-7 ��Ϊ4���ֽ�һ��д��
    
    BOOT_EEPROM_Program_xbytes(addr_b, ptr,2,true);//2015-3-12 from false to true    
}


//****************************************************************************
byte EE_PROM_1word(word addr,word data)
{
#if 0
    byte i;
    /*
    while(FCLKDIV!=0xd0){
        for(i=0;i<200;i++){;}
        while(!FSTAT_CCIF){;}
        FCLKDIV=0x50;
        for(i=0;i<200;i++){;}
    }
    */
    while(!FSTAT_CCIF){;}
    
    if(FSTAT_ACCERR||FSTAT_FPVIOL){
        setReg8(FSTAT, 0x30U);
    }

    FCCOBIX=0x00;//
    FCCOBHI=0x11;
    FCCOBLO=0X00;
    FCCOBIX=0x01;
    FCCOB=addr;
    FCCOBIX=0x02;
    FCCOB=data;
    FSTAT_CCIF=1;
    
    add_tmp=addr;
    dat_tmp=data;
    
    while(!FSTAT_CCIF){;}

    for(i=0;i<200;i++){;}
#endif
    return 0;
  
}

//2015-3-7 һ�α�4bytes������Ӧ�Ĳ�����4��byte��EEPROM
//////////////////////��̰˸��ֽ�   д�����԰˸��ֽ�Ϊ��λ==one phase
//len������Ҫд���WORD����������BYTE
word BOOT_EEPROM_Program_xbytes(word address, word *ptr,byte len,byte blnWaitend)//2015-3-7 add 
{
#if 0
  byte i;   
  
  while((FSTAT & FSTAT_CCIF_MASK) == 0);   //wait if command in progress
  
  FSTAT = 0x30;                 //clear ACCERR and PVIOL
  
  FCCOBIX = 0x00;
  FCCOB = 0x1100 ;
  
  FCCOBIX = 0x01;
  FCCOB = (address & 0xFFFE);   //����д��һ����
  
  for(i=2; i<len+2; i++)        //fill data (4 words) to FCCOB register
  {
    FCCOBIX = i;
    FCCOB = *ptr;
    ptr++;
  }
 
  FSTAT = 0x80;                          //launch command

  //��Ҫ�ȴ�����ʱ
  if(blnWaitend){
    
      while((FSTAT & FSTAT_CCIF_MASK) == 0); //wait for done
     
               
      if((FSTAT & (FSTAT_ACCERR_MASK | FSTAT_FPVIOL_MASK)) != 0) 
      {
        return 2;
      }
      else
      {
        return 1;
      }
  } else {
  
      return 0;
  }
  #endif 
	return 0;
}


//****************************************************************************
byte EE_ERASE_1word(word addr) 
{
#if 0
    byte k;
    byte i;
    
    //*((char *)((word)EE_BASE_ADR+adr));
    //���������ĸ���ַ������
    
    addr_b=EE_BASE_ADR+(addr&0xfffc);
    for(i=0;i<=3;i++){
        dat_EE_b[i]=*((char *)(addr_b+i));
    }
    
    /*
    while(FCLKDIV!=0xd0){
        while(!FSTAT_CCIF){;}
        FCLKDIV=0x50;
        for(k=0;k<200;k++){;}
    } */
   
    while(!FSTAT_CCIF){;}
    
    if(FSTAT_ACCERR||FSTAT_FPVIOL){
        setReg8(FSTAT, 0x30U);
    }
    
    FCCOBIX=0x00;
    FCCOB=0x1200;
    FCCOBIX=0x01;
    FCCOB=addr_b;//addr;//ע���������Ҫ������ַ��
    FSTAT_CCIF=1;
    
    while(!FSTAT_CCIF){;}
    for(k=0;k<201;k++){;}
 
#endif 
		return 0;
}
 

//****************************************************************************
byte EE_PROM_ALL(word *ptr,byte nn) 
{
    byte k;
    for(k=0;k<nn;k++)
    {
        (void)EE_PROM_1word((word)(EE_BASE_ADR+Addr_Mul_two(k)),*ptr++); 
    }
    
    return 0;
}


//****************************************************************************
byte EE_ERAZ_ALL(byte nn)
{
    byte z;
    
    for(z=0;z<nn;z+=2)
    {
        (void)EE_ERASE_1word((word)(EE_BASE_ADR+Addr_Mul_two(z)));  
    }

    return 0; 
}



//����Ϊʹ������EEPROM�����
    void CheckWriting_ext(void){
        word w;
        
        if(VF_ISWRITING_EE){
            w=T_MAINTMR-t_lstWREE;
            if(w>=T_WREE){
                VF_ISWRITING_EE=false;
            }
        }
    }

    //���³�������zqc��д��2008-3-18 �ύ
    unsigned char rdrive_iic_ext(word addr)
      {
        unsigned char i;


        vbat_tmp=GetVbat_Tmp();//2013-10-12  //2015-6-15 ֮ǰ������
        if(IS_KEY_OFF&&vbat_tmp<VBAT_FOR_EE)return 0; // 2013-10-12 

        if(VF_NOACK_EE){
                  //if(v_nb_NoAck<255)
                  v_nb_NoAck++;//2013-5-10
        }
        if(VF_SCLER_EE){
                  if(v_nb_SCLER<255)v_nb_SCLER++;//2013-5-10
        }

        VF_NOACK_EE=false;
        VF_SCLER_EE=false;
        
        bit_start();
        
        if(!VF_NOACK_EE&&!VF_SCLER_EE){
          
            write_byte(((byte)(addr>>8))*2+ADR_IIC_MEM_WR); //((byte)(adr>>7))+ADR_IIC_MEM_WR  2013-1-26
            
            if(!VF_NOACK_EE&&!VF_SCLER_EE){
              
                write_byte((byte)addr);    //2013-1-26

                if(!VF_NOACK_EE&&!VF_SCLER_EE){
                    bit_start();
                    
                    if(!VF_NOACK_EE&&!VF_SCLER_EE){
                        write_byte((word)ADR_IIC_MEM_RD+((byte)(addr>>8))*2); //ADR_IIC_MEM_RD+(byte)(adr>>7)  2013-1-26

                        if(!VF_NOACK_EE&&!VF_SCLER_EE)i=read_byte();
                    }
                }
            }
        } 
        
        bit_stop(); //2013-5-12 ����ʧ��ҲҪbitstop
        if(VF_NOACK_EE||VF_SCLER_EE)i=0;

     //****************
        return(i);
    }

    void wdrive_iic_ext(word addr,unsigned char data)  //2013-2-18 from void to byte
    {
        
//        unsigned int i;
        word w;
        
        vbat_tmp=GetVbat_Tmp();//2013-10-12
        if(IS_KEY_OFF&&vbat_tmp<VBAT_FOR_EE){
            VF_WRFAIL_EE=false;
            return; // 2013-10-12 
        }
        
        /*
        if((addr==ADR_SET_MINUTE)||(addr==ADR_SET_HOUR)
                ||(addr==ADR_SET_DAY)||(addr==ADR_SET_MONTH)||(addr==ADR_SET_YEAR))VF_JPROECU_EE=true; //ECU�ձ����
        
        */
        
        /*
        if((addr==E_DTI_T3FK0_ADR)
            ||(addr==E_DTI_T3FK0_AB)
            )VF_JINI_EE=true;      //ECU�ձ�����

        */
        
        /*----------------------------------------*/     
        /*
        //���ʱ�������жϣ����������������򲻱�
        if(addr==INJ_SHIFT_OK)
        {
             //if(VFF_NOZZLEINI_DOMFAILURE_DTC||VFF_NOZZLEINI_VBATNOTDOM_DTC||!VF_AT_0LEVEL_PAIR ||VF_NOTCAB_PAIR)return;    
             if(VFF_NOZZLEINI_DOMFAILURE_DTC||VFF_NOZZLEINI_VBATNOTDOM_DTC ||VF_NOTCAB_PAIR)data=0;//2010-5-6     

        }
        */
        /*----------------------------------------*/     
        
        if(VF_NOACK_EE){
                  if(v_nb_NoAck<255)v_nb_NoAck++;//2013-5-10
        }
        if(VF_SCLER_EE){
                  if(v_nb_SCLER<255)v_nb_SCLER++;//2013-5-10
        }

        VF_NOACK_EE=false;
        VF_SCLER_EE=false;//2013-5-11
        
        bit_start();
     //**************
        
        if(!VF_NOACK_EE&&!VF_SCLER_EE){
     
            write_byte(((byte)(addr>>8))*2+(word)ADR_IIC_MEM_WR);  //2013-1-26
                            //2013-2-17ע�ͣ���ADDR>>7 == (ADDR>>8)*2
                            //2013-2-17ע�ͣ���ADDR>>7 ��= (ADDR>>8)*2
            if(!VF_NOACK_EE&&!VF_SCLER_EE){
                write_byte((byte)addr);
                if(!VF_NOACK_EE&&!VF_SCLER_EE){
                    write_byte(data);
                }
            }
        }
          
        bit_stop();
        
        VF_ISWRITING_EE=true; 
        t_lstWREE=T_MAINTMR;//PWM2->CNT
        
        
             //for(i=0;i<0xb00;i++){} //0XB00(2816)---925*4US=3700us
            if(!VF_MMOVE_ENG||!VF_KEYON_HARD){//2013-2-18 ADD CONDICTION  2013-3-1 add conditction VF_KEYON_HARD
                //for(i=0;i<4000;i++){} //
                do{
                    w=T_MAINTMR-t_lstWREE;
                    CLR_WDG;    
                }while(w<T_WREE);
                VF_ISWRITING_EE=false;   //2013-3-2 jiuzheng
                
                //2013-5-10 add eeprom check
                //���Ӷ�д������У��
                
                if(rdrive_iic_ext(addr)!=data)VF_WRFAIL_EE=true;
            }
        
        
        
     //****************
    }

    /*
        ִ��ǰ�����ù�SCL��SDA��״̬
        ִ�к�SCL��SDA���ǵ͵�ƽ
    */
    void bit_start(void)
    {   
       // V_TEST_ON

        P_SCL_OUT;//2013-5-22
        //SDA FROM H TO L  WHEN SCL H
        //2013-2-18 NEW
        
        P_SDAH;               //2013-5-20

        P_SCLH;  // P_SCLH hold time 5us
        DELAY4US;//2013-5-11 14:29    
        
            
        P_SDAH;
        DELAY1US; //0.6US

        if(P_SCLST==0){     //2013-5-20 TI QIAN DAO    P_SDALZHIQIAN
            VF_SCLER_EE=1;
        }
        if(P_SDAST==0){      //2013-5-20
            VF_SCLER_EE=1;
        }

        P_SDAL;// 4us later, P_SCLL
        //DELAY1US; //0.6US
        DELAY4US;

        P_SCLL;
        //DELAY1US;
       // V_TEST_OFF
        
    }

    /*
      ִ��ǰ�����ù�SDA��SCL��״̬��
      ִ�к�SDA��SCL���Ǹߵ�ƽ
    */

    void bit_stop(void)
    {   //SDA FROM L TO H, WHEN SCL H
      
        //2013-2-18 NEW
        P_SDAL;    //
        DELAY4US;//2013-5-21

        P_SCLH;   // P_SCLH hold 4us, then P_SDAH
        //DELAY1US; //0.6US
        DELAY4US;//2013-5-11
        
        if(P_SCLST==0){   //2013-5-11 ADD 
            VF_SCLER_EE=1;
        }
        if(P_SDAST==1){   //2013-5-20 ADD
            VF_SCLER_EE=1;
        }

        P_SDAH;
        //DELAY1US;

        DELAY4US;//2013-5-20 ADD

    }

    /*
        ÿ�ν���ʱ����Ҫ��֤SCLΪ��
        
        �뿪ʱSCLΪ��

         P_SDAH;
         DELAY1US;
         P_SCLH;

    */
    unsigned char read_byte(void)
    {
        //unsigned char i,j;
        
        byte j;
        sbyte i;
        
        
        //ÿ�ν���ʱ����Ҫ��֤SCLΪ��
        
        //DELAY1US;
        P_SDAH;//���ݿ�����Ϊ����
        
        for(i=8,j=0;i>=0;i--)
        {
            
            //delay_t1();
            //PORT_SCL_H;
            //delay_t1();

            P_SCLH;
            //DELAY0D5US;
            //DELAY1US;
            DELAY1US;//2013-5-11
            if(P_SCLST==0){
                VF_SCLER_EE=1;
            }

            //if(P_SCLST==0){VF_IICER_EE=1;}
            if(i>0) {
                j=j<<1;
                if(P_SDAST){j=j|0x01;} 
                else j=j&0xfe;
            } else{
                //DELAY1US;
                DELAY1US;//2013-5-22 CHANGE FROM delay1us

                //if(P_SDA){VF_NOACK_EE=1;} //2013-3-4 ���������飬����Ӧ����û��ACK��
            }
            P_SCLL;
            //DELAY0D5US;
            DELAY1US;  //�͵�ƽ����1.2us
        }
        

        return(j);

    }
      
    /*
        write_byte:
        
        ִ��ǰ����Ҫ��֤SCL�ǵ͵�ƽ
        
        ÿ�ε���֮��
            SDA�ûظ�
            SCL�õ�

    */
    void write_byte(unsigned char i)
    {  
      //****write a byte*****
        unsigned char j;
        
        //��ʱ��SCLӦ���ǣ�
        
        //��ҪSCL�ǵͣ�
        //VF_IICER_EE=0;
        
        
        //delay_t1(); 
        //delay_t1();
        //DELAY0D5US;
        P_SCLL;
        
        for(j=8;j>0;j--)
        {

             if(i&0x80)
             { //���i�����λΪ1
              
                 P_SDAH;
             }
             else
             { //���i�����λΪ0
                 //DELAY0D5US;//0.1us

                 P_SDAL;
             }
             DELAY1US;
             
             P_SCLH;
             //DELAY2US; //2013-5-20 add   //2013-5-22 DELETE
             //2013-5-20 ADD
             if(i&0x80)
             { //���i�����λΪ1
              
                 if(P_SDAST==0)VF_SCLER_EE=1;
             }
             else
             { //���i�����λΪ0
                 //DELAY0D5US;//0.1us

                 if(P_SDAST==1)VF_SCLER_EE=1;
             }
             
             DELAY1US;

             if(P_SCLST==0){
                  VF_SCLER_EE=1;
             }

             i=i<<1; 
             P_SCLL;
        }
        
          
        
        //PORT_SDA_H;
        P_SDAH;//����̬���ȴ�����
        DELAY2US;
        
        //*****juge the ack***
        //if(!VF_IICER_EE)
        {
          
            P_SCLH;
            //DELAY2US;//DELAY0D5US OK
            DELAY2US;
            //if(P_SCLST==0){VF_IICER_EE=1;}

            if(P_SCLST==0){
                VF_SCLER_EE=1;
            }
             
            if(P_SDAST){
                VF_NOACK_EE=1;
            }

            P_SCLL;

            //DELAY1US;
          
        }
            //***************************************
    }

//����Ϊʹ��MCU����EEPROM�����
    void CheckWriting(void){
#if 0
        word w;
        
        if(VF_ISWRITING_EE){
            w=T_MAINTMR-t_lstWREE;
            if(w>=T_WREE){
                //deT3=w;
                if(FSTAT_CCIF)//2015-3-7 add this condiction
                {
                    VF_ISWRITING_EE=false;
                }
            }
        }
#endif
    /**/
    }

    byte rdrive_iic(word adr)
    {
#if 1
        vbat_tmp=GetVbat_Tmp();//2013-10-12  //2014-8-12
        
        
        
        if(IS_KEY_OFF&&vbat_tmp<VBAT_FOR_EE)return 0; // 2013-10-12 


        VF_NOACK_EE=false;
        VF_SCLER_EE=false;
        
        //if(VF_NOACK_EE||VF_SCLER_EE)i=0;
        //���·����16bit ECU��EEPROM�Ĳ���
        
        //���ӵ�ַƫ��
        return *((char *)(/*(word)*/EE_BASE_ADR+adr));
#endif
			//return 0;
    }

    void wdrive_iic(word addr,byte data)
    {
#if 0
        word w;
        
        vbat_tmp=GetVbat_Tmp();//2013-10-12
        if(IS_KEY_OFF&&vbat_tmp<VBAT_FOR_EE){
            VF_WRFAIL_EE=false;
            return; // 2013-10-12 
        }
        
        //EE_PROTECT_NONE;///2014-11-26
        
        /*
        //2008-11-9 ���ӣ�ECU�Ƿ�״̬�����仯��
        //2008-11-11 ����,���ʱ,��ʱ����д��Щ��ַ,��Ϊ��λ���ж���������֮��Ͳ���д����
        //if((addr==ADR_ECU_NUM_HH)||(addr==ADR_ECU_NUM_HL)
        //        ||(addr==ADR_ECU_NUM_H)||(addr==ADR_ECU_NUM_L))VF_JPROECU_EE=true; //ECU�ձ����
        //��˱��Ϊ:
        if((addr==ADR_SET_MINUTE)||(addr==ADR_SET_HOUR)
                ||(addr==ADR_SET_DAY)||(addr==ADR_SET_MONTH)||(addr==ADR_SET_YEAR))VF_JPROECU_EE=true; //ECU�ձ����
        
        if((addr==E_DTI_T3FK0_ADR)
            ||(addr==E_DTI_T3FK0_AB)
            )VF_JINI_EE=true;      //ECU�ձ�����

        *///2014-10-6 delete
        
        /*
        //2009-7-31 ADD
        //INJ_SHIFT_OK
        //���ʱ�������жϣ����������������򲻱�
        if(addr==INJ_SHIFT_OK)
        {
             //if(VFF_NOZZLEINI_DOMFAILURE_DTC||VFF_NOZZLEINI_VBATNOTDOM_DTC||!VF_AT_0LEVEL_PAIR ||VF_NOTCAB_PAIR)return;    
             if(VFF_NOZZLEINI_DOMFAILURE_DTC||VFF_NOZZLEINI_VBATNOTDOM_DTC ||VF_NOTCAB_PAIR)data=0;//2010-5-6     

        }
        
        *///2014-10-6 delete
        VF_NOACK_EE=false;
        VF_SCLER_EE=false;//2013-5-11
        
        
        VF_ISWRITING_EE=true; 
        t_lstWREE=T_MAINTMR;
        
        //����д��EEPROMһ���ֽ�
        addr_b=addr;
        //deDTFI=T_MAINTMR;
        Write_EE_1wordBack(addr,(word)data);
        
        //deDTFI=T_MAINTMR-deDTFI;
         //for(i=0;i<0xb00;i++){} //0XB00(2816)---925*4US=3700us
        
        #if SYS==SYS_PI

            
            if(!VF_MMOVE_ENG||!VF_KEYON_HARD)  //2015-3-12 delete this condiction
            
            {//2013-2-18 ADD CONDICTION  2013-3-1 add conditction VF_KEYON_HARD
                //for(i=0;i<4000;i++){} //
                
                //2015-6-15
                do{
                    w=T_MAINTMR-t_lstWREE;
                    CLR_WDG;    
                }while(w<T_WREE);
                
                
                //2015-3-7 change to the following:
                while(!FSTAT_CCIF){;}//2015-3-7 //
                
                VF_ISWRITING_EE=false;   //2013-3-2 jiuzheng
                
                //2013-5-10 add eeprom check
                //���Ӷ�д������У��
                
                if(rdrive_iic(addr)!=data)VF_WRFAIL_EE=true;
            }
        #else
            //��PIϵͳ��ֱ�ӵȴ�д�꣬
            //2015-6-25
                do{
                    w=T_MAINTMR-t_lstWREE;
                    CLR_WDG;    
                }while(w<T_WREE);
                
                
                //2015-3-7 change to the following:
                while(!FSTAT_CCIF){;}//2015-3-7 //2015-6-25 move out
                
                VF_ISWRITING_EE=false;   //2013-3-2 jiuzheng
                
                //2013-5-10 add eeprom check
                //���Ӷ�д������У��
                
                if(rdrive_iic(addr)!=data)VF_WRFAIL_EE=true;
        
        #endif
        

        //EE_PROTECT_ALL;///2014-11-26
        
    /* */
#endif
    }
