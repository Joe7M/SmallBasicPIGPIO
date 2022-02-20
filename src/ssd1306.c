
/******************************************************************************
 SSD1306 OLED plugin for SmallBASIC                                           *
                                                                              *
 Graphic implementation: Adafruit Industries SSD1306 OLED                     *
 driver and graphics library.                                                 *
                                                                              *
 The plugin is for I2C mode only.                                             *
                                                                              *
 https://github.com/Joe7M/SmallBasicPIGPIO                                    *
 License: MIT                                                                 *
*******************************************************************************/

#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <pigpio.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include <sched.h>

#include "gpio.h"
#include "ssd1306.h"

#define SSD1306_swap(a, b) { int8_t t = a; a = b; b = t; }

int ssd1306 = 0;
uint8_t PosX = 0, PosY = 0, TextSize = 1;
int TextWrap = true;
uint8_t SSD1306_LCDWIDTH = 128;
uint8_t SSD1306_LCDHEIGHT = 64;
char SSD1306_I2C_ADDRESS = 0x3C;
uint8_t InvertDisplay = false;

void SSD1306_Command(char command);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color = true);
void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color = true);
void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color = true);
void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color = true);
void SSD1306_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color = true);
void SSD1306_DrawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t color);
void SSD1306_FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color = true);
void SSD1306_FillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, int8_t delta, uint8_t color = true);
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color = true);
void SSD1306_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color = true);
void SSD1306_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color = true);
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color = true);
void SSD1306_FillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color = true);
void SSD1306_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size, uint8_t color = true);
void SSD1306_TextSize(uint8_t t_size);
void SSD1306_GotoXY(uint8_t x, uint8_t y);
void SSD1306_Print(uint8_t c, uint8_t color = true);


const char font[] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46
};

const char font2[] = {
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

static char SSD1306_CommandGraphicsBuffer[128 * 64 / 8 + 1] = {
0x40, //To send the whole buffer with pigpio in one go, the first byte is the ssd1306-command 'send data'
// the rest of the buffer is the graphics buffer
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0xe0,
0xe0, 0xe0, 0xc0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0xe0,
0xe0, 0xe0, 0xe0, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x67, 0x67, 0x67, 0x66, 0x66, 0x7e, 0x7e, 0x7e, 0x3c, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x06, 0x06, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x66, 0x66, 0x7f, 0x7f, 0x7f, 0x39, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x06,
0x06, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x63, 0x67, 0x67, 0x67, 0x66, 0x66, 0x7e, 0x7e, 0x7e, 0x3c, 0x00,
0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x3f, 0x7f, 0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38,
0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38,
0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0xc6,
0xc6, 0x38, 0x38, 0xc6, 0xc6, 0x38, 0x38, 0x00, 0xfe, 0x22, 0x22, 0x22, 0x1c, 0x00, 0xfa, 0x00, 0x7c, 0x82,
0x92, 0x92, 0x74, 0x00, 0xfe, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x82, 0xfe, 0x82, 0x00, 0x7c, 0x82, 0x82, 0x82,
0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x08, 0xe8, 0xe8, 0xe8, 0x08, 0xf8, 0x00, 0x50, 0x30, 0x38, 0xa0,
0x00, 0x48, 0x70, 0xd0, 0xd8, 0x00, 0xf8, 0x08, 0xe8, 0xe8, 0xe8, 0x08, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xab, 0x8a, 0x4a, 0x8a, 0xda, 0x12, 0xab, 0x08, 0x2b, 0xed,
0x07, 0x10, 0xf3, 0x98, 0xae, 0x21, 0x67, 0x88, 0xc3, 0xda, 0x82, 0x4a, 0x82, 0xea, 0x23, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef, 0x20, 0xa1, 0xa9, 0xaf, 0x20, 0xea, 0x0f,
0xbf, 0x28, 0x52, 0x95, 0xcd, 0xa1, 0x8a, 0x96, 0xfb, 0x8f, 0xae, 0x8b, 0xfc, 0xc8, 0x9a, 0x22, 0x22, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x08, 0x0b, 0x0b, 0x0b, 0x08,
0x0f, 0x00, 0x0f, 0x05, 0x0e, 0x05, 0x04, 0x08, 0x0c, 0x0d, 0x07, 0x01, 0x05, 0x0e, 0x05, 0x0a, 0x0b, 0x09,
0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

char *SSD1306_GraphicsBuffer = &SSD1306_CommandGraphicsBuffer[1];

void SSD1306_Command(char command)
{
    char input_buffer[2];

    input_buffer[0] = 0x00;   // Co = 0, D/C = 0 -> Next Byte is a command
    input_buffer[1] = command;

    i2cWriteDevice(ssd1306, input_buffer, 2);
}

int BASIC_CMD_SSD1306_Open(int argc, slib_par_t *params, var_t *retval)
{

    SSD1306_I2C_ADDRESS = get_param_int(argc, params, 0, 0x3C);
    SSD1306_LCDWIDTH = get_param_int(argc, params, 1, 128);
    SSD1306_LCDHEIGHT = get_param_int(argc, params, 2, 64);

    if(SSD1306_LCDWIDTH != 128 || (SSD1306_LCDHEIGHT != 64 && SSD1306_LCDHEIGHT != 32))
    {
        v_setstr(retval, "SSD1306: Only OLED with 128x64 or 128x32 is supported");
        return 0;
    }

    ssd1306 = i2cOpen(1, SSD1306_I2C_ADDRESS, 0);

    // Init sequence according to specsheet

    // Display off
    SSD1306_Command(0xAE);

    // Set Multiplex Ratio
    SSD1306_Command(0xA8);
    SSD1306_Command(SSD1306_LCDHEIGHT - 1);

    // Display Offset
    SSD1306_Command(0xD3);
    SSD1306_Command(0x0);

    // Set Display Clock Divide Ratio / Oscillator Frequency
    // suggested ratio 0x80
    SSD1306_Command(0xD5);
    SSD1306_Command(0x80);

    // Display Start Line -> 0
    SSD1306_Command(0x40);

    // Set Segment Re-map: column address 127 is mapped to SEG0
    SSD1306_Command(0xA1);

    // Set COM Output Scan Direction: remapped mode. Scan from COM[N-1] to COM0
    SSD1306_Command(0xC8);

    //  Set COM Pins Hardware Configuration
    //  Alternative COM pin configuration + Disable COM Left/Right remap
    SSD1306_Command(0xDA);
    SSD1306_Command(0x12);

    // Set Contrast Control (Brightness)
    // 0 to 255
    SSD1306_Command(0x81);
    SSD1306_Command(200);

    // Entire Display ON: A4h command enable display outputs according to the GDDRAM contents
    SSD1306_Command(0xA4);

    // Set Normal Display: This command sets the display to be either normal or inverse.
    // In normal display a RAM data of 1 indicates an  “ON” pixel while in inverse display
    // a RAM data of 0 indicates an “ON” pixel
    SSD1306_Command(0xA6);

    // Charge Pump Setting
    // Enable Charge Pump
    SSD1306_Command(0x8D);
    SSD1306_Command(0x14);

    // Memory Addressing Mode
    // Horizontal Addressing Mode
    SSD1306_Command(0x20);
    SSD1306_Command(0x00);

    // turn on display
    SSD1306_Command(0xAF);

    return(1);

}

int BASIC_CMD_SSD1306_Close(int argc, slib_par_t *params, var_t *retval)
{
    SSD1306_Command(0xAE);    // Display off

    i2cClose(ssd1306);

    return(1);
}


int BASIC_CMD_SSD1306_Display(int argc, slib_par_t *params, var_t *retval)
{
    //dump screen
    //for(uint32_t ii = 0 ; ii < SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8; ii++)
    //    printf("0x%02x, ", SSD1306_GraphicsBuffer[ii]);

    i2cWriteDevice(ssd1306, SSD1306_CommandGraphicsBuffer, SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8 + 1);
    return(1);
}


int BASIC_CMD_SSD1306_ClearDisplay(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t color = get_param_int(argc, params, 0, 0);

    if(color > 0) color = 0xFF;

    for (uint16_t i = 0; i < (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT / 8); i++)
    {
        SSD1306_GraphicsBuffer[i] = color;
    }

    SSD1306_GotoXY(0, 0);

    return(1);
}

int BASIC_CMD_SSD1306_SetBrightness(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t Brightness = get_param_int(argc, params, 0, 255);

    SSD1306_Command(0x81);
    SSD1306_Command(Brightness);

    return(1);
}


int BASIC_CMD_SSD1306_InvertDisplay(int argc, slib_par_t *params, var_t *retval)
{
    InvertDisplay = !InvertDisplay;

    if(InvertDisplay)
        SSD1306_Command(0xA7);    //inverted display
    else
        SSD1306_Command(0xA6);     //normal display

    return(1);
}



int BASIC_CMD_SSD1306_Pset(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x = get_param_int(argc, params, 0, 0);
    uint8_t y = get_param_int(argc, params, 1, 0);
    uint8_t color = get_param_int(argc, params, 2, 1);

    SSD1306_DrawPixel(x, y, color);

    return(1);
}


int BASIC_CMD_SSD1306_Line(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x1 = get_param_int(argc, params, 0, 0);
    uint8_t y1 = get_param_int(argc, params, 1, 0);
    uint8_t x2 = get_param_int(argc, params, 2, 0);
    uint8_t y2 = get_param_int(argc, params, 3, 0);
    uint8_t color = get_param_int(argc, params, 4, 1);

    SSD1306_DrawLine(x1, y1, x2, y2, color);

    return(1);
}

int BASIC_CMD_SSD1306_Rect(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x1 = get_param_int(argc, params, 0, 0);
    uint8_t y1 = get_param_int(argc, params, 1, 0);
    uint8_t x2 = get_param_int(argc, params, 2, 0);
    uint8_t y2 = get_param_int(argc, params, 3, 0);
    uint8_t color = get_param_int(argc, params, 4, 1);
    uint8_t filled = get_param_int(argc, params, 5, 0);

    if(filled)
        SSD1306_FillRect(x1, y1, x2 - x1, y2 - y1, color);
    else
        SSD1306_DrawRect(x1, y1, x2 - x1, y2 - y1, color);

    return(1);
}

int BASIC_CMD_SSD1306_Circle(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x = get_param_int(argc, params, 0, 0);
    uint8_t y = get_param_int(argc, params, 1, 0);
    uint8_t r = get_param_int(argc, params, 2, 0);
    uint8_t color = get_param_int(argc, params, 3, 1);
    uint8_t filled = get_param_int(argc, params, 4, 0);

    if(filled)
        SSD1306_FillCircle(x, y, r, color);
    else
        SSD1306_DrawCircle(x, y, r, color);

    return(1);
}

int BASIC_CMD_SSD1306_RoundRect(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x1 = get_param_int(argc, params, 0, 0);
    uint8_t y1 = get_param_int(argc, params, 1, 0);
    uint8_t x2 = get_param_int(argc, params, 2, 0);
    uint8_t y2 = get_param_int(argc, params, 3, 0);
    uint8_t r = get_param_int(argc, params, 4, 3);
    uint8_t color = get_param_int(argc, params, 5, 1);
    uint8_t filled = get_param_int(argc, params, 6, 0);

    if(filled)
        SSD1306_FillRoundRect(x1, y1, x2 - x1, y2 - y1, r, color);
    else
        SSD1306_DrawRoundRect(x1, y1, x2 - x1, y2 - y1, r, color);

    return(1);
}

int BASIC_CMD_SSD1306_Triangle(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x1 = get_param_int(argc, params, 0, 0);
    uint8_t y1 = get_param_int(argc, params, 1, 0);
    uint8_t x2 = get_param_int(argc, params, 2, 0);
    uint8_t y2 = get_param_int(argc, params, 3, 0);
    uint8_t x3 = get_param_int(argc, params, 4, 0);
    uint8_t y3 = get_param_int(argc, params, 5, 0);
    uint8_t color = get_param_int(argc, params, 6, 1);
    uint8_t filled = get_param_int(argc, params, 7, 0);

    if(filled)
        SSD1306_FillTriangle(x1, y1, x2, y2, x3, y3, color);
    else
        SSD1306_DrawTriangle(x1, y1, x2, y2, x3, y3, color);

    return(1);
}

int BASIC_CMD_SSD1306_Print(int argc, slib_par_t *params, var_t *retval)
{
    const char *text = get_param_str(argc, params, 0, NULL);
    char *text1 = new char[strlen(text) + 2];
    strcpy(text1,text);
    strcat(text1,"\n");

    uint8_t color = get_param_int(argc, params, 1, 1);

    SSD1306_DrawText(PosX, PosY, text1, TextSize, color);

    return(1);
}

int BASIC_CMD_SSD1306_SetTextSize(int argc, slib_par_t *params, var_t *retval)
{
    TextSize = get_param_int(argc, params, 0, 8) / 8;

    if(TextSize = 0) TextSize = 1;
    if(TextSize > 8) TextSize = 8;

    return(1);
}

int BASIC_CMD_SSD1306_At(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x = get_param_int(argc, params, 0, 0);
    uint8_t y = get_param_int(argc, params, 1, 0);

    SSD1306_GotoXY(x, y);

    return(1);
}



int BASIC_CMD_SSD1306_CopyToDisplay(int argc, slib_par_t *params, var_t *retval)
{

    if(!is_param_array(argc, params, 0))
    {
        v_setstr(retval, "SSD1306: BitBlt requires an array");
        return 0;
    }

    var_p_t array = params[0].var_p;  //Get array

    if(v_maxdim(array) != 2)
    {
        v_setstr(retval, "SSD1306: BitBlt requires an 2D-array");
        return 0;
    }

    uint32_t SizeX = v_ubound(array, 0) - v_lbound(array, 0) + 1;
    uint32_t SizeY = v_ubound(array, 1) - v_lbound(array, 1) + 1;

    if(SizeX > SSD1306_LCDWIDTH || SizeY > SSD1306_LCDHEIGHT)
    {
        v_setstr(retval, "SSD1306: BitBlt requires an 2D-array with maximal dimensions of the OLED.");
        return 0;
    }

    uint8_t PosX = get_param_int(argc, params, 1, 0);  //Get pos x
    uint8_t PosY = get_param_int(argc, params, 2, 0);  //Get pos y

    uint8_t DrawSizeX = SizeX;
    uint8_t DrawSizeY = SizeY;

    if(SSD1306_LCDWIDTH - PosX < SizeX) DrawSizeX = SSD1306_LCDWIDTH - PosX;
    if(SSD1306_LCDHEIGHT - PosY < SizeY) DrawSizeY = SSD1306_LCDHEIGHT - PosY;

    uint8_t BitBltMode = get_param_int(argc, params, 3, 0);  //Get mode

    //printf("Elements: %d\n",v_asize(array));
    //printf("SizeX: %d\n", SizeX);
    //printf("SizeY: %d\n", SizeY);
    //printf("DrawSizeX: %d\n", DrawSizeX);
    //printf("DrawSizeY: %d\n", DrawSizeY);

    switch(BitBltMode)
    {
        case 1: //Black is transparent; Value = 0 -> transparent; Value > 0 -> white

            for(uint32_t xx = 0; xx < DrawSizeX; xx++)
            {
                for(uint32_t yy = 0; yy < DrawSizeY; yy++)
                {

                    if(get_array_elem_num(array, xx * SizeY + yy))
                    {
                        SSD1306_GraphicsBuffer[xx + PosX + (uint16_t)((yy +  PosY)/ 8) * SSD1306_LCDWIDTH] |=  (1 << ((yy + PosY) & 7));
                    }
                }
            }

            break;

        case 2: //White is transparent; Value = 0 -> black; Value > 0 -> transparent

            for(uint32_t xx = 0; xx < DrawSizeX; xx++)
            {
                for(uint32_t yy = 0; yy < DrawSizeY; yy++)
                {

                    if(!get_array_elem_num(array, xx * SizeY + yy))
                    {
                        SSD1306_GraphicsBuffer[xx + PosX + (uint16_t)((yy +  PosY)/ 8) * SSD1306_LCDWIDTH] &=  ~(1 << ((yy + PosY) & 7));
                    }
                }

            }

            break;

        case 3: // Value > 1 and Value < 0xFF are transparent
                // Value = 0 -> black; Value = 0xFF -> white

            for(uint32_t xx = 0; xx < DrawSizeX; xx++)
            {
                for(uint32_t yy = 0; yy < DrawSizeY; yy++)
                {

                    if(get_array_elem_num(array, xx * SizeY + yy) == 0) // Black
                        SSD1306_GraphicsBuffer[xx + PosX + (uint16_t)((yy +  PosY)/ 8) * SSD1306_LCDWIDTH] &=  ~(1 << ((yy + PosY) & 7));
                    else if(get_array_elem_num(array, xx * SizeY + yy) == 0xFF) // White
                        SSD1306_GraphicsBuffer[xx + PosX + (uint16_t)((yy +  PosY)/ 8) * SSD1306_LCDWIDTH] |=  (1 << ((yy + PosY) & 7));
                }

            }

            break;

        default: //No transparancy; Value = 0 -> black; Value > 0 -> white
            for(uint32_t xx = 0; xx < DrawSizeX; xx++)
            {
                for(uint32_t yy = 0; yy < DrawSizeY; yy++)
                {
                    SSD1306_DrawPixel(xx + PosX, yy + PosY, get_array_elem_num(array, xx * SizeY + yy));
                }

            }
    }

    return(1);
}

int BASIC_CMD_SSD1306_CopyFromDisplay(int argc, slib_par_t *params, var_t *retval)
{
    uint8_t x = get_param_int(argc, params, 0, 0);
    uint8_t y = get_param_int(argc, params, 1, 0);
    uint8_t w = get_param_int(argc, params, 2, SSD1306_LCDWIDTH - 1);
    uint8_t h = get_param_int(argc, params, 3, SSD1306_LCDHEIGHT - 1);

    if(x >= SSD1306_LCDWIDTH || y >= SSD1306_LCDHEIGHT) return(1);
    if(x + w > SSD1306_LCDWIDTH) w = SSD1306_LCDWIDTH - x;
    if(y + h > SSD1306_LCDHEIGHT) h = SSD1306_LCDHEIGHT - y;

    v_tomatrix(retval, w, h);

    uint32_t ii = 0;
    for(uint8_t xx = x; xx < x + w; xx++)
    {
        for(uint8_t yy = y; yy < y + h; yy++)
        {

            v_setreal(v_elem(retval, ii),  255*((SSD1306_GraphicsBuffer[xx + (uint16_t)(yy / 8) * SSD1306_LCDWIDTH] & (1 << (yy & 7))) >> (yy & 7)) );
            ii++;
        }
    }


    return(1);
}



void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT)) return;

    if(color)
    {
        SSD1306_GraphicsBuffer[x + (uint16_t)(y / 8) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
    }
    else
    {
        SSD1306_GraphicsBuffer[x + (uint16_t)(y / 8) * SSD1306_LCDWIDTH] &=  ~(1 << (y & 7));
    }
}


void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    int8_t steep;
    int8_t ystep;
    uint8_t dx, dy;
    int8_t err;

    steep = abs(y1 - y0) > abs(x1 - x0);

    if(steep)
    {
        SSD1306_swap(x0, y0);
        SSD1306_swap(x1, y1);
    }

    if(x0 > x1)
    {
        SSD1306_swap(x0, x1);
        SSD1306_swap(y0, y1);
    }

    dx = x1 - x0;
    dy = abs(y1 - y0);
    err = dx / 2;

    if(y0 < y1)
        ystep = 1;
    else
        ystep = -1;

    for(; x0 <= x1; x0++)
    {
        if (steep)
        {
            if(color)
                SSD1306_DrawPixel(y0, x0, true);
            else
                SSD1306_DrawPixel(y0, x0, false);
        }
        else
        {
            if(color)
                SSD1306_DrawPixel(x0, y0,true);
            else
                SSD1306_DrawPixel(x0, y0, false);
        }

        err -= dy;
        if(err < 0)
        {
            y0  += ystep;
            err += dx;
        }
    }
}

void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
    SSD1306_DrawLine(x, y, x + w - 1, y, color);
}

void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{
    SSD1306_DrawLine(x, y, x, y + h - 1, color);
}

void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
    for (int8_t i = x; i < x + w; i++)
    {
        SSD1306_DrawFastVLine(i, y, h, color);
    }
}


void SSD1306_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
    int8_t f = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x = 0;
    int8_t y = r;

    SSD1306_DrawPixel(x0  , y0 + r, color);
    SSD1306_DrawPixel(x0  , y0 - r, color);
    SSD1306_DrawPixel(x0 + r, y0, color);
    SSD1306_DrawPixel(x0 - r, y0, color);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(x0 + x, y0 + y, color);
        SSD1306_DrawPixel(x0 - x, y0 + y, color);
        SSD1306_DrawPixel(x0 + x, y0 - y, color);
        SSD1306_DrawPixel(x0 - x, y0 - y, color);
        SSD1306_DrawPixel(x0 + y, y0 + x, color);
        SSD1306_DrawPixel(x0 - y, y0 + x, color);
        SSD1306_DrawPixel(x0 + y, y0 - x, color);
        SSD1306_DrawPixel(x0 - y, y0 - x, color);
    }

}

void SSD1306_DrawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t color)
{
    int8_t f     = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x     = 0;
    int8_t y     = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4)
        {
            SSD1306_DrawPixel(x0 + x, y0 + y, color);
            SSD1306_DrawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2)
        {
            SSD1306_DrawPixel(x0 + x, y0 - y, color);
            SSD1306_DrawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8)
        {
            SSD1306_DrawPixel(x0 - y, y0 + x, color);
            SSD1306_DrawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1)
        {
            SSD1306_DrawPixel(x0 - y, y0 - x, color);
            SSD1306_DrawPixel(x0 - x, y0 - y, color);
        }
    }

}

void SSD1306_FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
    SSD1306_DrawFastVLine(x0, y0 - r, 2 * r + 1, color);
    SSD1306_FillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void SSD1306_FillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, int8_t delta, uint8_t color)
{
    int8_t f     = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x     = 0;
    int8_t y     = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x01)
        {
            SSD1306_DrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            SSD1306_DrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x02)
        {
            SSD1306_DrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            SSD1306_DrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }

}

// Draw a rectangle
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
    SSD1306_DrawFastHLine(x, y, w, color);
    SSD1306_DrawFastHLine(x, y + h - 1, w, color);
    SSD1306_DrawFastVLine(x, y, h, color);
    SSD1306_DrawFastVLine(x + w - 1, y, h, color);
}

// Draw a rounded rectangle
void SSD1306_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    // smarter version
    SSD1306_DrawFastHLine(x + r, y, w - 2 * r, color); // Top
    SSD1306_DrawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    SSD1306_DrawFastVLine(x, y + r, h - 2 * r, color); // Left
    SSD1306_DrawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    SSD1306_DrawCircleHelper(x + r, y + r, r, 1, color);
    SSD1306_DrawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    SSD1306_DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    SSD1306_DrawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void SSD1306_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color )
{
    // smarter version
    SSD1306_FillRect(x + r, y, w - 2 * r, h, color);
    // draw four corners
    SSD1306_FillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    SSD1306_FillCircleHelper(x + r        , y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    SSD1306_DrawLine(x0, y0, x1, y1, color);
    SSD1306_DrawLine(x1, y1, x2, y2, color);
    SSD1306_DrawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void SSD1306_FillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    int8_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1)
    {
        SSD1306_swap(y0, y1); SSD1306_swap(x0, x1);
    }
    if (y1 > y2)
    {
        SSD1306_swap(y2, y1); SSD1306_swap(x2, x1);
    }
    if (y0 > y1)
    {
        SSD1306_swap(y0, y1); SSD1306_swap(x0, x1);
    }

    if(y0 == y2)
    { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        SSD1306_DrawFastHLine(a, y0, b - a + 1, color);
        return;
    }

    int8_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t  sa   = 0, sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1 - 1; // Skip it

    for(y = y0; y <= last; y++)
    {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) SSD1306_swap(a, b);

        SSD1306_DrawFastHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y <= y2; y++)
    {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) SSD1306_swap(a, b);

        SSD1306_DrawFastHLine(a, y, b - a + 1, color);
    }
}

void SSD1306_DrawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t size)
{
    SSD1306_GotoXY(x, y);
    SSD1306_TextSize(size);
    SSD1306_Print(c);
}

void SSD1306_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size, uint8_t color)
{
    SSD1306_GotoXY(x, y);
    SSD1306_TextSize(size);

    while(*_text != '\0')
        SSD1306_Print(*_text++, color);

}

// move cursor to position (x, y)
void SSD1306_GotoXY(uint8_t x, uint8_t y)
{
    if((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
        return;
    PosX = x;
    PosY = y;
}

// set text size
void SSD1306_TextSize(uint8_t t_size)
{
    if(t_size < 1)
        t_size = 1;
    TextSize = t_size;
}

/* print single char
    \a  Set cursor position to upper left (0, 0)
    \b  Move back one position
    \n  Go to start of current line
    \r  Go to line below
*/
void SSD1306_Print(uint8_t c, uint8_t color)
{
    int _color;
    uint8_t i, j, line;

    if (c == ' ' && PosX == 0 && TextWrap)
        return;

    if(c == '\a')
    {
        PosX = PosY = 0;
        return;
    }

    if( (c == '\b') && (PosX >= TextSize * 6) )
    {
        PosX -= TextSize * 6;
        return;
    }

    if(c == '\r')
    {
        PosX = 0;
        return;
    }

    if(c == '\n')
    {
        PosY += TextSize * 8;
        if((PosY + TextSize * 7) > SSD1306_LCDHEIGHT)
        PosY = 0;
        PosX = 0;
        return;
    }

    if((c < ' ') || (c > '~'))
        c = '?';

    for(i = 0; i < 5; i++ )
    {
        if(c < 'S')
            line = font[(c - ' ') * 5 + i];
        else
            line = font2[(c - 'S') * 5 + i];

        for(j = 0; j < 7; j++, line >>= 1)
        {
            if(line & 0x01)
                _color = !!color;
            else
                _color = !color;
            if(TextSize == 1) SSD1306_DrawPixel(PosX + i, PosY + j, _color);
            else               SSD1306_FillRect(PosX + (i * TextSize), PosY + (j * TextSize), TextSize, TextSize, _color);
        }
    }

    SSD1306_FillRect(PosX + (5 * TextSize), PosY, TextSize, 7 * TextSize, !color);

    PosX += TextSize * 6;

    if(PosX > (SSD1306_LCDWIDTH + TextSize * 6))
        PosX = SSD1306_LCDWIDTH;

    if (TextWrap && (PosX + (TextSize * 5)) > SSD1306_LCDWIDTH)
    {
        PosX = 0;
        PosY += TextSize * 8;
        if((PosY + TextSize * 7) > SSD1306_LCDHEIGHT)
        PosY = 0;
    }
}




