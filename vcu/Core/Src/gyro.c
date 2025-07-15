#include "gyro.h"
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef* hadc_gyro = &hadc2;

float gyro_degrees[2];  // -90 ile +90 derece arasında maplenmiş değerler

#define GYRO_BIAS      2048    // Ortalama sabit nokta (örnek)
#define GYRO_MAX_DEV   1024.0f // +/- 1024 sapma  → 1.25V civarı

void gyro_start(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adc_raw;

    // ADC2_IN4 -> PA4
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc_gyro, &sConfig);
    HAL_ADC_Start(hadc_gyro);
    HAL_ADC_PollForConversion(hadc_gyro, HAL_MAX_DELAY);
    adc_raw = HAL_ADC_GetValue(hadc_gyro);
    gyro_degrees[0] = ((int32_t)adc_raw - GYRO_BIAS) * (90.0f / GYRO_MAX_DEV);
    HAL_ADC_Stop(hadc_gyro);

    // ADC2_IN5 -> PA5
    sConfig.Channel = ADC_CHANNEL_5;
    HAL_ADC_ConfigChannel(hadc_gyro, &sConfig);
    HAL_ADC_Start(hadc_gyro);
    HAL_ADC_PollForConversion(hadc_gyro, HAL_MAX_DELAY);
    adc_raw = HAL_ADC_GetValue(hadc_gyro);
    gyro_degrees[1] = ((int32_t)adc_raw - GYRO_BIAS) * (90.0f / GYRO_MAX_DEV);
    HAL_ADC_Stop(hadc_gyro);
}