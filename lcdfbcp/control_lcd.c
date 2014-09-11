/*
  ST7735 LCD Controller driver

  Yasuhiro ISHII ishii.yasuhiro@gmail.com

  This software is distributed under the license of Apache 2.0.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "gpiolib.h"
#include "defgpio.h"

void lcd_command_write(int fd,unsigned char c);
void lcd_data_write(int fd,unsigned char d);
void lcd_writeData(int fd,unsigned char* buff,int len);
void LCD_fillColor(int fd,unsigned short color);
void LCD_updateDisplay(int fd,unsigned char* buff);
void lcd_controllerInitial(int fd);

#define SPI_TRANSFER_UNIT	(1024)

void lcd_writeData(int fd,unsigned char* buff,int len)
{
#if 1
  int i;
  int write_size;

  gpiolib_setGpioData(GPIO_LCD_RS,true);

  if(len <= SPI_TRANSFER_UNIT){
    write(fd,buff,len);
  } else {
    for(i=0;i<len;i+=SPI_TRANSFER_UNIT){
      write_size = len - i;
      write_size = write_size > SPI_TRANSFER_UNIT ? SPI_TRANSFER_UNIT : write_size;
      write(fd,(buff+i),write_size);
    }
  }
#else
  int i;
  for(i=0;i<len;i++){
    lcd_data_write(fd,*(buff+i));
  }
#endif
}

void  lcd_command_write(int fd,unsigned char c)
{
  gpiolib_setGpioData(GPIO_LCD_RS,false);
  write(fd,&c,1);
}

void  lcd_data_write(int fd,unsigned char d)
{
  gpiolib_setGpioData(GPIO_LCD_RS,true);
  write(fd,&d,1);
}

int LCD_initial(void)
{
    int fd;
    int result;
    uint8_t mode = SPI_MODE_1;
    uint8_t bits = 8;
    uint32_t speed = 15000000; // on the spec,SCLK max freq=15.152[MHz]
    //uint32_t speed =    100000; // for debug(transferring slowly to check LCD updates
    uint16_t delay;

    gpiolib_setGpioDirection(GPIO_LCD_RS,false);
    gpiolib_setGpioData(GPIO_LCD_RS,false);

    gpiolib_setGpioDirection(GPIO_LCD_RESET,false);
    gpiolib_setGpioData(GPIO_LCD_RESET,false);
    usleep(100*1000);
    gpiolib_setGpioData(GPIO_LCD_RESET,true);
    usleep(100*1000);


    fd = open("/dev/spidev0.0",O_RDWR);
    if(fd == -1){
	printf("Device open error\n");
	return -1;
    }

    /*
      set SPI read/write mode
    */
    result = ioctl(fd,SPI_IOC_WR_MODE,&mode);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }
    result = ioctl(fd,SPI_IOC_RD_MODE,&mode);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }

    result = ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }
    result = ioctl(fd,SPI_IOC_RD_BITS_PER_WORD,&bits);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }

    result = ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }
    result = ioctl(fd,SPI_IOC_RD_MAX_SPEED_HZ,&speed);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return -1;
    }

    lcd_controllerInitial(fd);

    return (fd);
}


void lcd_controllerInitial(int fd)
{
  int i;
  const struct lcd_init_data {
    bool cmd;
    unsigned char data;
  } lcd_init_data_table[] = {
    { true,	0xB1 },		// FRMCTR1
    { false,	0x01 },
    { false,	0x2C },
    { false,	0x2D },

    { true,	0xB2 },		// FRMCTR2
    { false,	0x01 },
    { false,	0x2C },
    { false,	0x2D },

    { true,	0xB3 },		// FRMCTR3
    { false,	0x01 },
    { false,	0x2C },
    { false,	0x2D },
    { false,	0x01 },
    { false,	0x2C },
    { false,	0x2D },

    { true,	0xB4 },		// INVCTR
    { false,	0x07 },

    { true,	0xC0 },		// PWCTR1
    { false,	0xA2 },
    { false,	0x02 },
    { false,	0x84 },

    { true,	0xC1 },		// PWCTR2
    { false,	0xC5 },

    { true,	0xC2 },		// PWCTR3
    { false,	0x0A },
    { false,	0x00 },

    { true,	0xC3 },		// PWCTR4
    { false,	0x8A },
    { false,	0x2A },

    { true,	0xC4 },		// PWCTR5
    { false,	0x8A },
    { false,	0xEE },

    { true,	0xC5 },		// VMCTR1
    { false,	0x0E },

    { true,	0x36 },//MX, MY, RGB mode  p61
    //    { false,	0xC8 }, // 1100 8000 my,mx,mv,ml,rgb,mh,0,0
    { false,	0xA8 }, // mv my |  1010 8000 my,mx,mv,ml,rgb,mh,0,0

    { true,	0xe0 },		// GAMCTRP1
    { false,	0x02 },
    { false,	0x1c },
    { false,	0x07 },
    { false,	0x12 },
    { false,	0x37 },
    { false,	0x32 },
    { false,	0x29 },
    { false,	0x2d },
    { false,	0x29 },
    { false,	0x25 },
    { false,	0x2b },
    { false,	0x39 },
    { false,	0x00 },
    { false,	0x01 },
    { false,	0x03 },
    { false,	0x10 },

    { true,	0xe1 },
    { false,	0x03 },
    { false,	0x1d },
    { false,	0x07 },
    { false,	0x06 },
    { false,	0x2e },
    { false,	0x2c },
    { false,	0x29 },
    { false,	0x2d },
    { false,	0x2e },
    { false,	0x2e },
    { false,	0x37 },
    { false,	0x3f },
    { false,	0x00 },
    { false,	0x00 },
    { false,	0x02 },
    { false,	0x10 },

    { true,	0x2a },		// CASET
    { false,	0x00 },
    { false,	0x01 },
    { false,	0x00 },
    { false,	0xA0 },

    { true,	0x2b },		// RASET
    { false,	0x00 },
    { false,	0x02 },
    { false,	0x00 },
    { false,	0x81 },

    { true,	0x3A }, 
    { false,	0x05 },		// 16bit/pixel
    { true,	0x29 },
  };

  lcd_command_write(fd,0x11);
  usleep(120*1000);

  for(i=0;i<sizeof(lcd_init_data_table)/sizeof(lcd_init_data_table[0]);i++){
    if (lcd_init_data_table[i].cmd == true){
      lcd_command_write(fd,lcd_init_data_table[i].data);
    } else {
      lcd_data_write(fd,lcd_init_data_table[i].data);
    }
  }
  usleep(120*1000);
}

void LCD_updateDisplay(int fd,unsigned char* buff)
{
  lcd_command_write(fd,0x2C);
  lcd_data_write(fd,0);
  lcd_writeData(fd,(unsigned char*)buff,128*160*2);
}

void LCD_fillColor(int fd,unsigned short color)
{
  int i;
  unsigned short buff[160*128*2];

  for (i=0;i<sizeof(buff)/sizeof(unsigned short);i++){
    buff[i] = color;
  }
  LCD_updateDisplay(fd,(unsigned char*)buff);
}

