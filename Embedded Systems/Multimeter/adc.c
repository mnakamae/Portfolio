/*
 * adc.c
 *
 *  Created on: May 18, 2020
 *      Author: nakamae
 */

#include "adc.h"
#include "uart.h"
#include "msp.h"

static unsigned int adc_value = 0;
static unsigned int calibration_shift = 0;
static unsigned int calibration_scale = 5;
static unsigned int adc_history[AVG_LENGTH] = {0, 0, 0, 0};
static unsigned int max_value = 0;
static unsigned int min_value = 16383;

void adc_init()
{
    // GPIO Setup
    P5->SEL1 |= BIT4;                 // Configure P5.4 for ADC
    P5->SEL0 |= BIT4;

    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_3;     // Use sampling timer, 14-bit conversion

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1; // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;        // Enable ADC conv complete interrupt
    ADC14->CTL0 |= ADC14_CTL0_ENC;        // Enable conversions

    // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
}

void adc_record(){
    adc_log_reading();

    ADC14->CTL0 |= ADC14_CTL0_SC; //start taking samples
}

void adc_report_avg()
{
    unsigned int avg_val  = adc_get_avg();
    uart_write_volts(avg_val);
}

inline void adc_curr_val(unsigned int val)
{
    adc_value = val;
}

void adc_log_reading()
{
    int i;
    for(i = 0; i < AVG_LENGTH - 1; i++)
    {
        adc_history[i + 1] = adc_history[i];// shift old values up one
    }
    adc_history[0] = adc_value; // store the most recent value
}

unsigned int adc_get_avg()
{
    int i;
    unsigned int sum = 0;
    for(i = 0; i < AVG_LENGTH; i++)
    {
        sum += adc_history[i];
    }
    return sum / AVG_LENGTH;
}

void uart_write_volts(unsigned int val_mv)
{
    //got the scaling factor from max val 16383/3.3 which is the max voltage
    uint32_t meh = val_mv; //in order to get the eight resolution some more bits were needed
    uart_write_int(val_mv / 4964);//output the volts
    uart_transmitChar('.');
    uart_write_int((val_mv / 496) % 10 );//output decivolts
    uart_write_int((meh * 100 /4964) % 10 );//output centivolts
    uart_write_int((meh * 1000 /4964) % 10); //output milivolts
    uart_transmitChar('V');
    uart_transmit_nl();
}
