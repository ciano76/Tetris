
//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "clock.h"
#include "LaunchPad.h"

void main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();

  

  ili9341_init()

  // Endless loop to prevent program from ending
  while (1);

} /* main */

