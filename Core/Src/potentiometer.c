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
#define POT_BT_SIZE (POT_SIZE * 2) + 2

#define INDEXES 150
Potentiometers potentiometers;
const uint8_t adcNormal[INDEXES] = { 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 11, 12, 13, 13, 14, 15, 16, 16, 17, 18, 19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 28, 29, 30, 31, 32, 32, 33, 34, 35, 36, 36, 37, 38, 38, 39, 40, 41, 41, 42, 43, 44, 44, 45, 46, 47, 47, 48, 49, 50, 51, 51, 52, 53, 54, 55, 56, 58, 59, 61, 62, 64, 65, 67, 69, 71, 73, 74, 76, 79, 81, 84, 87, 89, 92, 95, 99, 102, 106, 109, 113, 117, 121, 123, 128, 131, 135, 139, 143, 147, 151, 155, 160, 164, 170, 175, 179, 184, 191, 197, 204, 210, 217, 224, 233, 245, 253, 255, 255, 0,  };
//const uint8_t adcReversed[INDEXES] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 253, 252, 249, 248, 247, 245, 244, 243, 241, 240, 239, 237, 236, 234, 232, 231, 229, 227, 226, 225, 223, 221, 220, 219, 217, 216, 215, 213, 211, 210, 209, 207, 207, 205, 204, 202, 201, 200, 199, 197, 196, 195, 193, 192, 191, 190, 188, 187, 186, 185, 184, 182, 181, 180, 179, 178, 177, 175, 174, 173, 171, 169, 167, 164, 162, 160, 157, 155, 152, 150, 148, 142, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 108, 104, 101, 97, 94, 91, 87, 84, 81, 78, 75, 72, 69, 65, 63, 59, 57, 53, 50, 46, 43, 40, 37, 33, 30, 26, 22, 18, 14, 9, 7, 0,  };

//const uint8_t calibration[256] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 250, 243, 224, 210, 198, 188, 180, 172, 165, 157, 153, 146, 142, 136, 132, 127, 123, 118, 114, 110, 107, 104, 100, 97, 94, 91, 88, 85, 82, 80, 77, 74, 72, 69, 68, 65, 64, 62, 61, 59, 58, 57, 55, 54, 53, 51, 51, 50, 48, 47, 47, 46, 45, 45, 44, 43, 43, 42, 42, 41, 40, 40, 39, 39, 38, 37, 37, 36, 36, 35, 35, 34, 33, 32, 32, 31, 31, 30, 30, 29, 28, 28, 27, 26, 25, 25, 24, 24, 23, 22, 21, 21, 20, 19, 18, 18, 17, 16, 16, 15, 14, 13, 13, 12, 12, 11, 10, 10, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0 };

// CH0 - normal
//const uint8_t calibration0[256] = { 3, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 31, 32, 33, 33, 34, 34, 35, 36, 36, 37, 38, 38, 39, 39, 40, 41, 41, 42, 43, 43, 44, 44, 45, 46, 47, 47, 48, 49, 50, 52, 53, 54, 55, 56, 58, 59, 61, 62, 64, 65, 68, 69, 72, 74, 76, 79, 82, 85, 87, 90, 94, 97, 100, 103, 106, 110, 113, 117, 121, 125, 130, 134, 138, 143, 149, 154, 161, 167, 175, 181, 191, 200, 211, 224, 243, 249, 255, 255, 255, 255, 255, 255, 255, 0,  };

// CH1 - reverse
//const uint8_t calibration1[256] = { 4, 4, 4, 4, 4, 4, 4, 4, 7, 13, 18, 27, 44, 58, 69, 79, 88, 97, 102, 110, 115, 122, 127, 133, 137, 141, 145, 149, 153, 156, 160, 164, 167, 171, 173, 177, 179, 182, 185, 188, 191, 193, 195, 198, 200, 202, 204, 206, 207, 209, 210, 211, 213, 214, 215, 216, 217, 218, 219, 220, 221, 221, 222, 223, 224, 224, 225, 225, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 232, 232, 233, 233, 234, 234, 235, 236, 236, 237, 237, 238, 239, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 249, 249, 250, 251, 252, 252, 253, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,  };
//const uint8_t calibration[256] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 253, 253, 252, 251, 251, 250, 249, 248, 247, 246, 246, 245, 245, 244, 243, 243, 242, 241, 240, 239, 239, 238, 238, 237, 236, 236, 235, 234, 234, 233, 233, 232, 232, 231, 230, 230, 229, 228, 227, 227, 227, 226, 225, 225, 224, 224, 223, 222, 221, 220, 220, 219, 218, 217, 216, 214, 213, 212, 210, 210, 208, 206, 205, 203, 201, 198, 196, 193, 191, 188, 186, 183, 180, 177, 174, 171, 169, 165, 162, 158, 155, 151, 148, 143, 140, 135, 131, 126, 121, 115, 109, 103, 98, 89, 82, 70, 60, 48, 33, 20, 15, 11, 5, 4, 4, 4, 0,  };


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
	// Reset last so it updates in mobile GUI
	uint32_t i;
	for (i = 0; i < POT_SIZE; i++)
	{
		potentiometers.pots[i].last = 0xFFFFFFFF;
	}

	// Enable or disable updates
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
	potentiometers.pots[POT_INDEX_VOLUME].currentReverse = volume1;
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
		uint8_t rx[POT_BT_SIZE];
		uint32_t index;

		for (i = 0; i < POT_SIZE; i++)
		{
			Potentiometer* pot = &potentiometers.pots[i];
			//if ((pot->last != pot->current) || (pot->logarithmic && (pot->lastReverse != pot->currentReverse)))
			if (pot->last != pot->current)
			{
				pot->last = pot->current;
				pot->lastCount = 0;
				potentiometers.send = 1;
				//pot->lastReverse = pot->currentReverse;
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
			rx[index] = pot->logarithmic ? POTENTIOMETERS_GetIndexFromValue(pot) : pot->last;
			rx[index + 1] = pot->active;
		}

		if (potentiometers.send)
		{
			potentiometers.send = 0;
			rx[0] = COMMAND_UPDATE_POTENTIOMETERS;
			rx[POT_BT_SIZE - 1] = BLUETOOTH_CalculateCrc(rx, POT_BT_SIZE - 1);
			BLUETOOTH_SendData(rx, POT_BT_SIZE);
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
