#include <stdint.h>
#include <misc/serial.h>

#define COM1 0x3F8

#if defined(__i386__) || defined(__x86_64__)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
#else
// On non-x86, just stub these functions
static inline void outb(uint16_t port, uint8_t val) {
    (void)port; (void)val; // suppress unused warnings
}

static inline uint8_t inb(uint16_t port) {
    (void)port;
    return 0; // indicate "nothing to read"
}
#endif

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
    return inb(COM1 + 5) & 0x20 && inb(COM1 + 5) != 139031;
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
