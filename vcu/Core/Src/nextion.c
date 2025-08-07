#include "nextion.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "apps.h"
#include <string.h>
#include <stdio.h>

// ============================
// External UART handle
// ============================

extern UART_HandleTypeDef huart2;  // Nextion display connected to USART2

// ============================
// Private Constants
// ============================

/**
 * @brief End-of-command sequence required by Nextion protocol.
 */
static const uint8_t NEXTION_END_CMD[3] = {0xFF, 0xFF, 0xFF};

// ============================
// Function Implementations
// ============================

/**
 * @brief Sends a raw command string to the Nextion display.
 *
 * Appends 0xFF 0xFF 0xFF after the command to indicate end of command.
 *
 * @param cmd Null-terminated command string (e.g., "t0.txt=\"Hello\"")
 */
void Nextion_SendCommand(const char *cmd)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, NEXTION_END_CMD, sizeof(NEXTION_END_CMD), HAL_MAX_DELAY);
}

/**
 * @brief Updates throttle percentage or RPM value on Nextion display (t9 field).
 *
 * This example uses field `t9.txt` for visualizing APPS value or RPM.
 * You may customize the label name based on your GUI layout.
 *
 * @param rpm The numeric value to display (interpreted as % or RPM).
 */
void Nextion_UpdateRPM(float rpm)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "t9.txt=\"%d\"", (int)rpm);
    Nextion_SendCommand(buffer);
}

/**
 * @brief Updates the RTD (Ready-To-Drive) status on the Nextion display.
 *
 * Updates field `t10.txt` with either "ON" or "OFF" based on system state.
 *
 * @param rtd_state True = RTD ON, False = RTD OFF
 */
void Nextion_UpdateR2D(bool rtd_state)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t10.txt=\"%s\"", rtd_state ? "ON" : "OFF");
    Nextion_SendCommand(buffer);
}
