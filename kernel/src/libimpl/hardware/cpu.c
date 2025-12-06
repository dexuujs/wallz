#include <cpu.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uint64_t chkbit() {
    return sizeof(void*) * 8;
}