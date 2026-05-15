//#include <Arduino.h>
#include <avr/io.h>
#include <avr/portpins.h>
#include "pins.h"

#define HIGH 0x1
#define LOW  0x0

#define INPUT  0x0
#define OUTPUT 0x1



int servoPin = 3; // Set the pin of servo to A3

// Define servopin as a constant value (PC3 / A3)
const int servopin = 3;

// Pin definitions
//#define SCL_Pin A5  // Set the clock pin to A5
//#define SDA_Pin A4  // Set data pin to A4
/*
int left_ctrl = 2;  // Define the direction control pins of group B motor
int left_pwm = 5;   // Define the PWM control pins of group B motor
int right_ctrl = 4; // Define the direction control pins of group A motor
int right_pwm = 6;  // Define the PWM control pins of group A motor
//const int servopin = A3; // Set the pin of servo to A3 
*/
/*
int L_pin = PB3; // Define the left tracking sensor pin as D11
int M_pin = PD7;  // Define the middle tracking sensor pin as D7
int R_pin = PB0;  // Define the right tracking sensor pin as D8

int trigPin = 12; // TRIG Pin be connected to D12
int echoPin = 13; // ECHO Pin be connected to D13
*/
/*
void initializePins() {
    pinMode(left_ctrl, OUTPUT); // Set direction control pins of group B motor to OUTPUT
    pinMode(left_pwm, OUTPUT);  // Set PWM control pins of group B motor to OUTPUT
    pinMode(right_ctrl, OUTPUT); // Set direction control pins of group A motor to OUTPUT
    pinMode(right_pwm, OUTPUT);  // Set PWM control pins of group A motor to OUTPUT
    DDRB &= ~(1 << L_pin); // Set PB0 as input       // Tracking sensor pins are configured for input mode
    DDRD &= ~(1 << M_pin); // Set PB0 as input
    DDRB &= ~(1 << R_pin); // Set PB0 as input
    pinMode(trigPin, OUTPUT);    // Define TRIG as the output mode
    pinMode(echoPin, INPUT);     // Define ECHO as the input mode
    //pinMode(SCL_Pin, OUTPUT);    // Set the clock pin to output
    //pinMode(SDA_Pin, OUTPUT);    // Set the data pin to output
}
*/