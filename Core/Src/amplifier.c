/*
 * amp.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "amplifier.h"
#include "system.h"
#include "led.h"
#include "input.h"
#include "protection.h"
#include "encoder.h"
#include "utilities.h"
#include "potentiometer.h"
#include "mcp23008.h"
#include "bluetooth.h"


void AMP_SetPowerPin(const GPIO_PinState state)
{
	HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, state);
}

// Speakers A
void AMP_SetSpeakersA(const uint8_t state)
{
	system.settings.speakersA = state;
	LED_Set(LED_SPEAKERS_A, state);
	HAL_GPIO_WritePin(SPEAKERS_ENABLE_GPIO_Port, SPEAKERS_ENABLE_Pin, (GPIO_PinState)system.settings.speakersA);
}

// Speakers B
void AMP_SetSpeakersB(const uint8_t state)
{
	system.settings.speakersB = state;
	LED_Set(LED_SPEAKERS_B, state);
	//HAL_GPIO_WritePin(SPEAKERS_B_GPIO_Port, SPEAKERS_B_Pin, (GPIO_PinState)system.settings.speakersB);
}

// Direct
void AMP_SetDirect(const uint8_t state)
{
	system.settings.direct = state;
	LED_Set(LED_DIRECT, state);
	HAL_GPIO_WritePin(DIRECT_ENABLE_GPIO_Port, DIRECT_ENABLE_Pin, (GPIO_PinState)system.settings.direct);
}

// Loudness / Bass Boost
void AMP_SetLoudness(const uint8_t state)
{
	system.settings.loudness = state;
	LED_Set(LED_LOUDNESS, state);
	HAL_GPIO_WritePin(BASS_ENABLE_GPIO_Port, BASS_ENABLE_Pin, (GPIO_PinState)system.settings.loudness);
}

void AMP_GoToPowerOff(void)
{
	system.power.state = PoweringOff;
}

//TODO: Careful with delays here, more than 200ms in delays causes
// amp to re-power itself when using IR remote control... Check why!!!
void AMP_PowerOff(void)
{
	// Mute
	INPUT_Mute(1);
	// Delay
	HAL_Delay(50);
	// Standby LED pin Off - Led turns On
	LED_Standby(GPIO_PIN_RESET);
	// All leds off
	LED_AllOff();
	// Delay
	HAL_Delay(50);
	// All inputs and DAC off
	INPUT_AllOff();
	// Delay
	HAL_Delay(50);
	// Turn OFF
	// Speakers A
	AMP_SetSpeakersA(0);
	// Speakers B
	AMP_SetSpeakersB(0);
	// Direct
	AMP_SetDirect(0);
	// Loudness
	AMP_SetLoudness(0);
	// Power Off
	AMP_SetPowerPin(GPIO_PIN_RESET);
	// Disable pots update
	POTENTIOMETERS_DisableUpdate();
	// Send system
	BLUETOOTH_SendSystem();
}

void AMP_PowerToggle(void)
{
	switch (system.power.state)
	{
		case Off:
		{
			system.power.phase = InitialPhase;
			system.power.state = PoweringOn;
		} break;

		case PoweringOff:
		{

		} break;

		case PoweringOn:
		{
			system.power.phase = Idle;
			system.power.state = PoweringOff;
		} break;

		case On:
		{
			system.power.phase = Idle;
			system.power.state = PoweringOff;
		} break;
	}
}

// New PowerOn function with stages
void AMP_ProcessPower(void)
{
	if (system.power.state == PoweringOn)
	{
		switch (system.power.phase)
		{
			case Idle:
			{

			} break;

			case InitialPhase:
			{
				// Protection Check, EEPROM Load
				PROTECTION_LoadCheck();

				system.power.phase++;
			} break;

			case PowerOnPhase: {
				// Mute
				INPUT_Mute(1);
				// Delay
				HAL_Delay(100);

				// Power On
				AMP_SetPowerPin(GPIO_PIN_SET);

				// Standby LED pin On - Led turns Off
				LED_Standby(GPIO_PIN_SET);
				// Delay
				HAL_Delay(100);

				system.power.phase++;
			} break;

			case ProtectionPhase:
			{
				// Direct check 3 times
				PROTECTION_DirectCheck();
				HAL_Delay(100);
				PROTECTION_DirectCheck();
				HAL_Delay(100);
				PROTECTION_DirectCheck();

				system.power.phase++;
			} break;

			case LoadPhase:
			{
				// System Load
				uint32_t status = SYSTEM_Load();
				if (!status)
				{
					SYSTEM_Initialize();
					SYSTEM_Save();
				}

				system.power.phase++;
			} break;

			case LedPhase:
			{
				LED_SetBrightness(systemValues.brightness[system.settings.brightnessIndex]);
				LED_Set(LED_POWER, GPIO_PIN_SET);
				HAL_Delay(200);

				system.power.phase++;
			} break;

			case DelayPhase1:
			{
				HAL_Delay(400);

				system.power.phase++;
			} break;

			case InputPhase:
			{
				// Input Phase
				// Input
				INPUT_Activate();
				// TODO: Direct & Loudness ne idu skupa jedno s drugim. Direct gasi loudness! Sredi to !!!
				// Direct
				AMP_SetDirect(system.settings.direct);
				// Loudness
				AMP_SetLoudness(system.settings.loudness);
				// Setup MCP23008
				MCP23008_Setup();

				// Delay
				HAL_Delay(200);

				system.power.phase++;
			} break;

			case DelayPhase2:
			{
				HAL_Delay(400);

				system.power.phase++;
			} break;

			case SpeakersPhase:
			{
				// Speakers A
				AMP_SetSpeakersA(system.settings.speakersA);
				// Speakers B
				AMP_SetSpeakersB(system.settings.speakersB);

				system.power.phase++;
			} break;

			case FinalPhase:
			{
				// Set led volume
				//LED_SetVolume(system.settings.volumeRed, system.settings.volumeGreen, system.settings.volumeBlue);

				// Enable motors update
				POTENTIOMETERS_EnableUpdate();

				// Unmute
				INPUT_Mute(0);

				system.power.phase = Idle;
				system.power.state = On;

				// Send System
				BLUETOOTH_SendSystem();
			} break;
		}
	}
	else if (system.power.state == PoweringOff)
	{
		system.power.state = Off;
		AMP_PowerOff();
	}
}



