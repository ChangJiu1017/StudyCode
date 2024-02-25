
#include "Service_IIC.h"




//void wdrive_iic_ext(word addr,unsigned char data)
//{

//}
/*
void bit_start(void);
void bit_stop(void);
unsigned char read_byte(void);
void write_byte(unsigned char i);

void bit_start(void)
{   
		P_SCL_OUT;
		P_SDAH
		P_SCLH
		DELAY4US;   
		P_SDAH
		DELAY1US;
		P_SDAL
		DELAY4US;
		P_SCLL
		
}



void bit_stop(void)
{   //SDA FROM L TO H, WHEN SCL H
	
		P_SDAL   //
		DELAY4US;
		P_SCLH   // P_SCLH hold 4us, then P_SDAH
		//DELAY1US; //0.6US
		DELAY4US;
		P_SDAH
		DELAY4US;
}


unsigned char read_byte(void)
{
		byte j;
		sbyte i;
	
		DELAY2US;
		P_SDAH;
		P_SDASetIn
		for(i=8,j=0;i>=0;i--)
		{
				P_SCLH;
				DELAY1US;//2013-5-11
				if(i>0) {
						j=j<<1;
						if(P_SDAST){j=j|0x01;} 
						else j=j&0xfe;
				} else{
						DELAY1US;//2013-5-22 CHANGE FROM delay1us
				}
				P_SCLL;
				DELAY1US;
		}
		return(j);
}
      
byte ackok;
void write_byte(unsigned char i)
{  
	//****write a byte*****
		unsigned char j;
	
		DELAY2US;
		P_SCLL;
		for(j=8;j>0;j--)
		{
				 if(i&0x80)
				 {
						 P_SDAH;
				 }
				 else
				 { 
						 P_SDAL;
				 }
				 DELAY1US;
				 P_SCLH;
				 DELAY1US;
				 i=i<<1; 
				 P_SCLL;
		}
		P_SDAH;
		P_SDASetIn;
		DELAY2US;
		P_SCLH;
		DELAY2US;
		if(P_SDAST==0)
		{
				ackok = 1;
		}
		else
		{
				ackok = 0;
				//bit_stop();
				//DELAY2US;
				//bit_start();
		}
		P_SCLL;
		
}
*/






