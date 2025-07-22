/*
 * hall.c
 *
 *  Created on: Jul 22, 2025
 *      Author: turjay
 */

#include "hall.h"

#define UREF_VOLTAGE    2.5f
#define ADC_RESOLUTION  4096.0f
#define VREF            3.3f
#define IPN             400.0f

extern ADC_HandleTypeDef hadc1;

uint32_t adc_value = 0;
float current = 0.0f;

static 	float Read_Current_From_HTFS400P(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    adc_value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    float voltage = ((float)adc_value * VREF) / ADC_RESOLUTION;
    current = ((voltage - UREF_VOLTAGE) * IPN) / 1.25f;

    return current;
}

void HALL_Loop(void)
{
	  current = Read_Current_From_HTFS400P();
}
