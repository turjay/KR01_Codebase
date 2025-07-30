/**
 ******************************************************************************
 * @file    ready2drive.h
 * @brief   Ready-to-Drive (R2D) sisteminin başlık dosyası
 * @details Bu modül, FSAE aracında "Ready-to-Drive" mantığını yönetir.
 *          R2D butonu, fren (BPPS) ve APPS entegrasyonu ile çalışır.
 ******************************************************************************
 */

#ifndef READY2DRIVE_H
#define READY2DRIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32f4xx_hal.h"

/* ------------------------ Dışa Açık Değişkenler ------------------------ */

/**
 * @brief R2D sisteminin aktiflik durumu.
 *        true: R2D aktif (araç sürüşe hazır), false: R2D kapalı.
 */
extern bool rtd_active;

/**
 * @brief APPS sisteminin aktiflik durumu.
 *        R2D aktif olduğunda otomatik olarak APPS de etkinleşir.
 */
extern bool apps_enabled;

/**
 * @brief Buzzer kontrol bayrağı.
 *        true: Buzzer açık, false: Buzzer kapalı.
 */
extern bool buzzer_on;

/* ------------------------ Fonksiyon Prototipleri ------------------------ */

/**
 * @brief Ready-to-Drive sistemini başlatır.
 *
 * - R2D butonunun başlangıç durumunu okur.
 * - Sistem başlatıldığında güvenli başlangıç sağlar.
 */
void R2D_Init(void);

/**
 * @brief Ready-to-Drive ana döngü fonksiyonu.
 *
 * - R2D butonuna basılmasını (düşen kenar) algılar.
 * - BPPS (fren) basılıyken R2D'yi aktif eder.
 * - APPS modülünü kontrol eder.
 * - Buzzer yönetimi yapar.
 */
void R2D_Loop(void);

#ifdef __cplusplus
}
#endif

#endif /* READY2DRIVE_H */
