/**
  ******************************************************************************
  * @file    app.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Main routine
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "gd32f10x_rcc.h"
#include "gd32f10x_gpio.h"
#include "usbd_cdc_core.h"
#include "usbd_user.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "gd32f10x_can.h"
#include "can_init.h"
#include "hw_config.h"
#include "shell.h"


/* Private variables ---------------------------------------------------------*/
USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint8_t USB_DATA_Buffer[USB_CDC_DATA_PACKET_SIZE];
extern	int	can1_resive0;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main routine will construct a USB virtual ComPort device
  * @param  None
  * @retval None
  */
RCC_ClocksPara RCC_Clocks;

void USB_Send_Data(uint8_t data)
{
  USB_DATA_Buffer[0] = data;
//��� ���� ��� �� ������� ! ��������� �������� �� ������
	CDC_DataTx(USB_DATA_Buffer, 1);
}
//-------------------------------------------------------------------------------------------------------
//������� ������ � ����������� ���� usb
//���� �� �������� ����� ������ �������� USB_Send_Data, ���������� ��������� �� ������� � ������
//void	usb_out(char *pString)
//{
//	  while (*pString != 0x00)
//	{ 
//		USB_Send_Data(*pString++);		//������� ������ � �����, ����� HOST ������ ��� ��� � ������ ��� '���'
//																	//�� ��� ��� ��� ��������� �� ������ ����������, ������� ���� ��������
//	}
//}




int main(void)
{
	    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USER_desc, &USBD_CDC_cb);
			RCC_GetClocksFreq(&RCC_Clocks);	
			uint16_t Count = 0;
			LEDs_init();
			init_CAN();
			
	
			while(1)	
				
				{
	//			config_adapter();
//					if(arg_happy==1)
//					{
//						selectargv(argv[0]);				//���������� ���������
//						initargv();									//������� ���������
//						arg_happy=0;								//������� ������� ������������� ��������� ����������
//					}
//					
//					if (can1_resive0 == 1)
//					{
//						printcan1();
//						can1_resive0=0;
//					}
					
//					if (Count == 0xffff)
//					{
//						Count = 0;
//						usb_out("abcd");
//					} else {
//						Count ++;
//					}
					
//					if (Count == 0xffff)
//						{
//							Count = 0;
//							CAN_Send_Test();
//						} else {
//							Count++;
//						}
				}

}



/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
