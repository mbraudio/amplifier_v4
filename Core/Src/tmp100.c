/*
 * tmp100.c
 *
 *  Created on: 16. pro 2021.
 *      Author: Admin
 */
#include "tmp100.h"
#include "bluetooth.h"

#define TMP100_TIMEOUT 100
#define TMP100_READ_TIME 300 // x 10ms = XXX

I2C_HandleTypeDef* thi2c;
Temperature temperature;

void TMP100_Init(I2C_HandleTypeDef* h)
{
	thi2c = h;
	TMP100_WriteConfiguration(TMP100_ADDRESS_R);
	TMP100_WriteConfiguration(TMP100_ADDRESS_L);
	temperature.rightChannel = 0;
	temperature.leftChannel = 0;
	temperature.timer = 0;
}

void TMP100_WriteConfiguration(const uint8_t address)
{
	uint8_t data[2] = { TMP100_REG_CONFIGURATION, TMP100_DEFAULT_CONFIGURATION };
	HAL_I2C_Master_Transmit(thi2c, address, data, 2, TMP100_TIMEOUT);
	// Return address to 0 for temperature readout
	data[0] = TMP100_REG_TEMPERATURE;
	HAL_I2C_Master_Transmit(thi2c, address, data, 1, TMP100_TIMEOUT);
}

int16_t TMP100_ReadTemperature(const uint8_t address)
{
	uint8_t data[2] = { 0x00, 0x00 };
	HAL_I2C_Master_Receive(thi2c, address, data, 2, TMP100_TIMEOUT);
	uint32_t temp = ((data[0] << 8) + data[1]) >> 4;
	uint32_t cels = ((temp * 625) / 10000);
	if (temp >= 0x800) {
		cels -= 256; // Negative, minus values
	}
	return cels;
}

void TMP100_ReadTemperatures(void)
{
	temperature.rightChannel = TMP100_ReadTemperature(TMP100_ADDRESS_R_READ);
	temperature.leftChannel = TMP100_ReadTemperature(TMP100_ADDRESS_L_READ);
}

void TMP100_Process(void)
{
	if (temperature.timer > TMP100_READ_TIME)
	{
		temperature.timer = 0;
		TMP100_ReadTemperatures();
		BLUETOOTH_Send2(COMMAND_UPDATE_TEMPERATURE, temperature.rightChannel, temperature.leftChannel);
	}
}
