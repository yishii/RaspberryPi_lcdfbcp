#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool isRaspberryPi2(void)
{
  FILE* fp;
  char buff[256];
  const char* raspberrypi2_device_name = "BCM2709";
  const char* raspberrypi3_device_name = "BCM2835";
  bool newraspberrypi = false;

  fp = fopen("/proc/cpuinfo","r");
  while(fgets(buff,sizeof(buff),fp) != NULL){
    if(strstr(buff,raspberrypi2_device_name) != NULL){
      newraspberrypi = true;
    }
    if(strstr(buff,raspberrypi3_device_name) != NULL){
      newraspberrypi = true;
    }
  }
  fclose(fp);

  //printf("RasPiDetect : %s                ", newraspberrypi ? "True" : "False");

  return newraspberrypi;
}

  
