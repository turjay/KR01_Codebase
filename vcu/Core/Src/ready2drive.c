#include "ready2drive.h"
#include "main.h"
#include "apps.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart2;

GPIO_PinState prev_rtd_button = GPIO_PIN_SET;
GPIO_PinState rtd_button;

char uart_buf[100];
bool rtd_active = false;
bool apps_enabled = false;
bool buzzer_on = false;

void R2D_Loop(void) {
    rtd_button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    GPIO_PinState brake_pressed = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);

    if (prev_rtd_button == GPIO_PIN_SET && rtd_button == GPIO_PIN_RESET) {
        if (!rtd_active && brake_pressed == GPIO_PIN_SET) {
            rtd_active = true;
            apps_enabled = true;
            APPS_Init();
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // Buzzer ON
            HAL_Delay(2000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        } else if (rtd_active) {
            rtd_active = false;
            apps_enabled = false;
            APPS_Deinit();
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Buzzer OFF
        }
    }

    prev_rtd_button = rtd_button;

    if (apps_enabled) {
        APPS_Loop();
    }

    if (brake_pressed == GPIO_PIN_SET)
        HAL_UART_Transmit(&huart2, (uint8_t*)"Brake ON\r\n", 10, HAL_MAX_DELAY);
    if (rtd_button == GPIO_PIN_RESET)
        HAL_UART_Transmit(&huart2, (uint8_t*)"RTD ON\r\n", 9, HAL_MAX_DELAY);

    snprintf(uart_buf, sizeof(uart_buf),
             "APPS: %lu, %lu\r\n", adcdata[0], adcdata[1]);
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

    HAL_Delay(300);
}
