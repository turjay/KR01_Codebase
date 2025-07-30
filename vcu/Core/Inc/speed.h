#ifndef SPEED_H
#define SPEED_H

#include "stm32f4xx_hal.h"

// ===================== Hız Verisi Yapısı =====================
typedef struct {
    float speed_rpm;   /**< RPM (devir/dakika) */
    float speed_kph;   /**< Hız (km/h) */
} SPEED_Data;

// ===================== Fonksiyon Prototipleri =====================
void SPEED_Init(TIM_HandleTypeDef *htim);
void SPEED_SetWheelRadius(float radius_m);  /**< Teker yarıçapını ayarlar (m) */
void SPEED_Loop(void);
SPEED_Data SPEED_GetData(void);

#endif
