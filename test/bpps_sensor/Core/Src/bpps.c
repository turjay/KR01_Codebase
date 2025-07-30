#include "bpps.h"
#include <stdio.h>

static ADC_HandleTypeDef* bpps_hadc;
static float bpps_offset = 0.0f;
static uint8_t is_calibrated = 0;

void BPPS_Init(ADC_HandleTypeDef* hadc) {
    bpps_hadc = hadc;

    // Başlangıçta offset kalibrasyonu
    uint32_t adc_val = 0;
    HAL_ADC_Start(bpps_hadc);
    if (HAL_ADC_PollForConversion(bpps_hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(bpps_hadc);
    }
    HAL_ADC_Stop(bpps_hadc);

    float voltage = (adc_val / 4095.0f) * 5.0f;
    bpps_offset = voltage * 4.0f;  // MPa olarak offset
    is_calibrated = 1;
}

void BPPS_Loop(void) {
    if (!is_calibrated) return;

    uint32_t adc_val = 0;
    HAL_ADC_Start(bpps_hadc);
    if (HAL_ADC_PollForConversion(bpps_hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(bpps_hadc);
    }
    HAL_ADC_Stop(bpps_hadc);

    float voltage = (adc_val / 4095.0f) * 5.0f;
    float pressure = (voltage * 4.0f) - bpps_offset;

    if (pressure < 0.0f) pressure = 0.0f;

    // Burada fren mantığını içeriye gömdük
    if (pressure > 3.0f) {
        // Örnek: frenleme aktifse LED yak, logla vs.
        // HAL_GPIO_WritePin(GPIOX, GPIO_PIN_Y, GPIO_PIN_SET);
    } else {
        // HAL_GPIO_WritePin(GPIOX, GPIO_PIN_Y, GPIO_PIN_RESET);
    }
    float brakePressure = BPPS_GetPressure();
}
