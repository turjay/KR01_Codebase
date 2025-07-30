#ifndef READY2DRIVE_H
#define READY2DRIVE_H

#include "main.h"
#include <stdbool.h>

// R2D durumları
extern bool rtd_active;
extern bool apps_enabled;
extern bool buzzer_on;

// Başlatıcı ve döngü fonksiyonları
void R2D_Init(void);
void R2D_Loop(void);

#endif // READY2DRIVE_H
