/*
 * wm874x.c
 *
 *  Created on: Dec 8, 2021
 *      Author: Admin
 */
#include "wm874x.h"

SPI_HandleTypeDef* hspi;


void WM874X_Initialize(SPI_HandleTypeDef* h)
{
	hspi = h;
	HAL_GPIO_WritePin(WM_CS_GPIO_Port, WM_CS_Pin, GPIO_PIN_SET);
}

void WM874X_Setup(void)
{
	uint32_t status = 0;

	status += WM874X_Write(WM874X_REG_06_FILTER_CONTROL, 0x00);
	status += WM874X_Write(WM874X_REG_07_MODE_CONTROL_1, 0x00);

	if (status)
	{
		Error_Handler();
	}
}

uint32_t WM874X_Write(const uint8_t reg, const uint8_t data)
{
	HAL_StatusTypeDef sts;
	uint8_t txData[2] = { (reg << 1), data };
	HAL_GPIO_WritePin(WM_CS_GPIO_Port, WM_CS_Pin, GPIO_PIN_RESET);
	sts = HAL_SPI_Transmit(hspi, txData, 2, 100);
	HAL_GPIO_WritePin(WM_CS_GPIO_Port, WM_CS_Pin, GPIO_PIN_SET);
	return sts;
}

uint32_t WM874X_SetSampleRate(const WMSampleRate rate)
{
	WM874X_REGISTER_7 reg;
	reg.mode = ModePCM;
	reg.samplingRatio = SamplingRatioAuto;
	reg.sampleRate = rate;
	reg.mode8x = Mode8XDisabled;
	return WM874X_Write(WM874X_REG_07_MODE_CONTROL_1, reg.value);
}

void WM874X_Mute(const uint32_t enable)
{
	uint8_t data = enable ? 0x08 : 0x00;
	WM874X_Write(WM874X_REG_04_VOLUME_CONTROL, data);
}

