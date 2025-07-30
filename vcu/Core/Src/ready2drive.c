#include "ready2drive.h"
#include "apps.h"
#include "bpps.h"       // brake_active flag için eklendi
#include "nextion.h"

static GPIO_PinState prev_rtd_button = GPIO_PIN_SET;
bool rtd_active = false;
bool apps_enabled = false;
bool buzzer_on = false;

/**
 * @brief Ready-to-Drive sistemini başlatır.
 *        R2D butonunun ilk durumunu okur.
 */
void R2D_Init(void)
{
    prev_rtd_button = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8); // R2D buton pini (PD8)
}

/**
 * @brief Ready-to-Drive ana döngü fonksiyonu.
 *
 * - R2D butonunu okur (düşen kenar algılar).
 * - Fren durumu (brake_active) ile birlikte R2D'yi kontrol eder.
 * - APPS ve buzzer kontrolünü yapar.
 */
void R2D_Loop(void)
{
    // RTD butonunu oku
    GPIO_PinState rtd_button = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8);

    // RTD buton düşen kenar algılama
    if (prev_rtd_button == GPIO_PIN_SET && rtd_button == GPIO_PIN_RESET)
    {
        if (!rtd_active && brake_active) {
            // R2D aktif ediliyor: Fren basılı ve RTD buton basıldı
            rtd_active = true;
            apps_enabled = true;
            APPS_Init();

            // Buzzer 2 saniye çalar
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
            HAL_Delay(2000);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        }
        else if (rtd_active) {
            // R2D devreden çıkarılıyor
            rtd_active = false;
            apps_enabled = false;
            APPS_Deinit();

            // Buzzer kapatılır (eğer açıksa)
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        }
    }

    prev_rtd_button = rtd_button;

    // Buzzer uzatmalı kontrol
    if (buzzer_on) {
        HAL_Delay(3000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        buzzer_on = false;
    }

    // APPS döngüsü çalıştırılır (eğer aktifse)
    if (apps_enabled) {
        APPS_Loop();
    }
}
