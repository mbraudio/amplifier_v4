/*
 * pcm9211.c
 *
 *  Created on: 5. pro 2021.
 *      Author: Admin
 */
#include "pcm9211.h"

SPI_HandleTypeDef* pcmspi;

void PCM9211_Initialize(SPI_HandleTypeDef* h)
{
	pcmspi = h;
	HAL_GPIO_WritePin(DIR_CS_GPIO_Port, DIR_CS_Pin, GPIO_PIN_SET);
}

void PCM9211_Setup(const uint8_t input)
{
	uint32_t status = 0;

	HAL_GPIO_WritePin(DIR_RESET_GPIO_Port, DIR_RESET_Pin, GPIO_PIN_SET);

	HAL_Delay(100);

	//status += PCM9211_WriteCheck(PCM9211_REG_20_ERROR_OUTPUT_CONDITION, 0b00000101); // ERROR is INT0 and NPCM is INT1
	//status += PCM9211_WriteCheck(PCM9211_REG_25_ERROR_CAUSE_SETTING, 0b00111000); // EFSCHG + EFSLMT + ENPCM
	status += PCM9211_WriteCheck(PCM9211_REG_25_ERROR_CAUSE_SETTING, 0b00101000); // EFSCHG + ENPCM
	//status += PCM9211_WriteCheck(PCM9211_REG_27_DIR_ACCEPTABLE_FS_RANGE_SETTING_AND_MASK, 0b00001111); // HILMT + LOLMT - limiting both
	status += PCM9211_WriteCheck(PCM9211_REG_28_NON_PCM_DEFINITION_SETTING, 0b00000111); // 0b00100111
	status += PCM9211_WriteCheck(PCM9211_REG_2A_INT0_OUTPUT_CAUSE_MASK_SETTING, 0b01000000); // Mask all to INT0
	status += PCM9211_WriteCheck(PCM9211_REG_2B_INT1_OUTPUT_CAUSE_MASK_SETTING, 0b10111111); // Mask all to INT1
	status += PCM9211_WriteCheck(PCM9211_REG_2E_INT0_INT1_OUTPUT_POLARITY_SETTING, 0b01000100); // Positive logic
	status += PCM9211_WriteCheck(PCM9211_REG_30_DIR_RECOVERED_SYSTEM_CLOCL_RATIO_SETTING, 0b00010010); // PLL SCK Dividing Ratio Automatic Control
	status += PCM9211_WriteCheck(PCM9211_REG_34_DIR_INPUT_BIPHASE_SOURCE_SELECT_COAX_AMP_CONTROL, (0b11000000 | input)); // Power down Coax amps, RXIN2 or RXIN4 selected as input
	status += PCM9211_WriteCheck(PCM9211_REG_35_RECOUT0_OUTPUT_BIPHASE_SOURCE_SELECT, 0b00010010); // MPO0 mute
	status += PCM9211_WriteCheck(PCM9211_REG_36_RECOUT1_OUTPUT_BIPHASE_SOURCE_SELECT, 0b00010010); // MPO1 mute
	status += PCM9211_WriteCheck(PCM9211_REG_37_PORT_FS_CALCULATOR_MEASUREMENT_TARGET_SETTINGS, 0b00000101); // FS calculator main port
	status += PCM9211_WriteCheck(PCM9211_REG_40_SYSTEM_RESET_CONTROL, 0b11100010); // Power down ADC and DIT

	if (status)
	{
		Error_Handler();
	}
}

void PCM9211_Write(const uint8_t reg, const uint8_t data)
{
	uint8_t txData[2] = { reg, data };
	HAL_GPIO_WritePin(DIR_CS_GPIO_Port, DIR_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(pcmspi, txData, 2, 100);
	HAL_GPIO_WritePin(DIR_CS_GPIO_Port, DIR_CS_Pin, GPIO_PIN_SET);
}

uint8_t PCM9211_Read(const uint8_t reg)
{
	uint8_t txData[2] = { (reg + 0x80), 0 };
	uint8_t rxData[2] = { 0, 0 };
	HAL_GPIO_WritePin(DIR_CS_GPIO_Port, DIR_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(pcmspi, txData, rxData, 2, 100);
	HAL_GPIO_WritePin(DIR_CS_GPIO_Port, DIR_CS_Pin, GPIO_PIN_SET);
	return rxData[1];
}

uint32_t PCM9211_WriteCheck(const uint8_t reg, const uint8_t data)
{
	PCM9211_Write(reg, data);
	uint8_t rxData = PCM9211_Read(reg);
	return (rxData != data);
}

