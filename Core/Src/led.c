/*
 * led.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */
#include "led.h"
#include "main.h"

TIM_HandleTypeDef* htim;
LedData ledData[LED_COUNT];

void LED_Initialize(TIM_HandleTypeDef* h)
{
	uint32_t led;

	htim = h;

	led = LED_POWER;
	ledData[led].gpio = LED_POWER_GPIO_Port;
	ledData[led].pin = LED_POWER_Pin;

	led = LED_APD_BLUETOOTH;
	ledData[led].gpio = LED_APD_GPIO_Port;
	ledData[led].pin = LED_APD_Pin;

	led = LED_DIRECT;
	ledData[led].gpio = LED_DIRECT_GPIO_Port;
	ledData[led].pin = LED_DIRECT_Pin;

	led = LED_SPEAKERS_A;
	ledData[led].gpio = LED_SPEAKERS_A_GPIO_Port;
	ledData[led].pin = LED_SPEAKERS_A_Pin;

	led = LED_SPEAKERS_B;
	ledData[led].gpio = LED_SPEAKERS_B_GPIO_Port;
	ledData[led].pin = LED_SPEAKERS_B_Pin;

	led = LED_LOUDNESS;
	ledData[led].gpio = LED_LOUDNESS_GPIO_Port;
	ledData[led].pin = LED_LOUDNESS_Pin;

	led = LED_INPUT_SACD;
	ledData[led].gpio = LED_SACD_GPIO_Port;
	ledData[led].pin = LED_SACD_Pin;

	led = LED_INPUT_NETWORK;
	ledData[led].gpio = LED_NETWORK_GPIO_Port;
	ledData[led].pin = LED_NETWORK_Pin;

	led = LED_INPUT_PHONO;
	ledData[led].gpio = LED_PHONO_GPIO_Port;
	ledData[led].pin = LED_PHONO_Pin;

	led = LED_INPUT_TUNER;
	ledData[led].gpio = LED_TUNER_GPIO_Port;
	ledData[led].pin = LED_TUNER_Pin;

	led = LED_INPUT_AUX;
	ledData[led].gpio = LED_AUX_GPIO_Port;
	ledData[led].pin = LED_AUX_Pin;

	led = LED_INPUT_RECORDER;
	ledData[led].gpio = LED_RECORDER_GPIO_Port;
	ledData[led].pin = LED_RECORDER_Pin;

	led = LED_AMP_DIRECT;
	ledData[led].gpio = LED_AMP_DIRECT_GPIO_Port;
	ledData[led].pin = LED_AMP_DIRECT_Pin;

	LED_AllOff();
	LED_SetBrightness(LED_DEFAULT_BRIGHTNESS);
}

void LED_SetBrightness(const uint8_t brightness)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (uint32_t)brightness;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);

	if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
}

inline void LED_Standby(const GPIO_PinState state)
{
	HAL_GPIO_WritePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin, state);
}

void LED_Set(const uint32_t led, const GPIO_PinState state)
{
	LedData* data = &ledData[led];
	HAL_GPIO_WritePin(data->gpio, data->pin, state);
}

/*
void LED_SetVolume(const uint16_t red, const uint16_t green, const uint16_t blue)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM2;
	sConfigOC.Pulse = green;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_Stop(htimVolume, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(htimVolume, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(htimVolume, TIM_CHANNEL_4);

	if (HAL_TIM_PWM_ConfigChannel(htimVolume, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	sConfigOC.Pulse = blue;
	if (HAL_TIM_PWM_ConfigChannel(htimVolume, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	sConfigOC.Pulse = red;
	if (HAL_TIM_PWM_ConfigChannel(htimVolume, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_PWM_Start(htimVolume, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htimVolume, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(htimVolume, TIM_CHANNEL_4);
}
*/

void LED_AllOff(void)
{
	uint32_t i;
	for (i = 0; i < LED_COUNT; i++)
	{
		LED_Set(i, GPIO_PIN_RESET);
	}

	//LED_SetVolume(0, 0, 0);
}


