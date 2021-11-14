/*
 * heartbeat.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#include "heartbeat.h"
#include "stm32l0xx.h"
#include "main.h"

#define HEARTBEAT_COUNT 50 // 50 * 10ms = 500ms

static uint32_t heartbeatCounter = 0;

void HEARTBEAT_Process(void)
{
	heartbeatCounter++;
	if (heartbeatCounter >= HEARTBEAT_COUNT)
	{
		HAL_GPIO_TogglePin(LED_HEARTBEAT_GPIO_Port, LED_HEARTBEAT_Pin);
		heartbeatCounter = 0;
	}
}
