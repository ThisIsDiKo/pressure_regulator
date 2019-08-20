/*
 * usefull.c
 *
 *  Created on: 17 апр. 2019 г.
 *      Author: ADiKo
 */


#include "useful.h"
#include "structures.h"

extern UART_HandleTypeDef huart1;
uint8_t messageLength = 0;
extern char message[128];
extern struct controllerData controllerSettings;

void init_rf433(){
	CMD_RF_ON;

	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+FU1\r",7, 0x2000);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+B19200\r",7, 0x2000);

	HAL_Delay(50);
	messageLength = sprintf(message, "AT+C%03d\r", controllerSettings.rfChannel);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*) message, messageLength);
	HAL_Delay(50);
	CMD_RF_OFF;
}
