/**
 ******************************************************************************
 * @file    nextion.h
 * @brief   Nextion HMI ekranı için başlık dosyası
 * @details SoC, Speed ve R2D durum bilgilerini ekrana yansıtır.
 ******************************************************************************
 */

#ifndef NEXTION_H
#define NEXTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Nextion'a ham komut gönderir.
 * @param cmd Komut stringi (örn: "t0.txt=\"Hello\"")
 */
void Nextion_SendCommand(const char *cmd);

/**
 * @brief SoC değerini ekrana gönderir.
 * @param soc Pil doluluk oranı (0-100 %)
 */
void Nextion_UpdateSoC(uint8_t soc);

/**
 * @brief Araç hızını ekrana gönderir.
 * @param speed Araç hızı (km/h)
 */
void Nextion_UpdateSpeed(uint16_t speed);

/**
 * @brief Ready-to-Drive (R2D) durumunu ekrana gönderir.
 * @param rtd_state true: ON, false: OFF
 */
void Nextion_UpdateR2D(bool rtd_state);

#ifdef __cplusplus
}
#endif

#endif /* NEXTION_H */
