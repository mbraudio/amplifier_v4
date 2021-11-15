/*
 * buttons.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdint.h>

#define BUTTON_PRESSED_TIME			5   // 50ms
#define BUTTON_HOLD_TIME			200	// 2 seconds
#define BUTTON_LONG_HOLD_TIME		400	// 4 seconds

typedef enum
{
	Released = 0L,
	Triggered,
	Pressed,
	Hold
} ButtonState;

typedef struct
{
	ButtonState		state;
	uint32_t		time;
} Button;

typedef struct _ButtonManager
{
	Button power;
	Button loudness;
	Button speakersA;
	Button speakersB;
	Button direct;
	Button selector;

	uint32_t timer;
} ButtonManager;


extern volatile ButtonManager buttons;


void BUTTONS_Initialize(void);
uint32_t BUTTONS_PressValid(const uint32_t time, const uint32_t value);
void BUTTONS_ProcessADC_MainGroup(const uint8_t value);
void BUTTONS_ProcessADC_SelectorGroup(const uint8_t value);

#endif /* INC_BUTTONS_H_ */
