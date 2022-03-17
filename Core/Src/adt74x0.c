/*
 * adt74x0.c
 * Drivers for operating ADT74x0 Temp. Sensor.
 *  Created on: Mar 15, 2022
 *      Author: yaman
 */
#include "adt74x0.h"

uint8_t ui8configAdt7420 = (FAULT_TRIGGER_4 | CT_PIN_POLARITY | INT_PIN_POLARITY
		| INT_CT_MODE | CONTINUOUS_CONVERSION_MODE | RESOLUTION_13_BITS);

HAL_StatusTypeDef adt74x0_init(I2C_HandleTypeDef *hi2c) {
	HAL_StatusTypeDef rslt;

	rslt = adt74x0_reset(hi2c);

	if (rslt != HAL_OK) {
		return rslt;
	}

	HAL_Delay(1);

	rslt = HAL_I2C_Mem_Write(hi2c, ADT74x0_ADDRESS, CONFIG_REG, 1,
			&ui8configAdt7420, 1, HAL_MAX_DELAY);

	if (rslt != HAL_OK) {
		return rslt;
	}

	rslt = adt74x0_write_temp_reg(hi2c, T_HIGH_SETPOINT_MSB_REG, TEMP_HIGH_SETPOINT, 2);

	rslt = adt74x0_write_temp_reg(hi2c, T_LOW_SETPOINT_MSB_REG, TEMP_LOW_SETPOINT, 2);

	rslt = adt74x0_write_temp_reg(hi2c, T_CRIT_SETPOINT_MSB_REG, TEMP_CRITICAL_SETPOINT, 2);

	rslt = adt74x0_write_temp_reg(hi2c, T_HYST_SETPOINT_REG, TEMP_HYSTERESIS_SETPOINT, 1);

	return rslt;
}

HAL_StatusTypeDef adt74x0_read_chip_id(I2C_HandleTypeDef *i2c, uint8_t *chip_id) {

	uint8_t result = HAL_I2C_Master_Transmit(i2c, ADT74x0_ADDRESS, chip_id, 1,
			HAL_MAX_DELAY);

	return result;
}

HAL_StatusTypeDef adt74x0_write_temp_reg(I2C_HandleTypeDef *i2c,
		uint8_t temp_reg, int16_t degrees, uint8_t num_bytes) {

	uint8_t result;
	uint16_t volatile temp_set_point = 0;
	uint8_t payload[2];

	temp_set_point = adt74x0_convert_degrees_to_hex(degrees);

	if (num_bytes == 2) {
		payload[0] = temp_set_point >> 8;
		payload[1] = temp_set_point & 0x00FF;
	} else if (num_bytes == 1) {
		payload[0] = temp_set_point;
	}

	result = HAL_I2C_Mem_Write(i2c, ADT74x0_ADDRESS, temp_reg, 1, payload,
			num_bytes, HAL_MAX_DELAY);

	return result;
}
/**
 * Copied directly from Analog's drivers
 @brief Converts the degrees C to hex
 @param i16degrees - temperature in degrees C
 @return ui16degreeInHex - temperature digital word
 **/

uint16_t adt74x0_convert_degrees_to_hex(int16_t i16degrees) {

	uint16_t ui16degreeInHex = 0;

	if ((ui8configAdt7420 & 0x80) == 0x80) { /* Checking to see if coding is 16- or 13- bit */
		if (i16degrees < 0) {
			ui16degreeInHex = (i16degrees * 128) + 65536; /* 16-bit negative temp to hex conversion */
		} else {
			ui16degreeInHex = i16degrees * 128; /* 16-bit positive temp to hex conversion */
		}
	}

	else {
		if (i16degrees < 0) {
			ui16degreeInHex = (i16degrees * 16) + 8192; /* 13-bit negative temp to hex conversion */
			ui16degreeInHex = ui16degreeInHex << 3;
		}

		else {
			ui16degreeInHex = i16degrees * 16; /* 13-bit positive temp to hex conversion */
			ui16degreeInHex = ui16degreeInHex << 3;
		}
	}

	return ui16degreeInHex;
}

/**
 * Copied directly from Analog's drivers
 @brief Converts hex to degrees C
 @param ui16tempResults - temperature data in hex
 @return fTemp - temperature in degrees C
 **/

float adt74x0_convert_hex_to_degrees(uint16_t ui16tempResults) {

	float fTemp = 0.0;

	//Check the status of the temperature sign bit (MSB)
	if ((ui16tempResults & 0x8000) == 0x8000) { /* If sign bit is 1 use the negative temperature equation */

		if ((ui8configAdt7420 & 0x80) == 0x80) {
			fTemp = (((float) ui16tempResults - 65536) / 128); /* 16-bit temperature word data */
		}

		else {
			ui16tempResults = ui16tempResults >> 3;
			fTemp = (((float) ui16tempResults - 8192) / 16); /* 13-bit temperature word data */
		}
	}

	else { /*If sign bit is 0, use the positive temperature equation */

		if ((ui8configAdt7420 & 0x80) == 0x80) {
			fTemp = (float) ui16tempResults / 128; /* 16-bit temperature word data */
		}

		else {
			ui16tempResults = ui16tempResults >> 3;
			fTemp = ((float) ui16tempResults / 16); /* 13-bit temperature word data */
		}

	}
	return fTemp;
}

HAL_StatusTypeDef adt74x0_reset(I2C_HandleTypeDef *hi2c) {

	uint8_t reset_cmd[1] = { SOFTWARE_RESET };
	return HAL_I2C_Master_Transmit(hi2c, ADT74x0_ADDRESS, reset_cmd, 1,
			HAL_MAX_DELAY);
}
