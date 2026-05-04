



#include "LaunchPad.h"
#include "clock.h"
#include "adc.h"
#include "clock.h"
#include "spi.h"


// ILI9341 command codes
#define ILI9341_SWRESET   0x01
#define ILI9341_SLPOUT    0x11
#define ILI9341_DISPON    0x29
#define ILI9341_CASET     0x2A
#define ILI9341_PASET     0x2B
#define ILI9341_RAMWR     0x2C
#define ILI9341_MADCTL    0x36
#define ILI9341_COLMOD    0x3A
#define ILI9341_VSCRDEF   0x33
#define ILI9341_VSCRSADD  0x37
#define ILI9341_INVON     0x21
#define ILI9341_INVOFF    0x20

// MADCTL bits
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_BGR 0x08


/* ================================
   LCD PIN INITIALIZATION
   ================================ */

void lcd_pins_init(void);


/* ================================
   CONTROL PIN MACROS
   ================================ */

#define CS_LOW()    (GPIOB->DOUTCLR31_0 = (1<<0))
#define CS_HIGH()   (GPIOB->DOUTSET31_0 = (1<<0))

#define DC_LOW()    (GPIOB->DOUTCLR31_0 = (1<<6))
#define DC_HIGH()   (GPIOB->DOUTSET31_0 = (1<<6))

#define RESET_LOW()  (GPIOB->DOUTCLR31_0 = (1<<1))
#define RESET_HIGH() (GPIOB->DOUTSET31_0 = (1<<1))


void lcd_reset(void);
void spi1_write(uint8_t data);
void spi1_write16(uint16_t data);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void ili9341_init(void);

