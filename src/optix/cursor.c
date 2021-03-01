#include "cursor.h"

struct optix_cursor_t optix_cursor = {
    .widget = {
        .transform = {
            .x = LCD_WIDTH / 2 -  OPTIX_CURSOR_WIDTH / 2,
            .y = LCD_HEIGHT / 2 - OPTIX_CURSOR_HEIGHT / 2,
            .width =  OPTIX_CURSOR_WIDTH,
            .height = OPTIX_CURSOR_HEIGHT,
        },
        .state = {.visible = true},
        .update = optix_UpdateCursor_default,
        .render = optix_RenderCursor_default,
    },
};

//initialize
void optix_InitializeCursor(void) {
    struct optix_widget *widget = &optix_cursor.widget;
    widget->transform.x = LCD_WIDTH / 2;
    widget->transform.y = LCD_HEIGHT / 2;
    widget->transform.width =  OPTIX_CURSOR_WIDTH;
    widget->transform.height = OPTIX_CURSOR_HEIGHT;
    widget->update = optix_UpdateCursor_default;
    widget->render = optix_RenderCursor_default;
    widget->state.visible = true;
}

//this will also handle the box-based mode
void optix_UpdateCursor_default(void) {
    struct optix_transform *transform = &optix_cursor.widget.transform;
    //start by updating the last x and y position of the cursor
    optix_cursor.last_x = transform->x;
    optix_cursor.last_y = transform->y;
    optix_cursor.state = OPTIX_CURSOR_NORMAL;
    if (optix_cursor.widget.state.visible) {
        if (kb_Data[7] & kb_Up)    transform->y -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Down)  transform->y += OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Left)  transform->x -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Right) transform->x += OPTIX_CURSOR_SPEED;
        if (transform->x > LCD_WIDTH - transform->width) transform->x = LCD_WIDTH - transform->width;
        if (transform->y > LCD_HEIGHT - transform->height) transform->y = LCD_HEIGHT - transform->height;
        if (transform->x < 0) transform->x = 0;
        if (transform->y < 0) transform->y = 0;
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
    gfx_sprite_t *spr[] = {
        cursor_normal,
        cursor_pointer,
        cursor_move,
        cursor_resize_horizontal,
        cursor_resize_vertical,
    };
    gfx_SetTransparentColor(255);
    gfx_TransparentSprite_NoClip(spr[optix_cursor.state], optix_cursor.widget.transform.x, optix_cursor.widget.transform.y);
}