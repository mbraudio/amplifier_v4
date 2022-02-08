/*
 * protection.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "protection.h"
#include "amplifier.h"
#include "utilities.h"
#include "system.h"
#include "eeprom.h"
#include "main.h"
#include "tmp100.h"
#include <string.h>

// Errors
#define ERROR_3_DC						3
#define ERROR_4_VOLTAGE					4
#define ERROR_5_TEMPERATURE				5
// Flags
#define PROTECTION_CLEAR_FLAG			0x00
#define PROTECTION_ENABLED_FLAG_1		0xAA
#define PROTECTION_ENABLED_FLAG_2		0x55
#define PROTECTION_DELAY_SHORT			200
#define PROTECTION_DELAY_LONG 			1200

#define PROTECTION_SIZE					sizeof(Protection)
#define PROTECTION_EEPROM_ADDRESS		64

Protection protection;

void PROTECTION_Initialize(void)
{
	protection.dc1 = 0;
	protection.dc2 = 0;
	protection.temperature1 = 0;
	protection.temperature2 = 0;
	protection.voltage1 = 0;
	protection.voltage2 = 0;
	protection.crc = 0;

	// TODO: Disable this here!
	PROTECTION_Reset();
}

inline void PROTECTION_Save(void)
{
	protection.crc = UTILITIES_CalculateCrc((uint8_t*)&protection, (PROTECTION_SIZE - 1));
	EEPROM_Write(PROTECTION_EEPROM_ADDRESS, PROTECTION_SIZE, (uint8_t*)&protection);
}

inline uint32_t PROTECTION_Load(void)
{
	uint8_t crc;
	EEPROM_Read(PROTECTION_EEPROM_ADDRESS, PROTECTION_SIZE, (uint8_t*)&protection);
	crc = UTILITIES_CalculateCrc((uint8_t*)&protection, (PROTECTION_SIZE - 1));
	return (protection.crc == crc);
}

void PROTECTION_Reset(void)
{
	memset((uint8_t*)&protection, PROTECTION_CLEAR_FLAG, (PROTECTION_SIZE - 1));
	PROTECTION_Save();
}

void PROTECTION_Process(void)
{
	if (system.states.protectionTriggeredDc)
	{
		system.states.protectionTriggeredDc = 0;
		PROTECTION_EnableDc();
	}

	if (system.states.protectionTriggeredVoltage)
	{
		system.states.protectionTriggeredVoltage = 0;
		PROTECTION_EnableVoltage();
	}

	if (system.states.protectionTriggeredTemperature)
	{
		system.states.protectionTriggeredTemperature = 0;
		PROTECTION_EnableTemperature();
	}
}

void PROTECTION_EnableDc(void)
{
	// Immediately disable power to transformers
	AMP_SetPowerPin(GPIO_PIN_RESET);

	// Save new protection state
	if ((protection.dc1 != PROTECTION_ENABLED_FLAG_1) || (protection.dc2 != PROTECTION_ENABLED_FLAG_2))
	{
		protection.dc1 = PROTECTION_ENABLED_FLAG_1;
		protection.dc2 = PROTECTION_ENABLED_FLAG_2;
		PROTECTION_Save();
	}

	// Go to entire power off process
	AMP_GoToPowerOff();

	// Notify error
	PROTECTION_NotifyError(ERROR_3_DC);
}

void PROTECTION_EnableVoltage(void)
{
	// Immediately disable power to transformers
	AMP_SetPowerPin(GPIO_PIN_RESET);

	// Save new protection state
	if ((protection.voltage1 != PROTECTION_ENABLED_FLAG_1) || (protection.voltage2 != PROTECTION_ENABLED_FLAG_2))
	{
		protection.voltage1 = PROTECTION_ENABLED_FLAG_1;
		protection.voltage2 = PROTECTION_ENABLED_FLAG_2;
		PROTECTION_Save();
	}

	// Go to entire power off process
	AMP_GoToPowerOff();

	// Notify error
	PROTECTION_NotifyError(ERROR_4_VOLTAGE);
}

void PROTECTION_EnableTemperature(void)
{
	// Immediately disable power to transformers
	AMP_SetPowerPin(GPIO_PIN_RESET);

	// Save new protection state
	if ((protection.temperature1 != PROTECTION_ENABLED_FLAG_1) || (protection.temperature2 != PROTECTION_ENABLED_FLAG_2))
	{
		protection.temperature1 = PROTECTION_ENABLED_FLAG_1;
		protection.temperature2 = PROTECTION_ENABLED_FLAG_2;
		PROTECTION_Save();
	}

	// Go to entire power off process
	AMP_GoToPowerOff();

	// Notify error
	PROTECTION_NotifyError(ERROR_5_TEMPERATURE);
}

void PROTECTION_DisableTemperature(void)
{
	// Save new protection state
	if ((protection.temperature1 != PROTECTION_CLEAR_FLAG) || (protection.temperature2 != PROTECTION_CLEAR_FLAG))
	{
		protection.temperature1 = PROTECTION_CLEAR_FLAG;
		protection.temperature2 = PROTECTION_CLEAR_FLAG;
		PROTECTION_Save();
	}
}

void PROTECTION_NotifyError(const uint32_t errorId)
{
	HAL_GPIO_WritePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin, GPIO_PIN_SET);
	while (1)
	{
		for (uint32_t i = 0; i < errorId; i++)
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
	uint32_t status = PROTECTION_Load();
	if (!status)
	{
		PROTECTION_Reset();
		return;
	}

	if ((protection.dc1 == PROTECTION_ENABLED_FLAG_1) && (protection.dc2 == PROTECTION_ENABLED_FLAG_2))
	{
		PROTECTION_EnableDc();
	}

	if ((protection.voltage1 == PROTECTION_ENABLED_FLAG_1) && (protection.voltage2 == PROTECTION_ENABLED_FLAG_2))
	{
		PROTECTION_EnableVoltage();
	}

	if ((protection.temperature1 == PROTECTION_ENABLED_FLAG_1) && (protection.temperature2 == PROTECTION_ENABLED_FLAG_2))
	{
		TMP100_ReadTemperatures();
		if ((temperature.rightChannel >= PROTECTION_TEMPERATURE_ON) || (temperature.leftChannel >= PROTECTION_TEMPERATURE_ON))
		{
			PROTECTION_EnableTemperature();
		}
		else if ((temperature.rightChannel <= PROTECTION_TEMPERATURE_OFF) && (temperature.leftChannel <= PROTECTION_TEMPERATURE_OFF))
		{
			PROTECTION_DisableTemperature();
		}
	}
}

void PROTECTION_DirectCheck(void)
{
	// DC
	GPIO_PinState state;
	state = HAL_GPIO_ReadPin(DC_PROTECT_GPIO_Port, DC_PROTECT_Pin);
	if (state == GPIO_PIN_RESET)
	{
		state = HAL_GPIO_ReadPin(DC_PROTECT_GPIO_Port, DC_PROTECT_Pin);
		if (state == GPIO_PIN_RESET)
		{
			PROTECTION_EnableDc();
		}
	}

	// Voltage
	state = HAL_GPIO_ReadPin(V_PROTECT_GPIO_Port, V_PROTECT_Pin);
	if (state == GPIO_PIN_RESET)
	{
		state = HAL_GPIO_ReadPin(V_PROTECT_GPIO_Port, V_PROTECT_Pin);
		if (state == GPIO_PIN_RESET)
		{
			PROTECTION_EnableVoltage();
		}
	}

	// Temperature
	if ((temperature.rightChannel >= PROTECTION_TEMPERATURE_ON) || (temperature.leftChannel >= PROTECTION_TEMPERATURE_ON))
	{
		PROTECTION_EnableTemperature();
	}
}



