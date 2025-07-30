#include "nextion.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;
extern uint16_t pwm;

void Nextion_SendCommand(char* cmd) {
    uint8_t end_cmd[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, end_cmd, 3, HAL_MAX_DELAY);
}

void SCREEN_Loop() {
    char screen_buffer[256];

    // Update the PWM value on t9 text field only once per call
    snprintf(screen_buffer, sizeof(screen_buffer), "t9.txt=\"%d\"", (int)pwm);
    Nextion_SendCommand(buffer);
}
