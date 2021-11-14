/*
 * encoder.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */
#include "encoder.h"
#include "stm32l0xx.h"
#include "led.h"

RotaryEncoder encoder;

#define ENCODER_PIN_A	GPIO_PIN_4
#define ENCODER_PIN_B	GPIO_PIN_5

void ENCODER_Initialize(const uint32_t timeout, void(*valueChanged)(const int32_t), void(*valueConfirmed)(void))
{
	encoder.a = 1;
	encoder.b = 1;
	encoder.previous = 0;
	encoder.index = 0;
	encoder.value = 0;
	encoder.result = 0;
	encoder.timer = 0;
	encoder.confirmTimeout = timeout;
	encoder.readyChange = 0;
	encoder.readyConfirm = 0;
	encoder.valueChanged = valueChanged;
	encoder.valueConfirmed = valueConfirmed;
}

void ENCODER_Encode(const uint16_t pin)
{
	if (pin == ENCODER_PIN_B)
	{
		if (EXTI->FTSR & ENCODER_PIN_B)
		{
			CLEAR_BIT(EXTI->FTSR, ENCODER_PIN_B);
			SET_BIT(EXTI->RTSR, ENCODER_PIN_B);
			encoder.a = 0;
			if (encoder.b)
			{
				encoder.index = 0;
				encoder.value = 0;
				encoder.timer = 0;
			}
		}
		else if (EXTI->RTSR & ENCODER_PIN_B)
		{
			SET_BIT(EXTI->FTSR, ENCODER_PIN_B);
			CLEAR_BIT(EXTI->RTSR, ENCODER_PIN_B);
			encoder.a = 1;
		}
	}
	else if (pin == ENCODER_PIN_A)
	{
		if (EXTI->FTSR & ENCODER_PIN_A)
		{
			CLEAR_BIT(EXTI->FTSR, ENCODER_PIN_A);
			SET_BIT(EXTI->RTSR, ENCODER_PIN_A);
			encoder.b = 0;
			if (encoder.a)
			{
				encoder.index = 0;
				encoder.value = 0;
				encoder.timer = 0;
			}
		}
		else if (EXTI->RTSR & ENCODER_PIN_A)
		{
			CLEAR_BIT(EXTI->RTSR, ENCODER_PIN_A);
			SET_BIT(EXTI->FTSR, ENCODER_PIN_A);
			encoder.b = 1;
		}
	}

	uint32_t val = (encoder.a << 1) | encoder.b;
	encoder.value |= val << (encoder.index * 2);

	encoder.index++;

	if (encoder.a && encoder.b)
	{
		encoder.result = encoder.value;
		encoder.value = 0;
		encoder.index = 0;
		encoder.timer = 0;
		encoder.readyChange = 1;
	}
}

void ENCODER_Process(void)
{
	if (encoder.readyChange)
	{
		if (encoder.result == ENCODER_COMMAND_PLUS)
		{
			encoder.readyConfirm = 1;
			encoder.valueChanged(ENCODER_PLUS);
		}
		else if (encoder.result == ENCODER_COMMAND_MINUS)
		{
			encoder.readyConfirm = 1;
			encoder.valueChanged(ENCODER_MINUS);
		}
		encoder.readyChange = 0;
	}

	if (encoder.readyConfirm && (encoder.timer >= encoder.confirmTimeout))
	{
		encoder.valueConfirmed();
		encoder.readyConfirm = 0;
	}
}


