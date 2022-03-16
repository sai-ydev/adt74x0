/*
 * adt74x0.c
 * Drivers for operating ADT74x0 Temp. Sensor.
 *  Created on: Mar 15, 2022
 *      Author: yaman
 */
#include "adt74x0.h"



void adt74x0_reset(I2C_HandleTypeDef *hi2c){
	uint8_t reset_cmd[1] = {SOFTWARE_RESET};
	HAL_I2C_Master_Transmit(hi2c, ADT74x0_ADDRESS, reset_cmd, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
}
