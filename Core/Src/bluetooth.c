/*
 * bluetooth.c
 *
 *  Created on: Jan 9, 2022
 *      Author: Admin
 */
#include "bluetooth.h"
#include "led.h"
#include "uart.h"
#include "amplifier.h"
#include "input.h"
#include "system.h"
#include "potentiometer.h"
#include "calibrator.h"

void BLUETOOTH_Process(const uint8_t* data, const uint32_t size)
{
	if (size == 0)
	{
		return;
	}

	if (!BLUETOOTH_CalculateCrc(data, size))
	{
		return;
	}

	uint32_t command = data[0];

	switch (command)
	{
		case COMMAND_SYSTEM_DATA:
		{
			BLUETOOTH_SendSystem();
			POTENTIOMETERS_EnableUpdate();
		} break;

		case COMMAND_TOGGLE_POWER:
		{
			AMP_PowerToggle();
		} break;

		case COMMAND_TOGGLE_MUTE:
		{
			INPUT_Mute(data[1]);
			BLUETOOTH_Send(command, system.states.mute);
		} break;

		case COMMAND_CHANGE_INPUT:
		{
			INPUT_SetDirect(data[1]);
		} break;

		case COMMAND_TOGGLE_DIRECT:
		{
			AMP_SetDirect(!system.settings.direct);
			BLUETOOTH_Send(command, system.settings.direct);
			SYSTEM_Save();
		} break;

		case COMMAND_TOGGLE_SPEAKER_A:
		{
			AMP_SetSpeakersA(!system.settings.speakersA);
			BLUETOOTH_Send(command, system.settings.speakersA);
			SYSTEM_Save();
		} break;

		case COMMAND_TOGGLE_SPEAKER_B:
		{
			AMP_SetSpeakersB(!system.settings.speakersB);
			BLUETOOTH_Send(command, system.settings.speakersB);
			SYSTEM_Save();
		} break;

		case COMMAND_TOGGLE_LOUDNESS:
		{
			AMP_SetLoudness(!system.settings.loudness);
			BLUETOOTH_Send(command, system.settings.loudness);
			SYSTEM_Save();
		} break;

		case COMMAND_TOGGLE_PAMP_DIRECT:
		{
			//AMP_SetLoudness(!system.settings.loudness);
			//SYSTEM_Save();
			//BLUETOOTH_Send(command, system.settings.loudness);
		} break;

		case COMMAND_UPDATE_VOLUME_VALUE:
		{
			POTENTIOMETERS_Start(POT_INDEX_VOLUME, data[1]);
		} break;

		case COMMAND_UPDATE_BASS_VALUE:
		{
			POTENTIOMETERS_Start(POT_INDEX_BASS, data[1]);
		} break;

		case COMMAND_UPDATE_TREBLE_VALUE:
		{
			POTENTIOMETERS_Start(POT_INDEX_TREBLE, data[1]);
		} break;

		case COMMAND_UPDATE_BALANCE_VALUE:
		{
			POTENTIOMETERS_Start(POT_INDEX_BALANCE, data[1]);
		} break;

		case COMMAND_UPDATE_TEMPERATURE:
		{

		} break;

		case COMMAND_BRIGHTNESS_INDEX:
		{
			system.settings.brightnessIndex = data[1];
			uint8_t brightness = systemValues.brightness[system.settings.brightnessIndex];
			LED_SetBrightness(brightness);
			LED_SetVolumeKnobLed(brightness, system.settings.volumeKnobLed);
		} break;

		case COMMAND_SET_VOLUME_KNOB_LED:
		{
			AMP_SetVolumeKnobLed(data[1]);
			BLUETOOTH_Send(command, system.settings.volumeKnobLed);
		} break;

		case COMMAND_CALIBRATE:
		{
			CALIBRATOR_Start(data[1], data[2]);
		} break;
	}
}

uint32_t BLUETOOTH_CalculateCrc(const uint8_t* data, const uint32_t size)
{
	uint32_t length = size - 1;
	uint8_t crc = 0;
	uint32_t i;
	for (i = 0; i < length; i++)
	{
		crc += data[i];
	}
	return (crc == data[length]);
}

void BLUETOOTH_Send(const uint8_t command, const uint8_t value)
{
	uint8_t crc = command + value;
	uint8_t data[3] = { command, value, crc };
	UART_Send(data, 3);
}

void BLUETOOTH_Send2(const uint8_t command, const uint8_t value0, const uint8_t value1)
{
	uint8_t crc = command + value0 + value1;
	uint8_t data[4] = { command, value0, value1, crc };
	UART_Send(data, 4);
}

void BLUETOOTH_SendData(const uint8_t* data, const uint8_t length)
{
	UART_Send(data, length);
}

void BLUETOOTH_SendSystem(void)
{
	uint8_t data[SYSTEM_VALUE_COUNT];
	uint16_t length = SYSTEM_Serialize(data);
	BLUETOOTH_SendData(data, length);
}



