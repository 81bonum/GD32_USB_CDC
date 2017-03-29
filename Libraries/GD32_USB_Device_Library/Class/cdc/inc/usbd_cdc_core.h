/**
  ******************************************************************************
  * @file    usbd_cdc_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for the usbd_cdc_core.c file.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_CDC_CORE_H
#define __GD32F10X_USBD_CDC_CORE_H

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_CDC
  * @{
  */

/** @defgroup USBD_CDC_Exported_Defines
  * @{
  */
#define USB_CDC_CONFIG_DESC_SIZE                (67)
#define USB_CDC_DESC_SIZE                       (58)

#define CDC_DESC_TYPE                           0x21

#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00

#define CDC_DATA_IN_PACKET_SIZE                 USB_CDC_DATA_PACKET_SIZE
#define CDC_DATA_OUT_PACKET_SIZE                USB_CDC_DATA_PACKET_SIZE

/* CDC Requests */
#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF

/**
  * @}
  */ 


/** @defgroup USBD_CDC_Exported_Types_Definitions
  * @{
  */
typedef struct _CDC_IF_FOP
{
    uint16_t (*pIf_Init)     (void);
    uint16_t (*pIf_DeInit)   (void);
    uint16_t (*pIf_Ctrl)     (uint32_t Cmd, uint8_t* CmdBuf, uint32_t Len);
		uint16_t (*pIf_DataTx)   (uint8_t* DataBuf, uint32_t Len);
		uint16_t (*pIf_DataRx)   (uint8_t* DataBuf, uint32_t Len);
}
CDC_IF_Prop_TypeDef;

/**
  * @}
  */

/** @defgroup USBD_CDC_Exported_Variables
  * @{
  */
extern USBD_Class_cb_TypeDef  USBD_CDC_cb;

/**
  * @}
  */

#endif  /* __GD32F10X_USB_CDC_CORE_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
