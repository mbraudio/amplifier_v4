/*
 * utilities.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */
#include "utilities.h"

uint8_t UTILITIES_CalculateCrc(uint8_t* buffer, const uint32_t size)
{
	uint32_t i;
	uint8_t crc;
	crc = 0;
	for (i = 0; i < size; i++)
	{
		crc += buffer[i];
	}
	return crc;
}

