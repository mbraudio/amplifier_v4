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

// Speakers
void AMP_SetSpeakersA(const uint8_t state);
void AMP_SetSpeakersB(const uint8_t state);

// Direct
void AMP_SetDirect(const uint8_t state);

// Loudness
void AMP_SetLoudness(const uint8_t state);
// Volume LED
void AMP_SetVolumeKnobLed(const uint8_t state);
// DAC Filter
void AMP_SetDacFilter(const uint8_t filter);
// Bluetooth
void AMP_EnableBluetooth(const uint8_t state);

// Power
void AMP_GoToPowerOff(void);
void AMP_PowerOff(void);
void AMP_PowerToggle(void);

// Power processing
void AMP_ProcessPower(void);

#endif /* INC_AMPLIFIER_H_ */
