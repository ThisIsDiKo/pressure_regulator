/*
 * globals.h
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <string.h>

#define DEBUG_SERIAL	0

#define BLYNK_LED_PORT							GPIOB
#define BLYNK_LED_PIN							GPIO_PIN_6
#define A_LED_PORT							GPIOB
#define A_LED_PIN							GPIO_PIN_7
#define B_LED_PORT							GPIOB
#define B_LED_PIN							GPIO_PIN_8
#define C_LED_PORT							GPIOB
#define C_LED_PIN							GPIO_PIN_9

#define BTN_PORT							GPIOB
#define BTN_PIN							GPIO_PIN_5

#define SW1_PORT								GPIOC
#define SW1_PIN									GPIO_PIN_9
#define SW2_PORT								GPIOC
#define SW2_PIN									GPIO_PIN_8

#define C1_UP_PORT								GPIOC
#define C1_UP_PIN								GPIO_PIN_3
#define C1_DOWN_PORT							GPIOC
#define C1_DOWN_PIN								GPIO_PIN_2
#define C2_UP_PORT								GPIOC
#define C2_UP_PIN								GPIO_PIN_1
#define C2_DOWN_PORT							GPIOC
#define C2_DOWN_PIN								GPIO_PIN_0
#define C3_UP_PORT								GPIOC
#define C3_UP_PIN								GPIO_PIN_12
#define C3_DOWN_PORT							GPIOC
#define C3_DOWN_PIN								GPIO_PIN_11
#define C4_UP_PORT								GPIOC
#define C4_UP_PIN								GPIO_PIN_10
#define C4_DOWN_PORT							GPIOA
#define C4_DOWN_PIN								GPIO_PIN_15

#define CMD_RF_PORT								GPIOD
#define CMD_RF_PIN								GPIO_PIN_2

#define C1_UP_ON								HAL_GPIO_WritePin(C1_UP_PORT, C1_UP_PIN, GPIO_PIN_SET)
#define C1_UP_OFF								HAL_GPIO_WritePin(C1_UP_PORT, C1_UP_PIN, GPIO_PIN_RESET)
#define C1_DOWN_ON								HAL_GPIO_WritePin(C1_DOWN_PORT, C1_DOWN_PIN, GPIO_PIN_SET)
#define C1_DOWN_OFF								HAL_GPIO_WritePin(C1_DOWN_PORT, C1_DOWN_PIN, GPIO_PIN_RESET)

#define C2_UP_ON								HAL_GPIO_WritePin(C2_UP_PORT, C2_UP_PIN, GPIO_PIN_SET)
#define C2_UP_OFF								HAL_GPIO_WritePin(C2_UP_PORT, C2_UP_PIN, GPIO_PIN_RESET)
#define C2_DOWN_ON								HAL_GPIO_WritePin(C2_DOWN_PORT, C2_DOWN_PIN, GPIO_PIN_SET)
#define C2_DOWN_OFF								HAL_GPIO_WritePin(C2_DOWN_PORT, C2_DOWN_PIN, GPIO_PIN_RESET)

#define C3_UP_ON								HAL_GPIO_WritePin(C3_UP_PORT, C3_UP_PIN, GPIO_PIN_SET)
#define C3_UP_OFF								HAL_GPIO_WritePin(C3_UP_PORT, C3_UP_PIN, GPIO_PIN_RESET)
#define C3_DOWN_ON								HAL_GPIO_WritePin(C3_DOWN_PORT, C3_DOWN_PIN, GPIO_PIN_SET)
#define C3_DOWN_OFF								HAL_GPIO_WritePin(C3_DOWN_PORT, C3_DOWN_PIN, GPIO_PIN_RESET)

#define C4_UP_ON								HAL_GPIO_WritePin(C4_UP_PORT, C4_UP_PIN, GPIO_PIN_SET)
#define C4_UP_OFF								HAL_GPIO_WritePin(C4_UP_PORT, C4_UP_PIN, GPIO_PIN_RESET)
#define C4_DOWN_ON								HAL_GPIO_WritePin(C4_DOWN_PORT, C4_DOWN_PIN, GPIO_PIN_SET)
#define C4_DOWN_OFF								HAL_GPIO_WritePin(C4_DOWN_PORT, C4_DOWN_PIN, GPIO_PIN_RESET)

#define CMD_RF_OFF								HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define CMD_RF_ON								HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)

#define MAX_COMMAND_LENGTH						64
#define COMMAND_QUEUE_SIZE						2


#define SENS_1						0
#define SENS_2						1
#define SENS_3						2
#define SENS_4						3

//#define SENS_1						3
//#define SENS_2						2
//#define SENS_3						1
//#define SENS_4						0

#define PRESSURE_ACCURACY			75

#endif /* GLOBALS_H_ */
