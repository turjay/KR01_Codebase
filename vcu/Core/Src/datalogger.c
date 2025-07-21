// SONRA HALLEDİLECEK

/*
#include "datalogger.h"
#include <stdio.h>

uint16_t adc_dma_buffer[ADC_BUFFER_SIZE];
VehicleData_t carData;
static volatile uint8_t buffer_ready = 0;
static ADC_HandleTypeDef* adc_handle;

void Datalogger_Init(ADC_HandleTypeDef* hadc) {
    adc_handle = hadc;
    HAL_ADC_Start_DMA(adc_handle, (uint32_t*)adc_dma_buffer, ADC_BUFFER_SIZE);
}

static uint16_t data[10];

static void ParseADCBuffer(uint8_t half) {
    uint16_t* data = &adc_dma_buffer[half == 1 ? 0 : 5];

    carData.apps_value       = data[0];
    carData.bpps_value       = data[1];
    carData.brake_pressure   = data[2];
    carData.steering_angle   = data[3];
    carData.motor_temp       = data[4];
    carData.timestamp_ms     = HAL_GetTick();
}

void Datalogger_Process(void) {
    if (buffer_ready != 0) {
        ParseADCBuffer(buffer_ready);
        buffer_ready = 0;

        // DEBUG: UART çıktısı
        printf("Time: %lu, APPS: %u, BPPS: %u, Brake: %u, Angle: %u, Temp: %u\r\n",
            carData.timestamp_ms,
            carData.apps_value,
            carData.bpps_value,
            carData.brake_pressure,
            carData.steering_angle,
            carData.motor_temp
        );
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == adc_handle->Instance) {
        buffer_ready = 1;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == adc_handle->Instance) {
        buffer_ready = 2;
    }
}
*/