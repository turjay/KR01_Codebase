/*
 * hall.h
 *
 *  Created on: Jul 22, 2025
 *      Author: turjay
 */

#ifndef __HALL_H
#define __HALL_H

#include "main.h"

extern uint32_t adc_value;
extern float current;

void HALL_Loop(void);
static float Read_Current_From_HTFS400P(void);

#endif /* INC_HALL_H_ */
