#include "cursor.h"

optix_cursor_t optix_cursor;

//initialize
void optix_InitializeCursor(void) {
    optix_cursor.x = LCD_WIDTH / 2;
    optix_cursor.y = LCD_HEIGHT / 2;
    optix_cursor.update = optix_UpdateCursor_default;
    optix_cursor.render = optix_RenderCursor_default;
    optix_cursor.active = true;
}

//this will also handle the box-based mode
void optix_UpdateCursor_default(void) {
    if (optix_cursor.active) {
        if (kb_Data[7] & kb_Up)    optix_cursor.y -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Down)  optix_cursor.y += OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Left)  optix_cursor.x -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Right) optix_cursor.x += OPTIX_CURSOR_SPEED;
    } else {
        int cursor_direction;
        //kb_Scan will be called elsewhere
        if (kb_Data[7] & kb_Up)         cursor_direction = OPTIX_CURSOR_UP;
        else if (kb_Data[7] & kb_Down)  cursor_direction = OPTIX_CURSOR_DOWN;
        else if (kb_Data[7] & kb_Left)  cursor_direction = OPTIX_CURSOR_LEFT;
        else if (kb_Data[7] & kb_Right) cursor_direction = OPTIX_CURSOR_RIGHT;
        else cursor_direction = OPTIX_CURSOR_NO_DIR;
        //the box-based GUI mode (implement later)
    }
}  

void optix_RenderCursor_default(void) {
    //just make it a circle for now
    gfx_SetColor(224);
    gfx_Circle(optix_cursor.x, optix_cursor.y, 5);
}