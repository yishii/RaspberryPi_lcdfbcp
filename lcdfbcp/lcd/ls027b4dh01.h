/*

  LS027B4DH01 control module

  Author : Yasuhiro ISHII

  LCD Data:
  400x280 1bpp Monochrome LCD
  
 */

#ifndef __LS027B4DH01_H__
#define __LS027B4DH01_H__

#include "lcd.h"

class LCD_LS027B4DH01 : public Lcd
{
 public:
  LCD_LS027B4DH01();
  ~LCD_LS027B4DH01();
  int getLcdWidth();
  int getLcdHeight();
  //int getLcdPixelFormat();
  bool fillColor(unsigned char r,unsigned char g,unsigned char b);
  void updateDisplay(unsigned char* buff);

 private:
  static const int LCD_WIDTH = 400;
  static const int LCD_HEIGHT = 240;
  static const LcdPixelFormat LCD_PIXELFORMAT = PIXELFORMAT_2BPP;
  static const int SPI_TRANSFER_UNIT = 1024;

  int spi_fd;
  bool spi_initial();
  void lcd_command_write(unsigned char c);
  void lcd_data_write(unsigned char d);
  void lcd_writeData(unsigned char* buff,int len);
  void lcd_controllerInitial();
};

#endif /* __LS027B4DH01_H__ */

