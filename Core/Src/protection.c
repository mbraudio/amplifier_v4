/*
 * protection.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "protection.h"
//#include "amp.h" //TODO: Enable and implement real protections !!!
#include "utilities.h"
#include <string.h>
#include "main.h"

// Errors
#define ERROR_1_DC						1
#define ERROR_2_OVERLOAD				2
#define ERROR_3_OVERHEAT				3
#define ERROR_4_FAULTY_VOLTAGE			4
// Flags
#define PROTECTION_CLEAR_FLAG			0xFF
#define PROTECTION_ENABLED_FLAG_1		0xAA
#define PROTECTION_ENABLED_FLAG_2		0x55
#define PROTECTION_DELAY_SHORT			200
#define PROTECTION_DELAY_LONG 			2000

Protection protection;

inline void PROTECTION_Save(void)
{
	//FRAM_Write(FRAM_PROTECTION_INDEX, PROTECTION_SIZE, (uint8_t*)&protection);
}

inline void PROTECTION_Load(void)
{
	//FRAM_Read(FRAM_PROTECTION_INDEX, PROTECTION_SIZE, (uint8_t*)&protection);
}

void PROTECTION_Reset(void)
{
	memset((uint8_t*)&protection, PROTECTION_CLEAR_FLAG, sizeof(Protection));
	PROTECTION_Save();
}

void PROTECTION_EnableDc(void)
{
	protection.dc1 = PROTECTION_ENABLED_FLAG_1;
	protection.dc2 = PROTECTION_ENABLED_FLAG_2;
	PROTECTION_Save();
	//AMP_PowerOff(); //TODO: Enable...
}

void PROTECTION_EnableOverheat(void)
{
	protection.overheat1 = PROTECTION_ENABLED_FLAG_1;
	protection.overheat2 = PROTECTION_ENABLED_FLAG_2;
	PROTECTION_Save();
	//AMP_PowerOff(); //TODO: Enable...
}

void PROTECTION_EnableVoltage(void)
{
	protection.voltage1 = PROTECTION_ENABLED_FLAG_1;
	protection.voltage2 = PROTECTION_ENABLED_FLAG_2;
	PROTECTION_Save();
	//AMP_PowerOff(); //TODO: Enable...
}

void PROTECTION_NotifyError(const uint32_t errorId)
{
	uint32_t i;
	while (1)
	{
		for (i = 0; i < errorId; i++)
		{
			// Reset Watchdog
			//UTIL_ResetWatchdog();
			HAL_GPIO_TogglePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin);
			HAL_Delay(PROTECTION_DELAY_SHORT);
			HAL_GPIO_TogglePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin);
			HAL_Delay(PROTECTION_DELAY_SHORT);
		}
		HAL_Delay(PROTECTION_DELAY_LONG);
	}
}

void PROTECTION_LoadCheck(void)
{
	PROTECTION_Load();

	if ((protection.dc1 == PROTECTION_ENABLED_FLAG_1) && (protection.dc2 == PROTECTION_ENABLED_FLAG_2))
	{
		PROTECTION_NotifyError(ERROR_1_DC);
	}

	if ((protection.overheat1 == PROTECTION_ENABLED_FLAG_1) && (protection.overheat2 == PROTECTION_ENABLED_FLAG_2))
	{
		PROTECTION_NotifyError(ERROR_3_OVERHEAT);
	}

	if ((protection.voltage1 == PROTECTION_ENABLED_FLAG_1) && (protection.voltage2 == PROTECTION_ENABLED_FLAG_2))
	{
		PROTECTION_NotifyError(ERROR_4_FAULTY_VOLTAGE);
	}
}

void PROTECTION_LiveCheck(void)
{
	// TODO: Enable this...
	// DC - PD2 (not used here due schematics bug...)
	// Overload - PD3
	/*
	GPIO_PinState state;
	state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3);
	if (state == 0)
	{
		HAL_Delay(20);
		state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3);
		if (state == 0)
		{
			PROTECTION_EnableOverload();
		}
	}

	// Thermal - PD4
	state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4);
	if (state == 0)
	{
		HAL_Delay(20);
		state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4);
		if (state == 0)
		{
			PROTECTION_EnableOverheat();
		}
	}

	// Voltage - PD5
	state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5);
	if (state == 0)
	{
		HAL_Delay(20);
		state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5);
		if (state == 0)
		{
			PROTECTION_EnableVoltage();
		}
	}
	*/
}



