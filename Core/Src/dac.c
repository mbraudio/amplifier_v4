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
	}
}

static uint32_t cnt0 = 0;
static uint32_t cnt1 = 0;
void DAC_PCM9211_NpcmHandler(void)
{
	uint8_t npcm = PCM9211_Read(PCM9211_REG_2D_INT1_OUTPUT_REGISTER);
	if (npcm & PCM9211_INT1_ONPCM_MASK)
	{
		cnt1++;
	}
	else
	{
		cnt0++;
	}
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

