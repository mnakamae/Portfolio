/*
 * delay.c
 *
 *  Created on: Apr 19, 2020
 *      Author: nakamae
 */
#include "msp.h"
#include "delay.h"

#define DELAY100MS 300000u // DELAY * 1 mS assuming 3 MHz clock
#define PORT_MCLK_OUT (P4)
#define BITM_MCLK_OUT (BIT3)

#define DELAY100MS 300000u // DELAY * 1 mS assuming 3 MHz clock
#define PORT_MCLK_OUT (P4)
#define BITM_MCLK_OUT (BIT3)

void set_DCO(int val)
{
    /* Setting up Master CLK based on freq_sel */
    CS->KEY = CS_KEY_VAL; // unlocks CS
    CS->CTL0 = val;
    CS->CTL1 = 0b011;
    CS->KEY = 0; // locks CS

}

void delay_us(int us)
{
    int count, freq = CS->CTL0;

    /* tuned for 40ms */
    if (freq == FREQ_1_5MHz)
    {
        count = us / 7;
    }
    /* tuned for 40us */
    else if (freq == FREQ_3MHz)
    {
            count = us / 6;
    }

    /* tuned for 40ms */
    else if (freq == FREQ_6MHz)
    {
                count = (us * 10) / 18;
    }

    /* tuned for 40us */
    else if (freq == FREQ_12MHz)
    {
                count = us;
    }
    /* tuned for 40ms */
    else if (freq == FREQ_24MHz)
    {
                count = (us * 100) / 46;
    }
    else if (freq == FREQ_48MHz)
    {
                count = (us * 48) / 10;
    }
    while( count > 0)
    {
        __delay_cycles(1);
        count-=1;
    }
}
