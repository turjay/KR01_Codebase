/**
 ******************************************************************************
 * @file    speed.c
 * @brief   AGS200 Hall-effect hız sensörü sürücü kaynak dosyası
 ******************************************************************************
 * @details
 *  - Tek yönlü motor uygulamaları için hız ölçümü yapılır.
 *  - Quadrature çıkışın yalnızca Kanal A kullanılır.
 *  - Timer üzerinden darbe aralıkları ölçülerek frekans, ardından RPM ve km/h hesaplanır.
 ******************************************************************************
 */

#include "speed.h"
#include <math.h>

// ========================= Kullanıcı Tanımları ========================= //
#define PULSES_PER_REV   20   /**< Çark üzerindeki diş sayısı */

// ========================= Dahili Değişkenler ========================= //
static TIM_HandleTypeDef *speed_htim;     /**< Hız ölçümü için kullanılan Timer handle */
static SPEED_Data speed_data = {0};       /**< Sensörden elde edilen hız ve RPM verileri */
static uint32_t last_pulse_time = 0;      /**< Son darbenin algılandığı zaman (Timer sayaç değeri) */
static uint8_t last_A = 0;                /**< Kanal A'nın önceki durumu (kenar algılama için) */
static float wheel_radius_m = 0.3f;       /**< Teker yarıçapı (metre cinsinden, varsayılan 0.3m) */

// ========================= Harici GPIO Okuma Fonksiyonu ========================= //
// Sensörün Kanal A çıkışına bağlı GPIO pinini okur.
// Kullanıcı, sensör bağlantısına göre bu fonksiyonu projede tanımlamalıdır.
extern uint8_t READ_CHANNEL_A(void);

/**
 * @brief   Hız sensörünü başlatır.
 * @param   htim: Hız ölçümü için kullanılacak Timer handle.
 */
void SPEED_Init(TIM_HandleTypeDef *htim) {
    speed_htim = htim;
    HAL_TIM_Base_Start(htim);  /**< Timer'ı başlatır */
}

/**
 * @brief   Teker yarıçapını (metre cinsinden) ayarlar.
 * @param   radius_m: Teker yarıçapı (ör: 0.3m)
 */
void SPEED_SetWheelRadius(float radius_m) {
    wheel_radius_m = radius_m;
}

/**
 * @brief   Sensör sinyallerini okuyarak hız (RPM ve km/h) hesaplar.
 */
void SPEED_Loop(void) {
    uint8_t A = READ_CHANNEL_A();  /**< Kanal A'nın anlık durumunu oku */

    // ===================== Kenar Algılama =====================
    if (A != last_A) {  /**< A kanalında kenar değişimi olduysa */
        uint32_t now = __HAL_TIM_GET_COUNTER(speed_htim);   /**< Timer sayacını oku */
        uint32_t dt = now - last_pulse_time;                /**< İki darbe arası süre */
        last_pulse_time = now;

        // ===================== RPM ve Hız Hesaplama =====================
        if (dt > 0) {
            float freq = (float)HAL_RCC_GetPCLK1Freq() / dt;               /**< Frekans (Hz) */
            speed_data.speed_rpm = (freq * 60.0f) / PULSES_PER_REV;        /**< RPM hesabı */

            // RPM → km/h dönüşümü
            // Hız (km/h) = RPM × (Çevre (m) / 60) × 3.6
            float wheel_circumference = 2.0f * (float)M_PI * wheel_radius_m;
            speed_data.speed_kph = speed_data.speed_rpm * wheel_circumference / 60.0f * 3.6f;
        }
    }

    // ===================== Duruş Algılama =====================
    if ((__HAL_TIM_GET_COUNTER(speed_htim) - last_pulse_time) > (HAL_RCC_GetPCLK1Freq() / 2)) {
        speed_data.speed_rpm = 0;
        speed_data.speed_kph = 0.0f;
    }

    last_A = A;
}

/**
 * @brief   Hesaplanan hız verilerini döndürür.
 * @retval  SPEED_Data: RPM ve km/h bilgilerini içeren yapı.
 */
SPEED_Data SPEED_GetData(void) {
    return speed_data;
}
