/*
 * led.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>
#include "stm32l0xx.h"

typedef struct
{
	GPIO_TypeDef*  	gpio;
	uint16_t		pin;
} LedData;

#define LED_MAX_BRIGHTNESS 255
#define LED_DEFAULT_BRIGHTNESS 127

// GPIO Leds
// Leds
#define 	LED_POWER						0
#define 	LED_APD_BLUETOOTH				1
#define 	LED_DIRECT						2
#define 	LED_SPEAKERS_A					3
#define 	LED_SPEAKERS_B					4
#define 	LED_LOUDNESS					5
#define 	LED_INPUT_SACD					6
#define 	LED_INPUT_NETWORK				7
#define 	LED_INPUT_PHONO					8
#define 	LED_INPUT_TUNER					9
#define 	LED_INPUT_AUX					10
#define 	LED_INPUT_RECORDER				11
#define 	LED_AMP_DIRECT					12

#define 	LED_COUNT (LED_AMP_DIRECT + 1)

void LED_Initialize(TIM_HandleTypeDef* h);
void LED_SetBrightness(const uint8_t brightness);
void LED_Set(const uint32_t led, const GPIO_PinState state);
void LED_Standby(const GPIO_PinState state);
//void LED_SetVolume(const uint16_t red, const uint16_t green, const uint16_t blue);
void LED_AllOff(void);

#endif /* INC_LED_H_ */
