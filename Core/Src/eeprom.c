/*
 * eeprom.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */
#include "eeprom.h"
#include "stm32l0xx.h"
#include <string.h>

void EEPROM_Write(uint32_t address, const uint32_t size, uint8_t* data)
{
	uint32_t i;
	address += DATA_EEPROM_BASE;

	HAL_FLASHEx_DATAEEPROM_Unlock();

	for (i = 0; i < size; i++)
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, data[i]);
	}

	HAL_FLASHEx_DATAEEPROM_Lock();
}

void EEPROM_Read(uint32_t address, const uint32_t size, uint8_t* data)
{
	address += DATA_EEPROM_BASE;
	memcpy(data, (uint32_t*)address, size);
}

