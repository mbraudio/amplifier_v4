/*
 * dac.c
 *
 *  Created on: Dec 10, 2021
 *      Author: Admin
 */
#include "dac.h"
#include "system.h"
#include "main.h"
#include "bluetooth.h"

DAC dac;

void DAC_Initialize(SPI_HandleTypeDef* h)
{
	PCM9211_Initialize(h);
	WM874X_Initialize(h);
	dac.error = 0;
	dac.npcm = 0;
	dac.input = 0;
	dac.exactSampleRate = PCM9211_FREQUENCY_48kHz;
	dac.bitRate = b24i2s;
}

void DAC_Setup(const uint8_t input)
{
	dac.input = input;
	PCM9211_Setup(input);
	WM874X_Setup();
}

void DAC_ResetWM874xSampleRate(void)
{
	WMSampleRate rate;
	rate = DAC_GetSampleRate();
	WM874X_SetSampleRate(rate);
}

void DAC_PCM9211_ErrorHandler(void)
{
	uint8_t error = PCM9211_Read(PCM9211_REG_2C_INT0_OUTPUT_REGISTER);
	if ((error & PCM9211_INT0_OERROR_MASK) && (error & PCM9211_INT0_OFSCHG_MASK))
	{
		DAC_ResetWM874xSampleRate();
		DAC_GetBitRate();
	}
}

void DAC_PCM9211_NpcmHandler(void)
{
	uint8_t npcm = PCM9211_Read(PCM9211_REG_2D_INT1_OUTPUT_REGISTER);
	uint32_t enabled = npcm & PCM9211_INT1_ONPCM_MASK;
	if (HAL_GPIO_ReadPin(DIR_NPCM_GPIO_Port, DIR_NPCM_Pin) == GPIO_PIN_SET)
	{
		if (enabled)
		{
			SYSTEM_NpcmMute(1);
			WM874X_Mute(1);
		}
	}
	else
	{
		SYSTEM_NpcmMute(0);
		WM874X_Mute(0);
	}
}

WMSampleRate DAC_GetSampleRate(void)
{
	uint8_t result = 0;

	// Check if FS calculation is done and read frequency of main port
	do {
		result = PCM9211_Read(PCM9211_REG_38_PORT_FS_CALCULATOR_RESULT_OUTPUT);
	} while (result & 0x80);

	dac.exactSampleRate = result & 0x0F;

	// Find frequency and set WM874X sample rate
	switch (dac.exactSampleRate)
	{
		case PCM9211_FREQUENCY_8kHz:
		case PCM9211_FREQUENCY_11kHz:
		case PCM9211_FREQUENCY_12kHz:
		case PCM9211_FREQUENCY_16kHz:
		case PCM9211_FREQUENCY_22kHz:
		case PCM9211_FREQUENCY_24kHz:
		case PCM9211_FREQUENCY_32kHz:
		case PCM9211_FREQUENCY_44kHz:
		case PCM9211_FREQUENCY_48kHz:
		case PCM9211_FREQUENCY_64kHz: {
			return SampleRateLow48kHz;
		} break;

		case PCM9211_FREQUENCY_88kHz:
		case PCM9211_FREQUENCY_96kHz:
		case PCM9211_FREQUENCY_128kHz: {
			return SampleRateMedium96Khz;
		} break;

		case PCM9211_FREQUENCY_176kHz:
		case PCM9211_FREQUENCY_192kHz: {
			return SampleRateHigh192kHz;
		} break;
	}

	return SampleRateLow48kHz;
}

PCM9211BitRate DAC_GetBitRate(void)
{
	uint8_t result = 0;

	result = PCM9211_Read(PCM9211_REG_2F_DIR_OUTPUT_DATA_FORMAT);
	switch (result) {
		case PCM9211_DIR_DATA_FORMAT_24BIT_RIGHT_JUSTIFIED: { dac.bitRate = b24rj; break; }
		case PCM9211_DIR_DATA_FORMAT_16BIT_RIGHT_JUSTIFIED: { dac.bitRate = b16rj; break; }
		case PCM9211_DIR_DATA_FORMAT_24BIT_I2S: 			{ dac.bitRate = b24i2s; break; }
		case PCM9211_DIR_DATA_FORMAT_24BIT_LEFT_JUSTIFIED: 	{ dac.bitRate = b24lj; break; }
		default: { dac.bitRate = b24i2s; break; }
	}

	return dac.bitRate;
}

void DAC_Process(void)
{
	if (dac.error)
	{
		dac.error = 0;
		DAC_PCM9211_ErrorHandler();
		// Send DAC data
		BLUETOOTH_Send3(COMMAND_UPDATE_DAC_DATA, dac.input, dac.exactSampleRate, dac.bitRate);
	}

	if (dac.npcm)
	{
		dac.npcm = 0;
		DAC_PCM9211_NpcmHandler();
	}
}

