



#include "LaunchPad.h"
#include "clock.h"
#include "adc.h"
#include "clock.h"
#include "spi.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"


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


void lcd_reset(void);
void spi1_write(uint8_t data);
void spi1_write16(uint16_t data);


void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void ili9341_init(void);



/* ===============================
   LCD PIN INITIALIZATION
   ================================ */

void lcd_pins_init(void)
{

    IOMUX->SECCFG.PINCM[LP_SPI_CS0_IOMUX] =
        (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM23_PF_GPIOB_DIO06);

    GPIOB->DOE31_0 |= LP_SPI_CS0_MASK;

    
    // PB6 = CS
    IOMUX->SECCFG.PINCM[6] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<6);
    GPIOB->DOUTSET31_0 = (1<<6);

    // PB0 = DC
    IOMUX->SECCFG.PINCM[0] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<0);
    GPIOB->DOUTSET31_0 = (1<<0);

    // PB1 = RESET
    IOMUX->SECCFG.PINCM[1] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<1);
    GPIOB->DOUTSET31_0 = (1<<1);
}
/* ================================
   CONTROL PIN MACROS
   ================================ */

#define CS_LOW()    (GPIOB->DOUTCLR31_0 = (1<<0))
#define CS_HIGH()   (GPIOB->DOUTSET31_0 = (1<<0))

#define DC_LOW()    (GPIOB->DOUTCLR31_0 = (1<<6))
#define DC_HIGH()   (GPIOB->DOUTSET31_0 = (1<<6))

#define RESET_LOW()  (GPIOB->DOUTCLR31_0 = (1<<1))
#define RESET_HIGH() (GPIOB->DOUTSET31_0 = (1<<1))

/* ================================
   HARDWARE RESET
   ================================ */

void lcd_reset(void)
{
    RESET_LOW();
    msec_delay(20);
    RESET_HIGH();
    msec_delay(150);
}

/* ================================
   SPI WRITE BYTE
   ================================ */

void spi1_write(uint8_t data)
{
    while (!(SPI1->STAT & (1<<1)));   // TX ready
    
    SPI1->TXDATA = data;

    while (SPI1->STAT & (1<<6));      // Wait while busy
}

void spi1_write16(uint16_t data)
{
    while (!(SPI1->STAT & (1<<1)));   // TX ready
    
    SPI1->TXDATA = data;

    while (SPI1->STAT & (1<<6));      // Wait while busy
}



/* ================================
   SEND COMMAND
   ================================ */

void lcd_cmd(uint8_t cmd)
{
    CS_LOW();
    DC_LOW();
    spi1_write(cmd);
    CS_HIGH();
}

/* ================================
   SEND DATA
   ================================ */

void lcd_data(uint8_t data)
{
    CS_LOW();
    DC_HIGH();
    spi1_write(data);
    CS_HIGH();
}

/* ================================
   ILI9341 INITIALIZATION
   (Minimal Tetris Version)
   ================================ */

void ili9341_init(void)
{
    lcd_reset();

    lcd_cmd(ILI9341_SWRESET);
    msec_delay(150);

    lcd_cmd(ILI9341_SLPOUT);
    msec_delay(150);

    lcd_cmd(ILI9341_COLMOD);
    lcd_data(0x55);      // 16-bit RGB565
    msec_delay(10);

    lcd_cmd(ILI9341_MADCTL);
    lcd_data(MADCTL_MX | MADCTL_BGR);   // Landscape + BGR

    lcd_cmd(ILI9341_DISPON);
    msec_delay(100);
}

 