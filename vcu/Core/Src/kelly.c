#include "kelly.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

// ============================
// Internal State
// ============================

static CAN_HandleTypeDef *kelly_hcan = NULL;  // Pointer to the used CAN interface

// ============================
// Constants
// ============================

/**
 * @brief Extended 29-bit CAN ID for RPM data from Kelly controller.
 */
#define KELLY_RPM_ID  0x0CF11E05

// ============================
// Public Variables
// ============================

/**
 * @brief Last received RPM value from Kelly controller.
 */
volatile uint16_t rpm = 0;

/**
 * @brief Raw CAN data buffer from Kelly controller.
 */
volatile uint8_t kelly_data[8] = {0};

// ============================
// Function Implementations
// ============================

/**
 * @brief Initializes the Kelly CAN interface and configures filter.
 *
 * - Configures filter to accept only RPM message (0x0CF11E05).
 * - Starts the CAN peripheral.
 * - Enables RX FIFO0 interrupt callback.
 *
 * @param hcan Pointer to initialized CAN handle (e.g., &hcan1 or &hcan2)
 */
void Kelly_Init(CAN_HandleTypeDef *hcan)
{
    kelly_hcan = hcan;

    // Configure CAN filter for only 0x0CF11E05
    CAN_FilterTypeDef filter = {0};
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14; // For dual CAN

    // Compute 32-bit ID and mask (shifted left by 3 for hardware register alignment)
    uint32_t id   = (KELLY_RPM_ID << 3);         // 0x0CF11E05 << 3 = 0x6788F028
    uint32_t mask = (0x1FFFFFFF << 3);           // Match all 29 bits

    filter.FilterIdHigh     = (id >> 16) & 0xFFFF;
    filter.FilterIdLow      = id & 0xFFFF;
    filter.FilterMaskIdHigh = (mask >> 16) & 0xFFFF;
    filter.FilterMaskIdLow  = mask & 0xFFFF;

    HAL_CAN_ConfigFilter(kelly_hcan, &filter);

    // Start CAN and enable RX interrupt
    HAL_CAN_Start(kelly_hcan);
    HAL_CAN_ActivateNotification(kelly_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
 * @brief CAN receive interrupt callback.
 *
 * This function is called when a message is pending in FIFO0.
 * It checks for a match with the Kelly RPM ID and updates the RPM value.
 *
 * @param hcan Pointer to the CAN handle which triggered the interrupt
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rxHeader;

    // Read the received CAN message
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, (uint8_t*)kelly_data) == HAL_OK)
    {
        // Check if the message is from the Kelly controller (RPM message)
        if (rxHeader.ExtId == KELLY_RPM_ID)
        {
            // Decode RPM from data[0] and data[1] (little-endian)
            rpm = ((uint16_t)kelly_data[1] << 8) | kelly_data[0];
        }
    }
}
