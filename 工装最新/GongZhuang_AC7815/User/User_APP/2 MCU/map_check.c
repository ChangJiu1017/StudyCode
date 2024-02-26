#include "public.h"
#include "add_flash.h"

#include "ee.h"

#include "status_st.h"


const word FLAG_MAP_IN_EE[4][2]={
    DADD_MAP_INEE_FLG(0),0x11, //0x400+0x200
    DADD_MAP_INEE_FLG(1),0x22, //0x400+0x200
    DADD_MAP_INEE_FLG(2),0x33, //0x400+0x200
    DADD_MAP_INEE_FLG(3),0x44  //0x400+0x200
};


/*
    ����ֵ��EEPROM���Ƿ���flash map�ı���
    
*/
byte Check_EEinFlash(void)
{
    byte i;
    
    for(i=0;i<=3;i++){
      
        if(Load_D_DK(FLAG_MAP_IN_EE[i][0],false)!=(byte)FLAG_MAP_IN_EE[i][1] ){
            return false;
        }
    }
    
    return true;
    
}

/*
    ����FLASH MAP�Ѿ������浽EEPROM�ı�ʶ
*/
void Set_Flag_EEinFlash(byte blnClear)
{
    byte i;
    
    for(i=0;i<=3;i++){
        if(blnClear){
            Save_Data_EE(FLAG_MAP_IN_EE[i][0],0);
            Save_Data_EE(FLAG_MAP_IN_EE[i][0]+1,1); 
            VF_MAP_INEE_FLASH=false;
            
        } else{
            Save_Data_EE(FLAG_MAP_IN_EE[i][0],FLAG_MAP_IN_EE[i][1]);
            Save_Data_EE(FLAG_MAP_IN_EE[i][0]+1,(byte)(FLAG_MAP_IN_EE[i][1]+1));
            VF_MAP_INEE_FLASH=true;
        }
    }
}

/*
    ��λ��У��EEPROM�����
*/
void Check_EE_afterReset(void)
{
    
    //2015-6-23
    if(VF_VPOR_HARD)
    {

        #if M0_RELEASE
            //����Ƿ����汾����map����FLASH������EE
            VF_MAP_INEE_FLASH=false;    
        #else
            //����Ǳ궨�汾���򿴵�ǰEE����û��map
            VF_MAP_INEE_FLASH=Check_EEinFlash();    
            
        #endif    
    }
}

/*
    FLASH ��EEPROM�ĵ�ַת��
    ���룺map��FLASH�е�ָ��
    ���أ�byteֵ
*/
byte ByteData_FlashAtEE(byte * ptr) //��EEPROM�л�ȡFLASH map������
{
    //(word)(ptr-DADD_FLASH2EE-)              
    #if M0_RELEASE

    #else
        if(VF_MAP_INEE_FLASH){
              ptr-=DADD_FLASH2EE;  
        }
    #endif
    
    //return *ptr;
    return (byte)*ptr;    //2014-10-4

}

/*
    FLASH ��EEPROM�ĵ�ַת��
    ���룺map��FLASH�е�ָ��
    ���أ�ָ��
*/
byte * Ptr_FlashAtEE(byte * ptr) 
{
    //(word)(ptr-DADD_FLASH2EE-)              
    #if M0_RELEASE

    #else
        if(VF_MAP_INEE_FLASH){
              ptr-=DADD_FLASH2EE;  
        }
    #endif

    return ptr;

}


/*
    ���ܣ�����map���ڵ�ҳ��λ�ã���ȡ��EEPROM����Ե�ַ
*/
word GetAdr_inEEbyPN(byte PN,word SN){
    word add;
    
    #if M0_RELEASE
        add=ADD_FLASH_START+(word)PN*0x200+SN;
    
    #else
    
        //2015-6-23
        if(VF_MAP_INEE_FLASH) 
        {
          
            add=DADD_ST_MAPINEE0+(word)PN*0x200+SN;
        } 
        else
        {   //MAP ��FLASH��
            add=ADD_FLASH_START+(word)PN*0x200+SN;
        }
    #endif
    
    return add;    
}

/*
    ��MAP��FLASH������EEPROM��
    �βΣ�
        blnClear--true:��EEPROM�е�ֵ����
                --false����FLASH�е�map������EEPROM��

    waste time:
        14s for writing and reading 0x9FF bytes,  5.47ms/byte
*/
void Copy_Map_fromFlash2EE(byte blnClear)
{
    word i;
    word add;
    byte data;
    byte rd;
    
    byte data2;//2014-12-2


    #if M0_RELEASE

    #else
    
        //ADD_FLASH_START
        //DADD_FLASH_REF_END
        //if(!VF_MAP_INEE_FLASH||blnClear||VF_ISBOOTED_FLASH)
        if(!VF_MAP_INEE_FLASH||blnClear)//2015-1-17��ʱ��������
        { //2014-12-4 add condiction
              //�������������������ˢ��map
            Set_Flag_EEinFlash(true);//�ڲ���ǰ���������־���Է�ֹ����һ������
            //VF_MAP_INEE_FLASH=false;
            for(i=0;i<DADD_FLASH_REF_END;i++){
                add=i+DADD_ST_MAPINEE0;
                data=*(byte *)(ADD_FLASH_START+i);
                if(blnClear)data=0;
                
                data2=Load_Data_EE(add);
                
                if(data2!=data)
                { //2014-12-2 add condiction
                
                    Save_Data_EE(add,data);
                    
                    if(!blnClear){
                        rd=Load_Data_EE(add);
                        if(rd!=data){
                            //����з���û��д�Եģ����˳�
                            CLR_WDG;
                            return;    
                        }
                    }
                }
                
                CLR_WDG;
            }
            
            Back_EE_fromExt();//2015-3-23
            
            Set_Flag_EEinFlash(blnClear);
        }
        //Save_Data_EE(FLAG_MAP_IN_EE[i][0],FLAG_MAP_IN_EE[i][1]);    
    #endif
}

/*
    ���ܣ���ȡ�������͵�map��ֵ
    x_num:ÿһ�еĸ�����
    i_y��y����
    i_x��x����
*/
byte Get_Map_byPtr(byte * ptr,byte x_num,byte i_y,byte i_x)
{
    word index;
    
    index=(word)x_num*i_y+i_x;
    
    return *(byte *)(ptr+index);
}


//����ֵ����Ҫ������FLASH��ַ�Ƿ���Ч�����������CMD����
//        flashAdd������ַ
//        Cmd�������������� 1  ҳ����
//                          2  �ֽ�д��
//                          3  �ֽڶ���
byte Check_FlashAdd_Valid(word ADD_ROM,byte ADD_PAGE,byte VF_ROM_MCU)
{
    byte b;
    
    b=false;
    
    if(VF_ROM_MCU>=1)
    {
        if((ADD_PAGE<=NO_MAX_BLOCK/*14*/)&&(ADD_PAGE>=NO_MIN_BLOCK/*0*/)&&(ADD_ROM<SIZE_FLASH_BLOCK/*512*/)) //2008-10-26 ADD ) AND (
        {
            b=true;
        }
    }
    else
    {
        b=true;
    }
    return b;
}


/*
    ��ҳ��͵�ַ��õ�ַָ��
    2014-10-6
*/
unsigned long int GetPtrbyPNandAd(byte n) 
{
    return ADD_FLASH_BLOCK(n);    
}








