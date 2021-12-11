/*
 * pcm9211_registers.h
 *
 *  Created on: Dec 10, 2021
 *      Author: Admin
 */

#ifndef INC_PCM9211_REGISTERS_H_
#define INC_PCM9211_REGISTERS_H_

#define PCM9211_REG_20_ERROR_OUTPUT_CONDITION							0x20
#define PCM9211_REG_21_DIR_INITIAL_SETTINGS_1_OF_3						0x21
#define PCM9211_REG_22_DIR_INITIAL_SETTINGS_2_OF_3						0x22
#define PCM9211_REG_23_DIR_INITIAL_SETTINGS_3_OF_3						0x23
#define PCM9211_REG_24_OSCILLATION_CIRCUIT_CONTROL						0x24
#define PCM9211_REG_25_ERROR_CAUSE_SETTING								0x25
#define PCM9211_REG_26_AUTO_SOURCE_SELECTOR_CAUSE_SETTING				0x26
#define PCM9211_REG_27_DIR_ACCEPTABLE_FS_RANGE_SETTING_AND_MASK			0x27
#define PCM9211_REG_28_NON_PCM_DEFINITION_SETTING						0x28
#define PCM9211_REG_29_DTS_CD_LD_DETECTION_SETTING						0x29
#define PCM9211_REG_2A_INT0_OUTPUT_CAUSE_MASK_SETTING					0x2A
#define PCM9211_REG_2B_INT1_OUTPUT_CAUSE_MASK_SETTING					0x2B
#define PCM9211_REG_2C_INT0_OUTPUT_REGISTER								0x2C // R
#define PCM9211_REG_2D_INT1_OUTPUT_REGISTER								0x2D // R
#define PCM9211_REG_2E_INT0_INT1_OUTPUT_POLARITY_SETTING				0x2E
#define PCM9211_REG_2F_DIR_OUTPUT_DATA_FORMAT							0x2F
#define PCM9211_REG_30_DIR_RECOVERED_SYSTEM_CLOCL_RATIO_SETTING 		0x30
#define PCM9211_REG_31_XTI_SOURCE_CLOCL_FREQUENCY_SETTING				0x31
#define PCM9211_REG_32_DIR_SOURCE_SEC_BIT_LR_CLOCK_FREQUENCY_SETTING 	0x32
#define PCM9211_REG_33_XTI_SOURCE_SEC_BIT_LR_CLOCK_FREQUENCY_SETTING 	0x33
#define PCM9211_REG_34_DIR_INPUT_BIPHASE_SOURCE_SELECT_COAX_AMP_CONTROL 0x34
#define PCM9211_REG_35_RECOUT0_OUTPUT_BIPHASE_SOURCE_SELECT				0x35
#define PCM9211_REG_36_RECOUT1_OUTPUT_BIPHASE_SOURCE_SELECT				0x36
#define PCM9211_REG_37_PORT_FS_CALCULATOR_MEASUREMENT_TARGET_SETTINGS	0x37
#define PCM9211_REG_38_PORT_FS_CALCULATOR_RESULT_OUTPUT					0x38 // R
#define PCM9211_REG_39_INCOMING_BIPHASE_INFO_AND_CALC_FS_OUTPUT			0x39 // R
#define PCM9211_REG_3A_PC_BUFFER_BYTE0									0x3A // R
#define PCM9211_REG_3B_PC_BUFFER_BYTE1									0x3B // R
#define PCM9211_REG_3C_PD_BUFFER_BYTE0									0x3C // R
#define PCM9211_REG_3D_PD_BUFFER_BYTE1									0x3D // R
#define PCM9211_REG_40_SYSTEM_RESET_CONTROL								0x40
#define PCM9211_REG_42_ADC_FUNCTION_CONTROL_1_OF_3						0x42
#define PCM9211_REG_46_ADC_L_CH_DIGITAL_ATT_CONTROL						0x46
#define PCM9211_REG_47_ADC_R_CH_DIGITAL_ATT_CONTROL						0x47
#define PCM9211_REG_48_ADC_FUNCTION_CONTROL_2_OF_3						0x48
#define PCM9211_REG_49_ADC_FUNCTION_CONTROL_3_OF_3						0x49
#define PCM9211_REG_5A_DIR_CHANNEL_STATUS_DATA_BUFFER_1_OF_6			0x5A // R
#define PCM9211_REG_5B_DIR_CHANNEL_STATUS_DATA_BUFFER_2_OF_6			0x5B // R
#define PCM9211_REG_5C_DIR_CHANNEL_STATUS_DATA_BUFFER_3_OF_6			0x5C // R
#define PCM9211_REG_5D_DIR_CHANNEL_STATUS_DATA_BUFFER_4_OF_6			0x5D // R
#define PCM9211_REG_5E_DIR_CHANNEL_STATUS_DATA_BUFFER_5_OF_6			0x5E // R
#define PCM9211_REG_5F_DIR_CHANNEL_STATUS_DATA_BUFFER_6_OF_6			0x5F // R
#define PCM9211_REG_60_DIT_FUNCTION_CONTROL_1_OF_3						0x60
#define PCM9211_REG_61_DIT_FUNCTION_CONTROL_2_OF_3						0x61
#define PCM9211_REG_62_DIT_FUNCTION_CONTROL_3_OF_3						0x62
#define PCM9211_REG_63_DIT_CHANNEL_STATUS_DATA_BUFFER_1_OF_6			0x63
#define PCM9211_REG_64_DIT_CHANNEL_STATUS_DATA_BUFFER_2_OF_6			0x64
#define PCM9211_REG_65_DIT_CHANNEL_STATUS_DATA_BUFFER_3_OF_6			0x65
#define PCM9211_REG_66_DIT_CHANNEL_STATUS_DATA_BUFFER_4_OF_6			0x66
#define PCM9211_REG_67_DIT_CHANNEL_STATUS_DATA_BUFFER_5_OF_6			0x67
#define PCM9211_REG_68_DIT_CHANNEL_STATUS_DATA_BUFFER_6_OF_6			0x68
#define PCM9211_REG_6A_MAIN_OUTPUT_AND_AOXOUT_PORT_DATA_MUTE_CONTROL	0x6A
#define PCM9211_REG_6B_MAIN_OUTPUT_PORT_OUTPUT_SOURCE_SETTING			0x6B
#define PCM9211_REG_6C_AUX_OUTPUT_PORT_OUTPUT_SOURCE_SETTING			0x6C
#define PCM9211_REG_6D_MPIO_B_AND_MAIN_OUTPUT_PORT_HI_Z_CONTROL			0x6D
#define PCM9211_REG_6E_MPIO_C_AND_MPIO_A_HI_Z_CONTROL					0x6E
#define PCM9211_REG_6F_MPIO_A_B_C_GROUP_FUNCTION_ASSIGN					0x6F
#define PCM9211_REG_70_MPIO_A_FLAGS_GPIO_ASSIGN_SETTING					0x70
#define PCM9211_REG_71_MPIO_B_C_FLAGS_GPIO_ASSIGN_SETTING				0x71
#define PCM9211_REG_72_MPIO_A1_A0_OUTPUT_FLAG_SELECT					0x72
#define PCM9211_REG_73_MPIO_A3_A2_OUTPUT_FLAG_SELECT					0x73
#define PCM9211_REG_74_MPIO_B1_B0_OUTPUT_FLAG_SELECT					0x74
#define PCM9211_REG_75_MPIO_B3_B2_OUTPUT_FLAG_SELECT					0x75
#define PCM9211_REG_76_MPIO_C1_C0_OUTPUT_FLAG_SELECT					0x76
#define PCM9211_REG_77_MPIO_C3_C2_OUTPUT_FLAG_SELECT					0x77
#define PCM9211_REG_78_MPIO1_MPIO0_FUNCTION_ASSIGN_SETING				0x78
#define PCM9211_REG_79_GPIO_IO_DIRECTION_CONTROL_FOR_MPIO_A_B			0x79
#define PCM9211_REG_7A_GPIO_IO_DIRECTION_CONTROL_FOR_MPIO_C				0x7A
#define PCM9211_REG_7B_GPIO_OUTPUT_DATA_SETTING_FOR_MPIO_A_B			0x7B
#define PCM9211_REG_7C_GPIO_OUTPUT_DATA_SETTING_FOR_MPIO_C				0x7C
#define PCM9211_REG_7D_GPIO_INPUT_DATA_REGISTER_FOR_MPIO_A_B			0x7D // R
#define PCM9211_REG_7E_GPIO_INPUT_DATA_REGISTER_FOR_MPIO_C				0x7E // R

#define PCM9211_FREQUENCY_OUT_OF_RANGE	0
#define PCM9211_FREQUENCY_8kHz			1
#define PCM9211_FREQUENCY_11kHz			2
#define PCM9211_FREQUENCY_12kHz			3
#define PCM9211_FREQUENCY_16kHz			4
#define PCM9211_FREQUENCY_22kHz			5
#define PCM9211_FREQUENCY_24kHz			6
#define PCM9211_FREQUENCY_32kHz			7
#define PCM9211_FREQUENCY_44kHz			8
#define PCM9211_FREQUENCY_48kHz			9
#define PCM9211_FREQUENCY_64kHz			10
#define PCM9211_FREQUENCY_88kHz			11
#define PCM9211_FREQUENCY_96kHz			12
#define PCM9211_FREQUENCY_128kHz		13
#define PCM9211_FREQUENCY_176kHz		14
#define PCM9211_FREQUENCY_192kHz		15

#define PCM9211_INT0_OERROR_MASK		0x80
#define PCM9211_INT0_ONPCM_MASK 		0x40
#define PCM9211_INT0_OEMPHF_MASK	    0x20
#define PCM9211_INT0_ODTSCD_MASK	    0x10
#define PCM9211_INT0_OCSRNW_MASK	    0x08
#define PCM9211_INT0_OPCRNW_MASK	    0x04
#define PCM9211_INT0_OFSCHG_MASK	    0x02

#define PCM9211_INT1_OERROR_MASK		0x80
#define PCM9211_INT1_ONPCM_MASK 		0x40
#define PCM9211_INT1_OEMPHF_MASK	    0x20
#define PCM9211_INT1_ODTSCD_MASK	    0x10
#define PCM9211_INT1_OCSRNW_MASK	    0x08
#define PCM9211_INT1_OPCRNW_MASK	    0x04
#define PCM9211_INT1_OFSCHG_MASK	    0x02
#define PCM9211_INT1_OADLVL_MASK	    0x01

#endif /* INC_PCM9211_REGISTERS_H_ */
