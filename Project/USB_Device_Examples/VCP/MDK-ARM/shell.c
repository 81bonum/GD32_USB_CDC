#include "shell.h"
#include "gd32f10x_gpio.h"
#include	"gd32f10x_can.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//uint8_t halfbyte_to_hexascii(uint8_t _halfbyte)
//{
//_halfbyte &= 0x0F;
//if(_halfbyte >= 10) return('A' + _halfbyte - 10); 
//else return('0' + _halfbyte); 
//}

//uint8_t hexascii_to_halfbyte(uint8_t _ascii)
//{
//if((_ascii >= '0') && (_ascii <= '9')) return(_ascii - '0'); 
//if((_ascii >= 'a') && (_ascii <= 'f')) return(_ascii - 'a'); 
//if((_ascii >= 'A') && (_ascii <= 'F')) return(_ascii - 'A'); 
//return(0xFF);
//}


	int TimingDelay;

	
	char 		sdstring[64]=" ";

//	extern	CanTxMessage TxMessage;
	extern	uint8_t		USB_DATA_Buffer[];
	uint16_t	test_USB_Rx_Cnt;
	
	uint8_t	cmd,arg4,arg3,arg2,arg1,crc;
	

	char 		strhex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int 		strtoint(char *str);
	int 		strtoargum(char *str);
	void 		initargv(void);
	int			putinbuffer(uint8_t	data);
	int			selectargv(char *str);	
	void		sdinit(void);
	void		USB_Send_Data(uint8_t data);
	void		usb_out(char *pString);
	void 		Delay(uint32_t nTime);
	void		printcan1(void);
	
	struct Buffer_Data {
		char setting;
		char mask_one;
		char mask_two;
		char setting_speed;
		char crc;
	}

	Buffer_Data inform[4];
//---------------------------------------------------------------------------------------------------
//печать принятого сообщения Can 1
//
void	printcan1(void)
	{
			char	sdsh,sdsl;
			uint8_t	dshell;

					dshell	= RxMessage.FF;										//просмотрим IDE байт
					if (dshell == CAN_FF_STANDARD)							//если заголовок стандартный
					{	
						sprintf(sdstring,"\r\n");									//переведем строку
						
//						dshell	= (RxMessage.StdId	&	0x0f00)>>8;		//печатаем старший байт стандартного заголовка
//						sdsh=strhex[((dshell & 0xf0)>>4)];				//выделим старший символ
//						sdsl=strhex[(dshell & 0x0f)];							//выделим младший символ
//							if(sdsh==0){sdsh='0';}									//костыль - какойто глюк при выводе 0 в терминал
//							if(sdsl==0){sdsl='0';}									//костыль - какойто глюк при выводе 0 в терминал
//								USB_Send_Data(sdsh);									//отправим в usb-comm
//								USB_Send_Data(sdsl);									//отправим в usb-comm
//								USB_Send_Data(' ');										//напечатаем пробел

//						dshell	= RxMessage.StdId	&	0xff;						//печатаем младший байт стандартного заголовка
//						sdsh=strhex[((dshell & 0xf0)>>4)];				//выделим старший символ
//						sdsl=strhex[(dshell & 0x0f)];							//выделим младший символ
//							if(sdsh==0){sdsh='0';}									//костыль - какойто глюк при выводе 0 в терминал
//							if(sdsl==0){sdsl='0';}									//костыль - какойто глюк при выводе 0 в терминал
//								USB_Send_Data(sdsh);									//отправим в usb-comm
//								USB_Send_Data(sdsl);									//отправим в usb-comm
//								USB_Send_Data(' ');										//напечатаем пробел
							
						dshell = RxMessage.Data[0];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');
						dshell = RxMessage.Data[1];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');	
						dshell = RxMessage.Data[2];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');
						dshell = RxMessage.Data[3];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');		
						dshell = RxMessage.Data[4];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');
						dshell = RxMessage.Data[5];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');	
						dshell = RxMessage.Data[6];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');	
						dshell = RxMessage.Data[7];
						sdsh= strhex[dshell];
							USB_Send_Data(sdsh);
							USB_Send_Data(' ');	
					}
	
	}
//---------------------------------------------------------------------------------------------------
	void config_adapter(void)
	{
		
		inform[0].setting = USB_DATA_Buffer[20];
		inform[1].setting = USB_DATA_Buffer[21];
		inform[2].setting = USB_DATA_Buffer[22];
		inform[3].setting = USB_DATA_Buffer[23];
		
		inform[0].mask_one = USB_DATA_Buffer[24];
		inform[1].mask_one = USB_DATA_Buffer[25];
		inform[2].mask_one = USB_DATA_Buffer[26];
		inform[3].mask_one = USB_DATA_Buffer[27];
		
		inform[0].mask_two = USB_DATA_Buffer[28];
		inform[1].mask_two = USB_DATA_Buffer[29];
		inform[2].mask_two = USB_DATA_Buffer[30];
		inform[3].mask_two = USB_DATA_Buffer[31];
		
		inform[0].setting_speed = USB_DATA_Buffer[32];
		inform[1].setting_speed = USB_DATA_Buffer[33];
		inform[2].setting_speed = USB_DATA_Buffer[34];
		
		inform[0].crc = USB_DATA_Buffer[35];
		
		for (int index = 0; index < 35; ++index) {
			USB_DATA_Buffer[index] = 0;
		}
	}
	
	int getSpeed() {
		if (inform[2].setting_speed == '41') {
			return 300;
		}
		else if (inform[2].setting_speed == '43') {
			return 150;
		}
		else if (inform[2].setting_speed == '45') {
			return 100;
		}
		else if (inform[2].setting_speed == '4B') {
			return 50;
		}
		else if (inform[2].setting_speed == '5D') {
			return 20;
		}
	}
	
//---------------------------------------------------------------------------------------------------
void Delay(uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0); 
}
//---------------------------------------------------------------------------------------------------


