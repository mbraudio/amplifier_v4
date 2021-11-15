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

void INPUT_Write(const uint16_t index);

void INPUT_Initialize(void)
{
	// Set values
	input.inputs[0].led = LED_INPUT_SACD;
	input.inputs[0].value = 0;
	input.inputs[1].led = LED_INPUT_NETWORK;
	input.inputs[1].value = 1;
	input.inputs[2].led = LED_INPUT_PHONO;
	input.inputs[2].value = 2;
	input.inputs[3].led = LED_INPUT_TUNER;
	input.inputs[3].value = 3;
	input.inputs[4].led = LED_INPUT_AUX;
	input.inputs[4].value = 4;
	input.inputs[5].led = LED_INPUT_RECORDER;
	input.inputs[5].value = 5;
}

void INPUT_Activate(void)
{
	LED_Set(input.inputs[system.settings.input].led, GPIO_PIN_SET);
	INPUT_Write(input.inputs[system.settings.input].value);
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
	INPUT_Write(input.inputs[system.settings.input].value);
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

void INPUT_Write(const uint16_t value)
{
	// TODO: Code here...
}

void INPUT_Mute(const uint32_t status)
{
	const GPIO_PinState value = status ? GPIO_PIN_SET : GPIO_PIN_RESET;
	// TODO: Code here...
	SYSTEM_Mute((uint8_t)value);

}



