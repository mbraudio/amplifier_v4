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
	// Set values
	input.inputs[0].led = LED_INPUT_SACD;
	input.inputs[0].value = 0;
	input.inputs[0].digital = 1;

	input.inputs[1].led = LED_INPUT_NETWORK;
	input.inputs[1].value = 1;
	input.inputs[1].digital = 0;

	input.inputs[2].led = LED_INPUT_TUNER;
	input.inputs[2].value = 2;
	input.inputs[2].digital = 1;

	input.inputs[3].led = LED_INPUT_AUX;
	input.inputs[3].value = 3;
	input.inputs[3].digital = 0;

	input.inputs[4].led = LED_INPUT_RECORDER;
	input.inputs[4].value = 4;
	input.inputs[4].digital = 0;
}

void INPUT_EnableDAC(void)
{
	/* For digital input
	 * - turn on PHONO LED (WHITE LED)
	 * - enable DAC power
	*/
	if (input.inputs[system.settings.input].digital) {
		LED_Set(LED_INPUT_PHONO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_SET);
	} else {
		LED_Set(LED_INPUT_PHONO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_RESET);
	}
}

void INPUT_Activate(void)
{
	LED_Set(input.inputs[system.settings.input].led, GPIO_PIN_SET);
	INPUT_EnableDAC();
	INPUT_Set(&input.inputs[system.settings.input]);
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
	INPUT_EnableDAC();
	INPUT_Set(&input.inputs[system.settings.input]);
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
	HAL_GPIO_WritePin(INPUT_0_GPIO_Port, INPUT_0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INPUT_1_GPIO_Port, INPUT_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INPUT_2_GPIO_Port, INPUT_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(INPUT_DAC_GPIO_Port, INPUT_DAC_Pin, GPIO_PIN_RESET);
}

void INPUT_AllOff(void)
{
	INPUT_InputsOff();
	HAL_GPIO_WritePin(DAC_ENABLE_GPIO_Port, DAC_ENABLE_Pin, GPIO_PIN_RESET);
}

void INPUT_Set(const InputData* data)
{
	INPUT_InputsOff();

	//HAL_Delay(100);

	switch (data->value) {
		case 0: {
			// SACD - DIGITAL - OPTICAL 0
			HAL_GPIO_WritePin(INPUT_DAC_GPIO_Port, INPUT_DAC_Pin, GPIO_PIN_SET);
		} break;

		case 1: {
			// NETWORK - ANALOG
			HAL_GPIO_WritePin(INPUT_1_GPIO_Port, INPUT_1_Pin, GPIO_PIN_SET);
		} break;

		case 2: {
			// TUNER - DIGITAL - OPTICAL 1
			HAL_GPIO_WritePin(INPUT_DAC_GPIO_Port, INPUT_DAC_Pin, GPIO_PIN_SET);
		} break;

		case 3: {
			// AUX - ANALOG
			HAL_GPIO_WritePin(INPUT_0_GPIO_Port, INPUT_0_Pin, GPIO_PIN_SET);
		} break;

		case 4: {
			// RECORDER - ANALOG
			HAL_GPIO_WritePin(INPUT_2_GPIO_Port, INPUT_2_Pin, GPIO_PIN_SET);
		} break;
	}
}

// MUTE is by default ENABLED. To disable it, set MUTE_DISABLE_Pin to SET!
void INPUT_Mute(const uint32_t status)
{
	const GPIO_PinState value = status ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(MUTE_DISABLE_GPIO_Port, MUTE_DISABLE_Pin, value);
	SYSTEM_Mute((uint8_t)status);
}



