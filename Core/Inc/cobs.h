/*
 * cobs.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_COBS_H_
#define INC_COBS_H_

#include <stdint.h>

uint16_t COBS_Encode(const uint8_t *src, const uint16_t length, uint8_t *dst);
uint16_t COBS_Decode(const uint8_t *src, const uint16_t length, uint8_t *dst);

#endif /* INC_COBS_H_ */
