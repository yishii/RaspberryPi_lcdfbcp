/*

  ST7735 control module

  Author : Yasuhiro ISHII

 */

#ifndef __ST7735_H__
#define __ST7735_H__

#include "lcd.h"

class LCD_ST7735 : public Lcd
{
 public:
  LCD_ST7735();
  ~LCD_ST7735();
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

#endif /* __ST7735_H__ */
