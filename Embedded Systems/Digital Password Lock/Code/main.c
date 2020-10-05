//this is the main
#include "msp.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

#define LOCKED      &FSM[0]
#define UNLOCKED    &FSM[1]
#define TIMEOUT     &FSM[2]
#define RESET       &FSM[3]
#define PASSWORD    "1234"
#define SECOND      1000
#define PASSLEN     4

struct State
{
    struct State *Next[3];
};

struct State FSM[4] =
{
 {{LOCKED, UNLOCKED, TIMEOUT}}, //LOCKED state
 {{UNLOCKED, LOCKED, RESET}}, //UNLOCKED state
 {{TIMEOUT, LOCKED,LOCKED}},  //TIMEOUT state
 {{RESET,LOCKED, LOCKED}} //RESET state
};
//this function prints the two strings on the two different lines
void print_screen(char line1[], char line2[])
{
    clear_LCD();
    write_string_LCD(line1);
    LCD_command(0xC0);
    write_string_LCD(line2);
}

//compares the two strings and returns 0 if they are equal, 42 else
int comp(char str1[], char str2[])
{
    int i;
    for (i=0; i<PASSLEN; i++)
    {
        if (str1[i] != str2[i])
        {
            return 42;
        }
    }
    return 0;
}

/**
 * main.c
 *  this is the main of the function and it just consists of a finite state machine
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	char lock1[]= "LOCKED";
	char lock2[]= "ENTER KEY ";
	char time1[]= "STOP YOU'VE BROKEN";
	char time2[]= "THE LAW ";
	char res1[]= "RESET PASSWORD";
	char res2[]= "ENTER PASS: ";
	char ulock1[]= "UNLOCKED";
	char ulock2[]= "# to change PASSWORD ";
	char nums[] = "0123456789*0#";
	char guess[PASSLEN];
	char key[PASSLEN]= PASSWORD;
	char next = 0, flag = 0, tries = 0;;
	char i = 0;
	struct State *ptr= LOCKED ;    //state pointer
              
            LCD_init();
	keypad_Init();
	set_DCO(FREQ_3MHz);

	delay_ms(SECOND);
	while (1)
	{
	    next = 0;
	    if (ptr == LOCKED)
	    {
	        if (flag == 0)
	        {
	            print_screen(lock1,lock2);
	            flag++;
	        }
	        if(i == PASSLEN) //check guess length against password length
	        {
	            i = 0;
	            flag = 0;
	            if(!comp(guess, key))
	            {
	                tries = 0;
	                next = 1;
	            }
	            else  //wrong guess
	            {
	                tries++;
	            }
	            if (tries == 3)
	            {
	                next = 2;
	            }
	        }
	        char in = keypad_Getkey();
	        if (in != 0xFF)
	        {
	            if (nums[in] == '*') // clear the screen
	            {
	                print_screen(lock1,lock2);
	                i = 0;
	            }
	            else if (nums[in] !='#' ) // write to the screen
	            {
	                write_char_LCD(nums[in]);
	                guess[i] = nums[in];
	                i++;
	            }
	        }
	        delay_ms(SECOND);
	    }
	    else if (ptr == UNLOCKED)
	    {
	        if(flag == 0)
	        {
	            print_screen(ulock1, ulock2);
	            flag++;
	        }
	        char in = keypad_Getkey();
	        if (in != 0xFF)
	        {
	            flag = 0;
	            if(nums[in]!='#') // reset pin combination
	            {
	                next = 1;
	            }
	            else
	            {
	                next = 2;
	            }
	        }
	        delay_ms(SECOND);
	    }
	    else if (ptr == RESET)
	    {
	        if(flag==0)
	        {
	            print_screen(res1, res2);
	            flag++;
	        }
	        if(i == PASSLEN)
	        {
	            i = 0;
	            flag = 0;
	            next = 1;
	        }
	        else
	        {
	            char in = keypad_Getkey();
	            if (in != 0xFF)
	            {
	                if (!(nums[in] == '*' || nums[in] == '#')) //accept only valid numbers
	                {
	                    write_char_LCD(nums[in]);
	                    key[i] = nums[in];
	                    i++;
	                }
	            }
	            delay_ms(SECOND);
	        }
	    }

	    else if (ptr == TIMEOUT)
	    {
	        print_screen(time1, time2);
	        delay_ms(10 * SECOND);
	        next = 1;
	        flag = 0;
	        tries = 0;
	    }
	    ptr = ptr->Next[next];
	}
}
