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
    返回值：EEPROM中是否有flash map的备份
    
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
    设置FLASH MAP已经被保存到EEPROM的标识
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
    复位后校核EEPROM的情况
*/
void Check_EE_afterReset(void)
{
    
    //2015-6-23
    if(VF_VPOR_HARD)
    {

        #if M0_RELEASE
            //如果是发布版本，则map放在FLASH，不在EE
            VF_MAP_INEE_FLASH=false;    
        #else
            //如果是标定版本，则看当前EE中有没有map
            VF_MAP_INEE_FLASH=Check_EEinFlash();    
            
        #endif    
    }
}

/*
    FLASH 到EEPROM的地址转化
    输入：map在FLASH中的指针
    返回：byte值
*/
byte ByteData_FlashAtEE(byte * ptr) //从EEPROM中获取FLASH map的数据
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
    FLASH 到EEPROM的地址转化
    输入：map在FLASH中的指针
    返回：指针
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
    功能：根据map所在的页和位置，获取在EEPROM的相对地址
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
        {   //MAP 在FLASH中
            add=ADD_FLASH_START+(word)PN*0x200+SN;
        }
    #endif
    
    return add;    
}

/*
    将MAP从FLASH拷贝到EEPROM中
    形参：
        blnClear--true:将EEPROM中的值清零
                --false，将FLASH中的map拷贝到EEPROM中

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
        if(!VF_MAP_INEE_FLASH||blnClear)//2015-1-17临时调整回来
        { //2014-12-4 add condiction
              //如果刚升级过，则重新刷新map
            Set_Flag_EEinFlash(true);//在操作前，先清楚标志，以防止操作一半的情况
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
                            //如果有发现没有写对的，则退出
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
    功能：获取数组类型的map的值
    x_num:每一行的个数，
    i_y：y索引
    i_x：x索引
*/
byte Get_Map_byPtr(byte * ptr,byte x_num,byte i_y,byte i_x)
{
    word index;
    
    index=(word)x_num*i_y+i_x;
    
    return *(byte *)(ptr+index);
}


//返回值：需要操作的FLASH地址是否有效，具体操作由CMD决定
//        flashAdd－－地址
//        Cmd－－操作，－－ 1  页擦除
//                          2  字节写入
//                          3  字节读出
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
    由页码和地址获得地址指针
    2014-10-6
*/
unsigned long int GetPtrbyPNandAd(byte n) 
{
    return ADD_FLASH_BLOCK(n);    
}








