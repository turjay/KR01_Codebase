#include "apps.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <math.h>  // fabsf için

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;

uint32_t adcdata[ADC_CHANNEL_COUNT];

static bool diff_flag = false;
static bool permanent_fault = false;
static uint32_t diff_start_time = 0;

// Önceden atanacak sensör aralıkları
static uint16_t sensor1_min = 0, sensor1_max = 4095;
static uint16_t sensor2_min = 0, sensor2_max = 4095;

extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;

static uint32_t pwm_buffer[1] = {0}; // PWM Duty Buffer

void APPS_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, adcdata, ADC_CHANNEL_COUNT);
    HAL_TIM_Base_Start(&htim4);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, pwm_buffer, 1);
}

void APPS_Loop(void)
{
    int32_t val1 = adcdata[0];
    int32_t val2 = adcdata[1];

    float norm1 = (val1 - sensor1_min) / (float)(sensor1_max - sensor1_min);
    float norm2 = (val2 - sensor2_min) / (float)(sensor2_max - sensor2_min);

    if (norm1 < 0.0f) norm1 = 0.0f;
    if (norm1 > 1.0f) norm1 = 1.0f;
    if (norm2 < 0.0f) norm2 = 0.0f;
    if (norm2 > 1.0f) norm2 = 1.0f;

    float diff = fabsf(norm1 - norm2);

    uint32_t now = __HAL_TIM_GET_COUNTER(&htim4);

    if (diff > (TOLERANCE_PERCENT / 100.0f)) {
        if (!diff_flag) {
            diff_flag = true;
            diff_start_time = now;
        } else {
            uint32_t elapsed = (now >= diff_start_time) ? (now - diff_start_time) : (0xFFFFFFFF - diff_start_time + now);
            if (elapsed >= DIFF_TIME_MS) {
                permanent_fault = true;
            }
        }
    } else {
        diff_flag = false;
        permanent_fault = false;
    }

    // PWM duty güncelle (örnek: norm1 duty olarak)
    pwm_buffer[0] = (uint32_t)(norm1 * 999);  // 0–999 arası
}


bool APPS_IsPermanentFault(void)
{
    return permanent_fault;
}
