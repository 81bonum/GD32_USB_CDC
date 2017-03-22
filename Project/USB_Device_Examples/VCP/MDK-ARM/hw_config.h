#ifndef __LED
#define __LED

#define LED_PORT 					GPIOA
#define LED_G 						GPIO_PIN_4
#define LED_Y							GPIO_PIN_5
#define PORTA_PERIPH			RCC_APB2PERIPH_GPIOA


void LEDs_init(void);

#endif //__LED
