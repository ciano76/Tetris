/* ================================
   LCD PIN INITIALIZATION
   ================================ */

void lcd_pins_init(void)
{
    // --- SPI1 Pins ---
    // PB8 = MOSI
    IOMUX->SECCFG.PINCM[8] = 0x00000021;

    // PB9 = SCK
    IOMUX->SECCFG.PINCM[9] = 0x00000021;

    // PB7 = MISO (not used)
    IOMUX->SECCFG.PINCM[7] = 0x00000021;

    // --- Control Pins ---

    // PB6 = CS
    IOMUX->SECCFG.PINCM[6] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<0);
    GPIOB->DOUTSET31_0 = (1<<0);   // CS HIGH (idle)

    // PB0 = DC
    IOMUX->SECCFG.PINCM[0] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<6);
    GPIOB->DOUTSET31_0 = (1<<6);   // DC HIGH (idle)

    // PB1 = RESET
    IOMUX->SECCFG.PINCM[1] = 0x00000081;
    GPIOB->DOESET31_0 = (1<<1);
    GPIOB->DOUTSET31_0 = (1<<1);   // RESET HIGH (idle)
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

