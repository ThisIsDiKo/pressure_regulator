/*
 * structures.h
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: ADiKo
 */

#include "globals.h"

struct Filtered{
	uint16_t sens_1;
	uint16_t sens_2;
	uint16_t sens_3;
	uint16_t sens_4;
};

struct controllerData{
	uint8_t  rfChannel;
	uint8_t  rere;
	uint16_t rfBaudRate;
	uint16_t clientID;
	uint16_t qeqe;
	//uint32_t rfID;
	uint32_t rfMaster;
	uint16_t storedPos_1[4];
	uint16_t storedPos_2[4];
	uint16_t storedPos_3[4];
};

enum WorkState{
	FREE,
	WORKING
};

enum Compensation{
	OFF,
	ON
};

