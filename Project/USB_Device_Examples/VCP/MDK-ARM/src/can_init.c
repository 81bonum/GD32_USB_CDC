/**
  *****************************************************************************
  * @title		can.c
  * @platform	STM32F103
  * @author		Dmitry Ovcharenko (https://www.smartmode.info/)
  * @version	V1.0.0
  * @date		20.06.2016
  *
  * @brief		?????? CAN - ?????????????, ?????-????????
  *
  *******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2016 SmartMODE</center></h2>
*/


#include "gd32f10x.h"
#include "gd32f10x_gpio.h"
#include "gd32f10x_rcc.h"
#include "gd32f10x_can.h"
#include "gd32f10x_misc.h"
#include "can_init.h"
#include "shell.h"



	int	can1_resive0;				//������� ��� ������� � ����� � ����� �����������
	int	can1_resive1;				//������� ��� ������� � ����� � ����� �����������
	extern	uint8_t halfbyte_to_hexascii(uint8_t _halfbyte);
	extern uint16_t CDC_DataTx (uint8_t* Buf, uint32_t Len);
	extern void USB_Send_Data(uint8_t data);
	CanRxMessage msg_buf_can;
	CanTxMessage TxMessage;
	CanRxMessage RxMessage;

/**
  * @brief  ????????? ?????? ?????-??????, CAN
  * @note   ??? ??????? ????????????? ??? ????????? ????????????????? ????? STM32F103.
  * @param  None
  * @retval None
  */
void init_CAN(void)
{
	GPIO_InitPara GPIO_InitStructure;

	/* CAN GPIOs configuration */
	RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE); 		// ???????? ???????????? AFIO
	RCC_APB2PeriphClock_Enable(CAN1_Periph, ENABLE); 				// ???????? ???????????? ?????

	RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_CAN1, ENABLE);		// ???????? ???????????? CAN-????

	// ??????????? CAN RX pin
	GPIO_InitStructure.GPIO_Pin   = CAN1_RX_SOURCE;
	GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

	// ??????????? CAN TX pin
	GPIO_InitStructure.GPIO_Pin   = CAN1_TX_SOURCE;
	GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

	#ifdef CAN1_ReMap
		GPIO_PinRemapConfig(GPIO_REMAP1_CAN1 , ENABLE);			// �������� Can1 �� PB8, PB9
	#endif

	// ????????????? ????
	CAN_InitPara CAN_InitStructure;

	CAN_DeInit( CAN1);
	CAN_StructInit(&CAN_InitStructure);

	// CAN cell init
	CAN_InitStructure.CAN_TTC = DISABLE;
	CAN_InitStructure.CAN_ABOR = DISABLE;
	CAN_InitStructure.CAN_AWK = DISABLE;
	CAN_InitStructure.CAN_ARD = ENABLE; // ������������� ��� ��������
	CAN_InitStructure.CAN_RFOD = DISABLE;
	CAN_InitStructure.CAN_TFO = DISABLE;
	CAN_InitStructure.CAN_Mode = MODE;	// ����� ������ ���������� � can_init.h
	CAN_InitStructure.CAN_SJW = CAN_SJW_1TQ;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_10TQ;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5TQ;
	CAN_InitStructure.CAN_Prescaler = CAN1_SPEED_PRESCALE;	// ������������ ���������� � can_init.h
	CAN_Init(CAN1, &CAN_InitStructure);

	// CAN filter init
	CAN_FilterInitPara CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FILTERMODE_MASK;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterInitStructure.CAN_FilterListHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterListLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskListHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskListLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssociation = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterWork = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// CAN FIFO0 message pending interrupt enable
	CAN_INTConfig(CAN1, CAN_INT_RFNE0, ENABLE);

	// NVIC Configuration
	// Enable CAN1 RX0 interrupt IRQ channel
	NVIC_InitPara NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQ = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
	NVIC_InitStructure.NVIC_IRQSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  ���������� �� ������ � CAN
  * @note   None
  * @param  None
  * @retval None
  */


void CAN1_RX0_IRQHandler(void)
{
	RxMessage.DLC = 	0x00;
	RxMessage.ExtId = 	0x00;
	RxMessage.FF = 	0x00;
	RxMessage.FT = 	0x00;
	RxMessage.FI = 	0x00;
	RxMessage.StdId = 	0x00;
	RxMessage.Data [0] = 0x00;
	RxMessage.Data [1] = 0x00;
	RxMessage.Data [2] = 0x00;
	RxMessage.Data [3] = 0x00;
	RxMessage.Data [4] = 0x00;
	RxMessage.Data [5] = 0x00;
	RxMessage.Data [6] = 0x00;
	RxMessage.Data [7] = 0x00;
	
	if (CAN_GetIntBitState(CAN1, CAN_INT_RFNE0) != RESET)		// 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

		if (RxMessage.FF == CAN_FF_STANDARD)
		{
//			if (RxMessage.StdId == CAN_CMD_Test_Send)		// 
//			{
//				CAN_Send_Ok();
//			}
			if (RxMessage.StdId == CAN_CMD_Test_Ok)			// 
			{
				GPIOA->DOR^= GPIO_DOR_DOR5;
				can1_resive0 = 1;
			}
		}
	}
}

/**
  * @brief  ???????? ???????? ??????? ? ????
  * @note   ?????????? ??? ???????? ?????
  * @param  None
  * @retval None
  */


void CAN_Send_Test(void)
{

	TxMessage.StdId = CAN_CMD_Test_Send;			// ??????? ????

	TxMessage.ExtId = 0x00;							// ??????????? ??????? ????????? ??? ??????

	TxMessage.FF = CAN_FF_STANDARD;				// ?????? ?????
	TxMessage.FT = CAN_FT_DATA;					// ??? ?????????
	TxMessage.DLC = 8;								// ????? ????? ?????? 3 - ????????? ??? ?????

	TxMessage.Data[0] = 0;				
	TxMessage.Data[1] = 1;				
	TxMessage.Data[2] = 2;						
	TxMessage.Data[3] = 3;					
	TxMessage.Data[4] = 4;						
	TxMessage.Data[5] = 5;						
	TxMessage.Data[6] = 6;						
	TxMessage.Data[7] = 7;						
	
	
	CAN_Transmit(CAN1, &TxMessage);
}


/**
  * @brief  ???????? ????????????? ????????? ???????? ??????? ? ????
  * @note   ?????????? ?????? ??????? ??? ??????
  * @param  None
  * @retval None
  */
void CAN_Send_Ok(void)
{
	CanTxMessage TxMessage;
	TxMessage.StdId = CAN_CMD_Test_Ok;				// ??????? ????

	TxMessage.ExtId = 0x00;							// ??????????? ??????? ????????? ??? ??????

	TxMessage.FF = CAN_FF_STANDARD;				// ?????? ?????
	TxMessage.FT = CAN_FT_DATA;					// ??? ?????????
	TxMessage.DLC = 0;								// ????? ????? ?????? 0 - ?????? ?? ????????

	CAN_Transmit(CAN1, &TxMessage);
}

