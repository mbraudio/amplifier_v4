/*
 * system.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <stdint.h>

#define SYSTEM_VALUE_COUNT		14 // 1 command + 10 Settings + 2 States + 1 crc
#define BRIGHTNESS_VALUES_COUNT 5

// Power - external interrupt pin
#define POWER_PIN	GPIO_PIN_14

typedef struct
{
	uint8_t		apd:1;
	uint8_t		direct:1;
	uint8_t		loudness:1;
	uint8_t		speakersA:1;
	uint8_t		speakersB:1;
	uint8_t 	input:3;
	uint8_t 	brightnessIndex;
	uint8_t 	volumeLed;
	uint8_t		crc;
} __attribute__((packed)) Settings;

typedef struct
{
	uint8_t		mute;
	uint8_t		npcmMute;
	uint8_t		protectionTriggeredDc;
	uint8_t		protectionTriggeredVoltage;
} __attribute__((packed)) States;

typedef enum
{
	Off = 0,
	PoweringOff,
	PoweringOn,
	On
} PowerState;

typedef enum
{
	Idle = 0,
	InitialPhase,
	PowerOnPhase,
	ProtectionPhase,
	LoadPhase,
	LedPhase,
	DelayPhase1,
	InputPhase,
	DelayPhase2,
	SpeakersPhase,
	FinalPhase
} PowerPhase;

typedef struct
{
	PowerState state;
	PowerPhase phase;
} Power;

typedef struct
{
	Settings	settings;
	States		states;
	Power		power;
} __attribute__((packed)) System;

typedef struct
{
	uint8_t brightness[BRIGHTNESS_VALUES_COUNT];
} SystemValues;

extern System system;
extern SystemValues systemValues;

void SYSTEM_Initialize(void);
void SYSTEM_InitializeValues(void);
void SYSTEM_Save(void);
uint32_t SYSTEM_Load(void);

uint8_t SYSTEM_IncreaseBrightness(void);
uint8_t SYSTEM_GetBrightness(void);
void SYSTEM_EnableVolumeLed(const uint8_t state);
void SYSTEM_Mute(const uint8_t value);
void SYSTEM_NpcmMute(const uint8_t value);
uint16_t SYSTEM_Serialize(uint8_t* buffer);


#endif /* INC_SYSTEM_H_ */
