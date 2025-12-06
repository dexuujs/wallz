#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <memalloc.h>
#include <misc/serial.h>
#include <gui/drawmgr.h>
#include <gui/guidef.h>
#include <gui/guitypes.h>
#include <gui/drawhelper.h>

void draw_rectangle(volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
                    int sx, int sy, int width, int height, uint32_t color) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int px = sx + i;
            int py = sy + j;
            if (px >= 0 && px < (int)Framebuffer->width &&
                py >= 0 && py < (int)Framebuffer->height) {
                FramebufferPointer[py * (Framebuffer->pitch / 4) + px] = color;
            }
        }
    }
}

void draw_rectangle_rounded(volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
                            int sx, int sy, int width, int height, int8_t roundness, uint32_t color) {
    int r = roundness < 0 ? 0 : roundness;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int px = sx + i;
            int py = sy + j;

            if (px < 0 || px >= (int)Framebuffer->width || py < 0 || py >= (int)Framebuffer->height)
                continue;

            // Check if the pixel is in a corner region and outside the rounding circle.
            // If it's outside the circle, it's the clipped area, so we skip drawing the color.
            
            // Top-left corner: (i - r) * (i - r) + (j - r) * (j - r) > r * r
            // Center of circle is at (r, r) relative to the rectangle's top-left (0, 0).
            if (i < r && j < r) {
                if ((i - r) * (i - r) + (j - r) * (j - r) > r * r) continue;
            }

            // Top-right corner: (i - (width - r)) * (i - (width - r)) + (j - r) * (j - r) > r * r
            // Center of circle is at (width - r, r).
            else if (i >= width - r && j < r) {
                if ((i - (width - r)) * (i - (width - r)) + (j - r) * (j - r) > r * r) continue;
            }

            // Bottom-left corner: (i - r) * (i - r) + (j - (height - r)) * (j - (height - r)) > r * r
            // Center of circle is at (r, height - r).
            else if (i < r && j >= height - r) {
                if ((i - r) * (i - r) + (j - (height - r)) * (j - (height - r)) > r * r) continue;
            }

            // Bottom-right corner: (i - (width - r)) * (i - (width - r)) + (j - (height - r)) * (j - (height - r)) > r * r
            // Center of circle is at (width - r, height - r).
            else if (i >= width - r && j >= height - r) {
                if ((i - (width - r)) * (i - (width - r)) + (j - (height - r)) * (j - (height - r)) > r * r) continue;
            }
            
            // If none of the 'continue' conditions were met, draw the color.
            FramebufferPointer[py * (Framebuffer->pitch / 4) + px] = color;
        }
    }
}