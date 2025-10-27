/*
 * global.h
 *
 *  Created on: Oct 27, 2025
 *      Author: WINDOWS10
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "main.h"
#define REFRESH_RATE 100
#define TIMER_CYCLE 1
#define SHIFT_CYCLE 500
#define BUTTON_SCAN 10
#define BUTTON_THRESHOLD 5

extern uint32_t timestamp;
void displayMSG (char *str);
#endif /* INC_GLOBAL_H_ */
