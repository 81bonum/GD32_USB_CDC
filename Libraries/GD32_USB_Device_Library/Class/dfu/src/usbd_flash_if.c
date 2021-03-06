/**
  ******************************************************************************
  * @file    usbd_flash_if.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB DFU device flash interface functions
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_flash_if.h"
#include "usbd_dfu_mal.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_DFU
  * @{
  */

/** @defgroup USBD_DFU_FlashIf_Private_FunctionPrototypes
  * @{
  */
static uint8_t  Flash_If_Init      (void);
static uint8_t  Flash_If_DeInit    (void);
static uint8_t  Flash_If_Erase     (uint32_t Addr);
static uint8_t  Flash_If_Write     (uint32_t Addr, uint32_t Len);
static uint8_t* Flash_If_Read      (uint32_t Addr, uint32_t Len);
static uint8_t  Flash_If_CheckAddr (uint32_t Addr);

/**
  * @}
  */

/** @defgroup USBD_DFU_FlashIf_Private_Variables
  * @{
  */
DFU_MAL_Property_TypeDef DFU_Flash_cb =
{
    (const uint8_t *)FLASH_IF_STRING,

    Flash_If_Init,
    Flash_If_DeInit,
    Flash_If_Erase,
    Flash_If_Write,
    Flash_If_Read,
    Flash_If_CheckAddr,
    60, /* Flash erase timeout in ms */
    80  /* Flash programming timeout in ms (80us * RAM Buffer size (1024 Bytes) */
};

/**
  * @}
  */

/** @defgroup USBD_DFU_FlashIf_Private_Functions
  * @{
  */

/**
  * @brief  Flash memory interface initialization routine.
  * @param  None
  * @retval MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t  Flash_If_Init (void)
{
    /* Unlock the internal flash */
    FMC_Unlock();

    return MAL_OK;
}

/**
  * @brief  Flash memory interface deinitialization routine.
  * @param  None
  * @retval MAL_OK if all operation is right, MAL_FAIL else
  */
static uint8_t  Flash_If_DeInit (void)
{
    /* Lock the internal flash */
    FMC_Lock();

    return MAL_OK;
}

/**
  * @brief  Erase flash sector
  * @param  Addr: address to be written to.
  * @retval MAL_OK
  */
static uint8_t  Flash_If_Erase (uint32_t Addr)
{
    /* Call the standard flash erase-page function */
    FMC_ErasePage(Addr);

    return MAL_OK;
}

/**
  * @brief  Flash memory write routine
  * @param  Addr: address to be written to
  * @param  Len: length of data to be written (in bytes).
  * @retval MAL_OK
  */
static uint8_t  Flash_If_Write (uint32_t Addr, uint32_t Len)
{
    uint32_t idx = 0;

    if (Len & 0x03) /* Not an aligned data */
    {
        for (idx = Len; idx < ((Len & 0xFFFC) + 4); idx++)
        {
            MAL_Buffer[idx] = 0xFF;
        }
    }

    /* Data received are word multiple */
    for (idx = 0; idx <  Len; idx += 4)
    {
        FMC_ProgramWord(Addr, *(uint32_t *)(MAL_Buffer + idx));
        Addr += 4;
    }

    return MAL_OK;
}

/**
  * @brief  Flash memory read routine
  * @param  Addr: address to be read from
  * @param  Len: length of data to be read (in bytes)
  * @retval Pointer to the physical address where data should be read
  */
static uint8_t *Flash_If_Read (uint32_t Addr, uint32_t Len)
{
    return  (uint8_t *)(Addr);
}

/**
  * @brief  Check if the address is an allowed address for this memory
  * @param  Addr: address to be checked.
  * @retval MAL_OK if the address is allowed, MAL_FAIL else.
  */
static uint8_t  Flash_If_CheckAddr (uint32_t Addr)
{
    if ((Addr >= FLASH_START_ADDR) && (Addr < FLASH_END_ADDR))
    {
        return MAL_OK;
    }
    else
    {
        return MAL_FAIL;
    }
}

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

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
