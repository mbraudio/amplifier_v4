/*
 * ir_nec.h
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */

#ifndef INC_IR_NEC_H_
#define INC_IR_NEC_H_

#include <stdint.h>

// Remote Control: Pioneer AXD7644

// AMPLIFIER
#define IR_CODE_ON_STANDBY				0xE31C5AA5
#define IR_CODE_DIMMER_1				0xA9565AA5
#define IR_CODE_DIMMER_2				0xF9065AA5
#define IR_CODE_SACD_CD					0xB34C5AA5
#define IR_CODE_NETWORK					0x38C75AA5 // Twin 0xA35C5AA5
#define IR_CODE_PHONO					0xB24D5AA5
#define IR_CODE_TUNER					0xB8475AA5
#define IR_CODE_AUX						0xB04F5AA5
#define IR_CODE_RECORDER				0xB14E5AA5
#define IR_CODE_OPTION					0x36C95AA5 // Twin 0x619E5AA5
#define IR_CODE_INPUT_MINUS				0x3EC15AA5 // Twin 0xA9565AA5
#define IR_CODE_INPUT_PLUS				0xAA555AA5
#define IR_CODE_LOUDNESS				0xBB445AA5
#define IR_CODE_VOLUME_PLUS				0xF50A5AA5
#define IR_CODE_SPEAKERS_A				0xFF005AA5 // Twin 0xA6595AA5
#define IR_CODE_MUTE					0xED125AA5
#define IR_CODE_VOLUME_MINUS			0xF40B5AA5
#define IR_CODE_SPEAKERS_B				0xFE015AA5 // Twin 0xA6595AA5
#define IR_CODE_DIRECT					0xF7085AA5 // Twin 0xA25D5AA5
#define IR_CODE_APD						0x3BC45AA5 // Twin 0xA9565AA5
// SACD PLAYER
#define IR_CODE_SACD_ON_OFF				0xE31C5DA2
#define IR_CODE_SACD_PLAY				0x47B85DA2 // Twin 0x21DE5DA2
#define IR_CODE_SACD_FUNC				0x7F805DA2 // Twin 0x39C65DA2
#define IR_CODE_SACD_REWIND				0xEE115DA2
#define IR_CODE_SACD_STOP				0xE9165DA2
#define IR_CODE_SACD_FORWARD			0xEF105DA2
// NETWORK AUDIO PLAYER
// First part
#define IR_CODE_NAP_ON_OFF				0xC33C50AF // Twin 0xDF2058A7
#define IR_CODE_NAP_PLAY				0xC63950AF // Twin 0xDF2058A7
#define IR_CODE_NAP_FUNC				0xD52A50AF // Twin 0xDF2058A7
#define IR_CODE_NAP_REWIND				0xCE3150AF // Twin 0xDF2058A7
#define IR_CODE_NAP_STOP				0xC93650AF // Twin 0xDF2058A7
#define IR_CODE_NAP_FORWARD				0xCF3050AF // Twin 0xDF2058A7
// Second part
#define IR_CODE_NAP_SETUP				0xC23D50AF // Twin 0xDF2058A7
#define IR_CODE_NAP_UP					0x9F6050AF // Twin 0xDF2058A7
#define IR_CODE_NAP_SOUND				0xC03F50AF // Twin 0xDF2058A7
#define IR_CODE_NAP_LEFT				0x9C6350AF // Twin 0xDF2058A7
#define IR_CODE_NAP_ENTER				0x9B6450AF // Twin 0xDF2058A7
#define IR_CODE_NAP_RIGHT				0x9D6250AF // Twin 0xDF2058A7
#define IR_CODE_NAP_HOME_MENU			0xC13E50AF // Twin 0xDF2058A7
#define IR_CODE_NAP_DOWN				0x9E6150AF // Twin 0xDF2058A7
#define IR_CODE_NAP_RETURN				0x9A6550AF // Twin 0xDF2058A7
// IR external interrupt id pin
#define IR_PIN	GPIO_PIN_11

/*
typedef struct
{
	uint8_t		address;
	uint8_t		addressReverse;
	uint8_t		command;
	uint8_t		commandReverse;
} IRCode;*/

/*typedef union
{
	uint32_t	value;
	IRCode		code;
} IRCommand;*/

typedef struct
{
	uint32_t	command;
	uint32_t	lastCommand;
	uint32_t	commandToDecode;
	uint32_t  	counter;
	uint32_t  	ignoreTime;
	uint32_t 	ready;
	uint32_t	index;
} IR;

extern IR ir;

void IR_Initialize(void);
void IR_Encode(void);
void IR_Decode(void);
void IR_Process(void);
void IR_ProcessTimer(void);


#endif /* INC_IR_NEC_H_ */
