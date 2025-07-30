#include "apps.h"
#include <stdlib.h>
#include <math.h>  // fabsf ve log10f için

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;

bool apps_enabled = false;
float log_pwm = 0.0f;
float norm1 = 0.0f;

uint32_t adcdata[ADC_CHANNEL_COUNT] = {0};

static bool diff_flag = false;
static bool permanent_fault = false;
static uint32_t diff_start_time = 0;

// Sensör aralıkları
static uint16_t sensor1_min = 1280, sensor1_max = 4095;
static uint16_t sensor2_min = 640,  sensor2_max = 1980;

static uint32_t pwm_buffer[1] = {0}; // PWM Duty Buffer

// Değeri 0–100 aralığına sıkıştır ve normalize et
static float map_clamped(int32_t x, int32_t in_min, int32_t in_max)
{
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;

    return ((float)(x - in_min) / (in_max - in_min)) * 100.0f;
}

void APPS_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, adcdata, ADC_CHANNEL_COUNT);
    HAL_TIM_Base_Start(&htim4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    apps_enabled = true;
}

void APPS_Loop(void)
{
    if (!apps_enabled)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        return;
    }

    norm1 = map_clamped(adcdata[0], sensor1_min, sensor1_max);
    float norm2 = map_clamped(adcdata[1], sensor2_min, sensor2_max);

    // Güvenli sınırlama (0–100 arası)
    if (norm1 < 0.0f) norm1 = 0.0f;
    if (norm1 > 100.0f) norm1 = 100.0f;
    if (norm2 < 0.0f) norm2 = 0.0f;
    if (norm2 > 100.0f) norm2 = 100.0f;

    float diff = fabsf(norm1 - norm2);
    uint32_t now = __HAL_TIM_GET_COUNTER(&htim4);

    if (false) {  // çünkü artık farklar 0-1 değil, 0–100 arası
        if (!diff_flag) {
            diff_flag = true;
            diff_start_time = now;
        } else {
            uint32_t elapsed = (now >= diff_start_time)
                ? (now - diff_start_time)
                : (0xFFFFFFFF - diff_start_time + now + 1);

            if (false) {
                permanent_fault = true;
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
            }
        }
    } else {
        diff_flag = false;
    }

    if (!permanent_fault) {
        float norm1_scaled = norm1 / 100.0f;  // log hesaplaması için 0–1'e indir
        log_pwm = log10f(9.0f * norm1_scaled + 1.0f); // log(1)=0, log(10)=1

        pwm_buffer[0] = (uint32_t)(log_pwm * 49);    // 0–49 duty aralığı

        if (pwm_buffer[0] < 1) {
            pwm_buffer[0] = 0;
        } else if (pwm_buffer[0] > 48) {
            pwm_buffer[0] = 49;
        }

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_buffer[0]);
    }
}

void APPS_Deinit(void)
{
    HAL_ADC_Stop_DMA(&hadc1);

    // PWM'i durdur
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    // PA6'yı GPIO output olarak yeniden yapılandır
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA6'yı LOW yap → %100 kesinlikle 0V
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

    adcdata[0] = 0;
    adcdata[1] = 0;
    filtered_pwm = 0.0f;
}

bool APPS_IsPermanentFault(void)
{
    return permanent_fault;
}
