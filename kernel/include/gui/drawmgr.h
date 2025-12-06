#include <gui/guitypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <gui/guidef.h>

#pragma once

GUI_STATUS Draw(Widget* Target, volatile uint32_t* FramebufferPointer, struct limine_framebuffer* Framebuffer);