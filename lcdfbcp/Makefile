#
# Makefile for lcd_fbcp
#
# Yasuhiro ISHII,9/10/2014
#

CROSS		 = 
CC		 = $(CROSS)g++
CFLAGS		 = -O0 -I. -g -L/opt/vc/lib
CFLAGS		+= -I/opt/vc/include
CFLAGS		+= -I/opt/vc/include/interface/vcos/pthreads
CFLAGS		+= -I/opt/vc/include/interface/vmcs_host
CFLAGS		+= -I/opt/vc/include/interface/vmcs_host/linux
CFLAGS		+= -I.
CFLAGS		+= -I./lcd
CFLAGS		+= -lbcm_host
OBJS		 = main.o gpiolib.o fbcp.o hwdetect.o
OBJS		+= lcd/st7735.o
OBJS		+= lcd/st7735r.o
TARGET		 = lcdfbcp

.PHONY: clean purge all

all:	$(TARGET)

.c.o:
	@echo "Compile : " $<
	@$(CC) $(CFLAGS) -c $<

.cpp.o:
	@echo "Compile : " $<
	@$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	@echo "Link : " $@
	@$(CC) $(CFLAGS) -o $@ $(notdir $(OBJS))
	@echo "lcdfbcp is now built and ready to execute!"

clean:
	@echo "Clean project"
	rm -f *.bak $(TARGET) $(OBJS)

purge:
	rm -f *~
	rm -f ./lcd/*~

install:
	@echo "install program to /usr/bin"
	install $(TARGET) /usr/bin

depend: $(OBJS:.o=.cpp)
	-@ makedepend -- $(CFLAGS) -- -Y $^ -f Makefile 2> /dev/null

# DO NOT DELETE
