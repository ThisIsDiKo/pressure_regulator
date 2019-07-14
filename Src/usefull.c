/*
 * usefull.c
 *
 *  Created on: 17 апр. 2019 г.
 *      Author: ADiKo
 */


#include "useful.h"

extern UART_HandleTypeDef huart1;

void init_rf433(){
	CMD_RF_ON;
	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+FU1\r",7, 0x2000);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+B19200\r",7, 0x2000);
	HAL_Delay(50);
	CMD_RF_OFF;
}
