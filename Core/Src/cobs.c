/*
 * cobs.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Admin
 */
#include "cobs.h"

/*
 * StuffData byte stuffs �length� bytes of
 * data at the location pointed to by �ptr�,
 * writing the output to the location pointed
 * to by �dst�.
 */
uint16_t COBS_Encode(const uint8_t *src, const uint16_t length, uint8_t *dst)
{
	const uint8_t *end = src + length;
	uint8_t *code_ptr = dst++;
	uint8_t code = 0x01;
	uint16_t size = 0;

	while (src < end)
	{
		if (*src == 0)
		{
			*code_ptr = code;
			code_ptr = dst++;
			size++;
			code = 0x01;
		}
		else
		{
			*dst++ = *src;
			size++;
			if (++code == 0xFF)
			{
				*code_ptr = code;
				code_ptr = dst++;
				size++;
				code = 0x01;
			}
		}
		src++;
	}

	*code_ptr = code;
	code_ptr = dst++;
	size++;

	return size;
}

/*
 * UnStuffData decodes �length� bytes of
 * data at the location pointed to by �ptr�,
 * writing the output to the location pointed
 * to by �dst�.
 */

uint16_t COBS_Decode(const uint8_t *src, const uint16_t length, uint8_t *dst)
{
	if (length == 0)
	{
		return 0;
	}

	uint16_t size = 0;
	const uint8_t *end = src + length;
	while (src < end)
	{
		int code = *src++;
		for (int i = 1; i < code; i++)
		{
			*dst++ = *src++;
			size++;
		}
		if ((code < 0xFF) && (src < end))
		{
			*dst++ = 0;
			size++;
		}
	}

	return size;
}



