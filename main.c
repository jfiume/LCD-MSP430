#include <msp430.h>
#include <driverlib.h> // Required for the LCD
#include "myGpio.h" // Required for the LCD
#include "myClocks.h" // Required for the LCD
#include "myLcd.h" // Required for the LCD


const unsigned int BATTERY[] =  //list of battery symbols
{
 LCD_B6,
 LCD_B5,
 LCD_B4,
 LCD_B3,
 LCD_B2,
 LCD_B1
};


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Watchdog timer stop
    // initialize LCD
    initGPIO(); // Initializes inputs and outputs for LCD
    initClocks(); // Initialize clocks for LCD
    myLCD_init(); // Prepares LCD to receive commands

    TA0CTL |= TASSEL_1|MC_1|ID_0; // ACLK, count up to CCR, divide clock by 1
    TA0CCR0 = 0x8000;           // counter limit 0x8000 = 32768 = 1 second

    unsigned int j = 1;             //create a flag for displaying the name
    unsigned int i = 1;             //create an index for the numbers counting
    int bat_index = 0;              //create an index for the battery symbols array
    while(1)                        //loop forever
    {
        if (TA0CTL & TAIFG)   //If flag is set from counting up
        {
            if (j == 1)
            {
                myLCD_showChar( 'J', 1 ); // Display "J" in space 1
                myLCD_showChar( 'O', 2 ); // Display "O" in space 2
                myLCD_showChar( 'S', 3 ); // Display "S" in space 3
                myLCD_showChar( 'E', 4 ); // Display "E" in space 4
                myLCD_showChar( 'P', 5 ); // Display "P" in space 5
                myLCD_showChar( 'H', 6 ); // Display "H" in space 6
                j = 0;      //turn the name display flag off
            }
            else if (i <= 6)     //count from 1 to 6
            {
                myLCD_displayNumber(i);     //display the number on the LCD screen
                i++;                        //increment the number
            }
            else
            {
                myLCD_showChar( ' ', 6 );   //blank the LCD screen
                if (bat_index <= 5)         //allow battery indexes in range of battery symbol list
                {
                    myLCD_showSymbol(LCD_UPDATE, BATTERY[bat_index], 0);    //display the battery symbol at given index
                    bat_index++;        //increment the battery index
                }
                else
                {
                    for (;bat_index >= 0; bat_index--)  //count down from the highest battery index 5-0
                    {
                        myLCD_showSymbol(LCD_CLEAR, BATTERY[bat_index], 0);    //blank the battery symbol at the given index
                    }
                    i = 1;      //set the numbers index back to its staring position
                    j = 1;      //set the name display flag
                }
            }
            TA0CTL &= ~BIT0;    //Reset Flag
        }
    }
}
