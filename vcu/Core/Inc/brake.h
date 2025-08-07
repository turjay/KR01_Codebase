#ifndef BRAKE_H
#define BRAKE_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================
// Public Variables
// ============================

/**
 * @brief Latest brake ADC reading (0–4095 for 12-bit ADC).
 *
 * Updated by Brake_Update().
 */
extern volatile uint16_t brake_adc_value;

// ============================
// Function Prototypes
// ============================

/**
 * @brief Updates the brake ADC reading.
 *
 * Performs a single ADC2 conversion and stores the result
 * in `brake_adc_value`. If timeout occurs, stores 0xDDDD.
 */
void Brake_Update(void);

/**
 * @brief Determines whether the brake is pressed.
 *
 * Compares `brake_adc_value` to a given threshold.
 *
 * @param threshold ADC value considered as "pressed" (e.g. 800–1200).
 * @return true if brake is pressed, false otherwise.
 */
bool Brake_IsPressed(uint16_t threshold);

#ifdef __cplusplus
}
#endif

#endif // BRAKE_H
