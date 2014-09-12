/*

  ST7735R control module

  Author : Yasuhiro ISHII

 */

#ifndef __ST7735R_H__
#define __ST7735R_H__

#include "lcd.h"

class LCD_ST7735R : public Lcd
{
 public:
  LCD_ST7735R();
  ~LCD_ST7735R();
  int getLcdWidth();
  int getLcdHeight();
  bool fillColor(unsigned char r,unsigned char g,unsigned char b);
  void updateDisplay(unsigned char* buff);

 private:
  static const int LCD_WIDTH = 160;
  static const int LCD_HEIGHT = 128;
  static const int SPI_TRANSFER_UNIT = 1024;

  int spi_fd;
  bool use_st7735r;
  bool spi_initial();
  void lcd_command_write(unsigned char c);
  void lcd_data_write(unsigned char d);
  void lcd_writeData(unsigned char* buff,int len);
  void lcd_controllerInitial();
};

#endif /* __ST7735R_H__ */
