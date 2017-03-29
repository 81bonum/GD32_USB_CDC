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
#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint8_t USB_DATA_Buffer[USB_CDC_DATA_PACKET_SIZE];
extern	int	can1_resive0;

uint8_t interface_state = 0;
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
//тут надо что то сделать ! попробуем отдавать по одному
	CDC_DataTx(USB_DATA_Buffer, 1);
}





int main(void)
{
	    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USER_desc, &USBD_CDC_cb);
			RCC_GetClocksFreq(&RCC_Clocks);	
			uint16_t Count = 0;
			LEDs_init();
			init_CAN();
			GPIOA->DOR^= GPIO_DOR_DOR5;
			GPIOA->DOR^= GPIO_DOR_DOR4;
			Delay(100);
			GPIOA->DOR^= GPIO_DOR_DOR5;
			GPIOA->DOR^= GPIO_DOR_DOR4;
			
			while(1)		
				{		



					//CDC_DataTx(end, 3);
					/*if (can1_resive0 == 1)
					{
						printcan1();
						can1_resive0=0;
					}*/
				}

}



/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
