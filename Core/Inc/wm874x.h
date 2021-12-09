/*
 * wm874x.h
 *
 *  Created on: Dec 8, 2021
 *      Author: Admin
 */
#include "main.h"
#include "wm8742x_registers.h"

void WM874X_Initialize(SPI_HandleTypeDef* h);
void WM874X_Setup(void);
uint32_t WM874X_Write(const uint8_t reg, const uint8_t data);
uint32_t WM874X_SetSampleRate(const WMSampleRate rate);
