#include <avr/io.h>  
#include <stdlib.h>			// for integer -> char converting  
 

#define MOTOR_PWM_TIMER_TOP 255
#define MOTOR_MINIMUM_SPEED 0
#define MOTOR_MAXIMUM_SPEED 100

#define MOTORS_DDRx DDRD
// #define LEFT_MOTOR_OCnx  OCR0A
// #define RIGHT_MOTOR_OCnx OCR0B

#define THRESHOLD_LEFT 150
#define THRESHOLD_CENTER 500
#define THRESHOLD_RIGHT 850

#define motor_speed 75
#define motor_speed_At_turn 55



// Left motors physically connected to B/B1 ports:
#define left_ctrl PD4    // previously "right_ctrl"
#define left_pwm  PD6    // previously "right_pwm" (OCR0A)

// Right motors physically connected to A/A1 ports:
#define right_ctrl PD2   // previously "left_ctrl"
#define right_pwm  PD5   // previously "left_pwm" (OCR0B)

#define LEFT_MOTOR_OCnx  OCR0A  // PWM register matches PD6
#define RIGHT_MOTOR_OCnx OCR0B  // PWM register matches PD5
// void initMotor()
// {
//     // Set direction control pins and PWM pins as outputs
//     DDRD |= (1 << left_ctrl) | (1 << right_ctrl) | (1 << left_pwm) | (1 << right_pwm);

//     // Fast PWM, non-inverting mode on OC0A & OC0B
//     TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
//     TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler=64 (~976 Hz)
// }


void initMotor()
{
    // Direction control and PWM pins as output
    DDRD |= (1 << left_ctrl) | (1 << right_ctrl) | (1 << left_pwm) | (1 << right_pwm);
 

    DDRD |= (1 << PD5) | (1 << PD6);  // Explicit PWM pins as outputs (clearly defined)


    // Configure Timer0 for Fast PWM, Non-Inverting Mode
    TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler = 64 (~976 Hz PWM frequency)
}


 



