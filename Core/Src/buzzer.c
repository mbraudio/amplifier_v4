/*
 * buzzer.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#include "buzzer.h"
#include "main.h"
#include "stm32l0xx.h"

inline void BUZZER_Off(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

inline void BUZZER_On(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
}

void BUZZER_PowerOn(void)
{
	uint32_t i;
	BUZZER_Off();
	for (i = 0; i < 2; i++) {
		BUZZER_On();
		HAL_Delay(14);
		BUZZER_Off();
		HAL_Delay(220);
	}
}
