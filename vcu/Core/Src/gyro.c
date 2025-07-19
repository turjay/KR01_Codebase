#include "gyro.h"
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc2;

ADC_HandleTypeDef* hadc_gyro = &hadc2;

uint16_t gyrodata[2];

void GYRO_Loop(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    // ADC2_IN4 -> PA4
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc_gyro, &sConfig);
    HAL_ADC_Start(hadc_gyro);
    HAL_ADC_PollForConversion(hadc_gyro, HAL_MAX_DELAY);
    gyrodata[0] = HAL_ADC_GetValue(hadc_gyro);
    HAL_ADC_Stop(hadc_gyro);

    // ADC2_IN5 -> PA5
    sConfig.Channel = ADC_CHANNEL_5;
    HAL_ADC_ConfigChannel(hadc_gyro, &sConfig);
    HAL_ADC_Start(hadc_gyro);
    HAL_ADC_PollForConversion(hadc_gyro, HAL_MAX_DELAY);
    gyrodata[1] = HAL_ADC_GetValue(hadc_gyro);
    HAL_ADC_Stop(hadc_gyro);
}
