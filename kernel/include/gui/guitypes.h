#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_CHILDREN 128
#define MAX_NAME_LEN 256
#define TEXT_POOL_SIZE 1048576  // 1 MB for all text

extern char TextPool[TEXT_POOL_SIZE];
extern uint32_t TextPoolIndex;

typedef struct {
    uint16_t x;
    uint16_t y;
} t2D;

typedef struct {
    void (*Main)(void* obj, void* context);
    void* context;
} Callback;

typedef struct {
    uint32_t HexCode;
    bool IsAlpha;
} RGBA;

typedef struct {
    t2D Position;
    t2D Size;
    bool Visible;
    Callback ObjectAddEvent;
    Callback Draw;
} GuiObjectParameters;

typedef struct {
    GuiObjectParameters Parameters;
    RGBA Color;
    int8_t Roundness;
} Rect;

typedef struct {
    GuiObjectParameters Parameters;
    uint32_t TextOffset;
    uint16_t TextLength;
    RGBA TextColor;
} Label;

typedef struct {
    GuiObjectParameters Parameters;
    Label Text;
    RGBA BackgroundColor;
} Button;

typedef enum {
    GUI_RECT,
    GUI_LABEL,
    GUI_BUTTON
} GuiType;

typedef struct {
    GuiType Type;
    void* Object;
} GuiChild;

typedef struct {
    GuiChild Children[MAX_CHILDREN];
    uint32_t Count;
} GuiTable;

bool AddChild(GuiTable* table, GuiType type, void* obj);

uint32_t AddTextToPool(const char* text, uint16_t length);

typedef struct {
    GuiObjectParameters Parameters;
    char Name[MAX_NAME_LEN];
    GuiTable Content;
} Panel;

typedef struct {
    char Name[MAX_NAME_LEN];
    char TitleContent[MAX_NAME_LEN];
    GuiObjectParameters Parameters;
    bool Fullscreen;
    bool Minimized;
    Panel Content;
    bool HideWindowTitle;
    Panel WindowTitle;
    bool Draggable;
} Window;

typedef enum {
    WIDGET_RECT,
    WIDGET_BUTTON,
    WIDGET_LABEL,
    WIDGET_PANEL
} WidgetType;

typedef struct {
    WidgetType type;
    void* data;
} Widget;