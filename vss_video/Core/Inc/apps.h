#ifndef __APPS_H
#define __APPS_H

#include <stdbool.h>
#include <stdint.h>

#define ADC_CHANNEL_COUNT 2

// extern float norm1;

extern uint32_t adcdata[ADC_CHANNEL_COUNT];
extern bool apps_enabled;

void APPS_Init(void);
void APPS_Loop(void);
void APPS_Deinit(void);
bool APPS_IsPermanentFault(void);

#endif
