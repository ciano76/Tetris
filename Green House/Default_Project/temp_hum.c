//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//   DESIGNERS NAME:  Oreste & Marciano
//        FILE NAME:  temp_hum.c  
//*****************************************************************************
//*****************************************************************************

// == STANARD C INCLUDE FILES ==
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// ==  LOADS MSP LAUNCHPAD BOARD SUPPORT MACROS AND DEFINITIONS == 
#include "LaunchPad.h"
#include "adc.h"
#include "clock.h"
#include "lcd1602.h"
#include <ti/devices/msp/msp.h>

// == FUNCTION PROTOTYPES == 
void loop(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
float convert_to_fahrenheit(float celcius);

// == SYMBOLIC CONSTANTS ==
#define TEMP_CHANNEL 5
#define PROGRAM_END_DELAY  1000
#define BUTTON_DELAY 20

// == GLOBAL VARIABLES ==
bool g_pb1_pressed = false;
bool g_pb2_pressed = false;


int main(void)
{
  // == CONFIGURE ==
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  I2C_mstr_init();
  lcd1602_init();

  // == INTERRUPTS ==
  config_pb1_interrupt();
  config_pb2_interrupt();
  
  // == LOOP ==
  //  - TODO: add humidity on line 1? 
  //  - display current temp on line 2, updates by pressing PB2
  loop();


  // CLEANUP
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);

  while (1);
} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the temperature from the temperature sensor on
//  line 2 of the LCD. Pressing PB2 updates the temperature. It is displayed in
//  Fahrenheit
// -----------------------------------------------------------------------------
void loop(void)
{

  lcd_clear();

  bool done = false;
  uint32_t temp_raw;
  float temp;

  while (!done)
  {

    ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
    uint32_t pot_raw = ADC0_in(POT_CHANNEL);

    // == DISPLAY TEMPERATURE ==
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Temp = ");
    lcd_write_byte(convert_to_fahrenheit(temp));
    lcd_write_char(0xDF);
    lcd_write_string("F");

    // == UPDATE TEMP IF PB2 PRESSED ==
    if(g_pb2_pressed) 
    {
      temp_raw   = ADC0_in(TEMP_CHANNEL);
      temp = thermistor_calc_temperature(temp_raw);
      g_pb2_pressed = false;
      msec_delay(BUTTON_DELAY);
    }

    // == STOP PROGRAM WHEN PB1 is PRESSED ==
    if (g_pb1_pressed)
    {
      done = true;
      lcd_clear();
      lcd_write_string("Program Stopped.");
      msec_delay(PROGRAM_END_DELAY);
      leds_off();
      g_pb1_pressed = false;
    }
  }
} /* loop */

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function takes in a celcius temperature value as a float and returns 
//    the equivalent fahreneheit value as a float
//
// INPUT PARAMETERS:
//    celcius - temperature in celcius
//
// RETURN:
//    float - temperature converted to celcius
// -----------------------------------------------------------------------------
float convert_to_fahrenheit(float celcius)
{
  if(celcius == 0){
    return (0);
  }
  return (celcius * 1.8) + 32;  
} /* convert_to_fahrenheit */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Sets up an interrupt for when Push Button 1 is pressed
//
// INPUT PARAMETERS:
//    None
//
// RETURN:
//    None
// -----------------------------------------------------------------------------
void config_pb1_interrupt(void)
{
  // Set PB1 to rising edge (after inversion)
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

  // Ensure bit is cleared
  GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

  // Unmask PB1 to allow interrupt
  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_SetPriority(GPIOB_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

// -----------------------------------------------------------------------------
// DESCRIPTION:
//    Sets up an interrupt for when Push Button 2 is pressed
// -----------------------------------------------------------------------------
void config_pb2_interrupt(void)
{
  // Set PB2 to rising edge (after inversion)
  GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

  // Ensure bit is cleared
  GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

  // Unmask PB2 to allow interrupt
  GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

  NVIC_SetPriority(GPIOA_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

void GROUP1_IRQHandler(void)
{
  uint32_t group_gpio_iidx;
  uint32_t gpio_mis;

  do
  {
    group_gpio_iidx = CPUSS->INT_GROUP[1].IIDX;

    switch (group_gpio_iidx)
    {
      case (CPUSS_INT_GROUP_IIDX_STAT_INT0): // GPIOA
        gpio_mis = GPIOA->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) ==
            GPIO_CPU_INT_MIS_DIO15_SET)
        {
          g_pb2_pressed       = true;
          GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
        } /* if */
        break;

      case (CPUSS_INT_GROUP_IIDX_STAT_INT1): // GPIOB
        gpio_mis = GPIOB->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) ==
            GPIO_CPU_INT_MIS_DIO18_SET)
        {
          g_pb1_pressed       = true;
          GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
        } /* if */
        break;
    } /* switch */
  } while (group_gpio_iidx != 0);
} /* GROUP1_IRQHandler */