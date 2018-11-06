#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <math.h>
#include "ifttt.h"

float ReadTemperature(char serial_number[]);

int main(int argc, char *argv[])
{
  float lowest, highest, new, last_message;
  char serial[] = "28-01131e7c7bfd";

  char low[5], hi[5], now[5];
  
  low[5] = '\0';
  hi[5] = '\0';
  now[5] = '\0';
  
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
    delay(1000);
    printf("current read %.1f\n", new);
  }
  
  return 0;
}

float ReadTemperature(char serial_number[]){
  char dir[] = "/sys/bus/w1/devices/00-000000000000/w1_slave";
  int i, n;
  float result;
  char buf[101];
  FILE *fd;
  for(i = 0; i < 15; i++){
    int index = i + 20;
    dir[index] = serial_number[i];
  }  

  if((fd = fopen(dir, "r")) == (FILE *)NULL){
  }
  
  n = fread(buf, 1, 100, fd);
  buf[n] = '\0';
  
  result = ((buf[n-4] - 48) * 0.1 + (buf[n-5] - 48) + (buf[n-6] - 48) * 10);
  
  fclose(fd);
  return result;
}
