/*
  ST7735 LCD Controller driver

  Yasuhiro ISHII ishii.yasuhiro@gmail.com

  This software is distributed under the license of Apache 2.0.
*/

#ifndef __CONTROL_LCD_H__
#define __CONTROL_LCD_H__

int LCD_initial(void);
void LCD_updateDisplay(int fd,unsigned char* buff);
void LCD_fillColor(int fd,unsigned short color);


inline unsigned short LCD_colorConvert(unsigned char r,unsigned char g,unsigned char b)
{
  unsigned short tr,tg,tb;
  tr = r;
  tg = g;
  tb = b;
  // RRRRR GGGGGG BBBBB
  return ((tb) | (tg << 6) | (tr << 11));
}




#endif /* __CONTROL_LCD_H__ */
