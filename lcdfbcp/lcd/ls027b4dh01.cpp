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
#include "ls027b4dh01.h"

/*
  LCD memo

  resolution : 400x240   400*240 = 96000 bits(12000 bytes)
*/

LCD_LS027B4DH01::LCD_LS027B4DH01()
{
  spi_initial();
  lcd_controllerInitial();
  printf("Initialized as LS027B4DH01\n");
}

LCD_LS027B4DH01::~LCD_LS027B4DH01()
{
  // spi terminate process
}

int LCD_LS027B4DH01::getLcdWidth()
{
  return LCD_WIDTH;
}

int LCD_LS027B4DH01::getLcdHeight()
{
  return LCD_HEIGHT;
}



void LCD_LS027B4DH01::lcd_writeData(unsigned char* buff,int len)
{
#if 1
  int i;
  int write_size;

  gpiolib_setGpioData(GPIO_LCD_RS,true);

  if(len <= SPI_TRANSFER_UNIT){
    write(spi_fd,buff,len);
  } else {
    for(i=0;i<len;i+=SPI_TRANSFER_UNIT){
      write_size = len - i;
      write_size = write_size > SPI_TRANSFER_UNIT ? SPI_TRANSFER_UNIT : write_size;
      write(spi_fd,(buff+i),write_size);
    }
  }
#else
  int i;
  for(i=0;i<len;i++){
    lcd_data_write(spi_fd,*(buff+i));
  }
#endif
}

void LCD_LS027B4DH01::lcd_command_write(unsigned char c)
{
  gpiolib_setGpioData(GPIO_LCD_RS,false);
  write(spi_fd,&c,1);
}

void LCD_LS027B4DH01::lcd_data_write(unsigned char d)
{
  gpiolib_setGpioData(GPIO_LCD_RS,true);
  write(spi_fd,&d,1);
}

bool LCD_LS027B4DH01::spi_initial(void)
{
    int result;
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 2000000; // on the spec,SCLK max freq=2[MHz]
    //uint32_t speed =    100000; // for debug(transferring slowly to check LCD updates
    uint16_t delay;

    /*
    gpiolib_setGpioDirection(GPIO_LCD_RS,false);
    gpiolib_setGpioData(GPIO_LCD_RS,false);

    gpiolib_setGpioDirection(GPIO_LCD_RESET,false);
    gpiolib_setGpioData(GPIO_LCD_RESET,false);
    usleep(100*1000);
    gpiolib_setGpioData(GPIO_LCD_RESET,true);
    usleep(100*1000);
    */

    spi_fd = open("/dev/spidev0.0",O_RDWR);
    if(spi_fd == -1){
	printf("Device open error\n");
	return false;
    }

    /*
      set SPI read/write mode
    */
    result = ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }
    result = ioctl(spi_fd,SPI_IOC_RD_MODE,&mode);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }

    result = ioctl(spi_fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }
    result = ioctl(spi_fd,SPI_IOC_RD_BITS_PER_WORD,&bits);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }


    result = ioctl(spi_fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }
    result = ioctl(spi_fd,SPI_IOC_RD_MAX_SPEED_HZ,&speed);
    if(result < 0){
	printf("error(L%d)\n",__LINE__);
	return false;
    }

    return true;
}


void LCD_LS027B4DH01::lcd_controllerInitial()
{


}

void LCD_LS027B4DH01::updateDisplay(unsigned char* buff)
{
  int i;
  //int j;
  unsigned char * buff_send = new unsigned char[LCD_HEIGHT*(LCD_WIDTH/8 + 2) + 2];
  memset(buff_send,0,sizeof(buff_send));

  for(i=0;i<240;i++){
    buff_send++;
    *buff_send++ = (unsigned char)i;
    memcpy((void*)buff_send,(const void*)buff,LCD_WIDTH/8);
    buff_send += LCD_WIDTH/8;
  }

  lcd_writeData(buff_send,sizeof(buff_send));
  delete [] buff_send;
}

bool LCD_LS027B4DH01::fillColor(unsigned char r,unsigned char g,unsigned char b)
{
  int i;
  unsigned short buff[LCD_HEIGHT*LCD_WIDTH*2];
  unsigned short color;
  color = r + g + b; // temp!!

  for (i=0;i<sizeof(buff)/sizeof(unsigned short);i++){
    buff[i] = color;
  }
  updateDisplay((unsigned char*)buff);
}

