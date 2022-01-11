/*
 * calibrator.h
 *
 *  Created on: 11. sij 2022.
 *      Author: Admin
 */

#ifndef INC_CALIBRATOR_H_
#define INC_CALIBRATOR_H_

#include <stdint.h>

#define CALIBRATION_STEPS 150

typedef struct
{
	uint8_t 	data[CALIBRATION_STEPS];
	uint32_t 	active;
	uint32_t	channel;
	uint32_t	delay;
	uint32_t 	current;
} CalibrationManager;

void CALIBRATOR_Initialize(void);
void CALIBRATOR_Start(const uint32_t channel, const uint32_t delay);
void CALIBRATOR_Process(void);

#endif /* INC_CALIBRATOR_H_ */
