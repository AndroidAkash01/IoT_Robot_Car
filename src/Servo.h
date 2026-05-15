 #include <avr/io.h>  
#include <stdlib.h>			// for integer -> char converting  
#include <pins_arduino.h>
#include <avr/interrupt.h>
 
// #include "Car.h"

// #define servopin A3  //set the pin of servo to A3

#define TRIGGER_PIN PB4
#define ECHO_PIN PB5

 
#define SERVO_PIN PB1  // Use Arduino Pin 9 (OC1A) for hardware PWM

#define TOP 20000
#define SERVO_MIN 950
#define SERVO_MAX 4700
#define RANGE (SERVO_MAX - SERVO_MIN) / 180



#define TRIGGER_DDR  DDRB
#define TRIGGER_PORT PORTB
#define ECHO_DDR  DDRB
#define ECHO_PORT PINB

#define TIMER2_PRESCALER_BITS ((1 << CS22) | (1 << CS21) | (1 << CS20)) // 1024 prescaler
#define TIMER2_OVERFLOW 10

void ultrasonic_init();
void servo_init(); 
void watchDogInit();


volatile unsigned int pulse_width = 0;
volatile uint8_t i = 0;

int counter  = 0;
int obstacle_left, obstacle_front, obstacle_right;
char ServoDirection;

volatile uint8_t servoUpdateFlag = 0;

int scanForSpace  = 0;




 void setupServo() {
   
      DDRD |= (1 << TRIGGER_PIN); 
      DDRB &= ~(1 << ECHO_PIN);


    //   DDRC |= (1 << servopin); 

      servo_init();

      ultrasonic_init();


 

}
 
 
void ultrasonic_init() {
    TRIGGER_DDR |= (1 << TRIGGER_PIN);   // Trigger pin as OUTPUT
    ECHO_DDR &= ~(1 << ECHO_PIN);  // Echo pin as INPUT

    // Enable pin change interrupt for echo pin
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT5);

    // Enable global interrupts
    sei();

    // Configure Timer2 for CTC mode
    OCR2A = 255;
    TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
    TCCR2A |= (1 << WGM21);  // CTC mode
    TCCR2B |= TIMER2_PRESCALER_BITS; // Set prescaler
}

ISR(PCINT0_vect) {
    if (!(ECHO_PORT & (1 << ECHO_PIN))) { // ECHO pin goes LOW
        pulse_width = TCNT2; // Read Timer2 value
    }
    TCNT2 = 0; // Reset Timer2
}

ISR(TIMER2_COMPA_vect) {
    i = (i >= TIMER2_OVERFLOW) ? 0 : i + 1;
    if (i == 0) {
        TRIGGER_PORT |= (1 << TRIGGER_PIN); // Set Trigger HIGH
    }
    else if (i == TIMER2_OVERFLOW) {
        TRIGGER_PORT &= ~(1 << TRIGGER_PIN); // Set Trigger LOW
    }
}

unsigned int get_distance() {
    return pulse_width ; // Convert pulse width to distance in cm
}


 



 
 



 


void servo_init(void) {
    DDRB |= (1 << SERVO_PIN);  // Set Pin 9 as OUTPUT (PB1 / OC1A)

    // Configure Timer1 for Fast PWM (ICR1 as TOP), Prescaler 8
    TCCR1A = (1 << WGM11) | (1 << COM1A1); 
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Prescaler 8

    ICR1 = TOP;  // Set PWM period (20ms / 50Hz)

    watchDogInit();   

     
}

// Set Servo Angle (0° to 180°)
void set_angle(int angle) {
    OCR1A = SERVO_MIN + ((long)RANGE * angle);
}

 



void watchDogInit() {
 

    WDTCSR = (1 << WDCE) | (1 << WDE);  // Enable changes to WDTCSR
    WDTCSR = (1 << WDIE) | (1 << WDP0);  // Set **16ms timeout**, enable interrupt
    sei();
}





// Timer0 Compare Match Interrupt (Fires Every 1 Second)
ISR(WDT_vect) {

    // uart_puts("Hello, World!!\n");
   
   counter = counter + 1;

    if(counter % 7 == 0) {
 
        servoUpdateFlag = 1;   

        ServoDirection = Lane;

    } 
   if(counter == 15) {
 
    servoUpdateFlag = 2;  

    counter = 0;
    

   }

    
}



 


 