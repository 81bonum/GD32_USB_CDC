#include "shell.h"
#include "gd32f10x_gpio.h"
#include	"gd32f10x_can.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>





	int TimingDelay;

	
	char 		sdstring[64]=" ";

//	extern	CanTxMessage TxMessage;
	extern	uint8_t		USB_DATA_Buffer[];
	uint16_t	test_USB_Rx_Cnt;
	
	uint8_t	cmd,arg4,arg3,arg2,arg1,crc;
	uint8_t CDC_add_buf_to_transmit(uint8_t* Buf, uint16_t Len);

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
	extern uint16_t CDC_DataTx (uint8_t* Buf, uint32_t Len);
	
uint8_t hexascii_to_halfbyte(uint8_t _ascii)
	{
		 if((_ascii >= '0') && (_ascii <= '9')) return(_ascii - '0') ; 
		 if((_ascii >= 'a') && (_ascii <= 'f')) return(_ascii - 'a') ; 
		 if((_ascii >= 'A') && (_ascii <= 'F')) return(_ascii - 'A') ; 
		 return(0xFF)	;
	}
		
uint8_t halfbyte_to_hexascii(uint8_t _halfbyte)
	{
			 _halfbyte &= 0x0F ;
			 if(_halfbyte >= 10) return('A' + _halfbyte - 10) ; 
				else               return('0' + _halfbyte) ; 
	}
	
	struct Buffer_Data {		//Структура для массива привема конфигурационного 36 bait пакета.
		unsigned char codePC;
		unsigned char mask_one;
		unsigned char mask_two;
		unsigned char setting_speed;
		unsigned char crc;
	}	inform[4];
	
	
//---------------------------------------------------------------------------------------------------
//печать принятого сообщения Can 1
//
void	printcan1(void)
	{
				uint32_t num_bytes ;
				uint8_t buf[512] ;
			
		  num_bytes = 0 ;
			buf[num_bytes++] = '\n' ;
//			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.StdId)>>8);
//			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.StdId)>>4);
//			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.StdId));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.DLC)>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.DLC));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[0])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[0]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[1])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[1]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[2])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[2]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[3])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[3]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[4])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[4]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[5])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[5]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[6])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[6]));
			buf[num_bytes++] = ' ' ;
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[7])>>4);
			buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[7]));
			buf[num_bytes++] = '\r';
			
			CDC_DataTx(buf, num_bytes);
			
	}
//---------------------------------------------------------------------------------------------------
 void getConfPacked(void)										
	{
		
		inform[0].codePC = USB_DATA_Buffer[20];					// Номер ПК в сети		
		inform[1].codePC = USB_DATA_Buffer[21];					
		inform[2].codePC = USB_DATA_Buffer[22];
		inform[3].codePC = USB_DATA_Buffer[23];
		
		inform[0].mask_one = USB_DATA_Buffer[24];				// Режим прослушки FF - обычный режим, 00 - слушать все.
		inform[1].mask_one = USB_DATA_Buffer[25];
		inform[2].mask_one = USB_DATA_Buffer[26];
		inform[3].mask_one = USB_DATA_Buffer[27];
		
		inform[0].mask_two = USB_DATA_Buffer[28];				
		inform[1].mask_two = USB_DATA_Buffer[29];
		inform[2].mask_two = USB_DATA_Buffer[30];
		inform[3].mask_two = USB_DATA_Buffer[31];				
		
		inform[0].setting_speed = USB_DATA_Buffer[32];	// Скорость задающие байты.
		inform[1].setting_speed = USB_DATA_Buffer[33];
		inform[2].setting_speed = USB_DATA_Buffer[34];
		
		inform[0].crc = USB_DATA_Buffer[35];
		
		uint8_t end[3];
			end[0] = 0x30;
			end[1] = 0x0A;
			end[2] = 0x0D;
			
			
		if (inform[0].crc == 0xDF) {	
			CDC_DataTx(end, 1);
		}

		
//		for (int index = 0; index < 35; ++index) {
//			USB_DATA_Buffer[index] = 0;
//		}
	}
	
	int getSpeed() {
		if (inform[2].setting_speed == 0x41) {				//300kb
			return 5; 															
		}
		else if (inform[2].setting_speed == 0x43) {		//150kb
			return 10;
		}
		else if (inform[2].setting_speed == 0x45) {		//100kb
			return 15;
		}
		else if (inform[2].setting_speed == 0x4B) {		//50kb
			return 30;
		}
		else if (inform[2].setting_speed == 0x5D) {		//20kb
			return 75;
		}
	}
//---------------------------------------------------------------------------------------------------
	
#define RX_DATA_SIZE  1000
#define TX_DATA_SIZE  1000

	uint32_t WriteTxBufFS = 0;
	uint32_t ReadTxBufFS = 0;


uint8_t UserRxBufferFS[RX_DATA_SIZE];
uint8_t UserTxBufferFS[TX_DATA_SIZE];
extern 	uint8_t interface_state;

	
uint8_t CDC_add_buf_to_tranmsit(uint8_t* Buf, uint16_t Len)
	{
		uint16_t _cnt = Len;
		
		while(_cnt)
		{
			UserTxBufferFS[WriteTxBufFS] = *Buf;
			WriteTxBufFS++;
			Buf++;
			WriteTxBufFS %= TX_DATA_SIZE;
			_cnt--;
		}
		return(0);
	}
	
uint8_t CDC_periodic_callback(void)										//переодическая проверка есть ли что на передачу
{
	uint32_t buffptr;
	uint32_t buffsize;
	
	if (ReadTxBufFS != WriteTxBufFS)
	{
		__disable_irq();
		if (ReadTxBufFS > WriteTxBufFS)
		{
			buffsize = TX_DATA_SIZE - ReadTxBufFS;
		}
		else
		{
			buffsize = WriteTxBufFS = ReadTxBufFS;
		}
		__enable_irq();
		
			buffptr = ReadTxBufFS;
			
			if(interface_state != 1) return(1);
				
			USBD_CDC_SetTxBuffer(hUsbDevice_0, (uint8_t*)&UserTxBufferFS[buffptr], buffsize);
			
			if(USBD_CDC_TransmitPacket(hUsbDevice_0) == USBD_OK)
			{
				ptrReadUserTxBufferFS += buffsize;
				if (ptrReadUserTxBufferFS == APP_TX_DATA_SIZE)
				{
					ptrReadUserTxBufferFS = 0;
				}
			}
	}
}	
	
	
	
//---------------------------------------------------------------------------------------------------
void Delay (uint32_t ms)
{
	volatile uint32_t nCount;
	RCC_ClocksPara RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
  nCount=(RCC_Clocks.AHB_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);

}

//---------------------------------------------------------------------------------------------------


