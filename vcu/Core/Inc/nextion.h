#ifndef NEXTION_H
#define NEXTION_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================
// Function Prototypes
// ============================

/**
 * @brief Sends a raw command string to the Nextion display.
 *
 * The function appends three 0xFF bytes (end marker) required by the Nextion protocol.
 *
 * @param cmd The command string to send (e.g., "t0.txt=\"Hello\"")
 */
void Nextion_SendCommand(const char *cmd);

/**
 * @brief Updates the display with the current RPM or throttle percentage.
 *
 * Sends a command to update the `t9.txt` field with the given RPM value.
 * You can rename this function or adjust the label if you're displaying throttle % instead.
 *
 * @param rpm The value to display (RPM or percentage).
 */
void Nextion_UpdateRPM(float rpm);

/**
 * @brief Updates the display with the RTD (Ready-To-Drive) state.
 *
 * Sends a command to update `t10.txt` field with "ON" or "OFF".
 *
 * @param rtd_state True if RTD is active, false otherwise.
 */
void Nextion_UpdateR2D(bool rtd_state);

#ifdef __cplusplus
}
#endif

#endif // NEXTION_H
