#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include "ifttt.h"

void AlarmOff(int *to_init);
void AlarmArming(int *to_init);
void AlarmArmed(int *to_init);
void AlarmTriggered(int *to_init);
void AlarmSounding(int *to_init);

double current_time;
char current_state = 0;

int main(int argc, char *argv[])
{
  int button, to_init, last_button;
  to_init = 1;
  wiringPiSetup ();
  pinMode(0, INPUT);
  
  pinMode (1, OUTPUT);
  pinMode (2, OUTPUT);
  
  pinMode (3, INPUT);
  pullUpDnControl(3, PUD_UP);
  
  pinMode (4, OUTPUT);
  
  while (1) {
    current_time = (double) time(NULL);
    if(last_button != digitalRead(3)) button = !digitalRead(3);
    else button = 0;
    last_button = digitalRead(3);
    switch (current_state) {
    	case 0:
        AlarmOff(&to_init);
        if(button) {
          current_state = 1;
          to_init = 1;
        }
        break;
      case 1:
        AlarmArming(&to_init);
        break;
      case 2:
        AlarmArmed(&to_init);
        if(button) {
          current_state = 0;
          to_init = 1;
        }
        break;
      case 3:
        AlarmTriggered(&to_init);
        if(button) {
          current_state = 0;
          to_init = 1;
        }
        break;
      case 4:
        AlarmSounding(&to_init);
        if(button) {
          current_state = 0;
          to_init = 1;
        }
        break;
    }
  }
  
  
/*
  ifttt("https://maker.ifttt.com/trigger/button_pressed/with/key/56-YpOKO17vOh-gimC2xK1qRAhRdzXXXX", "my1", "my 2", "my 33333");
*/

/*  printf("Trying to connect to server\n");
    ifttt("http://red.eecs.yorku.ca:8080/trigger/event/with/key/123", "my1", "my 2", "my 33333");
*/  return 0;
}

void AlarmOff(int *to_init){
  if(*to_init){
    printf("Alarm Off\n");
    *to_init = 0;
        
    digitalWrite (1, HIGH);
    digitalWrite (2, LOW);
  }
}

void AlarmArming(int *to_init){
  static double start_time;
  if(*to_init){
    printf("Alarm Arming\n");
    *to_init = 0;
    start_time = current_time;
  }
  
  digitalWrite(1, LOW); delay(500);
  digitalWrite(1, HIGH); delay(500);
  
  if(current_time - start_time >= 10){
    current_state = 2;
    *to_init = 1;
  }
}

void AlarmArmed(int *to_init){
  if(*to_init){
    printf("Alarm Armed\n");
    *to_init = 0;
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
  }

  if(digitalRead(0)){
    current_state = 3;
    *to_init = 1;
  }
}

void AlarmTriggered(int *to_init){
  static double start_time;
  static double led_time;
  static double last_led_time;
  static int led_status = 1;
  if(*to_init){
    printf("Alarm triggered\n");
    *to_init = 0;
    digitalWrite(4, LOW);
    start_time = current_time;
    led_time = current_time;
    last_led_time = current_time;
    
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
  }
  
  led_time = current_time - last_led_time;
  
  if(led_time > 1){
    last_led_time += 1;
    if(led_status) {
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      led_status = 0;
    }else {
      digitalWrite(1, HIGH);
      digitalWrite(2, HIGH);
      led_status = 1;
    }
  }
  
  if(current_time - start_time >= 10){
    current_state = 4;
    *to_init = 1;
  }
}

void AlarmSounding(int *to_init){
  if(*to_init){
    printf("Alarm Sounding\n");
    *to_init = 0;
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    delay (2000); 
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    delay (2000); 

    printf("sending the email\n");
    ifttt("https://maker.ifttt.com/trigger/Lab05/with/key/W17kRhCtZa-nACHobFKeI", "Lab05", "Shiyi", "214438469");
  }
}
