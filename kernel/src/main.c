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
#include <cpu.h>

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

#if defined(__i386__) || defined(__x86_64__)
#define Hang() {__asm__ volatile ("hlt");}
#else
#define Hang() do {} while(0) // no-op for other architectures
#endif

void kmain(void) {
    /* [[ stage 1 - variables ]] */
    struct limine_framebuffer *Framebuffer;
    volatile uint32_t *FramebufferPointer;
    GUI_STATUS GUIStatus;
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
    /* x,y,sx,sy,rnd/tck?,clr */
    draw_rectangle(FramebufferPointer, Framebuffer, 0, 0, Framebuffer->width, Framebuffer->height, 0x202020);
    draw_rectangle_rounded(FramebufferPointer, Framebuffer, 10, 10, 400, 400, 8, 0x323232);
    draw_rectangle_rounded(FramebufferPointer,Framebuffer,20,40,380,360,8,0xFFFFFF);
    draw_rectangle(FramebufferPointer,Framebuffer,20,20,380,20,0x323232);
    draw_rectangle_rounded(FramebufferPointer,Framebuffer,375,12,25,25,10,0xFF0000);
    draw_rectangle_rounded(FramebufferPointer,Framebuffer,345,12,25,25,10,0xFF8000);
    draw_rectangle_rounded(FramebufferPointer,Framebuffer,315,12,25,25,10,0x00FF00);
    // dirty hack while heap is not implemented
    Widget myWidget_instance;
    Widget* myWidget = &myWidget_instance;
    // dirty hack end
    Rect myRectangle;
    GuiObjectParameters myWidget_Parameters;

    myWidget_Parameters.Visible = true;
    myWidget_Parameters.Position = (t2D){ .x = 10, .y = 20 };
    myWidget_Parameters.Size = (t2D){ .x = 50, .y = 50 };

    myWidget->type = WIDGET_RECT;

    myRectangle.Parameters = myWidget_Parameters;
    myRectangle.Color = (RGBA){ .HexCode = 0xFF00FF, .IsAlpha = false };
    myRectangle.Roundness = 8;

    myWidget->data = &myRectangle;
    GUIStatus = Draw(myWidget, FramebufferPointer, Framebuffer);
    if (GUIStatus != GUI_DRAW_SUCCESS)
        serial_puts("Erreur!\n");
    /* [[ end ]] */

    // We're done, just hang...
    Hang();
}