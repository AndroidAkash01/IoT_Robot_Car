#include <avr/io.h>  
#include <stdlib.h>			// for integer -> char converting  
#include <pins_arduino.h>
#include "uart.h"

#define HIGH 0x1
#define LOW  0x0

int speeds = 100;

char Lane;
char My_Lane;
char RIGHT = 'R';
char LEFT  = 'L';
int Brake = 0;

int Middle_Val;


void updateLaneOnObstacle(int obstacle_left,
                          int obstacle_front,
                          int obstacle_right,
                          int distance) {

  if(obstacle_front < distance) {

    if(My_Lane == RIGHT) {
     if(obstacle_left > distance){
       My_Lane = LEFT;
     }else{
        Brake = 1;
     }

    }else{
     if(obstacle_right > distance){
        My_Lane = RIGHT;
     }else{
        Brake = 1;
     }


    }

  }
}


void analogWrite(int pin, uint8_t value) {
   if (pin == left_pwm) {
      OCR0B = value;  // pin 5 PWM (OC0B)
   } else if (pin == right_pwm) {
      OCR0A = value;  // pin 6 PWM (OC0A)
   }
}

 

void digitalWrite(uint8_t pin, uint8_t value) {
   if (value == HIGH) {
      PORTD |= (1 << pin);
   } else {
      PORTD &= ~(1 << pin);
   }
}



void car_front_(int speed)//define the state of going front
{
  digitalWrite(left_ctrl,HIGH);
  analogWrite(left_pwm,(255-speed));
  digitalWrite(right_ctrl,HIGH);
  analogWrite(right_pwm,(255-speed));
}

void car_turn(int leftSpeed, int rightSpeed) {
  digitalWrite(left_ctrl, HIGH);
  analogWrite(left_pwm, leftSpeed);
  digitalWrite(right_ctrl, HIGH);
  analogWrite(right_pwm, rightSpeed);

}

//################ MOTION FUNCTIONS #####################


void car_front()//define the state of going front
{
  digitalWrite(left_ctrl,HIGH);
  analogWrite(left_pwm,(255-speeds));
  digitalWrite(right_ctrl,HIGH);
  analogWrite(right_pwm,(255-speeds));
}

void car_back()//define the status of going back
{
  digitalWrite(left_ctrl,LOW);
  analogWrite(left_pwm,(255-speeds));
  digitalWrite(right_ctrl,LOW);
  analogWrite(right_pwm,(255-speeds));
}
void car_left()//set the status of left turning
{
  digitalWrite(left_ctrl, HIGH);
  analogWrite(left_pwm, (255-speeds));  
  digitalWrite(right_ctrl, LOW);
  analogWrite(right_pwm, (255-speeds));
}
void car_right()//set the status of right turning
{
  digitalWrite(left_ctrl, LOW);
  analogWrite(left_pwm, (255-speeds));
  digitalWrite(right_ctrl, HIGH);
  analogWrite(right_pwm, (255-speeds));
}
void car_Stop()//define the state of stop
{
 
  digitalWrite(left_ctrl,LOW);
  analogWrite(left_pwm,0);
  digitalWrite(right_ctrl,LOW);
  analogWrite(right_pwm,0);
}


void car_left2(int val)//set the status of left turning
{
  digitalWrite(left_ctrl, HIGH);
  analogWrite(left_pwm, (val));  
  digitalWrite(right_ctrl, LOW);
  analogWrite(right_pwm, (val));
}
void car_right2(int val)//set the status of right turning
{
  digitalWrite(left_ctrl, LOW);
  analogWrite(left_pwm, (val));
  digitalWrite(right_ctrl, HIGH);
  analogWrite(right_pwm, (val));
}
