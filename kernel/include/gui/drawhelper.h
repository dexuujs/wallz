#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <memalloc.h>
#include <misc/serial.h>
#include <gui/drawmgr.h>
#include <gui/guidef.h>
#include <gui/guitypes.h>

void draw_rectangle(volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
                    int sx, int sy, int width, int height, uint32_t color);

void draw_rectangle_rounded(volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
                            int sx, int sy, int width, int height, int8_t roundness, uint32_t color);