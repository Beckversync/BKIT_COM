/*
 * Hardware_Interface_Layer.h
 *
 *  Created on: Jan 31, 2025
 *      Author: ASUS
 */

#ifndef INC_HARDWARE_INTERFACE_LAYER_H_
#define INC_HARDWARE_INTERFACE_LAYER_H_
#include "main.h"
#include "Error_Code.h"

typedef struct {
	UART_HandleTypeDef * uart;
	uint16_t baudrate;
} UART_type_t;

typedef struct {
	I2C_HandleTypeDef * i2c;
	uint32_t address;
} I2C_type_t;

typedef struct {
	SPI_HandleTypeDef * spi;
	uint16_t CPOL;
	uint16_t CPHA;
} SPI_type_t;

typedef struct {
	UART_HandleTypeDef * uart;
	uint32_t baudrate;
	uint32_t address;
} RS485_type_t;

typedef struct {
	UART_type_t * uart;
	I2C_type_t * i2c;
	SPI_type_t * spi;
	RS485_type_t * RS485;
} BKIT_COM_type_t;

enum PROTOCOL {UART, I2C, SPI, RS485};

extern uint8_t uart_en, i2c_en, spi_en, RS485_en;
extern BKIT_COM_type_t Com;
extern enum PROTOCOL Com_Send_Signal;

void hw_init(void);
enum error_code uart_config(uint8_t uart_x, uint32_t baudrate);
enum error_code i2c_config(uint8_t i2c_x, uint32_t address);
enum error_code spi_config(uint8_t spi_x, uint16_t CPOL, uint16_t CPHA);
enum error_code RS485_config(uint8_t uart_x, uint32_t baudrate, uint32_t address);

enum error_code hw_send(uint8_t * data, uint32_t data_length);
enum error_code hw_send_byte(uint8_t data);
enum error_code hw_receive(uint8_t * data);

#endif /* INC_HARDWARE_INTERFACE_LAYER_H_ */
