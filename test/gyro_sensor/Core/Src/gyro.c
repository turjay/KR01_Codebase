#include "gyro.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;

// Örnek: PGN = 0xFF50 (gyroscope)
// Node ID = 0xE2 (226) → 0x18FF50E2
#define GYRO_CAN_ID 0x18FF50E2

void Gyro_Init(void)
{
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = (GYRO_CAN_ID >> 13) & 0xFFFF;
    filter.FilterIdLow  = (GYRO_CAN_ID << 3)  & 0xFFFF;
    filter.FilterMaskIdHigh = 0x1FFF; // 29-bit mask
    filter.FilterMaskIdLow  = 0xFFF8;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);
    HAL_CAN_Start(&hcan1);
}

HAL_StatusTypeDef Gyro_Read(GyroData_t *gyro)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];

    if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0)
        return HAL_ERROR;

    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &header, data) != HAL_OK)
        return HAL_ERROR;

    if (header.ExtId != GYRO_CAN_ID || header.IDE != CAN_ID_EXT)
        return HAL_ERROR;

    int16_t raw_x = (int16_t)(data[0] | (data[1] << 8));
    int16_t raw_y = (int16_t)(data[2] | (data[3] << 8));
    int16_t raw_z = (int16_t)(data[4] | (data[5] << 8));

    // Datasheet'e göre çözünürlük: 0.1 dps
    gyro->x = raw_x * 0.1f;
    gyro->y = raw_y * 0.1f;
    gyro->z = raw_z * 0.1f;

    return HAL_OK;
}