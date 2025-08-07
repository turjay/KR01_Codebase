#include "brake.h"

// ============================
// External Variables
// ============================

extern ADC_HandleTypeDef hadc2;  // ADC2 handle from main.c

// ============================
// Public Variables
// ============================

/**
 * @brief Last read ADC value from the brake pressure sensor.
 */
volatile uint16_t brake_adc_value = 0;

// ============================
// Function Implementations
// ============================

/**
 * @brief Reads the current brake sensor value via ADC.
 *
 * Starts a blocking ADC conversion on ADC2.
 * If the conversion completes successfully within 100ms,
 * stores the result in `brake_adc_value`.
 * If not, sets it to a known invalid value (0xDDDD).
 */
void Brake_Update(void)
{
    HAL_ADC_Start(&hadc2);

    if (HAL_ADC_PollForConversion(&hadc2, 100) == HAL_OK)
    {
        brake_adc_value = HAL_ADC_GetValue(&hadc2);
    }
    else
    {
        // ADC conversion timeout or error
        brake_adc_value = 0xDDDD;
    }

    HAL_ADC_Stop(&hadc2);
}

/**
 * @brief Checks if brake is considered "pressed".
 *
 * Compares the latest ADC reading with a defined threshold.
 *
 * @param threshold Minimum ADC value considered as "pressed".
 * @return true if brake is pressed, false otherwise.
 */
bool Brake_IsPressed(uint16_t threshold)
{
    return (brake_adc_value > threshold);
}
