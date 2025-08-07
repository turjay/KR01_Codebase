#include "brake.h"
#include "ready2drive.h"
#include "apps.h"

// ============================
// Internal State
// ============================

static GPIO_PinState prev_rtd_button = GPIO_PIN_SET; // Last state of the RTD button

bool rtd_active = false;     // RTD system status flag
bool apps_enabled = false;   // APPS system enable flag (shared with APPS)
bool buzzer_on = false;      // Buzzer override flag
volatile uint16_t brake_adc_live = 0; // Live brake ADC value (optional use)

#define BRAKE_PRESS_THRESHOLD 300

// ============================
// Function Implementations
// ============================

/**
 * @brief Initializes the Ready-to-Drive (RTD) system.
 *
 * Reads the initial state of the RTD button.
 */
void R2D_Init(void)
{
    prev_rtd_button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
}

/**
 * @brief Main loop for the RTD system.
 *
 * - Monitors RTD button and brake input.
 * - Handles falling edge detection on RTD button.
 * - Enables/disables throttle control through APPS.
 * - Controls buzzer based on RTD status.
 */
void R2D_Loop(void)
{
    // Read RTD button state
    GPIO_PinState rtd_button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);

    // Update brake ADC value and check if brake is pressed
    Brake_Update();
    bool brake_pressed = Brake_IsPressed(BRAKE_PRESS_THRESHOLD);

    // Detect falling edge on RTD button
    if (prev_rtd_button == GPIO_PIN_SET && rtd_button == GPIO_PIN_RESET)
    {
        if (!rtd_active && brake_pressed)
        {
            // Activate RTD if brake is pressed and RTD was inactive
            rtd_active = true;
            apps_enabled = true;
            APPS_Init();

            // Turn on buzzer for 2 seconds
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
            HAL_Delay(2000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        }
        else if (rtd_active)
        {
            // Deactivate RTD if already active
            rtd_active = false;
            apps_enabled = false;
            APPS_Deinit();

            // Ensure buzzer is OFF
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        }
    }

    // Update previous button state for next loop
    prev_rtd_button = rtd_button;

    // Optional buzzer control via external flag
    if (buzzer_on)
    {
        HAL_Delay(3000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        buzzer_on = false;
    }

    // Run APPS logic only if system is enabled
    if (apps_enabled)
    {
        APPS_Loop();
    }
}
