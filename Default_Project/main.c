//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code ... *** COMPLETE THIS BASED ON LAB REQUIREMENTS ***
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "graphics.h"
#include "lcd.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Panel size
#define TFT_WIDTH   240
#define TFT_HEIGHT  320

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  spi1_init();

  // enter your code here
  lcd_pins_init();
  ili9341_init();

  ili9341_set_addr_window(0, 0, TFT_WIDTH, TFT_HEIGHT);


  for(int i = 0; i<= 1000; i++){
    spi1_write16(RED);
  }

  

 // Endless loop to prevent program from ending
 while (1);

} /* main */

