#ifndef GYRO_H_
#define GYRO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern float gyro_degrees[2];

void gyro_start(void);

#ifdef __cplusplus
}
#endif

#endif /* GYRO_H_ */