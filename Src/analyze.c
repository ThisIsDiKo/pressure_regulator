/*
 * analyze.c
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: ADiKo
 */

#include "globals.h"
#include "structures.h"
#include "analyze.h"

extern uint16_t nessPressure[4];
extern uint8_t pressIsLower[4];
extern uint16_t filteredPressure[4];
//extern struct Filtered filteredData;

extern enum WorkState workState;
extern enum Compensation pressureCompensation;

extern xSemaphoreHandle xPressureCompensationSemaphore;


uint8_t airSystemType = 0;

extern const uint32_t UP_PORT[4];
extern const uint32_t UP_PIN[4];
extern const uint32_t DOWN_PORT[4];
extern const uint32_t DOWN_PIN[4];

void xAnalyzeTask(void *arguments){
	portBASE_TYPE xStatus;
	uint8_t i = 0;
	uint8_t analyzeCounter[4] = {0};

	xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
	for(;;){
		xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
		if (xStatus == pdPASS){
			if (airSystemType == 0){ // Если мы работаем с ресивером
				workState = FREE;
				for (i = 0; i < 4; i++){
					if (pressIsLower[i]){
						if (filteredPressure[i] < nessPressure[i]){
							HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
							HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_SET);
							workState = WORKING;
						}
					}
					else {
						if (filteredPressure[i] > nessPressure[i]){
							HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_SET);
							HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
							workState = WORKING;
						}
					}
				}

				if (workState == FREE){
					pressureCompensation = OFF;
				}
				else{
					pressureCompensation = ON;
				}

				//Запускаем импульс
				vTaskDelay(500 / portTICK_RATE_MS);
				for (i = 0; i < 4; i++){
					HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
				}
				vTaskDelay(500 / portTICK_RATE_MS);
				for (i = 0; i < 4; i++){
					HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
				}
				//Останавливаем импульс
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else{
				if (prescalerCounter >= 5){
					prescalerCounter = 0;
					workState = FREE;

					for (i = 0; i < 4; i++){
						if (analyzeCounter[i] < 5){
							workState = WORKING;
							if (pressureIsLower[i]){
								if (filteredPressure[i] < nessPressure[i]){
									analyzeCounter[i] = 0;
									HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_SET);
									HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
								}
								else{
									analyzeCounter[i] += 1;
									HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
									HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
								}
							}
							else{
								if (filteredPressure[i] > nessPressure[i]){
									analyzeCounter[i] = 0;
									HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
									HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_SET);
								}
								else{
									analyzeCounter[i] += 1;
									HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
									HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
								}
							}
						}
						else{
							analyzeCounter[i] = 0;
							HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
							HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
						}
					}

					if (workState == FREE){
						prescalerCounter = 10;
						pressureCompensation = OFF;
					}
					else{
						pressureCompensation = ON;
					}
				}
				else{
					prescalerCounter++;
				}
			}
		}
	}
	vTaskDelete(NULL);
}
