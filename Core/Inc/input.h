/*
 * input.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#include <stdint.h>
#include "main.h"

#define INPUT_MAX_SELECTION		5

typedef struct
{
	uint16_t 		led;
	GPIO_TypeDef* 	port;
	uint16_t		pin;
	uint8_t  		digital;
} InputData;

typedef struct
{
	InputData	inputs[INPUT_MAX_SELECTION];
	uint32_t 	npcmTimer;
	uint32_t 	muteTimer;
} Input;

extern Input input;

void INPUT_Initialize(void);
void INPUT_Process(void);
void INPUT_Activate(void);
void INPUT_Changed(const int8_t direction);
void INPUT_Confirmed(void);
void INPUT_SetDirect(const uint8_t newIndex);
void INPUT_InputsOff(void);
void INPUT_AllOff(void);

void INPUT_Mute(const uint32_t status);

#endif /* INC_INPUT_H_ */
