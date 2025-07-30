#include "apps.h"
#include "nextion.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <math.h>

// Harici timer ve ADC tanımları
extern TIM_HandleTypeDef htim4;  // Zaman ölçümü için kullanılan timer
extern TIM_HandleTypeDef htim3;  // PWM çıkışı için kullanılan timer
extern ADC_HandleTypeDef hadc1;  // APPS sensörleri için ADC

// Global değişkenler
static float norm1 = 0.0f;                   // APPS1 normalize değeri (%)
uint32_t adcdata[ADC_CHANNEL_COUNT];         // ADC DMA buffer
uint16_t pwm[1];                             // PWM yüzdesi (%)

// Hata ve filtreleme bayrakları
static bool diff_flag = false;               // Sensör farkı izleme bayrağı
static bool permanent_fault = false;         // Kalıcı hata bayrağı
static uint32_t diff_start_time = 0;         // Fark oluşum başlangıç zamanı (ms veya timer count)
static float filtered_pwm = 0.0f;            // Filtrelenmiş PWM değeri

// Sensör kalibrasyon min/max değerleri (ADC raw)
static uint16_t sensor1_min = 1280, sensor1_max = 4095;  // APPS1
static uint16_t sensor2_min = 640,  sensor2_max = 1980;  // APPS2

/**
 * @brief ADC değerini %0 - %100 aralığına map eder ve clamp uygular.
 * @param x       ADC değeri (raw)
 * @param in_min  ADC minimum kalibrasyon değeri
 * @param in_max  ADC maksimum kalibrasyon değeri
 * @return float  Normalize edilmiş pedal değeri (%)
 */
static float map_clamped(int32_t x, int32_t in_min, int32_t in_max)
{
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    return ((float)(x - in_min) / (in_max - in_min)) * 100.0f;
}

/**
 * @brief APPS modülünü başlatır (ADC DMA ve PWM başlatılır).
 */
void APPS_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, adcdata, ADC_CHANNEL_COUNT);  // ADC DMA başlat
    HAL_TIM_Base_Start(&htim4);                             // Zaman ölçümü için timer başlat
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);               // PWM çıkışı başlat
    apps_enabled = true;                                    // APPS modülü aktif
}

/**
 * @brief APPS ana döngüsü (pedal okumaları, hata kontrolü, PWM çıkışı).
 */
void APPS_Loop(void)
{
    // Timer post-init (CubeMX'in kritik gerekliliği)
    HAL_TIM_MspPostInit(&htim3);

    // APPS devre dışı ise PWM sıfırlanır
    if (!apps_enabled) {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
        return;
    }

    // ADC okumalarını normalize et
    norm1 = map_clamped(adcdata[0], sensor1_min, sensor1_max);  // APPS1
    float norm2 = map_clamped(adcdata[1], sensor2_min, sensor2_max); // APPS2

    // Pedal alt eşiği (noise engellemek için %10 altını sıfırla)
    if (norm1 < 10.0f) norm1 = 0.0f;
    if (norm2 < 10.0f) norm2 = 0.0f;

    // %0-%100 aralığında clamp
    norm1 = fminf(fmaxf(norm1, 0.0f), 100.0f);
    norm2 = fminf(fmaxf(norm2, 0.0f), 100.0f);

    // Sensör farkını kontrol et
    float diff = fabsf(norm1 - norm2);
    uint32_t now = __HAL_TIM_GET_COUNTER(&htim4); // Timer sayacı

    if (diff > 10.0f) {  // Sensörler arasında %10'dan fazla fark varsa
        if (!diff_flag) {
            diff_flag = true;
            diff_start_time = now;  // Fark başladığı zamanı kaydet
        } else {
            // Timer taşmasını hesaba kat
            uint32_t elapsed = (now >= diff_start_time)
                             ? (now - diff_start_time)
                             : (0xFFFFFFFF - diff_start_time + now + 1);

            // Fark 100ms sürerse kalıcı hata
            if (elapsed >= 100) {
                permanent_fault = true;
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0); // PWM kes
                return;
            }
        }
    } else {
        diff_flag = false; // Fark düzeldiyse bayrağı sıfırla
    }

    // Kalıcı hata yoksa PWM hesapla
    if (!permanent_fault) {
        float norm1_scaled = norm1 / 100.0f;

        // Logaritmik pedal tepkisi
        float log_pwm = log10f(9.0f * norm1_scaled + 1.0f);
        float pwm_raw = log_pwm * 49.0f; // 0-49 arası PWM değeri

        // Low-pass filtre uygula
        const float alpha = 0.1f;
        filtered_pwm = (alpha * pwm_raw) + ((1.0f - alpha) * filtered_pwm);

        // Yuvarla
        uint32_t smooth_pwm = (uint32_t)(filtered_pwm + 0.5f);

        // Alt-üst limitler
        if (smooth_pwm < 3) smooth_pwm = 0;
        if (smooth_pwm > 48) smooth_pwm = 49;

        // PWM çıkışı uygula
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, smooth_pwm);

        // PWM yüzdesini kaydet
        pwm[0] = smooth_pwm * 100 / 49;
    }
}

/**
 * @brief APPS modülünü devre dışı bırakır, PWM çıkışını kapatır.
 */
void APPS_Deinit(void)
{
    HAL_ADC_Stop_DMA(&hadc1);                        // ADC durdur
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);         // PWM durdur

    // PWM pini (PA6) GPIO output moduna alınır ve LOW çekilir
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Kesinlikle 0V çıkış

    // Değişkenleri sıfırla
    adcdata[0] = 0;
    adcdata[1] = 0;
    filtered_pwm = 0.0f;
}

/**
 * @brief APPS kalıcı hata durumunu döndürür.
 * @return true: Kalıcı hata var, false: Hata yok.
 */
bool APPS_IsPermanentFault(void)
{
    return permanent_fault;
}
