/*
  This software is distributed under Apache 2.0 license


  memo:

 */
#include <stdio.h>
#include <unistd.h>
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
#include "hwdetect.h"

bool gpiolib_init(void);
bool gpiolib_setDirectionGpio17(bool in_xout);
bool gpiolib_setDataGpio17(bool level);

#define BCM2835_PERI_BASE	(0x20000000)
#define BCM2836_PERI_BASE	(0x3F000000)
#define GPIO_OFFSET		(0x00200000)
#define BLOCK_SIZE		(1024*4)

#define INP_GPIO(g)		*(gpio_mem+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)		*(gpio_mem+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_SET		*(gpio_mem+7)
#define GPIO_CLR		*(gpio_mem+10)

static volatile unsigned * gpio_mem = NULL;

bool gpiolib_init(void)
{
    int memfd;
    bool ret = false;
    int gpio_base_address;

    printf("Detected hardware : ");
    if(isRaspberryPi2()){
      gpio_base_address = BCM2836_PERI_BASE + GPIO_OFFSET;
      printf("RaspberryPi2\n");
    } else {
      gpio_base_address = BCM2835_PERI_BASE + GPIO_OFFSET;
      printf("RaspberryPi\n");
    }

    memfd = open("/dev/mem",O_RDWR|O_SYNC);
    if(memfd < 0){
	printf("%s : memory open error\n",__func__);
    } else {
      gpio_mem = (volatile unsigned*)mmap(NULL,BLOCK_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,memfd,gpio_base_address);
	close(memfd);
	ret = true;
    }
    return(ret);
}

bool gpiolib_setGpioDirection(int gpio_no,bool in_xout)
{
    bool ret = false;

    if(gpio_mem != NULL){
	INP_GPIO(gpio_no);
	if(in_xout == false){
	    OUT_GPIO(gpio_no);
	    ret = true;
	}
    }
    return(ret);
}

bool gpiolib_setGpioData(int gpio_no,bool level)
{
    bool ret = false;

    if(gpio_mem != NULL){
	if(level == true){
	    GPIO_SET = 1 << gpio_no;
	} else {
	    GPIO_CLR = 1 << gpio_no;
	}
    }
    return(ret);
}

