#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool isRaspberryPi2(void)
{
  FILE* fp;
  char buff[256];
  const char* raspberrypi2_device_name = "BCM2709";
  bool raspberrypi2 = false;

  fp = fopen("/proc/cpuinfo","r");
  while(fgets(buff,sizeof(buff),fp) != NULL){
    if(strstr(buff,raspberrypi2_device_name) != NULL){
      raspberrypi2 = true;
    }
  }
  fclose(fp);

  return raspberrypi2;
}

  
