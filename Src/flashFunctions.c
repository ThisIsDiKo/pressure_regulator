/*
 * flashFunctions.c
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#include "flashFunctions.h"

#include "globals.h"
#include "structures.h"

#define SETTINGS_FLASH_PAGE_ADDR	0x0801FC00
#define CONTROLLER_LENGTH_WORDS		(sizeof(controllerSettings)	/ 4)


extern UART_HandleTypeDef huart1;

extern struct controllerData controllerSettings;

void mWrite_flash(void){
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t pageError;
	uint16_t i;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = SETTINGS_FLASH_PAGE_ADDR;
	EraseInitStruct.NbPages = 1;

	if(HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK){
		#if DEBUG_SERIAL
			HAL_UART_Transmit(&huart1, (uint8_t*) "Erase Flash Error\r\n", 19, 0x1000);
		#endif
	}

	uint32_t *source_addr = (void *)&controllerSettings;
	uint32_t *dest_addr = (uint32_t *) SETTINGS_FLASH_PAGE_ADDR;
	for (i=0; i<CONTROLLER_LENGTH_WORDS; i++) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)dest_addr, *source_addr)!= HAL_OK){
			#if DEBUG_SERIAL
				HAL_UART_Transmit(&huart1, (uint8_t*) "Write Flash Error\r\n", 19, 0x1000);
			#endif
		}
		source_addr++;
		dest_addr++;
	}

	HAL_FLASH_Lock();

	#if DEBUG_SERIAL
		HAL_UART_Transmit(&huart1, (uint8_t*) "Flash ok\r\n", 10, 0x1000);
	#endif
}

void mRead_flash(void){
	#if DEBUG_SERIAL
		HAL_UART_Transmit(&huart1, (uint8_t*) "reading flash\r\n", 15, 0x1000);
	#endif
	uint32_t *source_addr = (uint32_t *)SETTINGS_FLASH_PAGE_ADDR;
    uint32_t *dest_addr = (void*)&controllerSettings;

    for (uint16_t i=0; i<CONTROLLER_LENGTH_WORDS; i++) {
        *dest_addr = *(__IO uint32_t*)source_addr;
        source_addr++;
        dest_addr++;
    }

	#if DEBUG_SERIAL
		HAL_UART_Transmit(&huart1, (uint8_t*) "Flash read\r\n", 12, 0x1000);
	#endif
}

