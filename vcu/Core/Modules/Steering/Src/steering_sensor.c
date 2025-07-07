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
    return (raw / MAX_ADC_VALUE) * VREF;
}

static float voltage_to_angle(float voltage) {
    if (voltage < SENSOR_MIN_VOLT) voltage = SENSOR_MIN_VOLT;
    if (voltage > SENSOR_MAX_VOLT) voltage = SENSOR_MAX_VOLT;
    return ((voltage - SENSOR_MIN_VOLT) / (SENSOR_MAX_VOLT - SENSOR_MIN_VOLT)) * SENSOR_FULL_ANGLE;
}

void SteeringSensor_ReadAngles(float* angle1, float* angle2) {
    uint32_t adc_values[2];

    HAL_ADC_Start(hadc_local);

    for (int i = 0; i < 2; i++) {
        HAL_ADC_PollForConversion(hadc_local, HAL_MAX_DELAY);
        adc_values[i] = HAL_ADC_GetValue(hadc_local);
    }

    HAL_ADC_Stop(hadc_local);

    float voltage1 = adc_to_voltage(adc_values[0]);  // OUT1
    float voltage2 = adc_to_voltage(adc_values[1]);  // OUT2

    *angle1 = voltage_to_angle(voltage1);
    *angle2 = voltage_to_angle(voltage2);
}
