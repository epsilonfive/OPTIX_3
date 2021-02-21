#ifndef CURSOR_H
#define CURSOR_H

//includes
#include "globals.h"
#include "gfx/gfx.h"


//defines
#define OPTIX_CURSOR_SPEED     2
//states
#define OPTIX_CURSOR_NORMAL    0
#define OPTIX_CURSOR_OVER_ITEM 1
#define OPTIX_CURSOR_MOVE      2
//cursor directions
#define OPTIX_CURSOR_UP        0
#define OPTIX_CURSOR_DOWN      1
#define OPTIX_CURSOR_LEFT      2
#define OPTIX_CURSOR_RIGHT     3
#define OPTIX_CURSOR_NO_DIR    4
//cursor information
#define OPTIX_CURSOR_WIDTH     16
#define OPTIX_CURSOR_HEIGHT    16

//structs
struct optix_cursor_t {
    struct optix_widget widget;
    int last_x;
    int last_y;
    int state;
};
extern struct optix_cursor_t optix_cursor;

//functions
void optix_InitializeCursor(void);
void optix_UpdateCursor_default(void);
void optix_RenderCursor_default(void);

#endif