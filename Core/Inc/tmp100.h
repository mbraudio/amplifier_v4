/*
 * tmp100.h
 *
 *  Created on: 16. pro 2021.
 *      Author: Admin
 */

#ifndef INC_TMP100_H_
#define INC_TMP100_H_

#include "main.h"

#define TMP100_ADDRESS_L			0b10011100
#define TMP100_ADDRESS_L_READ		0b10011101
#define TMP100_ADDRESS_R			0b10010100
#define TMP100_ADDRESS_R_READ		0b10010101

#define TMP100_REG_TEMPERATURE		0x00 // 16bit
#define TMP100_REG_CONFIGURATION	0x01 // 8bit
#define TMP100_REG_T_LOW			0x02 // 8bit
#define TMP100_REG_T_HIGH			0x03 // 8bit

// BITS: OS/ALERT | R1 | R0 | F1 | F0 | POL | TM | SD
#define TMP100_DEFAULT_CONFIGURATION	0b01100000 // 12bit conversion

// Temperature for protection trigger and release. Temp. protection is self releasing!
#define PROTECTION_TEMPERATURE_ON	55 // On this temperature turn on protection
#define PROTECTION_TEMPERATURE_OFF	50 // On this temperature release protection


void TMP100_Init(I2C_HandleTypeDef* h);
void TMP100_WriteConfiguration(const uint8_t address);
int16_t TMP100_ReadTemperature(const uint8_t address);
void TMP100_ReadTemperatures(void);
void TMP100_Process(void);

typedef struct
{
	int16_t 	rightChannel;
	int16_t		leftChannel;
	uint32_t 	timer;
} Temperature;

extern Temperature temperature;

#endif /* INC_TMP100_H_ */
