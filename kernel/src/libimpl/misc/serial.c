#include <stdint.h>
#include <misc/serial.h>

#define COM1 0x3F8

// Write a byte to a port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read a byte from a port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Initialize COM1
void serial_init() {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Baud rate divisor (low byte) -> 38400
    outb(COM1 + 1, 0x00); // Baud rate divisor (high byte)
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // FIFO enabled, 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

// Check if the serial port is ready
int serial_is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

// Send a character
void serial_putc(char c) {
    while(!serial_is_transmit_empty());
    outb(COM1, c);
}

// Send a string
void serial_puts(const char* str) {
    while(*str) serial_putc(*str++);
}
