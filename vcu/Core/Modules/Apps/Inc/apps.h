#ifndef __APPS_H
#define __APPS_H

#include <stdbool.h>
#include <stdint.h>

#define ADC_CHANNEL_COUNT 2


extern uint32_t adcdata[ADC_CHANNEL_COUNT];

void APPS_Init(void);
void APPS_Loop(void);
bool APPS_IsPermanentFault(void);

#endif
