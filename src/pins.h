#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

// Pin definitions
#define SCL_Pin A5  // Set the clock pin to A5
#define SDA_Pin A4  // Set data pin to A4
#define HIGH 0x1
#define LOW  0x0

extern int left_ctrl;  // Define the direction control pins of group B motor
extern int left_pwm;   // Define the PWM control pins of group B motor
extern int right_ctrl; // Define the direction control pins of group A motor
extern int right_pwm;  // Define the PWM control pins of group A motor
extern const int servopin; // Set the pin of servo to A3 

extern int L_pin; // Define the left tracking sensor pin as D11
extern int M_pin; // Define the middle tracking sensor pin as D7
extern int R_pin; // Define the right tracking sensor pin as D8

extern int trigPin; // TRIG Pin be connected to D12
extern int echoPin; // ECHO Pin be connected to D13

void initializePins();

#endif // PINS_H
