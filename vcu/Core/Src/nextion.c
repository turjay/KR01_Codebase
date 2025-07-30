#include "nextion.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern UART_HandleTypeDef huart2;

/**
 * @brief Nextion'a komut gönderir.
 * @param cmd Gönderilecek komut stringi (örn: "t0.txt=\"Hello\"")
 */
void Nextion_SendCommand(const char *cmd) {
    const uint8_t end_cmd[3] = {0xFF, 0xFF, 0xFF};  // Nextion terminatörü
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, end_cmd, 3, HAL_MAX_DELAY);
}

/**
 * @brief Ekranda SoC (State of Charge) bilgisini günceller.
 * @param soc Pil doluluk oranı (0-100 %)
 */
void Nextion_UpdateSoC(uint8_t soc) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t9.txt=\"%d%%\"", soc); // Örn: "85%"
    Nextion_SendCommand(buffer);
}

/**
 * @brief Ekranda Speed (Araç Hızı) bilgisini günceller.
 * @param speed Araç hızı (km/h cinsinden)
 */
void Nextion_UpdateSpeed(uint16_t speed) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t10.txt=\"%d km/h\"", speed);
    Nextion_SendCommand(buffer);
}

/**
 * @brief R2D (Ready-to-Drive) durumunu ekrana yazar.
 * @param rtd_state true: ON, false: OFF
 */
void Nextion_UpdateR2D(bool rtd_state) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "t11.txt=\"%s\"", rtd_state ? "ON" : "OFF");
    Nextion_SendCommand(buffer);
}
