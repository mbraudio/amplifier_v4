/*
 * ir_nec.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "ir_nec.h"
#include "stm32l0xx.h"
#include "led.h"
#include "system.h"
#include "potentiometer.h"
#include "input.h"
#include "amplifier.h"

/* INFO:
	 Timer is 0.1ms.
	 Total length of one packet is around 65ms.
	 There are dual or quad packets!
	 Dual packet last around 130ms and quad packet last around 260ms.
	 Dual packets repeat single command: A A | A A
	 Quad packets repeat dual commands: A B | A B | A B | A B
*/

// Defines
#define IR_BITS						32
#define IR_BEGINING_TIME			120 // 120 * 0.1 ms => 12 ms
#define IR_ONE_TIME					19 // 19 * 0.1 ms => 1.9 ms for '1'

// Ignore times
#define IR_IGNORE_TIME_POWER		2000 // N * 0.1ms = time
#define IR_IGNORE_TIME_DIMMER		340
#define IR_IGNORE_TIME_VOLUME		400
#define IR_IGNORE_TIME_SPEAKERS		340
#define IR_IGNORE_TIME_INPUT		340

IR ir;


void IR_Initialize(void)
{
	ir.command = 0;
	ir.lastCommand = 0;
	ir.commandToDecode = 0;
	ir.counter = 0;
	ir.ignoreTime = IR_IGNORE_TIME_POWER;
	ir.ready = 0;
	ir.index = 0;
}

void IR_Encode(void)
{
	if (ir.counter >= IR_BEGINING_TIME)
	{
		ir.index = 0;
		ir.command = 0;
	}
	else
	{
		if (ir.counter >= IR_ONE_TIME)
		{
			ir.command |= (1 << ir.index);
		}

		ir.index++;

		if (ir.index >= IR_BITS)
		{
			ir.commandToDecode = ir.command;
			ir.ready = 1;
		}
	}

	ir.counter = 0;
}

void IR_Decode(void)
{
	if ((system.power.state != On) && (ir.commandToDecode != IR_CODE_ON_STANDBY))
	{
		ir.commandToDecode = 0;
		return;
	}

	switch (ir.commandToDecode)
	{
		case IR_CODE_ON_STANDBY:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_POWER;
				AMP_PowerToggle();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_DIMMER_2:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_DIMMER;
				LED_SetBrightness(SYSTEM_IncreaseBrightness());
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_VOLUME_PLUS:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_VolumePlusIr();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_VOLUME_MINUS:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_VolumeMinusIr();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		// Bass
		case IR_CODE_NAP_ON_OFF: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_BassPlus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_NAP_REWIND: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_BassMinus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		// Treble
		case IR_CODE_NAP_PLAY: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_TreblePlus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_NAP_STOP: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_TrebleMinus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		// Balance
		case IR_CODE_NAP_FUNC: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_BalancePlus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_NAP_FORWARD: {
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_VOLUME;
				POTENTIOMETERS_BalanceMinus();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		// Speakers
		case IR_CODE_SPEAKERS_A:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_SPEAKERS;
				AMP_SetSpeakersA(!system.settings.speakersA);
				SYSTEM_Save();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_SPEAKERS_B:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_SPEAKERS;
				AMP_SetSpeakersB(!system.settings.speakersB);
				SYSTEM_Save();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_INPUT_MINUS:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_INPUT;
				INPUT_Changed(-1);
				INPUT_Confirmed();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_INPUT_PLUS:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_INPUT;
				INPUT_Changed(1);
				INPUT_Confirmed();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_MUTE:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_INPUT;
				uint8_t mute = !system.states.mute;
				INPUT_Mute(mute);
				ir.lastCommand = ir.commandToDecode;
				//BLUETOOTH_Send(COMMAND_TOGGLE_MUTE, system.states.mute); // TODO: Need to update data on BLE channel for all of this options...
			}
		} break;

		case IR_CODE_LOUDNESS:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_INPUT;
				AMP_SetLoudness(!system.settings.loudness);
				SYSTEM_Save();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		case IR_CODE_DIRECT:
		{
			if (ir.lastCommand != ir.commandToDecode)
			{
				ir.ignoreTime = IR_IGNORE_TIME_INPUT;
				AMP_SetDirect(!system.settings.direct);
				SYSTEM_Save();
				ir.lastCommand = ir.commandToDecode;
			}
		} break;

		default:
		{
			//ir.counter = 0;
			//ir.lastCommand = 0;
		} break;
	}

	ir.commandToDecode = 0;
}

void IR_Disengage(void)
{
	if (potentiometers.activeIR)
	{
		POTENTIOMETERS_StopIr();
	}
}

void IR_Process(void)
{
	// Process
	if (ir.ready)
	{
		IR_Decode();
		ir.ready = 0;
	}
}

void IR_ProcessTimer(void)
{
	ir.counter++;
	if (ir.lastCommand && (ir.counter > ir.ignoreTime))
	{
		IR_Disengage();
		ir.lastCommand = 0;
	}
}




