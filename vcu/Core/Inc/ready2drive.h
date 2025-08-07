#ifndef READY2DRIVE_H
#define READY2DRIVE_H

#include <stdbool.h>
#include <stdint.h>

// ============================
// Global Variables
// ============================

/**
 * @brief Indicates whether the RTD (Ready-To-Drive) system is active.
 */
extern bool rtd_active;

/**
 * @brief Indicates whether APPS system is currently enabled.
 *
 * Used by APPS to decide whether throttle logic should run.
 */
extern bool apps_enabled;

/**
 * @brief Manual control flag to enable buzzer for 3 seconds.
 *
 * Set this flag to true from other modules to trigger buzzer alert.
 */
extern bool buzzer_on;

/**
 * @brief Current live brake ADC value (can be monitored externally).
 */
extern volatile uint16_t brake_adc_live;

// ============================
// Function Prototypes
// ============================

/**
 * @brief Initializes the RTD system.
 *
 * Should be called once during startup.
 */
void R2D_Init(void);

/**
 * @brief Main loop function for RTD.
 *
 * Should be called periodically (e.g., in main loop or timer loop).
 * Handles:
 *  - RTD button falling-edge detection
 *  - Brake press validation
 *  - Buzzer control
 *  - Enabling/disabling APPS system
 */
void R2D_Loop(void);

#endif // READY2DRIVE_H
