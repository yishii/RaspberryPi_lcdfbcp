RaspberryPi_lcdfbcp
===================
direct video buffer copy to LCD directly connected with.

Author : Yasuhiro ISHII ishii.yasuhiro@gmail.com

How to install
===
1) setup Raspbian to make SPI enable

 please refer to the instructions of this site below(in Japanese)

 http://mknod.jp/?cat=2

2) $ cd lcdfbcp

3) $ make

4) $ sudo make install

Then,the main process lcdfbcp will be installed at /usr/bin


If you want to launch this program at booting the system automatically,
please add /etc/inittab to below: (recommended to add it at the beginning
of this file)

lf::respawn:/usr/bin/lcdfbcp

For RaspberryPi Type B(Rev.1.0) users:
===
RaspberryPi Type B(Rev.1.0) is still not officially supported with this program,but
if you want to use this with it,please modify defgpio.h as described below:


before) #define GPIO_LCD_RESET (27)

after ) #define GPIO_LCD_RESET (21)

This program will support Type B 1.0 in later release,sorry for inconvenience.

Thanks
