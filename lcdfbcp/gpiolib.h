#ifndef __GPIOLIB_H__
#define __GPIOLIB_H__

extern bool gpiolib_init(void);
extern bool gpiolib_setGpioDirection(int gpio_no,bool in_xout);
extern bool gpiolib_setGpioData(int gpio_no,bool level);

#endif /* __GPIOLIB_H__ */
