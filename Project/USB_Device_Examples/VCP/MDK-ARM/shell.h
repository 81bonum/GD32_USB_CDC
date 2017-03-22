#include 	<stdint.h>
#include 	<stdlib.h>
#include 	<stdio.h>
#include	<string.h>		//стандартная библиотека работы со строками
#include "gd32f10x_can.h"

extern	char 		strhex[];
extern	char 		string[];

extern	CanRxMessage RxMessage;
extern 	CanTxMessage TxMessage;


void config_adapter(void);
void	printcan1(void);
void Delay(uint32_t nTime);
void initargv(void);
int	selectargv(char *str);
int	putinbuffer(uint8_t	data);