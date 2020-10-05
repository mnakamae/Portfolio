#include "msp.h"
#include "uart.h"
#include "adc.h"

/*
 * main.c
 */
void main(void)
{
    volatile uint32_t i, j;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    UART0_init();
    adc_init();
    // Enable global interrupt
    __enable_irq();
    while (1)
    {
        for (j = 20; j > 0; j--)
        {
            for (i = 20000; i > 0; i--);  // Delay
            adc_record();
        }
        adc_report_avg();
    }
}

void ADC14_IRQHandler(void) 
{
    adc_curr_val(ADC14->MEM[0]);
}
