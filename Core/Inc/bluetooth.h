/*
 * bluetooth.h
 *
 *  Created on: Jan 9, 2022
 *      Author: Admin
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <stdint.h>


#define COMMAND_SYSTEM_DATA					1
#define COMMAND_TOGGLE_POWER				2
#define COMMAND_TOGGLE_MUTE					3
#define COMMAND_CHANGE_INPUT				4
#define COMMAND_TOGGLE_DIRECT				5
#define COMMAND_TOGGLE_SPEAKER_A			6
#define COMMAND_TOGGLE_SPEAKER_B			7
#define COMMAND_TOGGLE_LOUDNESS				8
#define COMMAND_TOGGLE_PAMP_DIRECT  		9
#define COMMAND_UPDATE_VOLUME_VALUE			10
#define COMMAND_UPDATE_BASS_VALUE			11
#define COMMAND_UPDATE_TREBLE_VALUE			12
#define COMMAND_UPDATE_BALANCE_VALUE		13
#define COMMAND_UPDATE_TEMPERATURE			14
#define COMMAND_BRIGHTNESS_INDEX			15
#define COMMAND_SET_VOLUME_KNOB_LED		    16
#define COMMAND_UPDATE_DAC_DATA      		17
#define COMMAND_UPDATE_DAC_FILTER      		18

#define COMMAND_CALIBRATE 				 	100
#define COMMAND_SEND_CALIBRATION_DATA_1  	101
#define COMMAND_SEND_CALIBRATION_DATA_2  	102


void BLUETOOTH_Process(const uint8_t* data, const uint32_t size);
uint32_t BLUETOOTH_CalculateCrc(const uint8_t* data, const uint32_t size);
void BLUETOOTH_Send(const uint8_t command, const uint8_t value);
void BLUETOOTH_Send2(const uint8_t command, const uint8_t value0, const uint8_t value1);
void BLUETOOTH_Send3(const uint8_t command, const uint8_t value0, const uint8_t value1, const uint8_t value2);
void BLUETOOTH_SendData(const uint8_t* data, const uint8_t length);

void BLUETOOTH_SendSystem(void);

#endif /* INC_BLUETOOTH_H_ */
