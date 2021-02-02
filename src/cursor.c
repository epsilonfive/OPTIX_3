#include "cursor.h"

optix_cursor_t optix_cursor;

//this will also handle the box-based mode
void optix_UpdateCursor_default(void) {
    int cursor_direction;
    //kb_Scan will be called elsewhere
    if (kb_Data[7] & kb_Up)         cursor_direction = OPTIX_CURSOR_UP;
    else if (kb_Data[7] & kb_Down)  cursor_direction = OPTIX_CURSOR_DOWN;
    else if (kb_Data[7] & kb_Left)  cursor_direction = OPTIX_CURSOR_LEFT;
    else if (kb_Data[7] & kb_Right) cursor_direction = OPTIX_CURSOR_RIGHT;
    else cursor_direction = OPTIX_CURSOR_NO_DIR;
    if (optix_cursor.active) {
        switch (cursor_direction) {
            case OPTIX_CURSOR_UP:
                optix_cursor.y--;
                break;
            case OPTIX_CURSOR_DOWN:
                optix_cursor.y++;
                break;
            case OPTIX_CURSOR_LEFT:
                optix_cursor.x--;
                break;
            case OPTIX_CURSOR_RIGHT:
                optix_cursor.x++;
                break;
        }
    } else {
        //the box-based GUI mode (implement later)
    }
    int i;
    //this is pretty easy, just loop through everything and check if we're currently over it, and if so, set its state to selected
    for (i = 0; i < optix_gui_control.num_buttons; i++) {
        optix_button_t *temp = &optix_button[i];
        if (!temp->state.visible) continue;
        //we don't want to have it be selected if the cursor x is less than temp->x, or if cursor x is to the left of the item
        if (gfx_CheckRectangleHotspot(temp->transform.x, temp->transform.y, temp->transform.width, temp->transform.height, 
            optix_cursor.x, optix_cursor.y, OPTIX_CURSOR_WIDTH, OPTIX_CURSOR_HEIGHT))
            temp->state.selected = true;
        else temp->state.selected = false;
    }
}  

void optix_RenderCursor_default(void) {
    //just make it a circle for now
    gfx_SetColor(224);
    gfx_Circle(optix_cursor.x, optix_cursor.y, 5);
}