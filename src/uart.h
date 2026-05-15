#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 9600
#endif

void initUART(void);

void uart_putc(uint8_t c);

void uart_puts(const char *s);

void uart_putu16(uint16_t v);

uint8_t uart_available(void);

uint8_t uart_getc_nonblock(uint8_t *c);

uint8_t uart_getc_blocking(void);

#endif










// #ifndef _UART_H_
// #define _UART_H_

// #include <avr/io.h>  
// #include <stdlib.h>			// for integer -> char converting  

// #ifndef F_CPU
// /* prevent compiler error by supplying a default */
// # warning "F_CPU not defined. Define F_CPU with 16 Mhz."
// #define F_CPU 16000000UL    // Quarz with 16 Mhz  */
// #endif

// #define UART_BAUDRATE 9600 
// #define UART_BAUD_SELECT ((F_CPU / (UART_BAUDRATE * 16L))-1) 










// /******************************************************************//**
//  * Sends a single character via UART (eg. to a Hyperterminal).
//  *********************************************************************/
// void initUART()
// {
// 	UBRR0H = (UART_BAUD_SELECT>>8); 
//   	UBRR0L = UART_BAUD_SELECT; 

// 	UCSR0B |= (1<<TXEN0)|(1<<RXEN0); 	//Transmit/Receive enable
	
//  	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); 
//  	UCSR0C &= ~(1<<UMSEL00) & ~(1<<UPM00) & ~(1<<USBS0);
// }

// /******************************************************************//**
//  * Sends a single character via UART (eg. to a Hyperterminal).
//  *********************************************************************/
// void uart_putc(unsigned char c)
// {
//     while (!(UCSR0A & (1<<UDRE0)))  // wait until sending is possible
//     {
//     }                             
//     UDR0 = c;                       // sending signs
// }
 
// /******************************************************************//**
//  * Sends a complete string via UART (eg. to a Hyperterminal).
//  *********************************************************************/
// void uart_puts (char *s)
// {
//     while (*s)
//     {                               // sending char until "/0"
//         uart_putc(*s);
//         s++;
//     }
// }

// /******************************************************************//**
//  * Sends an Integer via UART (eg. to a Hyperterminal).
//  *********************************************************************/
// void uart_puti16 (uint16_t value)
// {
// 	char _buffer[6];
// 	itoa( value, _buffer, 10 );	//conversion from integer to char
// 	uart_puts(_buffer);
// }

// /******************************************************************//**
//  * Sends an Integer in Binaryformat via UART (eg. to a Hyperterminal).
//  *********************************************************************/
// void uart_puti16B (uint16_t value)
// {
// 	char _buffer[17];
// 	itoa( value, _buffer, 2 );	//conversion from integer to char
// 	uart_puts(_buffer);
// }

// unsigned char uart_receivec(void)
// {
// 	/* Wait for data to be received */
// 	while (!(UCSR0A & (1<<RXC0)))
// 	;
// 	/* Get and return received data from buffer */
// 	return UDR0;
// }


// void UART_send(char data) {
//     // Set TX pin as output (Normal TX Mode)
//     DDRD |= (1 << PD1);  

//     // Send data one bit at a time
//     for (int i = 0; i < 8; i++) {
//         if (data & (1 << i)) {
//             // Instead of driving TX HIGH (5V), set it as INPUT (weak pull-up)
//             DDRD &= ~(1 << PD1);  
//         } else {
//             // Drive TX LOW (0V)
//             DDRD |= (1 << PD1);
//             PORTD &= ~(1 << PD1);
//         }
//         _delay_us(104); // Approximate 9600 baud rate
//     }

//     // Restore TX pin to HIGH state (pull-up)
//     DDRD &= ~(1 << PD1);  
// }


// void UART_init(uint16_t baud) {
//     uint16_t UBRR_val = (F_CPU / (16UL * baud)) - 1;
//     UBRR0H = (UBRR_val >> 8);
//     UBRR0L = UBRR_val;
//     UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // Enable TX and RX
//     UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, no parity, 1 stop bit
// }


// char UART_BT_receive() {
//     while (!(UCSR0A & (1 << RXC0)));  // Wait for data to be received

// 	uart_putc(UDR0 );

//     return UDR0;  // Return received byte
// }







// #endif /* _UART_H_ */
