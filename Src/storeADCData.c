/*
 * storeADCData.c
 *
 *  Created on: 25 ���. 2019 �.
 *      Author: ADiKo
 */

#include "storeADCData.h"
#include "globals.h"
#include "structures.h"


extern ADC_HandleTypeDef hadc1;

extern xSemaphoreHandle xPressureCompensationSemaphore;

extern uint16_t filteredPressure[4];
extern enum Compensation pressureCompensation;
extern uint16_t ADCRawData[4];
extern uint16_t sensorValue[4];

uint8_t prevCompensation = OFF;
uint8_t lastTimeCommand = 0;

extern GPIO_TypeDef * UP_PORT[4];
extern uint32_t UP_PIN[4];
extern GPIO_TypeDef * DOWN_PORT[4];
extern uint32_t DOWN_PIN[4];

uint16_t fir_filter(uint16_t *signal, uint16_t sample){

	uint32_t filteredSample = 0;

	//10 Hz
//	uint32_t FIRCoef[10] = {
//	         1758,
//	         -797,
//	        -3499,
//	         9034,
//	        20357,
//	         9034,
//	        -3499,
//	         -797,
//	         1758,
//	         -581
//		    };
//	uint32_t DCgain = 32768;

	//5 Hz Fcut = 1 Hz
//	uint32_t FIRCoef[10] = {
//	        -2605,
//	        -4732,
//	         3500,
//	        20841,
//	        30419,
//	        20841,
//	         3500,
//	        -4732,
//	        -2605,
//	         1108
//		    };
//	uint32_t DCgain = 65536;

	//20 hz 5 hz
	uint32_t FIRCoef[10] = {
		    -87,
		  -2085,
		   2358,
		  18158,
		  28406,
		  18158,
		   2358,
		  -2085,
		    -87,
		    444
			    };
		uint32_t DCgain = 65536;



	uint8_t i = 0;

	for (i = 10 - 1; i> 0; i--){
		signal[i] = signal[i-1];
	}

	signal[0] = sample;
	filteredSample = 0;

	for (i = 0 ; i < 10; i++){
		filteredSample += FIRCoef[i] * (uint32_t)signal[i];
	}
	filteredSample = filteredSample / DCgain;

	return (uint16_t) filteredSample;
}

void xStoreADCDataTask(void* arguments){

	uint16_t sens1_array[10] = {1};
	uint16_t sens2_array[10] = {2};
	uint16_t sens3_array[10] = {3};
	uint16_t sens4_array[10] = {4};
	uint8_t i = 0;



	for(;;){

		if (lastTimeCommand > 50){
			for (i  = 0; i < 4; i++){
				HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
			}
			lastTimeCommand = 0;
		}
		else{
			lastTimeCommand++;
		}

		sensorValue[SENS_1] = ADCRawData[SENS_1];
		sensorValue[SENS_2] = ADCRawData[SENS_2];
		sensorValue[SENS_3] = ADCRawData[SENS_3];
		sensorValue[SENS_4] = ADCRawData[SENS_4];

		HAL_ADCEx_InjectedStart_IT(&hadc1);

		//TODO: ������ ���� ��������� ����������
		//LEFT_UP_ON;
		filteredPressure[0] = fir_filter(sens1_array, sensorValue[SENS_1]);
		filteredPressure[1] = fir_filter(sens2_array, sensorValue[SENS_2]);
		filteredPressure[2] = fir_filter(sens3_array, sensorValue[SENS_3]);
		filteredPressure[3] = fir_filter(sens4_array, sensorValue[SENS_4]);
		//LEFT_UP_OFF;


		if (pressureCompensation == ON){
			prevCompensation = ON;
			xSemaphoreGive(xPressureCompensationSemaphore);
		}
		else{
			if (prevCompensation == ON){
				for (i  = 0; i < 4; i++){
					HAL_GPIO_WritePin(UP_PORT[i], UP_PIN[i], GPIO_PIN_RESET);
					HAL_GPIO_WritePin(DOWN_PORT[i], DOWN_PIN[i], GPIO_PIN_RESET);
				}
				prevCompensation = OFF;
			}

		}

		vTaskDelay(ADC_DATA_PERIOD / portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
}
