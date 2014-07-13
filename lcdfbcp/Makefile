#
# Makefile for lcd_fbcp
#
# Yasuhiro ISHII,6/7/2014
#

CROSS		 = 
CC		 = $(CROSS)gcc
CFLAGS		 = -O0 -I. -g -L/opt/vc/lib
CFLAGS		+= -I/opt/vc/include
CFLAGS		+= -I/opt/vc/include/interface/vcos/pthreads
CFLAGS		+= -I/opt/vc/include/interface/vmcs_host
CFLAGS		+= -I/opt/vc/include/interface/vmcs_host/linux
CFLAGS		+= -lbcm_host
OBJS		 = lcdc.o gpiolib.o control_lcd.o fbcp.o
TARGET		 = lcdfbcp

.PHONY: clean purge all

all:	$(TARGET)

.c.o:
	@echo "========================================================"
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	@echo "Clean project =========================================="
	rm -f *.bak $(TARGET) $(OBJS)

purge:
	rm -f *~

install:
	install $(TARGET) /usr/bin

depend: $(OBJS:.o=.c)
	-@ makedepend -- $(CFLAGS) -- -Y $^ -f Makefile 2> /dev/null

# DO NOT DELETE
