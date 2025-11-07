/*
 * parser.h
 *
 *  Created on: Nov 7, 2025
 *      Author: WINDOWS10
 */

#ifndef INC_PARSER_H_
#define INC_PARSER_H_

#include "main.h"

#define MAX_COMMAND_BUFFER 30

extern uint8_t command_flag; // 0 = no command, 1 = !RST#, 2 = !OK#
extern char command_buffer[MAX_COMMAND_BUFFER];

void parser_init(UART_HandleTypeDef *huart);


#endif /* INC_PARSER_H_ */
