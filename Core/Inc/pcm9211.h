/*
 * pcm9211.h
 *
 *  Created on: 5. pro 2021.
 *      Author: Admin
 */

#ifndef INC_PCM9211_H_
#define INC_PCM9211_H_

#include "main.h"
#include "pcm9211_registers.h"

#define PCM9211_ERROR_PIN	GPIO_PIN_7
#define PCM9211_NPCM_PIN 	GPIO_PIN_3

void PCM9211_Initialize(SPI_HandleTypeDef* h);
void PCM9211_Setup(void);
void PCM9211_Write(const uint8_t reg, const uint8_t data);
uint8_t PCM9211_Read(const uint8_t reg);
uint32_t PCM9211_WriteCheck(const uint8_t reg, const uint8_t data);

#endif /* INC_PCM9211_H_ */
