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
void MCP23008_VolumePlus(void);
void MCP23008_VolumeMinus(void);
void MCP23008_VolumeStop(void);

#endif /* INC_MCP23008_H_ */
