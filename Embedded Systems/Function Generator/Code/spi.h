spi.h


/*
 * spi.h
 *
 *  Created on: May 8, 2020
 *      Author: nakamae
 */

#ifndef SPI_H_
#define SPI_H_
#include "msp.h"

#define SPI_CS_PIN    BIT6
#define CONTROL_BITS (0b01110000)
#define DAC_DATA_MASK (0x0FFF)

void dac_init();
void spi_transmit(const uint16_t data);

void cs_low();
void cs_high();

#endif /* SPI_H_ */
