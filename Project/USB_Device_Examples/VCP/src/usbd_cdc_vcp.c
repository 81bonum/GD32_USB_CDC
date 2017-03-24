/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB virtual ComPort media access layer functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"

/* Private variables ---------------------------------------------------------*/
LINE_CODING linecoding =
{
   115200, /* Baud rate     */
    0x00,   /* Stop bits - 1 */
    0x00,   /* Parity - none */
    0x08    /* Num of bits 8 */
};

extern uint8_t  APP_DATA_Buffer[];
extern uint32_t AppBufInPtr;

/**
  * @brief  Handle the CDC device class requests
  * @param  Cmd: command code
  * @param  CmdBuf: command data buffer pointer
  * @param  Len: count of data to be sent (in bytes)
  * @retval USBD_OK
  */
static uint16_t  VCP_Ctrl (uint8_t Cmd, uint8_t* CmdBuf, uint16_t Len)
{
    switch (Cmd)
    {
        case SEND_ENCAPSULATED_COMMAND:
            /* No operation for this driver */
            break;

        case GET_ENCAPSULATED_RESPONSE:
            /* No operation for this driver */
            break;

        case SET_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case GET_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case CLEAR_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case SET_LINE_CODING:
            linecoding.dwDTERate = (uint32_t)(CmdBuf[0] | (CmdBuf[1] << 8) | (CmdBuf[2] << 16) | (CmdBuf[3] << 24));
            linecoding.bCharFormat = CmdBuf[4];
            linecoding.bParityType = CmdBuf[5];
            linecoding.bDataBits	 = CmdBuf[6];
//            VCP_COMConfig(SETTING_CONFIG);  /* Set the new configuration */
            break;

        case GET_LINE_CODING:
            CmdBuf[0] = (uint8_t)(linecoding.dwDTERate);
            CmdBuf[1] = (uint8_t)(linecoding.dwDTERate >> 8);
            CmdBuf[2] = (uint8_t)(linecoding.dwDTERate >> 16);
            CmdBuf[3] = (uint8_t)(linecoding.dwDTERate >> 24);
            CmdBuf[4] = linecoding.bCharFormat;
            CmdBuf[5] = linecoding.bParityType;
            CmdBuf[6] = linecoding.bDataBits;
            break;

        case SET_CONTROL_LINE_STATE:
						
            break;

        case SEND_BREAK:
            /* No operation for this driver */
            break;

        default:
            break;
    }

    return USBD_OK;
}



uint16_t CDC_DataTx (uint8_t* Buf, uint32_t Len)			//передача массива на ПК
{			
		uint8_t x;
		for (x=0; x<Len; x++)
		{
				APP_DATA_Buffer[AppBufInPtr] = Buf[x];
				AppBufInPtr++;
		}
		/* To avoid buffer overflow */
		if(AppBufInPtr == APP_RX_DATA_SIZE)  
		{
					AppBufInPtr = 0;
		}
		
		return USBD_OK;
}





/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
