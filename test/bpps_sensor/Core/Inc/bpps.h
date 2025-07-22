#ifndef BPPS_H
#define BPPS_H

#include "stm32f4xx_hal.h"

void BPPS_Init(ADC_HandleTypeDef* hadc);
void BPPS_Loop(void);

#endif
