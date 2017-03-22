#include "gd32f10x.h"
#include "gd32f10x_gpio.h"
#include "gd32f10x_rcc.h"
#include "hw_config.h"


void LEDs_init(void) {
	
	RCC->APB2CCR |= RCC_APB2CCR_PAEN;
//	RCC->APB1CCR |= RCC_APB1CCR_TIMER6EN; 
	
	GPIOA->CTLR1 &= GPIO_CTLR1_CF4;
	GPIOA->CTLR1 &= GPIO_CTLR1_CF5;
	
	GPIOA->CTLR1 |= GPIO_CTLR1_MD4_0;
	GPIOA->CTLR1 |= GPIO_CTLR1_MD5_0;
//	GPIO_InitPara GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = LED_G | LED_Y;
//	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_10MHZ;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
