/*
 * reader.c
 *
 *  Created on: Jun 7, 2020
 *      Author: nakamae
 */
#include "msp.h"
#include "delay.h"
#include "reader.h"
// direction of the magnetic field
uint8_t direction=0;
// the second track look up table
uint8_t SECOND[16]= { 0b00001,/*        0, (0H)    Data*/
                                          0b10000, /*       1 (1H) */
                                          0b01000,/*        2 (2H)      " */
                                          0b11001, /*       3 (3H)      "*/
                                          0b00100, /*       4 (4H)      "*/
                                          0b10101, /*       5 (5H)      "*/
                                          0b01101, /*       6 (6H)      "*/
                                          0b11100, /*       7 (7H)      "*/
                                          0b00010, /*       8 (8H)      "*/
                                          0b10011, /*       9 (9H)      "*/
                                          0b01011, /*       : (AH)    Control*/
                                          0b11010, /*       ; (BH)    Start Sentinel*/
                                          0b00111, /*       < (CH)    Control*/
                                          0b10110, /*       = (DH)    Field Separator*/
                                          0b01110, /*       > (EH)    Control*/
                                          0b11111  /*       ? (FH)    End Sentinel*/
                                         };
// the first track look up table
uint8_t TABLE[64] = { 0b0000001, /*  space (0H) */
                                        0b1000000, /*      ! (1H) */
                                        0b0100000, /*       (2H) */
                                        0b1100001, /*      # (3H) */
                                        0b0010000, /*      $ (4H) */
                                        0b1010001, /*      % (5H)  SS*/
                                        0b0110001, /*      & (6H) */
                                        0b1110000, /*      ' (7H) */
                                        0b0001000, /*      ( (8H) */
                                        0b1001001, /*      ) (9H) */
                                        0b0101001, /*      * (AH) */
                                        0b1101000, /*      + (BH) */
                                        0b0011001, /*      , (CH) */
                                        0b1011000, /*      - (DH) */
                                        0b0111000, /*      . (EH) */
                                        0b1111001, /*      / (FH) */
                                        0b0000100, /*      0 (10H) */
                                        0b1000101, /*      1 (11H) */
                                        0b0100101, /*      2 (12H) */
                                        0b1100100, /*      3 (13H) */
                                        0b0010101, /*      4 (14H) */
                                        0b1010100, /*      5 (15H) */
                                        0b0110100, /*      6 (16H) */
                                        0b1110101, /*      7 (17H) */
                                        0b0001101, /*      8 (18H) */
                                        0b1001100, /*      9 (19H) */
                                        0b0101100, /*      : (1AH) */
                                        0b1101101, /*      ; (1BH) */
                                        0b0011100, /*      < (1CH) */
                                        0b1011101, /*      = (1DH) */
                                        0b0111101, /*      > (1EH) */
                                        0b1111100, /*      ? (1FH) ES*/
                                        0b0000010, /*      @ (20H) */
                                        0b1000011, /*      A (21H) */
                                        0b0100011, /*      B (22H) */
                                        0b1100010, /*      C (23H) */
                                        0b0010011, /*      D (24H) */
                                        0b1010010, /*      E (25H) */
                                        0b0110010, /*      F (26H) */
                                        0b1110011, /*      G (27H) */
                                        0b0001011, /*      H (28H) */
                                        0b1001010, /*      I (29H) */
                                        0b0101010, /*      J (2AH) */
                                        0b1101011, /*      K (2BH) */                            
                                        0b0011010, /*      L (2CH) */
                                        0b1011011, /*      M (2DH) */
                                        0b0111011, /*      N (2EH) */
                                        0b1111010, /*      O (2FH) */
                                        0b0000111, /*      P (30H) */
                                        0b1000110, /*      Q (31H) */
                                        0b0100110, /*      R (32H) */
                                        0b1100111, /*      S (33H) */
                                        0b0010110, /*      T (34H) */
                                        0b1010111, /*      U (35H) */
                                        0b0110111, /*      V (36H) */
                                        0b1110110, /*      W (37H) */
                                        0b0001110, /*      X (38H) */
                                        0b1001111, /*      Y (39H) */
                                        0b0101111, /*      Z (3AH) */
                                        0b1101110, /*      [ (3BH) */
                                        0b0011111, /*      \ (3DH) */
                                        0b1011110, /*      ] (3EH) */
                                        0b0111110, /*      ^ (3FH) */
                                        0b1111111  /*      _ (40H) */ 
                                       };
// outputs a specified character in the magstripe reader format for the first track
void outputFunc(uint8_t character)
{

    int i;
    for (i = 0b1000000; i > 0b0000000; i = i >> 1)
    {
        if (character & i)
        {
            send(1); 
        }
        else
        {
            send(0);
        }
    }
}

// sends a change in magnetic field to the output pins
void send(uint8_t bit)
{
    direction ^= 1; // flip direction
    if (direction)
    {
        P3->OUT &= ~(BIT6);
        P3->OUT |= BIT7;
    }
    else
    {
        P3->OUT &= ~(BIT7);
        P3->OUT |= BIT6;
    }
    delay_us(200);

    if (bit) // if bit to send is a 1 flip the output bit a second time
    {
        direction ^= 1; // flip direction
        if (direction)
        {
            P3->OUT &= ~(BIT6);
            P3->OUT |= BIT7;
        }
        else
        {
            P3->OUT &= ~(BIT7);
            P3->OUT |= BIT6;
        }
    }
    delay_us(200);
}

// outputs a specified character in the magstripe reader format for second track
void output_sec(uint8_t character)
{

    int i;
    for (i = 0b10000; i > 0b0; i = i >> 1) // send a 1 or 0 based on ascii tables above
    {
        if (character & i)
        {
            send(1);
        }
        else
        {
            send(0);
        }
    }
}
