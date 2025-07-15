/**
  ******************************************************************************
  * @file           : ready2drive.c
  * @brief          : Initialize Ready to Drive system
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

#include "ready2drive.h"
#include "main.h"
#include "apps.h"
#include "gyro.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

static char uart_buf[64];

void Ready2Drive(void) {        
    gyro_start();
    GPIO_PinState brake_pressed = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
    GPIO_PinState rtd_button    = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);

    // RTD butonuna basıldı ve fren de basılıysa APPS aktif edilir
    if (brake_pressed == GPIO_PIN_SET && rtd_button == GPIO_PIN_SET && !apps_enabled) {
    apps_enabled = true;
    APPS_Init();

        if (!buzzer_beep) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // Buzzer ötsün
            HAL_Delay(3000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Buzzer sussun
            buzzer_beep = true;
        }
    }

    // RTD butonu tekrar kapatılırsa APPS'i devreden çıkar
    if (apps_enabled && rtd_button == GPIO_PIN_RESET) {
    apps_enabled = false;
    buzzer_beep = false;
    APPS_Deinit(); // Hem ADC hem PWM durur
    }

    if (apps_enabled) {
    APPS_Loop();  // Sadece APPS aktifse çalışsın
    }

  int gaz1 = APPS_GetGaz1Percent();
  int gaz2 = APPS_GetGaz2Percent();

    // UART'a APPS ve Gyro verisi gönderilerek R2D test edilir
    snprintf(uart_buf, sizeof(uart_buf),
         "APPS: %d, %d | GYRO: %.1f, %.1f\r\n",
          gaz1, gaz2, gyro_degrees[0], gyro_degrees[1]);

    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    HAL_Delay(300);
}