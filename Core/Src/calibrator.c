/*
 * calibrator.c
 *
 *  Created on: 11. sij 2022.
 *      Author: Admin
 */
#include "calibrator.h"
#include "adc.h"
#include "potentiometer.h"
#include "bluetooth.h"
#include "stm32l0xx_hal.h"
#include <string.h>

#define DATA_BYTES 16

CalibrationManager calibrationManager;

void CALIBRATOR_Initialize(void)
{
	calibrationManager.active = 0;
}

void CALIBRATOR_Start(const uint32_t channel, const uint32_t delay)
{
	calibrationManager.active = 0;
	calibrationManager.channel = channel;
	calibrationManager.delay = delay;
	calibrationManager.current = 0;
	memset(calibrationManager.data, 0, CALIBRATION_STEPS);
	calibrationManager.active = 1;
}

void CALIBRATOR_Process(void)
{
	if (calibrationManager.active)
	{
		if (calibrationManager.current < CALIBRATION_STEPS)
		{
			if (adc.dataReady)
			{
				calibrationManager.data[calibrationManager.current++] = adc.data[calibrationManager.channel];
				POTENTIOMETERS_VolumePlus();
				HAL_Delay(calibrationManager.delay);
				POTENTIOMETERS_Stop();
			}
		}
		else
		{
			calibrationManager.active = 0;
			calibrationManager.current = 0;

			uint8_t data[DATA_BYTES + 2];
			uint32_t parts = CALIBRATION_STEPS / DATA_BYTES;
			uint32_t remaining = CALIBRATION_STEPS % DATA_BYTES;
			uint32_t i;

			for (i = 0; i < parts; i++)
			{
				data[0] = COMMAND_SEND_CALIBRATION_DATA_1;
				data[1] = i;
				memcpy(&data[2], &calibrationManager.data[(i * DATA_BYTES)], DATA_BYTES);
				BLUETOOTH_SendData(data, (DATA_BYTES + 2));
				HAL_Delay(300);
			}

			if (remaining)
			{
				data[0] = COMMAND_SEND_CALIBRATION_DATA_2;
				data[1] = i;
				memcpy(&data[2], &calibrationManager.data[(CALIBRATION_STEPS - remaining)], remaining);
				BLUETOOTH_SendData(data, (remaining + 2));
			}
			else
			{
				BLUETOOTH_Send(COMMAND_SEND_CALIBRATION_DATA_2, 0);
			}

		}
	}
}

