#include "steering_sensor.h"

#define MAX_ADC_VALUE     4095.0f
#define VREF              3.3f
#define SENSOR_MIN_VOLT   0.5f
#define SENSOR_MAX_VOLT   4.5f
#define SENSOR_FULL_ANGLE 360.0f

static ADC_HandleTypeDef* hadc_local;

void SteeringSensor_Init(ADC_HandleTypeDef* hadc) {
    hadc_local = hadc;
}

static float adc_to_voltage(uint32_t raw) {
    return ((float)raw / MAX_ADC_VALUE) * VREF;
}

static float voltage_to_angle(float voltage) {
    if (voltage < SENSOR_MIN_VOLT) voltage = SENSOR_MIN_VOLT;
    if (voltage > SENSOR_MAX_VOLT) voltage = SENSOR_MAX_VOLT;
    return ((voltage - SENSOR_MIN_VOLT) / (SENSOR_MAX_VOLT - SENSOR_MIN_VOLT)) * SENSOR_FULL_ANGLE;
}

void SteeringSensor_ReadAngles(float* angle1, float* angle2) {
    uint32_t adc_val1 = 0, adc_val2 = 0;

    ADC_ChannelConfTypeDef sConfig = {0};

    // OUT1 -> ADC1_IN0 (PA0)
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc_local, &sConfig);
    HAL_ADC_Start(hadc_local);
    HAL_ADC_PollForConversion(hadc_local, HAL_MAX_DELAY);
    adc_val1 = HAL_ADC_GetValue(hadc_local);
    HAL_ADC_Stop(hadc_local);

    // OUT2 -> ADC1_IN1 (PA1)
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    HAL_ADC_ConfigChannel(hadc_local, &sConfig);
    HAL_ADC_Start(hadc_local);
    HAL_ADC_PollForConversion(hadc_local, HAL_MAX_DELAY);
    adc_val2 = HAL_ADC_GetValue(hadc_local);
    HAL_ADC_Stop(hadc_local);

    *angle1 = voltage_to_angle(adc_to_voltage(adc_val1));
    *angle2 = voltage_to_angle(adc_to_voltage(adc_val2));
}
