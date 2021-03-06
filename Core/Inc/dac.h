/*
 * dac.h
 *
 *  Created on: Dec 10, 2021
 *      Author: Admin
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "pcm9211.h"
#include "wm874x.h"
#include "wm8742x_registers.h"


void DAC_Initialize(SPI_HandleTypeDef* h);
void DAC_Setup(const uint8_t input);
void DAC_ResetWM874xSampleRate(void);
void DAC_PCM9211_ErrorHandler(void);
void DAC_PCM9211_NpcmHandler(void);
WMSampleRate DAC_GetSampleRate(void);
PCM9211BitRate DAC_GetBitRate(void);
void DAC_Process(void);


typedef struct {
	uint32_t error;
	uint32_t npcm;
	uint8_t  input;
	uint8_t  exactSampleRate;
	PCM9211BitRate  bitRate;
} DAC;

extern DAC dac;

#endif /* INC_DAC_H_ */
