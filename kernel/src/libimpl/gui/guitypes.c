#include <stdint.h>
#include <stdbool.h>
#include <gui/guitypes.h>

bool AddChild(GuiTable* table, GuiType type, void* obj) {
    if (table->Count >= MAX_CHILDREN) return false;

    table->Children[table->Count].Type = type;
    table->Children[table->Count].Object = obj;

    switch (type) {
        case GUI_RECT: {
            Rect* r = (Rect*)obj;
            if (r && r->Parameters.ObjectAddEvent.Main)
                r->Parameters.ObjectAddEvent.Main(r, r->Parameters.ObjectAddEvent.context);
            break;
        }

        case GUI_LABEL: {
            Label* l = (Label*)obj;
            if (l && l->Parameters.ObjectAddEvent.Main)
                l->Parameters.ObjectAddEvent.Main(l, l->Parameters.ObjectAddEvent.context);
            break;
        }

        case GUI_BUTTON: {
            Button* b = (Button*)obj;
            if (b && b->Parameters.ObjectAddEvent.Main)
                b->Parameters.ObjectAddEvent.Main(b, b->Parameters.ObjectAddEvent.context);
            break;
        }
    }

    table->Count++;
    return true;
}

uint32_t AddTextToPool(const char* text, uint16_t length) {
    if (TextPoolIndex + length >= TEXT_POOL_SIZE) return 0xFFFFFFFF;
    uint32_t offset = TextPoolIndex;
    for (uint16_t i = 0; i < length; i++) {
        TextPool[TextPoolIndex++] = text[i];
    }
    return offset;
}