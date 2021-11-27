/*
 * mcp23008.h
 *
 *  Created on: Nov 18, 2021
 *      Author: Admin
 */

#ifndef INC_MCP23008_H_
#define INC_MCP23008_H_

#include "main.h"

void MCP23008_Init(I2C_HandleTypeDef* h);
void MCP23008_Setup(void);

void MCP23008_Stop(void);

void MCP23008_VolumePlus(void);
void MCP23008_VolumeMinus(void);

void MCP23008_BalancePlus(void);
void MCP23008_BalanceMinus(void);

void MCP23008_TreblePlus(void);
void MCP23008_TrebleMinus(void);

void MCP23008_BassPlus(void);
void MCP23008_BassMinus(void);

#endif /* INC_MCP23008_H_ */
