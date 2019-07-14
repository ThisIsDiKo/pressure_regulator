/*
 * controllerInit.c
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#include "globals.h"

#include "controllerInit.h"

#include "flashFunctions.h"
#include "storeADCData.h"
#include "processCommand.h"
#include "analyze.h"
#include "structures.h"
#include "ScanBtn.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

extern xSemaphoreHandle xPressureCompensationSemaphore;
extern xQueueHandle xRecCommandQueue;
extern char message[128];
extern uint8_t recCommandByte;

extern struct controllerData controllerSettings;

extern uint16_t server_UID;
uint32_t unique_ID[3] = {0};

void xBlynkTask(void* arguments){
	for(;;){
		HAL_GPIO_TogglePin(BLYNK_LED_PORT, BLYNK_LED_PIN);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
	vTaskDelete(NULL);
}

void init_RF433(){
	CMD_RF_OFF;
	return;
}

void controller_initialize(){

	//Запуск прерываний
	HAL_UART_Receive_IT(&huart1, &recCommandByte, 1);
	HAL_ADCEx_InjectedStart_IT(&hadc1);

	//Чтение данных из памяти
	mRead_flash();

	  HAL_GetUID(unique_ID);
	  server_UID = (unique_ID[0] + unique_ID[1] + unique_ID[2]) / 65536;

#if DEBUG_SERIAL
	print_settings();
#endif

	//Инициализация радиопередатчика
	init_RF433();

	vSemaphoreCreateBinary(xPressureCompensationSemaphore);

	#if DEBUG_SERIAL
		uint32_t fre=xPortGetFreeHeapSize();
		sprintf(message, "Free heap: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
	#endif

	xTaskCreate(xBlynkTask,
				"Blynk",
				200,
				NULL,
				1,
				NULL);

	#if DEBUG_SERIAL
		fre=xPortGetFreeHeapSize();
		sprintf(message, "heap after Blynk: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
	#endif

	xTaskCreate(xScanButtonsTask,
							"Scan",
							200,
							NULL,
							1,
							NULL);

	#if DEBUG_CALIBRATION_SERIAL
		fre=xPortGetFreeHeapSize();
		sprintf(message, "heap after Scan: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 0xFFFF);
	#endif

	xTaskCreate(xStoreADCDataTask,
				"SADCData",
				512,
				NULL,
				1,
				NULL);

	#if DEBUG_SERIAL
		fre=xPortGetFreeHeapSize();
		sprintf(message, "heap after SADCData: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
	#endif

	xTaskCreate(xProcessCommandTask,
				"ProcCmd",
				512,
				NULL,
				3,
				NULL);

	#if DEBUG_SERIAL
		fre=xPortGetFreeHeapSize();
		sprintf(message, "heap after ProcCmd: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
	#endif

	xTaskCreate(xAnalyzeTask,
				"AnTsk",
				400,
				NULL,
				1,
				NULL);
	#if DEBUG_SERIAL
		fre=xPortGetFreeHeapSize();
		sprintf(message, "heap after AnTask: %ld\r\n", fre);
		HAL_UART_Transmit(&huart1, (uint8_t*) message, strlen(message), 0xFFFF);
	#endif

  	xRecCommandQueue = xQueueCreate(COMMAND_QUEUE_SIZE, MAX_COMMAND_LENGTH);

  	#if DEBUG_SERIAL
  		fre=xPortGetFreeHeapSize();
  		sprintf(message, "heap after queue: %ld\r\n", fre);
  		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
  	#endif
}

void print_settings(void){
	#if DEBUG_SERIAL
		sprintf(message, "Settings:\n");
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
		sprintf(message, "Stored 1:%d, %d, %d, %d\n", 	controllerSettings.storedPos_1[0],
														controllerSettings.storedPos_1[1],
														controllerSettings.storedPos_1[2],
														controllerSettings.storedPos_1[3]);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
		sprintf(message, "Stored 2:%d, %d, %d, %d\n", 	controllerSettings.storedPos_2[0],
														controllerSettings.storedPos_2[1],
														controllerSettings.storedPos_2[2],
														controllerSettings.storedPos_2[3]);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
		sprintf(message, "Stored 3:%d, %d, %d, %d\n", 	controllerSettings.storedPos_3[0],
														controllerSettings.storedPos_3[1],
														controllerSettings.storedPos_3[2],
														controllerSettings.storedPos_3[3]);
		HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 0xFFFF);
	#endif
}
