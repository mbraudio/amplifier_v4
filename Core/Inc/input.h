/*
 * input.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#include <stdint.h>

#define INPUT_MAX_SELECTION		6

typedef struct
{
	uint16_t led;
	uint8_t  value;
} InputData;

typedef struct
{
	InputData	inputs[INPUT_MAX_SELECTION];
} Input;

void INPUT_Initialize(void);
void INPUT_Activate(void);
void INPUT_Changed(const int32_t direction);
void INPUT_Confirmed(void);
void INPUT_SetDirect(const uint8_t newIndex);

void INPUT_Mute(const uint32_t status);

#endif /* INC_INPUT_H_ */
