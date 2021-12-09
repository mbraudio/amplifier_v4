/*
 * wm874x.h
 *
 *  Created on: Dec 8, 2021
 *      Author: Admin
 */
#include "main.h"

#define WM874X_REG_00_DACLLSB_ATTENUATION		0x00
#define WM874X_REG_01_DACLMSB_ATTENUATION		0x01
#define WM874X_REG_02_DACRLSB_ATTENUATION		0x02
#define WM874X_REG_03_DACRMSB_ATTENUATION		0x03
#define WM874X_REG_04_VOLUME_CONTROL			0x04
#define WM874X_REG_05_FORMAT_CONTROL			0x05
#define WM874X_REG_06_FILTER_CONTROL			0x06
#define WM874X_REG_07_MODE_CONTROL_1			0x07
#define WM874X_REG_08_MODE_CONTROL_2			0x08
#define WM874X_REG_09_SOFTWARE_RESET			0x09
#define WM874X_REG_20_ADDITIONAL_CONTROL_1		0x20

void WM874X_Initialize(SPI_HandleTypeDef* h);
void WM874X_Setup(void);
uint32_t WM874X_Write(const uint8_t reg, const uint8_t data);

