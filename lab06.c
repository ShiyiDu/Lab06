#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "ifttt.h"

float ReadTemperature(char serial_number[]);

int main(int argc, char *argv[])
{
  float lowest, highest, new, last_message;
  
  char low[5], hi[5], now[5];
  
  low[5] = '\0';
  hi[5] = '\0';
  now[5] = '\0';
  
  char serial[] = "28-01131e7c7bfd";
  
  last_message = ReadTemperature(serial);
  
  while(1){
    new = ReadTemperature(serial);
    if(new < lowest) lowest = new;
    if(new > lowest) highest = new;
    if(fabs(new-last_message) >= 1){
      last_message = new;
      
      gcvt(lowest, 1, low);
      gcvt(highest, 1, hi);
      gcvt(new, 1, now);
      
      low[5] = '\0';
      hi[5] = '\0';
      now[5] = '\0';
      
      printf("sending the email\n");
      ifttt("https://maker.ifttt.com/trigger/Lab05/with/key/W17kRhCtZa-nACHobFKeI", low, "Shiyi", "214438469");
    }
  }
  
  return 0;
}

float ReadTemperature(char serial_number[]){
  char dir[] = "/sys/bus/w1/devices/00-000000000000/w1_slave";
  for(int i = 0; i < 15; i++){
    int index = i + 20;
    dir[index] = serial_number[i];
  }
  
  FILE *fd;
  int n;
  char buf[101];
  
  if((fd = fopen(dir, "r")) == (FILE *)NULL){
    //not read
  }
  
  n = fread(buf, 1, 100, fd);
  bur[n] == '\0';
  
  float result = ((bur[n-3] - 48) * 0.1 + (bur[n-4] - 48) + (bur[n-5] - 48) * 10);
  
  fclose(fd);
  return result;
}