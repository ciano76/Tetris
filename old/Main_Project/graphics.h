// graphics.c
// Minimal ILI9341 graphics layer for MSPM0 Tetris
// Assumes these low-level functions/macros exist in your project:
//
//   void lcd_cmd(uint8_t cmd);
//   void lcd_data(uint8_t data);
//   void spi1_write16(uint16_t value);   // blocking 16-bit SPI write (MSB first)
//   void msec_delay(uint32_t ms);
//
// And RESET pin is already handled in your ili9341_init().

#include <stdint.h>
#include <stdbool.h>


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

// Panel size
#define TFT_WIDTH   240
#define TFT_HEIGHT  320

// -----------------------------------------------------------------------------
// Basic ILI9341 init
// -----------------------------------------------------------------------------

void ili9341_init(void);
void ili9341_set_rotation(uint8_t r);
void ili9341_invert(bool inv);
void ili9341_scroll(uint16_t y);
void ili9341_scroll_margins(uint16_t top, uint16_t bottom);


// -----------------------------------------------------------------------------
// Address window and primitive drawing
// -----------------------------------------------------------------------------

void ili9341_set_addr_window(uint16_t x, uint16_t y,uint16_t w, uint16_t h);


void ili9341_fill_screen(uint16_t color);


void ili9341_fill_rect(uint16_t x, uint16_t y,
                       uint16_t w, uint16_t h,
                       uint16_t color);


void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);


// -----------------------------------------------------------------------------
// Simple 5x7 font (ASCII 32–127) – declaration only; define in graphics_font.c
// -----------------------------------------------------------------------------

extern const uint8_t font5x7[];   // 5 bytes per char, rows top→bottom

static void draw_char(uint16_t x, uint16_t y,
                      char c,
                      uint16_t fg, uint16_t bg);


void ili9341_draw_string(uint16_t x, uint16_t y,
                         const char *s,
                         uint16_t fg, uint16_t bg);


// -----------------------------------------------------------------------------
// Color helper (RGB565)
// -----------------------------------------------------------------------------

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
