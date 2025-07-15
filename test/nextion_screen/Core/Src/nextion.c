#include "nextion.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"


int data_numbers[7] = {12, 45, 78, 3, 89, 23, 56};  // Sadece tanımlama
char* data_strings[3] = {"a", "b", "c"};
const char* address[10] = {"n1", "n2", "n3", "n4", "n5", "n6", "n7", "t4", "t9", "t10"};

extern UART_HandleTypeDef huart1;  // UART tanımlı olmalı

// Text bileşenine metin gönderme
void Nextion_SendText(UART_HandleTypeDef *huart, const char* component, const char* text) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s.txt=\"%s\"", component, text);
    HAL_UART_Transmit(huart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

    uint8_t end_cmd[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(huart, end_cmd, 3, HAL_MAX_DELAY);
}

// Tüm verileri döngüyle ekrana gönder
void Nextion_Loop() {
    char number_str[10];

    for (int i = 0; i < 10; i++) {
        if (i < 7) {
            snprintf(number_str, sizeof(number_str), "%d", data_numbers[i]);
            Nextion_SendText(&huart1, address[i], number_str);
        } else {
            Nextion_SendText(&huart1, address[i], data_strings[i - 7]);
        }
    }
}
