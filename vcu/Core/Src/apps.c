#include "apps.h"
#include "nextion.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <math.h>

// External hardware handles
extern TIM_HandleTypeDef htim4;      // Timer for time measurement (used for diff timing)
extern TIM_HandleTypeDef htim3;      // Timer for PWM output
extern ADC_HandleTypeDef hadc1;      // ADC handle for pedal sensors

// Global variables
float norm1 = 0.0f;                  // Normalized value of pedal sensor 1
uint32_t adcdata[ADC_CHANNEL_COUNT]; // ADC readings buffer
uint16_t pwm[1];                     // PWM output percentage

bool diff_flag = false;              // Flag for APPS sensor difference detection
bool permanent_fault = false;        // Latched permanent fault flag
uint32_t diff_start_time = 0;        // Timer value when difference was first detected
float filtered_pwm = 0.0f;           // Filtered PWM signal (low-pass filtered)

// Sensor calibration values (raw ADC range)
static uint16_t sensor1_min = 1900, sensor1_max = 4095;
static uint16_t sensor2_min = 950,  sensor2_max = 2250;

/**
 * @brief Maps a value from input range to 0-100% and clamps it.
 */
static float map_clamped(int32_t x, int32_t in_min, int32_t in_max)
{
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    return ((float)(x - in_min) / (in_max - in_min)) * 100.0f;
}

/**
 * @brief Initializes APPS system: ADC DMA, timing, and PWM.
 */
void APPS_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, adcdata, ADC_CHANNEL_COUNT);  // Start ADC in DMA mode
    HAL_TIM_Base_Start(&htim4);                             // Start timer (for diff timing)
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);               // Start PWM output
    apps_enabled = true;
}

/**
 * @brief Main APPS loop: Reads sensors, checks plausibility, and updates PWM output.
 */
void APPS_Loop(void)
{
    HAL_TIM_MspPostInit(&htim3);  // Critical: ensure PWM pin is initialized properly

    // Disable throttle if APPS is disabled
    if (!apps_enabled) {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);  // Force PWM to 0
        return;
    }

    // Normalize pedal sensor values (0-100%)
    norm1 = map_clamped(adcdata[0], sensor1_min, sensor1_max);
    float norm2 = map_clamped(adcdata[1], sensor2_min, sensor2_max);

    // Apply deadzone: ignore noise below 10%
    if (norm1 < 10.0f) norm1 = 0.0f;
    if (norm2 < 10.0f) norm2 = 0.0f;

    // Clamp normalized values to [0%, 100%]
    norm1 = fminf(fmaxf(norm1, 0.0f), 100.0f);
    norm2 = fminf(fmaxf(norm2, 0.0f), 100.0f);

    // Calculate absolute difference between sensors
    float diff = fabsf(norm1 - norm2);
    uint32_t now = __HAL_TIM_GET_COUNTER(&htim4);  // Current timer count

    // Plausibility check: detect sustained sensor difference >10%
    if (diff > 10.0f) {
        if (!diff_flag) {
            diff_flag = true;
            diff_start_time = now;
        } else {
            // Calculate elapsed time with overflow handling
            uint32_t elapsed = (now >= diff_start_time)
                             ? (now - diff_start_time)
                             : (0xFFFFFFFF - diff_start_time + now + 1);

            // If difference persists for >100 ms → permanent fault
            if (elapsed >= 100) {
                permanent_fault = true;
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);  // Force throttle to 0
                return;
            }
        }
    } else {
        diff_flag = false;
    }

    // If no permanent fault, calculate PWM output
    if (!permanent_fault) {
        float norm1_scaled = norm1 / 100.0f;

        // Apply logarithmic scaling for smoother control near low throttle
        float log_pwm = log10f(9.0f * norm1_scaled + 1.0f);
        float pwm_raw = log_pwm * 49.0f;  // Scale to PWM steps (0-49)

        // Apply low-pass filter for smoothing (alpha = 0.1)
        const float alpha = 1.0f;
        filtered_pwm = (alpha * pwm_raw) + ((1.0f - alpha) * filtered_pwm);

        // Round and clamp PWM value
        uint32_t smooth_pwm = (uint32_t)(filtered_pwm + 0.5f);
        if (smooth_pwm < 3) smooth_pwm = 0;
        if (smooth_pwm > 48) smooth_pwm = 49;

        // Update PWM output
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, smooth_pwm);
        pwm[0] = smooth_pwm * 100 / 49;  // Store as percentage (0-100%)
    }
}

/**
 * @brief Deinitializes APPS: stops ADC, PWM, and resets output pin to 0V.
 */
void APPS_Deinit(void)
{
    HAL_ADC_Stop_DMA(&hadc1);                     // Stop ADC DMA
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);      // Stop PWM

    // Reconfigure PA6 as GPIO output (force 0V)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // Ensure output is LOW

    // Reset variables
    adcdata[0] = 0;
    adcdata[1] = 0;
    filtered_pwm = 0.0f;
}

/**
 * @brief Returns true if a permanent APPS fault has occurred.
 */
bool APPS_IsPermanentFault(void)
{
    return permanent_fault;
}
