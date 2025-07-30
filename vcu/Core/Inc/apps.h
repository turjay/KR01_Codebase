/**
 ******************************************************************************
 * @file    apps.h
 * @brief   Accelerator Pedal Position Sensor (APPS) modülü başlık dosyası
 * @details APPS modülü, pedal sensörlerinden ADC verilerini okur,
 *          hata kontrolü yapar ve PWM çıkışı üretir.
 *          Ayrıca kalıcı hata (permanent fault) yönetimi içerir.
 ******************************************************************************
 */

#ifndef APPS_H
#define APPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* --------------------------- Makrolar --------------------------- */

// ADC kanal sayısı (pedal sensörleri için 2 kanal)
#define ADC_CHANNEL_COUNT   2

// APPS sensör toleransı (% fark)
#define APPS_DIFF_TOLERANCE 10.0f

// APPS sensör farkı hata süresi (ms veya timer count)
#define APPS_FAULT_DELAY    100

/* ------------------------- Dışa Açık Değişkenler ------------------------- */

/**
 * @brief ADC okuma buffer'ı (DMA ile doldurulur).
 *        adcdata[0] → APPS1, adcdata[1] → APPS2
 */
extern uint32_t adcdata[ADC_CHANNEL_COUNT];

/**
 * @brief PWM çıkış yüzdesi (%0 - %100)
 */
extern uint16_t pwm[1];

/**
 * @brief APPS modülü aktiflik bayrağı.
 *        true: Modül aktif, false: Modül devre dışı.
 */
extern bool apps_enabled;

/* ------------------------ Fonksiyon Prototipleri ------------------------ */

/**
 * @brief APPS modülünü başlatır.
 *
 * - ADC DMA başlatılır.
 * - Zamanlayıcı ve PWM konfigüre edilir.
 */
void APPS_Init(void);

/**
 * @brief APPS ana döngü fonksiyonu.
 *
 * - ADC verilerini okur.
 * - Pedal sensörlerini normalize eder (%0-100).
 * - Sensör farkını kontrol eder (plausibility check).
 * - PWM çıkışını hesaplar ve uygular.
 * - Hata durumunda PWM'i keser.
 */
void APPS_Loop(void);

/**
 * @brief APPS modülünü durdurur ve PWM çıkışını keser.
 *
 * - ADC DMA durdurulur.
 * - PWM timer kapatılır.
 * - PWM pini (PA6) GPIO output olarak LOW seviyesine çekilir.
 */
void APPS_Deinit(void);

/**
 * @brief Kalıcı hata durumunu sorgular.
 *
 * @return true: APPS kalıcı hata durumu var.
 * @return false: APPS normal çalışıyor.
 */
bool APPS_IsPermanentFault(void);

#ifdef __cplusplus
}
#endif

#endif /* APPS_H */
