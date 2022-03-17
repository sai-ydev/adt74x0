/*
 *   ADT74x0.h
 * 	 This header files contains all the register definitions
 * 	 and function calls used to interface the ADC chip
 *
 *
 */



#ifndef ADT74x0_H_
#define ADT74x0_H_

#include "stm32l4xx_hal.h"

/** definitions **/


/* ADT7420 register addresses */
#define TMP_MSB_REG 				0x00
#define TMP_LSB_REG					0x01
#define STATUS_REG					0x02
#define CONFIG_REG					0x03
#define T_HIGH_SETPOINT_MSB_REG		0x04
#define T_HIGH_SETPOINT_LSB_REG		0x05
#define T_LOW_SETPOINT_MSB_REG		0x06
#define T_LOW_SETPOINT_LSB_REG		0x07
#define T_CRIT_SETPOINT_MSB_REG		0x08
#define T_CRIT_SETPOINT_LSB_REG		0x09
#define T_HYST_SETPOINT_REG			0x0A
#define ID_REG						0x0B
#define SOFTWARE_RESET				0x2F

/* ADT4x0 I2C address */
#define ADT74x0_ADDRESS				(0x48 << 1)
#define CHIP_ID						0xCB

#define HIGH						1
#define LOW							0

/* Configuration Parameters */
#define FAULT_TRIGGER_1				0x00
#define FAULT_TRIGGER_2				0x01
#define FAULT_TRIGGER_3				0x02
#define FAULT_TRIGGER_4				0x03

#define CT_PIN_POLARITY				(LOW << 2)
#define INT_PIN_POLARITY			(LOW << 3)
#define INT_CT_MODE					(HIGH << 4)

#define CONTINUOUS_CONVERSION_MODE	0x00
#define ONE_SHOT_MODE				0x20
#define ONE_SAMPLE_PER_SECOND_MODE	0x40
#define SHUTDOWN_MODE				0x60

#define RESOLUTION_13_BITS			0x00
#define RESOLUTION_16_BITS			0x80

/* Temperature setpoints */

#define TEMP_HIGH_SETPOINT			75
#define TEMP_LOW_SETPOINT			0
#define TEMP_CRITICAL_SETPOINT		100
#define TEMP_HYSTERESIS_SETPOINT	5

/************** Global Variable *******************/

extern uint8_t ui8config;


/************** Function Declarations ***********/

HAL_StatusTypeDef adt74x0_init(I2C_HandleTypeDef *i2c);
HAL_StatusTypeDef adt74x0_read_chip_id(I2C_HandleTypeDef *i2c, uint8_t *chip_id);
uint16_t adt74x0_read_temp(void);
uint16_t adt74x0_convert_degress_to_hex(int16_t degrees);
float adt74x0_convert_hex_to_degrees(uint16_t temp_data);
void adt74x0_power_down(void);
void adt74x0_power_up(void);
HAL_StatusTypeDef adt74x0_reset(I2C_HandleTypeDef *i2c);

#endif /* ADT74x0_H_ */
