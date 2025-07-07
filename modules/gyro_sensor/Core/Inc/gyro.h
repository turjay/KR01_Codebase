#ifndef GYRO_H
#define GYRO_H

#include "stm32f4xx_hal.h"

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
} GyroData_t;

void Gyro_Init(void);
HAL_StatusTypeDef Gyro_Read(GyroData_t* data);

#endif // GYRO_H
