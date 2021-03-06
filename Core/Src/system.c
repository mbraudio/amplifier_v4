/*
 * system.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "system.h"
#include "eeprom.h"
#include "dac.h"
#include "utilities.h"
#include "bluetooth.h"

#define SETTINGS_SIZE				sizeof(Settings)
#define SETTINGS_EEPROM_ADDRESS		0

System system;
SystemValues systemValues;

void SYSTEM_Initialize(void)
{
	// Settings
	system.settings.apd = 0;
	system.settings.direct = 0;
	system.settings.loudness = 0;
	system.settings.speakersA = 0;
	system.settings.speakersB = 0;
	system.settings.input = 0;
	system.settings.brightnessIndex = 1;
	system.settings.volumeKnobLed = 1;
	system.settings.dacFilter = 0;
	system.settings.bluetoothEnabled = 0;
	// States
	system.states.mute = 0;
	system.states.npcmMute = 0;
	system.states.protectionTriggeredDc = 0;
	system.states.protectionTriggeredVoltage = 0;
	system.states.protectionTriggeredTemperature = 0;
}

void SYSTEM_InitializePower(void)
{
	// Power
	system.power.state = Off;
	system.power.phase = Idle;
}

void SYSTEM_InitializeValues(void)
{
	systemValues.brightness[0] = 8;
	systemValues.brightness[1] = 16;
	systemValues.brightness[2] = 45;
	systemValues.brightness[3] = 90;
	systemValues.brightness[4] = 140;
}

void SYSTEM_Save(void)
{
	system.settings.crc = UTILITIES_CalculateCrc((uint8_t*)&system.settings, (SETTINGS_SIZE - 1));
	EEPROM_Write(SETTINGS_EEPROM_ADDRESS, SETTINGS_SIZE, (uint8_t*)&system.settings);
}

uint32_t SYSTEM_Load(void)
{
	uint8_t crc;
	EEPROM_Read(SETTINGS_EEPROM_ADDRESS, SETTINGS_SIZE, (uint8_t*)&system.settings);
	crc = UTILITIES_CalculateCrc((uint8_t*)&system.settings, (SETTINGS_SIZE - 1));
	return (system.settings.crc == crc);
}

// Specific functions
uint8_t SYSTEM_IncreaseBrightness(void)
{
	system.settings.brightnessIndex++;
	if (system.settings.brightnessIndex >= BRIGHTNESS_VALUES_COUNT)
	{
		system.settings.brightnessIndex = 0;
	}
	SYSTEM_Save();
	return systemValues.brightness[system.settings.brightnessIndex];
}

inline uint8_t SYSTEM_GetBrightness(void)
{
	return systemValues.brightness[system.settings.brightnessIndex];
}

inline void SYSTEM_SetVolumeKnobLed(const uint8_t state)
{
	system.settings.volumeKnobLed = state;
}

inline void SYSTEM_Mute(const uint8_t value)
{
	system.states.mute = value;
}

inline void SYSTEM_NpcmMute(const uint8_t value)
{
	system.states.npcmMute = value;
}

uint16_t SYSTEM_Serialize(uint8_t* buffer)
{
	uint16_t count = 0;
	uint8_t crc = COMMAND_SYSTEM_DATA;
	buffer[count++] = COMMAND_SYSTEM_DATA;		// 0
	buffer[count++] = system.settings.apd; // 1
	crc += system.settings.apd;
	buffer[count++] = system.settings.direct; // 2
	crc += system.settings.direct;
	buffer[count++] = system.settings.loudness;  // 3
	crc += system.settings.loudness;
	buffer[count++] = system.settings.speakersA; // 4
	crc += system.settings.speakersA;
	buffer[count++] = system.settings.speakersB; // 5
	crc += system.settings.speakersB;
	buffer[count++] = system.settings.input; // 6
	crc += system.settings.input;
	buffer[count++] = system.settings.brightnessIndex; // 7
	crc += system.settings.brightnessIndex;
	buffer[count++] = system.settings.volumeKnobLed; // 8
	crc += system.settings.volumeKnobLed;
	buffer[count++] = system.power.state; // 9
	crc += system.power.state;
	buffer[count++] = system.states.mute; // 10
	crc += system.states.mute;
	buffer[count++] = dac.input; // 11
	crc += dac.input;
	buffer[count++] = dac.exactSampleRate; // 12
	crc += dac.exactSampleRate;
	buffer[count++] = dac.bitRate; // 13
	crc += dac.bitRate;
	buffer[count++] = system.settings.dacFilter; // 14
	crc += system.settings.dacFilter;
	buffer[count++] = crc;
	return count;
}


