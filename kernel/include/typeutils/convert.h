#include <stddef.h>

size_t uint_to_str(unsigned int x, char *str) {
    size_t i = 0;
    // Handle the case where the input is 0
    if (x == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }

    // Process individual digits
    while (x != 0) {
        str[i++] = (x % 10) + '0'; // Convert digit to ASCII char
        x = x / 10;
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string to get the correct order
    reverse(str, i);

    return i;
}