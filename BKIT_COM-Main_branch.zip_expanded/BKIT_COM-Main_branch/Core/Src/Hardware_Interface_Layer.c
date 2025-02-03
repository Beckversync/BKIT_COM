/*
 * Hardware_Interface_Layer.c
 *
 *  Created on: Jan 31, 2025
 *      Author: ASUS
 */
#include "Hardware_Interface_Layer.h"

uint8_t i2c_en = 0, spi_en = 0, uart_en = 0, RS485_en = 0;
BKIT_COM_type_t Com;
enum PROTOCOL Com_Send_Signal;

void hw_init(void) {
	if (i2c_en) {
		Com.i2c->i2c->Init.ClockSpeed = 100000;
		Com.i2c->i2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
		Com.i2c->i2c->Init.OwnAddress1 = 0;
		Com.i2c->i2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		Com.i2c->i2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		Com.i2c->i2c->Init.OwnAddress2 = 0;
		Com.i2c->i2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		Com.i2c->i2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if (HAL_I2C_Init(Com.i2c->i2c) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (spi_en) {
		SPI_HandleTypeDef hspi1;
		hspi1.Instance = Com.spi->spi_x;
		hspi1.Init.Mode = SPI_MODE_MASTER;
		hspi1.Init.Direction = SPI_DIRECTION_2LINES;
		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
		hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
		hspi1.Init.NSS = SPI_NSS_SOFT;
		hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
		hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
		hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		hspi1.Init.CRCPolynomial = 10;
		if (HAL_SPI_Init(&hspi1) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (uart_en) {
		UART_HandleTypeDef huart2;
		huart2.Instance = Com.uart->uart_x;
		huart2.Init.BaudRate = Com.uart->baudrate;
		huart2.Init.WordLength = UART_WORDLENGTH_8B;
		huart2.Init.StopBits = UART_STOPBITS_1;
		huart2.Init.Parity = UART_PARITY_NONE;
		huart2.Init.Mode = UART_MODE_TX_RX;
		huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart2.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&huart2) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (RS485_en) {
		UART_HandleTypeDef huart3;
		huart3.Instance = Com.RS485->uart_x;
		huart3.Init.BaudRate = Com.RS485->baudrate;
		huart3.Init.WordLength = UART_WORDLENGTH_8B;
		huart3.Init.StopBits = UART_STOPBITS_1;
		huart3.Init.Parity = UART_PARITY_NONE;
		huart3.Init.Mode = UART_MODE_TX_RX;
		huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart3.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&huart3) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

enum error_code i2c_config (uint8_t i2c_x, uint32_t address) {
	switch (i2c_x) {
		case 1:
			Com.i2c->i2c->Instance = I2C1;
			break;
		case 2:
			Com.i2c->i2c->Instance = I2C2;
			break;
		case 3:
			Com.i2c->i2c->Instance = I2C3;
			break;
		default:
			return BKITCOM_INVALID;
	}
	Com.i2c->address = address;
	i2c_en = 1;
	return BKITCOM_SUCCESS;
}

enum error_code spi_config (uint8_t spi_x, uint16_t CPOL, uint16_t CPHA) {
	switch (spi_x) {
		case 1:
			Com.spi->spi_x = SPI1;
			break;
		case 2:
			Com.spi->spi_x = SPI2;
			break;
		case 3:
			Com.spi->spi_x = SPI3;
			break;
		default:
			return BKITCOM_INVALID;
	}
	Com.spi->CPOL = CPOL;
	Com.spi->CPHA = CPHA;
	spi_en = 1;
	return BKITCOM_SUCCESS;
}

enum error_code uart_config (uint8_t uart_x, uint32_t baudrate) {
	switch (uart_x) {
		case 1:
			Com.uart->uart_x = USART1;
			break;
		case 2:
			Com.uart->uart_x = USART2;
			break;
		case 3:
			Com.uart->uart_x = USART3;
			break;
		default:
			return BKITCOM_INVALID;
	}
	Com.uart->baudrate = baudrate;
	uart_en = 1;
	return BKITCOM_SUCCESS;
}

enum error_code RS485_config (uint8_t uart_x, uint32_t baudrate, uint32_t address) {
	switch (uart_x) {
	case 1:
		Com.RS485->uart_x = USART1;
		break;
	case 2:
		Com.RS485->uart_x = USART2;
		break;
	case 3:
		Com.RS485->uart_x = USART3;
		break;
	default:
		return BKITCOM_INVALID;
	}
	Com.RS485->baudrate = baudrate;
	Com.RS485->address = address;
	RS485_en = 1;
	return BKITCOM_SUCCESS;
}

enum error_code hw_send(uint8_t * data, uint32_t data_length) {
	switch (Com_Send_Signal) {
	case UART:
		if (uart_en == 0)
			goto PROTOCOL_NOT_INIT;
		HAL_UART_Transmit(huart, pData, Size, Timeout)
		break;
	case I2C:
		if (i2c_en == 0)
			goto PROTOCOL_NOT_INIT;
		break;
	case SPI:
		if (spi_en == 0)
			goto PROTOCOL_NOT_INIT;
		break;
	case RS485:
		break;
	default:
		PROTOCOL_NOT_INIT:
		return BKITCOM_PROTOCOL_NOT_INIT;
	}
	return BKITCOM_SUCCESS;
}
enum error_code hw_send_byte(uint8_t data);
enum error_code hw_receive(uint8_t * data);
