/*
 * uart.h
 *
 *  Created on: Jan 9, 2022
 *      Author: Admin
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>
#include "stm32l0xx_hal.h"

#define UART_RX_SIZE			1
#define UART_RX_BUFFER_SIZE 	256
#define UART_RX_DATA_SIZE		UART_RX_BUFFER_SIZE

typedef struct
{
	UART_HandleTypeDef* huart;
	uint32_t	rxIndex;
	uint8_t		rxByte[UART_RX_SIZE];
	uint8_t		rxBuffer[UART_RX_BUFFER_SIZE];
	uint8_t		rxData[UART_RX_DATA_SIZE];
	uint8_t		rxDataDecoded[UART_RX_DATA_SIZE];
	uint32_t	rxDataLength;
	uint32_t	rxDataReady;
} UartHandler;


extern UartHandler uartHandler;

void UART_Init(UART_HandleTypeDef* huart);
void UART_Store(void);
void UART_Process(void);
void UART_Send(const uint8_t* data, const uint16_t size);

#endif /* INC_UART_H_ */
