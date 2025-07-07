#ifndef STEERING_SENSOR_H
#define STEERING_SENSOR_H

#include "main.h"

void SteeringSensor_Init(ADC_HandleTypeDef* hadc);
void SteeringSensor_ReadAngles(float* angle1, float* angle2);

#endif
