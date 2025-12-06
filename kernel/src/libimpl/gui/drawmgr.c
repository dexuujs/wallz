#include <gui/guitypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <gui/guidef.h>
#include <misc/serial.h>
#include <gui/drawhelper.h>

GUI_STATUS DrawRect(Widget* Target, volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer) {
    if (Target->type != WIDGET_RECT)
        return GUI_BAD_WIDGET_TYPE;
    if (!FramebufferPointer)
        return GUI_INVALID_FBPOINTER;
    if (!Framebuffer)
        return GUI_INVALID_FRAMEBUFFER;
    
    Rect *Wedget = (Rect*)Target->data;

    GuiObjectParameters WidgetParameters = Wedget->Parameters;

    t2D WidgetPosition = WidgetParameters.Position;
    t2D WidgetSize = WidgetParameters.Size;
    bool Visible = WidgetParameters.Visible;

    if (!Visible)
        return GUI_DRAW_SUCCESS;

    RGBA WidgetColor = Wedget->Color;
    int8_t Roundness = Wedget->Roundness;

    // Draw
    if (Roundness == 0)
        draw_rectangle(FramebufferPointer, Framebuffer, WidgetPosition.x, WidgetPosition.y, WidgetSize.x, WidgetSize.y, WidgetColor.HexCode);
    else
        draw_rectangle_rounded(FramebufferPointer, Framebuffer, WidgetPosition.x, WidgetPosition.y, WidgetSize.x, WidgetSize.y, Roundness, WidgetColor.HexCode);
    return GUI_DRAW_SUCCESS;
}

GUI_STATUS Draw(Widget* Target, volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer) {
    if (!FramebufferPointer)
        return GUI_INVALID_FBPOINTER;
    if (!Framebuffer)
        return GUI_INVALID_FRAMEBUFFER;
    if (!Target)
        return GUI_WIDGET_IS_NULL;

    switch (Target->type) {
        case WIDGET_RECT:
            serial_puts("Widget is type of Rect..\n");
            DrawRect(Target, FramebufferPointer, Framebuffer);
            break;
        case WIDGET_BUTTON:
            serial_puts("Widget is type of Button..\n");
            break;
        case WIDGET_PANEL:
            serial_puts("Widget is type of Panel..\n");
            break;
        case WIDGET_LABEL:
            serial_puts("Widget is type of Label..\n");
            break;
        default:
            return GUI_BAD_WIDGET_TYPE;
    }

    return GUI_DRAW_SUCCESS;
}