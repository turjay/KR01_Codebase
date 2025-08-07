#ifndef KELLY_H
#define KELLY_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================
// Public Variables
// ============================

/**
 * @brief Latest RPM value received from the Kelly motor controller.
 *
 * Updated via CAN interrupt.
 */
extern volatile uint16_t rpm;

/**
 * @brief Latest raw CAN data received from Kelly controller (8 bytes).
 */
extern volatile uint8_t kelly_data[8];

// ============================
// Function Prototypes
// ============================

/**
 * @brief Initializes Kelly CAN interface on CAN1 and starts reception.
 *
 * - Configures CAN filter to accept only RPM message (0x0CF11E05).
 * - Starts CAN peripheral (hcan1).
 * - Enables RX FIFO0 interrupt.
 */
void Kelly_Init(CAN_HandleTypeDef *hcan);

/**
 * @brief CAN RX FIFO0 interrupt callback (to be called from HAL).
 *
 * Extracts RPM value if message ID matches Kelly RPM PGN.
 *
 * @param hcan Pointer to the CAN handle which received the message
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif

#endif // KELLY_H
