/*
 * amplifier.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_AMPLIFIER_H_
#define INC_AMPLIFIER_H_

#include <stdint.h>
#include "main.h"

void AMP_SetPowerPin(const GPIO_PinState state);

// Mute
void AMP_MuteToggle(void);

// Speakers
void AMP_SetSpeakersA(const uint8_t state);
void AMP_SetSpeakersB(const uint8_t state);

// Direct
void AMP_SetDirect(const uint8_t state);

// Loudness
void AMP_SetLoudness(const uint8_t state);

// Power
void AMP_GoToPowerOff(void);
void AMP_PowerOff(void);
void AMP_PowerToggle(void);

// Power processing
void AMP_ProcessPower(void);

#endif /* INC_AMPLIFIER_H_ */
