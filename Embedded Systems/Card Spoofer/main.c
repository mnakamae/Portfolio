#include "msp.h"
#include "uart.h"
#include <string.h>
#include "reader.h"
#include "delay.h"
#include "stdio.h"
#include "keypad.h"

#define SPOOF 2
#define READ 1
#define HOLD 0

uint8_t state = HOLD;

// sets the state based on the keypressed
void readin(uint8_t key)
{
    switch(key)
    {
    case 1:
        state = READ;
        break;
    case 2:
        state = SPOOF;
        break;
    default:
        state = HOLD;
        break;
    }
}

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    char card[512] = "%2015037409328^STUDENT?;6278561807527737?"; // buffer to hold 
                                                                                                                                 // card info
    P3->DIR |= (BIT6 | BIT7); // Configure pin 3.6 and 3.7 as output
    P3->SEL0 &= ~(BIT6|BIT7); 
    P3->SEL1 &= ~(BIT6 |BIT7);
    P3->OUT = 0; // Set Pin 3.6 and 3.7 low
    UART0_init();  // initialize the UART
    keypad_init(); // initialize the keypad
    int zero_ind = 0, ind = 0, first_track = 0, card_track = 0;
    uint8_t lrc = 0, press;
    char tmp;
    while(1)
    {
        press = keypad_Getkey(); // get keypress
        if (press !=0xFF)
        {
            readin(press);
            delay_ms(750);
        }

        if (state == READ) // read the new card from the UART buffer
        {
            tmp = uart_getChar();
            while((tmp != 0xD ))
            {
                card[ind] = tmp;
                tmp = uart_getChar();
                ind++;
            }
            card[ind] = '\0';
            ind = 0;
            state = HOLD;
        }
        else if(state==SPOOF)
        {
            // write first track
            for (zero_ind  = 0; zero_ind < 62; zero_ind ++)
            {
                send(0); // pad with zeros
            }
            lrc = 0;
            for (card_track = 0; card[card_track] != ';' && card[card_track] !='\0'; 
                                                                                  card_track++) // end of first track or strlen
            {
                outputFunc(TABLE[(uint8_t) (card[card_track ] - ASCII_OFFSET_AGAIN)]);
                lrc ^= TABLE[(uint8_t) (card[card_track ] - ASCII_OFFSET_AGAIN)];
            }
            outputFunc(lrc);
            for (zero_ind  = 0; zero_ind < 63+35; zero_ind ++)
           {
               send(0); // pad with zeros
           }
           for (zero_ind  = 0; zero_ind < 22; zero_ind ++)
           {
               send(0); // pad with zeros
           }
           lrc = 0;

           // write second track
           for (; card[card_track ] !='\0'; card_track ++) // i < strlen(card)
           {
               output_sec(SECOND[(uint8_t) (card[card_track ] - 48)]);
               lrc ^= SECOND[(uint8_t) (card[card_track ] - 48)];
           }
           output_sec(lrc);
           for (zero_ind   = 0; zero_ind  < 61; zero_ind ++) 
           {
               send(0); // pad with zeros
           }
           lrc = 0;
       }
       else
       {
           continue;
       }
    }
}
