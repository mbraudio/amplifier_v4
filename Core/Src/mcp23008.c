/*
 * mcp23008.c
 *
 *  Created on: Nov 18, 2021
 *      Author: Admin
 */
#include "mcp23008.h"

// ADDRESESS
#define MCP23008_I2C_WRITE_ADDRESS  0x40
#define MCP23008_I2C_READ_ADDRESS   0x41

// REGISTERS
#define IODIR		0x00
#define IPOL		0x01
#define GPINTEN		0x02
#define DEFVAL		0x03
#define INTCON		0x04
#define IOCON		0x05
#define GPPU		0x06
#define INTF		0x07
#define INTCAP		0x08
#define GPIO		0x09
#define OLAT		0x0A

I2C_HandleTypeDef* hi2c;

void MCP23008_Init(I2C_HandleTypeDef* h)
{
	hi2c = h;
}

void MCP23008_Setup(void)
{
	// Disable reset
	HAL_GPIO_WritePin(VOL_RESET_GPIO_Port, VOL_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	// Set as outputs
	uint8_t data[2] = { IODIR, 0x00 };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, 100);
}

void MCP23008_VolumePlus(void)
{
	 uint8_t data[2] = { OLAT, 0x01 };
	 HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, 100);
}

void MCP23008_VolumeMinus(void)
{
	 uint8_t data[2] = { OLAT, 0x02 };
	 HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, 100);
}

void MCP23008_VolumeStop(void)
{
	uint8_t data[2] = { OLAT, 0x00 };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, 100);
}
