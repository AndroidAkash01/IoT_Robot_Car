#include <avr/io.h>  
#include <stdlib.h>			// for integer -> char converting  
#include <pins_arduino.h>
#include "uart.h"





#define L_ADC 0   // A0
#define M_ADC 4   // A4
#define R_ADC 2   // A2
 
int L_val, M_val, R_val, inner_R_val, inner_L_val;


void init_IR_Sensors()
{
    
 
    //   DDRC &= ~(1 << L_pin); // Set  as input
    //   DDRC &= ~(1 << M_pin); // Set  as input
    //   DDRC &= ~(1 << R_pin); // Set  as input

    DDRC &= ~((1 << PC0) | (1 << PC4) | (1 << PC2));


      
      PORTC &= ~(1 << PC0);
      PORTC &= ~(1 << PC3);
      PORTC &= ~(1 << PC4);
      PORTC &= ~(1 << PC1);
      PORTC &= ~(1 << PC5);

      // Enable internal pull-up resistors (optional, depends on your sensor)
    //   PORTB |= (1 << L_pin);
    //   PORTB |= (1 << M_pin);
    //   PORTB |= (1 << R_pin);

    PORTC &= ~((1 << PC0) | (1 << PC4) | (1 << PC2));
 

}


uint16_t readADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);  // Select ADC channel
    ADCSRA |= (1 << ADSC);  // Start ADC conversion
    while (ADCSRA & (1 << ADSC));  // Wait for conversion to complete
    return ADC;  // Return 10-bit ADC value (0-1023)
}


 

void read_IR_SensorValues() {

    L_val = readADC(L_ADC);
    M_val = readADC(M_ADC);
    R_val = readADC(R_ADC);  
 
    char str1[40];
    sprintf(str1, "L: %d, M: %d, R: %d\n", L_val, M_val, R_val);
    uart_puts(str1);

}


 