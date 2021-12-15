/*
 * input.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "input.h"
#include "led.h"
//#include "bluetooth.h"
#include "system.h"
#include "main.h"
#include "dac.h"

#define INPUT_LED_MUTE_TOGGLE_TIME		50

Input input;

void INPUT_Set(const InputData* data);

// SCHEMATIC
/*
 * > ANALOG
 * INPUT 0 - AUX
 * INPUT 1 - NETWORK
 * INPUT 2 - RECORDER
 * DIGITAL - OPTICAL
 * > DIGITAL
 * SACD - OPTICAL 1
 * TUNER - OPTICAL 2
 * PHONO LED - Used for DAC notification.
 */


void INPUT_Initialize(void)
{
	InputData* id;

	// Set values
	id = &input.inputs[0];
	id->led = LED_INPUT_SACD;
	id->port = INPUT_DAC_GPIO_Port;
	id->pin = INPUT_DAC_Pin;
	id->digital = PCM9211_INPUT_RXIN_2;

	id = &input.inputs[1];
	id->led = LED_INPUT_NETWORK;
	id->port = INPUT_1_GPIO_Port;
	id->pin = INPUT_1_Pin;
	id->digital = 0;

	id = &input.inputs[2];
	id->led = LED_INPUT_TUNER;
	id->port = INPUT_DAC_GPIO_Port;
	id->pin = INPUT_DAC_Pin;
	id->digital = PCM9211_INPUT_RXIN_4;

	id = &input.inputs[3];
	id->led = LED_INPUT_AUX;
	id->port = INPUT_0_GPIO_Port;
	id->pin = INPUT_0_Pin;
	id->digital = 0;

	id = &input.inputs[4];
	id->led = LED_INPUT_RECORDER;
	id->port = INPUT_2_GPIO_Port;
	id->pin = INPUT_2_Pin;
	id->digital = 0;

	input.npcmTimer = 0;
	input.muteTimer = 0;
}

void INPUT_Process(void)
{
	if (system.states.mute)
	{
		if (input.muteTimer >= INPUT_LED_MUTE_TOGGLE_TIME) // INPUT_LED_MUTE_TOGGLE_TIME * 10ms = XXXms
		{
			input.muteTimer = 0;
			LED_Toggle(input.inputs[system.settings.input].led);
		}
	}

	if (system.states.npcmMute)
	{
		if (input.npcmTimer >= INPUT_LED_MUTE_TOGGLE_TIME) // INPUT_LED_MUTE_TOGGLE_TIME * 10ms = XXXms
		{
			input.npcmTimer = 0;
			LED_Toggle(LED_INPUT_PHONO);
		}
	}
}

void INPUT_Activate(void)
{
	InputData* data = &input.inputs[system.settings.input];
	LED_Set(data->led, GPIO_PIN_SET);
	INPUT_Set(data);
}

void INPUT_Changed(const int32_t direction)
{
	int8_t index;
	// Current index
	index = system.settings.input;
	// Turn off current led
	LED_Set(input.inputs[index].led, GPIO_PIN_RESET);
	// Add/subtract value
	index += direction;
	if (index < 0) { index = (INPUT_MAX_SELECTION - 1); }
	if (index >= INPUT_MAX_SELECTION) { index = 0; }
	// Turn on new led
	LED_Set(input.inputs[index].led, GPIO_PIN_SET);
	// Save current index
	system.settings.input = index;
}

void INPUT_Confirmed(void)
{
	InputData* data = &input.inputs[system.settings.input];
	INPUT_Set(data);
	//BLUETOOTH_Send(COMMAND_CHANGE_INPUT, system.settings.input);
	SYSTEM_Save();
}

void INPUT_SetDirect(const uint8_t newIndex)
{
	// Turn off current led
	LED_Set(input.inputs[system.settings.input].led, GPIO_PIN_RESET);
	// Turn on new led
	LED_Set(input.inputs[newIndex].led, GPIO_PIN_SET);
	// Save current index
	system.settings.input = newIndex;
	INPUT_Confirmed();
}

void INPUT_InputsOff(void)
{
	uint32_t i;
	for (i = 0; i < INPUT_MAX_SELECTION; i++)
	{
		InputData* id = &input.inputs[i];
		HAL_GPIO_WritePin(id->port, id->pin, GPIO_PIN_RESET);
	}
}

void INPUT_AllOff(void)
{
	INPUT_InputsOff();
	HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_RESET);
}

void INPUT_Set(const InputData* data)
{
	INPUT_InputsOff();

	/* For digital input
	 * - turn on PHONO LED (WHITE LED)
	 * - enable DAC power
	*/
	if (data->digital)
	{
		LED_Set(LED_INPUT_PHONO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_SET);
		DAC_Setup(data->digital);
		DAC_ResetWM874xSampleRate();
		DAC_PCM9211_NpcmHandler();
	}
	else
	{
		SYSTEM_NpcmMute(0);
		LED_Set(LED_INPUT_PHONO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(data->port, data->pin, GPIO_PIN_SET);
}

// MUTE is by default ENABLED. To disable it, set MUTE_DISABLE_Pin to SET!
void INPUT_Mute(const uint32_t status)
{
	const GPIO_PinState value = status ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(MUTE_DISABLE_GPIO_Port, MUTE_DISABLE_Pin, value);
	if (!status)
	{
		LED_Set(input.inputs[system.settings.input].led, GPIO_PIN_SET);
	}
	SYSTEM_Mute((uint8_t)status);
}


