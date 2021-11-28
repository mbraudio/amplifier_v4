/*
 * protection.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_PROTECTION_H_
#define INC_PROTECTION_H_

#include <stdint.h>

typedef struct
{
	uint8_t		dc;
	uint8_t		overheat;
	uint8_t		voltage;
	uint8_t 	crc;
} __attribute__((packed)) Protection;

extern Protection protection;

void PROTECTION_Save(void);
void PROTECTION_Load(void);
void PROTECTION_Reset(void);
void PROTECTION_LoadCheck(void);
void PROTECTION_LiveCheck(void);
void PROTECTION_EnableDc(void);
void PROTECTION_EnableOverload(void);
//void PROTECTION_EnableOverheat(void);
void PROTECTION_EnableVoltage(void);

#endif /* INC_PROTECTION_H_ */
