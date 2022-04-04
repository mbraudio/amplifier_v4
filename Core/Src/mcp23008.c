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

#define MCP23008_TIMEOUT 100

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

static uint8_t command = 0;

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
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_Stop(void)
{
	command = 0;
	uint8_t data[2] = { OLAT, 0x00 };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

// Volume
void MCP23008_VolumePlus(void)
{
	command |= 0x01;
	command &= (uint8_t)~0x02;
	uint8_t data[2] = { OLAT, command/*0x01*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_VolumeMinus(void)
{
	command |= 0x02;
	command &= (uint8_t)~0x01;
	uint8_t data[2] = { OLAT, command/*0x02*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_VolumeStop(void)
{
	command &= (uint8_t)~0x01;
	command &= (uint8_t)~0x02;
	uint8_t data[2] = { OLAT, command };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

// Balance
void MCP23008_BalancePlus(void)
{
	command |= 0x08;
	command &= (uint8_t)~0x04;
	uint8_t data[2] = { OLAT, command/*0x08*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_BalanceMinus(void)
{
	command |= 0x04;
	command &= (uint8_t)~0x08;
	uint8_t data[2] = { OLAT, command/*0x04*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_BalanceStop(void)
{
	command &= (uint8_t)~0x08;
	command &= (uint8_t)~0x04;
	uint8_t data[2] = { OLAT, command };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

// Treble
void MCP23008_TreblePlus(void)
{
	command |= 0x10;
	command &= (uint8_t)~0x20;
	uint8_t data[2] = { OLAT, command/*0x10*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_TrebleMinus(void)
{
	command |= 0x20;
	command &= (uint8_t)~0x10;
	uint8_t data[2] = { OLAT, command/*0x20*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_TrebleStop(void)
{
	command &= (uint8_t)~0x10;
	command &= (uint8_t)~0x20;
	uint8_t data[2] = { OLAT, command };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

// Bass
void MCP23008_BassPlus(void)
{
	command |= 0x80;
	command &= (uint8_t)~0x40;
	uint8_t data[2] = { OLAT, command/*0x80*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_BassMinus(void)
{
	command |= 0x40;
	command &= (uint8_t)~0x80;
	uint8_t data[2] = { OLAT, command/*0x40*/ };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}

void MCP23008_BassStop(void)
{
	command &= (uint8_t)~0x80;
	command &= (uint8_t)~0x40;
	uint8_t data[2] = { OLAT, command };
	HAL_I2C_Master_Transmit(hi2c, MCP23008_I2C_WRITE_ADDRESS, data, 2, MCP23008_TIMEOUT);
}
