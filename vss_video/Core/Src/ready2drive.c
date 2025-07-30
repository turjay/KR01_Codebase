#include "ready2drive.h"
#include "apps.h"

static GPIO_PinState prev_rtd_button = GPIO_PIN_SET;
bool rtd_active = false;
bool apps_enabled = false;
bool buzzer_on = false;

void R2D_Init(void)
{
    prev_rtd_button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
}

void R2D_Loop(void)
{
    GPIO_PinState rtd_button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    GPIO_PinState brake_pressed = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);

    // RTD buton düşen kenar algılama
    if (prev_rtd_button == GPIO_PIN_SET && rtd_button == GPIO_PIN_RESET)
    {
        if (!rtd_active && brake_pressed == GPIO_PIN_SET) {
            rtd_active = true;
            apps_enabled = true;
            APPS_Init();

            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // Buzzer ON
            HAL_Delay(2000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        }
        else if (rtd_active) {
            rtd_active = false;
            apps_enabled = false;
            APPS_Deinit();

            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Buzzer OFF
        }
    }

    prev_rtd_button = rtd_button;

    if (buzzer_on) {
        HAL_Delay(3000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        buzzer_on = false;
    }

    if (apps_enabled) {
        APPS_Loop();
    }
}
