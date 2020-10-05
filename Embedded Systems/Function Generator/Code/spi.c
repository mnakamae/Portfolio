/*
 * SPI.c
 *
 *  Created on: May 8, 2020
 *      Author: nakamae
 */
#include "spi.h"

void dac_init(void)
{
    P3-> SEL0 &= ~(SPI_CS_PIN);
    P3-> SEL1 &= ~(SPI_CS_PIN);
    P3 ->DIR |= (SPI_CS_PIN);
    cs_high();
    P1->SEL0 |= BIT5 | BIT6 | BIT7;     // Set P1.5, P1.6, and P1.7 as
                                            // SPI pins functionality

    P2->DIR |= BIT0 | BIT1 | BIT2;      // set as output for LED

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset

    EUSCI_B0->CTLW0 =   EUSCI_B_CTLW0_SWRST  | // keep eUSCI in reset
                        EUSCI_B_CTLW0_MST    | // Set as SPI master
                       // EUSCI_B_CTLW0_CKPH |
                        EUSCI_B_CTLW0_SYNC   | // Set as synchronous mode
                        EUSCI_B_CTLW0_CKPL   | // Set clock polarity high
                        EUSCI_B_CTLW0_SSEL__SMCLK | // SMCLK
                        EUSCI_B_CTLW0_MSB;     // MSB first

    EUSCI_B0->BRW = 0x02;               // no div - fBitClock = fBRCLK/(UCBRx)

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;  // Initialize USCI state machine
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE;          // Enable RX interrupt

}

void cs_high()
{
    P3 -> OUT |= SPI_CS_PIN;
}

void cs_low()
{
    P3 -> OUT &= ~SPI_CS_PIN;
}

void spi_transmit( const uint16_t val)
{
    cs_low();
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));
    EUSCI_B0->TXBUF = (CONTROL_BITS | ((val & 0x3C0) >> 6));
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));
    EUSCI_B0->TXBUF = (val & 0xFF) << 2;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
    cs_high();
}
