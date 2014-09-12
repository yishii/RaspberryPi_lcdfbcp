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
#include "fbcp.h"
#include "lcd.h"
#include "lcd/st7735.h"
#include "lcd/st7735r.h"

void lcdc_main();
void st7735_init();
void st7735r_init();

Lcd* lcd;

static void fbcp_callback(unsigned char* buff)
{
  lcd->updateDisplay(buff);
}

struct LcdDevice {
  char* device_name_str;
  void (*init_func)(void);
} object_table[] = {
  { (char*)"st7735"      , st7735_init    },
  { (char*)"st7735r"     , st7735r_init   },
};

void st7735_init()
{
  LCD_ST7735* l = new LCD_ST7735;
  lcd = l;
}

void st7735r_init()
{
  LCD_ST7735R* l = new LCD_ST7735R;
  lcd = l;
}

int main(int argc,char** argv)
{
  int opt;
  int i;
  bool device_specified = false;

  gpiolib_init();

  while ((opt = getopt(argc, argv, "m:h")) != -1) {
    switch (opt) {
    case 'm':
      for(i=0;i<sizeof(object_table)/sizeof(object_table[0]);i++){
	if(strcmp(object_table[i].device_name_str,optarg) == 0){
	  (*(object_table[i].init_func))();
	  printf("Selected device : [%s]\n",optarg);
	  device_specified = true;
	  break;
	}
      }
      break;
    case 'h':
    default:
      fprintf(stderr, "Usage: %s -m [device_name]\n",argv[0]);
      fprintf(stderr, "Supported device_name\n");
      fprintf(stderr, "  st7735\n");
      fprintf(stderr, "  st7735r\n");
      exit(-1);
    }
  }

  if(device_specified == false){
    printf("Selected default device : [st7735]\n");
    st7735_init();
  }

#if 0
  while(1){
    lcd->fillColor(0xff,00,00);
    sleep(1);
    lcd->fillColor(0x00,00,0xff);
    sleep(1);
  }
#endif

  lcdc_main();
}

void lcdc_main()
{
  
  FBCP_process(fbcp_callback,lcd->getLcdWidth(),lcd->getLcdHeight());

  delete lcd;
}

