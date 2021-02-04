#ifndef CURSOR_H
#define CURSOR_H

//includes
#include "globals.h"

//defines
#define OPTIX_CURSOR_SPEED     2
//states
#define OPTIX_CURSOR_NORMAL    0
#define OPTIX_CURSOR_OVER_ITEM 1
//cursor directions
#define OPTIX_CURSOR_UP        0
#define OPTIX_CURSOR_DOWN      1
#define OPTIX_CURSOR_LEFT      2
#define OPTIX_CURSOR_RIGHT     3
#define OPTIX_CURSOR_NO_DIR    4
//cursor information
#define OPTIX_CURSOR_WIDTH    10
#define OPTIX_CURSOR_HEIGHT   10

//structs
typedef struct {
    int x;
    int y;
    //whether it's over a button, text field, etc.
    int state;
    //whether the cursor is active or not
    bool active;
    void (*update)(void);
    void (*render)(void);
} optix_cursor_t;
extern optix_cursor_t optix_cursor;

//functions
void optix_InitializeCursor(void);
void optix_UpdateCursor_default(void);
void optix_RenderCursor_default(void);

#endif