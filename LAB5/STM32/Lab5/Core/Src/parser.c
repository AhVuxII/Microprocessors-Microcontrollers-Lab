/*
 * parser.c
 *
 *  Created on: Nov 7, 2025
 *      Author: WINDOWS10
 */

#include "parser.h"

typedef enum{
	STATE_INIT,
	STATE_RECEIVE
} parse_state_t;

static parse_state_t parse_state = STATE_INIT;
static UART_HandleTypeDef *parse_uart;
static uint8_t temp_char = 0; // disconnect uart
static uint8_t command_index = 0;

uint8_t command_flag = 0;
char command_buffer[MAX_COMMAND_BUFFER];

// delete command
static void reset_command() {
    command_index = 0;
}

// add char to command
static void add_char_to_command(char c){
	if (command_index < MAX_COMMAND_BUFFER - 1){
		command_buffer[command_index++] = c;
	}
}

// add null and set flag
static void finish_command() {
	command_buffer[command_index] = '\0'; // ket thuc chuoi
	command_flag = 1;
}

// FSM
static void parse_fsm(char c){
	switch (parse_state){
		case STATE_INIT:
			if (c == '!'){
				parse_state = STATE_RECEIVE;
				reset_command();
			}
			break;

		case STATE_RECEIVE:
			if (c == '#'){
				finish_command();
				parse_state = STATE_INIT;
			}
			else if (c == '!'){
				reset_command();
			}
			else {
				add_char_to_command(c);
			}
				break;

		default: parse_state = STATE_INIT;
		break;
	}
}


void parser_init(UART_HandleTypeDef *huart) {
    parse_uart = huart;
    HAL_UART_Receive_IT(parse_uart, &temp_char, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == parse_uart->Instance) {
        parse_fsm(temp_char);
        HAL_UART_Receive_IT(parse_uart, &temp_char, 1);
    }
}



