/*
 * adc.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

#define ADC_NUMBER_OF_CHANNELS	6

typedef struct
{
	uint32_t 	rawData[ADC_NUMBER_OF_CHANNELS];
	uint32_t 	rawDataIndex;
	uint32_t 	dataReady;
	uint8_t 	data[ADC_NUMBER_OF_CHANNELS];
} ADCManager;

extern ADCManager adc;

void ADC_Initialize(void);
void ADC_StoreValue(const uint32_t value);
void ADC_Finalize(void);

#endif /* INC_ADC_H_ */
