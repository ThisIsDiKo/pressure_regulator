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

uint8_t airSystemType = 0;

void xAnalyzeTask(void *arguments){
	portBASE_TYPE xStatus;
	uint8_t i = 0;

	int16_t deltaPressure = 0;
	uint8_t prescalerCounter = 10;
	uint8_t analyzeCounterRef[4] = {5};

	xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
	for(;;){
		xStatus = xSemaphoreTake(xPressureCompensationSemaphore, portMAX_DELAY);
		if (xStatus == pdPASS){

			if (airSystemType == 0){ // Если мы работаем с ресивером
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

				if (pressIsLower[2]){
					if (filteredData.sens_3 < nessPressure[2]){
						C3_UP_ON;
						C3_DOWN_OFF;
						workState = WORKING;
					}
				}
				else{
					if (filteredData.sens_3 > nessPressure[2]){
						C3_UP_OFF;
						C3_DOWN_ON;
						workState = WORKING;
					}
				}

				if (pressIsLower[3]){
					if (filteredData.sens_4 < nessPressure[3]){
						C4_UP_ON;
						C4_DOWN_OFF;
						workState = WORKING;
					}
				}
				else{
					if (filteredData.sens_4 > nessPressure[3]){
						C4_UP_OFF;
						C4_DOWN_ON;
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
			else{
				if (prescalerCounter >= 5){
								prescalerCounter = 0;
											workState = FREE;

											if (analyzeCounter[0] < 5){
												workState = WORKING;
													if (pressIsLower[0]){
														if (filteredData.sens_1 < nessPressure[0]){
															analyzeCounter[0] = 0;
															analyzeCounterRef[0] = 5;
															C1_UP_ON;
															C1_DOWN_OFF;
														}
				//										else if (filteredData.sens_1 - nessPressure[0] > PRESS_ACCURACY){
				//											analyzeCounter[0] = 0;
				//											analyzeCounterRef[0] = 2;
				//											C1_UP_OFF;
				//											C1_DOWN_ON;
				//										}
														else{
															analyzeCounter[0] += 1;
															C1_UP_OFF;
															C1_DOWN_OFF;
														}
													}
													else{
														if (filteredData.sens_1 > nessPressure[0]){
															analyzeCounter[0] = 0;
															analyzeCounterRef[0] = 5;
															C1_DOWN_ON;
															C1_UP_OFF;
														}
				//										else if (nessPressure[0] - filteredData.sens_1 > PRESS_ACCURACY){
				//											analyzeCounter[0] = 0;
				//											analyzeCounterRef[0] = 2;
				//											C1_UP_ON;
				//											C1_DOWN_OFF;
				//										}
														else{
															analyzeCounter[0] += 1;
															C1_UP_OFF;
															C1_DOWN_OFF;
														}
													}
											}
											else{
												analyzeCounter[0] = 0;
												C1_UP_OFF;
												C1_DOWN_OFF;
											}

											if (analyzeCounter[1] < 5){
												workState = WORKING;
													if (pressIsLower[1]){
														if (filteredData.sens_2 < nessPressure[1]){
															analyzeCounter[1] = 0;
															analyzeCounterRef[1] = 5;
															C2_UP_ON;
															C2_DOWN_OFF;
														}
				//										else if (filteredData.sens_2 - nessPressure[1] > PRESS_ACCURACY){
				//											analyzeCounter[1] = 0;
				//											analyzeCounterRef[1] = 2;
				//											C2_UP_OFF;
				//											C2_DOWN_ON;
				//										}
														else{
															analyzeCounter[1] += 1;
															C2_UP_OFF;
															C2_DOWN_OFF;
														}
													}
													else{
														if (filteredData.sens_2 > nessPressure[1]){
															analyzeCounter[1] = 0;
															analyzeCounterRef[1] = 5;
															C2_DOWN_ON;
															C2_UP_OFF;
														}
				//										else if (nessPressure[1] - filteredData.sens_2 > PRESS_ACCURACY){
				//											analyzeCounter[1] = 0;
				//											analyzeCounterRef[1] = 2;
				//											C2_UP_ON;
				//											C2_DOWN_OFF;
				//										}
														else{
															analyzeCounter[1] += 1;
															C2_UP_OFF;
															C2_DOWN_OFF;
														}
													}
											}
											else{
												analyzeCounter[1] = 0;
												C2_UP_OFF;
												C2_DOWN_OFF;
											}

				//							if (analyzeCounter[2] < 5){
				//								workState = WORKING;
				//									if (pressIsLower[2]){
				//										if (filteredData.sens_3 < nessPressure[2]){
				//											analyzeCounter[2] = 0;
				//											C3_UP_ON;
				//											C3_DOWN_OFF;
				//										}
				//										else{
				//											analyzeCounter[2] += 1;
				//											C3_UP_OFF;
				//											C3_DOWN_OFF;
				//										}
				//									}
				//									else{
				//										if (filteredData.sens_3 > nessPressure[2]){
				//											analyzeCounter[2] = 0;
				//											C3_DOWN_ON;
				//											C3_UP_OFF;
				//										}
				//										else{
				//											analyzeCounter[2] += 1;
				//											C3_UP_OFF;
				//											C3_DOWN_OFF;
				//										}
				//									}
				//							}
				//							else{
				//								analyzeCounter[2] = 0;
				//								C3_UP_OFF;
				//								C3_DOWN_OFF;
				//							}


				//							if (analyzeCounter[3] < 5){
				//								workState = WORKING;
				//				//				deltaPressure = filteredData.sens_1 - nessPressure[0];
				//				//				deltaPressure = abs(deltaPressure);
				//				//				if (deltaPressure > PRESSURE_ACCURACY){
				//									if (pressIsLower[3]){
				//										if (filteredData.sens_4 < nessPressure[3]){
				//											analyzeCounter[3] = 0;
				//											C4_UP_ON;
				//											C4_DOWN_OFF;
				//										}
				//										else{
				//											analyzeCounter[3] += 1;
				//											C4_UP_OFF;
				//											C4_DOWN_OFF;
				//										}
				//									}
				//									else{
				//										if (filteredData.sens_4 > nessPressure[3]){
				//											analyzeCounter[3] = 0;
				//											C4_DOWN_ON;
				//											C4_UP_OFF;
				//										}
				//										else{
				//											analyzeCounter[3] += 1;
				//											C4_UP_OFF;
				//											C4_DOWN_OFF;
				//										}
				//									}
				//							}
				//							else{
				//								analyzeCounter[3] = 0;
				//								C4_UP_OFF;
				//								C4_DOWN_OFF;
				//							}

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
