#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

/* ---- Config ---- */
#define UART_UBRR ((F_CPU / (16UL * UART_BAUDRATE)) - 1)

/* Ring buffer (power-of-two size) */
#ifndef UART_RX_BUF_SIZE
#define UART_RX_BUF_SIZE 64
#endif

#if ((UART_RX_BUF_SIZE & (UART_RX_BUF_SIZE - 1)) != 0)
#error "UART_RX_BUF_SIZE must be power of two"
#endif

static volatile uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile uint8_t rx_head = 0;   // next write index
static volatile uint8_t rx_tail = 0;   // next read index

/* ---- Helpers ---- */
static inline uint8_t _rb_count(void) {
    return (uint8_t)((rx_head - rx_tail) & (UART_RX_BUF_SIZE - 1));
}
static inline uint8_t _rb_push(uint8_t b) {
    uint8_t next = (uint8_t)((rx_head + 1) & (UART_RX_BUF_SIZE - 1));
    if (next == rx_tail) return 0;       // overflow, drop
    rx_buf[rx_head] = b;
    rx_head = next;
    return 1;
}

/* ---- Init / TX ---- */
void initUART(void) {
    // Baud
    UBRR0H = (uint8_t)(UART_UBRR >> 8);
    UBRR0L = (uint8_t)(UART_UBRR & 0xFF);

    // 8N1
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // Enable RX, TX, and RX interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Flush stray RX
    while (UCSR0A & (1 << RXC0)) (void)UDR0;
}

void uart_putc(uint8_t c) {
    while (!(UCSR0A & (1 << UDRE0))) { /* wait */ }
    UDR0 = c;
}

void uart_puts(const char *s) {
    while (*s) uart_putc((uint8_t)*s++);
}

void uart_putu16(uint16_t v) {
    char buf[6];
    char *p = &buf[5];
    *p = '\0';
    do { *--p = (char)('0' + (v % 10)); v /= 10; } while (v);
    uart_puts(p);
}

/* ---- RX ISR ---- */
ISR(USART_RX_vect) {
    uint8_t b = UDR0;     // reading clears RXC0
    (void)_rb_push(b);    // drop on overflow
}

/* ---- RX API ---- */
uint8_t uart_available(void) {
    return _rb_count();
}

uint8_t uart_getc_nonblock(uint8_t *c) {
    if (rx_head == rx_tail) return 0;
    *c = rx_buf[rx_tail];
    rx_tail = (uint8_t)((rx_tail + 1) & (UART_RX_BUF_SIZE - 1));
    return 1;
}

uint8_t uart_getc_blocking(void) {
    while (rx_head == rx_tail) { /* wait */ }
    uint8_t b = rx_buf[rx_tail];
    rx_tail = (uint8_t)((rx_tail + 1) & (UART_RX_BUF_SIZE - 1));
    return b;
}
