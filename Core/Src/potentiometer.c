/*
 * potentiometer.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "potentiometer.h"
#include "main.h"
#include "adc.h"
#include "mcp23008.h"
#include "bluetooth.h"
#include "utilities.h"

#define POTENTIOMETERS_UPDATE_TIMEOUT 25 // POTENTIOMETERS_UPDATE_TIMEOUT * 10ms = X ms


#define INDEXES 150
Potentiometers potentiometers;
const uint8_t adcNormal[INDEXES] = { 0, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 7, 8, 8, 9, 10, 11, 11, 12, 12, 14, 15, 16, 17, 18, 18, 20, 21, 22, 24, 26, 28, 30, 33, 36, 36, 39, 49, 50, 52, 55, 57, 59, 61, 63, 66, 68, 69, 71, 73, 75, 77, 78, 79, 81, 83, 84, 85, 87, 88, 88, 89, 91, 92, 93, 93, 95, 96, 97, 98, 98, 98, 100, 101, 101, 102, 103, 103, 104, 105, 105, 106, 107, 107, 108, 108, 109, 109, 110, 110, 111, 112, 113, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 130, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 144, 145, 145, 147, 148, 150, 152, 153, 155, 157, 160, 162, 164, 168, 171, 175, 180, 187, 194, 207, 225, 234, 239, 247, 255  };


void POTENTIOMETER_Init(Potentiometer* pot, void(*plus)(void), void(*minus)(void), void(*stop)(void), const uint32_t logarithmic, const uint8_t command);

void POTENTIOMETERS_Initialize(void)
{
	potentiometers.activeIR = 0;
	potentiometers.update = 0;
	potentiometers.timer = 0;
	potentiometers.send = 0;
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_VOLUME], POTENTIOMETERS_VolumePlus, POTENTIOMETERS_VolumeMinus, POTENTIOMETERS_VolumeStop, 1, COMMAND_UPDATE_VOLUME_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_BASS], POTENTIOMETERS_BassPlus, POTENTIOMETERS_BassMinus, POTENTIOMETERS_BassStop, 0, COMMAND_UPDATE_BASS_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_TREBLE], POTENTIOMETERS_TreblePlus, POTENTIOMETERS_TrebleMinus, POTENTIOMETERS_TrebleStop, 0, COMMAND_UPDATE_TREBLE_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_BALANCE], POTENTIOMETERS_BalancePlus, POTENTIOMETERS_BalanceMinus, POTENTIOMETERS_BalanceStop, 0, COMMAND_UPDATE_BALANCE_VALUE);
}

void POTENTIOMETER_Init(Potentiometer* pot, void(*plus)(void), void(*minus)(void), void(*stop)(void), const uint32_t logarithmic, const uint8_t command)
{
	pot->current = 0;
	pot->actual = 0;
	pot->last = 0;
	pot->required = 0;
	pot->active = 0;
	pot->command = command;
	pot->logarithmic = logarithmic;
	pot->plusFunction = plus;
	pot->minusFunction = minus;
	pot->stopFunction = stop;
	pot->lastCount = 0;
	for (uint32_t i = 0; i < POT_MAX_VALUES; i++)
	{
		pot->values[i] = 0;
	}
}

void POTENTIOMETERS_Start(const uint8_t potIndex, const uint8_t index)
{
	Potentiometer* pot = &potentiometers.pots[potIndex];
	pot->active = 0;
	pot->required = pot->logarithmic ? POTENTIOMETERS_GetValueFromIndex(index) : index;
	pot->active = 1;
	potentiometers.timer = 0;
}

void POTENTIOMETERS_EnableUpdate(void)
{
	// Disable updates
	potentiometers.update = 0;

	// Reset last so it updates in mobile GUI
	uint32_t i;
	for (i = 0; i < POT_SIZE; i++)
	{
		potentiometers.pots[i].last = 0xFFFFFFFF;
	}

	// Enable updates
	potentiometers.update = 1;
}

void POTENTIOMETERS_DisableUpdate(void)
{
	potentiometers.update = 0;
}

void POTENTIOMETERS_SetValue(const uint32_t index, const uint8_t value)
{
	uint32_t sum = 0;
	uint32_t i;

	Potentiometer* pot = &potentiometers.pots[index];

	for (i = 0; i < POT_MAX_VALUES - 1; i++)
	{
		pot->values[i] = pot->values[i + 1];
	}

	pot->values[POT_MAX_VALUES - 1] = value;

	for (i = 0; i < POT_MAX_VALUES; i++)
	{
		sum += pot->values[i];
	}

	pot->current = sum / POT_MAX_VALUES;
	pot->actual = value;
}

void POTENTIOMETERS_SetCurrent(const uint8_t volume0, const uint8_t volume1, const uint8_t bass, const uint8_t treble, const uint8_t balance)
{
	POTENTIOMETERS_SetValue(POT_INDEX_VOLUME, volume0);
	POTENTIOMETERS_SetValue(POT_INDEX_BASS, bass);
	POTENTIOMETERS_SetValue(POT_INDEX_TREBLE, treble);
	POTENTIOMETERS_SetValue(POT_INDEX_BALANCE, balance);
}

void POTENTIOMETERS_Process(void)
{
	uint32_t i;
	for (i = 0; i < POT_SIZE; i++)
	{
		Potentiometer* pot = &potentiometers.pots[i];
		if (pot->active)
		{
			if (pot->required > pot->actual)
			{
				pot->plusFunction();
			}
			else if (pot->required < pot->actual)
			{
				pot->minusFunction();
			}
			else
			{
				pot->stopFunction();
				pot->active = 0;
			}
		}
	}

	if (potentiometers.update && (potentiometers.timer >= POTENTIOMETERS_UPDATE_TIMEOUT))
	{
		uint32_t index;

		for (i = 0; i < POT_SIZE; i++)
		{
			Potentiometer* pot = &potentiometers.pots[i];
			if (pot->last != pot->current)
			{
				pot->last = pot->current;
				pot->lastCount = 0;
				potentiometers.send = 1;
			}
			else
			{
				if (pot->active)
				{
					pot->lastCount++;
					if (pot->lastCount > POT_LAST_COUNT_MAX)
					{
						pot->stopFunction();
						pot->active = 0;
						pot->lastCount = 0;
					}
				}
			}

			index = (i * 2) + 1;
			potentiometers.data[index] = pot->logarithmic ? POTENTIOMETERS_GetIndexFromValue(pot) : pot->last;
			potentiometers.data[index + 1] = pot->active;
		}

		if (potentiometers.send)
		{
			potentiometers.send = 0;
			potentiometers.data[0] = COMMAND_UPDATE_POTENTIOMETERS;
			potentiometers.data[POT_DATA_SIZE - 1] = UTILITIES_CalculateCrc(potentiometers.data, POT_DATA_SIZE - 1);
			BLUETOOTH_SendData(potentiometers.data, POT_DATA_SIZE);
		}

		potentiometers.timer = 0;
	}
}

uint8_t POTENTIOMETERS_GetIndexFromValue(Potentiometer* pot)
{
	uint32_t i;
	for (i = 0; i < INDEXES; i++)
	{
		if (pot->last <= adcNormal[i])
		{
			return i;
		}
	}
	return 0;
}

uint8_t POTENTIOMETERS_GetValueFromIndex(const uint8_t index)
{
	return adcNormal[index];
}

void POTENTIOMETERS_Stop(void)
{
	MCP23008_Stop();
}

void POTENTIOMETERS_StopIr(void)
{
	potentiometers.activeIR = 0;
	MCP23008_Stop();
}

// VOLUME
void POTENTIOMETERS_VolumePlus(void)
{
	MCP23008_VolumePlus();
}

void POTENTIOMETERS_VolumePlusIr(void)
{
	potentiometers.activeIR = 1;
	POTENTIOMETERS_VolumePlus();
}

void POTENTIOMETERS_VolumeMinus(void)
{
	MCP23008_VolumeMinus();
}

void POTENTIOMETERS_VolumeMinusIr(void)
{
	potentiometers.activeIR = 1;
	POTENTIOMETERS_VolumeMinus();
}

void POTENTIOMETERS_VolumeStop(void)
{
	MCP23008_VolumeStop();
}

// BASS
void POTENTIOMETERS_BassPlus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_BassPlus();
}

void POTENTIOMETERS_BassMinus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_BassMinus();
}

void POTENTIOMETERS_BassStop(void)
{
	MCP23008_BassStop();
}

// TREBLE
void POTENTIOMETERS_TreblePlus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_TreblePlus();
}

void POTENTIOMETERS_TrebleMinus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_TrebleMinus();
}

void POTENTIOMETERS_TrebleStop(void)
{
	MCP23008_TrebleStop();
}

// BALANCE
void POTENTIOMETERS_BalancePlus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_BalancePlus();
}

void POTENTIOMETERS_BalanceMinus(void)
{
	potentiometers.activeIR = 1;
	MCP23008_BalanceMinus();
}

void POTENTIOMETERS_BalanceStop(void)
{
	MCP23008_BalanceStop();
}
