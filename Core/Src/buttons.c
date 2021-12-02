/*
 * buttons.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include <amplifier.h>
#include "buttons.h"
#include "led.h"
//#include "bluetooth.h"
#include "system.h"
#include "input.h"
#include "stm32l0xx.h"


// Buttons - Group 1
// Direct: 0
#define BUTTON_DIRECT_ADC_HI			10
// SpeakersA: 42
#define BUTTON_SPEAKERS_A_ADC_LOW	 	32
#define BUTTON_SPEAKERS_A_ADC_HI		52
// SpeakersB: 101
#define BUTTON_SPEAKERS_B_ADC_LOW	 	91
#define BUTTON_SPEAKERS_B_ADC_HI	    121
// Loudness: 151
#define BUTTON_LOUDNESS_ADC_LOW			141
#define BUTTON_LOUDNESS_ADC_HI			161
// Buttons - Group 2
// Selector: 0
#define BUTTON_SELECTOR_ADC_HI			10
// Max value is 256
#define BUTTON_ADC_MAX					220

volatile ButtonManager buttons;

void BUTTONS_Initialize(void)
{
	// Power
	buttons.power.state = Released;
	buttons.power.time = 0;

	// Group 1
	buttons.direct.state = Released;
	buttons.direct.time = 0;

	buttons.speakersA.state = Released;
	buttons.speakersA.time = 0;

	buttons.speakersB.state = Released;
	buttons.speakersB.time = 0;

	buttons.loudness.state = Released;
	buttons.loudness.time = 0;

	// Group 2
	buttons.selector.state = Released;
	buttons.selector.time = 0;

	// Time
	buttons.timer = 0;
}

uint32_t BUTTONS_PressValid(const uint32_t time, const uint32_t value)
{
	int32_t diff = buttons.timer - time;
	if (diff < 0) { diff *= -1; }
	return (diff >= value);
}

void BUTTONS_ProcessADC_MainGroup(const uint8_t value)
{
	if (system.power.state != On)
	{
		return;
	}

	// DIRECT
	if (value < BUTTON_DIRECT_ADC_HI)
	{
		switch (buttons.direct.state)
		{
			case Released:
			{
				buttons.direct.state = Triggered;
				buttons.direct.time = buttons.timer;
			} break;

			case Triggered:
			{
				if (BUTTONS_PressValid(buttons.direct.time, BUTTON_PRESSED_TIME))
				{
					buttons.direct.state = Pressed;
				}
			} break;

			case Pressed:
			{
				if (BUTTONS_PressValid(buttons.direct.time, BUTTON_HOLD_TIME))
				{
					buttons.direct.state = Hold;
				}
			} break;

			case Hold:
			{

			} break;
		}
	}
	else
	{
		if (buttons.direct.state == Pressed)
		{
			AMP_SetDirect(!system.settings.direct);
			//BLUETOOTH_Send(COMMAND_TOGGLE_DIRECT, system.settings.direct); // TODO: Implement...
			SYSTEM_Save();
		}
		else if (buttons.direct.state == Hold)
		{

		}
		buttons.direct.state = Released;
	}

	// SPEAKERS A
	if ((value > BUTTON_SPEAKERS_A_ADC_LOW) && (value < BUTTON_SPEAKERS_A_ADC_HI))
	{
		switch (buttons.speakersA.state)
		{
			case Released:
			{
				buttons.speakersA.state = Triggered;
				buttons.speakersA.time = buttons.timer;
			} break;

			case Triggered:
			{
				if (BUTTONS_PressValid(buttons.speakersA.time, BUTTON_PRESSED_TIME))
				{
					buttons.speakersA.state = Pressed;
				}
			} break;

			case Pressed:
			{
				if (BUTTONS_PressValid(buttons.speakersA.time, BUTTON_HOLD_TIME))
				{
					buttons.speakersA.state = Hold;
				}
			} break;

			case Hold:
			{

			} break;
		}
	}
	else
	{
		if (buttons.speakersA.state == Pressed)
		{
			AMP_SetSpeakersA(!system.settings.speakersA);
			//BLUETOOTH_Send(COMMAND_TOGGLE_SPEAKER_A, system.settings.speakersA); // TODO: Implement...
			SYSTEM_Save();
		}
		else if (buttons.speakersA.state == Hold)
		{

		}
		buttons.speakersA.state = Released;
	}

	// SPEAKERS B
	if ((value > BUTTON_SPEAKERS_B_ADC_LOW) && (value < BUTTON_SPEAKERS_B_ADC_HI))
	{
		switch (buttons.speakersB.state)
		{
			case Released:
			{
				buttons.speakersB.state = Triggered;
				buttons.speakersB.time = buttons.timer;
			} break;

			case Triggered:
			{
				if (BUTTONS_PressValid(buttons.speakersB.time, BUTTON_PRESSED_TIME))
				{
					buttons.speakersB.state = Pressed;
				}
			} break;

			case Pressed:
			{
				if (BUTTONS_PressValid(buttons.speakersB.time, BUTTON_HOLD_TIME))
				{
					buttons.speakersB.state = Hold;
				}
			} break;

			case Hold:
			{

			} break;
		}
	}
	else
	{
		if (buttons.speakersB.state == Pressed)
		{
			AMP_SetSpeakersB(!system.settings.speakersB);
			//BLUETOOTH_Send(COMMAND_TOGGLE_SPEAKER_B, system.settings.speakersB); // TODO: Implement...
			SYSTEM_Save();
		}
		else if (buttons.speakersB.state == Hold)
		{

		}
		buttons.speakersB.state = Released;
	}

	// LOUDNESS
	if ((value > BUTTON_LOUDNESS_ADC_LOW) && (value < BUTTON_LOUDNESS_ADC_HI))
	{
		switch (buttons.loudness.state)
		{
			case Released:
			{
				buttons.loudness.state = Triggered;
				buttons.loudness.time = buttons.timer;
			} break;

			case Triggered:
			{
				if (BUTTONS_PressValid(buttons.loudness.time, BUTTON_PRESSED_TIME))
				{
					buttons.loudness.state = Pressed;
				}
			} break;

			case Pressed:
			{
				if (BUTTONS_PressValid(buttons.loudness.time, BUTTON_HOLD_TIME))
				{
					buttons.loudness.state = Hold;
				}
			} break;

			case Hold:
			{

			} break;
		}
	}
	else
	{
		if (buttons.loudness.state == Pressed)
		{
			AMP_SetLoudness(!system.settings.loudness);
			//BLUETOOTH_Send(COMMAND_TOGGLE_LOUDNESS, system.settings.loudness); // TODO: Implement...
			SYSTEM_Save();
		}
		else if (buttons.loudness.state == Hold)
		{

		}
		buttons.loudness.state = Released;
	}
}

void BUTTONS_ProcessADC_SelectorGroup(const uint8_t value)
{
	if (system.power.state != On)
	{
		return;
	}

	// SELECTOR
	if (value < BUTTON_SELECTOR_ADC_HI)
	{
		switch (buttons.selector.state)
		{
			case Released:
			{
				buttons.selector.state = Triggered;
				buttons.selector.time = buttons.timer;
			} break;

			case Triggered:
			{
				if (BUTTONS_PressValid(buttons.selector.time, BUTTON_PRESSED_TIME))
				{
					buttons.selector.state = Pressed;
				}
			} break;

			case Pressed:
			{
				if (BUTTONS_PressValid(buttons.selector.time, BUTTON_HOLD_TIME))
				{
					buttons.selector.state = Hold;
				}
			} break;

			case Hold:
			{

			} break;
		}
	}
	else
	{
		if (buttons.selector.state == Pressed)
		{
			//LED_Set(LED_INPUT_SELECTOR, system.states.mute);
			INPUT_Mute(!system.states.mute);
			//BLUETOOTH_Send(COMMAND_TOGGLE_MUTE, system.states.mute); // TODO: Implement...
		}
		else if (buttons.selector.state == Hold)
		{

		}
		buttons.selector.state = Released;
	}
}



