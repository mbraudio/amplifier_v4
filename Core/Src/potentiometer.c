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

#define POTENTIOMETERS_UPDATE_TIMEOUT 10 // POTENTIOMETERS_UPDATE_TIMEOUT * 10ms = X ms

#define POT_SIZE 4

#define INDEXES 150
Potentiometers potentiometers;
const uint8_t adcValues[INDEXES] = { 3, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 14, 15, 16, 17, 19, 20, 22, 24, 27, 28, 30, 32, 35, 38, 48, 50, 54, 55, 58, 60, 64, 67, 70, 73, 76, 78, 81, 84, 87, 88, 91, 94, 95, 98, 100, 101, 104, 106, 108, 110, 111, 113, 115, 117, 119, 120, 122, 124, 125, 127, 128, 130, 131, 133, 134, 135, 137, 138, 139, 140, 142, 143, 144, 145, 146, 148, 149, 150, 151, 152, 155, 156, 158, 160, 163, 165, 166, 169, 171, 172, 174, 176, 178, 184, 187, 189, 190, 192, 192, 195, 198, 199, 202, 204, 206, 207, 209, 210, 212, 214, 216, 217, 219, 220, 222, 224, 225, 227, 228, 229, 231, 232, 234, 236, 238, 239, 241, 244, 246, 248, 252, 255, 255, 255, 255 };

//uint8_t testData[256];

void POTENTIOMETER_Init(Potentiometer* pot, void(*plus)(void), void(*minus)(void), void(*stop)(void), const uint32_t logarithmic, const uint8_t command);

void POTENTIOMETERS_Initialize(void)
{
	potentiometers.activeIR = 0;
	potentiometers.update = 0;
	potentiometers.timer = 0;
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_VOLUME], POTENTIOMETERS_VolumePlus, POTENTIOMETERS_VolumeMinus, POTENTIOMETERS_Stop, 1, COMMAND_UPDATE_VOLUME_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_BASS], POTENTIOMETERS_BassPlus, POTENTIOMETERS_BassMinus, POTENTIOMETERS_Stop, 0, COMMAND_UPDATE_BASS_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_TREBLE], POTENTIOMETERS_TreblePlus, POTENTIOMETERS_TrebleMinus, POTENTIOMETERS_Stop, 0, COMMAND_UPDATE_TREBLE_VALUE);
	POTENTIOMETER_Init(&potentiometers.pots[POT_INDEX_BALANCE], POTENTIOMETERS_BalancePlus, POTENTIOMETERS_BalanceMinus, POTENTIOMETERS_Stop, 0, COMMAND_UPDATE_BALANCE_VALUE);
}

void POTENTIOMETER_Init(Potentiometer* pot, void(*plus)(void), void(*minus)(void), void(*stop)(void), const uint32_t logarithmic, const uint8_t command)
{
	pot->current = 0;
	pot->currentReverse = 0;
	pot->last = 0;
	pot->lastReverse = 0;
	pot->required = 0;
	pot->active = 0;
	pot->command = command;
	pot->logarithmic = logarithmic;
	pot->plusFunction = plus;
	pot->minusFunction = minus;
	pot->stopFunction = stop;
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
	// Reset last so it updates in mobile GUI
	uint32_t i;
	for (i = 0; i < POT_SIZE; i++)
	{
		potentiometers.pots[i].last = 0;
	}

	// Enable or disable updates
	potentiometers.update = 1;
}

void POTENTIOMETERS_DisableUpdate(void)
{
	potentiometers.update = 0;
}

void POTENTIOMETERS_SetCurrent(const uint8_t volume0, const uint8_t volume1, const uint8_t bass, const uint8_t treble, const uint8_t balance)
{
	potentiometers.pots[POT_INDEX_VOLUME].current = volume0;
	potentiometers.pots[POT_INDEX_VOLUME].currentReverse = volume1;
	potentiometers.pots[POT_INDEX_BASS].current = bass;
	potentiometers.pots[POT_INDEX_TREBLE].current = treble;
	potentiometers.pots[POT_INDEX_BALANCE].current = balance;
}

void POTENTIOMETERS_Process(void)
{
	uint32_t i;
	for (i = 0; i < POT_SIZE; i++)
	{
		Potentiometer* pot = &potentiometers.pots[i];
		if (pot->active)
		{
			if (pot->required > pot->current)
			{
				pot->plusFunction();
			}
			else if (pot->required < pot->current)
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
		for (i = 0; i < POT_SIZE; i++)
		{
			Potentiometer* pot = &potentiometers.pots[i];
			if ((pot->last != pot->current) || (pot->logarithmic && (pot->lastReverse != pot->currentReverse)))
			{
				pot->last = pot->current;
				pot->lastReverse = pot->currentReverse;
				BLUETOOTH_Send2(pot->command, pot->logarithmic ? POTENTIOMETERS_GetIndexFromValue(pot) : pot->last, (uint8_t)pot->active);
			}
		}

		/*
		uint32_t i, parts, remaining, index;
		parts = 256 / 16;
		remaining = 256 % 16;
		for (i = 0; i < parts; i++)
		{
			index = i * 16;
			BLUETOOTH_SendData(testData + index, 16);
		}

		if (remaining)
		{
			BLUETOOTH_SendData(testData + index, remaining);
		}
		*/

		potentiometers.timer = 0;
	}
}

uint8_t POTENTIOMETERS_GetIndexFromValue(Potentiometer* pot)
{
	// TODO: Needs data from inverse side... Logarithmic pot has 2 reversed adc channels!
	uint32_t i;
	for (i = 0; i < INDEXES; i++)
	{
		if (pot->last <= adcValues[i])
		{
			return i;
		}
	}
	return 0;
}

uint8_t POTENTIOMETERS_GetValueFromIndex(const uint8_t index)
{
	return adcValues[index];
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
