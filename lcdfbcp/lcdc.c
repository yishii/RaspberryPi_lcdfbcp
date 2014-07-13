/*
  lcdfbcp module main

  Yasuhiro ISHII ishii.yasuhiro@gmail.com

  This software is distributed under the license of Apache 2.0.
*/

#include <stdio.h>
#include <stdbool.h>
#include "gpiolib.h"
#include "control_lcd.h"
#include "fbcp.h"

static int lcd_fd;

static void fbcp_callback(unsigned char* buff)
{
  LCD_updateDisplay(lcd_fd,buff);
}

int main(int argc,char** argv)
{
  gpiolib_init();
  
  lcd_fd = LCD_initial();

  FBCP_process(fbcp_callback,160,128);

  //
  while(1){
    LCD_fillColor(lcd_fd,LCD_colorConvert(255,255,255));
    usleep(1000*100);
    LCD_fillColor(lcd_fd,LCD_colorConvert(0,0,0));
    usleep(1000*100);
  }
  close(lcd_fd);
}

