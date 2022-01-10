/*
 * uart.c
 *
 *  Created on: Jan 9, 2022
 *      Author: Admin
 */
#include "uart.h"
#include "cobs.h"
#include "amplifier.h"
#include "bluetooth.h"
#include <string.h>

UartHandler uartHandler;

void UART_PrepareRxData(void);

void UART_Init(UART_HandleTypeDef* huart)
{
	uartHandler.huart = huart;
	uartHandler.rxIndex = 0;
	memset(uartHandler.rxByte, 0, UART_RX_SIZE);
	memset(uartHandler.rxBuffer, 0, UART_RX_BUFFER_SIZE);
	memset(uartHandler.rxData, 0, UART_RX_DATA_SIZE);
	memset(uartHandler.rxDataDecoded, 0, UART_RX_DATA_SIZE);
	uartHandler.rxDataLength = 0;
	uartHandler.rxDataReady = 0;
}

inline void UART_Store(void)
{
	uartHandler.rxBuffer[uartHandler.rxIndex] = uartHandler.rxByte[0];
	uartHandler.rxIndex++;
	if (uartHandler.rxByte[0] == 0)
	{
		UART_PrepareRxData();
	}
}

void UART_PrepareRxData(void)
{
	uartHandler.rxDataLength = uartHandler.rxIndex - 1;
	memcpy(uartHandler.rxData, uartHandler.rxBuffer, uartHandler.rxDataLength);
	uartHandler.rxIndex = 0;
	uartHandler.rxDataReady = 1;
}

void UART_Process(void)
{
	uint16_t size = COBS_Decode(uartHandler.rxData, uartHandler.rxDataLength, uartHandler.rxDataDecoded);
	BLUETOOTH_Process(uartHandler.rxDataDecoded, size);
	uartHandler.rxIndex = 0;
}

void UART_Send(const uint8_t* data, const uint16_t size)
{
	uint16_t length;
	uint8_t encoded[UART_RX_DATA_SIZE];
	length = COBS_Encode(data, size, encoded);
	HAL_UART_Transmit(uartHandler.huart, encoded, length, (length * 20));
	encoded[0] = 0;
	HAL_UART_Transmit(uartHandler.huart, encoded, 1, 20);
}


