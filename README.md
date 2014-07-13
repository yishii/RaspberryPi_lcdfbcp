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
