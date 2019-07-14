/*
 * analyze.c
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: ADiKo
 */

#include "globals.h"
#include "structures.h"
#include "analyze.h"

#include "math.h"

#define PRESS_ACCURACY		100

extern uint16_t nessPressure[4];
extern struct Filtered filteredData;

extern enum WorkState workState;
extern enum Compensation pressureCompensation;

extern uint8_t pressIsLower[4];

extern xSemaphoreHandle xPressureCompensationSemaphore;
extern UART_HandleTypeDef huart1;

uint8_t analyzeCounter[4] = {0};

void xAnalyzeTask(void *arguments){
	portBASE_TYPE xStatus;
	uint8_t i = 0;

	int16_t deltaPressure = 0;
	uint8_t prescalerCounter = 10;
	uint8_t analyzeCounterRef[4] = {5};
//	char messageDebug[16] = {0};
//	uint8_t mesDebugLen = 0;

	xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
	for(;;){
		xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
		if (xStatus == pdPASS){

			workState = FREE;

			if (pressIsLower[0]){
				if (filteredData.sens_1 < nessPressure[0]){
					C1_UP_ON;
					C1_DOWN_OFF;
					workState = WORKING;
				}
			}
			else{
				if (filteredData.sens_1 > nessPressure[0]){
					C1_UP_OFF;
					C1_DOWN_ON;
					workState = WORKING;
				}
			}

			if (pressIsLower[1]){
							if (filteredData.sens_2 < nessPressure[1]){
								C2_UP_ON;
								C2_DOWN_OFF;
								workState = WORKING;
							}
						}
						else{
							if (filteredData.sens_2 > nessPressure[1]){
								C2_UP_OFF;
								C2_DOWN_ON;
								workState = WORKING;
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
			C1_DOWN_OFF;
			C2_DOWN_OFF;
			C3_DOWN_OFF;
			C4_DOWN_OFF;

			vTaskDelay(500 / portTICK_RATE_MS);
			C1_UP_OFF;
			C2_UP_OFF;
			C3_UP_OFF;
			C4_UP_OFF;
			//Останавливаем импульс
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
	}
	vTaskDelete(NULL);
}
