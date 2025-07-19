/**
  ******************************************************************************
  * @file           : ready2drive.h
  * @brief          : Header for Ready to Drive system
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 KOU Racing.
  * All rights reserved.
  *
  * This software is licensed under MIT LICENSE.
  *
  ******************************************************************************
  */

#ifndef __READY2DRIVE_H
#define __READY2DRIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// RTD sistemini başlatır ve döngü içinde çağrılır
void R2D_Loop(void);

// Global flag'ler
extern bool apps_enabled;
extern bool buzzer_beep;

// UART veri tamponu
extern char uart_buf[100];

// ADC ve Gyro veri dizileri
extern uint32_t adcdata[2];
extern uint16_t gyrodata[2];

#ifdef __cplusplus
}
#endif

#endif /* __READY2DRIVE_H */
