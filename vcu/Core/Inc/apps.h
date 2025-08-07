#ifndef APPS_H
#define APPS_H

#include <stdbool.h>
#include <stdint.h>

// ============================
// Configuration
// ============================

/**
 * @brief Number of ADC channels used by APPS.
 *
 * Adjust this value according to the number of pedal sensors.
 */
#define ADC_CHANNEL_COUNT 2

// ============================
// External Variables
// ============================

/**
 * @brief Normalized pedal position (sensor 1) in percentage (0-100%).
 */
extern float norm1;

/**
 * @brief ADC data buffer for pedal sensors.
 */
extern uint32_t adcdata[ADC_CHANNEL_COUNT];

/**
 * @brief Current PWM output value in percentage (0-100%).
 */
extern uint16_t pwm[1];

/**
 * @brief Indicates if APPS system is enabled.
 *
 * This flag should be controlled by the main system logic (e.g., R2D state).
 */
extern bool apps_enabled;

// ============================
// Function Prototypes
// ============================

/**
 * @brief Initializes the APPS system.
 *
 * - Starts ADC in DMA mode.
 * - Starts timing for plausibility check.
 * - Initializes and starts PWM output.
 */
void APPS_Init(void);

/**
 * @brief Main APPS loop.
 *
 * - Reads ADC sensor data.
 * - Checks plausibility between two pedal sensors.
 * - Updates PWM output (with logarithmic scaling and filtering).
 * - Shuts down throttle in case of fault.
 */
void APPS_Loop(void);

/**
 * @brief Deinitializes the APPS system.
 *
 * - Stops ADC DMA and PWM.
 * - Reconfigures PWM pin as GPIO and forces it to 0V.
 * - Resets internal variables.
 */
void APPS_Deinit(void);

/**
 * @brief Checks if a permanent APPS fault has occurred.
 *
 * @return true  If a permanent fault has been latched (requires system reset to clear).
 * @return false If APPS is operating normally.
 */
bool APPS_IsPermanentFault(void);

#endif // APPS_H
