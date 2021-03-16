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
        //kb_Scan will be called elsewhere
        if (kb_Data[7] & kb_Up)         optix_cursor.direction = OPTIX_CURSOR_UP;
        else if (kb_Data[7] & kb_Down)  optix_cursor.direction = OPTIX_CURSOR_DOWN;
        else if (kb_Data[7] & kb_Left)  optix_cursor.direction = OPTIX_CURSOR_LEFT;
        else if (kb_Data[7] & kb_Right) optix_cursor.direction = OPTIX_CURSOR_RIGHT;
        else optix_cursor.direction = OPTIX_CURSOR_NO_DIR;
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

//returns a pointer to the closest element found within the array
//use the ternary operator excessively
struct optix_widget *optix_FindNearestElement(uint8_t direction, struct optix_widget *reference, struct optix_widget *stack[]) {
    int i = 0;
    int closest_score;
    struct optix_widget *closest;
    //this is going to have to be recursive
    //return the element that is the closest
    //ideally, you'd use this by passing in the children of a window but it would also work in other cases as well
    while (stack[i]) {
        int score = 0;
        //this will make sure that every element is tested, which we may (?) not want
        struct optix_widget *current = (stack[i]->child) ? optix_FindNearestElement(direction, reference, stack[i]->child) : stack[i];
        switch (direction) {
            case OPTIX_CURSOR_LEFT:
                score = (current->transform.x < reference->transform.x) * abs(current->transform.x - reference->transform.x);
                break;
            case OPTIX_CURSOR_RIGHT:
                score = (current->transform.x > reference->transform.x) * abs(current->transform.x - reference->transform.x);
                break;
            case OPTIX_CURSOR_UP:
                score = (current->transform.y < reference->transform.y) * abs(current->transform.y - reference->transform.y);
                break;
            case OPTIX_CURSOR_DOWN:
                score = (current->transform.y > reference->transform.y) * abs(current->transform.y - reference->transform.y);
                break;
        }
        //I finally get to use this
        closest_score = (score < closest_score && score) ? score : closest_score;
        closest = current;
    }
    return closest;
}