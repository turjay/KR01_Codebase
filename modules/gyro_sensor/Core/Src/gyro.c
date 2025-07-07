#include "gyro.h"
#include "main.h"
#include <string.h>

extern CAN_HandleTypeDef hcan1;

static CAN_FilterTypeDef gyro_filter;

void Gyro_Init(void) {
    // ID filtresi – örnek olarak 0x18FF50E2 (SAE J1939, PGN 0xFF50)
    gyro_filter.FilterBank = 0;
    gyro_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    gyro_filter.FilterScale = CAN_FILTERSCALE_32BIT;
    gyro_filter.FilterIdHigh = 0x18FF << 5; // upper bits of ID
    gyro_filter.FilterIdLow = 0x0000;
    gyro_filter.FilterMaskIdHigh = 0x1FFF << 5;
    gyro_filter.FilterMaskIdLow = 0x0000;
    gyro_filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    gyro_filter.FilterActivation = ENABLE;
    gyro_filter.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan1, &gyro_filter);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

HAL_StatusTypeDef Gyro_Read(GyroData_t* data) {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t rxData[8];

    if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0)
        return HAL_ERROR;

    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
        return HAL_ERROR;

    // raw değerler 2 byte signed, sırasıyla X, Y, Z
    int16_t gx = (rxData[0] << 8) | rxData[1];
    int16_t gy = (rxData[2] << 8) | rxData[3];
    int16_t gz = (rxData[4] << 8) | rxData[5];

    // datasheet’e göre çözünürlük < 0.1 dps, max range ±2000 dps
    data->gyro_x = gx * 0.1f;
    data->gyro_y = gy * 0.1f;
    data->gyro_z = gz * 0.1f;

    return HAL_OK;
}