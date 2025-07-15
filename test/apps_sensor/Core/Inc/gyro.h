#ifndef GYRO_H_
#define GYRO_H_

#include <stdint.h>

extern uint16_t gyrodata[2];

void GYRO_Init(void);
void GYRO_Loop(void);

#endif /* GYRO_H_ */
