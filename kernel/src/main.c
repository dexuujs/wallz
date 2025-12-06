#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <memalloc.h>
#include <misc/serial.h>
#include <typeutils/convert.h>

__attribute__((used, section(".limine_requests")))
volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request FramebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void Hang(void) {
    for (;;) {
        asm ("hlt");
    }
}

void draw_rectangle(uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
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

void draw_rectangle_rounded(uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer,
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

void kmain(void) {
    /* [[ stage 1 - variables ]] */
    struct limine_framebuffer *Framebuffer;
    volatile uint32_t *FramebufferPointer;
    /* [[ end ]] */

    /* [[ stage 2 - get framebuffer ]] */
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        Hang();
    }
    if (FramebufferRequest.response == NULL
     || FramebufferRequest.response->framebuffer_count < 1) {
        Hang();
    }
    Framebuffer = FramebufferRequest.response->framebuffers[0];
    FramebufferPointer = Framebuffer->address;

    serial_init();
    /* [[ end ]] */

    /* [[ stage 3 - draw.. etc. etc. ]] */
    draw_rectangle_rounded(FramebufferPointer, Framebuffer, 10, 10, 400, 400, 8, 0x323232);
    draw_rectangle_rounded(FramebufferPointer,Framebuffer,20,40,380,360,8,0xFFFFFF);
    draw_rectangle(FramebufferPointer,Framebuffer,20,20,380,20,0x323232);
    serial_puts("Hello, world!");
    /* [[ end ]] */

    // We're done, just hang...
    Hang();
}
