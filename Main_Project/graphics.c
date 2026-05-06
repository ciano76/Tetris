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
#include "lcd.h"
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

// Panel size
#define TFT_WIDTH   240
#define TFT_HEIGHT  320


void ili9341_set_rotation(uint8_t r);
void ili9341_invert(bool inv);
void ili9341_scroll(uint16_t y);
void ili9341_scroll_margins(uint16_t top, uint16_t bottom);
void ili9341_set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void ili9341_fill_screen(uint16_t color);
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
static void draw_char(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg);
void ili9341_draw_string(uint16_t x, uint16_t y,
                         const char *s,
                         uint16_t fg, uint16_t bg);
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);


// -----------------------------------------------------------------------------
// Rotation / inversion / scrolling
// -----------------------------------------------------------------------------

void ili9341_set_rotation(uint8_t r)
{
    uint8_t m;

    switch (r & 3) {
        case 0: m = (MADCTL_MX | MADCTL_BGR); break;                       // 0
        case 1: m = (MADCTL_MV | MADCTL_BGR); break;                       // 90
        case 2: m = (MADCTL_MY | MADCTL_BGR); break;                       // 180
        default: m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR); break; // 270
    }

    lcd_cmd(ILI9341_MADCTL);
    lcd_data(m);
}

void ili9341_invert(bool inv)
{
    lcd_cmd(inv ? ILI9341_INVON : ILI9341_INVOFF);
}

void ili9341_scroll(uint16_t y)
{
    lcd_cmd(ILI9341_VSCRSADD);
    lcd_data(y >> 8);
    lcd_data(y & 0xFF);
}

void ili9341_scroll_margins(uint16_t top, uint16_t bottom)
{
    uint16_t mid;

    if (top + bottom > TFT_HEIGHT) {
        return;
    }

    mid = TFT_HEIGHT - (top + bottom);

    lcd_cmd(ILI9341_VSCRDEF);
    lcd_data(top >> 8);
    lcd_data(top & 0xFF);
    lcd_data(mid >> 8);
    lcd_data(mid & 0xFF);
    lcd_data(bottom >> 8);
    lcd_data(bottom & 0xFF);
}

// -----------------------------------------------------------------------------
// Address window and primitive drawing
// -----------------------------------------------------------------------------

void ili9341_set_addr_window(uint16_t x, uint16_t y,
                                    uint16_t w, uint16_t h)
{
    uint16_t x2 = x + w - 1;
    uint16_t y2 = y + h - 1;

    // Column range
    lcd_cmd(ILI9341_CASET);
    lcd_data(x >> 8);
    lcd_data(x & 0xFF);
    lcd_data(x2 >> 8);
    lcd_data(x2 & 0xFF);

    // Row range
    lcd_cmd(ILI9341_PASET);
    lcd_data(y >> 8);
    lcd_data(y & 0xFF);
    lcd_data(y2 >> 8);
    lcd_data(y2 & 0xFF);

    // Prepare for pixel data
    lcd_cmd(ILI9341_RAMWR);
}

void ili9341_fill_screen(uint16_t color)
{
    uint32_t total = (uint32_t)TFT_WIDTH * (uint32_t)TFT_HEIGHT;
    uint32_t i;

    ili9341_set_addr_window(0, 0, TFT_WIDTH, TFT_HEIGHT);

    for (i = 0; i < total; i++) {
        spi1_write16(color);
    }
}

void ili9341_fill_rect(uint16_t x, uint16_t y,
                       uint16_t w, uint16_t h,
                       uint16_t color)
{
    uint32_t count;
    uint32_t i;

    if (x >= TFT_WIDTH || y >= TFT_HEIGHT) {
        return;
    }

    if ((x + w) > TFT_WIDTH) {
        w = TFT_WIDTH - x;
    }
    if ((y + h) > TFT_HEIGHT) {
        h = TFT_HEIGHT - y;
    }

    count = (uint32_t)w * (uint32_t)h;

    ili9341_set_addr_window(x, y, w, h);

    for (i = 0; i < count; i++) {
        spi1_write16(color);
    }
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= TFT_WIDTH || y >= TFT_HEIGHT) {
        return;
    }

    ili9341_set_addr_window(x, y, 1, 1);
    spi1_write16(color);
}

// -----------------------------------------------------------------------------
// Simple 5x7 font (ASCII 32–127) – declaration only; define in graphics_font.c
// -----------------------------------------------------------------------------

extern const uint8_t font5x7[];   // 5 bytes per char, rows top→bottom

static void draw_char(uint16_t x, uint16_t y,
                      char c,
                      uint16_t fg, uint16_t bg)
{
    uint8_t col, row;
    uint8_t line;
    const uint8_t *glyph;

    if (c < 32 || c > 126) {
        c = '?';
    }

    glyph = &font5x7[(c - 32) * 5];

    // 5x7 character in a 6x8 cell (1px spacing)
    ili9341_set_addr_window(x, y, 6, 8);

    for (row = 0; row < 8; row++) {
        for (col = 0; col < 6; col++) {
            if (col == 5 || row == 7) {
                spi1_write16(bg);
            } else {
                line = glyph[col];
                if (line & (1 << row)) {
                    spi1_write16(fg);
                } else {
                    spi1_write16(bg);
                }
            }
        }
    }
}

void ili9341_draw_string(uint16_t x, uint16_t y,
                         const char *s,
                         uint16_t fg, uint16_t bg)
{
    while (*s) {
        if (*s == '\n') {
            y += 8;
            x = 0;
        } else if (*s == '\r') {
            // ignore
        } else {
            draw_char(x, y, *s, fg, bg);
            x += 6;
        }
        s++;
    }
}

// -----------------------------------------------------------------------------
// Color helper (RGB565)
// -----------------------------------------------------------------------------

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((r & 0xF8) << 8) |
                      ((g & 0xFC) << 3) |
                      (b >> 3));
}
