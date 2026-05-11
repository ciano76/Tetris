//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Oreste RUiz
//
//      FILE NAME:  menu.c
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "LaunchPad.h"
#include "clock.h"
#include "lcd1602.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);




typedef enum
{
  HOME,
  TEMP,
  HUMIDITY
} menu_state;


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_mstr_init();
  lcd1602_init();


  // == INTERUPTS ==
  config_pb1_interrupt();
  config_pb2_interrupt();

  start_menu();

  // == CLEANUP ==
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);



} /* main */


int start_menu(void)
{

  menu_state current_state = HOME;
  bool done = false;

  lcd_clear();
  lcd_write_string("Press PB2 to Cycle Menu.");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB1 to Select.")
  while(!g_pb2_pressed){}
  g_pb2_pressed = false;

  while(!done)
  {
    lcd_clear();

    switch(current_state)
    {

      case HOME:
        // == LCD ==
        lcd_write_string("Welcome to the Greenhouse.")

        // == INTERRUPT ==
        if(g_pb2_pressed)
        {
          current_state = TEMP;
          g_pb2_pressed = false;
          msec_delay(10);
        }/*if*/

        break;
      case TEMP;
        // == LCD ==
        lcd_write_string("1. VIEW TEMPERATURE");

        // == INTERRUPTS ==
        if(g_pb1_pressed)
        {
          lcd_clear();
          lcd_write_string("Current Temperature")
          lcd_set_ddram_addr(LCD_LINE2_ADDR);
          lcd_write_string("Value: ");
        }
        
        if(g_pb2_pressed)
        {
          current_state = HUMIDITY;
          g_pb2_pressed = false;
          g_pb1_pressed = false;
          msec_delay(10);
        }
        break;
        
    } /* switch */
  } /* while */


} /* start_menu() */







//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Sets up an interrupt for when Push Button 1 is pressed
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
} /* config_pb1_interrupt */

//-----------------------------------------------------------------------------
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
} /* config_pb2_interrupt */

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