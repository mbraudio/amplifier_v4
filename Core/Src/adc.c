 /*
 * adc.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Admin
 */
#include "adc.h"
#include <string.h>

ADCManager adc;

void ADC_Initialize(void)
{
	adc.rawDataIndex = 0;
	adc.dataReady = 0;
	memset(adc.rawData, 0, (ADC_NUMBER_OF_CHANNELS * 4));
	memset(adc.data, 0, ADC_NUMBER_OF_CHANNELS);
}

inline void ADC_StoreValue(const uint32_t value)
{
	if (adc.rawDataIndex < ADC_NUMBER_OF_CHANNELS)
	{
		adc.rawData[adc.rawDataIndex++] = value;
	}
}

void ADC_Finalize(void)
{
	uint32_t i;
	for (i = 0; i < ADC_NUMBER_OF_CHANNELS; i++)
	{
		adc.data[i] = (uint8_t)adc.rawData[i];
	}
	adc.rawDataIndex = 0;
	adc.dataReady = 1;
}

