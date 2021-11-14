/*
 * eeprom.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <stdint.h>

void EEPROM_Write(uint32_t address, const uint32_t size, uint8_t* data);
void EEPROM_Read(uint32_t address, const uint32_t size, uint8_t* data);

#endif /* INC_EEPROM_H_ */
