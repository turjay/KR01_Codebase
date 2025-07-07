#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "main.h"

#define ADC_BUFFER_SIZE 10

typedef struct {
    uint32_t timestamp_ms;
    uint16_t apps_value;
    uint16_t bpps_value;
    uint16_t brake_pressure;
    uint16_t steering_angle;
    uint16_t motor_temp;
} VehicleData_t;

extern VehicleData_t carData;

void Datalogger_Init(ADC_HandleTypeDef* hadc);
void Datalogger_Process(void);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

#endif // DATALOGGER_H
