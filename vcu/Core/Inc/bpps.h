/**
 ******************************************************************************
 * @file    bpps.h
 * @brief   Brake Pedal Pressure Sensor (BPPS) başlık dosyası
 * @details BPPS modülü, fren pedal basıncını ADC üzerinden okur,
 *          offset kalibrasyonu yapar ve fren durumunu (brake_active) belirler.
 ******************************************************************************
 */

#ifndef BPPS_H
#define BPPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

/* ------------------------ Dışa Açık Değişkenler ------------------------ */

/**
 * @brief Fren pedalının aktif olup olmadığını belirten bayrak.
 *        true: Fren basılı, false: Fren bırakıldı.
 */
extern bool brake_active;

/* ------------------------ Fonksiyon Prototipleri ------------------------ */

/**
 * @brief BPPS modülünü başlatır ve offset kalibrasyonu yapar.
 * @param hadc Kullanılacak ADC handle.
 */
void BPPS_Init(ADC_HandleTypeDef* hadc);

/**
 * @brief BPPS ana döngü fonksiyonu (basınç okuma ve fren durumu güncelleme).
 */
void BPPS_Loop(void);

#ifdef __cplusplus
}
#endif

#endif /* BPPS_H */
