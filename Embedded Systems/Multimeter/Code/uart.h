#ifndef UART_H_
#define UART_H_
#include "stdint.h"

#define ASCII_OFFSET 48
#define MAX_DAC 1023
#define MAX_BUF 5

uint8_t uart_getChar(void);
void uart_transmitChar(uint8_t);
void UART0_init(void);
void scale(uint16_t *, uint8_t);
void uart_transmit_nl();
void uart_write_int(unsigned int num);
