/*
 * encoder.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>

#define ENCODER_A_PIN				GPIO_PIN_4
#define ENCODER_B_PIN				GPIO_PIN_5

#define ENCODER_PLUS				1
#define ENCODER_MINUS				-1

#define ENCODER_COMMAND_PLUS		0xE1
#define ENCODER_COMMAND_MINUS		0xD2

typedef struct
{
	uint32_t a;
	uint32_t b;
	int32_t  previous;
	int32_t  index;
	uint32_t value;
	uint32_t result;
	uint32_t timer;
	uint32_t confirmTimeout;
	uint32_t readyChange;
	uint32_t readyConfirm;
	void(*valueChanged)(const int32_t);
	void(*valueConfirmed)(void);
} RotaryEncoder;

extern RotaryEncoder encoder;

void ENCODER_Initialize(const uint32_t timeout, void(*valueChanged)(const int32_t), void(*valueConfirmed)(void));
void ENCODER_Encode(const uint16_t pin);
void ENCODER_Process(void);

#endif /* INC_ENCODER_H_ */
