#include "bpps.h"
#include <stdio.h>

// ---------------------- Statik Değişkenler ---------------------- //
static ADC_HandleTypeDef* bpps_hadc;  // BPPS ADC handle (harici ADC referansı)
static float bpps_offset = 0.0f;      // Başlangıçta kalibre edilen offset (MPa)
static uint8_t is_calibrated = 0;     // Kalibrasyon durumu (0: yapılmadı, 1: yapıldı)

// Fren bayrağı (ready2drive için kullanılacak)
bool brake_active = false;

/**
 * @brief BPPS modülünü başlatır ve offset kalibrasyonu yapar.
 *
 * @param hadc Brake Pressure Position Sensor için kullanılacak ADC handle.
 */
void BPPS_Init(ADC_HandleTypeDef* hadc)
{
    bpps_hadc = hadc;

    // Başlangıçta offset kalibrasyonu (araç frensiz durumda iken yapılmalı)
    uint32_t adc_val = 0;

    HAL_ADC_Start(bpps_hadc);
    if (HAL_ADC_PollForConversion(bpps_hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(bpps_hadc);
    }
    HAL_ADC_Stop(bpps_hadc);

    // ADC değeri voltaja dönüştürülür (0-4095 → 0-5V)
    float voltage = (adc_val / 4095.0f) * 5.0f;

    // Offset kalibrasyonu (sensör karakteristiği: 0-5V → 0-20 MPa)
    bpps_offset = voltage * 4.0f;
    is_calibrated = 1;
}

/**
 * @brief BPPS döngü fonksiyonu (ADC okuma, basınç hesaplama ve fren kontrol mantığı).
 */
void BPPS_Loop(void)
{
    if (!is_calibrated) return;

    uint32_t adc_val = 0;

    // ADC başlat ve okuma yap
    HAL_ADC_Start(bpps_hadc);
    if (HAL_ADC_PollForConversion(bpps_hadc, HAL_MAX_DELAY) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(bpps_hadc);
    }
    HAL_ADC_Stop(bpps_hadc);

    // ADC değeri voltaja çevrilir
    float voltage = (adc_val / 4095.0f) * 5.0f;

    // Basınç hesabı (0-5V → 0-20 MPa)
    float pressure = (voltage * 4.0f) - bpps_offset;

    if (pressure < 0.0f) pressure = 0.0f;

    // Fren eşiği kontrolü
    if (pressure > 3.0f) {
        brake_active = true;  // Fren basılı
    } else {
        brake_active = false; // Fren bırakıldı
    }
}
