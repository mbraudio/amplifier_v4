/*
 * wm8742x_registers.h
 *
 *  Created on: 9. pro 2021.
 *      Author: Admin
 */
#ifndef INC_WM8742X_REGISTERS_H_
#define INC_WM8742X_REGISTERS_H_

// REGISTERS
#define WM874X_REG_00_DACLLSB_ATTENUATION		0x00
#define WM874X_REG_01_DACLMSB_ATTENUATION		0x01
#define WM874X_REG_02_DACRLSB_ATTENUATION		0x02
#define WM874X_REG_03_DACRMSB_ATTENUATION		0x03
#define WM874X_REG_04_VOLUME_CONTROL			0x04
#define WM874X_REG_05_FORMAT_CONTROL			0x05
#define WM874X_REG_06_FILTER_CONTROL			0x06
#define WM874X_REG_07_MODE_CONTROL_1			0x07
#define WM874X_REG_08_MODE_CONTROL_2			0x08
#define WM874X_REG_09_SOFTWARE_RESET			0x09
#define WM874X_REG_20_ADDITIONAL_CONTROL_1		0x20



// REGISTER 6
typedef enum {
	FilterResponse1 = 0,
	FilterResponse2 = 1,
	FilterResponse3 = 2,
	FilterResponse4 = 3,
	FilterResponse5 = 4
} WMFilter;

typedef enum {
	DsdResponse1 = 0,
	DsdResponse2 = 1,
	DsdResponse3 = 2,
	DsdResponse4 = 3
} WMDSDFilter;

typedef enum {
	DeEmphOff = 0,
	DeEmph32kHz = 1,
	DeEmph44_1kHz = 2,
	DeEmph48kHz = 3
} WMDeEmphasis;

typedef enum {
	ZNormal = 0,
	ZLogic1 = 1
} WMZFlag;

typedef	union {
	uint8_t				value;
	struct {
		WMFilter		filter:3;
		WMDSDFilter 	dsd:2;
		WMDeEmphasis 	deemphasis:2;
		WMZFlag			zflag:1;
	};
} WM874X_REGISTER_6;


// REGISTER 7
typedef enum {
	ModePCM = 0,
	ModeDSDDirect = 1,
	ModeDSDPlus = 2
} WMMode;

typedef enum {
	SamplingRatioAuto = 0,
	SamplingRatio128fs = 1,
	SamplingRatio192fs = 2,
	SamplingRatio256fs = 3,
	SamplingRatio384fs = 4,
	SamplingRatio512fs = 5,
	SamplingRatio768fs = 6
} WMSamplingRatio;

typedef enum {
	SampleRateLow48kHz = 0,
	SampleRateMedium96Khz = 1,
	SampleRateHigh192kHz = 2
} WMSampleRate;

typedef enum {
	Mode8XDisabled = 0,
	Mode8XEnabled = 1
} WMMode8X;

typedef	union {
	uint8_t				value;
	struct {
		WMMode			mode:2;
		WMSamplingRatio samplingRatio:3;
		WMSampleRate 	sampleRate:2;
		WMMode8X		mode8x:1;
	};
} WM874X_REGISTER_7;

#endif /* INC_WM8742X_REGISTERS_H_ */
