/*
  LCDFBCP FOR RASPBERRY PI

  Author : Yasuhiro ISHII
*/

#ifndef __LCD_H__
#define __LCD_H__

#include <stdbool.h>

class Lcd
{
 public:
  virtual int getLcdWidth() = 0;
  virtual int getLcdHeight() = 0;
  virtual bool fillColor(unsigned char r,unsigned char g,unsigned char b) = 0;
  virtual void updateDisplay(unsigned char* buff) = 0;
};

#endif // __LCD_H__
