#ifndef CURSOR_H
#define CURSOR_H

//includes
#include "globals.h"
#include "gfx/gfx.h"
#include "elements/window.h"
#include "elements/sprite.h"

//defines
#define OPTIX_CURSOR_SPEED         3
#define OPTIX_BOX_SENSITIVITY      1
//states
#define OPTIX_CURSOR_NORMAL        0
#define OPTIX_CURSOR_OVER_ITEM     1
#define OPTIX_CURSOR_MOVE          2
#define OPTIX_CURSOR_RESIZE_HORIZ  3
#define OPTIX_CURSOR_RESIZE_VERT   4
#define OPTIX_CURSOR_TEXT          5
#define OPTIX_CURSOR_TEXT_UPPER    6
#define OPTIX_CURSOR_TEXT_LOWER    7
#define OPTIX_CURSOR_TEXT_MATH     8
//cursor directions
#define OPTIX_CURSOR_UP            0
#define OPTIX_CURSOR_DOWN          1
#define OPTIX_CURSOR_LEFT          2
#define OPTIX_CURSOR_RIGHT         3
#define OPTIX_CURSOR_NO_DIR        4
//cursor information
//the normal width of the cursor (just that top left corner)
#define OPTIX_CURSOR_WIDTH         1
#define OPTIX_CURSOR_HEIGHT        1
//hitbox for resizing
#define OPTIX_CURSOR_RESIZE_WIDTH  16
#define OPTIX_CURSOR_RESIZE_HEIGHT 16
//the size of the sprite
#define OPTIX_CURSOR_SPRITE_WIDTH  16
#define OPTIX_CURSOR_SPRITE_HEIGHT 16

//structs
struct optix_cursor_t {
    struct optix_widget widget;
    int last_x;
    int last_y;
    int state;
    //to be used when in box-based mode
    int direction;
    struct optix_widget *current_selection;
    //for partial redraw
    gfx_sprite_t *back;
};
extern struct optix_cursor_t optix_cursor;

//functions
void optix_InitializeCursor(void);
void optix_UpdateCursor_default(void);
void optix_RenderCursor_default(void);
void optix_RefreshCursorBackground(void);
void optix_RenderCursorBackground(void);
struct optix_widget *optix_FindNearestElement(uint8_t direction, struct optix_widget *reference, struct optix_widget *stack[]);

#endif