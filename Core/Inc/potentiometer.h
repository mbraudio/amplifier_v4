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

#define POT_INDEX_VOLUME_0 	0
#define POT_INDEX_VOLUME_1 	1
#define POT_INDEX_BASS 		2
#define POT_INDEX_TREBLE 	3
#define POT_INDEX_BALANCE 	4

typedef struct
{
	uint8_t 	current;
	uint8_t 	required;
	uint8_t 	last;
	uint32_t	active;
	uint8_t    	command;
	uint32_t    logarithmic;
	void(*plusFunction)(void);
	void(*minusFunction)(void);
	void(*stopFunction)(void);
} Potentiometer;

typedef struct
{
	uint32_t	activeIR;
	uint32_t	timer;
	uint32_t	update;
	Potentiometer pots[4];
} Potentiometers;

extern Potentiometers potentiometers;

void POTENTIOMETERS_Initialize(void);
void POTENTIOMETERS_Start(const uint8_t potIndex, const uint8_t index);
void POTENTIOMETERS_EnableUpdate(void);
void POTENTIOMETERS_DisableUpdate(void);

// VOLUME
void POTENTIOMETERS_VolumePlus(void);
void POTENTIOMETERS_VolumePlusIr(void);
void POTENTIOMETERS_VolumeMinus(void);
void POTENTIOMETERS_VolumeMinusIr(void);
void POTENTIOMETERS_VolumeStop(void);
void POTENTIOMETERS_VolumeStopIr(void);
// BASS
void POTENTIOMETERS_BassPlus(void);
void POTENTIOMETERS_BassMinus(void);
void POTENTIOMETERS_BassStop(void);
// TREBLE
void POTENTIOMETERS_TreblePlus(void);
void POTENTIOMETERS_TrebleMinus(void);
void POTENTIOMETERS_TrebleStop(void);
// BALANCE
void POTENTIOMETERS_BalancePlus(void);
void POTENTIOMETERS_BalanceMinus(void);
void POTENTIOMETERS_BalanceStop(void);

// Common
void POTENTIOMETERS_SetCurrent(const uint8_t volume0, const uint8_t volume1, const uint8_t bass, const uint8_t treble, const uint8_t balance);
void POTENTIOMETERS_Process(void);
uint8_t POTENTIOMETERS_GetValueFromIndex(const uint8_t index);
uint8_t POTENTIOMETERS_GetIndexFromValue(const uint8_t value);


#endif /* INC_POTENTIOMETER_H_ */
