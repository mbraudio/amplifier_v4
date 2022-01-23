/*
 * potentiometer.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_POTENTIOMETER_H_
#define INC_POTENTIOMETER_H_

#include <stdint.h>
#include "system.h"

#define NUMBER_OF_POTS		4
#define POT_INDEX_VOLUME 	0
#define POT_INDEX_BASS 		1
#define POT_INDEX_TREBLE 	2
#define POT_INDEX_BALANCE 	3

#define POT_LAST_COUNT_MAX 	6

typedef struct
{
	uint8_t		value0;
	uint8_t		value1;
	uint8_t		value2;
	uint8_t		value3;
	uint8_t		value4;
	uint8_t		value5;
	uint8_t 	current;
	uint8_t 	currentReverse; // Only for logarithmic
	uint8_t 	required;
	uint8_t 	last;
	uint8_t		lastReverse;
	uint32_t	active;
	uint8_t    	command;
	uint32_t    logarithmic;
	uint8_t		lastCount;
	void(*plusFunction)(void);
	void(*minusFunction)(void);
	void(*stopFunction)(void);
} Potentiometer;

typedef struct
{
	uint32_t		activeIR;
	uint32_t		timer;
	uint32_t		update;
	uint32_t		updateIndex;
	Potentiometer	pots[NUMBER_OF_POTS];
} Potentiometers;

extern Potentiometers potentiometers;

void POTENTIOMETERS_Initialize(void);
void POTENTIOMETERS_Start(const uint8_t potIndex, const uint8_t index);
void POTENTIOMETERS_EnableUpdate(void);
void POTENTIOMETERS_DisableUpdate(void);

void POTENTIOMETERS_Stop(void);
void POTENTIOMETERS_StopIr(void);

// VOLUME
void POTENTIOMETERS_VolumePlus(void);
void POTENTIOMETERS_VolumePlusIr(void);
void POTENTIOMETERS_VolumeMinus(void);
void POTENTIOMETERS_VolumeMinusIr(void);

// BASS
void POTENTIOMETERS_BassPlus(void);
void POTENTIOMETERS_BassMinus(void);

// TREBLE
void POTENTIOMETERS_TreblePlus(void);
void POTENTIOMETERS_TrebleMinus(void);

// BALANCE
void POTENTIOMETERS_BalancePlus(void);
void POTENTIOMETERS_BalanceMinus(void);

// Common
void POTENTIOMETERS_SetCurrent(const uint8_t volume0, const uint8_t volume1, const uint8_t bass, const uint8_t treble, const uint8_t balance);
void POTENTIOMETERS_Process(void);
uint8_t POTENTIOMETERS_GetValueFromIndex(const uint8_t index);
uint8_t POTENTIOMETERS_GetIndexFromValue(Potentiometer* pot);


#endif /* INC_POTENTIOMETER_H_ */
