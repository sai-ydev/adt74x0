/*
 * adt74x0.c
 * Drivers for operating ADT74x0 Temp. Sensor.
 *  Created on: Mar 15, 2022
 *      Author: yaman
 */
#include "adt74x0.h"

uint8_t ui8configAdt7420 = (FAULT_TRIGGER_4 | CT_PIN_POLARITY | INT_PIN_POLARITY | INT_CT_MODE |CONTINUOUS_CONVERSION_MODE | RESOLUTION_13_BITS);

uint8_t adt74x0_read_one_reg(I2C_HandleTypeDef *hi2c, uint8_t reg_address){

	uint8_t reg_data;

	reg_data = HAL_I2C_Mem_Read(hi2c, ADT74x0_ADDRESS, reg_address, 1, &reg_data, 1, HAL_MAX_DELAY);

	return reg_data;
}

void adt74x0_reset(I2C_HandleTypeDef *hi2c){
	uint8_t reset_cmd[1] = {SOFTWARE_RESET};
	HAL_I2C_Master_Transmit(hi2c, ADT74x0_ADDRESS, reset_cmd, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
}
