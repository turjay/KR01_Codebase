#ifndef __DATA_H__
#define __DATA_H__

#include "stm32f4xx_hal.h"

extern int data_numbers[7];
extern char* data_strings[3];

void Nextion_SendText(UART_HandleTypeDef *huart, const char* component, const char* text);
void Nextion_Loop(void);


#endif
