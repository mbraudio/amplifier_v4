/*
 * protection.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_PROTECTION_H_
#define INC_PROTECTION_H_

#include <stdint.h>

#define DC_PIN	GPIO_PIN_9
#define V_PIN   GPIO_PIN_1

typedef struct
{
	uint8_t		dc1;
	uint8_t		dc2;
	uint8_t		overheat1;
	uint8_t		overheat2;
	uint8_t		voltage1;
	uint8_t		voltage2;
	uint8_t 	crc;
} Protection;

extern Protection protection;

void PROTECTION_Initialize(void);
void PROTECTION_Save(void);
uint32_t PROTECTION_Load(void);
void PROTECTION_Reset(void);
void PROTECTION_Process(void);
void PROTECTION_LoadCheck(void);
void PROTECTION_DirectCheck(void);
void PROTECTION_EnableDc(void);
void PROTECTION_EnableOverload(void);
//void PROTECTION_EnableOverheat(void);
void PROTECTION_EnableVoltage(void);
void PROTECTION_NotifyError(const uint32_t errorId);

#endif /* INC_PROTECTION_H_ */
