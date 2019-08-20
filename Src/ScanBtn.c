/*
 * ScanBtn.c
 *
 *  Created on: 30 апр. 2019 г.
 *      Author: ADiKo
 */




/*
 * storeADCData.c
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#include "ScanBtn.h"
#include "globals.h"
#include "structures.h"
#include "useful.h"

extern UART_HandleTypeDef huart1;
extern uint8_t showBlynk;

void xScanButtonsTask(void* arguments){

	uint8_t prevBtnState = 0;
	uint8_t curBtnState = 0;
	uint8_t prevSysState = 0;

	for(;;){
		curBtnState = HAL_GPIO_ReadPin(HALL_SENS_PORT, HALL_SENS_PIN);
		if (curBtnState != prevBtnState){
			vTaskDelay(1 / portTICK_RATE_MS);
			curBtnState = HAL_GPIO_ReadPin(HALL_SENS_PORT, HALL_SENS_PIN);
			if (curBtnState != prevBtnState){
				prevBtnState = curBtnState;
				if (!prevBtnState){
					if (prevSysState > 0){
						prevSysState = 0;
						showBlynk = 0;
						//HAL_GPIO_WritePin(A_LED_PORT, A_LED_PIN, GPIO_PIN_SET);
						TIM3->CCR1 = 0;
						TIM3->CCR2 = 0;
						TIM3->CCR3 = 65535;
						CMD_RF_ON;
						vTaskDelay(50 / portTICK_RATE_MS);

						HAL_UART_Transmit(&huart1, (uint8_t*) "AT+C001\r", 8, 0x2000);

						vTaskDelay(50 / portTICK_RATE_MS);
						CMD_RF_OFF;
					}
					else{
						prevSysState = 1;

						//HAL_GPIO_WritePin(A_LED_PORT, A_LED_PIN, GPIO_PIN_RESET);
						//TIM3->CCR2 = 0;
						init_rf433();
						showBlynk = 1;
					}
				}
			}
		}

		vTaskDelay(10 / portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
}
