#include "msp.h"
#include "uart.h"

/* reads in a character from the UART cable buffer */
uint8_t uart_getChar(void)
{
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)) { }
    return EUSCI_A0->RXBUF;
}

/* sends a character to the UART cable buffer */
void uart_transmitChar(uint8_t c)
{
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)) { }
    EUSCI_A0->TXBUF = c;
}

void UART0_init(void)
{
    EUSCI_A0->CTLW0 |= EUSCI_B_CTLW0_SWRST;    /* put in reset mode for config */
    EUSCI_A0->MCTLW &= ~EUSCI_A_MCTLW_OS16;        /* disable oversampling */
    EUSCI_A0->CTLW0 =  EUSCI_B_CTLW0_UCSSEL_2|EUSCI_B_CTLW0_SWRST;   /* 1 stop bit, no parity, SMCLK, 8-bit data */
    EUSCI_A0->BRW = 26;         /* 3,000,000 / 115200 = 26 */
    P1->SEL0 |= (BIT2|BIT3);           /* P1.3, P1.2 for UART */
    P1->SEL1 &= ~(BIT2|BIT3);
    /* take UART out of reset mode */
    EUSCI_A0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;  }

void scale(uint16_t *data, uint8_t tmp)
{
    (*data) *= 10;
    (*data) += tmp;
}
