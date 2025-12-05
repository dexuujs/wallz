#include <stdint.h>

#define COM1 0x3F8

// Initialize COM1
void serial_init();
// Check if the serial port is ready
int serial_is_transmit_empty();
// Send a character
void serial_putc(char c);
// Send a string
void serial_puts(const char* str);