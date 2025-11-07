/*
 * uart.c
 *
 *  Created on: Nov 7, 2025
 *      Author: WINDOWS10
 */

#include "uart.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

typedef enum {
    STATE_RESET,  // !RST#
    STATE_ADC,    // !ADC=...#
    STATE_OK      // !OK#
} UartState_t;


static UartState_t Ustatus = STATE_RESET;
static uint32_t ADCvalue = 0;
static char str[100];

//timer
static uint32_t timer_start_tick = 0;
static uint32_t timer_timeout_duration = 3000;
static uint8_t timer_active = 0;


static void setTimer(int duration) {
    timer_start_tick = HAL_GetTick();
    timer_timeout_duration = duration;
    timer_active = 1;
}

static void clearTimer() {
    timer_active = 0;
}

static uint8_t isTimerExpired() {
    if (!timer_active) {
        return 0;
    }

    if (HAL_GetTick() - timer_start_tick >= timer_timeout_duration) {
        timer_active = 0;
        return 1;
    }
    return 0;
}


static void HandleReset(ADC_HandleTypeDef hadc1, UART_HandleTypeDef huart2) {
    if (command_flag == 1) {
        command_flag = 0;


        if (strcmp(command_buffer, "RST") == 0) {


            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 100);
            ADCvalue = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_Stop(&hadc1);

            HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str, "\r\n"), 1000);

            Ustatus = STATE_ADC;
            setTimer(3000);
        }
    }
}

static void HandleADC(UART_HandleTypeDef huart2) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str,
                      sprintf(str, "!ADC=%ld#\r\n", ADCvalue), 1000);

    Ustatus = STATE_OK;
}

static void HandleOK(UART_HandleTypeDef huart2) {

    if (command_flag == 1) {
        command_flag = 0;

        if (strcmp(command_buffer, "OK") == 0) {
            HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str, "\r\n"), 1000);
            Ustatus = STATE_RESET;
            clearTimer();
        }
    }

    if (isTimerExpired() == 1) {
        Ustatus = STATE_ADC;
        setTimer(3000);
    }
}


// FSM
void uart_fsm(ADC_HandleTypeDef hadc1, UART_HandleTypeDef huart2) {
    switch (Ustatus) {
        case STATE_RESET:
            HandleReset(hadc1, huart2);
            break;
        case STATE_ADC:
            HandleADC(huart2);
            break;
        case STATE_OK:
            HandleOK(huart2);
            break;
        default:
            Ustatus = STATE_RESET;
            break;
    }
}
