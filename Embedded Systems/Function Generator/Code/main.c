#include "msp.h"
#include "keypad.h"
#include "spi.h"
#include "sine.h"
#include "delay.h"

#define SQUARE 0
#define SINE 1
#define SAW 2
#define TANGLE 3

#define sq500 6022   // To get these values, took the DCO speed and divided by the desired 
#define sq400 7529  //wave speed 
#define sq300 10038
#define sq200 15057
#define sq100 30114

#define dut500 602
#define dut400 753
#define dut300 1004
#define dut200 1506
#define dut100 3011

#define sine100 234

#define tri100 116 // tangle wave

volatile static uint8_t state = SQUARE; // output a square wave upon starting up
volatile static uint8_t flag = 1; // when flag is set, output to the SPI
volatile static uint16_t data = 0; // data is the value output to the SPI
volatile static uint16_t dutybase = dut100;
volatile static uint8_t freq = 1; // output 100 hz upon starting up
volatile static uint8_t duty = 5; // output 50% duty cycle upon starting up

// set up the interrupts used for the waves
void timer_init(void)
{
    // setup TIMER_A0
   TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt
   TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
   TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled

   TIMER_A0->CCR[0] = sq100;   // set CCR0 count
   TIMER_A0->CCR[1] = dutybase * duty;
   TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP; // SMCLK, up mode
   NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31)| (1 << ((TA0_N_IRQn) & 31));   // set NVIC interrupt
}

// take in input from keypad to set function generator output
void readin(uint8_t key)
{
    switch(key)
    {
    case 11: // reset duty cycle to 50%
        duty = 5;
        TIMER_A0->CCR[1] = dutybase * duty;
        break;
    case 1: // set freq to 100 hz
        freq = 1;
        if (state == SQUARE)
        {
            TIMER_A0->CCR[0] = sq100;
            dutybase = dut100;
            TIMER_A0->CCR[1] = dutybase * duty ;
        }
        else if (state == TANGLE)
        {
            TIMER_A0->CCR[0] = tri100;
        }




        else
        {
            TIMER_A0->CCR[0] = sine100;
        }
        break;
    case 2: // spiset freq to 200 hz
        freq = 2;
        if (state == SQUARE)
        {
            TIMER_A0->CCR[0] = sq200;
            dutybase = dut200;
            TIMER_A0->CCR[1] = dutybase * duty ;
        }
        else if (state == TANGLE)
        {
            TIMER_A0->CCR[0] = tri100;
        }
        else
        {
            TIMER_A0->CCR[0] = sine100;
        }
        break;
    case 3: // set freq to 300 hz
        freq = 3;
        if (state == SQUARE)
        {
            TIMER_A0->CCR[0] = sq300;
            dutybase = dut300;
            TIMER_A0->CCR[1] = dutybase * duty ;
        }
        else if (state == TANGLE)
        {
            TIMER_A0->CCR[0] = tri100;
        }
        else
        {
            TIMER_A0->CCR[0] = sine100;
        }
        break;
    case 4: // set freq to 400 hz
        freq = 4;
        if (state == SQUARE)
        {
            TIMER_A0->CCR[0] = sq400;
            dutybase = dut400;
            TIMER_A0->CCR[1] = dutybase * duty ;
        }
        else if (state == TANGLE)
        {
            TIMER_A0->CCR[0] = tri100;
        }
        else
        {
            TIMER_A0->CCR[0] = sine100;
        }
        break;
    case 5: // set freq to 500 hz
        freq = 5;
        if (state == SQUARE)
        {
            TIMER_A0->CCR[0] = sq500;
            dutybase = dut500;
            TIMER_A0->CCR[1] = dutybase * duty ;
        }
        else if (state == TANGLE)
        {
            TIMER_A0->CCR[0] = tri100;
        }
        else
        {
            TIMER_A0->CCR[0] = sine100;
        }
        break;
    case 6:
       state = 3;
       set_DCO(FREQ_24MHz);
       TIMER_A0->CCR[0] = tri100;
       TIMER_A0->R = 0;
        break;
    case 7: // output a square wave
        state = 0;
        set_DCO(FREQ_3MHz);
        switch(freq) // change the period and duty cycle
        {
            case 1:
                TIMER_A0->CCR[0] = sq100;
                dutybase = dut100;
                break;
            case 2:
                TIMER_A0->CCR[0] = sq200;
                dutybase = dut200;
                break;
            case 3:
                TIMER_A0->CCR[0] = sq300;
                dutybase = dut300;
                break;
            case 4:
                TIMER_A0->CCR[0] = sq400;
                dutybase = dut400;
                break;
            case 5:
                TIMER_A0->CCR[0] = sq500;
                dutybase = dut500;
                break;
            default:
                break;
        }
        TIMER_A0->CCR[1] = dutybase * duty;
        break;
    case 8: // output a sine wave
        state = 1;
        set_DCO(FREQ_24MHz);
        TIMER_A0->CCR[0] = sine100;
        TIMER_A0->R = 0;
        break;
    case 9: // output a sawtooth wave
        state = 2;
        set_DCO(FREQ_24MHz);
        TIMER_A0->CCR[0] = sine100;
        TIMER_A0->R = 0;
        break;
    case 10: // decrease duty cycle by 10%
        if (duty > 1)
        {
            duty--;
        }
        TIMER_A0->CCR[1] = dutybase * duty ;
        break;
    case 12: // increase duty cycle by 10%
        if (duty < 9)
        {
           duty++;
        }
        TIMER_A0->CCR[1] = dutybase * duty ;
        break;
    default:
        break;
    }
}

void main(void)
{
    uint8_t press;
    set_DCO(FREQ_3MHz);
    dac_init();
    timer_init();
    keypad_init();
    __enable_irq();     // Enable global interrupt
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    while(1)
    {
        if (flag)
        {
            spi_transmit(data);
            flag = 0;
        }
        press = keypad_Getkey();
        if (press !=0xFF)
        {
	readin(press);
	delay_ms(500);// debounce the keypad
        }
    }
}

// handler used to output variable voltages for each wave
void TA0_0_IRQHandler(void) 
{
    volatile static uint16_t ind = 0;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
    if (state == SINE)
    {
        if (ind >= 1023)
        {
            ind = 0;
        }
        data = sin[ind];
        ind += freq;
    }
    else if (state == SAW)
    {
        if (ind >= 1023)
        {
            ind = 0;
        }
        data = sa[ind];
        ind += freq;
    }
    else if (state == TANGLE)
    {
            if (ind >= 2047)
            {
                ind = 0;
            }
            data = tri[ind];
            ind += freq;
     }
    else
    {
        data = 0x3FF;
    }
    flag = 1;
}

// handler used to output a low on a square wave
void TA0_N_IRQHandler(void)
{
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR1 interrupt
    data = 0;
    flag = 1;
}
