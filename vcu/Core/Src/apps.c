#include "apps.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <math.h>  // fabsf için

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;

extern bool apps_enabled;

uint32_t adcdata[ADC_CHANNEL_COUNT];
uint8_t test=0;

static bool diff_flag = false;
static bool permanent_fault = false;
static uint32_t diff_start_time = 0;

// Önceden atanacak sensör aralıkları
static uint16_t sensor1_min = 1450, sensor1_max = 4095;
static uint16_t sensor2_min = 710, sensor2_max = 2280;

extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;

static uint32_t pwm_buffer[1] = {0}; // PWM Duty Buffer

void APPS_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, adcdata, ADC_CHANNEL_COUNT);
    HAL_TIM_Base_Start(&htim4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void APPS_Loop(void) {
    if (!apps_enabled) {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);  // PWM'i sıfırla
        return;
    }

    int32_t val1 = adcdata[0];
    int32_t val2 = adcdata[1];

    // Normalize: 0–100 aralığına map
    float norm1 = (val1 - sensor1_min) * 100.0f / (sensor1_max - sensor1_min);
    float norm2 = (val2 - sensor2_min) * 100.0f / (sensor2_max - sensor2_min);

    // Clamp
    if (norm1 < 0.0f) norm1 = 0.0f;
    if (norm1 > 100.0f) norm1 = 100.0f;
    if (norm2 < 0.0f) norm2 = 0.0f;
    if (norm2 > 100.0f) norm2 = 100.0f;

    float diff = fabsf(norm1 - norm2);
    uint32_t now = __HAL_TIM_GET_COUNTER(&htim4);

    // %10 fark tespiti
    if (diff > 10.0f) {
        if (!diff_flag) {
            diff_flag = true;
            diff_start_time = now;
        } else {
            uint32_t elapsed = (now >= diff_start_time)
                             ? (now - diff_start_time)
                             : (0xFFFFFFFF - diff_start_time + now + 1);

            if (elapsed >= 1000) { // 100ms
                permanent_fault = true;

                // PWM çıkışını kapat
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);

                // ADC DMA'yı durdur
                HAL_ADC_Stop_DMA(&hadc1);

                // ADC verisini sıfırla
                for (int i = 0; i < ADC_CHANNEL_COUNT; ++i)
                    adcdata[i] = 0;
            }
        }
    } else {
        diff_flag = false;
    }

    // Kalıcı hata yoksa PWM duty ayarla
    if (!permanent_fault) {
        pwm_buffer[0] = (uint32_t)norm1;

        // Alt ve üst sınır koruması
        if (pwm_buffer[0] < 2)
            pwm_buffer[0] = 0;
        else if (pwm_buffer[0] > 98)
            pwm_buffer[0] = 100;

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_buffer[0]);
    }
}

void APPS_Deinit(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    adcdata[0] = 0;
    adcdata[1] = 0;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}

bool APPS_IsPermanentFault(void)
{
    return permanent_fault;
}