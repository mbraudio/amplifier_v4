/*
 * dac.c
 *
 *  Created on: Dec 10, 2021
 *      Author: Admin
 */
#include "dac.h"
#include "main.h"

DAC dac;

void DAC_Initialize(SPI_HandleTypeDef* h)
{
	PCM9211_Initialize(h);
	WM874X_Initialize(h);
	dac.error = 0;
	dac.npcm = 0;
}

void DAC_Setup(void)
{
	PCM9211_Setup();
	WM874X_Setup();
	DAC_PCM9211_ErrorHandler();
}

void DAC_ResetWmSampleRate(void)
{
	WMSampleRate rate;
	rate = DAC_GetSampleRate();
	WM874X_SetSampleRate(rate);
}

void DAC_PCM9211_ErrorHandler(void)
{
	HAL_Delay(20);
	uint8_t data = PCM9211_Read(PCM9211_REG_2C_INT0_OUTPUT_REGISTER);
	uint8_t data0 = PCM9211_Read(PCM9211_REG_2C_INT0_OUTPUT_REGISTER);
	uint8_t data1 = PCM9211_Read(PCM9211_REG_2D_INT1_OUTPUT_REGISTER);
	if (data & PCM9211_INT0_OERROR0_MASK)
	{
		if ((data & PCM9211_INT0_ONPCM_MASK) || (data & PCM9211_INT0_ODTSCD_MASK))
		{
			// MUTE
			PCM9211_Mute(1);
		}
		else
		{
			// UNMUTE
			PCM9211_Mute(0);
		}

		if (data & PCM9211_INT0_OFSCHG_MASK)
		{
			DAC_ResetWmSampleRate();
		}
	}

	// 0x2C registar se full čudno ponaša !!!

	// Na DTS(AC3 sa filmom) input se dobije 0x8E
	// Treba napraviti ako se dobije DTS i NPCM da se WM ili PCM MUTA !!!
	// PCM registar 0x6A - ima mute pa provjeri kako radi !!! PCM9211 dokument strana 97 !!!
	// Staviti mute ako je DTS ili NPCM detektiran...
}

void DAC_PCM9211_NpcmHandler(void)
{
	// THIS just never gets triggered...
}

WMSampleRate DAC_GetSampleRate(void)
{
	uint8_t result = 0;

	// Check if FS calculation is done and read frequency of main port
	do {
		result = PCM9211_Read(PCM9211_REG_38_PORT_FS_CALCULATOR_RESULT_OUTPUT);
	} while (result & 0x80);

	// Find frequency and set WM874X sample rate
	switch (result & 0x0F)
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

void DAC_Process(void)
{
	if (dac.error)
	{
		dac.error = 0;
		DAC_PCM9211_ErrorHandler();
	}

	if (dac.npcm)
	{
		dac.npcm = 0;
		DAC_PCM9211_NpcmHandler();
	}
}

