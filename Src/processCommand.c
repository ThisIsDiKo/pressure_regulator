/*
 * processCommand.c
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#include "globals.h"
#include "structures.h"

#include "processCommand.h"
#include "flashFunctions.h"

extern UART_HandleTypeDef huart1;

extern struct Filtered filteredData;
extern struct controllerData controllerSettings;
extern xQueueHandle xRecCommandQueue;

extern char message[128];
extern uint16_t nessPressure[4];
extern enum Compensation pressureCompensation;

uint8_t pressIsLower[4] = {0};
extern uint8_t lastTimeCommand;

extern uint16_t server_UID;

void xProcessCommandTask(void* arguments){
		portBASE_TYPE xStatus;
		uint8_t command[MAX_COMMAND_LENGTH] = {0};
		uint8_t messageLength = 0;
		uint16_t id = 0;
		char co = 0;
		char valve = 0;
		uint16_t channel = 0;

		char prev_command = 0;
		uint16_t sendData[4] = {0};

		for(;;){
			xStatus = xQueueReceive(xRecCommandQueue, command, portMAX_DELAY);
			if (xStatus == pdPASS){

				lastTimeCommand = 0;

				switch(command[0]){
					case 'm':{
						sscanf((char*)command, "m,%hu,%c,%c,\n", &id, &co, &valve);
						valve = command[10];
						co = co - '0';
						if (id == server_UID){
							sendData[0] = filteredData.sens_1;
							sendData[1] = filteredData.sens_2;
							sendData[2] = filteredData.sens_3;
							sendData[3] = filteredData.sens_4;
							messageLength = sprintf(message, "m,%hu,%hu,%hu,%hu,%hu,\n", controllerSettings.clientID,
																						 sendData[0],
																						 sendData[1],
																						 sendData[2],
																						 sendData[3]);
							HAL_UART_Transmit_DMA(&huart1, (uint8_t*) message, messageLength);
						}
						//Добавлена проверка на исследование состояния кнопок

						if (valve != prev_command){
							if (valve & 0b00000001) 	C1_UP_ON;
							else 				   		C1_UP_OFF;
							if (valve & 0b00000010) 	C1_DOWN_ON;
							else 				   		C1_DOWN_OFF;

							if (valve & 0b00000100) 	C2_UP_ON;
							else 				   		C2_UP_OFF;
							if (valve & 0b00001000) 	C2_DOWN_ON;
							else 				   		C2_DOWN_OFF;

							if (valve & 0b00010000) 	C3_UP_ON;
							else 				   		C3_UP_OFF;
							if (valve & 0b00100000) 	C3_DOWN_ON;
							else 				   		C3_DOWN_OFF;

							if (valve & 0b01000000) 	C4_UP_ON;
							else 				   		C4_UP_OFF;
							if (valve & 0b10000000) 	C4_DOWN_ON;
							else 				   		C4_DOWN_OFF;

							prev_command = valve;
						}
						break;
					}
					case 's':{
						if (command[1] == 'x'){
							sscanf((char*)command, "sx,%hu,\n", &id);
							if (id == server_UID){
								pressureCompensation = OFF;
							}
						}
						else if (command[1] == ','){
							sscanf((char*)command, "s,%hu,%hu,%hu,%hu,%hu,\n", &id, &nessPressure[0], &nessPressure[1],&nessPressure[2],&nessPressure[3]);
							if (id == server_UID){
								if (filteredData.sens_1 > nessPressure[0]) 	pressIsLower[0] = 0;
								else 										pressIsLower[0] = 1;
								if (filteredData.sens_2 > nessPressure[1]) 	pressIsLower[1] = 0;
								else 										pressIsLower[1] = 1;
								if (filteredData.sens_3 > nessPressure[2]) 	pressIsLower[2] = 0;
								else 										pressIsLower[2] = 1;
								if (filteredData.sens_4 > nessPressure[3]) 	pressIsLower[3] = 0;
								else 										pressIsLower[3] = 1;
								pressureCompensation = ON;
							}
						}
						break;
					}
					case 'x':{
						if (command[1] == '?'){
							sscanf((char*)command, "x?%hu,\n", &controllerSettings.clientID);
							messageLength = sprintf(message, "x,%05d,%05d,\n", controllerSettings.clientID, server_UID);

							HAL_UART_Transmit_DMA(&huart1, (uint8_t*) message, messageLength);
						}
						else if (command[1] == 'c'){

							sscanf((char*)command, "xc,%hu,%hu,\n", &id, &channel);
//							channel = channel - '0';

							messageLength = sprintf(message, "id,%05d,%05d,%03d\n", id, server_UID, channel);
							HAL_UART_Transmit(&huart1, (uint8_t*) message, messageLength, 0x2000);

							if (id == server_UID){
								controllerSettings.rfChannel = channel;
								mWrite_flash();
								messageLength = sprintf(message, "xc,%05d,ok,\n", controllerSettings.clientID);
								HAL_UART_Transmit_DMA(&huart1, (uint8_t*) message, messageLength);

								vTaskDelay(100 / portTICK_RATE_MS);
								CMD_RF_ON;
								vTaskDelay(50 / portTICK_RATE_MS);
								messageLength = sprintf(message, "AT+C%03d\r", channel);
								HAL_UART_Transmit_DMA(&huart1, (uint8_t*) message, messageLength);
								vTaskDelay(50 / portTICK_RATE_MS);
								CMD_RF_OFF;

								HAL_GPIO_WritePin(A_LED_PORT, A_LED_PIN, GPIO_PIN_RESET);

							}
						}
						break;
					}
				}
			}
		}
		vTaskDelete(NULL);
}
