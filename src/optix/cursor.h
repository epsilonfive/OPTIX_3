#ifndef CURSOR_H
#define CURSOR_H

//includes
#include <stdint.h>
#include <graphx.h>
#include "gui_control.h"

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
#define OPTIX_CURSOR_FORCE_UPDATE  5
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

//speed defines
//acceleration increase, per second
#define OPTIX_CURSOR_ACCELERATION  (float) 5
//pixels per second again
#define OPTIX_CURSOR_INITIAL_SPEED 0
//same here
#define OPTIX_CURSOR_MAX_SPEED     10


//structs
struct optix_cursor {
    struct optix_widget widget;
    int last_x;
    int last_y;
    //its real coordinates
    float current_speed;
    float true_x;
    float true_y;
    int state;
    //to be used when in box-based mode
    int direction;
    struct optix_widget *current_selection;
    //for partial redraw
    gfx_sprite_t *back;
    //if this is set to true, the GUI will not be updated or rendered for that loop (mostly for the cursor to use
    //when moving windows and such)
    bool gui_frozen;
};

//functions
void optix_InitializeCursor(struct optix_cursor *cursor);
void optix_UpdateCursor_default(struct optix_widget *widget);
void optix_RenderCursor_default(struct optix_widget *widget);
void optix_RefreshCursorBackground(struct optix_widget *widget);
void optix_RenderCursorBackground(struct optix_widget *widget);
struct optix_widget *optix_FindNearestElement(uint8_t direction, struct optix_widget *reference, struct optix_widget *stack[]);
void optix_HandleNearestElement(void);
struct optix_widget *optix_GetCurrentSelection(struct optix_widget *stack[]);
void optix_SetCurrentSelection(struct optix_widget *widget);

#endif