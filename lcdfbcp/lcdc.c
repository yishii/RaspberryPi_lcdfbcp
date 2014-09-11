/*
  lcdfbcp module main

  Yasuhiro ISHII ishii.yasuhiro@gmail.com

  This software is distributed under the license of Apache 2.0.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "gpiolib.h"
#include "control_lcd.h"
#include "fbcp.h"
#include "lcd.h"
#include "lcd/st7735.h"


void lcdc_main();
void st7735_init();

Lcd* lcd;

static void fbcp_callback(unsigned char* buff)
{
  lcd->updateDisplay(buff);
}

struct LcdDevice {
  char* device_name_str;
  void (*init_func)(void);
} object_table[] = {
  { (char*)"st7735" , st7735_init }
};

void st7735_init()
{
  LCD_ST7735* l = new LCD_ST7735;
  lcd = l;
}


int main(int argc,char** argv)
{
  int opt;
  int i;
  bool device_specified = false;

  while ((opt = getopt(argc, argv, "m:")) != -1) {
    switch (opt) {
    case 'm':
      for(i=0;i<sizeof(object_table)/sizeof(object_table[0]);i++){
	if(strcmp(object_table[i].device_name_str,optarg) == 0){
	  (*(object_table[i].init_func))();
	  printf("Selected device : [%s]\n",optarg);
	  device_specified = true;
	}
      }
      break;
    default:
      fprintf(stderr, "Usage: %s -m [device_name]\n",
	      argv[0]);
      exit(-1);
    }
  }

  if(device_specified == false){
    printf("Selected device : [st7735]\n");
    st7735_init();
  }

  lcdc_main();
}

void lcdc_main()
{
  gpiolib_init();
  
  FBCP_process(fbcp_callback,lcd->getLcdWidth(),lcd->getLcdHeight());

  delete lcd;
}

